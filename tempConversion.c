#include<stdio.h>

// print fahrenheit-celsius table: for fahr = 0 to 300

main()
{
	float fahr, celsius;
	int lower, upper, step;
	
	lower = 0;
	upper = 300;
	step = 20;
	printf("\nFahrenheit to Celsius Conversion Chart:\n\n");
	printf(" Fahrenheit      Celsius\n");
	fahr = lower;
	while(fahr <= upper)
	{
		celsius = (5.0/9.0) * (fahr-32.0);
		printf("%3.0f\t%13.1f\n",fahr,celsius);
		fahr = fahr + step;

	}

}