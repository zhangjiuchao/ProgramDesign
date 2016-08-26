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

extern QMap<QString,QString> ID_Password;    //��� id ������

extern QMap<QString,QString> ID_Name;       //��� id ���ǳ�

extern QMap<QString,QuestInfor*> questMap;     //�������id ������


extern bool LoadQuestion();             //������������

extern bool LoadUser(QString ,user*);       //����idΪQString ���û���Ϣ

extern QVector<QuestInfor*> popular_value_vec;     //���������������������ڰ��ȶ�ֵ����ʱʹ��

extern bool compare(QuestInfor*,QuestInfor*);     //�Ƚ�����������ȶ�ֵ��С������ʱʹ��

extern QVector<QuestInfor*> find_question(QStringList);   //���ؼ�����������

extern QVector<QuestInfor*> find_question(QString);    //��ʱ����������

extern QMutex mutex;                         //�������������߳�ͬ��

extern QSet< QString > online_set;           //��������û�id

#endif // QAS_SERVER_H
