#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log_in.h"
#include "addquestion.h"
#include <QString>
#include <QListWidgetItem>
#include <QMessageBox>
#include "QAS_client.h"
#include "changepassword.h"
#include <QFile>
#include <QTextStream>
#include "questionwindow.h"
#include "userwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    page_number_1(1),
    currentPage_1(1),
    page_number_2(1),
    currentPage_2(1),
    page_number_3(1),
    currentPage_3(1)
{

    ui->setupUi(this);
    display_way=0;

    ui->pushButton_8->setFlat(true);
    ui->pushButton_9->setFlat(true);

    ui->list_down2->setFlat(true);
    ui->list_down3->setFlat(true);
    ui->list_up2->setFlat(true);
    ui->list_up3->setFlat(true);

    ui->lineEdit_keyword->setPlaceholderText("请输入时间，如：20151220, 代表问题发布的年月日");

    ui->comboBox_sort->addItem("按时间排序");
    ui->comboBox_sort->addItem("按热度值排序");
    ui->comboBox_search_way->addItem("按时间搜索");
    ui->comboBox_search_way->addItem("按关键字搜索");

    question_list_model=new QStandardItemModel();
    focus_list_model=new QStandardItemModel();
    myquestion_list_model=new QStandardItemModel();

    ui->QuestionList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->QuestionList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->QuestionList->setModel(question_list_model);


    ui->focuslistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->focuslistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->focuslistView->setModel(focus_list_model);

    ui->myQuestListview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->myQuestListview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->myQuestListview->setModel(myquestion_list_model);

    search_result=new QStandardItemModel;
    ui->tableView_result->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_result->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_result->setModel(search_result);

    connect(ui->tableView_result,SIGNAL(doubleClicked(QModelIndex)),   //绑定查看搜索结果的信号与槽
            this,SLOT(display_search_question(QModelIndex)));

    connect(ui->myQuestListview,SIGNAL(doubleClicked(QModelIndex)),    //绑定查看我的问题的信号与槽
            this,SLOT(displayMyQuestion(QModelIndex)));

    connect(ui->QuestionList,SIGNAL(doubleClicked(QModelIndex)),      //绑定查看主页问题的信号与槽
            this,SLOT(displayQuestion(QModelIndex)));

    connect(ui->focuslistView,SIGNAL(doubleClicked(QModelIndex)),     //绑定查看关注用户信息的信号与槽
            this,SLOT(display_focus_user(QModelIndex)));

    connect(ui->comboBox_sort,SIGNAL(currentIndexChanged(int)),      //绑定显示问题顺序改变的信号与槽
            this,SLOT(proxyChange(int)));

    connect(ui->comboBox_search_way,SIGNAL(currentIndexChanged(int)),  //绑定搜索方式改变的信号与槽
            this, SLOT(change_search_way(int)));

    timer=new QTimer;                                              //设置定时器的时间间隔
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));       //绑定定时器信号与槽

    //绑定搜索结果的信号与槽
    connect(Socket,SIGNAL(response_for_search()),this,SLOT(search_response()));

    //绑定问题列表更新的信号与槽
    connect(Socket,SIGNAL(response_for_updatequestions()),this,SLOT(update_response()));

    //绑定问题列表换页结果的信号与槽
    connect(Socket,SIGNAL(response_for_changepage_question()),
            this,SLOT(question_page_response()));

    //绑定我的问题列表换页结果的信号与槽
    connect(Socket,SIGNAL(response_for_changepage_myquestion()),
            this,SLOT(myquestion_page_response()));

    //绑定我的关注列表换页结果的信号与槽
    connect(Socket,SIGNAL(response_for_changepage_myfocus()),
            this,SLOT(focus_page_response()));

    //绑定我的问题列表刷新结果的信号与槽
    connect(Socket,SIGNAL(response_for_myquestion()),
            this,SLOT(myquestions_response()));

    //绑定我的关注列表刷新结果的信号与槽
    connect(Socket,SIGNAL(response_for_myfocus()),
            this,SLOT(myfocus_response()));

    //绑定显示我的问题的信号与槽
    connect(Socket,SIGNAL(response_for_select_question()),
            this,SLOT(select_question_response()));

    //绑定查看关注用户的信号与槽
    connect(Socket,SIGNAL(response_for_user()),
            this,SLOT(select_user_response()));

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete question_list_model;
    delete focus_list_model;
    delete myquestion_list_model;
    delete search_result;
    delete timer;
}

