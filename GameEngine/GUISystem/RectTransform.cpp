#include "RectTransform.h"

void RectTransform::Update(RectTransform* parent)
{
	if(!parent)
	{
		ScreenPosition = LocalPosition;
		ScreenScale = LocalScale;
		return;
	}
	auto scaledLocalPos = LocalPosition *parent->ScreenScale;
	ScreenPosition = scaledLocalPos + parent->ScreenPosition;
	ScreenScale = LocalScale * parent->ScreenScale;
}
