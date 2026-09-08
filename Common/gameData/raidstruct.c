/*\
 *
 *    raidstruct.h/c - Copyright 2004-2006 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Common structures for supergroup bases and base raids
 *
 */

#include "gameData/raidstruct.h"
#include <utilitieslib/utils/timing.h>
#include <stdio.h>
#include <time.h>
#include <utilitieslib/components/MemoryPool.h>
#include "container/dbcontainerpack.h"
#include "comm_backend.h"
#include <utilitieslib/components/earray.h>


char * stringFromGameState( eIoPGameState state )
{
    static char iopGameState[300];
    iopGameState[0] = 0;

    if( state == IOP_GAME_NEVER_STARTED )
        strcat( iopGameState, "NotStarted ");
    if( state & IOP_GAME_CATHEDRAL_OPEN )
        strcat( iopGameState, "CathedralOpen ");
    if( state & IOP_GAME_CATHEDRAL_CLOSED )
        strcat( iopGameState, "CathedralClosed ");
    if( state & IOP_GAME_NOT_RUNNING)
        strcat( iopGameState, "Not Running ");
    if( state & IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS ) //This is the only one state allowed to be ored
        strcat( iopGameState, "DEBUG_ALLOW_RAIDS_AND_TRIALS ");
    
    return iopGameState;
}
char* SGBaseConstructInfoString(int sgid)
{
    static char buf[100];
    sprintf(buf, "B:%i:0", sgid);
    return buf;
}

int SGBaseDeconstructInfoString(char* mapinfo, int *sgid, int *userid)
{
    return 2 == sscanf(mapinfo, "B:%i:%i", sgid, userid);
}

int RaidDeconstructInfoString(char* mapinfo, int *sgid1, int *sgid2)
{
    return 2 == sscanf(mapinfo, "R:%i:%i", sgid1, sgid2);
}


// *********************************************************************************
//  ScheduledBaseRaid
// *********************************************************************************

LineDesc raidparticipant_line_desc[] =
{
    { PACKTYPE_CONREF,     CONTAINER_ENTS,        "DbId",             0,            },
        // Id for each participant
    { 0 },
};

StructDesc attacker_participant_desc[] =
{
    sizeof(U32),
    {AT_STRUCT_ARRAY, OFFSET(ScheduledBaseRaid, attacker_participants)},
    raidparticipant_line_desc,
    "TODO"
};

StructDesc defender_participant_desc[] =
{
    sizeof(U32),
    {AT_STRUCT_ARRAY, OFFSET(ScheduledBaseRaid, defender_participants)},
    raidparticipant_line_desc,
    "TODO"
};

LineDesc ScheduledBaseRaid_line_desc[] =
{
    {{ PACKTYPE_CONREF,            CONTAINER_SUPERGROUPS,            "AttackerSG",    OFFSET(ScheduledBaseRaid, attackersg)    },
        "attackers supergroup id"},
    {{ PACKTYPE_CONREF,            CONTAINER_SUPERGROUPS,            "DefenderSG",    OFFSET(ScheduledBaseRaid, defendersg)    },
        "defenders supergroup id"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "Time",            OFFSET(ScheduledBaseRaid, time)    },
        "time the raid will start (seconds since 2000)"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "Length",        OFFSET(ScheduledBaseRaid, length)    },
        "length of time for raid (seconds)"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "CompleteTime",    OFFSET(ScheduledBaseRaid, complete_time)    },
        "when the raid was completed (seconds since 2000)"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "ScheduledTime",OFFSET(ScheduledBaseRaid, scheduled_time)    },
        "when the raid was scheduled (seconds since 2000)"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "AttackersWon",    OFFSET(ScheduledBaseRaid, attackers_won)    },
        "1 if the attackers won the raid"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "Instant",        OFFSET(ScheduledBaseRaid, instant)    },
        "1 if the raid was an instant raid"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "MaxParticipants",    OFFSET(ScheduledBaseRaid, max_participants)    },
        "the maximum participants the raid will allow on each side"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "ForfeitChecked",    OFFSET(ScheduledBaseRaid, forfeit_checked)    },
        "1 if the raid has already been checked for player forfeit"},

    { PACKTYPE_SUB,    BASERAID_MAX_PARTICIPANTS,    "AttackerParticipants",    (intptr_t)attacker_participant_desc,                },
    { PACKTYPE_SUB,    BASERAID_MAX_PARTICIPANTS,    "DefenderParticipants",    (intptr_t)defender_participant_desc,                },
    { 0 },
};

