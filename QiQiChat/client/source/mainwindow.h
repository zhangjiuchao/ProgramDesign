#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMap>
#include "talkwindows.h"
#include <QStandardItemModel>
#include "QTimer"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    TalkWindows* getTalkWindow(QString id);     //ͨ��id �õ����촰��

    void insertItem(QString ip,TalkWindows* peerWin);

    void eraseItem(QString ip);



    void changeName(QString);

    void setinfor(QString,QString);        //��ʾ�˺ź��ǳ�

    void init();     //�����ڳ�ʼ�������
public slots:
    void response_for_communication();       //��������ķ�����Ϣ

    void response_for_addfriend();          //������Ӻ��ѵķ�����Ϣ

    void friend_request();           //�����������û��ĺ�������

    void updateinfor();
private slots:

    void getMessage();          //��ȡ  UDP ��Ϣ

    void on_pushButton_3_clicked();

    void select_friend(QModelIndex);      //ѡ���������

    void Timer_out();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_addfriend_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString,TalkWindows*> id_windows;   //map �����д洢��������˺ź����촰�ڵľ��

    QStandardItemModel *friends_list;      //���ڽ�����ʾ�����б�

    QStandardItem *online_friends;       //���ڽ�����ʾ���ߺ����б�

    QStandardItem *off_line_friends;      //���ڽ�����ʾ���ߺ����б�

    QTimer *timer;
};

#endif // MAINWINDOW_H
