#include "tree_functions.hpp"
#include "differ_functions.hpp"

int main(int argc, char* argv[]) 
{
    Differ differ = {};

    start_program(&differ);
    tree_graph_dump(differ.tree->root);

    title_tex_dump(differ.tree->root);

    int diff_number = 0;
    printf ("Введите степень производной: ");
    scanf ("%d", &diff_number);

    taking_nth_derivative(diff_number, differ.tree, 'x');

    tree_inorder_dump(stdout, differ.tree->root);
    printf("\n");
    tree_graph_dump(differ.tree->root);

    finish_program(&differ);

    return 0;
}

//void attriute premain
//espeak