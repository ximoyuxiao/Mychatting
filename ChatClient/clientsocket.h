#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

typedef void* message_st;
enum Status{
    SOCKET_STAT_DESTORY = 1,
    SOCKET_STAT_INIT,
    SOCKET_STAT_READ,
    SOCKET_STAT_WRITE,
    SOCKET_STAT_FLAG,
    SOCKET_STAT_EX,
    SOCKET_STAT_END
};
class ClientSocket
{
public:
    int sfd;
    int Status;
    int cnt;
public:
    ClientSocket();
    int InitSocket();
    int CloseSocket();
    int sendMsg(int len,message_st msg);
    int reciveMsg(int &len,message_st recmsg);
};

#endif // CLIENTSOCKET_H
