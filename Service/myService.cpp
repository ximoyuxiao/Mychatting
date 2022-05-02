#include"myService.h"
#include"../Tools/mlog.h"
#include"../Tools/tools.h"
#include "../Tools/Redis/redis.h"
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using std::min;
using std::max;
using namespace my;
bool loginService(phase_msg& pmsg,int fd)
{
    UserInfo uinfo;
    uinfo.getObj(pmsg);
    string pass;
    int uid;
    pmsg.findInt("uid",uid);
    pmsg.findKey("pass",pass);
    if(UserInfoDao::doSelectByUid(&uinfo,uid) == 1)
    {
        pmsg.sedStatusMsg(fd,"userError");
        return false;
    }
    else if(uinfo.getPass() == pass)
    {
        Redis redis;
        if(redis.connect())
        {
            int login = redis.getbit("user",uid);
            if(login)
            {
                pmsg.sedStatusMsg(fd,"logined");
                return false;
            }
            else
                redis.setbit("user",uid,1);
            redis.close();
        }
        else
        {
            WLOG("RedisOpenError");
        }
        pmsg.sedStatusMsg(fd,"ok");
        vector<MessageTable_Ptr> msgt;
        MessageTableDao::doSelectBytoid(msgt,uid);
        for(size_t i = 0;i<msgt.size();i++)
        {
            char msg[1024];
            int len = msgt[i]->tomsg(msg);
            pmsg.sedMsg(fd,msgt[i]->getMtype(),len,msg);
            MessageTableDao::doUpdate(msgt[i]);
        }
        MessageTableDao::FreeVector(msgt);
        return true;
    }
    else
    {
        pmsg.sedStatusMsg(fd,"passError");
        return false;
    }
    return false;
}
void registerService(phase_msg& pmsg,int fd)
{
    UserInfo uinfo;
    uinfo.getObj(pmsg);

    if(UserInfoDao::doInsert(&uinfo))
    {
        pmsg.sedStatusMsg(fd,"ok");
    }
    else
    {
        pmsg.sedStatusMsg(fd,"error");
    }
    return ;
}
void getFriendList(phase_msg& pmsg,int fd)
{
    int uid;
    pmsg.findInt("uid",uid);
    vector<Friends_Ptr> frds;
    FriendsDao::doSelectByuid(frds,uid);
    char msg[128];
    sprintf(msg,"%d",(int)frds.size());
    pmsg.sedStatusMsg(fd,msg);
    for(int i = 0;i<(int)frds.size();i++)
    {
        int len = frds[i]->tomsg(msg);
        pmsg.sedMsg(fd,MSG_RES,len,msg);
    }
    FriendsDao::FreeVector(frds);
    return ;
}
void getGroupList(phase_msg& pmsg,int fd)
{
    int uid;
    pmsg.findInt("uid",uid);
    vector<GroupFriend_Ptr> gfs;
    GroupFriendDao::doSelectByuid(gfs,uid);
    char msg[128];
    sprintf(msg,"%d",(int)gfs.size());
    pmsg.sedStatusMsg(fd,msg);
    for(int i = 0;i<(int)gfs.size();i++)
    {
        int len = gfs[i]->tomsg(msg);
        pmsg.sedMsg(fd,MSG_RES,len,msg);
    }
    GroupFriendDao::FreeVector(gfs);
    return ;
}
void addFriend(phase_msg& pmsg,int fd)
{
    Friends frd;
    frd.getObj(pmsg);
    string fname1;
    string fname2;
    UserInfo uinfo;
    if(frd.getUid() == frd.getFuid())
    {
        pmsg.sedStatusMsg(fd,"error");
        return ;
    }
    UserInfoDao::doSelectByUid(&uinfo,frd.getUid());
    fname1 = uinfo.getUname();

    UserInfoDao::doSelectByUid(&uinfo,frd.getFuid());
    fname2 = uinfo.getUname();
    frd.setFname(fname2);
    if(FriendsDao::doInsert(&frd))
    {
        int uid = frd.getUid();  
        frd.setUid(frd.getFuid());
        frd.setFuid(uid);
        frd.setFname(fname1);
        FriendsDao::doInsert(&frd);
        pmsg.sedStatusMsg(fd,"ok");
    }
    else
        pmsg.sedStatusMsg(fd,"error");
}
void addGroup(phase_msg& pmsg,int fd)
{
    GroupFriend grpf;
    grpf.getObj(pmsg);
    if(GroupFriendDao::doInsert(&grpf))
        pmsg.sedStatusMsg(fd,"ok");
    else
        pmsg.sedStatusMsg(fd,"error");

}
void BulidGroup(phase_msg&pmsg,int fd)
{
    Group grp;
    GroupFriend grpf;
    grp.getObj(pmsg);
    GroupDao::doInsert(&grp);
    GroupDao::doSelectgid(&grp);
    grpf.setGid(grp.getGid());
    grpf.setGuid(grp.getGuid());
    GroupFriendDao::doInsert(&grpf);
    pmsg.sedStatusMsg(fd,"ok");
}
void recvAllMsg(phase_msg&pmsg,int fd)  //接收最近的30条消息
{
    size_t recSize = 30;
    vector<MessageTable_Ptr> msgts;
    MessageTable msgt;
    msgt.getObj(pmsg);
    int isGroup;
    pmsg.findInt("isgroup",isGroup);
    if(!isGroup)
        MessageTableDao::doSelectBytuid(msgts,msgt.getUid(),msgt.getToid());
    else
        MessageTableDao::doSelectBygid(msgts,msgt.getToid());
    char msg[1024];
    size_t tt = recSize;
    if(msgts.size()<recSize)
        tt = msgts.size();
    sprintf(msg,"%lu",tt);
    pmsg.sedStatusMsg(fd,msg);
    int i = 0;
    if(msgts.size() > recSize)
        i = msgts.size() - recSize;
    for(;i < (int)msgts.size() ; i++)
    {
        int len = msgts[i]->tomsg(msg);
        pmsg.sedMsg(fd,MSG_RES,len,msg);
        if(msgts[i]->getMstatus() == 0)
        {
            msgts[i]->setMStatus(1);
            MessageTableDao::doUpdate(msgts[i]);
        }
    }
    MessageTableDao::FreeVector(msgts);
    return ;
}
void downFile(phase_msg&pmsg,int fd)    //展示最近可以下载文件
{
    
    string fileName;
    pmsg.findKey("fileName",fileName);
    fileName = "./SendFile/" + fileName.substr(fileName.find_last_of("/") + 1,fileName.size());
    ILOG("downFile:%s",fileName.c_str());
    int rfd = open(fileName.c_str(),O_RDONLY);
    off_t size = getFileSize(rfd);
    char status[10];
    sprintf(status,"%ld",size);
   // pmsg.sedStatusMsg(fd,status);
    ssize_t len;
    char buff[1024]="";
    while ((len = readBuff(rfd,buff,1024)))
    {
        if(len == -1)   {ELOG("downFile:error");return ;}
        writeBuff(fd,buff,len);
        fflush(NULL);
    }
    ILOG("downFile:success");
    close(rfd);
}

void delFriend(phase_msg& pmsg,int fd)
{
    Friends frd;
    frd.getObj(pmsg);
    FriendsDao::doDelete(&frd);
    Friends frdd(frd.getFuid(),frd.getUid());
    FriendsDao::doDelete(&frdd);
}