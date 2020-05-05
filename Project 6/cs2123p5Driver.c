/**************************************************************************************
   cs2123p5Driver.c skeletal - must be completed by student

   cs2123p5Driver.c skeletal completed by Carla Ramirez
*********************cs2123p5Driver.c*************************************************
Purpose:
    Create a diangosis tree using a binary tree which will be in an order
    maintained by the diagnosis in command file.
Command Parameters:
    N/A
Input:
    Command file < p5input.txt using command line arugments
SAMPLE INPUT:
    ROOT Q1
    TEXT Q Q1 Pain?
    *Define the left side of the tree
    NODE Q Q2 Q1 N
    TEXT Q Q2 Cough?
Results:
    Prints a diagnosis tree based on command file input provides a HELP function to
    show certain diagnosis based ont the answers.
Returns:
      0 - normal
     -1 - usage error
    903 - algorithm error
    900 - command line error
    905 - bad input 
Notes:
   1. This program uses a binary tree data structure to store the diagnosis tree information. 
   2. This program uses a pretty print approach to print the result of the tree to allow a
      nicer visual of the tree.
***********************************************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

// Define a string to return when getText can't find an ID.  This is done to
// allow a printf to simply print what getText() returns and not have to 
// deal with a null.
static char szUnknown[] = "*** unknown text id";


int main()
{
    // Variables for diagnosis tree
    NodeT *root = NULL;

    // Variables for text List
    TextEntry text;
    TextList textList = newTextList();

    // Variables for driver input control
    char szInputBuffer[MAX_LINE_SIZE+1];            // input command line
    int iScanfCnt;                                  // sscanf return
    char *pszRemaining;                             // getToken returns this
    Token szCommand;                                // Input Command

    //Variables for reading data input command arguments
    Element element;                                // Binary Tree element
    char szParentId[MAX_ID_SIZE + 1];               // parent Id for NODE command
    char cYN;                                       // Y or N value for NODE command
    char szAnswers[MAX_NUMBER_ANSWERS];             // Text for answers
    
    // Variables for results
    char *pszResultId;                              // Result of searchT or help
    
    // Read command lines until EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE, stdin) != NULL)
    {
        printf("%s", szInputBuffer);

        // If the line is just a comment, ignore it
        if (szInputBuffer[0] == '*')
            continue;                               // Command is a comment so skip it
        
        // get the command
        pszRemaining = getToken(szInputBuffer, szCommand, MAX_TOKEN_SIZE);

        // Determine what to invoke based on the command

        if (strcmp(szCommand, "ROOT") == 0)
        {   // ROOT id      - create the root for the tree
            pszRemaining = getToken(pszRemaining, element.szId, MAX_ID_SIZE);
            if (pszRemaining == NULL)
                ErrExit(ERR_DATA, "Invalid data for ROOT command, missing ID");

            element.cNodeType = 'Q';
            root = allocateNodeT(element);
        }

        else if (strcmp(szCommand, "NODE") == 0)
        {   // NODE type id parentId yn
            iScanfCnt = sscanf(pszRemaining, "%c %s %s %c"
                , &element.cNodeType, element.szId
                , szParentId, &cYN);
            if (iScanfCnt < 4)
                ErrExit(ERR_DATA, "Invalid data for NODE command: '%s'", pszRemaining);

            // insert your code to check warning cases and handle the insertion
		insertNodeT(root,element,szParentId,cYN);
        }

        else if (strcmp(szCommand, "TEXT") == 0)
        {   // TEXT type id displayText
            iScanfCnt = sscanf(pszRemaining, "%c %s %79[^\n]", &text.cType, text.szId, text.szText);
            if (iScanfCnt < 3)
                ErrExit(ERR_DATA, "Invalid data for TEXT command: '%s'", pszRemaining);

            addTextEntry(textList, text);
        }

        else if (strcmp(szCommand, "PRINT") == 0)
        {   // PRINT using your prettyPrintT routine
		prettyPrintT(root,1,textList);
        }

        else if (strcmp(szCommand, "HELP") == 0)
        {   // HELP id answers
            pszRemaining = getToken(pszRemaining, szAnswers, MAX_NUMBER_ANSWERS);
            if (pszRemaining == NULL)
                ErrExit(ERR_DATA, "Invalid data for HELP command, missing answers");

            pszResultId = help(root, szAnswers, 0, textList);

            // It is expected that help might return NULL, if the tree is 
            // not defined properly.  (not necessarily a student error)
            if (pszResultId == NULL)
                printf("\t*** Warning:  NULL returned from HELP\n");
            else 
                printf("\t%s: %s\n", pszResultId
                     , getText(textList, pszResultId));
        }

        else if (strcmp(szCommand, "DELETE") == 0)
        {
            pszRemaining = getToken(pszRemaining, element.szId, MAX_ID_SIZE);
            if (pszRemaining == NULL)
                ErrExit(ERR_DATA, "Invalid data for DELETE command, missing ID");

            // insert your code for the DELETE command which should remove the 
            // specified ID from its parent node and also free the subtree defined by
            // the specified ID
                deleteT(root,element.szId);
        }
    }
    // Your code to free the tree
    freeT(root);    
    // Free the textList
    free(textList);

    printf("\n");
    return 0;
}
/***************************help******************************************************
char * help(NodeT *p, char szAnswers[],int iAns, TextList textList)
Purpose:
    Returns the diagnosis based on the answers to the questions. If
    it doesnt have one, it returns NULL.
Parameters:
    I    NodeT *p            node which is top (root) to be passed in
    I    char szAnswers[]    array of answers to questions
    I    int iAns            index for incrementing through the array of answers
    I/O  TextList textList   list which contains many text entries to be added to tree
Notes:
    The answers are a string of Y and/or N answers. As the diagnosis, it displays the 
    question ID, its text and the answer. The result of HELP should cause a diagnosis ID
    and its text to be displayed.
Return value:
    Returns the textList information to display the ID and text for each node in diagnosis
    tree.
****************************************************************************************/
char * help(NodeT *p, char szAnswers[], int iAns, TextList textList)
{
   if(p == NULL)
	return NULL;
   //print right side of tree
   if(szAnswers[iAns] == 'Y')
   {
       printf("%c %s %s\n",p->element.cNodeType,p->element.szId,getText(textList,p->element.szId));
       help(p->pRight,szAnswers,iAns + 1,textList);
   }			
   //print left side of tree
   else if (szAnswers[iAns] == 'N')
   {
       printf("%c %s %s\n",p->element.cNodeType,p->element.szId,getText(textList,p->element.szId));
       help(p->pLeft,szAnswers,iAns + 1,textList);				
   }
}
/****************************deleteT****************************************************
void deleteT(NodeT *p, char szParentId[])
Purpose:
    Causes the specified node to be deleted from the tree and its parent will no longer
    reference it.
Parameters:
    I    NodeT *p            node which is the top (root) to be passed in
    I    char szParentId[]   array to search for the parent ID you wish to delete
Notes:
    Searches for the ID to be deleted from the tree then sets is Parent left and right
    children to be NULL.
***************************************************************************************/
void deleteT(NodeT *p, char szParentId[])
{
    NodeT *t;
    if(p == NULL)
        return;
    // find the node wish to delete from tree then free memory and set children to NULL
    t = searchT(p,szParentId);
    freeT(t);
    t->pLeft = NULL;
    t->pRight = NULL;
}
/*******************************freeT**************************************************
void freeT(Node *p)
Purpose:
    Free all nodes in this subtree.
Parameters:
    I    node which is the top (root) to be passed in
Notes:
    Uses recursion to traverse through the tree to then free all nodes.
**************************************************************************************/
void freeT(NodeT *p)
{
    if(p == NULL)
        return;
    freeT(p->pLeft);  // traverse tree to left
    freeT(p->pRight); // traverse tree to right
    free(p);
}
/**************************prettyPrintT*************************************************
void prettyPrintT(NodeT *p, int iIndent, TextList textList)
Purpose:
    print the contents of thre tree in a pretty print style
Parameters:
    I    NodeT *p            node which is the top of the tree (root) which is passed in
    I    int iIndent         allows print to  increase the amount of indentation each time for
                             a nicer print style.
    I    TextList textList   list of text entries
Notes:
    This function uses recursion to print each nodes information. The routine shows childern
    indented form the parent.
***************************************************************************************/
void prettyPrintT(NodeT *p, int iIndent, TextList textList)
{
    int i;
    if(p == NULL)
        return;
    prettyPrintT(p->pRight,iIndent + 1,textList);  
    printf("\n");
    for(i=0; i < iIndent; i++)
    {
        printf("\t");
    }
    printf("%c %s %s\n",p->element.cNodeType,p->element.szId,getText(textList,p->element.szId));
    prettyPrintT(p->pLeft,iIndent + 1,textList);
}
/***********************************searchT*********************************************
NodeT * searchT(NodeT *p, char szId[])
Purpose:
    Searches for parent of node to be inserted based on the ID in the command file. If no
    match is found then will return NULL.
Parameters:
    I   NodeT *p        node which is pointed to the top of the tree (root)
    I   char szId[]     ID of parent to search for and match
Notes:

Return Value:
    pointer to the node or NULL if not found    
**************************************************************************************/
NodeT * searchT(NodeT *p, char szId[])
{
    NodeT *t;
    if(p == NULL)
        return NULL;
    if(strcmp(szId,p->element.szId) == 0)
	return p;
    // if not found contiue to search the entire tree until Id found
    t = searchT(p->pLeft,szId);
    if( t != NULL)
        return t;
    else
	t = searchT(p->pRight,szId);
        return t;
}
/****************************allocateNodeT********************************************
NodeT * allocateNodeT(Element elem)
Purpose:
    Allocates a binary tree node, assigns the element value, and initalizes the pointers
    to NULL.
Parameters:
    I    Element elem    element value to be inserted into the node
Notes:
    memory is freed in main using freeT
Return Value:
    Returns a pointer to the newly allocated node.
**************************************************************************************/
NodeT * allocateNodeT(Element elem)
{
    NodeT *pNew = (NodeT*)malloc(sizeof(NodeT));
    pNew->element = elem;
    pNew->pLeft = NULL;
    pNew->pRight = NULL;
    return pNew;
}
/****************************insertNodeT**********************************************
NodeT * insertNodeT(NodeT *p, Element elem, char szParentId[], char cYN)
Purpose:
    Creates and inserted a newly allocated node into the diagnosis tree based on the
    instructions from the command file.
Parameters:
    I    NodeT *p            pointer to top of tree (root) to be passed in
    I    Element elem	     values of the newly created node
    I    char szParentId[]   Id to search for so that the node may be properly inserted as
                             child to parent node.
    I    char cYN            char to determine direction of te node pleft or pright
Notes:
    if the NODE command references a branch of its parent which already has been populated
    , shows a warning that an answer has already been provided.
Return value:
    pointer to newly created node
**************************************************************************************/
NodeT * insertNodeT(NodeT *p, Element elem, char szParentId[],char cYN)
{
    NodeT *t;
    NodeT *pNew = allocateNodeT(elem);
    t = searchT(p,szParentId);
    if(t == NULL)
    {
        printf("***Warning parent not found!****\n"); //warning to show parent was not found
	return NULL;					
    }
    else if (pNew->pRight == NULL && cYN == 'Y')
	t->pRight = pNew;
    else if (pNew->pLeft == NULL && cYN == 'N')
	t->pLeft = pNew;
    else{
	printf("****Warning parent already populated*****\n");   //parent node already exsist
   	return NULL;
    }
    return pNew;	
}
/***  T E X T L I S T  functions ***/

