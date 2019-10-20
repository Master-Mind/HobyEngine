#pragma once
#include "Component.h"
#include "Transform.h"
#include "Graphics.h"
#include "AssetDataBase.h"
#include "GraphicsAssets/Sprite.h"

class SpriteRenderer :public  Component
{
public:
	void Load() override;
	void Initialize() override;
	void Update(GraphicsUpdate *update);
	void SetSprite(AssetName name);
	void Destroy() override;
	AssetReference<Sprite> _mySprite;
private:
	Transform *_trans;
};
