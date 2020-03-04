1) test0.txt
	Tests the difference in running a background process vs a foreground proccess.
	The first 2 calls to "date" will be further apart than the second 2 calls since the first call 
	to sleep is in the foreground and is waited on. The second call is not.

2) test1.txt
	Tests a wide variety of bash commands aswell as all the built in commands for mysh.

3) hello_in
	Tests using file redirect for input. (use the testIn.txt file found in the parent folder).

4) hello_out
	Tests using file redirect for outputting to a file. If the file doesn't exist it is created.

5) hello_both
	Tests a combination of input and output redirection at the same time.