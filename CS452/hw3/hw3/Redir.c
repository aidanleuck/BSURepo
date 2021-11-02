
#include "Redir.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct{
    char* file;
    int *fp;
    int *openFile;
    int hasRedirect;
} *RedirData;
typedef struct data {
    RedirData data[END];
} *RedirRep;

extern Redir newRedir(T_redir redirec){
    RedirRep r = (RedirRep) malloc(sizeof(*r));
    r->data[STDIN] = malloc(sizeof(struct data));
    r->data[STDOUT] = malloc(sizeof(struct data));

    r->data[STDIN]->openFile = malloc(sizeof(int));
    r->data[STDIN]->fp = malloc(sizeof(int));
    r->data[STDOUT]->openFile = malloc(sizeof(int));
    r->data[STDOUT]->fp = malloc(sizeof(int));

    r->data[STDIN]->file = NULL;
    r->data[STDOUT]->file = NULL;
    r->data[STDIN]->hasRedirect = 0;
    r->data[STDOUT]->hasRedirect = 0;

    

    if(redirec->data[STDIN].file){
        r->data[STDIN]->file = strdup(redirec->data[STDIN].file->s);
        r->data[STDIN]->hasRedirect = 1;
    }
    if(redirec->data[STDOUT].file){
        r->data[STDOUT]->file = strdup(redirec->data[STDOUT].file->s);
        r->data[STDOUT]->hasRedirect = 1;
    }
    return r;
}
extern void free_redir(Redir redir){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];

    free(r);
}
void execRedir(Redir redir){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];

    if (stdinData->hasRedirect)
    {
        *stdinData->openFile = open(stdinData->file, O_RDONLY);
        *stdinData->fp = dup(0);
        close(0);
        dup(*stdinData->openFile);
    }
    if (stdoutData->hasRedirect)
    {
        *stdoutData->openFile = open(stdoutData->file, O_WRONLY | O_TRUNC , 0777);
        *stdoutData->fp = dup(1);
        close(1);
        dup(*stdoutData->openFile);
    }
    if (stdoutData == NULL && stdinData == NULL)
    {
        ERROR("No direction for redirection node");
    }
}
void closeDescriptors(Redir redir){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];
    if (stdinData->hasRedirect)
    {
        dup2(*stdinData->fp, 0);
        close(*stdinData->openFile);
    }
    if (stdoutData->hasRedirect)
    {
        dup2(*stdoutData->fp, 1);
        close(*stdoutData->openFile);
    }
}
