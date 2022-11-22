#pragma once
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

void   tree_optimizer(Node * node);
void   constant_folding(Node * node, int * was_simplified);
void   neutral_expressions(Node * node, int * was_simplified);

void   constant_calculation(Node * node, int side, int * was_simplified);
Node * const_calc(Node * node, int * was_simplified);
int    is_special_case(Node * node, int * was_simplified);
int    is_node_const(Node * node);

int get_G(const char * str);
int get_E(void);
int get_T(void);
int get_P(void);
int get_N(void);
