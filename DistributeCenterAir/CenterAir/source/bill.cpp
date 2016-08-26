#include "bill.h"
#include "ui_bill.h"

Bill::Bill(Room* rr,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bill)
{
    ui->setupUi(this);
    QString str="";
    str=QString::number(rr->getRoomNo())+" �ŷ����˷��������굥����:";
    ui->textBrowser->append(str);
    str="������ʱ��: "+QString::number(rr->getTotalTimeLen());
    ui->textBrowser->append(str);
    str="���ػ�����: "+QString::number(rr->getOpenCloseTimes());
    ui->textBrowser->append(str);
    str="�¶ȵ��ڴ���: "+QString::number(rr->getTempScheduleTimes());
    ui->textBrowser->append(str);
    str="���ٵ��ڴ���: "+QString::number(rr->getWindScheduleTimes());
    ui->textBrowser->append(str);
    str="������ȴ���: "+QString::number(rr->getScheduledTimes());
    ui->textBrowser->append(str);
    str="�����ܽ��Ϊ: "+QString::number(rr->getTotalCost());
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
