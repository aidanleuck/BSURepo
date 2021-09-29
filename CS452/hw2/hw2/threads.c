#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "threads.h"
#include "mole.h"
#include "lawn.h"
#include "error.h"


pthread_t** create_thread(TFunction passFunc, int n, void *arg){
    pthread_t** threadList = malloc(sizeof(pthread_t*) * (n));
    *threadList = malloc(sizeof(pthread_t)*(n));

    for(int i =0; i < n; i++){
        pthread_t thread_id = 0;
        int returnVal = pthread_create(&thread_id, NULL, passFunc, arg);

        if(returnVal){
            ERROR("A thread failed to be created");
        }

        *(*(threadList)+i) = thread_id;
    }
    return threadList;
}
void wait_thread(pthread_t** threads, int n){
    for(int i = 0; i < n; i++){
       pthread_t waitID =  (*threads)[i]; 
       void* exitCode;
       int returnVal = pthread_join(waitID, &exitCode);
       if(returnVal){
           ERROR("An error occured waiting for a thread to exit");
       }
       if(exitCode){
           ERROR("A thread executed abnormally");
       }
    }
    free(*threads);
    free(threads);
}