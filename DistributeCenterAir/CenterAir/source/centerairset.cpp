#include "centerairset.h"
#include "ui_centerairset.h"

CenterAirSet::CenterAirSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CenterAirSet)
{
    ui->setupUi(this);
}

CenterAirSet::~CenterAirSet()
{
    delete ui;
}
