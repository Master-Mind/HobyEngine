///
//This is part of a WIP c++ reflection system that I am doing as a side project
//There are still features to add, but I felt like it made a good vertical slice
//of my work. I have unit tests for just about everything seen here in another file
//Example usage:
//(in Foo.cpp)
//DEFINE_META(Foo)
//ADD_MEMBER(Bar, (int)MetaProp::FloatSlider)
//END_META(Foo)
#pragma once
#include <vector>
#include <functional>
#include "MetaInterface.h"
#include "Delegate.h"
#include "Fuckheaders.h"
//member of a class
class Member
{
public:
	friend class MetaInterface;
	size_t GetOffset() const
	{
		return _offset;
	}
	const char * GetName() const
	{
		return _name;
	}
	MetaReference GetMeta() const
	{
		return _myMeta;
	}

	void SetOffset(size_t off)
	{
		_offset = off;
	}
	void SetName(const char *name)
	{
		_name = name;
	}
	void SetMeta(MetaReference met)
	{
		_myMeta = met;
	}

	void SetProperties(AttribList &&props)
	{
		_properties = props;
	}

	void SetProperties() const
	{
	}

	template<typename T>
	bool HasProperty() const
	{
		return _properties.has<T>();
	}

	void SetToGenAss();
private:
	size_t _offset;
	const char *_name;
	MetaReference _myMeta;
	AttribList _properties;
};

class MemberDeligate
{
public:
	friend class MetaInterface;
	const char * GetName() const
	{
		return _name;
	}

	void SetProperties(AttribList &&props)
	{
		_properties = props;
	}

	void SetFunc(void *func)
	{
		_func = func;
	}

	void SetName(const char *name)
	{
		_name = name;
	}

	void SetProperties() const
	{
	}

	template<typename T>
	bool HasProperty() const
	{
		return _properties.has<T>();
	}

	void SetToGenAss();
private:
	void *_func;
	const char *_name;
	AttribList _properties;
};

class MemberStaticDeligate
{
public:
	friend class MetaInterface;
	const char * GetName() const
	{
		return _name;
	}

	void SetProperties(AttribList &&props)
	{
		_properties = props;
	}

	void SetFunc(void *func)
	{
		_func = func;
	}

	void SetName(const char *name)
	{
		_name = name;
	}

	void SetProperties() const
	{
	}

	template<typename T>
	bool HasProperty() const
	{
		return _properties.has<T>();
	}

	void SetToGenAss();
private:
	void *_func;
	const char *_name;
	AttribList _properties;
};

template<typename T, typename... Ts>
constexpr bool contains()
{
	return std::disjunction_v<std::is_same<T, Ts>...>;
}
//interface so that metadata can be stored and accessed from a single source without templates
//(i.e. a parser that needs to get a type by reference)
class LuaRegistry;
class VariantRange;
//meta data for T
template<typename T>
class MetaData : public MetaInterface
{
public:
	//Constructor gets defined uniquely for each type in macros, like for the example above
	//Constructor is then called by a static instance of MetaData<T>
	MetaData(const char* typeName);

	const char *GetName() const override
	{
		return _typeName;
	}

	const Member *GetMember(const int memIndex) const override
	{
		return &_members[memIndex];
	}
	

	size_t GetMemberCount() const
	{
		return _members.size();
	}

	void Construct(void *memblock) const override
	{
		new (memblock) T;
	}
	std::string ToString(void *memblock) const override
	{
		if(_toStr)
		{
			return _toStr(*reinterpret_cast<T*>(memblock));
		}

		return "";
	}
	void FromString(const char *str, void *memblock) const override
	{
		if(_fromStr)
		{
			*reinterpret_cast<T*>(memblock) = _fromStr(str);
		}
	}

	bool IsRange() const override
	{
		return _toRange != nullptr;
	}
	bool IsBase() const override
	{
		return _toStr != nullptr && _fromStr != nullptr;
	}

	size_t GetID() const override
	{
		return typeid(T).hash_code();
	}

