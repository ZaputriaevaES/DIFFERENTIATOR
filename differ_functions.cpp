#include "differ_functions.hpp"
#include "tree_functions.hpp"
#include "differ_DSL.hpp"

static int count_png = 0;

const char * NAME_GRAPH_FILE = "graph.dot";

const char * s = NULL;

void tree_graph_dump(Node * root) 
{
    assert(root);
    FILE * dot_file = fopen(NAME_GRAPH_FILE, "w");

    fprintf(dot_file,"digraph TREE{\n");

    node_graph_dump(dot_file, root, root->left);
    node_graph_dump(dot_file, root, root->right);

    fprintf(dot_file,"}\n");

    fclose(dot_file);

    // const int size_cmd = 100; 
    // char cmd[size_cmd] = ""; 
    // sprintf(cmd, "graph.dot -Tpng -o output%d.png", count_png);
    // count_png++;
    // system(cmd);
}

void node_graph_dump(FILE * dot_file, Node * node, Node * node_son) 
{
    
    if (node_son == NULL) return;

    fprintf   (dot_file, "\tnode%p[label=\"", node);
    node_data_dump(dot_file, node);
    fprintf   (dot_file, "\"]\n");

    fprintf   (dot_file, "\tnode%p[label=\"", node_son);
    node_data_dump(dot_file, node_son);
    fprintf   (dot_file, "\"]\n");
    
    fprintf(dot_file, "\tnode%p -> node%p\n", node, node_son);

    node_graph_dump(dot_file, node_son, node_son->left);
    node_graph_dump(dot_file, node_son, node_son->right);

}

void tree_inorder_dump (FILE * file, Node * node)
{
    assert(file != NULL);

    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) 
    {
        fprintf(file, "(");
        node_data_dump(file, node);
        fprintf(file, ")");
    } 
    else 
    {
        if (node->left)
        {
            fprintf(file, "(");
            tree_inorder_dump(file, node->left);
        }

        node_data_dump(file, node);

        if (node->right) 
        {
            tree_inorder_dump(file, node->right);
            fprintf(file, ")");
        }
    }

}

void node_data_dump(FILE * file, Node * node)
{
    assert(node);

    switch(node->type)
    {
        case VARIABLE: 
        {
            fprintf(file, "%c", node->data.var);
            break;
        }
        case NUMBER: 
        {
            fprintf(file, "%d", node->data.num);            
            break;
        }
        case OPERATOR: 
        {
            fprintf(file, "%c", operator_decryption_int_to_char(node->data.opr));            
            break;
        }
        default:
        {
            printf("type is written to the node on\n");
        }
    }
}

char operator_decryption_int_to_char(int opr)
{
    switch(opr)
    {
        case ADD: 
        {
            return '+';
            break;
        }
        case SUB: 
        {
            return '-';        
            break;
        }
        case MUL: 
        {
            return '*';             
            break;
        }
        case DIV: 
        {
            return '/';             
            break;
        }     
        case SIN: 
        {
            return 'S';        
            break;
        }
        case COS: 
        {
            return 'C';             
            break;
        }
        case POW: 
        {
            return '^';             
            break;
        }                  
        case LOG: 
        {
            return 'L';             
            break;
        }   
        default:
        {
            printf("error\n");
        }
    }
}


Node * tree_copy(Node * node)
{
    assert(node);

    Node * n_node = (Node*) calloc(1, sizeof(Node));
    *n_node = *node;
    if (node->left != NULL) {
        n_node->left = tree_copy(node->left);
    }
    if (node->right != NULL) {
        n_node->right = tree_copy(node->right);
    }

    return n_node;
}


Node * tree_differentiation(Tree * tree, char var)
{
    assert(tree);
    Tree * diff_tree = (Tree*) calloc(1, sizeof(Tree));

    diff_tree->root = tree_copy(tree->root);
    if (diff_tree->root == NULL) 
        printf("copy not ok\n");
    else 
        printf("copy ok\n");

    diff_tree->root = node_differentiation(diff_tree, diff_tree->root, var);
    tree_inorder_dump(stdout, diff_tree->root);

    tree_optimizer(diff_tree->root);
    tree_inorder_dump(stdout, diff_tree->root);

    tree_graph_dump(diff_tree->root);
}

