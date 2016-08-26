#include "answerinfor.h"
#include <QMessageBox>
AnswerInfor::AnswerInfor(QString str1,QString str2,QString str3,QString str4,int num)
        :BasicInfo(str1,str2,str3,str4),praiseNum(num)
{
}

QTextStream& operator<<(QTextStream& os,AnswerInfor* answer)
{

    os<<answer->outPut();
    return os;
}

QTextStream& operator >>(QTextStream& is,AnswerInfor* answer)
{
    QString str;
    is>>str;
    answer->setID(str);
    is>>str;
    answer->setIDofCreator(str);
    is>>str;
    answer->setTimeofCreate(str);
    is>>str;
    answer->setcontent(str);
    is>>answer->praiseNum;

    for(int i=0;i<answer->praiseNum;++i)
    {
        is>>str;
        answer->praiseID.insert(str);
    }
    return is;
}

void AnswerInfor::addPraise(QString id)
{
    QSet<QString>::const_iterator it=praiseID.find(id);
    if(it==praiseID.end())
    {
        praiseID.insert(id);
        praiseNum++;
    }
    else
    {
        QMessageBox *mm=new QMessageBox;
        mm->setText("不可重复点赞");
        mm->show();
    }
}

int AnswerInfor::getpraiseNum()
{
    return praiseNum;
}

bool AnswerInfor::isPraise(QString id)
{
    QSet<QString>::const_iterator it=praiseID.find(id);
    if(it==praiseID.end())
        return false;
    return true;
}

QString AnswerInfor::outPut()
{
    QString str="\r\n";
    str=str+getID()+"\r\n"+getIDIDofCreator()+"  "+getTimeofCreate()+"\r\n\r\n"+
            getcontent()+"\r\n"+QString::number(praiseNum);

    QSet<QString>::const_iterator it=praiseID.begin(),
                                   iter=praiseID.end();
    while(it!=iter)
    {
        str=str+"\r\n"+*it;
        it++;
    }

    return str;
}
