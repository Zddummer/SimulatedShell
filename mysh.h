/*
 * Zach Dummer
 *
 * Header file for mysh.c
 *
 * Date last modified : 3/3/2020
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
#include <sys/stat.h>
#include <fcntl.h>
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
 * A job struct.
 */
typedef struct job_t {
    char * strFullCommand;
    int intArgCount;
    char **arrArgArray;
    bool isBackground;
    char * binary;
} job_t;

/*
 * A book keeping struct for jobs in the background.
 */
typedef struct bgJob {
	pid_t pid;
	bool blnIsRunning;
	bool blnWasDisplayed;
	char * strFullCommand;
} bgJob;

/******************************
 * Global Variables
 ******************************/
/*
 * Book keeping variables.
 */
char **arrJobHistory = NULL;
int intJobHistorySize = 0;

bgJob **arrBGJobs = NULL;
int intBGJobSize = 0;
 
/*
 * Variables for file redirect.
 */
int intFileDescriptor = 0;
char *strOutputFileName;
char *strInputFileName;

/*
 * Booleans.
 */
bool blnIsBatch = false;
bool blnWasOutputRedirected = false;
bool blnWasInputRedirected = false;

/*
 * Counts
 */
int intTotalJobs    = 0;
int intTotalJobsInBackground = 0;
int intTotalHistory = 0;

/******************************
 * Function declarations
 ******************************/

/*
 * Check valid args and set mode
 *
 * Parameters:
 *   arg count
 *	 arg array
 *
 * Returns:
 *   void
 */
void parseCommandLine(int argc, char * argv[]);

/*
 * Start of pipeline for batch mode
 *
 * Parameters:
 *   arg count
 *	 arg array
 *
 * Returns:
 *   void
 */
void runInBatchMode(int argc, char * argv[]);

/*
 * Start of pipeline for interactive mode
 *
 * Parameters:
 *   arg count
 *	 arg array
 *
 * Returns:
 *   void
 */
void runInInteractiveMode();

/*
 * Creates jobs from string
 *
 * Parameters:
 *   Command
 *
 * Returns:
 *   void
 */
void createJobs(char *strInputFromCLI);

/*
 * adds job to history for book keeping
 *
 * Parameters:
 *   Job to add
 *
 * Returns:
 *   void
 */
void addJobToHistory(job_t *CurrentJob);

/*
 * book keeping for background jobs
 *
 * Parameters:
 *   Job to add
 *
 * Returns:
 *   void
 */
void addJobToBG(bgJob *jobToAdd);

/*
 * Changes program output from STDOUT to a file
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   void
 */
void handleOutRedirect();

/*
 * Changes program input from STDIN to a file
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   void
 */
void handleInRedirect();

/*
 * executes a created job
 *
 * Parameters:
 *   Job to execute
 *
 * Returns:
 *   true if succseful
 *	 else false
 */
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
