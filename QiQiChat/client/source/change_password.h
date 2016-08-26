#ifndef CHANGE_PASSWORD_H
#define CHANGE_PASSWORD_H

#include <QDialog>
#include <QTimer>
namespace Ui {
    class change_password;
}

class change_password : public QDialog
{
    Q_OBJECT

public:
    explicit change_password( QWidget *parent = 0 );

    ~change_password();

public slots:

    void change_response();

private slots:

    void on_pushButton_ok_clicked();

    void timerOut();

    void on_pushButton_cancell_clicked();

private:
    Ui::change_password *ui;

    QTimer *timer;
};

#endif // CHANGE_PASSWORD_H
