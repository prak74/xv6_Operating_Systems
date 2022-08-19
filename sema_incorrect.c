#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 5

int count = 0;
zem_t mutex;
zem_t barrier;

void* do_work(void* data)
{
    int thread_id = *((int *)data);
    for(int i=0; i<400;i++);
    printf("Work done by thread %d\n", thread_id);
    
    zem_down(&mutex);
    count++;
    if(count==NUM_THREADS) 
        zem_up(&barrier);
    zem_down(&barrier);
    zem_up(&barrier);
    zem_up(&mutex);

    printf("Thread %d released\n", thread_id);
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t mythreads[NUM_THREADS];
    int mythread_id[NUM_THREADS];

    zem_init(&mutex, 1);
    zem_init(&barrier, 0);
    
    for(int i =0; i < NUM_THREADS; i++)
    {
        mythread_id[i] = i;
        pthread_create(&mythreads[i], NULL, do_work, (void *)&mythread_id[i]);
    }

    for(int i =0; i < NUM_THREADS; i++)
    {
        pthread_join(mythreads[i], NULL);
    }

    printf("Work completed\n");

    return 0;
}