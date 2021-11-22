#include "MenuBar.h"

GUI::MenuBar::MenuBar(Element *parent) : Element(parent)
{
	MyTrans.LocalPosition = MyTrans.ScreenPosition = glm::vec2(0, 0);
	MyTrans.LocalScale = MyTrans.ScreenScale = glm::vec2(1, 0.05);
	FillColor = glm::vec4(0.01, 0.01, 0.01, 1);
	BorderColor = glm::vec4(0, 1, 0, 1);
}

void GUI::MenuBar::AddChild(Element* child)
{
	Children.push_back(child);
	child->MyTrans.LocalPosition = glm::vec2();
}
