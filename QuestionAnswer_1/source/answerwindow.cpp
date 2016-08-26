#include "answerwindow.h"
#include "ui_answerwindow.h"
#include "Question_Answer_System.h"
#include "QMessageBox"

AnswerWindow::AnswerWindow( AnswerInfor* answer,QuestInfor* quest,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnswerWindow),  
    currentAnswer(answer),
    host_question(quest)
{
    ui->setupUi(this);
    QString id=currentAnswer->getIDIDofCreator();
    QString name_of_creactor=ID_Name.find(id).value();
    ui->label_name->setText(name_of_creactor);

    QString praisenum_str="";
    praisenum_str.setNum(currentAnswer->getpraiseNum());
    QString str="";
    if(currentAnswer->isPraise(HostUser->getID()))
    {
        str="����";
        ui->pushButton_praise->setEnabled(false);
    }
    else
    {
        str="��";
    }

    if(HostUser->isFocus(id))
    {
         ui->pushButton_focus->setText("ȡ����ע");
    }
    else
        ui->pushButton_focus->setText("��ע");
    str=str+"("+praisenum_str+")";

    ui->pushButton_praise->setText(str);

    ui->textBrowser->setText(currentAnswer->getcontent());

    QString idOfCreactor=currentAnswer->getIDIDofCreator();
    QString time=currentAnswer->getTimeofCreate();
    QString name=ID_Name.find(idOfCreactor).value();

    ui->label_name->setText(name);
    ui->label_time->setText(time);


}

AnswerWindow::~AnswerWindow()
{
    delete ui;
}

void AnswerWindow::on_pushButton_focus_clicked()
{
    QString id_of_creactor=currentAnswer->getIDIDofCreator();
    bool isSuccess=HostUser->AddFocus(id_of_creactor);
    if(isSuccess)
        ui->pushButton_focus->setText("ȡ����ע");
    else
        ui->pushButton_focus->setText("��ע");

}

void AnswerWindow::on_pushButton_praise_clicked()
{
        currentAnswer->addPraise(HostUser->getID());
        host_question->add_popular_value();
        QString praisenum_str="";
        praisenum_str.setNum(currentAnswer->getpraiseNum());
        QString str="����";
        str=str+"("+praisenum_str+")";
        ui->pushButton_praise->setText(str);
        ui->pushButton_praise->setEnabled(false);
}
