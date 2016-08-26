#ifndef BASICINFO_H
#define BASICINFO_H

#include <QString>
class BasicInfo
{
public:
    BasicInfo(QString str1,QString str2,QString str3,QString str4);

    virtual QString outPut()=0;

    QString getID();
    QString getIDIDofCreator();
    QString getTimeofCreate();
    QString getcontent();

    void setID(QString);
    void setIDofCreator(QString);
    void setTimeofCreate(QString);
    void setcontent(QString);
private:
    QString ID;
    QString IDofCreator;
    QString TimeofCreate;
    QString content;

};

#endif // BASICINFO_H
