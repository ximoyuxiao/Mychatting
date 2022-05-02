#ifndef __PROTO_H__
#define __PROTO_H__
#define MAX_SOCKET_FD 65535
#define MASSAGELEN 1460
#define SERVER "127.0.0.1"
#define PORT   "3806"
typedef enum{
    MSG_EOT = 0,
    MSG_INIT, 
    MSG_RES,
    MSG_OK,
    MSG_LOGIN,          /*登录 √*/
    MSG_REGISTER,       /*注册 √*/
    MSG_EXIT,           /*注销 √*/
    
    MSG_ADDFRIEND,      /*添加好友 √*/
    MSG_ACADD,          /*同意添加好友 时间原因暂时不实现*/
    MSG_DELFRIEND,      /*删除好友*/
    MSG_FIRENDLIST,     /*好友列表 √*/

    MSG_BULIDGROUP,     /*建立群聊 √*/
    MSG_ADDGRUOP,       /*加入群聊 √*/
    MSG_DELGROUP,       /*退出群聊 时间原因暂时不实现*/
    MSG_GROUPLIST,      /*群聊列表 √*/
    MSG_FINDGROUPSFRIED,/*获取群友列表 √*/

    MSG_SENDMSG,        /*发送消息*/
    MSG_GRUOPMSG,       /*发送群消息*/
    MSG_RECMSG,         /*接收消息*/

    MSG_UPFILE,         /*上传文件*/
    MSG_DOWNFILE,       /*下载文件*/

    MSG_HEART,          /*心跳检测  时间原因暂时不实现*/
    MSG_CHAGSTAT       /*更改自身状态 时间原因暂不实现*/
}msgType_t;
typedef struct message{
    msgType_t type;
    int     length;
    char    data[MASSAGELEN];
}message_t,msg_t,*message_ptr;
#endif
