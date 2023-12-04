#include "STL_wolfram.h"

static int
CopyStack (BinTree* tree, BinTree* newTree);

double
Eval (NodeBinTree* node, Stack_Variable* stk)
{
    if (node == nullptr) return 0;

    if (node->data->variable != NodeBinTreeData::VARIABLE_POISON)
    {
        return stk->data[node->data->variable].value;
    }

    double left  = Eval (node->left,  stk);
    double right = Eval (node->right, stk);

//    printf ("%d\n", node->data->opCode);
    switch (node->data->opCode)
    {
        case 0:

            return node->data->value;

        case '+':

            return left + right;

        case '-':

            return left - right;

        case '*':

            return left * right;

        case '/':

            if (right == 0) assert (right);
            return left / right;

        case '^':

            return pow (left, right);

        case SIN + OPCODE:

            return sin (right);

        case COS + OPCODE:

            return cos (right);

        case LN + OPCODE:

            if (right == 0) assert (right);

            if (node->left == nullptr) return log (right);
            return log (right) / log (left);

        default:

            printf ("INCORRECT OPCODE!!!\n");
    }

    return 0;
}

#define POISON NodeBinTreeData::VALUE_POISON
#define dL PartialDerivative (node->left,  variable)
#define dR PartialDerivative (node->right, variable)
#define cN BinTreeCopy (node)
#define cL BinTreeCopy (node->left)
#define cR BinTreeCopy (node->right)
#define constL  NodeBinTreeCtor (NodeBinTreeDataCtor (node->left->data->value))
#define constR  NodeBinTreeCtor (NodeBinTreeDataCtor (node->right->data->value))
#define _CONST(x)  NodeBinTreeCtor (NodeBinTreeDataCtor (x))
#define _ADD(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '+'), x, y)
#define _SUB(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '-'), x, y)
#define _MUL(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '*'), x, y)
#define _DIV(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '/'), x, y)
#define _POW(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '^'), x, y)
#define _SIN(x)    NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, SIN + OPCODE), nullptr, x)
#define _COS(x)    NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, COS + OPCODE), nullptr, x)
#define _LN(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, LN  + OPCODE), nullptr, x)

BinTree*
Differentiate (BinTree* tree)
{
    if (tree == nullptr) return nullptr;

    NodeBinTree* node  = nullptr;
    node = PartialDerivative (tree->root, 0);

    if (tree->variable->size != 1)
    {
        node = _POW (node, _CONST (2));

        for (int i = 1; i < tree->variable->size; i++)
        {
            node = _ADD (node, _POW (PartialDerivative (tree->root, i), _CONST (2)));
        }

        node = _POW (node, _CONST (0.5));
    }

    BinTree* dtree = BinTreeCtor (node);
    dtree->buf = tree->buf;
    CopyStack (tree, dtree);

    return dtree;
}

NodeBinTree*
PartialDerivative (NodeBinTree* node, int variable)
{
    if (node == nullptr) return nullptr;

    if (node->data->variable != NodeBinTreeData::VARIABLE_POISON)
    {
        if (variable != node->data->variable) return _CONST (0);
        return _CONST (1);
    }

    if (node->data->opCode == NodeBinTreeData::OPCODE_POISON)
    {
        return _CONST (0);
    }

    switch (node->data->opCode)
    {
        case '+':

            return
            _ADD (dL, dR);

        case '-':

            return
            _SUB (dL, dR);

        case '*':

            return
            _ADD (_MUL (dL, cR),
                  _MUL (cL, dR));

        case '/':

            return
            _DIV (_SUB (_MUL (dL, cR),
                        _MUL (cL, dR)),
                  _POW (cR, _CONST (2)));

        case '^':
                /// a ^ b
            if (IsConstData (node->left) &&
                IsConstData (node->right))
            {
                return _CONST (0);
            }
                /// f(x) ^ a
            if (IsConstData (node->right))
            {
                return
                _MUL (_MUL (constR,
                            _POW (cL, _CONST (node->right->data->value - 1))),
                      dL);
            }
                /// a ^ f(x)
            if (IsConstData (node->left))
            {
                return
                _MUL (_MUL (cN,
                            _LN (constL)),
                      dR);
            }
                /// f(x) ^ g(x)
            return
            _MUL (cN,
                  _ADD (_MUL (dR,
                              _LN (node->left)),
                        _MUL (_DIV (cR, cL),
                              dL)));

        case LN + OPCODE:
                /// ln f(x)
            if (node->left == nullptr)
            {
                return
                _DIV (dR, cR);
            }
                /// log_a  b
            if (IsConstData (node->left) &&
                IsConstData (node->right))
            {
                return _CONST (0);
            }
                /// log_a  f(x)
            if (IsConstData (node->left))
            {
                return
                _DIV (dR,
                      _MUL (cR,
                            _LN (constL)));
            }
                /// log_f(x)  a
            if (IsConstData (node->right))
            {
                return
                _DIV (_MUL (_MUL (_CONST (-1),
                                  _LN (constR)),
                            dR),
                      _MUL (cL,
                            _POW (_LN (constR),
                                  _CONST (2))));
            }
            // log_f(x)  g(x)

        case SIN + OPCODE:

            return _MUL (_COS (node->right),
                         dR);
        case COS + OPCODE:

            return _MUL (_MUL (_CONST (-1),
                               _SIN (node->right)),
                         dR);
    }

    return nullptr;
}

#undef POISON
#undef dL
#undef dR
#undef cL
#undef cR
#undef constL
#undef constR
#undef _CONST
#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV
#undef _POW
#undef _SIN
#undef _COS
#undef _LN

int
GetVariable (BinTree* binTree)
{
    if (binTree == nullptr) return 0;

    for (int i = 0; i < binTree->variable->size; i++)
    {
        printf ("Введите значения переменной: %s = ",
            binTree->variable->data[i].name);
        scanf ("%lg", &binTree->variable->data[i].value);
    }

    return 0;
}

int
IsConstData (NodeBinTree* node)
{
    if (node == nullptr) return 0;

    if (node->data->variable == NodeBinTreeData::VARIABLE_POISON &&
        node->data->opCode   == NodeBinTreeData::OPCODE_POISON)
    {
        return 1;
    }

    return 0;
}

static int
CopyStack (BinTree* tree, BinTree* newTree)
{
    if (tree == nullptr)    return 0;
    if (newTree == nullptr) return 0;

    Variable var = { .name = nullptr, .value = 0 };

    for (int i = 0; i < tree->variable->size; ++i)
    {
        var.name  = tree->variable->data[i].name;
        var.value = tree->variable->data[i].value;

        StackPush (newTree->variable, var);
    }

    /// TO DO
    /// можно копировать не весь стек
    ///(возможно используются не все переменные)

    return 0;
}

