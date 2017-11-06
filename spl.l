%{
#ifdef PRINT
#define RETURN(type,token) printf(#type " : " #token "\n")
#else
#define	RETURN(type,token) return(token)
#endif
%}

delim					[ \t\n]
ws						{delim}+
character				[A-Za-z]
character_constant		\'{character}\'
identifier				{character}({character}|{digit})*
digit					[0-9]
number      			{digit}+
number_constant			{number}([.]?{number})?
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

{identifier}			RETURN(identifier, IDENTIFIER);

{character_constant}	RETURN(character_constant, CHARACTER_CONSTANT);

{number_constant}		{
							k = atoi(yytext);
							RETURN(number_constant, NUMBER_CONSTANT);
						};


