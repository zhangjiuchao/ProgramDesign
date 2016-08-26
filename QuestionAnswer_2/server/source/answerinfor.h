#ifndef ANSWERINFOR_H
#define ANSWERINFOR_H
#include "basicinfo.h"
#include "QTextStream"
#include <QSet>

class AnswerInfor : public BasicInfo
{
public:
    AnswerInfor(QString str1,QString str2,QString str3,QString str4,int num);

    friend QTextStream& operator<<(QTextStream& os,AnswerInfor* answer);
    friend QTextStream& operator >>(QTextStream& is,AnswerInfor*answer);
    QString outPut();

    void addPraise(QString);
    bool isPraise(QString id);
    int getpraiseNum();
private:
    int praiseNum;
    QSet<QString> praiseID;
};

#endif // ANSWERINFOR_H
