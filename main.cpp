#include "../Akinator/Bintree/STL_bintree_struct.h"
#include "STL_wolfram.h"
#include "Graphviz/STL_graphviz.h"

int main (const int argc, const char** argv)
{
    File file = { .name = argv[1] };
    BinTree* tree = BinTreeReadInorderWithoutNil (&file);

    printf ("\n\nREAD OK!\n");

    Stack_Variable stk = { 0 };
    StackCtor (&stk);

    GetVariable (tree->root, &stk);
    printf ("Answer: %lg\n", Eval (tree->root, &stk));

    StackDtor (&stk);

    STL_GraphvizBinTree (tree->root);
//    STL_GraphvizBinTree1 (tree->root);

    printf ("\n\nOK!\n");

    return 0;
}
