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
    void readWriteXML(QString* path = nullptr, bool list = false, QString* name = nullptr);
    std::unique_ptr<std::vector<xmlData*>> xmlVec;
private:
    void creatDoc(QString* path, bool list, QString* name);
    void parseString(QString& line);
    QFile* xmlFile;
    QDomElement lastElement;
    QDomDocument doc;

};

struct xmlData
{
    QString url;
    bool list;
    QString name;
};

#endif // XMLLIBRARY_H
