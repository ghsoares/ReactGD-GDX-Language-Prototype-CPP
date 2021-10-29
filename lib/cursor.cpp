#include <sstream>
#include "cursor.h"

Cursor::Cursor(std::string i)
{
	this->input = i;
	this->inputLength = i.length();
	this->character = i.at(0);
	this->pos = 0;
	this->line = 0;
	this->column = 0;
	this->eof = false;
	this->lineBreak = false;
}

char Cursor::getChar(int pos)
{
	if (pos < 0 || pos >= this->inputLength)
	{
		return EOF;
	}
	return this->input.at(pos);
}

void Cursor::walk(bool back)
{
	if (back)
	{
		this->pos--;
		this->column--;
		if (this->lineBreak)
		{
			this->line--;
			this->column = 0;
			for (int i = this->pos - 1; i >= 0; i--)
			{
				if (this->getChar(i) == '\n')
					break;
				this->column++;
			}
		}
	}
	else
	{
		this->pos++;
		this->column++;
		if (this->lineBreak)
		{
			this->line++;
			this->column = 0;
		}
	}
	this->character = this->getChar(this->pos);
	this->eof = this->character == EOF;
	this->lineBreak = this->character == '\n';
}

void Cursor::walkTimes(int times, bool back)
{
	for (int i = 0; i < times; i++)
	{
		this->walk(back);
	}
}

void Cursor::move(int pos)
{
	if (pos < 0)
		pos = 0;
	if (pos > this->inputLength - 1)
		pos = this->inputLength - 1;

	this->pos = 0;
	this->character = this->input.at(0);
	this->line = 0;
	this->column = 0;
	this->eof = false;
	this->lineBreak = this->character == '\n';

	while (this->pos < pos)
		this->walk();
}

void Cursor::skipIgnore()
{
	while (this->character == ' ' ||
				 this->character == '\n' ||
				 this->character == '\t')
		this->walk();

	if (this->character == '#')
	{
		while (!this->eof && this->character != '\n')
			this->walk();
	}

	while (this->character == ' ' ||
				 this->character == '\n' ||
				 this->character == '\t')
		this->walk();
}

std::string Cursor::toString()
{
	std::stringstream ss("");
	ss << "(P:" << this->pos;
	ss << " L:" << this->line;
	ss << " C:" << this->column;
	ss << " \"" << this->character << "\")";

	return ss.str();
}
