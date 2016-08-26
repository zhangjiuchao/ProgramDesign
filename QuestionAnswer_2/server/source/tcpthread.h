#ifndef TCPTHREAD_H
#define TCPTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include "user.h"
#include <QVector>

class TcpThread : public QThread
{
    Q_OBJECT

public:
    TcpThread(int socketDiscriptor, QObject *parent=0);

    void run();

    QTcpSocket* getSocket();

signals:
    void bytesArrived(qint64,qint32,int);

    void error(QTcpSocket::SocketError socketError);

private slots:

    void receiveMessage();

private:
    int socketDescriptor;

    QTcpSocket *tcpSocket;

    user *HostUser;

    QVector<QuestInfor*> value_vec;

    int isRun;
};

#endif // TCPTHREAD_H
