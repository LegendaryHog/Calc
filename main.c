#include "calc.h"

int main (void)
{
    formula* f = (formula*) calloc (1, sizeof (formula));
    f->str = Read ("formula.txt", NULL);
    f->p = 0;
    printf ("Formula: %s\nValue: %d\n", f->str, GetG (f));
    free (f->str);
    free (f);
    return 0;
}