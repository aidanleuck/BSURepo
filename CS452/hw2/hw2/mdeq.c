#include "mdeq.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int maxCapacity;
    pthread_mutex_t mutex;
    pthread_cond_t signalConsume;
    pthread_cond_t signalProduce;
    Deq q;
} *Mrep;

Mdeq mdeq_new(int capacity){
    Mrep mdeq = (Mrep) malloc(sizeof(*mdeq));
    mdeq->q = deq_new();
    mdeq->maxCapacity = capacity;
    pthread_mutex_init(&mdeq->mutex, 0);
    pthread_cond_init(&mdeq->signalConsume, 0);
    pthread_cond_init(&mdeq->signalProduce, 0 );
    return (Mdeq) mdeq;
}
void mdeq_tail_put(Mdeq mdeq, Data d){
    Mrep rep = (Mrep)(mdeq);
    pthread_mutex_lock(&rep->mutex);
    while(deq_len(rep->q) >= rep->maxCapacity && rep->maxCapacity > 0){
        pthread_cond_wait(&rep->signalConsume, &rep->mutex);
    }
    deq_tail_put(rep->q, d);
    pthread_cond_signal(&rep->signalProduce);
    pthread_mutex_unlock(&rep->mutex);
}
Data mdeq_head_get(Mdeq mdeq){
    Mrep rep = (Mrep)(mdeq);
    pthread_mutex_lock(&rep->mutex);
    while(deq_len(rep->q) == 0){
        pthread_cond_wait(&rep->signalProduce, &rep->mutex);
    }
    Data returnedVal = deq_head_get(rep->q);
    pthread_cond_signal(&rep->signalConsume);
    pthread_mutex_unlock(&rep->mutex);
    return returnedVal;
}
void mdeq_del(Mdeq mdeq, DeqMapF remFunction){
    Mrep rep = (Mrep)(mdeq);
    pthread_mutex_destroy(&rep->mutex);
    pthread_cond_destroy(&rep->signalProduce);
    pthread_cond_destroy(&rep->signalConsume);
    deq_del(rep->q, remFunction);
    free(rep);
}