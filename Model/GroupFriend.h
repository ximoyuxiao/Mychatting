#ifndef GROUP_FRIEND_H__
#define GROUP_FRIEND_H__
#include"../Tools/phase_msg.h"
class GroupFriend
{
private:
    int gid;
    int guid;
public:
    GroupFriend(int gid=0,int guid=0);
    ~GroupFriend();
    void setGid(const int gid);
    void setGuid(const int guid);
    int getGid(void);
    int getGuid(void);
    int tomsg(char* msg);
    void getObj(phase_msg &pmsg);
};
typedef GroupFriend* GroupFriend_Ptr;
#endif