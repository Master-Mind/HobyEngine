#pragma once
#include <utility>
#include "MetaAttribute.h"
#include "TestInter.h"

class [[MetaAttr::Reflect(), MetaAttr::BindToLua()]] Base
{
	
};

class [[MetaAttr::Reflect()]]  Dirived : public Base
{
	
};


class [[MetaAttr::BindToLua()]] Foo
{
public:
	[[MetaAttr::Reflect(), MetaAttr::BindToLua()]]
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
	class [[MetaAttr::Reflect()]] TestFoo : public TestInter
	{
	public:
		bool Run() override;
		const char* GetName() override;
	};
}