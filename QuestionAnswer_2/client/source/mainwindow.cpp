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

    ui->lineEdit_keyword->setPlaceholderText("������ʱ�䣬�磺20151220, �������ⷢ����������");

    ui->comboBox_sort->addItem("��ʱ������");
    ui->comboBox_sort->addItem("���ȶ�ֵ����");
    ui->comboBox_search_way->addItem("��ʱ������");
    ui->comboBox_search_way->addItem("���ؼ�������");

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

    connect(ui->tableView_result,SIGNAL(doubleClicked(QModelIndex)),   //�󶨲鿴����������ź����
            this,SLOT(display_search_question(QModelIndex)));

    connect(ui->myQuestListview,SIGNAL(doubleClicked(QModelIndex)),    //�󶨲鿴�ҵ�������ź����
            this,SLOT(displayMyQuestion(QModelIndex)));

    connect(ui->QuestionList,SIGNAL(doubleClicked(QModelIndex)),      //�󶨲鿴��ҳ������ź����
            this,SLOT(displayQuestion(QModelIndex)));

    connect(ui->focuslistView,SIGNAL(doubleClicked(QModelIndex)),     //�󶨲鿴��ע�û���Ϣ���ź����
            this,SLOT(display_focus_user(QModelIndex)));

    connect(ui->comboBox_sort,SIGNAL(currentIndexChanged(int)),      //����ʾ����˳��ı���ź����
            this,SLOT(proxyChange(int)));

    connect(ui->comboBox_search_way,SIGNAL(currentIndexChanged(int)),  //��������ʽ�ı���ź����
            this, SLOT(change_search_way(int)));

    timer=new QTimer;                                              //���ö�ʱ����ʱ����
    timer->setInterval(15000);

    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));       //�󶨶�ʱ���ź����

    //������������ź����
    connect(Socket,SIGNAL(response_for_search()),this,SLOT(search_response()));

    //�������б���µ��ź����
    connect(Socket,SIGNAL(response_for_updatequestions()),this,SLOT(update_response()));

    //�������б�ҳ������ź����
    connect(Socket,SIGNAL(response_for_changepage_question()),
            this,SLOT(question_page_response()));

    //���ҵ������б�ҳ������ź����
    connect(Socket,SIGNAL(response_for_changepage_myquestion()),
            this,SLOT(myquestion_page_response()));

    //���ҵĹ�ע�б�ҳ������ź����
    connect(Socket,SIGNAL(response_for_changepage_myfocus()),
            this,SLOT(focus_page_response()));

    //���ҵ������б�ˢ�½�����ź����
    connect(Socket,SIGNAL(response_for_myquestion()),
            this,SLOT(myquestions_response()));

    //���ҵĹ�ע�б�ˢ�½�����ź����
    connect(Socket,SIGNAL(response_for_myfocus()),
            this,SLOT(myfocus_response()));

    //����ʾ�ҵ�������ź����
    connect(Socket,SIGNAL(response_for_select_question()),
            this,SLOT(select_question_response()));

    //�󶨲鿴��ע�û����ź����
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

//�޸�������ʽ
void MainWindow::change_search_way(int index)
{
    if(index==0)
        ui->lineEdit_keyword->setPlaceholderText("������ʱ�䣬�磺20151220, �������ⷢ����������");
    else
        ui->lineEdit_keyword->setPlaceholderText("������ؼ��֣��磺C++  Map");
}

//�޸�����
void MainWindow::on_pushButton_3_clicked()
{
    ChangePassword *newWindow=new ChangePassword();
    newWindow->setAttribute(Qt::WA_DeleteOnClose,true);
    newWindow->show();
}

//�˳�
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

//����
void MainWindow::on_pushButton_7_clicked()
{
    AddQuestion *questionWin = new AddQuestion(this);
    questionWin->setAttribute(Qt::WA_DeleteOnClose,true);
    questionWin->exec();
}

//���������б���һҳ
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

//���������б���һҳ
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

//����ʽ�ı�
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

//ˢ��
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


//�ҵ������б�ˢ��
void MainWindow::on_pushButton_4_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_MYQUESTION");
    Socket->sendMessage();

    ui->stackedWidget->setCurrentIndex(0);
}

//ˢ���ҵĹ�ע�б�
void MainWindow::on_pushButton_5_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_MYFOCUS");
    Socket->sendMessage();

    ui->stackedWidget->setCurrentIndex(1);
}

//��ʾ����
void MainWindow::displayQuestion(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question = question_list_model->item(Row)->text();

    timer->start();


    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();

}

//��ʾ����
void MainWindow::displayMyQuestion(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=myquestion_list_model->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();

}

//�ҵ������б���һҳ
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

//�ҵ������б���һҳ
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

