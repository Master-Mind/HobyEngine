#include "MetaDB.h"
#include "MetaEnum.h"
std::unordered_map<std::string, void *> *AllEnums::_enums;
void AllEnums::AddEnum(const std::string& name, void * e)
{
	if(!_enums)
	{
		_enums = new std::unordered_map<std::basic_string<char>, void*>();
	}
	_enums->insert({ name,e });
}

void* AllEnums::GetEnum(const std::string& name)
{
	return (*_enums)[name];
}
