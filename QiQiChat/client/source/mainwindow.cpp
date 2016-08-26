
//登录成功后的主窗口

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat_client.h"
#include "dialog.h"
#include <QMessageBox>
#include <change_name.h>
#include "change_password.h"
#include "add_friend.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket;     //和其他对象通信的 UDP

    udpSocket->bind(PortofClient,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(getMessage()));

    friends_list = new QStandardItemModel();
    friends_list->setHorizontalHeaderLabels(QStringList()<<"账号"<<"昵称");

    ui->treeView_friends->setModel(friends_list);
    ui->treeView_friends->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView_friends->setSelectionBehavior(QAbstractItemView::SelectRows);

    online_friends = new QStandardItem("在线好友");
    friends_list->appendRow(online_friends);


    off_line_friends = new QStandardItem("离线好友");
    friends_list->appendRow(off_line_friends);


    connect(ui->treeView_friends,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(select_friend(QModelIndex)));

    connect(Socket,SIGNAL(ResponseForCommunication()),
            this,SLOT(response_for_communication()));

    connect(Socket,SIGNAL(ResponseForAddfriend()),
            this,SLOT(response_for_addfriend()));

    connect(Socket,SIGNAL(Addrequest()),this,SLOT(friend_request()));

    connect(Socket,SIGNAL(update()),this,SLOT(updateinfor()));

    timer = new QTimer;
    timer->setInterval(5000);

    connect(timer,SIGNAL(timeout()),
            this,SLOT(Timer_out()));

}



void MainWindow::setinfor(QString account,QString name)  //在界面显示当前用户的id和昵称
{
    ui->label_account->setText(account);

    ui->label_name_2->setText(name);
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::getMessage()         //从UDP 中接收信息
{
    while(udpSocket->hasPendingDatagrams())
    {

        QByteArray datagram;
        QHostAddress senderAdress;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size(),&senderAdress);

        QDataStream in(&datagram,QIODevice::ReadOnly);

        QString senderIP = senderAdress.toString();
        QString messageType;
        in>>messageType;


        if(messageType == "MESSAGE")           //接收到的信息类型为“MESSAGE” 时，信息是其他用户的聊天消息，
        {                                     //显示到对应的聊天窗口
            QString id_peer;
            QString name_peer;
            in>>id_peer>>name_peer;

            TalkWindows *receiveWin=getTalkWindow(id_peer);  //获得对应聊天对象的聊天窗口的句柄

            if(receiveWin == NULL)
            {
                receiveWin = new TalkWindows(senderIP,name_peer,true);

                id_windows.insert(id_peer,receiveWin);
            }

            QString message;
            in>>message;

            receiveWin->show();
            receiveWin->getMessage(message);     //聊天窗口显示消息

        }


    }
}



TalkWindows* MainWindow::getTalkWindow(QString id)  //从 id_windows 查找到与 id 相关联的聊天窗口句柄
{
    QMap<QString,TalkWindows*>::const_iterator it = id_windows.find(id);

    if(it != id_windows.end())
        return it.value();
    else
        return NULL;
}



void MainWindow::insertItem(QString id, TalkWindows *peerWin) //向id_windows 插入键值对
{
    id_windows.insert(id,peerWin);
}



void MainWindow::eraseItem(QString id)     //取消键值对
{
    id_windows.remove(id);
}



void MainWindow::updateinfor()       //更新好友信息
{
    QDataStream in(Socket->block);

    QString friend_account;
    QString friend_name;
    int isOnline;


    friends_list->clear();
    friends_list->setHorizontalHeaderLabels(QStringList()<<"账号"<<"昵称");

    online_friends = new QStandardItem("在线好友");
    friends_list->appendRow(online_friends);

    off_line_friends = new QStandardItem("离线好友");
    friends_list->appendRow(off_line_friends);


    while(!in.atEnd())
    {
        in>>friend_account>>friend_name>>isOnline;

        if(isOnline)
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(friend_account));
            newRow.push_back(new QStandardItem(friend_name));
            online_friends->appendRow(newRow);
        }
        else
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(friend_account));
            newRow.push_back(new QStandardItem(friend_name));
            off_line_friends->appendRow(newRow);
        }
    }

    Socket->block.clear();      //清空 socket 的消息

}



void MainWindow::init()         //主页面初始化
{
    QDataStream in(Socket->block);

    QString str;
    in>>str>>host_account>>host_name;

    ui->label_account->setText(host_account);
    ui->label_name_2->setText(host_name);

    QString friend_account;
    QString friend_name;
    int isOnline;


    while(!in.atEnd())        //初始化好友信息
    {
        in>>friend_account>>friend_name>>isOnline;

        if(isOnline)
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(friend_account));
            newRow.push_back(new QStandardItem(friend_name));
            online_friends->appendRow(newRow);
        }
        else
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(friend_account));
            newRow.push_back(new QStandardItem(friend_name));
            off_line_friends->appendRow(newRow);
        }
    }

    Socket->block.clear();

}



