#ifndef PIPELINE_H
#define PIPELINE_H

typedef void *Pipeline;

#include "Command.h"
#include "Jobs.h"
extern void swapFd(Pipeline pipeline, Command command);
extern void writeOut(Pipeline pipeline);
extern void setupPipeLine(Pipeline pipeline);
extern void incrementPipe(Pipeline pipeline, Command command);
extern void redirectPipe(Pipeline pipeline, Command command);

extern Pipeline newPipeline(int fg);
extern void addPipeline(Pipeline pipeline, Command command);
extern int sizePipeline(Pipeline pipeline);
extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof);
extern void freePipeline(Pipeline pipeline);
extern void setupPipeline(Pipeline pipeline);



#endif
