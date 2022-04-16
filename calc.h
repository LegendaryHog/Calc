#ifndef CALCH
#define CALCH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SKIPSPACES f->p += SkipSpaces (f->str + f->p);

enum OPER {
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    DEG = '^',
    SIN = 's',
    COS = 'c',
    SQRT = 'S',
    CBRT = 'C',
};

enum BRACE {
    LBRAC = '(',
    RBRAC = ')',
};

enum TYPE {
    OPER,
    CONST,
};

typedef struct lexem {
    enum TYPE type;
    union VALUE {
        enum OPER op;
        enum BRACE brac;
        double coval;
    } val;
} lex_t;

typedef struct lex_arr {
    lex_t* lexems;
    size_t size;
    size_t capacity;
} lex_arr;

typedef struct formula {
    char*  str;
    size_t p;
} formula;

lex_arr* lexarrCtor (size_t startcap);

int lexarrFill (lex_arr* lexarr);

int lexarrResize (lex_arr* lexarr);

int lexarrPush (lex_arr* lexarr, lex_t push);

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

#endif