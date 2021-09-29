#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "lawn.h"
#include "mole.h"
#include "threads.h"

#include "mdeq.h"

Mdeq queue = NULL;

static void deleteMole(Data d){
  Mole m = (Mole)d;
  free(m);
}

static void *produce(void *args)
{
  void **newArgs = args;
  Lawn l = newArgs[0];
  Mdeq queue = newArgs[1];
  Mole mole = mole_new(l, 0, 0);

  mdeq_tail_put(queue, mole);
  return 0;
}
static void *consume()
{
  Mole whackedMole = (Mole) mdeq_head_get(queue);
  mole_whack(whackedMole);
  return 0;
}

int main()
{
  srandom(time(0));
  const int n = 100;
  const int maxCap = 5;
  Lawn lawn = lawn_new(0, 0);
  queue = mdeq_new(maxCap);

  void **args = malloc(sizeof(void*) * 2);
  args[0] = lawn;
  args[1] = queue;
  
  pthread_t** consumeThreads = create_thread(consume, n, NULL);
  pthread_t** produceThreads = create_thread(produce, n, args);
  wait_thread(produceThreads, n);
  wait_thread(consumeThreads, n);
 

  lawn_free(lawn);
  free(args);
  mdeq_del(queue, &deleteMole);
}