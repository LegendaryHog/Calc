#ifndef CALCH
#define CALCH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIPSPACES f->p += SkipSpaces (f->str + f->p);

typedef struct formula {
    char*  str;
    size_t p;
} formula;

int   GetG (formula* f);

int   GetN (formula* f);

int   GetT (formula* f);

int   GetE (formula* f);

int   GetP (formula* f);

int   SyntaxError (formula* f);

char* Read (const char* filename, long* ptrbufsz);

size_t SkipSpaces (const char* str);

#endif