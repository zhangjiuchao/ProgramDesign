
//修改密码模块

#include "change_password.h"
#include "ui_change_password.h"
#include "QMessageBox"
#include "chat_client.h"


change_password::change_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_password)
{
    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    timer=new QTimer;
    timer->setInterval(5000);

    connect(timer,SIGNAL( timeout() ),
            this,SLOT(timerOut()));

    connect(Socket,SIGNAL(ResponseForChangepassword()),
            this,SLOT(change_response()));
}



change_password::~change_password()
{
    delete ui;
}



void change_password::on_pushButton_ok_clicked()    //修改密码
{
    QString old_password = ui->lineEdit->text();
    QString new_password_1 = ui->lineEdit_2->text();
    QString new_password_2 = ui->lineEdit_3->text();

    if(old_password == ""||new_password_1 == ""||new_password_2 == "")
    {
        QMessageBox *m1 = new QMessageBox;
        m1->setText("密码不能为空");
        m1->show();
    }
    else
    {

        if(new_password_1 != new_password_2)
        {
            QMessageBox *m2 = new QMessageBox;
            m2->setText("两次输入的修改密码不相同");
            m2->show();
        }
        else   //向服务器发送密码修改请求及信息
        {
            timer->start();
            Socket->newConnect();
            QDataStream out(&(Socket->block),QIODevice::WriteOnly);
            out<<tr("CHANGEPASSWORD")<<host_account<<old_password<<new_password_1;
            Socket->sendMessage();
        }

    }
}



void change_password::change_response()    //服务器对修改密码的回应
{
    timer->stop();
    QDataStream in(Socket->block);

    QString isChanged;
    in>>isChanged;


    if(isChanged == "SUCCESS")             //修改成功
    {
        QMessageBox *m2 = new QMessageBox;
        m2->setText("修改成功");
        this->close();
        m2->show();
    }
    else                                   //修改失败
    {
        QMessageBox *m3 = new QMessageBox;
        m3->setText("修改失败");
        m3->show();
    }
}



void change_password::timerOut()      //连接服务器超时
{
    timer->stop();

    QMessageBox *m3 = new QMessageBox;
    m3->setText("修改失败,请检查你的网络");
    m3->show();
}



void change_password::on_pushButton_cancell_clicked()     //取消修改
{
    this->close();
}
