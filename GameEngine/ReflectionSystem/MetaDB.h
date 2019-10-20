#pragma once
#include <unordered_map>
#include <typeinfo>
#include "MetaReference.h"
#include "MetaProperties.h"
#include "MetaAttribute.h"
class MetaInterface;
#include "..\Memory\ComponentFactory.h"
#include "..\Memory\TypeAllocator.h"
class MetaDB
{
public:
	MetaDB();
	void Init();
	template<typename T,  typename ...Members>
	void DefineMeta(const char *name, T *wew, const char *memberNames, Members ...members)
	{
	}
	friend class MetaReference;
	template<typename T>
	friend MetaReference GetMetaData();
	static const char *getMetaName(size_t id);
	static MetaReference getMeta(std::string type);
	static const MetaReference getMeta(size_t id);
	template<typename T>
	static const MetaReference getMeta()
	{
		return getMap()[typeid(T).hash_code()];
	}
	static const size_t getTheFuckingOffsetForFuckingLuaBecauseFuckingHeaderFiles(const size_t id, const int member);
	template<typename T>
	static void AddMetaData(MetaInterface* newDat);

	static MetaDB *METADB;
private:
	void MetaFixup();
	void FixupInheritance();
	static std::unordered_map<size_t, MetaInterface*> &getMap()
	{
		static std::unordered_map<size_t, MetaInterface*> _metaMap;
		return _metaMap;
	}

	static std::unordered_map<std::string, MetaInterface*> &getNameMap()
	{
		static std::unordered_map<std::string, MetaInterface*> _metaMap;
		return _metaMap;
	}
};
#define TYPE_ID(TYPE) \
	typeid(TYPE).hash_code()

#define DEFINE_META(TYPE) \
MetaData<TYPE>::MetaData(const char* typeName) : _typeName(typeName)\
{\
	TYPE *temp = nullptr;\
	_typeName = typeName; \
	_isBase = false; \
	if constexpr(std::is_base_of<Component, TYPE>::value)\
	{\
		ComponentFactory::GetAllocFuncs().insert({typeid(TYPE).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<TYPE>::Allocate)});\
	}\
	const size_t countStart = __COUNTER__;

#define ADD_PROPERTIES(...) \
	_properties.AddAttributes(__VA_ARGS__)

#define ADD_LUA_CONSTRUCTOR(...)

#define ADD_MEMBER(MEMBER, ...) \
	AddMember<__COUNTER__ - countStart>(nullptr, #MEMBER, &temp->MEMBER, MetaAttr::PlaceHolder(), __VA_ARGS__);

#define ADD_ASSET(MEMBER, ...) \
	AddMember<__COUNTER__ - countStart>(nullptr, #MEMBER, &temp->MEMBER, MetaAttr::IsAsset(), __VA_ARGS__);

#define ADD_STATIC_FUNC(MEMBER, ...) \
	AddStaticFunc(nullptr, #MEMBER, T::MEMBER, MetaAttr::PlaceHolder(), __VA_ARGS__);

#define ADD_MEMBER_FUNC(MEMBER, ...) \
	AddDelegate(nullptr, #MEMBER, makeDelegate(&T::MEMBER), MetaAttr::PlaceHolder(), __VA_ARGS__);

#define END_META(TYPE) \
	if(_properties.has<MetaAttr::BindToLua>())\
	{\
	}\
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));\
}\
MetaData<TYPE>  Data##TYPE(#TYPE);
#define METAENUM(ATTRIBS, NAME, TYPE, ...) \
	MetaEnum<TYPE> Data##NAME##TYPE(AttribList(ATTRIBS), #NAME, #__VA_ARGS__, {__VA_ARGS__})


#include "LuaHandler.h"
#include "MetaData.h"
#include "MetaEnum.h"


template<typename T>
MetaReference GetMetaData()
{
	if (!MetaDB::getMap().count(typeid(T).hash_code()))
	{
		MetaDB::getMap()[(typeid(T).hash_code())] = nullptr;
	}
	return MetaDB::getMap()[(typeid(T).hash_code())];
}


template<typename T>
void MetaDB::AddMetaData(MetaInterface* newDat)
{
	getMap().insert({ typeid(T).hash_code(), newDat });
	getNameMap().insert({ newDat->GetName(), newDat });
}