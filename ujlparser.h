#ifndef UJLPARSER_H
#define UJLPARSER_H
#include <ujldata.h>
#include <QDebug>
#include <fstream>

class ujlparser
{
public:
    int loadFile(QString path);
    int parseInfo();

private:
    // Getting/Parsing types of subtitle holders
    void getCutscene(uint32_t cutsceneAddress);
    void processCutscene();
    void getGameplay();
    void processGameplay();

    // Subtitle functions
    int processLine(uint32_t position, bool dfEnc, bool &isLinked);
    std::string parseLine(uint32_t position);
    std::string parseEUCJP(std::string orgString);
};

#endif // ujlPARSER_H
