/*
 * Test program of both infile and outfile redirection.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
	char strInputFromCLI[1024];

	printf("Creating file with out...\n");

	if(fgets(strInputFromCLI, 1024, stdin) == NULL)
    {
    	printf("ERROR: invalid input!\n");    
    }
    printf("Output from file:\n%s\n", strInputFromCLI);

	return 0;
}