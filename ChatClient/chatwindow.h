#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "../Model/Friends.h"
#include "../Model/MessageTable.h"
#include<QModelIndex>
using std::vector;
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT
private:
    Friends frd;
    vector<MessageTable> msgt;
    int nowpos;
    int maxMsgId;
    pthread_t tid;
private:
    static void* downFile(void* args);
    static void* updateMsg(void* args);
    static void* upFile(void* args);
public:
    explicit ChatWindow(QWidget *parent = 0,Friends frd = Friends());
    ~ChatWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void closeEvent(QCloseEvent *);


private:
    Ui::ChatWindow *ui;
};

#endif // CHATWINDOW_H