//�ҵĹ�ע�б���һҳ
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

//�ҵĹ�ע�б���һҳ
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

//�鿴��ע�û�
void MainWindow::display_focus_user(QModelIndex index)
{
    int Row=index.row();
    QString id_of_focus=focus_list_model->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYUSER")<<id_of_focus;
    Socket->sendMessage();
}

//�鿴��������
void MainWindow::display_search_question(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=search_result->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();
}

//����
void MainWindow::on_pushButton_search_clicked()
{
    QString content_search = ui->lineEdit_keyword->text();
    ui->lineEdit_keyword->clear();
    if(content_search == "")
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("�������ݲ���Ϊ��");
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

//��ʼ��
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
        QString question_title;           //��ʼ����������
        int value;

        question_list_model->clear();
        question_list_model->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
        question_list_model->setHorizontalHeaderItem(1,new QStandardItem("�������ǳ�"));
        question_list_model->setHorizontalHeaderItem(2,new QStandardItem("����"));
        question_list_model->setHorizontalHeaderItem(3,new QStandardItem("�ȶ�ֵ"));

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

        in>>count;                    //��ʼ���ҵ���������

        myquestion_list_model ->clear();
        myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
        myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("����"));
        myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("�ȶ�ֵ"));
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
        QString name;                //��ʼ���ҵ����й�ע�б�

        focus_list_model->clear();
        focus_list_model->setHorizontalHeaderItem(0,new QStandardItem("ID"));
        focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("�ǳ�"));
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

//��ʱ��Ӧ
void MainWindow::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ�������������");
    msg->exec();
}

//�������
void MainWindow::search_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int count;
    in>>count;


    search_result->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    search_result->setHorizontalHeaderItem(1,new QStandardItem("�������ǳ�"));
    search_result->setHorizontalHeaderItem(2,new QStandardItem("����"));
    search_result->setHorizontalHeaderItem(3,new QStandardItem("�ȶ�ֵ"));
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

//���½��
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
    question_list_model->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    question_list_model->setHorizontalHeaderItem(1,new QStandardItem("�������ǳ�"));
    question_list_model->setHorizontalHeaderItem(2,new QStandardItem("����"));
    question_list_model->setHorizontalHeaderItem(3,new QStandardItem("�ȶ�ֵ"));
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

//���ⷭҳ��Ӧ
void MainWindow::question_page_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString id_question;
    QString creator_name;
    QString question_title;
    int value;

    question_list_model->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    question_list_model->setHorizontalHeaderItem(1,new QStandardItem("�������ǳ�"));
    question_list_model->setHorizontalHeaderItem(2,new QStandardItem("����"));
    question_list_model->setHorizontalHeaderItem(3,new QStandardItem("�ȶ�ֵ"));
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

//�ҵ����ⷭҳ��Ӧ
void MainWindow::myquestion_page_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("����"));
    myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("�ȶ�ֵ"));
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

//�ҵĹ�ע�б�ҳ��Ӧ
void MainWindow::focus_page_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString id;
    QString name;                //��ʼ���ҵ����й�ע�б�

    focus_list_model->setHorizontalHeaderItem(0,new QStandardItem("ID"));
    focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("�ǳ�"));
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

//ˢ���ҵ������б��Ӧ
void MainWindow::myquestions_response()
{
    timer->stop();
    QDataStream in(Socket->block);

    int number_of_myquestion;
    in>>number_of_myquestion;

    page_number_2 = number_of_myquestion/11+1;
    currentPage_2 = 1;

    int count;
    in>>count;                    //��ʼ���ҵ���������

    myquestion_list_model ->clear();
    myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("����"));
    myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("�ȶ�ֵ"));
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

//ˢ���ҵĹ�ע�б��Ӧ
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
    focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("�ǳ�"));
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


//���ʻ�Ӧ
void MainWindow::ask_reaponse()
{
    timer->stop();
    QDataStream in(Socket->block);

    int is;
    in>>is;
    if(is)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("�������ɹ�");
        msg->exec();
    }
    else
    {
        QMessageBox *msg1 = new QMessageBox;
        msg1->setText("�������ʧ��");
        msg1->exec();
    }
}

//��ʾ�����Ӧ
void MainWindow::select_question_response()
{
    timer->stop();
    QuestionWindow *queWin = new QuestionWindow;
    queWin->setAttribute(Qt::WA_DeleteOnClose,true);
    queWin->init();
    queWin->show();
}

//�鿴��ע�û��Ļ�Ӧ
void MainWindow::select_user_response()
{
    timer->stop();
    UserWindow *userWin = new UserWindow;
    userWin->setAttribute(Qt::WA_DeleteOnClose,true);
    userWin->init();
    userWin->show();
}
