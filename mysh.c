/*
 * Zach Dummer
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"

void parseCommandLine(int argc, char * argv[])
{
	/*
     * If no command line arguments were passed then this is an interactive
     * mode run.
     */
    if(argc == 1)
    {
        blnIsBatch = false;
    }
    /*
     * If command line arguments were supplied then this is batch mode.
     */
    else
    {
        blnIsBatch = true;
    }
}

void handleOutRedirect()
{
    int intErrorCheck;

    intFileDescriptor = open(strOutputFileName, O_RDWR | O_CREAT | O_TRUNC, 0644);

    if(intFileDescriptor < 0)
    {
        fprintf(stderr, "ERROR: could not output to file!\n");
        builtin_exit();
    }

    intErrorCheck = dup2(intFileDescriptor, STDOUT_FILENO);

    if(intErrorCheck < 0)
    {
        fprintf(stderr, "ERROR: on dup2() system call!\n");
        builtin_exit();
    }
}

void handleInRedirect()
{
	int intErrorCheck;

    intFileDescriptor = open(strInputFileName, O_RDONLY | O_EXCL);

    if(intFileDescriptor < 0)
    {
        fprintf(stderr, "ERROR: could not find file!\n");
        builtin_exit();
    }

    intErrorCheck = dup2(intFileDescriptor, STDIN_FILENO);

    if(intErrorCheck < 0)
    {
        fprintf(stderr, "ERROR: on dup2() system call!\n");
        builtin_exit();
    }
}

void createJobs(char *strInputFromCLI)
{
	int index = 0;
	int intNumJobs = 0;

    char **arrCommands = NULL;
    char *command = malloc(strlen(strInputFromCLI) + 1);
    strcpy(command, strInputFromCLI);
    char *tok = strtok(command, " ");

    /*
     * Loop through string and break it up into an array
     */
    while(tok != NULL)
    {
        if(strcmp(tok, "<") == 0)
        {
            strInputFileName = strdup(strtok(NULL, " "));
            blnWasInputRedirected = true;
            break;
        }
        else if(strcmp(tok, ">") == 0)
        {
            strOutputFileName = strdup(strtok(NULL, " "));
            blnWasOutputRedirected = true;
            break;
        }
        else if(strcmp(tok, ";") == 0)
        {
        	intNumJobs++;
        }
        else if(strcmp(tok, "&") == 0)
        {
        	intNumJobs++;
        }

        arrCommands = realloc(arrCommands, sizeof(char*) * (index + 1));
        char *dup = malloc(strlen(tok) + 1);
        dup = strdup(tok); // changed to use strdup
        arrCommands[index++] = dup;
        tok = strtok(NULL, " ");

    }

    arrCommands = realloc(arrCommands, sizeof(char*) * (index + 1));
    arrCommands[index] = NULL;
    free(command);

    int i;
    for(i = 0; i < index; i++)
    {
        struct job_t *CurrentJob = malloc(sizeof(job_t));
        CurrentJob->intArgCount = 0;
        CurrentJob->arrArgArray = NULL;
        CurrentJob->isBackground = false;
        char *strTemp = strdup(arrCommands[i]);
        CurrentJob->binary = strdup(strTemp);

        i++;
        int intTempArrIndex = 0;
        char **arrTempArgs = NULL;
        while(arrCommands[i] != NULL && (strcmp(arrCommands[i], ";") != 0))
        {

            if(strcmp(arrCommands[i], "&") == 0)
            {
                CurrentJob->isBackground = true;
                break;
            }
            arrTempArgs = realloc(arrTempArgs, sizeof(char*) * (intTempArrIndex + 1));
            char *strTemp = malloc(strlen(arrCommands[i]) + 1);
            strTemp = strdup(arrCommands[i]); // Changed to use strdup
            arrTempArgs[intTempArrIndex++] = strTemp;

            i++;
        }
        arrTempArgs = realloc(arrTempArgs, sizeof(char*) * (intTempArrIndex + 1));
        arrTempArgs[intTempArrIndex] = NULL;

        CurrentJob->intArgCount = intTempArrIndex;
        CurrentJob->arrArgArray = (char **)malloc(sizeof(char *) * (intTempArrIndex + 1));
        CurrentJob->strFullCommand = (char *)malloc(sizeof(char));
        CurrentJob->strFullCommand[0] = '\0';
        strcat(CurrentJob->strFullCommand, CurrentJob->binary);
        int j;
        for(j = 0; j < intTempArrIndex; j++)
        {
            char *strTempArg = strdup(arrTempArgs[j]);
            CurrentJob->arrArgArray[j] = strdup(arrTempArgs[j]);

            strcat(CurrentJob->strFullCommand, " ");
            strcat(CurrentJob->strFullCommand, strTempArg);
        }
        addJobToHistory(CurrentJob);
        executeCommand(CurrentJob);
        
    }

    
}

