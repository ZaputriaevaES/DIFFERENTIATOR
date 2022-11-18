#include "tree_functions.hpp"
#include "differ_functions.hpp"

FILE * input_file = fopen("input_file.txt", "r+");
FILE * file_tree  = fopen("tree_dump.txt", "w");


int main(int argc, char* argv[]) 
{
    Differ differ = {};

    start_program(&differ);

    //tree_preorder_dump(file_tree, differ.tree->root);
    tree_inorder_dump(stdout, differ.tree->root);
    tree_graph_dump(differ.tree->root);

    // printf("\nWHAT?\n");  

    // Tree * copy_tree = (Tree *) calloc(1, sizeof(Tree));
    // copy_tree->root = tree_copy(differ.tree->root);
    // tree_inorder_dump(stdout, copy_tree->root);

    printf("\nGOODBYE\n");  

    tree_differentiation(differ.tree, 'x');

    finish_program(&differ);

    fclose(input_file);

    return 0;
}