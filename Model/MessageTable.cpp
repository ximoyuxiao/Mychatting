#include"MessageTable.h"

MessageTable::MessageTable(
        int msgid,
        int uid,
        int toid,
        msgType_t mtype,
        string mData,
        int mStatus,
        string ctime):msgid(msgid),uid(uid),toid(toid),mtype(mtype),mData(mData),mStatus(mStatus),ctime(ctime){
}
MessageTable::~MessageTable(){}

void MessageTable::setMsgid(const int msgid){this->msgid = msgid;}
void MessageTable::setUid(const int uid){this->uid = uid;}
void MessageTable::setToid(const int toid){this->toid = toid;}
void MessageTable::setMtyep(const msgType_t mtype){this->mtype = mtype;}
void MessageTable::setMData(const string mData){this->mData = mData;}
void MessageTable::setMStatus(const int mstatus){this->mStatus = mstatus;}
void MessageTable::setCtime(const string ctime){this->ctime = ctime;}

int MessageTable::getMsgid(void){return msgid;}
int MessageTable::getUid(void){return uid;}
int MessageTable::getToid(void){return toid;}
msgType_t MessageTable::getMtype(void){return mtype;}
string MessageTable::getMData(void){return mData;}
int MessageTable::getMstatus(void){return mStatus;}
string MessageTable::getCtime(void){return ctime;}
int MessageTable::tomsg(char* msg)
{
    sprintf(msg,"msgid=%d&uid=%d&toid=%d&mtype=%d&mData=%s&mStatus=%d&ctime=%s&",msgid,uid,toid,mtype,mData.c_str(),mStatus,ctime.c_str());
    return strlen(msg);
}
void MessageTable::getObj(phase_msg &pmsg)
{
    pmsg.findInt("msgid",msgid);
    pmsg.findInt("uid",uid);
    pmsg.findInt("toid",toid);
    int type;
    pmsg.findInt("mtype",type);
    mtype = (msgType_t)type;
    pmsg.findInt("mStatus",mStatus);
    pmsg.findKey("mData",mData);
    pmsg.findKey("ctime",ctime);
}

bool MessageTable::operator<(const MessageTable other)
{
    return this->msgid < other.msgid;
}