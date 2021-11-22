#pragma once
#include <glm/vec2.hpp>
struct GLFWwindow;
struct ImGuiIO;
class InputHandler
{
public:
	friend class WindowHandler;
	InputHandler();
	static glm::vec2  GetMouseScreenPos();
	static void FillInGUIIO(ImGuiIO &io);
	GLFWwindow *window;
private:
	void  SetMouseScreenPos(glm::vec2 newPos);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static InputHandler * handler;
	glm::vec2 mouseScreenPos;
};
