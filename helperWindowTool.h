#ifndef HELPERWINDOW_H
#define HELPERWINDOW_H
#include <QStringListModel>
#include "xmllibrary.h"

bool updateName(std::vector<QModelIndex>& indices, QStringListModel* ml, XMlLibrary* mainXml)
{
    if(indices.empty())
        return false;
    QStringList list = ml->stringList();
    for(auto in : indices)
    {
        int i = in.row();
        QString st = in.data().toString();
        if (mainXml->xmlVec->at(i)->url.size() > 1)
        {
            QStringList split = list.at(i).split("List: ");
            if(split.size() == 1)
                list.replace(i,"List: "+st);
            else
                list.replace(i, st);
            mainXml->xmlVec->at(i)->name = "List: "+ st;
        }
        else
            mainXml->xmlVec->at(i)->name = st;
    }
    ml->setStringList(list);
    indices.clear();

    qDebug() <<" updateName ";
    return true;
}






#endif // HELPERWINDOW_H
