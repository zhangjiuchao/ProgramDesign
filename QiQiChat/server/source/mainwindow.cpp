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

    QHostAddress host("10.206.36.67");     //监听的服务器运行的IP地址

    if(!tcpserver->listen( host , 6666 ))      //监听服务器的6666端口
    {
        QMessageBox *message = new QMessageBox;
        message->setText("监听端口失败");
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
