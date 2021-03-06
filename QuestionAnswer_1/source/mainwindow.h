#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStandardItemModel>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setQuestionList();
    void setFocusList();
    void setMyQuestionList();


private slots:
    void on_pushButton_clicked();


    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void proxyChange(int);

    void on_pushButton_2_pressed();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void displayQuestion(QModelIndex);

    void display_search_question(QModelIndex);

    void displayMyQuestion(QModelIndex);

    void display_focus_user(QModelIndex);

    void on_list_down2_clicked();

    void on_list_up2_clicked();

    void on_list_down3_clicked();

    void on_list_up3_clicked();

    void on_pushButton_search_clicked();

    void change_search_way(int);

private:
    Ui::MainWindow *ui;
    int page_number_1;
    int currentPage_1;
    int page_number_2;
    int currentPage_2;
    int page_number_3;
    int currentPage_3;
    QStandardItemModel* question_list_model;
    QStandardItemModel* focus_list_model;
    QStandardItemModel* myquestion_list_model;
    QStandardItemModel* search_result;
    int display_way;
};

#endif // MAINWINDOW_H
