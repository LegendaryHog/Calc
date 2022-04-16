#include "calc.h"

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
    long end = ftell (text);
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

size_t SkipSpaces (const char* str)
{
    size_t i = 0;
    for (i = 0; str[i] == ' ' || str[i] == '\t'; i++) {;}
    return i;
}

double GetG (formula* f)
{
    SKIPSPACES
    double val = GetE (f);
    SKIPSPACES
    if (f->str[f->p] != '$')
        return SyntaxError (f);
    else
        return val;
}

double GetE (formula* f)
{
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
    SKIPSPACES
    if (f->str[f->p] == '(')
    {
        f->p++;
        SKIPSPACES
        double val = GetE (f);
        SKIPSPACES
        if (f->str[f->p] != ')')
        {
            return SyntaxError (f);
        }
        f->p++;
        SKIPSPACES
        return val;
    }
    else
    {
        SKIPSPACES
        return GetN (f);
    }
}

double GetN (formula* f)
{
    SKIPSPACES
    double val = 0;
    if (sscanf (f->str + f->p, "%lf", &val) != 1)
        return SyntaxError (f);
    else
        f->p += SkipNumber (f->str + f->p);
        SKIPSPACES
        return val;
}

size_t SkipNumber (const char* str)
{
    size_t i = 0;
    for (i = 0; (str[i] >= '0' && str[i] <= '9') || str[i] == '.'; i++) {;}
    return i;
}

double SyntaxError (formula* f)
{
    fprintf (stderr, "OH SHIIIIIT SYNTAX ERROR\nstr: %s\n %*s\nposition: %zd\n", f->str, (int)f->p + 5, "^", f->p);
    return NAN;
}
