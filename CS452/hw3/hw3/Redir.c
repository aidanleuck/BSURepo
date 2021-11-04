
#include "Redir.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct redir{
    char* file;
    int *openFile;
    int hasRedirect;
} *RedirData;
typedef struct data {
    RedirData data[END];
} *RedirRep;

// Creates a new redirection node
extern Redir newRedir(T_redir redirec){
    RedirRep r = (RedirRep) malloc(sizeof(*r));

    // Allocate memory for both sides (input and output)
    r->data[STDIN] = malloc(sizeof(struct redir));
    r->data[STDOUT] = malloc(sizeof(struct redir));

    // Allocate memory for saved integer fd's
    r->data[STDIN]->openFile = malloc(sizeof(int));
    r->data[STDOUT]->openFile = malloc(sizeof(int));

    // Sets defaults
    r->data[STDIN]->file = NULL;
    r->data[STDOUT]->file = NULL;
    r->data[STDIN]->hasRedirect = 0;
    r->data[STDOUT]->hasRedirect = 0;

     // Sets the file to redirect to if applicable
    if(redirec->data[STDIN].file){
        r->data[STDIN]->file = redirec->data[STDIN].file->s;
        r->data[STDIN]->hasRedirect = 1;
    }
    if(redirec->data[STDOUT].file){
        r->data[STDOUT]->file = redirec->data[STDOUT].file->s;
        r->data[STDOUT]->hasRedirect = 1;
    }
    return r;
}
// Frees memory
extern void free_redir(Redir redir){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];

    free(stdinData->openFile);
    free(stdoutData->openFile);

    free(stdoutData);
    free(stdinData);

    free(r);
}
// Executes the redirection by fiddling with fd's
void execRedir(Redir redir){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];

    if (stdinData->hasRedirect)
    {
        *stdinData->openFile = open(stdinData->file, O_RDONLY);
        dup2(*stdinData->openFile, STDIN_FILENO);
    }
    if (stdoutData->hasRedirect)
    {
        *stdoutData->openFile = open(stdoutData->file, O_WRONLY | O_TRUNC , 0777);
        dup2(*stdoutData->openFile, STDOUT_FILENO);
    }
    if (stdoutData == NULL && stdinData == NULL)
    {
        ERROR("No direction for redirection node");
    }
}

// Closes the file descriptors
void closeDescriptors(Redir redir, int in, int out){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];
    if (stdinData->hasRedirect)
    {
        close(*stdinData->openFile);
    }
    if (stdoutData->hasRedirect)
    {
        close(*stdoutData->openFile);
    }

    // Always restore the file descriptors regardless if a redirection occured
    dup2(in, 0);
    dup2(out, 1);
    close(in);
    close(out);
    
}