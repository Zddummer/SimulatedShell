/*
 * Zach Dummer
 *
 * CS441/541: Project 3
 *
 */
#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* For fork, exec, sleep */
#include <sys/types.h>
#include <unistd.h>
/* For waitpid */
#include <sys/wait.h>

/******************************
 * Defines
 ******************************/
#define MAX_COMMAND_LINE 1024
#define PROMPT ("mysh$ ")


/******************************
 * Structures
 ******************************/
/*
 * A job struct.  Feel free to change as needed.
 */
typedef struct job_t {
    char * strFullCommand;
    int intArgCount;
    char **arrArgArray;
    bool isBackground;
    char * binary;
} job_t;

typedef struct bgJob {
	pid_t pid;
	bool blnIsRunning;
	bool blnWasDisplayed;
	char * strFullCommand;
} bgJob;

/******************************
 * Global Variables
 ******************************/
char **arrJobHistory = NULL;
int intJobHistorySize = 0;

bgJob **arrBGJobs = NULL;
int intBGJobSize = 0;
 
/*
 * Interactive or batch mode
 */
bool blnIsBatch = false;


/*
 * Counts
 */
int intTotalJobs    = 0;
int intTotalJobsInBackground = 0;
int intTotalHistory = 0;

/******************************
 * Function declarations
 ******************************/
void parseCommandLine(int argc, char * argv[]);
void createJobs(char *strInputFromCLI);
void runInBatchMode(int argc, char * argv[]);
void runInInteractiveMode();
void addJobToHistory(job_t *CurrentJob);
void addJobToBG(bgJob *jobToAdd);
bool executeCommand(job_t *CurrentJob);

/*
 * Built-in 'exit' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
void builtin_exit(void);

/*
 * Built-in 'jobs' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_jobs(void);

/*
 * Built-in 'history' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_history(void);

/*
 * Built-in 'wait' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_wait(void);

/*
 * Built-in 'fg' command
 *
 * Parameters:
 *   None (use default behavior)
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_fg(void);

/*
 * Built-in 'fg' command
 *
 * Parameters:
 *   Job id
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_fg_num(int intJobNum);

#endif /* MYSHELL_H */
