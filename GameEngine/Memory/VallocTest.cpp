#include "VallocTest.h"
#include "VariableAllocator.h"
#include "Debugger.h"
#include "TestManager.h"

//DEFAULT_REGISTER(VallocTest)

VallocTest::VallocTest()
{
}


VallocTest::~VallocTest()
{
}

bool VallocTest::Run()
{
	VariableAllocator alloc(0);

	DEBUGLOG("Testing basic allocations...", DTag::Testing);
	int *i = VTALLOC(&alloc, int);
	*i = 0;
	alloc.deallocate(i);
	DEBUGASSERTMSG(alloc.checkLeaks().size() == 0, "Allocator still reports a leak!", DTag::Testing);

	DEBUGLOG("Testing leak detection...", DTag::Testing);
	i = VTALLOC(&alloc, int);
	*i = 0;
	auto leaks = alloc.checkLeaks();
	DEBUGASSERTMSG(leaks.size() == 1, "Allocator isn't reporting the leak!", DTag::Testing);

	for(auto &it : leaks)
	{
		DEBUGLOG(it.c_str(), DTag::Testing);
	}

	alloc.deallocate(i);

	return true;
}

GET_TEST_NAME_IMPL(VallocTest)
