#include "STL_graphviz.h"

#define pr(...) fprintf (fp, __VA_ARGS__)

static void PrintSubtree (FILE* fp, const NodeBinTree* node, int* num);

int STL_GraphvizBinTree1 (const NodeBinTree* node)
{
    FILE* fp = fopen ("STL_graphviz_png.dot", "w");
    assert (fp);

    pr ("digraph STL\n{\n"
        "rankdir = TB\n"
        "graph [ splines = ortho, splines = true ]\n"
        "node  [ shape = record, style = filled, fillcolor = \"#E0FFFF\", color = \"#A0FFFF\" ]\n"
        "edge  [ arrowhead = none ]\n\n");

    int num = 0;

    PrintSubtree (fp, node, &num);

    pr ("\n}\n");

    fclose (fp);

}

static void PrintSubtree (FILE* fp, const NodeBinTree* node, int* num)
{
    fprintf (fp, "\nf%d", *num);

    int curNum = *num;

    if (node->data->opCode == 0)
        pr (" [ label = " BIN_TREE_DATA_VALUE_PRINT_SPECIFIER ", \
        fillcolor = \"#CCFFCC\", color = \"#CCFFCC\" ]\n", node->data->value);
    else
        pr (" [ label = \"" BIN_TREE_DATA_OPCODE_PRINT_SPECIFIER "\", \
        fillcolor = \"#E0FFFF\", color = \"#A0FFFF\" ]\n", node->data->opCode);

    if (node->left != nullptr)
    {
        (*num)++;
        fprintf (fp, "f%d->f%d \n", curNum, *num);
        PrintSubtree (fp, node->left, num);
    }

    if (node->right != nullptr)
    {
        fprintf (fp, "f%d->f%d \n", curNum, *num);
        PrintSubtree (fp, node->right, num);
    }

    (*num)++;
}

#undef pr
