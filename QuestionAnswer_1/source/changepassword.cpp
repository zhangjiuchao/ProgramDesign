#include "changepassword.h"
#include "ui_changepassword.h"
#include "Question_Answer_System.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
ChangePassword::ChangePassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    this->setFocus();
    ui->lineEdit->setPlaceholderText("原密码");
    ui->lineEdit_2->setPlaceholderText("新密码");
    ui->lineEdit_3->setPlaceholderText("确认新密码");
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_pushButton_clicked()
{
    QString oldPassword=ui->lineEdit->text();
    QString newPassword1=ui->lineEdit_2->text();
    QString newPassword2=ui->lineEdit_3->text();
    if(oldPassword==""||newPassword1==""||newPassword2=="")
    {
        QMessageBox *message=new QMessageBox;
        message->setText("请按要求填写信息");
        message->show();
    }
    else
    {
        if(is_exist_space(newPassword1))
        {
            QMessageBox *mm=new QMessageBox;
            mm->setText("信息中不允许有空格和回车出现");
            mm->show();
        }
        else
        {
            bool ischanged=HostUser->changePassword(oldPassword,newPassword1,newPassword2);
            if(ischanged)
            {

                QString str="Data\\IDPassword.txt";
                QFile file1(str);
                try{

                    if(!file1.open(QIODevice::WriteOnly))
                        throw QString("文件打开失败");

                    QTextStream out(&file1);
                    QMap<QString,QString>::const_iterator it=ID_Password.begin(),iter_id_name;
                    for(;it!=ID_Password.end();++it)
                    {
                        QString id=it.key();
                        out<<it.key()<<"   ";
                        out<<it.value()<<"  ";

                        iter_id_name=ID_Name.find(id);
                        out<<iter_id_name.value()<<"\r\n";
                    }
                    QMessageBox *message2=new QMessageBox;
                    message2->setText("密码修改成功");
                    this->close();
                    file1.close();
                    message2->exec();
                }
                catch(QString message)
                {
                    QMessageBox *msg=new QMessageBox;
                    msg->setText(message);
                    msg->exec();
                }


            }
        }
    }

}

void ChangePassword::on_pushButton_2_clicked()
{
    this->close();
}
