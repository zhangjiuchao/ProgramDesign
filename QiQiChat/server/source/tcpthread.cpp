#include "tcpthread.h"
#include <QtGui>
#include "chatserver.h"
#include "mymutex.h"
#include <QSqlQuery>
#include <QMessageBox>


//Ϊĳһ�ͻ��˷�����߳�



TcpThread::TcpThread(int socketDiscriptor,QObject *parent)
    : QThread(parent),socketDescriptor(socketDiscriptor)
{
    timer = new QTimer;
    timer->setInterval(5000);      //���ø���ʱ����Ϊ5��
    timer->start();

    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

}



void TcpThread::run()
{

    tcpSocket = new QTcpSocket;

    if(!tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket->error());
    }
    else
    {

        connect(tcpSocket,SIGNAL(readyRead()),
                this,SLOT(receiveMessage()),Qt::BlockingQueuedConnection);
        exec();
    }
}



void TcpThread::update()
{

    {
        myMutex locker(&mutex);  //�����µĿ飬����һ�����������뿪�����ʱ���������Զ����

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);

        out<<tr("UPDATE");

        QSqlQuery query(db);     //�������ݿ�

        query.exec("select * from User where account="+peer_account);

        if(query.first())
        {
            int friends_num = query.value(6).toInt();

            for(int i=1;i<=friends_num;++i)                 //����¼�û��ĺ�����Ϣ���͸��û�
            {
                int pos = 6+i;
                QString friend_account=query.value(pos).toString();

                QSqlQuery query_1(db);
                query_1.exec("select account,name,isOnline from User where account="+friend_account);
                while(query_1.next())
                {
                    out<<query_1.value(0).toString();
                    out<<query_1.value(1).toString();
                    out<<query_1.value(2).toInt();
                }
            }

            tcpSocket->write(block);
        }
    }
}



QTcpSocket* TcpThread::getSocket()
{
    return tcpSocket;
}

