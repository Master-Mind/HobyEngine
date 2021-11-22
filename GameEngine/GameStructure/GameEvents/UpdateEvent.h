#pragma once
#include "Event.h"
class UpdateEvent : public Event
{
public:
	float Dt();
	float RealDt();
};
