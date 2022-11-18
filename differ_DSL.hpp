#define LEFT_NODE   node->left
#define RIGHT_NODE  node->right

#define DIFF_LEFT   node_differentiation(tree, node->left, var)
#define DIFF_RIGHT  node_differentiation(tree, node->right, var)

#define COPY_LEFT(old_left)   tree_copy(old_left)
#define COPY_RIGHT(old_right) tree_copy(old_right)

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

//------------------------------------------------------------------------------------------------------------------------         

#define DIFF_NUM(node)  {return CREATE_NUM(0);}

#define DIFF_VAR(node)  {if (node->data.var == var) return CREATE_NUM(1); else return CREATE_NUM(9);}       

#define DIFF_ADD(node)  {return ADD_NODES(DIFF_LEFT, DIFF_RIGHT);}

#define DIFF_SUB(node)  {return SUB_NODES(DIFF_LEFT, DIFF_RIGHT);} 

#define DIFF_MUL(node)  {Node * old_left  = node->left; Node * old_right = node->right; \
                        return ADD_NODES(MUL_NODES(DIFF_LEFT, COPY_RIGHT(old_right)), MUL_NODES(COPY_LEFT(old_left), DIFF_RIGHT));}

#define DIFF_DIV(node)  {Node * old_left  = node->left; Node * old_right = node->right; \
                        return DIV_NODES(SUB_NODES(MUL_NODES(DIFF_LEFT, COPY_RIGHT(old_right)), MUL_NODES(COPY_LEFT(old_left), DIFF_RIGHT)), MUL_NODES(COPY_RIGHT(old_right), COPY_RIGHT(old_right)));}

#define DIFF_SIN(node)  {Node * old_left  = node->left;  Node * old_right = node->right; \
                        return MUL_NODES(COS_NODES(old_left, COPY_RIGHT(old_right)), DIFF_RIGHT);}


#define DIFF_COS(node) {Node * old_left  = node->left;  Node * old_right = node->right; \
                        return MUL_NODES(MUL_NODES(SIN_NODES(old_left, COPY_RIGHT(old_right)), CREATE_NUM(-1)), DIFF_RIGHT);}

#define DIFF_POW(node) {Node * old_left  = node->left;  Node * old_right = node->right; \
                        return MUL_NODES(MUL_NODES(CREATE_NUM(old_right->data.num), POW_NODES(old_left, CREATE_NUM(old_right->data.num - 1))), DIFF_LEFT);}

