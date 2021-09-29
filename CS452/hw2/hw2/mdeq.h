#include "deq.h"

typedef void* Mdeq;
void mdeq_del(Mdeq, DeqMapF);
Mdeq mdeq_new(int);
void mdeq_tail_put(Mdeq, Data);
Data mdeq_head_get(Mdeq);