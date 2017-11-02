#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include "previewpage.h"
//#include <QWebEngineView>
#include <QQmlApplicationEngine>
//#include <qtwebengineglobal.h>
#include <QWebChannel>
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //loadTextFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_newSite_clicked()
{

}

void MainWindow::on_newList_clicked()
{

}

void MainWindow::on_overView_clicked()
{

}
