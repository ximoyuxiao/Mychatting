#include"GroupDao.h"
#include"../Tools/mydb.h"
#include"../Tools/mlog.h"
using namespace my;
GroupDao::GroupDao(){}
GroupDao::~GroupDao(){}
int GroupDao::doSelectByGid(Group_Ptr group,int gid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
        return -1*stat;
    char sql[1024];
    sprintf(sql,"select guid,gname from GroupTable where gid = '%d'",gid);
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    if(res == NULL)
        return 1;
    int rows = mysql_num_rows(res);
    if(!rows) return 1;
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    if(row == 0)    return 1;
    group->setGid(gid);
    group->setGuid(atoi(row[0]));
    group->setGname(row[1]);
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int GroupDao::doSelectByUid(vector<Group_Ptr> &groups,int uid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
        return -1*stat;
    char sql[1024];
    sprintf(sql,"select gid,gname from GroupTable where uid = '%d'",uid);
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
        Group_Ptr grp = new Group(uid,atoi(row[0]),row[1]);
        groups.push_back(grp);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int GroupDao::doInsert(Group_Ptr group)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"insert into GroupTable values(null,%d,'%s')",group->getGuid(),group->getGname().c_str());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
int GroupDao::doDelete(int gid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"delete from GroupTable where gid = '%d'",gid);
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
int GroupDao::doSelectgid(Group_Ptr gropu)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select max(gid) from GroupTable where guid = '%d'",gropu->getGuid());
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    gropu->setGid(atoi(row[0]));
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}

void GroupDao::FreeVector(vector<Group_Ptr>& friends)
{
    for(auto frd:friends)
    {
        delete frd;
    }
    friends.clear();
}