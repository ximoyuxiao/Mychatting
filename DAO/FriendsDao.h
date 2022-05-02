#ifndef FRIEND_DAO_H__
#define FRIEND_DAO_H__
#include"../Model/Friends.h"
#include<vector>
using std::vector;
class FriendsDao
{
public:
    FriendsDao();
    ~FriendsDao();
    static int doSelectByuid(vector<Friends_Ptr> &friends,int uid);
    static int doInsert(Friends_Ptr frd);
    static int doUpdate(Friends_Ptr frd);
    static int doDelete(Friends_Ptr frd);
    static void FreeVector(vector<Friends_Ptr>& friends);
};
#endif