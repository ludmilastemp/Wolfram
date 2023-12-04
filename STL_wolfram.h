#ifndef STL_wolfram_
#define STL_wolfram_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "../Akinator/Bintree/STL_bintree_node_struct.h"
#include "../Akinator/Bintree/STL_bintree_struct.h"

double
Eval (NodeBinTree* node, Stack_Variable* stk);

int
GetVariable (BinTree* binTree);

BinTree*
Differentiate (BinTree* binTree);

NodeBinTree*
PartialDerivative (NodeBinTree* node, int variable);

int
IsConstData (NodeBinTree* node);

#endif /* STL_wolfram_ */
