#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientsocket.h"
#include "../Tools/phase_msg.h"
#include "chatwindow.h"
#include "groupchatwindow.h"
#include <QMessageBox>
#include "recivethread.h"
#include <unistd.h>
MainWindow::MainWindow(QWidget *parent,UserInfo uinfo) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),uinfo(uinfo)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit(uinfo.getUname().c_str()));
    getFriendList();
    pthread_t tid;
    pthread_create(&tid,NULL,recMsgList,(void*)this);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0;i<this->fptr.size();i++)
    {
        delete fptr[i];
    }
    fptr.clear();
    for(int i = 0;i<this->gfptrs.size();i++)
    {
        delete gfptrs[i];
    }
    gfptrs.clear();
}

void MainWindow::getFriendList()
{
    isflist = true;
    for(int i = 0;i<this->fptr.size();i++)
    {
        delete fptr[i];
    }
    fptr.clear();
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    char msg[1024];
    int len = uinfo.tomsg(msg);
    pmsg.sedMsg(fd,MSG_FIRENDLIST,len,msg);
    pmsg.recMsg(fd);
    int status;
    pmsg.findInt("status",status);
    for(int i = 0;i<status;i++)
    {
        pmsg.recMsg(fd);
        Friends_Ptr frd = new Friends();
        frd->getObj(pmsg);
        fptr.push_back(frd);
    }
    ui->listWidget->clear();
    char fuid[11];
    for(int i = 0;i<fptr.size();i++)
    {
        sprintf(fuid,"%d",fptr[i]->getFuid());
        ui->listWidget->addItem("name:" + QString::fromLocal8Bit(fptr[i]->getFname().c_str()) + "fuid:" + fuid);
    }
    ReciveThread::closeFd(fd);
}
void MainWindow::getGroupList()
{
    isflist = false;
    for(int i = 0;i<this->gfptrs.size();i++)
    {
        delete gfptrs[i];
    }
    gfptrs.clear();
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    char msg[1024];
    int len = uinfo.tomsg(msg);
    pmsg.sedMsg(fd,MSG_GROUPLIST,len,msg);
    pmsg.recMsg(fd);
    int status;
    pmsg.findInt("status",status);
    for(int i = 0;i<status;i++)
    {
        pmsg.recMsg(fd);
        GroupFriend_Ptr grf = new GroupFriend();
        grf->getObj(pmsg);
        this->gfptrs.push_back(grf);
    }
    ui->listWidget->clear();
    char gid[5];
    for(int i = 0;i<gfptrs.size();i++)
    {
        sprintf(gid,"%d",gfptrs[i]->getGid());
        ui->listWidget->addItem(QString("gid:") + gid);
    }
    ReciveThread::closeFd(fd);
}

void MainWindow::on_pushButton_7_clicked()
{
    getFriendList();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->lineEdit->text().trimmed() == "")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("输入不能为空"));
        return ;
    }
    int fuid = ui->lineEdit->text().trimmed().toInt();
    Friends frd;
    frd.setFuid(fuid);
    frd.setUid(uinfo.getUid());
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    char msg[128];
    int len = frd.tomsg(msg);
    pmsg.sedMsg(fd,MSG_ADDFRIEND,len,msg);
    pmsg.recMsg(fd);
    string status;
    pmsg.findKey("status",status);
    if(status == "ok")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("添加成功"));
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("添加失败"));
    }
    getFriendList();
    ReciveThread::closeFd(fd);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ui->lineEdit->text().trimmed() == "")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("输入不能为空"));
        return ;
    }
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    GroupFriend gf;
    gf.setGuid(uinfo.getUid());
    int gid = ui->lineEdit->text().trimmed().toInt();
    gf.setGid(gid);
    char msg[128];
    int len = gf.tomsg(msg);
    pmsg.sedMsg(fd,MSG_ADDGRUOP,len,msg);
    pmsg.recMsg(fd);
    string status;
    pmsg.findKey("status",status);
    if(status == "ok")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("添加成功"));
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("添加失败"));
    }
    getGroupList();
    ReciveThread::closeFd(fd);
}

void MainWindow::on_pushButton_6_clicked()
{
    if(ui->lineEdit->text().trimmed() == "")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("输入不能为空"));
        return ;
    }
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    Group grp;
    grp.setGuid(uinfo.getUid());
    string gname = ui->lineEdit->text().trimmed().toStdString();
    grp.setGname(gname);
    char msg[128];
    int len = grp.tomsg(msg);
    pmsg.sedMsg(fd,MSG_BULIDGROUP,len,msg);
    pmsg.recMsg(fd);
    string status;
    pmsg.findKey("status",status);
    if(status == "ok")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("添加成功"));
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("添加失败"));
    }
    getGroupList();
    ReciveThread::closeFd(fd);
}

void MainWindow::on_pushButton_8_clicked()
{
    getGroupList();
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if(isflist == true)
    {
        ChatWindow *cw = new ChatWindow(this,*fptr[row]);
        cw->show();
    }
    else
    {
        GroupChatWindow *gcw = new GroupChatWindow(this,*gfptrs[row]);
        gcw->show();
    }
}
void MainWindow::closeEvent(QCloseEvent *)
{
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    char msg[120];
    sprintf(msg,"uid=%d&",uinfo.getUid());
    int len = strlen(msg);
    pmsg.sedMsg(fd,MSG_EXIT,len,msg);
    ReciveThread::closeFd(fd);

}

void* MainWindow::recMsgList(void* args)
{
    MainWindow* mw = (MainWindow*)args;

    while(true)
    {
        mw->flist.clear();
        ReciveThread::getFriList(mw->flist);
        mw->ui->listWidget_2->clear();
        for(size_t i = 0;i<mw->flist.size();i++)
        {
            char fid[11] ="";
            sprintf(fid,"%d",mw->flist[i]);
            mw->ui->listWidget_2->addItem(QString::fromLatin1("uid:") + QString::fromLocal8Bit(fid));
        }
        sleep(1);
    }
    return NULL;
}

void MainWindow::on_listWidget_2_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    string uidStr = ui->listWidget_2->item(row)->text().toLocal8Bit().data();
    printf("%s\n",uidStr.c_str());
    int uid = atoi(uidStr.substr(4,uidStr.size()).c_str());
    printf("%d\n",uid);
    Friends frd;
    for(auto temp:fptr)
    {
        if(temp->getFuid() == uid)
        {
            frd = *temp;
            break;
        }
    }
    ChatWindow *cw = new ChatWindow(this,frd);
    cw->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    if(ui->lineEdit->text().trimmed() == "")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("输入不能为空"));
        return ;
    }
    Friends frd;
    frd.setUid(this->uinfo.getUid());
    int fuid = ui->lineEdit->text().toInt();
    frd.setFuid(fuid);
    char msg[64];
    int len = frd.tomsg(msg);
    int fd = ReciveThread::getFd();
    phase_msg pmsg;
    pmsg.sedMsg(fd,MSG_DELFRIEND,len,msg);
    getFriendList();
    ReciveThread::closeFd(fd);
}
