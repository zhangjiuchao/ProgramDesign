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
    ui->lineEdit->setPlaceholderText("ԭ����");
    ui->lineEdit_2->setPlaceholderText("������");
    ui->lineEdit_3->setPlaceholderText("ȷ��������");
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


//�޸��������
void ChangePassword::on_pushButton_clicked()
{
    QString oldPassword = ui->lineEdit->text();
    QString newPassword1 = ui->lineEdit_2->text();
    QString newPassword2 = ui->lineEdit_3->text();
    if(oldPassword == "" || newPassword1 == "" || newPassword2 == "")
    {
        QMessageBox *message = new QMessageBox;
        message->setText("���벻��Ϊ��");
        message->show();
    }
    else
    {
        if(is_exist_space(newPassword1))
        {
            QMessageBox *mm = new QMessageBox;
            mm->setText("��Ϣ�в������пո�ͻس�����");
            mm->show();
        }
        else
        {
            if(newPassword1 != newPassword2)
            {
                QMessageBox *msg = new QMessageBox;
                msg->setText("������������벻ͬ");
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

//ȡ���޸��������
void ChangePassword::on_pushButton_2_clicked()
{
    this->close();
}


//������������޸�����Ľ��
void ChangePassword::changepassword_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    int success;
    in>>success;

    if(success)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("�����޸ĳɹ�");
        msg->exec();
        this->close();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("�����޸�ʧ��");
        msg->exec();
    }
    Socket->block.clear();
}


//��ʱ��Ӧ
void ChangePassword::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ��������������");
    msg->exec();
}
