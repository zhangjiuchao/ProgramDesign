#ifndef LOG_IN_H
#define LOG_IN_H

#include <QMainWindow>

namespace Ui {
    class Log_in;
}

class Log_in : public QMainWindow
{
    Q_OBJECT

public:
    explicit Log_in(QWidget *parent = 0);
    ~Log_in();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Log_in *ui;
};

#endif // LOG_IN_H