/******************** addTextEntry **************************************
void addTextEntry(TextList textList, TextEntry text)
Purpose:
    Adds a text entry to the TextList.  It replaces it if it already exists.
Parameters:
    I/O TextList textList       A text list which contains many text
                                entries. The text entry is added to
                                this.
    I   TextEntry text          The text entry to be added to the
                                text list. 
Returns:
    n/a
Notes:
    - This uses an array implementation of TextList.  If the number of
      entries excedes MAX_TEXT_LIST_SIZE, it exits.
**************************************************************************/
void addTextEntry(TextList textList, TextEntry text)
{
    // See if it already exists
    int iFound = findText(textList, text.szId);
    if (iFound >= 0)
    {   // Replace the text entry
        textList->arrayM[iFound] = text;
        return;
    }
    // Not found, so try to insert it
    // Array boundary check
    if (textList->iNumEntry >= MAX_TEXT_LIST_SIZE)
        ErrExit(ERR_DATA, "Too many text entries");
    textList->arrayM[textList->iNumEntry] = text;
    textList->iNumEntry++;
}

/******************** getText **************************************
char * getText(TextList textList, char *pszId)
Purpose:
    Finds a text entry based on the specified ID and returns its text.
Parameters:
    I   TextList textList       A text list which contains many text
                                entries. 
    I   char *pszId             The text entry to be found in the
                                text list. 
Returns:
    A pointer to the text string of the specified ID.  If not found,
    it returns a pointer to szUnknown.
Notes:
    - This uses an array implementation of TextList.  
    - References the static global variable szUnknown which is returned
      when the specified ID is not found.  This allows getText() to be used
      in a printf() knowing that a non-NULL value is returned even when 
      the specified ID is not found in the text list.
    - Invokes findText to find it.
**************************************************************************/
char * getText(TextList textList, char *pszId)
{
    int iFound = findText(textList, pszId);
    if (iFound >= 0)
        return textList->arrayM[iFound].szText;
    else
       return szUnknown;
}
/******************** findText **************************************
int findText(TextList textList, char *pszId)
Purpose:
    Finds a text entry based on the specified ID and returns 
    its subscript.
Parameters:
    I   TextList textList       A text list which contains many text
                                entries. 
    I   char *pszId             The text entry to be found in the
                                text list. 
Returns:
    >= 0 - subscript of the specified ID in the text list. 
    -1   - not found 
Notes:
    - This uses an array implementation of TextList.  
**************************************************************************/
int findText(TextList textList, char *pszId)
{
    int i;
    for (i = 0; i < textList->iNumEntry; i++)
    {
        if (strcmp(textList->arrayM[i].szId, pszId) == 0)
            return i; 
    }
    return -1;  // not found
}

