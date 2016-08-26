#include <QtGui/QApplication>
#include "dialog.h"
#include <QTextCodec>
#include <chat_client.h>


QString host_account;

QString host_name;

TcpSocket *Socket;

QString IPofServer="10.206.36.67";       //服务器IP

int PortofServer=6666;

QUdpSocket *udpSocket;

int PortofClient=7777;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QFont font = a.font();
    a.addLibraryPath("./plugins");
    a.setFont(font);

    Socket = new TcpSocket;        //用于与服务器通信的socket

    Dialog w;
    w.setAttribute(Qt::WA_DeleteOnClose,true);
    w.show();

    return a.exec();
}
