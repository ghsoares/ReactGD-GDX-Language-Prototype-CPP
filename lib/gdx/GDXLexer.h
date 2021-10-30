#ifndef T_GDXLEXER
#define T_GDXLEXER

#include "../lexer.h"
#include <map>

struct TagToken : public Token
{
public:
	std::string id;
	std::string className;
	std::map<std::string, std::string> properties;
	std::vector<TagToken> children;

	TagToken(): Token() {}

	TagToken(
			Cursor start, Cursor end,
			std::string id, std::string className,
			std::map<std::string, std::string> properties,
			std::vector<TagToken> children) : Token(start, end), id(id), className(className),
																				properties(properties), children(children)
	{
	}
};

struct ClassNameToken : public Token
{
public:
	std::string className;

	ClassNameToken(): Token() {}

	ClassNameToken(
			Cursor start, Cursor end,
			std::string className) : Token(start, end), className(className)
	{
	}
};

struct ImportToken : public Token
{
public:
	std::string className;
	std::string relativePath;

	ImportToken(): Token() {}

	ImportToken(
			Cursor start, Cursor end,
			std::string className,
			std::string relativePath) : Token(start, end), className(className), relativePath(relativePath)
	{
	}
};

class GDXLexer : public Lexer
{
protected:
	Token getToken() override;

private:
	bool T_GDX_BLOCK_START();
	bool T_GDX_BLOCK_END();
	bool T_VALUE();
	bool T_SYMBOL();
	bool T_LITERAL();
	bool T_INT();
	bool T_FLOAT();
	bool T_STRING();
	bool T_ACCESSOR();
	bool T_ACCESSOR_ANY();
	bool T_GDBLOCK();
	bool T_FUNCTION_CALL();
	bool T_FUNCTION_CALL_ARGS();

	TagToken gdxBlockToken();
	ImportToken importToken();
	ClassNameToken classNameToken();

public:
	GDXLexer(std::string input) : Lexer(input) {}
};

#endif
