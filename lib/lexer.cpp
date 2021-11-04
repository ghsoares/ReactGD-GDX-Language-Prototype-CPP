#include "lexer.h"

void Lexer::reset()
{
	cursor = new Cursor(source);
	scope_stack.clear();
	string_stack.clear();
	range_stack.clear();
}

void Lexer::set_source(std::string *source)
{
	source = source;
	reset();
}

std::string *Lexer::get_source()
{
	return source;
}

std::string *Lexer::get_str(int pos)
{
	if (string_stack.size() == 0)
		return source;
	while (pos < 0)
	{
		pos += string_stack.size();
	}
	while (pos >= string_stack.size())
	{
		pos -= string_stack.size();
	}
	return string_stack[pos];
}

//void set_str(int pos, std::string *str);

void Lexer::set_str(int pos, std::string *str)
{
	if (string_stack.size() == 0)
		return;
	while (pos < 0)
	{
		pos += string_stack.size();
	}
	while (pos >= string_stack.size())
	{
		pos -= string_stack.size();
	}

	string_stack[pos] = str;
}

CursorRange *Lexer::get_range(int pos)
{
	if (range_stack.size() == 0)
	{
		return new CursorRange(new Cursor(source), new Cursor(source));
	}
	while (pos < 0)
	{
		pos += string_stack.size();
	}
	while (pos >= string_stack.size())
	{
		pos -= string_stack.size();
	}
	return range_stack[pos];
}

Lexer *Lexer::match_start()
{
	if (curr_scope)
	{
		scope_stack.push_back(curr_scope);
	}
	cursor->skipIgnore();

	curr_scope = new MatchScope(new Cursor(*cursor), string_stack.size());

	return this;
}

Lexer *Lexer::match_end(bool returnCursor)
{
	if (curr_scope->prevMatch)
	{
		Cursor *cursorStart = curr_scope->cursorStart;
		Cursor *cursorEnd = new Cursor(
				*get_range(-1)->start);
		
		std::string str = source->substr(
				cursorStart->pos,
				cursorEnd->pos - cursorStart->pos + 1);
		if (str != *string_stack[string_stack.size() - 1])
		{

			string_stack.push_back(&str);
			range_stack.push_back(new CursorRange(cursorStart, cursorEnd));
		}
	}
	else
	{
		string_stack = std::vector<std::string *>(
				string_stack.begin(),
				string_stack.begin() + curr_scope->stackPos);
		range_stack = std::vector<CursorRange *>(
				range_stack.begin(),
				range_stack.begin() + curr_scope->stackPos);
		if (returnCursor)
		{
			cursor->move(curr_scope->cursorStart->pos);
		}
	}
	curr_scope = scope_stack[scope_stack.size()];
	scope_stack.pop_back();

	return this;
}

bool Lexer::found_match()
{
	return curr_scope->prevMatch;
}

Lexer *Lexer::m_and()
{
	if (!curr_scope->matching)
		return this;

	if (!curr_scope->prevMatch)
	{
		curr_scope->matching = false;
	}

	return this;
}

Lexer *Lexer::m_or()
{
	if (!curr_scope->matching)
		return this;

	if (curr_scope->prevMatch)
	{
		curr_scope->matching = false;
	}

	return this;
}

Lexer *Lexer::m_not()
{
	if (!curr_scope->matching)
		return this;

	curr_scope->notMatch = true;

	return this;
}

Lexer *Lexer::match(std::string str)
{
	if (!curr_scope->matching)
		return this;

	cursor->skipIgnore();

	Cursor *tCursor = new Cursor(*cursor);

	std::string sliced = source->substr(
			cursor->pos,
			cursor->pos + str.size());
	bool matched = sliced == str;

	if (curr_scope->notMatch)
	{
		matched = !matched;
	}
	else
	{
		if (matched)
		{
			string_stack.push_back(&str);
			Cursor *startCursor = new Cursor(*cursor);
			cursor->walkTimes(str.size() - 1);
			Cursor *endCursor = new Cursor(*cursor);
			range_stack.push_back(new CursorRange(startCursor, endCursor));
			cursor->walkTimes(1);
		}
	}

	if (!matched && curr_scope->expectMsg != "")
	{
		throw LexerException(curr_scope->expectMsg, tCursor);
	}

	curr_scope->notMatch = false;
	curr_scope->prevMatch = matched;
	curr_scope->expectMsg = "";

	return this;
}

Lexer *Lexer::match(std::regex reg)
{
	if (!curr_scope->matching)
		return this;

	cursor->skipIgnore();

	Cursor *tCursor = new Cursor(*cursor);

	const std::string ss = source->substr(cursor->pos);
	std::smatch match;
	bool matched = std::regex_search(ss, match, reg) && match.position() == 0;

	if (curr_scope->notMatch)
	{
		matched = !matched;
	}
	else
	{
		if (matched)
		{
			std::string match_str = match.str();
			string_stack.push_back(&match_str);
			Cursor *startCursor = new Cursor(*cursor);
			cursor->walkTimes(match.length() - 1);
			Cursor *endCursor = new Cursor(*cursor);
			range_stack.push_back(new CursorRange(startCursor, endCursor));
			cursor->walkTimes(1);
		}
	}

	if (!matched && curr_scope->expectMsg != "")
	{
		throw LexerException(curr_scope->expectMsg, tCursor);
	}

	curr_scope->notMatch = false;
	curr_scope->prevMatch = matched;
	curr_scope->expectMsg = "";

	return this;
}

Lexer *Lexer::expect_next(std::string msg)
{
	if (!this->curr_scope->matching)
		return this;

	this->curr_scope->expectMsg = msg;

	return this;
}

Lexer *Lexer::expect_prev(std::string msg)
{
	if (!this->curr_scope->matching)
		return this;

	if (!this->curr_scope->prevMatch)
	{
		throw LexerException(
				msg, get_range(-1)->start);
	}

	return this;
}
