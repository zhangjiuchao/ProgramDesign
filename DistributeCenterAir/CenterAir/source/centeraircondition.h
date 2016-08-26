#ifndef CENTERAIRCONDITION_H
#define CENTERAIRCONDITION_H

#include "centerair.h"
#include "room.h"
#include "communication.h"
#include "schedule.h"
#include<QMap>
#include<QSqlQuery>
#include <QSqlDatabase>
#include "mainwindow.h"
extern int nRoom;
extern int nThread;
extern int clientPort;
extern int serverPort;
extern CenterAir* centerAir;
extern QMap<int,Room*> rooms;
extern Communication* communicate;
extern Schedule* schedule;
extern MainWindow *mainwin;
extern QSqlDatabase db;   //���ݿ����
#endif // CENTERAIRCONDITION_H
