#pragma once
#include <boost/tokenizer.hpp>
#include <map>
#include <unordered_map>
#include "MetaAttribute.h"
#include "MetaInterface.h"
class AllEnums
{
public:
	static void AddEnum(const std::string& name,void *e);
	static void *GetEnum(const std::string &name);
private:
	static std::unordered_map<std::string, void *> *_enums;
};



//TODO: this won't work. need to have some other type for the typename, also fix map insertion
template<typename T>
class MetaEnum : MetaInterface
{
public:
	MetaEnum(const AttribList attribs, const char *name, std::string names, std::vector<T> &&values)
	{
		_attribs = attribs;
		_name = name;
		boost::tokenizer<> tok(names);
		//tok.assign(names.begin(), names.end());
		int val = 0;
		for(auto n = tok.begin(); n != tok.end(); ++n)
		{
			++n;
			if((*n)[0] == '<' || (*n)[0] == '>')
			{
				++n;
				continue;
			}
			_values.insert({ n->c_str(), values[val] });
			val++;
		}

		if(_attribs.has<MetaAttr::BindToLua>())
		{
			LuaHandler::BindEnum(*this);
		}

		AllEnums::AddEnum(name, new MetaEnum(*this));
	}
	const char* GetName() const override;
	const Member* GetMember(int memNum) const override;
	size_t GetMemberCount() const override;
	T operator[](const char *i)
	{
		return _values[i];
	}


	typename std::map<std::string, T>::iterator begin()
	{
		return _values.begin();
	}
	typename std::map<std::string, T>::iterator end()
	{
		return _values.end();
	}

	size_t GetID() const override
	{
		return typeid(T).hash_code();
	}

	bool HasAttribute(size_t type_id) const override
	{
		return _attribs.has(type_id);
	}

	const std::vector<MetaReference>& GetSubClasses() const override;

	MetaReference GetParentClass() const override;

private:
	void AddSubClass(MetaReference ref) override;

public:
	class EnumIterator
	{
	public:

	private:
		typename std::map<const char *, T>::iterator me;
	};


	friend class LuaHandler;
private:	
	const char *_name;
	std::map<std::string, T> _values; 
	AttribList _attribs;
};
template <typename T>
const char* MetaEnum<T>::GetName() const
{
	return _name;
}

template <typename T>
const Member* MetaEnum<T>::GetMember(int memNum) const
{
	return nullptr;
}

template <typename T>
size_t MetaEnum<T>::GetMemberCount() const
{
	return 0;
}

template <typename T>
const std::vector<MetaReference>& MetaEnum<T>::GetSubClasses() const
{
	return {};
}

template <typename T>
MetaReference MetaEnum<T>::GetParentClass() const
{
	return nullptr;
}

template <typename T>
void MetaEnum<T>::AddSubClass(MetaReference ref)
{
}
