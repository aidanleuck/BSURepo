#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERR(s) err(s,__FILE__,__LINE__)

static void err(char *s, char *file, int line) {
  fprintf(stderr,"%s:%d: %s\n",file,line,s);
  exit(1);
}
// Per device struct
typedef struct
{
    char *sep;      // Default list of separators
} Device;

// Per init struct
typedef struct
{
    char *s;            // Input string
    char *sep;          // List of separators
    size_t inputSize;   // Size of the input
    int ioctl;          // Keeps track if next write will be to separator
    size_t sepLength;   // Length of the separators
    size_t inputScanned; // How much of the input has been scanned so far
} Scanner;

// <summary>Creates an instance of the scanner</summary>
// <param name = "inode">Inode associated with the driver</param>
// <param name = "filp">Holds private data about an instance</param>
// <returns>File descriptor to the scanner</returns>
static Scanner* open(Device* device)
{
    // Allocates memory for the scanner
    Scanner* scanner = (Scanner*)malloc(sizeof(*scanner));
    if(!scanner){
        printf("%s","failed to malloc scanner");
    } 

    // Allocates memory for the default list of separators
    scanner->sep = malloc(strlen(device->sep) + 1);
    if(!scanner->sep){
      printf("%s", "Malloc failed");
    }

    // Copies the default list to the scanner instance
    strcpy(scanner->sep, device->sep);
    scanner->sepLength = strlen(device->sep);

    // Sets ioctl to 1 by default (next write will not be to separators)
    scanner->ioctl = 1;
    return scanner;
}

// <summary>Frees instance of a scanner</summary>
// <param name = "inode">Inode associated with the driver</param>
// <param name = "filp">Holds private data about an instance</param>
// <returns>Whether release was succesful</returns>
static int release(Scanner* scanner){
    free(scanner->sep);
    free(scanner);
    return 0;
}

// <summary>Determines if a character is in the list of separators</summary>
// <param name = "scan">Instance of scanner to check</param>
// <param name = "cmp">The character to check if in list</param>
// <returns>1 if in list 0 otherwise</returns>
static int inSep(Scanner* scan, char cmp)
{
    int i;
    for (i = 0; i < scan->sepLength; i++)
    {
        if (scan->sep[i] == cmp)
        {
            return 1;
        }
    }
    return 0;
}

// <summary>Reads input from device and separates into tokens</summary>
// <param name = "buf">The buffer to fill and return to user</param>
// <param name = "filp">Holds private data about an instance</param>
// <param name = "charRequested">Number of characters requested by user</param>
// <param name = "f_pos">File offset to start at</param>
// <returns>Number of characters read, EOF, or 0 if reading a token</returns>
extern ssize_t read(Scanner* scan, char *buf, size_t charRequested)
{
    // Allocate enough memory to hold the number of characters requested
    char *currentString = (char*) malloc(sizeof(char) * (charRequested + 1));
    memset(currentString, 0, sizeof(char) * charRequested + 1);
    if(!currentString){
        printf("%s", "Scanner failed");
    }

    size_t numCharRead = 0; // Keep track of number of characters read
    int tokenFound = 0;     // Keep track if a token was found

    while (numCharRead < charRequested && !tokenFound && scan->inputScanned < scan->inputSize)
    {
        // Gets the current character in input
        char currChar = (scan->s[scan->inputScanned]);

        // Checks if the current character was a token
        tokenFound = inSep(scan, currChar);

        // If not a separator add it to our string
        if (!tokenFound)
        {
            currentString[numCharRead] = currChar;
            currentString[numCharRead + 1] = '\0';

            // Increments the number of characters in buffer
            numCharRead++;

            // Increments how much of the input has been scanned
            scan->inputScanned++;
        }
    }
    strcpy(buf, currentString);
    free(currentString);

    // Sets numcharRead accordingly
    if(scan->inputScanned == scan->inputSize && numCharRead == 0){
        numCharRead = -1;
        free(scan->s);
    }
    if(tokenFound && numCharRead == 0){
        scan->inputScanned++;
    }

    return numCharRead;
}

// <summary>Sets flag in driver to tell us that next write will be list of separators</summary>
// <param name = "filp">Holds private data about an instance</param>
// <param name = "cmd">command to execute</param>
// <param name = "arg">arg to execute</param>
// <returns>0</returns>
static long ioctl(Scanner* scan,
                  unsigned int cmd,
                  unsigned long arg)
{
    if (cmd == 0 && arg == 0)
    {
        scan->ioctl = 0;
    }
    return 0;
}

// <summary>Writes device input into scanner memory</summary>
// <param name = "filp">Holds private data about an instance</param>
// <param name = "line">Line written to device</param>
// <param name = "len">Length of the input</param>
// <param name = "f_pos">Offset to start reading from</param>
// <returns>Number of characters written to device</returns>
extern ssize_t write(Scanner* scan, const char *line, 
size_t len)
{
    // Check if ioctl set or not
    if (!scan->ioctl)
    {
        free(scan->sep);
        scan->sep = malloc(sizeof(char) * (len + 1));
        scan->sep = memset(scan->sep, 0, sizeof(char)* (len + 1));
        strcpy(scan->sep, line);
        scan->sepLength = len;
        scan->ioctl = 1;
    }
    else
    {
        scan->s = malloc(sizeof(char) * (len + 1));
        scan->s = memset(scan->s, 0, sizeof(char) * (len + 1));
        strcpy(scan->s, line);
        scan->inputSize = len;
        scan->inputScanned = 0;
    }

    return len;
}

// Initializes the scanner with default values
static Device* my_init(void)
{
    Device* device = malloc(sizeof(*device));
    // List of default separators
    const char *defaultSep = ";,+-=!@./#$%&*: ";
    device->sep = (char *)malloc(strlen(defaultSep) + 1);
    if (!device->sep)
    {
        printf("kmalloc failed\n");
    }
    strcpy(device->sep, defaultSep);

    return device;
}

// Handles exit of the device driver
static void my_exit(Device* device) {
  free(device->sep);
  free(device);
}
int main(){
    Device* device = my_init();
    Scanner *scanner = open(device);

enum {max=100};
  char buf[max+1];
  int len;


  if (ioctl(scanner,0,0))
    ERR("ioctl() for separators failed");
  if (write(scanner,":",1)!=1)
    ERR("write() of separators failed");

  char *line;
  while (scanf("%m[^\n]\n",&line)!=EOF) {
    len=strlen(line);
    if (len!=write(scanner,line,len))
      ERR("write() of data failed");
    free(line);
    while ((len=read(scanner,buf,max))>=0) {
      buf[len]=0;
      printf("%s%s",buf,(len ? "" : "\n"));
    }
  }

  release(scanner);
  my_exit(device);
  return 0;
}
