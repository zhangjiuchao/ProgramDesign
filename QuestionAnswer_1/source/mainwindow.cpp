#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log_in.h"
#include "addquestion.h"
#include <QString>
#include <QListWidgetItem>
#include <QMessageBox>
#include "Question_Answer_System.h"
#include "changepassword.h"
#include <QFile>
#include <QTextStream>
#include "questionwindow.h"
#include "userwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    page_number_1(1),
    currentPage_1(1),
    page_number_2(1),
    currentPage_2(1),
    page_number_3(1),
    currentPage_3(1)
{
    display_way=0; //问题显示方式初始化为0，即按时间顺序显示

    ui->setupUi(this);

    ui->pushButton_8->setFlat(true);
    ui->pushButton_9->setFlat(true);
    ui->list_down2->setFlat(true);
    ui->list_down3->setFlat(true);
    ui->list_up2->setFlat(true);
    ui->list_up3->setFlat(true);

    ui->lineEdit_keyword->setPlaceholderText("请输入时间，如：20151220, 代表问题发布的年月日");
    ui->label_hostname->setText(HostUser->getName());
    ui->comboBox_sort->addItem("按时间排序");
    ui->comboBox_sort->addItem("按热度值排序");
    ui->comboBox_search_way->addItem("按时间搜索");
    ui->comboBox_search_way->addItem("按关键字搜索");
    question_list_model=new QStandardItemModel();
    focus_list_model=new QStandardItemModel();
    myquestion_list_model=new QStandardItemModel();

    ui->QuestionList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->QuestionList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->QuestionList->setModel(question_list_model);


    ui->focuslistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->focuslistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->focuslistView->setModel(focus_list_model);

    ui->myQuestListview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->myQuestListview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->myQuestListview->setModel(myquestion_list_model);

    search_result=new QStandardItemModel;
    ui->tableView_result->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_result->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_result->setModel(search_result);

    connect(ui->tableView_result,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(display_search_question(QModelIndex)));

    connect(ui->myQuestListview,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayMyQuestion(QModelIndex)));

    connect(ui->QuestionList,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(displayQuestion(QModelIndex)));

    connect(ui->focuslistView,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(display_focus_user(QModelIndex)));

    connect(ui->comboBox_sort,SIGNAL(currentIndexChanged(int)),
            this,SLOT(proxyChange(int)));
    connect(ui->comboBox_search_way,SIGNAL(currentIndexChanged(int)),
            this, SLOT(change_search_way(int)));

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_search_way(int index)
{
    if(index==0)
        ui->lineEdit_keyword->setPlaceholderText("请输入时间，如：20151220, 代表问题发布的年月日");
    else
        ui->lineEdit_keyword->setPlaceholderText("请输入关键字，如：C++  Map");
}


void MainWindow::on_pushButton_3_clicked()
{
    ChangePassword *newWindow=new ChangePassword();
    newWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    QString str="Data\\"+HostUser->getID()+".txt";
    QFile file1(str);

    try{
        if(!file1.open(QIODevice::WriteOnly))
            throw QString("用户信息文件丢失");
        QTextStream out(&file1);

        out<<HostUser;

        file1.close();
    }
    catch(QString message)
    {
        QMessageBox *msg=new QMessageBox;
        msg->setText(message);
        msg->show();
    }

    str="Data\\all_question.txt";
    file1.setFileName(str);

    try{
        if(!file1.open(QIODevice::WriteOnly))
            throw QString("问题文件打开失败");
        QTextStream out(&file1);
        QMap<QString,QuestInfor*>::const_iterator it=questMap.begin();
        for(;it!=questMap.end();++it)
        {
            out<<it.value();
        }

        file1.close();
    }

    catch(QString message)
    {
        QMessageBox *msg=new QMessageBox;
        msg->setText(message);
        msg->exec();
    }

    Log_in *logWin=new Log_in;
    delete HostUser;
    HostUser=NULL;
    logWin->show();
    this->close();

}


void MainWindow::on_pushButton_7_clicked()
{
    AddQuestion *questionWin=new AddQuestion(this);
    questionWin->exec();
}


void MainWindow::on_pushButton_8_clicked()
{
   if(currentPage_1<page_number_1)
   {
        currentPage_1++;
        setQuestionList();
   }

}


void MainWindow::on_pushButton_9_clicked()
{
    if(currentPage_1>1)
    {
        currentPage_1--;
        setQuestionList();
    }
}


