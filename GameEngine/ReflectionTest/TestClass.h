#pragma once
#include <utility>
#include "MetaAttribute.h"
#include "TestBase.h"

class [[MetaAttr::Reflect(), MetaAttr::BindToLua()]] Base
{
	
};

class [[MetaAttr::Reflect()]]  Dirived : public Base
{
	
};

enum [[MetaAttr::Reflect()]] fooEnum
{
	foo1,
	foo2,
	foo3
};

class [[MetaAttr::BindToLua()]] Foo
{
public:
	int Bar = 0666 + 2;
	Foo();
	~Foo();

	[[MetaAttr::BindToLua()]]
	int baz(int wew = 213234);
private:
	void borf(Dirived *wew);
};

namespace TestSpace
{
	class [[MetaAttr::Reflect()]] TestFoo : public TestBase
	{
	public:
		bool Run() override;
		const char* GetName() override;
	};
}