
//���˺�ע��ģ��

#include "register_window.h"
#include "chat_client.h"
#include "ui_register_window.h"
#include "dialog.h"
#include "mainwindow.h"
#include "QMessageBox"


register_window::register_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::register_window)
{
    ui->setupUi(this);
    this->setFocus();

    ui->lineEdit_ID->setPlaceholderText("�����˻���");
    ui->lineEdit_name->setPlaceholderText("�����ǳ�");
    ui->lineEdit_password->setPlaceholderText("��������");
    ui->lineEdit_password_2->setPlaceholderText("ȷ������");

    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password_2->setEchoMode(QLineEdit::Password);

    timer = new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(connect_timeout()));

    connect(Socket,SIGNAL(ResponseForRegiste()),this,SLOT(response_for_registe()));
}



register_window::~register_window()
{
    delete ui;
}



void register_window::on_pushButton_2_clicked()     //ȡ��ע�ᣬ���ص�¼����
{
    Dialog *log_window = new Dialog;
    log_window->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    log_window->show();
}



void register_window::on_pushButton_clicked()      //ע�����˺�
{
    QString id = ui->lineEdit_ID->text();
    QString name = ui->lineEdit_name->text();
    QString password = ui->lineEdit_password->text();
    QString password_2 = ui->lineEdit_password_2->text();

    if(id == ""||name == ""||password == ""||password_2 == "")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("���������������Ϊ��");
        mm->exec();
    }
    else
    {

       if(password != password_2)   //������������벻ͬ
       {
           QMessageBox *m = new QMessageBox;
           m->setText("��ȷ������������ͬ");
           m->show();
       }
       else          //�����������ע����Ϣ
       {
           timer->start();
           ui->pushButton->setText("����ע��...");

           Socket->newConnect();

           QDataStream out(&(Socket->block),QIODevice::WriteOnly);
           out<<tr("REGISTER")<<id<<name<<password;
           Socket->sendMessage();
       }

    }
}



void register_window::connect_timeout()   //���ӷ�������ʱ
{
    timer->stop();

    Socket->tcpsocket->abort();

    QMessageBox *message = new QMessageBox;
    message->setText("���ӳ�ʱ�������������");
    ui->pushButton->setText("ȷ��");
    message->show();
}


void register_window::response_for_registe() //���������˺�ע��Ļ�Ӧ
{
    timer->stop();
    QDataStream in(Socket->block);

    QString isRegisted;
    in>>isRegisted;

    if(isRegisted == "SUCCESS")          //ע��ɹ�
    {
        MainWindow *mainWin = new MainWindow;       //��ʾ��ҳ��
        mainWin->setAttribute(Qt::WA_DeleteOnClose,true);

        this->close();

        host_account = ui->lineEdit_ID->text();
        host_name = ui->lineEdit_name->text();
        mainWin->setinfor(host_account,host_name);
        mainWin->show();

        Socket->block.clear();
    }
    else               //ע��ʧ��
    {
        QMessageBox *message = new QMessageBox;
        message->setText("ע��ʧ��");
        message->show();
        ui->pushButton->setText("ȷ��");

        Socket->tcpsocket->abort(); //�Ͽ�����
    }
}

