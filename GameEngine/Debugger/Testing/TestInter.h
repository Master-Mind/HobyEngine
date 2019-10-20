#pragma once
#include "StaticConstructor.h"
#include "MetaAttribute.h"

class [[MetaAttr::Reflect()]] TestInter
{
public:
	virtual ~TestInter() = default;
	virtual bool Run() { return false; }
	virtual const char* GetName() { return "you forgot to name this you dumb fuck"; }
private:
};


#define GET_TEST_NAME_IMPL(TEST_TYPE) \
const char* TEST_TYPE::GetName()\
{\
	return #TEST_TYPE;\
}