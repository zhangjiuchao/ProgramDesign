#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
#include "tcpsocket.h"
#include <QString>
#include <QUdpSocket>

extern TcpSocket *Socket;     //用于和服务器通信的TCP socket

extern QString IPofServer;    //服务器的IP

extern int PortofServer;       //服务器的端口号

extern QUdpSocket *udpSocket;      //用于聊天的UDP

extern int PortofClient;        //客户端的统一端口号

extern QString host_account;       //用户的账号

extern QString host_name;        //用户的昵称

#endif // CHAT_CLIENT_H
