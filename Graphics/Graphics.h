#pragma once
#include "WindowHandling/WindowHandler.h"
#include <glm/mat4x4.hpp>
#include "Transform.h"
#include "GraphicsAssets/Model.h"
#include "Event.h"
class Shader;
class Sprite;
#define GRAPHICS_FRIENDS \
friend class SpriteRenderer;\
friend class AssetDataBase;\
friend class Graphics;\
friend class Editor
class GraphicsUpdate : Event
{
public:
	float Dt();
	float RealDt();
};

class Graphics
{
public:
	void Initialize();
	void Update();
	void Render(Sprite& sprite, Transform* Transform);
	void RenderGUI(glm::vec2 pos, glm::vec2 scale, glm::vec4 fillColor, glm::vec4 borderColor, std::string text);
	void SetScreenSize(int width, int height);
	void GetScreenSize(float &width, float &height);
	void RenderEditor();
	void RenderDebug(std::vector<DebugLineVert> &lines);
	glm::vec2 WorldToScreen(glm::vec3 vec);
	glm::vec3 ScreenToWorld(glm::vec2 vec);
	void SuppressGraphicsErrors();
	void UnSuppressGraphicsErrors();
	void SuppressGraphicsTypeErrors();
	void UnSuppressGraphicsTypeErrors();
	glm::mat4x4 WorldView;
	glm::mat4x4 Orthographic;
	static Graphics * ENGINE;
	bool _suppressGraphicsTypeErrors;
	void Minimize();
private:
	void ResetScissor();
	WindowHandler winHandler;
	int screenWidth;
	int screenHeight;
	float aspectRatio;
	Model _quadPrimative;
	float gameZoom;
	AssetReference<Shader> ImShader;
	AssetReference<Shader> DebugShader;
	unsigned imguiVBO, imguiVAO, imguiIndices;
	unsigned debugVBO, debugVAO, debugInstance;
};
