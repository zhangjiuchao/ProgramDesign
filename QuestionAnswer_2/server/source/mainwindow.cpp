#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpserver.h"
#include <QMessageBox>
#include <QFile>
#include <QAS_server.h>
#include "mymutex.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpserver = new TcpServer(this);

    QHostAddress host("192.168.43.210");     //监听的服务器运行的IP地址

    if(!tcpserver->listen(host,7777))      //监听服务器的7777端口
    {
        QMessageBox *message = new QMessageBox;
        message->setText("监听端口失败");
        message->show();
        close();
    }


    online_user=new QStandardItemModel;

    ui->tableView_online->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_online->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_online->setModel(online_user);

    online_user->setHorizontalHeaderItem(0,new QStandardItem("用户ID"));
    online_user->setHorizontalHeaderItem(1,new QStandardItem("用户昵称"));
    online_user->setHorizontalHeaderItem(2,new QStandardItem("在线IP"));

    ui->tableView_online->setColumnWidth(0,150);
    ui->tableView_online->setColumnWidth(1,150);

    ui->tableView_online->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_online->verticalHeader()->hide();

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete online_user;
}

void MainWindow::on_pushButton_exit_clicked()
{
    {
        myMutex locker(&mutex);
        QString str="Data\\all_question.txt";
        QFile file1(str);

        try{
            if(!file1.open(QIODevice::WriteOnly))
                throw QString("问题文件打开失败");
            QTextStream out(&file1);
            QMap<QString,QuestInfor*>::const_iterator it=questMap.begin();
            for(;it!=questMap.end();++it)
            {
                out<<it.value();
            }

            file1.close();
        }

        catch(QString message)
        {
            QMessageBox *msg=new QMessageBox;
            msg->setText(message);
            msg->exec();
        }

        QString str1="Data\\IDPassword.txt";
        QFile file2(str1);
        try{

            if(!file2.open(QIODevice::WriteOnly))
                throw QString("文件打开失败");

            QTextStream out(&file2);
            QMap<QString,QString>::const_iterator it=ID_Password.begin(),iter_id_name;
            for(;it!=ID_Password.end();++it)
            {
                QString id=it.key();
                out<<it.key()<<"   ";
                out<<it.value()<<"  ";

                iter_id_name=ID_Name.find(id);
                out<<iter_id_name.value()<<"\r\n";
            }
            file2.close();
        }
        catch(QString message)
        {
            QMessageBox *msg=new QMessageBox;
            msg->setText(message);
            msg->exec();
        }


        this->close();
    }

}


int MainWindow::find_user(QString id)
{
    int nRow=online_user->rowCount();
    for(int i=0;i<nRow;++i)
    {
        QString current_id=online_user->item(i)->text();
        if(current_id == id)
            return i;
    }

    return -1;
}


void MainWindow::change_online_table(QString id,QString name,QString ip)
{
    int row=find_user(id);

    if(row == -1)
    {
        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));
        newRow.push_back(new QStandardItem(name));
        newRow.push_back(new QStandardItem(ip));

        online_user->appendRow(newRow);
    }
    else
        online_user->removeRow(row);
}
