#pragma once
#include "TestInter.h"

class VallocTest :
	public TestInter
{
public:
	VallocTest();
	~VallocTest();
	bool Run() override;
	const char* GetName() override;
};

