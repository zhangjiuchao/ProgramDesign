#include <QtGui/QApplication>

#include "mainwindow.h"
#include "CreatConnection.h"
#include <QMessageBox>
#include "chatserver.h"

QMutex mutex;

QSqlDatabase db;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QFont font=a.font();
    font.setPointSize(16);

    a.addLibraryPath("./plugins");
    a.setFont(font);


    bool isOpen=CreatDatabase();

    if(isOpen)
    {
        MainWindow w;
        w.show();
        return a.exec();
    }


    else
    {
        QMessageBox *message = new QMessageBox;
        message->setText("数据库打开失败");
        message->exec();
        return a.exec();
    }

}

