#include "mymutex.h"



myMutex::myMutex(QMutex* Mutex)
    :mutex(Mutex)
{
    mutex->lock();
}



myMutex::~myMutex()
{
    mutex->unlock();
}
