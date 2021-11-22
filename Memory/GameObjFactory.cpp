#include "GameObjFactory.h"
#include "TypeAllocator.h"
#include <string>
#include "Debugger.h"
#include"MetaDB.h"

GameObject* GameObjFactory::CreateObject(GameObject* parent, StaticString<16> name)
{
	GameObject *ret = TypeAllocator<GameObject>::Allocate();
	new (ret) GameObject;
	ret->SetName(name);
	if(parent)
		parent->AddChild(ret);
	ret->_parent = parent;
	return ret;
}

void GameObjFactory::DestroyObject(GameObject* obj)
{
	TypeAllocator<GameObject>::DeAllocate(obj);
}
