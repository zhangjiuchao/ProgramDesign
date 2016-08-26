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

    ui->lineEdit->setPlaceholderText("�����˺�");
    ui->lineEdit_2->setPlaceholderText("�ǳ�");
    ui->lineEdit_3->setPlaceholderText("����");
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setPlaceholderText("ȷ������");
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);

    timer = new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //��ע�������ź����
    connect(Socket,SIGNAL(response_for_registe()),this,SLOT(registe_response()));

}

Register::~Register()
{
    delete ui;
    delete timer;
}


//ȡ��ע��
void Register::on_pushButton_2_clicked()
{
    Log_in *LogWindows = new Log_in;
    LogWindows->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    LogWindows->show();
}

//ע�����
void Register::on_pushButton_clicked()
{
    QString IDstring = ui->lineEdit->text();
    QString Namestring = ui->lineEdit_2->text();
    QString Pstring = ui->lineEdit_3->text();
    QString Pstring_2 = ui->lineEdit_4->text();
    if(IDstring == ""||Namestring == ""||Pstring == ""||Pstring_2 == "")
    {
        QMessageBox *Inform1 = new QMessageBox;
        Inform1->setText("�밴Ҫ����д��Ϣ");
        Inform1->show();
    }
    else
    {
        if(Pstring != Pstring_2)   //������������벻ͬ
        {
            QMessageBox *m = new QMessageBox;
            m->setText("��ȷ������������ͬ");
            m->show();
        }
        else          //�����������ע����Ϣ
        {
            if(is_exist_space(IDstring)||is_exist_space(Namestring)
                    ||is_exist_space(Pstring)||is_exist_space(Pstring_2))
            {
                QMessageBox *mm=new QMessageBox;
                mm->setText("��Ϣ�в������пո�ͻس�����");
                mm->show();
            }
            else
            {
                timer->start();
                ui->pushButton->setText("����ע��...");

                Socket->newConnect();

                QDataStream out(&(Socket->block),QIODevice::WriteOnly);
                out<<tr("REGISTER")<<IDstring<<Namestring<<Pstring;
                Socket->sendMessage();
            }

        }
    }
}


//���������ص�ע����
void Register::registe_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int isRegisted;
    in>>isRegisted;

    if(isRegisted)          //ע��ɹ�
    {
        MainWindow *mainWin = new MainWindow;       //��ʾ��ҳ��
        mainWin->setAttribute(Qt::WA_DeleteOnClose,true);
        mainWin->init();
        this->close();

        mainWin->show();

        Socket->block.clear();
    }
    else               //ע��ʧ��
    {
        QMessageBox *message = new QMessageBox;
        message->setText("ע��ʧ��");
        message->show();
        ui->pushButton->setText("ȷ��");

        Socket->tcpsocket->abort(); //�Ͽ�����
    }
}

//��ʱ��Ӧ
void Register::time_out()
{
    QMessageBox *msg=new QMessageBox;
    msg->setText("���ӳ�ʱ�������������");
    ui->pushButton->setText("ע��");
    msg->exec();
}
