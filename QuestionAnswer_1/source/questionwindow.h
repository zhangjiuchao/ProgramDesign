#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H
#include "questinfor.h"
#include <QDialog>
#include <QStandardItemModel>
namespace Ui {
    class QuestionWindow;
}

class QuestionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionWindow(QuestInfor* prob,QWidget *parent = 0);
    ~QuestionWindow();
    void displayAllAnswer();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void displayAnswer(QModelIndex);

    void on_pushButton_2_clicked();

private:
    Ui::QuestionWindow *ui;
    QuestInfor *question;
    QStandardItemModel *answer_model;
};

#endif // QUESTIONWINDOW_H
