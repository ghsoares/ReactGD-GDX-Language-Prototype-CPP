#include <iostream>
#include "lib/lexer.h"

int main()
{
	std::string input = "Something to test";
	input = "09123 123";

	Lexer lexer(input);


	return 0;
}