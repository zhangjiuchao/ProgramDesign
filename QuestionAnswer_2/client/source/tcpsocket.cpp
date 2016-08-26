
//TCP 模块

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


//socket 接收消息，并区分消息头部的标志符，识别出服务器对某一操作的回应，并发出相应的信号，通知某窗口进行接收结果

void TcpSocket::readMessage()    //接收新消息
{
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_4_6);

    QString messageType;
    in>>messageType;

    block=tcpsocket->readAll();


    //判断消息的类型，根据消息的类型发送不同的信号，供相应的窗口接收，进行相应操作


    if(messageType == "RES_LOG")   //对登录的回应
        emit response_for_log();

    else if(messageType == "RES_REGISTE") //对注册的回应
        emit response_for_registe();

    else if(messageType == "RES_CHANGEPASSWORD")  //对密码修改的回应
        emit response_for_changepassword();

    else if(messageType == "MY_QUESTIONS")          //对更新问题列表的回应
        emit response_for_myquestion();

    else if(messageType == "MY_FOCUS")       //对更新关注列表的回应
        emit response_for_myfocus();

    else if(messageType == "ADD_QUESTION")           //对提问的回应
        emit response_for_ask();

    else if(messageType == "RES_SEARCH")           //对搜索的回应
        emit response_for_search();

    else if(messageType == "RES_USER")          //对查看关注用户的回应
        emit response_for_user();

    else if(messageType == "RES_QUES")          //对查看问题的回应
        emit response_for_select_question();

    else if(messageType == "RES_ANS")           //对查看回答的回应
        emit response_for_select_answer();

    else if(messageType == "RES_UPDATE")         //对刷新问题列表的回应
        emit response_for_updatequestions();

    else if(messageType == "RES_CP_QUES")       //对问题列表换页的回应
        emit response_for_changepage_question();

    else if(messageType == "RES_CP_MYQUES")       //对我的问题列表换页的回应
        emit response_for_changepage_myquestion();

    else if(messageType == "RES_CP_FOCUS")            // 对我的关注列表换页的回应
        emit response_for_changepage_myfocus();

    else if(messageType == "RES_USER_ASK")
        emit response_for_update_askque();          //对刷新用户的提问列表的回应

    else if(messageType == "RES_USER_REPLY")        //对刷新用户的回答列表的回应
        emit response_for_update_replyque();

    else if(messageType == "RES_FOCUS")            //对关注用户的回应
        emit response_for_focus();

    else if(messageType == "RES_ANSWERS")           //对刷新问题的回答列表的回应
        emit response_for_answers();

    else if(messageType == "RES_ADDANSWER")          //对添加回答的回应
        emit response_for_addanswer();

    else if(messageType == "RES_PRAISE")               //对点赞的回应
        emit response_for_praise();

}



void TcpSocket::sendMessage()       // socket 发送消息
{
    tcpsocket->write(block);
}



void TcpSocket::newConnect()        //新连接，连接服务器
{
    tcpsocket->abort();

    tcpsocket->connectToHost(IPofServer,PortofServer);
}


