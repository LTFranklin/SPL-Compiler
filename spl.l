%{
#ifdef PRINT
#include "declarations.c"
#define RETURN(type,token) printf(#type " : " #token "\n")
#define SETYYLVAL(x)
#else
#define	RETURN(type,token) return(token)
#define SETYYLVAL(x) yylval.iVal = x
#endif



/* Declare Symbol Table Type and Array as imported types */

#include <string.h>

extern SYMTABNODEPTR symTab[SYMTABSIZE];
extern int currentSymTabSize;
int installId(char *);
%}

delim					[ \t\n]
ws						{delim}+
character				[A-Za-z]
character_constant			\'{character}\'
identifier				{character}({character}|{digit})*
digit					[0-9]
number      			{digit}+
number_constant			{number}
real_constant			{number}([.]?{number})?
anything				.

%%

						int k;

{ws}					; /* white space, do nothing */

ENDP					RETURN(keyword, ENDP);

DECLARATIONS			RETURN(keyword, DECLARATIONS);
CODE					RETURN(keyword, CODE);

OF						RETURN(keyword, OF);
TYPE					RETURN(keyword, TYPE);

CHARACTER				RETURN(keyword, CHARACTER);
INTEGER					RETURN(keyword, INTEGER);
REAL					RETURN(keyword, REAL);

IF						RETURN(keyword, IF);
THEN					RETURN(keyword, THEN);
ELSE					RETURN(keyword, ELSE);
ENDIF					RETURN(keyword, ENDIF);

WHILE					RETURN(keyword, WHILE);
DO						RETURN(keyword, DO);
ENDWHILE				RETURN(keyword, ENDWHILE);
ENDDO					RETURN(keyword, ENDDO);

FOR						RETURN(keyword, FOR);
IS						RETURN(keyword, IS);
BY						RETURN(keyword, BY);
TO						RETURN(keyword, TO);
ENDFOR					RETURN(keyword, ENDFOR);

WRITE					RETURN(keyword, WRITE);
READ					RETURN(keyword, READ);
NEWLINE					RETURN(keyword, NEWLINE);

NOT						RETURN(keyword, NOT);
AND						RETURN(keyword, AND);
OR						RETURN(keyword, OR);

"+"						RETURN(binaryOp, PLUS);
"-"						RETURN(binaryOp, MINUS);
"*"						RETURN(binaryOp, MULTIPLY);
"/"						RETURN(binaryOp, DIVIDE);

"="						RETURN(comparator, EQUAL_TO);
"<>"					RETURN(comparator, NOT_EQUAL_TO);
"<"						RETURN(comparator, LESS_THAN);
">"						RETURN(comparator, GREATER_THAN);
"<="					RETURN(comparator, LESS_THAN_OR_EQUAL_TO);
">="					RETURN(comparator, GREATER_THAN_OR_EQUAL_TO);



":"						RETURN(seperator, COLON);
"."						RETURN(seperator, FULL_STOP);
","						RETURN(seperator, COMMA);
";"						RETURN(seperator, SEMI_COLON);

"->"					RETURN(assignment, ASSIGNMENT);

"("						RETURN(bracket, OPEN);
")"						RETURN(bracket, CLOSE);

"'"						RETURN(quote, QUOTE); 

{identifier}			{
					SETYYLVAL(installId(yytext));
					RETURN(identifier, IDENTIFIER);
				}

{character_constant}		{
					SETYYLVAL(installId(yytext));
					RETURN(character_constant, CHARACTER_CONSTANT);
				}
						
{number_constant}		{
					SETYYLVAL(installId(yytext));
					RETURN(int_constant, INT_CONSTANT);
				}
{real_constant}			{
					SETYYLVAL(installId(yytext));
					RETURN(real_constant, REAL_CONSTANT);
				}

%%						


SYMTABNODEPTR newSymTabNode()
{
    return ((SYMTABNODEPTR)malloc(sizeof(SYMTABNODE)));
}

int lookup(char *s)
{
    extern SYMTABNODEPTR symTab[SYMTABSIZE];
    extern int currentSymTabSize;
    int i;

    for(i=0; i<currentSymTabSize; i++)
    {
        if(strncmp(s,symTab[i]->identifier,IDLENGTH) == 0)
        {
            return (i);
        }
    }
    return (-1);    
}

/* Look up an identifier in the symbol table, if its there return
   its index.  If its not there, put it in the end position,
   as long as the table isn't full, and return its index.
*/

int installId(char *id) 
{
    extern SYMTABNODEPTR symTab[SYMTABSIZE]; 
    extern int currentSymTabSize;
    int index;

    index = lookup(id);
    if (index >= 0)
    {
        return (index);
    }
    else 
       if (currentSymTabSize >= SYMTABSIZE) 
          /* SYMTAB is full */
          return (NOTHING) ;
    else
    {
       symTab[currentSymTabSize] = newSymTabNode();
       /* Recommended code for preventing buffer overrun on bounded strings */
       strncpy(symTab[currentSymTabSize]->identifier,id,IDLENGTH);
       symTab[currentSymTabSize]->identifier[IDLENGTH-1] = '\0';
       return(currentSymTabSize++);
    }
}




