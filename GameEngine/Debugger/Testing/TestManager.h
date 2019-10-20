#pragma once
#include <vector>
#include "Singleton.h"
class TestInter;

class TestManager
{
public:
	void RunTests();
	void RegisterTest(TestInter *test);
private:
	std::vector<TestInter *> _tests;
};

DECLARE_SINGLETON(TestManager);