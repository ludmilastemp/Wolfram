#include "STL_simplify.h"

static int
CountNodes (NodeBinTree* node);

NodeBinTree*
STL_SimplifyEval (NodeBinTree* node)
{
    if (node == nullptr) return nullptr;

    int initialnNode = CountNodes (node);
    int nNode        = initialnNode + 1;
    int newNode      = initialnNode;
    int nSimplify    = 0;

    printf ("\n\nБыло узлов: %d\n\n", initialnNode);

    while (newNode < nNode)
    {
        nNode = newNode;
        STL_FoldConst (node);
        newNode = CountNodes (node);

        if (newNode < nNode)
        {
            printf ("Проведено STL_FoldConst\n");
            printf ("Стало узлов: %d\n\n", newNode);
            nSimplify++;
        }

        nNode = newNode;
        node    = STL_DeleteNeutralNode (node);
        newNode = CountNodes (node);

        if (newNode < nNode)
        {
            printf ("Проведено STL_DeleteNeutralNode\n");
            printf ("Стало узлов: %d\n\n", newNode);
            nSimplify++;
        }
    }

    printf ("\nПроведено упрощений %d\n", nSimplify);
    printf ("Количество узлов уменьшиловь на %d\n\n", initialnNode - newNode);

    return node;
}

int
STL_FoldConst (NodeBinTree* node)
{
    if (node == nullptr) return 0;

    if (node->data->opCode == 0)
    {
        if (node->data->variable != -1)
        {
            return 1;
        }

        return 0;
    }

    int left  = STL_FoldConst (node->left);
    int right = STL_FoldConst (node->right);

    if (left == 0 && right == 0)
    {
        node->data->value  = Eval (node, nullptr);
        node->data->opCode = 0;

        node->left  = NodeBinTreeDtor (node->left);
        node->right = NodeBinTreeDtor (node->right);

        return 0;
    }

    return 1;
}

#define POISON     NodeBinTreeData::VALUE_POISON
#define dataL      node->left->data->value
#define dataR      node->right->data->value
#define _CONST(x)  NodeBinTreeCtor (NodeBinTreeDataCtor (x))
#define _MUL(x, y) NodeBinTreeCtor (NodeBinTreeDataCtor (POISON, '*'), x, y)

NodeBinTree*
STL_DeleteNeutralNode (NodeBinTree* node)
{
    if (node == nullptr) return 0;
    if (node->data->opCode == 0) return node;

    node->left  = STL_DeleteNeutralNode (node->left);
    node->right = STL_DeleteNeutralNode (node->right);

    switch (node->data->opCode)
    {
        case '+':

            if (IsConstData (node->left))
            {
                if (dataL == 0)
                    return node->right;
            }

            if (IsConstData (node->right))
            {
                if (dataR == 0)
                    return node->left;
            }

            break;

        case '-':

            if (IsConstData (node->right))
            {
                if (dataR == 0)
                    return node->left;
            }

            if (IsConstData (node->left))
            {
                if (dataL == 0)
                    return _MUL (node->right, _CONST (-1));
            }

            break;

        case '*':

            if (IsConstData (node->left))
            {
                if (dataL == 1)
                    return node->right;

                if (dataL == 0)
                    return _CONST (0);
            }

            if (IsConstData (node->right))
            {
                if (dataR == 1)
                    return node->left;

                if (dataR == 0)
                    return _CONST (0);

                if (dataR == -1)
                    return _MUL (_CONST (-1), node->left);
            }

            break;

        case '/':

            if (IsConstData (node->left))
            {
                if (dataL == 0)
                    return _CONST (0);
            }

            break;

        case '^':

            if (IsConstData (node->left))
            {
                if (dataL == 1)
                    return _CONST (1);

                if (dataL == 0)
                    return _CONST (0);
            }

            if (IsConstData (node->right))
            {
                if (dataR == 1)
                    return node->left;

                if (dataR == 0)
                    return _CONST (1);
            }

            break;

        case LN + OPCODE:

            if (IsConstData (node->right))
            {
                if (dataR == 1)
                    return _CONST (0);
            }

            break;

        case SIN + OPCODE:
        case COS + OPCODE:

            break;

        default:
            printf ("INCORRECT OPCODE!!!\n");
    }

    return node;
}

#undef POISON
#undef dataL
#undef dataR
#undef _CONST
#undef _MUL

static int
CountNodes (NodeBinTree* node)
{
    if (node == nullptr) return 0;

    return CountNodes (node->left) + CountNodes (node->right) + 1;
}

//int
//IsConstData (NodeBinTree* node);
