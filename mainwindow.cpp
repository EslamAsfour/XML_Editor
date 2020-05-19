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


  // while(!in.atEnd())
    {
     //  s= in.readAll();
      // ui->Page1->findChild<QTextBrowser*>("textBrowser_Input")->insertPlainText(s);
   }
    int nMilliseconds = myTimer.elapsed();
    inputFile.close();
    qDebug() << nMilliseconds/1000;



}

void MainWindow::on_pushButton_clicked()
{

    QString InPathRdy = "C:/Users/LEGION/Documents/InRdy.txt" ;
     qDebug() << "Fiil Tree start";
    MainTree.FillTree(InPathRdy);
    qDebug() << "Fiil Tree end";
    QString OutPath = "C:/Users/LEGION/Documents/Output1.txt" ;
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
    //while(!in.atEnd())
     {
       // s= in.readAll();
       // ui->Page1->findChild<QTextBrowser*>("textBrowser_Output")->insertPlainText(s);
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








}
