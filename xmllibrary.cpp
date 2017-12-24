#include "xmllibrary.h"
#include <QIODevice>
#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <qDebug>

XMlLibrary::XMlLibrary(QString* path)
    : xmlVec(new std::vector<xmlData*>)
{
    if (path)
        xmlFile = new QFile(*path);
    else
    {
        QString current =  QDir::currentPath();
        QStringList list = current.split(QFileInfo(QCoreApplication::applicationFilePath()).fileName());
        xmlFile = new QFile(list.at(0)+QString("main.xml"));
    }
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
    bool wasContent = false;
    QDomElement rootDocElem = doc.documentElement();
    QDomNode n = rootDocElem.firstChild();
    if (!xmlVec && !n.isNull())
        xmlVec = std::make_unique<std::vector<xmlData*>>();
    while(!n.isNull()) {
       QDomElement e = n.toElement(); // try to convert the node to an element.
       QString  test = e.tagName();
        if(e.tagName() == "item")
        {
            wasContent = true;
            QDomElement urlT = e.firstChildElement("url");
            QDomElement listT = e.firstChildElement("list");
            QDomElement nameT = e.firstChildElement("name");
            xmlData* temp = new xmlData();
            if(!urlT.isNull())
                temp->url = urlT.text();
            if(!listT.isNull())
                temp->list = listT.text() == "true" || listT.text() == "True";
            if(!nameT.isNull())
                temp->name = nameT.text();
            xmlVec->push_back(temp);
        }
        n = n.nextSibling();
    }
    if(rootDocElem.tagName() == "" || wasContent /*&& path*/)
    {
        creatDoc(rootDocElem, path, list, name);
        if (!xmlFile->open(QIODevice::WriteOnly | QFile::Text))
        {
            qDebug() << "Error writing to XML";
            xmlFile->close();
            return;
        }
        QTextStream output(xmlFile);
        output << doc.toString();
        xmlFile->close();
    }
}

void XMlLibrary::creatDoc(QDomElement& docFirst ,QString* path, bool list, QString* name)
{

    QDomDocument doc = docFirst.toDocument();
    QDomElement item = doc.createElement("item");
    QDomElement urlE = doc.createElement("url");
    QDomElement listE = doc.createElement("list");
    QDomElement nameE = doc.createElement("name");

    if(path)
    {
        QDomText urlText = doc.createTextNode(*path);
        urlE.appendChild(urlText);
    }
    QDomText listText = doc.createTextNode(list? "true":"false");
    listE.appendChild(listText);
    if(name)
    {
        QDomText nameText = doc.createTextNode(*name);
        nameE.appendChild(nameText);
    }

    item.appendChild(urlE);
    item.appendChild(listE);
    item.appendChild(nameE);
    //    item.setAttribute("sss","aa");
    docFirst.appendChild(item);
    xmlData* temp = new xmlData();
    if(!urlE.isNull())
        temp->url = urlE.text();
    if(!listE.isNull())
        temp->list = listE.text() == "true" || listE.text() == "True";
    if(!nameE.isNull())
        temp->name = nameE.text();
    xmlVec->push_back(temp);



//    if (!xmlFile->open(QIODevice::WriteOnly | QFile::Text))
//    {
//        qDebug() << "Error writing to XML";
//        xmlFile->close();
//        return;
//    }
//    QTextStream output(xmlFile);
//    output << doc.toString();
//    xmlFile->close();

}

