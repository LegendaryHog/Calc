#include "calc.h"

double GetG (formula* f)
{
    //printf ("call GetG\n");
    SKIPSPACES
    double val = GetE (f);
    SKIPSPACES
    if (f->str[f->p] != '$')
    {
        printf ("\npee pee poo poo\n");
        return SyntaxError (f);
    }
    else
        return val;
}

double GetE (formula* f)
{
    //printf ("call GetE\n");
    SKIPSPACES
    double val = GetT (f);
    SKIPSPACES
    while (f->str[f->p] == '+' || f->str[f->p] == '-')
    {
        char op = f->str[f->p];
        f->p++;
        SKIPSPACES
        double val2 = GetT (f);
        SKIPSPACES
        if (op == '+')
            val += val2;
        else
            val -= val2;
    }
    SKIPSPACES
    return val;
}

double GetT (formula* f)
{
    //printf ("call GetT\n");
    SKIPSPACES
    double val = GetP0 (f);
    SKIPSPACES
    while (f->str[f->p] == '*' || f->str[f->p] == '/')
    {
        char op = f->str[f->p];
        f->p++;
        SKIPSPACES
        double val2 = GetP0 (f);
        SKIPSPACES
        if (op == '*')
            val *= val2;
        else
            val /= val2;
    }
    SKIPSPACES
    return val;
}

double GetP0 (formula* f)
{
    //printf ("call GetP0\n");
    double val1 = GetP (f);
    SKIPSPACES
    if (f->str[f->p] != '^')
    {
        SKIPSPACES
        return val1;
    }
    f->p++;
    SKIPSPACES
    double val2 = GetP0 (f);
    SKIPSPACES
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
    //printf ("call GetP\n");
    SKIPSPACES
    if (f->str[f->p] == '(')
    {
        f->p++;
        SKIPSPACES
        double val = GetE (f);
        SKIPSPACES
        if (f->str[f->p] != ')')
        {
            //printf ("\ndick\n");
            return SyntaxError (f);
        }
        f->p++;
        SKIPSPACES
        return val;
    }
    else if (f->str[f->p] >= '0' && f->str[f->p] <= '9')
    {
        SKIPSPACES
        //printf ("\nhui\n");
        return GetN (f);
    }
    else
    {
        //printf ("else");
        if (strncmp (f->str + f->p, "sin", strlen ("sin")) == 0)
        {
            SKIPSPACES
            f->p += strlen ("sin");
            return sin (GetP (f));
        }
        if (strncmp (f->str + f->p, "cos", strlen ("cos")) == 0)
        {
            SKIPSPACES
            f->p += strlen ("cos");
            return cos (GetP (f));
        }
        if (strncmp (f->str + f->p, "sqrt", strlen ("sqrt")) == 0)
        {
            SKIPSPACES
            f->p += strlen ("sqrt");
            return sqrt (GetP (f));
        }
        if (strncmp (f->str + f->p, "cbrt", strlen ("cbrt")) == 0)
        {
            SKIPSPACES
            f->p += strlen ("cbrt");
            return cbrt (GetP (f));
        }
    }
}

double GetN (formula* f)
{
    //printf ("call GetN\n");
    SKIPSPACES
    double val = 0;
    if (sscanf (f->str + f->p, "%lf", &val) != 1)
    {
        //printf ("\ncock\n");
        return SyntaxError (f);
    }
    f->p += SkipNumber (f->str + f->p);
    SKIPSPACES
    return val;
}

/*double SyntaxError (formula* f)
{
    fprintf (stderr, "OH SHIIIIIT SYNTAX ERROR\nstr: %s\n %*s\nposition: %zd\n", f->str, (int)f->p + 5, "^", f->p);
    return NAN;
}*/

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

int isadd (lex_t lexem);
{
    if (lexem.type == OPERAND && lexem.val.)
        
}

int issub (lex_t lexem);

int ismul (lex_t lexem);

int isdiv (lex_t lexem);

int isdeg (lex_t lexem);

int issin (lex_t lexem);

int iscos (lex_t lexem);

int issqrt (lex_t lexem);

int iscbrt (lex_t lexem);

int islbr (lex_t lexem);

int isrbr (lex_t lexem);

int isconst (lex_t lexem);




