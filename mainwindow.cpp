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
#include "helperWindowTool.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    desc = QSysInfo::productType() == "winrt" || "osx" ? true : false;
    listViewModel = new QStringListModel(this);
    QStringList list;
    listViewModel->setStringList(fillOverviewList(list));
    ui->ItemInList->setModel(listViewModel);
    listIsChanged = mainXml.xmlVec.capacity();
}

MainWindow::~MainWindow()
{
    setOverviewListName(wasModified, listViewModel, mainXml);
    if(listIsChanged)
        mainXml.writeXML();
    delete ui;
    qDebug() << " Destroy MainWindow !!--" << endl;
}

void MainWindow::on_newSite_clicked()
{
    QFileInfo info(QFileDialog::getOpenFileName(this));
    QString item = info.baseName();
    QString path = info.absoluteFilePath();
    if(path.isEmpty())
        return;
    QStringList list = listViewModel->stringList();
    list << item;
    listViewModel->setStringList(list);
    xmlData temp;
    temp.name = item;
    temp.url.push_back(path);
    convertWebarchiveToHtml(temp, desc);
    mainXml.xmlVec.push_back(temp);
    listIsChanged = true;
}

void MainWindow::on_newList_clicked()
{
    qDebug() << "on_newList_clicked";
    QStringList list = listViewModel->stringList();
    list << "empty list-> add a side with double click" ;
    listViewModel->setStringList(list);
}

void MainWindow::on_deBrow_clicked()
{
    if(!deBrow)
        deBrow = true;
    else
        deBrow = false;
}

bool MainWindow::filterXMLData(QString &name, std::vector<QUrl>& url)
{
    for(auto& x :  mainXml.xmlVec)
    {
        if(name == x.name)
        {
            for(auto urlLoop : x.url)
            {
                QUrl ur;
                QFileInfo info(urlLoop);
                if(!desc && info.completeSuffix() == "webarchive" || deBrow)
                {
                    ur = startupUrl(&urlLoop);
                }
                else
                {
                    bool unix = QSysInfo::productType() == "winrt" ? false : true;
                    if(info.completeSuffix() == "webarchive")
                    {
                        if(unix)
                        {
                            urlLoop = info.canonicalPath()+"/NoWebArchive" + "/" + info.baseName() + ".html";
                        }
                        else
                        {
                            urlLoop = info.canonicalPath()+ "\\NoWebArchive" + "\\" + info.baseName() + ".html";
                        }
                    }
                    ur = startupUrl(&urlLoop);
                }
                ur.setScheme("file");
                url.push_back(ur);
            }
            return x.url.size() > 1 ? true : false;
        }
    }
    return false;
}

QStringList MainWindow::fillOverviewList(QStringList& list)
{
    mainXml.readXML();
    if (!mainXml.xmlVec.empty())
    {
        for(auto &x : mainXml.xmlVec)
        {
            list << x.name;
            convertWebarchiveToHtml(x, desc);
        }
    }
    return list;
}

void MainWindow::itemListClicked(const QModelIndex &index)
{
        QString titel = index.data().toString();
        bool update = setOverviewListName(wasModified, listViewModel, mainXml);
        if(!titel.startsWith("List") && update)
            titel = "List: "+ titel;
        std::vector<QUrl> urlVec;
        filterXMLData(titel, urlVec);
        for(auto& url : urlVec)
        {
            QFileInfo info(url.toString());
            if(IsRuning)
            {
                if(!desc && info.completeSuffix() == "webarchive" || deBrow)
                {
                    QDesktopServices::openUrl(url);
                }
                else
                {
                    QVariant value;
                    QMetaObject::invokeMethod(browserApp->rootObjects().
                                              first()->findChild<QObject*>("browserWindow"),"tabCount",
                                              Q_RETURN_ARG(QVariant, value));
                    if(value.toInt() == 0)
                    {
                        browserApp->load(QUrl("qrc:/ApplicationRoot.qml"));
                        QMetaObject::invokeMethod(browserApp->rootObjects().first(), "load",
                                                  Q_ARG(QVariant, url));
                    }
                    else
                        QMetaObject::invokeMethod(browserApp->rootObjects().
                                                  first()->findChild<QObject*>("browserWindow"),"createTab",
                                                  Q_ARG(QVariant, url));
                }
            }
            else
            {
                if((!desc && info.completeSuffix() == "webarchive") || deBrow)
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
            qDebug() <<" ----------------------------";
        }
}

void MainWindow::doubleClickFileBrowser(const QModelIndex &index)
{
    QFileInfo info = QFileDialog::getOpenFileName(this);
    QString path = info.absoluteFilePath();
    if(!path.isEmpty())
    {
        if(index.row() >= mainXml.xmlVec.size())
        {
            xmlData temp;
            temp.url.push_back(path);
            convertWebarchiveToHtml(temp, desc);
            mainXml.xmlVec.push_back(temp);
        }
        else
        {
            mainXml.xmlVec.at(index.row()).url.push_back(path);
        }
        wasModified.push_back(index);
        listIsChanged = true;
    }
}

void MainWindow::itemListDoubleClicked(const QModelIndex &index)
{
    doubleClick = false;
    QTimer::singleShot(5000, this, [&]{doubleClickFileBrowser(index);});
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

void MainWindow::on_ItemInList_clicked(const QModelIndex &index)
{
    QTimer::singleShot(200, this, [&]{clickHandler(index);});
}

void MainWindow::clickHandler(const QModelIndex& index)
{
    if(!doubleClick)
        itemListClicked(index);
    else
        itemListDoubleClicked(index);
}

void MainWindow::on_ItemInList_doubleClicked(const QModelIndex &index)
{
    doubleClick = true;
}
