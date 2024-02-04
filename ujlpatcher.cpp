#include "ujlpatcher.h"
#include <types.h>
#include <ujldata.h>
#include <QFile>
#include <QDebug>

std::fstream orgFile;
std::fstream outFile;

void ujlpatcher::ApplyChanges(int position, int size, int difference, int emptyBank) {
    int emptyStart = fileInfo.emptyArea[emptyBank].startAddress;
    int emptyEnd = fileInfo.emptyArea[emptyBank].endAddress;

    if(position < (emptyStart + fileInfo.binPositionStart)) { /* TODO: test "&& 0 < difference" to spare more bytes */
        // Move it to a area that it can be freely managed
        for(Subtitle &sub : Subtitles) {
            if((emptyEnd != 0) && sub.Position > emptyEnd) continue; // TODO: fix for release
            if(sub.Position == position) sub.Position = emptyStart + fileInfo.binPositionStart;
            else if(sub.Position >= emptyStart + fileInfo.binPositionStart) sub.Position += size + 1;
        }
    } else {
        for(Subtitle &sub : Subtitles) {
            // Arealdy in a good position, only add difference
            if((emptyEnd != 0) && sub.Position > emptyEnd + fileInfo.binPositionStart) continue;
            if(position < sub.Position && sub.Position >= emptyStart + fileInfo.binPositionStart) sub.Position += difference;
        }
    }
}


int ujlpatcher::PatchFile(QString path) {
    if(!QFile::exists(fileInfo.orgFilePath)) return 2;
    if(path != fileInfo.orgFilePath) {
        // Copy original file
        if(QFile::exists(path)) QFile::remove(path);
        QFile::copy(fileInfo.orgFilePath, path);
    }

    outFile.open(path.toUtf8(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if(!outFile.is_open()) return 4;

    // Modify pointers
    for(int c = 0; c < Cutscenes.size(); c++) {
        outFile.seekg(Cutscenes[c].Position);
        for(cLine &clin : Cutscenes[c].Subtitles) {
            outFile.write(reinterpret_cast<char *>(&Subtitles[clin.localisation].Position), sizeof(uint32_t));
            outFile.write(reinterpret_cast<char *>(&clin.timestart), sizeof(uint32_t));
            outFile.write(reinterpret_cast<char *>(&clin.timeend), sizeof(uint32_t));
        }
    }

    // Modify pointers [gameplay]
    if(fileInfo.isStageFile) {
        outFile.seekg(fileInfo.addrList.gameplayAddress);
        for(int g = 0; g < Gameplays.size(); g++) {
            uint32_t zero = 0;
            for(gLine &glin : Gameplays[g].Subtitles) {
                outFile.write(reinterpret_cast<char *>(&zero), sizeof(uint32_t));
                outFile.write(reinterpret_cast<char *>(&glin.unknown), sizeof(uint32_t));
                outFile.write(reinterpret_cast<char *>(&Subtitles[glin.localisation].Position), sizeof(uint32_t));
            }
            if(g != Gameplays.size() - 1) outFile.write(reinterpret_cast<char *>(&zero), sizeof(zero));
        }
    }

    // Write Subtitles
    for(Subtitle &sub : Subtitles) {
        outFile.seekg(sub.Position - fileInfo.binPositionStart);

        std::string string;
        string = sub.Text.toLatin1().constData();

        // Replacements
        std::replace(string.begin(), string.end(), '@', (char)0x0A); // PTR2SDE-like editing
        for(Replace rep : ReplacementTable) {
            std::replace(string.begin(), string.end(), rep.Character[0].toLatin1(), rep.Replacement[0].toLatin1());
        }

        outFile.write(string.c_str(), string.size());
        if(sizeof(sub.Text) != 0) outFile.put(0x00);
    }

    qDebug() << "file is patched! " << path;
    outFile.close();
    return 0;
}
