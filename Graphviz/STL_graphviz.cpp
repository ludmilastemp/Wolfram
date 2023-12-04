#include "STL_graphviz.h"

#define pr(...) fprintf (fp, __VA_ARGS__)

static void PrintSubtree (FILE* fp, const NodeBinTree* node, int* num, Stack_Variable* stk);

void STL_GraphvizBinTree (BinTree* tree)
{
    FILE* fp = fopen ("Graphviz/STL_graphviz_png.dot", "w");
    assert (fp);

    pr ("digraph STL\n{\n"
        "rankdir = TB\n"
        "graph [ splines = ortho, splines = true ]\n"
        "node  [ shape = record, style = filled, fillcolor = \"#E0FFFF\", color = \"#A0FFFF\" ]\n"
        "edge  [ arrowhead = none ]\n\n");

    int num = 0;

    PrintSubtree (fp, tree->root, &num, tree->variable);

    pr ("\n}\n");

    fclose (fp);

}

static void PrintSubtree (FILE* fp, const NodeBinTree* node, int* num, Stack_Variable* stk)
{
    fprintf (fp, "\nf%d", *num);

    int curNum = *num;

    if (node->data->variable != NodeBinTreeData::VARIABLE_POISON)
        pr (" [ label = \"" BIN_TREE_DATA_VARIABLE_PRINT_SPECIFIER "\", \
        fillcolor = \"#FFE0FF\", color = \"#FFA0FF\" ]\n", stk->data[node->data->variable].name);
    else
    if (node->data->opCode == NodeBinTreeData::OPCODE_POISON)
        pr (" [ label = " BIN_TREE_DATA_VALUE_PRINT_SPECIFIER ", \
        fillcolor = \"#E0FFE0\", color = \"#ACFFAC\" ]\n", node->data->value);
    else  {

//        printf ("OPCODE = %d\n", node->data->opCode);
        switch (node->data->opCode)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':

                pr (" [ label = \"" BIN_TREE_DATA_OPCODE_PRINT_SPECIFIER "\", \
                fillcolor = \"#E0FFFF\", color = \"#A0FFFF\" ]\n", node->data->opCode);

                break;

            case SIN + OPCODE:
            case COS + OPCODE:
            case LN  + OPCODE:

                pr (" [ label = \" %s \", \
                fillcolor = \"#E0FFFF\", color = \"#A0FFFF\" ]\n", operation[node->data->opCode - OPCODE]);

                break;

            default:

                printf ("APAPAPAPAPAPAPPPPAPAAPAPAP");
        }
    }

    (*num)++;

    if (node->left != nullptr)
    {
        fprintf (fp, "f%d->f%d \n", curNum, *num);
        PrintSubtree (fp, node->left, num, stk);
    }

    if (node->right != nullptr)
    {
        fprintf (fp, "f%d->f%d \n", curNum, *num);
        PrintSubtree (fp, node->right, num, stk);
    }

    (*num)++;
}

#undef pr
