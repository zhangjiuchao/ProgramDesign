#include "log_in.h"
#include "ui_log_in.h"
#include "register.h"
#include "QAS_client.h"
#include "mainwindow.h"
#include "QMessageBox"


bool LoadHostUser(QString filetoOpen);


Log_in::Log_in(QWidget *parent) :
   QMainWindow(parent),
    ui(new Ui::Log_in)
{
    ui->setupUi(this);
    ui->pushButton_2->setFlat(true);
    ui->lineEdit->setPlaceholderText("�����˺�");
    ui->lineEdit_2->setPlaceholderText("��������");
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->pushButton->setShortcut(Qt::Key_Enter);

    timer=new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));
    connect(Socket,SIGNAL(response_for_log()),this,SLOT(log_response()));

    setWindowTitle("Ϲ��ϵͳ��¼");
}

Log_in::~Log_in()
{
    delete ui;
    delete timer;
    delete Socket;
}


//��¼����
void Log_in::on_pushButton_clicked()
{
   QString IDstring = ui->lineEdit->text();
   QString Passstring = ui->lineEdit_2->text();

   if( IDstring == "" || Passstring == "" )
   {
       QMessageBox *Inform1 = new QMessageBox;
       Inform1->setText("�������˺ź�����");
       Inform1->show();
   }
   else
   {
        timer->start();

        Socket->newConnect();               //��¼����

        ui->pushButton->setText("���ڵ�½");
        QDataStream out(&(Socket->block),QIODevice::WriteOnly);

        out<<tr("LOG")<<IDstring<<Passstring;

        Socket->sendMessage();

   }
}


//��ע�ᴰ��
void Log_in::on_pushButton_2_clicked()
{
    Register *reg = new Register;
    reg->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    reg->exec();
}



//��ʱ��Ӧ
void Log_in::time_out()
{
    timer->stop();

    ui->pushButton->setText("ȷ�ϵ�¼");

    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ�������������");
    msg->show();

}


//������������صĵ�¼���
void Log_in::log_response()
{
    timer->stop();

    QDataStream in(Socket->block);

    int isLog;
    in>>isLog;



    if(isLog == 0)     //��¼ʧ��
    {
        QMessageBox *message = new QMessageBox;
        message->setText("��½ʧ�ܣ�\n���û���������");
        message->show();

        ui->pushButton->setText("ȷ�ϵ�½");

        Socket->tcpsocket->abort();         //�Ͽ�����

    }
    else
    {
        if(isLog == 1)
        {
            QMessageBox *message = new QMessageBox;
            message->setText("��½ʧ�ܣ�\n�������");
            message->show();

            ui->pushButton->setText("ȷ�ϵ�½");

            Socket->tcpsocket->abort();         //�Ͽ�����
        }
        else
        {
            if(isLog == 2)
            {
                QMessageBox *message = new QMessageBox;
                message->setText("��½ʧ�ܣ�\n���û������ߣ������ظ���¼");
                message->show();

                ui->pushButton->setText("ȷ�ϵ�½");

                Socket->tcpsocket->abort();         //�Ͽ�����
            }
            else
            {
                if(isLog == 3)
                {
                    QMessageBox *message = new QMessageBox;
                    message->setText("��½ʧ�ܣ�\n���û��ļ���ʧ");
                    message->show();

                    ui->pushButton->setText("ȷ�ϵ�½");

                    Socket->tcpsocket->abort();         //�Ͽ�����
                }

                else                            //��¼�ɹ�
                {

                    MainWindow *mainWin = new MainWindow;
                    mainWin->setAttribute(Qt::WA_DeleteOnClose,true);
                    this->close();

                    mainWin->init();           //�����ڳ�ʼ��

                    mainWin->show();

                    Socket->block.clear();
                }

            }
        }
    }








}
