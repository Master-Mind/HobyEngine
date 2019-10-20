#pragma once
#include "Asset.h"
#include "Scene.h"

class SceneManager
{
public:
	static void LoadScene(AssetReference<Scene> newScene);
	static void ReLoadScene();
	static void DestroyAll();
	static AssetReference<Scene> GetActiveScene();
private:
	static AssetReference<Scene> _curScene;
};
