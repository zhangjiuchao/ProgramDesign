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
    ui->lineEdit->setPlaceholderText("�����˺�");
    ui->lineEdit_2->setPlaceholderText("�ǳ�");
    ui->lineEdit_3->setPlaceholderText("����");
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setPlaceholderText("ȷ������");
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
        Inform1->setText("�밴Ҫ����д��Ϣ");
        Inform1->show();
    }
    else
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
            QMap<QString,QString>::iterator it=ID_Password.find(IDstring);
            if(it!=ID_Password.end())
            {
                QMessageBox *Inform=new QMessageBox;
                Inform->setText("�Բ��𣡸��˻��ѱ�ע��\n�����������˺�");
                Inform->show();
            }
            else
            {
                if(Pstring!=Pstring_2)
                {
                    QMessageBox *Inform2=new QMessageBox;
                    Inform2->setText("�������벻��ͬ\n���ٴ�ȷ��");
                    Inform2->show();
                }
                else
                {
                    QFile files("Data\\"+IDstring+".txt");
                    if(!files.open(QIODevice::WriteOnly|QIODevice::Text))
                    {
                          QMessageBox *Inform3=new QMessageBox;
                          Inform3->setText(IDstring+".txt �ļ���ʧ��");
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
                                Inform4->setText("�ļ� IDPassword.txt ��ʧ��");
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
                            Inform3->setText("ע��ɹ�");

                            HostUser=new user(IDstring,Namestring,Pstring);

                            MainWindow *MainWin=new MainWindow;
                            if(!LoadQuestion())
                            {
                                QMessageBox *mm=new QMessageBox;
                                mm->setText("������Ϣ��ʧ");
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
