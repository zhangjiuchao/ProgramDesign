#ifndef MYMUTEX_H
#define MYMUTEX_H
#include <QMutex>


//��װ������������֤�������ܹ���ȷ�ͷ�

class myMutex
{
public:
    myMutex(QMutex*);

    ~myMutex();
private:
    QMutex* mutex;
};

#endif // MYMUTEX_H
