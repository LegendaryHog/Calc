#include "calc.h"

double GetG (formula* f)
{
    //printf ("call GetG: p = %zd\n", f->p);
    
    double val = GetE (f);
    
    if (!isend (ACTLEX))
    {
        //printf ("\npee pee poo poo\n");
        return SyntaxError (f);
    }
    else
        return val;
}

double GetE (formula* f)
{
    //printf ("call GetE: p = %zd\n", f->p);
    double val = GetT (f);
    while (isadd (ACTLEX) || issub (ACTLEX))
    {
        char op = ACTLEX.val.op;
        f->p++;
        double val2 = GetT (f);
        if (op == '+')
            val += val2;
        else
            val -= val2;
    }
    return val;
}

double GetT (formula* f)
{
    //printf ("call GetT: p = %zd\n", f->p);  
    double val = GetP0 (f);
    while (ismul (ACTLEX) || isdiv (ACTLEX))
    {
        char op = ACTLEX.val.op;
        f->p++;
        
        double val2 = GetP0 (f);
        
        if (op == '*')
            val *= val2;
        else
            val /= val2;
    }
    
    return val;
}

double GetP0 (formula* f)
{
    //printf ("call GetP0: p = %zd\n", f->p);
    double val1 = GetP (f);
    if (!isdeg (ACTLEX))
    {
        return val1;
    }
    f->p++;
    
    double val2 = GetP0 (f);
    
    return pow (val1, val2);
}

int powint (int base, int deg)
{
    double res = 1;
    for (double i = 0; i < deg; i++)
    {
        res *= base;
    }
    return res;
}

double GetP (formula* f)
{
    //printf ("call GetP: p = %zd\n", f->p);
    
    if (islbr (ACTLEX))
    {
        f->p++;
        double val = GetE (f);
        if (!isrbr (ACTLEX))
        {
            ////printf ("Expected RBRAC\n");
            return SyntaxError (f);
        }
        f->p++;
        return val;
    }
    else if (isconst (ACTLEX))
    {
        ////printf ("hui\n");
        return GetN (f);
    }
    else
    {
        ////printf ("else");
        if (issin (ACTLEX))
        { 
            f->p++;
            return sin (GetP (f));
        }
        if (iscos (ACTLEX))
        {
            f->p++;
            return cos (GetP (f));
        }
        if (issqrt (ACTLEX))
        {
            f->p++;
            return sqrt (GetP (f));
        }
        if (iscbrt (ACTLEX))
        {
            f->p++;
            return cbrt (GetP (f));
        }
        return SyntaxError (f);
    }
}

double GetN (formula* f)
{
    //printf ("call GetN: p = %zd\n", f->p);
    if (!isconst (ACTLEX))
    {
        //printf ("\ncock\n");
        return SyntaxError (f);
    }
    double val = ACTLEX.val.coval;
    f->p++;
    
    return val;
}

double SyntaxError (formula* f)
{
    fprintf (stderr, "SYNTAX ERROR\n");
    fprintf (stderr, "lexem: %d %c\n", ACTLEX.type, ACTLEX.val.brac);
    int strpos = 0;
    int errstart = 0;
    int errend = 0;
    for (size_t i = 0; i < f->lexarr->size; i++)
    {
        if (i == f->p)
        {
            errstart = strpos;
        }
        if (i >= f->p)
        {
            errend = strpos;
        }
        switch (f->lexarr->lexs[i].type) {
            case OPERAND:
                switch (f->lexarr->lexs[i].val.op) {
                    case ADD: case SUB: case MUL: case DIV: case DEG:
                        strpos += fprintf (stderr, "%c ", f->lexarr->lexs[i].val.op);
                        break;
                    case SIN:
                        strpos += fprintf (stderr, "sin ");
                        break;
                    case COS:
                        strpos += fprintf (stderr, "cos ");
                        break;
                    case SQRT:
                        strpos += fprintf (stderr, "sqrt ");
                        break;
                    case CBRT:
                        strpos += fprintf (stderr, "cbrt ");
                        break;
                }
                break;
            case BRAC:
                if (islbr (f->lexarr->lexs[i]))
                    strpos += fprintf (stderr, "( ");
                else
                    strpos += fprintf (stderr, ") ");
                break;
            case CONST:
                strpos += fprintf (stderr, "%lg ", f->lexarr->lexs[i].val.coval);
                break;
        }
    }
    fputc ('\n', stderr);
    fprintf (stderr, "\033[31m");
    for (int i = 0; i < errstart; i++)
    {
        fputc (' ', stderr);
    }
    for (int i = errstart; i < errend - 1; i++)
    {
        fputc ('~', stderr);
    }
    fprintf (stderr, "\n%*s\n%*s\n%*s\n\n", errstart + 1, "^", errstart + 1, "|", errstart + 1, "|");
    fprintf (stderr, "\033[0m");
    return NAN;
}

char* Read (const char* filename, long* ptrbufsz)
{
    if (filename == NULL)
    {
        return NULL;
    }
    size_t bufsz = 0;
    FILE* text = fopen (filename, "r");
    if (text == NULL)
    {
        return NULL;
    }
    fseek (text, 0, SEEK_SET);
    long start = ftell (text);
    fseek (text, 0, SEEK_END);
    long end   = ftell (text);
    fseek (text, 0, SEEK_SET);
    bufsz = end - start;

    char* buffer = (char*) calloc (bufsz + 1, sizeof (char));
    fread (buffer, sizeof (char), bufsz, text);
    if (ptrbufsz != NULL)
    {
        *ptrbufsz == bufsz;
    }
    fclose (text);
    return buffer;
}

int isadd (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == ADD)
        return 1;
    else
        return 0;    
}

int issub (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == SUB)
        return 1;
    else
        return 0;  
}

int ismul (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == MUL)
        return 1;
    else
        return 0;  
}

int isdiv (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == DIV)
        return 1;
    else
        return 0;  
}

int isdeg (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == DEG)
        return 1;
    else
        return 0;  
}

int issin (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == SIN)
        return 1;
    else
        return 0;  
}

int iscos (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == COS)
        return 1;
    else
        return 0;  
}

int issqrt (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == SQRT)
        return 1;
    else
        return 0;  
}

int iscbrt (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == CBRT)
        return 1;
    else
        return 0;  
}

int islbr (lex_t lexem)
{
    if (lexem.type == BRAC && lexem.val.brac == LBRAC)
        return 1;
    else
        return 0;  
}

int isrbr (lex_t lexem)
{
    if (lexem.type == BRAC && lexem.val.brac == RBRAC)
        return 1;
    else
        return 0;
}

int isconst (lex_t lexem)
{
    if (lexem.type == CONST)
        return 1;
    else
        return 0;
}

int isend (lex_t lexem)
{
    if (lexem.type == END)
        return 1;
    else
        return 0; 
}




