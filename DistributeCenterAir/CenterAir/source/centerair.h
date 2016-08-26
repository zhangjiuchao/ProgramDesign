#ifndef CENTERAIR_H
#define CENTERAIR_H
#include<QString>
#include<QMap>
#include "room.h"

class CenterAir
{
public:
    CenterAir();
    void setmodel(int mod);
    bool isOpen();
    void open();
    void close();
    int getModel();
    void setTemperature(double low,double high);
private:
    bool flag;
    int model;
    double roomTemp;
    double Ltemperature;
    int Htemperature;
};

#endif // CENTERAIR_H
