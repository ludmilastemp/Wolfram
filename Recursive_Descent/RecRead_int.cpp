// 22:49
#include "RecRead.h"
// 23:05

typedef double DATA_T;

static DATA_T GetExspretion  (RecursDescent* recDescent);
static DATA_T GetT           (RecursDescent* recDescent);  //
static DATA_T GetUnary       (RecursDescent* recDescent);
static DATA_T GetParenthesis (RecursDescent* recDescent);
static DATA_T GetNumber      (RecursDescent* recDescent);

DATA_T GetG (const char* str)
{
    RecursDescent recDescent = { .str = str, .pos = 0, .error = 0 };

    return GetExspretion (&recDescent);
}

DATA_T GetExspretion (RecursDescent* recDescent)
{
    DATA_T val = GetT (recDescent);

    while (recDescent->str[recDescent->pos] == '+' ||
           recDescent->str[recDescent->pos] == '-')
    {
        char op = recDescent->str[recDescent->pos];
        recDescent->pos++;

        DATA_T val2 = GetT (recDescent);

        switch (op)
        {
            case '+':

                val += val2;

                break;

            case '-':

                val -= val2;

                break;

            default:
                assert (false);
        }
    }

    return val;
}

DATA_T GetT (RecursDescent* recDescent)
{
    DATA_T val = GetUnary (recDescent);

    while (recDescent->str[recDescent->pos] == '*' ||
           recDescent->str[recDescent->pos] == '/' ||
           recDescent->str[recDescent->pos] == '^')
    {
        char op = recDescent->pos[recDescent->str];
        recDescent->pos++;

        DATA_T val2 = GetUnary (recDescent);

        switch (op)
        {
            case '*':

                val *= val2;

                break;

            case '/':

                val /= val2;

                break;

            case '^':

                val = pow (val, val2);

                break;

            default:

                assert (false);
        }
    }

    return val;
}

const int      nOperation   = 3;
const char*    operation [] = {"sin", "cos", "ln"};
int         lenOperation [] = {3, 3, 2};

enum Operation
{
    SIN = 0,
    COS = 1,
    LN  = 2,
};

DATA_T GetUnary (RecursDescent* recDescent)
{
    int i = 0;
    for (; i < nOperation; i++)
    {
        if (strncmp (recDescent->str + recDescent->pos,
                     operation[i], lenOperation[i]) == 0)
        {
            recDescent->pos += lenOperation [i];
            break;
        }
    }

    DATA_T val = GetParenthesis (recDescent);

    switch (i)
    {
        case SIN:

            val = sin (val);

            break;

        case COS:

            val = cos (val);

            break;

        case LN:

            val = log (val);

            break;

        default:

            break;
    }

    return val;
}

DATA_T GetParenthesis (RecursDescent* recDescent)
{
    if (recDescent->str[recDescent->pos] == '(')
    {
        recDescent->pos++;

        DATA_T val = GetExspretion (recDescent);

        assert (recDescent->str[recDescent->pos] == ')');
        recDescent->pos++;

        return val;
    }

    return GetNumber (recDescent);
}

DATA_T GetNumber (RecursDescent* recDescent)
{
    int old_p  = recDescent->pos;
    DATA_T val = 0;

//    int sign   = 1;
//
//    if (recDescent->str[recDescent->pos] == '-')
//    {
//        sign = -1;
//        recDescent->pos++;
//    }
//
//    while ('0' <= recDescent->str[recDescent->pos] && recDescent->str[recDescent->pos] <= '9')
//    {
//        val = val * 10 + recDescent->str[recDescent->pos] - '0';
//        recDescent->pos++;
//    }
//
//    assert (recDescent->pos > old_p);
//
//    return val * sign;

    int nLen = 0;
    sscanf (recDescent->str + recDescent->pos, "%lg %n", &val, &nLen);

    if (nLen)
    {
        recDescent->pos += nLen;
    }

    assert (recDescent->pos > old_p);

    return val;
}
