#include "calc.h"

int main (void)
{
    formula* f = (formula*) calloc (1, sizeof (formula));
    f->lexarr = lexarrCtor (16);
    const char* str = Read ("formula.txt", NULL);
    lexarrFill (lexarr, str);
    f->p = 0;
    printf ("Formula: %s\nValue: %lf\n", f->str, GetG (f));
    free (f->str);
    free (f);
    lexarrDump (lexarr);
    lexarrDtor (lexarr);
    return 0;
}