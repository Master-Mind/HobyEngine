#define PARSERHOOKSTART(...) //used to find various different sections of the codegen file
#define PARSERHOOKEND(...) //used to find various different section of the codegen file
#include "MetaDB.h"
#include "MetaReference.h"
PARSERHOOKSTART("");
PARSERHOOKEND("");
PARSERHOOKSTART("C:\Users\pholl\source\repos\GameEngine\GameEngine\Core\CoreComps\Transform.h");
#include "C:\Users\pholl\source\repos\GameEngine\GameEngine\Core\CoreComps\Transform.h"
MetaData<Transform>::MetaData(const char* typeName) : _typeName(typeName)
{
	Transform *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, Transform>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(Transform).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<Transform>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect(),MetaAttr::BindToLua());
	AddMember<__COUNTER__ - countStart>(nullptr, "Translation", &temp->Translation, MetaAttr::PlaceHolder());//
	AddMember<__COUNTER__ - countStart>(nullptr, "Rotation", &temp->Rotation, MetaAttr::PlaceHolder());//
	AddMember<__COUNTER__ - countStart>(nullptr, "Scale", &temp->Scale, MetaAttr::PlaceHolder());//
	AddMember<__COUNTER__ - countStart>(nullptr, "TransformMatrix", &temp->TransformMatrix, MetaAttr::PlaceHolder());//
	_parentClass = MetaDB::getMeta<Component>();
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<Transform>  DataTransform("Transform");
PARSERHOOKEND("C:\Users\pholl\source\repos\GameEngine\GameEngine\Core\CoreComps\Transform.h");
PARSERHOOKSTART("C:\Users\pholl\source\repos\GameEngine\GameEngine\GameStructure\GameObject.h");
#include "C:\Users\pholl\source\repos\GameEngine\GameEngine\GameStructure\GameObject.h"
MetaData<GameObject>::MetaData(const char* typeName) : _typeName(typeName)
{
	GameObject *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, GameObject>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(GameObject).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<GameObject>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect(),MetaAttr::BindToLua());
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<GameObject>  DataGameObject("GameObject");
PARSERHOOKEND("C:\Users\pholl\source\repos\GameEngine\GameEngine\GameStructure\GameObject.h");
PARSERHOOKSTART("C:\Users\pholl\source\repos\GameEngine\Debugger\Testing\TestInter.h");
#include "C:\Users\pholl\source\repos\GameEngine\Debugger\Testing\TestInter.h"
MetaData<TestInter>::MetaData(const char* typeName) : _typeName(typeName)
{
	TestInter *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, TestInter>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(TestInter).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<TestInter>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect());
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<TestInter>  DataTestInter("TestInter");
PARSERHOOKEND("C:\Users\pholl\source\repos\GameEngine\Debugger\Testing\TestInter.h");
PARSERHOOKSTART("C:\Users\pholl\source\repos\GameEngine\Utilities\Testing\StringTestBattery.h");
#include "C:\Users\pholl\source\repos\GameEngine\Utilities\Testing\StringTestBattery.h"
MetaData<StringTestBattery>::MetaData(const char* typeName) : _typeName(typeName)
{
	StringTestBattery *temp = nullptr;
	_isBase = false;
	if constexpr(std::is_base_of<Component, StringTestBattery>::value)
	{
		ComponentFactory::GetAllocFuncs().insert({typeid(StringTestBattery).hash_code(),  reinterpret_cast<void*(*)()>(&TypeAllocator<StringTestBattery>::Allocate)});
	}
	const size_t countStart = __COUNTER__;
	_properties.AddAttributes(MetaAttr::Reflect());
	_parentClass = MetaDB::getMeta<TestInter>();
	if(_properties.has<MetaAttr::BindToLua>())
	{
	}
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));
}
MetaData<StringTestBattery>  DataStringTestBattery("StringTestBattery");
PARSERHOOKEND("C:\Users\pholl\source\repos\GameEngine\Utilities\Testing\StringTestBattery.h");
