#pragma once
#include "FrameRateController.h"
#include "../Graphics/Graphics.h"
#include "../Memory/GameObjFactory.h"
#include "../Memory/ComponentFactory.h"
#include "UpdateEvent.h"
#include "EventSystem.h"
#include "../Editor/Editor.h"
#include "MetaDB.h"

class CoreEngine
{
public:
	void Init();
	void Start();
	static void PostQuit();
	static CoreEngine *CORE;
	GameObjFactory GameObjectFactory;
	ComponentFactory ComponentFac;
	friend class UpdateEvent;
	friend class GraphicsUpdate;
	friend class Editor;
	EventSystem event_system;
	Editor editor;
private:
	UpdateEvent _theOneTrueUpdate;
	FrameRateController frc;
	LuaHandler handler;
	bool quit;
	Graphics _graphicsSystem;
};
