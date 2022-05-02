#ifndef USERINFO_DAO_H__
#define USERINFO_DAO_H__
#include"../Model/UserInfo.h"
#include"../Tools/mydb.h"
class UserInfoDao
{
public:
    UserInfoDao();
    ~UserInfoDao();
    static int doSelectByUid(UserInfo_Ptr uinfo,int uid);
    static bool doInsert(UserInfo_Ptr uinfo);
    static bool doModify(UserInfo_Ptr uinfo);
};
#endif