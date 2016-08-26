#ifndef EXCELDIALOG_H
#define EXCELDIALOG_H

#include <QDialog>
#include<QStandardItemModel>
namespace Ui {
    class excelDialog;
}

class excelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit excelDialog(QWidget *parent = 0);
    ~excelDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::excelDialog *ui;
    QStandardItemModel* excelList;
};

#endif // EXCELDIALOG_H
