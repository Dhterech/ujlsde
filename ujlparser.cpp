#include "ujlparser.h"
#include <algorithm>
#include <sstream>

char buff[] = {0};

std::fstream rawFile;

int ujlparser::loadFile(QString path)
{
    rawFile.open(path.toUtf8(), std::ios_base::in | std::ios_base::binary);
    if(!rawFile.is_open()) return 4;
    ujldata::ClearAll();

    // Starts reading the file process
    parseInfo();
    qDebug() << "-- Searching cutscene lines past " << rawFile.tellg();
    getCutscene(fileInfo.addrList.cutsceneAddress);
    qDebug() << "-- Searching gameplay lines past " << rawFile.tellg();
    getGameplay();
    if(fileInfo.addrList.cutsceneEndAddress != (char)0xFFFF) {
        qDebug() << "-- Searching cutscene end lines past " << rawFile.tellg();
        getCutscene(fileInfo.addrList.cutsceneEndAddress);
    }

    qDebug() << "file is loaded! " << path;
    fileInfo.orgFilePath = path;
    rawFile.close();
    return 0;
}

int ujlparser::parseInfo() {
    char magicbyte;
    rawFile.get(magicbyte);
    for(BINInfo stage : StageDatabase) {
        if((char)magicbyte != (char)stage.Magic) continue;

        if(stage.STGRRegAddr[fileInfo.currentRegion].gameplayAddress != (char)0xFFFF) fileInfo.isStageFile = true;

        fileInfo.languageCount = 1; // TODO: NTSC-J/PAL & where the fuck are the others?
        fileInfo.Name = stage.STGName;
        fileInfo.Description = stage.STGDescription;

        fileInfo.addrList.cutsceneAddress = stage.STGRRegAddr[fileInfo.currentRegion].cutsceneAddress;
        fileInfo.addrList.cutsceneEndAddress = stage.STGRRegAddr[fileInfo.currentRegion].cutsceneEndAddress;
        fileInfo.addrList.gameplayAddress = stage.STGRRegAddr[fileInfo.currentRegion].gameplayAddress;
        fileInfo.emptyArea = stage.STGEmptyArea;
        break;
    };
    return 0;
}

void ujlparser::getCutscene(uint32_t cutsceneAddress) {
    rawFile.seekg(cutsceneAddress);
    processCutscene();
    return;
}

void ujlparser::processCutscene() {
    for(int cl = 0; cl < fileInfo.languageCount; cl++) {
        CTable cutscene;

        cutscene.Language = cl;
        cutscene.Position = rawFile.tellg();
        while(rawFile.peek() != EOF) {
            cLine tmpLine;

            uint32_t tmpLangPos;
            rawFile.read(reinterpret_cast<char *>(&tmpLangPos), sizeof(tmpLangPos));
            rawFile.read(reinterpret_cast<char *>(&tmpLine.timestart), sizeof(tmpLine.timestart));
            rawFile.read(reinterpret_cast<char *>(&tmpLine.timeend), sizeof(tmpLine.timeend));

            if(tmpLangPos == 0 || tmpLangPos < fileInfo.binPositionStart) break;

            bool isLinked;
            int tmpSubID = processLine(tmpLangPos, (fileInfo.languageCount == 2 && cl == 1), isLinked);
            tmpLine.localisation = tmpSubID;
            tmpLine.localelinked = isLinked;

            cutscene.Subtitles.push_back(tmpLine);
        }

        cutscene.EndPosition = rawFile.tellg();
        Cutscenes.push_back(cutscene);
    }

    qDebug() << "-- End cutscene on " << rawFile.tellg();
    return;
}

void ujlparser::getGameplay() {
    rawFile.seekg(fileInfo.addrList.gameplayAddress);
    processGameplay();
}

void ujlparser::processGameplay() {
    for(int cl = 0; cl < fileInfo.languageCount; cl++) {
        uint32_t lastLangPos = 0;
        GTable gameplay;

        gameplay.Language = cl;
        gameplay.Position = rawFile.tellg();
        while(rawFile.peek() != EOF) {
            gLine tmpLine;

            uint32_t tmpLangPos;
            rawFile.ignore(4);
            rawFile.read(reinterpret_cast<char *>(&tmpLine.unknown), sizeof(tmpLine.unknown));
            rawFile.read(reinterpret_cast<char *>(&tmpLangPos), sizeof(tmpLangPos));

            if(tmpLangPos == 0 || tmpLangPos < fileInfo.binPositionStart) break;
            tmpLine.owner = (lastLangPos == tmpLangPos) ? 1 : 0;
            lastLangPos = (tmpLine.owner == 0) ? tmpLangPos : 0;
            lastLangPos = tmpLangPos;

            bool isLinked;
            int tmpSubID = processLine(tmpLangPos, (fileInfo.languageCount == 2 && cl == 1), isLinked);
            tmpLine.localisation = tmpSubID;
            tmpLine.localelinked = isLinked;

            gameplay.Subtitles.push_back(tmpLine);
        }

        gameplay.EndPosition = rawFile.tellg();
        Gameplays.push_back(gameplay);
    }

    qDebug() << "-- End cutscene on " << rawFile.tellg();
    return;
}

std::string ujlparser::parseLine(uint32_t position) {
    std::stringstream subtitle;
    int lastPos = rawFile.tellg();
    char a[1];
    rawFile.seekg(position - fileInfo.binPositionStart);
    while(rawFile.peek() != 0x00 && rawFile.peek() != EOF) {
        rawFile.read(a, 1);
        if(a[0] == (char)0x0A) subtitle << "@";
        else if(a[0] != (char)0x00) subtitle << a[0];
    }
    rawFile.seekg(lastPos);
    return subtitle.str();
}

int ujlparser::processLine(uint32_t position, bool eucJP, bool &isLinked) {
    auto subFinder = std::find_if(Subtitles.begin(), Subtitles.end(), [&position](const Subtitle& obj) {return obj.Position == position;});
    isLinked = (subFinder != Subtitles.end());
    if(isLinked) return std::distance(Subtitles.begin(), subFinder);

    Subtitle tmpSub;

    tmpSub.Position = position;
    std::string rawText = parseLine(position);
    tmpSub.Text = QString::fromLatin1(rawText);

    Subtitles.push_back(tmpSub);
    return Subtitles.size() - 1;
}
