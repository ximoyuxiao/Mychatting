#ifndef RECIVETHREAD_H
#define RECIVETHREAD_H
#include <map>
#include <pthread.h>
#include <vector>
#include "../Model/MessageTable.h"
#include "clientsocket.h"
using std::map;
using std::vector;
#define MAXRECFD 2
class ReciveThread
{
private:
    static ClientSocket  wcs[MAXRECFD];
    static ClientSocket* cs;
    static map<int,vector<MessageTable> > FriendMsg;
    static map<int,vector<MessageTable> > GroupMsg;
    static map<int,int> friMap;
    static map<int,int> grpMap;
    static pthread_mutex_t fri_mutex;
    static pthread_mutex_t grp_mutex;
    static pthread_mutex_t friMap_mutex;
    static pthread_mutex_t grpMap_mutex;
    static void* recThread(void* args);
    ReciveThread(ClientSocket* cs);
    ~ReciveThread();
public:
    static int getFd();
    static int closeFd(int fd);
    static ReciveThread* getInstance(ClientSocket* cs);
    static void delInstance();
    static bool getFriendMessage(vector<MessageTable> &msgt,int fuid);
    static bool getGropuMessage(vector<MessageTable> &msgt,int gid);
    static bool getNewFriendMessage(vector<MessageTable> &msgt,int msgid,int fuid);
    static bool getNewGroupMessage(vector<MessageTable> &msgt,int msgid,int fuid);
    static bool getFriList(vector<int> &fids);
    static bool getGrpList(vector<int> &gids);
    static bool eraseFriKey(int fid);
    static bool eraseGroKey(int gid);

};

#endif // RECIVETHREAD_H
