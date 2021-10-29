#ifndef T_LEXER
#define T_LEXER

#include <iostream>
#include <vector>
#include "cursor.h"

class Lexer
{
private:
	std::string input;

protected:
	Cursor cursor;
	std::vector<std::string> matchStack;
	std::vector<std::vector<Cursor>> cursorStack;

public:
	Lexer(std::string input);

	std::string getStr(int pos);
	void setStr(int pos, std::string str);
	Cursor getCursorStart(int pos);
	Cursor getCursorEnd(int pos);

	bool match(std::string str);
};

#endif