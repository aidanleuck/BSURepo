#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <poll.h>

#include "Pipeline.h"
#include "deq.h"
#include "error.h"

typedef struct {
  Deq processes;
  int fg;			// not "&"
  int count;
  int *pFd;
  int *oFd;
} *PipelineRep;

extern Pipeline newPipeline(int fg) {
  PipelineRep r=(PipelineRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->processes=deq_new();
  r->fg=fg;
  r->count = 0;
  return r;
}

extern void addPipeline(Pipeline pipeline, Command command) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_tail_put(r->processes,command);
}

extern int sizePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  return deq_len(r->processes);
}
extern void setupPipeLine(Pipeline pipeline){
  PipelineRep r=(PipelineRep)pipeline;
  r->pFd = malloc(sizeof(int)* 2);
  r->oFd = malloc(sizeof(int) *2);

  if(pipe(r->pFd) == -1){
    ERROR("Failed to pipe");
  }
  r->oFd[0] = dup(STDIN_FILENO);
  r->oFd[1] = dup(STDOUT_FILENO);
  
  dup2(r->pFd[0], STDIN_FILENO);
  dup2(r->pFd[1], STDOUT_FILENO);

}

extern void swapFd(Pipeline pipeline){
  PipelineRep r=(PipelineRep)pipeline;

    int oldStdin = dup(r->pFd[0]);
    int oldStdout = dup(r->pFd[1]);

    close(r->pFd[0]);
    close(r->pFd[1]);

    r->pFd[0] = dup(oldStdout);
    r->pFd[1] = dup(oldStdin);

    close(oldStdout);
    close(oldStdin);

    // Swap the actual stdin/stdout descriptors
    dup2(r->pFd[1], STDIN_FILENO);
    dup2(r->pFd[0], STDOUT_FILENO);

    char* bfr = "TEST";
    ssize_t test = write(r->pFd[1], bfr, sizeof(bfr));


}
extern void writeOut(Pipeline pipeline){
  PipelineRep r=(PipelineRep)pipeline;
  char bfr [20000];

  dup2(r->oFd[0], STDIN_FILENO);
  dup2(r->oFd[1], STDOUT_FILENO);
  ssize_t bytesRead = 0;

  // Create a poll on the file structure so we can determine if stdout is EOF.
  struct pollfd stdout_poll = { .fd = r->pFd[1], .events = POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI };
  do{bytesRead = read(r->pFd[1], &bfr, sizeof(bfr));
    if(bytesRead < 0)
      ERROR("Reading file descriptor failed");
    if(write(STDOUT_FILENO, bfr, bytesRead)<0)
      ERROR("Failed to write to stdout");
    memset(&bfr, 0, sizeof(bfr));
  }
  // Terminates loop immediately if the file descriptor has no data
  while(poll(&stdout_poll, 1, 0));

  close(r->pFd[0]);
  close(r->pFd[1]);  
}


static void execute(Pipeline pipeline, Jobs jobs, int *jobbed, int *eof) {
  PipelineRep r=(PipelineRep)pipeline;
  //setupPipeLine(pipeline);

  for (int i=0; i<sizePipeline(r) && !*eof; i++){
    execCommand(deq_head_ith(r->processes,i),pipeline,jobs,jobbed,eof,r->fg);
  }
  //writeOut(pipeline);
  
}

extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof) {
  int jobbed= 0;
  execute(pipeline,jobs,&jobbed,eof);
  if (!jobbed)
    freePipeline(pipeline);	// for fg builtins, and such
}

extern void freePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_del(r->processes,freeCommand);
  free(r);
}
