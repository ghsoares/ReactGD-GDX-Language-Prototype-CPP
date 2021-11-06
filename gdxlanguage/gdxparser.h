/*
private args: ParserArgs;
    private source: string;
    private variables: VariableDeclarationToken[];
    private functions: FunctionDeclarationToken[];
    private off: number;
    private treeStack: TagParsed[];
    private treeStart: Cursor;
*/

#ifndef GDXPARSER_H
#define GDXPARSER_H

#include "gdxlexer.h"

class GDXParser
{
private:
	GDXLexer *lexer;

public:
	GDXParser(GDXLexer *lexer) : lexer(lexer) {}

	
};

#endif