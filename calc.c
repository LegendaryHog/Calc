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

int GetG (formula* f)
{
    SKIPSPACES
    int val = GetE (f);
    SKIPSPACES
    if (f->str[f->p] != '$')
        return SyntaxError (f);
    else
        return val;
}

int GetE (formula* f)
{
    SKIPSPACES
    int val = GetT (f);
    SKIPSPACES
    while (f->str[f->p] == '+' || f->str[f->p] == '-')
    {
        char op = f->str[f->p];
        f->p++;
        SKIPSPACES
        int val2 = GetT (f);
        SKIPSPACES
        if (op == '+')
            val += val2;
        else
            val -= val2;
    }
    SKIPSPACES
    return val;
}

int GetT (formula* f)
{
    SKIPSPACES
    int val = GetP (f);
    SKIPSPACES
    while (f->str[f->p] == '*' || f->str[f->p] == '/')
    {
        char op = f->str[f->p];
        f->p++;
        SKIPSPACES
        int val2 = GetP (f);
        SKIPSPACES
        if (op == '*')
            val *= val2;
        else
            val /= val2;
    }
    SKIPSPACES
    return val;
}

int GetP (formula* f)
{
    SKIPSPACES
    if (f->str[f->p] == '(')
    {
        f->p++;
        SKIPSPACES
        int val = GetE (f);
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

int GetN (formula* f)
{
    SKIPSPACES
    int val = 0;
    size_t startpos = f->p;
    while ('0' <= f->str[f->p] && f->str[f->p] <= '9')
    {
        val = val * 10 + (f->str[f->p] - '0');
        f->p++;
    }
    if (startpos == f->p)
        return SyntaxError (f);
    else
        return val;
}

int SyntaxError (formula* f)
{
    fprintf (stderr, "OH SHIIIIIT SYNTAX ERROR\nstr: %s\n %*s\nposition: %zd\n", f->str, (int)f->p + 5, "^", f->p);
    return 69;
}
