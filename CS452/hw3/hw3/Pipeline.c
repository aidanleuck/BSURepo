#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

extern void writeFd(Pipeline pipeline){
  PipelineRep r=(PipelineRep)pipeline;

  char bfr[20000];
  ssize_t bytesRead = read(r->pFd[0], &bfr, sizeof(bfr));

  dup2(r->oFd[0], STDOUT_FILENO);
  dup2(r->oFd[1], STDIN_FILENO);
  write(STDIN_FILENO, &bfr, bytesRead);

  close(r->pFd[0]);
  close(r->pFd[1]);
}

void redirectPipe(Pipeline pipeline){
  PipelineRep r=(PipelineRep)pipeline;

  char bfr[20000];


  int rPipe[2];
  pipe(rPipe);

  ssize_t bytesRead = read(r->pFd[0], &bfr, sizeof(bfr));

  dup2(rPipe[0], r->oFd[0]);
  dup2(rPipe[1], r->oFd[1]);

  
  close(r->pFd[0]);
  close(r->pFd[1]);

  r->pFd[0] = rPipe[0];
  r->pFd[1] = rPipe[1];

  ssize_t n = write(r->pFd[1], &bfr, bytesRead);

 
}

static void execute(Pipeline pipeline, Jobs jobs, int *jobbed, int *eof) {
  PipelineRep r=(PipelineRep)pipeline;
  setupPipeLine(pipeline);
  for (int i=0; i<sizePipeline(r) && !*eof; i++){
    execCommand(deq_head_ith(r->processes,i),pipeline,jobs,jobbed,eof,r->fg);
    
  }
  writeFd(pipeline);
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
