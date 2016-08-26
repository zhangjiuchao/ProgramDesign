#ifndef CENTERAIRSET_H
#define CENTERAIRSET_H

#include <QDialog>

namespace Ui {
    class CenterAirSet;
}

class CenterAirSet : public QDialog
{
    Q_OBJECT

public:
    explicit CenterAirSet(QWidget *parent = 0);
    ~CenterAirSet();

private:
    Ui::CenterAirSet *ui;
};

#endif // CENTERAIRSET_H
