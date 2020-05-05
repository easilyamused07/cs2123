#include <stdio.h>
// program prints input one word per line
 int main () 
 {    
     int c;
     while((c = getchar()) != EOF)
     {
         if(c == ' ' || c == '\t' || c == '\n')
             putchar('\n');
         // flush the output!!!
         else
         // store the output!!!
             putchar(c);
       
     }
	return 0;
 }