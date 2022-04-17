#include "calc.h"

size_t gdcounter1 = 0;

const lex_t NULLSTRUCT = {};

Node* GetG (formula* f)
{
    //printf ("call GetG: p = %zd\n", f->p);
    
    Node* val = GetE (f);
    
    if (!isend (ACTLEX))
    {
        //printf ("\npee pee poo poo\n");
        return SyntaxError (f);
    }
    else
        return val;
}

Node* GetE (formula* f)
{
    Node* tree = NULL;
    Node* val = GetT (f);
    Node* new = NULL;
    tree = val;

    while (isadd (ACTLEX) || issub (ACTLEX))
    {
        new = PlantTree (NULLSTRUCT);
        new->left = tree;
        char op = ACTLEX.val.op;
        f->p++;
        new->right = GetT (f);
        if (op == '+')
        {
            new->data.type = OPERAND;
            new->data.val.op = ADD;
        }
        else
        {
            new->data.type = OPERAND;
            new->data.val.op = SUB;
        }
        tree = new;
    }
    return tree;
}

Node* GetT (formula* f)
{
    Node* tree = NULL;
    Node* val = GetP0 (f);
    Node* new = NULL;
    tree = val;

    while (ismul (ACTLEX) || isdiv (ACTLEX))
    {
        new = PlantTree (NULLSTRUCT);
        new->left = tree;
        char op = ACTLEX.val.op;
        f->p++;
        new->right = GetP0 (f);

        if (op == '*')
        {
            new->data.type = OPERAND;
            new->data.val.op = MUL;
        }
        else
        {
            new->data.type = OPERAND;
            new->data.val.op = DIV;
        }
        tree = new;
    }
    return tree;
}

Node* GetP0 (formula* f)
{
    Node* tree = PlantTree (NULLSTRUCT);
    Node* val1 = GetP (f);
    if (!isdeg (ACTLEX))
    {
        ChopDown (tree);
        return val1;
    }
    f->p++;
    tree->right = GetP0 (f);
    tree->left = val1;
    tree->data.type = OPERAND;
    tree->data.val.op = DEG;
    return tree;
}

int powint (int base, int deg)
{
    int res = 1;
    for (int i = 0; i < deg; i++)
    {
        res *= base;
    }
    return res;
}

Node* GetP (formula* f)
{
    //printf ("call GetP: p = %zd\n", f->p);
    
    if (islbr (ACTLEX))
    {
        f->p++;
        Node* val = GetE (f);
        if (!isrbr (ACTLEX))
        {
            return SyntaxError (f);
        }
        f->p++;
        return val;
    }
    else if (isconst (ACTLEX))
    {
        return GetN (f);
    }
    else
    {
        Node* tree = PlantTree (NULLSTRUCT);
        if (issin (ACTLEX))
        { 
            f->p++;
            tree->data.type = OPERAND;
            tree->data.val.op = SIN;
            tree->left = GetP (f);
            return tree;
        }
        if (iscos (ACTLEX))
        {
            f->p++;
            tree->data.type = OPERAND;
            tree->data.val.op = COS;
            tree->left = GetP (f);
            return tree;
        }
        if (issqrt (ACTLEX))
        {
            f->p++;
            tree->data.type = OPERAND;
            tree->data.val.op = SQRT;
            tree->left = GetP (f);
            return tree;
        }
        if (iscbrt (ACTLEX))
        {
            f->p++;
            tree->data.type = OPERAND;
            tree->data.val.op = CBRT;
            tree->left = GetP (f);
            return tree;
        }
        return SyntaxError (f);
    }
}

Node* GetN (formula* f)
{
    Node* tree = PlantTree (NULLSTRUCT);
    if (!isconst (ACTLEX))
    {
        return SyntaxError (f);
    }
    tree->data.type = CONST;
    tree->data.val.coval = ACTLEX.val.coval;
    f->p++;
    return tree;
}

