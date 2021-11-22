
#include "Graphics.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Memory/TypeAllocator.h"
#include "GraphicsComps/SpriteRenderer.h"
#include <glm/detail/_vectorize.hpp>
#include <glm/matrix.hpp>
#include "../GUISystem/GUI.h"
#include "../GUISystem/GUI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core.h"
#include "glad/glad.h"
#include "gl/GL.h"
#include "glad/glad.h"
#include "glad/glad.h"
Graphics * Graphics::ENGINE;
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::string grapicserr;
	grapicserr.reserve(512);
	grapicserr = grapicserr + "Graphics error (" + std::to_string(id) + "): " + message + '\n';
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             grapicserr += "Source: Source API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   grapicserr += "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: grapicserr += "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     grapicserr += "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     grapicserr += "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           grapicserr += "Source: Other"; break;
	} grapicserr += '\n';

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               grapicserr +=  "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: grapicserr +=  "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  grapicserr +=  "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         grapicserr +=  "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         grapicserr +=  "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              grapicserr +=  "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          grapicserr +=  "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           grapicserr +=  "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               grapicserr +=  "Type: Other"; break;
	} grapicserr += '\n';

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         grapicserr += "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       grapicserr += "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          grapicserr += "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: grapicserr += "Severity: notification"; break;
	} grapicserr += '\n';
	grapicserr += '\n';
	DEBUGASSERTMSG(false, grapicserr.c_str(), DTag::Graphics);
}
glm::mat4x4 translateMat(glm::vec3 trans)
{
	glm::mat4x4 ret;

	ret[0][3] = trans[0];
	ret[1][3] = trans[1];
	ret[2][3] = trans[2];

	return ret;
}

float GraphicsUpdate::Dt()
{
	return CoreEngine::CORE->frc.GetDT();
}

float GraphicsUpdate::RealDt()
{
	return CoreEngine::CORE->frc.GetRealDT();
}
void Graphics::Initialize()
{
	gameZoom = 10;
	ENGINE = this; 
	SetScreenSize(1920, 1080);
	winHandler.Initialize(screenWidth, screenHeight);
	DEBUGASSERTMSG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD", DTag::Graphics);
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	Orthographic = glm::ortho(-gameZoom, gameZoom, -gameZoom / aspectRatio, gameZoom / aspectRatio, 0.5f, 1000.0f);
	auto trans = glm::translate(glm::mat4x4(1), glm::vec3(0, 0, -40));
	auto sc = glm::scale(glm::mat4x4(1), glm::vec3(1.0f / 4, 1.0f / 2, 1));
	WorldView = trans;
	_quadPrimative.SetVerts(
	{
		{ { 0.5f, 0.5f, 0 } ,{ 0, 0, 0 } ,{ 1, 1 } },
		{ { 0.5f, -0.5f, 0 },{ 0, 0, 0 },{ 1, 0 } },
		{ { -0.5f, -0.5f, 0 } ,{ 0, 0, 0 },{ 0, 0 } },
		{ { -0.5f, 0.5f, 0 } ,{ 0, 0, 0 },{ 0, 1 } }
	}
	, {0,1,3,
	   1,2,3});
	_quadPrimative.Bind();

	ImShader = AssetDataBase::RequestAsset<Shader>("imgui");
	glGenBuffers(1, &imguiVBO);
	glGenBuffers(1, &imguiIndices);
	glGenVertexArrays(1, &imguiVAO);

	glBindVertexArray(imguiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, imguiVBO);

	unsigned vertsize = sizeof(ImDrawVert);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertsize, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertsize, (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertsize, (void*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glEnable(GL_LINE_SMOOTH);
	DebugShader = AssetDataBase::RequestAsset<Shader>("Debug");


	glGenBuffers(1, &debugVBO);
	glGenBuffers(1, &debugInstance);
	glGenVertexArrays(1, &debugVAO);

	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
	//glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), lineBuffer, GL_STATIC_DRAW);

	glBindVertexArray(debugVAO);

	//Enable vertex position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugLineVert), (void*)offsetof(DebugLineVert, pos));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DebugLineVert), (void*)offsetof(DebugLineVert, color));
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(DebugLineVert), (void*)offsetof(DebugLineVert, color));
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DebugLineVert), (void*)offsetof(DebugLineVert, thickness));
	//glEnableVertexAttribArray(4);

	//glVertexAttribDivisor(3, 1);
	//glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	EditorGUI::Initialize();
}

void Graphics::Update()
{
	//poll events
	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CoreEngine::CORE->event_system.Trigger<GraphicsUpdate>();
	CoreEngine::CORE->editor.Update();
	//EditorGUI::StartFrame();
	//EditorGUI::BeginMenuBar();
	//EditorGUI::EndMenuBar();
	//EditorGUI::EndFrame(this);
	Debugger::DrawAll();
	glfwSwapBuffers(winHandler.getWindow());
	winHandler.Update();
}

void Graphics::Render(Sprite& sprite, Transform *Transform)
{
	sprite._myShader->Use();
	sprite._myShader->SetMat4("view", WorldView);
	sprite._myShader->SetMat4("model", Transform->TransformMatrix);
	sprite._myShader->SetMat4("projection", Orthographic);
	sprite._myShader->SetTex("ourTexture", sprite._myTex->_texID);
	sprite.Use(0);
	_quadPrimative.Draw();
	//winHandler.getWindow().draw(sprite);
}

