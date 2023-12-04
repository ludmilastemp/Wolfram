#ifndef STL_simplify_
#define STL_simplify_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "../../Akinator/BinTree/STL_bintree_node_struct.h"
#include "../../Akinator/BinTree/STL_bintree_struct.h"
#include "../STL_wolfram.h"

NodeBinTree*
STL_SimplifyEval (NodeBinTree* node);

int
STL_FoldConst (NodeBinTree* node);

NodeBinTree*
STL_DeleteNeutralNode (NodeBinTree* node);

#endif /* STL_simplify_ */
