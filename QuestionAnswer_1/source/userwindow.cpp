#include "userwindow.h"
#include "ui_userwindow.h"
#include "Question_Answer_System.h"
#include "questionwindow.h"
UserWindow::UserWindow(user* client,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserWindow),
    currentUser(client)
{
    ui->setupUi(this);
    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    ui->label_name->setText(currentUser->getName());
    if(HostUser->isFocus(currentUser->getID()))
    {
        ui->pushButton_focus->setText("取消关注");
    }
    else
        ui->pushButton_focus->setText("关注");

    questionlist_ask=new QStandardItemModel();
    questionlist_reply=new QStandardItemModel();

    ui->tableView_ask->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_ask->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_ask->setModel(questionlist_ask);

    ui->tableView_reply->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_reply->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_reply->setModel(questionlist_reply);

    connect(ui->tableView_ask,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayquestion_ask(QModelIndex)));
    connect(ui->tableView_reply,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayquestion_reply(QModelIndex)));
    set_questionlist_ask();
    set_questionlist_reply();
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::set_questionlist_ask()
{
    questionlist_ask->clear();
    questionlist_ask->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    questionlist_ask->setHorizontalHeaderItem(1,new QStandardItem("标题"));
    questionlist_ask->setHorizontalHeaderItem(2,new QStandardItem("热度值"));
    ui->tableView_ask->setColumnWidth(0,120);
    ui->tableView_ask->setColumnWidth(1,250);
    ui->tableView_ask->setColumnWidth(2,50);
    QList<QString> myquest = currentUser->getMyQuestion();

    int it = myquest.size()-1;
    for(;it>=0;--it)
    {
        QString id=myquest.value(it);
        QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
        if(iter!=questMap.end())
        {
            QuestInfor *currentQuestion=iter.value();

            QString title="";
            if(currentQuestion!=NULL)
            {
                title = currentQuestion->gettitle();
            }


            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id));
            newRow.push_back(new QStandardItem(title));
            newRow.push_back(new QStandardItem(QString::number(currentQuestion->get_popular_value())));
            questionlist_ask->appendRow(newRow);
        }
    }
}


void UserWindow::set_questionlist_reply()
{
    questionlist_reply->clear();
    questionlist_reply->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    questionlist_reply->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
    questionlist_reply->setHorizontalHeaderItem(2,new QStandardItem("标题"));
    questionlist_reply->setHorizontalHeaderItem(3,new QStandardItem("热度值"));
    ui->tableView_reply->setColumnWidth(0,120);
    QList<QString> quest = currentUser->get_question_my_anwser();
    int it = quest.size()-1;
    for(;it>=0;--it)
    {

          QString id=quest.value(it);
          QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);

          if(iter!=questMap.end())
          {

               QuestInfor *currentQuestion=iter.value();

               QString title="";
               if(currentQuestion!=NULL)
               {
                    title = currentQuestion->gettitle();
               }

               QString id_of_creator=currentQuestion->getIDIDofCreator();
               QString name_of_creator=ID_Name.find(id_of_creator).value();
               QList<QStandardItem*> newRow;
               newRow.push_back(new QStandardItem(id));
               newRow.push_back(new QStandardItem(name_of_creator));
               newRow.push_back(new QStandardItem(title));
               newRow.push_back(new QStandardItem(QString::number(currentQuestion->get_popular_value())));
               questionlist_reply->appendRow(newRow);
          }
     }
}

void UserWindow::displayquestion_ask(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=questionlist_ask->item(Row)->text();

    QuestInfor* currentQuestion=questMap.find(id_of_question).value();

    QuestionWindow *Win=new QuestionWindow(currentQuestion,this);
    Win->show();
}

void UserWindow::displayquestion_reply(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=questionlist_reply->item(Row)->text();

    QuestInfor* currentQuestion=questMap.find(id_of_question).value();

    QuestionWindow *Win=new QuestionWindow(currentQuestion,this);
    Win->show();
}

void UserWindow::on_pushButton_update_1_clicked()
{
    set_questionlist_reply();
}

void UserWindow::on_pushButton_update_2_clicked()
{
    set_questionlist_ask();
}

void UserWindow::on_pushButton_focus_clicked()
{
    bool isSuccess = HostUser->AddFocus(currentUser->getID());
    if(isSuccess)
    {
        ui->pushButton_focus->setText("取消关注");
    }
    else
        ui->pushButton_focus->setText("关注");
}

void UserWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UserWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
