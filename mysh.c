/*
 * Zach Dummer
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"

int main(int argc, char * argv[]) {
  int ret;

    /*
     * Parse Command line arguments to check if this is an interactive or batch
     * mode run.
     */
    if( 0 != (ret = parse_args_main(argc, argv)) ) {
        fprintf(stderr, "Error: Invalid command line!\n");
        return -1;
    }

    /*
     * If in batch mode then process all batch files
     */
    if( TRUE == is_batch) {
        if( TRUE == is_debug ) {
            printf("Batch Mode!\n");
        }

        if( 0 != (ret = batch_mode()) ) {
            fprintf(stderr, "Error: Batch mode returned a failure!\n");
        }
    }
    /*
     * Otherwise proceed in interactive mode
     */
    else if( FALSE == is_batch ) {
        if( TRUE == is_debug ) {
            printf("Interactive Mode!\n");
        }

        if( 0 != (ret = interactive_mode()) ) {
            fprintf(stderr, "Error: Interactive mode returned a failure!\n");
        }
    }
    /*
     * This should never happen, but otherwise unknown mode
     */
    else {
        fprintf(stderr, "Error: Unknown execution mode!\n");
        return -1;
    }


    /*
     * Display counts
     */
    printf("-------------------------------\n");
    printf("Total number of jobs               = %d\n", total_jobs);
    printf("Total number of jobs in history    = %d\n", total_history);
    printf("Total number of jobs in background = %d\n", total_jobs_bg);

    /*
     * Cleanup
     */


    return 0;
}

int parse_args_main(int argc, char **argv)
{

    /*
     * If no command line arguments were passed then this is an interactive
     * mode run.
     */

    /*
     * If command line arguments were supplied then this is batch mode.
     */

    return 0;
}

int batch_mode(void)
{

    /*
     * For each file...
     */

        /*
         * Open the batch file
         * If there was an error then print a message and move on to the next file.
         * Otherwise, 
         *   - Read one line at a time.
         *   - strip off new line
         *   - parse and execute
         * Close the file
         */

    /*
     * Cleanup
     */


    return 0;
}

int handleFGJobs(job_t arrFGJobs[], int intArraySize)
{
	// To Do
	return 0;
}


int handleBGJobs(job_t arrFGJobs[], int intArraySize)
{
	// To Do
	return 0;
}

int createJobs(char *strInputFromCLI)
{
	int intTotalSizeOfCommands = 0;
	int intNumberOfFGJobs = 0;
	int intNumberOfBGJobs = 0;
	char *strFGDesignator = ";";
	char *strBGDesignator = "&";
    int intFGIndex = 0;
    int intBGIndex = 0;

	int index=0;
    char **arrCommands=NULL;
    char *command= malloc(strlen(strInputFromCLI)+1);
    strcpy(command, strInputFromCLI);
    char *tok = strtok(command, " ");
    
    /*
     * Loop through string and break it up into an array
     */
    while(tok!=NULL) {
        intTotalSizeOfCommands++;
        if(strcmp(tok, strFGDesignator) == 0)
        {
        	intNumberOfFGJobs++;
        }
        else if(strcmp(tok, strBGDesignator) == 0)
        {
        	intNumberOfBGJobs++;
        }
        arrCommands = realloc(arrCommands, sizeof(char*)*(index+1));
        char *dup = malloc(strlen(tok)+1);
        strcpy(dup, tok);
        arrCommands[index++] = dup;
        tok = strtok(NULL, " ");
    }
    arrCommands = realloc(arrCommands, sizeof(char*)*(index+1));
    arrCommands[index]=NULL;
    free(command);
    
    if( (strcmp(arrCommands[index - 1], ";") != 0) && (strcmp(arrCommands[index - 1], "&") != 0) )
    {
    	intNumberOfFGJobs++;
    }

    job_t arrFGJobs[intNumberOfFGJobs];
    job_t arrBGJobs[intNumberOfBGJobs];


	int i;
	for(i = 0; i < intTotalSizeOfCommands; i++){
		char *strFullCommand;
		while(arrCommands[i] != NULL && (strcmp(arrCommands[i], ";") != 0) && (strcmp(arrCommands[i], "&") != 0))
		{
			if(arrCommands[i + 1] != NULL && (strcmp(arrCommands[i + 1], ";") != 0) && (strcmp(arrCommands[i + 1], "&") != 0))
			{
				strFullCommand = malloc(strlen(arrCommands[i] + strlen(arrCommands[i + 1]) + 2));
				strFullCommand[0] = '\0';
				strcat(strFullCommand, arrCommands[i]);
				strcat(strFullCommand, " ");
				strcat(strFullCommand, arrCommands[i + 1]);
				i += 2;
			}
			else
			{
				strFullCommand = malloc(strlen(arrCommands[i] + 1));
				strFullCommand[0] = '\0';
				strcat(strFullCommand, arrCommands[i]);
				i++;
			}
		}

		if(arrCommands[i] != NULL && strcmp(arrCommands[i], "&") == 0)
		{
			arrBGJobs[intBGIndex].full_command = strFullCommand;
			arrBGJobs[intBGIndex].is_background = TRUE;
			arrBGJobs[intBGIndex].binary = NULL;
			intBGIndex++;
		}
		else
		{
			arrFGJobs[intFGIndex].full_command = strFullCommand;
			arrBGJobs[intFGIndex].is_background = FALSE;
			arrBGJobs[intBGIndex].binary = NULL;
			intFGIndex++;
		}
	}

	handleBGJobs(arrBGJobs, intNumberOfBGJobs);
	handleFGJobs(arrFGJobs, intNumberOfFGJobs);

	return 0;
}

int interactive_mode(void)
{

	bool blnIsRunning = true;

    do {
    	
    	char strInputFromCLI[MAX_COMMAND_LINE];

        /*
         * Print the prompt
         */
        printf("%s", PROMPT);
        /*
         * Read stdin, break out of loop if Ctrl-D
         */
        if(fgets(strInputFromCLI, MAX_COMMAND_LINE, stdin) == NULL)
        {
        	printf("\n"); /* print a newline so next command is on a newline */
        	exit(0);
        }
        else
        {
        	/* Strip off the newline */
        	strtok(strInputFromCLI, "\n");
        	
        	/*
         	 * Parse and execute the command
         	 */
        	createJobs(strInputFromCLI);
        }

    } while(blnIsRunning);

    /*
     * Cleanup
     */

    return 0;
}

/*
 * You will want one or more helper functions for parsing the commands 
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job)
{

    /*
     * Display the job
     */


    /*
     * Launch the job in either the foreground or background
     */

    /*
     * Some accounting
     */

    return 0;
}

int builtin_exit(void)
{

    return 0;
}

int builtin_jobs(void)
{

    return 0;
}

int builtin_history(void)
{

    return 0;
}

int builtin_wait(void)
{

    return 0;
}

int builtin_fg(void)
{

    return 0;
}

int builtin_fg_num(int job_num)
{

    return 0;
}
