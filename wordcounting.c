#include<stdio.h>

#define IN 1 // inside a word
#define OUT 0 // outside a word

// count lines, words, and characters in input
main ()
{
// read in files
	FILE *fpin;
	char buff[255];
	int c, nl, nw, nc, state;
	fpin = fopen("text.txt","r");
	if (fpin == NULL)
		printf("Error opening file.\n");

	state = OUT;
	nl = nw = nc = 0;
	while((c = fgetc(fpin)) != EOF)
	{
		++nc;
		if( c == '\n')
			++nl;
		if( c == ' ' || c == '\n' || c == '\t')
			state = OUT;
		else if (state == OUT){
			state = IN;
			++nw;
		}
	}
	printf("# new lines = %d\n# new words = %d\n# new chars = %d\n",nl,nw,nc);
	
}
