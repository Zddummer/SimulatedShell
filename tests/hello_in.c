/*
 * Test program of infile redirection.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
	char strInputFromCLI[1024];

	printf("Enter your name: ");

	if(fgets(strInputFromCLI, 1024, stdin) == NULL)
    {
    	printf("ERROR: invalid input!\n");    
    }
    printf("Hello %s\n", strInputFromCLI);

	return 0;
}