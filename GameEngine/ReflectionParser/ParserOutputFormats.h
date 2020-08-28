#pragma once

const char classReflStart[] =
"MetaData<%s>::MetaData(const char* typeName) : _typeName(typeName)\n"
"{\n"
"	%s *temp = nullptr;\n"
"	_isBase = false;\n"
"	const size_t countStart = __COUNTER__;\n";

const char classReflEnd[] = 
"	if(_properties.has<MetaAttr::BindToLua>())\n\
	{\n\
	}\n\
	MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this));\n\
}\n\
MetaData<%s>  Data%s(\"%s\");\n";
const char AddMember[] = 
"	AddMember<__COUNTER__ - countStart>(nullptr, \"%s\", &temp->%s, MetaAttr::PlaceHolder()";

const char constructorRefl[] =
"	if(std::is_default_constructible<T>::value)\n"
"		_defaultConstructor = [](){reutrn T()};";

const char destructorRefl[] =
"	if(std::is_default_destructible<T>::value)\n"
"		_defaultdestructor = [](){~T()};";

const char assignRefl[] =
"	if(std::is_default_destructible<T>::value)\n"
"		_defaultdestructor = [](){~T()};";