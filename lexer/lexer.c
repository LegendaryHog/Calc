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
        if (lexarrResize (lexarr) == 1)
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
    printf ("%p\n", lexarr->lexs);
    lexarr->lexs = (lex_t*) realloc (lexarr->lexs, lexarr->capacity * sizeof (lex_t));
    if (lexarr->lexs == NULL)
    {
        fprintf (stderr, "MEM OFF\n");
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
    for (;;)
    {
        p += SkipSpaces (str + p);
        if (str[p] == '+')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = ADD;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == '-')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = SUB;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == '*')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = MUL;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == '/')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = DIV;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == '^')
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = DEG;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (strncmp (str + p, "sin", strlen ("sin")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = SIN;
            p += strlen ("sin");
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (strncmp (str + p, "cos", strlen ("cos")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = COS;
            p += strlen ("cos");
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (strncmp (str + p, "sqrt", strlen ("sqrt")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = SQRT;
            p += strlen ("sqrt");
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (strncmp (str + p, "cbrt", strlen ("cbrt")) == 0)
        {
            lex_t lex = {};
            lex.type = OPERAND;
            lex.val.op = CBRT;
            p += strlen ("cbrt");
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == '(')
        {
            lex_t lex = {};
            lex.type = BRAC;
            lex.val.brac = LBRAC;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == ')')
        {
            lex_t lex = {};
            lex.type = BRAC;
            lex.val.brac = RBRAC;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] >= '0' && str[p] <= '9')
        {
            lex_t lex = {};
            lex.type = CONST;
            sscanf (str + p, "%lf", &lex.val.coval);
            p += SkipNumber (str + p);
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
        }
        else if (str[p] == '\0' || str[p] == '$')
        {
            lex_t lex = {};
            lex.type = END;
            p++;
            if (lexarrPush (lexarr, lex) == 1)
                return 1;
            break;
        }
        else
        {
            fprintf (stderr, "LEXER ERROR: UNEXPECTED SYMBOL (str[p] = %c)\nstr: %s\n%*s\n%*s\n%*s\n\n", str[p], str, (int)p + 6, "^", (int)p + 6, "|", (int)p + 6, "|");
            return 1;
        }
        p += SkipSpaces (str + p);
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

int fprintelem (FILE* file, lex_t lexem)
{
    switch (lexem.type) {
            case OPERAND:
                switch (f->lexarr->lexs[i].val.op) {
                    case ADD: case SUB: case MUL: case DIV: case DEG:
                        return fprintf (file, "%c ", lexem.val.op);
                    case SIN:
                        return fprintf (file, "sin ");
                    case COS:
                        return fprintf (file, "cos ");
                    case SQRT:
                        return fprintf (file, "sqrt ");
                    case CBRT:
                        return fprintf (file, "cbrt ");
                }
                break;
            case BRAC:
                if (islbr (lexem))
                    return fprintf (file, "( ");
                else
                    return fprintf (file, ") ");
            case CONST:
                return fprintf (file, "%lg ", lexem.val.coval);
            default::
                fprintf (file, "ERROR\n");
                return 0;
        }
}





