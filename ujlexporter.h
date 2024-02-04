#ifndef UJLEXPORTER_H
#define UJLEXPORTER_H

#include "ujldata.h"
#include <QString>
#include <QFile>

class ujlexporter
{
private:
    std::fstream outFile;
    void iSaveReplaceTable();

public:
    QString head = "UJL!SDE!";
    void SaveReplaceTable(QString path);
    void SaveProject(QString path);
};

#endif // UJLEXPORTER_H
