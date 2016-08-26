#ifndef ADD_FRIEND_H
#define ADD_FRIEND_H

#include <QDialog>

namespace Ui {
    class Add_Friend;
}

class Add_Friend : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Friend( QWidget *parent = 0 );

    ~Add_Friend();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Add_Friend *ui;
};

#endif // ADD_FRIEND_H
