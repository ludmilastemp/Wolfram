#include "STL_Explore.h"

void STL_Explore (BinTree* tree)
{
    if (tree == nullptr) return;

    STL_Latex (nullptr, "Я не знаю, зачем вы открыли этот файл, но теперь страдайте!\\\\");
    STL_Latex (tree, "Изначальное уравнение:");
    STL_SimplifyEval (tree->root);
    STL_Latex (tree, "Упрощенный вид изначального уравнения:");
//    STL_GraphvizBinTree (tree);

    BinTree* dtree = Differentiate (tree);

    STL_Latex (nullptr, "\\\\Ты думаешь всё? Неее, посмотри на производную!\\\\");
    STL_Latex (dtree, "Первая производная:");
    STL_SimplifyEval (dtree->root);
    STL_Latex (dtree, "Упрощенный вид первой производной:");
//    STL_GraphvizBinTree (dtree);

//    BinTree* d2tree = Differentiate (dtree);
//
//    STL_Latex (nullptr, "\\\\Думаешь это конец? Хахаха, это слишком наивно!\\\\");
//    STL_Latex (d2tree, "Вторая производная:");
//    STL_SimplifyEval (d2tree->root);
//    STL_Latex (d2tree, "Упрощенный вид второй производной:");
//    STL_GraphvizBinTree (dtree);

    printf ("BLABLABLA");
    return;
}
