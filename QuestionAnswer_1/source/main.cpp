#include <QtGui/QApplication>
#include "log_in.h"
#include "Question_Answer_System.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGui>

void LoadAccountInformation();

QMap<QString,QString> ID_Name;

QMap<QString,QuestInfor*> questMap;

QVector<QuestInfor*> popular_value_vec;

QMap<QString,QString> ID_Password;

user *HostUser;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QFont font=a.font();
    font.setPointSize(10);
    a.addLibraryPath("./plugins");
    a.setFont(font);


    LoadAccountInformation();

    return a.exec();
}

void LoadAccountInformation()
{
    QString str="Data\\IDPassword.txt";
    QFile file1(str);

    try{
        if(!file1.open(QIODevice::ReadOnly))
            throw QString("账号信息文件打开失败");

        Log_in *w=new Log_in;
        w->show();

        QString IDstring="";
        QString Passwordstring="";
        QString Namestring="";
        QTextStream in(&file1);
        if(!file1.atEnd())
        {
            str=in.readAll();
            str=str.simplified();
            QStringList instring=str.split(' ');
            while(instring.size()>0)
            {
                IDstring=instring.takeFirst();
                Passwordstring=instring.takeFirst();
                Namestring=instring.takeFirst();

                ID_Password.insert(IDstring,Passwordstring);

                ID_Name.insert(IDstring,Namestring);
            }
        }

        file1.close();
    }
    catch(QString message)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText(message);
        msg->exec();
    }

}


bool is_exist_space(QString str)
{
    for(int i=0;i<str.size();++i)
    {
        if(str[i]==0x0020||str[i]==0x0A)
            return true;
    }

    return false;
}

bool LoadQuestion()
{
    QString file_to_open="Data\\all_question.txt";
    QFile file(file_to_open);

    try{
        if(!file.open(QIODevice::ReadOnly))
            throw QString("问题文件打开失败");
        QTextStream in(&file);

        while(!in.atEnd())
        {

            QuestInfor *newQuestion=new QuestInfor("","","","","");
            in>>newQuestion;

            questMap.insert(newQuestion->getID(),newQuestion);
            popular_value_vec.push_back(newQuestion);

        }
        file.close();
        return true;
    }
    catch(QString message)
    {
        QMessageBox *msg=new QMessageBox;
        msg->setText(message);
        msg->exec();
        return false;
    }
}


bool compare(QuestInfor * question1, QuestInfor *question2)
{
    int popular_value_1=question1->get_popular_value();
    int popular_value_2=question2->get_popular_value();
    return popular_value_1>popular_value_2;
}


QVector<QuestInfor*> find_question(QStringList str)
{
    QVector<QuestInfor*> result;
    QMap<QString,QuestInfor*>::const_iterator it=questMap.begin(),
                                              iter=questMap.end();
    while(it!=iter)
    {
        QuestInfor *question=it.value();
        if(question->find(str))
        {
            result.push_back(question);
        }

        ++it;
    }

    return result;
}

QVector<QuestInfor*> find_question(QString str)
{
    QVector<QuestInfor*> result;
    QMap<QString,QuestInfor*>::const_iterator it=questMap.begin(),
                                              iter=questMap.end();
    while(it!=iter)
    {

        if((it.key()).contains(str))
        {
            result.push_back(it.value());
        }
        it++;
    }

    return result;
}
