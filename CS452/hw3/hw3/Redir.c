
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
} *RedirData;
typedef struct data {
    RedirData data[END];
} *RedirRep;

extern Redir newRedir(T_redir redirec){
    RedirRep r = (RedirRep) malloc(sizeof(*r));
    r->data[STDIN] = malloc(sizeof(struct data));
    r->data[STDOUT] = malloc(sizeof(struct data));
    if(redirec->data[STDIN].file){
        r->data[STDIN]->file = redirec->data[STDIN].file->s;
    }
    if(redirec->data[STDOUT].file){
        r->data[STDOUT]->file = redirec->data[STDOUT].file->s;
    }
    return r;
}
extern void free_redir(Redir redir){
     RedirRep r = (RedirRep)redir;
     RedirData stdinData = r->data[STDIN];
     RedirData stdoutData = r->data[STDOUT];

     if(stdinData != NULL){
         free(stdinData->fp);
         free(stdinData->openFile);
         free(stdinData);
     }
     if(stdoutData != NULL){
         free(stdoutData->fp);
         free(stdoutData->openFile);
         free(stdoutData);
     }
     free(r);
}
void execRedir(Redir redir){
    RedirRep r = (RedirRep)redir;
    RedirData stdinData = r->data[STDIN];
    RedirData stdoutData = r->data[STDOUT];
    if (stdinData->file != NULL)
    {
        stdinData->openFile = malloc(sizeof(int));
        stdinData->fp = malloc(sizeof(int));

        *stdinData->openFile = open(stdinData->file, O_RDONLY);
        *stdinData->fp = dup(0);
        close(0);
        dup(*stdinData->openFile);
    }
    if (stdoutData->file != NULL)
    {
        stdoutData->openFile = malloc(sizeof(int));
        stdoutData->fp = malloc(sizeof(int));

        *stdoutData->openFile = open(stdoutData->file, O_WRONLY | O_CREAT, 0777);
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
    if (stdinData->fp != NULL)
    {
        dup2(*stdinData->fp, 0);
        close(*stdinData->openFile);
    }
    if (stdoutData->fp != NULL)
    {
        dup2(*stdoutData->fp, 1);
        close(*stdoutData->openFile);
    }
}
