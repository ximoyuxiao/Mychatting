#include "recivethread.h"
#include "../Tools/phase_msg.h"
#include <pthread.h>
#include <algorithm>
#include<unistd.h>
#include<iostream>
pthread_mutex_t ReciveThread::fri_mutex    =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ReciveThread::grp_mutex    =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ReciveThread::friMap_mutex =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ReciveThread::grpMap_mutex =PTHREAD_MUTEX_INITIALIZER;
ClientSocket* ReciveThread::cs = nullptr;
map<int,vector<MessageTable> > ReciveThread::FriendMsg = map<int,vector<MessageTable> >();
map<int,vector<MessageTable> > ReciveThread::GroupMsg = map<int,vector<MessageTable> >();
map<int,int> ReciveThread::friMap = map<int,int>();
map<int,int> ReciveThread::grpMap = map<int,int>();
ClientSocket ReciveThread::wcs[] ={};
using std::pair;
using std::sort;
using std::cin;
using std::cout;
using std::endl;
ReciveThread::ReciveThread(ClientSocket *cs)
{

}
void* ReciveThread::recThread(void* args)
{
    ClientSocket *cs = (ClientSocket*)args;
    int fd = cs->sfd;
    phase_msg pmsg;
    while (true)
    {
        pmsg.recMsg(fd);
        MessageTable msgt;
        msgt.getObj(pmsg);

        switch (msgt.getMtype())
        {

            case MSG_SENDMSG:
            case MSG_UPFILE:
            {
                pthread_mutex_lock(&fri_mutex);
                if(FriendMsg.find(msgt.getUid()) == FriendMsg.end())
                    FriendMsg[msgt.getUid()] = vector<MessageTable>();
                vector<MessageTable>&temp =  FriendMsg[msgt.getUid()];
                temp.push_back(msgt);
                pthread_mutex_unlock(&fri_mutex);

                pthread_mutex_lock(&friMap_mutex);
                if(friMap.find(msgt.getUid()) == friMap.end())
                    friMap[msgt.getUid()] = 1;
                else
                    friMap[msgt.getUid()]++;
                pthread_mutex_unlock(&friMap_mutex);
                break;
            }
            case MSG_GRUOPMSG:
            {
                 pthread_mutex_lock(&grp_mutex);
                 GroupMsg[msgt.getToid()].push_back(msgt);
                 pthread_mutex_unlock(&grp_mutex);

                 pthread_mutex_lock(&grpMap_mutex);
                 if(grpMap.find(msgt.getToid()) == grpMap.end())
                     grpMap[msgt.getToid()] = 1;
                 else
                     grpMap[msgt.getToid()]++;
                 pthread_mutex_unlock(&grpMap_mutex);
                 break;
            }
            default:
                break;
        }
    }
    cs->CloseSocket();
}

ReciveThread::~ReciveThread()
{
    cs->CloseSocket();
    for(int i = 0;i<MAXRECFD;i++)
        wcs[i].CloseSocket();
}
int ReciveThread::getFd()
{
    for(int i = 0;i<MAXRECFD;i++)
    {
        if(wcs[i].Status == SOCKET_STAT_INIT)
        {
            cout<<"user fd:"<<wcs[i].sfd<<endl;
            wcs[i].Status = SOCKET_STAT_EX;
            return ReciveThread::wcs[i].sfd;
        }
    }

    ClientSocket cs;
    cout<<"Create Socket:"<<endl;
    return cs.InitSocket();
}
int  ReciveThread::closeFd(int fd)
{
    cout<<"ret FD:"<<fd<<endl;
    for(int i = 0;i<MAXRECFD;i++)
    {
        if(fd == wcs[i].sfd)
        {
            wcs[i].Status = SOCKET_STAT_INIT;
            return 0;
        }
    }
    close(fd);
    return 0;
}
ReciveThread* ReciveThread::getInstance(ClientSocket* cs)
{
    if(ReciveThread::cs == nullptr)
    {
        for(int i = 0;i<MAXRECFD;i++)
            wcs[i].InitSocket();
        ReciveThread::cs = cs;
        pthread_t tid;
        pthread_create(&tid,NULL,recThread,(void*)cs);
    }

}
void ReciveThread::delInstance()
{
}
bool ReciveThread::getFriendMessage(vector<MessageTable> &msgt,int fuid)
{
    if(FriendMsg.find(fuid) == FriendMsg.end()) return false;
    pthread_mutex_lock(&fri_mutex);
    for(int i = 0;i<FriendMsg[fuid].size();i++)
        msgt.push_back(FriendMsg[fuid][i]);
    pthread_mutex_unlock(&fri_mutex);
    return true;
}
bool ReciveThread::getGropuMessage(vector<MessageTable> &msgt,int gid)
{
    if(GroupMsg.find(gid) == GroupMsg.end()) return false;
    pthread_mutex_lock(&grp_mutex);
    for(int i = 0;i<GroupMsg[gid].size();i++)
        msgt.push_back(GroupMsg[gid][i]);
    pthread_mutex_unlock(&grp_mutex);
    return true;
}
bool ReciveThread::getNewFriendMessage(vector<MessageTable> &msgt,int msgid,int fuid)
{
    bool flag = false;
    if(FriendMsg.find(fuid) == FriendMsg.end())
    {
        return false;
    }
    pthread_mutex_lock(&fri_mutex);
    for(int i = 0;i<FriendMsg[fuid].size();i++)
    {
        if(FriendMsg[fuid][i].getMsgid() > msgid)
        {
            flag = true;
            msgt.push_back(FriendMsg[fuid][i]);
        }
    }
    pthread_mutex_unlock(&fri_mutex);
    sort(msgt.begin(),msgt.end());
    return flag;
}
bool ReciveThread::getNewGroupMessage(vector<MessageTable> &msgt,int msgid,int gid)
{
    bool flag = false;
    if(GroupMsg.find(gid) == GroupMsg.end()) return false;
    pthread_mutex_lock(&grp_mutex);
    for(int i = 0;i<GroupMsg[gid].size();i++)
        if(GroupMsg[gid][i].getMsgid() > msgid)
        {
            flag = true;
            msgt.push_back(GroupMsg[gid][i]);
        }
    pthread_mutex_unlock(&grp_mutex);
    sort(msgt.begin(),msgt.end());
    return flag;
}
bool ReciveThread::getFriList(vector<int> &fids)
{
    pthread_mutex_lock(&friMap_mutex);
    for(pair<int,int> temp:friMap)
    {
        if(temp.second != 0)
            fids.push_back(temp.first);
    }
    pthread_mutex_unlock(&friMap_mutex);
    return true;
}
bool ReciveThread::getGrpList(vector<int> &gids)
{
    pthread_mutex_lock(&grpMap_mutex);
    for(pair<int,int> temp:grpMap)
    {
        if(temp.second != 0)
            gids.push_back(temp.first);
    }
    pthread_mutex_unlock(&grpMap_mutex);
    return true;
}
bool ReciveThread::eraseFriKey(int fid)
{
    pthread_mutex_lock(&friMap_mutex);
    if(friMap.find(fid) != friMap.end())
        friMap[fid] = 0;
    pthread_mutex_unlock(&friMap_mutex);
    return true;
}
bool ReciveThread::eraseGroKey(int gid)
{
    pthread_mutex_lock(&grpMap_mutex);
    if(grpMap.find(gid) != grpMap.end())
        grpMap[gid] = 0;
    pthread_mutex_unlock(&grpMap_mutex);
    return true;
}
