#pragma once
#include "Element.h"

namespace GUI
{
	class MenuBar : public Element
	{
	public:
		MenuBar(Element *parent);

		void AddChild(Element* child) override;
	private:

	};
}
