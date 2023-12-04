#include "STL_latex.h"

//#define pr(...) fprintf (fp, __VA_ARGS__)
#define pr(...) sprintf (str + pos, __VA_ARGS__);\
                pos = strlen (str)

static char str[1000]   = { 0 };   // как объединить эти 2???
static int        pos   = 0;       // - постоянный strlen
static int      nIndex  = 0;       // stack->size
static char* index[100] = { 0 };   // stack

static double
PrintSubtree (FILE* fp, NodeBinTree* node, Stack_Variable* stk);

static double
PrintSubtreeWithBrackets (FILE* fp, NodeBinTree* node, Stack_Variable* stk);

static int
IsOperation (FILE* fp, const NodeBinTree* node, Stack_Variable* stk);

static double
RecCountStrlenLatex (NodeBinTree* node, Stack_Variable* stk);

static double
CountStrlenLatex (int sign, double left, double right);

static int isOpen = 0;

void STL_Latex (BinTree* tree, const char* const name)
{
    FILE* fp = nullptr;

    if (!isOpen){
        fp = fopen ("Latex/STL_latex_txt.dot", "w");
        assert (fp);

        fprintf (fp, "\
            \n\\Large\
            \n\\title{Добро пожаловать}\
            \n\\author{Стемпоржецкая Л.В. \\\\ Б05-331}\
            \n\\maketitle");

        isOpen = 1;
    }

    else
    {
        fp = fopen ("Latex/STL_latex_txt.dot", "a");
        assert (fp);
    }

    pos  = 0;
    nIndex = 0;

    fprintf (fp, "\n\n%s\n", name);

    if (tree == nullptr)
    {
        fclose (fp);
        return;
    }

    fprintf (fp, "\\begin{equation*}\n");

//    printf ("\tlatex len rec = %lg\n", RecCountStrlenLatex (tree->root, tree->variable));
//    printf ("\tlatex len sub = %lg\n", PrintSubtree (fp, tree->root, tree->variable));

    PrintSubtree (fp, tree->root, tree->variable);

    /// обработка выражений вида y = A, где A = ...
    if (str [1] == nIndex - 1 + 'A' && pos == 3)
    {
//        str = index[--nIndex];
        strcpy (str, index[--nIndex]);
        pos = strlen (str);
    }


    if (nIndex) pr (", где ");
    for (int i = 0; i < nIndex; i++)
    {
        pr ("\\end{equation*}\n");
        pr ("\\begin{equation*}\n");

        pr ("\t%c = %s\n", 'A' + i, index[i]);

        free (index[i]);
    }

    fprintf (fp, "%s", str);

    fprintf (fp, "\n\\end{equation*}\n");


//    printf ("\n\n%s\n\n", str);
//    printf ("\n\n%d\n\n", pos);

    fclose (fp);
}

static double
PrintSubtree (FILE* fp, NodeBinTree* node, Stack_Variable* stk)
{
    if (fp == nullptr || node == nullptr) return 0;

//    printf ("%d %s \n", pos, str);

    int lenData = IsOperation (fp, node, stk);
    if (lenData != 0) return lenData;

    int oldPos = pos;

    int left  = 0;
    int right = 0;

    switch (node->data->opCode)
    {
        case '+':

            left = PrintSubtree (fp, node->left, stk);

            pr ("+");

            right = PrintSubtree (fp, node->right, stk);

            break;

        case '-':

            left = PrintSubtree (fp, node->left, stk);

            pr ("-");

            right = PrintSubtreeWithBrackets (fp, node->right, stk);

            break;

        case '*':

            left = PrintSubtreeWithBrackets (fp, node->left, stk);

            /// const * variable without '*' f.e. 7x + 5b
            if (node->left->data->opCode    == NodeBinTreeData::OPCODE_POISON &&
                node->left->data->variable  == NodeBinTreeData::VARIABLE_POISON &&
                node->right->data->variable != NodeBinTreeData::VARIABLE_POISON) ;
            else pr ("\\cdot ");

            right = PrintSubtreeWithBrackets (fp, node->right, stk);

            break;

        case '/':

            pr ("\\frac{");

            left = PrintSubtree (fp, node->left, stk);

            pr ("}{");

            right = PrintSubtree (fp, node->right, stk);

            pr ("}");

            break;

        case '^':

            pr ("{");

            if (node->left->data->opCode != 0) { pr ("("); }

            left = PrintSubtree (fp, node->left, stk);

            if (node->left->data->opCode != 0) { pr (")"); }

            pr ("}^{");

            right = PrintSubtree (fp, node->right, stk);

            pr ("}");

            break;

        case SIN + OPCODE:

            pr ("sin(");

            right = PrintSubtree (fp, node->right, stk);

            pr (")");

            break;

        case COS + OPCODE:

            pr ("cos(");

            right = PrintSubtree (fp, node->right, stk);

            pr (")");

            break;

        case LN + OPCODE:

            if (node->left != 0)
            {
                pr ("\\log_", node->data->opCode);

                if (node->left->data->opCode != 0) pr ("{");

                left = PrintSubtree (fp, node->left, stk);

                if (node->left->data->opCode != 0) pr ("}");
            }

            else pr ("\\ln", node->data->opCode);

            if (node->right->data->opCode != 0) pr ("(");

            right = PrintSubtree (fp, node->right, stk);

            if (node->right->data->opCode != 0) pr (")");

            break;
    }

    double len = CountStrlenLatex (node->data->opCode, left, right);

    /// проверка, нужно ли делать замену
    if (len > LATEX_MAX_LEN)
    {
        index[nIndex] = (char*) calloc (pos - oldPos + 1, sizeof (char));
        if (index[nIndex] == nullptr)
        {
            printf ("ERROR_NO_MEMORY\n");
            return 0;
        }

        strcpy (index[nIndex], str + oldPos);

        pos = oldPos;
        pr (" %c ", 'A' + nIndex);

        nIndex++;
        return 1;
    }

    return len;
}

