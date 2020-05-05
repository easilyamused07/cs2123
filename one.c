#include<stdio.h>

int main()
{
// ask user to input two digits to calculate sum
	int num1, num2;
	printf("\n Enter a number: \n");
	scanf("%d",&num1);
	if (num1 != 1)
		printf("Input a different number\n. ");
	printf("Enter a second number to get sum: \n");
	scanf("%d",&num2);
	int result = sum(num1,num2);
	printf("\n the sum equals %d\n",result);
return 0;
}
int sum(int a, int b)
{

	int sum = a + b;
	
	return sum;

}
