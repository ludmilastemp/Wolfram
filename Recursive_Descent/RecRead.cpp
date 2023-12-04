// 22:49
#include "RecRead.h"
// 23:05

static DATA_T GetExspretion  (RecursDescent* recDescent);
static DATA_T GetTerm        (RecursDescent* recDescent);
static DATA_T GetUnary       (RecursDescent* recDescent);
static DATA_T GetPrimary     (RecursDescent* recDescent);
static DATA_T GetNumber      (RecursDescent* recDescent);

static int
FindVariable (RecursDescent* recDescent);

static int
StrlenVarName (const char* const buf);

BinTree* GetG (struct File* file)
{
    STL_Fread (file);

    BinTree* tree  = BinTreeCtor ();
    tree->buf = file->text;

    RecursDescent recDescent = { .str   = file->text,
                                 .pos   = 0,
                                 .error = 0,
                                 .stk   = tree->variable };

    tree->root = GetExspretion (&recDescent);    // \0

/// ставим \0 в конце названия каждой переменной,
/// для корректной работы %s
    for (int i = 0; i < tree->variable->size; i++)
    {
        tree->variable->data[i].name
            [StrlenVarName (tree->variable->data[i].name)] = '\0';
    }

    return tree;
}

#define POISON NodeBinTreeData::VALUE_POISON
#define _CONST(x)   NodeBinTreeCtor (NodeBinTreeDataCtor (x))
#define _ADD(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '+'), x, y)
#define _SUB(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '-'), x, y)

DATA_T GetExspretion (RecursDescent* recDescent)
{
    DATA_T val = GetTerm (recDescent);

    while (recDescent->str[recDescent->pos] == '+' ||
           recDescent->str[recDescent->pos] == '-')
    {
        char op = recDescent->str[recDescent->pos];
        recDescent->pos++;

        DATA_T val2 = GetTerm (recDescent);

        switch (op)
        {
            case '+':

                val = _ADD (val, val2);

                break;

            case '-':

                val = _SUB (val, val2);

                break;

            default:
                assert (false);
        }
    }

    return val;
}

#define _MUL(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '*'), x, y)
#define _DIV(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '/'), x, y)
#define _POW(x, y)  NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '^'), x, y)

DATA_T GetTerm (RecursDescent* recDescent)
{
    DATA_T val = GetUnary (recDescent);

    while (recDescent->str[recDescent->pos] == '*' ||
           recDescent->str[recDescent->pos] == '/' ||
           recDescent->str[recDescent->pos] == '^')
    {
        char op = recDescent->pos[recDescent->str];
        recDescent->pos++;

        DATA_T val2 = GetUnary (recDescent);

        switch (op)
        {
            case '*':

                val = _MUL (val, val2);

                break;

            case '/':

                val = _DIV (val, val2);

                break;

            case '^':

                val = _POW (val, val2);

                break;

            default:

                assert (false);
        }
    }

    return val;
}

#define _SIN(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, SIN + OPCODE), nullptr, x)
#define _COS(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, COS + OPCODE), nullptr, x)
#define _LN(x)      NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, LN  + OPCODE), nullptr, x)

DATA_T GetUnary (RecursDescent* recDescent)
{
    int i = 0;
    for (; i < nOperation; i++)
    {
        if (strncmp (recDescent->str + recDescent->pos,
                     operation[i], lenOperation[i]) == 0)
        {
            recDescent->pos += lenOperation [i];
            break;
        }
    }

    DATA_T val = GetPrimary (recDescent);

    switch (i)
    {
        case SIN:

            val = _SIN (val);

            break;

        case COS:

            val = _COS (val);

            break;

        case LN:

            val = _LN (val);

            break;

        default:

            break;
    }

    return val;
}

DATA_T GetPrimary (RecursDescent* recDescent)
{
    if (recDescent->str[recDescent->pos] == '(')
    {
        recDescent->pos++;

        DATA_T val = GetExspretion (recDescent);

        assert (recDescent->str[recDescent->pos] == ')');
        recDescent->pos++;

        return val;
    }

    return GetNumber (recDescent);
}

DATA_T GetNumber (RecursDescent* recDescent)
{
    int old_p  = recDescent->pos;
    double val = 0;
    /*
    int sign   = 1;

    if (recDescent->str[recDescent->pos] == '-')
    {
        sign = -1;
        recDescent->pos++;
    }

    while ('0' <= recDescent->str[recDescent->pos] && recDescent->str[recDescent->pos] <= '9')
    {
        val = val * 10 + recDescent->str[recDescent->pos] - '0';
        recDescent->pos++;
    }

    assert (recDescent->pos > old_p);

    if (sign == -1) return _MUL (_CONST (-1), _CONST (val));
    return _CONST (val);   */

    int nLen = 0;
    sscanf (recDescent->str + recDescent->pos, "%lg %n", &val, &nLen);

    if (nLen)
    {
        recDescent->pos += nLen;
        return _CONST (val);
    }

    NodeBinTree* node = NodeBinTreeCtor (NodeBinTreeDataCtor ());
    node->data->variable = FindVariable (recDescent);

    assert (recDescent->pos > old_p);

    return node;

}

static int
FindVariable (RecursDescent* recDescent)
{
    int nameLen = StrlenVarName (recDescent->str + recDescent->pos);

    int variable = 0;
    for (; variable < recDescent->stk->size; variable++)
    {
        if (strncmp (recDescent->stk->data[variable].name, recDescent->str + recDescent->pos, nameLen) == 0)
        {
            recDescent->pos += nameLen;
            return variable;
        }
    }

    Variable var = { .name = recDescent->str + recDescent->pos, .value = 0 };

    StackPush (recDescent->stk, var);

    recDescent->pos += nameLen;

    return variable;
}

#undef POISON
#undef _CONST
#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV
#undef _POW
#undef _SIN
#undef _COS
#undef _LN

/// так как после переменной нет пробела,
/// нельзя испольновать sscanf
static int
StrlenVarName (const char* const buf)
{
    int i = 0;
    while (true)
    {
        switch (*(buf + i))
        {
            case ')':
            case '(':
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case LN  + OPCODE:
            case SIN + OPCODE:
            case COS + OPCODE:
            {
                return i;
            }
        }
        i++;
    }
}
