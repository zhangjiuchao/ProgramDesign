
//��¼�ɹ����������

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

    udpSocket = new QUdpSocket;     //����������ͨ�ŵ� UDP

    udpSocket->bind(PortofClient,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(getMessage()));

    friends_list = new QStandardItemModel();
    friends_list->setHorizontalHeaderLabels(QStringList()<<"�˺�"<<"�ǳ�");

    ui->treeView_friends->setModel(friends_list);
    ui->treeView_friends->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView_friends->setSelectionBehavior(QAbstractItemView::SelectRows);

    online_friends = new QStandardItem("���ߺ���");
    friends_list->appendRow(online_friends);


    off_line_friends = new QStandardItem("���ߺ���");
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



void MainWindow::setinfor(QString account,QString name)  //�ڽ�����ʾ��ǰ�û���id���ǳ�
{
    ui->label_account->setText(account);

    ui->label_name_2->setText(name);
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::getMessage()         //��UDP �н�����Ϣ
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


        if(messageType == "MESSAGE")           //���յ�����Ϣ����Ϊ��MESSAGE�� ʱ����Ϣ�������û���������Ϣ��
        {                                     //��ʾ����Ӧ�����촰��
            QString id_peer;
            QString name_peer;
            in>>id_peer>>name_peer;

            TalkWindows *receiveWin=getTalkWindow(id_peer);  //��ö�Ӧ�����������촰�ڵľ��

            if(receiveWin == NULL)
            {
                receiveWin = new TalkWindows(senderIP,name_peer,true);

                id_windows.insert(id_peer,receiveWin);
            }

            QString message;
            in>>message;

            receiveWin->show();
            receiveWin->getMessage(message);     //���촰����ʾ��Ϣ

        }


    }
}



TalkWindows* MainWindow::getTalkWindow(QString id)  //�� id_windows ���ҵ��� id ����������촰�ھ��
{
    QMap<QString,TalkWindows*>::const_iterator it = id_windows.find(id);

    if(it != id_windows.end())
        return it.value();
    else
        return NULL;
}



void MainWindow::insertItem(QString id, TalkWindows *peerWin) //��id_windows �����ֵ��
{
    id_windows.insert(id,peerWin);
}



void MainWindow::eraseItem(QString id)     //ȡ����ֵ��
{
    id_windows.remove(id);
}



void MainWindow::updateinfor()       //���º�����Ϣ
{
    QDataStream in(Socket->block);

    QString friend_account;
    QString friend_name;
    int isOnline;


    friends_list->clear();
    friends_list->setHorizontalHeaderLabels(QStringList()<<"�˺�"<<"�ǳ�");

    online_friends = new QStandardItem("���ߺ���");
    friends_list->appendRow(online_friends);

    off_line_friends = new QStandardItem("���ߺ���");
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

    Socket->block.clear();      //��� socket ����Ϣ

}



void MainWindow::init()         //��ҳ���ʼ��
{
    QDataStream in(Socket->block);

    QString str;
    in>>str>>host_account>>host_name;

    ui->label_account->setText(host_account);
    ui->label_name_2->setText(host_name);

    QString friend_account;
    QString friend_name;
    int isOnline;


    while(!in.atEnd())        //��ʼ��������Ϣ
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



void MainWindow::on_pushButton_3_clicked() //�˳���¼����
{
    timer->start();

    Socket->newConnect();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("EXIT")<<host_account;
    Socket->sendMessage();


    Dialog *log = new Dialog;           //�˻ص���¼����
    log->setAttribute(Qt::WA_DeleteOnClose,true);
    log->show();
    this->close();
}



void MainWindow::select_friend(QModelIndex index)  //ѡ���������
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
        out<<tr("TALK")<<host_account<<account;   //����������ͱ��û��˺źͺ����˺ţ���ȡ���ѵ�IP
        Socket->sendMessage();
    }

}



void MainWindow::response_for_communication()   //����������������Ļظ�
{
    timer->stop();

    QDataStream in(Socket->block);

    QString senderIP;
    QString peer_name;
    QString peer_id;

    in>>senderIP;
    in>>peer_id;     //��ȡ���ѵ�IP��ַ
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



void MainWindow::Timer_out()     //���ӳ�ʱ
{
    timer->stop();

    QMessageBox *mm = new QMessageBox;
    mm->setText("�������Ӵ��������������");
    mm->show();
}



void MainWindow::on_pushButton_clicked()   //�����ı��ǳƴ���
{
    change_name *win = new change_name(this);

    win->setAttribute(Qt::WA_DeleteOnClose,true);
    win->show();
}



void MainWindow::changeName(QString newname)  //�ǳ��޸ĳɹ�������ҳ������ǳ�
{
    ui->label_name_2->setText(newname);
}



void MainWindow::on_pushButton_2_clicked()       //�����޸����봰��
{
    change_password *win = new change_password(this);
    win->setAttribute(Qt::WA_DeleteOnClose,true);
    win->show();
}



void MainWindow::on_pushButton_addfriend_clicked()    //������Ӻ��Ѵ���
{
    Add_Friend *win = new Add_Friend(this);
    win->setAttribute(Qt::WA_DeleteOnClose,true);
    win->show();
}



void MainWindow::response_for_addfriend()      //��ȡ�Է��Ժ�������Ļ�Ӧ
{
    QString message;
    QDataStream in(Socket->block);
    in>>message;

    if(message == "OK")       //��������ͨ��
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
        mm->setText("��Ӻ��ѳɹ�");
        mm->show();
    }
    else        //�������󱻾ܾ�
    {

        QMessageBox *mm = new QMessageBox;
        mm->setText("��Ӻ���ʧ��");
        mm->show();
    }
}



void MainWindow::friend_request()        //���µĺ������󵽴�
{
    QString id_request;      //��������ĶԷ��˺�
    QString name_request;   //��������ĶԷ��˺�
    int isOnline;

    QDataStream in(Socket->block);

    in>>id_request;
    in>>name_request;
    in>>isOnline;

    QMessageBox msgBox;

    msgBox.setText("���µĺ������󵽴�");    //����������ʾ
    QString str = "�˺ţ�";
    str = str+id_request+"\n�ǳƣ�"+name_request;
    msgBox.setInformativeText(str);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

    int ret = msgBox.exec();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);

    Socket->newConnect();

    switch(ret)
    {
        case QMessageBox::Yes:     //��Ӧ��Ӻ���

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


    case QMessageBox::No:         //�ܾ���Ӻ���

        out<<tr("RESPONSEFORFRIEND")<<id_request<<host_account<<tr("NO");

        Socket->sendMessage();
        break;

    default:
            break;
    }
}
