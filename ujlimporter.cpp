#include "ujlimporter.h"

void ujlimporter::LoadReplaceTable(QString path) {
    savFile.open(path.toUtf8(), std::ios_base::in | std::ios_base::binary);

    iLoadReplaceTable();

    savFile.close();
}

void ujlimporter::iLoadReplaceTable() {
    while(savFile.peek() != EOF) {
        Replace repl;
        savFile.read(reinterpret_cast<char *>(&repl.Character), sizeof(repl.Character));
        savFile.read(reinterpret_cast<char *>(&repl.Replacement), sizeof(repl.Replacement));
        ReplacementTable.push_back(repl);
    }
}

void ujlimporter::LoadProject(QString path) {
    uint32_t tempUint32;
    char buff[8];
    savFile.open(path.toUtf8(), std::ios_base::in | std::ios_base::binary);

    savFile.read(buff, 8);
    if(buff[0] != head[0]) return;

    savFile.read(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
    Cutscenes.resize(tempUint32);
    for(int c = 0; c < Cutscenes.size(); c++) {
        CTable cTab;
        savFile.read(reinterpret_cast<char *>(&cTab.Position), sizeof(cTab.Position));
        savFile.read(reinterpret_cast<char *>(&cTab.EndPosition), sizeof(cTab.EndPosition));

        savFile.read(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
        cTab.Subtitles.resize(tempUint32);
        for(int s = 0; s < cTab.Subtitles.size(); s++) {
            cLine cSub;
            savFile.write(reinterpret_cast<char *>(&cSub.localelinked), sizeof(cSub.localelinked));
            savFile.write(reinterpret_cast<char *>(&cSub.localisation), sizeof(cSub.localisation));
            cTab.Subtitles.push_back(cSub);
        }
        Cutscenes.push_back(cTab);
    }

    savFile.read(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
    Gameplays.resize(tempUint32);
    for(int g = 0; g < Gameplays.size(); g++) {
        GTable gTab;
        savFile.write(reinterpret_cast<char *>(&gTab.Position), sizeof(gTab.Position));
        savFile.write(reinterpret_cast<char *>(&gTab.EndPosition), sizeof(gTab.EndPosition));
        savFile.write(reinterpret_cast<char *>(&gTab.Language), sizeof(gTab.Language));

        savFile.read(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
        gTab.Subtitles.resize(tempUint32);
        for(int s = 0; s < gTab.Subtitles.size(); s++) {
            gLine gSub;
            savFile.read(reinterpret_cast<char *>(&gSub.localelinked), sizeof(gSub.localelinked));
            savFile.read(reinterpret_cast<char *>(&gSub.localisation), sizeof(gSub.localisation));
            savFile.read(reinterpret_cast<char *>(&gSub.owner), sizeof(gSub.owner));
            savFile.read(reinterpret_cast<char *>(&gSub.unknown), sizeof(gSub.unknown));
            gTab.Subtitles.push_back(gSub);
        }
        Gameplays.push_back(gTab);
    }

    savFile.read(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
    Subtitles.resize(tempUint32);
    for(int s = 0; s < Subtitles.size(); s++) {
        Subtitle sub;
        savFile.read(reinterpret_cast<char *>(&sub.isEucJP), sizeof(sub.isEucJP));
        savFile.read(reinterpret_cast<char *>(&sub.Position), sizeof(sub.Position));

        savFile.read(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
        char abc[tempUint32];
        savFile.read(reinterpret_cast<char *>(&abc), tempUint32);
        Subtitles.push_back(sub);
    }

    iLoadReplaceTable();

    savFile.close();
}
