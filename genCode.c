#include "declarations.c"
#include<stdio.h>
#include<string.h>
#include <stdbool.h>

#define oneDeep first->first
#define twoDeep first->first->first
#define threeDeep first->first->first->first

int cPos,iPos,rPos,indent,oPos = 0;
char *output[1000];
char *cArr[10];
char *iArr[10];
char *rArr[10];
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
void confirmVarDec(char*);
void confirmVar(char*);
void checkNum(TERNARY_TREE);
char* getVar(TERNARY_TREE);
void getCon(TERNARY_TREE);
void addIndent();
void createOutput(char*);
void printOutput();
void optimise();
void varIniCheck(int);
void iniCleanUp(int);

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
				fprintf(stderr,"\nError: Program not closed correctly\n");
				exit(1);
			}
			createOutput("#include <stdio.h>\n");
			/*get the method name*/
			progName = getVar(t -> first);
			/*print the parameters*/
			createOutput("void main()\n{\n");
			/*add an indent*/
			indent++;
			/*get the rest of the program*/
			gen(t -> second);
			/*decrease the indent*/
			indent--;
			/*print the closing bracket*/
			createOutput("}\n");
			optimise();
			printOutput();
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
				createOutput("!(");
				/*go back through the tree*/
				gen(t -> first);
				/*close the statement*/
				createOutput(")");
				return;
			}
			/*handle the conditional*/
			gen(t -> first);
			genComp(t -> second);
			gen(t -> third);
			return;
		case(CONDITIONAL_AND):
			gen(t -> first);
			createOutput(" && ");
			gen(t -> second);
			return;
		case(CONDITIONAL_OR):
			gen(t -> first);
			createOutput(" || ");
			gen(t -> second);
			return;
		case(EXPRESSION):
			gen(t -> first);
			return;
		case(EXPRESSION_PLUS):
			/*makes sure the program isnt trying to use arithmatic on chars -> add error msg?*/
			checkNum(t -> twoDeep);
			gen(t -> first);
			createOutput(" + ");
			gen(t -> second);;
			return;
		case(EXPRESSION_MINUS):
			checkNum(t -> twoDeep);
			gen(t -> first);
			createOutput(" - ");
			gen(t -> second);
			return;
		case(TERM):
			gen(t -> first);
			return;
		case(TERM_MUL):
			checkNum(t -> oneDeep);
			gen(t -> first);
			createOutput(" * ");
			gen(t -> second);
			return;
		case(TERM_DIV):
			checkNum(t -> oneDeep);
			gen(t -> first);
			createOutput(" / ");
			gen(t -> second);
			return;
		case(VALUE):
			gen(t -> first);
			return;
		case(BRA_VALUE):
			if(node[(t -> threeDeep) -> nodeIdentifier] != NULL)
			{
				createOutput("(");
				gen(t -> first);
				createOutput(")");
			}
			return;
		case(VARIABLE):
			createOutput(getVar(t));
			return;
		case(CHAR_CONSTANT):
			getCon(t);
			return;
		case(NUM_CONSTANT):
			gen(t -> first);
			return;
		case(NEG_CONSTANT):
			createOutput("-");
			gen(t -> first);
			return;
		case(INTEGER_CONSTANT):
			createOutput(getVar(t));
			return;
		case(FLOAT_CONSTANT):
			createOutput(getVar(t));
			return;
	}
}

