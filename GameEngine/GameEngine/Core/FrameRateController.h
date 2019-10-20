#pragma once
#include <chrono>

class FrameRateController
{
public:
	FrameRateController();
	static float GetDT();
	static float GetRealDT();
	static float GetDTScale();
	static void SetDTScale(float scale);
	void update();

private:
	static float dt;
	static float dtReal;
	static float dtScale;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastTim;
};
