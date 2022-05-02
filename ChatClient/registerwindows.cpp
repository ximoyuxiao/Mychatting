#include "registerwindows.h"
#include "ui_registerwindows.h"
#include "clientsocket.h"
#include "../Tools/phase_msg.h"
#include "../Model/UserInfo.h"
#include <QMessageBox>

RegisterWindows::RegisterWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindows)
{
    ui->setupUi(this);
}

RegisterWindows::~RegisterWindows()
{
    delete ui;
}

void RegisterWindows::on_pushButton_clicked()
{
    if(ui->lineEdit->text().trimmed()=="" || ui->lineEdit_2->text().trimmed()==""|| ui->lineEdit_3->text().trimmed() =="")
    {
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("请输入注册信息"));
        return ;
    }
    int uid = ui->lineEdit->text().trimmed().toInt();
    string pass = ui->lineEdit_2->text().trimmed().toStdString();
    string uname = ui->lineEdit_3->text().trimmed().toLocal8Bit().data();
    ClientSocket cs;
    int fd = cs.InitSocket();
    UserInfo uinfo(uid,pass,uname);
    phase_msg pmsg;
    char msg[1024];
    int len = uinfo.tomsg(msg);
    pmsg.sedMsg(fd,MSG_REGISTER,len,msg);
    pmsg.recMsg(fd);
    string status;
    pmsg.findKey("status",status);
    if(status == "ok")
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("注册成功"));
    else
        QMessageBox::information(this,QString::fromLocal8Bit("墨羽提示"),QString::fromLocal8Bit("注册失败"));
    cs.CloseSocket();
    return ;
}
