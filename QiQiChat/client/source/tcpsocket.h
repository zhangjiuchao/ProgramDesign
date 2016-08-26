#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include <QObject>


class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket( QObject *parent = 0 );

    void sendMessage();    //������Ϣ

    QTcpSocket *tcpsocket;      //�������ͨ�ŵ� socket

    QByteArray block;        //������Ϣ������
signals:

    void ResponseForLog();

    void ResponseForRegiste();

    void ResponseForCommunication();

    void ResponseForChangename();

    void ResponseForChangepassword();

    void ResponseForAddfriend();

    void Addrequest();

    void update();
public slots:

    void readMessage();

    void newConnect();

};

#endif // TCPSOCKET_H
