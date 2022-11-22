#include "tree_functions.hpp"
#include "differ_functions.hpp"

int main(int argc, char* argv[]) 
{
    Differ differ = {};

    //int val = get_G("3*(5-3)\0");
    //printf("result: %d\n", val);

    start_program(&differ);

    tree_inorder_dump(stdout, differ.tree->root);
    tree_graph_dump(differ.tree->root);

    tree_differentiation(differ.tree, 'x');

    finish_program(&differ);

    return 0;
}

//void attriute premain
//espeak