#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMap>
#include "talkwindows.h"
#include <QStandardItemModel>
#include "QTimer"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    TalkWindows* getTalkWindow(QString id);     //通过id 得到聊天窗口

    void insertItem(QString ip,TalkWindows* peerWin);

    void eraseItem(QString ip);



    void changeName(QString);

    void setinfor(QString,QString);        //显示账号和昵称

    void init();     //主窗口初始化或更新
public slots:
    void response_for_communication();       //请求聊天的返回信息

    void response_for_addfriend();          //请求添加好友的返回信息

    void friend_request();           //来自于其他用户的好友请求

    void updateinfor();
private slots:

    void getMessage();          //读取  UDP 信息

    void on_pushButton_3_clicked();

    void select_friend(QModelIndex);      //选择聊天好友

    void Timer_out();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_addfriend_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString,TalkWindows*> id_windows;   //map 容器中存储聊天对象账号和聊天窗口的句柄

    QStandardItemModel *friends_list;      //用于界面显示好友列表

    QStandardItem *online_friends;       //用于界面显示在线好友列表

    QStandardItem *off_line_friends;      //用于界面显示离线好友列表

    QTimer *timer;
};

#endif // MAINWINDOW_H