/*method that places the varaiable in the respective arrays*/
void genDBlock(TERNARY_TREE t)
{
	confirmVarDec(getVar(t));
	if(typeID == 'c')
	{
		symTab[t -> item] -> type = 'c';
		cArr[cPos] = getVar(t);
		cPos++;
	}
	if(typeID == 'i')
	{	
		symTab[t -> item] -> type = 'i';
		iArr[iPos] = getVar(t);
		iPos++;
	}
	if(typeID == 'r')
	{
		symTab[t -> item] -> type = 'r';
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
		createOutput("int ");
		/*go through the array*/
		int i;
		for(i = 0; i < iPos; i++)
		{
			/*print out the variable*/
			createOutput(iArr[i]);
			/*if its not the end of the array*/
			if(i != iPos - 1)
			{
				/*print a comma*/
				createOutput(",");
			}
			else
			{
				/*else print a semi-colon*/
				createOutput(";\n");
			}
		}
	}

	if(cPos != 0)
	{
		addIndent();
		createOutput("char ");
		int i;
		for(i = 0; i < cPos; i++)
		{
			createOutput(cArr[i]);
			if(i != cPos - 1)
			{
				createOutput(",");
			}
			else
			{
				createOutput(";\n");
			}
		}
	}

	if(rPos != 0)
	{
		addIndent();
		createOutput("float ");
		int i;
		for(i = 0; i < rPos; i++)
		{
			createOutput(rArr[i]);
			if(i != rPos - 1)
			{
				createOutput(",");
			}
			else
			{
				createOutput(";\n");
			}
		}
	}
	createOutput("\n");
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
			/*flag it as intialised*/
			symTab[t -> second -> item] -> initialised = 1;
			/*print the equals*/
			createOutput(" = ");
			/*print out the value*/
			if(node[t -> threeDeep -> nodeIdentifier] == "CHAR_CONSTANT")
			{
				createOutput("'");
				gen(t->first);
				createOutput("'");
			}
			else
			{
				if(node[t -> threeDeep -> nodeIdentifier] == "VARIABLE")
				{
					if(symTab[t -> threeDeep -> item] -> initialised != 1)
					{
						fprintf(stderr,"Error: Assigning non-initialised variable %s",symTab[t -> threeDeep -> first -> item] -> identifier);
					}
				}
				gen(t -> first);
			}
			/*close the statement*/
			createOutput(";\n");
			return;
		/*if its an if statement*/
		case(IF_STATEMENT):
			addIndent();
			/*print out the start of the statement*/
			createOutput("if(");
			/*print out the conditional*/
			gen(t -> first);
			/*close the condition and print out the statement list*/
			createOutput(")\n");
			addIndent();
			indent++;
			createOutput("{\n");
			gen(t -> second);
			indent--;
			addIndent();
			createOutput("}\n");
			/*check for an else and print it out if needed*/
			if(t -> third != NULL)
			{
				addIndent();
				createOutput("else\n");
				addIndent();
				indent++;
				createOutput("{\n");
				gen(t -> third);
				indent--;
				addIndent();
				createOutput("}\n");
			}
			return;
		case(WHILE_STATEMENT):
			/*print out the while statement*/
			addIndent();
			createOutput("while(");
			/*go through the conditional*/
			gen(t -> first);
			/*close the statement*/
			createOutput(")\n");
			addIndent();
			indent++;
			createOutput("{\n");
			/*go through the statement list*/
			gen(t -> second);
			indent--;
			addIndent();
			createOutput("}\n");
			return;
		case(DO_STATEMENT):
			/*print the do*/
			addIndent();
			createOutput("do\n");
			addIndent();
			indent++;
			createOutput("{\n");
			/*go through the statements*/
			gen(t -> first);
			indent--;
			addIndent();
			/*close the do while and print the the conditional*/
			createOutput("} while(");
			gen(t -> second);
			createOutput(");\n");
			return;
		case(FOR_STATEMENT):
			/*print the for*/
			addIndent();
			createOutput("for(");
			/*make sure that the variable isnt a int*/
			if(getVarType(getVar(t -> oneDeep)) == 'c')
			{
				fprintf(stderr,"\nError: Using a character as an integer\n");
				exit(1);
			}
			/*get the variable name*/
			gen(t->oneDeep);
			/*flag the var as initialised*/
			symTab[t -> oneDeep -> item] -> initialised = 1;
			createOutput(" = ");
			/* get the value to assign to it*/
			checkNum((t -> first) -> second);
			gen((t -> first) -> second);
			createOutput("; ");
			/*get the value again*/
			gen(t->oneDeep);
			if(node[(((t -> first) -> third)-> twoDeep)-> nodeIdentifier] == "NUM_CONSTANT")
			{
				createOutput(" <= ");
			}
			else
			{
				createOutput(" >= ");
			}
			/*get the value it needs to reach*/
			gen(t -> second);
			createOutput("; ");
			/*set the incriment*/
			gen(t -> oneDeep);
			createOutput(" = ");
			gen(t -> oneDeep);
			createOutput(" + ");
			gen(((t -> first) -> third)-> first);
			createOutput(")\n");
			addIndent();
			indent++;
			createOutput("{\n");
			gen(t -> third);
			indent--;
			addIndent();
			createOutput("}\n");
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
				createOutput("printf(\"\\n\");\n");
			}
			return;
		/*if its a read statement*/
		case(READ_STATEMENT):
			addIndent();
			/*print the starting line*/
			createOutput("scanf(\" ");
			printVarType(t);
			createOutput("\", &");
			/*find the var*/
			gen(t -> first);
			/*flag the var as initialised*/
			symTab[t -> first -> item] -> initialised = 1;
			/*close the statement*/
			createOutput(");\n");
			return;
	}
}

