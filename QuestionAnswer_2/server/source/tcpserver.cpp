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

    connect(newThread,SIGNAL(finished()),
            this,SLOT(deleteLater()));
    newThread->start();

}



