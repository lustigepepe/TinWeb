#include "printer.h"
#include <QPrintDialog>
#include <QPrinter>
#include <QDebug>
#include <QPainter>
#include <QtPrintSupport>
#include <QPrintDialog>

Printer::Printer(QObject* parent) :
    QObject(parent)
{
    QObject::connect(this, SIGNAL(triggerPrint()), this, SLOT(print()));
}

void Printer::setUrl(QString &url)
{
    if(m_url == url)
        return;
    QString literal = "file://";
    QStringList splitList;
    if(url.contains(literal))
    {
        splitList = url.split(literal);
        m_url = splitList.at(1);
    }
    else
        m_url = url;
    emit triggerPrint();
}

QString Printer::url()
{
    return m_url;
}

void Printer::print()
{
    QFile file(m_url);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QPrinter printer;
    QTextEdit textEdit;
    QTextStream in(&file);
    QString line, value;
    while (in.readLineInto(&line))
        value +=  line + "\n";
    file.close();
    QPrintDialog dlg(&printer, nullptr);
    if(dlg.exec() == QDialog::Accepted) {
        textEdit.insertPlainText(value);
        textEdit.print(&printer);
    }
}
