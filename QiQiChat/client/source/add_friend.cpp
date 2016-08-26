
//添加好友模块

#include "add_friend.h"
#include "ui_add_friend.h"
#include "chat_client.h"
#include <QMessageBox>


Add_Friend::Add_Friend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Friend)
{
    ui->setupUi(this);
}


Add_Friend::~Add_Friend()
{
    delete ui;
}

void Add_Friend::on_pushButton_2_clicked()     //取消添加好友
{
    this->close();
}


void Add_Friend::on_pushButton_clicked()    //添加好友
{
    QString add_account = ui->lineEdit->text();

    if(add_account=="")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("账号不能为空");
        mm->show();
    }
    else          //向服务器发送添加好友请求
    {
        Socket->newConnect();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);

        out<<tr("FRIENDREQUEST")<<add_account<<host_account;

        Socket->sendMessage();
        close();
    }
}
