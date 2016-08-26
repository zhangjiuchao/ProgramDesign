#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include<QTimer>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void writeLog(QString);
public slots:
    void displayRoomsState();
private slots:
    void on_summer_clicked();

    void on_winter_clicked();

    void on_airOpen_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* roomStateList;
    QTimer *timer;
    bool isOpen;
};

#endif // MAINWINDOW_H
