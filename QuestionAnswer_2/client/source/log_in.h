#ifndef LOG_IN_H
#define LOG_IN_H

#include <QMainWindow>
#include <QTimer>
namespace Ui {
    class Log_in;
}

class Log_in : public QMainWindow
{
    Q_OBJECT

public:
    explicit Log_in(QWidget *parent = 0);
    ~Log_in();

public slots:
    void log_response();


private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void time_out();

private:
    Ui::Log_in *ui;

    QTimer* timer;
};

#endif // LOG_IN_H
