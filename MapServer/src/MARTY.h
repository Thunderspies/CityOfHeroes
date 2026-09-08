#ifndef MARTY_H
#define MARTY_H

#include <utilitieslib/utils/structDefines.h>

typedef struct Entity Entity;

#define MAX_MARTY_EXP_MINUTE_RECORDS    120
#define MAX_REWARD_THROTTLE_TRACKS 2

typedef enum MARTY_ExperienceType
{
    MARTY_ExperienceArchitect,
    MARTY_ExperienceCombined,
    MARTY_ExperienceTypeCount,
}MARTY_ExperienceType;

typedef struct MARTY_RewardData
{
    int            expMinuteAccum;
    int            expCurrentMinute;
    U32            expCircleBuffer[MAX_MARTY_EXP_MINUTE_RECORDS];
    int            expRingBufferLocation;
    int            exp5MinSum;
    int            exp15MinSum;
    int            exp30MinSum;
    int            exp60MinSum;
    int            exp120MinSum;
    int            expThrottled;
}MARTY_RewardData;
typedef struct MARTYLevelMods
{
    int level;                //    base 0
    int    expGain1Min;
    int    expGain5Min;
    int    expGain15Min;
    int    expGain30Min;
    int    expGain60Min;
    int    expGain120Min;
}MARTYLevelMods;
typedef struct MARTYExpMods
{
    MARTYLevelMods **perLevelThrottles;
    int action;
    int expType;
    char *logAppendMsg;
}MARTYExpMods;
typedef struct MARTYLevelupShifts
{
    int level;
    float mod;
}MARTYLevelupShifts;
typedef struct MARTYMods
{
    MARTYExpMods **perActionType;
    MARTYLevelupShifts **levelupShifts;
}MARTYMods;
extern MARTYMods g_MARTYMods;
extern StaticDefineInt ParseMARTYExperienceTypes[];
extern StaticDefineInt ParseMARTYActions[];
void MARTY_Tick(Entity *e);
void validateMARTYMods(MARTYMods *mods, char *filename);
void clearMARTYHistory(Entity *e);
int displayMARTYSum(Entity *e, int experienceType, int interval);
void MARTY_NormalizeExpHistory(Entity *e, int oldLevel);
#endif