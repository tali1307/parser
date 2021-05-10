#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

ParserInfo subroutineBody();
ParserInfo paramList();
ParserInfo statement();
ParserInfo varDeclarStatement();
ParserInfo letStatement();
ParserInfo ifStatement();
ParserInfo whileStatement();
ParserInfo doStatement();
ParserInfo returnStatement();
ParserInfo expression();
ParserInfo subroutineCall();

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
	printf("\n hello from subroutinedeclar");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "method") == 0 || strcmp(pi.tk.lx, "function") == 0)
		;
	else
	{
		pi.er = subroutineDeclarErr;
		return pi;
	}
	pi.tk = PeekNextToken();
	if (strcmp(pi.tk.lx, "char") == 0 || strcmp(pi.tk.lx, "int") == 0 || strcmp(pi.tk.lx, "boolean") == 0 || pi.tk.tp == ID)
		type();
	else if (strcmp(pi.tk.lx, "void") == 0)
		;
	else
	{
		pi.er = syntaxError;
		return pi;
	}
	pi.tk = GetNextToken();
	pi.tk = GetNextToken();
	if (pi.tk.tp == ID)
		;
	else
	{
		pi.er = idExpected;
		return pi;
	}
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "(") == 0)
		;
	else
	{
		pi.er = openParenExpected;
		return pi;
	}
	pi = paramList();
	if (pi.er != none)
		return pi;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, ")") == 0)
		;
	else
	{
		pi.er = closeParenExpected;
		return pi;
	}
	pi = subroutineBody();
	return pi;
}

ParserInfo paramList()
{
	printf("\n hello from paramList");
	ParserInfo pi;
	pi.er = none;
	pi.tk = PeekNextToken();
	if (strcmp(pi.tk.lx, "char") == 0 || strcmp(pi.tk.lx, "int") == 0 || strcmp(pi.tk.lx, "boolean") == 0 || pi.tk.tp == ID)
	{
		pi = type();
		if (pi.er != none)
			return pi;
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
			pi = type();
			if (pi.er != none)
				return pi;
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
		return pi;
	}
	else if (strcmp(pi.tk.lx, ")") == 0)
		;
	else
	{
		pi.er = syntaxError;
		return pi;
	}
	return pi;
}

ParserInfo subroutineBody()
{
	printf("\n hello from subroutineBody");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "{") == 0)
		;
	else
	{
		pi.er = openBraceExpected;
		return pi;
	}
	while (1)
	{
		pi = statement(); // multiple recurrences??
		if (pi.er != none)
			break;
		if (strcmp(PeekNextToken().lx, "}") == 0)
			break;
	}
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

ParserInfo statement()
{
	printf("\n hello from statement");
	ParserInfo pi;
	pi.er = none;
	pi.tk = PeekNextToken();
	if (strcmp(pi.tk.lx, "var") == 0)
		pi = varDeclarStatement();
	else if (strcmp(pi.tk.lx, "let") == 0)
		pi = letStatement();
	else if (strcmp(pi.tk.lx, "if") == 0)
		pi = ifStatement();
	else if (strcmp(pi.tk.lx, "while") == 0)
		pi = whileStatement();
	else if (strcmp(pi.tk.lx, "do") == 0)
		pi = doStatement();
	else if (strcmp(pi.tk.lx, "return") == 0)
		pi = returnStatement();
	else
	{
		pi.er = syntaxError;
		return pi;
	}
	return pi;
}

ParserInfo varDeclarStatement()
{
	printf("\n hello from varDeclarStatement");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	pi = type();
	if (pi.er != none)
		return pi;
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

ParserInfo letStatement()
{
	printf("\n hello from letStatement");
	ParserInfo pi;
	pi.er = none;
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
	while (1)
	{
		if (strcmp(pi.tk.lx, "[") != 0)
			break;
		pi = expression();
		if (pi.er != none)
			return pi;
		pi.tk = GetNextToken();
		if (strcmp(pi.tk.lx, "]") == 0)
			;
		else
		{
			pi.er = closeBraceExpected;
			return pi;
		}
		break;
	}
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "=") == 0)
		;
	else
	{
		pi.er = equalExpected;
		return pi;
	}
	pi = expression();
	if (pi.er != none)
		return pi;
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

ParserInfo ifStatement()
{
	printf("\n hello from ifStatement");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "(") == 0)
		;
	else
	{
		pi.er = openParenExpected;
		return pi;
	}
	pi = expression();
	if (pi.er != none)
		return pi;
	if (strcmp(pi.tk.lx, ")") == 0)
		;
	else
	{
		pi.er = closeParenExpected;
		return pi;
	}
	pi = subroutineBody();
	if (pi.er != none)
		return pi;
	pi.tk = PeekNextToken();
	while (1)
	{
		if (strcmp(pi.tk.lx, "else") != 0)
			break;
		pi = subroutineBody();
		if (pi.er != none)
			return pi;
		break;
	}
	return pi;
}

ParserInfo whileStatement()
{
	printf("\n hello from whileStatement");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "(") == 0)
		;
	else
	{
		pi.er = openParenExpected;
		return pi;
	}
	pi = expression();
	if (pi.er != none)
		return pi;
	if (strcmp(pi.tk.lx, ")") == 0)
		;
	else
	{
		pi.er = closeParenExpected;
		return pi;
	}
	pi = subroutineBody();
	return pi;
}

ParserInfo doStatement()
{
	printf("\n hello from doStatement");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	pi = subroutineCall();
	if (pi.er != none)
		return pi;
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

ParserInfo returnStatement()
{
	printf("\n hello from returnStatement");
	ParserInfo pi;
	pi.er = none;
	pi.tk = GetNextToken();
	pi.tk = PeekNextToken();
	while (1)
	{
		if (strcmp(pi.tk.lx, ";") == 0)
			break;
		pi = expression();
		if (pi.er != none)
			return pi;
		break;
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

ParserInfo memberDeclar()
{
	printf("\n hello from memberdeclar");
	ParserInfo pi;
	pi.er = none;
	pi.tk = PeekNextToken();
	if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
		pi = classVarDeclar();
	else if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "method") == 0 || strcmp(pi.tk.lx, "function") == 0)
		pi = subroutineDeclar();
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
	while (1)
	{
		pi = memberDeclar(); // multiple recurrences??
		if (pi.er != none)
			break;
		if (strcmp(PeekNextToken().lx, "}") == 0)
			break;
	}
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
