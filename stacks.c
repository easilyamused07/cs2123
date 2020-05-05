#include<stdio.h>

#define MAXSIZE 100

int A[MAXSIZE];
int top = -1;
void push(int x)
{
	if(top == MAXSIZE - 1) {
		printf("Stack overflow\n");
		return;
	}
		A[++top] = x;
	
}
void pop()
{
	if(top == -1){
		printf("Stack is empty\n");
		return;
		}
	--top;
}

char Top()
{
	return A[top];
}
void print()

{
    int i;
    printf("Stack: ");
    for(i=0; i<=top;i++)
        printf("%d",A[i]);
    printf("\n");
}

char main ()
{
push(1);print();
push(2);print();
push(3);print();
pop();print();
push(4);print();


}

