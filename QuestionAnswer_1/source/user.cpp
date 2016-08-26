#include "user.h"
#include <QDateTime>
#include <questinfor.h>
#include "Question_Answer_System.h"
#include <QMessageBox>

user::user(QString str1,QString str2,QString str3):
   ID(str1),userName(str2),Password(str3)
{
}

bool user::AddFocus(QString focusID)
{
    if(focusList.count(focusID)==0)
    {
        focusList.push_back(focusID);
        return true;
    }
    focusList.removeAll(focusID);
    return false;
}

void user::AddQuest(QString title, QString content)
{

    QDateTime currentTime=QDateTime::currentDateTime();
    QString questionID=currentTime.toString("yyyyMMddhhmmss");
    QString creatTime=currentTime.toString("yyyy-MM-dd-hh:mm:ss");
    QuestInfor *newQuest=new QuestInfor(questionID,ID,creatTime,content,title);

    questMap.insert(questionID,newQuest);
    myQuestion.push_back(questionID);
    popular_value_vec.push_back(newQuest);
}

void user::AddAnswer(QString id_answer)
{
    question_my_answer.push_back(id_answer);
}

void user::eraseQuest(QString question_id)
{
    myQuestion.removeOne(question_id);

    questMap.remove(question_id);
}


void user::eraseFocus(QString eraseID)
{
    focusList.removeOne(eraseID);
}


bool user::changePassword(QString str1, QString str2, QString str3)
{
    if(str1!=Password)
    {
        QMessageBox *message=new QMessageBox;
        message->setText("修改失败\n原密码错误");
        message->show();
        return false;
    }
    else
    {
        if(str2!=str3)
        {
            QMessageBox *message1=new QMessageBox;
            message1->setText("修改失败\n两次输入的修改密码不一致");
            message1->show();
            return false;
        }
        else
        {
           Password=str2;
           QMap<QString,QString>::iterator it=ID_Password.find(HostUser->getID());
           it.value()=Password;

           return true;
        }
    }
}


QTextStream& operator<<(QTextStream& os,const user* host)
{
    os<<host->ID<<" ";
    os<<host->Password<<"  ";
    os<<host->userName<<"\r\n";

    int number_of_focus=host->focusList.size();
    os<<number_of_focus;
    for(int i=0;i<number_of_focus;++i)
        os<<"\r\n"<<host->focusList[i];

    int number_of_question=host->myQuestion.size();
    os<<"\r\n"<<number_of_question;
    for(int i=0;i<number_of_question;++i)
        os<<"\r\n"<<host->myQuestion[i];

    int number_of_answer=host->question_my_answer.size();
    os<<"\r\n"<<number_of_answer;
    for(int i=0;i<number_of_answer;++i)
        os<<"\r\n"<<host->question_my_answer[i];

    return os;
}

QTextStream& operator>>(QTextStream& is,user* host)
{

    is>>host->ID;

    is>>host->Password;
    is>>host->userName;

    int NumberofFocus;
    is>>NumberofFocus;

    for(int i=0;i<NumberofFocus;++i)
    {
        QString focus_id;
        is>>focus_id;
        host->focusList.push_back(focus_id);
    }

    int NumberofQuestion;
    is>>NumberofQuestion;
    for(int i=0;i<NumberofQuestion;++i)
    {
        QString question_id;
        is>>question_id;
        host->myQuestion.push_back(question_id);
     }

    int Numberofanswer;
    is>>Numberofanswer;
    for(int i=0;i<Numberofanswer;++i)
    {
        QString answer_id;
        is>>answer_id;
        host->question_my_answer.push_back(answer_id);
    }

    return is;
}

 QString user::getID()
{
    return ID;
}

 QString user::getName()
{
    return userName;
}

 QString user::getPassword()
{
    return Password;
}

QList<QString> user::getFocusList()
{
    return focusList;
}

QList<QString> user::getMyQuestion()
{
    return myQuestion;
}

QList<QString> user::get_question_my_anwser()
{
    return question_my_answer;
}

bool user::isFocus(QString id)
{
    if(focusList.count(id)==0)
        return false;
    return true;
}
