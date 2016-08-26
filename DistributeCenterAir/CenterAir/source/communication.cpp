#include "communication.h"
#include "centeraircondition.h"
#include<windows.h>
#include<QDataStream>
#include<QIODevice>
#include<QSqlQuery>
#include<bill.h>
Communication::Communication()
{
    udpsocket=new QUdpSocket;
    udpsocket->bind(QHostAddress("10.206.36.75"),serverPort);

    connect(udpsocket,SIGNAL(readyRead()),
            this,SLOT(readData()));
}


void Communication::readData()
{
    while(udpsocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpsocket->pendingDatagramSize());
        QHostAddress sender;
        udpsocket->readDatagram(datagram.data(), datagram.size(),
                                &sender);
        QDataStream in(&datagram,QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_3);

        QString msgType;
        in>>msgType;

        if(msgType=="RoomOpen"){
            int rNo=0;
            for(;rNo<nRoom;++rNo){
                Room* r=rooms.value(rNo);
                if(r->RoomisAvailable())
                    break;
            }

            QByteArray data;
            QDataStream out(&data,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_3);
            out<<tr("RoomOpenRespond");

            if(rNo==nRoom){
            //    out<<0;
                udpsocket->writeDatagram(data,sender,clientPort);
                QString log="房间已满，拒绝一顾客入住.";
                mainwin->writeLog(log);
            }
            else{
                Room *room=rooms.value(rNo);
                room->setHostAddress(sender);
            //    out<<1;
                out<<room->getRoomNo();
                out<<room->getRoomTemp();
                out<<room->getRoomWind();
              //  out<<centerAir->isOpen();
                out<<centerAir->getModel();
                room->send(data);
                QString log=QString::number(rNo)+"号房入住";
                mainwin->writeLog(log);

                room->setAvailable(false);
            }
        }
        else if(msgType=="Open"){
            int rNo;
            in>>rNo;
            Room *rr=rooms.value(rNo,NULL);
            if(rr!=NULL){
                rr->setAirState(true);
                rr->addOpenCloseTimes();

                QSqlQuery query0(db);
                query0.prepare("update Excel set openclosetimes=openclosetimes+1 where id="+QString::number(rNo));
                query0.exec();

                QString log=QString::number(rNo)+"号房空调开启.";
                mainwin->writeLog(log);
            }
        }
        else if(msgType=="ChangeRequire"){

            int rNo;
            in>>rNo;
            Room *rr=rooms.value(rNo,NULL);
            if(rr!=NULL){
                QString log=QString::number(rNo)+"号房发起温控请求.";
                mainwin->writeLog(log);
                if(centerAir->isOpen() && rr->AirisAvailable()){
                    QTime time=QTime::currentTime();
                    rr->setRequestTime(time);
                    double destTemp;
                    int destWind;

                    in>>destTemp>>destWind;

                    if(rr->getDestTemp()!=destTemp){
                        rr->addTempScheduleTimes();

                        QSqlQuery query0(db);
                        query0.prepare("update Excel set tempscheduletimes=tempscheduletimes+1 where id="+QString::number(rNo));
                        query0.exec();
                    }

                    if(rr->getRoomWind()!=destWind)
                    {
                        rr->addWindScheduleTimes();
                        QSqlQuery query0(db);
                        query0.exec("update Excel set windscheduletimes=windscheduletimes+1 where id="+QString::number(rNo));
                    }
                    if(rr->isServing()){
                        rr->setDestTemp(destTemp);
                        rr->setWindSpeed(destWind);
                    }
                    else{
                        rr->setDestTemp(destTemp);
                        rr->setWindSpeed(destWind);
                        if(destTemp!=rr->getRoomTemp())
                            schedule->scheduleAlgorithm(rr);
                    }
                }

            }
        }
        else if(msgType=="Close"){
            int rNo;
            in>>rNo;
            Room *rr=rooms.value(rNo,NULL);
            if(rr!=NULL){
                rr->setAirState(false);
                rr->setService(false);
                rr->addOpenCloseTimes();
                QSqlQuery query0(db);
                query0.prepare("update Excel set openclosetimes=openclosetimes+1 where id="+QString::number(rNo));
                query0.exec();
                Sleep(1000);
                rr->naturalTempChange();
                QString log=QString::number(rNo)+"号房空调关闭.";
                mainwin->writeLog(log);
            }

        }
        else if(msgType=="RoomClose"){
            int rNo;
            in>>rNo;
            Room *rr=rooms.value(rNo,NULL);
            if(rr!=NULL){
                Bill *newBill=new Bill(rr);
                rr->recoverState();
                newBill->show();
                QString log=QString::number(rNo)+"号房退房.";
                mainwin->writeLog(log);

            }
        }
    }
}
