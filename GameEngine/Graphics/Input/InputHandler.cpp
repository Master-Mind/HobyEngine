#include "InputHandler.h"
#include <GLFW/glfw3.h>
#include "Core.h"
#include <imgui.h>

InputHandler * InputHandler::handler;

InputHandler::InputHandler()
{
	handler = this;
}

glm::vec2 InputHandler::GetMouseScreenPos()
{
	return handler->mouseScreenPos;
}

void InputHandler::FillInGUIIO(ImGuiIO& io)
{
	io.MousePos.x =GetMouseScreenPos().x;
	io.MousePos.y =GetMouseScreenPos().y;
	io.MouseDown[0] = glfwGetMouseButton(handler->window, 0);
	io.MouseDown[1] = glfwGetMouseButton(handler->window, 1);
	io.MouseDown[2] = glfwGetMouseButton(handler->window, 2);
	io.MouseDown[3] = glfwGetMouseButton(handler->window, 3);
	io.MouseDown[4] = glfwGetMouseButton(handler->window, 4);
}

void InputHandler::SetMouseScreenPos(glm::vec2 newPos)
{
	mouseScreenPos = newPos;
}
void InputHandler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	handler->mouseScreenPos = { xpos , ypos};
}