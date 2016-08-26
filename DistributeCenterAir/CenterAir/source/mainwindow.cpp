#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "centeraircondition.h"
#include<QString>
#include "exceldialog.h"
#include<QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tempDomin->setEnabled(false);
    ui->tempDomin->setText("18--25");
    ui->summer->setChecked(true);
    isOpen=false;
    timer=new QTimer;
    timer->setInterval(1000);qDebug()<<"aaaaaaaaa";
    connect(timer,SIGNAL(timeout()),this,SLOT(displayRoomsState()));
    roomStateList=new QStandardItemModel;
    ui->roomsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->roomsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->roomsList->setModel(roomStateList);
    displayRoomsState();
    timer->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeLog(QString log)
{
    QString time=QTime::currentTime().toString("hh:mm:ss");
    ui->textBrowser->append(time);
    ui->textBrowser->append(log);
}

void MainWindow::displayRoomsState()
{
    roomStateList->clear();
    roomStateList->setHorizontalHeaderItem(0,new QStandardItem("����"));
    roomStateList->setHorizontalHeaderItem(1,new QStandardItem("����״̬"));
    roomStateList->setHorizontalHeaderItem(2,new QStandardItem("�յ�״̬"));
    roomStateList->setHorizontalHeaderItem(3,new QStandardItem("�����¶�"));
    roomStateList->setHorizontalHeaderItem(4,new QStandardItem("Ŀ���¶�"));
    roomStateList->setHorizontalHeaderItem(5,new QStandardItem("����"));
    roomStateList->setHorizontalHeaderItem(6,new QStandardItem("�ܷ���"));

    ui->roomsList->setColumnWidth(0,50);
    ui->roomsList->setColumnWidth(1,80);
    ui->roomsList->setColumnWidth(2,80);
    ui->roomsList->setColumnWidth(3,80);
    ui->roomsList->setColumnWidth(4,80);
    ui->roomsList->setColumnWidth(5,80);
    ui->roomsList->horizontalHeader()->setStretchLastSection(true);
    ui->roomsList->verticalHeader()->hide();

    for(int i=0;i<nRoom;++i)
    {
        Room* room=rooms.value(i);

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(QString::number(i)));

        QString roomstate;
        if(room->RoomisAvailable())
            roomstate="����";
        else
            roomstate="����ס";
        newRow.push_back(new QStandardItem(roomstate));

        QString airState;
        if(!room->AirisAvailable())
            airState="δ����";
        else if(room->isServing())
            airState="������";
        else
            airState="δ����";
        newRow.push_back(new QStandardItem(airState));

        double curTemp=room->getRoomTemp();
        newRow.push_back(new QStandardItem(QString::number(curTemp)));

        double destTemp=room->getDestTemp();
        newRow.push_back(new QStandardItem(QString::number(destTemp)));

        QString windSpeed;
        if(room->getRoomWind()==1)
            windSpeed="С����";
        else if(room->getRoomWind()==2)
            windSpeed="�з���";
        else
            windSpeed="�����";
        newRow.push_back(new QStandardItem(windSpeed));

        double totalCost=room->getTotalCost();
        newRow.push_back(new QStandardItem(QString::number(totalCost)));

        roomStateList->appendRow(newRow);
        if(!room->RoomisAvailable()){
            QByteArray data;
            QDataStream out(&data,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_3);
            out<<tr("CurrentState");
            out<<curTemp;
            out<<totalCost;
            room->send(data);
        }

    }
}

void MainWindow::on_summer_clicked()
{
    ui->tempDomin->setText("18--25");
}

void MainWindow::on_winter_clicked()
{
    ui->tempDomin->setText("25--32");
}

void MainWindow::on_airOpen_clicked()
{
    if(!isOpen){
        isOpen=true;
        ui->summer->setEnabled(false);
        ui->winter->setEnabled(false);
        ui->AirState->setText("������");
        ui->airOpen->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/onoff_����.jpg);}");
        centerAir->open();
        if(ui->summer->isChecked()){
            centerAir->setmodel(0);
            centerAir->setTemperature(15,25);
        }
        else{
            centerAir->setmodel(1);
            centerAir->setTemperature(25,32);
        }

        schedule=new Schedule;

        writeLog("����յ�����.");
    }
    else{
        isOpen=false;
        ui->airOpen->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/onoff.jpg);}");
        ui->summer->setEnabled(true);
        ui->winter->setEnabled(true);
        ui->AirState->setText("δ����");
        centerAir->close();
        for(int i=0;i<nRoom;++i){
            Room* rr=rooms.value(i);
            if(rr->isServing()){
                rr->setService(false);
                rr->naturalTempChange();
            }
        }
        writeLog("����յ��ر�.");
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    excelDialog *excel=new excelDialog();
    excel->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textBrowser->clear();
}