//修改搜索方式
void MainWindow::change_search_way(int index)
{
    if(index==0)
        ui->lineEdit_keyword->setPlaceholderText("请输入时间，如：20151220, 代表问题发布的年月日");
    else
        ui->lineEdit_keyword->setPlaceholderText("请输入关键字，如：C++  Map");
}

//修改密码
void MainWindow::on_pushButton_3_clicked()
{
    ChangePassword *newWindow=new ChangePassword();
    newWindow->setAttribute(Qt::WA_DeleteOnClose,true);
    newWindow->show();
}

//退出
void MainWindow::on_pushButton_clicked()
{

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("EXIT");
    Socket->sendMessage();

   // Log_in *logWin = new Log_in;
   // logWin->setAttribute(Qt::WA_DeleteOnClose,true);
    this->close();
    //logWin->show();
    w->show();

}

//提问
void MainWindow::on_pushButton_7_clicked()
{
    AddQuestion *questionWin = new AddQuestion(this);
    questionWin->setAttribute(Qt::WA_DeleteOnClose,true);
    questionWin->exec();
}

//所有问题列表下一页
void MainWindow::on_pushButton_8_clicked()
{
   if(currentPage_1<page_number_1)
   {
        currentPage_1++;

        timer->start();


        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("CHANGEPAGE_1")<<currentPage_1<<display_way;
        Socket->sendMessage();

        QString display1="",display2="";
        QString display=display1.setNum(currentPage_1)+"/"+display2.setNum(page_number_1);

        ui->label->setText(display);

        question_list_model->clear();
   }

}

//所有问题列表上一页
void MainWindow::on_pushButton_9_clicked()
{
    if(currentPage_1>1)
    {
        currentPage_1--;

        timer->start();


        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("CHANGEPAGE_1")<<currentPage_1<<display_way;
        Socket->sendMessage();

        QString display1="",display2="";
        QString display=display1.setNum(currentPage_1)+"/"+display2.setNum(page_number_1);

        ui->label->setText(display);

        question_list_model->clear();
    }
}

//排序方式改变
void MainWindow::proxyChange(int index)
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE");
    if(index == 0)
    {
        out<<tr("TIME");
        display_way = 0;

    }
    else
    {
        out<<tr("VALUE");
        display_way = 1;
    }
    Socket->sendMessage();
}

//刷新
void MainWindow::on_pushButton_2_pressed()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE");
    if(display_way == 0)
    {
        out<<tr("TIME");
    }
    else
    {
        out<<tr("VALUE");
    }
    Socket->sendMessage();
}


//我的问题列表刷新
void MainWindow::on_pushButton_4_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_MYQUESTION");
    Socket->sendMessage();

    ui->stackedWidget->setCurrentIndex(0);
}

//刷新我的关注列表
void MainWindow::on_pushButton_5_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_MYFOCUS");
    Socket->sendMessage();

    ui->stackedWidget->setCurrentIndex(1);
}

//显示问题
void MainWindow::displayQuestion(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question = question_list_model->item(Row)->text();

    timer->start();


    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();

}

//显示问题
void MainWindow::displayMyQuestion(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=myquestion_list_model->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();

}

//我的问题列表下一页
void MainWindow::on_list_down2_clicked()
{
    if(currentPage_2<page_number_2)
    {
        currentPage_2++;
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("CHANGEPAGE_2")<<currentPage_2;
        Socket->sendMessage();

        QString display1="",display2="";
        QString display=display1.setNum(currentPage_2)+"/"+display2.setNum(page_number_2);

        ui->label_2->setText(display);

        myquestion_list_model->clear();
    }
}

//我的问题列表上一页
void MainWindow::on_list_up2_clicked()
{
    if(currentPage_2>1)
    {
        currentPage_2--;
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("CHANGEPAGE_2")<<currentPage_2;
        Socket->sendMessage();

        QString display1="",display2="";
        QString display=display1.setNum(currentPage_2)+"/"+display2.setNum(page_number_2);

        ui->label_2->setText(display);

        myquestion_list_model->clear();
    }
}

