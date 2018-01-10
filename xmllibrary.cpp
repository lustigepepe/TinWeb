#include "xmllibrary.h"
#include <QIODevice>
#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QDomProcessingInstruction>
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
    writeXML();
    if (xmlFile)
        delete xmlFile;
    if (xmlVec)
    {
        for (auto &x : *xmlVec)
            delete x;
    }
}

void XMlLibrary::readXML(QString* path, QString* list, QString* name)
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
           if(!lastElement.isElement())
           {
               wasContent = true;
               QDomElement urlT = e.firstChildElement("url");
               QDomElement nameT = e.firstChildElement("name");
               xmlData* temp = new xmlData();
               if(!urlT.isNull())
                   temp->url = urlT.text();
               if(!nameT.isNull())
                   temp->name = nameT.text();
               for(QDomNode listNode = e.firstChild();!listNode.isNull();
                   listNode = listNode.nextSibling())
               {
                   if (listNode.toElement().tagName() == "list")
                      temp->list.push_back(listNode.toElement().text());
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
        QDomText urlText = doc.createTextNode(data->url);
        urlE.appendChild(urlText);
        QDomText nameText = doc.createTextNode(data->name);
        nameE.appendChild(nameText);
        item.appendChild(urlE);
        item.appendChild(nameE);

        if (!data->list.empty())
        {
            for(auto record : data->list)
            {
                QDomElement listE = doc.createElement("list");
                QDomText listText = doc.createTextNode(record);
                listE.appendChild(listText);
                item.appendChild(listE);
            }
        }
        mainTag.appendChild(item);
    }
    doc.appendChild(mainTag);

    QTextStream output(xmlFile);
    output << doc.toString();
    xmlFile->close();
}

