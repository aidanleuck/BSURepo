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
  int *pFd;
  int *oFd;
} *PipelineRep;

extern Pipeline newPipeline(int fg) {
  PipelineRep r=(PipelineRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->processes=deq_new();
  r->fg=fg;
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
  int pipelineLength = sizePipeline(r);
  r->pFd = malloc(sizeof(int)* 2 * pipelineLength);
  r->oFd = malloc(sizeof(int) * 2);
  for(int i =0; i + 2 < 2 * pipelineLength; i+=2){
    if(pipe(r->pFd + i) == -1){
      ERROR("Failed to pipe");
    }
  }
  r->oFd[0] = dup(STDIN_FILENO);
  r->oFd[1] = dup(STDOUT_FILENO);

}
extern void redirectPipe(Pipeline pipeline, Command currCommand){
  PipelineRep r=(PipelineRep)pipeline;
  int lastIndex = sizePipeline(r) - 1;
  Command fCommand = deq_head_ith(r->processes, 0);
  Command lCommand = deq_head_ith(r->processes, lastIndex);
  int oldOutput = r->pFd[0];
  int oldInput = r->pFd[1];

  if(fCommand){
    close(r->pFd[1]);
    dup2(r->pFd[0], r->pFd[1]);
  }
  else if(lCommand){
    close(r->pFd[1]);
    dup2(r->pFd[0], r->pFd[1]);
  }
  else{ 
    dup2(*(r->pFd-1), r->pFd[1]);
    dup2(r->pFd[1], STDIN_FILENO);
    
  }
}
extern void incrementPipe(Pipeline pipeline, Command command){
  PipelineRep r=(PipelineRep)pipeline;
  int lastIndex = sizePipeline(r) - 1;
  Command fCommand = deq_head_ith(r->processes, 0);
  Command lCommand = deq_head_ith(r->processes, lastIndex);

  if(command != fCommand && command != lCommand){
    r->pFd +=2;
  }
  
}
extern void swapFd(Pipeline pipeline, Command currCommand){
  PipelineRep r=(PipelineRep)pipeline;
  int lastIndex = sizePipeline(r) - 1;
  Command fCommand = deq_head_ith(r->processes, 0);
  Command lCommand = deq_head_ith(r->processes, lastIndex);
  if(lastIndex > 0){
     if(fCommand == currCommand){
      dup2(r->pFd[0], STDIN_FILENO);
      dup2(r->pFd[1], STDOUT_FILENO);
      close(r->pFd[1]);
    }
    if(currCommand == lCommand){
      dup2(r->pFd[0], STDIN_FILENO);
      close(r->pFd[1]);
    }
    if(currCommand != lCommand && currCommand != fCommand){
      dup2(*(r->pFd-2), STDIN_FILENO);
      dup2(r->pFd[1], STDOUT_FILENO);
      close(*(r->pFd-2));
    }
  }
}

static void execute(Pipeline pipeline, Jobs jobs, int *jobbed, int *eof) {
  PipelineRep r=(PipelineRep)pipeline;
  setupPipeLine(pipeline);

  for (int i=0; i<sizePipeline(r) && !*eof; i++){
    execCommand(deq_head_ith(r->processes,i),pipeline,jobs,jobbed,eof,r->fg);
    
  }
    dup2(r->oFd[0], STDIN_FILENO);
    dup2(r->oFd[1], STDOUT_FILENO);
    close(r->oFd[0]);
    close(r->oFd[1]);
  
}

extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof) {
  int jobbed= 0;
  execute(pipeline,jobs,&jobbed,eof);
  if (!jobbed)
    freePipeline(pipeline);	// for fg builtins, and such

  if(*eof){
    wait(NULL);
    exit(0);
  }
}

extern void freePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_del(r->processes,freeCommand);
  free(r);
}
