//�������ݿ�ģ��

#ifndef CREATCONNECTION_H
#define CREATCONNECTION_H
#include "chatserver.h"

#include <QtSql>
#include <QSqlQuery>


bool CreatDatabase()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost"); //�������ݿ�������
    db.setDatabaseName("Chat.db"); //�������ݿ���
    db.setUserName("zhangjiuchao"); //�������ݿ�����û���
    db.setPassword("1325849oo"); //������ݿ��������


    if(!db.open())
        return false;

    QSqlQuery query(db);

    query.exec("create table User(id int primary key,account varchar(20),name varchar(20),"
              "password varchar(20),isOnline int,IP varchar(30),friendsnum int,"
              "friend_1 varchar(20),friend_2 varchar(20),friend_3 varchar(20),"
              "friend_4 varchar(20),friend_5 varchar(20),friend_6 varchar(20))");

    return true;
}

#endif // CREATCONNECTION_H
