
//服务器的TCP模块

#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent=0);



signals:

    void bytesArrived(qint64,quint32,int);

protected:
    void incomingConnection(int socketDescriptor);

};

#endif // TCPSERVER_H
