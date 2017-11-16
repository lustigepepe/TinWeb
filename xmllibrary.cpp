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
    if (!xmlFile->open(QIODevice::ReadOnly) || doc.setContent(xmlFile) )
        return;
    //    QTextStream in(xmlFile);
    //    QString line;
    //    while (in.readLineInto(&line))
    //    {
    //        xmlData* temp = new xmlData();
    //        temp->name = line;
    //        if(xmlVec)
    //            xmlVec->push_back(temp);
    //    }
    xmlFile->close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull() && e.tagName() == "item") {
            QDomElement url = n.firstChildElement("url");
            QDomElement list = n.firstChildElement("list");
            QDomElement name = n.firstChildElement("name");

            xmlData* temp = new xmlData();
            if(!url.toDocument().isNull())
                temp->url = url.toDocument().toString();
            if(!list.toDocument().isNull())
                temp->list = list.toDocument().toString() == "true" || "True" ? true : false;
            if(!name.toDocument().isNull())
                temp->name = name.toDocument().toString();
            xmlVec->push_back(temp);
        }
    }
    n = n.nextSibling();
}

void XMlLibrary::parseString(QString &line)
{

}