void MainWindow::setQuestionList()
{
    question_list_model->clear();
    question_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    question_list_model->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
    question_list_model->setHorizontalHeaderItem(2,new QStandardItem("标题"));
    question_list_model->setHorizontalHeaderItem(3,new QStandardItem("热度值"));
    ui->QuestionList->setColumnWidth(0,120);
    ui->QuestionList->setColumnWidth(2,300);
    ui->QuestionList->setColumnWidth(3,50);
    if(display_way==0)
    {
        page_number_1=questMap.size()/11+1;

        QMap<QString,QuestInfor*>::const_iterator it=questMap.end()-1-(currentPage_1-1)*11;
        int i=0;
        while(it!=questMap.begin()-1&&i<11)
        {
                QuestInfor* question=it.value();
                QString id=question->getID();
                QString id_of_creator=question->getIDIDofCreator();
                QString name_of_creator=ID_Name.find(id_of_creator).value();

                QString title=question->gettitle();

                QList<QStandardItem*> newRow;
                newRow.push_back(new QStandardItem(id));
                newRow.push_back(new QStandardItem(name_of_creator));
                newRow.push_back(new QStandardItem(title));
                newRow.push_back(new QStandardItem(QString::number(question->get_popular_value())));
                question_list_model->appendRow(newRow);

                it--;
                ++i;
        }
    }

    else
    {
        int vecSize=popular_value_vec.size();
        page_number_1=vecSize/11+1;

        int pos=(currentPage_1-1)*11;

        int i=0;

        while(pos<vecSize&&i<11)
        {
            QuestInfor* question=popular_value_vec.value(pos);
            QString id=question->getID();
            QString id_of_creator=question->getIDIDofCreator();
            QString name_of_creator=ID_Name.find(id_of_creator).value();
            QString title=question->gettitle();

            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id));
            newRow.push_back(new QStandardItem(name_of_creator));
            newRow.push_back(new QStandardItem(title));
            newRow.push_back(new QStandardItem(QString::number(question->get_popular_value())));
            question_list_model->appendRow(newRow);

            pos++;
            ++i;
        }
    }

    QString display1="",display2="";
    QString display=display1.setNum(currentPage_1)+"/"+display2.setNum(page_number_1);

    ui->label->setText(display);

}

void MainWindow::proxyChange(int index)
{
    if(index==0)
    {
        currentPage_1=1;
        display_way=0;
        setQuestionList();
    }
    else{
        qSort(popular_value_vec.begin(),popular_value_vec.end(),compare);
        currentPage_1=1;
        display_way=1;
        setQuestionList();
    }
}

void MainWindow::on_pushButton_2_pressed()
{
    currentPage_1=1;

    qSort(popular_value_vec.begin(),popular_value_vec.end(),compare);

    setQuestionList();
}

void MainWindow::setFocusList()
{
    focus_list_model->clear();
    focus_list_model->setHorizontalHeaderItem(0,new QStandardItem("ID"));
    focus_list_model->setHorizontalHeaderItem(1,new QStandardItem("昵称"));
    ui->focuslistView->setColumnWidth(0,150);
    ui->focuslistView->setColumnWidth(1,150);
    QList<QString> focus=HostUser->getFocusList();

    page_number_3=focus.size()/11+1;

    int it=(currentPage_3-1)*11;

    int i=0;
    while(it<focus.size()&&i<11)
    {
        QString id=focus.value(it);
        QString name=ID_Name.find(id).value();

        QList<QStandardItem*> newRow;
        newRow.push_back(new QStandardItem(id));
        newRow.push_back(new QStandardItem(name));

        focus_list_model->appendRow(newRow);

        it++;
        i++;
    }

    QString display1="",display2="";
    QString display=display1.setNum(currentPage_3)+"/"+display2.setNum(page_number_3);

    ui->label_3->setText(display);

}

void MainWindow::setMyQuestionList()
{
    myquestion_list_model ->clear();
    myquestion_list_model->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
    myquestion_list_model->setHorizontalHeaderItem(1,new QStandardItem("标题"));
    myquestion_list_model->setHorizontalHeaderItem(2,new QStandardItem("热度值"));
    ui->myQuestListview->setColumnWidth(0,120);
    ui->myQuestListview->setColumnWidth(1,270);
    ui->myQuestListview->setColumnWidth(2,50);

    QList<QString> myquest = HostUser->getMyQuestion();

    page_number_2 = myquest.size()/11+1;

    int i = 0;
    int it = myquest.size()-1-(currentPage_2-1)*11;

    while(it>=0&&i<11)
    {

        QString id=myquest.value(it);
        QMap<QString,QuestInfor*>::const_iterator iter=questMap.find(id);
        if(iter!=questMap.end())
        {
            QuestInfor *currentQuestion=iter.value();

            QString title="";
            if(currentQuestion!=NULL)
            {
                title = currentQuestion->gettitle();
            }


            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id));
            newRow.push_back(new QStandardItem(title));
            newRow.push_back(new QStandardItem(QString::number(currentQuestion->get_popular_value())));
            myquestion_list_model->appendRow(newRow);



            QString display1="",display2="";
            QString display=display1.setNum(currentPage_2)+"/"+display2.setNum(page_number_2);

            ui->label_2->setText(display);
        }
        it--;
        i++;
    }

}






