#ifndef QAS_CLIENT_H
#define QAS_CLIENT_H

#include <QString>
#include "tcpsocket.h"
#include <QTimer>
#include "log_in.h"

extern TcpSocket *Socket;     //���ںͷ�����ͨ�ŵ�TCP socket

extern QString IPofServer;    //��������IP

extern int PortofServer;       //�������Ķ˿ں�

extern bool is_exist_space(QString);   //�ж��ַ������Ƿ��пո�

extern Log_in *w;

#endif // QAS_CLIENT_H
