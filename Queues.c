#include<stdio.h>
#include<stdlib.h>
//array implementation for queues

#define MAX_SIZE 101

struct Queue {

	int A[MAX_SIZE];
	int front, rear;
} 
int front = -1;
int rear = -1;
typedef enum {false,true} bool;

q.front = -1; 
q.rear = -1; // queue is empty at this point
//checks whether queue is empty or not
bool IsEmpty()
{
	if (front == -1 && rear == -1)
		return 0;
	else	
		return 1;

}
//checks if queue is full or not
bool IsFull()
{
	if ((rear+1)%MAX_SIZE == front)
		return 1; // return true
}

// Inserts an element in queue at rear end

void Enqueue (int x)
{
	printf("Enqueuing %d \n",x);
	if (IsFull())
	{
		printf("Error: Queue is full\n");
		return;
	}
	if (IsEmpty())
	{
		front = rear = 0;
	}
	else
	{
		rear = (rear+1)%MAX_SIZE;
	}
	 A[rear] = x;

}
// Removes an element in Queue from front end. 
void Dequeue()
{
	printf("Dequeuing \n");
	if (IsEmpty())
	{
		printf("Error: Queue is Empty\n");
		return;
	}
	else if (front == rear)
	{
		rear = front  = -1;
	}
	else
	{
		front = (front+1)%MAX_SIZE;
	}
}
//returns element at the front of queue
int Front()
{
	if(front == -1)
	{
		printf("Error: cannot return value from empty queue\n");
		return -1;
	}
	return q.A[front];
}
/* 
  Printing the elements in queue from front to rear. 
  This function is only to test the code. 
  This is not a standard function for Queue implementation. 
	*/
void Print ()
{
	//finding number of elements in queue
	int i;
	int count = (rear+MAX_SIZE-front)%MAX_SIZE + 1;
	printf("Queue          : ");
	for(i=0; i < count; i++)
	{
		int index = (front+i)%MAX_SIZE; //index of element while traversing circularly from front
		printf("A[%d] ",q.A[index]);
	}
	printf("\n\n");
};
int main()
{
	//driver code to test the implementation
	//printing the elements in queue after each enqueue or dequeue
	Enqueue(2);
	Print();
	Enqueue(3);
	Print();

}

