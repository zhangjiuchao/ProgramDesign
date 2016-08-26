
//��¼ģ��


#include "dialog.h"
#include "ui_dialog.h"
#include "chat_client.h"
#include "mainwindow.h"
#include "register_window.h"
#include "talkwindows.h"
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    this->setFocus();

    ui->registerButton->setFlat(true);

    ui->accountlineEdit->setPlaceholderText("�����˺�");
    ui->passwordlineEdit->setPlaceholderText("��������");
    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);

    timer=new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(connect_timeout()));

    connect(Socket,SIGNAL(ResponseForLog()),SLOT(response_for_log()));
}



Dialog::~Dialog()
{
    delete ui;
}



void Dialog::on_closeProgram_clicked()      //�˳�����
{
    this->close();
}



void Dialog::on_logButton_clicked()  //��¼
{

    QString id = ui->accountlineEdit->text();
    QString password = ui->passwordlineEdit->text();

    if(id == "" || password == "")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("�˺Ż����벻��Ϊ��");
        mm->show();
    }
    else
    {
        timer->start();

        Socket->newConnect();

        ui->logButton->setText("���ڵ�½...");
        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("LOG")<<id<<password;

        Socket->sendMessage();
    }

}



void Dialog::connect_timeout()   //���ӳ�ʱ
{
    timer->stop();

    Socket->tcpsocket->abort();


    QMessageBox *message = new QMessageBox;
    message->setText("���ӳ�ʱ�������������");
    ui->logButton->setText("ȷ�ϵ�¼");
    message->show();
}



void Dialog::response_for_log() //�������Ե�¼�Ļ�Ӧ
{
    timer->stop();

    QDataStream in(Socket->block);

    QString isLog,account;
    in>>isLog;
    in>>account;


    if(isLog == "SUCCESS")     //��¼�ɹ�
    {
        MainWindow *mainWin = new MainWindow;
        mainWin->setAttribute(Qt::WA_DeleteOnClose,true);
        this->close();

        mainWin->init();

        mainWin->show();

        Socket->block.clear();
    }
    else                            //��¼ʧ��
    {
        QMessageBox *message = new QMessageBox;
        message->setText("��½ʧ�ܣ�\n�û������������");
        message->show();

        ui->logButton->setText("ȷ�ϵ�½");

        Socket->tcpsocket->abort();         //�Ͽ�����
    }
}



void Dialog::on_registerButton_clicked()       //����ע�ᴰ��
{

    register_window *Rwindow = new register_window;
    Rwindow->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    Rwindow->show();

}
