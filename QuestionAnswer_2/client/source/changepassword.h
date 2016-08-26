#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QWidget>
#include <QTimer.h>
namespace Ui {
    class ChangePassword;
}

class ChangePassword : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = 0);
    ~ChangePassword();
public slots:
    void changepassword_res();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void time_out();
private:
    Ui::ChangePassword *ui;
    QTimer *timer;
};

#endif // CHANGEPASSWORD_H
