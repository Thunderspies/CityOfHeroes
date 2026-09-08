/*\
 *
 *    taskdef.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles static task definitions and initialization checks
 *    on these
 *
 */

#include "task.h"
#include "storyarcprivate.h"
#include "Reward.h"
#include "dbcomm/staticMapInfo.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "svr/svr_player.h" // for reloading tasksets
#include "entity/character_eval.h"

// *********************************************************************************
//  Parse definitions
// *********************************************************************************

// should be updated along with TaskState enum
char* g_taskStateNames[] =
{
    "No state",          // TASK_NONE
    "Assigned",          // TASK_ASSIGNED
    "Marked Success", //TASK_MARKED_SUCCESS,
    "Marked Failure", //TASK_MARKED_FAILURE
    "Succeeded",      // TASK_SUCCEEDED
    "Failed",          // TASK_FAILED
};

StaticDefineInt ParseTaskType[] = 
{
    DEFINE_INT
    { "taskMission",            TASK_MISSION },
    { "taskRandomEncounter",    TASK_RANDOMENCOUNTER },
    { "taskRandomNPC",            TASK_RANDOMNPC },
    { "taskKillX",                TASK_KILLX },
    { "taskDeliverItem",        TASK_DELIVERITEM },
    { "taskVisitLocation",        TASK_VISITLOCATION },
    { "taskCraftInvention",        TASK_CRAFTINVENTION },
    { "taskCompound",            TASK_COMPOUND },
    { "taskContactIntroduction",TASK_CONTACTINTRO },
    { "taskTokenCount",            TASK_TOKENCOUNT },
    { "taskZowie",                TASK_ZOWIE },
    { "taskGoToVolume",            TASK_GOTOVOLUME },
    DEFINE_END
};

StaticDefineInt ParseNewspaperType[] = 
{
    DEFINE_INT
    { "GetItem",                NEWSPAPER_GETITEM },
    { "Hostage",                NEWSPAPER_HOSTAGE },
    { "Defeat",                    NEWSPAPER_DEFEAT },
    { "Heist",                    NEWSPAPER_HEIST },
    DEFINE_END
};

StaticDefineInt ParseDeliveryMethod[] =
{
    DEFINE_INT
    { "Any",                    DELIVERYMETHOD_ANY },
    { "CellOnly",                DELIVERYMETHOD_CELLONLY },
    { "InPersonOnly",            DELIVERYMETHOD_INPERSONONLY },
    DEFINE_END
};

StaticDefineInt ParsePvPType[] = 
{
    DEFINE_INT
    { "Patrol",                    PVP_PATROL },
    { "HeroDamage",                PVP_HERODAMAGE },
    { "HeroResist",                PVP_HERORESIST },
    { "VillainDamage",            PVP_VILLAINDAMAGE },
    { "VillainResist",            PVP_VILLAINRESIST },
    DEFINE_END
};

StaticDefineInt ParseTaskAlliance[] = 
{
    DEFINE_INT
    { "Both",                    SA_ALLIANCE_BOTH            },
    { "Hero",                    SA_ALLIANCE_HERO            },
    { "Villain",                SA_ALLIANCE_VILLAIN            },
    DEFINE_END
};

StaticDefineInt ParseTaskFlag[] = 
{
    DEFINE_INT
    { "taskRequired",            TASK_REQUIRED },
    { "taskNotRequired",        0 },
    { "taskRepeatable",            TASK_REPEATONFAILURE },
    { "taskNotRepeatable",        0 },
    { "taskRepeatOnFailure",    TASK_REPEATONFAILURE },
    { "taskRepeatOnSuccess",    TASK_REPEATONSUCCESS },
    { "taskUrgent",                TASK_URGENT },    // urgent tasks will be given out before any other tasks
    { "taskNotUrgent",            0 },
    { "taskUnique",                TASK_UNIQUE },    // unique tasks will not be given out twice, even across contact sets
    { "taskNotUnique",            0 },
    { "taskLong",                TASK_LONG },
    { "taskShort",                TASK_SHORT },    // contact tasks should be marked as one of these 2 types
    { "taskDontReturnToContact",TASK_DONTRETURNTOCONTACT },
    { "taskHeist",                TASK_HEIST },
    { "taskNoTeamComplete",        TASK_NOTEAMCOMPLETE },
    { "taskSupergroup",            TASK_SUPERGROUP | TASK_DONTRETURNTOCONTACT }, // Supergroup missions dont return to contact
    { "taskEnforceTimeLimit",    TASK_ENFORCETIMELIMIT },    // Task will still boot you from a mission even when completed
    { "taskDelayedTimerStart",    TASK_DELAYEDTIMERSTART },    // Task timer does not start until entering the mission
    { "taskNoAbort",            TASK_NOAUTOCOMPLETE },        // This task cannot be auto-completed (was called aborted)
    { "taskNoAutoComplete",        TASK_NOAUTOCOMPLETE },        // This task cannot be auto-completed
    { "taskFlashback",            TASK_FLASHBACK },            // This task is available through the flashback system
    { "taskFlashbackOnly",        TASK_FLASHBACK_ONLY },        // This task is only available through the flashback system
    { "taskPlayerCreated",        TASK_PLAYERCREATED },        // This task was created by a player
    { "taskAbandonable",        TASK_ABANDONABLE },            // Deprecated
    { "taskNotAbandonable",        TASK_NOT_ABANDONABLE },        // This task cannot be abandoned and picked up again later
    { "taskAutoIssue",            TASK_AUTO_ISSUE },
    DEFINE_END
};

StaticDefineInt ParseTaskTokenFlag[] = 
{
    DEFINE_INT
    { "resetAtStart",            TOKEN_FLAG_RESET_AT_START }, // reset token count at start of mission
    DEFINE_END
};

StaticDefineInt ParseRewardSoundChannel[] = 
{
    DEFINE_INT
    { "Music",                    SOUND_MUSIC        },
    { "SFX",                    SOUND_GAME        },
    { "Voiceover",                SOUND_VOICEOVER    },
    DEFINE_END
};

TokenizerParseInfo ParseStoryReward[] = 
{
    { "{",                    TOK_START,        0},
    { "Dialog",                TOK_STRING(StoryReward,rewardDialog,0) },
    { "SOLDialog",            TOK_STRING(StoryReward,SOLdialog,0) },
    { "Caption",            TOK_STRING(StoryReward,caption,0) },
    { "AddClue",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,addclues) },
    { "AddClues",            TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,addclues) },
    { "RemoveClue",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,removeclues) },
    { "RemoveClues",        TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,removeclues) },
    { "AddToken",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,addTokens) },
    { "AddTokens",            TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,addTokens) },
    { "AddTokenToAll",        TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,addTokensToAll) },
    { "AddTokensToAll",        TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,addTokensToAll) },
    { "RemoveToken",        TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,removeTokens) },
    { "RemoveTokens",        TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,removeTokens) },
    { "RemoveTokenFromAll",    TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,removeTokensFromAll) },
    { "RemoveTokensFromAll",TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,removeTokensFromAll) },
    { "PopHelp",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,popHelp) },
    { "AbandonContacts",    TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,abandonContacts) },
    { "PlaySound",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,rewardSoundName) },
    { "PlaySoundChannel",    TOK_INTARRAY(StoryReward,rewardSoundChannel), ParseRewardSoundChannel},
    { "PlaySoundVolumeLevel",    TOK_F32ARRAY(StoryReward,rewardSoundVolumeLevel) },
    { "FadeSoundChannel",    TOK_INT(StoryReward,rewardSoundFadeChannel,0), ParseRewardSoundChannel},
    { "FadeSoundTime",        TOK_F32(StoryReward,rewardSoundFadeTime,-1.0f) },
    { "PlayFXOnPlayer",        TOK_STRING(StoryReward,rewardPlayFXOnPlayer,0) },
    { "ContactPoints",        TOK_INT(StoryReward,contactPoints,0) },
    { "CostumeSlot",        TOK_INT(StoryReward,costumeSlot,0) },
    { "PrimaryReward",         TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,primaryReward) },
    { "SecondaryReward",    TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,secondaryReward) },
    { "Chance",                TOK_STRUCT(StoryReward,rewardSets,ParseRewardSet) },
    { "NewContactReward",    TOK_INT(StoryReward,newContactReward,0) },
    { "RandomFameString",    TOK_STRING(StoryReward,famestring,0) },
    { "SouvenirClue",        TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryReward,souvenirClues) },
    { "BadgeStat",            TOK_NO_TRANSLATE | TOK_STRING(StoryReward,badgeStat,0) },
    { "BonusTime",            TOK_INT(StoryReward,bonusTime,0) },
    { "FloatText",            TOK_STRING(StoryReward,floatText,0) },
    { "ArchitectBadgeStat",            TOK_STRING(StoryReward,architectBadgeStat,0) },
    { "ArchitectBadgeStatTest",        TOK_STRING(StoryReward,architectBadgeStatTest,0) },
    { "}",                    TOK_END,            0},
    { "", 0, 0 }
};

