#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"

#include <QMainWindow>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QStringListModel>
namespace Ui {
class MainWindow;
}

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

private:
    Ui::MainWindow *ui;
    Document m_content;
    QStringListModel *listViewModel;
};

#endif // MAINWINDOW_H
