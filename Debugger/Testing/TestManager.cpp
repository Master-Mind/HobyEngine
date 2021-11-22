#include "TestManager.h"
#include "TestInter.h"
#include "Debugger.h"
DEFINE_SINGLETON(TestManager);

void TestManager::RunTests()
{
	for (auto test : _tests)
	{
		const char *testName = test->GetName();
		std::string testResult = "The test \"";
		testResult += test->GetName();
		testResult += " has ";

		if(!test->Run())
		{
			DEBUGASSERTFAILED((testResult + "failed").c_str(), DTag::Testing);
		}
		else
		{
			DEBUGLOG((testResult + "passed").c_str(), DTag::Testing);
		}
	}
}

void TestManager::RegisterTest(TestInter* test)
{
	_tests.push_back(test);
}