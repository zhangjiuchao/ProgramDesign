#include "basicinfo.h"

BasicInfo::BasicInfo(QString str1,QString str2,QString str3,QString str4)
    :ID(str1),IDofCreator(str2),TimeofCreate(str3),content(str4)
{

}

QString BasicInfo::getID()
{
    return ID;
}


QString BasicInfo::getIDIDofCreator()
{
    return IDofCreator;
}

QString BasicInfo::getTimeofCreate()
{
    return TimeofCreate;
}

QString BasicInfo::getcontent()
{
    return content;
}

void BasicInfo::setID(QString str)
{
    ID=str;
}

void BasicInfo::setIDofCreator(QString str)
{
    IDofCreator=str;
}

void BasicInfo::setTimeofCreate(QString str)
{
    TimeofCreate=str;
}

void BasicInfo::setcontent(QString str)
{
    content=str;
}
