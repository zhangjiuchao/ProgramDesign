#ifndef RUNNING_H
#define RUNNING_H

#include<QPair>
#include<QThread>
#include<room.h>
class Running:public QThread
{
    Q_OBJECT
public:
    Running();
    void run();
    void setRoom(Room* r);
    bool isRunning();
    int getRoom();
    Room *room;
    bool running;
};

#endif // RUNNING_H
