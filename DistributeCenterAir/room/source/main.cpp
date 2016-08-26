#include "mainwindow.h"
#include <QApplication>
#include "hotelroom.h"

QString signstr;
int airconditionmodel;
QString local_ip;
QString online_ip = "10.206.36.71";
QString readfile;
int roomfull;
int roomnum;
double currentroomtemp;
double targetroomtemp=25;
double currentcost=0;
int currentwindspeed;
int targetwindspeed;
bool centralcondition;
bool roomcondition = false;
int localport = 5000;
int centralport = 5000;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
