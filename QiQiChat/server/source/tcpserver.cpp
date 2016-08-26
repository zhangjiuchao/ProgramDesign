#include "tcpserver.h"
#include "tcpthread.h"
#include <QMessageBox>



TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
}



void TcpServer::incomingConnection(int socketDescriptor)  //有新的连接时自动调用创建新的线程
{

    TcpThread *newThread = new TcpThread(socketDescriptor,this);

    connect(newThread,SIGNAL(friend_request(QString,QString)),
            this,SLOT(friend_request_slot(QString,QString)));

    connect(newThread,SIGNAL(friend_response(QString,QString,QString)),
            this,SLOT(friend_response_slot(QString,QString,QString)));

    connect(this,SIGNAL(friend_request_sig(QString,QString)),
            newThread,SLOT(friendrequest(QString,QString)));

    connect(this,SIGNAL(friend_response_sig(QString,QString,QString)),
             newThread,SLOT(friendresponse(QString,QString,QString)));

    addPendingConnection(newThread->getSocket());

    connect(newThread,SIGNAL(finished()),
            this,SLOT(deleteLater()));

    newThread->start();
}



void TcpServer::friend_request_slot(QString id1, QString id2)
{
    emit friend_request_sig(id1,id2);
}



void TcpServer::friend_response_slot(QString id1, QString id2,QString message)
{
    emit friend_response_sig(id1,id2,message);
}
