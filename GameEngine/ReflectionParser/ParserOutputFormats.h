#pragma once
#include <filesystem>
#include <rapidxml.hpp>
#include <unordered_map>
#include <clang-c/Index.h>

#include "AST.h"
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

class ParserOutputFormat
{
public:
	ParserOutputFormat(const std::filesystem::path& filename);
	~ParserOutputFormat();
	CXCursorKind GetKind() const;
	void Format(std::string& output, const AST::Node& curnode, bool debugPrint = false) const;
private:
	void FormatInternal(std::string& output, const AST::Node& curnode, const rapidxml::xml_node<>* curxmlNode, bool debugPrint) const;
	bool EvaluateFormattingCondition(const AST::Node& curnode, const rapidxml::xml_attribute<>* condition) const;
	//returns null if the var is empty, throws if var isn't even on the given class (i.e. asking for a base_class but passing in a function as the node)
	std::string GetVarAsString(const AST::Node& curnode, const char* varName) const;
	const AST::Node *GetVarAsNode(const AST::Node& curnode, const char* varName, int optIndex = -1) const;
	bool IsVarAnArray(const AST::Node& curnode, const char* varName) const;
	void FormatVar(std::string& output, const AST::Node& curnode, const rapidxml::xml_node<>* curxmlNode, bool debugPrint) const;
	CXCursorKind kind = CXCursor_InvalidCode;
	char* buffer = nullptr;
	rapidxml::xml_node<>* root;
	rapidxml::xml_document<> doc;
};

class POFManager
{
public:
	static bool Init();
	static const ParserOutputFormat *GetFormatter(CXCursorKind kindToFormat);
	static const ParserOutputFormat* GetFormatter(std::string nameOfFormat);
private:
	static std::unordered_map<std::string, ParserOutputFormat*> formats;
	static std::unordered_map<CXCursorKind, ParserOutputFormat*> formatsByKind;
};
