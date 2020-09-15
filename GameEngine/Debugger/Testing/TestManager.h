#pragma once
#include <vector>
#include "Singleton.h"
class TestBase;

class TestManager
{
public:
	void RunTests();
	void RegisterTest(TestBase *test);
private:
	std::vector<TestBase *> _tests;
};

DECLARE_SINGLETON(TestManager);