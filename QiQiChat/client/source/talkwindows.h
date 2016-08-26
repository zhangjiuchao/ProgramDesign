#ifndef TALKWINDOWS_H
#define TALKWINDOWS_H

#include <QDialog>

namespace Ui {
    class TalkWindows;
}

class TalkWindows : public QDialog
{
    Q_OBJECT

public:
     TalkWindows( QString str , QString str2 , bool online  ,QWidget *parent = 0 );
    ~TalkWindows();

    void getMessage(QString message);      //��ȡ�Է����͵�������Ϣ

private slots:

    void on_SendButton_clicked();

private:
    Ui::TalkWindows *ui;

    QString peerIP;       //�Է� IP��ַ

    QString peerName;     //�Է��ǳ�

    bool isOnline;
};

#endif // TALKWINDOWS_H
