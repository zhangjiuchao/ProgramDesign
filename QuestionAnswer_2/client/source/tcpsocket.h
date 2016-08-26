#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include <QObject>


class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket( QObject *parent = 0 );

    void sendMessage();    //发送信息

    QTcpSocket *tcpsocket;      //与服务器通信的 socket

    QByteArray block;        //发送消息缓冲区


signals:

    void response_for_log();          //服务器对登录的回应

    void response_for_registe();       //服务器对注册的回应

    void response_for_changepassword();       //服务器对修改密码的回应

    void response_for_myfocus();              //服务器对查看关注列表的回应

    void response_for_myquestion();           //服务器对查看我的问题列表的回应

    void response_for_ask();                  //服务器对提问的回应

    void response_for_search();              //服务器对问题搜索的回应

    void response_for_user();               //服务器对查看关注用户信息的回应

    void response_for_select_question();      //服务器对查看问题的回应

    void response_for_select_answer();          //服务器对查看回答的回应

    void response_for_updatequestions();         //刷新所有问题

    void response_for_changepage_question();           //服务器对于所有问题换页的回应

    void response_for_changepage_myfocus();            //服务器对于关注用户换页的回应

    void response_for_changepage_myquestion();         //服务器对于用户问题换页的回应

    void response_for_update_askque();                  //服务器对刷新用户提问列表的回应

    void response_for_update_replyque();                //服务器对刷新用户回答问题的回应

    void response_for_focus();                            //服务器对关注用户的回应

    void response_for_answers();                         //服务器刷新问题的回答列表的回应

    void response_for_addanswer();                       //服务器对回答问题的回应

    void response_for_praise();                       //服务器对点赞的回应
public slots:

    void readMessage();

    void newConnect();

};

#endif // TCPSOCKET_H
