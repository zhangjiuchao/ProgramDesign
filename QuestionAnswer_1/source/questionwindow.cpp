#include "questionwindow.h"
#include "ui_questionwindow.h"
#include <QDateTime>
#include "Question_Answer_System.h"
#include "answerinfor.h"
#include "answerwindow.h"
#include "QDebug"
#include <QMessageBox>
QuestionWindow::QuestionWindow(QuestInfor* prob,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionWindow),
    question(prob)
{
    ui->setupUi(this);

    ui->textBrowser->setText(question->gettitle());
    ui->textBrowser_2->setText(question->getcontent());
    QString idOfCreactor=question->getIDIDofCreator();
    QString time=question->getTimeofCreate();
    QString name=ID_Name.find(idOfCreactor).value();

    ui->label_name->setText(name);
    ui->label_time->setText(time);

    if(HostUser->isFocus(idOfCreactor))
    {
        ui->pushButton_4->setText("取消关注");
    }
    else
        ui->pushButton_4->setText("关注");

    answer_model=new QStandardItemModel();

    ui->tableView_answer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_answer->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView_answer->setModel(answer_model);

    connect(ui->tableView_answer,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayAnswer(QModelIndex)));

    displayAllAnswer();
}

QuestionWindow::~QuestionWindow()
{
    delete ui;
}

void QuestionWindow::displayAllAnswer()
{

    answer_model->clear();

    answer_model->setHorizontalHeaderItem(0,new QStandardItem("回答ID"));
    answer_model->setHorizontalHeaderItem(1,new QStandardItem("回答者"));
    answer_model->setHorizontalHeaderItem(2,new QStandardItem("回答内容"));
    answer_model->setHorizontalHeaderItem(3,new QStandardItem("赞"));
    ui->tableView_answer->setColumnWidth(0,120);
    ui->tableView_answer->setColumnWidth(1,120);
    ui->tableView_answer->setColumnWidth(2,180);
    ui->tableView_answer->setColumnWidth(3,50);

    QList<AnswerInfor*> allAnswer=question->getAnswer();

    for(int i=0;i<allAnswer.size();++i)
    {
        AnswerInfor* currentAnswer=allAnswer.value(i);

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(currentAnswer->getID()));

        QString name=ID_Name.find(currentAnswer->getIDIDofCreator()).value();
        newRow.push_back(new QStandardItem(name));
        newRow.push_back(new QStandardItem(currentAnswer->getcontent()));

        QString praiseStr="";
        praiseStr=praiseStr.setNum(currentAnswer->getpraiseNum());
        newRow.push_back(new QStandardItem(praiseStr));


        answer_model->appendRow(newRow);
    }
}

void QuestionWindow::on_pushButton_clicked()
{
    QDateTime dt=QDateTime::currentDateTime();
    QString time=dt.toString("yyyy-MM-dd-hh:mm:ss");
    QString id=dt.toString("yyyyMMddhhmmss");
    QString content=ui->textEdit->toPlainText();
    if(content!=""&&(!is_exist_space(content)))
    {
        question->CreateAnswer(id,HostUser->getID(),time,content,0);
        HostUser->AddAnswer(question->getID());
        question->add_popular_value();

        ui->textEdit->clear();

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));

        newRow.push_back(new QStandardItem(HostUser->getName()));
        newRow.push_back(new QStandardItem(content));

        QString praiseStr="";
        praiseStr=praiseStr.setNum(0);
        newRow.push_back(new QStandardItem(praiseStr));


        answer_model->insertRow(0,newRow);

    }
    else
    {
        QMessageBox *mm=new QMessageBox;
        mm->setText("信息中不允许有空格和回车出现");
        mm->show();
    }

}

void QuestionWindow::on_pushButton_4_clicked()
{
   bool isSuccess = HostUser->AddFocus(question->getIDIDofCreator());
   if(isSuccess)
   {
       ui->pushButton_4->setText("取消关注");
   }
   else
       ui->pushButton_4->setText("关注");

}

void QuestionWindow::displayAnswer(QModelIndex index)
{
    int Row=index.row();

    QList<AnswerInfor*> answers=question->getAnswer();

    AnswerInfor* current_answer=answers.value(Row);

    AnswerWindow* win=new AnswerWindow( current_answer,question);
    win->show();
}

void QuestionWindow::on_pushButton_2_clicked()
{
    displayAllAnswer();
}
