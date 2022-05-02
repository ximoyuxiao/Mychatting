#ifndef __THREAD_ADMIN_H__
#define __THREAD_ADMIN_H__
#include<pthread.h>

typedef struct Thread_args{
    int threadid;
    int fd;
}thread_args_t;

typedef thread_args_t* thread_args_pt;

typedef void*(threadFunc)(void*) ;
/**
  * function:get a thread admin user 
  * return:
  *     sucess return 0;
  *     faild  return ERRNO  
*/
int initThreadAdmin();
/*
    *function:create a thread.
    args:
        1、a function point void*(threadFunc)(void*);
        2、the function's args
    return
        faild :return -1 and set errno
        success :return a number in stand of the threadnumber;
*/
int addthread(threadFunc,thread_args_pt args);

/*
    function delete a thread (no wait) it's stand of the thread status set THREAD_STATU_EXIT
    args:
        1.a threadid is addthread function ret val
        2.set a thread status (NULL is not get)
    return  
        faild: return -1
        sucess:return 1
*/
int delthread(int threadid);

void destoryThreadAdmin();
#endif