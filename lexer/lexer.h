#ifndef LEXERH
#define LEXERH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    OPERAND = 1,
    CONST,
    BRAC,
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
    lex_t* lexs;
    size_t size;
    size_t capacity;
} lex_arr;

lex_arr* lexarrCtor (size_t startcap);

int      lexarrFill (lex_arr* lexarr, const char* str);

int      lexarrResize (lex_arr* lexarr);

int      lexarrPush (lex_arr* lexarr, lex_t push);

int      lexarrDtor (lex_arr* lexarr);

size_t   SkipSpaces (const char* str);

size_t   SkipNumber (const char* str);

int      lexarrDump (lex_arr* lexarr);


#endif