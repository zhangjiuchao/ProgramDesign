#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include<QUdpSocket>

class Communication:public QObject
{
    Q_OBJECT
public:
    Communication();

public slots:
    void readData();
private:
    QUdpSocket *udpsocket;
};

#endif // COMMUNICATION_H
