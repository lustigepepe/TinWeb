#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"
#include "xmllibrary.h"
#include <QMainWindow>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QStringListModel>
#include <QListView>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include "singleton.h"
#include <QDebug>

typedef QApplication Application;

namespace Ui {
class MainWindow;
}
class Browser;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    std::unique_ptr<QQmlApplicationEngine> browserApp;
    ~MainWindow();
    bool IsRuning = false;

private slots:
    void on_newSite_clicked();
    void on_newList_clicked();
    void on_overView_clicked();
    void on_ItemInList_doubleClicked(const QModelIndex &index);
    void on_ItemInList_clicked(const QModelIndex &index);

protected:
    void dropEvent(QDropEvent *e);
private:
    typedef QWidget super;
//    QQmlApplicationEngine* browserApp;
    QUrl startupUrl(QString* url = nullptr);
    bool isLeftClick;
    Ui::MainWindow *ui;
    Document m_content;
    QStringListModel *listViewModel;
    XMlLibrary* mainXml;
    bool filterXMLData(QString& name, QUrl& url);
    QStringList fillOverviewList(QStringList& list);
    bool desc = false;
};

class Browser : public Singleton<Browser>
{
public:
    friend class Singleton;
    void createBrowser();
    std::unique_ptr<QQmlApplicationEngine> browser;
private:
    Browser()=default;
    Browser(const Browser&);
    Browser& operator = (const Browser&);
};
#endif // MAINWINDOW_H
