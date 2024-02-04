#ifndef UJLDATA_H
#define UJLDATA_H

#include <fstream>
#include <types.h>
#include <stageinfo.h>

extern CurInfo fileInfo;

extern std::vector<CTable> Cutscenes;
extern std::vector<GTable> Gameplays;
extern std::vector<Subtitle> Subtitles;
extern std::vector<Replace> ReplacementTable;

class ujldata {
public:
    void static ClearAll();
};

#endif // ujlDATA_H
