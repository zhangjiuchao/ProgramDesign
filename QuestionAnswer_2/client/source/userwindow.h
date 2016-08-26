#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>

#include <QStandardItemModel>
#include <QModelIndex>
#include <QTimer>
namespace Ui {
    class UserWindow;
}

class UserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = 0);
    ~UserWindow();

    void init();

public slots:
    void update_ask();

    void update_reply();

    void focus_res();
private slots:
    void displayquestion_ask(QModelIndex);

    void displayquestion_reply(QModelIndex);

    void on_pushButton_update_1_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_focus_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void time_out();
private:
    Ui::UserWindow *ui;
    QStandardItemModel *questionlist_ask;
    QStandardItemModel *questionlist_reply;
    QTimer *timer;

    QString id_user;
};

#endif // USERWINDOW_H
