#include"MessageTableDao.h"
#include"../Tools/mydb.h"
#include "../Tools/mlog.h"
using namespace my;
MessageTableDao::MessageTableDao(/* args */){}
MessageTableDao::~MessageTableDao(){}
int MessageTableDao::doInsert(MessageTable_Ptr msgp)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"insert into MessageTable values (null,%d,%d,%d,'%s',%d,'%s')",
    msgp->getUid(),msgp->getToid(),
    msgp->getMtype(),msgp->getMData().c_str(),
    msgp->getMstatus(),msgp->getCtime().c_str());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}
int MessageTableDao::doSelectByuid(vector<MessageTable_Ptr> &msgps,int uid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select msgid,uid,toid,mtype,mData,mStatus,ctime from MessageTable where uid = '%d'",uid);
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
        MessageTable_Ptr mtp = new MessageTable(atoi(row[0]),atoi(row[1]),atoi(row[2]),(msgType_t)atoi(row[3]),row[4],atoi(row[5]),row[6]);
        msgps.push_back(mtp);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int MessageTableDao::doSelectBytoid(vector<MessageTable_Ptr> &msgps,int toid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select msgid,uid,toid,mtype,mData,mStatus,ctime from MessageTable where toid = '%d' and mtype = %d and mStatus = 0",toid,MSG_SENDMSG);
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
        MessageTable_Ptr mtp = new MessageTable(atoi(row[0]),atoi(row[1]),atoi(row[2]),(msgType_t)atoi(row[3]),row[4],atoi(row[5]),row[6]);
        msgps.push_back(mtp);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}

int MessageTableDao::doSelectBygid(vector<MessageTable_Ptr>&msgs,int toid)
{
        
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select msgid,uid,toid,mtype,mData,mStatus,ctime from MessageTable where toid = '%d' and mtype = %d",toid,MSG_GRUOPMSG);
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
        MessageTable_Ptr mtp = new MessageTable(atoi(row[0]),atoi(row[1]),atoi(row[2]),(msgType_t)atoi(row[3]),row[4],atoi(row[5]),row[6]);
        msgs.push_back(mtp);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int MessageTableDao::doSelectBytuid(vector<MessageTable_Ptr> &msgps,int uid,int toid)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,
    "select msgid,uid,toid,mtype,mData,mStatus,ctime from MessageTable where (uid = %d and toid =%d) or (uid = %d and toid = %d) ORDER BY msgid"
    ,uid,toid,toid,uid);
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
        MessageTable_Ptr mtp = new MessageTable(atoi(row[0]),atoi(row[1]),atoi(row[2]),(msgType_t)atoi(row[3]),row[4],atoi(row[5]),row[6]);
        msgps.push_back(mtp);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}
int MessageTableDao::doUpdate(MessageTable_Ptr msgp)
{
    
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"update MessageTable set mStatus=1 where msgid = %d",msgp->getMsgid());
    ILOG("%s",sql);
    int res = mysql_query(&mysql,sql) == 0;
    mysql_close(&mysql);
    return res;
}

int MessageTableDao::doSelectMsgId(MessageTable_Ptr msgp)
{
    MYSQL mysql;
    int stat = getDatabase(&mysql);
    if(stat)
    {
        return -1*stat;
    }
    char sql[1024];
    sprintf(sql,"select max(msgid) from MessageTable where uid = %d",msgp->getUid());
    ILOG("%s",sql);
    mysql_query(&mysql,sql);
    MYSQL_RES *res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    msgp->setMsgid(atoi(row[0]));
    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}

void MessageTableDao::FreeVector(vector<MessageTable_Ptr>& friends)
{
    for(auto frd:friends)
    {
        delete frd;
    }
    friends.clear();
}