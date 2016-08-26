#ifndef TCPTHREAD_H
#define TCPTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <QTimer>


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

    void friend_request(QString,QString);

    void friend_response(QString,QString,QString);


public slots:

    void receiveMessage();

    void friendrequest(QString,QString);

    void friendresponse(QString ,QString,QString);

    void update();


private:
    int socketDescriptor;

    QTcpSocket *tcpSocket;

    QByteArray inBlock;

    QString peer_account;

    QTimer *timer;
};

#endif // TCPTHREAD_H
