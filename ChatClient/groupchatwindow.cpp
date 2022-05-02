#include "groupchatwindow.h"
#include "ui_groupchatwindow.h"
#include "recivethread.h"
#include "../Tools/tools.h"
#include <unistd.h>
GroupChatWindow::GroupChatWindow(QWidget *parent,GroupFriend grf) :
    QMainWindow(parent),grf(grf),
    ui(new Ui::GroupChatWindow)
{
    ui->setupUi(this);
    char title[10];
    sprintf(title,"%d",grf.getGid());
    this->setWindowTitle(QString::fromLocal8Bit(title));
    this->maxMsgId = 0;
    this->nowpos = -1;
    //getgroupsFriendsList();
    pthread_create(&tid,NULL,updateMsg,(void*)this);
}

GroupChatWindow::~GroupChatWindow()
{
    delete ui;
}
void* GroupChatWindow::updateMsg(void* args)
{
    GroupChatWindow* cw = (GroupChatWindow*)args;
    vector<MessageTable> &cmsgt = cw->msgt;
    phase_msg pmsg;
    int fd = ReciveThread::getFd();
    char data[1024] ="";
    sprintf(data,"isgroup=1&uid=%d&toid=%d&",cw->grf.getGuid(),cw->grf.getGid());
    pmsg.sedMsg(fd,MSG_RECMSG,strlen(data),data);
    pmsg.recMsg(fd);
    int status = 0;
    pmsg.findInt("status",status);
    for(int i = 0;i<status;i++)
    {
        pmsg.recMsg(fd);
        MessageTable msgt;
        msgt.getObj(pmsg);
        cmsgt.push_back(msgt);
        char msg[1024];
        msgt.tomsg(msg);
        //printf("%s\n",msg);
    }
    pmsg.sedStatusMsg(fd,"ok");
    ReciveThread::closeFd(fd);
    while(true)
    {
        ReciveThread::getNewGroupMessage(cw->msgt,cw->maxMsgId,cw->grf.getGid());
        vector<MessageTable> &cmsgt = cw->msgt;
        for(int i = cw->nowpos + 1;i<cw->msgt.size();i++)
        {
            cw->nowpos = i;
            cw->maxMsgId =cmsgt[i].getMsgid();
            char uid[10];
            sprintf(uid,"%d:",cmsgt[i].getUid());
            cw->ui->listWidget->addItem(QString::fromStdString(cmsgt[i].getCtime()) +":" + QString::fromLocal8Bit(uid) + "MSG:" +  QString::fromLocal8Bit(cmsgt[i].getMData().c_str()));
        }
        sleep(1);
    }
}
void GroupChatWindow::getgroupsFriendsList()
{
    vector<int> uids;
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    char msg[1024]="";
    sprintf(msg,"gid=%d&",grf.getGid());
    pmsg.sedMsg(fd,MSG_FINDGROUPSFRIED,strlen(msg),msg);

    ReciveThread::closeFd(fd);
}
void GroupChatWindow::on_pushButton_clicked()
{
    QString  msgData =ui->textEdit->toPlainText();
    ui->textEdit->clear();
    string data = string(msgData.toLocal8Bit().data());
    MessageTable buff = MessageTable();
    char time[30]={0};
    getTime(time);
    buff.setCtime(time);
    buff.setUid(grf.getGuid());
    buff.setToid(grf.getGid());
    buff.setMsgid(0);
    buff.setMData(data);
    buff.setMtyep(MSG_GRUOPMSG);
    buff.setMStatus(0);
    char msg[1024];
    int len =  buff.tomsg(msg);
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    pmsg.sedMsg(fd,MSG_GRUOPMSG,len,msg);
    int status;
    pmsg.recMsg(fd);
    pmsg.findInt("status",status);
    buff.setMsgid(status);
    this->msgt.push_back(buff);
    ReciveThread::closeFd(fd);
}

void GroupChatWindow::closeEvent(QCloseEvent *)
{
    pthread_cancel(tid);
    pthread_join(tid,NULL);
}
