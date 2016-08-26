#include "running.h"
#include "centeraircondition.h"
#include "windows.h"
Running::Running()
{
    room=NULL;
    running=false;
}

void Running::setRoom(Room *r)
{
    room=r;
}

int Running::getRoom()
{
    return room->getRoomNo();
}

bool Running::isRunning()
{
    return running;
}

void Running::run()
{
    room->setServerTimeLen();
    running=true;
    while(room->isServing())
    {
        double startTemp=room->getRoomTemp();
        double destTemp=room->getDestTemp();
        int ws=room->getRoomWind();
        bool flag=startTemp>destTemp?true:false;
        if(ws==1){

            Sleep(3000*60);
            if(flag)
                startTemp--;
            else{
                startTemp++;
            }
            room->setRoomTemp(startTemp);
            room->addServerTimeLen(3);
            QSqlQuery query0(db);
            query0.exec("update Excel set timelength=timelength+3 where id="+QString::number(room->getRoomNo()));
            room->addCost(1);
            query0.exec("update Excel set totalmoney=totalmoney+1 where id="+QString::number(room->getRoomNo()));
        }
        else if(ws==2){
            Sleep(2000*60);
            if(flag)
                startTemp--;
            else
                startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(1);
            room->addServerTimeLen(2);
            QSqlQuery query0(db);
            query0.exec("update Excel set timelength=timelength+2 where id="+QString::number(room->getRoomNo()));
            query0.exec("update Excel set totalmoney=totalmoney+1 where id="+QString::number(room->getRoomNo()));
        }
        else{
            Sleep(1000*60);
            if(flag)
                startTemp--;
            else
                startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(1);
            room->addServerTimeLen(1);
            QSqlQuery query0(db);
            query0.exec("update Excel set timelength=timelength+1 where id="+QString::number(room->getRoomNo()));
            query0.exec("update Excel set totalmoney=totalmoney+1 where id="+QString::number(room->getRoomNo()));
        }
        if(flag && startTemp<=destTemp)
            room->setService(false);
        if(!flag && startTemp>=destTemp)
            room->setService(false);
    }
    running=false;
    room->setServerTimeLen();
    room->setService(false);
    schedule->rmRequest(room->getRoomNo());
    room->naturalTempChange();   
}
