#include "lexer.h"

lex_arr* lexarrCtor (size_t startcap)
{
    lex_arr* lexarr = (lex_arr*) calloc (1, sizeof (lex_arr));
    lexarr->capacity = startcap;
    lexarr->size = 0;
    lexarr->lexs = (lex_t*) calloc (lexarr->capacity, sizeof (lex_t));
    return lexarr;
}

int lexarrDtor (lex_arr* lexarr)
{
    if (!lexarr)
    {
        return 1;
    }
    free (lexarr->lexs);
    free (lexarr);
    return 0;
}

int lexarrPush (lex_arr* lexarr, lex_t push)
{
    if (lexarr->size == lexarr->capacity)
    {
        if (lexarrResize (lexarr) == 1);
        {
            fprintf (stderr, "MEMORY OFF\n");
            return 1;
        }
    }
    lexarr->lexs[lexarr->size++] = push;
    return 0;
}

int lexarrResize (lex_arr* lexarr)
{
    lexarr->capacity *= 2;
    lexarr->lexs = (lex_t*) realloc (lexarr->lexs, lexarr->capacity * sizeof (lex_t));
    if (!lexarr->lexs)
    {
        return 1;
    }
    for (size_t i = lexarr->capacity / 2; i < lexarr->capacity; i++)
    {
        lexarr->lexs[i].type = 0;
    }
    return 0;
}

size_t SkipNumber (const char* str)
{
    size_t i = 0;
    for (i = 0; (str[i] >= '0' && str[i] <= '9') || str[i] == '.'; i++) {;}
    return i;
}

size_t SkipSpaces (const char* str)
{
    size_t i = 0;
    for (i = 0; str[i] == ' ' || str[i] == '\t'; i++) {;}
    return i;
}

int lexarrFill (lex_arr* lexarr, const char* str)
{
    size_t p = 0;
    while (str[p] != '\0' && str[p] != '$')
    {
        p += SkipSpaces (str + p);
        if (str[p] == '+')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = ADD;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (str[p] == '-')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = SUB;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (str[p] == '*')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = MUL;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (str[p] == '/')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = DIV;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (str[p] == '^')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = DEG;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (strncmp (str + p, "sin", strlen ("sin")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = SIN;
            p += strlen ("sin");
            lexarrPush (lexarr, lex);
        }
        else if (strncmp (str + p, "cos", strlen ("cos")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = COS;
            p += strlen ("cos");
            lexarrPush (lexarr, lex);
        }
        else if (strncmp (str + p, "sqrt", strlen ("sqrt")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = SQRT;
            p += strlen ("sqrt");
            lexarrPush (lexarr, lex);
        }
        else if (strncmp (str + p, "cbrt", strlen ("cbrt")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = CBRT;
            p += strlen ("cbrt");
            lexarrPush (lexarr, lex);
        }
        else if (str[p] == '(')
        {
            lex_t lex = {};
            lex.type = BRAC;
            lex.val.brac = LBRAC;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (str[p] == ')')
        {
            lex_t lex = {};
            lex.type = BRAC;
            lex.val.brac = RBRAC;
            p++;
            lexarrPush (lexarr, lex);
        }
        else if (str[p] >= '0' && str[p] <= '9')
        {
            lex_t lex = {};
            lex.type = CONST;
            sscanf (str + p, "%lf", &lex.val.coval);
            p += SkipNumber (str + p);
            lexarrPush (lexarr, lex);
        }
        else
        {
            fprintf (stderr, "SYNTAX ERROR: UNEXPECTED SYMBOL (str[p] = %c)\nstr: %s\n%*s\n%*s\n%*s\n\n", str[p], str, (int)p + 6, "^", (int)p + 6, "|", (int)p + 6, "|");
            return 1;
        }
    }
    return 0;
}

int lexarrDump (lex_arr* lexarr)
{
    FILE* file = stdout;
    if (lexarr == NULL)
    {
        fprintf (stderr, "lexarr NULL pointer\n");
        return 1;
    }

    fprintf (file, "lexarr: %p\n\tcapacity: %zd\n\tsize: %zd\n\tlexs: %p\n\n", lexarr, lexarr->capacity, lexarr->size, lexarr->lexs);

    if (lexarr->lexs == NULL)
    {
        fprintf (stderr, "lexs array NULL pointer\n");
        return 1;
    }

    for (size_t i = 0; i < lexarr->size; i++)
    {
        fprintf (file, "pos: %zd\n", i);
        switch (lexarr->lexs[i].type) {
            case OPERAND:
                fprintf (file, "\ttype: operand\n");
                fprintf (file, "\toperand: %c\n", (char)lexarr->lexs[i].val.op);
                break;
            case BRAC:
                fprintf (file, "\ttype: brace\n");
                fprintf (file, "\tbrace: %c\n", (char)lexarr->lexs[i].val.brac);
                break;
            case CONST:
                fprintf (file, "\ttype: const\n");
                fprintf (file, "\tconst: %lf\n", lexarr->lexs[i].val.coval);
                break;
        }
    }
    return 0;
}





