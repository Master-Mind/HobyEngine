#include "GameObject.h"
#include "MetaDB.h"

void GameObject::SetName(NameType newName)
{
	_name = newName;
}

NameType GameObject::GetName()
{
	return _name;
}

const std::unordered_map<const char*, GameObject*>& GameObject::GetChildren()
{
	return _children;
}

void GameObject::AddChild(GameObject* obj)
{
	_children.insert({ obj->_name.cstr(), obj });
}

void GameObject::OnLoad()
{
	for (auto& comp : _components)
	{
		comp.second->Load();
	}
}

void GameObject::AddComponentGeneric(Component* comp, size_t id)
{
	_components.insert({ id, comp });
}

const std::unordered_map<size_t, Component*>& GameObject::GetComponents()
{
	return _components;
}

void GameObject::Initialize()
{
	for (auto comp : _components)
	{
		comp.second->Owner = this;
		comp.second->Initialize();
	}
	for (auto child : _children)
	{
		child.second->Initialize();
	}
}

void GameObject::Destroy()
{
	for (auto comp : _components)
	{
		comp.second->Owner = this;
		comp.second->Destroy();
	}
	for (auto child : _children)
	{
		child.second->Destroy();
	}
}

void GameObject::Unload()
{
	for (auto comp : _components)
	{
		comp.second->Owner = this;
		comp.second->Unload();
	}
	for (auto child : _children)
	{
		child.second->Unload();
	}
}
