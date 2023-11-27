#include "STL_wolfram.h"

static Variable*
EvalVariable (NodeBinTree* node, Stack_Variable* stk);

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
