#pragma once
#include "Component.h"
#include "GameObject.h"
#include "TypeAllocator.h"

class ComponentFactory
{
public:
	template<typename T>
	static T* CreateComponent(GameObject* parent)
	{
		Component *comp = TypeAllocator<T>::Allocate();
		comp->Owner = parent;
		parent->AddComponent(reinterpret_cast<T*>(comp));
		return reinterpret_cast<T*>(comp);
	}
	static Component* CreateComponentID(GameObject* parent, size_t id)
	{
		Component *comp = reinterpret_cast<Component*>(GetAllocFuncs()[id]());
		comp->Owner = parent;
		parent->AddComponentGeneric(comp, id);
		return comp;
	}
	template<typename T>
	static void DestroyComponent(T* obj)
	{
		TypeAllocator<T>::DeAllocate(obj);
	}
	static std::unordered_map<size_t, void *(*)()> &GetAllocFuncs()
	{
		static std::unordered_map<size_t, void *(*)()> _allocFuncs;
		return _allocFuncs;
	}
private:
};
