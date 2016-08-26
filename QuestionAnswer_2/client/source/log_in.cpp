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
    ui->lineEdit->setPlaceholderText("输入账号");
    ui->lineEdit_2->setPlaceholderText("输入密码");
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->pushButton->setShortcut(Qt::Key_Enter);

    timer=new QTimer;
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));
    connect(Socket,SIGNAL(response_for_log()),this,SLOT(log_response()));

    setWindowTitle("瞎扯系统登录");
}

Log_in::~Log_in()
{
    delete ui;
    delete timer;
    delete Socket;
}


//登录操作
void Log_in::on_pushButton_clicked()
{
   QString IDstring = ui->lineEdit->text();
   QString Passstring = ui->lineEdit_2->text();

   if( IDstring == "" || Passstring == "" )
   {
       QMessageBox *Inform1 = new QMessageBox;
       Inform1->setText("请输入账号和密码");
       Inform1->show();
   }
   else
   {
        timer->start();

        Socket->newConnect();               //登录连接

        ui->pushButton->setText("正在登陆");
        QDataStream out(&(Socket->block),QIODevice::WriteOnly);

        out<<tr("LOG")<<IDstring<<Passstring;

        Socket->sendMessage();

   }
}


//打开注册窗口
void Log_in::on_pushButton_2_clicked()
{
    Register *reg = new Register;
    reg->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    reg->exec();
}



//超时响应
void Log_in::time_out()
{
    timer->stop();

    ui->pushButton->setText("确认登录");

    QMessageBox *msg = new QMessageBox;
    msg->setText("连接超时，请检查你的网络");
    msg->show();

}


//处理服务器返回的登录结果
void Log_in::log_response()
{
    timer->stop();

    QDataStream in(Socket->block);

    int isLog;
    in>>isLog;



    if(isLog == 0)     //登录失败
    {
        QMessageBox *message = new QMessageBox;
        message->setText("登陆失败！\n该用户名不存在");
        message->show();

        ui->pushButton->setText("确认登陆");

        Socket->tcpsocket->abort();         //断开连接

    }
    else
    {
        if(isLog == 1)
        {
            QMessageBox *message = new QMessageBox;
            message->setText("登陆失败！\n密码错误");
            message->show();

            ui->pushButton->setText("确认登陆");

            Socket->tcpsocket->abort();         //断开连接
        }
        else
        {
            if(isLog == 2)
            {
                QMessageBox *message = new QMessageBox;
                message->setText("登陆失败！\n该用户已在线，不可重复登录");
                message->show();

                ui->pushButton->setText("确认登陆");

                Socket->tcpsocket->abort();         //断开连接
            }
            else
            {
                if(isLog == 3)
                {
                    QMessageBox *message = new QMessageBox;
                    message->setText("登陆失败！\n该用户文件丢失");
                    message->show();

                    ui->pushButton->setText("确认登陆");

                    Socket->tcpsocket->abort();         //断开连接
                }

                else                            //登录成功
                {

                    MainWindow *mainWin = new MainWindow;
                    mainWin->setAttribute(Qt::WA_DeleteOnClose,true);
                    this->close();

                    mainWin->init();           //主窗口初始化

                    mainWin->show();

                    Socket->block.clear();
                }

            }
        }
    }








}
