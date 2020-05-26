#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "xml_tree.h"
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget_9->setCurrentIndex(0);
    QFile o1(QDir::currentPath() +"/InRdy.txt");
    QFile o2(QDir::currentPath() +"/XMLFormat.txt");
    QFile o3(QDir::currentPath() +"/Output1.txt");
    o1.open(QFile::WriteOnly|QFile::Truncate);
    o2.open(QFile::WriteOnly|QFile::Truncate);
    o3.open(QFile::WriteOnly|QFile::Truncate);
    QMessageBox::information(this,"Output Dir",QDir::currentPath()  );
    JSONDone = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
   File_Path =  QFileDialog::getOpenFileName(this,"Choose the file","C://");
   QFile inputFile(File_Path);
   if(! inputFile.open(QFile::ReadOnly | QFile::Text))
   {
       QMessageBox::warning(this,"Error","File Not Opened");
       return;
   }

   QTextStream in(&inputFile);
   QString  s;

    //Formatting
   qDebug() << "Start";
   MainTree.MakeItReady(File_Path);
   qDebug() << "End";
   //Show the input fill in the GUI
    QTime myTimer;
    myTimer.start();


   for(int i = 0 ; i < 100 ; i++)
    {
      s= in.readLine();
      ui->Page1->findChild<QTextBrowser*>("textBrowser_Input")->append(s);

   }
    int nMilliseconds = myTimer.elapsed();
    inputFile.close();
    qDebug() << nMilliseconds/1000;



}

void MainWindow::on_pushButton_clicked()
{

    QString InPathRdy = QDir::currentPath() +"/InRdy.txt" ;
     qDebug() << "Fiil Tree start";
    MainTree.FillTree(InPathRdy);
    qDebug() << "Fiil Tree end";
    QString OutPath = QDir::currentPath() +"/Output1.txt" ;
    QFile OutFile(OutPath);
    if(OutFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QString s;
        QTextStream out(&OutFile);
        formatting_xml(MainTree.GetHead(),s,0);
        out << s;
    }
    qDebug() << "Print Done";
    OutFile.flush();
    OutFile.close();
    QFile inputFile(OutPath);
    if(! inputFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Error","File Not Opened");
        return;
    }

    QTextStream in(&inputFile);
    QString  s;


    //Show the input fill in the GUI
    for(int i = 0 ; i < 100 ; i++)
     {
        s= in.readLine();
        ui->Page1->findChild<QTextBrowser*>("textBrowser_Output")->append(s);
    }

   inputFile.close();



}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget_9->setCurrentIndex(1);
}

void MainWindow::on_Submit_Word_clicked()
{
   QString word =  ui->Page2->findChild<QLineEdit*>("lineEdit_Word")->text();

   if(word == "")
   {
       QMessageBox::warning(this,"Empty Input","Please make sure you fill the input field");
       return;
   }
   //Call rasha's Functions
   QString Out,num;
   qDebug() << "word info start";
   Out += "Def of input word is :\n";
   MainTree.word_info(MainTree.GetHead(),word,Out,num);
   ui->Page2->findChild<QLabel*>("label_Num")->setText(num);
   Out += "Hypernym of input word is :\n";
   qDebug() << "word info end";
   MainTree.word_id(MainTree.GetHead(),word,Out);
   qDebug() << "word id end";

   qDebug() << Out;
   ui->Page2->findChild<QTextBrowser*>("textBrowser_OutPage2")->insertPlainText(Out);

}

void MainWindow::on_pushButton_JSON_Form_clicked()
{

     ui->Page1->findChild<QTextBrowser*>("textBrowser_Output")->setText("");
    if(JSONDone == false)
    {
        QMessageBox::information(this,"Warning", "This operation may take some time formatting big data");
        int lvl = 0;

        QString out;
       MainTree.XMLtoJSON(MainTree.GetHead(),lvl,out);
       QFile Out(QDir::currentPath() + "/XMLFormat.txt");

           if (!Out.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
           {
               return;
           }
           QTextStream outfile(&Out);
           outfile << out;
           JSONDone = true;

    }
       QFile JSON(QDir::currentPath() +"/XMLFormat.txt");
       if(! JSON.open(QFile::ReadOnly | QFile::Text))
       {
           QMessageBox::warning(this,"Error","File Not Opened");
           return;
       }
       QTextStream in(&JSON);
       QString  s;
       for (int var = 0; var < 100; ++var)
       {
           s= in.readLine();
           ui->Page1->findChild<QTextBrowser*>("textBrowser_Output")->append(s);
       }
}

void MainWindow::on_pushButton_PrettyForm_clicked()
{
    ui->Page1->findChild<QTextBrowser*>("textBrowser_Output")->setText("");
    QFile XML(QDir::currentPath() +"/Output1.txt");
    if(! XML.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Error","File Not Opened");
        return;
    }
    QTextStream in(&XML);
    QString  s;
    for (int var = 0; var < 100; ++var)
    {
        s= in.readLine();
        ui->Page1->findChild<QTextBrowser*>("textBrowser_Output")->append(s);
    }

}
