#ifndef CURSOR_H
#define CURSOR_H

#include <iostream>
#include <sstream>

struct Cursor
{
private:
	std::string *input;
	int input_length;
	bool indenting;

	char get_char(int p);

public:
	char character;
	int pos;
	int line;
	int column;
	int indent;
	bool eof;
	bool line_break;

	Cursor(std::string *i);

	void walk();
	void walk_times(int times);
	void move(int toPos);
	void skip_ignore();
};

struct CursorRange
{
public:
	Cursor *start;
	Cursor *end;

	CursorRange(Cursor *start, Cursor *end) : start(start), end(end) {}
	const int length() {
		return (end->pos - start->pos) + 1;
	}
};

#endif