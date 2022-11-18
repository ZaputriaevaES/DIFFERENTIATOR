#include "tree_utilities.hpp"

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

void  finish_program(Differ * differ);
void  tree_dtor (Tree * tree);
void  node_dtor(Node * node);