StructDesc ScheduledBaseRaid_desc[] =
{
    sizeof(ScheduledBaseRaid),
    {AT_NOT_ARRAY,{0}},
    ScheduledBaseRaid_line_desc,
    "TODO"
};

CONTAINERSTRUCT_IMPL(ScheduledBaseRaid, CONTAINER_BASERAIDS);

int BaseRaidNumParticipants(ScheduledBaseRaid* raid, int attacker)
{
    U32 *parts = attacker? raid->attacker_participants: raid->defender_participants;
    int i, count = 0;
    for (i = 0; i < BASERAID_MAX_PARTICIPANTS; i++)
        if (parts[i])
            count++;
    return count;
}

int BaseRaidIsParticipant(ScheduledBaseRaid* raid, U32 dbid, int attacker)
{
    U32 *parts = attacker? raid->attacker_participants: raid->defender_participants;
    int i;
    for (i = 0; i < BASERAID_MAX_PARTICIPANTS; i++)
        if (parts[i] == dbid)
            return 1;
    return 0;
}

int BaseRaidAddParticipant(ScheduledBaseRaid* raid, U32 dbid, int attacker)
{
    U32 *parts = attacker? raid->attacker_participants: raid->defender_participants;
    int i;
    for (i = 0; i < BASERAID_MAX_PARTICIPANTS; i++)
        if (parts[i] == 0)
        {
            parts[i] = dbid;
            return 1;
        }
    return 0;
}

int BaseRaidRemoveParticipant(ScheduledBaseRaid* raid, U32 dbid, int attacker)
{
    U32 *parts = attacker? raid->attacker_participants: raid->defender_participants;
    int i;
    for (i = 0; i < BASERAID_MAX_PARTICIPANTS; i++)
        if (parts[i] == dbid)
        {
            parts[i] = 0;
            return 1;
        }
    return 0;
}

// BaseRaidForEachSorted - sorts into time order
static ScheduledBaseRaid** g_sortedraids;
static int reverseTimeSort(const void* leftData, const void* rightData)
{
    const ScheduledBaseRaid** left = (const ScheduledBaseRaid**)leftData;
    const ScheduledBaseRaid** right = (const ScheduledBaseRaid**)rightData;
    if ((*left)->time == (*right)->time)
        return 0;
    if ((*left)->time < (*right)->time)
        return 1;
    return -1;
}
static void sortRaids(ScheduledBaseRaid* raid, U32 raidid)
{
    int loc = (int)bfind(&raid, g_sortedraids, eaSize(&g_sortedraids), sizeof(ScheduledBaseRaid*), reverseTimeSort);
    eaInsert(&g_sortedraids, raid, loc);
}

