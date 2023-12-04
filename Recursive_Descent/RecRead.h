#ifndef STL_RecRead_
#define STL_RecRead_

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "../../Akinator/Bintree/STL_bintree_node_struct.h"
#include "../../Akinator/Bintree/STL_bintree_struct.h"

typedef NodeBinTree* DATA_T;

struct RecursDescent
{
    char* str;
    int pos;
    int error;
    Stack_Variable* stk;
};

BinTree* GetG (struct File* file);

#endif /* STL_RecRead_ */
