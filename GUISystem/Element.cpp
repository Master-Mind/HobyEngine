#include "Element.h"
#include "../Graphics/Graphics.h"
#include "GUI.h"

namespace GUI
{
	void Element::Render(Graphics *graphics)
	{
		if (Parent)
		{
			MyTrans.Update(&Parent->MyTrans);
		}
		else
		{
			MyTrans.Update(nullptr);
		}
		//graphics->RenderGUI(MyTrans.ScreenPosition, MyTrans.ScreenScale, FillColor, BorderColor, Text);

		for (auto child : Children)
		{
			child->Render(graphics);
		}
	}

	void Element::AddChild(Element* child)
	{
	}

	Element::Element(Element *parent) : Parent(parent)
	{
		//Text.setFillColor(sf::Color(0,0,0,255));
		//Text.setCharacterSize(12);
		//Text.setStyle(sf::Text::Style::Regular);
		//Text.setFont(EditorGUI::GUIFont);
	}

	Element::~Element()
	{
		Children.clear();
	}
}
