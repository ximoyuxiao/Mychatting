#include "clientsocket.h"
#include"../proto.h"
#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
ClientSocket::ClientSocket()
{
    this->Status = SOCKET_STAT_DESTORY;
    this->sfd = -1;
    cnt = 0;
}
int ClientSocket::InitSocket()
{
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port  = htons(atoi(PORT));
    inet_pton(AF_INET,SERVER,&raddr.sin_addr);
    if(connect(sfd,(sockaddr*)&raddr,sizeof(raddr)))
        return -errno;
    this->Status = SOCKET_STAT_INIT;
    return sfd;

}
int ClientSocket::CloseSocket()
{
    close(sfd);
    return 1;

}
int ClientSocket::sendMsg(int len,message_st msg)
{
    int rlen;
    int pos = 0;
    while(pos < len)
    {
        while(rlen = write(sfd,msg + pos,len - pos)<0)
        {
            if(errno == EAGAIN || errno ==EINTR)
                continue;
            return -errno;
        }
        pos += rlen;
    }
    return pos;
}
int ClientSocket::reciveMsg(int &len,message_st recmsg)
{
    while(len = read(sfd,recmsg,MASSAGELEN)<0)
    {
        if(errno == EAGAIN || errno == EINTR)
            continue;
        return -errno;
    }
    return 0;
}
