#ifndef STL_wolfram_
#define STL_wolfram_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "../Akinator/Bintree/STL_bintree_node_struct.h"
#include "Stack_config/STACK_FILE_TYPE_VARIABLE.h"
#include "../Stack/generic.h"

double
Eval (NodeBinTree* node, Stack_Variable* stk);

int
GetVariable (NodeBinTree* node, Stack_Variable* stk);


#endif /* STL_wolfram_ */
