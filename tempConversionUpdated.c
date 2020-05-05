#include<stdio.h>

#define HDEGREE 300 // upper limit for degree
#define LDEGREE 0 //lower limit for degree
#define STEP 20 //step size

// print fahrenheit-celsius table: for fahr = 300 to 0 using for loop and constants

main()
{
	int fahr;
	int celsius;
	printf("\nFahrenheit to Celsius Conversion Chart:\n\n");
	printf(" Fahrenheit      Celsius\n");
	for(fahr = HDEGREE; fahr >=LDEGREE; fahr = fahr - STEP)
	{
		printf("%3.0f\t%13.1f\n",fahr,(5.0/9.0) * (fahr-32));
	}

}