#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "clientsocket.h"
#include "../Tools/phase_msg.h"
#include "../Model/UserInfo.h"
#include "chatwindow.h"
#include <string>
#include<QMessageBox>
#include "mainwindow.h"
#include "pthread.h"
#include "loginwindow.h"
#include "registerwindows.h"
#include "recivethread.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
using std::string;
LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
void LoginWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="" || ui->lineEdit_2->text().trimmed() == "")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("账号或密码不能为空"));
        return ;
    }
    int uid = ui->lineEdit->text().trimmed().toInt();
    string pass = ui->lineEdit_2->text().trimmed().toStdString();
    UserInfo uinfo(uid,pass,"");
    ClientSocket *cs = new ClientSocket();
    int fd = cs->InitSocket();
    phase_msg pmsg;
    char msg[1024];
    int len = uinfo.tomsg(msg);
    pmsg.sedMsg(fd,MSG_LOGIN,len,msg);
    pmsg.recMsg(fd);
    string status;
    pmsg.findKey("status",status);
    if(status == "ok")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("登录成功"));
        ReciveThread::getInstance(cs);
        MainWindow* mw = new MainWindow(this,uinfo);
        this->hide();
        mw->show();
        char path[24];
        sprintf(path,"%d",uinfo.getUid());
        mkdir(path,0777);
        return ;
    }
    else if(status == "userError")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("账号错误"));
        cs->CloseSocket();

    }
    else if(status== "logined")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("此账号已在其他及其登录"));
        cs->CloseSocket();
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("密码错误"));
        cs->CloseSocket();
    }
}

void LoginWindow::on_lineEdit_2_returnPressed()
{
    this->on_pushButton_clicked();
}

void LoginWindow::on_pushButton_2_clicked()
{
    RegisterWindows* rw = new RegisterWindows(this);
    rw->show();
}
