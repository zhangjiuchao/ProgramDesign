
//登录模块


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

    ui->accountlineEdit->setPlaceholderText("输入账号");
    ui->passwordlineEdit->setPlaceholderText("输入密码");
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



void Dialog::on_closeProgram_clicked()      //退出程序
{
    this->close();
}



void Dialog::on_logButton_clicked()  //登录
{

    QString id = ui->accountlineEdit->text();
    QString password = ui->passwordlineEdit->text();

    if(id == "" || password == "")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("账号或密码不能为空");
        mm->show();
    }
    else
    {
        timer->start();

        Socket->newConnect();

        ui->logButton->setText("正在登陆...");
        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("LOG")<<id<<password;

        Socket->sendMessage();
    }

}



void Dialog::connect_timeout()   //连接超时
{
    timer->stop();

    Socket->tcpsocket->abort();


    QMessageBox *message = new QMessageBox;
    message->setText("连接超时，请检查你的网络");
    ui->logButton->setText("确认登录");
    message->show();
}



void Dialog::response_for_log() //服务器对登录的回应
{
    timer->stop();

    QDataStream in(Socket->block);

    QString isLog,account;
    in>>isLog;
    in>>account;


    if(isLog == "SUCCESS")     //登录成功
    {
        MainWindow *mainWin = new MainWindow;
        mainWin->setAttribute(Qt::WA_DeleteOnClose,true);
        this->close();

        mainWin->init();

        mainWin->show();

        Socket->block.clear();
    }
    else                            //登录失败
    {
        QMessageBox *message = new QMessageBox;
        message->setText("登陆失败！\n用户名或密码错误");
        message->show();

        ui->logButton->setText("确认登陆");

        Socket->tcpsocket->abort();         //断开连接
    }
}



void Dialog::on_registerButton_clicked()       //启动注册窗口
{

    register_window *Rwindow = new register_window;
    Rwindow->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    Rwindow->show();

}
