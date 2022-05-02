/********************************************************************************
** Form generated from reading UI file 'groupchatwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPCHATWINDOW_H
#define UI_GROUPCHATWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupChatWindow
{
public:
    QWidget *centralwidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QListWidget *listWidget;
    QTextEdit *textEdit;
    QPushButton *pushButton;
    QMenuBar *menubar;

    void setupUi(QMainWindow *GroupChatWindow)
    {
        if (GroupChatWindow->objectName().isEmpty())
            GroupChatWindow->setObjectName(QString::fromUtf8("GroupChatWindow"));
        GroupChatWindow->resize(594, 516);
        centralwidget = new QWidget(GroupChatWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 601, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 599, 319));
        listWidget = new QListWidget(scrollAreaWidgetContents);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 591, 321));
        scrollArea->setWidget(scrollAreaWidgetContents);
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(0, 320, 481, 171));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(480, 430, 121, 61));
        GroupChatWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GroupChatWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 594, 28));
        GroupChatWindow->setMenuBar(menubar);

        retranslateUi(GroupChatWindow);

        QMetaObject::connectSlotsByName(GroupChatWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GroupChatWindow)
    {
        GroupChatWindow->setWindowTitle(QApplication::translate("GroupChatWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("GroupChatWindow", "\345\217\221\351\200\201\346\266\210\346\201\257", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GroupChatWindow: public Ui_GroupChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPCHATWINDOW_H
