#ifndef MESSAGE_DAO_H__
#define MESSAGE_DAO_H__
#include"../Model/MessageTable.h"
#include<vector>
using std::vector;
class MessageTableDao
{
private:
    /* data */
public:
    MessageTableDao(/* args */);
    ~MessageTableDao();
    static int doInsert(MessageTable_Ptr msgp);
    static int doSelectByuid(vector<MessageTable_Ptr> &msgps,int uid);
    static int doSelectBytoid(vector<MessageTable_Ptr> &msgps,int toid);
    static int doSelectBytuid(vector<MessageTable_Ptr> &msgps,int uid,int toid);
    static int doUpdate(MessageTable_Ptr msgp);
    static int doSelectMsgId(MessageTable_Ptr msgp);
    static int doSelectBygid(vector<MessageTable_Ptr>&msgs,int toid);
    static void FreeVector(vector<MessageTable_Ptr>& friends);
};
#endif
