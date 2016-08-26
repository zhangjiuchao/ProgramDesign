#include "schedule.h"
#include "centeraircondition.h"
#include<windows.h>
#include <QDebug>
Schedule::Schedule()
{
    for(int i=0;i<nThread;++i){
        Running* newThread=new Running();
        runThreads.push_back(newThread);
    }
}

void Schedule::scheduleAlgorithm(Room* room)
{
    if(!centerAir->isOpen())
        return;
    if(room==NULL)
    {
        if(waitingRoom.size()>0){
            int pos=0;
            Room *rr=rooms.value(waitingRoom[0]);
            int len=waitingRoom.size();
            for(int i=1;i<len;++i){
                Room *temp=rooms.value(waitingRoom[i]);
                if( temp->getRoomWind()>rr->getRoomWind() ){
                    rr=temp;
                    pos=i;
                }
                else{
                    if(temp->getRoomWind()==rr->getRoomWind()
                            && temp->getRequestTime()<rr->getRequestTime()){
                        rr=temp;
                        pos=i;
                    }
                }
            }

            waitingRoom.remove(pos);
            runningRoom.push_back(rr->getRoomNo());
            rr->addScheduledTimes();
            QSqlQuery query0(db);
            query0.prepare("update Excel set scheduletimes=scheduletimes+1 where id="+QString::number(rr->getRoomNo()));
            query0.exec();
            for(int i=0;i<nThread;++i)
            {
                Running* curThread=runThreads[i];
                if(!curThread->isRunning()){
                    curThread->setRoom(rr);
                    curThread->start();

                    rr->setService(true);
                }
            }
        }
    }
    else{

        int nWaitingRoom=waitingRoom.size();
        for(int i=0;i<nWaitingRoom;++i){
            if(waitingRoom[i]==room->getRoomNo()){
                waitingRoom.remove(i);
                break;
            }
        }
        if(runningRoom.size()<nThread){

            room->addScheduledTimes();
            QSqlQuery query0(db);
            query0.prepare("update Excel set scheduletimes=scheduletimes+1 where id="+QString::number(room->getRoomNo()));
            query0.exec();
            for(int i=0;i<nThread;++i)
            {
                Running* curThread=runThreads[i];
                if(!curThread->isRunning()){
                    curThread->setRoom(room);
                    curThread->start();
                    room->setService(true);
                    runningRoom.push_back(room->getRoomNo());
                    break;
                }
            }
        }
        else
        {
            int pos=0;
            int roomNo=runningRoom[pos];
            for(int i=1;i<nThread;++i){
                Room* r1=rooms.value(roomNo);
                Room* r2=rooms.value(runningRoom[i]);

                if(r1->getRoomWind()<r2->getRoomWind())
                    pos=i;
                else{
                    if(r1->getRoomWind()==r2->getRoomWind() &&
                            r1->getServerTimeLen()<r2->getServerTimeLen())
                        pos=i;
                }
            }

            Room* r1=rooms.value(runningRoom[pos]);
            r1->setServerTimeLen();
            if(r1->getRoomWind()<room->getRoomWind()){
                room->addScheduledTimes();
                QSqlQuery query0(db);
                query0.prepare("update Excel set scheduletimes=scheduletimes+1 where id="+QString::number(room->getRoomNo()));
                query0.exec();
                runningRoom.remove(pos);
                waitingRoom.push_back(runningRoom[pos]);
                runningRoom.push_back(room->getRoomNo());
                for(int i=0;i<nThread;++i)
                {
                    Running* curThread=runThreads[i];
                    if(curThread->getRoom()==r1->getRoomNo()){
                        curThread->setRoom(room);
                        r1->setService(false);
                        r1->naturalTempChange();
                        room->setService(true);
                        break;
                    }
                }
            }
            else{
                waitingRoom.push_back(room->getRoomNo());
            }
        }
    }
}

void Schedule::rmRequest(int roomNO)
{
    runningRoom.remove(roomNO);
    scheduleAlgorithm();
}
