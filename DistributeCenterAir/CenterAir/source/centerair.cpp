#include "centeraircondition.h"

CenterAir::CenterAir()
{
    flag=false;
}

void CenterAir::setmodel(int mod)
{
    model=mod;
}

int CenterAir::getModel()
{
    return model;
}

void CenterAir::setTemperature(double low, double high)
{
    Ltemperature=low;
    Htemperature=high;
}

bool CenterAir::isOpen()
{
    return flag;
}

void CenterAir::open()
{
    flag=true;
}

void CenterAir::close()
{
    flag=false;
}
