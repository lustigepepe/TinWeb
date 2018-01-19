#include "xmllibrary.h"
#include <QIODevice>
#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QDomProcessingInstruction>
#include <QStandardPaths>

XMlLibrary::XMlLibrary(QString& path)
    : xmlVec(new std::vector<xmlData*>)
{
    if (!path.isEmpty())
        xmlFile = new QFile(path);
    else
    {
        QString root = QDir::homePath();
        xmlFile = new QFile(root+QString("/TinWeb.xml"));
    }
}
XMlLibrary::XMlLibrary()
    : xmlVec(new std::vector<xmlData*>)
{
        QString root = QDir::homePath();
        xmlFile = new QFile(root+QString("/TinWeb.xml"));
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

void XMlLibrary::readXML()
{
    QDomDocument doc;
    if (!xmlFile->open(QIODevice::ReadWrite))
        return;
    if (!doc.setContent(xmlFile)) {
        xmlFile->close();
        return;
    }
    xmlFile->close();
    QDomElement rootDocElem = doc.documentElement();
    QDomNode n = rootDocElem.firstChild();
    if (!xmlVec && !n.isNull())
        xmlVec = std::make_unique<std::vector<xmlData*>>();
    while(!n.isNull()) {
       QDomElement e = n.toElement(); // try to convert the node to an element.
       QString  test = e.tagName();
       if(e.tagName() == "item")
       {
           if(!lastElement.isElement())
           {
               QDomElement nameT = e.firstChildElement("name");
               xmlData* temp = new xmlData();
               if(!nameT.isNull())
                   temp->name = nameT.text();
               for(QDomNode urlNode = e.firstChild();!urlNode.isNull();
                   urlNode = urlNode.nextSibling())
               {
                   if (urlNode.toElement().tagName() == "url")
                      temp->url.push_back(urlNode.toElement().text());
               }
               xmlVec->push_back(temp);
           }
       }
       n = n.nextSibling();
    }
}

void XMlLibrary::writeXML()
{
    if (!xmlFile->open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << "Error writing to XML";
        xmlFile->close();
        return;
    }
    QDomDocument doc;
    QDomProcessingInstruction proIn = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(proIn);

    QDomElement mainTag = doc.createElement("mainXml");
    for(auto& data : *xmlVec)
    {
        QDomElement item = doc.createElement("item");
        QDomElement urlE = doc.createElement("url");
        QDomElement nameE = doc.createElement("name");

        QDomText nameText = doc.createTextNode(data->name);
        nameE.appendChild(nameText);
        item.appendChild(nameE);

        if (!data->url.empty())
        {
            for(auto record : data->url)
            {
                QDomElement urlE = doc.createElement("url");
                QDomText urlText = doc.createTextNode(record);
                urlE.appendChild(urlText);
                item.appendChild(urlE);
            }
        }
        mainTag.appendChild(item);
    }
    doc.appendChild(mainTag);
    QTextStream output(xmlFile);
    output << doc.toString();
    xmlFile->close();
}