Node* SyntaxError (formula* f)
{
    fprintf (stderr, "SYNTAX ERROR\n");
    fprintf (stderr, "lexem: %d %c\n", ACTLEX.type, ACTLEX.val.brac);
    int strpos = 0;
    int errstart = 0;
    int errend = 0;
    for (size_t i = 0; i < f->lexarr->size; i++)
    {
        if (i == f->p)
        {
            errstart = strpos;
        }
        if (i >= f->p)
        {
            errend = strpos;
        }
        strpos += fprintelem (stderr, f->lexarr->lexs[i]);
    }
    fputc ('\n', stderr);
    fprintf (stderr, "\033[31m");
    for (int i = 0; i < errstart; i++)
    {
        fputc (' ', stderr);
    }
    for (int i = errstart; i < errend - 1; i++)
    {
        fputc ('~', stderr);
    }
    fprintf (stderr, "\n%*s\n%*s\n%*s\n\n", errstart + 1, "^", errstart + 1, "|", errstart + 1, "|");
    fprintf (stderr, "\033[0m");
    return NULL;
}

char* Read (const char* filename, long* ptrbufsz)
{
    if (filename == NULL)
    {
        return NULL;
    }
    size_t bufsz = 0;
    FILE* text = fopen (filename, "r");
    if (text == NULL)
    {
        return NULL;
    }
    fseek (text, 0, SEEK_SET);
    long start = ftell (text);
    fseek (text, 0, SEEK_END);
    long end   = ftell (text);
    fseek (text, 0, SEEK_SET);
    bufsz = end - start;

    char* buffer = (char*) calloc (bufsz + 1, sizeof (char));
    fread (buffer, sizeof (char), bufsz, text);
    if (ptrbufsz != NULL)
    {
        *ptrbufsz == bufsz;
    }
    fclose (text);
    return buffer;
}

void PrintLexem (const Node* const node, FILE* f)
{
    fprintf (f, "\tNODE_%p[label = \"", node);
    fprintelem (f, node->data);
    fprintf (f, "\"];\n");
    if (node->left)
    {
        PrintLexem (node->left, f);
        fprintf (f, "\tNODE_%p -> NODE_%p [style = bold, dir = both, arrowhead = crow, arrowtail = dot, color = chocolate4];\n", node, node->left);
    }
    if (node->right)
    {
        PrintLexem (node->right, f);
        fprintf (f, "\tNODE_%p -> NODE_%p [style = bold, dir = both, arrowhead = crow, arrowtail = dot, color = chocolate4];\n", node, node->right);
    }
}

int FormulaDump (Node* tree)
{
    assert (tree != NULL);
    FILE* graph = fopen ("logs/graph_dump.dot", "w");
    if (graph == NULL)
    {
        system ("mkdir logs/");
        graph = fopen ("logs/graph_dump.dot", "w");
    }
    fprintf (graph, "digraph G{\n");
    fprintf (graph, "\trankdir=TB;\n");
    fprintf (graph, "\tnode[color = forestgreen, shape=oval, penwidth=3.0, style = filled, fillcolor = green];\n");
    PrintLexem (tree, graph);
    fprintf (graph, "}\n");
    fclose (graph);
    char* cmd_mes = (char*) calloc (LEN0, sizeof (char));
    sprintf (cmd_mes, "dot -Tpng logs/graph_dump.dot -o logs/FormulaDump%zd.png", gdcounter1);
    system (cmd_mes);
    free (cmd_mes);
    system ("rm logs/graph_dump.dot");
    gdcounter1++;
}

int isadd (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == ADD)
        return 1;
    else
        return 0;    
}

int issub (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == SUB)
        return 1;
    else
        return 0;  
}

int ismul (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == MUL)
        return 1;
    else
        return 0;  
}

int isdiv (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == DIV)
        return 1;
    else
        return 0;  
}

int isdeg (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == DEG)
        return 1;
    else
        return 0;  
}

int issin (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == SIN)
        return 1;
    else
        return 0;  
}

int iscos (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == COS)
        return 1;
    else
        return 0;  
}

int issqrt (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == SQRT)
        return 1;
    else
        return 0;  
}

int iscbrt (lex_t lexem)
{
    if (lexem.type == OPERAND && lexem.val.op == CBRT)
        return 1;
    else
        return 0;  
}

int islbr (lex_t lexem)
{
    if (lexem.type == BRAC && lexem.val.brac == LBRAC)
        return 1;
    else
        return 0;  
}

int isrbr (lex_t lexem)
{
    if (lexem.type == BRAC && lexem.val.brac == RBRAC)
        return 1;
    else
        return 0;
}

int isconst (lex_t lexem)
{
    if (lexem.type == CONST)
        return 1;
    else
        return 0;
}

int isend (lex_t lexem)
{
    if (lexem.type == END)
        return 1;
    else
        return 0; 
}




