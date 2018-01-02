#include "xmllibrary.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include "previewpage.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <memory>
#include <QtWidgets>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
// WebEngine
#include "utils.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QtWebEngine/qtwebengineglobal.h>
#include <QQuickItem>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    listViewModel = new QStringListModel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
//    if(browserApp)
//        delete browserApp;
    if (mainXml)
        delete mainXml;
    qDebug() << " Destroy MainWindow !!--" << endl;
}

void MainWindow::on_newSite_clicked()
{
    // neue seite öffnen um dann eine seite angeben die in der liste erscheint
    // rechtsklick -> edit

    mainXml = new XMlLibrary();
    QString path("schnick/schnack");
    QString name("schnickMan");

    mainXml->readWriteXML(&path, false, &name);
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
    ui->ItemInList->setModel(listViewModel);
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
QUrl Browser::startupUrl(QString* url)
{
    QUrl ret;
    QStringList args(qApp->arguments());
    args.takeFirst();
    Q_FOREACH (const QString& arg, args) {
        if (arg.startsWith(QLatin1Char('-')))
             continue;
        ret = Utils::fromUserInput(arg);
        if (ret.isValid())
            return ret;
    }
    return QUrl((url ?  *url : "http://qt.io/"));
}

QUrl MainWindow::startupUrl(QString* url)
{
    QUrl ret;
    QStringList args(qApp->arguments());
    args.takeFirst();
    Q_FOREACH (const QString& arg, args) {
        if (arg.startsWith(QLatin1Char('-')))
             continue;
        ret = Utils::fromUserInput(arg);
        if (ret.isValid())
            return ret;
    }
    return QUrl((url ?  *url : "http://qt.io/"));
}

bool MainWindow::filterXMLData(QString &name, QUrl& url)
{
    for(auto& x :  *mainXml->xmlVec)
    {
        if(name == x->name)
        {
            url = startupUrl(&x->url);
            return x->list;
        }
    }
    return false;
}

void MainWindow::on_ItemInList_clicked(const QModelIndex &index)
{
    QUrl url;
    QString string (index.data().toString());
    if(IsRuning)
    {
        filterXMLData(string, url);

        //        QMetaObject::invokeMethod(browserApp->rootObjects().
        //                                  first()->findChild<QObject*>("browserWindow"),"createTab",
        //                                  Q_ARG(QVariant, url);

        QMetaObject::invokeMethod(browserApp->rootObjects().
                                  first()->findChild<QObject*>("browserWindow"),"createTab",
                                  Q_ARG(QVariant, startupUrl()));
    }
    else
    {

        filterXMLData(string, url);


        browserApp->load(QUrl("qrc:/ApplicationRoot.qml"));
        QMetaObject::invokeMethod(browserApp->rootObjects().first(), "load",
                                  Q_ARG(QVariant, startupUrl()));

        //        QMetaObject::invokeMethod(browserApp->rootObjects().first(), "load",
        //                                  Q_ARG(QVariant, url);
        qDebug() << string << " " << url;
        IsRuning = true;
    }
}

void MainWindow::on_ItemInList_doubleClicked(const QModelIndex &index)
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

void Browser::createBrowser()
{
    QtWebEngine::initialize();
    browser = std::make_unique<QQmlApplicationEngine>();
    Utils utils;
    browser->rootContext()->setContextProperty("utils", &utils);
    browser->load(QUrl("qrc:/ApplicationRoot.qml"));
    QMetaObject::invokeMethod(browser->rootObjects().first(), "load", Q_ARG(QVariant, startupUrl()));
}
