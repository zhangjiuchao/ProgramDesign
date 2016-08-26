#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hotelroom.h"
#include "QIODevice"
#include "instructions.h"

QByteArray ByteArray;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始设置
    //取消关闭按钮
    setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint
                   | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    ui->room_num->setEnabled(false);
    ui->current_temp->setEnabled(false);
    ui->CCmodel->setEnabled(false);
    ui->CCstate->setEnabled(false);
    ui->current_cost->setEnabled(false);
    ui->target_temp->setEnabled(false);
    ui->radio_low->setEnabled(false);
    ui->radio_middle->setEnabled(false);
    ui->radio_high->setEnabled(false);
    ui->targettempup->setEnabled(false);
    ui->targettempdown->setEnabled(false);
    ui->changerequest->setEnabled(false);
    ui->current_cost->setText("0");
    //本机地址
    QString localHostName = QHostInfo::localHostName();

    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            local_ip = address.toString();
        }
    }

    //监听及发送
    udpSocket1 = new QUdpSocket(this);
    udpSocket2 = new QUdpSocket(this);
    hostaddr1 = new QHostAddress(local_ip);
    hostaddr2 = new QHostAddress(online_ip);
    bool conn=udpSocket1->bind(*hostaddr1,localport,QUdpSocket::ShareAddress);//监听本机上的6666端口，如果有消息到来，就会发送readyRead()信号。
    if(!conn)
    {
         QMessageBox box;
         box.setText(tr("wrong link"));
         box.exec();
    }
    else{
        //接受和发送
        connect(udpSocket1,SIGNAL(readyRead()),this,SLOT(receive()));
    }
    //发送开房请求
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out<<tr("RoomOpen");
    udpSocket2->writeDatagram(datagram,*hostaddr2,centralport);
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//接收信息并进行数据处理
void MainWindow::receive()
{
    while(udpSocket1->hasPendingDatagrams()){
        QByteArray data;
        data.resize(udpSocket1->pendingDatagramSize());
        udpSocket1->readDatagram(data.data(),data.size());

        QDataStream in(&data, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_3);
        in >> signstr;
        qDebug()<<signstr;
        if(signstr=="RoomOpenRespond"){
        /*    in>>roomfull;
            if(roomfull==0){
                QMessageBox box;
                box.setText(tr("man le !"));
                box.exec();
            }*/
      //      else{
                in>>roomnum>>currentroomtemp>>currentwindspeed;//>>centralcondition;
               // if(centralcondition){
                    in>>airconditionmodel;;
             //   }

                ui->room_num->setText(QString::number(roomnum));
                ui->current_temp->setText(QString::number(currentroomtemp));
                ui->target_temp->setText(QString::number(currentroomtemp));
              //  ui->CCmodel->setText(QString::number(airconditionmodel));
                if(!airconditionmodel)
                    ui->CCmodel->setText("Summer");
                else
                    ui->CCmodel->setText("Winter");
              /*  if(centralcondition==0)
                     ui->CCstate->setText("close");
                else
                    ui->CCstate->setText("open");*/

                if(currentwindspeed==1)
                    ui->radio_low->setChecked(true);
                else if(currentwindspeed==2)
                    ui->radio_middle->setChecked(true);
                else
                    ui->radio_high->setChecked(true);

                //this->show();
        //    }
        }
        else if(signstr=="OpenRespond"){
            roomcondition=true;
        }
        else if(signstr=="CurrentState"){
            in>>currentroomtemp>>currentcost;
            ui->current_temp->setText(QString::number(currentroomtemp));
            ui->current_cost->setText(QString::number(currentcost));
        }
        else if(signstr=="CloseRespond"){
            roomcondition=false;
        }
    }
}

//空调开/关机请求
void MainWindow::on_AC_onoff_clicked(){

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    if(roomcondition==true){
        out<<tr("Close")<<roomnum;
        ui->AC_onoff->setStyleSheet("QPushButton{border-image: url(:/onoff.jpg);}");
        roomcondition=false;
        ui->target_temp->setEnabled(false);
        ui->radio_low->setEnabled(false);
        ui->radio_middle->setEnabled(false);
        ui->radio_high->setEnabled(false);
        ui->targettempup->setEnabled(false);
        ui->targettempdown->setEnabled(false);
        ui->changerequest->setEnabled(false);

        ui->radio_low->setChecked(false);
        ui->radio_middle->setChecked(false);
        ui->radio_high->setChecked(false);
        ui->CCmodel->setText(" ");
        ui->CCstate->setText(" ");
        ui->target_temp->setText(" ");
    }
    else{
        out<<tr("Open")<<roomnum;
        ui->AC_onoff->setStyleSheet("QPushButton{border-image: url(:/onofff.jpg);}");
        roomcondition=true;
        ui->target_temp->setEnabled(true);
        ui->radio_low->setEnabled(true);
        ui->radio_middle->setEnabled(true);
        ui->radio_high->setEnabled(true);
        ui->targettempup->setEnabled(true);
        ui->targettempdown->setEnabled(true);
        ui->changerequest->setEnabled(true);
        //ui->CCmodel->setText(airconditionmodel);
        if(!airconditionmodel)
            ui->CCmodel->setText("Summer");
        else
            ui->CCmodel->setText("Winter");

        targetroomtemp=currentroomtemp;
        ui->target_temp->setText(QString::number(targetroomtemp));
        //if(centralcondition==0)
            // ui->CCstate->setText("close");
      //  else
           // ui->CCstate->setText("open");

    }
    udpSocket2->writeDatagram(datagram,*hostaddr2,centralport);
}

//发送调节请求
void MainWindow::on_changerequest_clicked(){

    if(ui->radio_low->isChecked())
        targetwindspeed = 1;
    if(ui->radio_middle->isChecked())
        targetwindspeed = 2;
    if(ui->radio_high->isChecked())
        targetwindspeed = 3;

     QByteArray datagram;
     QDataStream out(&datagram, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_4_3);
     out<<tr("ChangeRequire")<<roomnum<<targetroomtemp<<targetwindspeed;
     udpSocket2->writeDatagram(datagram,*hostaddr2,centralport);
}

//温度上调
void MainWindow::on_targettempup_clicked(){
    if(airconditionmodel == 0){
        if(targetroomtemp<=24){
            targetroomtemp++;
            ui->target_temp->setText(QString::number(targetroomtemp));
        }
    }
    else{
        if(targetroomtemp<=31){
            targetroomtemp++;
            ui->target_temp->setText(QString::number(targetroomtemp));
        }
    }


}


//温度下调
void MainWindow::on_targettempdown_clicked(){
    if(airconditionmodel == 0){
        if(targetroomtemp>=19){
            targetroomtemp--;
            ui->target_temp->setText(QString::number(targetroomtemp));
        }
    }
    else{
        if(targetroomtemp>=26){
            targetroomtemp--;
            ui->target_temp->setText(QString::number(targetroomtemp));
        }
    }
}

//说明书调取
void MainWindow::on_instruction_clicked(){

    instructions *instructionshow = new instructions;
    instructionshow->show();
}

//退房
void MainWindow::on_exit_clicked(){

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out<<tr("RoomClose")<<roomnum;
    udpSocket2->writeDatagram(datagram,*hostaddr2,centralport);
    this->close();
}
