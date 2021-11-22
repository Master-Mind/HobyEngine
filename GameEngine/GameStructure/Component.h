#pragma once
class GameObject;
class Component
{
public:
	virtual void Load();
	virtual void Initialize();
	virtual void Destroy();
	virtual void Unload();
	GameObject *Owner;
};

inline void Component::Load()
{
}

inline void Component::Unload()
{
}

inline void Component::Initialize()
{
}

inline void Component::Destroy()
{
}
