#ifndef XMLLIBRARY_H
#define XMLLIBRARY_H

#include <QString>
#include <QFile>
#include <memory>

using xmlData = struct xmlData;

class XMlLibrary
{
public:
    XMlLibrary(QString* path = nullptr);
    ~XMlLibrary();
    void readWriteSite(QString* path = nullptr, bool list = false, QString* name = nullptr);
    std::unique_ptr<std::vector<xmlData*>> xmlVec;
private:
    void parseString(QString& line);
    QFile* xmlFile;
};

struct xmlData
{
    QString url;
    bool list;
    QString name;
};

#endif // XMLLIBRARY_H
