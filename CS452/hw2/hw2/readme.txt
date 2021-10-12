This program creates multiple moles on n different threads specified in main.c
The amount of congestion when creating moles can be changed by modifying the 
maxCapacity variable inside of main (0 being no limit while any number greater than
0 being the max number of moles displayed at once). 

The program works by using a double ended data structure that has been wrapped with 
condition variables and mutex locks. This allows the deq.c in main to be thread safe
so that no thread accesses the queue at the same time. 

There are a few different modules used in main.c. There is the supplied code mole.c and 
lawn.c that are used to display the moles on the lawn. This code was provided for me and 
uses the FLTK library in order to display this view. The modules that I created are 
deq.c, mdeq.c, and threads.c

Deq.c is the simple double ended queue data structure that allows nodes to be added at
either end. This was written in HW1. mdeq.c is a wrapper around the deq.c module that 
allows the deq.c module to be thread-safe (allows multi-threading). Threads.c handles is
simply a wrapper around pthread_join and pthread_create that allow a user to create and wait
on n threads specified in main.c.

The program ins not interactive and simply displays green moles on the screen which are then 
whacked (turns red). The program exits once all threads have exited.

RUN INSTRUCTIONS:
To run this program simply type 
$ make run 
This should compile and run the program.

To run valgrind on this program you can simply type:
$ make valgrind.

GRADER NOTE:
Note that even though make valgrind has a few errors, they are issues caused in the lawn.c and
mole.c module. (Modules I didn't create) For this reason please disregard errors in modules I did
not alter or write.