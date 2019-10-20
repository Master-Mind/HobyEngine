#pragma once
#include <unordered_map>
#include "Asset.h"
#include <boost/filesystem.hpp>
#include "Debugger.h"
class Asset;
typedef StaticString<32> AssetName;
class AssetDataBase
{
public:
	static Asset *GetAsset(AssetName name);
	template<typename T>
	friend T *GetAsset(AssetName name);
	template<typename T>
	static AssetReference<T> RequestAsset(AssetName name);
	template<typename T>
	static int ToIndex(int nameHash)
	{
		return typeid(T).hash_code() + nameHash;
	}
	template<typename T>
	static void RequestAssetIntoRef(AssetReference<T> &ref)
	{
		ref = RequestAsset<T>(ref.name);
	}
	template<typename T>
	static AssetName ToIndex(AssetName name)
	{
		return name;
	}
	//static int HashName(AssetName name)
	//{
	//	return hasher(name);
	//}
private:
	static std::hash<const char *> hasher;
	static std::unordered_map<std::string, Asset *> _assets;
	template<typename T>
	static Asset *LoadAsset(AssetName name);
};

template <typename T>
T* GetAsset(AssetName name)
{
	if constexpr(Debugger::IsInDebug)
	{
		DEBUGASSERT(AssetDataBase::_assets.count(name.toString()) > 0, DTag::AssetDatabase);
	}
	return static_cast<T*>(AssetDataBase::_assets[name.toString()]);
}

template <typename T>
AssetReference<T> AssetDataBase::RequestAsset(StaticString<32> name)
{
	if(_assets.count(name.toString()))
	{
		return _assets[name.toString()];
	}

	return LoadAsset<T>(name);
}
