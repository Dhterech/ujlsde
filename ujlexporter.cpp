#include "ujlexporter.h"

void ujlexporter::SaveReplaceTable(QString path) {
    outFile.open(path.toUtf8(), std::ios_base::out | std::ios_base::binary);

    iSaveReplaceTable();

    outFile.close();
}

void ujlexporter::iSaveReplaceTable() {
    for(Replace repl : ReplacementTable) {
        outFile.write(reinterpret_cast<char *>(&repl.Character), sizeof(repl.Character));
        outFile.write(reinterpret_cast<char *>(&repl.Replacement), sizeof(repl.Replacement));
    }
}

void ujlexporter::SaveProject(QString path) {
    uint32_t tempUint32;
    outFile.open(path.toUtf8(), std::ios_base::out | std::ios_base::binary);

    std::string abc = head.toLatin1().constData();
    outFile.write(abc.c_str(), abc.size());

    tempUint32 = Cutscenes.size();
    outFile.write(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
    for(CTable ctable : Cutscenes) {
        outFile.write(reinterpret_cast<char *>(&ctable.Position), sizeof(ctable.Position));
        outFile.write(reinterpret_cast<char *>(&ctable.EndPosition), sizeof(ctable.EndPosition));

        tempUint32 = ctable.Subtitles.size();
        outFile.write(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
        for(cLine csub : ctable.Subtitles) {
            outFile.write(reinterpret_cast<char *>(&csub.localelinked), sizeof(csub.localelinked));
            outFile.write(reinterpret_cast<char *>(&csub.localisation), sizeof(csub.localisation));
        }
    }

    tempUint32 = Gameplays.size();
    outFile.write(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
    for(GTable gtable : Gameplays) {
        outFile.write(reinterpret_cast<char *>(&gtable.Position), sizeof(gtable.Position));
        outFile.write(reinterpret_cast<char *>(&gtable.EndPosition), sizeof(gtable.EndPosition));
        outFile.write(reinterpret_cast<char *>(&gtable.Language), sizeof(gtable.Language));

        tempUint32 = gtable.Subtitles.size();
        outFile.write(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
        for(gLine gsub : gtable.Subtitles) {
            outFile.write(reinterpret_cast<char *>(&gsub.localelinked), sizeof(gsub.localelinked));
            outFile.write(reinterpret_cast<char *>(&gsub.localisation), sizeof(gsub.localisation));
            outFile.write(reinterpret_cast<char *>(&gsub.owner), sizeof(gsub.owner));
            outFile.write(reinterpret_cast<char *>(&gsub.unknown), sizeof(gsub.unknown));
        }
    }

    tempUint32 = Subtitles.size();
    outFile.write(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
    for(Subtitle sub : Subtitles) {
        outFile.write(reinterpret_cast<char *>(&sub.isEucJP), sizeof(sub.isEucJP));
        outFile.write(reinterpret_cast<char *>(&sub.Position), sizeof(sub.Position));

        std::string string = sub.Text.toLatin1().constData();
        tempUint32 = string.size();
        outFile.write(reinterpret_cast<char *>(&tempUint32), sizeof(tempUint32));
        outFile.write(string.c_str(), string.size());
    }

    iSaveReplaceTable();

    outFile.close();
}
