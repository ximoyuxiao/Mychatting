#include"UserInfoDao.h"
#include"../Tools/mydb.h"
#include"../Tools/mlog.h"
using namespace my;
UserInfoDao::UserInfoDao(){}
UserInfoDao::~UserInfoDao(){}
int UserInfoDao::doSelectByUid(UserInfo_Ptr uinfo,int uid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select upass,uname from UserInfo where uid = '%d'",uid);
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    if(res == NULL) return 1;
    int rows = mysql_num_rows(res);
    if(rows == 0)
        return 1;
    MYSQL_ROW row = mysql_fetch_row(res);
    uinfo->setUid(uid);
    uinfo->setPass(row[0]);
    uinfo->setUname(row[1]);
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
bool UserInfoDao::doInsert(UserInfo_Ptr uinfo)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
        return false;
    char sql[1024];
    sprintf(sql,"insert into UserInfo values ('%d','%s','%s')",uinfo->getUid(),uinfo->getPass().c_str(),uinfo->getUname().c_str());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
bool UserInfoDao::doModify(UserInfo_Ptr uinfo)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
        return false;
    char sql[1024];
    sprintf(sql,"update UserInfo set upass='%s',uname='%s' where uid='%d'",uinfo->getPass().c_str(),uinfo->getUname().c_str(),uinfo->getUid());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}