#include "room.h"
#include "centeraircondition.h"
#include <windows.h>
#include<cmath>
Room::Room(int rn,int rt)
{
    roomNO=rn;
    roomTemp=rt;
    startTemp=rt;
    destTemp=rt;
    windSpeed=2;
    totalCost=0;
    roomState=true;
    airState=false;
    serving=false;
    opencloseTimes=0;
    tempScheduleTimes=0;
    windScheduleTimes=0;
    ScheduledTimes=0;
    totalTimeLen=0;
    udpsocket=new QUdpSocket;
}

int Room::getRoomNo()
{
    return roomNO;
}

double Room::getRoomTemp()
{
    return roomTemp;
}

double Room::getDestTemp()
{
    return destTemp;
}

double Room::getTotalCost()
{
    return totalCost;
}

int Room::getRoomWind()
{
    return windSpeed;
}

void Room::setHostAddress(QHostAddress addr)
{
    roomAddress=addr;
}


void Room::setDestTemp(double temp)
{
    destTemp=temp;
}

void Room::setWindSpeed(int wind)
{
    windSpeed=wind;
}

void Room::setRoomTemp(double temp)
{
    roomTemp=temp;
}

void Room::addCost(double money)
{
    totalCost+=money;
}

bool Room::RoomisAvailable()
{
    return roomState;
}

bool Room::AirisAvailable()
{
    return airState;
}

void Room::setAvailable(bool flag)
{
    roomState=flag;
}

void Room::setAirState(bool flag)
{
    airState=flag;
}

void Room::send(QByteArray data)
{

    udpsocket->writeDatagram(data,roomAddress,clientPort);
}

void Room::naturalTempChange()
{
    int model=centerAir->getModel();
    bool flag=true;
    double st=roomTemp;
    if(model==0){
        while(flag && !serving && roomTemp<startTemp){
            Sleep(1000*60);
            roomTemp+=0.5;
            if(fabs(st-roomTemp)>=1.0){
                flag=false;
            }
        }
    }
    else{
        while(flag && !serving &&roomTemp>startTemp){
            Sleep(1000*60);
            roomTemp-=0.5;
            if(fabs(st-roomTemp)>=1.0){
                flag=false;
            }
        }
    }
    if(airState){
        qDebug()<<"abc";
        schedule->scheduleAlgorithm(this);
    }
}

bool Room::isServing()
{
    return serving;
}

void Room::setService(bool flag)
{
    serving=flag;
}

QTime Room::getRequestTime()
{
    return lastRequestTime;
}

void Room::setRequestTime(QTime time)
{
    lastRequestTime=time;
}


int Room::getServerTimeLen()
{
    return serverTimeLen;
}

void Room::addOpenCloseTimes()
{
    opencloseTimes++;
}

int Room::getOpenCloseTimes()
{
    return opencloseTimes;
}

void Room::setServerTimeLen()
{
    serverTimeLen=0;
}

void Room::addServerTimeLen(int timelen)
{
    serverTimeLen+=timelen;
    totalTimeLen+=timelen;
}

int Room::getTotalTimeLen()
{
    return totalTimeLen;
}

void Room::addTempScheduleTimes()
{
    tempScheduleTimes++;
}

void Room::addWindScheduleTimes()
{
    windScheduleTimes++;
}

void Room::addScheduledTimes()
{
    ScheduledTimes++;
}

int Room::getTempScheduleTimes()
{
    return tempScheduleTimes;
}

int Room::getWindScheduleTimes()
{
    return windScheduleTimes;
}

int Room::getScheduledTimes()
{
    return ScheduledTimes;
}

void Room::recoverState()
{
    roomTemp=25;
    startTemp=25;
    destTemp=25;
    windSpeed=2;
    totalCost=0;
    roomState=true;
    airState=false;
    serving=false;
    opencloseTimes=0;
    tempScheduleTimes=0;
    windScheduleTimes=0;
    ScheduledTimes=0;
    totalTimeLen=0;
}
