#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

ParserInfo type()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "char") == 0 || strcmp(pi.tk.lx, "int") == 0 || strcmp(pi.tk.lx, "boolean") == 0 || pi.tk.tp == ID)
		;
	else
	{
		pi.er = illegalType;
		return pi;
	}
	return pi;
}

ParserInfo classVarDeclar()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
		;
	else
	{
		pi.er = classVarErr;
		return pi;
	}
	pi = type();
	pi.tk = GetNextToken();
	if (pi.tk.tp == ID)
		;
	else
	{
		pi.er = idExpected;
		return pi;
	}
	pi.tk = PeekNextToken();
	while (1)
	{
		if (strcmp(pi.tk.lx, ",") != 0)
			break;
		pi.tk = GetNextToken();
		pi.tk = GetNextToken();
		if (pi.tk.tp == ID)
			;
		else
		{
			pi.er = idExpected;
			return pi;
		}
		pi.tk = PeekNextToken();
	}
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, ";") == 0)
		;
	else
	{
		pi.er = semicolonExpected;
		return pi;
	}
	return pi;
}

ParserInfo subroutineDeclar()
{
	;
}

ParserInfo memberDeclar()
{
	printf("\n hello from memberdeclar");
	ParserInfo pi;
	pi.er = none;
	pi.tk = PeekNextToken();
	if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
		classVarDeclar();
	else if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "method") == 0 || strcmp(pi.tk.lx, "function") == 0)
		subroutineDeclar();
	else
		pi.er = memberDeclarErr;
	return pi;
}

ParserInfo classDeclar()
{
	printf("\n hello from classdeclar");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "class") == 0)
		;
	else
	{
		pi.er = classExpected;
		return pi;
	}
	pi.tk = GetNextToken();
	if (pi.tk.tp == ID)
		;
	else
	{
		pi.er = idExpected;
		return pi;
	}
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "{") == 0)
		;
	else
	{
		pi.er = openBraceExpected;
		return pi;
	}
	pi = memberDeclar();
	if (pi.er != none)
		return pi;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "}") == 0)
		;
	else
	{
		pi.er = closeBraceExpected;
		return pi;
	}
	return pi;
}

int InitParser(char *file_name)
{
	InitLexer(file_name);
	return 1;
}

ParserInfo Parse()
{
	ParserInfo pi;
	pi.er = none;
	pi = classDeclar();
	return pi;
}

int StopParser()
{
	StopLexer();
	return 1;
}

#ifndef TEST_PARSER
int main()
{
	InitParser("Square.jack");
	ParserInfo pi;
	pi = Parse();
	printf("\n token lexeme: %s and pi error: %d", pi.tk.lx, pi.er);
	if (pi.er == 0)
		printf("none");
	if (pi.er == 1)
		printf("lexerErr");
	if (pi.er == 2)
		printf("classExpected");
	if (pi.er == 3)
		printf("idExpected");
	if (pi.er == 4)
		printf("openBraceExpected");
	if (pi.er == 5)
		printf("closeBraceExpected");
	if (pi.er == 6)
		printf("memberDeclarErr");
	if (pi.er == 7)
		printf("classVarErr");
	if (pi.er == 8)
		printf("illegalType");
	if (pi.er == 9)
		printf("semicolonExpected");
	if (pi.er == 10)
		printf("subroutineDeclarErr");
	if (pi.er == 11)
		printf("openParenExpected");
	if (pi.er == 12)
		printf("closeParenExpected");
	if (pi.er == 13)
		printf("closeBracketExpected");
	if (pi.er == 14)
		printf("equalExpected");
	if (pi.er == 15)
		printf("syntaxError");
	printf("\n");
	return 1;
}
#endif
