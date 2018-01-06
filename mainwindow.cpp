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
    desc = QSysInfo::productType() == "winrt" || "osx" ? true : false;
    mainXml = new XMlLibrary();
    listViewModel = new QStringListModel(this);
    QStringList list = fillOverviewList();
    if(!list.empty())
        listViewModel->setStringList(list);
    ui->ItemInList->setModel(listViewModel);

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

    QString pathT("schnick/schnack");
    QString name("schnickMan");

    mainXml->readWriteXML(&pathT, false, &name);
    QFileInfo info( QFileDialog::getOpenFileName(this));
    QString item = info.baseName();
    QString path = info.absoluteFilePath();
    qDebug() << "Item"<< item << " -|- " << path;
    mainXml->readWriteXML(&path, false, &item);

    QStringList list = fillOverviewList();
    list << "----------------------"
            "" << "Reverie" << "Prelude";
    listViewModel->setStringList(list);

}

void MainWindow::on_newList_clicked()
{

}

void MainWindow::on_overView_clicked()
{
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
    return QUrl((url ?  *url : "http://qt.io/"), QUrl::TolerantMode);

}

void convertWebarchiveToHtml(xmlData* data)
{
    QFileInfo info(data->url);
    bool unix = QSysInfo::productType() == "winrt" ? false : true;
    QString newFolder, newFile;
    if(unix)
    {
        newFolder = (info.canonicalPath()+"/NoWebArchive");
        newFile = newFolder + "/" + data->name + ".html";
    }
    else
    {
        newFolder = (info.canonicalPath()+"\\NoWebArchive");
        newFile = newFolder + "\\"+ data->name + ".html";
    }

    QProcess *proc = new QProcess();
    if(info.completeSuffix() == "webarchive" && ! QFile(newFile).exists())
    {

        if(!QDir(newFolder).exists())
            QDir().mkdir(newFolder);

        QString program;
        QStringList arguments;
        if(unix)
        {
            program = "/usr/bin/textutil";
            arguments << "-convert"  << "html" << data->url
                      << "-output" << newFile;
        }
        else
        {
            program = "C:\Windows\SysWOW64\textutil";
            arguments << "-convert"  << "html" << data->url
                      << "-output" << newFile;
        }
        proc->start(program, arguments);
        proc->closeWriteChannel();
    }
}

bool MainWindow::filterXMLData(QString &name, QUrl& url)
{
    for(auto& x :  *mainXml->xmlVec)
    {
        if(name == x->name)
        {
            if(!desc)
            {
                url = startupUrl(&x->url);
            }
            else
            {

                bool unix = QSysInfo::productType() == "winrt" ? false : true;
                QFileInfo info(x->url);
                if(unix)
                {
                    x->url = info.canonicalPath()+"/NoWebArchive" + "/" + name + ".html";
                }
                else
                {
                    x->url = info.canonicalPath()+"\\NoWebArchive" + "\\" + name + ".html";

                }
                url = startupUrl(&x->url);
                url.setScheme("file");
            }
            return x->list;
        }
    }
    return false;
}

QStringList MainWindow::fillOverviewList()
{
    mainXml->readWriteXML();
    QStringList list;
    if (mainXml && !mainXml->xmlVec->empty())
    {
        for(auto &x : *mainXml->xmlVec)
        {
            list << x->name;
            if(desc)
                convertWebarchiveToHtml(x);
        }
    }
    return list;
}

void MainWindow::on_ItemInList_clicked(const QModelIndex &index)
{
    QUrl url;
    QString titel (index.data().toString());
    filterXMLData(titel, url);
    if(IsRuning)
    {
        if(!desc)
        {
            QDesktopServices::openUrl(url);
        }
        else
        {
            QMetaObject::invokeMethod(browserApp->rootObjects().
                                      first()->findChild<QObject*>("browserWindow"),"createTab",
                                      Q_ARG(QVariant, url));
        }
        //        QMetaObject::invokeMethod(browserApp->rootObjects().
        //                                  first()->findChild<QObject*>("browserWindow"),"createTab",
        //                                  Q_ARG(QVariant, startupUrl()));
    }
    else
    {
        url.setScheme("file");
        if(!desc)
        {
            QDesktopServices::openUrl(url);
        }
        else
        {
            browserApp->load(QUrl("qrc:/ApplicationRoot.qml"));
            QMetaObject::invokeMethod(browserApp->rootObjects().first(), "load",
                                      Q_ARG(QVariant, url));
        }
        IsRuning = true;
    }
    qDebug() << titel << " " << url;
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
    //    QMetaObject::invokeMethod(browser->rootObjects().first(), "load", Q_ARG(QVariant, startupUrl()));
}
