#ifndef XMLLIBRARY_H
#define XMLLIBRARY_H

#include <QString>
#include <QFile>
#include <memory>
#include <QDomDocument>

using xmlData = struct xmlData;

class XMlLibrary
{
public:
    XMlLibrary(QString* path = nullptr);
    ~XMlLibrary();
    void readXML(QString* path = nullptr, QString* list = nullptr, QString* name = nullptr);
    std::unique_ptr<std::vector<xmlData*>> xmlVec;
private:
    void writeXML();
    void parseString(QString& line);
    QFile* xmlFile;
    QDomElement lastElement;
    QDomDocument doc;

};

struct xmlData
{
    QString url;
    std::vector<QString> list;
    QString name;
};

#endif // XMLLIBRARY_H
