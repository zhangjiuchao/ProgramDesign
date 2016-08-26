#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "room.h"
#include "running.h"
#include<QVector>
#include<QObject>
class Schedule : public QObject
{
    Q_OBJECT
public:
    Schedule();
    void scheduleAlgorithm(Room *room=NULL);
    void rmRequest(int roomNO);
private:
    QVector<int> waitingRoom;
    QVector<int> runningRoom;
    QVector<Running*> runThreads;
};

#endif // SCHEDULE_H
