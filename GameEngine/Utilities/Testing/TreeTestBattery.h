#pragma once
#include <vector>
#include "TestInter.h"

class TreeTestBattery : public TestInter
{
public:
	bool Run() override;
	const char* GetName() override;

private:
};
