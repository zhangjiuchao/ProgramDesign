#ifndef ROOM_H
#define ROOM_H
#include<QString>
#include<QTime>
#include<QHostAddress>
#include<QByteArray>
#include<QUdpSocket>
#include "QMutex"
class Room
{

public:
    Room(int rno,int rtemp);
    int getRoomNo();
    double getRoomTemp();
    double getDestTemp();
    double getTotalCost();
    int getRoomWind();

    void setHostAddress(QHostAddress addr);
    void setDestTemp(double);
    void setWindSpeed(int wind);
    void setRoomTemp(double);
    void addCost(double money);

    bool RoomisAvailable();
    bool AirisAvailable();
    void setAvailable(bool flag);
    void setAirState(bool flag);
    bool isServing();
    void setService(bool flag);
    void send(QByteArray);
    void naturalTempChange();
    QTime getRequestTime();
    void setRequestTime(QTime time);
    int getServerTimeLen();
    int getTotalTimeLen();
    int getOpenCloseTimes();
    void addOpenCloseTimes();
    void setServerTimeLen();
    void addServerTimeLen(int timelen);
    void addTempScheduleTimes();
    void addWindScheduleTimes();
    void addScheduledTimes();
    int getTempScheduleTimes();
    int getWindScheduleTimes();
    int getScheduledTimes();
    void recoverState();
private:
    int roomNO;
    double startTemp;
    double roomTemp;
    double destTemp;
    int windSpeed;
    double totalCost;
    int tempScheduleTimes;
    int windScheduleTimes;
    int ScheduledTimes;
    QTime lastRequestTime;
    int serverTimeLen;
    int totalTimeLen;
    QHostAddress roomAddress;
    bool roomState;
    bool airState;
    bool serving;
    int opencloseTimes;
    QUdpSocket *udpsocket;
    QMutex mutex;
};

#endif // ROOM_H