void TcpThread::receiveMessage()
{
    QDataStream in(tcpSocket);

    QString ip_peer=tcpSocket->peerAddress().toString();

    in.setVersion(QDataStream::Qt_4_6);
    QString newsType;
    in>>newsType;

    if(newsType== "LOG")           //��¼������
    {

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        QString id,password;
        in>>id>>password;
        {
            myMutex locker(&mutex);
            QSqlQuery query(db);

            if(query.exec("select * from User where account="+id))    //�������ݿ���id��Ӧ���û���Ϣ
            {


                if(query.first())
                {
                    QString account=query.value(1).toString();
                    QString name=query.value(2).toString();
                    QString password_1=query.value(3).toString();

                    if(password_1!=password)        //������󣬵�¼ʧ��
                    {
                        out<<tr("RESPONSEFORLOG");
                        out<<tr("FAIL");
                        tcpSocket->write(block);
                    }

                    else
                    {
                        out<<tr("RESPONSEFORLOG");           //��¼�ɹ��������û�����Ϣ
                        out<<tr("SUCCESS");

                        peer_account=id;

                        out<<account<<name;

                        QSqlQuery query_0(db);


                        query_0.prepare("update User set isOnline=:isOline ,IP=:IP where account="+id);

                        query_0.bindValue(":isOnline",1);
                        query_0.bindValue(":IP",ip_peer);
                        query_0.exec();

                        query.exec("select * from User where account="+id);
                        if(query.first())
                        {
                            int friends_num=query.value(6).toInt();    //�����û��ĺ�����Ϣ

                            for(int i=1;i<=friends_num;++i)
                            {
                                int pos=6+i;
                                QString friend_account=query.value(pos).toString();
                                QSqlQuery query_1(db);
                                query_1.exec("select account,name,isOnline from User where account="+friend_account);
                                while(query_1.next())
                                {
                                    out<<query_1.value(0).toString();
                                    out<<query_1.value(1).toString();
                                    out<<query_1.value(2).toInt();
                                }
                            }
                        }


                        tcpSocket->write(block);
                    }
                }
                else
                {
                    out<<tr("RESPONSEFORLOG");
                    out<<tr("FAIL");
                    tcpSocket->write(block);
                }
            }

        }

    }


    if(newsType=="REGISTER")        //ע��������
    {

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        QString id,name,password;
        in>>id>>name>>password;

        {
            myMutex locker(&mutex);
            QSqlQuery query(db);

            query.exec("select IP from User where account="+id);


            if(query.first())       //�˺�id�ѱ�ע�ᣬע��ʧ��
            {
                out<<tr("RESPONSEFORREGISTER");
                out<<tr("FAIL");
                tcpSocket->write(block);
            }
            else                  //ע��ɹ����������ݿ⣬�����û���ʾ��Ϣ
            {
                query.prepare("insert into User(account,name,password,isOnline,IP,friendsnum)"
                              "values(:account,:name,:password,:isOnline,:IP,:friendsnum)");
                query.bindValue(":account",id);
                query.bindValue(":name",name);
                query.bindValue(":password",password);
                query.bindValue(":isOnline",1);
                query.bindValue(":IP",ip_peer);
                query.bindValue(":friendsnum",0);
                if(!query.exec())
                {
                    out<<tr("RESPONSEFORREGISTER");
                    out<<tr("FAIL");
                    tcpSocket->write(block);
                }
                else
                {
                    out<<tr("RESPONSEFORREGISTER");
                    out<<tr("SUCCESS");

                    peer_account = id;

                    tcpSocket->write(block);
                }
            }

        }
    }


    if(newsType == "EXIT")             //�˳���¼�������������ݿ��� id��״̬��Ϊ����
    {
        QString account;
        in>>account;
        {
            myMutex locker(&mutex);
            QSqlQuery query(db);
            query.exec("update User set isOnline=0 where account="+account);
            tcpSocket->abort();
        }

    }


    if(newsType == "TALK")           //�������쵽���������ݿ������������IP���ظ������û�
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);

        QString id_user,id_friend;
        in>>id_user>>id_friend;

        peer_account = id_user;

        {
            myMutex locker(&mutex);
            QSqlQuery query(db);
            if(query.exec("select IP,name,isOnline from User where account="+id_friend))
            {
                if(query.first())
                {
                    QString ip_friend = query.value(0).toString();
                    QString name_friend = query.value(1).toString();
                    int online = query.value(2).toInt();

                    out<<tr("RESPONSEFORCOMMUNICATION");
                    out<<ip_friend;
                    out<<id_friend;
                    out<<name_friend;
                    out<<online;
                    tcpSocket->write(block);
                }
            }
        }

    }


    if(newsType == "CHANGENAME")       //�޸��ǳ����������������ݿ⣬���û�������Ϣ
    {
        QString account,newName;
        in>>account>>newName;

        peer_account = account;

        {
            myMutex locker(&mutex);
            QSqlQuery query(db);

            query.prepare("update User set name=:name where account="+peer_account);
            query.bindValue(":name",newName);
            if(query.exec())
            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);

                out<<tr("RESPONSEFORCHANGENAME");
                out<<tr("SUCCESS");
                tcpSocket->write(block);
            }
            else
            {
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);

                out<<tr("RESPONSEFORCHANGENAME");
                out<<tr("FAIL");
                tcpSocket->write(block);
            }
        }
    }


    if(newsType == "CHANGEPASSWORD")     //�޸��������������������ݿ⣬���û��ظ���Ϣ
    {
        QString old_password;
        QString new_password;
        QString account;
        in>>account;
        in>>old_password;
        in>>new_password;
        peer_account = account;

        {
            myMutex locker(&mutex);
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<tr("RESPONSEFORCHANGEPASSWORD");

            QSqlQuery query(db);

            if(query.exec("select password from User where account="+peer_account))
            {
                if(query.first())
                {
                    if(query.value(0).toString() == old_password)
                    {
                        QSqlQuery query_1(db);
                        query_1.prepare("update User set password=:password where account="+peer_account);
                        query_1.bindValue(":password",new_password);
                        if(query_1.exec())
                        {
                            out<<tr("SUCCESS");
                        }
                        else
                        {
                            out<<tr("FAIL");
                        }
                    }
                    else
                    {
                        out<<tr("FAIL");
                    }
                }
                else
                {
                    out<<tr("FAIL");
                }
            }
            else
            {
                out<<tr("FAIL");
            }
            tcpSocket->write(block);
        }

    }


    if(newsType == "FRIENDREQUEST")        //����������������Ϣ���͸�����Ķ���
    {
        QString friend_id;
        QString user_id;
        in>>friend_id>>user_id;

        peer_account = user_id;

        emit friend_request(friend_id,user_id);
    }


    if(newsType == "RESPONSEFORFRIEND")        //��������ظ�������������ͨ������������ݿ⣬�������û��ظ���Ϣ��
    {                                          //�����󱻾ܾ�����������û��ظ���Ϣ
        QString friend_id;
        QString user_id;
        QString message;

        peer_account = user_id;

        in>>friend_id>>user_id>>message;
        if(message == "OK")
        {
            {
                myMutex locker(&mutex);

                QSqlQuery query(db);


                query.exec("select friendsnum from User where account="+user_id);

                if(query.first())
                {
                    int number_friends = query.value(0).toInt()+1;
                    QString str = "friend_";
                    str=str+QString::number(number_friends);

                    query.prepare("update User set friendsnum=:friendsnum where account="+user_id);
                    query.bindValue(":friendsnum",number_friends);
                    query.exec();
                    query.exec("update User set "+str+"="+friend_id+" where account="+user_id);

                }

                QSqlQuery query0(db);
                query0.exec("select friendsnum from User where account="+friend_id);
                if(query0.first())
                {
                    int number_friends = query0.value(0).toInt()+1;
                    QString str = "friend_";
                    str = str+QString::number(number_friends);

                    query0.prepare("update User set friendsnum=:friendsnum where account="+friend_id);
                    query0.bindValue(":friendsnum",number_friends);
                    query0.exec();
                    query0.exec("update User set "+str+"="+user_id+" where account="+friend_id);
                }

            }
        }

        emit friend_response(friend_id,user_id,message);
    }

}



void TcpThread::friendrequest(QString friend_id, QString user_id) //������ڱ��û��ĺ�������
{
    if(friend_id == peer_account)
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);

        {
            myMutex locker(&mutex);

            QSqlQuery query(db);
            out<<tr("FRIENDREQUEST");
            query.exec("select name,isOnline from User where account="+user_id);
            if(query.first())
            {
                QString user_name = query.value(0).toString();

                int online = query.value(1).toInt();
                out<<user_id;
                out<<user_name;
                out<<online;
            }
            tcpSocket->write(block);
        }

    }
}



void TcpThread::friendresponse(QString friend_id, QString user_id, QString message)
{
    if(friend_id == peer_account)                   //����Ա��û��ĺ��������Ӧ
    {
        if(message == "OK")
        {
            {
                myMutex locker(&mutex);
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out<<tr("FRIENDRESPONSE");
                out<<tr("OK");

                QSqlQuery query(db);

                query.exec("select name,isOnline from User where account="+user_id);
                if(query.first())
                {
                    QString name = query.value(0).toString();
                    int online=query.value(1).toInt();

                    out<<user_id<<name<<online;

                    tcpSocket->write(block);
                }         
            }
        }
        else
        {
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out<<tr("FRIENDRESPONSE");
            out<<tr("NO");
            tcpSocket->write(block);
        }
    }
}