extern StaticDefineInt ParseVillainType[];

TokenizerParseInfo ParseStoryTask[] = 
{
    { "", TOK_STRUCTPARAM | TOK_INT(StoryTask,type,    0),    ParseTaskType },
    { "", TOK_STRUCTPARAM | TOK_FLAGS(StoryTask,flags,    0),    ParseTaskFlag },
    { "",                TOK_CURRENTFILE(StoryTask,filename) },
    { "{",                TOK_START,        0},
    { "Name",            TOK_NO_TRANSLATE | TOK_STRING(StoryTask,logicalname,0) },
    { "Deprecated",        TOK_BOOLFLAG(StoryTask,deprecated,0), },
//    { "Mission",        TOK_FILENAME(StoryTask,missionfile,0) },
    { "MissionDef",        TOK_STRUCT(StoryTask,missiondef,ParseMissionDef) },
    { "MissionEditor",    TOK_FILENAME(StoryTask,missioneditorfile,0), },
    { "Spawn",            TOK_FILENAME(StoryTask,spawnfile,0) },
    { "SpawnDef",        TOK_STRUCT(StoryTask,spawndefs,ParseSpawnDef)    },
    { "ClueDef",        TOK_STRUCT(StoryTask,clues,ParseStoryClue) },
    { "VillainGroup",    TOK_INT(StoryTask,villainGroup,0),    ParseVillainGroupEnum },
    { "VillainGroupType",TOK_INT(StoryTask,villainGroupType,0),    ParseVillainType },
    { "TaskFlags",        TOK_REDUNDANTNAME | TOK_FLAGS(StoryTask,flags,0),    ParseTaskFlag },
    { "Dialog",            TOK_STRUCT(StoryTask, dialogDefs, ParseDialogDef)    },
    { "MaxPlayers",        TOK_INT(StoryTask,maxPlayers,0) },
    
    // strings
    { "IntroString",                TOK_STRING(StoryTask,detail_intro_text,0) },
    { "SOLIntroString",                TOK_STRING(StoryTask,SOLdetail_intro_text,0) },
    { "HeadlineString",                TOK_STRING(StoryTask,headline_text,0) },
    { "AcceptString",                TOK_STRING(StoryTask,yes_text,0) },
    { "DeclineString",                TOK_STRING(StoryTask,no_text,0) },
    { "SendoffString",                TOK_STRING(StoryTask,sendoff_text,0) },
    { "SOLSendoffString",            TOK_STRING(StoryTask,SOLsendoff_text,0) },
    { "DeclineSendoffString",        TOK_STRING(StoryTask,decline_sendoff_text,0) },
    { "SOLDeclineSendoffString",    TOK_STRING(StoryTask,SOLdecline_sendoff_text,0) },
    { "YoureStillBusyString",        TOK_STRING(StoryTask,busy_text,0) },
    { "SOLYoureStillBusyString",    TOK_STRING(StoryTask,SOLbusy_text,0) },
    { "ActiveTaskString",            TOK_STRING(StoryTask,inprogress_description,0) },
    { "TaskCompleteDescription",    TOK_STRING(StoryTask,taskComplete_description,0) },
    { "TaskFailedDescription",        TOK_STRING(StoryTask,taskFailed_description,0) },
    { "TaskAbandonedString",        TOK_STRING(StoryTask,task_abandoned,0) },

    { "TaskTitle",                    TOK_STRING(StoryTask,taskTitle,0) },
    { "TaskSubTitle",                TOK_STRING(StoryTask,taskSubTitle,0) },
    { "TaskIssueTitle",                TOK_STRING(StoryTask,taskIssueTitle,0) },
    { "FlashbackDescription",        TOK_STRING(StoryTask,flashbackDescription,0) },
    { "Alliance",                    TOK_INT(StoryTask,alliance, SA_ALLIANCE_BOTH), ParseTaskAlliance },
    
    { "CompleteRequires",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,completeRequires)},
    { "FlashbackRequires",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,flashbackRequires)},
    { "FlashbackRequiresFailedText",TOK_STRING(StoryTask, flashbackRequiresFailed, 0) },
     { "FlashbackTeamRequires",        TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask, flashbackTeamRequires) },
     { "FlashbackTeamRequiresFailedText",    TOK_STRING(StoryTask, flashbackTeamFailed, 0) },
    { "Requires",                    TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,assignRequires)},
    { "RequiresFailedText",            TOK_STRING(StoryTask,assignRequiresFailed,0) },
    { "TeamRequires",                TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask, teamRequires) },
    { "TeamRequiresFailedText",        TOK_STRING(StoryTask, teamRequiresFailed, 0) },
    { "AutoIssueRequires",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask, autoIssueRequires) },

    // other fields
    { "MinPlayerLevel",    TOK_INT(StoryTask,minPlayerLevel,    1) },
    { "MaxPlayerLevel", TOK_INT(StoryTask,maxPlayerLevel,    MAX_PLAYER_SECURITY_LEVEL) },
    { "TimeoutMinutes",    TOK_INT(StoryTask,timeoutMins, 0) },
    { "ForceTaskLevel", TOK_INT(StoryTask,forceTaskLevel, 0) },

    // Special Type for Newspapers and PvPTasks
    { "PvPType",        TOK_INT(StoryTask,pvpType,        PVP_NOTYPE), ParsePvPType },
    { "NewspaperType",    TOK_INT(StoryTask,newspaperType,    NEWSPAPER_NOTYPE), ParseNewspaperType },

    // Task location
    { "LocationName",    TOK_NO_TRANSLATE | TOK_STRING(StoryTask, taskLocationName, 0) },
    { "LocationMap",    TOK_NO_TRANSLATE | TOK_STRING(StoryTask, taskLocationMap, 0) },

    // Kill task
    { "VillainType",        TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainType,0) },
    { "VillainCount",        TOK_INT(StoryTask,villainCount,0) },
    { "VillainMap",            TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainMap,0) },
    { "VillainNeighborhood", TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainNeighborhood,0) },
    { "VillainVolumeName",    TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainVolumeName,0) },
    { "VillainDescription", TOK_STRING(StoryTask,villainDescription,0) },
    { "VillainSingularDescription", TOK_STRING(StoryTask,villainSingularDescription,0) },
    { "VillainType2",        TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainType2,0) },
    { "VillainCount2",        TOK_INT(StoryTask,villainCount2,0) },
    { "VillainMap2",        TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainMap2,0) },
    { "VillainNeighborhood2", TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainNeighborhood2,0) },
    { "VillainVolumeName2",    TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainVolumeName2,0) },
    { "VillainDescription2", TOK_STRING(StoryTask,villainDescription2,0) },
    { "VillainSingularDescription2", TOK_STRING(StoryTask,villainSingularDescription2,0) },

    // Visit location task
    { "VisitLocationName",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,visitLocationNames)},

    // Delivery Item task
    { "DeliveryTargetName",        TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,deliveryTargetNames) },
    { "DeliveryDialog",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,deliveryTargetDialogs) },
    { "DeliveryDialogStartPage",TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryTask,deliveryTargetDialogStarts) },
    { "DeliveryMethod",            TOK_INTARRAY(StoryTask,deliveryTargetMethods), ParseDeliveryMethod },

    // Craft invention task,
    { "InventionName",            TOK_NO_TRANSLATE | TOK_STRING(StoryTask,craftInventionName,0) },

    // Token Count task,
    { "TokenName",                TOK_NO_TRANSLATE | TOK_STRING(StoryTask,tokenName,0) },
    { "TokenCount",                TOK_INT(StoryTask,tokenCount,0) },
    { "TokenProgressString",    TOK_STRING(StoryTask,tokenProgressString,0) },
    { "TokenFlags",                TOK_FLAGS(StoryTask,tokenFlags,0),    ParseTaskTokenFlag },

    // Zowie task
    { "ZowieType",                TOK_NO_TRANSLATE | TOK_STRING(StoryTask,zowieType,0) },
    { "ZowiePoolSize",            TOK_NO_TRANSLATE | TOK_STRING(StoryTask,zowiePoolSize,0) },
    { "ZowieDisplayName",        TOK_STRING(StoryTask,zowieDisplayName,0) },
    { "ZowieCount",                TOK_INT(StoryTask,zowieCount,0) },
    { "ZowieDescription",        TOK_STRING(StoryTask,zowieDescription,0) },
    { "ZowieSingularDescription", TOK_STRING(StoryTask,zowieSingularDescription,0) },
    { "ZowieMap",                TOK_NO_TRANSLATE | TOK_STRING(StoryTask,villainMap,0) },

    { "InteractDelay",            TOK_INT(StoryTask,interactDelay,0) },
    { "InteractBeginString",    TOK_STRING(StoryTask,interactBeginString,0) },
    { "InteractCompleteString", TOK_STRING(StoryTask,interactCompleteString,0) },
    { "InteractInterruptedString", TOK_STRING(StoryTask,interactInterruptedString,0) },
    { "InteractActionString",    TOK_STRING(StoryTask,interactActionString,0) },

    // Go To Volume task
    { "VolumeMapName",            TOK_STRING(StoryTask,volumeMapName,0) },
    { "VolumeName",                TOK_STRING(StoryTask,volumeName,0) },
    { "VolumeDescription",        TOK_STRING(StoryTask,volumeDescription,0) },

    // Compound task
    { "TaskDef",                TOK_STRUCT(StoryTask,children, ParseStoryTask) },

    // reward times
    { "TaskBegin",                TOK_STRUCT(StoryTask,taskBegin,ParseStoryReward) },
    { "TaskBeginFlashback",        TOK_STRUCT(StoryTask,taskBeginFlashback,ParseStoryReward) },
    { "TaskBeginNoFlashback",    TOK_STRUCT(StoryTask,taskBeginNoFlashback,ParseStoryReward) },
    { "TaskSuccess",            TOK_STRUCT(StoryTask,taskSuccess,ParseStoryReward) },
    { "TaskSuccessFlashback",    TOK_STRUCT(StoryTask,taskSuccessFlashback,ParseStoryReward) },
    { "TaskSuccessNoFlashback",    TOK_STRUCT(StoryTask,taskSuccessNoFlashback,ParseStoryReward) },
    { "TaskFailure",            TOK_STRUCT(StoryTask,taskFailure,ParseStoryReward) },
    { "TaskAbandon",            TOK_STRUCT(StoryTask,taskAbandon,ParseStoryReward) },
    { "ReturnSuccess",            TOK_STRUCT(StoryTask,returnSuccess,ParseStoryReward) },
    { "ReturnSuccessFlashback",    TOK_STRUCT(StoryTask,returnSuccessFlashback,ParseStoryReward) },
    { "ReturnSuccessNoFlashback",    TOK_STRUCT(StoryTask,returnSuccessNoFlashback,ParseStoryReward) },
    { "ReturnFailure",            TOK_STRUCT(StoryTask,returnFailure,ParseStoryReward) },
    { "TaskComplete",            TOK_REDUNDANTNAME | TOK_STRUCT(StoryTask,taskSuccess,ParseStoryReward) },

    { "}",                        TOK_END,            0 },
    SCRIPTVARS_STD_PARSE(StoryTask)
    { "", 0, 0 }
};

