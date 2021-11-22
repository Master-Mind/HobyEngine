#pragma once
#include "StringTestBattery.h"
#include "TestManager.h"

//DEFAULT_REGISTER(StringTestBattery)
bool StringTestBattery::Run()
{
	//TODO: see if memcmp is the same as strcmp
	return true;
}

GET_TEST_NAME_IMPL(StringTestBattery)