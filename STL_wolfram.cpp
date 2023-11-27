#include "STL_wolfram.h"

static Variable*
EvalVariable (NodeBinTree* node, Stack_Variable* stk);

static int
IsConstData (NodeBinTree* node);

double
Eval (NodeBinTree* node, Stack_Variable* stk)
{
    if (node == nullptr) return 0;

    if (node->data->variable != nullptr)
    {
        return EvalVariable (node, stk) ->value;
    }

    double left  = Eval (node->left,  stk);
    double right = Eval (node->right, stk);

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
            return left / right;
        case '^':
            return pow (left, right);
        case 'l':
            if (node->left == nullptr) return log (right);
            return log (right) / log (left);
        default:
            printf ("INCORRECT OPCODE!!!\n");
    }

    return 0;
}

int
GetVariable (NodeBinTree* node, Stack_Variable* stk)
{
    if (node == nullptr) return 0;

    GetVariable (node->left,  stk);
    GetVariable (node->right, stk);

    if (node->data->variable     == nullptr) return 0;
    if (EvalVariable (node, stk) != nullptr) return 0;

    Variable var = { .name = node->data->variable, .value = 0};

    printf ("Введите значения переменной: %s = ", var.name);
    scanf ("%lg", &var.value);

    StackPush (stk, var);

    return 0;
}

#define d(x) Differentiate (x)
#define c(x) BinTreeCopy (x)
#define _CONST(x)  NodeBinTreeCtor (NodeBinTreeDataCtor (x, 0, nullptr), nullptr, nullptr, nullptr)
#define _ADD(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (0, '+', nullptr), x, y, nullptr)
#define _SUB(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (0, '-', nullptr), x, y, nullptr)
#define _MUL(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr), x, y, nullptr)
#define _DIV(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (0, '/', nullptr), x, y, nullptr)
#define _POW(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (0, '^', nullptr), x, y, nullptr)
#define _LN(x)     NodeBinTreeCtor (NodeBinTreeDataCtor (0, 'l', nullptr), nullptr, x, nullptr)

NodeBinTree*
Differentiate (NodeBinTree* node)
{
    if (node == nullptr) return nullptr;

    if (node->data->variable != nullptr)
    {
        return _CONST (1);
    }
    if (node->data->opCode   == 0)
    {
        return _CONST (0);
    }

    switch (node->data->opCode)
    {
        case '+':
            return
            _ADD (d(node->left), d(node->right));
        case '-':
            return
            _SUB (d(node->left), d(node->right));
        case '*':
            return
            _ADD (_MUL (d(node->left), c(node->right)),
                  _MUL (c(node->left), d(node->right)));
        case '/':
            return
            _DIV (_SUB (_MUL (d(node->left), c(node->right)),
                        _MUL (c(node->left), d(node->right))),
                  _POW (c(node->right), _CONST (2)));
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
                _MUL (_MUL (_CONST (node->right->data->value),
                            _POW (c(node->left), _CONST (node->right->data->value - 1))),
                      d(node->left));
            }
                /// a ^ f(x)
            if (IsConstData (node->left))
            {
                return
                _MUL (_MUL (c(node),
                            _LN (_CONST (node->left->data->value))),
                      d(node->right));
            }
            // f(x) ^ g(x)
        case 'l':
                /// ln f(x)
            if (node->left == nullptr)
            {
                return
                _DIV (d(node->right), c(node->right));
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
                _DIV (d(node->right),
                      _MUL (c(node->right),
                            _LN (_CONST (node->left->data->value))));
            }
                /// log_f(x)  a
            if (IsConstData (node->right))
            {
                return
                _DIV (_MUL (_MUL (_CONST (-1),
                                  _LN (_CONST (node->right->data->value))),
                            d(node->right)),
                      _MUL (c(node->left),
                            _POW (_LN (_CONST (node->right->data->value)),
                                  _CONST (2))));
            }
            // log_f(x)  g(x)
    }

    return nullptr;
}

#undef d
#undef c
#undef _CONST
#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV
#undef _POW
#undef _LN

static Variable*
EvalVariable (NodeBinTree* node, Stack_Variable* stk)
{
    if (node == nullptr || stk == nullptr) return nullptr;

    for (int i = 0; i < stk->size; i++)
    {
        if (strcmp (node->data->variable, stk->data[i].name) == 0)
        {
            return &(stk->data[i]);
        }
    }

    return nullptr;
}

static int
IsConstData (NodeBinTree* node)
{
    if (node == nullptr) return 0;

    if (node->data->variable == nullptr &&
        node->data->opCode   == 0)         return 1;

    return 0;
}