TokenizerParseInfo ParseStoryTaskSet[] = {
    { "",                TOK_CURRENTFILE(StoryTaskSet,filename) },
    { "{",                TOK_START,        0},
    { "TaskDef",        TOK_STRUCT(StoryTaskSet,tasks,ParseStoryTask) },
    { "}",                TOK_END,            0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseStoryTaskSetList[] = {
    { "TaskSet",        TOK_STRUCT(StoryTaskSetList,sets,ParseStoryTaskSet) },
    { "", 0, 0 }
};

SHARED_MEMORY StoryTaskSetList g_tasksets = {0};


// *********************************************************************************
//  Static def loading & verification checks
// *********************************************************************************

static int VerifySouvenirClueNames(const char** clueNames, const char* filename)
{
    int i;
    int result = 1;

    if(!clueNames)
        return result;

    for(i = 0; i < eaSize(&clueNames); i++)
    {
        if(!scFindByName(clueNames[i]))
        {
            ErrorFilenamef(filename, "Task referenced invalid souvenir clue \"%s\"", clueNames[i]);
            result = 0;
        }
    }

    return result;
}

int VerifyRewardBlock(const StoryReward *reward, const char *rewardName, const char *filename, int allowSuperGroupName, int allowLastContactName, int allowRescuer)
{
    int returnMe = 1;
    int playSoundCount = eaSize(&reward->rewardSoundName);
    int playSoundChannelCount = eaiSize(&reward->rewardSoundChannel);
    int playSoundVolumeCount = eafSize(&reward->rewardSoundVolumeLevel);

    if (!rewardSetsVerifyAndProcess(cpp_const_cast(RewardSet**)(reward->rewardSets)))
        returnMe = 0;

    if (reward->rewardDialog)
    {
        if(strnicmp(rewardName, "return", 6) == 0)
            saUtilVerifyContactParams(reward->rewardDialog, filename, 1, allowSuperGroupName, 1, 0, allowLastContactName, allowRescuer); // only owners should see this when online
        else
            saUtilVerifyContactParams(reward->rewardDialog, filename, 1, allowSuperGroupName, 0, 0, allowLastContactName, allowRescuer); // owner many not be online
    }

    VerifySouvenirClueNames(reward->souvenirClues, filename);

    if (playSoundCount < playSoundChannelCount)
    {
        ErrorFilenamef(filename, "Reward Block \"%s\" has too many PlaySoundChannel entries", rewardName);
        returnMe = 0;
    }

    if (playSoundCount < playSoundVolumeCount)
    {
        ErrorFilenamef(filename, "Reward Block \"%s\" has too many PlaySoundVolumeLevel entries", rewardName);
        returnMe = 0;
    }

    return returnMe;
}

// called by TaskPreprocess and recursively
int TaskPreprocessInternal(StoryTask* task, const char* filename, int child, int allowSuperGroupName, int inStoryArc)
{
    int ret = 1;

    // do string modifications first
    if (task->villainNeighborhood)
        saUtilReplaceChars(cpp_const_cast(char*)(task->villainNeighborhood), '_', ' ');
    if (task->villainNeighborhood2)
        saUtilReplaceChars(cpp_const_cast(char*)(task->villainNeighborhood2), '_', ' ');

    // Not much we can do here because of the merging, rethink later perhaps
    if (task->missioneditorfile)
        return 1;

    // ignore restrictions if task is deprecated
    if (task->deprecated) return 1;

    // random encounters don't need the usual contact strings
    if (task->type == TASK_RANDOMENCOUNTER || task->type == TASK_RANDOMNPC)
        return 1;

    // check restrictions on unique tasks
    if (TASK_IS_UNIQUE(task))
    {
        if (inStoryArc)
        {
            ErrorFilenamef(filename, "Storyarc task cannot be unique");
            ret = 0;
            task->flags &= ~TASK_UNIQUE;
        }
        else if (child)
        {
            ErrorFilenamef(filename, "Child task cannot be unique");
            ret = 0;
            task->flags &= ~TASK_UNIQUE;
        }
        else if (!task->logicalname)
        {
            ErrorFilenamef(filename, "Unique task requires logical name");
            ret = 0;
            task->flags &= ~TASK_UNIQUE;
        }
    }

    // repeatable flag
    if (TASK_CAN_REPEATONSUCCESS(task) || TASK_CAN_REPEATONFAILURE(task))
    {
        if (child)
        {
            ErrorFilenamef(filename, "Child task cannot be repeatable");
            ret = 0;
            task->flags &= ~TASK_REPEATONSUCCESS;
            task->flags &= ~TASK_REPEATONFAILURE;
        }
    }

    // Heist needs decline and decline send off strings
    if (TASK_IS_HEIST(task))
    {
        if (!task->no_text)
        { ErrorFilenamef(filename, "Heist Task is missing decline text\n"); ret = 0; }
        if (!task->decline_sendoff_text)
        { ErrorFilenamef(filename, "Heist Task is missing decline send off text\n"); ret = 0; }
    }

    // need basic send text if we are not a child task
    if (!child)
    {
        if (!task->detail_intro_text)
        { ErrorFilenamef(filename, "Task is missing detailed intro text"); ret = 0; }
        if (!task->yes_text)
        { ErrorFilenamef(filename, "Task is missing accept text"); ret = 0; }
        if (!task->sendoff_text)
        { ErrorFilenamef(filename, "Task is missing send text"); ret = 0; }
    }

    // don't need in progress text or busy text if we are compound
    if (task->type != TASK_COMPOUND)
    {
        if (!task->inprogress_description)
        { ErrorFilenamef(filename, "Task is missing in progress text"); ret = 0; }
        if (!task->busy_text)
        { ErrorFilenamef(filename, "Task is missing busy text"); ret = 0; }
    }

    // check task children
    if (task->type != TASK_COMPOUND) // only compound tasks can have children
    {
        if (task->children)
        { ErrorFilenamef(filename, "Task has children but is not a compound task"); ret = 0; }
    }
    else if (child) // only one level of children allowed
    {
        if (task->children)
        { ErrorFilenamef(filename, "Child task has more children"); ret = 0; }
    }
    else // task is compound
    {
        int i, n;
        n = eaSize(&task->children);
        if (n < 2)
        { ErrorFilenamef(filename, "Compound task doesn't have 2 or more children"); ret = 0; }
        for (i = 0; i < n; i++)
        {
            if (!TaskPreprocessInternal(cpp_const_cast(StoryTask*)(task->children[i]), filename, 1, allowSuperGroupName, inStoryArc))
                ret = 0;
        }
    }

    // children should not have several fields
    if (child)
    {
        if (task->deprecated)
        { ErrorFilenamef(filename, "A child task was marked as deprecated (mark parent instead)"); ret = 0; }
        if (task->detail_intro_text)
        { ErrorFilenamef(filename, "Child task has intro text"); ret = 0; }
        if (task->yes_text)
        { ErrorFilenamef(filename, "Child task has accept text"); ret = 0; }
        if (task->sendoff_text)
        { ErrorFilenamef(filename, "Child task has sendoff text"); ret = 0; }
        if (task->returnSuccess)
        { ErrorFilenamef(filename, "Child task has ReturnSuccess reward"); ret = 0; }
        if (task->returnSuccessFlashback)
        { ErrorFilenamef(filename, "Child task has ReturnSuccessFlashback reward"); ret = 0; }
        if (task->returnSuccessNoFlashback)
        { ErrorFilenamef(filename, "Child task has ReturnSuccessNoFlashback reward"); ret = 0; }
        if (task->returnFailure)
        { ErrorFilenamef(filename, "Child task has ReturnFailure reward"); ret = 0; }
        if (task->forceTaskLevel)
        { ErrorFilenamef(filename, "Child task cannot force the task level"); ret = 0; }
    }

    // verify no more than one reward structure
    if (eaSize(&task->returnSuccess) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 ReturnSuccess reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->returnSuccessFlashback) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 ReturnSuccessFlashback reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->returnSuccessNoFlashback) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 ReturnSuccessNoFlashback reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->returnFailure) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 ReturnFailure reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskBegin) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskBegin reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskBeginFlashback) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskBeginFlashback reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskBeginNoFlashback) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskBeginNoFlashback reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskSuccess) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskSuccess reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskSuccessFlashback) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskSuccessFlashback reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskSuccessNoFlashback) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskSuccessNoFlashback reward for task %s\n", task->logicalname); ret = 0; }
    if (eaSize(&task->taskFailure) > 1)
    { ErrorFilenamef(filename, "Contact has more than 1 taskFailure reward for task %s\n", task->logicalname); ret = 0; }

    // verify the contents of the reward structure
    rewardVerifyErrorInfo("Task reward error", filename);
    rewardVerifyMaxErrorCount(5);
    rewardVerifyErrorInfo(task->logicalname, task->filename);

    if(task->taskBegin)
    {
        ret = VerifyRewardBlock(task->taskBegin[0], "taskBegin", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->taskBeginFlashback)
    {
        ret = VerifyRewardBlock(task->taskBeginFlashback[0], "taskBeginFlashback", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->taskBeginNoFlashback)
    {
        ret = VerifyRewardBlock(task->taskBeginNoFlashback[0], "taskBeginNoFlashback", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->taskSuccess)
    {
        ret = VerifyRewardBlock(task->taskSuccess[0], "taskSuccess", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->taskSuccessFlashback)
    {
        ret = VerifyRewardBlock(task->taskSuccessFlashback[0], "taskSuccessFlashback", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->taskSuccessNoFlashback)
    {
        ret = VerifyRewardBlock(task->taskSuccessNoFlashback[0], "taskSuccessNoFlashback", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->taskFailure)
    {
        ret = VerifyRewardBlock(task->taskFailure[0], "taskFailure", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->returnSuccess)
    {
        ret = VerifyRewardBlock(task->returnSuccess[0], "returnSuccess", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->returnSuccessFlashback)
    {
        ret = VerifyRewardBlock(task->returnSuccessFlashback[0], "returnSuccessFlashback", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->returnSuccessNoFlashback)
    {
        ret = VerifyRewardBlock(task->returnSuccessNoFlashback[0], "returnSuccessNoFlashback", filename, allowSuperGroupName, 0, 0) && ret;
    }
    if(task->returnFailure)
    {
        ret = VerifyRewardBlock(task->returnFailure[0], "returnFailure", filename, allowSuperGroupName, 0, 0) && ret;
    }

    if (eaSize(&task->missiondef) || task->missionref)
    {
        const MissionDef* mission = eaSize(&task->missiondef) ? task->missiondef[0] : task->missionref;
        int curobj, numobj = eaSize(&mission->objectives);
        for (curobj = 0; curobj < numobj; curobj++)
            if (mission->objectives[curobj]->reward && !rewardSetsVerifyAndProcess(cpp_const_cast(RewardSet**)(mission->objectives[curobj]->reward[0]->rewardSets)))
                ret = 0;
    }

    // verify that parameters are correct
    saUtilVerifyContactParams(task->detail_intro_text, filename, 1, allowSuperGroupName, 0, 0, 0, 0);        // owner may not be online
    saUtilVerifyContactParams(task->inprogress_description, filename, 1, allowSuperGroupName, 0, 0, 0, 0);    // owner may not be online
    saUtilVerifyContactParams(task->yes_text, filename, 1, allowSuperGroupName, 1, 0, 0, 0);
    saUtilVerifyContactParams(task->no_text, filename, 1, allowSuperGroupName, 1, 0, 0, 0);
    saUtilVerifyContactParams(task->sendoff_text, filename, 1, allowSuperGroupName, 1, 0, 0, 0);
    saUtilVerifyContactParams(task->decline_sendoff_text, filename, 1, allowSuperGroupName, 1, 0, 0, 0);
    saUtilVerifyContactParams(task->taskComplete_description, filename, 1, allowSuperGroupName, 1, 0, 0, 0);
    saUtilVerifyContactParams(task->taskFailed_description, filename, 1, allowSuperGroupName, 1, 0, 0, 0);
    saUtilVerifyContactParams(task->busy_text, filename, 1, allowSuperGroupName, 1, 0, 0, 0);

    // otherwise, ok
    return ret;
}

// run this after loading all the tasks -
// hooks up mission references & does error checking
//  - uses TaskPreprocessInternal for actual work
int TaskPreprocess(StoryTask* task, const char* filename, int allowSuperGroupName, int inStoryArc)
{
    return TaskPreprocessInternal(task, filename, 0, allowSuperGroupName, inStoryArc);
}

// hooks up mission references, etc.
void TaskPostprocess(StoryTask* task, const char* filename)
{
    int numdefs, i, n;

    // make sure we have the unique name added
    if (TASK_IS_UNIQUE(task))
        UniqueTaskAdd(task->logicalname);

    // check for flashback task
    if (TASK_CAN_FLASHBACK(task))
        StoryArcFlashbackMission(task);

    // check for too many definitions
    numdefs = eaSize(&task->missiondef);
    if (numdefs > 1)
        ErrorFilenamef(filename, "More than one mission definition given for task");
    if (numdefs && task->missionfile)
        ErrorFilenamef(filename, "External file and mission definition given for task");

    // hook up mission
    if (task->missionfile)
    {
        ErrorFilenamef(filename, "Tasks can no longer specify mission files directly");
    }

    // check to make sure we have a ref iff we need one
    if (!task->deprecated)
    {
        if (task->type == TASK_MISSION && !(numdefs || task->missionref))
            ErrorFilenamef(filename, "Task is missing mission");
        if (task->type != TASK_MISSION && (numdefs || task->missionref))
            ErrorFilenamef(filename, "Mission defined for errand task");
    }

    // deal with the redundant time field
    if (numdefs && task->missiondef[0]->missiontime)
        task->timeoutMins = task->missiondef[0]->missiontime; 
    if (task->missionref && task->missionref->missiontime)
        task->timeoutMins = task->missionref->missiontime; 

    // check for too many spawndefs
    numdefs = eaSize(&task->spawndefs);
    //if (numdefs > 1)
    //    ErrorFilenamef(filename, "More than one spawndef given for task");
    //if (numdefs && task->spawnfile)
    //    ErrorFilenamef(filename, "External spawndef and spawn definition given for task");

    // hook up spawndef
    if (task->spawnfile)
    {
        task->spawnref = SpawnDefFromFilename(task->spawnfile);
        if (!task->spawnref)
            ErrorFilenamef(filename, "Task has invalid spawndef reference %s", task->spawnfile);
    }

    // make sure we have a spawndef for a random encounter
    if (!task->deprecated && task->type == TASK_RANDOMENCOUNTER && !(numdefs || task->spawnref))
        ErrorFilenamef(filename, "Spawndef missing for random encounter task");

    // if we have a spawndef, make sure it has a matching type
    if (task->spawnref && !task->spawnref->spawntype)
        ErrorFilenamef(filename, "A task with a spawndef is missing SpawnType field");
    if (numdefs && !task->spawndefs[0]->spawntype)
        ErrorFilenamef(filename, "A task with a spawndef is missing SpawnType field");
    if (numdefs)
        VerifySpawnDef(task->spawndefs[0], 0);

    n = eaSize(&task->spawndefs);
    for (i = 0; i < n; i++)
    {
        SpawnDef *spawn = cpp_const_cast(SpawnDef*)(task->spawndefs[i]);
        SpawnDef_ParseVisionPhaseNames(spawn);
    }

    if (eaSize(&task->missiondef) || task->missionref)
    {
        MissionDef* mission = cpp_const_cast(MissionDef*)(eaSize(&task->missiondef) ? task->missiondef[0] : task->missionref);
        mission->vs.higherscope = &task->vs;  // hook up mission var scopes so they can look in the parent task for vars (or the story arc as well)
    }

    // cycle through children
    n = eaSize(&task->children);
    for (i = 0; i < n; i++)
    {
        StoryTask* child = cpp_const_cast(StoryTask*)(task->children[i]);
        TaskPostprocess(child, filename);
        child->alliance = task->alliance;
        child->vs.higherscope = &task->vs;  // hook up child tasks so they can look in the parent task for vars (or the story arc as well)
    }
}

void TaskValidate(StoryTask* task, const char* filename)
{
    int i, n;

    if (task->completeRequires)
        chareval_Validate(task->completeRequires, task->filename);
    if (task->flashbackRequires)
        chareval_Validate(task->flashbackRequires, task->filename);
    if (task->assignRequires)
        chareval_Validate(task->assignRequires, task->filename);
    if (task->flashbackTeamRequires)
        chareval_Validate(task->flashbackTeamRequires, task->filename);
    if (task->teamRequires)
        chareval_Validate(task->teamRequires, task->filename);
    if (task->autoIssueRequires)
        chareval_Validate(task->autoIssueRequires, task->filename);

    if (task->missiondef)
    {
        for (i = 0; i < eaSize(&task->missiondef[0]->objectives); i++)
        {
            if (task->missiondef[0]->objectives[i]->charRequires)
                chareval_Validate(task->missiondef[0]->objectives[i]->charRequires, task->filename);
        }
    }

    if(task->dialogDefs)
    {
        DialogDefs_Verify(filename, task->dialogDefs);
    }

    // cycle through children
    n = eaSize(&task->children);
    for (i = 0; i < n; i++)
    {
        StoryTask* child = cpp_const_cast(StoryTask*)(task->children[i]);
        TaskValidate(child, filename);
    }
}

static int VerifyCanGetEncounterLayout(const char* layout, int level)
{
    int i, n;
    int found = 0;
    n = eaSize(&mapSpecList.mapSpecs);
    for (i = 0; i < n; i++)
    {
        const MapSpec* spec = mapSpecList.mapSpecs[i];
        if (!MapSpecCanOverrideSpawn(spec, level)) continue;
        found = StringArrayFind(spec->encounterLayouts, layout) >= 0;
        if (found) break;
    } // for each map
    return found;
}

static int VerifyTaskSpawndef(const StoryTask* task, const SpawnDef* spawndef, const char* filename, int minlevel, int maxlevel)
{
    ScriptVarsTable vars = {0};
    const char* layout;
    int level;
    int ret = 1;

    if (!spawndef->spawntype)
    {
        ErrorFilenamef(filename, "Spawndef in task %s doesn't specify a spawn layout to override", task->logicalname);
        return 0;
    }

    // we're making the assumption here that the layout is going to resolve to a single value
    saBuildScriptVarsTable(&vars, NULL, 0, 0, NULL, task, NULL, NULL, NULL, 0, 0, false);
    ScriptVarsTablePushScope(&vars, &spawndef->vs);
    layout = ScriptVarsTableLookup(&vars, spawndef->spawntype);

    // verify we can find a correct encounter layout.  Our strategy is to see that the layout is
    // available for every 5 levels of task range
    if (minlevel < 1) minlevel = 1;
    if (maxlevel < minlevel || maxlevel > MAX_PLAYER_SECURITY_LEVEL) maxlevel = MAX_PLAYER_SECURITY_LEVEL;
    for (level = minlevel; level <= maxlevel; level += 5)
    {
        if (!VerifyCanGetEncounterLayout(layout, level))
        {
            ErrorFilenamef(filename, "Task %s (levels %i-%i) specifies spawn layout %s, but this is not available at level %i from any city zone",
                task->logicalname, minlevel, maxlevel, layout, level);
            ret = 0;
        }
    }
    return ret;
}

static int VerifyDeliveryTargets(const StoryTask* task, const char* filename, int minlevel, int maxlevel)
{
    int ret = 1;
    char** possibleTargets = 0;
    int found = 0;
    int deliveryTargetIndex, deliveryTargetCount;
    int possibleTargetIndex, possibleTargetCount;
    int mapIndex, mapCount = eaSize(&mapSpecList.mapSpecs);
    int count;

    if ((task->type == TASK_DELIVERITEM || task->type == TASK_CONTACTINTRO)
        && (!task->deliveryTargetNames || !eaSize(&task->deliveryTargetNames)))
    {
        ErrorFilenamef(filename, "Task %s is a delivery task, but has no target", task->logicalname);
        ret = 0;
    }

    // check every possible target exists at right level
    deliveryTargetCount = eaSize(&task->deliveryTargetNames);
    for (deliveryTargetIndex = 0; deliveryTargetIndex < deliveryTargetCount; deliveryTargetIndex++)
    {
        ScriptVarsLookupComplete(&task->vs, task->deliveryTargetNames[deliveryTargetIndex], &possibleTargets);
        possibleTargetCount = eaSize(&possibleTargets);
        for (possibleTargetIndex = 0; possibleTargetIndex < possibleTargetCount; possibleTargetIndex++)
        {
            char* target = possibleTargets[possibleTargetIndex];
            for (mapIndex = 0; mapIndex < mapCount; mapIndex++)
            {
                const MapSpec* spec = mapSpecList.mapSpecs[mapIndex];
                found = StringArrayFind(spec->persistentNPCs, target);
                if (found)
                {
                    if (minlevel < spec->entryLevelRange.min)
                    {
                        ErrorFilenamef(filename, "Task %s requests delivery to %s, but this target is in city %s and not available at level %i",
                            task->logicalname, target, spec->mapfile, minlevel);
                        ret = 0;
                    }
                    break;
                }
            } // each map
            if (!found)
            {
                ErrorFilenamef(filename, "Task %s requests delivery to %s, but this target does not exist",
                    task->logicalname, target);
            }
        } // each possible target
        ScriptVarsFreeList(&possibleTargets);
    } // each delivery target in task

    count = eaSize(&task->deliveryTargetDialogs);
    if (deliveryTargetCount > 1 && count)  // allow multiple potential targets for a basic delivery
    {
        if (deliveryTargetCount != count)
        {
            ErrorFilenamef(filename, "Task %s has %d delivery targets but %d dialog trees!",
                                    task->logicalname, deliveryTargetCount, count);
        }

        count = eaSize(&task->deliveryTargetDialogStarts);
        if (deliveryTargetCount != count)
        {
            ErrorFilenamef(filename, "Task %s has %d delivery targets but %d dialog tree start pages!",
                task->logicalname, deliveryTargetCount, count);
        }

        count = eaiSize(&(int*)cpp_const_cast(DeliveryMethod*)(task->deliveryTargetMethods));
        if (deliveryTargetCount != count)
        {
            ErrorFilenamef(filename, "Task %s has %d delivery targets but %d delivery methods!",
                task->logicalname, deliveryTargetCount, count);
        }
    }

    return ret;
}

static int VerifyLocations(StoryTask *task, const char *filename)
{
    if (task->taskLocationName && !task->taskLocationMap)
    {
        ErrorFilenamef(filename, "Task %s has LocationName %s but no LocationMap!",
            task->logicalname, task->taskLocationName);
    }
    else if (!task->taskLocationName && task->taskLocationMap)
    {
        ErrorFilenamef(filename, "Task %s has LocationMap %s but no LocationName!",
            task->logicalname, task->taskLocationMap);
    }

    return 1; // doesn't actually do anything
}

static int VerifyVisitLocationTask(const StoryTask* task, const char* filename, int minlevel, int maxlevel)
{
    int ret = 1; 

    if (!eaSize(&task->visitLocationNames))
    {
        ErrorFilenamef(filename, "Task %s is a visit location task, but has no locations", task->logicalname);
        ret = 0;
    }
    else
    {
        // check every location exists at right level
        int found = 0;
        int t, numt;
        int m, numm;

        numt = eaSize(&task->visitLocationNames);
        for (t = 0; t < numt; t++)
        {
            const char* target = task->visitLocationNames[t];
            numm = eaSize(&mapSpecList.mapSpecs);
            for (m = 0; m < numm; m++)
            {
                const MapSpec* spec = mapSpecList.mapSpecs[m];
                found = StringArrayFind(spec->visitLocations, target) >= 0;
                if (found)
                {
                    if (minlevel < spec->entryLevelRange.min)
                    {
                        ErrorFilenamef(filename, "Task %s requests touching %s, but this target is in city %s and not available at level %i",
                            task->logicalname, target, spec->mapfile, minlevel);
                        ret = 0;
                    }
                    break;
                }
            } // each map
            if (!found)
            {
                ErrorFilenamef(filename, "Task %s requests touching %s, but this target does not exist",
                    task->logicalname, target);
                ret = 0;
            }
        } // each location
    }
    return ret;
}

static int VerifyKillTask(const char* filename, const char* logicalname, const char* villainType, const char* villainMap, int minlevel, int maxlevel)
{
    int ret = 1;
    if (!villainType) return 1; //ok
    if (villainMap)
    {
        const MapSpec* spec = MapSpecFromMapName(villainMap);
        if (!spec)
        {
            ErrorFilenamef(filename, "Invalid map specified for kill task %s -> %s", logicalname, villainMap);
            ret = 0;
        }
        else
        {
            const char *villainGroup;
            const VillainDef *villainDef;
            int found;

            if (villainDef = villainFindByName(villainType))
            {
                villainGroup = villainGroupGetName(villainDef->group);
            }
            else
            {
                villainGroup = villainType;
            }
            
            found = StringArrayFind(spec->villainGroups, villainGroup) >= 0;
            if (!found)
            {
                ErrorFilenamef(filename, "Design", 2, "Invalid villain group %s specified in kill task %s, group doesn't exist in map %s",
                    villainType, logicalname, villainMap);
                ErrorFilenamef(filename, "Invalid villain group %s specified in kill task %s, group doesn't exist in map %s",
                    villainType, logicalname, villainMap);
                ErrorFilenamef(filename, "Invalid villain %s (group %s) specified in kill task %s, group doesn't exist in map %s",
                    villainType, villainGroup, logicalname, villainMap);
                ret = 0;
            }
        }
    }
    else // no map specified
    {
        // make sure we can get a map at this level
        int found = 0;
        int m, numm;
        const char *villainGroup;
        const VillainDef *villainDef;

        if (villainDef = villainFindByName(villainType))
        {
            villainGroup = villainGroupGetName(villainDef->group);
        }
        else
        {
            villainGroup = villainType;
        }

        numm = eaSize(&mapSpecList.mapSpecs);
        for (m = 0; m < numm; m++)
        {
            const MapSpec* spec = mapSpecList.mapSpecs[m];
            if (minlevel < spec->entryLevelRange.min)
                continue;
            found = StringArrayFind(spec->villainGroups, villainGroup) >= 0;
            if (found)
                break;
        }
        if (!found && stricmp(villainGroup, "Random"))
        {
            ErrorFilenamef(filename, "Design", 2, "Kill task %s specifies villain group %s, but there are no city zones with %s at level %i",
                logicalname, villainType, villainType, minlevel);
            ret = 0;
        }
    }
    return ret;
}    

static int TaskCheckRewardForSecondary(StoryReward **pReward)
{
    int i, size;

    if (!pReward)
        return false;

    size = eaSize(&pReward);
    for (i = 0; i < size; i++)
    {
        if (pReward[i]->secondaryReward != NULL)
            return true;
    }

    return false;
}

int TaskProcessAndCheckLevels(StoryTask* task, const char* filename, int minlevel, int maxlevel)
{
    int i, n, ret = 1;

    if (isProductionMode())
        return 1;
    if (task->deprecated) 
        return 1;
    if (!minlevel) 
        minlevel = task->minPlayerLevel;
    if (!maxlevel) 
        maxlevel = task->maxPlayerLevel;

    if (eaSize(&task->missiondef))
    {
        if (!VerifyAndProcessMissionDefinition(cpp_const_cast(MissionDef*)(task->missiondef[0]), task->logicalname, minlevel, maxlevel))
            ret = 0;
    }
    else if (task->missionref)
    {
        if (!VerifyAndProcessMissionDefinition(cpp_const_cast(MissionDef*)(task->missionref), task->logicalname, minlevel, maxlevel))
            ret = 0;
    }

    if (eaSize(&task->spawndefs))
    {
        if (!VerifyTaskSpawndef(task, task->spawndefs[0], filename, minlevel, maxlevel))
            ret = 0;
    }
    else if (task->spawnref)
    {
        if (!VerifyTaskSpawndef(task, task->spawnref, filename, minlevel, maxlevel))
            ret = 0;
    }

    // Check all tasks now, any type can have delivery targets
    if (!VerifyDeliveryTargets(task, filename, minlevel, maxlevel))
        ret = 0;

    if (!VerifyLocations(task, filename))
        ret = 0;

    if (task->type == TASK_VISITLOCATION)
    {
        if (!VerifyVisitLocationTask(task, filename, minlevel, maxlevel))
            ret = 0;
    }

    if (task->type == TASK_KILLX)
    {
        if (!VerifyKillTask(filename, task->logicalname, task->villainType, task->villainMap, minlevel, maxlevel)) 
            ret = 0;
        if (!VerifyKillTask(filename, task->logicalname, task->villainType2, task->villainMap2, minlevel, maxlevel)) 
            ret = 0;
    }

    if (task->type == TASK_GOTOVOLUME)
    {
        // should I try to verify that a volume with the specified name exists somewhere?
    }

    n = eaSize(&task->children);
    for (i = 0; i < n; i++)
    {
        if (!TaskProcessAndCheckLevels(cpp_const_cast(StoryTask*)(task->children[i]), filename, minlevel, maxlevel))
            ret = 0;
    }
    return ret;
}

// *********************************************************************************
//  Utility functions for task defs
// *********************************************************************************

// returns the task definition specified, without consideration for the current child if any
const StoryTask* TaskParentDefinition(const StoryTaskHandle *sahandle)
{
    if (isStoryarcHandle(sahandle)) 
        return StoryArcTaskDefinition(sahandle);
    else 
        return ContactTaskDefinition(sahandle);
}

// given the parent definition and a current child pos, return the child's definition
const StoryTask* TaskChildDefinition(const StoryTask* parent, int compoundPos)
{
    int n;

    if (!parent) 
        return NULL;
    n = eaSize(&parent->children);
    if (compoundPos < 0 || compoundPos >= n) 
        return NULL;
    return parent->children[compoundPos];
}

// returns the definition of the selected task, or its current child.  whichever is appropriate
const StoryTask* TaskDefinition(const StoryTaskHandle *sahandle)
{
    const StoryTask* def = TaskParentDefinition(sahandle);
    if (def && def->type == TASK_COMPOUND)
        def = TaskChildDefinition(def, sahandle->compoundPos);
    return def;
}

// returns what type this task will start out as (looks at first child if compound)
TaskType TaskStartingType(StoryTask* taskdef)
{
    if (taskdef->type == TASK_COMPOUND)
        return taskdef->children[0]->type;
    return taskdef->type;
}

TaskSubHandle TaskFromLogicalName(const ContactDef* contactdef, const char* logicalname)
{
    int i, n;
    n = eaSize(&contactdef->tasks);
    for (i = 0; i < n; i++)
    {
        if (!contactdef->tasks[i]->logicalname)
            continue;
        if (!stricmp(contactdef->tasks[i]->logicalname, logicalname))
            return StoryIndexToHandle(i);
    }
    return 0;
}

const StoryTask *TaskDefFromLogicalName(char* logicalname)
{
    int i, j, n;
    n = eaSize(&g_tasksets.sets);
    for (i = 0; i < n; i++)
    {
        int count = eaSize(&(g_tasksets.sets[i]->tasks));

        for (j = 0; j < count; j++)
        {
            const StoryTask *pTask = g_tasksets.sets[i]->tasks[j];
            if (pTask != NULL && pTask->logicalname != NULL && !strcmp(logicalname, pTask->logicalname))
                return g_tasksets.sets[i]->tasks[j];
        }
    }
    return NULL;
}


const MissionDef* TaskMissionDefinition(const StoryTaskHandle *sahandle)
{
    const StoryTask* def = TaskDefinition(sahandle);
    return TaskMissionDefinitionFromStoryTask(def);
}

const MissionDef* TaskMissionDefinitionFromStoryTask(const StoryTask* def)
{
    if (!def) 
        return NULL;
    if (def->missionref) 
        return def->missionref;
    if (eaSize(&def->missiondef)) 
        return def->missiondef[0];
    return NULL;
}

const SpawnDef* TaskSpawnDef(const StoryTask* def)
{
    if (!def) 
        return NULL;
    if (def->spawnref) 
        return def->spawnref;
    if (eaSize(&def->spawndefs)) 
        return def->spawndefs[0];
    return NULL;
}

// returns filename of storyarc or contact
const char* TaskFileName(TaskHandle context)
{
    if (context<0) 
        return StoryArcFileName(context);
    else
        return ContactFileName(context);
}

// returns handle of storyarc or contact
TaskHandle TaskGetHandle(const char* filename)
{
    StoryTaskHandle sa = StoryArcGetHandle(filename);
    if (sa.context) 
        return sa.context;
    return ContactGetHandle(filename);
}

// return whether task isn't totally empty
int TaskSanityCheck(const StoryTask* taskdef)
{
    return taskdef->detail_intro_text || taskdef->sendoff_text || taskdef->inprogress_description || taskdef->busy_text;
}

const StoryClue** TaskGetClues(StoryTaskHandle *sahandle)
{
    if (isStoryarcHandle(sahandle))
    {
        const StoryArc* arc;
        arc = StoryArcDefinition(sahandle);
        if( arc )
            return arc->clues;
        else
            return 0;
    }
    else
    {
        const StoryTask *task = TaskParentDefinition(sahandle);
        return task ? task->clues : 0;
    }
}

// as above, but must be called with a parent def
int TaskContainsMissionDef(const StoryTask* parent)
{
    if (!parent) return 0;
    if (parent->type == TASK_MISSION) return 1;
    if (parent->type == TASK_COMPOUND)
    {
        int i, n;
        n = eaSize(&parent->children);
        for (i = 0; i < n; i++)
        {
            if (parent->children[i]->type == TASK_MISSION)
                return 1;
        }
    }
    return 0;
}

// returns whether ANY PART of this task is a mission
int TaskContainsMission(StoryTaskHandle *sahandle)
{
    return TaskContainsMissionDef(TaskParentDefinition(sahandle));
}

// *********************************************************************************
//  Task sets
// *********************************************************************************

// less restrictive - for debug commands
static const StoryTaskSet* TaskSetFindLoose(const char* filename)
{
    char buf[MAX_PATH], buf2[MAX_PATH];
    int i, n;

    // try to look it up exactly
    const StoryTaskSet* ret = TaskSetFind(filename);
    if (ret) return ret;

    // otherwise, try matching the initial part of the filename
    saUtilFilePrefix(buf, filename);
    n = eaSize(&g_tasksets.sets);
    for (i = 0; i < n; i++)
    {
        saUtilFilePrefix(buf2, g_tasksets.sets[i]->filename);
        if (!stricmp(buf, buf2))
            return g_tasksets.sets[i];
    }
    return NULL;
}

static int TaskSetPreprocess(StoryTaskSet* taskset)
{
    int ret = 1;
    int task, ntasks = eaSize(&taskset->tasks);
    saUtilPreprocessScripts(ParseStoryTaskSet, taskset);
    for (task = 0; task < ntasks; task++)
    {
        StoryTask* taskdef = cpp_const_cast(StoryTask*)(taskset->tasks[task]);
        if (!taskdef->deprecated &&    !taskdef->type == TASK_CONTACTINTRO && eaSize(&taskdef->returnSuccess) < 1)
        {
            ErrorFilenamef(taskset->filename, "Contact has no ReturnSuccess reward for task %s\n", taskdef->logicalname);
            ret = 0;
        }
        if (!TaskPreprocess(taskdef, taskset->filename, 1, 0))
            ret = 0;
    }
    return ret;
}

// let each task do it's own preprocessing
bool TaskSetPreprocessAll(ParseTable* pti, void* structptr)
{
    StoryTaskSetList * tasksets = (StoryTaskSetList *)structptr;
    bool ret = true;
    int set, nsets = eaSize(&tasksets->sets);
    for (set = 0; set < nsets; set++)
        ret &= TaskSetPreprocess(cpp_const_cast(StoryTaskSet*)(tasksets->sets[set]));
    return ret;
}

// Extract vars and check string length for the in progress description
// Make sure the length is less than or equal to 32(should be 30 but cutting them some slack
int TaskCheckStringLengths(StoryTask* taskdef)
{
    ScriptVarsTable vars = {0};
    int nvars, var;
    const char* description = (taskdef->inprogress_description)?saUtilTableLocalize(taskdef->inprogress_description, &vars):NULL;
    char* varname = NULL;

    if (description)
    {
        if (strstriConst(description, "ITEM_NAME"))
            varname = "ITEM_NAME";
        else if (strstriConst(description, "PERSON_NAME"))
            varname = "PERSON_NAME";
        else if (strstriConst(description, "BOSS_NAME"))
            varname = "BOSS_NAME";
    
        if (varname)
        {
            nvars = eaSize(&taskdef->vs.vars);
            for (var = 0; var < nvars; var++)
            {
                char* currvar = VarName(taskdef->vs.vars[var]);
                if (!stricmp(currvar, varname))
                {
                    char* groupname = VarGroupName(taskdef->vs.vars[var]);
                    int val, numvalues = VarNumValues(taskdef->vs.vars[var]);

                    for (val = 0; val < numvalues; val++)
                    {
                        ScriptVarsTablePushVar(&vars, varname, VarValue(taskdef->vs.vars[var], val));
                        description = (taskdef->inprogress_description)?saUtilTableLocalize(taskdef->inprogress_description, &vars):NULL;
                        ScriptVarsTablePopVar(&vars);
                        if (strlen(description) > 30)
                            printf("\n%s\n%s(%i)\n", taskdef->filename, description, strlen(description));
                    }
                }
            }
        }
        else 
        {
            if (strlen(description) > 30)
                printf("\n%s\n%s(%i)\n", taskdef->filename, description, strlen(description));
        }
    }
    return 0;
}

static void TaskSetPostprocess(StoryTaskSet* taskset)
{
    int task, ntasks = eaSize(&taskset->tasks);
    saUtilPreprocessScripts(ParseStoryTaskSet, taskset);
    for (task = 0; task < ntasks; task++)
    {
        TaskPostprocess(cpp_const_cast(StoryTask*)(taskset->tasks[task]), taskset->filename);
        TaskProcessAndCheckLevels(cpp_const_cast(StoryTask*)(taskset->tasks[task]), taskset->filename, 0, 0);
        // LH: Add at a later date when design has done some cleanup on these.
        //TaskCheckStringLengths(taskset->tasks[task]);
    }
}

// let each task do it's own postprocessing
bool TaskSetPostprocessAll(ParseTable* pti, void* structptr, bool shared_memory)
{
    StoryTaskSetList * tasksets = (StoryTaskSetList *)structptr;
    int set, nsets = eaSize(&tasksets->sets);
    for (set = 0; set < nsets; set++)
        TaskSetPostprocess(cpp_const_cast(StoryTaskSet*)(tasksets->sets[set]));
    return true;
}

// Search for a recently reloaded taskset def that still needs to be preprocessed
// Different search because the def has not yet been processed
static StoryTaskSet* TaskSetFindUnprocessed(const char* relpath)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_tasksets.sets);
    strcpy(cleanedName, strstriConst(relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
        if (!stricmp(cleanedName, g_tasksets.sets[i]->filename))
            return cpp_const_cast(StoryTaskSet*)(g_tasksets.sets[i]);
    return NULL;
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void TaskSetReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseStoryTaskSetList, sizeof(StoryTaskSetList), (void*)&g_tasksets, NULL, NULL))
    {
        StoryTaskSet* taskset = TaskSetFindUnprocessed(relpath);
        TaskSetPreprocess(taskset);
        TaskSetPostprocess(taskset);
        ContactReattachTaskSets(taskset);

        // If the parser reload changed the task significantly, we need to
        // force the server to restart the mission before the next server
        // tick to prevent the server from crashing.
        if (isDevelopmentMode() && OnMissionMap())
        {
            PlayerEnts * players = &player_ents[PLAYER_ENTS_ACTIVE];
            if (players->count) {
                EncounterForceReload(players->ents[0]);
            }
        }
    }
    else
    {
        Errorf("Error reloading taskset: %s\n", relpath);
    }
}

static void TaskSetSetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.taskset", TaskSetReloadCallback);
}

void TaskSetPreload()
{
    int flags = PARSER_SERVERONLY;
    if (g_tasksets.sets)
        flags = PARSER_FORCEREBUILD; // Re-load!
    ParserUnload(ParseStoryTaskSetList, &g_tasksets, sizeof(g_tasksets));

    if (!ParserLoadFilesShared("SM_tasksets.bin", SCRIPTS_DIR, ".taskset", "tasksets.bin", flags,
        ParseStoryTaskSetList, &g_tasksets, sizeof(g_tasksets), NULL, NULL, TaskSetPreprocessAll, NULL, TaskSetPostprocessAll))
    {
        printf("Error loading task set definitions\n");
    }
    TaskSetSetupCallbacks();
}

const StoryTaskSet* TaskSetFind(const char* filename)
{
    char buf[MAX_PATH];
    int i, n;

    saUtilCleanFileName(buf, filename);
    n = eaSize(&g_tasksets.sets);
    for (i = 0; i < n; i++)
    {
        if (!strcmp(buf, g_tasksets.sets[i]->filename))
            return g_tasksets.sets[i];
    }
    return NULL;
}

int TaskSetCountTasks(char* filename)
{
    int retval = 0;
    const StoryTaskSet* set;
    int count;

    if (filename)
    {
        set = TaskSetFind(filename);

        if (set)
        {
            for (count = 0; count < eaSize(&(set->tasks)); count++)
            {
                if (!(set->tasks[count]->deprecated))
                {
                    retval++;
                }
            }
        }
    }

    return retval;
}

int TaskSetTotalTasks(void)
{
    int i, n, t, numt;
    int result = 0;
    n = eaSize(&g_tasksets.sets);
    for (i = 0; i < n; i++)
    {
        numt = eaSize(&g_tasksets.sets[i]->tasks);
        for (t = 0; t < numt; t++)
        {
            if (!g_tasksets.sets[i]->tasks[t]->deprecated)
                result++;
        }
    }
    return result;
}

void FixHigherscopePointers(StoryTask *pTask)
{
    int i, n;

    assert(pTask);

    // cycle through children recursively
    n = eaSize(&pTask->children);
    for (i = 0; i < n; i++)
    {
        StoryTask* child = cpp_const_cast(StoryTask*)(pTask->children[i]);
        FixHigherscopePointers(child);
        child->vs.higherscope = &pTask->vs;  // hook up child tasks so they can look in the parent task for vars (or the story arc as well)
    }

    if (eaSize(&pTask->missiondef) || pTask->missionref)
    {
        MissionDef* mission = cpp_const_cast(MissionDef*)(eaSize(&pTask->missiondef) ? pTask->missiondef[0] : pTask->missionref);
        mission->vs.higherscope = &pTask->vs;
    }

    pTask->vs.higherscope = NULL;
}

StoryTask *TaskDefStructCopy(const StoryTask *pTask)
{
    StoryTask *pCopy;

    pCopy = ParserAllocStruct(sizeof(*pCopy));
    ParserCopyStruct(ParseStoryTask, pTask, sizeof(*pCopy), pCopy);

    // fix higherscope pointers (leaves the task higherscope NULL)
    FixHigherscopePointers(pCopy);

    return pCopy;
}

int TaskDefIsFlashback(const StoryTask *pTask)
{
     return (TASK_CAN_FLASHBACK(pTask));
}
