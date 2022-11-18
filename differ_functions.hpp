#include "tree_utilities.hpp"

void tree_graph_dump(Node * root);
void node_graph_dump(FILE * dot_file, Node * node, Node * node_son);

void  tree_inorder_dump  (FILE * file, Node * node);
void  node_data_dump(FILE * file, Node * node);
char  operator_decryption_int_to_char(int opr);

Node * tree_copy(Node * node);

Node * tree_differentiation(Tree * tree, char var);
Node * node_differentiation(Tree * tree, Node * node, char var);

Node * create_node(Node_type type, Node_data data, Node * left, Node * right);