#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QTimer>
namespace Ui {
    class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();


public slots:
    void registe_response();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void time_out();
private:
    Ui::Register *ui;
    QTimer *timer;
};

#endif // REGISTER_H
