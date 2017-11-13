#include "declarations.c"
#include<stdio.h>

int cPos,iPos,rPos = 0;
char* cArr[5][1];
char* iArr[5][1];
char* rArr[5][1];
char typeID;

void gen(TERNARY_TREE);
void genDBlock(TERNARY_TREE);
void genState(TERNARY_TREE);
void genComp(TERNARY_TREE);
void getVar(TERNARY_TREE);
void getCon(TERNARY_TREE);

//start
void gen(TERNARY_TREE t)
{
	if(t == NULL) return;
	switch(t -> nodeIdentifier)
	{
		//if its a program
		case(PROGRAM):
			//get the method name
			//gen(t -> first);
			//print the parameters
			printf("void main()\n{\n");
			//get the rest of the program
			gen(t -> second);
			//print the closing bracket
			printf("}\n");
			return;
		//if its a block, statement list or output list
		case(BLOCK):
		case(STATEMENT_LIST):
		case(OUTPUT_LIST):
			//pass the branches through
			gen(t -> first);
			gen(t -> second);
			return;
		case(DECLARATION_BLOCK):
			//FUCKING COMMA FUCKS #@*&#%#!*
			gen(t -> third);
			gen(t -> second);
			//gen(t -> first);
			if(node[(t -> second) -> nodeIdentifier] != "DECLARATION_BLOCK")
			{
				if(t -> third == NULL)
				{
					gen(t -> first);
				printf(",\n");
					return;
				}
			}
			gen(t -> first);
			printf(";\n");
			return;
		case(VAR_TYPE_CHAR):
			printf("char ");
			return;
		case(VAR_TYPE_INT):
			printf("int ");
			return;
		case(VAR_TYPE_REAL):
			printf("real ");
			return;
		case(STATEMENT):
			genState(t -> first);
			return;
		case(CONDITIONAL):
			if((t -> second) == NULL)
			{
				printf("!(");
				gen(t -> first);
				printf(")");
				return;
			}
			gen(t -> first);
			genComp(t -> second);
			gen(t -> third);
			return;
		case(CONDITIONAL_AND):
			gen(t -> first);
			printf(" && ");
			gen(t -> second);
			return;
		case(CONDITIONAL_OR):
			gen(t -> first);
			printf(" || ");
			gen(t -> second);
			return;
		case(EXPRESSION):
			gen(t -> first);
			return;
		case(EXPRESSION_PLUS):
			gen(t -> first);
			printf(" + ");
			gen(t -> second);
			return;
		case(EXPRESSION_MINUS):
			gen(t -> first);
			printf(" - ");
			gen(t -> second);
			return;
		case(TERM):
			gen(t -> first);
			return;
		case(TERM_MUL):
			gen(t -> first);
			printf(" * ");
			gen(t -> second);
			return;
		case(TERM_DIV):
			gen(t -> first);
			printf(" / ");
			gen(t -> second);
			return;
		case(VALUE):
			gen(t -> first);
			return;
		case(VARIABLE):
			getVar(t);
			return;
		case(CHAR_CONSTANT):
			getCon(t);
			return;
		case(NUM_CONSTANT):
			getVar(t);
			return;
		case(NEG_CONSTANT):
			printf("-");
			getVar(t);
			return;
	}
}

void genDecBlock(TERNARY_TREE t)
{
	if(typeID == 'c')
	{
		cArr[cPos] = symTab[t -> item] -> identifier;
		cPos++;
	}
	if(typeID == 'i')
	{
		iArr[iPos] = symTab[t -> item] -> identifier;
		iPos++;
	}
	if(typeID == 'r')
	{
		rArr[rPos] = symTab[t -> item] -> identifier;
		rPos++;
	}
}

//handles all the various statements
void genState(TERNARY_TREE t)
{
	switch(t -> nodeIdentifier)
	{
		//if its an assignment
		case(ASSIGNMENT_STATEMENT):
			//get the variable name
			gen(t -> second);
			//print the equals
			printf(" = ");
			//do the rest
			gen(t -> first);
			//close the statement
			printf(";\n");
			return;
		//if its an if statement
		case(IF_STATEMENT):
			//print out the start of the statement
			printf("if(");
			//print out the conditional
			gen(t -> first);
			//close the condition and print out the statement list
			printf(")\n{\n");
			gen(t -> second);
			printf("}\n");
			//check for an else and print it out if needed
			if(t -> third != NULL)
			{
				printf("else\n{\n");
				gen(t -> third);
				printf("}\n");
			}
			return;
		case(WHILE_STATEMENT):
			printf("while(");
			gen(t -> first);
			printf(")\n{\n");
			gen(t -> second);
			printf("}\n");
			return;
		case(DO_STATEMENT):
			printf("do\n{\n");
			gen(t -> first);
			printf("} while(");
			gen(t -> second);
			printf(")\n");
			return;
		case(FOR_STATEMENT):
			printf("for(");
			gen((t -> first) -> first);
			printf(" = ");
			gen((t -> first) -> second);
			printf(", ");
			gen((t -> first) -> first);
			printf(" != ");
			gen(t -> second);
			printf(", ");
			gen((t -> first) -> first);
			printf(" = ");
			gen((t -> first) -> first);
			printf(" + ");
			gen((t -> first) -> third);
			printf(")\n{\n");
			gen(t -> third);
			printf("}\n");
			return;
		//if its  a write statement
		case(WRITE_STATEMENT):
			//print out the start of the function
			printf("printf(");
			//if the first branch isnt empty
			if((t -> first) != NULL)
			{
				//if the third node from this (value) is constant
				if(node[((((t -> first) -> first) -> first) -> nodeIdentifier)] == "CHAR_CONSTANT")
				{
					//print speech marks around the constants
					printf("\"");
					gen(t -> first);
					printf("\"");
				}
				else
				{
					//pass it it throught the generator without any speech marks
					gen(t -> first);
				}
			}
			//else its a newline
			else
			{
				printf("\"\\n\"");
			}
			//close off the statement
			printf(");\n");
			return;
		//if its a read statement
		case(READ_STATEMENT):
			//print the starting line -- needs to be able to find the variable type to substitute in for the %s
			printf("scanf(\"[placeholder]\", &");
			//find the var
			gen(t -> first);
			//close the statement
			printf(");\n");
			return;
	}
}

void genComp(TERNARY_TREE t)
{
	switch(t -> nodeIdentifier)
	{
		case(ET):
			printf(" == ");
			return;
		case(NET):
			printf(" != ");
			return;
		case(LT):
			printf(" < ");
			return;
		case(GT):
			printf(" > ");
			return;
		case(LTOET):
			printf(" <= ");
			return;
		case(GTOET):
			printf(" >= ");
			return;
	}
}

//gets constants from the symbol table
void getCon(TERNARY_TREE t)
{
	//as its saved as 'x' it should only take the middle element
	char* str = (symTab[t -> item] -> identifier);
	printf("%c",str[1]);
}

void getVar(TERNARY_TREE t)
{
	printf("%s",symTab[t -> item] -> identifier);
}
