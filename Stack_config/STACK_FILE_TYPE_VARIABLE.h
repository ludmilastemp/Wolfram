#ifndef STACK_FILE_TYPE_Variable_
#define STACK_FILE_TYPE_Variable_

//#include "STL_wolfram.h"
struct Variable
{
    const char* name;
    double value;
};

#define STACK_T Variable
#define STACK Stack_Variable
//#define STACK_INCORRECT_DATA nullptr

#endif /* STACK_FILE_TYPE_Variable_ */
