#include <iostream>
#include "lib/gdx/GDXLexer.h"

int main()
{
	std::string input = "extends ReactGDComponent\nclass_name Test";
	input = "09123 123";

	GDXLexer lexer(input);
	auto tokens = lexer.tokenize();

	for (int i = 0; i < tokens.size(); i++) {
		ClassNameToken* classNameToken = static_cast<ClassNameToken*>(&tokens[i]);

		if (classNameToken != 0) {
			std::cout << classNameToken->className << std::endl;
		}
	}

	return 0;
}