void runInBatchMode(int argc, char * argv[])
{
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
        char strLine[MAX_COMMAND_LINE];
        if( fCurrentFile == NULL)
        {
            printf("ERROR: The file \"%s\" could not be found!\n", argv[i]);
        }
        else
        {
            while(fscanf(fCurrentFile, "%[^\n]\n", strLine) != EOF)
            {
                createJobs(strLine);
            }
        }
    }
}

void runInInteractiveMode()
{
	bool blnKeepRunning = true;

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
        	builtin_exit();
        }
        else
        {
        	/* Strip off the newline */
        	strtok(strInputFromCLI, "\n");

        	if(strcmp(strInputFromCLI, "\n") == 0)
        	{
        		if(isatty(STDIN_FILENO) == 0)
        		{
        			printf("\n");
        		}
        		continue;
        	}

        	if(isatty(STDIN_FILENO) == 0)
        	{
        		printf("\n");
        	}
        	/*
         	 * Parse and execute the command
         	 */
        	createJobs(strInputFromCLI);
        }

    } while(blnKeepRunning);
}

void addJobToHistory(job_t *CurrentJob)
{
    char *command = malloc(strlen(CurrentJob->strFullCommand) + 1);
    command = strdup(CurrentJob->strFullCommand); // changed to strdup

    arrJobHistory = realloc(arrJobHistory, sizeof(char*) * (intJobHistorySize + 1));
    char *dup = malloc(strlen(command) + 1);
    dup = strdup(command); // changed to strdup

    if(CurrentJob->isBackground)
    {
        strcat(dup, " &");
    }

    arrJobHistory[intJobHistorySize++] = dup;

    intTotalHistory++;
}

void addJobToBG(bgJob *jobToAdd)
{    
    arrBGJobs = realloc(arrBGJobs, sizeof(bgJob*) * (intBGJobSize + 1));
    arrBGJobs[intBGJobSize++] = jobToAdd;
}

bool executeCommand(job_t *CurrentJob)
{
	pid_t c_pid = 0;
	int status = 0;
	char *binary = NULL;
	char **args = NULL;

    intTotalJobs++;

	binary = strdup(CurrentJob->binary);

    if(strcmp(binary, "jobs") == 0)
    {
        intTotalJobs--;
        builtin_jobs();
        return true;
    }
    else if(strcmp(binary, "history") == 0)
    {
        intTotalJobs--;
        builtin_history();
        return true;
    }
    else if(strcmp(binary, "wait") == 0)
    {
        builtin_wait();
        return true;
    }
    else if(strcmp(binary, "fg") == 0)
    {
        if(CurrentJob->intArgCount == 0)
        {
            builtin_fg();
            return true;
        }
        else
        {
            builtin_fg_num(atoi(CurrentJob->arrArgArray[1]));
            return true;
        }
    }
    else if(strcmp(binary, "exit") == 0)
    {
        intTotalJobs --;
        builtin_exit();
    }

	args = (char **)malloc(sizeof(char *) * (CurrentJob->intArgCount + 1));
	
    args[0] = strdup(binary);
	
    int i;
    for(i = 1; i <= CurrentJob->intArgCount; i++)
    {
        char *temp = strdup(CurrentJob->arrArgArray[i - 1]);
        args[i] = strdup(temp);
    }

	args[i] = NULL;

	c_pid = fork();

	if(c_pid < 0)
	{
		fprintf(stderr, "ERROR: Fork failed!\n");
		return false;
	}
	else if(c_pid == 0)
	{
		if(blnWasOutputRedirected)
		{
			handleOutRedirect();
		}
		if(blnWasInputRedirected)
		{
			handleInRedirect();
		}

		execvp(binary, args);

		fprintf(stderr, "ERROR: Exec failed!\n");
		exit(-1);
	}
	else
	{
		if(CurrentJob->isBackground == false)
        {
            waitpid(c_pid, &status, 0);
            if(blnWasOutputRedirected){
            	blnWasOutputRedirected = false;
            }
        }
        else
        {
        	waitpid(0, NULL, WNOHANG);
            intTotalJobsInBackground++;

            bgJob *newJob = malloc(sizeof(bgJob));
            newJob->pid = c_pid;
            newJob->blnIsRunning = true;
            newJob->blnWasDisplayed = false;
            newJob->strFullCommand = strdup(CurrentJob->strFullCommand);

            addJobToBG(newJob);
        }
	}
	return true;
}

