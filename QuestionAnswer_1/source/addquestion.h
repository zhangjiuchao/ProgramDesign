#ifndef ADDQUESTION_H
#define ADDQUESTION_H

#include <QDialog>

namespace Ui {
    class AddQuestion;
}

class AddQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit AddQuestion(QWidget *parent = 0);
    ~AddQuestion();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::AddQuestion *ui;
};

#endif // ADDQUESTION_H
