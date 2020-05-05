/***********************************************************************
cs2123p4.c by Carla Ramirez


*************************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
//#define_CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p4.h"

char szQTwo[5] = {'q','T','w','o'};
char szQOne[5] = {'q','O','n','e'};
Event event;
// Main program for the driver
FILE *pACInputFile;    

int main(int argc, char *argv[])
{
    char *pszInput = NULL;     
    Simulation sim;              
    sim = newSimulation();     
    char szInputBuffer[MAX_LINE_SIZE];    
    
  // read text lines containing queries until EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE,stdin) != NULL)
    {
        sscanf(szInputBuffer,"%ld %d %d %d",&event.widget.lWidgetNr,&event.widget.iStep1tu,&event.widget.iStep2tu,&event.widget.iArrivalTime);
        event.iEventType = EVT_ARRIVAL;
        event.iTime = event.widget.iArrivalTime;
        if(event.iTime <= 300)
	    insertOrderedLL(sim->eventList,event);      
    }
    runSimulation(sim,1000);    
}
void arrival(Simulation simulation, Event event)
{
    simulation->iClock = event.iTime;
    insertOrderedLL(simulation->eventList,event);
    simulation->lWidgetCount++;
    printEvents(simulation,event);
}
Simulation newSimulation()
{
    Simulation sim = (Simulation)malloc(sizeof(SimulationImp));
    int iClock = 0;
    sim->eventList = newLinkedList();
    return sim;
}
void queueUp(Simulation simulation,Event event, Queue queue, Server server, int bSwitch)
{
      QElement element;
	 NodeQ *q;
         NodeLL *p;
	 element.widget = event.widget;
	 int bTrue = 0;
	 element.iEnterQTime = simulation->iClock;
	 insertQ(queue,element);
	     queue->lQueueWaitSum += q->element.iEnterQTime;  // or the event.iTime?
		 queue->lQueueWidgetTotalCount++;
         printEvents(simulation,event);
	 q = queue->pHead;
	 q = q->pNext;
	 int iTargetID = q->element.widget.lWidgetNr;
	 p = simulation->eventList->pHead;
	 while(p != NULL && bTrue == 0)
	 {
	     if(p->event.iEventType == EVT_SERVER1_COMPLETE && p->event.widget.lWidgetNr == iTargetID)
             { 
	        event.iTime = p->event.iTime;
	        bTrue = 1;
             } 
	     else
	         p = p->pNext;
	 }
         if(bSwitch == 1)
         {
	    event.iEventType = EVT_SERVER1_COMPLETE;
	    insertOrderedLL(simulation->eventList,event);
            printEvents(simulation,event);
         }
         else
        {   event.iEventType = EVT_SERVER2_COMPLETE;
            insertOrderedLL(simulation->eventList,event);
            printEvents(simulation,event);
         }  
}
void seize(Simulation simulation, Event event, Queue queue, Server server, int bSwitch)
{
     NodeQ *q;
     QElement element;
     event.widget = server->widget;
     if(server->bBusy = FALSE)
         removeQ(queue,&element);
     server->bBusy = TRUE;
     int iLeaveQTime = simulation->iClock;  
     printEvents(simulation,event); 	 
     if(bSwitch == 1)
     {
         event.iEventType = EVT_SERVER1_COMPLETE;   
         event.iTime = simulation->iClock + server->widget.iStep1tu;
         insertOrderedLL(simulation->eventList,event);
         printEvents(simulation,event);
     }
     else
     {
         event.iEventType = EVT_SERVER2_COMPLETE;
         event.iTime = simulation->iClock + server->widget.iStep2tu;
         insertOrderedLL(simulation->eventList,event);
         printEvents(simulation,event);
     }
}
void release(Simulation simulation,Event event, Queue queue, Server server)
{
    server->bBusy = FALSE;

    event.iTime = simulation->iClock;
    event.iEventType = EVT_ENTER_QUEUE2;
    insertOrderedLL(simulation->eventList,event);
    printEvents(simulation,event);
}
void leaveSystem(Simulation simulation,Event event)
{
    int iExitSystem = simulation->iClock;
    simulation->lSystemTimeSum = iExitSystem - event.widget.iArrivalTime;
    printf("%3d\t %3ld\t Exit System\n",event.iTime,event.widget.lWidgetNr);
}
void printLL(LinkedList list)
{
    NodeLL *p;
    for(p = list->pHead; p != NULL; p = p->pNext)
       printf("%ld %d %d %d \n",p->event.widget.lWidgetNr,p->event.widget.iStep1tu,p->event.widget.iStep2tu,p->event.widget.iArrivalTime);

}
void printEvents(Simulation simulation, Event event)
{
    char *szEvent;
    NodeLL *p;
    switch(event.iEventType)
    {
    case EVT_ARRIVAL:
       szEvent = "Arrived";
    case EVT_ENTER_QUEUE1:
       szEvent = "Enter Queue 1";
    case EVT_LEAVE_QUEUE1:
       szEvent = "Leave Queue 1";
    case EVT_SEIZE_SERVER1:
        szEvent = "Seized Server 1";
    case EVT_SERVER1_COMPLETE:
        szEvent = "Relased Server 1";
    case EVT_ENTER_QUEUE2:
        szEvent = "Enter Queue 2";
    case EVT_LEAVE_QUEUE2:
        szEvent = "Leave Queue 2";
    case EVT_SEIZE_SERVER2:
        szEvent = "Seized Server 2";
    case EVT_SERVER2_COMPLETE:
        szEvent = "Relased Server 2"; 
    }
    printf("%3d\t %3ld\t %s\n",event.iTime,event.widget.lWidgetNr,szEvent);
}
void runSimulation(Simulation simulation, int iTimeLimit)
{
   Queue q1, q2;
   Server server1, server2; 
   q1 = newQueue(szQOne);
   q2 = newQueue(szQTwo);
   q1->lQueueWaitSum = 0;
   q1->lQueueWidgetTotalCount = 0;
   q2->lQueueWaitSum = 0;
   q2->lQueueWidgetTotalCount = 0;
   QElement element;
   printf("Time\t Widget\t  Event\n");
   while(removeLL(simulation->eventList,&event))
    {
       simulation->bVerbose = TRUE;
       simulation->iClock = event.iTime;
        switch(event.iEventType)
        {
        case EVT_ARRIVAL:                                
            arrival(simulation,event);
            printEvents(simulation,event);   	
            break;		
        case EVT_ENTER_QUEUE1:
            queueUp(simulation,event,q1,server1,1);
            break;
        case EVT_LEAVE_QUEUE1:
	     removeQ(q1,&element);
             event.iTime = simulation->iClock;
             event.iEventType = EVT_SEIZE_SERVER1;
             insertOrderedLL(simulation->eventList,event); 
            break;
        case EVT_SEIZE_SERVER1:
             seize(simulation,event,q1,server1,1);
            break;
        case EVT_SERVER1_COMPLETE:
             release(simulation,event,q1,server1);
             seize(simulation,event,q1,server1,1);
        case EVT_ENTER_QUEUE2:
             queueUp(simulation,event,q2,server2,2);
            break;
        case EVT_LEAVE_QUEUE2:
             removeQ(q2,&element);
             event.iTime = simulation->iClock;
             event.iEventType = EVT_SEIZE_SERVER2;
             insertOrderedLL(simulation->eventList,event);            
             break;
        case EVT_SEIZE_SERVER2:
            seize(simulation,event,q2,server2,2);
            break;
       case EVT_SERVER2_COMPLETE:
            release(simulation,event,q2,server2);
            seize(simulation,event,q2,server2,2);
            leaveSystem(simulation,event);
        default:
            ErrExit(ERR_ALGORITHM,"Uknown event at time %d\n",event.iEventType);
        }
    }
}
Queue newQueue(char szQueueNm[])
{
    Queue q = (Queue)malloc(sizeof(QueueImp));
    // Mark the list as empty
    q->pHead = NULL;   // empty list
    q->pFoot = NULL;   // empty list
    strcpy(q->szQName, szQueueNm);
    return q;
}
void  insertQ(Queue queue, QElement element)
{
    NodeQ *pNew;
    pNew = allocNodeQ(queue, element);
    // check for empty
    if (queue->pFoot == NULL)
    {
        queue->pFoot = pNew;
        queue->pHead = pNew;
    }
    else
    {   // insert after foot
        queue->pFoot->pNext = pNew;
        queue->pFoot = pNew;
    }
}
NodeQ *allocNodeQ(Queue queue, QElement value)
{
    NodeQ *pNew;
    pNew = (NodeQ *)malloc(sizeof(NodeQ));
    if (pNew == NULL)
        ErrExit(ERR_ALGORITHM, "No available memory for queue");
    pNew->element = value;
    pNew->pNext = NULL;
    return pNew;
}
int removeQ(Queue queue, QElement *pFromQElement)
{
    NodeQ *p;
    // check for empty
    if (queue->pHead == NULL)
        return FALSE;
    p = queue->pHead;
    *pFromQElement = p->element;
    queue->pHead = p->pNext;
    // Removing the node could make the list empty.
    // See if we need to update pFoot, due to empty list
    if (queue->pHead == NULL)
        queue->pFoot = NULL;
    free(p);
    return TRUE;
}
LinkedList newLinkedList ()
{
    LinkedList list = (LinkedList) malloc(sizeof(LinkedListImp));
    // Mark the list as empty
    list->pHead = NULL;   // empty list
    return list;
}
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes)
{
    NodeLL *p;
    // used when the list is empty or we need to insert at the beginning
    *ppPrecedes = NULL;
    // Traverse through the list looking for where the key belongs or
    // the end of the list.
    for (p = list->pHead; p != NULL; p = p->pNext)
    {
        if (match == p->event.iTime)
            return p;
        if (match < p->event.iTime)
            return NULL;
        *ppPrecedes = p;
    }
    // Not found return NULL
    return NULL;
}
NodeLL *insertOrderedLL(LinkedList list, Event value)
{
    
    NodeLL *pNew, *pFind, *pPrecedes;
    // see if it already exists
    pFind = searchLL(list, value.iTime, &pPrecedes);

    // doesn't already exist.  Allocate a node and insert.
    pNew = allocateNodeLL(list, value);

    // Check for inserting at the beginning of the list
    // this will also handle when the list is empty
    if (pPrecedes == NULL)
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
NodeLL *allocateNodeLL(LinkedList list, Event value)
{
    NodeLL *pNew;
    pNew = (NodeLL *)malloc(sizeof(NodeLL));
    if (pNew == NULL)
        ErrExit(ERR_ALGORITHM, "No available memory for linked list");
    pNew->event = value;
    pNew->pNext = NULL;
    return pNew;
}
int removeLL(LinkedList list, Event  *pValue)
{
    NodeLL *p;
    if (list->pHead == NULL)
        return FALSE;
    *pValue = list->pHead->event;
    p = list->pHead;
    list->pHead = list->pHead->pNext;
    free(p);
    return TRUE;
  
}
void freeNode(LinkedList list, NodeLL *pNode)
{
     free(pNode);
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
    fprintf(stderr, "p4 -aA ACInputFileName -aB BInputFileName -aC ACInputFileName\n");
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
