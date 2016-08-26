#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QDialog>
#include "QTimer"
namespace Ui {
    class register_window;
}

class register_window : public QDialog
{
    Q_OBJECT

public:
    explicit register_window( QWidget *parent = 0 );
    ~register_window();

public slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void connect_timeout();

    void response_for_registe();         //注册账号返回的信息

private:
    Ui::register_window *ui;

    QTimer *timer;
};

#endif // REGISTER_WINDOW_H
