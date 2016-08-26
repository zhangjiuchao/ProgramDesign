#ifndef QAS_SERVER_H
#define QAS_SERVER_H

#include "user.h"
#include <QString>
#include <QMap>
#include <QVector>
#include "questinfor.h"
#include <QStringList>
#include <QtCore>
#include <QMutex>
#include <QSet>
#include "mainwindow.h"

using namespace std;

extern MainWindow *w;

extern QMap<QString,QString> ID_Password;    //存放 id 和密码

extern QMap<QString,QString> ID_Name;       //存放 id 和昵称

extern QMap<QString,QuestInfor*> questMap;     //存放问题id 和问题


extern bool LoadQuestion();             //加载所有问题

extern bool LoadUser(QString ,user*);       //加载id为QString 的用户信息

extern QVector<QuestInfor*> popular_value_vec;     //存放所以问题的向量，便于按热度值排序时使用

extern bool compare(QuestInfor*,QuestInfor*);     //比较两个问题的热度值大小，排序时使用

extern QVector<QuestInfor*> find_question(QStringList);   //按关键字搜索问题

extern QVector<QuestInfor*> find_question(QString);    //按时间搜索问题

extern QMutex mutex;                         //互斥锁，用于线程同步

extern QSet< QString > online_set;           //存放在线用户id

#endif // QAS_SERVER_H
