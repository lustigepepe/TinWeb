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
 }
}

void XMlLibrary::readWriteSite(QString* path, bool list, QString* name)
{
    QDomDocument doc;
    if (!xmlFile->open(QIODevice::ReadWrite))
        return;
    if (!doc.setContent(xmlFile)) {
        xmlFile->close();
        return;
    }
    xmlFile->close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if (!xmlVec && !n.isNull())
        xmlVec = std::make_unique<std::vector<xmlData*>>();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
       QString  test = e.tagName();
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

//        else if(!e.isNull() && e.tagName() == "item")
//        {
//            QDomElement url = e.firstChildElement("url");
//            QDomElement list = e.firstChildElement("list");
//            QDomElement name = e.firstChildElement("name");
//            xmlData* temp = new xmlData();
//            if(!url.isNull())
//                temp->url = url.text();
//            if(!list.isNull())
//                temp->list = list.text() == "true" || list.text() == "True";
//            if(!name.isNull())
//                temp->name = name.text();
//            xmlVec->push_back(temp);
//        }

        n = n.nextSibling();
    }
}

