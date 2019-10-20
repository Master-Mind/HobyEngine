#include "Core.h"
#include "Debugger.h"
#include <iostream>
#include "StaticString.h"
#include "../Memory/TypeAllocator.h"
#include "Transform.h"
#include "GameObject.h"
#include "GraphicsComps/SpriteRenderer.h"
#include "EventSystem.h"
#include "GameSerializer.h"
#include "FileSystem.h"
#include "Scenemanager.h"
#include "LuaHandler.h"
#include <imgui.h>
#include "TestManager.h"

CoreEngine *CoreEngine::CORE = nullptr;
void CoreEngine::Init()
{
	CORE = this;
	quit = false;
	MetaDB::METADB->Init();
	_graphicsSystem.Initialize();
	
}

void CoreEngine::Start()
{
	theTestManager->RunTests();
	LuaHandler::StartUp();
	LuaHandler::TestLua();
	event_system.Register(&_theOneTrueUpdate);
	int i = 1;
	editor.Initialize();
	while(!quit)
	{
		frc.update();
		//TODO: Fix the update loop
		event_system.Trigger<UpdateEvent>();
		//Debugger::Log(FrameRateController::GetDT());
		_graphicsSystem.Update();
	}
	ImGui::DestroyContext();
	SceneManager::DestroyAll();
}

void CoreEngine::PostQuit()
{
	CORE->quit = true;
}
