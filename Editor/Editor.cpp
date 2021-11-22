#include "Editor.h"
#include <imgui.h>
#include <cmath>
#include "../Graphics/Graphics.h"
#include "GraphicsAssets/Sprite.h"
#include <glad/glad.h>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/perpendicular.hpp>
#include "Core.h"
#include "Scene.h"
#include "Scenemanager.h"
#include "../Graphics/Color.h"

void Editor::Initialize()
{
	_selectedObj = nullptr;
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	Graphics::ENGINE->GetScreenSize(io->DisplaySize.x, io->DisplaySize.y);
	textTex = new Texture();
	unsigned char * texData;
	int texWidth, texHeight;
	io->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 24);
	io->Fonts->Build();

	io->Fonts->GetTexDataAsRGBA32(&texData, &texWidth, &texHeight);
	glGenTextures(1, &textTex->_texID);
	glBindTexture(GL_TEXTURE_2D, textTex->_texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	glBindTexture(GL_TEXTURE_2D, 0);
	io->Fonts->SetTexID(textTex);
	_debugWindows.emplace_back(DebugWindow(DTag::All));
	//std::ofstream ofile("wew.bmp", std::ios::out | std::ios::binary);
	//ofile.write(reinterpret_cast<char *>(texData), texWidth *texHeight);
}

void Editor::Update()
{
	io->DeltaTime = CoreEngine::CORE->frc.GetRealDT();
	InputHandler::FillInGUIIO(*io);
	ImGui::NewFrame();
	_mouseDelta = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
	_mouseDelta -= _lastMouse;
	_lastMouse = {ImGui::GetMousePos().x, ImGui::GetMousePos().y};
	ImGui::BeginMainMenuBar();
	//Debugger::DumpLogToEditWIndow();
	ImGui::Text(WindowHandler::WindowName);
	ImGui::SameLine(ImGui::GetWindowWidth() - 90);
	if(ImGui::Button("-"))
	{
		Graphics::ENGINE->Minimize();
	}
	if (ImGui::Button("O"))
	{

	}
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.5,0,0,1 });
	if (ImGui::Button("X"))
	{
		CoreEngine::PostQuit();
	}
	ImGui::PopStyleColor();

	ImGui::EndMainMenuBar();
	int x = ImGui::CalcTextSize(WindowHandler::WindowName).x + 10;
	x = FileDropDown(x);
	x = DebugDropDown(x);
	Outliner();
	for (int i = 0; i < _debugWindows.size(); ++i)
	{
		if(_debugWindows[i].Display(i))
		{
			_debugWindows.erase(_debugWindows.begin() + i);
			i--;
		}
	}
	if(diolog.Active())
	{
		if(!diolog.Update())
		{
			SceneManager::LoadScene(diolog.loadSelectedFile<Scene>());
			_selectedObj = nullptr;
		}
	}
	
	if(_selectedObj && _selectedObj->GetComponent<Transform>())
	{
		DrawSelection();
	}

	ImGui::Render();
	
	Graphics::ENGINE->RenderEditor();
}


int Editor::FileDropDown(int xPos)
{
	ImGui::Begin("File ");
	ImGui::SetWindowPos(ImVec2(xPos, 0));
	int ret = ImGui::GetWindowWidth();
	if (ImGui::Button("Open"))
	{
		diolog.Activate();
	}
	ImGui::End();
	return ret + xPos;
}

void Editor::Outliner()
{
	if (!SceneManager::GetActiveScene().isNull())
	{
		ImGui::Begin("Outliner");
		GameObject* root = SceneManager::GetActiveScene()->getRootObj();
		OutlinerInternal(root);
		ImGui::End();
	}
}

void Editor::OutlinerInternal(GameObject* root)
{
	if(root == _selectedObj)
	{
		if (ImGui::Button(('>' + root->GetName().toString()).c_str()))
		{
			_selectedObj = root;
		}
	}
	else
	{
		if (ImGui::Button(root->GetName()))
		{
			_selectedObj = root;
		}
	}
	ImGui::Indent(10);
	for (auto child : root->GetChildren())
	{
		OutlinerInternal(child.second);
	}
	ImGui::Indent(0);
}

void Editor::DrawSelection()
{
	glm::vec4 topleft = { -0.5,0.5,0.1,1 };
	glm::vec4 topRight = { 0.5,0.5,0.1,1 };
	glm::vec4 bottomLeft = { -0.5,-0.5,0.1,1 };
	glm::vec4 bottomRight = { 0.5,-0.5,0.1,1 };
	Transform *trans = _selectedObj->GetComponent<Transform>();
	topleft = trans->TransformMatrix * topleft;
	topRight = trans->TransformMatrix * topRight;
	bottomLeft = trans->TransformMatrix * bottomLeft;
	bottomRight = trans->TransformMatrix * bottomRight;

	Debugger::DrawLine(topleft, topRight, { 0,1,1,1 });
	Debugger::DrawLine(topRight, bottomRight, { 0,1,1,1 });
	Debugger::DrawLine(bottomRight, bottomLeft, { 0,1,1,1 });
	Debugger::DrawLine(bottomLeft, topleft, { 0,1,1,1 });

	trans->Translation = DrawPositionHandle(trans->Translation, glm::quat());
}

