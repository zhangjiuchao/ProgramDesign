#include "changepassword.h"
#include "ui_changepassword.h"
#include "QAS_client.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
ChangePassword::ChangePassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    this->setFocus();
    ui->lineEdit->setPlaceholderText("原密码");
    ui->lineEdit_2->setPlaceholderText("新密码");
    ui->lineEdit_3->setPlaceholderText("确认新密码");
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

    timer = new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    connect(Socket,SIGNAL(response_for_changepassword()),
            this, SLOT(changepassword_res()));

}

ChangePassword::~ChangePassword()
{
    delete ui;
    delete timer;
}


//修改密码操作
void ChangePassword::on_pushButton_clicked()
{
    QString oldPassword = ui->lineEdit->text();
    QString newPassword1 = ui->lineEdit_2->text();
    QString newPassword2 = ui->lineEdit_3->text();
    if(oldPassword == "" || newPassword1 == "" || newPassword2 == "")
    {
        QMessageBox *message = new QMessageBox;
        message->setText("密码不能为空");
        message->show();
    }
    else
    {
        if(is_exist_space(newPassword1))
        {
            QMessageBox *mm = new QMessageBox;
            mm->setText("信息中不允许有空格和回车出现");
            mm->show();
        }
        else
        {
            if(newPassword1 != newPassword2)
            {
                QMessageBox *msg = new QMessageBox;
                msg->setText("两次输入的密码不同");
                msg->exec();
            }
            else
            {
                timer->start();

                QDataStream out(&(Socket->block),QIODevice::WriteOnly);
                out<<tr("CHANGEPASSWORD")<<oldPassword<<newPassword1;
                Socket->sendMessage();
            }
        }
    }

}

//取消修改密码操作
void ChangePassword::on_pushButton_2_clicked()
{
    this->close();
}


//处理服务器对修改密码的结果
void ChangePassword::changepassword_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    int success;
    in>>success;

    if(success)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("密码修改成功");
        msg->exec();
        this->close();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("密码修改失败");
        msg->exec();
    }
    Socket->block.clear();
}


//超时响应
void ChangePassword::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("连接超时，请检查您的网络");
    msg->exec();
}
