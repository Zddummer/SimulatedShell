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

        if( 0 != (ret = batch_mode(argc, argv)) ) {
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
    if(argc == 1)
    {
        is_batch = FALSE;
    }
    /*
     * If command line arguments were supplied then this is batch mode.
     */
    else
    {
        is_batch = TRUE;
    }

    return 0;
}

int batch_mode(int argc, char * argv[])
{

    /*
     * For each file...
     */
    int i;
    for(i = 1; i < argc; i++)
    {
        /*
         * Open the batch file
         * If there was an error then print a message and move on to the next file.
         * Otherwise, 
         *   - Read one line at a time.
         *   - strip off new line
         *   - parse and execute
         * Close the file
         */
        FILE *fCurrentFile;
        fCurrentFile = fopen(argv[i], "r");
        char strLine[30];
        if( fCurrentFile == NULL)
        {
            printf("ERROR: The file \"%s\" could not be found!\n", argv[i]);
        }
        else
        {
            while(fscanf(fCurrentFile, "%[^\n]\n", strLine) != EOF)
            {
                createJobs(strLine);
                total_jobs++;
                total_history++;
                
                int j;
                int length = strlen(strLine);

                for (j = 0; j < length; j++) {
                    if (strLine[j] == '&') {
                        total_jobs_bg++;
                    }
                }
            }
        }
    }

    /*
     * Cleanup
     */


    return 0;
}

int handlels(char **args, int argc, int is_bg)
{   
    int i;
    for (i = argc; i > 0; i--)
    {
        if(i < argc)
        {
            printf("\n");
        }
        printf("%s:\n", args[i]);

        pid_t c_pid = 0;
        int status = 0;
        char **temp = NULL;

        temp = (char **)malloc(sizeof(char *) * 3);
        temp[0] = strdup("/bin/ls");
        temp[1] = strdup(args[i]);
        temp[2] = NULL;


        c_pid = fork();

        if(c_pid < 0)
        {
            fprintf(stderr, "ERROR: fork failed!\n");
            return -1;
        }
        else if(c_pid == 0)
        {
            execvp(temp[0], temp);

            fprintf(stderr, "ERROR: Exec failed!\n");
            exit(-1);
        }
        else
        {
            if(is_bg == FALSE)
            {
                waitpid(c_pid, &status, 0);
            }
        }
        
        /* saftey */
        if(i > MAX_COMMAND_LINE){
            exit(-1);
        }
    }
    return 0;
}

int handleJobs(job_t *arrJobs[], int intArraySize)
{
    int i;
    for(i = 0; i < intArraySize; i++)
    {
        pid_t c_pid = 0;
        int status = 0;
        char *binary = NULL;
        char **args = NULL;
        int j;

        binary = strdup(arrJobs[i]->binary);
        args = (char **)malloc(sizeof(char *) + sizeof(arrJobs[i]->argv));

        args[0] = strdup(binary);
        for(j = 0; j < arrJobs[i]->argc; j++)
        {
            args[j + 1] = strdup(arrJobs[i]->argv[j]);
        }
        args[arrJobs[i]->argc + 2] = NULL;

        if((strcmp(binary, "ls") == 0 || strcmp(binary, "/bin/ls") == 0) && arrJobs[i]->argc > 1)
        {
            handlels(args, arrJobs[i]->argc, arrJobs[i]->is_background);
        }
        else if(strcmp(binary, "jobs") == 0)
        {
            builtin_jobs();
        }
        else if(strcmp(binary, "history") == 0)
        {
            builtin_history();
        }
        else if(strcmp(binary, "wait") == 0)
        {
            builtin_wait();
        }
        else if(strcmp(binary, "fg") == 0)
        {
            builtin_fg();
        }
        else if(strcmp(binary, "exit") == 0)
        {
            builtin_exit();
        }
        else
        {
            c_pid = fork();

            if(c_pid < 0)
            {
                fprintf(stderr, "ERROR: fork failed!\n");
                return -1;
            }
            else if(c_pid == 0)
            {
                execvp(binary, args);

                fprintf(stderr, "ERROR: Exec failed!\n");
                exit(-1);
            }
            else
            {
                if(arrJobs[i]->is_background == FALSE)
                {
                    waitpid(c_pid, &status, 0);
                }
            }
        }

        /* saftey */
        if(i > MAX_COMMAND_LINE){
            exit(-1);
        }
    }
    return 0;
}

int createJobs(char *strInputFromCLI)
{
	int intTotalSizeOfCommands = 0;
	int intNumberOfJobs = 0;

	int index=0;
    char **arrCommands=NULL;
    char *command= malloc(strlen(strInputFromCLI)+1);
    strcpy(command, strInputFromCLI);
    char *tok = strtok(command, " ");

    int intJobIndex = 0;
    
    /*
     * Loop through string and break it up into an array
     */
    while(tok!=NULL) {
        intTotalSizeOfCommands++;
        if(strcmp(tok, ";") == 0)
        {
        	intNumberOfJobs++;
        }
        else if(strcmp(tok, "&") == 0)
        {
        	intNumberOfJobs++;
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
    	intNumberOfJobs++;
    }

    job_t *arrJobs[intNumberOfJobs];

    int i;
	for(i = 0; i < intTotalSizeOfCommands; i++)
    {
		
        char *strFullCommand;
        job_t *CurrentJob = CurrentJob = malloc(sizeof(job_t));
        CurrentJob->argc = 0;
        CurrentJob->argv = NULL;
        CurrentJob->is_background = FALSE;
        CurrentJob->binary = arrCommands[i];

        i++;
        int j = 0;
		while(arrCommands[i] != NULL && (strcmp(arrCommands[i], ";") != 0))
		{

            if(strcmp(arrCommands[i], "&") == 0)
            {
                CurrentJob->is_background = TRUE;
                break;
            }

            CurrentJob->argv = realloc(CurrentJob->argv, sizeof(char*)*(j + 1));
            char *dup = malloc(strlen(arrCommands[i]) + 1);
            strcpy(dup, arrCommands[i]);
            CurrentJob->argv[j] = dup;
            CurrentJob->argc++;

            i++;
            j++;
		}

        strFullCommand = malloc(strlen(CurrentJob->binary) + sizeof(CurrentJob->argv) + CurrentJob->argc);
        strFullCommand[0] = '\0';
        strcat(strFullCommand, CurrentJob->binary);

        int numArgs;
        for(numArgs = 0; numArgs < CurrentJob->argc; numArgs++)
        {
            strcat(strFullCommand, " ");
            strcat(strFullCommand, CurrentJob->argv[numArgs]);
        }

        CurrentJob->full_command = strFullCommand;
        arrJobs[intJobIndex] = CurrentJob;
        intJobIndex++;
	}

	handleJobs(arrJobs, intNumberOfJobs);

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
