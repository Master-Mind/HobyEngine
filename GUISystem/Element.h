#pragma once
#include <vector>
#include <glm/vec4.hpp>
#include "RectTransform.h"

class Graphics;
namespace GUI
{

	class Element
	{
	public:
		Element(Element *parent);
		~Element();
		void Render(Graphics *graphics);
		virtual void AddChild(Element *child);
		std::vector<Element *> Children;
		Element * Parent;
		RectTransform MyTrans;
		glm::vec4 FillColor = glm::vec4(0, 1, 0, 0);
		glm::vec4 BorderColor = glm::vec4(0, 0, 0, 1);
	private:
	};

}