static void sortRaidsAdapter(void* arg0, U32 arg1)
{
    sortRaids((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void BaseRaidForEachSorted(int (*func)(ScheduledBaseRaid*, U32))
{
    int i;
    if (!g_sortedraids)
        eaCreate(&g_sortedraids);
    eaSetSize(&g_sortedraids, 0);
    cstoreForEach(g_ScheduledBaseRaidStore, sortRaidsAdapter);
    for (i = eaSize(&g_sortedraids)-1; i >= 0; i--)
        func(g_sortedraids[i], g_sortedraids[i]->id);
}

void BaseRaidClearAll()
{
    cstoreClearAll( g_ScheduledBaseRaidStore );
}


static int sg_id;

static int attacker_count;
static void countAttackers(ScheduledBaseRaid* raid, U32 raidid)
{
    if( raid->attackersg == sg_id )
        attacker_count++;
}

static int defender_count;
static void countDefenders(ScheduledBaseRaid* raid, U32 raidid)
{
    if( raid->defendersg == sg_id )
        defender_count++;
}

static void countAttackersAdapter(void* arg0, U32 arg1)
{
    countAttackers((ScheduledBaseRaid*)arg0, (U32)arg1);
}

int BaseRaidCountAttacking(int supergroup_id)
{
    attacker_count = 0;
    sg_id = supergroup_id;
    cstoreForEach(g_ScheduledBaseRaidStore, countAttackersAdapter);
    return attacker_count;
}

static void countDefendersAdapter(void* arg0, U32 arg1)
{
    countDefenders((ScheduledBaseRaid*)arg0, (U32)arg1);
}

int BaseRaidCountDefending(int supergroup_id)
{
    defender_count = 0;
    sg_id = supergroup_id;
    cstoreForEach(g_ScheduledBaseRaidStore, countDefendersAdapter);
    return defender_count;
}


// *********************************************************************************
//  SupergroupRaidInfo
// *********************************************************************************

LineDesc SupergroupRaidInfo_line_desc[] =
{
    {{ PACKTYPE_INT,            SIZE_INT32,            "PlayerType",    OFFSET(SupergroupRaidInfo, playertype)    },
        "player type for supergroup - villain or hero"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "WindowHour",    OFFSET(SupergroupRaidInfo, window_hour)    },
        "what hour the supergroup raid window starts at (0-23, server time)"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "WindowDays",    OFFSET(SupergroupRaidInfo, window_days)    },
        "bitfield representing the days of the week the supergroup raid window is open (bit 0 = sunday)"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "OpenMount",    OFFSET(SupergroupRaidInfo, open_mount)    },
        "whether this supergroup has an open mount point and can conduct a raid"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "MaxRaidSize",    OFFSET(SupergroupRaidInfo, max_raid_size)    },
        "maximum raid size this supergroups base will support"},
    { 0 },
};

StructDesc SupergroupRaidInfo_desc[] =
{
    sizeof(SupergroupRaidInfo),
    {AT_NOT_ARRAY,{0}},
    SupergroupRaidInfo_line_desc,

    "TODO"
};

CONTAINERSTRUCT_IMPL(SupergroupRaidInfo, CONTAINER_SGRAIDINFO);

// somewhat optimized send/receive for client
void SupergroupRaidInfoSend(Packet* pak, SupergroupRaidInfo* info)
{
    pktSendBitsAuto(pak, info->id);
    pktSendBits(pak, 1, info->playertype);
    pktSendBits(pak, BASERAID_WINDOW_BITS, info->window_hour);
    pktSendBits(pak, BASERAID_DAYS_BITS, info->window_days);
    pktSendBits(pak, BASERAID_TEAMS_BITS, info->max_raid_size/BASERAID_TEAM_SIZE);
    pktSendBits(pak, BASERAID_WINDOW_BITS, info->window_length);
}

void SupergroupRaidInfoGet(Packet* pak, SupergroupRaidInfo* info)
{
    info->id = pktGetBitsAuto(pak);
    info->playertype = pktGetBits(pak, 1);
    info->window_hour = pktGetBits(pak, BASERAID_WINDOW_BITS);
    info->window_days = pktGetBits(pak, BASERAID_DAYS_BITS);
    info->max_raid_size = BASERAID_TEAM_SIZE*pktGetBits(pak, BASERAID_TEAMS_BITS);
    info->window_length = pktGetBits(pak, BASERAID_WINDOW_BITS);
}

void SupergroupRaidInfoGetWindowTimes(SupergroupRaidInfo* info, U32 fromtime, U32* first, U32* second)
{
    const U32 ONEDAY = 3600 * 24;
    const U32 ONEWEEK = ONEDAY * 7;
    int dayofweek, savedfirst = 0;
    U32 time;
    struct tm tms;

    // assumes we will in fact have two bits of window_days set
    *first = *second = 0;
    for (dayofweek = 0; dayofweek < 7; dayofweek++)
    {
        int mask = 1 << dayofweek; 
        if (mask & info->window_days)
        {
            // get the day of week I want, then rectify
            timerMakeTimeStructFromSecondsSince2000NoLocalization(fromtime, &tms); //tms = Server Time localized
            tms.tm_min = tms.tm_sec = tms.tm_isdst = 0;
            tms.tm_hour = info->window_hour;
            tms.tm_mday += -tms.tm_wday + dayofweek;    
            if (tms.tm_mday < 1) 
                tms.tm_mday += 7;
            if (tms.tm_mday > timerDaysInMonth(tms.tm_mon, tms.tm_year + 1900)) 
            {
                tms.tm_mday -= timerDaysInMonth(tms.tm_mon, tms.tm_year + 1900);
                tms.tm_mon++;
                if (tms.tm_mon > 11)
                    tms.tm_mon = 0;

            }

            // require that start time is greater than 24 hours away, within next week
            time = timerGetSecondsSince2000FromTimeStructNoLocalization(&tms);
            if (time < fromtime + ONEDAY)
                time += ONEWEEK;
            if (time > fromtime + ONEDAY + ONEWEEK)
                time -= ONEWEEK;

            // save result
            if (!savedfirst)
            {
                *first = time;
                savedfirst = 1;
            }
            else
                *second = time;
        }
    }
}

