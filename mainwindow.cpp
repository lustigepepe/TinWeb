#include "xmllibrary.h"
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
#include <QtWidgets>
#include <QFileInfo>
#include <QDir>

extern std::pair<int, char**> appInt;
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
    if (mainXml)
        delete mainXml;
}


void MainWindow::on_newSite_clicked()
{
    // neue seite öffnen um dann eine seite angeben die in der liste erscheint
    // rechtsklick -> edit

//    QString fileName = QFileDialog::getOpenFileName(this);

    //QString st (":/xml/xml/main.xml");
    mainXml = new XMlLibrary();
    QString path("schnick/schnack");
    QString name("schnickMan");

    mainXml->readWriteSite(&path, false, &name);
//    mainXml->creatWhat();
    QFileInfo info( QFileDialog::getOpenFileName(this));
    QString base = info.baseName();
    QStringList list;
    list << "Clair de Lune" << "Reverie" << "Prelude";
    if (mainXml && !mainXml->xmlVec->empty())
    {
        for(auto &x : *mainXml->xmlVec)
            list << x->name;
    }
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


void MainWindow::on_allSitesLists_clicked(const QModelIndex &index)
{

}


void MainWindow::on_allSitesLists_doubleClicked(const QModelIndex &index)
{



}



void MainWindow::dropEvent(QDropEvent *e)
{
//    if (event->button() == Qt::LeftButton)
//    {
//        isLeftClick = true;

//    }
//    else if (event->button() == Qt::RightButton)
//    {
//        isLeftClick = false;
//    }
    //super::mousePressEvent(event);
}


