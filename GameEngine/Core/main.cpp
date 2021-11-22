#include "Core.h"
#include "TestInter.h"

int  main()
{
	ClassThatOnlyExistsSoThatTheLinkerDoesntChuckMaShiz::callAll();
	CoreEngine core;

	core.Init();
	core.Start();

	return 0; 
}
