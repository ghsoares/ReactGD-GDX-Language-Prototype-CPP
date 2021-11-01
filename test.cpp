#include <iostream>
#include "lib/lexer.h"

int main()
{
	std::string input = "Something to test";

	Lexer lexer;
	lexer.set_source(&input);

	lexer.match("Something");
	if (lexer.found_match())
	{
		std::cout << lexer.get_str(-1) << std::endl;
	}
	else
	{
		std::cout << "no match" << std::endl;
	}

	return 0;
}