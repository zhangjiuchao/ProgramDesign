#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
#include "tcpsocket.h"
#include <QString>
#include <QUdpSocket>

extern TcpSocket *Socket;     //���ںͷ�����ͨ�ŵ�TCP socket

extern QString IPofServer;    //��������IP

extern int PortofServer;       //�������Ķ˿ں�

extern QUdpSocket *udpSocket;      //���������UDP

extern int PortofClient;        //�ͻ��˵�ͳһ�˿ں�

extern QString host_account;       //�û����˺�

extern QString host_name;        //�û����ǳ�

#endif // CHAT_CLIENT_H
