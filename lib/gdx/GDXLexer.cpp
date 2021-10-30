#include "GDXLexer.h"

Token GDXLexer::getToken()
{
	return (
			gdxBlockToken() | importToken() | classNameToken());
}

TagToken GDXLexer::gdxBlockToken()
{
	return TagToken();
}

ImportToken GDXLexer::importToken()
{
	return ImportToken();
}

ClassNameToken GDXLexer::classNameToken()
{
	if (match("class_name"))
	{
		const Cursor start = getCursorStart(-1);
		expect(T_SYMBOL, "Expected name");
		const std::string className = getStr(-1);
		const Cursor end = getCursorEnd(-1);

		return ClassNameToken(
				start, end, className);
	}

	return ClassNameToken();
}

bool GDXLexer::T_GDX_BLOCK_START()
{
	return match("(") && match("<");
}

bool GDXLexer::T_GDX_BLOCK_END()
{
	return match("(") && match("<");
}

bool GDXLexer::T_VALUE()
{
	auto a = T_GDBLOCK;

	return (
			match((matchFunc)&T_GDBLOCK) ||
			match((matchFunc)&T_ACCESSOR) ||
			match((matchFunc)&T_FUNCTION_CALL) ||
			match((matchFunc)&T_LITERAL) ||
			match((matchFunc)&T_SYMBOL));
}

bool GDXLexer::T_SYMBOL()
{
	return match(std::regex("(_|[a-z]|[A-Z])(_|:|[a-z]|[A-Z]|[0-9])*"));
}

bool GDXLexer::T_INT()
{
	return (
			match(std::regex("[+-]?0x([0-9]|[a-f]|[A-F])+")) ||
			match(std::regex("[+-]?0b[01]+")) ||
			match(std::regex("[+-]?[0-9]+")));
}

bool GDXLexer::T_FLOAT()
{
	return (
			match(std::regex("[+-]?[0-9]+\.[0-9]*e[+-]?[0-9]+")) ||
			match(std::regex("[+-]?[0-9]+\.[0-9]*[fF]?")) ||
			match(std::regex("[+-]?[0-9]+")));
}

bool GDXLexer::T_STRING()
{
	if (match(std::regex("\"\"\"(.|\n)*\"\"\"")))
	{
		std::string str = getStr(-1);
		setStr(-1, str);
		return true;
	}

	return match(std::regex("\".*?\"|\'.*?\'"));
}

bool GDXLexer::T_ACCESSOR()
{
	return match((matchFunc)[this]()
							 { return (
										 T_ACCESSOR_ANY() &&
										 match(".") &&
										 expect((matchFunc)&T_ACCESSOR_ANY, "Expected value") &&
										 matchWhile([this]()
																{ return match(".") && expect((matchFunc)&T_ACCESSOR_ANY, "Expected value"); })); },
							 true);
}

bool GDXLexer::T_ACCESSOR_ANY()
{
	return (
			T_GDBLOCK() ||
			T_FUNCTION_CALL() ||
			T_LITERAL() ||
			T_SYMBOL());
}

bool GDXLexer::T_GDBLOCK()
{
	int mPos = matchStack.size();
	const bool foundBlock = match([this]()
																{ return matchScope("{", "}"); });
	if (foundBlock)
	{
		setStr(
				-1,
				fromSliced(
						getCursorStart(mPos).pos + 1,
						getCursorEnd(-1).pos - 1));
		return true;
	}

	return false;
}

bool GDXLexer::T_FUNCTION_CALL()
{
	return match([this]()
							 { return (
										 T_SYMBOL() &&
										 match("(") &&
										 T_FUNCTION_CALL_ARGS() &&
										 expect(")", "Expected \")\"")); });
}

bool GDXLexer::T_FUNCTION_CALL_ARGS()
{
	if (T_VALUE())
	{
		return matchWhile([this]()
											{ return (
														match(",") &&
														expect(T_VALUE, "Expected value")); });
	}
	return true;
}