Node * node_differentiation(Tree * tree, Node * node, char var)
{
    assert(node);
    printf("\n-------------------\n");
    tree_inorder_dump(stdout, node);


    switch(node->type)
    {
        case VARIABLE: 
        {
            printf("var_diff\n");
            printf("\n-------------------\n");
            DIFF_VAR(node);
            break;
        }
        case NUMBER: 
        {
            printf("num_diff\n");
            printf("\n-------------------\n");
            DIFF_NUM(node);
            break;
        }
        case OPERATOR: 
        {
            printf("opr_diff %d\n", node->data.opr);
            printf("\n-------------------\n");
            switch(node->data.opr) 
            {
            case ADD: DIFF_ADD(node); break;         
            case SUB: DIFF_SUB(node); break;        
            case MUL: DIFF_MUL(node); break;
            case DIV: DIFF_DIV(node); break;
            case SIN: DIFF_SIN(node); break;        
            case COS: DIFF_COS(node); break;
            case POW: DIFF_POW(node); break;
            case LOG: DIFF_LOG(node); break;
            }
            break;
        }
        default:
        {
            printf("error\n");
        }
    }
}

Node * create_node(Node_type type, Node_data data, Node * left, Node * right) 
{
    Node * node = (Node *) calloc(1, sizeof(Node));

    node->left = left;
    node->right = right;
    
    node->type = type;
    node->data = data;

    return node;
}


void tree_optimizer(Node * node)
{
    assert(node != NULL);

    int was_simplified_const = 0;
    int was_simplified_neutr = 0;

    constant_folding(node, &was_simplified_const);
    neutral_expressions(node, &was_simplified_neutr);

    while(was_simplified_const != 0 || was_simplified_neutr != 0)
    {
        was_simplified_const = 0;
        was_simplified_neutr = 0;
        constant_folding(node, &was_simplified_const);
        neutral_expressions(node, &was_simplified_neutr);
    }

}


void constant_folding(Node * node, int * was_simplified)
{
    assert(node != NULL);

    if(node->left != NULL)
    {
        if(is_node_const(node->left)) {constant_calculation(node, LEFT, was_simplified);}
        else constant_folding(node->left, was_simplified);
    }
    if(node->right != NULL)
    {
        if(is_node_const(node->right)) {constant_calculation(node, RIGHT, was_simplified);}
        else constant_folding(node->right, was_simplified);
    }
}

void constant_calculation(Node * node, int side, int * was_simplified)
{
    assert(node != NULL);   

    if(side == LEFT) 
        {node->left = const_calc(node->left, was_simplified); }
    else 
        {node->right = const_calc(node->right, was_simplified); }
}

Node * const_calc(Node * node, int * was_simplified)
{
   
   if(node->left == NULL && node->right == NULL) return node;

   else if(node->left->left == NULL && node->right->left == NULL) 
   { 
        int calc = 0;

        if(node->data.opr == ADD) 
        {calc = (node->left->data.num + node->right->data.num); node = node_ctor(node);
        node->type = NUMBER; node->data.num = calc;*was_simplified = 1;}
        else if(node->data.opr == SUB) 
        {calc = (node->left->data.num - node->right->data.num); node = node_ctor(node);
        node->type = NUMBER; node->data.num = calc;*was_simplified = 1;}
        else if(node->data.opr == MUL) 
        {calc = (node->left->data.num * node->right->data.num); node = node_ctor(node);
        node->type = NUMBER; node->data.num = calc;*was_simplified = 1;}
        else if(node->data.opr == DIV) 
        {calc = (node->left->data.num / node->right->data.num); node = node_ctor(node);
        node->type = NUMBER; node->data.num = calc;*was_simplified = 1;}

        //*was_simplified = 1;
        return node;
   }
   else 
   {
       const_calc(node->left, was_simplified);
       const_calc(node->right, was_simplified);
   }
}


