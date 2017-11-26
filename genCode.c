#include "declarations.c"
#include<stdio.h>
#include<string.h>
#include <stdbool.h>

#define oneDeep first->first
#define twoDeep first->first->first
#define threeDeep first->first->first->first

int cPos,iPos,rPos,indent = 0;
char *cArr[5];
char *iArr[5];
char *rArr[5];
char *progName;
char typeID;
bool declaring = true;

void gen(TERNARY_TREE);
void genDBlock(TERNARY_TREE);
void printDecs();
void genState(TERNARY_TREE);
void writeOutput(TERNARY_TREE);
char getVarType(char*);
void printVarType(TERNARY_TREE);
void genComp(TERNARY_TREE);
void confirmVar(char*);
void checkNum(TERNARY_TREE);
char* getVar(TERNARY_TREE);
void getCon(TERNARY_TREE);
void addIndent();

/*start*/
void gen(TERNARY_TREE t)
{
	if(t == NULL) return;
	switch(t -> nodeIdentifier)
	{
		/*if its a program*/
		case(PROGRAM):
			/*make sure the program is closed correctly*/
			if(getVar(t -> first) != getVar(t -> third))
			{
				exit(1);
			}
			printf("#include <stdio.h>\n");
			/*get the method name*/
			progName = getVar(t -> first);
			/*print the parameters*/
			printf("void main()\n{\n");
			/*add an indent*/
			indent++;
			/*get the rest of the program*/
			gen(t -> second);
			/*decrease the indent*/
			indent--;
			/*print the closing bracket*/
			printf("}\n");
			return;
		/*if its a block, statement list or output list*/
		case(BLOCK):
			/*pass the first branch*/
			gen(t -> first);
			/*if the second branch isnt null*/
			if(t -> second != NULL)
			{
				/*the first branch contained the declarations so print them out*/
				printDecs();
				/*flag they're done*/
				declaring = false;
				/*and continue with the tree*/
				gen(t -> second);
			}
			return;
		case(STATEMENT_LIST):
			gen(t -> first);
			gen(t -> second);
			return;
		case(OUTPUT_LIST):
			addIndent();
			/*pass the branches through*/
			writeOutput(t -> first);
			gen(t -> second);
			return;
		case(DECLARATION_BLOCK):
			/*go through the tree backwards because the type is stored last*/
			gen(t -> third);
			/*get the data type*/
			gen(t -> second);
			/*then store all the varables*/
			genDBlock(t -> first);
			return;
		case(VAR_TYPE_CHAR):
			/*flag the type as char*/
			typeID = 'c';
			return;
		case(VAR_TYPE_INT):
			/*flag the type as int*/
			typeID = 'i';
			return;
		case(VAR_TYPE_REAL):
			/*flag the type as real*/
			typeID = 'r';
			return;
		case(STATEMENT):
			/*pass it into the statement generation*/
			genState(t -> first);
			return;
		case(CONDITIONAL):
			/*if te second branch is null*/
			if((t -> second) == NULL)
			{
				/*the statement is a not statement so add the ! */
				printf("!(");
				/*go back through the tree*/
				gen(t -> first);
				/*close the statement*/
				printf(")");
				return;
			}
			/*handle the conditional*/
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
			/*makes sure the program isnt trying to use arithmatic on chars -> add error msg?*/
			checkNum(t -> twoDeep);
			gen(t -> first);
			printf(" + ");
			gen(t -> second);;
			return;
		case(EXPRESSION_MINUS):
			checkNum(t -> twoDeep);
			gen(t -> first);
			printf(" - ");
			gen(t -> second);
			return;
		case(TERM):
			gen(t -> first);
			return;
		case(TERM_MUL):
			checkNum(t -> oneDeep);
			gen(t -> first);
			printf(" * ");
			gen(t -> second);
			return;
		case(TERM_DIV):
			checkNum(t -> oneDeep);
			gen(t -> first);
			printf(" / ");
			gen(t -> second);
			return;
		case(VALUE):
			gen(t -> first);
			return;
		case(BRA_VALUE):
			if(node[(t -> threeDeep) -> nodeIdentifier] != NULL)
			{
				printf("(");
				gen(t -> first);
				printf(")");
			}
			return;
		case(VARIABLE):
			printf("%s",getVar(t));
			return;
		case(CHAR_CONSTANT):
			getCon(t);
			return;
		case(NUM_CONSTANT):
			gen(t -> first);
			return;
		case(NEG_CONSTANT):
			printf("-");
			gen(t -> first);
			return;
		case(INTEGER_CONSTANT):
			printf("%s",getVar(t));
			return;
		case(FLOAT_CONSTANT):
			printf("%s",getVar(t));
			return;
	}
}

