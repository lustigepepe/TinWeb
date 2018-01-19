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
    XMlLibrary(QString& path);
    XMlLibrary();

    ~XMlLibrary();
    void readXML();
    std::unique_ptr<std::vector<xmlData*>> xmlVec;
    void writeXML();
private:
    void parseString(QString& line);
    QFile* xmlFile;
    QDomElement lastElement;
    QDomDocument doc;
};

struct xmlData
{
    QString name;
    std::vector<QString> url;
};

#endif // XMLLIBRARY_H
