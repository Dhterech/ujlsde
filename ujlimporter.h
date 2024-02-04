#ifndef UJLIMPORTER_H
#define UJLIMPORTER_H

#include "ujldata.h"
#include "ujlexporter.h"
#include <QString>
#include <QFile>

class ujlimporter
{
private:
    std::fstream savFile;
    void iLoadReplaceTable();

public:
    QString head = "UJL!SDE!";
    void LoadReplaceTable(QString path);
    void LoadProject(QString path);
};

#endif // SDE1IMPORTER_H
