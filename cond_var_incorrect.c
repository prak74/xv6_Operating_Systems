#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <wait.h>

#define NUM_THREADS 5

int count = 0;
pthread_cond_t cond;
pthread_mutex_t mutex[NUM_THREADS];

void* do_work(void* data)
{
    int thread_id = *((int *)data);
    for(int i=0; i<400;i++);
    printf("Work done by thread %d\n", thread_id);
    count++;
    
    pthread_mutex_lock(&mutex[thread_id]);
    pthread_cond_wait(&cond, &mutex[thread_id]);
    if(count == NUM_THREADS)
        pthread_cond_broadcast(&cond);    
    pthread_mutex_unlock(&mutex[thread_id]);

    printf("Thread %d released\n", thread_id);
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t mythreads[NUM_THREADS];
    int mythread_id[NUM_THREADS];

    if(pthread_cond_init(&cond, NULL) != 0){
        printf("pthread_cond_init failed\n");
        return(0);
    }
    
    for(int i =0; i < NUM_THREADS; i++)
    {
        if(pthread_mutex_init(&mutex[i], NULL) != 0){
            printf("pthread_mutex_init failed\n");
            return(0);
        }
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