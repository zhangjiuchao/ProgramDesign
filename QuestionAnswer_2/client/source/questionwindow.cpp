#include "questionwindow.h"
#include "ui_questionwindow.h"
#include <QDateTime>
#include "QAS_client.h"
#include "answerwindow.h"
#include "QDebug"
#include <QMessageBox>
QuestionWindow::QuestionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionWindow)

{
    ui->setupUi(this);

    timer = new QTimer;
    timer->setInterval(10000);

    answer_model=new QStandardItemModel();   //显示回答列表的控件

    ui->tableView_answer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_answer->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView_answer->setModel(answer_model);

    //绑定查看回答操作的信号与槽
    connect(ui->tableView_answer,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayAnswer(QModelIndex)));

    //绑定定时器超时的信号与槽
    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //绑定关注结果的信号与槽
    connect(Socket,SIGNAL(response_for_focus()),
            this,SLOT(focus_res()));

    //绑定查看回答回应的信号与槽
    connect(Socket,SIGNAL(response_for_select_answer()),
            this,SLOT(answer_res()));

    //绑定回答列表更新结果的信号与槽
    connect(Socket,SIGNAL(response_for_answers()),
            this,SLOT(update_answer()));

    //绑定添加回答结果的信号与槽
    connect(Socket,SIGNAL(response_for_addanswer()),
            this,SLOT(add_answer_res()));
}

QuestionWindow::~QuestionWindow()
{
    delete ui;
    delete timer;
    delete answer_model;
}

//窗口初始化
void QuestionWindow::init()
{
    QDataStream in(Socket->block);
    in>>id_question;
    in>>id_user;

    QString name_creator;
    in>>name_creator;
    ui->label_name->setText(name_creator);

    QString time_creator;
    in>>time_creator;
    ui->label_time->setText(time_creator);

    int isFocus;
    in>>isFocus;

    if(isFocus)
    {
        ui->pushButton_4->setText("取消关注");
    }
    else
        ui->pushButton_4->setText("关注");

    QString title_question;
    in>>title_question;
    ui->textBrowser->setText(title_question);

    QString content;
    in>>content;
    ui->textBrowser_2->setText(content);

    answer_model->clear();

    answer_model->setHorizontalHeaderItem(0,new QStandardItem("回答ID"));
    answer_model->setHorizontalHeaderItem(1,new QStandardItem("回答者"));
    answer_model->setHorizontalHeaderItem(2,new QStandardItem("回答内容"));
    answer_model->setHorizontalHeaderItem(3,new QStandardItem("赞"));
    ui->tableView_answer->setColumnWidth(0,120);
    ui->tableView_answer->setColumnWidth(1,120);
    ui->tableView_answer->setColumnWidth(2,180);
    ui->tableView_answer->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_answer->verticalHeader()->hide();

    int number_answers;
    in>>number_answers;

    QString id;
    int praiseNum;

    for(int i=0;i<number_answers;++i)
    {
        in>>id;
        in>>name_creator;
        in>>content;
        in>>praiseNum;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));
        newRow.push_back(new QStandardItem(name_creator));
        newRow.push_back(new QStandardItem(content));
        newRow.push_back(new QStandardItem(QString::number(praiseNum)));
        answer_model->appendRow(newRow);
    }

    Socket->block.clear();
}


//回答
void QuestionWindow::on_pushButton_clicked()
{
    QString content = ui->textEdit->toPlainText();
    if(content!=""&&(!is_exist_space(content)))
    {
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);

        out<<tr("ADD_ANSWER")<<id_question<<content;
        Socket->sendMessage();
        ui->textEdit->clear();
    }
    else
    {
        QMessageBox *mm = new QMessageBox;
        mm->setText("信息中不允许有空格和回车出现");
        mm->show();
    }

}

//关注题主
void QuestionWindow::on_pushButton_4_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("FOCUS")<<id_user;
    Socket->sendMessage();

}

//查看回答
void QuestionWindow::displayAnswer(QModelIndex index)
{
    int Row=index.row();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYANSWER")<<id_question<<Row;
    Socket->sendMessage();

}

//刷新回答列表
void QuestionWindow::on_pushButton_2_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_ANSWERS")<<id_question;
    Socket->sendMessage();
}

//超时响应
void QuestionWindow::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("连接超时，请检查您的网络");
    msg->exec();
}


//处理服务器返回的关注结果
void QuestionWindow::focus_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString focus_id;
    in>>focus_id;
    if(focus_id == id_user)        //如果关注响应是当前用户时，进行操作
    {
        int isSuccess;
        in>>isSuccess;

        if(isSuccess)
            ui->pushButton_4->setText("取消关注");
        else
            ui->pushButton_4->setText("关注");
    }

}


//处理服务器返回的回答列表的更新结果
void QuestionWindow::update_answer()
{
    timer->stop();
    QDataStream in(Socket->block);

    answer_model->clear();

    answer_model->setHorizontalHeaderItem(0,new QStandardItem("回答ID"));
    answer_model->setHorizontalHeaderItem(1,new QStandardItem("回答者"));
    answer_model->setHorizontalHeaderItem(2,new QStandardItem("回答内容"));
    answer_model->setHorizontalHeaderItem(3,new QStandardItem("赞"));
    ui->tableView_answer->setColumnWidth(0,120);
    ui->tableView_answer->setColumnWidth(1,120);
    ui->tableView_answer->setColumnWidth(2,180);
    ui->tableView_answer->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_answer->verticalHeader()->hide();

    int number_answers;
    in>>number_answers;

    QString id_answer;
    QString name_creator;
    QString content;
    int praiseNum;

    for(int i=0;i<number_answers;++i)
    {
        in>>id_answer;
        in>>name_creator;
        in>>content;
        in>>praiseNum;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_answer));
        newRow.push_back(new QStandardItem(name_creator));
        newRow.push_back(new QStandardItem(content));
        newRow.push_back(new QStandardItem(QString::number(praiseNum)));
        answer_model->appendRow(newRow);
    }

    Socket->block.clear();
}


//处理服务器返回的添加回答结果
void QuestionWindow::add_answer_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    int success;
    in>>success;

    if(success)
    {
        QString id_answer;
        QString name_creator;
        QString content;

        in>>id_answer;
        in>>name_creator;
        in>>content;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_answer));
        newRow.push_back(new QStandardItem(name_creator));
        newRow.push_back(new QStandardItem(content));
        newRow.push_back(new QStandardItem("0"));

        answer_model->insertRow(0,newRow);
        Socket->block.clear();
        QMessageBox *msg = new QMessageBox;
        msg->setText("创建回答成功");
        msg->exec();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("创建回答失败");
        msg->exec();
    }
}

//服务器返回查看某一回答的详细信息
void QuestionWindow::answer_res()
{
    timer->stop();
    AnswerWindow *ansWin = new AnswerWindow;
    ansWin->setAttribute(Qt::WA_DeleteOnClose,true);
    ansWin->init();
    ansWin->show();
}
