#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Interpreter.h"
#include "Redir.h"
#include "Sequence.h"
#include "Pipeline.h"
#include "Command.h"

static Command i_command(T_command t);
static void i_pipeline(T_pipeline t, Pipeline pipeline);
static void i_sequence(T_sequence t, Sequence sequence);

// Interprets a command 
static Command i_command(T_command t) {
  if (!t)
    return 0;
  Command command=0;

  if (t->words)
    command=newCommand(t->words, t->redirection);
  return command;
}

// Interprets a pipeline, adding nested pipelines as needed
static void i_pipeline(T_pipeline t, Pipeline pipeline) {
  if (!t)
    return;
  addPipeline(pipeline,i_command(t->command));
  i_pipeline(t->pipeline,pipeline);
}

// Interprets each sequence
static void i_sequence(T_sequence t, Sequence sequence) {
  if (!t)
    return;
  int fg = 1;
  
  fg = strcmp(t->op, "&") ? 1 : 0;
    
  Pipeline pipeline=newPipeline(fg);
  i_pipeline(t->pipeline,pipeline);
  addSequence(sequence,pipeline);
  i_sequence(t->sequence,sequence);
}

// Interprets the entire tree starting at sequences
extern void interpretTree(Tree t, int *eof, Jobs jobs) {
  if (!t)
    return;
  Sequence sequence=newSequence();
  i_sequence(t,sequence);
  execSequence(sequence,jobs,eof);
}
