#ifndef ANSWERWINDOW_H
#define ANSWERWINDOW_H
#include<answerinfor.h>
#include <QDialog>
#include "questinfor.h"
namespace Ui {
    class AnswerWindow;
}

class AnswerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnswerWindow( AnswerInfor*,QuestInfor*, QWidget *parent = 0);
    ~AnswerWindow();

private slots:
    void on_pushButton_focus_clicked();

    void on_pushButton_praise_clicked();

private:
    Ui::AnswerWindow *ui;
    AnswerInfor* currentAnswer;
    QuestInfor *host_question;
};

#endif // ANSWERWINDOW_H
