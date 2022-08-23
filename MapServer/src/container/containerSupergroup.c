/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "containerSupergroup.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "container/dbcontainerpack.h"
#include <utilitieslib/utils/file.h>
#include "entity/RewardToken.h"
#include "entity/Supergroup.h"
#include "team.h"
#include "bases/basedata.h"
#include "entity/SgrpServer.h"
#include "dbcomm/dbdoor.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/taskdef.h"
#include "bases/DetailRecipe.h"
#include "dbcomm/logcomm.h"
#include <utilitieslib/utils/error.h>

#if SERVER || GAME
#include "containerloadsave.h"
#endif


static const char *DetailToAttr(intptr_t num);
static intptr_t AttrToDetail(const char *pch);

static const char *RecipeInvItemToAttr(intptr_t num)
{
    if(num)
    {
        const DetailRecipe *pRecipe = (const DetailRecipe *)num;
        return pRecipe->pchName;
    }

    return "";
}

static intptr_t AttrToRecipeInvItem(const char *pch)
{
    extern ParseLink g_DetailRecipe_Link;
    return (intptr_t)ParserLinkFromString(&g_DetailRecipe_Link, pch);
}


MP_DEFINE(SpecialDetail);
SpecialDetail *CreateSpecialDetail(void)
{
    MP_CREATE(SpecialDetail, 16);
    return MP_ALLOC(SpecialDetail);
}

void DestroySpecialDetail(SpecialDetail *p)
{
    MP_FREE(SpecialDetail, p);
}

static const char *DetailToAttr(intptr_t num)
{
    if(num)
    {
        Detail *pDetail = (Detail *)num;
        return pDetail->pchName;
    }

    return "";
}

#if RAIDSERVER || STATSERVER
MP_DEFINE( Detail );
void MakeDummyDetailMP( void )
{
    MP_CREATE( Detail, 100 );  //MEM pool for the dummy details used by the raidserver's version of the SuperGroup loader
}

void DestroyDummyDetailMP( void )
{
    //TO DO destroy strings in the memory pool -- currently a memory leak,
    //but it doesn't matter, since we never call this function anymore
    MP_DESTROY( Detail );
}

Detail * getDummyDetail( const char * pch )
{
    static StashTable ht_dummyDetails = 0; 
    Detail * pDetail;
    if( !pch )
        return 0;

    MakeDummyDetailMP();

    if(!ht_dummyDetails)
        ht_dummyDetails = stashTableCreateWithStringKeys(5000,StashDeepCopyKeys);

    stashFindPointer( ht_dummyDetails, pch, &pDetail );
    if( pDetail )
    {
        return pDetail;
    }
    
    pDetail = MP_ALLOC( Detail );
    pDetail->pchName = strdup( pch );

    stashAddPointer( ht_dummyDetails, pDetail->pchName, pDetail, false );

    return pDetail;
} 
#endif  

static intptr_t AttrToDetail(const char *pch)
{
#if SERVER
    extern ParseLink g_base_detailInfoLink;
    return (intptr_t)ParserLinkFromString(&g_base_detailInfoLink, pch);
#endif
#if RAIDSERVER || STATSERVER //Raidserver just needs the name, and doesn't care about anything else
    return (intptr_t)getDummyDetail( pch );
#else
    return 0; // Not needed except on mapserver (i.e. not in statserver)
#endif
}

LineDesc sgrp_special_detail_line_desc[] =
{
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "Detail",   OFFSET(SpecialDetail, pDetail),      INOUT(AttrToDetail, DetailToAttr) },
        "special detail name"},

    {{ PACKTYPE_DATE, 0,            "Creation", OFFSET(SpecialDetail, creation_time) },
        "time this detail was created"},

    {{ PACKTYPE_INT,  SIZE_INT32,     "Flags",    OFFSET(SpecialDetail, iFlags)        },
        "flags that are on this detail"},

    {{ PACKTYPE_DATE, 0,            "Expires", OFFSET(SpecialDetail, expiration_time) },
        "time this detail expires"},

    { 0 },
};

StructDesc sgroup_special_detail_desc[] =
{
    sizeof(SpecialDetail),
    { AT_EARRAY, OFFSET(Supergroup, specialDetails) },
    sgrp_special_detail_line_desc,
    
    "TODO"
};

