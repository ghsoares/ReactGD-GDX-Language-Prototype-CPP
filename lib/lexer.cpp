#include "lexer.h"

Lexer::Lexer(std::string input) : cursor(input)
{
	this->input = input;
}

std::string Lexer::getStr(int pos)
{
	if (this->matchStack.size() == 0)
		return this->input;
	while (pos < 0)
		pos += this->matchStack.size();
	while (pos >= this->matchStack.size())
		pos -= this->matchStack.size();

	return this->matchStack[pos];
}

void Lexer::setStr(int pos, std::string str)
{
	if (this->matchStack.size() == 0)
		return;
	while (pos < 0)
		pos += this->matchStack.size();
	while (pos >= this->matchStack.size())
		pos -= this->matchStack.size();

	this->matchStack[pos] = str;
}

Cursor Lexer::getCursorStart(int pos)
{
	if (this->cursorStack.size() == 0)
		return Cursor(this->input);
	while (pos < 0)
		pos += this->cursorStack.size();
	while (pos >= this->cursorStack.size())
		pos -= this->cursorStack.size();

	return this->cursorStack[pos][0];
}

Cursor Lexer::getCursorEnd(int pos)
{
	if (this->cursorStack.size() == 0)
		return Cursor(this->input);
	while (pos < 0)
		pos += this->cursorStack.size();
	while (pos >= this->cursorStack.size())
		pos -= this->cursorStack.size();

	return this->cursorStack[pos][1];
}

bool Lexer::match(std::string str)
{
	this->cursor.skipIgnore();

	const std::string ss = this->input.substr(this->cursor.pos, str.length());

	if (ss == str)
	{
		this->matchStack.push_back(ss);
		const Cursor startCursor = this->cursor;
		this->cursor.walkTimes(str.length());
		const Cursor endCursor = this->cursor;
		this->cursorStack.push_back({startCursor, endCursor});

		return true;
	}

	return false;
}

bool Lexer::match(bool (*func)(), bool sliceRange)
{
	this->cursor.skipIgnore();

	const Cursor tCursor = this->cursor;
	const int mPos = this->matchStack.size();

	if (func())
	{
		if (sliceRange)
		{
			const std::string str = this->input.substr(
					tCursor.pos,
					this->getCursorEnd(-1).pos - tCursor.pos + 1);
			this->matchStack.push_back(str);
		}
		else
		{
			const std::string str = this->fromSliced(mPos);
			this->matchStack.push_back(str);
		}
		this->cursorStack.push_back({tCursor, this->cursor});
		return true;
	}
	else
	{
		for (int i = this->cursorStack.size() - 1; i >= mPos; i--)
		{
			this->matchStack.pop_back();
			this->cursorStack.pop_back();
		}
		this->cursor.move(tCursor.pos);
	}
	return false;
}

bool Lexer::match(std::function<bool()> func, bool sliceRange)
{
	this->cursor.skipIgnore();

	const Cursor tCursor = this->cursor;
	const int mPos = this->matchStack.size();

	if (func())
	{
		if (sliceRange)
		{
			const std::string str = this->input.substr(
					tCursor.pos,
					this->getCursorEnd(-1).pos - tCursor.pos + 1);
			this->matchStack.push_back(str);
		}
		else
		{
			const std::string str = this->fromSliced(mPos);
			this->matchStack.push_back(str);
		}
		this->cursorStack.push_back({tCursor, this->cursor});
		return true;
	}
	else
	{
		for (int i = this->cursorStack.size() - 1; i >= mPos; i--)
		{
			this->matchStack.pop_back();
			this->cursorStack.pop_back();
		}
		this->cursor.move(tCursor.pos);
	}
	return false;
}

bool Lexer::match(std::regex reg)
{
	this->cursor.skipIgnore();

	const std::string ss = this->input.substr(this->cursor.pos);
	std::smatch match;

	if (std::regex_search(ss, match, reg))
	{
		if (match.position() == 0)
		{
			this->matchStack.push_back(match.str());
			const Cursor startCursor = this->cursor;
			this->cursor.walkTimes(match.length());
			const Cursor endCursor = this->cursor;
			this->cursorStack.push_back({startCursor, endCursor});
			return true;
		}
	}

	return false;
}

template <typename T>
bool Lexer::matchUntil(T str)
{
	while (!this->cursor.eof)
	{
		if (this->match(str))
		{
			return true;
		}
		else
			this->cursor.walk();
	}
	return false;
}

template <typename T>
bool Lexer::matchWhile(T str)
{
	while (!this->cursor.eof)
	{
		if (!this->match(str))
			break;
	}
	return true;
}

template <typename T>
bool Lexer::matchScope(T strOpen, T strClose)
{
	if (!this->match(strOpen))
		return false;
	int lvl = 0;

	while (!this->cursor.eof)
	{
		if (this->match(strOpen))
			lvl++;
		else if (this->match(strClose))
		{
			lvl--;
			if (lvl < 0)
				return true;
		}
		else
			this->cursor.walk();
	}

	return false;
}

template <typename T>
bool Lexer::expect(T str, std::string msg) throw(LexerException) {
	const Cursor tCursor = this->cursor;

	if (this->match(str)) return true;

	throw(LexerException(msg, tCursor));
}
