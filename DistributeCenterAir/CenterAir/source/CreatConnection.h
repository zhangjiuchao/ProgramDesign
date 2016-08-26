
//�������ݿ�ģ��

#ifndef CREATCONNECTION_H
#define CREATCONNECTION_H

#include "centeraircondition.h"
#include <QtSql>
#include <QSqlQuery>
#include<QDebug>

bool CreatDatabase()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("zhangjiuchao"); //�������ݿ�������
    db.setDatabaseName("CenterAir.db"); //�������ݿ���
    db.setUserName("zhangjiuchao"); //�������ݿ�����û���
    db.setPassword("1325849oo"); //������ݿ��������


    if(!db.open())
        return false;

    QSqlQuery query(db);

    query.exec("create table Excel(id int primary key,timelength int,totalmoney double,"
              "openclosetimes int,tempscheduletimes int,windscheduletimes int,scheduletimes int)");
    for(int i=0;i<nRoom;++i){
        query.prepare("insert into Excel(id,timelength,totalmoney,openclosetimes,tempscheduletimes,windscheduletimes,scheduletimes)"
                      "values(:id,:timelength,:totalmoney,:openclosetimes,:tempscheduletimes,:windscheduletimes,:scheduletimes)");
        query.bindValue(":id",i);
        query.bindValue(":timelength",0);
        query.bindValue(":totalmoney",0);
        query.bindValue(":openclosetimes",0);
        query.bindValue(":tempscheduletimes",0);
        query.bindValue(":windscheduletimes",0);
        query.bindValue(":scheduletimes",0);
    }

    return true;
}

#endif // CREATCONNECTION_H
