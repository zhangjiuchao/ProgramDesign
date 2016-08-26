#include <QtGui/QApplication>
#include "log_in.h"
#include <QTextCodec>
#include "QAS_client.h"

QString IPofServer = "192.168.43.210";    //服务器IP

int PortofServer = 7777;               //服务器端口

TcpSocket *Socket;

Log_in *w;
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

    //Log_in w;
   // w.setAttribute(Qt::WA_DeleteOnClose,true);
    w=new Log_in;
    w->show();

    return a.exec();
}

bool is_exist_space(QString str)
{
    for(int i=0;i<str.size();++i)
    {
        if(str[i] == 0x0020||str[i] == 0x0A)
            return true;
    }

    return false;
}
