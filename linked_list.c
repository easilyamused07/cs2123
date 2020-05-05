#include<stdio.h>

struct Node{
int data;
struct Node* link;

};

struct Node* head;

void Insert(int x)
{
 Node* temp = (Node*)malloc(sizeof(Node));
temp->data = x;
temp->link = NULL;
head = temp;
}
void Print()
{
int i;
struct Node* temp = head;
printf("List: ");
while(temp != NULL)
{
	printf(" %d",temp->data);
	temp = temp->link;
}
printf("\n");
}

int main()
{
head = NULL;
int i;

for(i=0;i<10;i++)
{
	Insert(i);
}
Print();

}