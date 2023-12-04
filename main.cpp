#include "../Akinator/Bintree/STL_bintree_struct.h"
#include "STL_wolfram.h"
#include "Graphviz/STL_graphviz.h"
#include "Latex/STL_latex.h"
#include "Simplify/STL_simplify.h"
#include "Recursive_Descent/RecRead.h"
#include "STL_Explore.h"

#include <stdio.h>

int main (const int argc, const char** argv)
{
    File file = { .name = argv[1] };

    BinTree* tree = GetG (&file);
//    BinTree* tree = BinTreeReadInorderWithoutNil (&file);
    if (tree == nullptr) { printf ("TREE NULL\n"); return 0; }
    printf ("\nREAD OK!\n\n\n");

    STL_Explore (tree);

//    GetVariable (tree);  // in func
                           //in struct (было / не было)
//    printf ("Answer: %lg\n", Eval (tree->root, tree->variable));

//    STL_SimplifyEval (tree->root);
//    STL_Latex (tree, "Изначальное уравнение:");
//    STL_GraphvizBinTree (tree);
//
//    BinTree* dtree = Differentiate (tree);
//    printf ("Answer: %lg\n", Eval (dtree->root, tree->variable));
//
//    STL_SimplifyEval (dtree->root);
//    STL_Latex (dtree, "Первая производная:");
//    STL_GraphvizBinTree (dtree);

    BinTreeDtor (tree);

//    dtree->buf = nullptr;
//    BinTreeDtor (dtree);

    printf ("\n\nOK!\n");

    return 0;
}

