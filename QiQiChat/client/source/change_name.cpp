
//修改昵称模块

#include "change_name.h"
#include "ui_change_name.h"
#include "chat_client.h"
#include <QMessageBox>
#include <mainwindow.h>


change_name::change_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_name)
{
    ui->setupUi(this);

    timer = new QTimer;
    timer->setInterval(5000);

    connect( timer,SIGNAL( timeout() ),
            this,SLOT(timer_out()) );

    connect( Socket,SIGNAL( ResponseForChangename() ),
            this,SLOT( response_for_change_name() ) );
}



change_name::~change_name()
{
    delete ui;
}

void change_name::on_pushButton_ok_clicked()     //修改昵称
{
    QString new_name = ui->lineEdit->text();

    if(new_name == "")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("昵称不能为空");
        mm->show();
    }
    else                    //向服务器发送修改昵称请求及信息
    {
        timer->start();

        Socket->newConnect();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);

        out<<tr("CHANGENAME")<<host_account<<new_name;

        Socket->sendMessage();
    }
}



void change_name::response_for_change_name()   //服务器对修改昵称的回应
{
    timer->stop();

    QDataStream in(Socket->block);

    QString isChanged;

    in>>isChanged;
    if(isChanged == "SUCCESS")       //昵称修改成功，主页面更新昵称
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("修改昵称成功");
        this->close();

        MainWindow *parentwin = (MainWindow*)parentWidget();
        parentwin->changeName( ui->lineEdit->text() );
        mm->show();
    }
    else                         //昵称修改失败
    {
        QMessageBox *mm1 = new QMessageBox;
        mm1->setText("修改失败");
        mm1->show();
    }

}

void change_name::timer_out()      //连接超时
{
    timer->stop();

    QMessageBox *mm2 = new QMessageBox;
    mm2->setText("修改失败，请检查你的网络");
    mm2->show();
}

void change_name::on_pushButton_cancell_clicked()     //取消修改
{
    this->close();
}
