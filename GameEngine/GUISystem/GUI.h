#pragma once
#include <vector>
#include "Element.h"
#include <stack>


class EditorGUI
{
public:
	static void Initialize();
	static void StartFrame();
	static void BeginMenuBar();
	static void EndMenuBar();
	static void EndFrame(Graphics *graphics);
private:
	static void AddElement(GUI::Element *element);
	static void PopElement();
	static std::vector<GUI::Element *> _elements;
	static std::stack<GUI::Element *> ElementStack;
};
