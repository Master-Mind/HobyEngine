#pragma once
#include <vector>
#include "Lexer.h"

class QCompile
{
public:
	QCompile();
	void Compile(const char *str);
private:
	Lexer _lex;
};
