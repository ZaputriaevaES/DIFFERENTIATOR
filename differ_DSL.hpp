#define LEFT_NODE   node->left
#define RIGHT_NODE  node->right

#define DIFF_LEFT   node_differentiation(tree, node->left, var)
#define DIFF_RIGHT  node_differentiation(tree, node->right, var)

#define COPY_LEFT  tree_copy(LEFT_NODE)
#define COPY_RIGHT tree_copy(RIGHT_NODE)

//------------------------------------------------------------------------------------------------------------------------         

#define CREATE_NODE(type, data, left_node, right_node) create_node(type, data, left_node, right_node)

#define CREATE_NUM(num_data) create_node(NUMBER, {.num = num_data}, NULL, NULL)

#define CREATE_OPR(opr_data, left_node, right_node) create_node(OPERATOR, {.opr = opr_data}, left_node, right_node)

//------------------------------------------------------------------------------------------------------------------------          

#define ADD_NODES(left_node, right_node)  CREATE_OPR(ADD, left_node, right_node)
#define SUB_NODES(left_node, right_node)  CREATE_OPR(SUB, left_node, right_node)
#define MUL_NODES(left_node, right_node)  CREATE_OPR(MUL, left_node, right_node)
#define DIV_NODES(left_node, right_node)  CREATE_OPR(DIV, left_node, right_node)
#define SIN_NODES(left_node, right_node)  CREATE_OPR(SIN, left_node, right_node)
#define COS_NODES(left_node, right_node)  CREATE_OPR(COS, left_node, right_node)
#define POW_NODES(left_node, right_node)  CREATE_OPR(POW, left_node, right_node)
#define LOG_NODES(left_node, right_node)  CREATE_OPR(LOG, left_node, right_node)

//------------------------------------------------------------------------------------------------------------------------         

#define DIFF_NUM(node)  {return CREATE_NUM(0);}

#define DIFF_VAR(node)  {if (node->data.var == var) return CREATE_NUM(1); else return CREATE_NUM(0);}       

#define DIFF_ADD(node)  {return ADD_NODES(DIFF_LEFT, DIFF_RIGHT);}

#define DIFF_SUB(node)  {return SUB_NODES(DIFF_LEFT, DIFF_RIGHT);} 

#define DIFF_MUL(node)  {return ADD_NODES(MUL_NODES(DIFF_LEFT, COPY_RIGHT), MUL_NODES(COPY_LEFT, DIFF_RIGHT));}

#define DIFF_DIV(node)  {return DIV_NODES(SUB_NODES(MUL_NODES(DIFF_LEFT, COPY_RIGHT), MUL_NODES(COPY_LEFT, DIFF_RIGHT)), MUL_NODES(COPY_RIGHT, COPY_RIGHT));}

#define DIFF_SIN(node)  {return MUL_NODES(COS_NODES(LEFT_NODE, COPY_RIGHT), DIFF_RIGHT);}

#define DIFF_COS(node) {return MUL_NODES(MUL_NODES(SIN_NODES(LEFT_NODE, COPY_RIGHT), CREATE_NUM(-1)), DIFF_RIGHT);}

#define DIFF_POW(node) {if(RIGHT_NODE->type == NUMBER) return MUL_NODES(MUL_NODES(CREATE_NUM(RIGHT_NODE->data.num), POW_NODES(LEFT_NODE, CREATE_NUM(RIGHT_NODE->data.num - 1))), DIFF_LEFT); \
                        else return MUL_NODES(node, LOG_NODES(CREATE_NUM(1), CREATE_NUM(LEFT_NODE->data.num)));}

#define DIFF_LOG(node) {return MUL_NODES(DIV_NODES(CREATE_NUM(1), RIGHT_NODE), DIFF_RIGHT);}