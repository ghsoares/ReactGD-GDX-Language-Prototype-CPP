#include "lexer.h"

Lexer::Lexer(std::string input) : cursor(input)
{
	input = input;
}

std::string Lexer::getStr(int pos)
{
	if (matchStack.size() == 0)
		return input;
	while (pos < 0)
		pos += matchStack.size();
	while (pos >= matchStack.size())
		pos -= matchStack.size();

	return matchStack[pos];
}

void Lexer::setStr(int pos, std::string str)
{
	if (matchStack.size() == 0)
		return;
	while (pos < 0)
		pos += matchStack.size();
	while (pos >= matchStack.size())
		pos -= matchStack.size();

	matchStack[pos] = str;
}

Cursor Lexer::getCursorStart(int pos)
{
	if (cursorStack.size() == 0)
		return Cursor(input);
	while (pos < 0)
		pos += cursorStack.size();
	while (pos >= cursorStack.size())
		pos -= cursorStack.size();

	return cursorStack[pos][0];
}

Cursor Lexer::getCursorEnd(int pos)
{
	if (cursorStack.size() == 0)
		return Cursor(input);
	while (pos < 0)
		pos += cursorStack.size();
	while (pos >= cursorStack.size())
		pos -= cursorStack.size();

	return cursorStack[pos][1];
}

bool Lexer::match(std::string str)
{
	cursor.skipIgnore();

	const std::string ss = input.substr(cursor.pos, str.length());

	if (ss == str)
	{
		matchStack.push_back(ss);
		const Cursor startCursor = cursor;
		cursor.walkTimes(str.length());
		const Cursor endCursor = cursor;
		cursorStack.push_back({startCursor, endCursor});

		return true;
	}

	return false;
}

bool Lexer::match(std::regex reg)
{
	cursor.skipIgnore();

	const std::string ss = input.substr(cursor.pos);
	std::smatch match;

	if (std::regex_search(ss, match, reg))
	{
		if (match.position() == 0)
		{
			matchStack.push_back(match.str());
			const Cursor startCursor = cursor;
			cursor.walkTimes(match.length());
			const Cursor endCursor = cursor;
			cursorStack.push_back({startCursor, endCursor});
			return true;
		}
	}

	return false;
}

template <typename T>
bool Lexer::matchUntil(T str)
{
	while (!cursor.eof)
	{
		if (match(str))
		{
			return true;
		}
		else
			cursor.walk();
	}
	return false;
}

template <typename T>
bool Lexer::matchWhile(T str)
{
	while (!cursor.eof)
	{
		if (!match(str))
			break;
	}
	return true;
}

template <typename T>
bool Lexer::matchScope(T strOpen, T strClose)
{
	if (!match(strOpen))
		return false;
	int lvl = 0;

	while (!cursor.eof)
	{
		if (match(strOpen))
			lvl++;
		else if (match(strClose))
		{
			lvl--;
			if (lvl < 0)
				return true;
		}
		else
			cursor.walk();
	}

	return false;
}

template <typename T>
bool Lexer::expect(T str, std::string msg)
{
	const Cursor tCursor = cursor;

	if (match(str))
		return true;

	throw(LexerException(msg, tCursor));
}

std::vector<Token> Lexer::tokenize()
{
	std::vector<Token> tokens;

	while (!cursor.eof)
	{
		const Token token = getToken();
		if (!token.isNull())
		{
			tokens.push_back(token);
		}
		else
			cursor.walk();
	}

	return tokens;
}

Token& Token::operator|(const Token& b) {
	if (!isNull()) {
		return *this;
	} else {
		return (Token&)b;
	}
}