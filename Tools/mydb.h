#ifndef __MYDB__H__
#define __MYDB__H__
#include<mysql/mysql.h>
int getDatabase(MYSQL* mysql);
void CloseDatabase(MYSQL* mysql,MYSQL_RES* res);
#endif