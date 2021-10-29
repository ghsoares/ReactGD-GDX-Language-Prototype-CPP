#ifndef T_GDXLEXER
#define T_GDXLEXER

#include "../lexer.h"

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
	bool T_GDBLOCK();
	bool T_FUNCTION_CALL();
	bool T_FUNCTION_CALL_ARGS();
};

#endif
