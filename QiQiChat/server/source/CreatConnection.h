//连接数据库模块

#ifndef CREATCONNECTION_H
#define CREATCONNECTION_H
#include "chatserver.h"

#include <QtSql>
#include <QSqlQuery>


bool CreatDatabase()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost"); //设置数据库主机名
    db.setDatabaseName("Chat.db"); //设置数据库名
    db.setUserName("zhangjiuchao"); //设置数据库登入用户名
    db.setPassword("1325849oo"); //设计数据库登入密码


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
