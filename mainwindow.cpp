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
    listViewModel = new QStringListModel(this);
    //loadTextFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_newSite_clicked()
{
//    neue seite öffnen um dann eine seite angeben die in der liste erscheint
    // rechtsklick -> edit



    QStringList list;
    list << "Clair de Lune" << "Reverie" << "Prelude";

    // Populate our model
    listViewModel->setStringList(list);
    ui->allSitesLists->setModel(listViewModel);
//    // Glue model and view together
//    ui->listView->setModel(model);
//    ui->comboBox->setModel(model);
//    // Add additional feature so that
//    // we can manually modify the data in ListView
//    // It may be triggered by hitting any key or double-click etc.
//    ui->listView->
//            setEditTriggers(QAbstractItemView::AnyKeyPressed |
//                            QAbstractItemView::DoubleClicked);

}

void MainWindow::on_newList_clicked()
{

}

void MainWindow::on_overView_clicked()
{

}
