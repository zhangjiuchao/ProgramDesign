#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTimer>
namespace Ui {
    class QuestionWindow;
}

class QuestionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionWindow(QWidget *parent = 0);
    ~QuestionWindow();
    void init();
public slots:
    void update_answer();
    void add_answer_res();
    void focus_res();
    void answer_res();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void displayAnswer(QModelIndex);

    void on_pushButton_2_clicked();

    void time_out();
private:
    Ui::QuestionWindow *ui;
    QStandardItemModel *answer_model;
    QTimer *timer;
    QString id_user;
    QString id_question;
};

#endif // QUESTIONWINDOW_H
