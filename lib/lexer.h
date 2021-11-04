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
	Cursor *cursor;

public:
	explicit LexerException(std::string msg, Cursor *cursor) : msg(msg), cursor(cursor)
	{
	}

	const char *what() const throw()
	{
		return msg.c_str();
	}
};

struct MatchScope
{
public:
	Cursor *cursorStart;
	int stackPos;
	bool prevMatch;
	bool matching;
	bool notMatch;
	std::string expectMsg;

	MatchScope(
			Cursor *cursorStart,
			int stackPos) : cursorStart(cursorStart)
	{
		stackPos = stackPos;
		prevMatch = false;
		matching = true;
		notMatch = false;
		expectMsg = "";
	}
};

class Lexer
{
private:
	std::string *source;

public:
	MatchScope *curr_scope;
	Cursor *cursor;
	std::vector<MatchScope *> scope_stack;
	std::vector<std::string *> string_stack;
	std::vector<CursorRange *> range_stack;

	std::string *get_str(int pos);
	void set_str(int pos, std::string *str);
	CursorRange *get_range(int pos);

	Lexer *match_start();
	Lexer *match_end(bool resetCursor = true);
	bool found_match();
	Lexer *m_and();
	Lexer *m_or();
	Lexer *m_not();

	Lexer *match(std::string str);
	Lexer *match(std::regex reg);

	Lexer *expect_next(std::string msg);
	Lexer *expect_prev(std::string msg);

//public:
	void reset();
	void set_source(std::string *source);
	std::string *get_source();
};

#endif