//------------------------------------------------------------
//  @see MapServer/container/containerloadsave.c:reward_token_line_desc
//----------------------------------------------------------
LineDesc sgrp_reward_token_line_desc[] =
{
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "PieceName", OFFSET(RewardToken, reward) },
        "reward token name"},

    {{ PACKTYPE_INT,  SIZE_INT32, "RewardValue", OFFSET(RewardToken, val) },
        "reward token value"},

    {{ PACKTYPE_INT,  SIZE_INT32, "RewardTime", OFFSET(RewardToken, timer) },
        "reward token timer, usually last time rewarded"},

    { 0 },
};

StructDesc sgroup_reward_token_desc[] =
{
    sizeof(RewardToken),
    {AT_EARRAY, OFFSET(Supergroup, rewardTokens )},
    sgrp_reward_token_line_desc
};

LineDesc sgrp_recipe_line_desc[] =
{
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "Recipe",   OFFSET(RecipeInvItem, recipe) },
        "recipe name"},

    {{ PACKTYPE_INT,  SIZE_INT32,     "Count",    OFFSET(RecipeInvItem, count) },
        "number of this recipe the sg has"},

    { 0 },
};

StructDesc sgroup_recipe_desc[] =
{
    sizeof(RecipeInvItem),
    {AT_EARRAY, OFFSET(Supergroup, invRecipes)},
    sgrp_recipe_line_desc
};

LineDesc sgroup_member_line_desc[] =
{
    {{ PACKTYPE_CONREF, CONTAINER_ENTS,                        "Dbid",            OFFSET(SupergroupMemberInfo, dbid)            },
        "dbid of the sg member that has this rank"},

    {{ PACKTYPE_INT, SIZE_INT32,                        "Rank",            OFFSET(SupergroupMemberInfo, rank)            },
        "rank of this sg member"},

    { 0    },
};

StructDesc sgroup_member_desc[] =
{
    sizeof(SupergroupMemberInfo),
    {AT_EARRAY, OFFSET(Supergroup, memberranks )},
    sgroup_member_line_desc
};

LineDesc sg_ally_line_desc[] =
{
    {{ PACKTYPE_CONREF,    CONTAINER_SUPERGROUPS,        "AllyId",        OFFSET(SupergroupAlly, db_id)            },
        "supergroup id of this ally"},

    {{ PACKTYPE_INT, SIZE_INT32,                        "DisplayRank",    OFFSET(SupergroupAlly, minDisplayRank)    },
        "minimum rank a person from this group must be to talk to my supergroup"},

    {{ PACKTYPE_INT,    SIZE_INT32,                        "DontTalkTo",    OFFSET(SupergroupAlly, dontTalkTo)        },
        "if this is set this ally won't hear my sg chat"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(SupergroupAlly, name),    "AllyName",        OFFSET(SupergroupAlly, name)            },
        "sg name of this ally"},

    { 0 },
};

StructDesc sg_ally_struct_desc[] =
{
    sizeof(SupergroupAlly),
    {AT_STRUCT_ARRAY,{INDIRECTION(Supergroup, allies, 0)} },
    sg_ally_line_desc
};

static LineDesc sgroup_rank_linedesc[] =
{
    {{ PACKTYPE_STR_UTF8, SIZEOF2(SupergroupRank, name),        "Name",                        OFFSET(SupergroupRank, name),            },
        "display name of this rank"},

    {{ PACKTYPE_BIN_STR, SIZEOF2(SupergroupRank, permissionInt), "Permissions",        OFFSET(SupergroupRank, permissionInt)    },
        "bitfield of permissions for this rank"},

    {0}
};

static StructDesc sgroup_rank_desc[] =
{
    sizeof(SupergroupRank),
    { AT_STRUCT_ARRAY, OFFSET(Supergroup, rankList)    },
    sgroup_rank_linedesc
};

StashTable s_taskHandleFromFilename = 0;

static intptr_t sgTaskGetHandle(const char* filename)
{
    StashElement he = NULL;
#if SERVER
    return (intptr_t)TaskGetHandle(filename);
#else
    if( !s_taskHandleFromFilename )
    {
        s_taskHandleFromFilename = stashTableCreateWithStringKeys( 128, StashDeepCopyKeys );
    }
    stashFindElement(s_taskHandleFromFilename, filename, &he);
    if (!he)
        stashAddPointerAndGetElement(s_taskHandleFromFilename, filename, NULL, false, &he);
    assert(he);
    return (intptr_t)he;
#endif
}