//我的关注列表下一页
void MainWindow::on_list_down3_clicked()
{
    if(currentPage_3<page_number_3)
    {
        currentPage_3++;
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("CHANGEPAGE_3")<<currentPage_3;
        Socket->sendMessage();

        QString display1="",display2="";
        QString display=display1.setNum(currentPage_3)+"/"+display2.setNum(page_number_3);

        ui->label_3->setText(display);

        focus_list_model->clear();
    }
}

//我的关注列表上一页
void MainWindow::on_list_up3_clicked()
{
    if(currentPage_3>1)
    {
        currentPage_3--;
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("CHANGEPAGE_3")<<currentPage_3;
        Socket->sendMessage();

        QString display1="",display2="";
        QString display=display1.setNum(currentPage_3)+"/"+display2.setNum(page_number_3);

        ui->label_3->setText(display);

        focus_list_model->clear();
    }
}

//查看关注用户
void MainWindow::display_focus_user(QModelIndex index)
{
    int Row=index.row();
    QString id_of_focus=focus_list_model->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYUSER")<<id_of_focus;
    Socket->sendMessage();
}

//查看搜索问题
void MainWindow::display_search_question(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=search_result->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();
}

//搜索
void MainWindow::on_pushButton_search_clicked()
{
    QString content_search = ui->lineEdit_keyword->text();
    ui->lineEdit_keyword->clear();
    if(content_search == "")
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("搜索内容不能为空");
        msg->show();
    }
    else
    {
        timer->start();

        QDataStream out(&(Socket->block),QIODevice::WriteOnly);
        out<<tr("SEARCH");

        if(ui->comboBox_search_way->currentIndex()==0)
        {
            out<<tr("TIME");
        }
        else
        {
            out<<tr("KEYWORDS");
        }
        out<<content_search;

        Socket->sendMessage();

        search_result->clear();
    }
}

//初始化
void MainWindow::init()
{
    QDataStream in(Socket->block);

    int no_use;
    in>>no_use;

    QString host_name;
    in>>host_name;

    ui->label_hostname->setText(host_name);

    if(!in.atEnd())
    {
        int number_of_question;
        in>>number_of_question;

        page_number_1=number_of_question/11+1;
        currentPage_1=1;

        int count;
        in>>count;

        QString id_question;
        QString creator_name;
        QString question_title;           //初始化所有问题
        int value;

        question_list_model->clear();
        question_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
        question_list_model->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
        question_list_model->setHorizontalHeaderItem(2,new QStandardItem("标题"));
        question_list_model->setHorizontalHeaderItem(3,new QStandardItem("热度值"));

        ui->QuestionList->setColumnWidth(0,120);
        ui->QuestionList->setColumnWidth(1,120);
        ui->QuestionList->setColumnWidth(2,300);
        ui->QuestionList->horizontalHeader()->setStretchLastSection(true);
        ui->QuestionList->verticalHeader()->hide();
        for(int i=0;i<count;++i)
        {
            in>>id_question;
            in>>creator_name;
            in>>question_title;
            in>>value;

            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id_question));
            newRow.push_back(new QStandardItem(creator_name));
            newRow.push_back(new QStandardItem(question_title));
            newRow.push_back(new QStandardItem(QString::number(value)));
            question_list_model->appendRow(newRow);
        }

        QString display1="",display2="";
        QString display3=display1.setNum(currentPage_1)+"/"+display2.setNum(page_number_1);

        ui->label->setText(display3);

        int number_of_myquestion;
        in>>number_of_myquestion;

        page_number_2=number_of_myquestion/11+1;
        currentPage_2=1;

        in>>count;                    //初始化我的所有问题

        myquestion_list_model ->clear();
        myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
        myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("标题"));
        myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("热度值"));
        ui->myQuestListview->setColumnWidth(0,120);
        ui->myQuestListview->setColumnWidth(1,270);
        ui->myQuestListview->horizontalHeader()->setStretchLastSection(true);
        ui->myQuestListview->verticalHeader()->hide();

        for(int i=0;i<count;++i)
        {
            in>>id_question;
            in>>question_title;
            in>>value;

            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id_question));
            newRow.push_back(new QStandardItem(question_title));
            newRow.push_back(new QStandardItem(QString::number(value)));

            myquestion_list_model->appendRow(newRow);
        }

        QString display4="",display5="";
        QString display6=display4.setNum(currentPage_2)+"/"+display5.setNum(page_number_2);

        ui->label_2->setText(display6);

        int number_of_myfocus;
        in>>number_of_myfocus;

        page_number_3=number_of_myfocus/11+1;
        currentPage_3=1;

        QString id;
        QString name;                //初始化我的所有关注列表

        focus_list_model->clear();
        focus_list_model->setHorizontalHeaderItem(0,new QStandardItem("ID"));
        focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("昵称"));
        ui->focuslistView->setColumnWidth(0,200);
        ui->focuslistView->horizontalHeader()->setStretchLastSection(true);
        ui->focuslistView->verticalHeader()->hide();

        in>>count;
        for(int i=0;i<count;++i)
        {
            in>>id;
            in>>name;

            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id));
            newRow.push_back(new QStandardItem(name));

            focus_list_model->appendRow(newRow);
        }

        QString display7="",display8="";
        QString display9=display7.setNum(currentPage_3)+"/"+display8.setNum(page_number_3);

        ui->label_3->setText(display9);

        Socket->block.clear();
    }
}

