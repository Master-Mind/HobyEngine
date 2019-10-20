#pragma once
#include "StaticString.h"
#include <unordered_map>
#include "Component.h"
#include <typeinfo>
typedef StaticString<16> NameType;
typedef std::unordered_map<const char *, GameObject*> GameObjMap;
typedef std::unordered_map<size_t, Component*> CompMap;

class [[MetaAttr::Reflect(), MetaAttr::BindToLua()]] GameObject
{
public:
	GameObject()
		: _parent(nullptr),
		  _name(),
		  _components(),
		  _children()
	{
	}

	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	void SetName(NameType newName);
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	NameType GetName();
	const GameObjMap &GetChildren();
	void AddChild(GameObject *obj);
	void OnLoad();
	void Initialize();
	void Destroy();
	void Unload();
	template<typename T>
	void AddComponent(T *comp);
	void AddComponentGeneric(Component *comp, size_t id);
	template<typename T>
	T *GetComponent();
	const CompMap &GetComponents();
	friend class GameSerializer;
	friend class GameObjFactory;
private:
	GameObject * _parent;
	NameType _name;
	CompMap _components;
	GameObjMap _children;
};

template <typename T>
void GameObject::AddComponent(T* comp)
{
	_components[typeid(T).hash_code()] = comp;
}

template <typename T>
T* GameObject::GetComponent()
{
	return static_cast<T*>(_components[typeid(T).hash_code()]);
}
