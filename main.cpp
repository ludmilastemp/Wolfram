#include "../Akinator/Bintree/STL_bintree_struct.h"
#include "STL_graphviz.h"

int main (const int argc, const char** argv)
{
    File file = { .name = argv[1] };
    BinTree* tree = BinTreeReadInorderWithoutNil (&file);

    STL_GraphvizBinTree1 (tree->root);

    printf ("\n\nOK!");

    return 0;
}
