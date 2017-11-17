%{
#ifdef DEBUG
#define ACT(x) print_tree(x)
#else
#ifdef YYDEBUG
#define ACT(x)
#else
#define ACT(x) gen(x)
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include "genCode.c"

int yylex(void);
void yyerror(char *);

%}

%union
{
	int iVal;
	TERNARY_TREE tVal;
}


%type<iVal> CHARACTER_CONSTANT NUMBER_CONSTANT IDENTIFIER

%type<tVal>				program block declaration_block type statement_list statement assignment_statement if_statement while_statement do_statement for_statement write_statement read_statement output_list conditional comparator expression term value constant variable

%token					IDENTIFIER FULL_STOP ENDP DECLARATIONS CODE COMMA OF TYPE SEMI_COLON CHARACTER INTEGER REAL ASSIGNMENT IF THEN ELSE ENDIF WHILE DO ENDWHILE FOR IS BY TO ENDFOR WRITE OPEN CLOSE NEWLINE READ NOT AND OR EQUAL_TO NOT_EQUAL_TO LESS_THAN GREATER_THAN LESS_THAN_OR_EQUAL_TO GREATER_THAN_OR_EQUAL_TO PLUS MINUS MULTIPLY DIVIDE QUOTE NUMBER_CONSTANT CHARACTER_CONSTANT COLON ENDDO
%%
program			 		: variable COLON block ENDP variable FULL_STOP
					{
						TERNARY_TREE ParseTree;
						ParseTree = create_node(NOTHING,PROGRAM,$1,$3,$5);
						ACT(ParseTree);
					}
					;
block			 		: DECLARATIONS declaration_block CODE statement_list
					{
						$$ = create_node(NOTHING,BLOCK,$2,$4,NULL);
					}
					| CODE statement_list
					{
						$$ = create_node(NOTHING,BLOCK,$2,NULL,NULL);
					}
					;
declaration_block			: variable COMMA declaration_block
					{
						$$ = create_node(NOTHING,DECLARATION_BLOCK,$1,$3,NULL);
					}
					| variable OF TYPE type SEMI_COLON declaration_block
					{
						$$ = create_node(NOTHING,DECLARATION_BLOCK,$1,$4,$6);
					}
					| variable OF TYPE type SEMI_COLON
										{
						$$ = create_node(NOTHING,DECLARATION_BLOCK,$1,$4,NULL);
					}
					;
type					: CHARACTER
						{
							$$ = create_node(NOTHING,VAR_TYPE_CHAR,NULL,NULL,NULL);
						}
						| INTEGER
						{
							$$ = create_node(NOTHING,VAR_TYPE_INT,NULL,NULL,NULL);
						}						
						| REAL
						{
							$$ = create_node(NOTHING,VAR_TYPE_REAL,NULL,NULL,NULL);
						}
					;
statement_list				: statement SEMI_COLON statement_list
					{
						$$ = create_node(NOTHING,STATEMENT_LIST,$1,$3,NULL);
					}
					| statement
						{
							$$ = create_node(NOTHING,STATEMENT_LIST,$1,NULL,NULL);
						}
					;
statement			 	: assignment_statement
						{
							$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
						}
					| if_statement 
						{
							$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
						}
					| while_statement
						{
							$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
						}					
					| do_statement
						{
							$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
						}					
					| for_statement
						{
							$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
						}					
					| write_statement
					{
						$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
					}
					| read_statement
						{
							$$ = create_node(NOTHING,STATEMENT,$1,NULL,NULL);
						}
					;
assignment_statement			: expression ASSIGNMENT variable
						{
							$$ = create_node(NOTHING,ASSIGNMENT_STATEMENT,$1,$3,NULL);
						}
					;
if_statement				: IF conditional THEN statement_list ENDIF
						{
							$$ = create_node(NOTHING,IF_STATEMENT,$2,$4,NULL);
						}
					| IF conditional THEN statement_list ELSE statement_list ENDIF
						{
							$$ = create_node(NOTHING,IF_STATEMENT,$2,$4,$6);
						}
					;
while_statement				: WHILE conditional DO statement_list ENDWHILE	
						{
							$$ = create_node(NOTHING,WHILE_STATEMENT,$2,$4,NULL);
						}		
					;
do_statement				: DO statement_list WHILE conditional ENDDO
						{
							$$ = create_node(NOTHING,DO_STATEMENT,$2,$4,NULL);
						}
					;
for_statement				: FOR variable IS expression BY expression TO expression DO statement_list ENDFOR
						{
							$$ = create_node(NOTHING,FOR_STATEMENT,create_node(NOTHING,NOTHING,$2,$4,$6),$8,$10);
						}
					;
