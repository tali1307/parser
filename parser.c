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
ParserInfo expressionList();
ParserInfo relationalExpression();
ParserInfo ArithmeticExpression();
ParserInfo term();
ParserInfo factor();
ParserInfo operand();

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
	printf("\ntoken in subDeclar line 102 %s", pi.tk.lx);
	pi.tk = PeekNextToken();
	printf("\ntoken in subDeclar line 104 %s", pi.tk.lx);
	if (strcmp(pi.tk.lx, "char") == 0 || strcmp(pi.tk.lx, "int") == 0 || strcmp(pi.tk.lx, "boolean") == 0 || pi.tk.tp == ID)
		pi = type();
	else if (strcmp(pi.tk.lx, "void") == 0)
		GetNextToken();
	else
	{
		pi.er = syntaxError;
		return pi;
	}
	pi.tk = GetNextToken();
	printf("\ntoken in subDeclar line 115 %s", pi.tk.lx);
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
		printf("\nline 317 letStatement token %s", pi.tk.lx);
		pi = expression();
		if (pi.er != none)
			return pi;
		pi.tk = GetNextToken();
		if (strcmp(pi.tk.lx, "]") == 0)
			;
		else
		{
			pi.er = closeBracketExpected;
			return pi;
		}
		break;
	}
	pi.tk = GetNextToken();
	printf("\nline 334 letstatement the token is %s", pi.tk.lx);
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
	printf("\nline 345 letstatement the token is %s", pi.tk.lx);
	pi.tk = GetNextToken();
	printf("\nline 347 letstatement the token is %s", pi.tk.lx);
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
	pi.tk = GetNextToken();
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

ParserInfo subroutineCall()
{
	printf("\n hello from subroutineCall");
	ParserInfo pi;
	pi.er = none;
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
		if (strcmp(pi.tk.lx, ".") != 0)
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
		break;
	}
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, "(") == 0)
		;
	else
	{
		pi.er = openParenExpected;
		return pi;
	}
	printf("\nline 506 token %s", pi.tk.lx);
	pi = expressionList();
	if (pi.er != none)
		return pi;
	printf("\nline 506 token %s", pi.tk.lx);
	pi.tk = GetNextToken();
	if (strcmp(pi.tk.lx, ")") == 0)
		;
	else
	{
		pi.er = closeParenExpected;
		return pi;
	}
	return pi;
}

