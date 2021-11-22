//#define TESTING
#ifdef TESTING
#include <stdarg.h>
#include "Delegate.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "TestClass.h"
#include "MetaDB.h"
TEST_CASE("Basic Functionality")
{
	MetaDB db;
	auto ref = db.GetMetaData<Foo>();
	CHECK(ref->GetName() == "Foo");
	CHECK(ref->GetMember(0) != nullptr);
	CHECK(std::strcmp(ref->GetMember(0)->GetName(), "Bar") == 0);
	CHECK(ref->GetMember(0)->GetOffset()== 0);
	Foo foo;
	foo.Bar = 2;
	Delegate<int(int wew)> borz(&foo, &Foo::baz);
	Delegate<void(Base *wew)> borfz(&foo, &Foo::borf);

	borz.invoke(1);
	borfz.invoke(new Base);
}


TEST_CASE("Pause")
{
	int foo = 0;
}
#endif
