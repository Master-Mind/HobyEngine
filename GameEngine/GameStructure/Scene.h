#pragma once
#include "Asset.h"

class Scene : public Asset
{
public:
	void FullLoadNow();
	friend class SceneManager;
	const AssetName &GetName() { return name; }
	GameObject *getRootObj() { return _myRoot; }
private:
	GameObject * _myRoot;
};
