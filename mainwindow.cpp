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
//#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    desc = QSysInfo::productType() == "winrt" || "osx" ? true : false;
    mainXml = new XMlLibrary();
    listViewModel = new QStringListModel(this);
    QStringList list;
    listViewModel->setStringList(fillOverviewList(list));
    ui->ItemInList->setModel(listViewModel);
}

MainWindow::~MainWindow()
{
    updateName(wasModified, listViewModel, mainXml);
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

//    QFileInfo info( QFileDialog::getOpenFileName(this));
//    QString item = info.baseName();
//    QString path = info.absoluteFilePath();
//    qDebug() << "Item"<< item << " -|- " << path;
//    mainXml->readXML(&path, &item);
//    QStringList list = listViewModel->stringList();
//    listViewModel->setStringList(fillOverviewList(list));
}

void MainWindow::on_newList_clicked()
{

//        QFileInfo info( QFileDialog::getOpenFileName(this));
//        QString item = info.baseName();
//        QString path = info.absoluteFilePath();
//        qDebug() << "Item"<< item << " -|- " << path;
//        mainXml->readXML(&path, &item);
//        QStringList list = listViewModel->stringList();
//        list << "List: "+ item;
//        listViewModel->setStringList(fillOverviewList(list));
//        xmlData* temp = new xmlData();
//        if(!path.isNull())
//            temp->url = path;
//        if(!item.isNull())
//            temp->name = item;
//        mainXml->xmlVec->push_back(temp);
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

void convertWebarchiveToHtml(xmlData* data, bool desc)
{
    for (auto& url : data->url)
    {
        QFileInfo info(url);
        if(info.completeSuffix() == "webarchive" && desc)
        {
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
            if(!QFile(newFile).exists())
            {

                if(!QDir(newFolder).exists())
                    QDir().mkdir(newFolder);

                QString program;
                QStringList arguments;
                if(unix)
                {
                    program = "/usr/bin/textutil";
                    arguments << "-convert"  << "html" << url
                              << "-output" << newFile;
                }
                else
                {
                    program = "C:\Windows\SysWOW64\textutil";
                    arguments << "-convert"  << "html" << url
                              << "-output" << newFile;
                }

                proc->start(program, arguments);
                proc->closeWriteChannel();
            }
        }
    }
}

bool MainWindow::filterXMLData(QString &name, std::vector<QUrl>& url)
{
    for(auto& x :  *mainXml->xmlVec)
    {
        if(name == x->name)
        {
            for(auto& urlLoop : x->url)
            {
                QUrl ur;
                QFileInfo info(urlLoop);
                if(!desc || !(info.completeSuffix() == "webarchive"))
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
                            urlLoop = info.canonicalPath()+"/NoWebArchive" + "/" + name + ".html";
                        }
                        else
                        {
                            urlLoop = info.canonicalPath()+ "\\NoWebArchive" + "\\" + name + ".html";
                        }
                    }
                    ur = startupUrl(&urlLoop);
                }
                ur.setScheme("file");
                url.push_back(ur);
            }
            return x->url.size() > 1 ? true : false;
        }
    }
    return false;
}

QStringList MainWindow::fillOverviewList(QStringList& list)
{
    mainXml->readXML();
    if (mainXml && !mainXml->xmlVec->empty())
    {
        for(auto &x : *mainXml->xmlVec)
        {
            list << x->name;
//            for(auto& xUr : x->url)
//            {
                //                QFileInfo info(xUr);
                //                if(info.completeSuffix() == "webarchive" && desc)
                convertWebarchiveToHtml(x, desc);
//            }
        }
    }
    return list;
}

void MainWindow::itemListClicked(const QModelIndex &index)
{
//    QTimer *timer = new QTimer(this);
//       connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//       timer->start(1000);
//  QTimer::singleShot(2000, this, SIGNAL(test()));

//    QTimer::singleShot(200, this, SLOT(on_ItemInList_clicked()));
    if(doubleClick)
    {
        qDebug() << "notDoubleClick::Fail";
        QString titel = index.data().toString();
        bool update = updateName(wasModified, listViewModel, mainXml);
        if(!titel.startsWith("List") && update)
            titel = "List: "+ titel;
        std::vector<QUrl> urlVec;
        filterXMLData(titel, urlVec);
        for(auto& url : urlVec)
        {
            QFileInfo info(url.toString());
            if(IsRuning)
            {
                if(!desc && info.completeSuffix() == "webarchive")
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
                if(!desc && info.completeSuffix() == "webarchive")
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
}

void MainWindow::itemListDoubleClicked(const QModelIndex &index)
{
    doubleClick = false;


//    wasModified.push_back(index);
//    QFileInfo info( QFileDialog::getOpenFileName(this));
//    QString item = info.baseName();
//    QString path = info.absoluteFilePath();
//    qDebug() << "Item"<< item << " -|- " << path;

//    QStringList list = listViewModel->stringList();
//    list << "List: "+ item;

//    if(!path.isNull())
//        mainXml->xmlVec->at(index.row())->url.push_back(path);
//    if(!item.isNull())
//        mainXml->xmlVec->at(index.row())->name = item;
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
    qDebug() <<"NO Fun";
    if(!doubleClick)
        itemListClicked(index);
    else
        itemListDoubleClicked(index);
}

void MainWindow::on_ItemInList_doubleClicked(const QModelIndex &index)
{
    doubleClick = true;
}
