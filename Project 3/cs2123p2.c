#include "cs2123p2.h"
#include<stdio.h>
#include<string.h>

/*********************************evaluatePostfix***************************************************
*************cs2123p2.c by Carla Ramirez

void evaluatePostfix(Out out, Customer customerM[], int iNumCustomer, QueryResult resultM[])
Purpose:
	It returns the query result based on evaluating the query. resultM is an array of booleans corresponding
	to customerM array.
Parameters:
	I	Out out			struct to implement out array

	I	Customer customerM[]	struct that has customer id,name, and array of traits

	I	int iNumCustomer	number of customers in customerM

	I/O	QueryResult resultM[]	struct to implement query results array
Notes:
	The final resultM array contains boolean results for reach customer depending, will return
	true (1) if the customer satisfy the query, and return false (0) if the customer does not.
	In the main method if the customer does not satisfy the query they will not be printed in the
	output.
*********************************************************************************************************/
void evaluatePostfix(Out out, Customer customerM[], int iNumCustomer, QueryResult resultM[])
{
   int i,j;
   Element postElem; //the current element from postfix array
   Element op1,op2,result;	//elements in  postfix array push/popped using stack
   Stack stack = newStack(stack);// empty list to store operators to then be evaluated	
   Customer customer;	
   Trait trait;
//traverse through list of customers
   for(i=0; i < iNumCustomer; i++)
   {
//traverse though out array in postfix expression
	for(j=0; j < out->iOutCount; j++)
	{	
        	postElem = out->outM[j];
	       switch(postElem.iCategory)
         	{
	           case CAT_OPERAND:
	        	push(stack,postElem);	
	        	break;
	           case CAT_OPERATOR:
	        	if(strcmp(postElem.szToken,"=") == 0)
		        {
		              op2 = pop(stack);
	               	      op1 = pop(stack);
		              strcpy(trait.szTraitValue,op2.szToken);
		              strcpy(trait.szTraitType,op1.szToken);
		              result.bInclude = atLeastOne(&customerM[i], &trait);	
		              push(stack,result);  		
	        	}
	        	else if (strcmp(postElem.szToken,"NOTANY") == 0)
		        {
		             op2 = pop(stack);
		             op1= pop(stack);
		             strcpy(trait.szTraitValue,op2.szToken);
		             strcpy(trait.szTraitType,op1.szToken);
		             result.bInclude = notAny(&customerM[i], &trait);
		             push(stack,result);
		        }
		       else if (strcmp(postElem.szToken,"ONLY") == 0)
		        {
		            op2 = pop(stack);
		            op1= pop(stack);
		            strcpy(trait.szTraitValue,op2.szToken);
	                    strcpy(trait.szTraitType,op1.szToken);
		            result.bInclude = only(&customerM[i], &trait);
		            push(stack,result);
		        }
		       else if (strcmp(postElem.szToken,"AND") == 0)
		        {
		           op2 = pop(stack);
		           op1 = pop(stack);
		           result.bInclude = op2.bInclude && op1.bInclude;
		           push(stack,result);			
		        }	
	          	else if (strcmp(postElem.szToken,"OR") == 0)
		        {
		          op2 = pop(stack);
		          op2 = pop(stack);
		          result.bInclude = op2.bInclude || op1.bInclude;
		          push(stack,result);		
	     	        }	//end of else
		
	       }	//end of switch cases

	} // end of outter for loop
	freeStack(stack);
	result = pop(stack);
	resultM[i] = result.bInclude; // return true of false if the customer meets query conditions
    } //end of innter for loop

} // end function

/***********************convertToPostFix********************************************************************
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
	
****************************************************************************************************************/
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
/******************printCustomerData************************************************************************

char  printCustomerData(customerM, iNumberOfCustomers)

Purpose:
   	Prints customer id, name, traits and values for traits.

Parameters:
  	 O   Customer customerM[]     Customer array filled and returned
    	     

	  I  int iNumCustomer         The number of customers
Notes:
  	- Prints only valid customer info
  	- Prints multiple customer traits.
  	- If any or all of customer info is incorrect balnk space will appear.

Returned Value:

   	Customer info in table format.
*****************************************************************************************************************/

void printCustomerData(Customer customerM[], int iNumCustomer)
{
    int i;
    int j;
    	
    // Print a heading for the list of customers and traits
       printf("\n\nID     Customer Name:\n");
       printf("           Trait\t  Value\n");		
    for (i = 0; i < iNumCustomer; i++)
    {
        printf("%s   %-12s\n",customerM[i].szCustomerId,customerM[i].szCustomerName);

        // Print each of the traits
        for (j = 0; j < customerM[i].iNumberOfTraits; j++)
        {
            // Print a trait
	printf("\t    %-6s\t  %-8s\n",customerM[j].traitM[j].szTraitType,customerM[j].traitM[j].szTraitValue);

        } //end of inner for loop
    } //end of outter for loop
}// end of function
/******************** atLeastOne **************************************
int atLeastOne(Customer *pCustomer, Trait *pTrait)
Purpose:
    Determines whether a customer has at least one particular trait (type and
    value).  If he does, atLeastOne returns TRUE; otherwise, it 
    returns FALSE.
Parameters:
    I Customer pCustomer     One customer structure which also
                             contains traits of that customer.
    I Trait    pTrait        The trait that we do want this 
                             customer to have.
Notes:
    This function could be used by the function atLeast().
Return value:
    TRUE - customer did have a specified trait
    FALSE - customer didn't have it
**************************************************************************/
int atLeastOne(Customer *pCustomer, Trait *pTrait)
{
    return !notAny(pCustomer,pTrait);
}
/********************only**************************************
int only(Customer *pCustomer, Trait *pTrait)
Purpose:
    Determines whether a customer has only one particular trait (type and
    value).  If he does, only returns TRUE; otherwise, it 
    returns FALSE.
Parameters:
    I Customer pCustomer     One customer structure which also
                             contains traits of that customer.
    I Trait    pTrait        The only trait that we do want this 
                             customer to have.
Notes:
    This output will not be added to the resultM if the customers traits
    include additional traits for that trait type.
    
Return value:
    TRUE - customer did have only one specified trait
    FALSE - customer didn't have it
**************************************************************************/
int only(Customer *pCustomer, Trait *pTrait)
{

    int i;
    int bresult = 0; 	
    if (pCustomer == NULL)
        ErrExit(ERR_ALGORITHM
        , "received a NULL pointer");
    for (i = 0; i < (pCustomer->iNumberOfTraits); i++)
    {
        if(strcmp(pCustomer->traitM[i].szTraitType, pTrait->szTraitType) ==0
	   && strcmp(pCustomer->traitM[i].szTraitValue, pTrait->szTraitValue) == 0)
		
           bresult =  1;
	if(strcmp(pCustomer->traitM[i].szTraitType, pTrait->szTraitType) == 0
	   && strcmp(pCustomer->traitM[i].szTraitValue, pTrait->szTraitValue) != 0)
	return 0;
    }
	return bresult;

} // end of function



