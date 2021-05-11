ParserInfo expressionList()
{
    printf("\n hello from expressionList");
    ParserInfo pi;
    pi.er = none;
    pi.tk = PeekNextToken(); // change 1
    printf("\nline 525 whats our token, its %s", pi.tk.lx);
    if (strcmp(pi.tk.lx, "-") == 0 || strcmp(pi.tk.lx, "~") == 0 || pi.tk.tp == INT || pi.tk.tp == ID || pi.tk.tp == STRING || strcmp(pi.tk.lx, "true") == 0 || strcmp(pi.tk.lx, "false") == 0 || strcmp(pi.tk.lx, "null") == 0 || strcmp(pi.tk.lx, "this") == 0 || strcmp(pi.tk.lx, "(") == 0)
    {
        pi = expression();
        if (pi.er != none)
            return pi;
        printf("\nline 532 whats our token, its %s", pi.tk.lx);
        pi.tk = PeekNextToken();
        printf("\nline 534 whats our token, its %s", pi.tk.lx);
        while (1)
        {
            if (strcmp(pi.tk.lx, ",") != 0)
                break;
            printf("\nreally hope we're not here");
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
    pi = operand();
    if (pi.er != none)
        return pi;
    while (1)
    {
        if (strcmp(pi.tk.lx, "*") != 0 && strcmp(pi.tk.lx, "/") != 0)
            break;
        GetNextToken();
        pi = operand();
        if (pi.er != none)
            return pi;
    }
    return pi;
}

ParserInfo operand()
{
    printf("\n hello from operand");
    ParserInfo pi;
    pi.er = none;
    pi.tk = GetNextToken();
    printf("\nline 658 token: %s", pi.tk.lx);
    if (strcmp(pi.tk.lx, "-") == 0 || strcmp(pi.tk.lx, "~") == 0 || pi.tk.tp == INT || pi.tk.tp == ID || pi.tk.tp == STRING || strcmp(pi.tk.lx, "true") == 0 || strcmp(pi.tk.lx, "false") == 0 || strcmp(pi.tk.lx, "null") == 0 || strcmp(pi.tk.lx, "this") == 0 || strcmp(pi.tk.lx, "(") == 0)
        ;
    else
    {
        pi.er = syntaxError;
        return pi;
    }
    if (strcmp(pi.tk.lx, "(") == 0)
    {
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
    if (pi.tk.tp == ID)
    {
        printf("\ndid we at least get to ID?");
        printf("\nline 688 token %s", pi.tk.lx);
        while (1)
        {
            pi.tk = PeekNextToken();
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
        printf("\nok are we at the choice of expression and expList?");
        while (1)
        {
            pi.tk = PeekNextToken();
            if (strcmp(pi.tk.lx, "[") != 0 && strcmp(pi.tk.lx, "(") != 0)
                break;
            printf("\noh did we find our ( ?");
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
                printf("\nshould be in expression list here");
                printf("\nline 721 ok what token are we at? we are at : %s", pi.tk.lx);
                printf("\nnow we call expression list");
                pi = expressionList();
                printf("\nok what token are we at? we are at : %s", pi.tk.lx);
                printf("\nok line 723 error: %d", pi.er);
                if (pi.er != none)
                    return pi;
                pi.tk = GetNextToken();
                printf("\nline 727 ok what token are we at? we are at : %s", pi.tk.lx);
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
    printf("\nok line 752 token: %s", pi.tk.lx);
    return pi;
}