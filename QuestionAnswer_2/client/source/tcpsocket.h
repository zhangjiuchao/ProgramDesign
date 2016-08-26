#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include <QObject>


class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket( QObject *parent = 0 );

    void sendMessage();    //������Ϣ

    QTcpSocket *tcpsocket;      //�������ͨ�ŵ� socket

    QByteArray block;        //������Ϣ������


signals:

    void response_for_log();          //�������Ե�¼�Ļ�Ӧ

    void response_for_registe();       //��������ע��Ļ�Ӧ

    void response_for_changepassword();       //���������޸�����Ļ�Ӧ

    void response_for_myfocus();              //�������Բ鿴��ע�б�Ļ�Ӧ

    void response_for_myquestion();           //�������Բ鿴�ҵ������б�Ļ�Ӧ

    void response_for_ask();                  //�����������ʵĻ�Ӧ

    void response_for_search();              //�����������������Ļ�Ӧ

    void response_for_user();               //�������Բ鿴��ע�û���Ϣ�Ļ�Ӧ

    void response_for_select_question();      //�������Բ鿴����Ļ�Ӧ

    void response_for_select_answer();          //�������Բ鿴�ش�Ļ�Ӧ

    void response_for_updatequestions();         //ˢ����������

    void response_for_changepage_question();           //�����������������⻻ҳ�Ļ�Ӧ

    void response_for_changepage_myfocus();            //���������ڹ�ע�û���ҳ�Ļ�Ӧ

    void response_for_changepage_myquestion();         //�����������û����⻻ҳ�Ļ�Ӧ

    void response_for_update_askque();                  //��������ˢ���û������б�Ļ�Ӧ

    void response_for_update_replyque();                //��������ˢ���û��ش�����Ļ�Ӧ

    void response_for_focus();                            //�������Թ�ע�û��Ļ�Ӧ

    void response_for_answers();                         //������ˢ������Ļش��б�Ļ�Ӧ

    void response_for_addanswer();                       //�������Իش�����Ļ�Ӧ

    void response_for_praise();                       //�������Ե��޵Ļ�Ӧ
public slots:

    void readMessage();

    void newConnect();

};

#endif // TCPSOCKET_H