// mask out the selected days
U32 SupergroupRaidInfoWindowMask(U32 window_days, int maskfirst, int masksecond)
{
    int dayofweek;
    int donefirst = 0;

    for (dayofweek = 0; dayofweek < 7; dayofweek++)
    {
        U32 mask = 1 << dayofweek;
        if (mask & window_days)
        {
            if (!donefirst)
            {
                if (maskfirst)
                    window_days &= ~mask;
                donefirst = 1;
            }
            else
            {
                if (masksecond)
                    window_days &= ~mask;
            }
        }
    }
    return window_days;
}

// just a bit rotate for the seven day bits
U32 RaidWindowRotate(U32 window_days, int direction)
{
    // rectify so we're rolling right
    while (direction < 0) direction += 7;
    while (direction > 7) direction -= 7;
    while (direction--)
    {
        window_days <<= 1;
        if (window_days & (1 << 7))
        {
            window_days &= ~(1 << 7);
            window_days |= 1;
        }
    }
    return window_days;
}

// *********************************************************************************
//  items of power game
// *********************************************************************************

LineDesc ItemOfPower_line_desc[] =
{
    {{ PACKTYPE_INT,            SIZE_INT32,                    "ItemID",                OFFSET(ItemOfPower, id)                    },
        "id number of this item of power"},
    {{ PACKTYPE_STR_UTF8,            SIZEOF2(ItemOfPower, name),    "ItemName",                OFFSET(ItemOfPower,    name),                },
        "full name for the item of power"},
    {{ PACKTYPE_CONREF,            CONTAINER_SUPERGROUPS,        "ItemSuperGroupOwner",    OFFSET(ItemOfPower, superGroupOwner)    },
        "supergroup id of supergroup that owns this item"},
    {{ PACKTYPE_INT,            SIZE_INT32,                    "ItemCreationTime",        OFFSET(ItemOfPower, creationTime)        },
        "when the item of power was created (seconds since 2000)"},
    { 0 },
};

StructDesc ItemOfPower_desc[] =
{
    sizeof(ItemOfPower),
    {AT_NOT_ARRAY,{0}},
    ItemOfPower_line_desc,

    "TODO"
};

CONTAINERSTRUCT_IMPL(ItemOfPower, CONTAINER_ITEMSOFPOWER);

LineDesc ItemOfPowerGame_line_desc[] =
{
    {{ PACKTYPE_INT,            SIZE_INT32,            "IOPGameID",            OFFSET(ItemOfPowerGame, id)    },
        "id number of item of power game"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "IOPGameStartTime",        OFFSET(ItemOfPowerGame, startTime) },
        "when this game started"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "IOPGameState",            OFFSET(ItemOfPowerGame, state)    },
        "the current state of the item of power game"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "RaidLength",            OFFSET(ItemOfPowerGame, raid_length)    },
        "the length of all scheduled raids on the server"},
    {{ PACKTYPE_INT,            SIZE_INT32,            "RaidSize",                OFFSET(ItemOfPowerGame, raid_size)    },
        "the default size of raids on the server"},
    { 0 },
};

StructDesc ItemOfPowerGame_desc[] =
{
    sizeof(ItemOfPowerGame),
    {AT_NOT_ARRAY,{0}},
    ItemOfPowerGame_line_desc,

    "TODO"
};

CONTAINERSTRUCT_IMPL(ItemOfPowerGame, CONTAINER_ITEMOFPOWERGAMES);

// *********************************************************************************
//  raid stats
// *********************************************************************************

char* g_raidstatnames[4] = 
{
    "defended",            // !isattacker, !attackerwins
    "faileddefend",        // !isattacker, attackerwins
    "losses",            // isattacker, !attackerwins
    "wins",                // isattacker, attackerwins
};
