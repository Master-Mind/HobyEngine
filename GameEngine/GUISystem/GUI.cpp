#include "GUI.h"
#include "MenuBar.h"
#include <boost/filesystem.hpp>
#include "Debugger.h"

using namespace GUI;
std::vector<GUI::Element *> EditorGUI::_elements;
std::stack<GUI::Element *> EditorGUI::ElementStack;

void EditorGUI::Initialize()
{
	_elements.reserve(256);
	boost::filesystem::path fontPath = "C:\\Windows\\Fonts\\calibri.ttf";
}

void EditorGUI::StartFrame()
{
}


void EditorGUI::EndFrame(Graphics *graphics)
{
	for (auto element : _elements)
	{
		element->Render(graphics);
	}
	_elements.clear();
}

void EditorGUI::BeginMenuBar()
{
	AddElement(new MenuBar(nullptr));
}

void EditorGUI::EndMenuBar()
{
}
void EditorGUI::AddElement(GUI::Element* element)
{
	_elements.push_back(element);
	ElementStack.push(element);
}

void EditorGUI::PopElement()
{
	Element *el = ElementStack.top();
	ElementStack.pop();
	delete el;
}