/*method that places the varaiable in the respective arrays*/
void genDBlock(TERNARY_TREE t)
{
	if(typeID == 'c')
	{
		cArr[cPos] = getVar(t);
		cPos++;
	}
	if(typeID == 'i')
	{	
		iArr[iPos] = getVar(t);
		iPos++;
	}
	if(typeID == 'r')
	{
		rArr[rPos] = getVar(t);
		rPos++;
	}
	return;
}

/*print out the declarations blocks*/
void printDecs()
{
	/*if there is items in the array*/
	if(iPos != 0)
	{
		addIndent();
		/*print the data type*/
		printf("int ");
		/*go through the array*/
		int i;
		for(i = 0; i < iPos; i++)
		{
			/*print out the variable*/
			printf("%s",iArr[i]);
			/*if its not the end of the array*/
			if(i != iPos - 1)
			{
				/*print a comma*/
				printf(",");
			}
			else
			{
				/*else print a semi-colon*/
				printf(";\n");
			}
		}
	}

	if(cPos != 0)
	{
		addIndent();
		printf("char ");
		int i;
		for(i = 0; i < cPos; i++)
		{
			printf("%s",cArr[i]);
			if(i != cPos - 1)
			{
				printf(",");
			}
			else
			{
				printf(";\n");
			}
		}
	}

	if(rPos != 0)
	{
		addIndent();
		printf("float ");
		int i;
		for(i = 0; i < rPos; i++)
		{
			printf("%s",rArr[i]);
			if(i != rPos - 1)
			{
				printf(",");
			}
			else
			{
				printf(";\n");
			}
		}
	}
	printf("\n");
}

/*handles all the various statements*/
void genState(TERNARY_TREE t)
{
	switch(t -> nodeIdentifier)
	{
		/*if its an assignment*/
		case(ASSIGNMENT_STATEMENT):
			addIndent();
			/*get the variable name*/
			gen(t -> second);
			/*print the equals*/
			printf(" = ");
			/*print out the value*/
			if(node[t -> threeDeep -> nodeIdentifier] == "CHAR_CONSTANT")
			{
				printf("'");
				gen(t->first);
				printf("'");
			}
			else
			{
				gen(t -> first);
			}
			/*close the statement*/
			printf(";\n");
			return;
		/*if its an if statement*/
		case(IF_STATEMENT):
			addIndent();
			/*print out the start of the statement*/
			printf("if(");
			/*print out the conditional*/
			gen(t -> first);
			/*close the condition and print out the statement list*/
			printf(")\n");
			addIndent();
			indent++;
			printf("{\n");
			gen(t -> second);
			indent--;
			addIndent();
			printf("}\n");
			/*check for an else and print it out if needed*/
			if(t -> third != NULL)
			{
				addIndent();
				printf("else\n");
				addIndent();
				indent++;
				printf("{\n");
				gen(t -> third);
				indent--;
				addIndent();
				printf("}\n");
			}
			return;
		case(WHILE_STATEMENT):
			/*print out the while statement*/
			addIndent();
			printf("while(");
			/*go through the conditional*/
			gen(t -> first);
			/*close the statement*/
			printf(")\n");
			addIndent();
			indent++;
			printf("{\n");
			/*go through the statement list*/
			gen(t -> second);
			indent--;
			addIndent();
			printf("}\n");
			return;
		case(DO_STATEMENT):
			/*print the do*/
			addIndent();
			printf("do\n");
			addIndent();
			indent++;
			printf("{\n");
			/*go through the statements*/
			gen(t -> first);
			indent--;
			addIndent();
			/*close the do while and print the the conditional*/
			printf("} while(");
			gen(t -> second);
			printf(");\n");
			return;
		case(FOR_STATEMENT):
			/*print the for*/
			addIndent();
			printf("for(");
			/*make sure that the variable isnt a int*/
			if(getVarType(getVar(t -> oneDeep)) == 'c')
			{
				exit(1);
			}
			/*get the variable name*/
			gen(t->oneDeep);
			printf(" = ");
			/* get the value to assign to it*/
			checkNum((t -> first) -> second);
			gen((t -> first) -> second);
			printf("; ");
			/*get the value again*/
			gen(t->oneDeep);
			if(node[(((t -> first) -> third)-> twoDeep)-> nodeIdentifier] == "NUM_CONSTANT")
			{
				printf(" <= ");
			}
			else
			{
				printf(" >= ");
			}
			/*get the value it needs to reach*/
			gen(t -> second);
			printf("; ");
			/*set the incriment*/
			gen(t -> oneDeep);
			printf(" = ");
			gen(t -> oneDeep);
			printf(" + ");
			gen(((t -> first) -> third)-> first);
			printf(")\n");
			addIndent();
			indent++;
			printf("{\n");
			gen(t -> third);
			indent--;
			addIndent();
			printf("}\n");
			return;
		/*if its  a write statement*/
		case(WRITE_STATEMENT):
			/*if the first branch isnt null its an output list*/
			if(t -> first != NULL)
			{
				gen(t->first);
			}
			/*else its  a newline*/
			else
			{
				addIndent();
				printf("printf(\"\\n\");\n");
			}
			return;
		/*if its a read statement*/
		case(READ_STATEMENT):
			addIndent();
			/*print the starting line -- needs to be able to find the variable type to substitute in for the %s*/
			printf("scanf(\" ");
			printVarType(t);
			printf("\", &");
			/*find the var*/
			gen(t -> first);
			/*close the statement*/
			printf(");\n");
			return;
	}
}

