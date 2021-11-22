#pragma once
#include "../GUISystem/Element.h"

namespace GUI
{
	class Button : Element
	{
	public:
		Button(Element *parent);
	private:
		bool _down;
	};
}
