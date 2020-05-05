/*cs2123p1.c by Carla Ramirez
/***********************convertToPostFix***************************************
int convertToPostFix(char *pszInFix, Out out)
Purpose:
	Convert infix text to postfix notation. Return 0 if succesful conversion	
	occured. Otherwise return value to indicate an error occured. i.e missing
	right parenthsis or missing left parenthsis.

Parameters:
	I     char *pszInFix 	 infix in input file
	I/O   Out out		 Stores the postfix expression


Notes:
	Uses helper functions getToken and categorize in the cs2123p1Driver.c file
	getToken examines the input text and categorize sets the category and precedence
	to the operators.
	
Returns:
	0	Succesfuly converted from infix to postfix
	801	Missing right parenthsis
	802	Missing left parenthsis
	
**********************************************************************************/
#include "cs2123p1.h"
#include<stdio.h>
#include<string.h>

int convertToPostFix(char *pszInfix, Out out)
{
	int iConversion = 0;
	Element element; //element struct to pass by reference 	
	Stack stack = newStack(stack);// empty list to store operators
	Element value; // temp element to store pop element
	pszInfix = getToken(pszInfix,element.szToken,MAX_TOKEN);
	categorize(&element); //assign the iprecedence and icategory based on token
 	while(pszInfix != NULL)
	{
		switch(element.iCategory)
		{
		case CAT_OPERAND:
			addOut(out,element);
			break;
		case CAT_LPAREN:
			push(stack,element);
			break;
		case CAT_OPERATOR:
			while(!isEmpty(stack))
			{
				if(element.iPrecedence > topElement(stack).iPrecedence)
				break;					
				value = pop(stack);
				addOut(out,value);
				
			}
				push(stack,element);
				break;
		case CAT_RPAREN:
			while(!isEmpty(stack))
			{
				value = pop(stack);
				if(strcmp(value.szToken,"(") == 0)
					break;
					addOut(out,value);
			}
				if(strcmp(value.szToken,"(") != 0)
					iConversion = WARN_MISSING_LPAREN;//802
		}//end of switch		     		
			
			pszInfix = getToken(pszInfix,element.szToken,MAX_TOKEN);
			categorize(&element);
	}//end of while	  	
	while(!isEmpty(stack))
	{
		value = pop(stack);
		if(strcmp(value.szToken,"(") == 0)
			iConversion  = WARN_MISSING_RPAREN;//801
			addOut(out,value);
	}
	freeStack(stack);
	return iConversion;
}//end of func

