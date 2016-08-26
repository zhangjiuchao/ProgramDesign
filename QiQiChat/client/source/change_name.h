#ifndef CHANGE_NAME_H
#define CHANGE_NAME_H

#include <QDialog>
#include <QTimer>
namespace Ui {
    class change_name;
}

class change_name : public QDialog
{
    Q_OBJECT

public:
    explicit change_name( QWidget *parent = 0 );

    ~change_name();

signals:
    void changename( QString newname );

public slots:
    void response_for_change_name();

private slots:
    void on_pushButton_ok_clicked();

    void timer_out();

    void on_pushButton_cancell_clicked();

private:
    Ui::change_name *ui;

    QTimer *timer;
};

#endif // CHANGE_NAME_H
