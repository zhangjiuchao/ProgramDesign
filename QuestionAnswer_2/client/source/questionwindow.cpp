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

    answer_model=new QStandardItemModel();   //��ʾ�ش��б�Ŀؼ�

    ui->tableView_answer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_answer->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView_answer->setModel(answer_model);

    //�󶨲鿴�ش�������ź����
    connect(ui->tableView_answer,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayAnswer(QModelIndex)));

    //�󶨶�ʱ����ʱ���ź����
    connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //�󶨹�ע������ź����
    connect(Socket,SIGNAL(response_for_focus()),
            this,SLOT(focus_res()));

    //�󶨲鿴�ش��Ӧ���ź����
    connect(Socket,SIGNAL(response_for_select_answer()),
            this,SLOT(answer_res()));

    //�󶨻ش��б���½�����ź����
    connect(Socket,SIGNAL(response_for_answers()),
            this,SLOT(update_answer()));

    //����ӻش������ź����
    connect(Socket,SIGNAL(response_for_addanswer()),
            this,SLOT(add_answer_res()));
}

QuestionWindow::~QuestionWindow()
{
    delete ui;
    delete timer;
    delete answer_model;
}

//���ڳ�ʼ��
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
        ui->pushButton_4->setText("ȡ����ע");
    }
    else
        ui->pushButton_4->setText("��ע");

    QString title_question;
    in>>title_question;
    ui->textBrowser->setText(title_question);

    QString content;
    in>>content;
    ui->textBrowser_2->setText(content);

    answer_model->clear();

    answer_model->setHorizontalHeaderItem(0,new QStandardItem("�ش�ID"));
    answer_model->setHorizontalHeaderItem(1,new QStandardItem("�ش���"));
    answer_model->setHorizontalHeaderItem(2,new QStandardItem("�ش�����"));
    answer_model->setHorizontalHeaderItem(3,new QStandardItem("��"));
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


//�ش�
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
        mm->setText("��Ϣ�в������пո�ͻس�����");
        mm->show();
    }

}

//��ע����
void QuestionWindow::on_pushButton_4_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("FOCUS")<<id_user;
    Socket->sendMessage();

}

//�鿴�ش�
void QuestionWindow::displayAnswer(QModelIndex index)
{
    int Row=index.row();

    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("DISPLAYANSWER")<<id_question<<Row;
    Socket->sendMessage();

}

//ˢ�»ش��б�
void QuestionWindow::on_pushButton_2_clicked()
{
    timer->start();

    QDataStream out(&(Socket->block),QIODevice::WriteOnly);
    out<<tr("UPDATE_ANSWERS")<<id_question;
    Socket->sendMessage();
}

//��ʱ��Ӧ
void QuestionWindow::time_out()
{
    timer->stop();
    QMessageBox *msg = new QMessageBox;
    msg->setText("���ӳ�ʱ��������������");
    msg->exec();
}


//������������صĹ�ע���
void QuestionWindow::focus_res()
{
    timer->stop();
    QDataStream in(Socket->block);

    QString focus_id;
    in>>focus_id;
    if(focus_id == id_user)        //�����ע��Ӧ�ǵ�ǰ�û�ʱ�����в���
    {
        int isSuccess;
        in>>isSuccess;

        if(isSuccess)
            ui->pushButton_4->setText("ȡ����ע");
        else
            ui->pushButton_4->setText("��ע");
    }

}


//������������صĻش��б�ĸ��½��
void QuestionWindow::update_answer()
{
    timer->stop();
    QDataStream in(Socket->block);

    answer_model->clear();

    answer_model->setHorizontalHeaderItem(0,new QStandardItem("�ش�ID"));
    answer_model->setHorizontalHeaderItem(1,new QStandardItem("�ش���"));
    answer_model->setHorizontalHeaderItem(2,new QStandardItem("�ش�����"));
    answer_model->setHorizontalHeaderItem(3,new QStandardItem("��"));
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


//������������ص���ӻش���
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
        msg->setText("�����ش�ɹ�");
        msg->exec();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("�����ش�ʧ��");
        msg->exec();
    }
}

//���������ز鿴ĳһ�ش����ϸ��Ϣ
void QuestionWindow::answer_res()
{
    timer->stop();
    AnswerWindow *ansWin = new AnswerWindow;
    ansWin->setAttribute(Qt::WA_DeleteOnClose,true);
    ansWin->init();
    ansWin->show();
}
