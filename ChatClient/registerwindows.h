#ifndef REGISTERWINDOWS_H
#define REGISTERWINDOWS_H

#include <QMainWindow>

namespace Ui {
class RegisterWindows;
}

class RegisterWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindows(QWidget *parent = 0);
    ~RegisterWindows();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RegisterWindows *ui;
};

#endif // REGISTERWINDOWS_H
