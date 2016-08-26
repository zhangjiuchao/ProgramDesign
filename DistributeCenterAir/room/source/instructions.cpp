#include "instructions.h"
#include "ui_instructions.h"
#include <QDebug>
instructions::instructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::instructions)
{
    ui->setupUi(this);

    QFile instrution_data("G:\\myDoc\\SoftwareEngineer\\newroom\\room\\Hotelroom\\instructions.txt");
    if (instrution_data.open(QFile::ReadOnly)) {
        QTextStream in(&instrution_data);

        readfile= in.readLine();
        while(readfile!=NULL){
            ui->plainTextEdit->appendPlainText(readfile+"\n");
            readfile= in.readLine();
        }
        instrution_data.close();
    }

}

instructions::~instructions()
{
    delete ui;
}
