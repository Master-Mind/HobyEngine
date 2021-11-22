#pragma once
#include <string>
#include "Input/InputHandler.h"
struct GLFWwindow;
class WindowHandler
{
public:
	void Initialize(int width, int height);
	//polls events and stuff
	void Update();
	GLFWwindow *getWindow();
	std::string GetFontPath();
	static void FrameBufferSIzeCallback(GLFWwindow *win, int newWidth, int newHeight);
	static void WindowErrorHandler(int error, const char *text);
	void Iconify();
	static const char *WindowName;
private:
	InputHandler input;
	GLFWwindow *window;
	int WinWidth;
	int WinHeight;
};