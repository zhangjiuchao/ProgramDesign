#include "questinfor.h"
#include "ansidecl.h"
QuestInfor::QuestInfor(QString str1,QString str2,QString str3,QString str4,
                       QString str5)
            :BasicInfo(str1,str2,str3,str4),title(str5)
{
    popular_value=0;
}

void QuestInfor::CreateAnswer(QString str1, QString str2, QString str3, QString str4, int num)
{
    AnswerInfor *newAnswer=new AnswerInfor(str1,str2,str3,str4,num);
    answers.push_front(newAnswer);
}


QTextStream& operator<<(QTextStream& os,QuestInfor* question)
{

    os<<question->outPut();
    return os;
}

QTextStream& operator>>(QTextStream& is,QuestInfor* question)
{

    QString str;

    is>>str;
    question->setID(str);

    is>>str;
    question->setIDofCreator(str);

    is>>str;
    question->setTimeofCreate(str);

    is>>question->title;

    is>>str;
    question->setcontent(str);

    int number_of_keyword;
    is>>number_of_keyword;

    for(int i=0;i<number_of_keyword;++i)
    {
        is>>str;
        question->keyword.insert(str);
    }

    is>>question->popular_value;

    int number_of_answer;
    is>>number_of_answer;

    for(int i=0;i<number_of_answer;++i)
    {
        AnswerInfor *newAnswer=new AnswerInfor("","","","",0);
        is>>newAnswer;
        question->answers.push_back(newAnswer);
    }

    return is;
}

QString QuestInfor::gettitle()
{
    return title;
}

QList<AnswerInfor*> QuestInfor::getAnswer()
{
    return answers;
}

QString QuestInfor::outPut()
{
    QString str="\r\n";
    str=str+getID()+"\r\n"+getIDIDofCreator()+"  "+getTimeofCreate()+"\r\n\r\n"+
            title+"\r\n"+getcontent()+"\r\n\r\n";

    int number_of_keyword=keyword.size();
    str=str+QString::number(number_of_keyword);
    QSet<QString>::const_iterator it=keyword.begin(),
                                  iter=keyword.end();
    while(it!=iter)
    {
        str=str+"\r\n"+*it;
        it++;
    }
    int number_of_answer=answers.size();
    str=str+"\r\n"+QString::number(popular_value)+"\r\n"+QString::number(number_of_answer);
    for(int i=0;i<number_of_answer;++i)
    {
        str=str+answers[i]->outPut();
    }

    return str;
}


void QuestInfor::add_popular_value()
{
    popular_value++;
}

int QuestInfor::get_popular_value()
{
    return popular_value;
}


bool QuestInfor::find(QStringList strlist)
{
    QString find_keyword;
    while(strlist.size()>0)
    {
        find_keyword=strlist.takeFirst();

        if(!keyword.contains(find_keyword))
        {
            if(!title.contains(find_keyword))
                return false;
            keyword.insert(find_keyword);
        }
    }

    return true;
}


AnswerInfor* QuestInfor::find_answer(QString id_answer)
{
    int nAnswer=answers.size();
    for(int i=0;i<nAnswer;++i)
    {
        AnswerInfor *current=answers.value(i);
        if(current->getID()==id_answer)
            return current;
    }
    return NULL;
}
