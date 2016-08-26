#include "addquestion.h"
#include "ui_addquestion.h"
#include "mainwindow.h"
#include "QAS_client.h"
#include "QMessageBox"
AddQuestion::AddQuestion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddQuestion)
{
    ui->setupUi(this);
    this->setFocus();
    ui->lineEdit_title->setPlaceholderText("������Ŀ");

    timer = new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    connect(Socket,SIGNAL(response_for_ask()),
            this, SLOT(addquestion_res()));
}

AddQuestion::~AddQuestion()
{
    delete ui;
    delete timer;
}

void AddQuestion::on_pushButton_2_clicked()
{
    this->close();
}



void AddQuestion::on_pushButton_clicked()
{
    QString title = ui->lineEdit_title->text();
    QString content = ui->textEdit_content->toPlainText();

    if( title == ""|| content == "" ||
        is_exist_space( title ) || is_exist_space( content ) )
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("��Ϣ�в������пո��س�����");
        mm->show();
    }
    else
    {
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("ASK")<<title<<content;
        Socket->sendMessage();
    }

}



void AddQuestion::time_out()
{
    timer->stop();

    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ��������������");
    msg->exec();
}

void AddQuestion::addquestion_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    int success;
    in>>success;
    if(success)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("��������ɹ�");
        msg->exec();
        this->close();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("��������ʧ��");
        msg->exec();
        this->close();
    }
}
