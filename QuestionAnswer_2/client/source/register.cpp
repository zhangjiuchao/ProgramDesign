#include "register.h"
#include "ui_register.h"
#include "log_in.h"
#include "QAS_client.h"
#include "QFile"
#include "mainwindow.h"
#include "QDebug"
#include <QMessageBox>
#include "log_in.h"
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setFocus();

    ui->lineEdit->setPlaceholderText("输入账号");
    ui->lineEdit_2->setPlaceholderText("昵称");
    ui->lineEdit_3->setPlaceholderText("密码");
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setPlaceholderText("确认密码");
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);

    timer = new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //绑定注册结果的信号与槽
    connect(Socket,SIGNAL(response_for_registe()),this,SLOT(registe_response()));

}

Register::~Register()
{
    delete ui;
    delete timer;
}


//取消注册
void Register::on_pushButton_2_clicked()
{
    Log_in *LogWindows = new Log_in;
    LogWindows->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    LogWindows->show();
}

//注册操作
void Register::on_pushButton_clicked()
{
    QString IDstring = ui->lineEdit->text();
    QString Namestring = ui->lineEdit_2->text();
    QString Pstring = ui->lineEdit_3->text();
    QString Pstring_2 = ui->lineEdit_4->text();
    if(IDstring == ""||Namestring == ""||Pstring == ""||Pstring_2 == "")
    {
        QMessageBox *Inform1 = new QMessageBox;
        Inform1->setText("请按要求填写信息");
        Inform1->show();
    }
    else
    {
        if(Pstring != Pstring_2)   //两次输入的密码不同
        {
            QMessageBox *m = new QMessageBox;
            m->setText("请确认两次密码相同");
            m->show();
        }
        else          //向服务器发送注册信息
        {
            if(is_exist_space(IDstring)||is_exist_space(Namestring)
                    ||is_exist_space(Pstring)||is_exist_space(Pstring_2))
            {
                QMessageBox *mm=new QMessageBox;
                mm->setText("信息中不允许有空格和回车出现");
                mm->show();
            }
            else
            {
                timer->start();
                ui->pushButton->setText("正在注册...");

                Socket->newConnect();

                QDataStream out(&(Socket->block),QIODevice::WriteOnly);
                out<<tr("REGISTER")<<IDstring<<Namestring<<Pstring;
                Socket->sendMessage();
            }

        }
    }
}


//服务器返回的注册结果
void Register::registe_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int isRegisted;
    in>>isRegisted;

    if(isRegisted)          //注册成功
    {
        MainWindow *mainWin = new MainWindow;       //显示主页面
        mainWin->setAttribute(Qt::WA_DeleteOnClose,true);
        mainWin->init();
        this->close();

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

//超时响应
void Register::time_out()
{
    QMessageBox *msg=new QMessageBox;
    msg->setText("连接超时，请检查你的网络");
    ui->pushButton->setText("注册");
    msg->exec();
}
