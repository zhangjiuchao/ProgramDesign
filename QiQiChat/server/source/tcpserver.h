
//服务器的TCP模块

#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent=0);


public slots:
    void friend_request_slot(QString,QString);

    void friend_response_slot(QString,QString,QString);


signals:
    void friend_request_sig(QString,QString);

    void friend_response_sig(QString, QString,QString);

    void bytesArrived(qint64,quint32,int);


protected:
    void incomingConnection(int socketDescriptor);

};

#endif // TCPSERVER_H
