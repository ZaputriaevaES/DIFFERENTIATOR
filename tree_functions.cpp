#include "tree_functions.hpp"
#include "differ_DSL.hpp"

FILE * input_file = fopen("input_file.txt", "r+");
FILE * file_tree  = fopen("tree_dump.txt", "w");
FILE * file_tex   = fopen("tex_input.tex", "w");

const char * keywords[4] = {"Очевидно, что:", "Заметим, что:", "Нетрудно заметить:", "Не требует дальнейших комментариев:"};

void start_program(Differ * differ)
{
    differ_ctor(differ);
    printf("\n--\n%s\n--\n", differ->buffer);

    //differ->tree->root = tree_reading(differ->tree->root, &(differ->buffer));
    differ->tree->root = recursive_descent_reader(differ->buffer);
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
        //printf("1:%s\n", *text_buf);
        if (**text_buf == '(' || **text_buf == '\0')
        {
            node->left = tree_reading(node->left, text_buf);
        }

        obj_ctor(node, text_buf);

        //printf("3:%s\n", *text_buf);

        if (**text_buf == ')' || **text_buf == '\0')
        {
            (*text_buf)++;
            //printf("3.5:%s\n", *text_buf);
            return node;
        }

        //printf("4:%s\n", *text_buf);

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

    //printf("##%c##%p##\n", **text_buf, node);
 
    if(isalpha(**text_buf) && **text_buf != 'C' && **text_buf != 'S' && **text_buf != 'L') //переменная
    {
        node->type = VARIABLE;
        node->data.var = **text_buf;

        //printf("var:%c\n", node->data);
        //printf("buf:%s\n", *text_buf);
        (*text_buf)++;  
    }
    else if(isdigit(**text_buf)) //цифра
    {
        node->type = NUMBER;

        int num_data = 0;
        int num_sbl = 0;

        sscanf(*text_buf, "%d%n", &num_data, &num_sbl);
        node->data.num = num_data; 

        //printf("num:%d\n", node->data);
        //printf("buf:%s\n", *text_buf);

        (*text_buf) += num_sbl;  
    }
    else //оператор
    {
        node->type = OPERATOR;
        //node->data.opr = **text_buf;  
        node->data.opr = operator_decryption_char_to_int(**text_buf);

        //printf("opr:%c\n", operator_decryption_int_to_char(node->data.opr));   
        //printf("buf:%s\n", *text_buf);
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


bool not_riority_operator(Node * node, Node * parent) 
{
    if (!node || !parent) return false;

    if (parent->type == OPERATOR && node->type == OPERATOR && 
    ((parent->data.opr == ADD || parent->data.opr == SUB) || (node->data.opr == parent->data.opr)))
        return true;

    else return false;
}

void title_tex_dump(Node * node) 
{
    if (!file_tex) printf ("tex open error");

    fprintf(file_tex, "\\documentclass[12pt, letterpaper, twoside]{article}\n");
    fprintf(file_tex, "\\usepackage[T1]{fontenc}\n");
    fprintf(file_tex, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file_tex, "\\usepackage[russian]{babel}\n");
    fprintf(file_tex, "\\title{взятие производной функции}\n");
    fprintf(file_tex, "\\author{Запутряева Екатерина Сергеевна}\n");
    fprintf(file_tex, "\\date{25.11.2022}\n");
    fprintf(file_tex, "\\begin{document}\n");
    fprintf(file_tex, "\\maketitle\n");
    fprintf(file_tex, "\\newpage\n");
    fprintf(file_tex, "взятие производной n-нного порядка функции: \n");
    fprintf(file_tex, "$ f(x) = ");
    print_tex(file_tex, node, NULL);
    fprintf(file_tex, " $\n");

    //fprintf(file_tex, "\\end{document}\n");
    
    //fclose(file_tex);

    //system("pdflatex tex_input.tex");
}

void print_tex(FILE * file_tex,  Node * node, Node * parent) 
{
    if (!node) return;

    if (not_riority_operator(node, parent));
    else if (node->left && parent) fprintf(file_tex, "{(");
    
    if (parent && (parent->data.opr == POW || parent->data.opr == DIV)) fprintf(file_tex, "{");
    if (node->data.opr == DIV) fprintf(file_tex, "{");
    
    if (!(!node->left && node->right)); print_tex(file_tex, node->left, node);

    print_tex_node(file_tex, node);
    
    print_tex(file_tex, node->right, node);

    if (parent && (parent->data.opr == POW || parent->data.opr == DIV)) fprintf(file_tex, "}");
    if (node->data.opr == DIV) fprintf(file_tex, "}");

    if (not_riority_operator(node, parent)) ;
    else if (node->right && !(!node->left && node->right) && parent) fprintf(file_tex, ")}");
}

void print_tex_node(FILE * file_tex, Node * node) 
{
    if (node == NULL) return;

    switch (node->type) 
    {
        case OPERATOR:
        {
            switch (node->data.opr) 
            {
                case SIN:
                    fprintf(file_tex, "\\sin");
                    break;

                case COS:
                    fprintf(file_tex, "\\cos");
                    break;

                case LOG:
                    fprintf(file_tex, "\\ln");
                    break;
    
                case ADD:
                    fprintf(file_tex, " + ");
                    break;
                
                case SUB:
                    fprintf(file_tex, " - ");
                    break;
                
                case MUL:
                    fprintf(file_tex, " \\cdot ");
                    break;
                
                case DIV:
                    fprintf(file_tex, " \\over ");
                    break;

                case POW:
                    fprintf(file_tex, "^");
                    break;

                default:
                    fprintf(file_tex, "don't find oper");
                    break;
            }
            break;
        }
        case VARIABLE:
        {
            fprintf(file_tex, "%c", node->data.var);
            break;
        }
        case NUMBER:
        {
            if (node->data.num < 0) 
            {
                fprintf(file_tex, "(%d)", node->data.num);
            } 
            else 
            {
                fprintf(file_tex, "%d", node->data.num);
            }
            break;
        }
        default:
            break;
    }
}

void print_tex_keywords (FILE * tex_file)
{
    int random_keyword = rand()%4;
    fprintf (tex_file, "\\newline %s\n\n", keywords[random_keyword]);
}


Node * recursive_descent_reader(char * str)
{
    assert(str);
    char * s = 0;
    return get_G(str, &s);
}

Node * get_G(char * str, char ** s)
{
    *s = str;
    Node * val = get_E(s);
    assert(**s == '\0');
    return val;
}

Node * get_E(char ** s)
{
    Node * val = get_T(s);
    while(**s == '+' || **s == '-')
    {
        char op = **s;
        (*s)++;
        Node * val2 = get_T(s);

        if(op == '+') val = CREATE_OPR(ADD, val, val2);
        else val = CREATE_OPR(SUB, val, val2);
    }
    return val;
}

Node * get_T(char ** s)
{
    Node * val = get_D(s);
    while(**s == '*' || **s == '/')
    {
        char op = **s;
        (*s)++;
        Node * val2 = get_D(s);

        if(op == '*') val = CREATE_OPR(MUL, val, val2);
        else val = CREATE_OPR(DIV, val, val2);
    }
    return val;
}

Node * get_D(char ** s)
{
    Node * val = get_P(s);
    while(**s == '^')
    {
        (*s)++;
        Node * val2 = get_P(s);

        val = CREATE_OPR(POW, val, val2);
    }
    return val;
}

Node * get_P(char ** s)
{
    Node * val = NULL;
    int opr = 0;

    if(**s == '(')
    {
        (*s)++;
        val = get_E(s);
        assert(**s == ')');
        (*s)++;
    }
    else if(opr = get_opr(s)) 
    {
        (*s)++;
        val = get_E(s);
        assert(**s == ')');
        (*s)++;
        return CREATE_OPR(opr, NULL, val);
    }
    else if(**s > '9') val = get_V(s);
    else val = get_N(s);

    return val;
}

int get_opr(char ** s)
{
    assert(s);
    if((**s == 's') && (*(*s+1) == 'i') && (*(*s+2) == 'n')) {(*s) += 3; return SIN;}
    else if((**s == 'c') && (*(*s+1) == 'o') && (*(*s+2) == 's')) {(*s) += 3; return COS;}
    else if((**s == 'l') && (*(*s+1) == 'o') && (*(*s+2) == 'g')) {(*s) += 3; return LOG;}
    else return 0;
}

Node * get_N(char ** s)
{
    int val = 0;
    char * old_s = *s;
    while('0' <= **s && **s <= '9')
    {
        val = val * 10 + **s - '0';
        (*s)++;
    }
    assert(*s != old_s);
    return CREATE_NUM(val);
}

Node * get_V(char ** s)
{
    char val = 0;
    char * old_s = *s;
    while('a' <= **s && **s <= 'z')
    {
        val = **s;
        (*s)++;
    }
    assert(*s != old_s);
    return CREATE_VAR(val);
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

