#include "differ_functions.hpp"
#include "tree_functions.hpp"
#include "differ_DSL.hpp"

static int count_png = 0;

const char * NAME_GRAPH_FILE = "graph.dot";

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
            return 'P';             
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
            case LOG: DIFF_LOG(node);  break;
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
