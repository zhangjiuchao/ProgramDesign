#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QTimer>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog( QWidget *parent = 0 );
    ~Dialog();

public slots:
    void on_closeProgram_clicked();

    void on_logButton_clicked();

    void connect_timeout();     //���ӳ�ʱ

    void response_for_log();      //�������ķ��ص�¼��Ϣ


private slots:

    void on_registerButton_clicked();

private:
    Ui::Dialog *ui;

    QTimer *timer;
};

#endif // DIALOG_H
