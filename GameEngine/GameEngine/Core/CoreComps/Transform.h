#pragma once
#include "Component.h"
#include <glm/mat4x4.hpp>
class UpdateEvent;
class [[MetaAttr::Reflect(), MetaAttr::BindToLua()]] Transform : public Component
{
public: 
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	glm::vec3 Translation;
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	float Rotation;
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
	glm::vec3 Scale;
	glm::mat4x4 TransformMatrix;
	void Initialize() override;
	void Update(UpdateEvent *e);
	void Destroy() override;
};
