#pragma once
#include <vector>
#include "TestBase.h"

class [[MetaAttr::Reflect()]] StringTestBattery : public TestBase
{
public:
	bool Run() override;
	const char* GetName() override;

private:
};
