#include"mydb.h"
static const char *host = "localhost";
static const char *user = "moyutest";
static const char *pass ="199094212";
static const char *db = "Chatting";
static int port = 3306;
int getDatabase(MYSQL* mysql)
{
    mysql_init(mysql);
    if(!mysql_real_connect(mysql,host,user,pass,db,port,NULL,0))
    {
        mysql_query(mysql,"set names utf8");
        return 0;
    }
    return mysql_errno(mysql);
}
void CloseDatabase(MYSQL* mysql,MYSQL_RES* res)
{
    if(res != NULL)
        mysql_free_result(res);
    if(mysql != NULL)
        mysql_close(mysql);
}