//超时响应
void MainWindow::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("连接超时，请检查你的网络");
    msg->exec();
}

//搜索结果
void MainWindow::search_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int count;
    in>>count;


    search_result->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    search_result->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
    search_result->setHorizontalHeaderItem(2,new QStandardItem("标题"));
    search_result->setHorizontalHeaderItem(3,new QStandardItem("热度值"));
    ui->tableView_result->setColumnWidth(0,120);
    ui->tableView_result->setColumnWidth(1,120);
    ui->tableView_result->setColumnWidth(2,200);
    ui->tableView_result->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_result->verticalHeader()->hide();

    QString id;
    QString name_of_creator;
    QString title;
    int value;
    for(int i=0;i<count;++i)
    {
        in>>id;
        in>>name_of_creator;
        in>>title;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));
        newRow.push_back(new QStandardItem(name_of_creator));
        newRow.push_back(new QStandardItem(title));
        newRow.push_back(new QStandardItem(QString::number(value)));
        search_result->appendRow(newRow);
    }

    Socket->block.clear();
}

//更新结果
void MainWindow::update_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int number_of_question;
    in>>number_of_question;

    page_number_1=number_of_question/11+1;
    currentPage_1=1;

    int count;
    in>>count;

    QString id_question;
    QString creator_name;
    QString question_title;
    int value;

    question_list_model->clear();
    question_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    question_list_model->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
    question_list_model->setHorizontalHeaderItem(2,new QStandardItem("标题"));
    question_list_model->setHorizontalHeaderItem(3,new QStandardItem("热度值"));
    ui->QuestionList->setColumnWidth(0,120);
    ui->QuestionList->setColumnWidth(1,120);
    ui->QuestionList->setColumnWidth(2,300);
    ui->QuestionList->horizontalHeader()->setStretchLastSection(true);
    ui->QuestionList->verticalHeader()->hide();
    for(int i=0;i<count;++i)
    {
        in>>id_question;
        in>>creator_name;
        in>>question_title;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(creator_name));
        newRow.push_back(new QStandardItem(question_title));
        newRow.push_back(new QStandardItem(QString::number(value)));
        question_list_model->appendRow(newRow);
    }

    QString display1="",display2="";
    QString display=display1.setNum(currentPage_1)+"/"+display2.setNum(page_number_1);

    ui->label->setText(display);

    Socket->block.clear();
}

//问题翻页回应
void MainWindow::question_page_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString id_question;
    QString creator_name;
    QString question_title;
    int value;

    question_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    question_list_model->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
    question_list_model->setHorizontalHeaderItem(2,new QStandardItem("标题"));
    question_list_model->setHorizontalHeaderItem(3,new QStandardItem("热度值"));
    ui->QuestionList->setColumnWidth(0,120);
    ui->QuestionList->setColumnWidth(1,120);
    ui->QuestionList->setColumnWidth(2,300);
    ui->QuestionList->horizontalHeader()->setStretchLastSection(true);
    ui->QuestionList->verticalHeader()->hide();

    while(!in.atEnd())
    {
        in>>id_question;
        in>>creator_name;
        in>>question_title;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(creator_name));
        newRow.push_back(new QStandardItem(question_title));
        newRow.push_back(new QStandardItem(QString::number(value)));
        question_list_model->appendRow(newRow);
    }

    Socket->block.clear();
}

