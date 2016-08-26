#include "tcpserver.h"
#include "tcpthread.h"
#include <QMessageBox>



TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{

}



void TcpServer::incomingConnection(int socketDescriptor)  //���µ�����ʱ�Զ����ô����µ��߳�
{

    TcpThread *newThread = new TcpThread(socketDescriptor,this);

    connect(newThread,SIGNAL(finished()),
            this,SLOT(deleteLater()));
    newThread->start();

}



