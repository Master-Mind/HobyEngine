#include "UpdateEvent.h"
#include "Core.h"
float UpdateEvent::Dt()
{
	return CoreEngine::CORE->frc.GetDT();
}

float UpdateEvent::RealDt()
{
	return CoreEngine::CORE->frc.GetRealDT();
}
