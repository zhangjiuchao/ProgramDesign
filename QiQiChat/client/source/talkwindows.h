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

    void getMessage(QString message);      //获取对方发送的聊天消息

private slots:

    void on_SendButton_clicked();

private:
    Ui::TalkWindows *ui;

    QString peerIP;       //对方 IP地址

    QString peerName;     //对方昵称

    bool isOnline;
};

#endif // TALKWINDOWS_H
