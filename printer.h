#ifndef PRINTER_H
#define PRINTER_H
#include <QObject>
#include <QString>

class Printer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl)

public:
    explicit Printer(QObject *parent = nullptr);

    QString url();
    void setUrl(QString& url);

public slots:
    void print();

signals:
    void triggerPrint();

private:
    QString m_url;
};

#endif // PRINTER_H
