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
		/*this->matchStack.push_back(ss); // Getting error if I include this line

		const Cursor startCursor = this->cursor;
		this->cursor.walkTimes(str.length());
		const Cursor endCursor = this->cursor;
		this->cursorStack.push_back({startCursor, endCursor});
		*/

		return true;
	}

	return false;
}
