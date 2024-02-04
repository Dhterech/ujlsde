#ifndef STAGEINFO_H
#define STAGEINFO_H

#include <QString>
#include <vector>
#include <types.h>

// ZUNGLE PROPRIETARY! just compile with zunglec
// /j

struct AddressList {
    uint32_t cutsceneAddress;
    uint32_t gameplayAddress;
    uint32_t cutsceneEndAddress;
};

struct EmptyArea {
    uint32_t startAddress;
    uint32_t endAddress;
};

struct CurInfo {
    // GUI
    QString Name;
    QString Description;
    int subCount = 0;
    int cutCount = 0;
    int gamCount = 0;

    // Parse/Save
    QString orgFilePath;
    bool hasEdited;
    bool isStageFile;
    int currentRegion = 0;
    int languageCount;
    int binPositionStart = 0x801C5750;
    AddressList addrList;
    std::vector<EmptyArea> emptyArea;
};

struct BINInfo {
    int Magic;
    QString STGName;
    QString STGDescription;
    std::vector<AddressList> STGRRegAddr;
    std::vector<EmptyArea> STGEmptyArea;
};

// for gameplay, 8 bytes before the pointer
// for cutscenes, just at the pointer

// I will split this part in sections, why not?
// Intermission
const BINInfo Start = {0x04, "0 - Jet Baby!", "", {{0x00012D9C, 0, 0}}, {{0x00012F1C, 0x00013908}}};
// Stage 1 Singleplayer
const BINInfo Stage1 = {0x0E, "1 - [SinglePlayer] I Am A Master And You! (Now Wait a Minute! Who Is That!?)", "", {{112172, 116352, 0}}, {{0x8C68, 0x11B93}}};
const BINInfo Stage1P = {0x05, "1 - [SinglePlayer, Parappa ] I Am A Master And You! (Now Wait a Minute! Who Is That!?)", "", {{0, 0x199A8, 0}}, {{0x00019FB4, 0x0001C370}}};
// Stage 2 Singleplayer
const BINInfo Stage2 = {0x0f, "2 - [SinglePlayer] Fire Fire! ! (B, back Off!! You're in the way!)", "", {{0x0001286C, 0x00013B28, 0}}, {{0x00013F80, 0x0001555C}}};
const BINInfo Stage2P = {0x06, "2 - [SinglePlayer, Parappa] Fire Fire! ! (B, back Off!! You're in the way!)", "", {{0x00013914, 0x00014DB8, 0}}, {{0x00015370, 0x0001749F}}};
// Stage 2 Coop
const BINInfo Stage2COPR = {0x18, "2 - [MultiPlayer, Lammy coop with Rammy] Fire Fire! ! (B, back Off!! You're in the way!)", "", {{0x000156F4, 0x000169B0, 0}}, {{0x0001720C, 0x00019308}}};
const BINInfo Stage2COPP = {0x21, "2 - [MultiPlayer, Lammy coop with Parappa] Fire Fire! ! (B, back Off!! You're in the way!)", "", {{0x000103C4, 0x00011608, 0}}, {{0x00011EB8, 0x00014554}}};
// Stage 2 VS
const BINInfo Stage2VSR = {0x33, "2 - [MultiPlayer, Lammy VS Rammy] Fire Fire! ! (B, back Off!! You're in the way!)", "", {{0, 0x000158F4, 0}}, {{0x00016150, 0x0001824C}}};
const BINInfo Stage2VSP = {0x2A, "2 - [MultiPlayer, Lammy VS Parappa] Fire Fire! ! (B, back Off!! You're in the way!)", "", {{0, 0x00016950, 0}}, {{0x00017200, 0x000191DC}}};
// Stage 3 Singleplayer
const BINInfo Stage3 = {0x10, "3 - [Singleplayer] Baby Baby!! (After School Sale)", "", {{0x0001A354, 0x0001BF20, 0}}, {{0x0001C2CC, 0x0001D3D4}}};
const BINInfo Stage3P = {0x07, "3 - [Singleplayer, Parappa] Baby Baby!! (After School Sale)", "", {{0x00018F60, 0x00019F00, 0}}, {{0x0001A3A8, 0x0001BE10}}};
// Stage 3 Coop
const BINInfo Stage3COPR = {0x19, "3 - [Multiplayer, Lammy coop with Rammy] Baby Baby!! (After School Sale)", "", {{0x0001C794, 0x0001E360, 0}}, {{0x0001E99C, 0x000202C4}}};
const BINInfo Stage3COPP = {0x22, "3 - [Multiplayer, Lammy coop with Parappa] Baby Baby!! (After School Sale)", "", {{0x000112BC, 0x000120DC, 0}}, {{0x000127C8, 0x000145D0}}};
// Stage 3 VS
const BINInfo Stage3VSR = {0x34, "3 - [Multiplayer, Lammy VS Rammy] Baby Baby!! (After School Sale)", "", {{0, 0x0001C66C, 0}}, {{0x0001CCA8, 0x0001E5D0}}};
const BINInfo Stage3VSP = {0x2B, "3 - [Multiplayer, Lammy VS Parappa] Baby Baby!! (After School Sale)", "", {{0, 0x0001D80C, 0}}, {{0x0001DEF8, 0x0001FD00}}};
// Stage 4 Singleplayer
const BINInfo Stage4 = {0x11, "4 - [Singleplayer] Fright Flight!! (Now Which One Is My Left Hand?)", "", {{0x0001AC6C, 0x0001CAC0, 0}}, {{0x0001CFF8, 0x0001E700}}};
const BINInfo Stage4P = {0x08, "4 - [Singleplayer, Parappa] Fright Flight!! (Now Which One Is My Left Hand?)", "", {{0x0001BDC0, 0x0001D1E4, 0}}, {{0x0001D8C4, 0x0001FE0C}}};
// Stage 4 Coop
const BINInfo Stage4COPR = {0x1A, "4 - [Singleplayer Lammy coop with Rammy] Fright Flight!! (Now Which One Is My Left Hand?)", "", {{0x0001CE90, 0x0001ECE4, 0}}, {{0x0001F680, 0x000216C8}}};
const BINInfo Stage4COPP = {0x23, "4 - [Singleplayer Lammy coop with Parappa] Fright Flight!! (Now Which One Is My Left Hand?)", "", {{0x0001539C, 0x00016474, 0}}, {{0x00016E80, 0x00019528}}};
// Stage 4 VS
const BINInfo Stage4VSR = {0x35, "4 - [Singleplayer Lammy VS Rammy] Fright Flight!! (Now Which One Is My Left Hand?)", "", {{0, 0x0001CF38, 0}}, {{0x0001D8D4, 0x0001F91C}}};
const BINInfo Stage4VSP = {0x2C, "4 - [Singleplayer Lammy VS Parappa] Fright Flight!! (Now Which One Is My Left Hand?)", "", {{0, 0x0001FBA8, 0}}, {{0x000205B4, 0x00022C5C}}};
// Stage 5 Singleplayer
const BINInfo Stage5 = {0x12, "5 - [Singleplayer] Power Off! Power On! (You Said Anything, Didn't Ya?)", "", {{0x00012880, 0x00014668, 0}}, {{0x00014B5C, 0x000161F8}}};
const BINInfo Stage5P = {0x09, "5 - [Singleplayer, Parappa] Power Off! Power On! (You Said Anything, Didn't Ya?)", "", {{0x00013CD8, 0x000158BC, 0}}, {{0x00015F00, 0x000180DC}}};
// Stage 5 Coop
const BINInfo Stage5COPR = {0x1B, "5 - [Multiplayer, Lammy coop with Rammy] Power Off! Power On! (You Said Anything, Didn't Ya?", "", {{0x00014344, 0x0001612C, 0}}, {{0x00016A04, 0x00018A4C}}};
const BINInfo Stage5COPP = {0x24, "5 - [Multiplayer, Lammy coop with Parappa] Power Off! Power On! (You Said Anything, Didn't Ya?", "", {{0x00016750, 0x000180A4, 0}}, {{0x000189EC, 0x0001AFD4}}};
// Stage 5 VS
const BINInfo Stage5VSR = {0x36, "5 - [Multiplayer, Lammy VS Rammy] Power Off! Power On! (You Said Anything, Didn't Ya?", "", {{0, 0x000144AC, 0}}, {{0x00014D84, 0x00016D58}}};
const BINInfo Stage5VSP = {0x2D, "5 - [Multiplayer, Lammy VS Parappa] Power Off! Power On! (You Said Anything, Didn't Ya?", "", {{0, 0x00015744, 0}}, {{0x0001608C, 0x00018674}}};
// Stage 6 Singleplayer
const BINInfo Stage6 = {0x13, "6 - [Singleplayer] Taste of Teriyaki (Vital Idol)", "", {{0x00015C60, 0x000173B0, 0}}, {{0x0001783C, 0x00018C38}}};
const BINInfo Stage6P = {0x0A, "6 - [Singleplayer, Parappa] Taste of Teriyaki (Vital Idol)", "", {{0x00014CF8, 0x00015FF4, 0}}, {{0x0001660C, 0x00018668}}};
// Stage 6 Coop
const BINInfo Stage6COPR = {0x1C, "6 - [Multiplayer, Lammy coop with Rammy] Taste of Teriyaki (Vital Idol)", "", {{0x00018110, 0x00019860, 0}}, {{0x0001A0F4, 0x0001C010}}};
const BINInfo Stage6COPP = {0x25, "6 - [Multiplayer, Lammy coop with Parappa] Taste of Teriyaki (Vital Idol)", "", {{0x0001893C, 0x000199D0, 0}}, {{0x0001A3A0, 0x0001C9DC}}};
// Stage 6 VS
const BINInfo Stage6VSR = {0x37, "6 - [Multiplayer, Lammy VS Rammy] Taste of Teriyaki (Vital Idol)", "", {{0, 0x00018380, 0}}, {{0x00018C14, 0x0001AB30}}};
const BINInfo Stage6VSP = {0x2E, "6 - [Multiplayer, Lammy VS Parappa] Taste of Teriyaki (Vital Idol)", "", {{0, 0x00019000, 0}}, {{0x000199D0, 0x0001C00C}}};
// Stage 7 Singleplayer
const BINInfo Stage7 = {0x14, "7 - [Singleplayer] Got to Move! (MILKCAN, That's Us!)", "", {{0x000190FC, 0x0001B0DC, 0}}, {{0x0001B774, 0x0001D6C0}}};
const BINInfo Stage7P = {0x0b, "7 - [Singleplayer, Parappa] Got to Move! (MILKCAN, That's Us!)", "", {{0x000181A4, 0x000194C0, 0}}, {{0x00019D50, 0x0001CD0D}}};
// Stage 7 Coop
const BINInfo Stage7COPR = {0x1D, "7 - [Multiplayer, Lammy coop with Rammy] Got to Move! (MILKCAN, That's Us!)", "", {{0x0001B3F8, 0x0001D3D8, 0}}, {{0x0001DF04, 0x000209C1}}};
const BINInfo Stage7COPP = {0x26, "7 - [Multiplayer, Lammy coop with Parappa] Got to Move! (MILKCAN, That's Us!)", "", {{0x0001A318, 0x0001B394, 0}}, {{0x0001BFC8, 0x0001F325}}};
// Stage 7 VS
const BINInfo Stage7VSR = {0x38, "7 - [Multiplayer, Lammy VS Rammy] Got to Move! (MILKCAN, That's Us!)", "", {{0, 0x0001B430, 0}}, {{0x0001BF5C, 0x0001EA19}}};
const BINInfo Stage7VSP = {0x2F, "7 - [Multiplayer, Lammy VS Parappa] Got to Move! (MILKCAN, That's Us!)", "", {{0, 0x0001A988, 0}}, {{0x0001B5BC, 0x0001E919}}};
// Stage 8, AKA joe chin cutscenes and the final stg1 cutscenes
const BINInfo Stage8JC = {0x1E, "8 - Joe chin LapTop cutscene", "", {{0x00000B9C, 0, 0}}, {{0x00000C50, 0x000010A8}}};
const BINInfo Stage8S1 = {0x30, "8 - Stage 1 Part2 cutscene", "", {{0x00000B3C, 0, 0}}, {{0x00000BD8, 0x00000F88}}};




// Reminder to myself: add the fucking BINinfo into the stage database
// Counter for how many times i forgot about it : 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
const std::vector<BINInfo> StageDatabase = { Start, Stage1, Stage1P, Stage2, Stage2P, Stage2COPR, Stage2COPP, Stage2VSR, Stage2VSP, Stage3, Stage3P, Stage3COPR, Stage3COPP, Stage3VSR, Stage3VSP, Stage4, Stage4P, Stage4COPR, Stage4COPP, Stage4VSR, Stage4VSP, Stage5, Stage5P, Stage5COPR, Stage5COPP, Stage5VSR, Stage5VSP, Stage6, Stage6P, Stage6COPR, Stage6COPP, Stage6VSR, Stage6VSP, Stage7, Stage7P, Stage7COPR, Stage7COPP, Stage7VSR, Stage7VSP, Stage8JC, Stage8S1 };
#endif // STAGEINFO_H
