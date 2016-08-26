#include <QtGui/QApplication>
#include "mainwindow.h"
#include "centeraircondition.h"
#include<QTextCodec>
#include<QMessageBox>
#include "CreatConnection.h"
void StartUP();
int nRoom=4;
int nThread=3;
int clientPort=5000;
int serverPort=5000;

QSqlDatabase db;   //数据库变量
CenterAir *centerAir=NULL;
Communication* communicate=NULL;
Schedule* schedule=NULL;
QMap<int,Room*> rooms;
MainWindow *mainwin;
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

    bool isOpen=CreatDatabase();
    if(isOpen)
    {
        StartUP();
        mainwin=new MainWindow;qDebug()<<"cccccccc";
        mainwin->show();

    }

    else
    {
        QMessageBox *message = new QMessageBox;
        message->setText("数据库打开失败");
        message->exec();
        return a.exec();
    }

    return a.exec();
}

void StartUP()
{
    communicate=new Communication();
    centerAir=new CenterAir();
    for(int i=1;i<=nRoom;++i)
    {
        Room* newRoom1=new Room(1,25);
        rooms.insert(i,newRoom1);
    }
}
