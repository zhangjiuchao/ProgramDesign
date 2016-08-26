#ifndef QUESTION_ANSWER_SYSTEM_H
#define QUESTION_ANSWER_SYSTEM_H
#include "user.h"
#include <QString>
#include <QMap>
#include <QVector>
#include "questinfor.h"
#include <QStringList>
#include <QtCore>
using namespace std;

extern QMap<QString,QString> ID_Password;
extern QMap<QString,QString> ID_Name;

extern QMap<QString,QuestInfor*> questMap;

extern user *HostUser;
extern bool is_exist_space(QString);
extern bool LoadQuestion();
extern bool LoadUser(QString ,user*);

extern QVector<QuestInfor*> popular_value_vec;

extern bool compare(QuestInfor*,QuestInfor*);

extern QVector<QuestInfor*> find_question(QStringList);
extern QVector<QuestInfor*> find_question(QString);

#endif // QUESTION_ANSWER_SYSTEM_H


