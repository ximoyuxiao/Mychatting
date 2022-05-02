#include"phase_msg.h"
#include<stdlib.h>
#include<algorithm>
#include<unistd.h>
#include<iostream>
using std::cout;
using std::min;
using std::endl;
phase_msg::phase_msg()
{
    type = MSG_INIT;
    length = 0;
}

phase_msg::phase_msg(message_ptr msg)
{
    type = msg->type;
    length = msg->length;
    phase(msg->data,m,length);
}

phase_msg::~phase_msg()
{
    if(m.size())
        m.clear();
}

void  phase_msg::setmsg(message_ptr msgptr)
{
    type = msgptr->type;
    length = msgptr->length;
    phase(msgptr->data,m,length);
}

message_ptr phase_msg::getmsg()
{
    message_ptr tmp =  new message_t();
    tmp->length = length;
    tmp->type = type;
    return tmp;   
}

msgType_t phase_msg::msg_type()
{
    return type;
}

int  phase_msg::msg_length()
{
    return length;
}
int phase_msg::phase(const char* data,StringMap &m,int len)
{
    char *value = new char[len + 1];
    char *key  = new char[len + 1];
    int lastpos = 0;
    int kp = 0;
    bool ked = false;
    int vp = 0;
    for(int i = 0;i<len;i++)
    {
        if(ked)
        {
            if(data[i] == '&')
            {
                value[vp] = '\0';
                m[key] = value;
                vp = 0;
                kp = 0;
                ked = false;
                lastpos = i;
            }
            else
            {
                value[vp++] = data[i];
            }
        }
        else
        {
            if(data[i] == '=')
            {
                key[kp] = '\0';
                ked = true;
            }
            else
            {
                key[kp++] = data[i];
            }
        }
    }
    delete [] key;
    delete [] value;
    return lastpos + 1;
}//将msg转换为map
int  phase_msg::recMsg(int fd)
{
    message_ptr msg = new message_t();
    
    char temp[2*MASSAGELEN]="";
    msg->type = MSG_INIT;
    type = MSG_INIT;
    length = 0;
    int len = 0;
    do
    {
        while ( (len = read(fd,msg,sizeof(*msg)) ) < 0)
        {

            if(errno == EAGAIN || errno == EINTR)
                continue;
            return -errno;
        }
        if(len == 0||msg->type == MSG_EOT)
            break;
        type = msg->type;
        strcat(temp,msg->data);
        //printf("%s\n",temp);
        int wlen = phase(temp,m,strlen(temp));
        length += wlen;
        strcpy(temp,temp + wlen);
    }while(1);
    delete msg;
    return length;
}

int  phase_msg::sedMsg(int fd,msgType_t type,int count,char* data)
{
    int pos = 0;
    int len = 0;
    message_ptr msg = new message_t();
    msg->type = type;
    do
    {
        int wcnt = min(count,MASSAGELEN); 
        memcpy(msg->data,data + pos,wcnt);
        while((len = write(fd,msg,sizeof(*msg))) < 0)
        {
            if(errno == EAGAIN || errno == EINTR)
                continue;
            return -errno;
        }
        count -= wcnt;
        pos += wcnt;   
    }while(count > 0);
    sedEoTmsg(fd);
    fflush(NULL);
    delete msg;
    return pos;
}

bool phase_msg::sedEoTmsg(int fd)
{
    message_ptr msgtemp = new message_t();
    msgtemp->type = MSG_EOT;
    msgtemp->length = 0;
    msgtemp->data[0] = '\0';
    ssize_t len = 0;
    while( (len = write(fd,msgtemp,sizeof(*msgtemp))) <0 )
    {
        if(errno == EAGAIN || errno == EINTR)
            continue;
        return false;
    }
    delete msgtemp;
    return true;
}

bool phase_msg::resetMessage()
{
    type = MSG_INIT;
    length = 0;
    m.clear();
    return true;
}

bool phase_msg::findKey(const string key,string &value)
{
    StringMap::iterator ans =  m.find(key);
    if(ans == m.end() )
        return false;
    value = ans->second;
    return true;
}

bool phase_msg::sedStatusMsg(int fd,const char* msg)
{
    char temp[1024];
    sprintf(temp,"status=%s&",msg);
    sedMsg(fd,MSG_RES,strlen(temp),temp);
    return true;
}
bool phase_msg::findInt(const string key,int &value)
{
    StringMap::iterator ans =  m.find(key);
    if(ans == m.end() )
        return false;
    value = atoi(ans->second.c_str());
    return true;
}
