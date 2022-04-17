#ifndef CALCH
#define CALCH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lexer/lexer.h"

#define SKIPSPACES f->p += SkipSpaces (f->str + f->p);

typedef struct formula {
    lex_arr* lexarr;
    size_t p;
} formula;

double GetG (formula* f);

double GetN (formula* f);

double GetT (formula* f);

double GetE (formula* f);

double GetP (formula* f);

double GetP0 (formula* f);

double SyntaxError (formula* f);

int powint (int base, int deg);

char* Read (const char* filename, long* ptrbufsz);

size_t SkipSpaces (const char* str);

size_t SkipNumber (const char* str);
// is lex funcs
//------------------------------------------
int isadd (lex_t lexem);

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

//------------------------------------------


#endif