#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"
#include <QStandardItemModel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int find_user(QString id);        //找出用户id在窗口表中的位置

    void change_online_table(QString id,QString name,QString ip);
private slots:
    void on_pushButton_exit_clicked();

private:
    Ui::MainWindow *ui;
    TcpServer *tcpserver;

    QStandardItemModel *online_user;
};

#endif // MAINWINDOW_H
