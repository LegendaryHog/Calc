#include "calc.h"

int main (void)
{
    formula* f = (formula*) calloc (1, sizeof (formula));
    f->lexarr = lexarrCtor (16);
    f->str = Read ("formula.txt", NULL);
    if (lexarrFill (f->lexarr, f->str) == 1)
    {
        return 0;
    }
    f->p = 0;
    double res = GetG (f);
    if (isfinite (res))
    {
        printf ("Formula: %s\nValue: %lf\n", f->str, res);
    }
    //lexarrDump (f->lexarr);
    lexarrDtor (f->lexarr);
    free (f->str);
    free (f);
    return 0;
}