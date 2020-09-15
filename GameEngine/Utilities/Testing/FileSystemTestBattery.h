#pragma once
#include <vector>
#include "TestBase.h"

class FileSystemTestBattery : public TestBase
{
public:
	bool Run() override;
	const char* GetName() override;
private:
};
