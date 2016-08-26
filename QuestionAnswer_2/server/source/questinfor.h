#ifndef QUESTINFOR_H
#define QUESTINFOR_H
#include "basicinfo.h"
#include "answerinfor.h"
#include <QList>
#include "QTextStream"
#include <QSet>
#include <QStringList>

class QuestInfor : public BasicInfo
{
public:
    QuestInfor(QString str1,QString str2,QString str3,QString str4,QString str5);
    void CreateAnswer(QString str1,QString str2,QString str3,QString str4,int num);

    QString outPut();

    friend QTextStream& operator<<(QTextStream& os,QuestInfor* question);
    friend QTextStream& operator>>(QTextStream& is,QuestInfor* question);
    QString gettitle();
    QList<AnswerInfor*> getAnswer();

    int get_popular_value();
    void add_popular_value();

    bool find(QStringList);

    AnswerInfor* find_answer(QString);

private:
    QString title;
    QList<AnswerInfor*> answers;

    QSet<QString> keyword;
    int popular_value;

};

#endif // QUESTINFOR_H
