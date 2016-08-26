#ifndef USER_H
#define USER_H

//#include <iostream>
#include <QString>
#include <QList>
#include "questinfor.h"
#include <QTextStream>

class user
{

public:
    user(QString str1="",QString str2="",QString str3="");

    bool AddFocus(QString focusID);

    void AddQuest(QString title,QString content);

    void AddAnswer(QString);

    void eraseQuest(QString question_id);
    void eraseFocus(QString eraseID);
    bool changePassword(QString str1,QString str2);

    bool isFocus(QString id);

    friend QTextStream& operator<<(QTextStream& os,const user* host);
    friend QTextStream& operator>>(QTextStream& is,user* host);

    QList<QString> getFocusList();
    QList<QString> getMyQuestion();
    QList<QString> get_question_my_anwser();
    QString getID();
    QString getName();
    QString getPassword();
private:
    QString ID;
    QString userName;
    QString Password;
    QList<QString> focusList;
    QList<QString> myQuestion;
    QList<QString> question_my_answer;
};

#endif // USER_H
