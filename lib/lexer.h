#ifndef T_LEXER
#define T_LEXER

#include <iostream>
#include <vector>
#include <regex>
#include "cursor.h"

// Definition
struct LexerException : public std::exception
{
private:
	std::string msg;
	Cursor cursor;

public:
	explicit LexerException(std::string msg, Cursor cursor) : msg(msg), cursor(cursor)
	{
	}

	const char *what() const throw()
	{
		return this->msg.c_str();
	}
};

struct Token
{
public:
	Cursor start;
	Cursor end;
	Token(Cursor start, Cursor end) : start(start), end(end) {}
};

class Lexer
{
private:
	std::string input;

protected:
	Cursor cursor;
	std::vector<std::string> matchStack;
	std::vector<std::vector<Cursor>> cursorStack;

	std::string fromSliced(int from, int to = -1)
	{
		std::string s = "";
		if (to == -1)
			to = matchStack.size() - 1;

		for (int i = from; i <= to; i++)
		{
			s += this->matchStack[i];
		}

		return s;
	}

	std::string getStr(int pos);
	void setStr(int pos, std::string str);
	Cursor getCursorStart(int pos);
	Cursor getCursorEnd(int pos);

	bool match(std::string str);
	bool match(bool (*func)(), bool sliceRange = false);
	bool match(std::function<bool()> func, bool sliceRange = false);
	bool match(std::regex reg);

	template <typename T>
	bool matchUntil(T str);
	template <typename T>
	bool matchWhile(T str);
	template <typename T>
	bool matchScope(T strOpen, T strClose);
	template <typename T>
	bool expect(T str, std::string msg) throw(LexerException);

	virtual Token getToken();

public:
	Lexer(std::string input);
};

#endif