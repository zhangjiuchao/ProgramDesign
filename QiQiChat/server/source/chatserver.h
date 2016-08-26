#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMutex>

#include <QSqlDatabase>

extern QMutex mutex; //互斥锁

extern QSqlDatabase db;   //数据库变量

#endif // CHATSERVER_H
