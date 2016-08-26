
//TCP 模块

#include "tcpsocket.h"
#include "chat_client.h"
#include <QMessageBox>
#include <mainwindow.h>



TcpSocket::TcpSocket(QObject *parent) :
    QObject(parent)
{
    tcpsocket = new QTcpSocket;
    block.clear();
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage()));

}



void TcpSocket::readMessage()    //接收新消息
{
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_4_6);

    QString messageType;
    in>>messageType;

    block=tcpsocket->readAll();

    if(messageType == "RESPONSEFORLOG")   //对登录的回应
        emit ResponseForLog();

    else if(messageType == "RESPONSEFORREGISTER") //对注册的回应
        emit ResponseForRegiste();

    else if(messageType == "RESPONSEFORCOMMUNICATION")   //对聊天请求的回应
        emit ResponseForCommunication();

    else if(messageType == "RESPONSEFORCHANGENAME")    //对昵称修改的回应
        emit ResponseForChangename();

    else if(messageType == "RESPONSEFORCHANGEPASSWORD")  //对密码修改的回应
        emit ResponseForChangepassword();

    else if(messageType == "FRIENDRESPONSE")    //对好友请求的回应
        emit ResponseForAddfriend();

    else if(messageType == "FRIENDREQUEST")       //好友请求到达
        emit Addrequest();

    else if(messageType == "UPDATE")           //更新好友信息的 消息
        emit update();
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


