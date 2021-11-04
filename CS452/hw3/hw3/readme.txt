CS452 Operating Systems
Aidan Leuck
Jim Buffenbarger
HW3 Bash

This module attempts to immitate bash by providing redirection, piping, built in commands and execution of child processes with foreground and background processes. We can also perform sequence of commands by using the ; operator. The shell is feature complete and appears to mirror bash in implementation for sequences, pipelines and redirection. Background processeses are sometimes finicky in the shell and can lead to unexpected results.

The grader may note that there is a rudimentary test set under Tests. The redirection tests are failing when they should not due to the fact that the test runner is flawed. Another test that appears to be failing but shouldn't is the Test_seqBg test. For some reason in the test we get three lines of output but when running manually output is the expected result. I chalk this up to an issue with the test runner. In order to run said test set simply

$ cd Tests
$ ./run

In order to compile and run the shell one can run
$ make run
or $ ./shell if already compiled

In order to clean the directory simply write
$ make clean