void Graphics::RenderGUI(glm::vec2 pos, glm::vec2 scale, glm::vec4 fillColor, glm::vec4 borderColor, std::string text)
{
	//sf::RectangleShape rect(sf::Vector2f(scale.x * screenWidth, scale.y *screenHeight));
	//rect.setPosition(pos.x * screenWidth,pos.y *screenHeight);
	//rect.setFillColor(sf::Color(fillColor.x * 255, fillColor.y * 255, fillColor.z * 255, fillColor.w * 255));
	//rect.setOutlineColor(sf::Color(borderColor.x * 255, borderColor.y * 255, borderColor.z * 255, borderColor.w * 255));
	//rect.setOutlineThickness(1);
	//winHandler.getWindow().draw(rect);
	//if (text.getString().getSize() > 0)
	//{
	//	text.setPosition(pos.x * screenWidth, pos.y *screenHeight);
	//	winHandler.getWindow().draw(text);
	//}
}

void Graphics::Minimize()
{
	winHandler.Iconify();
}

void Graphics::ResetScissor()
{
	glScissor(0, 0, screenWidth, screenHeight);
}
void Graphics::SetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	aspectRatio = float(width) / height;

	Orthographic = glm::ortho(-gameZoom, gameZoom, -gameZoom / aspectRatio, gameZoom / aspectRatio, 0.5f, 1000.0f);
}

void Graphics::GetScreenSize(float& width, float& height)
{
	width = (float)screenWidth;
	height = (float)screenHeight;
}

void Graphics::RenderEditor()
{
	auto data = ImGui::GetDrawData();


	ImGuiIO& io = ImGui::GetIO();



	glBlendEquation(GL_FUNC_ADD);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	ImShader->Use();

	float fac = 1;
	glm::mat4 orthoMatrix = glm::ortho(0.0f, screenWidth * fac, screenHeight * fac, 0.0f, -1.0f, 1.0f);;
	std::string name = "viewMatrix";
	ImShader->SetMat4(name.c_str(), orthoMatrix);
	//glm::mat4 orthoMatrix = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 orthoMatrix = *(ACTIVE_CAMERA->GetProjectionMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(imguiVAO);

	for (int i = 0; i < data->CmdListsCount; i++)
	{
		ImDrawList* cur = data->CmdLists[i];
		ImDrawIdx* indexOffset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, imguiVBO);
		glBufferData(GL_ARRAY_BUFFER, cur->VtxBuffer.Size * sizeof(ImDrawVert), (GLvoid*)cur->VtxBuffer.begin(), GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		size_t vertSize = sizeof(ImDrawVert);

		std::vector<ImDrawVert> temp = std::vector<ImDrawVert>();

		//temp.reserve(cur->VtxBuffer.Size);
		temp.resize(cur->VtxBuffer.Size);

		memcpy(&(temp[0]), cur->VtxBuffer.begin(), cur->VtxBuffer.Size * sizeof(ImDrawVert));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imguiIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cur->IdxBuffer.Size * sizeof(ImDrawIdx), (GLvoid*)cur->IdxBuffer.Data, GL_STREAM_DRAW);


		for (int command = 0; command < cur->CmdBuffer.Size; command++)
		{
			ImDrawCmd* currentCommand = &cur->CmdBuffer[command];
			Texture *tex = static_cast<Texture*>(currentCommand->TextureId);
			ImShader->SetTex("Texture", tex->_texID);
			glBindTexture(GL_TEXTURE_2D, tex->_texID);
			SuppressGraphicsErrors();
			glScissor((int)currentCommand->ClipRect.x,(int)currentCommand->ClipRect.y + screenHeight, (int)(currentCommand->ClipRect.z - currentCommand->ClipRect.x), -(int)(currentCommand->ClipRect.w - currentCommand->ClipRect.y));
			UnSuppressGraphicsErrors(); 
			glDrawElements(GL_TRIANGLES, currentCommand->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)indexOffset);

			indexOffset += currentCommand->ElemCount;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	ResetScissor();
	glEnable(GL_DEPTH_TEST);
}

void Graphics::RenderDebug(std::vector<DebugLineVert>& lines)
{
	DebugShader->Use();

	DebugShader->SetMat4("view", WorldView);
	DebugShader->SetMat4("projection", Orthographic);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
	glBindVertexArray(debugVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, debugInstance);
	//
	//glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(DebugLineVert), lines.data(), GL_STREAM_DRAW);
	for (int i = 0; i < lines.size(); i += 2)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(DebugLineVert) * 2, &lines[i], GL_STREAM_DRAW);

		glDrawArrays(GL_LINES, 0, 2);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::vec2 Graphics::WorldToScreen(glm::vec3 vec)
{
	auto ret = Orthographic * WorldView * glm::vec4(vec.x, vec.y, vec.z, 1);
	return { (ret.x + 1) / 2 * screenWidth, (1 - ret.y) / 2 * screenHeight };
}

glm::vec3 Graphics::ScreenToWorld(glm::vec2 vec)
{
	vec.x *= 2.0f / screenWidth;
	vec.x -= 1;
	vec.y *= -2.0f / screenHeight;
	vec.x += 1;

	return glm::inverse(Orthographic * WorldView) * glm::vec4{vec.x, vec.y, 0, 1};
}

void Graphics::SuppressGraphicsErrors()
{
	glDisable(GL_DEBUG_OUTPUT);
	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

void Graphics::UnSuppressGraphicsErrors()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

void Graphics::SuppressGraphicsTypeErrors()
{
	_suppressGraphicsTypeErrors = true;
}

void Graphics::UnSuppressGraphicsTypeErrors()
{
	_suppressGraphicsTypeErrors = false;
}

