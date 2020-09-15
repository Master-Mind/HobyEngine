#pragma once
#include "TestBase.h"

class VallocTest :
	public TestBase
{
public:
	VallocTest();
	~VallocTest();
	bool Run() override;
	const char* GetName() override;
};

