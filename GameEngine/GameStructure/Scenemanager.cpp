#include "Scenemanager.h"
#include "../Memory/GameObjFactory.h"
#include "AssetDataBase.h"
AssetReference<Scene> SceneManager::_curScene("");
void SceneManager::LoadScene(AssetReference<Scene> newScene)
{
	DestroyAll();
	_curScene = newScene;
	_curScene->FullLoadNow();

}

void SceneManager::ReLoadScene()
{
	LoadScene(_curScene); //placeholder, replace with destroy/init pair
}

void SceneManager::DestroyAll()
{
	if (_curScene && _curScene->_myRoot)
	{
		_curScene->_myRoot->Destroy();
		_curScene->_myRoot->Unload();
		GameObjFactory::DestroyObject(_curScene->_myRoot);
	}
}

AssetReference<Scene> SceneManager::GetActiveScene()
{
	return _curScene;
}
