#include "QCompile.h"


void QCompile::Compile(const char* str)
{
	_lex.LexString(str);
}
QCompile::QCompile()
{
}