void MainWindow::on_pushButton_3_clicked() //退出登录函数
{
    timer->start();

    Socket->newConnect();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("EXIT")<<host_account;
    Socket->sendMessage();


    Dialog *log = new Dialog;           //退回到登录界面
    log->setAttribute(Qt::WA_DeleteOnClose,true);
    log->show();
    this->close();
}



void MainWindow::select_friend(QModelIndex index)  //选择聊天好友
{
    QModelIndex parentIndex = index.parent();

    int parentRow = parentIndex.row();
    int row = index.row();

    if(parentRow!=-1)
    {
        QStandardItem *parentItem=friends_list->item(parentRow);
        QStandardItem *selectItem=parentItem->child(row);
        QString account = selectItem->text();

        timer->start();

        Socket->newConnect();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("TALK")<<host_account<<account;   //向服务器发送本用户账号和好友账号，获取好友的IP
        Socket->sendMessage();
    }

}



void MainWindow::response_for_communication()   //服务器对聊天请求的回复
{
    timer->stop();

    QDataStream in(Socket->block);

    QString senderIP;
    QString peer_name;
    QString peer_id;

    in>>senderIP;
    in>>peer_id;     //获取好友的IP地址
    in>>peer_name;

    int isOnline;
    in>>isOnline;

    TalkWindows *receiveWin = getTalkWindow(peer_id);

    if(receiveWin == NULL)
    {
        receiveWin=new TalkWindows(senderIP,peer_name,isOnline);

        id_windows.insert(peer_id,receiveWin);
    }

    receiveWin->show();
}



void MainWindow::Timer_out()     //连接超时
{
    timer->stop();

    QMessageBox *mm = new QMessageBox;
    mm->setText("网络连接错误，请检查你的网络");
    mm->show();
}



void MainWindow::on_pushButton_clicked()   //启动改变昵称窗口
{
    change_name *win = new change_name(this);

    win->setAttribute(Qt::WA_DeleteOnClose,true);
    win->show();
}



void MainWindow::changeName(QString newname)  //昵称修改成功后，在主页面更新昵称
{
    ui->label_name_2->setText(newname);
}



void MainWindow::on_pushButton_2_clicked()       //启动修改密码窗口
{
    change_password *win = new change_password(this);
    win->setAttribute(Qt::WA_DeleteOnClose,true);
    win->show();
}



void MainWindow::on_pushButton_addfriend_clicked()    //启动添加好友窗口
{
    Add_Friend *win = new Add_Friend(this);
    win->setAttribute(Qt::WA_DeleteOnClose,true);
    win->show();
}



void MainWindow::response_for_addfriend()      //获取对方对好友请求的回应
{
    QString message;
    QDataStream in(Socket->block);
    in>>message;

    if(message == "OK")       //好友请求通过
    {
        QString id_friend;
        QString name_friend;
        int isOnline;

        in>>id_friend>>name_friend>>isOnline;

        if(isOnline)
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id_friend));
            newRow.push_back(new QStandardItem(name_friend));
            online_friends->appendRow(newRow);
        }
        else
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id_friend));
            newRow.push_back(new QStandardItem(name_friend));
            off_line_friends->appendRow(newRow);
        }

        QMessageBox *mm = new QMessageBox;
        mm->setText("添加好友成功");
        mm->show();
    }
    else        //好友请求被拒绝
    {

        QMessageBox *mm = new QMessageBox;
        mm->setText("添加好友失败");
        mm->show();
    }
}



void MainWindow::friend_request()        //有新的好友请求到达
{
    QString id_request;      //好友请求的对方账号
    QString name_request;   //好友请求的对方账号
    int isOnline;

    QDataStream in(Socket->block);

    in>>id_request;
    in>>name_request;
    in>>isOnline;

    QMessageBox msgBox;

    msgBox.setText("有新的好友请求到达");    //好友请求提示
    QString str = "账号：";
    str = str+id_request+"\n昵称："+name_request;
    msgBox.setInformativeText(str);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

    int ret = msgBox.exec();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);

    Socket->newConnect();

    switch(ret)
    {
        case QMessageBox::Yes:     //答应添加好友

        out<<tr("RESPONSEFORFRIEND")<<id_request<<host_account<<tr("OK");
        Socket->sendMessage();

        if(isOnline)
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id_request));
            newRow.push_back(new QStandardItem(name_request));
            online_friends->appendRow(newRow);
        }
        else
        {
            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id_request));
            newRow.push_back(new QStandardItem(name_request));
            off_line_friends->appendRow(newRow);
        }
        break;


    case QMessageBox::No:         //拒绝添加好友

        out<<tr("RESPONSEFORFRIEND")<<id_request<<host_account<<tr("NO");

        Socket->sendMessage();
        break;

    default:
            break;
    }
}
