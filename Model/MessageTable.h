#ifndef MESSAGE_TABLE_H__
#define MESSAGE_TABLE_H__
#include"../proto.h"
#include<string>
#include"../Tools/phase_msg.h"

using std::string;
using std::operator<;
class MessageTable
{
private:
    int msgid;
    int uid;
    int toid;
    msgType_t mtype;
    string mData;
    int mStatus;
    string ctime;
public:
    MessageTable(
        int msgid = 0,
        int uid=0,
        int toid=0,
        msgType_t mtype=MSG_OK,
        string mData="",
        int mStatus=0,
        string ctime = "");
    ~MessageTable();

    void setMsgid(const int msgid);
    void setUid(const int uid);
    void setToid(const int toid);
    void setMtyep(const msgType_t mtype);
    void setMData(const string mData);
    void setMStatus(const int mstatus);
    void setCtime(const string ctime);

    int getMsgid(void);
    int getUid(void);
    int getToid(void);
    msgType_t getMtype(void);
    string getMData(void);
    int getMstatus(void);
    string getCtime(void);
        int tomsg(char* msg);
    void getObj(phase_msg &pmsg);
    bool operator<(const MessageTable other); 
};
typedef MessageTable* MessageTable_Ptr;
#endif
