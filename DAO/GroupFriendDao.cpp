#include"GroupFriendDao.h"
#include"../Tools/mydb.h"
#include"../Tools/mlog.h"
#include<cstdio>
using namespace my;
GroupFriendDao::GroupFriendDao(){}
GroupFriendDao::~GroupFriendDao(){}
int GroupFriendDao::doSelectByuid(vector<GroupFriend_Ptr>& grfs,int uid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
        return -1*stat;
    char sql[1024];
    sprintf(sql,"select gid from GroupFriend where guid = '%d'",uid);
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    if(res == NULL)
        return 1;
    int rows = mysql_num_rows(res);
    if(!rows) return 1;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)))
    {
        GroupFriend_Ptr grpf = new GroupFriend(atoi(row[0]),uid);
        grfs.push_back(grpf);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int GroupFriendDao::doSelectBygid(vector<GroupFriend_Ptr>& grfs,int gid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
        return -1*stat;
    char sql[1024];
    sprintf(sql,"select guid from GroupFriend where gid = '%d'",gid);
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    if(res == NULL)
        return 1;
    int rows = mysql_num_rows(res);
    if(!rows) return 1;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)))
    {
        GroupFriend_Ptr grpf = new GroupFriend(gid,atoi(row[0]));
        grfs.push_back(grpf);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int GroupFriendDao::doInsert(GroupFriend_Ptr grpf)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"insert into GroupFriend values(%d,%d)",grpf->getGid(),grpf->getGuid());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
int GroupFriendDao::doDelete(GroupFriend_Ptr grpf)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"delete from GroupFriend where gid='%d' and guid=%d",grpf->getGid(),grpf->getGuid());
    ILOG("%s",sql);
    int res =  mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}

void GroupFriendDao::FreeVector(vector<GroupFriend_Ptr>& friends)
{
    for(auto frd:friends)
    {
        delete frd;
    }
    friends.clear();
}