#include "TestManager.h"
#include "TestBase.h"
#include "Debugger.h"
DEFINE_SINGLETON(TestManager);

void TestManager::RunTests()
{
	for (auto test : _tests)
	{
		const char *testName = test->GetName();
		std::string testResult = "The test \"";
		testResult += test->GetName();
		testResult += "\" has ";

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

void TestManager::RegisterTest(TestBase* test)
{
	_tests.push_back(test);
}