#ifndef CUSTOM_VILLAIN_GROUP_H__
#define CUSTOM_VILLAIN_GROUP_H__

#include <utilitieslib/utils/textparser.h>

typedef struct Entity Entity;
typedef struct PCC_Critter PCC_Critter;
typedef struct CustomNPCCostume CustomNPCCostume;
typedef struct CVG_ExistingVillain
{
    char *costumeIdx;
    char *pchName;
    char *displayName;
    char *description;
    int levelIdx;
    int setNum;
    CustomNPCCostume *npcCostume;
}CVG_ExistingVillain;

typedef struct CustomVG
{
    char *displayName;
    CVG_ExistingVillain **existingVillainList;
    PCC_Critter ** customVillainList;
    char *sourceFile;
    char **dontAutoSpawnEVs;
    char **dontAutoSpawnPCCs;
    int *badCritters;
    int *badStandardCritters;
    int reinsertRemovedCritter;
}CustomVG;

extern CustomVG **g_CustomVillainGroups;


typedef struct CompressedCVG
{
    char *displayName;
    CVG_ExistingVillain **existingVillainList;
    char **dontAutoSpawnEVs;
    char **dontAutoSpawnPCCs;
    int * customCritterIdx;
    int spawnCount[8];
}CompressedCVG;

typedef enum CVG_LoadErrors
{
    CVG_LE_VALID = 0,
    CVG_LE_NONAME = 1,
    CVG_LE_INVALID_NAME = 1 << 1,
    CVG_LE_INVALID_CRITTER = 1 << 2,
    CVG_LE_INVALID_STANDARD = 1 << 3,
    CVG_LE_INVALID_CRITTER_RANK = 1 << 4,
    CVG_LE_NO_CRITTERS = 1 << 5,
    CVG_LE_LEVELRANGE_HOLES = 1 << 6,
    CVG_LE_PROFANITY = 1 << 7,
    CVG_LE_NON_UNIQUE_EXISTING_VILLAINS = 1 << 8,
}CVG_LoadErrors;

void CVG_clearVillains(CustomVG *cvg);
int compareCritterGroups(const void* aData, const void* bData);
int CVG_isValidCVG(CustomVG *cvg, Entity *e, int fixup, int *playableErrorCode);
int CVG_CVGExists(char *cvgName);
int CVG_isValidRank(int rank, int isCritter);
int cvg_randomVillainDefRankMatch(int rank);
void CVG_getLevelsFromCompressedVG(CompressedCVG *cvg, int *minLevel, int *maxLevel, PCC_Critter **pccList);
void CVG_getLevelsFromUncompressedVG(CustomVG *cvg, int *minLevel, int *maxLevel);
extern TokenizerParseInfo parse_CustomVG[];
extern TokenizerParseInfo parse_ExistingVillain[];
extern TokenizerParseInfo parse_CompressedCVG[];
#endif