#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
   }
   QTextStream in(&inputFile);
    QString  s;
   while(!in.atEnd())
    {
       s= in.readAll();
//ui->Page1->findChild<QTextBrowser*>("textBrowser_Input")->setHorizontalScrollBarPolicy()
        ui->Page1->findChild<QTextBrowser*>("textBrowser_Input")->insertPlainText(s);

   }
 inputFile.close();



}