const char* sgTaskFileName(intptr_t context)
{
#if SERVER
    return TaskFileName((TaskHandle)context);
#else
    if(s_taskHandleFromFilename)
    {
        //FIXME
        // evil to cast const to non-const, but there's too much checked out right now to get into this.
        StashTableIterator hi;
        StashElement he = NULL;
        for(stashGetIterator(s_taskHandleFromFilename,&hi);stashGetNextElement(&hi, & he);)
        {
            const char *name = stashElementGetStringKey(he);
            if( (TaskHandle)he == (TaskHandle)context && name )
            {
                return name;
            }
        }
    }
    return NULL;
#endif
}

// ***********************************************************************************
// IMPORTANT: Make sure you also update this in containerloadsave.c task_line_desc
// Yes this is a terrible hack, but its the best option for now
// ***********************************************************************************

// @hack -AB: copied verabitim from containerloadsave.c. refactor into its own file :11/10/05
LineDesc sg_task_line_desc[] =
{
    // This table describes a supergroup task.  A task is given to the supergroup by a special contact.
    // Each row describes a single supergroup task.
    // Each supergroup can have at most one task.

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,        "ID",            OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, context),        INOUT(sgTaskGetHandle, sgTaskFileName)},
        "Handle of the contact or storyarc from which this task came"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SubHandle",    OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, subhandle)},
        "Identifies which task this is within the contact or storyarc"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "CompoundPos",    OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, compoundPos) },
        "The step of a compound that the player is currently on"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Seed",            OFFSET(StoryTaskInfo, seed)},
        "The random seed used to generate the task."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "State",        OFFSET(StoryTaskInfo, state)},    // FIXME!!! Translate to readble string or use attribs?
        "The current state of the task<br>"
        "   0 = TASK_NONE<br>"
        "   1 = TASK_ASSIGNED<br>"
        "   2 = TASK_MARKED_SUCCESS<br>"
        "   3 = TASK_MARKED_FAILURE<br>"
        "   4 = TASK_SUCCEEDED<br>"
        "   5 = TASK_FAILED"},

    {{ PACKTYPE_BIN_STR,    CLUE_BITFIELD_SIZE*4,    "Clues",    OFFSET(StoryTaskInfo, haveClues)},
        "Bitfield - Keeps track of the clues that the player has seen"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "ClueNeedsIntro",OFFSET(StoryTaskInfo, clueNeedsIntro)},
        "Marks which clue, if any, needs to be shown to the player next time they see the contact"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SpawnGiven",    OFFSET(StoryTaskInfo, spawnGiven)},
        "Flags whether or not the encounter associated with the task has been spawned"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Level",        OFFSET(StoryTaskInfo, level)},
        "The 1-based level of the task."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Timeout",        OFFSET(StoryTaskInfo, timeout)},
        "The time that the task timer will expire."},

    {{ PACKTYPE_CONREF,        CONTAINER_ENTS,        "AssignedDbId",        OFFSET(StoryTaskInfo, assignedDbId) },
        "DB ID - ContainerID of the player the task was assigned to"},

    {{ PACKTYPE_INT,        SIZE_INT32,        "AssignedTime",        OFFSET(StoryTaskInfo, assignedTime) },
        "Keeps track of when the task was assigned"},

    {{ PACKTYPE_INT,        SIZE_INT32,        "MissionMapId",        OFFSET(StoryTaskInfo, missionMapId) },
        "Map ID - The map ID for the mission map"},

    {{ PACKTYPE_INT,        SIZE_INT32,        "MissionDoorMapId",    OFFSET(StoryTaskInfo, doorMapId)    },
        "Map ID - The map ID for the map which contains the door to the mission."},

    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "MissionDoorPosX",    OFFSET(StoryTaskInfo, doorPos[0])   },
        "The location on the MissionDoorMap of the door to the mission."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "MissionDoorPosY",    OFFSET(StoryTaskInfo, doorPos[1])   },
        "The location on the MissionDoorMap of the door to the mission."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "MissionDoorPosZ",    OFFSET(StoryTaskInfo, doorPos[2])   },
        "The location on the MissionDoorMap of the door to the mission."},

    {{ PACKTYPE_BIN_STR, MAX_OBJECTIVE_INFOS*4, "CompleteObjectives", OFFSET(StoryTaskInfo, completeObjectives)},
        "Keeps track of which mission objectives have been completed"},

    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, villainType),    "VillainType",    OFFSET(StoryTaskInfo, villainType)},
        "Deprecated: no longer used"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "VillainCount",    OFFSET(StoryTaskInfo, curKillCount)},
        "Current number of villains killed for kill tasks"},

    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, villainType2),    "VillainType2",    OFFSET(StoryTaskInfo, villainType2)},
        "Deprecated: no longer used"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "VillainCount2",OFFSET(StoryTaskInfo, curKillCount2)},
        "Current number of villains killed for kill tasks, a second count for tracking a second villain group"},

    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, deliveryTargetName),    "DeliveryTargetName",OFFSET(StoryTaskInfo, deliveryTargetName)},
        "Deprecated: no longer used"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "NextVisitLocation", OFFSET(StoryTaskInfo, nextLocation) },
        "Index of the next location to visit(only for visit location tasks)"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SubtaskSuccess",OFFSET(StoryTaskInfo, subtaskSuccess) },
        "Bitfield - Marks whether each subtask within a compound task has been completed"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Notoriety",    OFFSET(StoryTaskInfo, old_notoriety)},
        "deprecated"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SkillLevel",    OFFSET(StoryTaskInfo, skillLevel)},
        "The skill level that this task was spawned at"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "VillainGroup",    OFFSET(StoryTaskInfo, villainGroup)},
        "The randomly generated villaingroup that should be spawned within the mission"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "MapSet",        OFFSET(StoryTaskInfo, mapSet)},
        "The randomly generated mapset that is to be used for this task"},

    {{ PACKTYPE_INT,        SIZE_INT8,            "TeamCompleted",OFFSET(StoryTaskInfo, teamCompleted)},
        "An identical task has been completed with a group meaning this task can be prompted for completion.<br>"
        "The value stored is the notoriety level that the task was teamcompleted on"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "LevelAdjust",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, levelAdjust )},
        "Level adustment of enemies  ( -1 to +4 )"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "TeamSize",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, teamSize )},
        "Team size this player is treated as ( 1 to 8 )"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "UpgradeAV",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, alwaysAV )},
        "it true, don't downgrade AV to EB, otherwise always do"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "DowngradeBoss",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, dontReduceBoss )},
        "if true, no bosses while solo"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "TimerType",    OFFSET(StoryTaskInfo, timerType)},
        "The type of timer used on the task.  1 is count up, -1 is count down."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "FailOnTimeout",    OFFSET(StoryTaskInfo, failOnTimeout)},
        "Whether the task will fail when the timer expires."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "TimeZero",        OFFSET(StoryTaskInfo, timezero)},
        "The time when the timer equals zero.  Differs from timeout on limited countups."},

    { 0 },
};


