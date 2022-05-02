#ifndef PHASE_MSG_H__
#define PHASE_MSG_H__
#include"../proto.h"
#include<map>
#include<string.h>
using std::map;
using std::string;
typedef map<string,string> StringMap;
class phase_msg
{
private:
    msgType_t type;
    int length;  
    StringMap   m;
public:
    phase_msg();
    phase_msg(message_ptr msgptr);
    ~phase_msg();
    int  phase(const char* data,StringMap &m,int len);//将msg转换为map
    //bool createMessage(message_ptr msg,int type,int length,char* data);
public:
    void       setmsg(message_ptr msgptr);
    message_ptr getmsg();
    msgType_t  msg_type();
    int  msg_length();
    int  recMsg(int fd);
    int  sedMsg(int fd,msgType_t type,int length,char* data);
    bool sedEoTmsg(int fd);
    bool sedStatusMsg(int fd,const char* msg);
    bool resetMessage();
    bool findKey(const string key,string &value);
    bool findInt(const string key,int &value);
};
#endif