void writeOutput(TERNARY_TREE t)
{	
	/*if the value is a variable*/
        if(node[(t -> first -> nodeIdentifier)] == "VARIABLE")
        {
		/*output an error if it isnt initialised*/
		if(symTab[t -> first -> item] -> initialised != 1)
		{
			fprintf(stderr,"Error: Variable %s is not initialised",symTab[t -> first -> item] -> identifier);
			exit(1);
		}
        	createOutput("printf(\"");
        	printVarType(t);	
        	createOutput("\", ");
        	gen(t -> first);
        	createOutput(");\n");
        }
        /*else its a constant*/
        else
        {
        	if(node[(t -> first -> nodeIdentifier)] == "CHAR_CONSTANT")
		{
			createOutput("printf(\"");
			gen(t -> first);
			createOutput("\");\n");
		}
		else
		{
			if(node[t -> oneDeep -> nodeIdentifier] == "FLOAT_CONSTANT")
			{
				createOutput("printf(\"%f\",");
				gen(t -> first);
				createOutput(");\n");
			}
			else
			{
				createOutput("printf(\"%d\",");
				gen(t -> first);
				createOutput(");\n");
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
		fprintf(stderr, "\nError: Variable %s doesn't exist\n", var);
		exit(1);
	}
}

void confirmVarDec(char* var)
{
	/*exit if the variable being declared already exists*/
	if(getVarType(var) != 'x' && declaring == true)
	{
		fprintf(stderr, "\nError: Varible %s has already been declared\n",var);
		exit(1);
	}
	/*exit if the var name is the same as the programs*/
	if(var == progName)
	{
		fprintf(stderr,"\nError: Variable %s shares an identifier with the program\n",var);
		exit(1);
	}
	/*if the variable name is the same as a reserved keyword, add an x on the end to avoid the error*/
	if(declaring == true)
	{
		int i=0;
		while(i<30)
		{
			if(strcmp(reserved[i],var) == 0)
			{
				var[strlen(var)] = 'x';
				return;
			}
			++i;
		}
	}
	return;
}

void checkNum(TERNARY_TREE t)
{
	if(node[t -> nodeIdentifier] == "CHAR_CONSTANT")
	{
		fprintf(stderr,"\nError: Attempting to use a character as a number\n");
		exit(1);
	}
	return;
}

void printVarType(TERNARY_TREE t)
{
	if(getVarType(getVar(t -> first)) == 'c')
	{
		createOutput("%c");
	}
	if(getVarType(getVar(t -> first)) == 'i')
	{
		createOutput("%d");
	}
	if(getVarType(getVar(t -> first)) == 'r')
	{
		createOutput("%f");
	}
}

void genComp(TERNARY_TREE t)
{
	switch(t -> nodeIdentifier)
	{
		case(ET):
			createOutput(" == ");
			return;
		case(NET):
			createOutput(" != ");
			return;
		case(LT):
			createOutput(" < ");
			return;
		case(GT):
			createOutput(" > ");
			return;
		case(LTOET):
			createOutput(" <= ");
			return;
		case(GTOET):
			createOutput(" >= ");
			return;
	}
}

/*gets constants from the symbol table*/
void getCon(TERNARY_TREE t)
{
	/*as its saved as 'x' it should only take the middle element*/
	char* str = (symTab[t -> item] -> identifier);
	char* nStr = malloc(2);
	nStr[0] = str[1];
	nStr[1] = '\0';
	createOutput(nStr);
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
		createOutput("\t");
		j--;
	}
}

void createOutput(char* c)
{
	output[oPos] = c;
	oPos++;
	return;
}

void printOutput()
{
	int i;
	for(i = 0;i < oPos;i++)
	{
		printf("%s",output[i]);
	}
	return;
}

void optimise()
{
	int i;
	for(i=0;i < oPos; i++)
	{
		if(!strcmp(output[i],"int ") || !strcmp(output[i],"char ") || !strcmp(output[i],"float "))
		{
			varIniCheck(i+1);
			iniCleanUp(i);
		}
	}

}

void varIniCheck(int pos)
{
	/*if the next value is a semi colon the type's decs are done*/
	if(!strcmp(output[pos],";\n"))
	{
		return;
	}
	else
	{
		/*if its not a comma*/
		if(strcmp(output[pos],","))
		{
			int i;
			/*find the value in the symbol table*/
			for (int i = 0; i < currentSymTabSize-1; i++)
			{
				if(!strcmp(symTab[i] -> identifier,output[pos]) && symTab[i] -> initialised != 1)
				{
					output[pos] = "";
					if(!strcmp(output[pos-1],","))
					{
						output[pos-1] = "";
					}
				}
			}
		}
		varIniCheck(pos+1);
	}
}

void iniCleanUp(int pos)
{
	int flag = 0;
	int i = pos+1;
	/*while a var's decs are being made*/
	while(strcmp(output[i],";\n"))
	{
		/*if there's a var flag it as okay*/
		if(strcmp(output[i],""))
		{
			flag = 1;
		}
		i++;
	}
	/*if there are no var being declared of that type anymore remove the type,indent and the semicolon*/
	if(flag == 0)
	{
		output[pos-1] = "";
		output[pos] = "";
		output[i] = "";
	}
	/*clean up any remianing commas*/
	if(!strcmp(output[i-2],",") && !strcmp(output[i-3],""))
	{
		output[i-2] = "";
	}
}
