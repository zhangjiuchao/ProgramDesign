#ifndef ANSWERWINDOW_H
#define ANSWERWINDOW_H

#include <QDialog>

#include <QTimer>
namespace Ui {
    class AnswerWindow;
}

class AnswerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnswerWindow(QWidget *parent = 0);
    ~AnswerWindow();
    void init();
public slots:
    void focus_res();

    void praise_res();
private slots:
    void on_pushButton_focus_clicked();

    void on_pushButton_praise_clicked();

    void time_out();
private:
    Ui::AnswerWindow *ui;
    QTimer *timer;
    QString id_user;
    QString id_answer;
    QString id_question;
};

#endif // ANSWERWINDOW_H
