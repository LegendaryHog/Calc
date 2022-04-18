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
    LN = 'l',
};

enum BRACE {
    LBRAC = '(',
    RBRAC = ')',
};

enum TYPE {
    OPERAND = 1,
    NUM,
    BRAC,
    CONST,
    END,
};

enum CONSTANT {
    PI,
    PHI,
    NUM_E,
};
typedef struct lexem {
    enum TYPE type;
    union VALUE {
        enum OPER op;
        enum BRACE brac;
        enum CONSTANT con;
        double num;
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

int      fprintelem (FILE* file, lex_t lexem);

int lexsin (const char* str);

int lexcos (const char* str);

int lexsqrt (const char* str);

int lexcbrt (const char* str);

int lexln (const char* str);

int lexpi (const char* str);

int lexphi (const char* str);

int lexnum_e (const char* str);


#endif