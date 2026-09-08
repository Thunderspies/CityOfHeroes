/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CHARACTER_DB_H__
#define CHARACTER_DB_H__

#include "entity/boost.h"
#include "entity/character_base.h"

typedef struct StructDesc StructDesc;
typedef struct StuffBuff StuffBuff;

#define MAX_DB_BOOSTS ((MAX_POWERS*6)+10)
    // The maximum number of boosts a player can have. Not used in the
    // code except in containerloadsave (the database interface).
    // +10 for the inventory

#define MAX_DB_INSPIRATIONS 20
    // The maximum number of inspirations a player can have. Not used in the
    // code except in containerloadsave (the database interface).

#define MAX_DB_ATTRIB_MODS 1000
    // The maximum number of attribs a player can have active. Not used in the
    // code except in containerloadsave (the database interface).

#define MAX_DB_POWER_VARS 4
    // The maximum number of vars stored for a power. If you change this 
    // number you will need to change the LineDescs in containerloadsave as 
    // well.

/***************************************************************************/
/***************************************************************************/

typedef struct DBBasePower
{
    const char *pchPowerName;
    const char *pchPowerSetName;
    const char *pchCategoryName;
} DBBasePower;

typedef struct DBRewardSlot
{ 
    RewardItemType type;
    int id;
} DBRewardSlot;

bool dbrewardslot_Valid(DBRewardSlot *slot);


typedef struct DBPower
{
    DBBasePower dbpowBase;

    int iUniqueID; // identifier that's unique against all other powers on the same Character
    int iID; // index into the powerset's power array, not reliable if power array changes

    int iLevelBought;
    int iNumBoostsBought;
    int iNumCharges;
    float fUsageTime;
    float fAvailableTime;
    U32 ulCreationTime;
    U32 ulRechargedAt;

    int bActive;

    int iPowerSetLevelBought;

    int afVars[MAX_DB_POWER_VARS];

    // Zero based build number this lives in, or -1 if shared
    int iBuildNum;

    // describes the slotted powerups
    int iSlottedPowerupsMask;
    DBRewardSlot aPowerupSlots[POWERUP_COST_SLOTS_MAX_COUNT];

    int bDisabled;
    int instanceBought;
} DBPower;

typedef struct DBBoost
{
    DBBasePower dbpowBase;

    int iID;

    int iLevel;
    int iNumCombines;
    int iIdx;

    int afVars[MAX_DB_POWER_VARS];

    // describes the slotted powerups
    int iSlottedPowerupsMask;
    DBRewardSlot aPowerupSlots[POWERUP_COST_SLOTS_MAX_COUNT];
} DBBoost;

typedef struct DBInspiration
{
    DBBasePower dbpowBase;
    int iCol;
    int iRow;

} DBInspiration;

typedef struct DBAttribMod
{
    DBBasePower dbpowBase;
    int iIdxTemplate;

    float fDuration;
    float fTimer;
    float fMagnitude;
    float fChance;
    int UID;
    int petFlags;
    const char *customFXToken;
    ColorPair uiTint;
    bool bSuppressedByStacking;
} DBAttribMod;

typedef struct DBPowers
{
    DBPower       powers[MAX_POWERS];
    DBBoost       boosts[MAX_DB_BOOSTS];
    DBInspiration inspirations[MAX_DB_INSPIRATIONS];
    DBAttribMod   attribmods[MAX_DB_ATTRIB_MODS];
} DBPowers;
typedef enum DB_RestoreAttrib_Types
{
    eRAT_ClearAll,
    eRAT_ClearAbusiveBuffs,
    eRAT_RestoreAll,
    eRAT_Count,
}DB_RestoreAttrib_Types;
bool unpackEntPowers(Entity *e, DBPowers *pdbpows, DB_RestoreAttrib_Types eRestoreAttrib, char *pchLogPrefix);
void packageEntPowers(Entity *e, StuffBuff *psb, StructDesc *desc);

const char *CharacterClassToAttr(intptr_t num);
intptr_t AttrToCharacterClass(const char *pch);
const char *CharacterOriginToAttr(intptr_t num);
intptr_t AttrToCharacterOrigin(const char *pch);


#endif /* #ifndef CHARACTER_DB_H__ */

/* End of File */

