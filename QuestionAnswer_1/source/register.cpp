#include "register.h"
#include "ui_register.h"
#include "log_in.h"
#include "Question_Answer_System.h"
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
    ui->lineEdit->setPlaceholderText("输入账号");
    ui->lineEdit_2->setPlaceholderText("昵称");
    ui->lineEdit_3->setPlaceholderText("密码");
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setPlaceholderText("确认密码");
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);

}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_2_clicked()
{
    Log_in *LogWindows=new Log_in;
    this->close();
    LogWindows->show();
}

void Register::on_pushButton_clicked()
{
    QString IDstring=ui->lineEdit->text();
    QString Namestring=ui->lineEdit_2->text();
    QString Pstring=ui->lineEdit_3->text();
    QString Pstring_2=ui->lineEdit_4->text();
    if(IDstring==""||Namestring==""||Pstring==""||Pstring_2=="")
    {
        QMessageBox *Inform1=new QMessageBox;
        Inform1->setText("请按要求填写信息");
        Inform1->show();
    }
    else
    {
        if(is_exist_space(IDstring)||is_exist_space(Namestring)
                ||is_exist_space(Pstring)||is_exist_space(Pstring_2))
        {
            QMessageBox *mm=new QMessageBox;
            mm->setText("信息中不允许有空格和回车出现");
            mm->show();
        }
        else
        {
            QMap<QString,QString>::iterator it=ID_Password.find(IDstring);
            if(it!=ID_Password.end())
            {
                QMessageBox *Inform=new QMessageBox;
                Inform->setText("对不起！该账户已被注册\n请重新输入账号");
                Inform->show();
            }
            else
            {
                if(Pstring!=Pstring_2)
                {
                    QMessageBox *Inform2=new QMessageBox;
                    Inform2->setText("两次密码不相同\n请再次确认");
                    Inform2->show();
                }
                else
                {
                    QFile files("Data\\"+IDstring+".txt");
                    if(!files.open(QIODevice::WriteOnly|QIODevice::Text))
                    {
                          QMessageBox *Inform3=new QMessageBox;
                          Inform3->setText(IDstring+".txt 文件打开失败");
                          Inform3->show();
                    }
                    else
                    {
                            QTextStream out(&files);
                            out<<IDstring<<"   "<<Pstring<<"  "<<Namestring<<"\r\n";
                            out<<0<<"\r\n"<<0;

                            files.close();

                            files.setFileName("Data\\IDPassword.txt");
                            if(!files.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
                            {
                                QMessageBox *Inform4=new QMessageBox;
                                Inform4->setText("文件 IDPassword.txt 打开失败");
                                Inform4->show();
                            }
                            else
                            {
                                QTextStream out(&files);
                                out<<IDstring<<"   "<<Pstring<<"  "<<Namestring<<"\n\r";
                                files.close();
                            }

                            ID_Password.insert(IDstring,Pstring);

                            ID_Name.insert(IDstring,Namestring);

                            this->close();

                            QMessageBox *Inform3=new QMessageBox;
                            Inform3->setText("注册成功");

                            HostUser=new user(IDstring,Namestring,Pstring);

                            MainWindow *MainWin=new MainWindow;
                            if(!LoadQuestion())
                            {
                                QMessageBox *mm=new QMessageBox;
                                mm->setText("问题信息丢失");
                                mm->show();
                            }
                            else
                            {
                                 MainWin->setQuestionList();
                                 MainWin->setMyQuestionList();
                                 MainWin->setFocusList();
                                 this->close();
                                 MainWin->show();
                                 Inform3->show();
                            }

                            MainWin->setQuestionList();
                            MainWin->show();
                            Inform3->show();
                    }

                }
        }

         }
    }
}
