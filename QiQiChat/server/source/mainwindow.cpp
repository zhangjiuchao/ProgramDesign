#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "QNetworkInterface"
#include "chatserver.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpserver = new TcpServer(this);

    QHostAddress host("10.206.36.67");     //�����ķ��������е�IP��ַ

    if(!tcpserver->listen( host , 6666 ))      //������������6666�˿�
    {
        QMessageBox *message = new QMessageBox;
        message->setText("�����˿�ʧ��");
        message->show();
        close();
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    db.close();

    close();
}
