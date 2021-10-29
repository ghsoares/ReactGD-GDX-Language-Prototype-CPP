#ifndef T_CURSOR
#define T_CURSOR

#include <iostream>

struct Cursor
{
private:
	std::string input;
	int inputLength;

	char getChar(int p);

public:
	char character;
	int pos;
	int line;
	int column;
	bool eof;
	bool lineBreak;

	Cursor(std::string i);

	void walk(bool back = false);
	void walkTimes(int times, bool back = false);
	void move(int toPos);
	void skipIgnore();
	std::string toString();
};

#endif