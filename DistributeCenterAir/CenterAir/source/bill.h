#ifndef BILL_H
#define BILL_H

#include <QDialog>
#include<room.h>
namespace Ui {
    class Bill;
}

class Bill : public QDialog
{
    Q_OBJECT

public:
    explicit Bill(Room*,QWidget *parent = 0);
    ~Bill();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Bill *ui;
};

#endif // BILL_H
