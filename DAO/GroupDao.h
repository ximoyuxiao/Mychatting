#ifndef GROUP_DAO_H__
#define GROUP_DAO_H__
#include"../Model/Group.h"
#include<vector>
using std::vector;
class GroupDao
{
public:
    GroupDao(/* args */);
    ~GroupDao();
    static int doSelectByGid(Group_Ptr group,int gid);
    static int doSelectByUid(vector<Group_Ptr> &groups,int uid);
    static int doInsert(Group_Ptr group);
    static int doDelete(int gid);
    static int doSelectgid(Group_Ptr gropu);
    static void FreeVector(vector<Group_Ptr>& friends);
};
#endif