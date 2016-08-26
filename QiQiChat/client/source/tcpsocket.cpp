
//TCP ģ��

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



void TcpSocket::readMessage()    //��������Ϣ
{
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_4_6);

    QString messageType;
    in>>messageType;

    block=tcpsocket->readAll();

    if(messageType == "RESPONSEFORLOG")   //�Ե�¼�Ļ�Ӧ
        emit ResponseForLog();

    else if(messageType == "RESPONSEFORREGISTER") //��ע��Ļ�Ӧ
        emit ResponseForRegiste();

    else if(messageType == "RESPONSEFORCOMMUNICATION")   //����������Ļ�Ӧ
        emit ResponseForCommunication();

    else if(messageType == "RESPONSEFORCHANGENAME")    //���ǳ��޸ĵĻ�Ӧ
        emit ResponseForChangename();

    else if(messageType == "RESPONSEFORCHANGEPASSWORD")  //�������޸ĵĻ�Ӧ
        emit ResponseForChangepassword();

    else if(messageType == "FRIENDRESPONSE")    //�Ժ�������Ļ�Ӧ
        emit ResponseForAddfriend();

    else if(messageType == "FRIENDREQUEST")       //�������󵽴�
        emit Addrequest();

    else if(messageType == "UPDATE")           //���º�����Ϣ�� ��Ϣ
        emit update();
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