	bool HasAttribute(size_t type_id) const override
	{
		return _properties.has(type_id);
	}

private:
	template<const size_t memberNum, typename S, typename ...Props>
	void AddMember(LuaRegistry* reg, const char* name, S member, Props ...properties);

	template<typename S, typename ...Props>
	void AddDelegate(LuaRegistry* reg, const char* name, Delegate<S>* member, Props ...properties)
	{
		MemberDeligate addboi;
		addboi.SetFunc((void*)member);
		addboi.SetName(name);
		addboi.SetProperties(AttribList(properties...));
		member->AddFunc<T>();
		if constexpr (contains<MetaAttr::BindToLua, Props...>())
		{
			//reg->push_back({ name, LuaHandler::LuaMemberFuncWrapper<T, S> });
		}

		_memberFuncs.push_back(addboi);
	}

	template<typename S, typename ...Props>
	void AddStaticFunc(LuaRegistry* reg, const char* name, S* member, Props ...properties)
	{
		MemberStaticDeligate addboi;
		StaticDelegate<S>::AssociateFunctionPointer(member);
		addboi.SetFunc((void*)member);
		addboi.SetName(name);
		addboi.SetProperties(AttribList(properties...));

		if constexpr (contains<MetaAttr::BindToLua, Props...>())
		{
			if constexpr (contains<MetaAttr::IsForDebug, Props...>())
			{
				//reg->push_back({ name, LuaHandler::LuaDebugFuncWrapper<S> });
			}
			else
			{
				//reg->push_back({ name, LuaHandler::LuaFuncWrapper<S> });
			}
		}

		_staticFuncs.push_back(addboi);
	}

	const std::vector<MetaReference>& GetSubClasses() const override
	{
		return _subClasses;
	}

	void AddSubClass(MetaReference ref) override
	{
		_subClasses.push_back(ref);
	}

	MetaReference GetParentClass() const override
	{
		return _parentClass;
	}

	const char *_typeName;
	std::vector<Member> _members;
	std::vector<MemberDeligate> _memberFuncs;
	std::vector<MemberStaticDeligate> _staticFuncs;
	std::function<std::string(T&)> _toStr;
	std::function<T(const char*)> _fromStr;
	std::function<void(VariantRange&,T&)> _toRange;
	bool _isBase;
	AttribList _properties;
	MetaReference _parentClass;
	std::vector<MetaReference> _subClasses;
	//TODO: Add a vector of MethodInfo
	std::function<T()> _defaultConstructor;
	std::function<void()> _defaultDestructor;
};

//include cpp because of how templates work
#include "MetaData.cpp"

template<typename T>
MetaReference GetMetaData();

template<typename T>
template<const size_t memberNum, typename S, typename ...Props>
inline void MetaData<T>::AddMember(LuaRegistry *reg, const char *name, S member, Props ...properties)
{
	Member addboi;
	//auto dat = GetMetaData<std::remove_pointer<S>::type>();
	addboi.SetMeta(typeid(std::remove_pointer<S>::type).hash_code());
	addboi.SetName(name);
	//bitwise or all flags together in a pack expansion
	 const bool fool = sizeof...(properties) > 0;
	 if constexpr(sizeof...(properties) > 0)
	 {
		 addboi.SetProperties(AttribList(properties...));

		 if constexpr(contains<MetaAttr::IsAsset, Props...>())
		 {
			 addboi.SetToGenAss();
		 }

		 if constexpr(contains<MetaAttr::BindToLua, Props...>())
		 {
			 //std::string strName = name;
			 //reg->push_back({ ("get" + strName).c_str(), LuaHandler::LuaGetter<T, typename std::remove_pointer<S>::type, memberNum> });
			 //reg->push_back({ ("set" + strName).c_str(), LuaHandler::LuaGetter<T, typename std::remove_pointer<S>::type, memberNum> });
		 }
	 }
	addboi.SetOffset(reinterpret_cast<size_t>(member));
	_members.push_back(addboi);
}

template <typename T>
MetaData<T>::MetaData(const char* typeName) : _typeName(typeName)
{
	//_memberCount = 0;
}