static StructDesc sg_task_desc[] =
{
    sizeof(StoryTaskInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(Supergroup, activetask, 0)}},
    sg_task_line_desc
};

static LineDesc sgrp_praet_bonus_line_desc[] =
{
    // Why not a CONREF here?
    {{ PACKTYPE_INT,        SIZE_INT32,            "ID",        { 0, 0, "Supergroup", "PraetBonusIds" }},
        "DBID of an ex-Praetorian who granted a Prestige bonus upon joining the SG"},

    { 0 },
};

static StructDesc sgrp_praet_bonus_desc[] =
{
    sizeof(U32),
    {AT_STRUCT_ARRAY,{OFFSET(Supergroup, praetBonusIds)}},
    sgrp_praet_bonus_line_desc
};

LineDesc supergroup_line_desc[] =
{
    // columns

    {{ PACKTYPE_INT,    SIZE_INT32,                            "LeaderId",                    OFFSET2(Supergroup, members, TeamMembers, leader  ) },
        "dbid of the leader -- not used"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup, motto),            "Motto",                    OFFSET(Supergroup,    motto),            },
        "sg motto"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup, name),            "Name",                        OFFSET(Supergroup,    name),    INOUT(0,0),    LINEDESCFLAG_INDEXEDCOLUMN    },
        "sg name"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup, msg),            "Message",                    OFFSET(Supergroup,    msg),            },
        "sg motd"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup,    leaderTitle),    "LeaderTitle",                OFFSET(Supergroup,    leaderTitle),    },
        "leader title -- not used anymore, on sgrpCustomRanks now"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup,    captainTitle),    "CaptainTitle",                OFFSET(Supergroup,    captainTitle),    },
        "captain title -- not used anymore, on sgrpCustomRanks now"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup,    memberTitle),    "MemberTitle",                OFFSET(Supergroup,    memberTitle),    },
        "member title -- not used anymore, on sgrpCustomRanks now"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup,    emblem),        "Emblem",                    OFFSET(Supergroup,    emblem),        },
        "index of the supergroup emblem"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "PrimaryColor",                OFFSET(Supergroup,    colors[0]),        },
        "first supergroup color"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "SecondaryColor",            OFFSET(Supergroup,    colors[1]),        },
        "second supergroup color"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "AllianceChatTalkRank",        OFFSET(Supergroup,    alliance_minTalkRank),},
        "min rank a person needs to be to talk"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(Supergroup, description),    "Description",                OFFSET(Supergroup,    description),    },
        "sg description"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "Tithe",                    OFFSET(Supergroup,    tithe),            },
        "percentage taken from supergroup earnings (not used)"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DemoteTimeout",            OFFSET(Supergroup,    demoteTimeout), },
        "time a leader has to be inactive to get demoted"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "Influence",                OFFSET(Supergroup,    influence),        },
        "amount of influence a supergroup has... afaik unused because we switched to prestige"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "Prestige",                    OFFSET(Supergroup,    prestige),        },
        "amount of prestige the supergroup has"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "PrestigeBase",                OFFSET(Supergroup,    prestigeBase),    },
        "amount of prestige invested in a sg base"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "PrestigeAddedUpkeep",        OFFSET(Supergroup,    prestigeAddedUpkeep),    },
        "prestige needed to upkeep base items"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "UpkeepRentDue",            OFFSET(Supergroup,    upkeep.prtgRentDue),    },
        "amount of base rent due"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "UpkeepSecsRentDueDate",    OFFSET(Supergroup,    upkeep.secsRentDueDate),},
        "time when rent is next due"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "BaseEntryPermission",        OFFSET(Supergroup,    entryPermission),    },
        "who is allowed to enter the base"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "SpacesForItemsOfPower",    OFFSET(Supergroup,    spacesForItemsOfPower),    },
        "the number of spaces for items of power in this group's base, includes IoP and mounts"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "PlayerType",                OFFSET(Supergroup,    playerType),    },
        "whether this is a hero or villain supergroup"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "Flags",                    OFFSET(Supergroup,    flags),    },
        "flags to track various things related to prestige"},

    {{ PACKTYPE_BIN_STR, BADGE_SG_BITFIELD_BYTE_SIZE,        "BadgesOwned",    OFFSET(Supergroup,    badgeStates.abitsOwned),},
        "supergroup badges owned"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "BonusCount",                OFFSET(Supergroup,    prestigeBonusCount),    },
        "how much bonus prestige this group has (for the number of members it has)"},

    {{ PACKTYPE_DATE,    0,                                    "DateCreated",                OFFSET(Supergroup,    dateCreated),            },
        "The date that the supergroup was created."},

	{{ PACKTYPE_INT,	SIZE_INT32,							"Passcode",					OFFSET(Supergroup,	passcode),	},
		"Hashed passcode; allows visitors to enter the base."},

	{{ PACKTYPE_STR_UTF8,	SIZEOF2(Supergroup, music),	"Music",					OFFSET(Supergroup,	music),	},
		"Background music played inside the Supergroup base."},

    // subtables

    {{ PACKTYPE_SUB,    MAX_SUPERGROUP_ALLIES,                "SuperGroupAllies",            (intptr_t)sg_ally_struct_desc            },
        "allies of this supergroup"},

    {{ PACKTYPE_EARRAY, (intptr_t)rewardtoken_Create,            "SgrpRewardTokens",            (intptr_t)sgroup_reward_token_desc        },
        "reward tokens this supergroup has"},

    {{ PACKTYPE_EARRAY, (intptr_t)createSupergroupMemberInfo,    "SgrpMembers",                (intptr_t)sgroup_member_desc                },
        "list of members and their ranks"},

    {{ PACKTYPE_EARRAY, (intptr_t)CreateSpecialDetail,        "SpecialDetails",            (intptr_t)sgroup_special_detail_desc        },
        "list of special details (base details that we need to know outside of the base"},

    {{ PACKTYPE_EARRAY, (intptr_t)CreateRecipeInvItem,        "Recipes",                    (intptr_t)sgroup_recipe_desc                },
        "number of supergroup recipes (unused)"},

    {{ PACKTYPE_SUB, NUM_SG_RANKS,                        "SgrpCustomRanks",            (intptr_t)sgroup_rank_desc                },
        "names and permissions for the different sg ranks"},

    {{ PACKTYPE_SUB, 1,                                    "SGTask",                    (intptr_t)sg_task_desc                    },
        "information for the supergroup mission this sg has"},

    {{ PACKTYPE_SUB, MAX_SUPER_GROUP_MEMBERS,            "SgrpPraetBonusIDs",        (intptr_t)sgrp_praet_bonus_desc            },
        "DBIDs of all members who granted this SG a Praetorian bonus"},

    { 0 },
};

