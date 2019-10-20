#include "WindowHandler.h"
#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input/InputHandler.h"

const char const *WindowHandler::WindowName = "Game Engine";
void WindowHandler::Initialize(int width, int height)
{
	WinWidth = width;
	WinHeight = height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	window = glfwCreateWindow(WinWidth, WinHeight, "Game Engine", nullptr, nullptr);
	DEBUGASSERTMSG(window, "Window could not load properly", DTag::WindowManagement);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, &FrameBufferSIzeCallback);
	glfwSetErrorCallback(&WindowErrorHandler);
	glfwSetCursorPosCallback(window, InputHandler::cursor_position_callback);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, Debugger::IsInDebug);
	input.window = window;
}

void WindowHandler::Update()
{
	if(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	else
	{
		CoreEngine::CORE->PostQuit();
	}
}

GLFWwindow *WindowHandler::getWindow()
{
	return window;
}

std::string WindowHandler::GetFontPath()
{
	return "C:\\Windows\\Fonts";
}

void WindowHandler::FrameBufferSIzeCallback(GLFWwindow* win, int newWidth, int newHeight)
{
	Graphics::ENGINE->SetScreenSize(newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

void WindowHandler::WindowErrorHandler(int error, const char* text)
{
	DEBUGASSERTMSG(false, text, DTag::WindowManagement);
}

void WindowHandler::Iconify()
{
	glfwIconifyWindow(window);
}
