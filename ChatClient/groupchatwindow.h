#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QMainWindow>
#include "../Model/GroupFriend.h"
#include "../Model/MessageTable.h"
#include <vector>
using std::vector;
namespace Ui {
class GroupChatWindow;
}

class GroupChatWindow : public QMainWindow
{
    Q_OBJECT
private:
    GroupFriend grf;
    vector<MessageTable> msgt;
    int nowpos;
    int maxMsgId;
    pthread_t tid;
    void getgroupsFriendsList();
public:
    explicit GroupChatWindow(QWidget *parent = 0,GroupFriend grf = GroupFriend());
    static void* updateMsg(void* args);
    ~GroupChatWindow();

private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *);
private:
    Ui::GroupChatWindow *ui;
};

#endif // GROUPCHATWINDOW_H