StructDesc supergroup_desc[] =
{
    sizeof(Supergroup),
    {AT_NOT_ARRAY,{0}},
    supergroup_line_desc
};

char *superGroupTemplate(void)
{
    return dbContainerTemplate(supergroup_desc);
}

char *superGroupSchema(void)
{
    return dbContainerSchema(supergroup_desc, "Supergroups");
}

char *packageSuperGroup(Supergroup *sg)
{
    char* result = dbContainerPackage(supergroup_desc,(char*)sg);

    // write to file for debugging
    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\supergroup_todbserver.txt", "wb");
        if (f) {
            fwrite(result, 1, strlen(result), f);
            fclose(f);
        }
    }

    return result;
}


static void fixupSGDoor(Entity* e)
{
    if (e->supergroup && e->supergroup->activetask && e->supergroup->activetask->doorMapId)
    {
        DoorEntry* door = dbFindGlobalMapObj(e->supergroup->activetask->doorMapId, e->supergroup->activetask->doorPos);
        if (door) e->supergroup->activetask->doorId = door->db_id;
    }
}


static void fixupSGPrestigeAndInfluence(Entity *e)
{
    // sanity check on prestige
    if (e->supergroup->prestige > MAX_INFLUENCE
        || e->supergroup->prestige < -MAX_INFLUENCE) //if enough negative, probably overflow
    {
        e->supergroup->prestige = MAX_INFLUENCE;
    }

    // sanity check on influence
    if (e->supergroup->influence > MAX_INFLUENCE 
        || e->supergroup->influence < -MAX_INFLUENCE) //if enough negative, probably overflow
    {
        e->supergroup->influence = MAX_INFLUENCE;
    }
}

