#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

#include "UpdateEvent.h"
#include "Core.h"

void Transform::Initialize()
{
	CoreEngine::CORE->event_system.Subscribe<UpdateEvent>(new EventHandler(this, &Transform::Update));
}

void Transform::Update(UpdateEvent* e)
{
	TransformMatrix = glm::translate(glm::mat4x4(), Translation) * glm::scale(glm::mat4x4(), Scale) *  glm::rotate(glm::mat4x4(), Rotation, {0,0,-1});
}

void Transform::Destroy()
{
	CoreEngine::CORE->event_system.UnSubscribe<UpdateEvent>(this);
}
