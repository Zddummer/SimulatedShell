Author(s):
Zach Dummer

Date:
3/3/2020

Description:
This program is a simplified version of a typical bash (Linux) shell.

How to build the software
Simply type "make" into the commandline from the path [<user>@CS441 cs441-shell]$ and hit enter.
	Example: [dummer.zach@CS441 cs441-shell]$ make

How to use the software
Once built you can run the shell buy typing in ./mysh and hitting enter.
You can run the shell in two modes, interactive and batch.

Batch mode: This mode allows you the enter a or multiple file name(s) or file path(s) after ./mysh
	which can contain commands which the shell will execute in the order provided.

Interactive mode: This mode allows the use to use the shell just like a typical bash shell

Built in shell commands:
	- jobs : lists the background jobs that have completed or are currently running
	- history : lists all the commands enterd to the shell reguardless of succesful execution
	- wait : waits for all the background jobs to complete
	- fg : brings most recent background job to foreground
	- fg <jobID> : brings a specific background job to the foreground
	- exit : waits for all background jobs to complete, displays statistics of the session, and exits the shell

How the software was tested
There are a list of test files in the given-tests, and tests folders, aswell as some sample programs to run in the tests folder.
Further information can be found in the README.md file in the tests folder.

Known bugs and problem areas
When redirecting input in interective mode, the file must be within the same path ./mysh was executed from

If you find a bug or issue please raise it @ https://bitbucket.org/zdummer/cs441-shell/src/master/