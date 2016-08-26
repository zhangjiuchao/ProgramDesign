#include "addquestion.h"
#include "ui_addquestion.h"
#include "mainwindow.h"
#include "Question_Answer_System.h"
#include "QMessageBox"
AddQuestion::AddQuestion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddQuestion)
{
    ui->setupUi(this);
    this->setFocus();
    ui->lineEdit_title->setPlaceholderText("������Ŀ");
}

AddQuestion::~AddQuestion()
{
    delete ui;
}

void AddQuestion::on_pushButton_2_clicked()
{
    this->close();
}

void AddQuestion::on_pushButton_clicked()
{
    QString title=ui->lineEdit_title->text();
    QString content=ui->textEdit_content->toPlainText();

    if(title==""||content==""||is_exist_space(title)||is_exist_space(content))
    {
        QMessageBox *mm=new QMessageBox;
        mm->setText("��Ϣ�в������пո��س�����");
        mm->show();
    }
    else
    {
        HostUser->AddQuest(title,content);

        this->close();
        QMessageBox *mm=new QMessageBox;
        mm->setText("��������ɹ�");
        mm->show();
    }

}
