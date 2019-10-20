#pragma once
#include <vector>
#include "TestInter.h"

class [[MetaAttr::Reflect()]] StringTestBattery : public TestInter
{
public:
	bool Run() override;
	const char* GetName() override;

private:
};
