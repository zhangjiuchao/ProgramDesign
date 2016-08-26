#ifndef QAS_CLIENT_H
#define QAS_CLIENT_H

#include <QString>
#include "tcpsocket.h"
#include <QTimer>
#include "log_in.h"

extern TcpSocket *Socket;     //用于和服务器通信的TCP socket

extern QString IPofServer;    //服务器的IP

extern int PortofServer;       //服务器的端口号

extern bool is_exist_space(QString);   //判断字符串中是否含有空格

extern Log_in *w;

#endif // QAS_CLIENT_H
