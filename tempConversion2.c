#include<stdio.h>
// prints tempature conversion from Celsius to Fahrenheit from 0 to 300 degrees
main ()
{
	float fahr, celsius;
	int lower, upper, step;
	
	lower = 0;
	upper = 300;
	step = 20;
	printf("\nCelsius to Fahrenheit Conversion Chart:\n\n");
	printf(" Celsius      Fahrenheit\n");
	celsius = lower;
	while(celsius <= upper)
	{
		fahr = celsius * 9.0/5.0 + 32.0;
		printf("%3.0f\t%13.1f\n",celsius,fahr);
		celsius = celsius + step;

	}

}