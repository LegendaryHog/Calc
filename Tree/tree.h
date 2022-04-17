#ifndef TREEH
#define TREEH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexer.h"

#define LEN0 90

typedef struct lexem {
    enum TYPE type;
    union VALUE {
        enum OPER op;
        enum BRACE brac;
        double coval;
    } val;
} lex_t;

typedef lex_t tree_t;


typedef struct Node {
    tree_t       data;
    struct Node* left;
    struct Node* right;
} Node;

Node*  PlantTree   (tree_t push);

int    ChopDown    (Node* tree);

Node*  SetLBranch  (Node* tree, tree_t push);

Node*  SetRBranch  (Node* tree, tree_t push);

int    TreeDbgDump (const Node* const tree);

#endif