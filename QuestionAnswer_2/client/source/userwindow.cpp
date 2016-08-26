#include "userwindow.h"
#include "ui_userwindow.h"
#include "QAS_client.h"
#include "questionwindow.h"
#include "QMessageBox"
UserWindow::UserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);


    timer=new QTimer;
    timer->setInterval(15000);

    questionlist_ask=new QStandardItemModel();
    questionlist_reply=new QStandardItemModel();

    ui->tableView_ask->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_ask->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_ask->setModel(questionlist_ask);

    ui->tableView_reply->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_reply->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_reply->setModel(questionlist_reply);

    //�󶨲鿴�û��������ź����
    connect(ui->tableView_ask,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayquestion_ask(QModelIndex)));

    //�󶨲鿴�û��ش�������ź����
    connect(ui->tableView_reply,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayquestion_reply(QModelIndex)));


    //���û������б�ˢ�½�����ź����
    connect(Socket,SIGNAL(response_for_update_askque()),
            this,SLOT(update_ask()));

    //���û��ش������б�ˢ�½�����ź����
    connect(Socket,SIGNAL(response_for_update_replyque()),
            this,SLOT(update_reply()));

    //�󶨹�ע������ź����
    connect(Socket,SIGNAL(response_for_focus()),
            this,SLOT(focus_res()));


    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

}

UserWindow::~UserWindow()
{
    delete ui;
    delete timer;
    delete questionlist_ask;
    delete questionlist_reply;
}


//���ڳ�ʼ��
void UserWindow::init()
{
    QDataStream in(Socket->block);

    in>>id_user;

    QString name_user;
    in>>name_user;
    ui->label_name->setText(name_user);

    int is_focus;
    in>>is_focus;

    if(is_focus)
    {
        ui->pushButton_focus->setText("ȡ����ע");
    }
    else
        ui->pushButton_focus->setText("��ע");

    int number_askque;
    in>>number_askque;

    QString id_question;
    QString title_question;
    int value;

    questionlist_ask->clear();
    questionlist_ask->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    questionlist_ask->setHorizontalHeaderItem(1,new QStandardItem("����"));
    questionlist_ask->setHorizontalHeaderItem(2,new QStandardItem("�ȶ�ֵ"));
    ui->tableView_ask->setColumnWidth(0,120);
    ui->tableView_ask->setColumnWidth(1,250);
    ui->tableView_ask->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_ask->verticalHeader()->hide();

    for(int i=0;i<number_askque;++i)
    {
        in>>id_question;
        in>>title_question;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(title_question));
        newRow.push_back(new QStandardItem(QString::number(value)));
        questionlist_ask->appendRow(newRow);
    }

    int number_replyque;
    in>>number_replyque;

    QString name_creator;

    questionlist_reply->clear();
    questionlist_reply->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    questionlist_reply->setHorizontalHeaderItem(1,new QStandardItem("�������ǳ�"));
    questionlist_reply->setHorizontalHeaderItem(2,new QStandardItem("����"));
    questionlist_reply->setHorizontalHeaderItem(3,new QStandardItem("�ȶ�ֵ"));
    ui->tableView_reply->setColumnWidth(0,120);
    ui->tableView_reply->setColumnWidth(1,120);
    ui->tableView_reply->setColumnWidth(2,150);
    ui->tableView_reply->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_reply->verticalHeader()->hide();

    for(int i=0;i<number_replyque;++i)
    {
        in>>id_question;
        in>>name_creator;
        in>>title_question;
        in>>value;
        qDebug()<<id_question;
        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(name_creator));
        newRow.push_back(new QStandardItem(title_question));
        newRow.push_back(new QStandardItem(QString::number(value)));
        questionlist_reply->appendRow(newRow);
    }

    Socket->block.clear();
}


//�鿴�û����������
void UserWindow::displayquestion_ask(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question = questionlist_ask->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();
}

//�鿴�û��ش���������
void UserWindow::displayquestion_reply(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=questionlist_reply->item(Row)->text();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYQUESTION")<<id_of_question;
    Socket->sendMessage();
}

//ˢ���û��ش�������б����
void UserWindow::on_pushButton_update_1_clicked()
{

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_REPLY")<<id_user;
    Socket->sendMessage();
}


//ˢ���û��������б����
void UserWindow::on_pushButton_update_2_clicked()
{

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_ASK")<<id_user;
    Socket->sendMessage();
}


//��ע�û�����
void UserWindow::on_pushButton_focus_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("FOCUS")<<id_user;
    Socket->sendMessage();
}

//��ʾ�û�����
void UserWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//��ʾ�û��ش�����
void UserWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


//��ʱ��Ӧ
void UserWindow::time_out()
{
    timer->stop();

    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ�������������");
    msg->exec();
}

//������������صĸ����û������б���
void UserWindow::update_ask()
{
    timer->stop();
    QDataStream in(Socket->block);

    questionlist_ask->clear();
    questionlist_ask->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    questionlist_ask->setHorizontalHeaderItem(1,new QStandardItem("����"));
    questionlist_ask->setHorizontalHeaderItem(2,new QStandardItem("�ȶ�ֵ"));
    ui->tableView_ask->setColumnWidth(0,120);
    ui->tableView_ask->setColumnWidth(1,250);
    ui->tableView_ask->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_ask->verticalHeader()->hide();

    int number_askque;
    in>>number_askque;

    QString id_question;
    QString title_question;
    int value;
    for(int i=0;i<number_askque;++i)
    {
        in>>id_question;
        in>>title_question;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(title_question));
        newRow.push_back(new QStandardItem(QString::number(value)));
        questionlist_ask->appendRow(newRow);
    }
    Socket->block.clear();
}


//������������صĸ����û��ش������б���
void UserWindow::update_reply()
{
    timer->stop();
    QDataStream in(Socket->block);

    questionlist_reply->clear();
    questionlist_reply->setHorizontalHeaderItem(0,new QStandardItem("����ID"));
    questionlist_reply->setHorizontalHeaderItem(1,new QStandardItem("�������ǳ�"));
    questionlist_reply->setHorizontalHeaderItem(2,new QStandardItem("����"));
    questionlist_reply->setHorizontalHeaderItem(3,new QStandardItem("�ȶ�ֵ"));
    ui->tableView_reply->setColumnWidth(0,120);
    ui->tableView_reply->setColumnWidth(1,120);
    ui->tableView_reply->setColumnWidth(2,150);
    ui->tableView_reply->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_reply->verticalHeader()->hide();

    int number_replyque;
    in>>number_replyque;

    QString id_question;
    QString name_creator;
    QString title_question;
    int value;
    for(int i=0;i<number_replyque;++i)
    {
        in>>id_question;
        in>>name_creator;
        in>>title_question;
        in>>value;

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id_question));
        newRow.push_back(new QStandardItem(name_creator));
        newRow.push_back(new QStandardItem(title_question));
        newRow.push_back(new QStandardItem(QString::number(value)));
        questionlist_reply->appendRow(newRow);
    }

    Socket->block.clear();
}


//������������صĹ�ע���
void UserWindow::focus_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString focus_id;
    in>>focus_id;


    if(focus_id == id_user)   //�����ע��Ӧ�ǵ�ǰ�û�ʱ�����в���
    {
        int isSuccess;
        in>>isSuccess;
        if(isSuccess)
            ui->pushButton_focus->setText("ȡ����ע");
        else
            ui->pushButton_focus->setText("��ע");
    }

}
