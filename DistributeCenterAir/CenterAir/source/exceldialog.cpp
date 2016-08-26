#include "exceldialog.h"
#include "ui_exceldialog.h"
#include "centeraircondition.h"
#include<QDebug>
excelDialog::excelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::excelDialog)
{
    ui->setupUi(this);
    excelList=new QStandardItemModel;
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setModel(excelList);

    excelList->setHorizontalHeaderItem(0,new QStandardItem("房号"));
    excelList->setHorizontalHeaderItem(1,new QStandardItem("服务时长"));
    excelList->setHorizontalHeaderItem(2,new QStandardItem("总费用"));
    excelList->setHorizontalHeaderItem(3,new QStandardItem("空调开关次数"));
    excelList->setHorizontalHeaderItem(4,new QStandardItem("温度调节次数"));
    excelList->setHorizontalHeaderItem(5,new QStandardItem("风速调节次数"));
    excelList->setHorizontalHeaderItem(6,new QStandardItem("调度次数"));

    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setColumnWidth(2,80);
    ui->tableView->setColumnWidth(3,80);
    ui->tableView->setColumnWidth(4,80);
    ui->tableView->setColumnWidth(5,80);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();

    for(int i=0;i<nRoom;++i){
        QSqlQuery query0(db);
        query0.exec("select * from Excel where id="+QString::number(i));
        while(query0.next()){
            QString id=query0.value(0).toString();
            QString timeLen=query0.value(1).toString();
            QString totalMoney=query0.value(2).toString();
            QString opencloseTime=query0.value(3).toString();
            QString tempscheduleTimes=query0.value(4).toString();
            QString windscheduletimes=query0.value(5).toString();
            QString scheduleTimes=query0.value(6).toString();

            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id));
            newRow.push_back(new QStandardItem(timeLen));
            newRow.push_back(new QStandardItem(totalMoney));
            newRow.push_back(new QStandardItem(opencloseTime));
            newRow.push_back(new QStandardItem(tempscheduleTimes));
            newRow.push_back(new QStandardItem(windscheduletimes));
            newRow.push_back(new QStandardItem(scheduleTimes));

            excelList->appendRow(newRow);
        }
    }
}

excelDialog::~excelDialog()
{
    delete ui;
}

void excelDialog::on_pushButton_clicked()
{
    this->close();
}
