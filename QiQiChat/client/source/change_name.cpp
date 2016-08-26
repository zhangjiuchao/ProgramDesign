
//�޸��ǳ�ģ��

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

void change_name::on_pushButton_ok_clicked()     //�޸��ǳ�
{
    QString new_name = ui->lineEdit->text();

    if(new_name == "")
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("�ǳƲ���Ϊ��");
        mm->show();
    }
    else                    //������������޸��ǳ�������Ϣ
    {
        timer->start();

        Socket->newConnect();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);

        out<<tr("CHANGENAME")<<host_account<<new_name;

        Socket->sendMessage();
    }
}



void change_name::response_for_change_name()   //���������޸��ǳƵĻ�Ӧ
{
    timer->stop();

    QDataStream in(Socket->block);

    QString isChanged;

    in>>isChanged;
    if(isChanged == "SUCCESS")       //�ǳ��޸ĳɹ�����ҳ������ǳ�
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("�޸��ǳƳɹ�");
        this->close();

        MainWindow *parentwin = (MainWindow*)parentWidget();
        parentwin->changeName( ui->lineEdit->text() );
        mm->show();
    }
    else                         //�ǳ��޸�ʧ��
    {
        QMessageBox *mm1 = new QMessageBox;
        mm1->setText("�޸�ʧ��");
        mm1->show();
    }

}

void change_name::timer_out()      //���ӳ�ʱ
{
    timer->stop();

    QMessageBox *mm2 = new QMessageBox;
    mm2->setText("�޸�ʧ�ܣ������������");
    mm2->show();
}

void change_name::on_pushButton_cancell_clicked()     //ȡ���޸�
{
    this->close();
}
