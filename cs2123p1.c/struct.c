
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


#define MAX_TOKEN 50
#define MAX_OUT_ITEM 50
// Token typedef used for operators, operands, and parentheses
typedef char Token[MAX_TOKEN + 1];

// Element typedef used for Element values placed in the stack or out
typedef struct
{
    Token szToken;
    int iCategory;
    int iPrecedence;
} Element;
// OutImp typedef defines how we implement out
typedef struct
{
    int iOutCount;
    Element outM[MAX_OUT_ITEM];
} OutImp;

// Out typedef defines a pointer to out
typedef OutImp *Out;
void addOut(Out out, Element element);
int main()
{
    char input[200];
    FILE *fp;
    fp = fopen("cs2123p1Input.txt","r");
    if (fp = NULL)
        printf("Error cannot open file.\n");

    fscanf(fp,"%s",input);
    printf("%s",input);

    fclose(fp);

    return 0;


}




/******************** addOut **************************************
void addOut(Out out, Element element)
Purpose:
    Adds an element to out.
Parameters:
    I/O Out out                 Stores the postfix expression
    I Element element           Element structure to be added to out.
Returns:
    n/a
Notes:
    - Since out uses an array, addOut does a boundary check for overflow.
**************************************************************************/
void addOut(Out out, Element element)
{
    if (out->iOutCount >= MAX_OUT_ITEM)
        printf("Overflow in the out array");
    out->outM[out->iOutCount++] = element;
}
