#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include"ThreadAdmin.h"
enum{
    THREAD_STATU_CANUSE = 0,
    THREAD_STATU_BUSY,
    THREAD_STATU_EXIT,
};
typedef int thread_statu_t;
typedef struct threadNode{
    thread_statu_t status;
    pthread_t pid;
}threadNode_t;
#define INITTHHREADNUM "1024"

typedef struct{
    struct threadNode *threadarr;
    pthread_mutex_t threadarrMutex;
    
    int threadnum;
    pthread_mutex_t diethreadMutex;
    pthread_cond_t  diethreadCond;
    
    int diethread;
    
    int threadsize;
    pthread_t wait_thread_pid;
}threadarr_t;
threadarr_t *threads;
static int threadRealoocUnlock()
{
    threadNode_t *oldthread = threads->threadarr;
    int ret = threads->threadsize;
    threads->threadsize <<= 1;
    threads->threadarr = (threadNode_t*)malloc(sizeof(threadNode_t)*threads->threadsize);
    memcpy(oldthread,threads->threadarr,sizeof(*oldthread));
    free(oldthread);
    return ret;
}
static int findaFreePosUnlock()
{
    for(int i = 0;i<threads->threadsize;i++)
    {
        if(threads->threadarr[i].status ==THREAD_STATU_CANUSE)
            return i;
    }
    return -1;
}
static void* __thread_wait(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&threads->diethreadMutex);
        while(threads->diethread == 0)
            pthread_cond_wait(&threads->diethreadCond,&threads->diethreadMutex);
        pthread_mutex_lock(&threads->threadarrMutex);
        for(int i = 0;i<threads->threadsize;i++)
        {
            if(threads->threadarr[i].status == THREAD_STATU_EXIT)
            {
                threads->threadarr[i].status = THREAD_STATU_CANUSE;
                pthread_join(threads->threadarr[i].pid,NULL);
            }
        }
        pthread_mutex_unlock(&threads->threadarrMutex);
        pthread_mutex_unlock(&threads->diethreadMutex);
    }
    pthread_exit(NULL);
}
int initThreadAdmin()
{
    threads = (threadarr_t*)malloc(sizeof(threadarr_t));
    threads->threadsize = atoi(INITTHHREADNUM);
    threads->threadarr = (threadNode_t*)malloc(sizeof(threadNode_t)*(threads->threadsize));
    threads->threadnum  = 0;
    threads->diethread =  0;
    pthread_mutex_init(&threads->threadarrMutex,NULL);
    pthread_mutex_init(&threads->diethreadMutex,NULL);
  
    pthread_cond_init(&threads->diethreadCond,NULL);

    if(threads->threadarr == NULL)
        return  ENOMEM;
    int ret = pthread_create(&threads->wait_thread_pid,NULL,__thread_wait,NULL); 
    if(ret != 0)    return ret;
    return 0;
}

void destoryThreadAdmin()
{
    pthread_cancel(threads->wait_thread_pid);
    pthread_join(threads->wait_thread_pid,NULL);
    pthread_mutex_lock(&threads->threadarrMutex);
    for(int i  = 0;i<threads->threadsize;i++)
    {
        thread_statu_t sta = threads->threadarr[i].status;
        switch (sta)
        {
            case THREAD_STATU_CANUSE:        
                break;
            case THREAD_STATU_BUSY:
                pthread_cancel(threads->threadarr[i].pid);
            case THREAD_STATU_EXIT:
                pthread_join(threads->threadarr[i].pid,NULL);
            default:
                break;
        }
    }
    pthread_mutex_unlock(&(threads->threadarrMutex));
    free(threads->threadarr);
    free(threads);
    threads = NULL;
}

int addthread(threadFunc func,thread_args_pt args)
{
    pthread_t tid;
    pthread_mutex_lock(&threads->threadarrMutex);
    int pos = findaFreePosUnlock();
    if(pos  == -1)
    {
        pos = threadRealoocUnlock();
        if(pos == -1)
        {
            pthread_mutex_unlock(&threads->threadarrMutex);
            return ENOMEM;
        }
    }
    if(args != NULL)
    {
        args->threadid = pos;
    }
    int ret = pthread_create(&tid,NULL,func,args);
    if(ret)
        return ret;
    threads->threadarr[pos].pid = tid;
    threads->threadarr[pos].status = THREAD_STATU_BUSY;
    pthread_mutex_unlock(&threads->threadarrMutex);
    return 0;
}

int delthread(int threadid)
{
    if(threadid <0 || threadid >= threads->threadsize)
        return EINVAL;
    pthread_mutex_lock(&threads->threadarrMutex);

    if(threads->threadarr[threadid].status != THREAD_STATU_BUSY)
    {
        pthread_mutex_unlock(&threads->threadarrMutex);
        return EINVAL;
    }
    threads->threadarr[threadid].status = THREAD_STATU_EXIT;
    pthread_cancel(threads->threadarr[threadid].pid);
    pthread_mutex_unlock(&threads->threadarrMutex);
    return 0;
}
