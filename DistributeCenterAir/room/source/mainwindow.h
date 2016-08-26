#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QtNetwork/QUdpSocket>
#include <QHostInfo>
#include <QMessageBox>

extern QByteArray ByteArray;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QHostAddress *hostaddr1;
    QHostAddress *hostaddr2;


private:
    Ui::MainWindow *ui;

    QUdpSocket *udpSocket1;
    QUdpSocket *udpSocket2;

private slots:
    void receive();
    void on_exit_clicked();
    void on_AC_onoff_clicked();
    void on_changerequest_clicked();
    void on_instruction_clicked();
    void on_targettempup_clicked();
    void on_targettempdown_clicked();
};

#endif // MAINWINDOW_H
