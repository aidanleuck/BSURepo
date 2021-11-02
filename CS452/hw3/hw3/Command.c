#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/history.h>

#include "Command.h"
#include "error.h"
#include "Redir.h"

typedef struct {
  char *file;
  char **argv;
  Redir redirec

} *CommandRep;

#define BIARGS CommandRep r, int *eof, Jobs jobs
#define BINAME(name) bi_##name
#define BIDEFN(name) static void BINAME(name) (BIARGS)
#define BIENTRY(name) {#name,BINAME(name)}

static char *owd=0;
static char *cwd=0;

static void builtin_args(CommandRep r, int n) {
  char **argv=r->argv;
  for (n++; *argv++; n--);
  if (n)
    ERROR("wrong number of arguments to builtin command"); // warn
}

BIDEFN(exit) {
  builtin_args(r,0);
  *eof=1;
}

BIDEFN(pwd) {
  builtin_args(r,0);
  if (!cwd)
    cwd=getcwd(0,0);
  printf("%s\n",cwd);
}

BIDEFN(cd) {
  builtin_args(r,1);
  if (strcmp(r->argv[1],"-")==0) {
    char *twd=cwd;
    cwd=owd;
    owd=twd;
  } else {
    if (owd) free(owd);
    owd=cwd;
    cwd=strdup(r->argv[1]);
  }
  if (cwd && chdir(cwd))
    ERROR("chdir() failed"); // warn
}

BIDEFN(history){
  HISTORY_STATE* state = history_get_history_state();
  HIST_ENTRY** entry = history_list();
  for(int i =0; i < state->length; i++){
    printf("%d: %s\n",i, entry[i]->line);
  }
}

static int builtin(BIARGS) {
  typedef struct {
    char *s;
    void (*f)(BIARGS);
  } Builtin;
  static const Builtin builtins[]={
    BIENTRY(exit),
    BIENTRY(pwd),
    BIENTRY(cd),
    BIENTRY(history),
    {0,0}
  };
  int i;
  for (i=0; builtins[i].s; i++)
    if (!strcmp(r->file,builtins[i].s)) {
      //execRedir(r->redirec);
      builtins[i].f(r,eof,jobs);
      return 1;
    }
  return 0;
}

static char **getargs(T_words words) {
  int n=0;
  T_words p=words;
  while (p) {
    p=p->words;
    n++;
  }
  char **argv=(char **)malloc(sizeof(char *)*(n+1));
  if (!argv)
    ERROR("malloc() failed");
  p=words;
  int i=0;
  while (p) {
    argv[i++]=strdup(p->word->s);
    p=p->words;
  }
  argv[i]=0;
  return argv;
}

extern Command newCommand(T_words words, T_redir redirec) {
  CommandRep r=(CommandRep)malloc(sizeof(*r));
  r->redirec = newRedir(redirec);
  if (!r)
    ERROR("malloc() failed");
  r->argv=getargs(words);
  r->file=r->argv[0];
  return r;
}
 static int waitChild(int pid){
  int status;
  waitpid(pid, &status, 0);
  return status;
}

static int child(CommandRep r, int fg) {
  int eof=0;
  Jobs jobs=newJobs();
  if (builtin(r,&eof,jobs))
    return;
  execvp(r->argv[0],r->argv);
  ERROR("execvp() failed");
  exit(0);
}

extern void execCommand(Command command, Pipeline pipeline, Jobs jobs,
			int *jobbed, int *eof, int fg) {
  CommandRep r = command;
  if (fg && builtin(r, eof, jobs) && sizePipeline(pipeline) == 1){
   //closeDescriptors(r->redirec);
   return;
  }
    
  if (!*jobbed)
  {
    *jobbed = 1;
    addJobs(jobs, pipeline);
  }
  incrementPipe(pipeline, command);
  int pid = fork();
  if (pid == -1)
    ERROR("fork() failed");
  if (pid == 0){
    execRedir(r->redirec);
    swapFd(pipeline, command);
    child(r, fg);
  }
  if (pid > 0) 
  {
    closeDescriptors(r->redirec);
    redirectPipe(pipeline, command); 

    if(fg){
      if(waitChild(pid) < 0)
        ERROR("Child exited abnormally");
    }
  }
}

extern void freeCommand(Command command) {
  CommandRep r=command;
  char **argv=r->argv;
  while (*argv)
    free(*argv++);
  free(r->argv);
  free_redir(r->redirec);
  free(r);
}

extern void freestateCommand() {
  if (cwd) free(cwd);
  if (owd) free(owd);
}
