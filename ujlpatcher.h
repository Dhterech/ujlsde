#ifndef UJLPATCHER_H
#define UJLPATCHER_H

#include <vector>
#include <sstream>
#include <QDebug>
#include <fstream>

class ujlpatcher
{
public:
    int PatchFile(QString path);
    void ApplyChanges(int position, int size, int difference, int emptyBank);
    std::string convrEUCJP(std::string modString);
};

#endif // UJLPATCHER_H
