#include "AssetDataBase.h"
#include "Debugger.h"
#include "GraphicsAssets/Sprite.h"
std::unordered_map<std::string, Asset *>  AssetDataBase::_assets;
std::hash<const char *> AssetDataBase::hasher;
Asset* AssetDataBase::GetAsset(AssetName name)
{
	if constexpr(Debugger::IsInDebug)
	{
		DEBUGASSERT(_assets.count(name.toString()) > 0, DTag::AssetDatabase);
		//return nullptr;
	}
	return _assets[name.toString()];
}

template <>
Asset* AssetDataBase::LoadAsset<Sprite>(AssetName name)
{
	Sprite *spit = new Sprite();
	boost::filesystem::path assPath = "Assets/Sprites/" + name.toString();
	spit->Load(assPath);
	_assets[name.toString()] = spit;
	_assets[name.toString()]->name = name;
	_assets[name.toString()]->loadProgress = 1;

	return _assets[name.toString()];
}

template <>
Asset* AssetDataBase::LoadAsset<Shader>(AssetName name)
{
	Shader *spit = new Shader();
	boost::filesystem::path vertPath = "Assets/Shaders/" + name.toString() + ".vert";
	boost::filesystem::path fragPath = "Assets/Shaders/" + name.toString() + ".frag";
	spit->Load(vertPath, fragPath);
	_assets[name.toString()] = spit;
	_assets[name.toString()]->name = name;
	_assets[name.toString()]->loadProgress = 1;

	return _assets[name.toString()];
}
template <>
Asset* AssetDataBase::LoadAsset<Texture>(AssetName name)
{
	Texture *tex = new Texture();
	boost::filesystem::path texPath = "Assets/Textures/" + name.toString();
	tex->Load(texPath);
	_assets[name.toString()] = tex;
	_assets[name.toString()]->name = name;
	_assets[name.toString()]->loadProgress = 1;

	return _assets[name.toString()];
}
DEFINE_META(GenericAssetRef)
ADD_MEMBER(name)
_toStr = [](GenericAssetRef&ass) {return ass.name.toString(); };
_fromStr = [](const char*str)
{
	GenericAssetRef ret;
	ret.name = str;
	return ret;
};
END_META(GenericAssetRef)
DEFINE_META(AssetName)
END_META(AssetName)
void Member::SetToGenAss()
{
	SetMeta(typeid(GenericAssetRef).hash_code());
}