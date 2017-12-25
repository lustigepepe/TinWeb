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
    ~MainWindow();

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

    bool isLeftClick;
    Ui::MainWindow *ui;
    Document m_content;
    QStringListModel *listViewModel;
    XMlLibrary* mainXml;
    friend class Browser;
};

class Browser : public Singleton <int>
{
public:
  // friend class Singleton;
    void createBrowser();
    std::unique_ptr<QQmlApplicationEngine> browser;
    QUrl startupUrl(QString* url = nullptr);

};
#endif // MAINWINDOW_H
