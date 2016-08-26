#include "tcpthread.h"
#include <QtGui>
#include "QAS_server.h"
#include "mymutex.h"
#include <QMessageBox>
#include "questinfor.h"

//为某一客户端服务的线程

bool LoadUser(QString filetoOpen,user* client);

TcpThread::TcpThread(int socketDiscriptor,QObject *parent)
    : QThread(parent),socketDescriptor(socketDiscriptor)
{
    tcpSocket = new QTcpSocket(this);
    if(!tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket->error());
    }
    else
    {

        connect(tcpSocket,SIGNAL(readyRead()),
                this,SLOT(receiveMessage()));

    }

    isRun = 1;
}



void TcpThread::run()
{
    while(isRun)
    {

    }
}


QTcpSocket* TcpThread::getSocket()
{
    return tcpSocket;
}


//socket 接收消息，并按照消息头部的标识符区分用户请求的类型，进行相应的操作，并返回给用户

void TcpThread::receiveMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    QString newsType;
    in>>newsType;

    if(newsType== "LOG")           //登录请求到来
    {

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        QString id,password;
        in>>id>>password;

        {
            myMutex locker(&mutex);

            QMap<QString,QString>::iterator it=ID_Password.find(id);

            if(it == ID_Password.end())
            {
                out<<tr("RES_LOG");
                out<<0;
                tcpSocket->write(block);
            }
            else
            {
                if(it.value()!=password)        //密码错误，登录失败
                {
                    out<<tr("RES_LOG");
                    out<<1;
                    tcpSocket->write(block);
                }

                else
                {
                    if(online_set.contains(id))
                    {
                        out<<tr("RES_LOG");
                        out<<2;
                        tcpSocket->write(block);
                    }
                    else
                    {
                        HostUser = new user;
                        if( !LoadUser(id,HostUser) )
                        {
                           out<<tr("RES_LOG");
                           out<<3;
                           tcpSocket->write(block);

                           delete HostUser;
                        }
                        else
                        {
                               online_set.insert(id);
                               QString peerIP=tcpSocket->peerAddress().toString();

                               w->change_online_table(id,HostUser->getName(),peerIP);

                               out<<tr("RES_LOG");           //登录成功，发送用户的信息
                               out<<4;
                               out<<HostUser->getName();

                               int number_questions=questMap.size();
                               out<<number_questions;

                               int count;
                               if(number_questions>=11)
                                   count=11;
                               else
                                   count=number_questions;

                               out<<count;
                               QuestInfor *ques;
                               int i=0;

                               QMap<QString,QuestInfor*>::const_iterator it=questMap.end()-1;
                               while(it!=questMap.begin()-1 && i<count)
                               {
                                   ques=it.value();
                                   out<<ques->getID();
                                   QString id_of_creator=ques->getIDIDofCreator();
                                   QString name_of_creator=ID_Name.find(id_of_creator).value();
                                   out<<name_of_creator;

                                   out<<ques->gettitle();
                                   out<<ques->get_popular_value();

                                   --it;
                                   ++i;
                               }

                               QList<QString> myquest = HostUser->getMyQuestion();

                               int number_myquestions;
                               number_myquestions = myquest.size();

                               out<<number_myquestions;

                               if(number_myquestions>=11)
                                   count=11;
                               else
                                   count = number_myquestions;
                               out<<count;

                               i=number_myquestions-1;

                               while(i>=number_myquestions-count)
                               {

                                   QString id=myquest.value(i);
                                   QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
                                   if(iter!=questMap.end())
                                   {
                                       ques=iter.value();

                                       out<<ques->getID();
                                       out<<ques->gettitle();
                                       out<<ques->get_popular_value();
                                   }
                                   --i;
                               }

                               QList<QString> focus=HostUser->getFocusList();

                               int number_myfocus(focus.size());
                               out<<number_myfocus;

                               if(number_myfocus>=11)
                                   count=11;
                               else
                                   count=number_myfocus;
                               out<<count;

                               for(i=0;i<count;++i)
                               {
                                   QString id=focus.value(i);
                                   QString name=ID_Name.find(id).value();

                                   out<<id;
                                   out<<name;
                               }

                               tcpSocket->write(block);

                        }
                    }

                }

            }

         }

    }

    if(newsType=="REGISTER")        //注册请求到来
    {

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        QString id,name,password;
        in>>id>>name>>password;

        {
            myMutex locker(&mutex);

            QMap<QString,QString>::iterator it=ID_Password.find(id);
            if(it!=ID_Password.end())          //注册失败，该账号已存在
            {
                out<<tr("RES_REGISTE");
                out<<0;
                tcpSocket->write(block);
            }
            else
            {
                online_set.insert(id);
                QString peerIP=tcpSocket->peerAddress().toString();

                w->change_online_table(id,name,peerIP);

                QFile files("Data\\"+id+".txt");
                if(!files.open(QIODevice::WriteOnly|QIODevice::Text))
                {
                      QMessageBox *Inform3=new QMessageBox;
                      Inform3->setText(id+".txt 文件打开失败");
                      Inform3->show();
                }
                else                                //注册成功
                {
                        QTextStream outtext1(&files);
                        outtext1<<id<<"   "<<password<<"  "<<name<<"\r\n";
                        outtext1<<0<<"\r\n"<<0;

                        files.close();

                        files.setFileName("Data\\IDPassword.txt");
                        if(!files.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
                        {
                            QMessageBox *Inform4=new QMessageBox;
                            Inform4->setText("文件 IDPassword.txt 打开失败");
                            Inform4->show();
                        }
                        else
                        {
                            QTextStream out1(&files);
                            out1<<id<<"   "<<password<<"  "<<name<<"\n\r";
                            files.close();
                        }

                        ID_Password.insert(id,password);

                        ID_Name.insert(id,name);

                        out<<tr("RES_REGISTE");
                        out<<1;
                        out<<name;

                        int number_questions=questMap.size();
                        out<<number_questions;

                        int count;
                        if(number_questions>=11)
                            count=11;
                        else
                            count=number_questions;

                        out<<count;
                        QuestInfor *ques;
                        int i=0;
                        QMap<QString,QuestInfor*>::const_iterator it=questMap.end()-1;
                        while(it!=questMap.begin()-1 && i<count)
                        {
                            ques=it.value();
                            out<<ques->getID();
                            QString id_of_creator=ques->getIDIDofCreator();
                            QString name_of_creator=ID_Name.find(id_of_creator).value();
                            out<<name_of_creator;

                            out<<ques->gettitle();
                            out<<ques->get_popular_value();

                            --it;
                            ++i;
                        }

                        out<<0;
                        out<<0;

                        tcpSocket->write(block);

                        HostUser=new user(id,name,password);
                }
            }

        }
    }

    if(newsType == "EXIT")             //退出登录请求到来，
    {

        {
            myMutex locker(&mutex);
            QString str="Data\\"+HostUser->getID()+".txt";
            QFile file1(str);

            try{
                if(!file1.open(QIODevice::WriteOnly))
                    throw QString("用户信息文件丢失");
                QTextStream out(&file1);

                out<<HostUser;

                file1.close();
            }
            catch(QString message)
            {
                QMessageBox *msg=new QMessageBox;
                msg->setText(message);
                msg->show();
            }

            online_set.remove(HostUser->getID());

            QString peerIP=tcpSocket->peerAddress().toString();

            w->change_online_table(HostUser->getID(),HostUser->getName(),peerIP);
            delete HostUser;

            isRun = 0;
        }
    }

    if(newsType == "CHANGEPASSWORD")     //修改密码请求到来，给用户回复信息
    {

        QString old_password;
        QString new_password;

        in>>old_password;
        in>>new_password;

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_CHANGEPASSWORD");

        bool ischanged=HostUser->changePassword(old_password,new_password);
        if(ischanged)
            out<<1;
        else
            out<<0;
        tcpSocket->write(block);
    }

    if(newsType == "UPDATE")         // 更新用户端的问题列表
    {
        QString way;
        in>>way;
        if(way == "TIME")
        {
            {
                myMutex locker(&mutex);
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);

                out<<tr("RES_UPDATE");
                int number_questions=questMap.size();
                out<<number_questions;

                int count;
                if(number_questions>=11)
                    count=11;
                else
                    count=number_questions;

                out<<count;
                QuestInfor *ques;
                int i=0;
                QMap<QString,QuestInfor*>::const_iterator it=questMap.end()-1;
                while(i<count)
                {
                    ques=it.value();

                    out<<ques->getID();
                    QString id_of_creator=ques->getIDIDofCreator();
                    QString name_of_creator=ID_Name.find(id_of_creator).value();
                    out<<name_of_creator;

                    out<<ques->gettitle();
                    out<<ques->get_popular_value();

                    --it;
                    ++i;
                }
                tcpSocket->write(block);
            }
        }
        else
        {
            {
                myMutex locker(&mutex);

                qSort(popular_value_vec.begin(),popular_value_vec.end(),compare);
                value_vec=popular_value_vec;

                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);

                out<<tr("RES_UPDATE");

                int vecSize=value_vec.size();
                out<<vecSize;
                int count;
                if(vecSize>=11)
                    count=11;
                else
                    count=vecSize;
                out<<count;

                QuestInfor *ques;
                for(int i=0;i<count;++i)
                {
                    ques=value_vec.value(i);
                    out<<ques->getID();

                    QString id_of_creator=ques->getIDIDofCreator();
                    QString name_of_creator=ID_Name.find(id_of_creator).value();
                    out<<name_of_creator;

                    out<<ques->gettitle();
                    out<<ques->get_popular_value();
                }
                tcpSocket->write(block);
            }
        }

    }

    if(newsType == "UPDATE_MYQUESTION")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("MY_QUESTIONS");

        QList<QString> myquest = HostUser->getMyQuestion();
        int listsize = myquest.size();
        out<<listsize;

        int count;
        if(listsize>=11)
            count=11;
        else
            count=listsize;
        out<<count;
        {
            myMutex locker(&mutex);

            int i=listsize-1;
            QuestInfor *ques;
            while(i>=listsize-count)
            {

                QString id=myquest.value(i);
                QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
                if(iter!=questMap.end())
                {
                    ques=iter.value();
                    out<<ques->getID();
                    out<<ques->gettitle();
                    out<<ques->get_popular_value();
                }
                --i;
            }
            tcpSocket->write(block);
        }
    }

    if(newsType == "UPDATE_MYFOCUS")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("MY_FOCUS");

        QList<QString> focus=HostUser->getFocusList();
        int listSize=focus.size();

        out<<listSize;
        int count;
        if(listSize>=11)
            count=11;
        else
            count=listSize;
        out<<count;

        {
            myMutex locker(&mutex);
            for(int i=0;i<count;++i)
            {
                QString id=focus.value(i);
                QString name=ID_Name.find(id).value();

                out<<id;
                out<<name;
            }
            tcpSocket->write(block);
        }
    }

    if(newsType == "CHANGEPAGE_1")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_CP_QUES");

        int currentpage;
        in>>currentpage;

        int display_way;
        in>>display_way;

        if(display_way == 0)
        {
            {
                myMutex locker(&mutex);

                QMap<QString,QuestInfor*>::const_iterator it=questMap.end()-1-(currentpage-1)*11;
                int i=0;
                while(it!=questMap.begin()-1&&i<11)
                {
                        QuestInfor* question=it.value();
                        QString id=question->getID();
                        QString id_of_creator=question->getIDIDofCreator();
                        QString name_of_creator=ID_Name.find(id_of_creator).value();

                        out<<id;
                        out<<name_of_creator;
                        out<<question->gettitle();
                        out<<question->get_popular_value();

                        it--;
                        ++i;
                }

            }
        }
        else
        {
            {
                myMutex locker(&mutex);

                int vecSize=value_vec.size();

                int pos=(currentpage-1)*11;

                int i=0;

                while(pos<vecSize&&i<11)
                {
                    QuestInfor* question=value_vec.value(pos);
                    QString id=question->getID();
                    QString id_of_creator=question->getIDIDofCreator();
                    QString name_of_creator=ID_Name.find(id_of_creator).value();
                    QString title=question->gettitle();

                    out<<id;
                    out<<name_of_creator;
                    out<<title;
                    out<<question->get_popular_value();

                    pos++;
                    ++i;
                }
            }
        }
        tcpSocket->write(block);
    }

    if(newsType == "CHANGEPAGE_2")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_CP_MYQUES");

        int currentpage;
        in>>currentpage;


        {
            myMutex locker(&mutex);

            QList<QString> myquest = HostUser->getMyQuestion();

            int i = 0;
            int it = myquest.size()-1-(currentpage-1)*11;

            while(it>=0&&i<11)
            {

                QString id=myquest.value(it);
                QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
                if(iter!=questMap.end())
                {
                    QuestInfor *currentQuestion=iter.value();

                    if(currentQuestion!=NULL)
                    {
                        out<<currentQuestion->getID();
                        out<<currentQuestion->gettitle();
                        out<<currentQuestion->get_popular_value();

                    }
                }
                --it;
                ++i;
            }
            tcpSocket->write(block);
        }
    }

    if(newsType == "CHANGEPAGE_3")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_CP_MYQUES");

        int currentpage;
        in>>currentpage;

        {
            myMutex locker(&mutex);

            QList<QString> focus=HostUser->getFocusList();

            int it=(currentpage-1)*11;

            int i=0;
            while(it<focus.size()&&i<11)
            {
                QString id=focus.value(it);
                QString name=ID_Name.find(id).value();

                out<<id;
                out<<name;
                it++;
                i++;
            }
            tcpSocket->write(block);
        }
    }

    if(newsType == "DISPLAYQUESTION")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_QUES");

        QString id_question;
        in>>id_question;

        QuestInfor* currentQuestion;

        {
            myMutex locker(&mutex);

            currentQuestion=questMap.find(id_question).value();

            out<<currentQuestion->getID();
            QString id_creator( currentQuestion->getIDIDofCreator() );

            out<<id_creator;

            QString name_creator=ID_Name.find(id_creator).value();
            out<<name_creator;
        }

        out<<currentQuestion->getTimeofCreate();

        if(HostUser->isFocus(currentQuestion->getIDIDofCreator()))
            out<<1;
        else
            out<<0;

        out<<currentQuestion->gettitle();
        out<<currentQuestion->getcontent();

        QList<AnswerInfor*> allAnswer=currentQuestion->getAnswer();
        int size=allAnswer.size();
        out<<size;

        for(int i=0;i<size;++i)
        {
            AnswerInfor* currentAnswer=allAnswer.value(i);

            out<<currentAnswer->getID();

            QString id_creator=currentAnswer->getIDIDofCreator();
            {
                myMutex locker(&mutex);
                QString name_creator=ID_Name.find(id_creator).value();

                out<<name_creator;
                out<<currentAnswer->getcontent();
                out<<currentAnswer->getpraiseNum();
            }
        }
        tcpSocket->write(block);
    }

    if(newsType == "DISPLAYUSER")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_USER");

        QString id_user;
        in>>id_user;
        {
            myMutex locker(&mutex);

            user* currentUser=new user;

            if( LoadUser(id_user,currentUser) )
            {
                out<<id_user;
                out<<currentUser->getName();

                if(HostUser->isFocus(id_user))
                    out<<1;
                else
                    out<<0;

                QList<QString> myquest = currentUser->getMyQuestion();

                int nQuestion=myquest.size();
                out<<nQuestion;

                int it = nQuestion-1;
                for(;it>=0;--it)
                {
                    QString id=myquest.value(it);
                    QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
                    if(iter!=questMap.end())
                    {
                        QuestInfor *currentQuestion=iter.value();

                        if(currentQuestion!=NULL)
                        {
                            out<<currentQuestion->getID();
                            out<<currentQuestion->gettitle();
                            out<<currentQuestion->get_popular_value();
                        }

                    }
                }

                QList<QString> quest = currentUser->get_question_my_anwser();
                nQuestion=quest.size();
                out<<nQuestion;

                for(int it = nQuestion-1;it>=0;--it)
                {

                      QString id=quest.value(it);
                      QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);

                      if(iter!=questMap.end())
                      {

                           QuestInfor *currentQuestion=iter.value();

                           if(currentQuestion!=NULL)
                           {
                               out<<currentQuestion->getID();

                               QString id_of_creator=currentQuestion->getIDIDofCreator();
                               QString name_of_creator=ID_Name.find(id_of_creator).value();

                               out<<name_of_creator;
                               out<<currentQuestion->gettitle();
                               out<<currentQuestion->get_popular_value();
                           }

                      }
                 }

                tcpSocket->write(block);
            }

        }
    }

    if(newsType == "SEARCH")
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_SEARCH");

        QString search_way;
        in>>search_way;

        QString key;
        in>>key;

        {
            myMutex locker(&mutex);

            QVector<QuestInfor*> result;
            if(search_way == "TIME")
            {
                result=find_question(key);
            }
            else
            {
                key=key.simplified();
                QStringList strlist=key.split(" ");
                result=find_question(strlist);
            }


            qSort(result.begin(),result.end(),compare);

            int count=result.size();
            out<<count;

            for(int i=0;i<count;++i)
            {
                QuestInfor* question=result.value(i);
                QString id=question->getID();
                QString id_of_creator=question->getIDIDofCreator();
                QString name_of_creator=ID_Name.find(id_of_creator).value();
                QString title=question->gettitle();

                out<<id;
                out<<name_of_creator;
                out<<title;
                out<<question->get_popular_value();
            }

            tcpSocket->write(block);
        }
    }

    if(newsType == "ASK")
    {

        QString title;
        in>>title;
        QString content;
        in>>content;



        {
            myMutex locker(&mutex);

            HostUser->AddQuest(title,content);

            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<tr("ADD_QUESTION");

            out<<1;

            tcpSocket->write(block);

            QString str="Data\\"+HostUser->getID()+".txt";   //将当前用户信息写回文件内
            QFile file1(str);

            try{
                if(!file1.open(QIODevice::WriteOnly))
                    throw QString("用户信息文件丢失");
                QTextStream out(&file1);

                out<<HostUser;

                file1.close();
            }
            catch(QString message)
            {
                QMessageBox *msg=new QMessageBox;
                msg->setText(message);
                msg->show();
            }
        }
    }

    if(newsType == "ADD_ANSWER")
    {
        QString id_question;
        in>>id_question;

        QString content;
        in>>content;

        {
            myMutex locker(&mutex);

            QDateTime dt=QDateTime::currentDateTime();
            QString time=dt.toString("yyyy-MM-dd-hh:mm:ss");
            QString id=dt.toString("yyyyMMddhhmmss");

            QuestInfor *question=questMap.find(id_question).value();

            if(question!=NULL)
            {
                question->CreateAnswer(id,HostUser->getID(),time,content,0);
                HostUser->AddAnswer(id_question);
                question->add_popular_value();

                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);

                out<<tr("RES_ADDANSWER");
                out<<1;
                out<<id;
                out<<HostUser->getName();
                out<<content;

                tcpSocket->write(block);


                //将当前用户信息写回文件夹内

                QString str="Data\\"+HostUser->getID()+".txt";
                QFile file1(str);

                try{
                    if(!file1.open(QIODevice::WriteOnly))
                        throw QString("用户信息文件丢失");
                    QTextStream out(&file1);

                    out<<HostUser;

                    file1.close();
                }
                catch(QString message)
                {
                    QMessageBox *msg=new QMessageBox;
                    msg->setText(message);
                    msg->show();
                }

            }
        }
    }

    if(newsType == "UPDATE_ANSWERS")
    {
        QString id_question;
        in>>id_question;

        {
            myMutex locker(&mutex);

            QuestInfor *question=questMap.find(id_question).value();

            if(question!=NULL)
            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<tr("RES_ANSWERS");

                QList<AnswerInfor*> allAnswer=question->getAnswer();

                int count=allAnswer.size();
                out<<count;
                for(int i=0;i<count;++i)
                {
                    AnswerInfor* currentAnswer=allAnswer.value(i);

                    out<<currentAnswer->getID();

                    QString id_creator=currentAnswer->getIDIDofCreator();
                    QString name_creator=ID_Name.find(id_creator).value();
                    out<<name_creator;

                    out<<currentAnswer->getcontent();
                    out<<currentAnswer->getpraiseNum();
                }


                tcpSocket->write(block);
            }
        }
    }

    if(newsType == "DISPLAYANSWER")
    {
        QString id_question;
        in>>id_question;

        int index;
        in>>index;

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_ANS");
        {
            myMutex locker(&mutex);

            QuestInfor *question = questMap.find(id_question).value();
            if(question!=NULL)
            {
                QList<AnswerInfor*> answers=question->getAnswer();

                AnswerInfor* current_answer=answers.value(index);



                out<<id_question;
                QString id=current_answer->getID();
                out<<id;

                QString id_creator=current_answer->getIDIDofCreator();
                QString name_creator=ID_Name.find(id_creator).value();

                out<<id_creator;
                out<<name_creator;
                out<<current_answer->getTimeofCreate();

                if(HostUser->isFocus(id_creator))
                    out<<1;
                else
                    out<<0;

                if(current_answer->isPraise(HostUser->getID()))
                    out<<1;
                else
                    out<<0;

                out<<current_answer->getpraiseNum();
                out<<current_answer->getcontent();

                tcpSocket->write(block);

            }

        }
    }

    if(newsType == "FOCUS")
    {
        QString id_user;
        in>>id_user;

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out<<tr("RES_FOCUS");

        bool isSuccess = HostUser->AddFocus(id_user);
        out<<id_user;

        if(isSuccess)
            out<<1;
        else
            out<<0;
        tcpSocket->write(block);
    }

    if(newsType == "PRAISE")
    {
        QString id_question;
        in>>id_question;

        QString id_answer;
        in>>id_answer;

        {
            myMutex locker(&mutex);
            QuestInfor *ques=questMap.find(id_question).value();

            AnswerInfor *answer=ques->find_answer(id_answer);
            if(answer!=NULL)
            {
                answer->addPraise(HostUser->getID());
                ques->add_popular_value();

                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<tr("RES_PRAISE");
                out<<id_answer;
                out<<1;
                out<<answer->getpraiseNum();

                tcpSocket->write(block);
            }
        }
    }

    if(newsType == "UPDATE_ASK")
    {
        QString id_user;
        in>>id_user;

        {
            myMutex locker(&mutex);
            user* currentUser=new user;

            if( LoadUser(id_user,currentUser) )
            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<tr("RES_USER_ASK");

                QList<QString> myquest = currentUser->getMyQuestion();

                int it = myquest.size()-1;
                out<<it+1;

                for(;it>=0;--it)
                {
                    QString id=myquest.value(it);
                    QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
                    if(iter!=questMap.end())
                    {
                        QuestInfor *currentQuestion=iter.value();

                        if(currentQuestion!=NULL)
                        {
                            out<<currentQuestion->getID();
                            out<<currentQuestion->gettitle();
                            out<<currentQuestion->get_popular_value();
                        }
                    }
                }

                tcpSocket->write(block);
            }
        }
    }

    if(newsType == "UPDATE_REPLY")
    {
        QString id_user;
        in>>id_user;

        {
            myMutex locker(&mutex);
            user* currentUser=new user;

            if( LoadUser(id_user,currentUser) )
            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<tr("RES_USER_REPLY");

                QList<QString> quest = currentUser->get_question_my_anwser();

                int it = quest.size()-1;
                out<<it+1;

                for(;it>=0;--it)
                {
                    QString id=quest.value(it);
                    QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
                    if(iter!=questMap.end())
                    {
                        QuestInfor *currentQuestion=iter.value();

                        if(currentQuestion!=NULL)
                        {
                            out<<currentQuestion->getID();
                            QString id_creator=currentQuestion->getIDIDofCreator();
                            QString name_creator=ID_Name.find(id_creator).value();

                            out<<name_creator;
                            out<<currentQuestion->gettitle();
                            out<<currentQuestion->get_popular_value();
                        }
                    }
                }

                tcpSocket->write(block);
            }
        }
    }
}


//加载某一用户的个人信息
bool LoadUser(QString filetoOpen,user* client)
{
    QString str="Data\\"+filetoOpen+".txt";
    QFile file1(str);
    try{

        if(!file1.open(QIODevice::ReadOnly))
            throw QString("文件打开失败");
        if(!file1.atEnd())
        {
            QTextStream in(&file1);

            in>>client;
        }
        else
        {
            throw QString("文件空白，用户信息丢失");
        }
    }
    catch(QString message)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText(message);
        msg->exec();
        return false;
    }

    file1.close();

    return true;
}
