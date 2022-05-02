#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include<list>
#include<queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <mysql/mysql.h>
#include <arpa/inet.h>
#include <cstring>
#include "proto.h"
#include "Tools/mydb.h"
// #include "Tools/ThreadAdmin.h"
#include "Model/UserInfo.h"
#include "Model/MessageTable.h"
#include "DAO/MessageTableDao.h"
#include "Service/myService.h"
// #include "Tools/Dlog.h"
#include "Tools/mlog.h"
#include "Tools/Tpool/threadpool.h"
#include "Tools/tools.h"
#include "Tools/epoll.h"
#include "Tools/Redis/redis.h"
#include<iostream>
using namespace std;
#define USERKILL SIGUSR1
using std::list;
using std::queue;
int sd;
int  wtid;
typedef struct user{
    int clientsd;
    UserInfo_Ptr userinfo;
    user(int clientsd,UserInfo_Ptr uinfo = NULL):clientsd(clientsd),userinfo(uinfo){}
}user_t;
list<user_t> ulist;
pthread_mutex_t list_muetex = PTHREAD_MUTEX_INITIALIZER;
queue<MessageTable_Ptr> msgq;
pthread_mutex_t msgq_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  msgq_cond  = PTHREAD_COND_INITIALIZER;
int epfd;
threadpool* tp;
bool die = false;
using namespace my;
void* wirte_thread(void* arg)  //这个线程负责去通知用户有消息到来。
{
    while(!die)
    {
        MessageTable_Ptr now = nullptr;
        pthread_mutex_lock(&msgq_mutex);
        while(msgq.empty() && die == false)
            pthread_cond_wait(&msgq_cond,&msgq_mutex);
        
        if(die) 
        {
            pthread_mutex_unlock(&msgq_mutex);
            return NULL;
        }
        now = msgq.front();
        msgq.pop();
        pthread_mutex_unlock(&msgq_mutex);     
        //处理消息
        pthread_mutex_lock(&list_muetex);
        phase_msg pmsg;
        switch (now->getMtype())
        {
            case MSG_SENDMSG:
            case MSG_UPFILE:
            {
                auto it = ulist.begin();
                while(it != ulist.end())
                {
                    auto user = *it;
                    if(user.userinfo!=nullptr && user.userinfo->getUid() == now->getToid())
                    {
                        //发送消息
                        char msg[1024];
                        int len = now->tomsg(msg);
                        int res = pmsg.sedMsg(user.clientsd,now->getMtype(),len,msg);
                        ILOG("sendmsg: %s",msg);
                        now->setMStatus(1);
                        MessageTableDao::doUpdate(now);
                        if(res < 0)
                        {
                            it = ulist.erase(it);
                            continue;
                        }
                        else
                        {
                            delete now;
                        }
                        break;
                    }
                    it++;
                }
                break;
            }
            case MSG_GRUOPMSG:
            {
                vector<GroupFriend_Ptr> grps;
                GroupFriendDao::doSelectBygid(grps,now->getToid());
                for(auto grp:grps)
                {
                    auto it = ulist.begin();
                    while(it != ulist.end())
                    {
                        auto user = *it;
                        if(user.userinfo->getUid() == grp->getGuid() && user.userinfo->getUid() != now->getUid())
                        {
                            char msg[1024];
                            int len = now->tomsg(msg);
                            int res = pmsg.sedMsg(user.clientsd,now->getMtype(),len,msg);
                            if(res  < 0 )
                            {
                                it = ulist.erase(it);
                                continue;
                            }
                            else
                            {
                                ILOG("sendmsg:%s",msg);
                            }
                        }
                        it++;
                    }
                }
                delete now;
                GroupFriendDao::FreeVector(grps);
                break;
            }
            default:break;
        }
        pthread_mutex_unlock(&list_muetex);
    }    

    return NULL;
} 