void MainWindow::on_pushButton_4_clicked()
{
    currentPage_2=1;
    setMyQuestionList();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_5_clicked()
{
    currentPage_3=1;
    setFocusList();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::displayQuestion(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=question_list_model->item(Row)->text();

    QuestInfor* currentQuestion=questMap.find(id_of_question).value();

    QuestionWindow *Win=new QuestionWindow(currentQuestion,this);
    Win->show();

}

void MainWindow::displayMyQuestion(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=myquestion_list_model->item(Row)->text();

    QuestInfor* currentQuestion=questMap.find(id_of_question).value();

    QuestionWindow *Win=new QuestionWindow(currentQuestion,this);
    Win->show();

}

void MainWindow::on_list_down2_clicked()
{
    if(currentPage_2<page_number_2)
    {
        currentPage_2++;
        setMyQuestionList();
    }
}

void MainWindow::on_list_up2_clicked()
{
    if(currentPage_2>1)
    {
        currentPage_2--;
        setMyQuestionList();
    }
}

void MainWindow::on_list_down3_clicked()
{
    if(currentPage_3<page_number_3)
    {
        currentPage_3++;
        setFocusList();
    }
}

void MainWindow::on_list_up3_clicked()
{
    if(currentPage_3>1)
    {
        currentPage_3--;
        setFocusList();
    }
}

void MainWindow::display_focus_user(QModelIndex index)
{
    int Row=index.row();
    QString id_of_focus=focus_list_model->item(Row)->text();

    user* currentUser=new user;
    LoadUser(id_of_focus,currentUser);

    UserWindow* win=new UserWindow(currentUser);

    win->show();

}

void MainWindow::display_search_question(QModelIndex index)
{
    int Row=index.row();
    QString id_of_question=search_result->item(Row)->text();

    QuestInfor* currentQuestion=questMap.find(id_of_question).value();

    QuestionWindow *Win=new QuestionWindow(currentQuestion,this);
    Win->show();
}

void MainWindow::on_pushButton_search_clicked()
{
    QString content_search=ui->lineEdit_keyword->text();
    ui->lineEdit_keyword->clear();
    if(content_search=="")
    {
        QMessageBox *msg=new QMessageBox;
        msg->setText("搜索内容不能为空");
        msg->show();
    }
    else
    {
        QVector<QuestInfor*> result;
        if(ui->comboBox_search_way->currentIndex()==0)
        {
            result=find_question(content_search);
        }
        else
        {
            content_search=content_search.simplified();
            QStringList strlist=content_search.split(" ");
            result=find_question(strlist);
        }

        search_result->clear();
        search_result->setHorizontalHeaderItem(0,new QStandardItem("问题ID"));
        search_result->setHorizontalHeaderItem(1,new QStandardItem("创建者昵称"));
        search_result->setHorizontalHeaderItem(2,new QStandardItem("标题"));
        search_result->setHorizontalHeaderItem(3,new QStandardItem("热度值"));
        ui->tableView_result->setColumnWidth(0,120);
        ui->tableView_result->setColumnWidth(1,120);
        ui->tableView_result->setColumnWidth(2,200);
        ui->tableView_result->setColumnWidth(3,50);

        qSort(result.begin(),result.end(),compare);

        int vecSize=result.size();

        for(int i=0;i<vecSize;++i)
        {
            QuestInfor* question=result.value(i);
            QString id=question->getID();
            QString id_of_creator=question->getIDIDofCreator();
            QString name_of_creator=ID_Name.find(id_of_creator).value();
            QString title=question->gettitle();

            QList<QStandardItem*> newRow;
            newRow.push_back(new QStandardItem(id));
            newRow.push_back(new QStandardItem(name_of_creator));
            newRow.push_back(new QStandardItem(title));
            newRow.push_back(new QStandardItem(QString::number(question->get_popular_value())));
            search_result->appendRow(newRow);
        }

    }
}
