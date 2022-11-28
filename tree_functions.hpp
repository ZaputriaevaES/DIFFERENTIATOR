#include "tree_utilities.hpp"
//#include "differ_DSL.hpp"

void   start_program (Differ * differ);
void   differ_ctor (Differ * differ);

Node * tree_reading(Node * node, char ** text_buf);
Node * node_ctor(Node * node);
void   obj_ctor(Node * node, char ** text_buf);
int    operator_decryption_char_to_int(char opr);
char   operator_decryption_int_to_char(int opr);

char * createBufferWithFread (size_t * elements, FILE * readFile, size_t * strNum);
size_t fileSizeDetection     (FILE * read);
size_t countNumberOfRows     (char * BUF, size_t elements);

bool not_riority_operator(Node * node, Node * parent);
void print_tex_keywords (FILE * tex_file);
void title_tex_dump(Node * node);
void print_tex(FILE * file_tex, Node * node, Node * parent);
void print_tex_node(FILE * file_tex, Node * node);

Node * create_node(Node_type type, Node_data data, Node * left, Node * right);
Node * recursive_descent_reader(char * str);
Node * get_G(char * str, char ** s);
Node * get_E(char ** s);
Node * get_T(char ** s);
Node * get_D(char ** s);
Node * get_P(char ** s);
Node * get_N(char ** s);
Node * get_V(char ** s);
int get_opr(char ** s);


void  finish_program(Differ * differ);
void  tree_dtor (Tree * tree);
void  node_dtor(Node * node);