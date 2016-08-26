#include "bill.h"
#include "ui_bill.h"

Bill::Bill(Room* rr,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bill)
{
    ui->setupUi(this);
    QString str="";
    str=QString::number(rr->getRoomNo())+" 号房间退房，消费详单如下:";
    ui->textBrowser->append(str);
    str="服务总时长: "+QString::number(rr->getTotalTimeLen());
    ui->textBrowser->append(str);
    str="开关机次数: "+QString::number(rr->getOpenCloseTimes());
    ui->textBrowser->append(str);
    str="温度调节次数: "+QString::number(rr->getTempScheduleTimes());
    ui->textBrowser->append(str);
    str="风速调节次数: "+QString::number(rr->getWindScheduleTimes());
    ui->textBrowser->append(str);
    str="请求调度次数: "+QString::number(rr->getScheduledTimes());
    ui->textBrowser->append(str);
    str="消费总金额为: "+QString::number(rr->getTotalCost());
    ui->textBrowser->append(str);
}

Bill::~Bill()
{
    delete ui;
}

void Bill::on_pushButton_clicked()
{
    this->close();
}
