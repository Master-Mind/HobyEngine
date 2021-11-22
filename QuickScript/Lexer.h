#pragma once
#include <vector>
#include <bitset>

class Lexer
{
public:
	Lexer();
	enum QTokType
	{
		Error,
		None,
		Type,
		Operator,
		VarName,
		EndLine
	};
	class Token
	{
	public:
		Token();
		Token(QTokType _type);
		const char *ToStr();
		QTokType type;
		size_t typeID;
	};


	std::vector<Token> LexString(const char *str);

private:

};
