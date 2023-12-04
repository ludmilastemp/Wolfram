#ifndef STL_latex_
#define STL_latex_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "../../Akinator/BinTree/STL_bintree_node_struct.h"
#include "../../Akinator/Bintree/STL_bintree_struct.h"

const int LATEX_MAX_LEN = 20;

void
STL_Latex (BinTree* binTree, const char* const str);

#endif /* STL_latex_ */
