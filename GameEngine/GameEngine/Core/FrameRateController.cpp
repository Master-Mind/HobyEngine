#include "FrameRateController.h"
#include <thread>
float FrameRateController::dt;
float FrameRateController::dtReal;
float FrameRateController::dtScale;

FrameRateController::FrameRateController()
{
	lastTim = std::chrono::high_resolution_clock::now();
	dt = 0;
	dtReal = 0;
	dtScale = 1;
}

float FrameRateController::GetDT()
{
	return dt;
}

float FrameRateController::GetRealDT()
{
	return dtReal;
}

float FrameRateController::GetDTScale()
{
	return dtScale;
}

void FrameRateController::SetDTScale(float scale)
{
	dtScale = scale;
}

void FrameRateController::update()
{
	//std::this_thread::sleep_for(std::chrono::duration<float>(0.01));
	std::chrono::duration<float> dtime = std::chrono::high_resolution_clock::now() - lastTim;
	long long nanosecs = std::chrono::duration_cast<std::chrono::nanoseconds>(dtime).count();
	dtReal = static_cast<double>(nanosecs) / 1000000000.0;
	dt = dtReal * dtScale;

	lastTim = std::chrono::high_resolution_clock::now();
}
