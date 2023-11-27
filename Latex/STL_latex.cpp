#include "STL_latex.h"

#define pr(...) fprintf (fp, __VA_ARGS__)

static int
PrintSubtree (FILE* fp, const NodeBinTree* node);

static int
PrintSubtreeWithBrackets (FILE* fp, const NodeBinTree* node);

static int
IsOperation (FILE* fp, const NodeBinTree* node);

int STL_Latex (const NodeBinTree* node)
{
    FILE* fp = fopen ("Latex/STL_latex_txt.dot", "w");
    assert (fp);

    pr ("\\documentclass{article}\
         \n\\usepackage{graphicx}\
         \n\\usepackage{amssymb}\
         \n\\usepackage[english]{babel}\
         \n\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\
         \n\\usepackage{amsmath}\
         \n\\usepackage{graphicx}\
         \n\\usepackage[colorlinks=true, allcolors=blue]{hyperref}");

    pr ("\\begin{document}\
         \n\\begin{equation*}\n");

    PrintSubtree (fp, node);

    pr ("\n\\end{equation*}\
         \n\\end{document}\n");

    fclose (fp);

    return 0; //void
}

static int
PrintSubtree (FILE* fp, const NodeBinTree* node)
{
    if (fp == nullptr || node == nullptr) return 0;

    if (!IsOperation (fp, node)) return 0;

    switch (node->data->opCode)
    {
        case '+':
            PrintSubtree (fp, node->left);
            pr ("+", node->data->opCode);
            PrintSubtree (fp, node->right);
            break;
        case '-':
            PrintSubtree (fp, node->left);
            pr ("-", node->data->opCode);
            PrintSubtreeWithBrackets (fp, node->right);
            break;
        case '*':
            PrintSubtreeWithBrackets (fp, node->left);
            pr ("*", node->data->opCode);
            PrintSubtreeWithBrackets (fp, node->right);
            break;
        case '/':
            pr ("\\frac{", node->data->opCode);
            PrintSubtree (fp, node->left);
            pr ("}{", node->data->opCode);
            PrintSubtree (fp, node->right);
            pr ("}", node->data->opCode);
            break;
        case '^':
            pr ("{", node->data->opCode);

            if (node->left->data->opCode != 0) pr ("(");
            PrintSubtree (fp, node->left);
            if (node->left->data->opCode != 0) pr (")");

            pr ("}^{", node->data->opCode);
            PrintSubtree (fp, node->right);
            pr ("}", node->data->opCode);
            break;
        case 'l':
            if (node->left != 0)
            {
                pr ("\\log_", node->data->opCode);
                if (node->left->data->opCode != 0) pr ("{");
                PrintSubtree (fp, node->left);
                if (node->left->data->opCode != 0) pr ("}");
            }
            else pr ("\\ln", node->data->opCode);

            if (node->right->data->opCode != 0) pr ("(");
            PrintSubtree (fp, node->right);
            if (node->right->data->opCode != 0) pr (")");

            break;
    }

    return 0;
}

static int
PrintSubtreeWithBrackets (FILE* fp, const NodeBinTree* node)
{
    if (fp == nullptr || node == nullptr) return 0;

    switch (node->data->opCode)
    {
        case 0:
            IsOperation (fp, node);
            break;
        case '+':
        case '-':
            pr ("(");
            PrintSubtree (fp, node);
            pr (")");
            break;
        case '*':
        case '/':
        case '^':
        case 'l':
            PrintSubtree (fp, node);
            break;
    }
}

static int
IsOperation (FILE* fp, const NodeBinTree* node)
{
    if (fp == nullptr || node == nullptr) return 0;

    if (node->data->variable != nullptr)
    {
        pr (" %s ", node->data->variable);
        return 0;
    }
    if (node->data->opCode == 0)
    {
        pr (" %lg ", node->data->value);
        return 0;
    }

    return 1;
}

#undef pr