void writeOutput(TERNARY_TREE t)
{	
	/*if the value is a variable*/
        if(node[(t -> first -> nodeIdentifier)] == "VARIABLE")
        {
        	printf("printf(\"");
        	printVarType(t);	
        	printf("\", ");
        	gen(t -> first);
        	printf(");\n");
        }
        /*else its a constant*/
        else
        {
        	if(node[(t -> first -> nodeIdentifier)] == "CHAR_CONSTANT")
		{
			printf("printf(\"");
			gen(t -> first);
			printf("\");\n");
		}
		else
		{
			if(node[t -> first -> first -> nodeIdentifier] == "FLOAT_CONSTANT")
			{
				printf("printf(\"%%f\",");
				gen(t -> first);
				printf(");\n");
			}
			else
			{
				printf("printf(\"%%d\",");
				gen(t -> first);
				printf(");\n");
			}
		}
	}
}

char getVarType(char* c)
{
	int i = 0;
	while(i < cPos)
	{
		if(cArr[i] == c)
		{
			return 'c';
		}
		i++;
	}
	i = 0;
	while(i < iPos)
	{
		if(iArr[i] == c)
		{
			return 'i';
		}
		i++;
	}
	i = 0;
	while(i < rPos)
	{
		if(rArr[i] == c)
		{
			return 'r';
		}
		i++;
	}
	/*if it doesnt exist return x*/
	return 'x';
}

void confirmVar(char* var)
{
	/*exit if the variable doesnt exist*/
	if(getVarType(var) == 'x'&& declaring == false)
	{
		exit(1);
	}
	/*exit if the variable being declared already exists*/
	if(getVarType(var) != 'x' && declaring == true)
	{
		exit(1);
	}
	/*exit if the var name is the same as the programs*/
	if(var == progName)
	{
		exit(1);
	}
	return;
}

void checkNum(TERNARY_TREE t)
{
	if(node[t -> nodeIdentifier] == "CHAR_CONSTANT")
	{
		exit(1);
	}
	return;
}

void printVarType(TERNARY_TREE t)
{
	if(getVarType(getVar(t -> first)) == 'c')
	{
		printf("%%c");
	}
	if(getVarType(getVar(t -> first)) == 'i')
	{
		printf("%%d");
	}
	if(getVarType(getVar(t -> first)) == 'r')
	{
		printf("%%f");
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

/*gets constants from the symbol table*/
void getCon(TERNARY_TREE t)
{
	/*as its saved as 'x' it should only take the middle element*/
	char* str = (symTab[t -> item] -> identifier);
	printf("%c",str[1]);
	return;
}

char* getVar(TERNARY_TREE t)
{
	char* x = symTab[t -> item] -> identifier;
	if(node[t -> nodeIdentifier] == "VARIABLE")
	{
		confirmVar(x);
	}

	/*printf("%s",node[t -> nodeIdentifier]);*/
	return x;
}

void addIndent()
{
	int j = indent;
	while(j > 0)
	{
		printf("\t");
		j--;
	}
}
