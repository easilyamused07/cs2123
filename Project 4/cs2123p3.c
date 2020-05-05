/******cs2123p3.c by Carla Ramirez*********************************************
Purpose:
   Create a linked list for managing timed events. Based on different clocktimes
   the program will simulate arrivals and departures. Each person has an arrival and
   depature based on stdin values.
Command Parameters:
  n/a ( p3Input.txt)
Input:
   Person File:
    Input file stream which contains records:
    People records are terminated by EOF.
    Person szName iDeltaDepart iDeltaNextArrvial
             11s      3d             3d
Results:
    Print the persons arrival and depature time in order.
      -print the name
      -print the time they arrive
      -print the time they depart
      -print wheather that time is an arrival for departure
Returns:
    0 - normal
    903 - error in algorithm
Notes:
    1. This program uses linked list with pointers
    2. This program uses stdin as input data
****************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p3.h"

Event event;  // global event variable to be used by all func
// Main program
int main(int argc, char *argv[])
{
    char szInputBuffer[100]; // buffer for input not to exceed 100 char
    Simulation sim; // list for simulation func
    int iTimeValue = 1000; // max time for loop in simulation
    int iDeltaNextArrival = 0; //temp value to store stdin values
    int iTime = 0; // temp value to store time

    sim = newSimulation();
    
    while(fgets(szInputBuffer, 100, stdin) != NULL)
    {
        sscanf(szInputBuffer," %s %d %d",event.person.szName,&event.person.iDepartUnits,&iDeltaNextArrival);        
        event.iTime = iTime; // set the time the person arrives
      
        event.person.iDepartUnits += event.iTime;  // set the time the person leaves (duration)
        event.iEventType = EVT_ARRIVE; //set eventtype to arrivals
        insertOrderedLL(sim->eventList,event);     
        iTime += iDeltaNextArrival; //advance time based on new arrival         
    }      			  
    runSimulation(sim, iTimeValue);     // func to simulate arrivals and  departures based on clocktime
}
/*******************************************newSimulation*******
Simulation newSimulation()
Purpose:
    Allocate memory for a simulation implementation and initializes the clock time to zero.
Parameters:
    n/a
Notes:
    This is used to store the eventlist. 
    Since the memory manager is managing free memory, we dont need to initlize the free list.
Return:
    n/a
*******************************************************************************/
Simulation newSimulation()
{
    Simulation sim = (Simulation)malloc(sizeof(SimulationImp));   
    int iClock = 0;
    sim->eventList = newLinkedList();
    return sim;
}
/**************************************************printLL********
void printLL(LinkedList list)
Purpose:
    Print the nodes inside of a linked list, calls printEvent to print persons
names, arrive time and depart time.
Parameters:
    I LinkedList list 
linked list which is a pointer to a linked list implementation
Notes:
    Prints the list from start to end.
Return Value:
    n/a
********************************************************************************/
void printLL(LinkedList list)
{
   NodeLL *p;
  for(p = list->pHead; p !=NULL; p = p->pNext)
	printEvent(p->event);
}
/****************************************************newLinkedList*******
LinkedList newLinkedList()
Purpose:
   Allocate memory for list size of linkedlist implementaion which contains
event and pointer to next node list.
Parameters:
    n/a
Notes:
    This is used by newSimulation to initalize the simulation->eventList.
    Since the memory manager is managing free memory, we dont have to 
initalize the free list.
Return Value:
    n/a
********************************************************************/
LinkedList newLinkedList()
{
    LinkedList list  = (LinkedList) malloc(sizeof(LinkedListImp));
    list->pHead = NULL; // empty list
    return list;
}
/*****************************************************freeNode*****
void freeNode(LinkedList list, NodeLL *pNode)
Purpose:
    Free the allocated memory previously allocated in functions newLinkedList
and newSimulation.

Parameters:
    I    LinkedList list
linked list which is a pointer to a linked list implementation
    I    NodeLL *pNode
contains the event and next node reference
Notes:

Return Value:
    n/a
******************************************************************/
void freeNode(LinkedList list, NodeLL *pNode)
{
    free(pNode);	
}
/*********************************************removeLL******
int removeLL(LinkedList list, Event *pValue)
Purpose:
    Remove the first node from the list and copy the event data from the
node. Free the memory of the preivously allocated memory.

Parameters:
    I    LinkedList list 
linked list which is a pointer to a linked list implementation
    I  Event *pValue
event structure data for reach person

Notes:
    This function calls freeNode to free the memory.

Return Value:
    0  True if succesfuly removed node
    1  False if list was empty and nothing was removed
****************************************************************/
int removeLL(LinkedList list, Event *pValue)
{
     NodeLL *pRemove;
     if(list->pHead == NULL)
	 return FALSE;
    *pValue = list->pHead->event;
     pRemove = list->pHead;
     list->pHead = list->pHead->pNext;
     freeNode(list,pRemove);
     return TRUE;
}
/*********************************************allocateNodeLL*****
NodeLL allocateNodeLL(LinkedList list, Event value)
Purpose:
    Create a new node for the list and set its event equal to value passed in.

Parameters:
    I   LinkedList list
a linked list which is a pointer to a linked list implementation
    I   Event value
event structure for each person

Notes:
    Return an ErrExit if there was a problem with allocating memory
Return Value:
    pNew   newly allocated node
*****************************************************************/
NodeLL *allocateNodeLL(LinkedList list, Event value)
{
    NodeLL *pNew;
    pNew = (NodeLL *)malloc(sizeof(NodeLL));
    if(pNew == NULL)
	ErrExit(ERR_ALGORITHM, "No avaliable memory for linked list");
    pNew->event = value;
    pNew->pNext = NULL;
    return pNew;
}
/*********************************************insertOrderedLL***
NodeLL insertOrderedLL(LinkedList, Event value)
Purpose:
    Inserts an element into an ordered linked list. This function allows
for duplicates, and returns the new node.

Parameters:
    I/O    LinkedList list
a linked list which is a pointer to a linked list implementation
    I/O    Event value
event structure for each person

Notes:
   This function was modified to allow duplicate as previously would
not allow duplicate to be inserted.

Return Value:
  pNew    new node    
**************************************************************/
NodeLL *insertOrderedLL(LinkedList list, Event value)
{
    NodeLL *pNew, *pFind,*pPrecedes;
    
    pFind = searchLL(list,value.iTime,&pPrecedes);
    // allocate a new node and insert	
    pNew = allocateNodeLL(list, value);

    // check for inserting at the beginning
    // this will also handle when the list is empty
    if(pPrecedes == NULL)
    {
         pNew->pNext = list->pHead;
         list->pHead = pNew;
    }
    else
    {
         pNew->pNext = pPrecedes->pNext;
         pPrecedes->pNext = pNew;
    }
     return pNew;
}
/**********************************************searchLL****
NodeLL searchLL(LinkedList list, int match, NodeLL **ppPrecedes)
Purpose:
    Searches for match in the list, if found, it functionally returns the
address of the node containing it. if not found, it returns NULL, it also will 
return a pointer to the node that predes where match exists(or should exist if not found).

Parameters:
    I    LinkedList list
a linked list which is a pointer to a linked list implementation
    I    int match
the value to be found
    I/O  NodeLL **ppPrecedes
pointer to the node that precedes where the match exsist
Notes:
    Searches to find the time value for each person which helps with the ordered list
Return Value:
    p    address of node that has the value searched
**********************************************************************/
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes)
{
    NodeLL *p;
    *ppPrecedes = NULL;
    for(p = list-> pHead; p!=NULL; p = p->pNext)
     {
	if(match == p->event.iTime)
	    return p;
	if(match < p->event.iTime)
            return NULL;
	*ppPrecedes = p;
     }
     return NULL;
}
/******************************************printEvent********
void printEvent(Event event)
Purpose:
    Prints the name of person, event time and wheather it is an arrival
or depature based on eventtype.
Parameters:
    I    Event event
event stucture that contains the event data for each person
Notes:
    
Return Value:
    n/a
****************************************************************/
void printEvent(Event event)
{
    char *szType;
    if(event.iEventType == EVT_ARRIVE)
       szType = "Arrive";
    else
       szType = "Depart";
    printf("%3d   %-11s   %8s\n",event.iTime,event.person.szName,szType);
}
/*************************************************generateDepature****
void generateDepature(Simulation simulation, Event event)
Purpose:
    Sets the depatures for each person and changes the event time to the
persons depature time.
Parameters:
    I    Simulation simulation
a linked list which is a pointer to a simulation implementation. Store event list data.
    Event event
an event structure that store event information for each perosn
Notes:

Return Value:
    n/a
***********************************************************************/
void generateDepature(Simulation simulation,Event event)
{
    event.iEventType = EVT_DEPART;
    event.iTime = event.person.iDepartUnits;
    insertOrderedLL(simulation->eventList,event);
}
/**************************************runSimulation****
void runSimulation(Simulation simulation, int iTimeValue)
Purpose:
    Loop through timed events and set clock ot the time fo the current
timed event. based on the event, it prints the time,person, and event.
Parameters:
    I   Simulation simulation
a linked list which is a pointer to a simulation implementation
    I    int iTimeValue
constant value to prevent infifinte loops
Notes:
    This function terminates once there are no more events in list. Also terminates if the clocks
time is greater than or equal to the second parameter
Return Value:
    n/a
**********************************************************/
void runSimulation(Simulation simulation, int iTimeValue)
{
  printf("Time  Person          Event\n");
  while(removeLL(simulation->eventList, &event ))
    {
    if(event.iTime > iTimeValue)
         break;
    simulation->iClock = event.iTime; //advance clock
     switch(event.iEventType)
     {
         case EVT_ARRIVE:
            printEvent(event);
            generateDepature(simulation,event);
            break;
         case EVT_DEPART:
	    printEvent(event);
	    break;
     }	    		
   }
}
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
    printf("Error: ");
    vprintf(szFmt, args);

    va_end(args);                                // begins.  They begin after szFmt.
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
    fprintf(stderr, "p3 -c InputFileName\n");
    if (iArg == USAGE_ONLY)
        exit(USAGE_ONLY); 
    else 
        exit(ERR_COMMAND_LINE);
}

/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSizePurpose:
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
}
