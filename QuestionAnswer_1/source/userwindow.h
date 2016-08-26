#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>
#include "user.h"
#include <QStandardItemModel>
#include <QModelIndex>
namespace Ui {
    class UserWindow;
}

class UserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserWindow(user*,QWidget *parent = 0);
    ~UserWindow();

    void set_questionlist_ask();
    void set_questionlist_reply();
private slots:
    void displayquestion_ask(QModelIndex);
    void displayquestion_reply(QModelIndex);
    void on_pushButton_update_1_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_focus_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UserWindow *ui;
    user *currentUser;
    QStandardItemModel *questionlist_ask;
    QStandardItemModel *questionlist_reply;
};

#endif // USERWINDOW_H
