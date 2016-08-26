
//TCP ģ��

#include "tcpsocket.h"
#include "QAS_client.h"
#include <QMessageBox>
#include <mainwindow.h>



TcpSocket::TcpSocket(QObject *parent) :
    QObject(parent)
{
    tcpsocket = new QTcpSocket;
    block.clear();

    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage()));

}


//socket ������Ϣ����������Ϣͷ���ı�־����ʶ�����������ĳһ�����Ļ�Ӧ����������Ӧ���źţ�֪ͨĳ���ڽ��н��ս��

void TcpSocket::readMessage()    //��������Ϣ
{
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_4_6);

    QString messageType;
    in>>messageType;

    block=tcpsocket->readAll();


    //�ж���Ϣ�����ͣ�������Ϣ�����ͷ��Ͳ�ͬ���źţ�����Ӧ�Ĵ��ڽ��գ�������Ӧ����


    if(messageType == "RES_LOG")   //�Ե�¼�Ļ�Ӧ
        emit response_for_log();

    else if(messageType == "RES_REGISTE") //��ע��Ļ�Ӧ
        emit response_for_registe();

    else if(messageType == "RES_CHANGEPASSWORD")  //�������޸ĵĻ�Ӧ
        emit response_for_changepassword();

    else if(messageType == "MY_QUESTIONS")          //�Ը��������б�Ļ�Ӧ
        emit response_for_myquestion();

    else if(messageType == "MY_FOCUS")       //�Ը��¹�ע�б�Ļ�Ӧ
        emit response_for_myfocus();

    else if(messageType == "ADD_QUESTION")           //�����ʵĻ�Ӧ
        emit response_for_ask();

    else if(messageType == "RES_SEARCH")           //�������Ļ�Ӧ
        emit response_for_search();

    else if(messageType == "RES_USER")          //�Բ鿴��ע�û��Ļ�Ӧ
        emit response_for_user();

    else if(messageType == "RES_QUES")          //�Բ鿴����Ļ�Ӧ
        emit response_for_select_question();

    else if(messageType == "RES_ANS")           //�Բ鿴�ش�Ļ�Ӧ
        emit response_for_select_answer();

    else if(messageType == "RES_UPDATE")         //��ˢ�������б�Ļ�Ӧ
        emit response_for_updatequestions();

    else if(messageType == "RES_CP_QUES")       //�������б�ҳ�Ļ�Ӧ
        emit response_for_changepage_question();

    else if(messageType == "RES_CP_MYQUES")       //���ҵ������б�ҳ�Ļ�Ӧ
        emit response_for_changepage_myquestion();

    else if(messageType == "RES_CP_FOCUS")            // ���ҵĹ�ע�б�ҳ�Ļ�Ӧ
        emit response_for_changepage_myfocus();

    else if(messageType == "RES_USER_ASK")
        emit response_for_update_askque();          //��ˢ���û��������б�Ļ�Ӧ

    else if(messageType == "RES_USER_REPLY")        //��ˢ���û��Ļش��б�Ļ�Ӧ
        emit response_for_update_replyque();

    else if(messageType == "RES_FOCUS")            //�Թ�ע�û��Ļ�Ӧ
        emit response_for_focus();

    else if(messageType == "RES_ANSWERS")           //��ˢ������Ļش��б�Ļ�Ӧ
        emit response_for_answers();

    else if(messageType == "RES_ADDANSWER")          //����ӻش�Ļ�Ӧ
        emit response_for_addanswer();

    else if(messageType == "RES_PRAISE")               //�Ե��޵Ļ�Ӧ
        emit response_for_praise();

}



void TcpSocket::sendMessage()       // socket ������Ϣ
{
    tcpsocket->write(block);
}



void TcpSocket::newConnect()        //�����ӣ����ӷ�����
{
    tcpsocket->abort();

    tcpsocket->connectToHost(IPofServer,PortofServer);
}


