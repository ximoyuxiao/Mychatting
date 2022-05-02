#include"UserInfo.h"
#include<cstring>
UserInfo::UserInfo(int uid,string pass,string uname):uid(uid),pass(pass),uname(uname){}
UserInfo::~UserInfo(){}
void UserInfo::setUid(const int uid){this->uid = uid;}
void UserInfo::setPass(const string pass){this->pass = pass;}
void UserInfo::setUname(const string uname){this->uname = uname;}
int UserInfo::getUid(void){return uid;}
string UserInfo::getPass(void){return pass;}
string UserInfo::getUname(void){return uname;}
int UserInfo::tomsg(char* msg)
{
    sprintf(msg,"uid=%d&pass=%s&uname=%s&",uid,pass.c_str(),uname.c_str());
    return strlen(msg);
}
void UserInfo::getObj(phase_msg &pmsg)
{
    pmsg.findInt("uid",uid);
    pmsg.findKey("pass",pass);
    pmsg.findKey("uname",uname);
}
