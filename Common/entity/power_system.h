/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef POWER_SYSTEM_H__
#define POWER_SYSTEM_H__

#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/utils/textparser_types.h>

typedef enum PowerSystem
{
    // Which power system to use for advancement, level lookup, etc.

    kPowerSystem_Powers = 0,
    kPowerSystem_Count
} PowerSystem;

#define kPowerSystem_Numbits 1


/***************************************************************************/
/***************************************************************************/

typedef struct Schedule
{
    const int *piFreeBoostSlotsOnPower;
        // Defines when free boost slots are opened on a power. It is based on
        // how many security levels a character has owned a particular power.

    const int *piPoolPowerSet;
        // Defines when the character gets a new pool power set.

    const int *piEpicPowerSet;
        // Defines when the character gets a new epic power set.

    const int *piPower;
        // Defines when the player can buy a new power. Based on security level.

    const int *piAssignableBoost;
        // Defines when the player can buy a new boost. Based on security level.

    const int *piInspirationCol;
        // Defines when inspiration column opens up

    const int *piInspirationRow;
        // Defines when inspiration row opens up

    const int *piFreeExpertiseSlotsOnSkill;
        // Defines when free expertise slots are opened on a skill. It is 
        // based on how many skill levels a character has owned a particular 
        // skill.

    const int *piSkill;
        // Defines when the player can buy a new skill. Based on skill level.

    const int *piAssignableExpertise;
        // Defines when the player can buy a new expertise slot. Based on 
        // skill level.

} Schedule;


typedef struct Schedules
{
    Schedule aSchedules;
} Schedules;

/***************************************************************************/
/***************************************************************************/

typedef struct ExperienceTable
{
    const int *piRequired;
    const int *piDefeatPenalty;
} ExperienceTable;

typedef struct ExperienceTables
{
    ExperienceTable aTables;
} ExperienceTables;

/***************************************************************************/
/***************************************************************************/

extern StaticDefineInt PowerSystemEnum[];
extern TokenizerParseInfo ParseSchedules[];
extern TokenizerParseInfo ParseExperienceTables[];

/***************************************************************************/
/***************************************************************************/

extern SHARED_MEMORY Schedules g_Schedules;
extern SHARED_MEMORY ExperienceTables g_ExperienceTables;

/***************************************************************************/
/***************************************************************************/

//
// Define the max level for each power system
// TODO: Read from file or derive it?
//
extern int g_aiMaxLevels[2][2];

#define MAX_PLAYER_LEVEL MAX_PLAYER_SECURITY_LEVEL

#define MAX_PLAYER_SECURITY_LEVEL 50 
#define MAX_PLAYER_WISDOM_LEVEL 50 


// Define the minimum security level required to use a system
extern int g_aiRequiredSecurityLevel[];
#define REQUIRED_SECURITY_LEVEL(system) g_aiRequiredSecurityLevel[system]

/***************************************************************************/
/***************************************************************************/


int CountForLevel(int iLevel, const int *piSchedule);
    // Given a schedule and a level, calculates how many <whatever> are
    //   available at that level.


#endif /* #ifndef POWER_SYSTEM_H__ */

/* End of File */

