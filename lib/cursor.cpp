#include <sstream>
#include "cursor.h"

Cursor::Cursor(std::string *i)
{
	input = i;
	inputLength = i->size();
	character = i->at(0);
	pos = 0;
	line = 0;
	column = 0;
	eof = false;
	lineBreak = false;
}

char Cursor::getChar(int pos)
{
	if (pos < 0 || pos >= inputLength)
	{
		return EOF;
	}
	return input->at(pos);
}

void Cursor::walk(bool back)
{
	if (back)
	{
		pos--;
		column--;
		if (lineBreak)
		{
			line--;
			column = 0;
			for (int i = pos - 1; i >= 0; i--)
			{
				if (getChar(i) == '\n')
					break;
				column++;
			}
		}
	}
	else
	{
		pos++;
		column++;
		if (lineBreak)
		{
			line++;
			column = 0;
		}
	}
	character = getChar(pos);
	eof = character == EOF;
	lineBreak = character == '\n';
}

void Cursor::walkTimes(int times, bool back)
{
	for (int i = 0; i < times; i++)
	{
		walk(back);
	}
}

void Cursor::move(int pos)
{
	if (pos < 0)
		pos = 0;
	if (pos > inputLength - 1)
		pos = inputLength - 1;

	pos = 0;
	character = input->at(0);
	line = 0;
	column = 0;
	eof = false;
	lineBreak = character == '\n';

	while (pos < pos)
		walk();
}

void Cursor::skipIgnore()
{
	while (character == ' ' ||
				 character == '\n' ||
				 character == '\t')
		walk();

	if (character == '#')
	{
		while (!eof && character != '\n')
			walk();
	}

	while (character == ' ' ||
				 character == '\n' ||
				 character == '\t')
		walk();
}

std::string Cursor::toString()
{
	std::stringstream ss("");
	ss << "(P:" << pos;
	ss << " L:" << line;
	ss << " C:" << column;
	ss << " \"" << character << "\")";

	return ss.str();
}
