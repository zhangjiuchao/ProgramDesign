#ifndef MYMUTEX_H
#define MYMUTEX_H
#include <QMutex>


//封装管理互斥锁，保证互斥锁能够正确释放

class myMutex
{
public:
    myMutex(QMutex*);

    ~myMutex();
private:
    QMutex* mutex;
};

#endif // MYMUTEX_H
