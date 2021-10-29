#include <iostream>
#include "lib/lexer.h"

bool matchSomething(Lexer *lexer) {
	return lexer->match("Something") && lexer->match("to") && lexer->match("test");
}

int main()
{
	std::string input = "Something to test";
	input = "09123 123";

	Lexer lexer(input);

	std::regex reg("[+-]?[0-9]+");

	if (lexer.match(reg)) {
		std::cout << "Matched!" << std::endl;
		std::cout << lexer.getStr(-1) << std::endl;
	} else {
		std::cout << "Not matched!" << std::endl;
	}

	if (lexer.match(reg)) {
		std::cout << "Matched!" << std::endl;
		std::cout << lexer.getStr(-1) << std::endl;
	} else {
		std::cout << "Not matched!" << std::endl;
	}

	return 0;
}