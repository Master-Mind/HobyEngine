#include "TestClass.h"
#include <functional>
#include "MetaDB.h"

int Foo::baz(int wew)
{
	return 0;
}

void Foo::borf(Dirived* wew)
{
}

bool TestSpace::TestFoo::Run()
{
	return true;
}

const char* TestSpace::TestFoo::GetName()
{
	return "TestFoo";
}

Foo::Foo()
{
}

Foo::~Foo()
{
}