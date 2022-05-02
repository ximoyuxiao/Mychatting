/********************************************************************************
** Form generated from reading UI file 'registerwindows.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWINDOWS_H
#define UI_REGISTERWINDOWS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterWindows
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QMenuBar *menubar;

    void setupUi(QMainWindow *RegisterWindows)
    {
        if (RegisterWindows->objectName().isEmpty())
            RegisterWindows->setObjectName(QString::fromUtf8("RegisterWindows"));
        RegisterWindows->resize(237, 224);
        centralwidget = new QWidget(RegisterWindows);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 150, 141, 25));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 40, 67, 21));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 80, 71, 17));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 110, 71, 17));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(80, 40, 113, 25));
        lineEdit->setMaxLength(10);
        lineEdit_2 = new QLineEdit(centralwidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(80, 80, 113, 25));
        lineEdit_2->setMaxLength(20);
        lineEdit_2->setEchoMode(QLineEdit::Password);
        lineEdit_3 = new QLineEdit(centralwidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(80, 110, 113, 25));
        lineEdit_3->setMaxLength(20);
        RegisterWindows->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RegisterWindows);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 237, 28));
        RegisterWindows->setMenuBar(menubar);

        retranslateUi(RegisterWindows);

        QMetaObject::connectSlotsByName(RegisterWindows);
    } // setupUi

    void retranslateUi(QMainWindow *RegisterWindows)
    {
        RegisterWindows->setWindowTitle(QApplication::translate("RegisterWindows", "MainWindow", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("RegisterWindows", "\346\263\250\345\206\214", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RegisterWindows", "\347\224\250\346\210\267ID:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RegisterWindows", "\347\224\250\346\210\267\345\257\206\347\240\201:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RegisterWindows", "\347\224\250\346\210\267\346\230\265\347\247\260:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RegisterWindows: public Ui_RegisterWindows {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWINDOWS_H
