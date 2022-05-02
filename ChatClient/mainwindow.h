#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Model/UserInfo.h"
#include "../Model/Friends.h"
#include "../Model/GroupFriend.h"
#include "../Model/Group.h"
#include "../Model/MessageTable.h"
#include <QModelIndex>
#include <vector>
using std::vector;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    UserInfo uinfo;
    vector<GroupFriend_Ptr> gfptrs;
    vector<Friends_Ptr> fptr;
    vector<int> flist;
    vector<int> glist;
    bool isflist;
public:
    explicit MainWindow(QWidget *parent = 0,UserInfo uinfo = UserInfo());
    ~MainWindow();
private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);
    void closeEvent(QCloseEvent *);
    void on_listWidget_2_doubleClicked(const QModelIndex &index);

    void on_pushButton_5_clicked();


private:
    void getFriendList();
    void getGroupList();
    static void* recMsgList(void*);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
