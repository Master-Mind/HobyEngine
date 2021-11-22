#pragma once
#include <glm/vec2.hpp>

class RectTransform
{
public:
	void Update(RectTransform *parent);
	glm::vec2 LocalPosition;
	glm::vec2 LocalScale;
	//Location of the closest corner to the screen anchor
	glm::vec2 ScreenPosition;
	//Location of the farthest corner to the screen anchor - the screenPos
	glm::vec2 ScreenScale;
};