/******************** newTextList **************************************
TextList newTextList()
Purpose:
    Allocates a new TextList and initalizes it.
Parameters:
    n/a
Returns:
    A pointer to the newly allocated TextList. 
Notes:
    - This uses an array implementation of TextList.  
    - Initializes the number of entries to 0.
**************************************************************************/
TextList newTextList()
{
    TextList textList = (TextList)malloc(sizeof(TextListImp));
    if (textList == NULL)
        ErrExit(ERR_ALGORITHM, "malloc ran out of memory for new TextList");
    textList->iNumEntry = 0;
    return textList;
}

/***  U T I L I T Y functions ***/

/******************** ErrExit **************************************
  void ErrExit(int iexitRC, char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of 
    arguments after szFmt varies dependent on the format codes in
    szFmt.  
    It also exits the program with the specified exit return code.
Parameters:
    I   int iexitRC             Exit return code for the program
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for 
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt. 
Notes:
    - Prints "ERROR: " followed by the formatted error message specified 
      in szFmt. 
    - Prints the file path and file name of the program having the error.
      This is the file that contains this routine.
    - Requires including <stdarg.h>
Returns:
    Returns a program exit return code:  the value of iexitRC.
**************************************************************************/
void ErrExit(int iexitRC, char szFmt[], ... )
{
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n");
    exit(iexitRC);
}
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    In general, this routine optionally prints error messages and diagnostics.
    It also prints usage information.

    If this is an argument error (iArg >= 0), it prints a formatted message 
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits 
    with ERR_COMMAND_LINE.

    If this is a usage error (but not specific to the argument), it prints 
    the specific message and its supplemental diagnostic information.  It 
    also shows the usage and exist with ERR_COMMAND_LINE. 

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript or control:
                                    > 0 - command argument subscript
                                    0 - USAGE_ONLY - show usage only
                                    -1 - USAGE_ERR - show message and usage
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    switch (iArg)
    {
        case USAGE_ERR:
            fprintf(stderr, "Error: %s %s\n"
                , pszMessage
                , pszDiagnosticInfo);
            break;
        case USAGE_ONLY:
            break;
        default:
            fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
                , iArg
                , pszMessage
                , pszDiagnosticInfo);
    }
    // print the usage information for any type of command line error
    fprintf(stderr, "p5 \n");
    if (iArg == USAGE_ONLY)
        exit(USAGE_ONLY); 
    else 
        exit(ERR_COMMAND_LINE);
}

/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
    Examines the input text to return the next token.  It also
    returns the position in the text after that token.  This function
    does not skip over white space, but it assumes the input uses 
    spaces to separate tokens.
Parameters:
    I   char *pszInputTxt       input buffer to be parsed
    O   char szToken[]          Returned token.
    I   int iTokenSize          The size of the token variable.  This is used
                                to prevent overwriting memory.  The size
                                should be the memory size minus 1 (for
                                the zero byte).
Returns:
    Functionally:
        Pointer to the next character following the delimiter after the token.
        NULL - no token found.
    szToken parm - the returned token.  If not found, it will be an
        empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
    - If a token isn't found, szToken is set to an empty string
    - This function does not skip over white space occurring prior to the token.
**************************************************************************/
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize)
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n\r";        // delimiters
    szToken[0] = '\0';

    // check for NULL pointer 
    if (pszInputTxt == NULL)
        ErrExit(ERR_ALGORITHM
        , "getToken passed a NULL pointer");

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // if the delim position is at the first character, return no token.
    if (iDelimPos == 0)
        return NULL;

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize;             // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    pszInputTxt += iDelimPos;
    if (*pszInputTxt == '\0')
        return pszInputTxt;
    else
        return pszInputTxt + 1;
}
