#pragma once
#include "GameObject.h"

class [[MetaAttr::Reflect(), MetaAttr::BindToLua()]] GameObjFactory
{
public:
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	static GameObject *CreateObject(GameObject* parent, StaticString<16>  name);
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	static void DestroyObject(GameObject *obj);
private:
};
