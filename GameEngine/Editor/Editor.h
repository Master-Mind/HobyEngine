#pragma once
#include <vector>
#include "DebugWindow.h"
#include "FileDialog.h"
#include "GraphicsAssets/Texture.h"
#include <glm/gtc/quaternion.hpp>
struct ImGuiIO;
class Sprite;
class Editor
{
public:
	void Initialize();
	void Update();
private:
	int DebugDropDown(int xPos);
	int FileDropDown(int xPos);
	void Outliner();
	void OutlinerInternal(GameObject *root);
	void DrawSelection();
	glm::vec3 DrawPositionHandle(glm::vec3 pos, glm::quat rotation);
	glm::quat DrawRotationHandle(glm::vec3 pos, glm::quat rotation);
	glm::vec3 DrawScaleHandle(glm::vec3 pos, glm::vec3 scale, glm::quat rotation);
	bool MouseOverPlane(glm::vec3 topleft, glm::vec3 topright, glm::vec3 bottomleft, glm::vec3 bottomrigh);
	bool DDrop = false;
	ImGuiIO * io;
	Texture *textTex;
	std::vector<DebugWindow> _debugWindows;
	FileDialog diolog;
	GameObject *_selectedObj;
	glm::vec2 _mouseDelta;
	glm::vec2 _lastMouse;

};
