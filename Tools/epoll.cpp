#include"epoll.h"
#include<sys/epoll.h>
void add_fd(int epfd,int fd,int event)
{
    epoll_event ep;
    ep.data.fd = fd;
    ep.events = event;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ep);
}
void del_fd(int epfd,int fd)
{
    epoll_event ep;
    ep.data.fd = fd;
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ep);
}
void motify_fd(int epfd,int fd,int event)
{
    epoll_event ep;
    ep.data.fd = fd;
    ep.events = event;
    epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ep);
}