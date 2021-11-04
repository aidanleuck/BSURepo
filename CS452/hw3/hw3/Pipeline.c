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

// Initialize a new pipeline
extern Pipeline newPipeline(int fg) {
  PipelineRep r=(PipelineRep)malloc(sizeof(*r));

  if (!r)
    ERROR("malloc() failed");
  r->processes=deq_new();
  r->fg=fg;
  return r;
}

static void allocatePipeline(Pipeline pipeline){
    PipelineRep r=(PipelineRep)pipeline;
    r->oFd = malloc(sizeof(int) * 2);
    r->oFd[0] = dup(STDIN_FILENO);
    r->oFd[1] = dup(STDOUT_FILENO);

    r->pFd = malloc(sizeof(int)* 2 * sizePipeline(r));
}
// Add pipeline to queue
extern void addPipeline(Pipeline pipeline, Command command) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_tail_put(r->processes,command);
}

// Gets the size of the pipeline
extern int sizePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  return deq_len(r->processes);
}

// Setups a pipeline with proper number of pipes
extern void setupPipeLine(Pipeline pipeline, int i){
  PipelineRep r=(PipelineRep)pipeline;
  int pipelineLength = sizePipeline(r);
  int lastIndex = pipelineLength - 1;
  int currIndex = i *2;
    if(pipe(r->pFd + (i*2)) == -1){
      ERROR("Failed to pipe");
    }
    // Check to make sure we are executing a pipeline
    if(pipelineLength > 1){
      // first iteration redirect output of pipe to stdout
      if(i == 0){
        dup2(r->pFd[1], STDOUT_FILENO);
        close(r->pFd[1]);
      }
      // Last iteration of the pipe send output of pipe to stdout
      else if (i==lastIndex){
        // Restore stdout
        dup2(r->oFd[1], STDOUT_FILENO);

        // Redirect read of pipe to stdin 
        dup2(r->pFd[currIndex - 2], STDIN_FILENO);

        // Close the pipe
        close(r->pFd[currIndex]);
        close(r->oFd[1]);
      }
      // If we are in the middle of a pipe 
      else{
        // Keep track of previous pipe read
        int oldReadEnd = r->pFd[currIndex - 2];

        // Duplicate the previous read end to stdin
        dup2(oldReadEnd, STDIN_FILENO);
        close(oldReadEnd);

        // redirect write end of pipe to stdout
        dup2(r->pFd[currIndex + 1], STDOUT_FILENO);
        close(r->pFd[currIndex + 1]);
      }
    }
  }

// Executes the pipeline
static void execute(Pipeline pipeline, Jobs jobs, int *jobbed, int *eof) {
  PipelineRep r=(PipelineRep)pipeline;
  allocatePipeline(pipeline);

  int fg = r->fg;

  fg = sizePipeline(r) > 1 ? 0 : fg;

  // Executes the command inside the pipeline  
  for (int i=0; i<sizePipeline(r) && !*eof; i++){
    setupPipeLine(pipeline, i);
    execCommand(deq_head_ith(r->processes,i),pipeline,jobs,jobbed,eof,fg);
    
  }
  // Restores stdin
  dup2(r->oFd[0], STDIN_FILENO);
  close(r->oFd[0]);
}

// Execute the pipeline 
extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof) {
  int jobbed= 0;
  execute(pipeline,jobs,&jobbed,eof);
  if (!jobbed)
    freePipeline(pipeline);	// for fg builtins, and such

  // Wait for all children to exit
  if(*eof){
    wait(NULL);
    exit(0);
  }
}

// Free the pipeline
extern void freePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_del(r->processes,freeCommand);
  free(r->oFd);
  free(r->pFd);
  free(r);
}
