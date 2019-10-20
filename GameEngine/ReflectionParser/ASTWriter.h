#pragma once
#include "AST.h"
#include <sstream>

class ASTWriter
{
public:
	ASTWriter(const AST &ast);
	//returns an array of class/global function/global variable reflections
	const std::string& write(const std::string &sourceFile = "");
	const std::string& GetSourceFile();
	~ASTWriter();
private:
	void write_internal(const AST::Node &curnode);
	const AST& ast_;
	std::string output_;
	std::string _sourceFile;
};

