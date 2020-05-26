#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "xml_tree.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_Submit_Word_clicked();

    void on_pushButton_JSON_Form_clicked();

    void on_pushButton_PrettyForm_clicked();

private:
    Ui::MainWindow *ui;
    QString File_Path;
    XML_Tree MainTree;
    bool JSONDone;
};
#endif // MAINWINDOW_H
