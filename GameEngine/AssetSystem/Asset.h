#pragma once
//#include "AssetDataBase.h"
#include <typeinfo>
#include "StaticString.h"
#include "MetaDB.h"

typedef StaticString<32> AssetName;
class Asset
{
	bool operator==(AssetName &checkName)
	{
		return name == checkName;
	}
	bool operator!=(AssetName &checkName)
	{
		return name != checkName;
	}
public:
	AssetName name;
	float loadProgress;
};
template<typename T>
class AssetReference
{
public:
	AssetReference()
		: name("")
	{
	}
	AssetReference(const char *name) //doesn't load asset
		: name(name)
	{
	}
	AssetReference(Asset &asset)
		: name(asset.name)
	{
	}
	AssetReference(Asset *asset)
		: name(asset->name)
	{
	}
	static MetaReference GetAssetMeta()
	{
		return GetMetaData<T>();
	}

	operator bool() const
	{
		return !isNull();
	}

	bool isNull() const
	{
		return name == "" || !AssetDataBase::GetAsset(name);
	}

	friend class AssetDataBase;
	T &operator*();
	const T &operator*() const;
	T *operator->();
	const T *operator->() const;
private:
	AssetName name;
};

class GenericAssetRef
{
public:
	AssetName name;
};
template<typename T>
T *GetAsset(AssetName name);
template <typename T>
T& AssetReference<T>::operator*()
{
	return *static_cast<T*>(AssetDataBase::GetAsset(name));
}

template <typename T>
const T& AssetReference<T>::operator*() const
{
	return *static_cast<T*>(AssetDataBase::GetAsset(name));
}

template <typename T>
T* AssetReference<T>::operator->()
{
	return  GetAsset<T>(name);
}

template <typename T>
const T* AssetReference<T>::operator->() const
{
	return  GetAsset<T>(name);
}
