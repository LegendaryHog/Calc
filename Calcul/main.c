#include "calc.h"

int main (void)
{
    formula* f = (formula*) calloc (1, sizeof (formula));
    f->lexarr = lexarrCtor (16);
    const char* str = Read ("formula.txt", NULL);
    lexarrFill (f->lexarr, str);
    f->p = 0;
    printf ("Formula: %s\nValue: %lf\n", str, GetG (f));
    lexarrDump (f->lexarr);
    lexarrDtor (f->lexarr);
    free (f);
    return 0;
}