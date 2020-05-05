#include<stdio.h>

int main (void)
//verify the expression getchar() != EOF is 0 or 1
{	
	if ((getchar() != EOF ) == 0)
		printf("EOF equals 0.\n");
		else if((getchar() != EOF)== 1)
		printf("EOF equals 1.\n");
		else
		printf("EOF does not equal 1 or 0");
		printf("Value of EOF is %d",EOF);
	return 0;
}