class write_job:public worker{
public:
    write_job(){};
    void run()
    {
        wirte_thread(NULL);
    }
};
void* job_thread(int  fd)
{
    /*开始读入数据*/
    phase_msg pmsg;
    if(pmsg.recMsg(fd) <= 0)
    {
        DLOG("fd:%d closed",fd);
        del_fd(epfd,fd);
        close(fd);
        return NULL;
    }
    switch (pmsg.msg_type())
    {
        case MSG_LOGIN:
        {
            bool res = loginService(pmsg,fd);
            if(res)
            {
                UserInfo_Ptr uinfo = new UserInfo();
                uinfo->getObj(pmsg);
                pthread_mutex_lock(&list_muetex);
                ulist.push_back(user_t(fd,uinfo));
                pthread_mutex_unlock(&list_muetex);
            }
            else
            {
                del_fd(epfd,fd);
                close(fd);
                return NULL;
            }
            break;
        }
        case MSG_REGISTER:
            registerService(pmsg,fd);
            break;
        case MSG_FIRENDLIST:
        {
            getFriendList(pmsg,fd);
            break;
        }
        case MSG_GROUPLIST:
        {
            getGroupList(pmsg,fd);
            break;
        }
        case MSG_ADDFRIEND:
        {
            addFriend(pmsg,fd);
            break;
        }
        case MSG_ADDGRUOP:
        {
            addGroup(pmsg,fd);
            break;
        }
        case MSG_BULIDGROUP:
        {
            BulidGroup(pmsg,fd);
            break;
        }
        case MSG_SENDMSG:
        case MSG_GRUOPMSG:
        {
            MessageTable msgt;
            msgt.getObj(pmsg);
            char time[30];
            getTime(time);
            msgt.setCtime(time);
            msgt.setMStatus(0);
            MessageTableDao::doInsert(&msgt);
            MessageTableDao::doSelectMsgId(&msgt);
            char msgid[10];
            sprintf(msgid,"%d",msgt.getMsgid());
            pmsg.sedStatusMsg(fd,msgid);
            pthread_mutex_lock(&msgq_mutex);
            msgq.push(new MessageTable(msgt));
            pthread_mutex_unlock(&msgq_mutex);
            pthread_cond_broadcast(&msgq_cond);
            break;
        }
        case MSG_RECMSG:
        {
            recvAllMsg(pmsg,fd);
            break;
        }
        case MSG_UPFILE:
        {
            Friends frd;
            frd.getObj(pmsg);
            string fileName;
            pmsg.findKey("fileName",fileName);
            fileName = fileName.substr(fileName.find_last_of("/") + 1,fileName.size());
            char perf[30];
            sprintf(perf,"./SendFile/%dto%d",frd.getUid(),frd.getFuid());
            fileName = perf+ fileName;
            ILOG("filename:%s",fileName.c_str());
            char buff[1024]="";
            int wfd = open(fileName.c_str(),O_WRONLY|O_CREAT,0666);
            if(wfd < 0)
            {
                ELOG(strerror(errno));
                break;
            }
            ssize_t len;
            while ((len = readBuff(fd,buff,1024))>0)
            {
                if(len < 0) ELOG("upfile:read error");
                writeBuff(wfd,buff,len);
                fflush(NULL);
            }
            ILOG("up success");
            close(wfd);
            
            //发送消息
            MessageTable mesg;
            char timer[30];
            getTime(timer);
            mesg.setCtime(timer);
            mesg.setMData(fileName.substr(fileName.find_last_of("/") + 1,fileName.size()));
            mesg.setMtyep(MSG_UPFILE);
            mesg.setUid(frd.getUid());
            mesg.setToid(frd.getFuid());
            MessageTableDao::doInsert(&mesg);
            MessageTableDao::doSelectMsgId(&mesg);
            char msgid[10];
            sprintf(msgid,"%d",mesg.getMsgid());
            pmsg.sedStatusMsg(fd,msgid);
            pthread_mutex_lock(&msgq_mutex);
            msgq.push(new MessageTable(mesg));
            pthread_mutex_unlock(&msgq_mutex);
            pthread_cond_broadcast(&msgq_cond);
            break;
        }
        case MSG_DOWNFILE:
        {
            downFile(pmsg,fd);
            close(fd);
            del_fd(epfd,fd);
            return NULL;
            break;
        }
        case MSG_EXIT:
        {
            int uid;
            Redis redis;
            pmsg.findInt("uid",uid);
            pthread_mutex_lock(&list_muetex);
            for(auto it = ulist.begin();it != ulist.end();it++)
            {
                auto user = *it;
                if(user.userinfo->getUid() == uid)
                {
                    ILOG("user is leave uid:%d and fd:%d",uid,user.clientsd);  
                    if(redis.connect())
                    {
                        redis.setbit("user",uid,0);
                        redis.close();
                    } 
                    else
                        WLOG("Redis Connect Error");
                    del_fd(epfd,user.clientsd);
                    close(user.clientsd);
                    delete user.userinfo;
                    ulist.erase(it);
                    break;
                }
            }
            pthread_mutex_unlock(&list_muetex);
            break;
        }
        case MSG_DELFRIEND:
            delFriend(pmsg,fd);
            break;
        default:
            break;
    }
    motify_fd(epfd,fd,EPOLLIN|EPOLLRDHUP|EPOLLET|EPOLLONESHOT);
    return NULL;
}
class read_job:public worker{
    int fd;
public:
    read_job(int fd = 0):fd(fd){}
    void run()
    {
        job_thread(fd);
    }
};
void initdaem()
{
    pid_t pid= fork();
    if(pid < 0)
    {
        DLOG("init:fork");
        exit(1);
    }
    if(pid != 0) exit(0);

    close(0);
    close(1);
    close(2);

    int fd = open("/dev/null",O_RDWR);
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if(fd>2)    
        close(fd);
    setsid();
    umask(7);
    //chdir("/home/chat/server");
    return ;
}
void sig_handle(int s)
{
    switch (s)
    {
  
        case USERKILL:
            die = true;
            close(sd);
            pthread_cond_broadcast(&msgq_cond);
            // tp->destory();
            DLOG("server exit!");
            mlog::getInstance()->destory();
            for(auto user:ulist)
            {
                del_fd(epfd,user.clientsd);
                close(user.clientsd);
                delete user.userinfo;
            }
            exit(0);
            break;
        default:
            break;
    }
}
void rejectFunc(worker *worker)
{
    ILOG("queue full");
    worker->run();
    return ;
}
int main(int argc,char** argv)
{
    initdaem();  //创建守护进程s
    signal(USERKILL,sig_handle);
    tp = threadpool::getPool(5,10,20,1000,rejectFunc);
    mlog::init("./log");
    sd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in laddr;
    laddr.sin_family =AF_INET;
    laddr.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET,SERVER,&laddr.sin_addr);
    int flag = 1;
    if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag))<0)
    {
        perror("setsockopt:");
        exit(0);
    }
    bind(sd,(sockaddr *)(&laddr),sizeof(laddr));
    listen(sd,200);

    if(tp == nullptr)
    {
        ELOG("iniThreadpool error\n");
        exit(1);
    }
    epfd = epoll_create(5);
    tp->excute(shared_ptr<worker>(new write_job()));
    mkdir("SendFile",0777);
    mkdir("log",0777);
    epoll_event events[MAX_SOCKET_FD];
    add_fd(epfd,sd,EPOLLIN|EPOLLET);
    int fd_len = 1;
    while(1)
    {
        int fds = epoll_wait(epfd,events,fd_len,-1);
        if(fds == -1)
        {
            if(errno == EINTR|| errno==EAGAIN)
                continue;
            ELOG("epoll_wait:%s",strerror(errno));
            exit(1);
        }
        for(int i = 0;i<fds;i++)
        {
            int fd = events[i].data.fd; 
            if(fd == sd)
            {
                struct sockaddr_in raddr;
                socklen_t len = sizeof(raddr);
                int clientsd = accept(sd,(sockaddr*)&raddr,&len);
                DLOG("add a client:%d",clientsd);
                add_fd(epfd,clientsd,EPOLLIN|EPOLLRDHUP|EPOLLET|EPOLLONESHOT);
                fd_len++;
            }
            else
            {
                if(events[i].events & EPOLLIN)
                {
                    DLOG("fd%d:deal with in server",fd);
                    tp->excute(shared_ptr<read_job>(new read_job(fd)));
                }
                else if(events[i].events & (EPOLLRDHUP))
                {
                    DLOG("fd:%d closed");
                    del_fd(epfd,fd);
                    close(fd);
                }
            }
        }
    }
}