write_statement				: WRITE OPEN output_list CLOSE
					{
						$$ = create_node(NOTHING,WRITE_STATEMENT,$3,NULL,NULL);
					}
					| NEWLINE
					{
						$$ = create_node(NOTHING,WRITE_STATEMENT,NULL,NULL,NULL);
					}
					;
read_statement				: READ OPEN variable CLOSE
						{
							$$ = create_node(NOTHING,READ_STATEMENT,$3,NULL,NULL);
						}
					;
output_list			 	: value COMMA output_list
					{
						$$ = create_node(NOTHING,OUTPUT_LIST,$1,$3,NULL);
					}
					| value
					{
						$$ = create_node(NOTHING,OUTPUT_LIST,$1,NULL,NULL);
					}
					;
conditional				: NOT conditional
						{
							$$ = create_node(NOTHING,CONDITIONAL,$2,NULL,NULL);
						}
					| expression comparator expression AND conditional
						{
							$$ = create_node(NOTHING,CONDITIONAL_AND,create_node(NOTHING,CONDITIONAL,$1,$2,$3),$5,NULL);
						}
					| expression comparator expression OR conditional
						{
							$$ = create_node(NOTHING,CONDITIONAL_OR,create_node(NOTHING,CONDITIONAL,$1,$2,$3),$5,NULL);
						}
					| expression comparator expression
						{
							$$ = create_node(NOTHING,CONDITIONAL,$1,$2,$3);
						}
					;
comparator			 	: EQUAL_TO
 						{
							$$ = create_node(NOTHING,ET,NULL,NULL,NULL);
						}
						| NOT_EQUAL_TO 
						{
							$$ = create_node(NOTHING,NET,NULL,NULL,NULL);
						}
						| LESS_THAN
						{
							$$ = create_node(NOTHING,LT,NULL,NULL,NULL);
						}
						| GREATER_THAN
 						{
							$$ = create_node(NOTHING,GT,NULL,NULL,NULL);
						}						
						| LESS_THAN_OR_EQUAL_TO
 						{
							$$ = create_node(NOTHING,LTOET,NULL,NULL,NULL);
						}						
						| GREATER_THAN_OR_EQUAL_TO
 						{
							$$ = create_node(NOTHING,GTOET,NULL,NULL,NULL);
						}
						;
expression			 	: term PLUS expression
 						{
							$$ = create_node(NOTHING,EXPRESSION_PLUS,$1,$3,NULL);
						}
					| term MINUS expression
					 	{
							$$ = create_node(NOTHING,EXPRESSION_MINUS,$1,$3,NULL);
						}
					| term
					 	{
							$$ = create_node(NOTHING,EXPRESSION,$1,NULL,NULL);
						}
					;
term			 		: value MULTIPLY term
 						{
							$$ = create_node(NOTHING,TERM_MUL,$1,$3,NULL);
						}
					| value DIVIDE term
					 	{
							$$ = create_node(NOTHING,TERM_DIV,$1,$3,NULL);
						}
					| value
					 	{
							$$ = create_node(NOTHING,TERM,$1,NULL,NULL);
						}
					;
value			 		: variable
 						{
							$$ = create_node(NOTHING,VALUE,$1,NULL,NULL);
						}
					| constant
					{
						$$ = create_node(NOTHING,VALUE,$1,NULL,NULL);
					}
					| OPEN expression CLOSE
					 	{
							$$ = create_node(NOTHING,VALUE,$2,NULL,NULL);
						}
					;
constant			 	: CHARACTER_CONSTANT
					{
						$$ = create_node($1,CHAR_CONSTANT,NULL,NULL,NULL);
					}
					| MINUS NUMBER_CONSTANT
					 	{
							$$ = create_node($2,NEG_CONSTANT,NULL,NULL,NULL);
						}
					| NUMBER_CONSTANT
					 	{
							$$ = create_node($1,NUM_CONSTANT,NULL,NULL,NULL);
						}
					;

variable				: IDENTIFIER
 						{
							$$ = create_node($1,VARIABLE,NULL,NULL,NULL);
						}
					;

%%

/*method that creates each node by placing the pass values into their respective variables */
TERNARY_TREE create_node(int ival, int case_identifier, TERNARY_TREE p1, TERNARY_TREE p2, TERNARY_TREE p3)
{
	TERNARY_TREE t;
	t = (TERNARY_TREE)malloc(sizeof(TREE_NODE));
	t -> item = ival;
	t -> nodeIdentifier = case_identifier;
	t -> first = p1;
	t -> second = p2;
	t -> third = p3;
	return(t);
}

void print_tree(TERNARY_TREE t)
{
	if(t == NULL)
	{
		return;
	}
	printf("Item: %s", symTab[t -> item] -> identifier);
	printf(" nodeIdentifier: %s\n",node[(t -> nodeIdentifier)]);
	print_tree(t -> first);
	print_tree(t -> second);
	print_tree(t -> third);	
}

#include "lex.yy.c"