void builtin_exit(void)
{
	int intNumJobsStillRunning = 0;

    int i;
    for(i = 0; i < intBGJobSize; i++)
    {
        if(waitpid(arrBGJobs[i]->pid, NULL, WNOHANG) == 0)
        { 
            intNumJobsStillRunning++;
        }
    }
    
    if(intNumJobsStillRunning > 0)
    {
    	printf("Still waiting on %d jobs to finish.\n", intNumJobsStillRunning);
    }

    builtin_wait();

    /*
     * Display counts
     */
    printf("-------------------------------\n");
    printf("Total number of jobs               = %d\n", intTotalJobs);
    printf("Total number of jobs in history    = %d\n", intTotalHistory);
    printf("Total number of jobs in background = %d\n", intTotalJobsInBackground);
    
    exit(0);
}

int builtin_jobs(void)
{
    int i;
    for(i = 0; i < intBGJobSize; i++)
    {
        if(waitpid(arrBGJobs[i]->pid, NULL, WNOHANG) == 0)
        { 
            arrBGJobs[i]->blnIsRunning = true;
        }
        else
        {
            arrBGJobs[i]->blnIsRunning = false;
        }

        if(arrBGJobs[i]->blnWasDisplayed == false){
            printf("[%d]\t", i + 1);
            if(arrBGJobs[i]->blnIsRunning == true){
                printf("Running\t");
            }
            else
            {
                printf("Done\t");
                arrBGJobs[i]->blnWasDisplayed = true;
            }
            printf("%s\n", arrBGJobs[i]->strFullCommand);
        }
    }
    return 0;
}

int builtin_history(void)
{

    int i;
    for(i = 0; i < intJobHistorySize; i++)
    {
        printf("%d\t%s\n", i + 1, arrJobHistory[i]);
    }
    return 0;
}

int builtin_wait(void)
{
    int i;
    for(i = 0; i < intBGJobSize; i++)
    {
        waitpid(arrBGJobs[i]->pid, NULL, 0);
    }
    return 0;
}

int builtin_fg(void)
{
    int i;
    for(i = intBGJobSize - 1; i > 0; i--){
        if(arrBGJobs[i]->blnIsRunning)
        {
            waitpid(arrBGJobs[i]->pid, NULL, 0);
            arrBGJobs[i]->blnWasDisplayed = true;
            return 0;
        }
    }
    fprintf(stderr, "ERROR: There is no running background Process!\n");
    return 0;
}

int builtin_fg_num(int intJobNum)
{
    intJobNum--;
    if(intJobNum < intBGJobSize && arrBGJobs[intJobNum]->blnIsRunning == true)
    {
        waitpid(arrBGJobs[intJobNum]->pid, NULL, 0);
        arrBGJobs[intJobNum]->blnWasDisplayed = true;
    }
    else
    {
        fprintf(stderr, "ERROR: That is not a running Process!\n");
    }
    return 0;
}

int main(int argc, char * argv[])
{
	parseCommandLine(argc, argv);

	if(blnIsBatch)
	{
		runInBatchMode(argc, argv);
        builtin_exit();
	}
	else
	{
		runInInteractiveMode();
	}

	return 0;
}