#include "tree_functions.hpp"

FILE * input_file = fopen("input_file.txt", "r+");
FILE * file_tree  = fopen("tree_dump.txt", "w");


void start_program(Differ * differ)
{
    differ_ctor(differ);

    differ->tree->root = tree_reading(differ->tree->root, &(differ->buffer));
}

void differ_ctor(Differ * differ) 
{
    assert(differ);
 
    differ->tree = (Tree *) calloc(1, sizeof(Tree));

    struct Text text_base = {0, 0};
    differ->buffer = createBufferWithFread(&text_base.amountOfElements, input_file, &text_base.amountOfStrings); 

    printf("%s\n", differ->buffer);

}


Node * tree_reading(Node * node, char ** text_buf)
{
    assert (node == NULL);
    assert (text_buf != NULL);

    if (**text_buf == '(') 
    {
        node = node_ctor(node);

        (*text_buf)++;
        printf("1:%s\n", *text_buf);
        if (**text_buf == '(' || **text_buf == '\0')
        {
            node->left = tree_reading(node->left, text_buf);
        }

        obj_ctor(node, text_buf);

        printf("3:%s\n", *text_buf);

        if (**text_buf == ')' || **text_buf == '\0')
        {
            (*text_buf)++;
            printf("3.5:%s\n", *text_buf);
            return node;
        }

        printf("4:%s\n", *text_buf);

        if (**text_buf == '(' || **text_buf == '\0')
        {
            node->right = tree_reading(node->right, text_buf);
        }

        (*text_buf)++;

    }

    return node;
}

Node * node_ctor(Node * node) 
{
    //assert (node == NULL);

    node = (Node *) calloc(1, sizeof(Node));
    assert(node);

    node->left = NULL;
    node->right = NULL;
    node->type = -1;
    node->data.emp = -1;

    return node;
}

void  obj_ctor(Node * node, char ** text_buf)
{
    assert(node);
    assert(text_buf);

    printf("##%c##%p##\n", **text_buf, node);
 
    if(isalpha(**text_buf) && **text_buf != 'C' && **text_buf != 'S' && **text_buf != 'L') //переменная
    {
        node->type = VARIABLE;
        node->data.var = **text_buf;

        printf("var:%c\n", node->data);
        printf("buf:%s\n", *text_buf);
        (*text_buf)++;  
    }
    else if(isdigit(**text_buf)) //цифра
    {
        node->type = NUMBER;

        int num_data = 0;
        int num_sbl = 0;

        sscanf(*text_buf, "%d%n", &num_data, &num_sbl);
        node->data.num = num_data; 

        printf("num:%d\n", node->data);
        printf("buf:%s\n", *text_buf);

        (*text_buf) += num_sbl;  
    }
    else //оператор
    {
        node->type = OPERATOR;
        //node->data.opr = **text_buf;  
        node->data.opr = operator_decryption_char_to_int(**text_buf);

        printf("opr:%c\n", operator_decryption_int_to_char(node->data.opr));   
        printf("buf:%s\n", *text_buf);
        (*text_buf)++;  
    }
}

int operator_decryption_char_to_int(char opr)
{
    switch(opr)
    {
        case '+': 
        {
            return ADD;
            break;
        }
        case '-': 
        {
            return SUB;        
            break;
        }
        case '*': 
        {
            return MUL;             
            break;
        }
        case '/': 
        {
            return DIV;             
            break;
        }   
        case 'S': 
        {
            return SIN;             
            break;
        }
        case 'C': 
        {
            return COS;             
            break;
        }
        case '^': 
        {
            return POW;             
            break;
        }     
        case 'L': 
        {
            return LOG;             
            break;
        }            
        default:
        {
            printf("error\n");
        }
    }
}


char * createBufferWithFread(size_t * elements, FILE * readFile, size_t * strNum)
{
    assert(readFile != NULL);

    *elements = fileSizeDetection(readFile);

    char * buffer = (char *)calloc(*elements, sizeof(char));

    fread(buffer, sizeof(char), *elements, readFile);

    buffer[*elements] = '\0';

    return buffer;
}

size_t fileSizeDetection(FILE * read)
{
    assert(read != NULL);

    struct stat stbuf;
    stat("input_file.txt", &stbuf);
    off_t fsize = stbuf.st_size;

    return fsize / sizeof(char) ;
}

size_t countNumberOfRows(char * BUF, size_t elements)
{
    assert(BUF != NULL);

    size_t n = 0; //количество символов новой строки
    for (size_t i = 0; i < elements; i++)
    {
        if (BUF[i] == '\n')
        {
            //BUF[i] = '\0';
            n++;
        }
    }

    n++;

    return n;
}


void finish_program(Differ * differ)
{
    tree_dtor(differ->tree);
}

void tree_dtor (Tree * tree)
{
    assert(tree);

    node_dtor(tree->root);
}

void node_dtor(Node * node) 
{
    assert(node);

    if (node->left != NULL) node_dtor(node->left);
    if (node->right != NULL) node_dtor(node->right);

    node->left = node->right = NULL;

    free(node);

    return;
}