#include "Core.h"
#include "TestBase.h"

int  main()
{
	ClassThatOnlyExistsSoThatTheLinkerDoesntChuckMaShiz::callAll();
	CoreEngine core;

	core.Init();
	core.Start();

	return 0; 
}
