#include"GroupFriend.h"
GroupFriend::GroupFriend(int gid,int guid):gid(gid),guid(guid){}
GroupFriend::~GroupFriend(){}
void GroupFriend::setGid(const int gid){this->gid = gid;}
void GroupFriend::setGuid(const int guid){this->guid = guid;}
int GroupFriend::getGid(void){return gid;}
int GroupFriend::getGuid(void){return guid;}
int GroupFriend::tomsg(char* msg)
{
    sprintf(msg,"gid=%d&guid=%d&",gid,guid);
    return strlen(msg);
}
void GroupFriend::getObj(phase_msg &pmsg)
{
    pmsg.findInt("gid",gid);
    pmsg.findInt("guid",guid);
}