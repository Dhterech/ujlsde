#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <QString>

class Subtitle {
public:
    QString Text;
    int Position;
    bool isEucJP;
};

struct cLine {
    int localisation;
    int timestart;
    int timeend;

    bool localelinked;
};

struct gLine {
    int localisation; //that's why you shoulndt use your other codes for a base to others programs
    int unknown;
    bool localelinked;

    int owner;
};

class CTable {
public:
    int Position;
    int EndPosition;
    int Language;
    std::vector<cLine> Subtitles;
};

class GTable {
public:
    int Position;
    int EndPosition;
    int Language;
    std::vector<gLine> Subtitles;
};

struct Patch {
    int Position;
    int Difference;
};

struct Replace {
    QString Character;
    QString Replacement;
};


#endif // TYPES_H
