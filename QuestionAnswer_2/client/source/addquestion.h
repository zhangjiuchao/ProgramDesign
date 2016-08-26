#ifndef ADDQUESTION_H
#define ADDQUESTION_H

#include <QDialog>
#include <QTimer>
namespace Ui {
    class AddQuestion;
}

class AddQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit AddQuestion(QWidget *parent = 0);
    ~AddQuestion();

public slots:
    void addquestion_res();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void time_out();
private:
    Ui::AddQuestion *ui;
    QTimer *timer;
};

#endif // ADDQUESTION_H
