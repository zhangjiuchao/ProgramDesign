
//����ģ��

#include "talkwindows.h"
#include "ui_talkwindows.h"
#include "chat_client.h"
#include "QDateTime"


TalkWindows::TalkWindows(QString str,QString str2,bool online,QWidget *parent) :
        QDialog(parent),
        ui(new Ui::TalkWindows),
        peerIP(str),
        peerName(str2),
        isOnline(online)
{
    ui->setupUi(this);

    ui->label_name->setText(peerName);
    if(online)
        ui->label_3->setText("����");
    else
        ui->label_3->setText("����");
}



TalkWindows::~TalkWindows()
{
    delete ui;
}



void TalkWindows::on_SendButton_clicked()      //������Ϣ
{
    QString message = ui->EditNews->toPlainText();

    if(message != "")
    {
        QByteArray outblock;
        QDataStream out(&outblock,QIODevice::WriteOnly);

        out<<tr("MESSAGE")<<host_account<<host_name<<message;

        //��Է����� UDP ��Ϣ
        udpSocket->writeDatagram(outblock,QHostAddress(peerIP),PortofClient);

        ui->EditNews->clear();

        QString time = QDateTime::currentDateTime().toString("hh:mm:ss  HH");

        //�ڴ�����ʾ�ҷ��͵���Ϣ
        ui->DisplayNews->setTextColor(Qt::blue);
        ui->DisplayNews->append("��   "+time);
        ui->DisplayNews->setTextColor(Qt::black);
        ui->DisplayNews->append(message);
        ui->EditNews->setFocus();
    }

}



void TalkWindows::getMessage(QString message)    //���նԷ����͵���Ϣ
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss  HH");

    ui->DisplayNews->setTextColor(Qt::red);
    ui->DisplayNews->append(peerName+"  "+time);
    ui->DisplayNews->setTextColor(Qt::black);
    ui->DisplayNews->append(message);

    ui->EditNews->setFocus();
}
