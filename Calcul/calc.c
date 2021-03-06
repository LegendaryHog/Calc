#include "calc.h"

size_t gdcounter1 = 0;

const double _PI_    = 3.1415926535;
const double _NUM_E_ = 2.7182818284;
const double _PHI_   = 1.618033989;

const lex_t NULLSTRUCT = {};

double SubstitAndCalc (Node* tree, const char* substit)
{
    if (Subtit (tree, substit) == 1)
    {
        fprintf (stderr, "ERROR: bad subtit\n");
        return 1;
    }
    return CalcTree (tree);
}

int Subtit (Node* tree, const char* subtit)
{
    vararr* varr = vararrCtor (1);
    size_t p = 0;
    while (subtit[p] != '\0' && subtit[p] != '\n' && subtit[p] != '\r')
    {
        if (varr->size != 0 && subtit[p] != ',')
        {
            fprintf (stderr, "Not found ',' after %zd variable", varr->size - 1);
            return 1;
        }
        else if (varr->size != 0)
        {
            p++;
        }
        struct VARIABLE var = {};
        p += SkipSpaces (subtit + p);
        if (sscanf (subtit + p, "%[^ =]", var.name) != 1)
        {
            fprintf (stderr, "Can't scan %zd variable name\n", varr->size);
            return 1;
        }
        p += SkipVar (subtit + p);
        p += SkipSpaces (subtit + p);
        if (subtit[p] != '=')
        {
            fprintf (stderr, "Not found '=' after %zd variable name\n", varr->size);
            return 1;
        }
        p++;
        p += SkipSpaces (subtit + p);
        if (sscanf (subtit + p, "%lf", &var.value) != 1)
        {
            fprintf (stderr, "Can't scan value of %zd variable", varr->size);
            return 1;
        }
        p += SkipNumber (subtit + p);
        p += SkipSpaces (subtit + p);
        vararrPush (varr, var);
    }
    int err = RecSubtit (tree, varr); 
    vararrDtor (varr);
    return err;
}

int RecSubtit (Node* node, vararr* varr)
{
    if (node->data.type == VAR)
    {
        size_t i = 0;
        for (i = 0; i < varr->size; i++)
        {
            if (strcmp (node->data.val.var.name, varr->arr[i].name) == 0)
            {
                node->data.val.var.value = varr->arr[i].value;
                break;
            }
        }
        if (i == varr->size)
        {
            fprintf (stderr, "ERROR: not found variable %s", node->data.val.var.name);
            return 1;
        }
    }
    int err = 0;
    if (node->left)
    {
        err += RecSubtit (node->left, varr);
    }
    if (node->right)
    {
        err += RecSubtit (node->right, varr);
    }
    if (err > 0)
        return 1;
    else
        return 0;
}

vararr* vararrCtor (size_t startcap)
{
    vararr* varr = (vararr*) calloc (1, sizeof (vararr));
    varr->arr = (struct VARIABLE*) calloc (startcap, sizeof (struct VARIABLE));
    varr->capacity = startcap;
    varr->size = 0;
    return varr; 
}

int vararrDtor (vararr* varr)
{
    if (!varr)
    {
        return 1;
    }
    free (varr->arr);
    free (varr);
    return 0;
}

int vararrResize (vararr* varr)
{
    varr->capacity *= 2;
    varr->arr = (struct VARIABLE*) realloc (varr->arr, varr->capacity * sizeof (struct VARIABLE));
    for (size_t i = varr->capacity/2; i < varr->capacity; i++)
    {
        varr->arr[i].value = NAN;
    }
    return 0;
}

int vararrPush (vararr* varr, struct VARIABLE push)
{
    if (varr->size == varr->capacity)
    {
        vararrResize (varr);
    }
    strcpy (varr->arr[varr->size].name, push.name);
    varr->arr[varr->size].value = push.value;
    varr->size++;
    return 0;
}

