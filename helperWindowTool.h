#ifndef HELPERWINDOW_H
#define HELPERWINDOW_H
#include <QStringListModel>
#include "xmllibrary.h"

QUrl startupUrl(QString* url)
{
    QUrl ret;
    QStringList args(qApp->arguments());
    args.takeFirst();
    Q_FOREACH (const QString& arg, args) {
        if (arg.startsWith(QLatin1Char('-')))
             continue;
        ret = Utils::fromUserInput(arg);
        if (ret.isValid())
            return ret;
    }
    return QUrl((url ?  *url : "http://qt.io/"), QUrl::TolerantMode);

}


bool setOverviewListName(std::vector<QModelIndex>& indices, QStringListModel* ml, XMlLibrary* mainXml)
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
        {
            mainXml->xmlVec->at(i)->name = st;
            ml->setStringList(list);
            indices.clear();
            qDebug() <<" setOverviewListName ";
            return false;
        }
    }
    ml->setStringList(list);
    indices.clear();
    qDebug() <<" setOverviewListName ";
    return true;
}


void convertWebarchiveToHtml(xmlData* data, bool desc)
{
    for (auto& url : data->url)
    {
        QFileInfo info(url);
        if(info.completeSuffix() == "webarchive" && desc)
        {
            bool unix = QSysInfo::productType() == "winrt" ? false : true;
            QString newFolder, newFile;
            if(unix)
            {
                newFolder = (info.canonicalPath()+"/NoWebArchive");
                newFile = newFolder + "/" + info.completeBaseName() + ".html";
            }
            else
            {
                newFolder = (info.canonicalPath()+"\\NoWebArchive");
                newFile = newFolder + "\\"+ info.completeBaseName() + ".html";
            }

            QProcess *proc = new QProcess();
            if(!QFile(newFile).exists())
            {

                if(!QDir(newFolder).exists())
                    QDir().mkdir(newFolder);

                QString program;
                QStringList arguments;
                if(unix)
                {
                    program = "/usr/bin/textutil";
                    arguments << "-convert"  << "html" << url
                              << "-output" << newFile;
                }
                else
                {
                    program = "C:\Windows\SysWOW64\textutil";
                    arguments << "-convert"  << "html" << url
                              << "-output" << newFile;
                }

                proc->start(program, arguments);
                proc->closeWriteChannel();
            }
        }
    }
}

#endif // HELPERWINDOW_H
