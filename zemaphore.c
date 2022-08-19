#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zemaphore* z, int init)
{
    z->counter = init;
    if(pthread_cond_init(&z->cond, NULL) != 0){
        printf("pthread_cond_init failed\n");
        exit(0);
    }
    if(pthread_mutex_init(&z->mutex, NULL) != 0){
        printf("pthread_mutex_init failed\n");
        exit(0);
    }
}

void zem_up(zemaphore* z)
{
    pthread_mutex_lock(&z->mutex);
    z->counter++;
    pthread_cond_signal(&z->cond);
    pthread_mutex_unlock(&z->mutex);
}

void zem_down(zemaphore* z)
{
    pthread_mutex_lock(&z->mutex);
    while(z->counter == 0){
        pthread_cond_wait(&z->cond, &z->mutex);
    }
    z->counter--;
    pthread_mutex_unlock(&z->mutex);
}