double CalcTree (Node* tree)
{
    if (tree == NULL)
    {
        fprintf (stderr, "FATAL ERROR: tree NULL pointer\n");
        return NAN;
    }
    if (isnum (tree->data))
        return tree->data.val.num;
    else if (ispi (tree->data))
        return _PI_;
    else if (isnum_e (tree->data))
        return _NUM_E_;
    else if (isphi (tree->data))
        return _PHI_;
    else if (isvar (tree->data))
        return tree->data.val.var.value;
    else {
        switch (tree->data.val.op) {
            case ADD:
                return CALC_L + CALC_R;
            case SUB:
                return CALC_L - CALC_R;
            case MUL:
                return CALC_L * CALC_R;
            case DIV:
                return CALC_L / CALC_R;
            case DEG:
                return pow (CALC_L, CALC_R);
            case SIN:
                return sin (CALC_L);
            case COS:
                return cos (CALC_L);
            case SQRT:
                return sqrt (CALC_L);
            case CBRT:
                return cbrt (CALC_L);
            case LN:
                return log (CALC_L);
            default:
                fprintf  (stderr, "FATAL ERROR: UNKNOWN TYPE OF OPER: %d\n", tree->data.val.op);
                return NAN;
        }
    }
}

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
            new->data.type = OPER;
            new->data.val.op = ADD;
        }
        else
        {
            new->data.type = OPER;
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
            new->data.type = OPER;
            new->data.val.op = MUL;
        }
        else
        {
            new->data.type = OPER;
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
    tree->data.type = OPER;
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
    else if (isconst (ACTLEX) || isnum (ACTLEX) || isvar (ACTLEX))
    {
        return GetN (f);
    }
    else
    {
        Node* tree = PlantTree (NULLSTRUCT);
        if (issin (ACTLEX))
        { 
            f->p++;
            tree->data.type = OPER;
            tree->data.val.op = SIN;
            tree->left = GetP (f);
            return tree;
        }
        if (iscos (ACTLEX))
        {
            f->p++;
            tree->data.type = OPER;
            tree->data.val.op = COS;
            tree->left = GetP (f);
            return tree;
        }
        if (issqrt (ACTLEX))
        {
            f->p++;
            tree->data.type = OPER;
            tree->data.val.op = SQRT;
            tree->left = GetP (f);
            return tree;
        }
        if (iscbrt (ACTLEX))
        {
            f->p++;
            tree->data.type = OPER;
            tree->data.val.op = CBRT;
            tree->left = GetP (f);
            return tree;
        }
        if (isln (ACTLEX))
        {
            f->p++;
            tree->data.type = OPER;
            tree->data.val.op = LN;
            tree->left = GetP (f);
            return tree;
        }
        fprintf (stderr, "SYNTAX ERROR FROM GetP ()\n");
        return SyntaxError (f);
    }
}

Node* GetN (formula* f)
{
    Node* tree = PlantTree (NULLSTRUCT);
    if (!isnum (ACTLEX) && !isconst (ACTLEX) && !isvar (ACTLEX))
    {
        fprintf (stderr, "SYNTAXERROR FROM GetN ()\n");
        return SyntaxError (f);
    }
    if (isconst (ACTLEX))
    {
        tree->data.type = CONST;
        tree->data.val.con = ACTLEX.val.con;
    }
    else if (isnum (ACTLEX))
    {
        tree->data.type = NUM;
        tree->data.val.num = ACTLEX.val.num;
    }
    else
    {
        tree->data.type = VAR;
        tree->data.val.var.value = NAN;
        strcpy (tree->data.val.var.name, ACTLEX.val.var.name);
    }
    f->p++;
    return tree;
}

Node* SyntaxError (formula* f)
{
    fprintf (stderr, "SYNTAX ERROR\n");
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
    if (lexem.type == OPER && lexem.val.op == ADD)
        return 1;
    else
        return 0;    
}

int issub (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == SUB)
        return 1;
    else
        return 0;  
}

int ismul (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == MUL)
        return 1;
    else
        return 0;  
}

int isdiv (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == DIV)
        return 1;
    else
        return 0;  
}

int isdeg (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == DEG)
        return 1;
    else
        return 0;  
}

int issin (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == SIN)
        return 1;
    else
        return 0;  
}

int iscos (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == COS)
        return 1;
    else
        return 0;  
}

int issqrt (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == SQRT)
        return 1;
    else
        return 0;  
}

int iscbrt (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == CBRT)
        return 1;
    else
        return 0;  
}

int isln (lex_t lexem)
{
    if (lexem.type == OPER && lexem.val.op == LN)
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

int isnum (lex_t lexem)
{
    if (lexem.type == NUM)
        return 1;
    else
        return 0;
}

int ispi (lex_t lexem)
{
    if (lexem.type == CONST && lexem.val.con == PI)
        return 1;
    else
        return 0;
}

int isphi (lex_t lexem)
{
    if (lexem.type == CONST && lexem.val.con == PHI)
        return 1;
    else
        return 0;
}

int isnum_e (lex_t lexem)
{
    if (lexem.type == CONST && lexem.val.con == NUM_E)
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

int isvar (lex_t lexem)
{
    if (lexem.type == VAR)
        return 1;
    else
        return 0;
}




