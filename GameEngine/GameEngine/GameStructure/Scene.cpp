#include "Scene.h"
#include "FileSystem.h"
#include "AssetDataBase.h"
void Scene::FullLoadNow()
{
	_myRoot = FileSystem::LoadLevel(name);
	_myRoot->OnLoad();
	_myRoot->Initialize();
	
}
template <>
Asset* AssetDataBase::LoadAsset<Scene>(AssetName name)
{
	Scene *spit = new Scene();
	spit->name = name;
	spit->loadProgress = 0;
	_assets[name.toString()] = spit;

	return _assets[name.toString()];
}