#ifndef USERINFO_H__
#define USERINFO_H__
#include<string>
#include<map>
#include"../Tools/phase_msg.h"
using std::string;
class UserInfo
{
private:
    int uid;
    string pass;
    string uname;
public:
    UserInfo(int uid = 0,string pass="",string uname="");
    ~UserInfo();
    void setUid(const int uid);
    void setPass(const string pass);
    void setUname(const string uname);
    int getUid(void);
    string getPass(void);
    string getUname(void);
    int tomsg(char* msg);
    void getObj(phase_msg &pmsg);

};
typedef UserInfo* UserInfo_Ptr;
#endif