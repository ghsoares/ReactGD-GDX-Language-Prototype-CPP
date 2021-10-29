#include "GDXLexer.h"

bool GDXLexer::T_GDX_BLOCK_START()
{
	return this->match("(") && this->match("<");
}

bool GDXLexer::T_GDX_BLOCK_END()
{
	return this->match("(") && this->match("<");
}

bool GDXLexer::T_VALUE()
{
	return (
			this->T_GDBLOCK() ||
			this->T_ACCESSOR() ||
			this->T_FUNCTION_CALL() ||
			this->T_LITERAL() ||
			this->T_SYMBOL());
}

bool GDXLexer::T_SYMBOL()
{
	return this->match(std::regex("(_|[a-z]|[A-Z])(_|:|[a-z]|[A-Z]|[0-9])*"));
}

bool GDXLexer::T_INT()
{
	return (
			this->match(std::regex("[+-]?0x([0-9]|[a-f]|[A-F])+")) ||
			this->match(std::regex("[+-]?0b[01]+")) ||
			this->match(std::regex("[+-]?[0-9]+")));
}

bool GDXLexer::T_FLOAT()
{
	return (
			this->match(std::regex("[+-]?[0-9]+\.[0-9]*e[+-]?[0-9]+")) ||
			this->match(std::regex("[+-]?[0-9]+\.[0-9]*[fF]?")) ||
			this->match(std::regex("[+-]?[0-9]+")));
}

bool GDXLexer::T_STRING()
{
	if (this->match(std::regex("\"\"\"(.|\n)*\"\"\"")))
	{
		std::string str = this->getStr(-1);
		this->setStr(-1, str);
		return true;
	}

	return this->match(std::regex("\".*?\"|\'.*?\'"));
}

bool GDXLexer::T_ACCESSOR()
{
	auto any = [this]() -> bool
	{
		return (
				this->T_GDBLOCK() ||
				this->T_FUNCTION_CALL() ||
				this->T_LITERAL() ||
				this->T_SYMBOL());
	};

	return this->match([this, any]() -> bool
										 { return (
													 any() &&
													 this->match(".") &&
													 this->expect(any, "Expected value") &&
													 this->matchWhile([this, any]() -> bool
																						{ return this->match(".") && this->expect(any, "Expected value"); })); },
										 true);
}

bool GDXLexer::T_GDBLOCK()
{
	int mPos = this->matchStack.size();
	const bool foundBlock = this->match([this]()
																			{ return matchScope("{", "}"); });
	if (foundBlock)
	{
		this->setStr(
				-1,
				this->fromSliced(
						this->getCursorStart(mPos).pos + 1,
						this->getCursorEnd(-1).pos - 1));
		return true;
	}

	return false;
}
