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
		return msg.c_str();
	}
};

struct Token
{
private:
	bool nullToken;

public:
	Cursor start;
	Cursor end;

	Token() : nullToken(true) {}
	Token(Cursor start, Cursor end) : nullToken(false), start(start), end(end) {}

	bool isNull() const { return nullToken; }

	Token& operator |(const Token& b);
};

typedef std::function<bool()> matchFunc;

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
			s += matchStack[i];
		}

		return s;
	}

	std::string getStr(int pos);
	void setStr(int pos, std::string str);
	Cursor getCursorStart(int pos);
	Cursor getCursorEnd(int pos);

	bool match(std::string str);
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

	std::vector<Token> tokenize();
};

#endif