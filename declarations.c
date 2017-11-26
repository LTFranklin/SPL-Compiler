#define NOTHING -1
#define SYMTABSIZE     50
#define IDLENGTH       15
#define NOTHING        -1
#define INDENTOFFSET    2

enum ParseTreeNodeType
{       
        PROGRAM,BLOCK,DECLARATION_BLOCK,VAR_TYPE_CHAR,VAR_TYPE_INT,VAR_TYPE_REAL,STATEMENT_LIST,STATEMENT,ASSIGNMENT_STATEMENT,IF_STATEMENT,WHILE_STATEMENT,DO_STATEMENT,FOR_STATEMENT,WRITE_STATEMENT,READ_STATEMENT,OUTPUT_LIST,CONDITIONAL,CONDITIONAL_OR,CONDITIONAL_AND,ET,NET,LT,GT,LTOET,GTOET,EXPRESSION,EXPRESSION_PLUS,EXPRESSION_MINUS,TERM,TERM_MUL,TERM_DIV,VALUE,BRA_VALUE,CHAR_CONSTANT,NEG_CONSTANT,NUM_CONSTANT,INTEGER_CONSTANT,FLOAT_CONSTANT,VARIABLE
};


/*Define a tree node as an item, variable and 3 other tree nodes */
struct treeNode
{
        int item;
        int nodeIdentifier;
        struct treeNode *first;
        struct treeNode *second;
        struct treeNode *third;
};

typedef struct treeNode TREE_NODE;
typedef TREE_NODE *TERNARY_TREE;
TERNARY_TREE create_node(int,int,TERNARY_TREE,TERNARY_TREE,TERNARY_TREE);
void print_tree(TERNARY_TREE);
void generate_code(TERNARY_TREE);

struct symTabNode {
    char identifier[IDLENGTH];
};

typedef  struct symTabNode SYMTABNODE;
typedef  SYMTABNODE        *SYMTABNODEPTR;

SYMTABNODEPTR  symTab[SYMTABSIZE];

int currentSymTabSize = 0;

char *node[] = {"PROGRAM","BLOCK","DECLARATION_BLOCK","VAR_TYPE_CHAR","VAR_TYPE_INT","VAR_TYPE_REAL","STATEMENT_LIST","STATEMENT","ASSIGNMENT_STATEMENT","IF_STATEMENT","WHILE_STATEMENT","DO_STATEMENT","FOR_STATEMENT","WRITE_STATEMENT","READ_STATEMENT","OUTPUT_LIST","CONDITIONAL","CONDITIONAL_OR","CONDITIONAL_AND","ET","NET","LT","GT","LTOET","GTOET","EXPRESSION","EXPRESSION_PLUS","EXPRESSION_MINUS","TERM","TERM_MUL","TERM_DIV","VALUE","BRA_VALUE","CHAR_CONSTANT","NEG_CONSTANT","NUM_CONSTANT","INTEGER_CONSTANT","FLOAT_CONSTANT","VARIABLE"};

char *reserved[32] =
{"if","else","switch","case","default","break","int","float","char","double","long","for","while","do","void","goto","auto","signed","const","extern","register","unsigned","return","continue","enum","sizeof","struct","typedef","union","volatile"};
