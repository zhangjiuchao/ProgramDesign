#include "answerwindow.h"
#include "ui_answerwindow.h"
#include "QAS_client.h"
#include "QMessageBox"

AnswerWindow::AnswerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnswerWindow)
{
    ui->setupUi(this);

    timer=new QTimer;         //设置定时器时间间隔为15秒，若超过15秒没有收到服务器的回应
                              //则表示网络连接出现故障
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    connect(Socket,SIGNAL(response_for_focus()),this,SLOT(focus_res()));

    connect(Socket,SIGNAL(response_for_praise()),
            this,SLOT(praise_res()));


}

AnswerWindow::~AnswerWindow()
{
    delete ui;
    delete timer;
}

//显示回答窗口初始化
void AnswerWindow::init()
{
    QDataStream in(Socket->block);

    in>>id_question;
    in>>id_answer;
    in>>id_user;

    QString name_user;
    in>>name_user;
    ui->label_name->setText(name_user);

    QString time_create;
    in>>time_create;
    ui->label_time->setText(time_create);

    int isFocus;
    in>>isFocus;
    if(isFocus)
    {
         ui->pushButton_focus->setText("取消关注");
    }
    else
        ui->pushButton_focus->setText("关注");

    int isPraise;
    int praiseNum;

    in>>isPraise;
    in>>praiseNum;

    QString str="";
    if(isPraise)
    {
        str = "已赞";
        ui->pushButton_praise->setEnabled(false);
    }
    else
    {
        str = "赞";
    }
    str=str+"("+QString::number(praiseNum)+")";
    ui->pushButton_praise->setText(str);

    QString content;
    in>>content;
    ui->textBrowser->setText(content);

    Socket->block.clear();
}


//关注用户操作
void AnswerWindow::on_pushButton_focus_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("FOCUS")<<id_user;
    Socket->sendMessage();

}


//给回答点赞操作
void AnswerWindow::on_pushButton_praise_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("PRAISE")<<id_question<<id_answer;
    Socket->sendMessage();
}


//超时响应
void AnswerWindow::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("连接超时，请检查您的网络");
    msg->exec();
}


//处理服务器对于关注操作的结果
void AnswerWindow::focus_res()
{
    timer->stop();
    QDataStream in(Socket->block);
    QString focus_id;
    in>>focus_id;
    if(focus_id == id_user)         //如果关注响应是当前用户时，进行操作
    {
        int isSuccess;
        in>>isSuccess;
        if(isSuccess)
            ui->pushButton_focus->setText("取消关注");
        else
            ui->pushButton_focus->setText("关注");
    }
}


//处理服务器对点赞操作的结果
void AnswerWindow::praise_res()
{
    timer->stop();
    QDataStream in(Socket->block);
    QString praise_id;
    in>>praise_id;
    if(praise_id == id_answer)      ////如果点赞响应是当前回答时，进行操作
    {
        int success;
        in>>success;

        int praiseNum;
        in>>praiseNum;

        if(success)
        {
            QString str = "已赞(";
            str = str+QString::number(praiseNum)+")";
            ui->pushButton_praise->setText(str);
            ui->pushButton_praise->setEnabled(false);
            QMessageBox *msg = new QMessageBox;
            msg->setText("点赞成功");
            msg->exec();
        }
        else
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("点赞失败");
            msg->exec();
        }
    }

}
