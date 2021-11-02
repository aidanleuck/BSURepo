#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Parser.h"
#include "Tree.h"
#include "Scanner.h"
#include "error.h"

static Scanner scan;

#undef ERROR
#define ERROR(s) ERRORLOC(__FILE__,__LINE__,"error","%s (pos: %d)",s,posScanner(scan))

static char *next()       { return nextScanner(scan); }
static char *curr()       { return currScanner(scan); }
static int   cmp(char *s) { return cmpScanner(scan,s); }
static int   eat(char *s) { return eatScanner(scan,s); }

static T_word p_word();
static T_words p_words();
static T_command p_command();
static T_pipeline p_pipeline();
static T_sequence p_sequence();
static T_redir p_redir();

static T_word p_word() {
  char *s=curr();
  if (!s)
    return 0;
  T_word word=new_word();
  word->s=strdup(s);
  next();
  return word;
}

static T_words p_words() {
  T_word word=p_word();
  if (!word)
    return 0;
  T_words words=new_words();
  words->word=word;
  if (cmp("|") || cmp("&") || cmp(";") || cmp(">") || cmp("<"))
    return words;
  words->words=p_words();
  return words;
}

static T_redir p_redir(){
  T_redir redirec = new_redir();
  redirec->data[STDIN].file = NULL;
  redirec->data[STDOUT].file = NULL;
 
  if(eat("<")){
    T_word fileIn = p_word();
    redirec->data[STDIN].file = fileIn;
    if(!fileIn){
      ERROR("No file for input");
    }
  }
  if(eat(">")){
    T_word fileOut = p_word();
    redirec->data[STDOUT].file = fileOut;
    if(!fileOut){
      ERROR("No file for output");
    }
  }
  return redirec;
}

static T_command p_command() {
  T_words words=0;
  T_redir redirec = 0;
  words=p_words();
  redirec = p_redir();
  if (!words)
    return 0;
  T_command command=new_command();
  command->words=words;
  command->redirection = redirec;
  return command;
}

static T_pipeline p_pipeline() {
  T_command command=p_command();
  if (!command)
    return 0;
  T_pipeline pipeline=new_pipeline();
  pipeline->command=command;
  if (eat("|"))
    pipeline->pipeline=p_pipeline();
  return pipeline;
}

static T_sequence p_sequence() {
  T_pipeline pipeline=p_pipeline();
  if (!pipeline)
    return 0;
  T_sequence sequence=new_sequence();
  sequence->pipeline=pipeline;
  char *op = malloc(sizeof(char));
  if (eat("&")) {
    *op = '&';
    sequence->op = op;
    sequence->sequence=p_sequence();
  }
  else if (eat(";")) {
    *op = ';';
    sequence->op = op;
    sequence->sequence=p_sequence();
  }
  else{
    sequence->op = op;
  }
  return sequence;
}

extern Tree parseTree(char *s) {
  scan=newScanner(s);
  Tree tree=p_sequence();
  if (curr())
    ERROR("extra characters at end of input");
  freeScanner(scan);
  return tree;
}

static void f_redir(T_redir t);
static void f_word(T_word t);
static void f_words(T_words t);
static void f_command(T_command t);
static void f_pipeline(T_pipeline t);
static void f_sequence(T_sequence t);

static void f_redir(T_redir t){
  if(!t){
    return;
  }
  if(t->data[STDIN].file){
    free(t->data[STDIN].file);
  }
  if(t->data[STDOUT].file){
    free(t->data[STDOUT].file);
  }
  free(t);
}

static void f_word(T_word t) {
  if (!t)
    return;
  if (t->s)
    free(t->s);
  free(t);
}

static void f_words(T_words t) {
  if (!t)
    return;
  f_word(t->word);
  f_words(t->words);
  free(t);
}

static void f_command(T_command t) {
  if (!t)
    return;
  f_words(t->words);
  f_redir(t->redirection);
  free(t);
}

static void f_pipeline(T_pipeline t) {
  if (!t)
    return;
  f_command(t->command);
  f_pipeline(t->pipeline);
  free(t);
}

static void f_sequence(T_sequence t) {
  if (!t)
    return;
  f_pipeline(t->pipeline);
  f_sequence(t->sequence);
  free(t);
}

extern void freeTree(Tree t) {
  f_sequence(t);
}
