Aidan Leuck
CS452 HW4
Jim Buffenbarger
November 15, 2021

This device driver implements a rudimentary scanner that separates strings on a set of characters.
This is accomplished through the use of read and writes to the device driver. Writing stores input
into kernel memory, while reading it separates the input into tokens that are then returned to a user
provided buffer.

In order to run and load this driver:

$ make          # Compiles driver
$ make install  # Installs the driver into kernel memory
$ make try      # Runs and compiles TryScanner executable

This scanner implements a rudimentary test suite leveraging the shell script given in the previous assignment.
The shell script has been lightly modified to work with this assignment because there is an extra file leveraged 
called args. The args file is used to pass separators to each individual test run. The word DEFAULT is reserved argument
that means the test will leverage the default arguments in the scanner. The rest of the suite works the same as the previous
assignment using inp to redirect stdin and comparing exp file to the output file.

Arguments files are formatted as follows:

If you want to use the default set of separators one should type DEFAULT on the first line of the file with no newlines afterwards.
If you want to use your own set of separators, on the first line type the set of characters you want to use, and on the second line
type the number of characters in the argument you are passing. An example could because

:   # Separator
1   # Number of separators

In order to run the test suite make sure the driver is installed given the steps above.
Next from the main directory 

$ make Tests    # Compile test runner
$ cd Tests      
$ ./run         # Runs test suite


