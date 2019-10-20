#include "Lexer.h"
#include <typeinfo>
#include "ErrorCodes.h"
#include <sstream>
#include <strstream>
#include <FlexLexer.h>

Lexer::Lexer()
{
}

Lexer::Token::Token() : type(None), typeID(0)
{

}

Lexer::Token::Token(QTokType _type) : type(_type)
{
}

const char* Lexer::Token::ToStr()
{
	static const char *names[] = { "(Error)",
		"(None)",
		"(Type)",
		"(Operator)",
		"(VarName)",
		"(EndLine)" };



	return names[type];
}

std::vector<Lexer::Token> Lexer::LexString(const char* str)
{
	std::vector<Token> ret;
	std::strstreambuf stream("", 1);
	std::istream instream(&stream);
	yyFlexLexer _lexer(&instream);

	_lexer.yylex();

	return ret;
}