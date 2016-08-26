#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QDialog>
#include "hotelroom.h"
#include "qtextstream.h"
namespace Ui {
class instructions;
}

class instructions : public QDialog
{
    Q_OBJECT

public:
    explicit instructions(QWidget *parent = 0);
    ~instructions();

private:
    Ui::instructions *ui;
};

#endif // INSTRUCTIONS_H