static double
PrintSubtreeWithBrackets (FILE* fp, NodeBinTree* node, Stack_Variable* stk)
{
    if (fp == nullptr || node == nullptr) return 0;

    int len = 0;

    switch (node->data->opCode)
    {
        case 0:

            len = IsOperation (fp, node, stk);

            break;

        case '+':
        case '-':

            pr ("(");

            len = PrintSubtree (fp, node, stk);
            len += 2;    /// две круглые скобки

            pr (")");

            break;

        case '*':
        case '/':
        case '^':
        case SIN + OPCODE:
        case COS + OPCODE:
        case LN  + OPCODE:

            len = PrintSubtree (fp, node, stk);

            break;
    }

    return len;
}

static int
IsOperation (FILE* fp, const NodeBinTree* node, Stack_Variable* stk)
{
/* функция
 * если fp == nullptr
 *      0 - узел не является оператором
 *      1 - узел является оператором
 * если fp != nullptr
 *      0 - узел является оператором
 *      иначе длина данных узла в строке
 */

    if (node == nullptr) return 0;

    if (node->data->variable != NodeBinTreeData::VARIABLE_POISON)
    {
        if (fp != nullptr && node != nullptr)
        {
            pr (" %s ", stk->data[node->data->variable].name);
            return strlen (stk->data[node->data->variable].name);
        }

        return 0;
    }

    if (node->data->opCode == NodeBinTreeData::OPCODE_POISON)
    {
        if (fp != nullptr && node != nullptr)
        {
            pr (" %lg ", node->data->value);

            char len_value[100] = { 0 };
            sprintf (len_value, "%lg", node->data->value);
            return strlen (len_value);
        }

        return 0;
    }

    if (fp != nullptr) return 0;

    return 1;
}

static double
CountStrlenLatex (int sign, double left, double right)
{
//    printf ("sign  = %d\n",  sign);
//    printf ("left  = %lg\n", left);
//    printf ("right = %lg\n\n\n", right);

    switch (sign)
    {
        case '+':
        case '-':
        case '*':

            return left + right + 1;

        case '/':

            if (left < right) return right;
            return left;

        case '^':

            return left + right * 0.71;

        case SIN + OPCODE:
        case COS + OPCODE:

            return 3 + right;

        case LN + OPCODE:

            return 2 + left * 0.5 + right;

        default:
            printf ("INCORRECT OPCODE!!!\n");
    }

    return 0;
}

static double
RecCountStrlenLatex (NodeBinTree* node, Stack_Variable* stk)
{
    if (node == nullptr) return 0;
    if (stk  == nullptr) return 0;

    double left  = RecCountStrlenLatex (node->left,  stk);
    double right = RecCountStrlenLatex (node->right, stk);

    if (!IsOperation (nullptr, node, nullptr))
    {
        if (node->data->variable != NodeBinTreeData::VARIABLE_POISON)
        {
//            printf ("name len = %d\n", strlen (stk->data[node->data->variable].name));
            return strlen (stk->data[node->data->variable].name);
        }
        else
        {
            char len_value[100] = { 0 };
            sprintf (len_value, "%lg", node->data->value);

//            printf ("value len = %d\n", strlen (len_value));
            return strlen (len_value);
        }
    }

    switch (node->data->opCode)
    {
        case '+':
        case '-':
        case '*':

            return left + right + 1;

        case '/':

            if (left < right) return right;
            return left;

        case '^':

            return left + right * 0.7;

        case SIN + OPCODE:
        case COS + OPCODE:

            return 3 + right;

        case LN + OPCODE:

            return 2 + left * 0.5 + right;

        default:
            printf ("INCORRECT OPCODE!!!\n");
    }

    return 0;
}

#undef pr
