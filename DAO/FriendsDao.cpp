#include"FriendsDao.h"
#include"../Tools/mydb.h"
#include"../Tools/mlog.h"
using namespace my;
FriendsDao::FriendsDao(){}
FriendsDao::~FriendsDao(){}
int FriendsDao::doSelectByuid(vector<Friends_Ptr> &friends,int uid)
{
    mlog::getInstance();
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select fuid,funame from Friends where uid = '%d'",uid);
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    if(res == NULL)
    {
        return 1;
    }
    int rows = mysql_num_rows(res);
    if(!rows) return 1;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)))
    {
        Friends_Ptr frd = new Friends(uid,atoi(row[0]),row[1]);
        friends.push_back(frd);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int FriendsDao::doInsert(Friends_Ptr frd)
{
    mlog::getInstance();
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"insert into Friends values(%d,%d,'%s')",frd->getUid(),frd->getFuid(),frd->getFname().c_str());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
int FriendsDao::doUpdate(Friends_Ptr frd)
{
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"update Friends set funame = '%s' where uid=%d and fuid='%d'",frd->getFname().c_str(),frd->getUid(),frd->getFuid());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
int FriendsDao::doDelete(Friends_Ptr frd)
{
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"delete from Friends where uid=%d and fuid=%d",frd->getUid(),frd->getFuid());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}

void FriendsDao::FreeVector(vector<Friends_Ptr>& friends)
{
    for(auto frd:friends)
    {
        delete frd;
    }
    friends.clear();
}