glm::vec3 Editor::DrawPositionHandle(glm::vec3 pos, glm::quat rotation)
{
	const float arrowLen = 2;
	const float arrowWidth = 0.5f;
	Color color = DColor::red;
	glm::vec3 localRight = rotation * glm::vec3{ 1,0,0 };
	glm::vec3 localUp = rotation * glm::vec3{ 0,1,0 };

	//x axis
	if (MouseOverPlane(pos + localUp * arrowWidth / 2.0f, 
		pos + localUp * arrowWidth / 2.0f + localRight * arrowLen,
		pos - localUp * arrowWidth / 2.0f, 
		pos - localUp * arrowWidth / 2.0f + localRight * arrowLen))
	{
		color = DColor::yellow;
		if(ImGui::IsMouseDragging())
		{
			glm::vec3 vec = Graphics::ENGINE->ScreenToWorld(_mouseDelta);
			pos.x += vec.x;
		}
	}
	Debugger::DrawArrow(pos, pos + localRight * arrowLen, color);

	//y axis
	if (MouseOverPlane(pos - localRight * arrowWidth / 2.0f,
		pos - localRight * arrowWidth / 2.0f + localUp * arrowLen,
		pos + localRight * arrowWidth / 2.0f,
		pos + localRight * arrowWidth / 2.0f + localUp * arrowLen))
	{
		color = DColor::yellow;
		if (ImGui::IsMouseDragging())
		{
			glm::vec3 vec = Graphics::ENGINE->ScreenToWorld(_mouseDelta);
			pos.y += vec.y;
		}
	}
	else
	{
		color = DColor::green;
	}
	Debugger::DrawArrow(pos, pos + localUp * arrowLen, color);


	return pos;
}

glm::quat Editor::DrawRotationHandle(glm::vec3 pos, glm::quat rotation)
{

	return rotation;
}

glm::vec3 Editor::DrawScaleHandle(glm::vec3 pos, glm::vec3 scale, glm::quat rotation)
{
	return scale;
}

inline bool same_sign(float a, float b) {
	return signbit(a) == signbit(b);
}
inline int intersect(glm::vec2 s1, glm::vec2 e1, glm::vec2 s2, glm::vec2 e2)
{
	return same_sign(dot((e1 - s1), (s2 - s1)), dot((s1 - e1), (s2 - e1))) &&
		same_sign(dot((e2 - s2), (s1 - s2)), dot((s2 - e2), (s1 - e2))) &&
		same_sign(dot((e1 - s1), (s2 - s1)), dot((s1 - e1), (s2 - e1))) &&
		same_sign(dot((e2 - s2), (s1 - s2)), dot((s2 - e2), (s1 - e2)));
}
bool Editor::MouseOverPlane(glm::vec3 topleft, glm::vec3 topright, glm::vec3 bottomleft, glm::vec3 bottomright)
{
	glm::vec2 tLeft = Graphics::ENGINE->WorldToScreen(topleft);
	glm::vec2 tright = Graphics::ENGINE->WorldToScreen(topright);
	glm::vec2 bleft = Graphics::ENGINE->WorldToScreen(bottomleft);
	glm::vec2 bright = Graphics::ENGINE->WorldToScreen(bottomright);

	glm::vec3 mouse = { ImGui::GetMousePos().x, ImGui::GetMousePos().y, -40 };
	DEBUGLOG((std::string("Mouse pos: {") + std::to_string(mouse.x) + ',' + std::to_string(mouse.y) + '}').c_str(), DTag::Editor);
	
	glm::vec2 pos;
	float dist = 0;
	bool ret1 = glm::intersectRayTriangle(mouse, { 0,0,1 }, { bright.x, bright.y, 0 }, { tright.x, tright.y, 0 }, { tLeft.x, tLeft.y, 0 }, pos, dist);
	pos = { 0,0 };
	dist = 0;
	bool ret2 = glm::intersectRayTriangle(mouse, { 0,0,1 }, { bright.x, bright.y, 0 }, { bleft.x, bleft.y, 0 }, { tLeft.x, tLeft.y, 0 }, pos, dist);
	return ret1 || ret2;
}

int Editor::DebugDropDown(int xPos)
{
	ImGui::Begin("Windows");
	ImGui::SetWindowPos(ImVec2(xPos, 0));
	int ret = ImGui::GetWindowWidth();
	if(ImGui::Button("Add Debug Window"))
	{
		_debugWindows.emplace_back(DebugWindow());
	}
	ImGui::End();
	return ret + xPos;
}

#include "imgui.cpp"
#include "imgui_draw.cpp"
#include "imgui_widgets.cpp"