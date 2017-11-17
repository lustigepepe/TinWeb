#include "xmllibrary.h"
#include <QIODevice>
#include <QDomDocument>

XMlLibrary::XMlLibrary(QString* path)
    : xmlVec(new std::vector<xmlData*>)
{
    if (path)
        xmlFile = new QFile(*path);
    else
        xmlFile = new QFile(":xml/xml/main.xml");
}

XMlLibrary::~XMlLibrary()
{
 if (xmlFile)
     delete xmlFile;
 if (xmlVec)
 {
     for (auto &x : *xmlVec)
            delete x;
     delete xmlVec;
 }
}

void XMlLibrary::writeSite(QString& path, bool list, QString& name)
{
      xmlFile->open(QIODevice::ReadWrite);
}

void XMlLibrary::readSite()
{
    QDomDocument doc;
    if (!xmlFile->open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(xmlFile)) {
        xmlFile->close();
        return;
    }
    xmlFile->close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull() && e.tagName() == "item")
        {
            QDomElement url = e.firstChildElement("url");
            QDomElement list = e.firstChildElement("list");
            QDomElement name = e.firstChildElement("name");
            xmlData* temp = new xmlData();
            if(!url.isNull())
                temp->url = url.text();
            if(!list.isNull())
                temp->list = list.text() == "true" || list.text() == "True";
            if(!name.isNull())
                temp->name = name.text();
            xmlVec->push_back(temp);
        }
        n = n.nextSibling();
    }
}

void XMlLibrary::parseString(QString &line)
{

}

