#include<stdio.h>

main ()
{
	FILE *fpin;
	char buff[255];
	int c, nl, nw, nc;
	fpin = fopen("text.txt","r");
	if (fpin == NULL)
		printf("Error opening file.\n");
	nl = nw = nc = 0;
	while((c = fgetc(fpin)) != EOF)
	{
		fgets(buff,255,fpin);
		printf("%s\n",buff);
		++nc;
		if ( c == '\n')
			++nl;
	}
		printf("newline is = %d new char is = %d\n",nl,nc);
	fclose(fpin);
}