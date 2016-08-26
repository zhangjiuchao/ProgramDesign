
//新账号注册模块

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

    ui->lineEdit_ID->setPlaceholderText("输入账户名");
    ui->lineEdit_name->setPlaceholderText("输入昵称");
    ui->lineEdit_password->setPlaceholderText("输入密码");
    ui->lineEdit_password_2->setPlaceholderText("确认密码");

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



void register_window::on_pushButton_2_clicked()     //取消注册，返回登录界面
{
    Dialog *log_window = new Dialog;
    log_window->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    log_window->show();
}



void register_window::on_pushButton_clicked()      //注册新账号
{
    QString id = ui->lineEdit_ID->text();
    QString name = ui->lineEdit_name->text();
    QString password = ui->lineEdit_password->text();
    QString password_2 = ui->lineEdit_password_2->text();

    if(id == ""||name == ""||password == ""||password_2 == "")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("所有项输入均不能为空");
        mm->exec();
    }
    else
    {

       if(password != password_2)   //两次输入的密码不同
       {
           QMessageBox *m = new QMessageBox;
           m->setText("请确认两次密码相同");
           m->show();
       }
       else          //向服务器发送注册信息
       {
           timer->start();
           ui->pushButton->setText("正在注册...");

           Socket->newConnect();

           QDataStream out(&(Socket->block),QIODevice::WriteOnly);
           out<<tr("REGISTER")<<id<<name<<password;
           Socket->sendMessage();
       }

    }
}



void register_window::connect_timeout()   //连接服务器超时
{
    timer->stop();

    Socket->tcpsocket->abort();

    QMessageBox *message = new QMessageBox;
    message->setText("连接超时，请检查你的网络");
    ui->pushButton->setText("确认");
    message->show();
}


void register_window::response_for_registe() //服务器对账号注册的回应
{
    timer->stop();
    QDataStream in(Socket->block);

    QString isRegisted;
    in>>isRegisted;

    if(isRegisted == "SUCCESS")          //注册成功
    {
        MainWindow *mainWin = new MainWindow;       //显示主页面
        mainWin->setAttribute(Qt::WA_DeleteOnClose,true);

        this->close();

        host_account = ui->lineEdit_ID->text();
        host_name = ui->lineEdit_name->text();
        mainWin->setinfor(host_account,host_name);
        mainWin->show();

        Socket->block.clear();
    }
    else               //注册失败
    {
        QMessageBox *message = new QMessageBox;
        message->setText("注册失败");
        message->show();
        ui->pushButton->setText("确认");

        Socket->tcpsocket->abort(); //断开连接
    }
}

