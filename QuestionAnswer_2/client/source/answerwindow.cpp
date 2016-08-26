#include "answerwindow.h"
#include "ui_answerwindow.h"
#include "QAS_client.h"
#include "QMessageBox"

AnswerWindow::AnswerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnswerWindow)
{
    ui->setupUi(this);

    timer=new QTimer;         //���ö�ʱ��ʱ����Ϊ15�룬������15��û���յ��������Ļ�Ӧ
                              //���ʾ�������ӳ��ֹ���
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

//��ʾ�ش𴰿ڳ�ʼ��
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
         ui->pushButton_focus->setText("ȡ����ע");
    }
    else
        ui->pushButton_focus->setText("��ע");

    int isPraise;
    int praiseNum;

    in>>isPraise;
    in>>praiseNum;

    QString str="";
    if(isPraise)
    {
        str = "����";
        ui->pushButton_praise->setEnabled(false);
    }
    else
    {
        str = "��";
    }
    str=str+"("+QString::number(praiseNum)+")";
    ui->pushButton_praise->setText(str);

    QString content;
    in>>content;
    ui->textBrowser->setText(content);

    Socket->block.clear();
}


//��ע�û�����
void AnswerWindow::on_pushButton_focus_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("FOCUS")<<id_user;
    Socket->sendMessage();

}


//���ش���޲���
void AnswerWindow::on_pushButton_praise_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("PRAISE")<<id_question<<id_answer;
    Socket->sendMessage();
}


//��ʱ��Ӧ
void AnswerWindow::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ��������������");
    msg->exec();
}


//������������ڹ�ע�����Ľ��
void AnswerWindow::focus_res()
{
    timer->stop();
    QDataStream in(Socket->block);
    QString focus_id;
    in>>focus_id;
    if(focus_id == id_user)         //�����ע��Ӧ�ǵ�ǰ�û�ʱ�����в���
    {
        int isSuccess;
        in>>isSuccess;
        if(isSuccess)
            ui->pushButton_focus->setText("ȡ����ע");
        else
            ui->pushButton_focus->setText("��ע");
    }
}


//����������Ե��޲����Ľ��
void AnswerWindow::praise_res()
{
    timer->stop();
    QDataStream in(Socket->block);
    QString praise_id;
    in>>praise_id;
    if(praise_id == id_answer)      ////���������Ӧ�ǵ�ǰ�ش�ʱ�����в���
    {
        int success;
        in>>success;

        int praiseNum;
        in>>praiseNum;

        if(success)
        {
            QString str = "����(";
            str = str+QString::number(praiseNum)+")";
            ui->pushButton_praise->setText(str);
            ui->pushButton_praise->setEnabled(false);
            QMessageBox *msg = new QMessageBox;
            msg->setText("���޳ɹ�");
            msg->exec();
        }
        else
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("����ʧ��");
            msg->exec();
        }
    }

}
