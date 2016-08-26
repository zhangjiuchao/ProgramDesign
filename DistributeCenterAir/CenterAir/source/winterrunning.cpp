#include "winterrunning.h"
#include<windows.h>
#include "centeraircondition.h"
WinterRunning::WinterRunning()
{
}


void WinterRunning::run()
{
    running=true;
    double startTemp=room->getRoomTemp();
    double destTemp=room->getDestTemp();
    int ws=room->getRoomWind();
    while(startTemp<destTemp){
        switch(ws){
        case -1:
            startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(0.5);
            Sleep(3000);
            break;
        case 0:
            startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(1);
            Sleep(2000);
            break;
        case 1:
            startTemp++;
            room->setRoomTemp(startTemp);
            room->addCost(1.5);
            Sleep(1000);
            break;
        }
    }
    running=false;
    schedule->rmRequest(room->getRoomNo());
}