//我的问题翻页回应
void MainWindow::myquestion_page_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("标题"));
    myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("热度值"));
    ui->myQuestListview->setColumnWidth(0,120);
    ui->myQuestListview->setColumnWidth(1,270);
    ui->myQuestListview->horizontalHeader()->setStretchLastSection(true);
    ui->myQuestListview->verticalHeader()->hide();

    QString id_question;
    QString question_title;
    int value;
    while(!in.atEnd())
    {
        in>>id_question;
        in>>question_title;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(question_title));
        newRow.push_back(new QStandardItem(QString::number(value)));

        myquestion_list_model->appendRow(newRow);
    }

    Socket->block.clear();
}

//我的关注列表翻页回应
void MainWindow::focus_page_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString id;
    QString name;                //初始化我的所有关注列表

    focus_list_model->setHorizontalHeaderItem(0,new QStandardItem("ID"));
    focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("昵称"));
    ui->focuslistView->setColumnWidth(0,200);
    ui->focuslistView->horizontalHeader()->setStretchLastSection(true);
    ui->focuslistView->verticalHeader()->hide();

    while(!in.atEnd())
    {
        in>>id;
        in>>name;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));
        newRow.push_back(new QStandardItem(name));

        focus_list_model->appendRow(newRow);
    }

    Socket->block.clear();
}

//刷新我的问题列表回应
void MainWindow::myquestions_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int number_of_myquestion;
    in>>number_of_myquestion;

    page_number_2 = number_of_myquestion/11+1;
    currentPage_2 = 1;

    int count;
    in>>count;                    //初始化我的所有问题

    myquestion_list_model ->clear();
    myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("标题"));
    myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("热度值"));
    ui->myQuestListview->setColumnWidth(0,120);
    ui->myQuestListview->setColumnWidth(1,270);
    ui->myQuestListview->horizontalHeader()->setStretchLastSection(true);
    ui->myQuestListview->verticalHeader()->hide();
    QString id_question;
    QString question_title;
    int value;
    for(int i=0;i<count;++i)
    {
        in>>id_question;
        in>>question_title;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(question_title));
        newRow.push_back(new QStandardItem(QString::number(value)));

        myquestion_list_model->appendRow(newRow);
    }

    QString display1="",display2="";
    QString display=display1.setNum(currentPage_2)+"/"+display2.setNum(page_number_2);

    ui->label_2->setText(display);

    Socket->block.clear();
}

//刷新我的关注列表回应
void MainWindow::myfocus_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int number_of_myfocus;
    in>>number_of_myfocus;

    page_number_3=number_of_myfocus/11+1;
    currentPage_3=1;

    QString id;
    QString name;

    focus_list_model->clear();
    focus_list_model->setHorizontalHeaderItem(0,new QStandardItem("ID"));
    focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("昵称"));
    ui->focuslistView->setColumnWidth(0,200);
    ui->focuslistView->horizontalHeader()->setStretchLastSection(true);
    ui->focuslistView->verticalHeader()->hide();

    int count;
    in>>count;
    for(int i=0;i<count;++i)
    {
        in>>id;
        in>>name;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));
        newRow.push_back(new QStandardItem(name));

        focus_list_model->appendRow(newRow);
    }

    QString display1="",display2="";
    QString display=display1.setNum(currentPage_3)+"/"+display2.setNum(page_number_3);

    ui->label_3->setText(display);

    Socket->block.clear();
}


//提问回应
void MainWindow::ask_reaponse()
{
    timer->stop();
    QDataStream in(Socket->block);

    int is;
    in>>is;
    if(is)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("添加问题成功");
        msg->exec();
    }
    else
    {
        QMessageBox *msg1 = new QMessageBox;
        msg1->setText("添加问题失败");
        msg1->exec();
    }
}

//显示问题回应
void MainWindow::select_question_response()
{
    timer->stop();
    QuestionWindow *queWin = new QuestionWindow;
    queWin->setAttribute(Qt::WA_DeleteOnClose,true);
    queWin->init();
    queWin->show();
}

//查看关注用户的回应
void MainWindow::select_user_response()
{
    timer->stop();
    UserWindow *userWin = new UserWindow;
    userWin->setAttribute(Qt::WA_DeleteOnClose,true);
    userWin->init();
    userWin->show();
}
