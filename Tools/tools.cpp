#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "tools.h"
#include <errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
void getTime(char* timer)
{
    time_t now;
    time(&now);
    struct tm *phare = localtime(&now);
    sprintf(timer,"%d-%d-%d [%d:%d:%d]",
    phare->tm_year + 1900,
    phare->tm_mon + 1,
    phare->tm_mday,
    phare->tm_hour,
    phare->tm_min,
    phare->tm_sec
    );
    return ;
}
ssize_t readBuff(int fd,void* buff,ssize_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char* bufp = (char*)buff;
    while(nleft > 0)
    {
        if((nread = read(fd,bufp,nleft)) < 0)
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0)
            break;
       nleft -= nread;
       bufp += nread;
    }
    return count - nleft;
}
ssize_t writeBuff(int fd,void* buff,ssize_t count)
{
    size_t nleft = count;
    ssize_t nwritten;
    char* bufp = (char*)buff;
    while(nleft > 0)
    {
        if( (nwritten = write(fd,bufp,nleft)) <0)
        {
            if(errno==EINTR)
                nwritten =  0;
            else
                return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return count;
}
void Fclose(int fd){close(fd);}

off_t getFileSize(int fd)
{
    struct stat fs;
    fstat(fd,&fs);
    return fs.st_size;   
}