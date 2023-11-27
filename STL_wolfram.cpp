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

NodeBinTree*
Differentiate (NodeBinTree* node)
{
    if (node == nullptr) return nullptr;

    if (node->data->variable != nullptr)
    {
        return NodeBinTreeCtor (NodeBinTreeDataCtor (1, 0, nullptr),
            nullptr, nullptr, nullptr);
    }
    if (node->data->opCode   == 0)
    {
        return NodeBinTreeCtor (NodeBinTreeDataCtor (0, 0, nullptr),
            nullptr, nullptr, nullptr);
    }

    switch (node->data->opCode)
    {
        case '+':
            return
            NodeBinTreeCtor (NodeBinTreeDataCtor (0, '+', nullptr),
                Differentiate (node->left),
                Differentiate (node->right),
                nullptr);
        case '-':
            return
            NodeBinTreeCtor (NodeBinTreeDataCtor (0, '-', nullptr),
                Differentiate (node->left),
                Differentiate (node->right),
                nullptr);
        case '*':
            return
            NodeBinTreeCtor (NodeBinTreeDataCtor (0, '+', nullptr),
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                    Differentiate (node->left),
                    BinTreeCopy (node->right),
                    nullptr),
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                    BinTreeCopy (node->left),
                    Differentiate (node->right),
                    nullptr),
                nullptr);
        case '/':
            return
            NodeBinTreeCtor (NodeBinTreeDataCtor (0, '/', nullptr),
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '-', nullptr),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        Differentiate (node->left),
                        BinTreeCopy (node->right),
                        nullptr),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        BinTreeCopy (node->left),
                        Differentiate (node->right),
                        nullptr),
                    nullptr),
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '^', nullptr),
                    BinTreeCopy (node->right),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (2, 0, nullptr),
                        nullptr, nullptr, nullptr),
                    nullptr),
                nullptr);
        case '^':
            if (node->left ->data->variable == nullptr &&
                node->left ->data->opCode   == 0 &&
                node->right->data->variable == nullptr &&
                node->right->data->opCode   == 0)
            {   // a ^ b
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, 0, nullptr),
                    nullptr, nullptr, nullptr);;
            }
            if (node->right->data->variable == nullptr &&
                node->right->data->opCode   == 0)
            {   // f(x) ^ a
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        NodeBinTreeCtor (NodeBinTreeDataCtor (node->right->data->value, 0, nullptr),
                            nullptr, nullptr, nullptr),
                        NodeBinTreeCtor (NodeBinTreeDataCtor (0, '^', nullptr),
                            BinTreeCopy (node->left),
                            NodeBinTreeCtor (NodeBinTreeDataCtor (node->right->data->value - 1, 0, nullptr),
                                nullptr, nullptr, nullptr)),
                        nullptr),
                    Differentiate (node->left),
                    nullptr);
            }
            if (node->left->data->variable == nullptr &&
                node->left->data->opCode   == 0)
            {   // a ^ f(x)
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        BinTreeCopy (node),
                        NodeBinTreeCtor (NodeBinTreeDataCtor (0, 'l', nullptr),
                            nullptr,
                            NodeBinTreeCtor (NodeBinTreeDataCtor (node->left->data->value, 0, nullptr),
                                nullptr, nullptr, nullptr),
                            nullptr),
                        nullptr),
                    Differentiate (node->right),
                    nullptr);
            }
            // f(x) ^ g(x)
        case 'l':
            if (node->left == nullptr)
            {   // ln f(x)
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '/', nullptr),
                    Differentiate (node->right),
                    BinTreeCopy (node->right),
                    nullptr);
            }
            if (node->left ->data->variable == nullptr &&
                node->left ->data->opCode   == 0 &&
                node->right->data->variable == nullptr &&
                node->right->data->opCode   == 0)
            {   // log_a  b
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, 0, nullptr),
                    nullptr, nullptr, nullptr);
            }
            if (node->left->data->variable == nullptr &&
                node->left->data->opCode   == 0)
            {   // log_a  f(x)
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '/', nullptr),
                    Differentiate (node->right),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        BinTreeCopy (node->right),
                        NodeBinTreeCtor (NodeBinTreeDataCtor (0, 'l', nullptr),
                            nullptr,
                            NodeBinTreeCtor (NodeBinTreeDataCtor (node->left->data->value, 0, nullptr),
                                nullptr, nullptr, nullptr),
                            nullptr),
                        nullptr),
                    nullptr);
            }
            if (node->right->data->variable == nullptr &&
                node->right->data->opCode   == 0)
            {   // log_f(x)  a
                return
                NodeBinTreeCtor (NodeBinTreeDataCtor (0, '/', nullptr),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                            NodeBinTreeCtor (NodeBinTreeDataCtor (-1, 0, nullptr),
                                nullptr, nullptr, nullptr),
                            NodeBinTreeCtor (NodeBinTreeDataCtor (0, 'l', nullptr),
                                nullptr,
                                NodeBinTreeCtor (NodeBinTreeDataCtor (node->right->data->value, 0, nullptr),
                                    nullptr, nullptr, nullptr),
                                nullptr),
                            nullptr),
                        Differentiate (node->right),
                        nullptr),
                    NodeBinTreeCtor (NodeBinTreeDataCtor (0, '*', nullptr),
                        BinTreeCopy (node->left),
                        NodeBinTreeCtor (NodeBinTreeDataCtor (0, '^', nullptr),
                            NodeBinTreeCtor (NodeBinTreeDataCtor (0, 'l', nullptr),
                                nullptr,
                                NodeBinTreeCtor (NodeBinTreeDataCtor (node->right->data->value, 0, nullptr),
                                    nullptr, nullptr, nullptr),
                                nullptr),
                            NodeBinTreeCtor (NodeBinTreeDataCtor (2, 0, nullptr),
                                nullptr, nullptr, nullptr),
                            nullptr),
                        nullptr),
                    nullptr);
            }
            // log_f(x)  g(x)

    }

    return nullptr;

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