void neutral_expressions(Node * node, int * was_simplified)
{
    assert(node != NULL);

    if(node->left != NULL)
    {
        if(is_special_case(node->left, was_simplified) != 1) neutral_expressions(node->left, was_simplified);
    }
    if(node->right != NULL)
    {
        if(is_special_case(node->right, was_simplified) != 1) neutral_expressions(node->right, was_simplified);
    }
}

int is_special_case(Node * node, int * was_simplified)
{
    if(MUL_LEFT_ON_1) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left;  
            *was_simplified = 1; return 1;}
    else if(MUL_RIGHT_ON_1) {
            COPY_NODE(node->right);
            node->left = node->right->left;  
            node->right = node->right->right; 
            *was_simplified = 1; return 1;}

    else if(DIV_ON_1) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left;  
            *was_simplified = 1; return 1;}

    else if(MUL_LEFT_ON_0) {node->left = NULL; node->right = NULL; node->type = NUMBER; node->data.num = 0;
            *was_simplified = 1; return 1;}
    else if(MUL_RIGHT_ON_0) {node->left = NULL; node->right = NULL; node->type = NUMBER; node->data.num = 0; 
            *was_simplified = 1; return 1;}

    else if(ADD_LEFT_WITH_0) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left;  
            *was_simplified = 1; return 1;}
    else if(ADD_RIGHT_WITH_0) {
            COPY_NODE(node->right);
            node->left = node->right->left;  
            node->right = node->right->right; 
            *was_simplified = 1; return 1;}
    else if(SUB_LEFT_WITH_0) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left;  
            *was_simplified = 1; return 1;}
    else if(SUB_RIGHT_WITH_0) {
            COPY_NODE(node->right);
            node->left = node->right->left;  
            node->right = node->right->right; 
            *was_simplified = 1; return 1;}

    else if(DIV_0_ON_LEFT) {node->left = NULL; node->right = NULL; node->type = NUMBER; node->data.num = 0; 
            *was_simplified = 1; return 1;}
    else if(DIV_0_ON_RIGHT) {node->left = NULL; node->right = NULL; node->type = NUMBER; node->data.num = 0; 
            *was_simplified = 1; return 1;}

    else if(POW_ON_1) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left;  
            *was_simplified = 1; return 1;}
    else if(POW_ON_0) {node->left = NULL; node->right = NULL; node->type = NUMBER; node->data.num = 1; 
            *was_simplified = 1; return 1;}
    else return 0;
}

int is_node_const(Node * node)
{
    if(node == NULL) return 0;
    int is_const = 1;

    if(node->type == VARIABLE || (node->type == OPERATOR && (node->data.opr == SIN ||node->data.opr == COS ||node->data.opr == POW || node->data.opr == LOG)))
    {is_const = 0;} 
    else 
    {
        int is_left_const = 0;
        int is_right_const = 0;
        int n = 0;
        if(node->left != NULL) 
            {is_left_const = (is_node_const(node->left)); n++;}
        if(node->right != NULL) 
            {is_right_const = (is_node_const(node->right)); n++;}
        if(n == 1)
            is_const = is_left_const | is_right_const;
        if(n == 2)
            is_const = is_left_const & is_right_const;
        else
            is_const = 1;
    } 

    return is_const;
}


int get_G(const char * str)
{
    s = str;
    int val = get_E();
    assert(*s == '\0');
    return val;

}

int get_E(void)
{
    int val = get_T();
    while(*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        int val2 = get_T();

        if(op == '+') val += val2;
        else val -= val2;
    }
    return val;
}

int get_T(void)
{
    int val = get_P();
    while(*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        int val2 = get_P();

        if(op == '*') val *= val2;
        else val /= val2;
    }
    return val;
}

int get_P(void)
{
    int val = 0;
    if(*s == '(')
    {
        s++;
        val = get_E();
        assert(*s == ')');
        s++;
    }
    else
        val = get_N();
    return val;
}

int get_N(void)
{
    int val = 0;
    const char * old_s = s;
    while('0' <= *s && *s <= '9')
    {
        val = val * 10 + *s - '0';
        s++;
    }
    assert(s != old_s);
    return val;
}