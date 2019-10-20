#pragma once
#include <vector>
#include "TestInter.h"

class FileSystemTestBattery : public TestInter
{
public:
	bool Run() override;
	const char* GetName() override;
private:
};
