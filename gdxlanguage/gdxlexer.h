#ifndef GDX_LEXER_H
#define GDX_LEXER_H

#include "../lexer/lexer.h"
#include <map>

struct ImportToken : public Token
{
public:
	std::string class_name;
	std::string relative_path;

	ImportToken(CursorRange *range,
				std::string class_name,
				std::string relative_path) : Token(range),
											 class_name(class_name),
											 relative_path(relative_path) {}
};

struct VariableToken : public Token
{
public:
	std::string name;
	std::string type;
	std::string value;

	VariableToken(
		CursorRange *range,
		std::string name,
		std::string type,
		std::string value) : Token(range),
							 name(name),
							 type(type),
							 value(value) {}
};

struct FunctionToken : public Token
{
public:
	std::string name;
	std::string return_type;
	std::vector<VariableToken *> args;

	FunctionToken(
		CursorRange *range,
		std::string name,
		std::string return_type,
		std::vector<VariableToken *> args) : Token(range), name(name), return_type(return_type), args(args) {}
};

struct TagToken : public Token
{
public:
	std::string type;
	std::string class_name;
	std::map<std::string, std::string> properties;

	TagToken(
		CursorRange *range,
		std::string type,
		std::string class_name,
		std::map<std::string, std::string> properties) : Token(range), type(type), class_name(class_name), properties(properties) {}
};

class GDXLexer : public Lexer
{
public:
	virtual Token *get_token();
	ImportToken *import();
	VariableToken *variable(bool require_prefix = true);
	FunctionToken *function();
	TagToken *tag();

private:
	std::map<std::string, std::string> tag_properties();
	void T_SYMBOL(bool prop = false);
	void T_LITERAL();
	void T_STRING(bool multiline = false);
	void T_FLOAT();
	void T_INT();
	void T_GDBLOCK();
	void T_FUNCTION();
};

#endif