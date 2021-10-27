#ifndef PIPELINE_H
#define PIPELINE_H

typedef void *Pipeline;

#include "Command.h"
#include "Jobs.h"
static void restoreFd(Pipeline pipeline, int* fd);
static void writeFd(Pipeline pipeline);
void redirPipe(Pipeline pipeline);

extern Pipeline newPipeline(int fg);
extern void addPipeline(Pipeline pipeline, Command command);
extern int sizePipeline(Pipeline pipeline);
extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof);
extern void freePipeline(Pipeline pipeline);
extern void setupPipeline(Pipeline pipeline);



#endif
