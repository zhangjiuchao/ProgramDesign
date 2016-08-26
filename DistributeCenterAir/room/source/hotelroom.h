#ifndef HOTELROOM
#define HOTELROOM

#include <QString>
#include <qfile.h>

extern QString signstr;
extern int roomfull;
extern int roomnum;
extern double currentroomtemp;
extern double targetroomtemp;
extern double currentcost;
extern int currentwindspeed;
extern int targetwindspeed;
extern bool centralcondition;
extern int airconditionmodel;
extern bool roomcondition;
extern QString local_ip;
extern QString online_ip;
extern int localport;
extern int centralport;
extern QString readfile;
#endif // HOTELROOM