void unpackSupergroup(Entity *e, char *mem, int send_to_client)
{
    if (!e->supergroup) {
        e->supergroup = createSupergroup();
    } else {
        clearSupergroup(e->supergroup);
    }
    if (!e->supergroup->activetask)
        e->supergroup->activetask = storyTaskInfoAlloc();

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\supergroup_fromdbserver.txt", "wb");
        if (f) {
            fwrite(mem, 1, strlen(mem), f);
            fclose(f);
        }
    }

    sgroup_DbContainerUnpack(e->supergroup, mem);

#if SERVER || GAME
    fixSuperGroupMode(e);
    fixSuperGroupFields(e);    
    if( e->pl && e->pl->supergroup_mode )
        e->send_costume = true;
#endif

    // it is okay to not do this on the Raid/Statserver
#if SERVER
    fixupSGDoor(e);
#endif

    fixupSGPrestigeAndInfluence(e);

#if SERVER
    if (e->supergroup->activetask && e->supergroup->activetask->sahandle.context)
    {
        if (!storyTaskInfoInit(e->supergroup->activetask))
        {
            storyTaskInfoDestroy(e->supergroup->activetask);
            e->supergroup->activetask = NULL;
        }
    }
    else
    {
        storyTaskInfoDestroy(e->supergroup->activetask);
        e->supergroup->activetask = NULL;
    }
#endif
}

//----------------------------------------
//  Unpack the supergroup.
// This still allocs a storyarc, so make
// sure to call destroy when you are finished
//----------------------------------------
void sgroup_DbContainerUnpack(Supergroup *sg, char *mem)
{
    if(verify( sg ))
    {
        clearSupergroup(sg);

        if (!sg->activetask)
        {
            sg->activetask = storyTaskInfoAlloc();
        }

        dbContainerUnpack(supergroup_desc,mem,(char*)sg);    
    }    
}
