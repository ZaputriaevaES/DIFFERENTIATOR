#ifndef AKINATOR_UTILITIES_HPP_INCLUDED
#define AKINATOR_UTILITIES_HPP_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "STACK/utils_stk.hpp"
#include "STACK/debug.hpp"
#include "STACK/stack.hpp"

#define MAX_STR_LEN 40


struct Text
{
    size_t amountOfElements;
    size_t amountOfStrings;
};

enum Node_type 
{
    NUMBER,
    VARIABLE,
    OPERATOR
};

enum Opr_type
{
    ADD,
    SUB,
    MUL,
    DIV,
    SIN,
    COS,
    POW
};

typedef int opr_t; //оператор
typedef int  num_t; //число
typedef char var_t; //переменная

union Node_data 
{
    int   emp;
    opr_t opr;
    num_t num;
    var_t var;
};


struct Node 
{
    union Node_data data;
    Node * left;
    Node * right;
    int type;
};

struct Tree 
{
    Node * root = NULL;
};

struct Differ 
{
    Tree * tree = {};
    char * buffer = NULL;
};

#endif