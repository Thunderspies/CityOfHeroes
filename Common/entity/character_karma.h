#ifndef CHARACTER_KARMA_H__
#define CHARACTER_KARMA_H__

#include <utilitieslib/utils/structDefines.h>

typedef struct Character Character;
typedef struct Entity Entity;
typedef struct BasePower BasePower;

typedef enum CharacterKarmaReason
{
    CKR_POWER_CLICK,
    CKR_BUFF_POWER_CLICK,
    CKR_ALL_OBJ_COMPLETE,
    CKR_TEAM_OBJ_COMPLETE,
    CKR_COUNT,
}CharacterKarmaReason;

typedef enum CharacterKarmaVars
{
    CKV_AMOUNT,
    CKV_LIFESPAN,
    CKV_STACK_LIMIT,
    CKV_COUNT,
}CharacterKarmaVars;

typedef struct ZoneEventKarmaVarTable
{
    int karmaVar[CKV_COUNT];
}ZoneEventKarmaVarTable;

extern StaticDefineInt CharacterKarmaReasonText [];

typedef struct CharacterKarma
{
    int karmaAmount;
    U32 timeStamp;
    int lifespan;
    int stackId;
}CharacterKarma;
typedef struct CharacterKarmaBucket
{
    CharacterKarma **charKarma;
}CharacterKarmaBucket;
typedef struct CharacterKarmaContainer
{
    CharacterKarmaBucket karmaBucket[CKR_COUNT];
    U32 bubbleActivatedTimestamp;
    U32 glowieBubbleActivatedTimestamp;
    int numNearbyBubbles;
    int inZoneEvent;
}CharacterKarmaContainer;

typedef struct ZoneEventKarmaClassMod
{
    char *pchClassName;
    F32 classMod;
}ZoneEventKarmaClassMod;
typedef struct ZoneEventKarmaTable
{
    ZoneEventKarmaVarTable karmaReasonTable[CKR_COUNT];
    ZoneEventKarmaClassMod **ppClassMod;
    int powerLifespanCap;        //    max life of a power
    int powerLifespanConst;    //    divisor in the power equation
    int powerStackModifier;    //    exponent of power stacking
    int powerBubbleDur;
    int powerBubbleRad;
    U32 pardonDuration;        //    duration is the duration that a pardon can save you from
    U32 pardonGrace;        //    grace is time before stats can be added
    int activeDuration;
}ZoneEventKarmaTable;

extern SHARED_MEMORY ZoneEventKarmaTable g_ZoneEventKarmaTable;
void CharacterAddKarma(Character *pchar, int reason, U32 currentTime);
void CharacterAddKarmaEx(Character *pchar, int reason, U32 currentTime, int amount, int lifespan);
void CharacterAddPowerKarma(Character *pchar, U32 currentTime, const BasePower *power);
void CharacterKarmaInit(Character *pchar);
void CharacterDestroyKarmaBuckets(Character *pchar);
int CharacterCalculateKarmaBucket(Character *pchar, int i, U32 currentTime);
int CharacterCalculateKarma(Character *pchar, U32 currentTime);
void CharacterKarmaStatsSend(Entity *e, U32 currentTime);
int CharacterKarma_SetKarmaValue(char *reasonStr, char *varStr, int newKarmaValue);
void CharacterKarmaActivateBubble(Character *pchar, U32 currentTime);
CharacterKarmaBucket* GetUpdatedBucket(Character *pchar, int reason, U32 currentTime);
#endif