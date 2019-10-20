#define PARSERHOOKSTART(...) //used to find various different sections of the codegen file
#define PARSERHOOKEND(...) //used to find various different section of the codegen file
#include "MetaDB.h"
#include "MetaReference.h"
PARSERHOOKSTART("C:\Users\pholl\source\repos\GameEngine\ReflectionTest\TestClass.h");
#include "C:\Users\pholl\source\repos\GameEngine\ReflectionTest\TestClass.h"
using namespace TestSpace;
MetaData<Base>::MetaData(const char* typeName) : _typeName(typeName)
{
	Base *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, Base>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(Base).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<Base>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect(),MetaAttr::BindToLua());
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<Base>  DataBase("Base");
using namespace TestSpace;
MetaData<Dirived>::MetaData(const char* typeName) : _typeName(typeName)
{
	Dirived *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, Dirived>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(Dirived).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<Dirived>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect());
	_parentClass = MetaDB::getMeta<Base>();
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<Dirived>  DataDirived("Dirived");
using namespace TestSpace;
MetaData<Foo>::MetaData(const char* typeName) : _typeName(typeName)
{
	Foo *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, Foo>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(Foo).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<Foo>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::BindToLua());
	AddMember<__COUNTER__ - countStart>(nullptr, "Bar", &temp->Bar, MetaAttr::PlaceHolder());//0666+2
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<Foo>  DataFoo("Foo");
using namespace TestSpace;
MetaData<TestFoo>::MetaData(const char* typeName) : _typeName(typeName)
{
	TestFoo *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, TestFoo>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(TestFoo).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<TestFoo>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect());
	_parentClass = MetaDB::getMeta<TestInter>();
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<TestFoo>  DataTestFoo("TestFoo");
PARSERHOOKEND("C:\Users\pholl\source\repos\GameEngine\ReflectionTest\TestClass.h");