ParserInfo expressionList()
{
	printf("\n hello from expressionList");
	ParserInfo pi;
	pi.er = none;
	pi.tk = PeekNextToken();
	if (strcmp(pi.tk.lx, "-") == 0 || strcmp(pi.tk.lx, "~") == 0 || pi.tk.tp == INT || pi.tk.tp == ID || pi.tk.tp == STRING || strcmp(pi.tk.lx, "true") == 0 || strcmp(pi.tk.lx, "false") == 0 || strcmp(pi.tk.lx, "null") == 0 || strcmp(pi.tk.lx, "this") == 0 || strcmp(pi.tk.lx, "(") == 0)
	{
		pi = expression();
		if (pi.er != none)
			return pi;
		pi.tk = GetNextToken();
		pi.tk = PeekNextToken();
		while (1)
		{
			if (strcmp(pi.tk.lx, ",") != 0)
				break;
			pi.tk = GetNextToken();
			pi = expression();
			if (pi.er != none)
				return pi;
			pi.tk = PeekNextToken();
		}
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

ParserInfo expression()
{
	printf("\n hello from expression");
	ParserInfo pi;
	pi.er = none;
	pi = relationalExpression();
	if (pi.er != none)
		return pi;
	while (1)
	{
		if (strcmp(pi.tk.lx, "&") != 0 && strcmp(pi.tk.lx, "|") != 0)
			break;
		GetNextToken();
		pi = relationalExpression();
		if (pi.er != none)
			return pi;
	}
	return pi;
}

ParserInfo relationalExpression()
{
	printf("\n hello from relationalExpression");
	ParserInfo pi;
	pi.er = none;
	pi = ArithmeticExpression();
	if (pi.er != none)
		return pi;
	while (1)
	{
		if (strcmp(pi.tk.lx, "=") != 0 && strcmp(pi.tk.lx, ">") != 0 && strcmp(pi.tk.lx, "<") != 0)
			break;
		printf(" line 592 arithmetic, token is %s", pi.tk.lx);
		GetNextToken();
		pi = ArithmeticExpression();
		if (pi.er != none)
			return pi;
	}
	return pi;
}

ParserInfo ArithmeticExpression()
{
	printf("\n hello from ArithmeticExpression");
	ParserInfo pi;
	pi.er = none;
	pi = term();
	if (pi.er != none)
		return pi;
	while (1)
	{
		if (strcmp(pi.tk.lx, "+") != 0 && strcmp(pi.tk.lx, "-") != 0)
			break;
		GetNextToken();
		pi = term();
		if (pi.er != none)
			return pi;
	}
	return pi;
}

ParserInfo term()
{
	printf("\n hello from term");
	ParserInfo pi;
	pi.er = none;
	pi = factor();
	if (pi.er != none)
		return pi;
	while (1)
	{
		if (strcmp(pi.tk.lx, "*") != 0 && strcmp(pi.tk.lx, "/") != 0)
			break;
		GetNextToken();
		pi = factor();
		if (pi.er != none)
			return pi;
	}
	return pi;
}

ParserInfo factor()
{
	printf("\n hello from factor");
	ParserInfo pi;
	pi.er = none;
	printf("\ntoken here13 %s", pi.tk.lx);
	pi.tk = PeekNextToken();
	printf("\ntoken here14 %s", pi.tk.lx);
	if (strcmp(pi.tk.lx, "-") == 0 || strcmp(pi.tk.lx, "~") == 0 || pi.tk.tp == INT || pi.tk.tp == STRING || pi.tk.tp == ID || strcmp(pi.tk.lx, "true") == 0 || strcmp(pi.tk.lx, "false") == 0 || strcmp(pi.tk.lx, "null") == 0 || strcmp(pi.tk.lx, "this") == 0 || strcmp(pi.tk.lx, "(") == 0)
		pi = operand();
	else
	{
		pi.er = syntaxError;
		return pi;
	}
	return pi;
}

ParserInfo operand()
{
	printf("\n hello from operand");
	ParserInfo pi;
	pi.er = none;
	printf("\ntoken here15 %s", pi.tk.lx);
	pi.tk = PeekNextToken();
	printf("\ntoken here16 %s", pi.tk.lx);
	printf("\nare we here1 %s", pi.tk.lx);
	if (pi.tk.tp == INT || pi.tk.tp == STRING || strcmp(pi.tk.lx, "true") == 0 || strcmp(pi.tk.lx, "false") == 0 || strcmp(pi.tk.lx, "null") == 0 || strcmp(pi.tk.lx, "this") == 0)
		;
	else if (strcmp(pi.tk.lx, "(") == 0)
	{
		pi.tk = GetNextToken();
		pi = expression();
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
	}
	else if (pi.tk.tp == ID)
	{
		printf("\ntoken here??6 %s", pi.tk.lx);
		pi.tk = GetNextToken();
		pi.tk = GetNextToken();
		printf("\ntoken here??7 %s", pi.tk.lx);
		printf("\ntoken here??8 %s", pi.tk.lx);
		while (1)
		{
			if (strcmp(pi.tk.lx, ".") != 0)
				break;
			printf("\nare we here2 %s", pi.tk.lx);
			pi.tk = GetNextToken();
			printf("\nare we here3 %s", pi.tk.lx);
			if (pi.tk.tp == ID)
				;
			else
			{
				pi.er = idExpected;
				return pi;
			}
			break;
		}
		printf("\ntoken here??9 %s", pi.tk.lx);
		pi.tk = PeekNextToken();
		printf("\ntoken here??10 %s", pi.tk.lx);
		while (1)
		{
			if (strcmp(pi.tk.lx, "[") != 0 && strcmp(pi.tk.lx, "(") != 0)
				break;
			printf("\ntoken here??11 %s", pi.tk.lx);
			pi.tk = GetNextToken();
			if (strcmp(pi.tk.lx, "[") == 0)
			{
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
			else if (strcmp(pi.tk.lx, "(") == 0)
			{
				printf("\ntoken here4 %s", pi.tk.lx);
				pi = expressionList();
				printf("\ntoken here12 %s", pi.tk.lx);
				if (pi.er != none)
					return pi;
				pi.tk = GetNextToken();
				printf("\ntoken here5 %s", pi.tk.lx);
				if (strcmp(pi.tk.lx, ")") == 0)
					;
				else
				{
					pi.er = closeParenExpected;
					return pi;
				}
				break;
			}
		}
	}
	else
	{
		pi.er = syntaxError;
		return pi;
	}
	return pi;
}

// put expressions back here

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
	InitParser("Main.jack");
	ParserInfo pi;
	pi = Parse();
	StopParser();
	printf("\n token lexeme: %s, line no: %d and pi error: ", pi.tk.lx, pi.tk.ln);
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
	printf("\n\n\n");
	return 1;
}
#endif
