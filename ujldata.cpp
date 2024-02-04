#include "ujldata.h"

CurInfo fileInfo;
std::vector<CTable> Cutscenes;
std::vector<GTable> Gameplays;
std::vector<Subtitle> Subtitles;
std::vector<Replace> ReplacementTable;

void ujldata::ClearAll() {
    Subtitles.clear();
    Cutscenes.clear();
    Gameplays.clear();

    fileInfo.subCount = 0;
    fileInfo.cutCount = 0;
    fileInfo.gamCount = 0;
    fileInfo.hasEdited = false;
}
