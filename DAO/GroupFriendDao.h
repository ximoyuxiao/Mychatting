#ifndef GROUP_FRIEND_DAO_H__
#define GROUP_FRIEND_DAO_H__
#include"../Model/GroupFriend.h"
#include<vector>
using std::vector;
class GroupFriendDao
{
public:
    GroupFriendDao();
    ~GroupFriendDao();
    static int doSelectByuid(vector<GroupFriend_Ptr>& grfs,int uid);
    static int doSelectBygid(vector<GroupFriend_Ptr>& grfs,int gid);
    static int doInsert(GroupFriend_Ptr grpf);
    static int doDelete(GroupFriend_Ptr grpf);
    static void FreeVector(vector<GroupFriend_Ptr>& friends);
};
#endif