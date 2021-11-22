#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Core.h"
#include "AssetDataBase.h"


void SpriteRenderer::Load()
{
	AssetDataBase::RequestAssetIntoRef(_mySprite);
}

void SpriteRenderer::Initialize()
{
	_trans = Owner->GetComponent<Transform>();
	CoreEngine::CORE->event_system.Subscribe<GraphicsUpdate>(new Delegate<void(Event*)>(this, &SpriteRenderer::Update));
}

void SpriteRenderer::Update(GraphicsUpdate *update)
{
	Graphics::ENGINE->Render(*_mySprite, _trans);
}

void SpriteRenderer::SetSprite(AssetName name)
{
	_mySprite = AssetDataBase::RequestAsset<Sprite>(name);
}

void SpriteRenderer::Destroy()
{
	CoreEngine::CORE->event_system.UnSubscribe<GraphicsUpdate>(this);
}