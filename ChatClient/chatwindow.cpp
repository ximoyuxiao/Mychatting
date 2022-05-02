#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "recivethread.h"
#include "../Tools/tools.h"
#include <string>
#include <vector>
#include<unistd.h>
#include <iostream>
#include <QFileDialog>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
using std::cout;
using std::endl;
using std::vector;
using std::string;
ChatWindow::ChatWindow(QWidget *parent,Friends frd) :
    QMainWindow(parent),frd(frd),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit(frd.getFname().c_str()));
    this->maxMsgId = 0;
    this->nowpos = -1;
    pthread_create(&tid,NULL,updateMsg,(void*)this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}
struct fileSend{
    char filename[1024];
    Friends frd;
    fileSend(char* filename,Friends frd):frd(frd){
        sprintf(this->filename,"%s",filename);
    }
};
void* ChatWindow::downFile(void* args)
{
    char* filename = (char*)(args);
    int wfd = open(filename,O_WRONLY|O_CREAT,0666);
    phase_msg pmsg;
    ClientSocket cs;
    int rfd = cs.InitSocket();
    char msg[128]="";
    sprintf(msg,"fileName=%s&",filename);
    pmsg.sedMsg(rfd,MSG_DOWNFILE,strlen(msg),msg);
    ssize_t len;
    char buff[1025]="";
    while((len = readBuff(rfd,buff,1024)))
    {
        if(len == -1)  return NULL;
        writeBuff(wfd,buff,len);
    }
    Fclose(wfd);
    cs.CloseSocket();
    delete []filename;
    return NULL;
}

void* ChatWindow::updateMsg(void* args)
{
    ChatWindow* cw = (ChatWindow*)args;
    vector<MessageTable> &cmsgt = cw->msgt;
    phase_msg pmsg;
    int fd = ReciveThread::getFd();
    if(fd == -1)
    {
      /**/
    }
    char data[1024] ="";
    sprintf(data,"isgroup=0&uid=%d&toid=%d&",cw->frd.getUid(),cw->frd.getFuid());
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
        ReciveThread::eraseFriKey(cw->frd.getFuid());
        ReciveThread::getNewFriendMessage(cw->msgt,cw->maxMsgId,cw->frd.getFuid());
        for(int i = cw->nowpos + 1;i<cw->msgt.size();i++)
        {
            cw->nowpos = i;
            if(cw->maxMsgId >= cmsgt[i].getMsgid())  continue;
            cw->maxMsgId =cmsgt[i].getMsgid();
            char uid[10];
            sprintf(uid,"%d:",cmsgt[i].getUid());
            if(cmsgt[i].getMtype() == MSG_SENDMSG)
                cw->ui->listWidget_2->addItem(QString::fromStdString(cmsgt[i].getCtime()) +":" + QString::fromLocal8Bit(uid) + "MSG:\n" +  QString::fromLocal8Bit(cmsgt[i].getMData().c_str()));
            else
            {
                cw->ui->listWidget->addItem(QString::fromLocal8Bit(cmsgt[i].getMData().c_str()));
            }

        }  
        sleep(1);
    }
}

void* ChatWindow::upFile(void* args)
{
   fileSend* fileargs = (fileSend*)(args);
   int fd = open(fileargs->filename,O_RDONLY);
   ClientSocket cs;
   phase_msg pmsg;
   char buff[1025]={0};
   fileargs->frd.tomsg(buff);
   sprintf(buff,"%sfileName=%s&",buff,fileargs->filename);
   printf("%s\n",buff);
   int sd = cs.InitSocket();
   pmsg.sedMsg(sd,MSG_UPFILE,strlen(buff),buff);
   ssize_t len;

   while ((len = readBuff(fd,buff,1024)))
   {
       if(len == -1) break;
       printf("%d\n",len);
       writeBuff(sd,buff,len);
   }
   printf("send OK\n");
   Fclose(fd);
   cs.CloseSocket();
   delete fileargs;
   pthread_exit(NULL);
}

void ChatWindow::on_pushButton_2_clicked()
{
    QString  msgData =ui->textEdit_2->toPlainText();
    string data = msgData.toLocal8Bit().data();
    ui->textEdit_2->clear();
    MessageTable buff = MessageTable();
    char time[30]={0};
    getTime(time);
    buff.setCtime(time);

    buff.setUid(frd.getUid());
    buff.setToid(frd.getFuid());
    buff.setMsgid(0);
    buff.setMData(data);
    buff.setMtyep(MSG_SENDMSG);
    buff.setMStatus(0);
    char msg[256];
    int len =  buff.tomsg(msg);
    int fd = ReciveThread::getFd();
    if(fd == -1)
    {
       /***/
    }
    phase_msg pmsg;
    pmsg.sedMsg(fd,MSG_SENDMSG,len,msg);
    int status;
    pmsg.recMsg(fd);
    pmsg.findInt("status",status);
    buff.setMsgid(status);
    this->msgt.push_back(buff);
    ReciveThread::closeFd(fd);
}

void ChatWindow::on_pushButton_clicked()
{
    char* filename =  QFileDialog::getOpenFileName(this).toLocal8Bit().data();
    pthread_t tid;
    if(strcmp(filename,"") == 0)
        return ;
    fileSend* args = new fileSend(filename,this->frd);
    pthread_create(&tid,NULL,upFile,(void*)args);
    /*感觉这边需要设计一个专门回收线程的。。。*/
    pthread_join(tid,NULL);
    QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("上传成功"));
}

void ChatWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    char *filename = new char[128];
    sprintf(filename,"%d/%s",frd.getUid(),ui->listWidget->item(index.row())->text().toLocal8Bit().data());
    pthread_t tid;
    pthread_create(&tid,NULL,downFile,(void*)filename);
    pthread_join(tid,NULL);
    QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("下载成功"));
    return ;
}

void ChatWindow::closeEvent(QCloseEvent *)
{
    pthread_cancel(tid);
    pthread_join(tid,NULL);
}
