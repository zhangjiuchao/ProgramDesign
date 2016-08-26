#include "summerrunning.h"
#include<QByteArray>
#include<windows.h>
#include "centeraircondition.h"
SummerRunning::SummerRunning()
{
}


void SummerRunning::run()
{
    running=true;
    double startTemp=room->getRoomTemp();
    double destTemp=room->getDestTemp();
    int ws=room->getRoomWind();
    bool flag=true;
    while(flag){
        switch(ws){
        case -1:
            if(startTemp>destTemp)
                startTemp--;
            else
                startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(0.5);
            Sleep(3000);
            break;
        case 0:
            if(startTemp>destTemp)
                startTemp--;
            else
                startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(1);
            Sleep(2000);
            break;
        case 1:
            if(startTemp>destTemp)
                startTemp--;
            else
                startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(1.5);
            Sleep(1000);
            break;
        }
        startTemp=room->getRoomTemp();
        destTemp=room->getDestTemp();
        ws=room->getRoomWind();
        if(startTemp==destTemp)
            flag=false;
    }
    running=false;
    room->setService(false);
    room->naturalTempChange();
    schedule->rmRequest(room->getRoomNo());
}
