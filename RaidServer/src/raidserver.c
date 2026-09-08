/*\
 *
 *    raidserver.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Handles central server functions for scheduled base raids
 *
 */

#include "raidserver.h"

#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include "entity/entVarUpdate.h"        // for PKT_BITS_TO_REP_DB_ID
#include <utilitieslib/network/sock.h>
#include <utilitieslib/language/MultiMessageStore.h>
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include "container/container_server.h"
#include "container/container_util.h"
#include "dbcomm/dbnamecache.h"
#include "iopserver.h"
#include "gameData/iopdata.h"
#include "raidinfo.h"
#include "cmdparse/cmdstatserver.h"
#include <time.h>
#include <utilitieslib/utils/file.h>
#include "dbcomm/logcomm.h"
#include <utilitieslib/utils/winutil.h>
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "container/containerSupergroup.h"
#include <utilitieslib/utils/log.h>

// we'll be automagically registered to serve the following containers:
static ContainerStore* storedefs[] =
{    g_ScheduledBaseRaidStore, 
    g_SupergroupRaidInfoStore,
    g_ItemOfPowerStore,
    g_ItemOfPowerGameStore,
    0,
};

// globals for main
#define RAIDSERVER_TICK_FREQ    10        // ticks/sec
U32        g_tickCount;

// for passing into ForEach stuff
static U32 param_sgid;
static U32 param_dbid;
static U32 *param_idlist;
static U32 param_time, param_time2;
static int param_int;
static int param_result;

// *********************************************************************************
//  db save & client notify stuff
// *********************************************************************************

// save to database and notify clients
void RaidUpdate(U32 raidid, int deleting)
{
    ContainerReflectInfo** reflectinfo = 0;
    ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, raidid);

    containerReflectAdd(&reflectinfo, CONTAINER_SUPERGROUPS, baseraid->attackersg, 0, deleting);
    containerReflectAdd(&reflectinfo, CONTAINER_SUPERGROUPS, baseraid->defendersg, 0, deleting);
    
    if (deleting)
    {
        dbContainerSendReflection(&reflectinfo, CONTAINER_BASERAIDS, NULL, &raidid, 1, CONTAINER_CMD_DELETE);
    }
    else
    {
        char* data = cstorePackage(g_ScheduledBaseRaidStore, baseraid, raidid);
        dbContainerSendReflection(&reflectinfo, CONTAINER_BASERAIDS, &data, &raidid, 1, CONTAINER_CMD_CREATE_MODIFY);
        free(data);
    }
    containerReflectDestroy(&reflectinfo);
}




void RaidDestroy(U32 raidid)
{
    RaidUpdate(raidid, 1);
    cstoreDelete(g_ScheduledBaseRaidStore, raidid);
}

U32 RaidAdd(U32 attackersg, U32 defendersg, U32 time, U32 length, U32 size, int instant)
{
    ScheduledBaseRaid* baseraid = cstoreAdd(g_ScheduledBaseRaidStore);
    if (baseraid)
    {
        baseraid->attackersg = attackersg;
        baseraid->defendersg = defendersg;
        baseraid->time = time;
        baseraid->length = length;
        baseraid->max_participants = size;
        baseraid->instant = instant;
        baseraid->scheduled_time = dbSecondsSince2000();
    }
    RaidUpdate(baseraid->id, 0);
    return baseraid->id;
}

U32 BaseRaidLength(void)
{
    ItemOfPowerGame* game = GetItemOfPowerGame();
    if (!game->raid_length)
        game->raid_length = BASERAID_DEFAULT_LENGTH;
    CLAMP(game->raid_length, MIN_BASERAID_LENGTH, MAX_BASERAID_LENGTH);
    return game->raid_length;
}

U32 BaseRaidSize(void)
{
    ItemOfPowerGame* game = GetItemOfPowerGame();
    if (!game->raid_size)
        game->raid_size = BASERAID_DEFAULT_SIZE;
    CLAMP(game->raid_size, 8, 32);
    return game->raid_size;
}

static int gatherSupergroups(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if (baseraid->attackersg == param_sgid ||
        baseraid->defendersg == param_sgid)
        eaiPush(&param_idlist, raidid);
    return 1;
}
static void gatherSupergroupsAdapter(void* arg0, U32 arg1)
{
    gatherSupergroups((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void PlayerFullUpdate(U32 sgid, U32 dbid, int add)
{
    int i, count;
    ContainerReflectInfo** reflect = 0;
    char** data = 0;

    // get all raids associated player's supergroup
    param_sgid = sgid;
    if (!param_idlist) eaiCreate(&param_idlist);
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ScheduledBaseRaidStore, gatherSupergroupsAdapter);
    count = eaiSize(&param_idlist);
    if (!count) return; // done

    // package all data
    eaCreate(&data);
    for (i = 0; i < eaiSize(&param_idlist); i++)
    {
        ScheduledBaseRaid *baseraid = cstoreGet(g_ScheduledBaseRaidStore, param_idlist[i]);
        eaPush(&data, cstorePackage(g_ScheduledBaseRaidStore, baseraid, param_idlist[i])); 
    }

    // send a reflection of all raids to player
    containerReflectAdd(&reflect, CONTAINER_ENTS, dbid, 0, !add);
    dbContainerSendReflection(&reflect, CONTAINER_BASERAIDS, data, param_idlist, eaiSize(&param_idlist), CONTAINER_CMD_REFLECT);

    // cleanup
    eaDestroyEx(&data, NULL);
    containerReflectDestroy(&reflect);
}

static int removeParticipant(ScheduledBaseRaid* baseraid, U32 raidid)
{
    int modified = 0;
    if (baseraid->attackersg == param_sgid)
        modified = BaseRaidRemoveParticipant(baseraid, param_dbid, 1);
    if (baseraid->defendersg == param_sgid)
        modified = BaseRaidRemoveParticipant(baseraid, param_dbid, 0);
    if (modified)
        RaidUpdate(baseraid->id, 0);
    return 1;
}
static void removeParticipantAdapter(void* arg0, U32 arg1)
{
    removeParticipant((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void PlayerLeavingSupergroup(U32 sgid, U32 dbid)
{
    param_sgid = sgid;
    param_dbid = dbid;
    cstoreForEach(g_ScheduledBaseRaidStore, removeParticipantAdapter);
}

// IN: sgid, int, time; OUT: time
static int checkTimeSlot(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if (baseraid->attackersg == param_sgid ||
        baseraid->defendersg == param_sgid)
    {
        if (baseraid->time > param_time-MAX_BASERAID_LENGTH && baseraid->time < param_time+MAX_BASERAID_LENGTH &&
            !baseraid->complete_time)
        {
            param_result = 1;
            return 0;
        }
    }
    else if (
        baseraid->attackersg == param_int ||
        baseraid->defendersg == param_int)
    {
        if (baseraid->time > param_time-MAX_BASERAID_LENGTH && baseraid->time < param_time+MAX_BASERAID_LENGTH &&
            !baseraid->complete_time)
        {
            param_result = 2;
            return 0;
        }
    }
    return 1;
}
static void checkTimeSlotAdapter(void* arg0, U32 arg1)
{
    checkTimeSlot((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void handleRaidChallenge(Packet* pak, U32 listid, U32 cid) // instant challenges only
{
    U32 sgid, othersg;
    U32 latest = timerSecondsSince2000();

    sgid = pktGetBitsPack(pak, 1);
    othersg = pktGetBitsPack(pak, 1);

    // schedule for 15 minutes from now
    if (sgid == othersg)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidSelf");
        return;
    }

    // schedule for the next available 8pm timeslot
    param_sgid = sgid;
    param_int = othersg;
    param_time = latest + 5 * 60;
    param_result = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, checkTimeSlotAdapter);
    if (param_result == 1)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidNoTimeToInvite");
        return;
    }
    else if (param_result == 2)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidTargetNoTime");
        return;
    }

    if( NumItemsOfPower( othersg ) < 1 && !SGHasOpenMount( othersg ) )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidTargetNotInstantRaidable");
        return;
    }

    // ok, schedule it
    RaidAdd(sgid, othersg, param_time, BaseRaidLength(), BaseRaidSize(), 1);
    dbContainerSendReceipt(0, "RaidChallengeAccepted");
}

void handleRaidSchedule(Packet* pak, U32 listid, U32 cid)
{
    Supergroup sg = {0};
    char err[1000];
    U32 sgid = pktGetBitsAuto(pak);
    U32 othersg = pktGetBitsAuto(pak);
    U32 time = pktGetBitsAuto(pak);
    U32 raidsize = pktGetBitsAuto(pak);


    if( !isScheduledRaidsAllowed() )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidTimeIsNotAllowed");
        return;
    }

    if( isCathdralOfPainOpenAtThisTime( time ) )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidTimeIsDuringCathedralOfPain");
        return;
    }

    if( !isCurrentGameRunningAtThisTime( time ) )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidTimeIsPastCurrentGame");
        return;
    }

    if (!SGHasOpenMount(sgid))
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidNoMountPoint");
        return;
    }
    time = timerClampToHourSS2(time, 1);
    if (!SGCanBeRaided(othersg, sgid, time, raidsize, err))
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, err);
        return;
    }

    // charge supergroup for prestige
    if ( !sgroup_LockAndLoad(sgid, &sg) )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidNotInSupergroup");
        return;
    }

    if (sg.prestige < BASERAID_SCHEDULE_COST)
    {
        sgroup_UpdateUnlock( sgid, &sg );
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidNoPrestige");
        return;
    }

    // all clear
    sg.prestige -= BASERAID_SCHEDULE_COST;
    sgroup_UpdateUnlock( sgid, &sg );
    RaidAdd(sgid, othersg, time, BaseRaidLength(), raidsize, 0);
    dbContainerSendReceipt(0, "RaidScheduled");    
}

void handleRaidCancel(Packet* pak, U32 listid, U32 cid)
{
    Supergroup sg = {0};
    ScheduledBaseRaid* raid;
    U32 sgid = pktGetBitsAuto(pak);
    U32 raidid = pktGetBitsAuto(pak);
    U32 now = timerSecondsSince2000();

    raid = cstoreGet(g_ScheduledBaseRaidStore, raidid);
    if (!raid)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidCancelNonexistent");
        return;
    }
    if (raid->attackersg != sgid)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidCancelWrongSG");
        // TODO - cheat log?
        return;
    }
    if (raid->time <= now)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidCancelStarted");
        return;
    }

    // all clear
    if (!raid->instant) // get a refund
    {
        if ( sgroup_LockAndLoad(sgid, &sg) )
        {
            sg.prestige += BASERAID_SCHEDULE_REFUND;
            sgroup_UpdateUnlock(sgid, &sg);
        }
    }
    RaidDestroy(raidid);
    dbContainerSendReceipt(0, "RaidCancelled");
}

// delete any raids relating to this supergroup
int supergroupDeleteIter(ScheduledBaseRaid* raid, U32 raidid)
{
    if (raid->attackersg == param_sgid ||
        raid->defendersg == param_sgid)
    {
        RaidDestroy(raidid);
    }
    return 1;
}
static void supergroupDeleteIterAdapter(void* arg0, U32 arg1)
{
    supergroupDeleteIter((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void handleSupergroupDelete(U32 sgid)
{
    param_sgid = sgid;
    cstoreForEach(g_ScheduledBaseRaidStore, supergroupDeleteIterAdapter);
    RaidInfoDestroy(sgid);
}

void handleBaseUpdate(Packet* pak, U32 listid, U32 cid)
{
    ContainerReflectInfo** reflect = 0;
    char** data = 0;
    int i, count;

    // gather raids affecting this supergroup
    param_sgid = pktGetBitsPack(pak, 1);
    if (!param_idlist) eaiCreate(&param_idlist);
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ScheduledBaseRaidStore, gatherSupergroupsAdapter);
    count = eaiSize(&param_idlist);

    // send update, even if zero
    eaCreate(&data);
    for (i = 0; i < eaiSize(&param_idlist); i++)
    {
        ScheduledBaseRaid *baseraid = cstoreGet(g_ScheduledBaseRaidStore, param_idlist[i]);
        eaPush(&data, cstorePackage(g_ScheduledBaseRaidStore, baseraid, param_idlist[i])); 
    }

    // send a reflection of all raids to mapserver
    containerReflectAdd(&reflect, REFLECT_LOCKID, dbRelayLockId(), 0, 0); // return to sender
    dbContainerSendReflection(&reflect, CONTAINER_BASERAIDS, data, param_idlist, eaiSize(&param_idlist), CONTAINER_CMD_REFLECT);

    // cleanup
    eaDestroyEx(&data, NULL);
    containerReflectDestroy(&reflect);
    dbContainerSendReceipt(0, 0);
}

// message from mapserver that raid has completed
void handleBaseComplete(Packet* pak, U32 listid, U32 cid)
{
    ScheduledBaseRaid* raid;
    U32 defendersg = pktGetBitsAuto(pak);
    int    attackers_won = pktGetBits(pak, 1);
    char why[1000];
    char nameOfItemOfPowerCaptured[1000];
    char timestr[1000];
    int creationTimeOfItemOfPowerCaptured;

    strncpyt(why, pktGetString(pak), 1000);
    strncpyt(nameOfItemOfPowerCaptured, pktGetString(pak), 1000);
    creationTimeOfItemOfPowerCaptured = pktGetBitsAuto( pak );

    raid = cstoreGet(g_ScheduledBaseRaidStore, cid);
    if (!raid || raid->defendersg != defendersg)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "ServerError");
        return;
    }
    if (raid->complete_time)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "ServerError");
        return;
    }
    raid->complete_time = dbSecondsSince2000();
    raid->attackers_won = attackers_won;
    RaidUpdate(raid->id, 0);
    dbContainerSendReceipt(0, 0);

    //If the attacker won, transfer the captured Item of Power, or a random one if none specified
    if ( raid->attackers_won && !raid->instant )
    {
        int iopID;
        iopID = getItemOfPowerFromSGByNameAndCreationTime( raid->defendersg, nameOfItemOfPowerCaptured, creationTimeOfItemOfPowerCaptured );

        if( !iopID )
            iopID = getRandomIopFromSG( raid->defendersg );

        if( iopID )
            iopTransfer( raid->defendersg, raid->attackersg, iopID );
    }

    // send stat updates
    if (!raid->instant)
    {
        char whyUnderscored[1000];
        Strncpyt(whyUnderscored, why);
        strchrReplace(whyUnderscored,' ','_');

        SgrpStat_SendPassthru(0, raid->attackersg, "sgstat_statadj sg.group.%s.raid 1", RAID_STAT(1, raid->attackers_won));
        SgrpStat_SendPassthru(0, raid->attackersg, "sgstat_statadj sg.group.%s.raid.%s 1", RAID_STAT(1, raid->attackers_won), whyUnderscored);

        SgrpStat_SendPassthru(0, raid->defendersg, "sgstat_statadj sg.group.%s.raid 1", RAID_STAT(0, raid->attackers_won));
        SgrpStat_SendPassthru(0, raid->defendersg, "sgstat_statadj sg.group.%s.raid.%s 1", RAID_STAT(0, raid->attackers_won), whyUnderscored);
    }

    // log the result
    timerMakeOffsetStringFromSeconds(timestr, raid->complete_time - raid->time);
    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "Base Raid %i complete: %s attacking %s, %s in %s, %s",
        raid->id, dbSupergroupNameFromId(raid->attackersg), dbSupergroupNameFromId(raid->defendersg),
        raid->attackers_won? "attackers won": "defenders won",
        timestr,
        why);
}

U32 TryBaseParticipate(U32 raidid, U32 sgid, U32 dbid, U32 member_since, int join, char errmsg[1000])
{
    ScheduledBaseRaid* raid;
    int attacker;

    raid = cstoreGet(g_ScheduledBaseRaidStore, raidid);
    if (!raid || (raid->attackersg != sgid && raid->defendersg != sgid))
    {
        strncpyt(errmsg, "RaidParticipateNonexistent", 1000);
        return CONTAINER_ERR_CANT_COMPLETE;
    }
    //if (now >= raid->time)
    //{
    //    dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidParticipateStarted");
    //    return;
    //}
    if (member_since > raid->scheduled_time)
    {
        strncpyt(errmsg, "RaidParticipateNotMember", 1000);
        return CONTAINER_ERR_CANT_COMPLETE;
    }
    attacker = raid->attackersg == sgid;
    if (BaseRaidIsParticipant(raid, dbid, attacker))
    {
        if (join)
        {
            strncpyt(errmsg, "RaidParticipateSuccess", 1000);
            return CONTAINER_ERR_CANT_COMPLETE;
        }
        BaseRaidRemoveParticipant(raid, dbid, attacker);
        RaidUpdate(raid->id, 0);
        strncpyt(errmsg, "RaidUnparticipateSuccess", 1000);
    }
    else // not already a participant
    {
        if (!join)
        {
            strncpyt(errmsg, "RaidUnparticipateSuccess", 1000);
            return CONTAINER_ERR_CANT_COMPLETE;
        }
        if (BaseRaidNumParticipants(raid, attacker) >= raid->max_participants)
        {
            strncpyt(errmsg, "RaidParticipateFull", 1000);
            return CONTAINER_ERR_CANT_COMPLETE;
        }
        BaseRaidAddParticipant(raid, dbid, attacker);
        RaidUpdate(raid->id, 0);
        strncpyt(errmsg, "RaidParticipateSuccess", 1000);
    }
    return 0;
}

void handleBaseParticipate(Packet* pak, U32 listid, U32 cid)
{
    U32 now = dbSecondsSince2000();
    U32 sgid = pktGetBitsAuto(pak);
    U32 dbid = pktGetBitsAuto(pak);
    U32 member_since = pktGetBitsAuto(pak);
    int join = pktGetBits(pak, 1);
    U32 err;
    char errmsg[1000];
    char transfer[1000];
    Packet* ret;

    // wrap up the error checking so we can send a receipt with the same transfer
    // string back
    strncpyt(transfer, pktGetString(pak), 1000);
    err = TryBaseParticipate(cid, sgid, dbid, member_since, join, errmsg);
    ret = dbContainerCreateReceipt(err, errmsg);
    pktSendString(ret, transfer);
    pktSend(&ret, &db_comm_link);
}

///////////////////////////////////////////////////////////////////////

static int checkOutdated(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if (baseraid->time < param_time)
        RaidDestroy(raidid);
    return 1;
}
static void checkOutdatedAdapter(void* arg0, U32 arg1)
{
    checkOutdated((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void CheckOutdatedRaids(void)
{
    param_time = dbSecondsSince2000() - (1.25 * MAX_BASERAID_LENGTH); // get rid of it after one hour and fifteen minutes
    cstoreForEach(g_ScheduledBaseRaidStore, checkOutdatedAdapter);
}

///////////////////////////////////////////////////////////////////////

static int *entsOnline = NULL;
static int *entsOffline = NULL;
static int netpacketcallback_OnlineEnts(Packet *pak, int cmd, NetLink *link)
{
    int count = pktGetBitsAuto(pak);
    int i;
    eaiClear(&entsOnline);
    eaiClear(&entsOffline);
    for( i = 0; i < count; ++i ) 
    {
        int dbid = pktGetBitsAuto(pak);
        bool online = pktGetBitsPack(pak, 1);
        if( online )
        {
            eaiPush(&entsOnline,dbid);
        }
        else
        {
            eaiPush(&entsOffline,dbid);
        }
    }
    return 1;
}

int* reqSomeOnlineEnts(int *ids, bool wantOnline)
{
    static PerformanceInfo* perfInfo;
    int i;
    int count = eaiSize( &ids );
    Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_REQ_SOME_ONLINE_ENTS);
#if defined(_M_X64)
    #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBitsPack( pak, 1, (U32)(uintptr_t)netpacketcallback_OnlineEnts);
    pktSendBitsAuto( pak, count);
    for(i = 0; i < count; ++i )
    {
        pktSendBitsAuto(pak,ids[i]);
    }
    pktSend(&pak, &db_comm_link);
    if(dbMessageScanUntil(__FUNCTION__, &perfInfo))
    {
        if( wantOnline )
        {
            return entsOnline;
        }
        else
        {
            return entsOffline;
        }
    }
    else
    {
        return NULL;
    }
}

static void checkSideForfeit(ScheduledBaseRaid* baseraid, int *parts, int attacker)
{
    int i, size;
    int *activeArray;

    eaiCreate(&activeArray);

    for (i = 0; i < BASERAID_MAX_PARTICIPANTS; i++)
    {
        if (parts[i] != 0)
            eaiPush(&activeArray, parts[i]);
    }
    reqSomeOnlineEnts(activeArray, false);

    // loop through all players to determine who should forfeit slots
    size = eaiSize(&entsOffline);
    for (i = 0; i < size; i++)
    {
        if (entsOffline[i])
        {
            // if not online
            BaseRaidRemoveParticipant(baseraid, entsOffline[i], attacker);
        }
    }

    eaiDestroy(&activeArray);
}

static int checkForfeit(ScheduledBaseRaid* baseraid, U32 raidid)
{

    if (baseraid->forfeit_checked == 0) 
    {
        if (baseraid->time < param_time) 
        {
            // check attackers
            checkSideForfeit(baseraid, baseraid->attacker_participants, 1);

            // check defenders
            checkSideForfeit(baseraid, baseraid->defender_participants, 0);

            // set it to checked
            baseraid->forfeit_checked = 1;

            // update everyone
            RaidUpdate(baseraid->id, 0);
        }
    }
    return 1;
}
static void checkForfeitAdapter(void* arg0, U32 arg1)
{
    checkForfeit((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void CheckForfeitPlayers(void)
{
    param_time = dbSecondsSince2000() + (5 * 60);    // forfeit players who aren't on line 5 mintues before raid begins

    cstoreForEach(g_ScheduledBaseRaidStore, checkForfeitAdapter);
}

///////////////////////////////////////////////////////////////////////

static int RaidDestroyWithRefund( ScheduledBaseRaid * raid, bool issueRefund )
{
    if (!raid->instant && issueRefund) 
    {
        Supergroup sg = {0};
        if ( sgroup_LockAndLoad(raid->attackersg, &sg) )
        {
            sg.prestige += BASERAID_SCHEDULE_REFUND;
            sgroup_UpdateUnlock(raid->attackersg, &sg);
        }
    }
    RaidDestroy(raid->id);

    return 1;
}

//// RemoveSGsScheduledRaids //////////////////////////////////////////////////////////
static int g_pmpSG;
static bool g_pmpRefund;
static bool g_pmpRemoveAttacks;
static bool g_pmpRemoveDefends;

int RaidDestroySGCallback( ScheduledBaseRaid* raid, U32 raidid )
{
    if ( raid->time > dbSecondsSince2000() ) {
        if ( raid->attackersg == g_pmpSG && g_pmpRemoveAttacks )
        {
            RaidDestroyWithRefund( raid, g_pmpRefund );
        }
        if ( raid->defendersg == g_pmpSG && g_pmpRemoveDefends )
        {
            RaidDestroyWithRefund( raid, g_pmpRefund );
        }
    }
    return 1;
}

static void RaidDestroySGCallbackAdapter(void* arg0, U32 arg1)
{
    RaidDestroySGCallback((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void RemoveSGsScheduledRaids(U32 sgid, int removeAttacks, bool removeDefends, bool issueRefund )
{
    g_pmpSG = sgid;
    g_pmpRefund = issueRefund;
    g_pmpRemoveAttacks = removeAttacks;
    g_pmpRemoveDefends = removeDefends;
    cstoreForEach( g_ScheduledBaseRaidStore, RaidDestroySGCallbackAdapter);
}

//// RemoveAllScheduledRaids //////////////////////////////////////////////////////////
static g_pmpAllRefund;

static int RaidDestroyCallback(ScheduledBaseRaid* raid, U32 raidid)
{
    RaidDestroyWithRefund( raid, g_pmpAllRefund );
    return 1;
}

static void RaidDestroyCallbackAdapter(void* arg0, U32 arg1)
{
    RaidDestroyCallback((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void RemoveAllScheduledRaids( int issueRefund )
{
    g_pmpAllRefund = issueRefund;
    cstoreForEach( g_ScheduledBaseRaidStore, RaidDestroyCallbackAdapter);
}

//// VerifyExistingRaids //////////////////////////////////////////////////////////
static int RaidVerifyCallback(ScheduledBaseRaid* raid, U32 raidid)
{
    if ( raid->time > dbSecondsSince2000() ) 
    {
        if( NumItemsOfPower( raid->attackersg ) > NumItemsOfPower( raid->defendersg ) )
            RaidDestroyWithRefund( raid, true );
    }

    return 1;
}

static void RaidVerifyCallbackAdapter(void* arg0, U32 arg1)
{
    RaidVerifyCallback((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void VerifyAllExistingRaids()
{
    cstoreForEach( g_ScheduledBaseRaidStore, RaidVerifyCallbackAdapter);
}

// *********************************************************************************
//  container handling
// *********************************************************************************

void handleSetVar(Packet* pak, U32 listid, U32 cid)
{
    char buf[2000];
    char* var = pktGetString(pak);

    if (stricmp(var, "raidlength")==0)
    {
        ItemOfPowerGame* game = GetItemOfPowerGame();
        game->raid_length = 60*atoi(pktGetString(pak));
        CLAMP(game->raid_length, MIN_BASERAID_LENGTH, MAX_BASERAID_LENGTH);
        ItemOfPowerGameUpdate();
        sprintf(buf, "Raid length adjusted to %i", game->raid_length);
        dbContainerSendReceipt(0, buf);
    }
    else if (stricmp(var, "raidsize")==0)
    {
        ItemOfPowerGame* game = GetItemOfPowerGame();
        game->raid_size = atoi(pktGetString(pak));
        CLAMP(game->raid_size, 8, 32);
        while (game->raid_size % 8) game->raid_size--;
        ItemOfPowerGameUpdate();
        sprintf(buf, "Raid size adjusted to %i", game->raid_size);
        dbContainerSendReceipt(0, buf);
    }
    else
    {
        sprintf(buf, "Unknown variable: %s", var);
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, buf);
    }
}

void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid)
{
    switch (cmd)
    {
        xcase RAIDCLIENT_RAID_CHALLENGE:
            handleRaidChallenge(pak, listid, cid);
        xcase RAIDCLIENT_BASE_UPDATE:
            handleBaseUpdate(pak, listid, cid);
        xcase RAIDCLIENT_RAID_COMPLETE:
            handleBaseComplete(pak, listid, cid);
        xcase RAIDCLIENT_RAID_PARTICIPATE:
            handleBaseParticipate(pak, listid, cid);
        xcase RAIDCLIENT_RAID_REQ_LIST:
            handleRequestRaidList(pak, listid, cid);
        xcase RAIDCLIENT_RAID_SET_WINDOW:
            handleRequestSetWindow(pak, listid, cid);
        xcase RAIDCLIENT_RAID_BASE_SETTINGS:
            handleBaseSettings(pak, listid, cid);
        xcase RAIDCLIENT_RAID_SCHEDULE:
            handleRaidSchedule(pak, listid, cid);
        xcase RAIDCLIENT_RAID_CANCEL:
            handleRaidCancel(pak, listid, cid);
        xcase RAIDCLIENT_NOTIFY_VILLAINSG:
            handleNotifyVillainSG(pak, listid, cid);
        xcase RAIDCLIENT_ITEM_OF_POWER_GRANT_NEW:
            handleItemOfPowerGrantNew(pak, listid, cid);
        xcase RAIDCLIENT_ITEM_OF_POWER_GRANT:
            handleItemOfPowerGrant(pak, listid, cid);
        xcase RAIDCLIENT_ITEM_OF_POWER_TRANSFER:
            handleItemOfPowerTransfer(pak, listid, cid);
        xcase RAIDCLIENT_ITEM_OF_POWER_SG_SYNCH:
            handleItemOfPowerSGSynch(pak, listid, cid);
        xcase RAIDCLIENT_ITEM_OF_POWER_GAME_SET_STATE:
            handleItemOfPowerGameSetState(pak, listid, cid);
        xcase RAIDCLIENT_ITEM_OF_POWER_GAME_REQUEST_UPDATE:
            handleItemOfPowerGameRequestUpdate(pak, listid, cid);
        xcase RAIDCLIENT_SETVAR:
            handleSetVar(pak, listid, cid);
        xcase RAIDCLIENT_SET_LOG_LEVELS:
            {
                int i;
                for( i = 0; i < LOG_COUNT; i++ )
                {
                    int log_level = pktGetBitsPack(pak,1);
                    logSetLevel(i,log_level);
                }
            }
        xcase RAIDCLIENT_TEST_LOG_LEVELS:
            {
                LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "RaidServer Log Line Test: Alert" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "RaidServer Log Line Test: Important" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "RaidServer Log Line Test: Verbose" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "RaidServer Log Line Test: Deprecated" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "RaidServer Log Line Test: Debug" );
            }
        xdefault:
            printf("Unknown command %d\n", cmd);
            return;
    }
}

void dealWithContainerReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str, Packet* pak) 
{
    // don't have any receipts we care about
}

void dealWithContainerReflection(int list_id, int *ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo)
{
    // don't have any reflections we need
}

// players get notification of any base raids affecting their supergroup
void dealWithNotify(U32 list_id, U32 cid, U32 dbid, int add)
{
    if (list_id == CONTAINER_SUPERGROUPS)
    {
        if (!dbid) // supergroup itself
        {
            if (!add) // supergroup being deleted
            {
                handleSupergroupDelete(cid);
                handleSupergroupDeleteForItemsOfPower(cid);
            }
        }
        else // player adding or exiting supergroup, or logging on
        {
            PlayerFullUpdate(cid, dbid, add);
            PlayerFullItemOfPowerUpdate(cid, dbid, add);
            if (!add)
                PlayerLeavingSupergroup(cid, dbid);
        }
    } // CONTAINER_SUPERGROUPS
}

// *********************************************************************************
//  debug menu
// *********************************************************************************

static void RaidPrompt(void)
{
    printf("Enter the raid id #: ");
}

static void PrintRaidVars(void)
{
    printf("Raid Length: %i \tRaid Size %i\n", BaseRaidLength(), BaseRaidSize());
}

static void PrintCurrentTime(void)
{
    char buf[200];
    printf("Current Raid Time is:%s\n", timerMakeDateStringFromSecondsSince2000(buf, timerSecondsSince2000()));
}


static int printraid(ScheduledBaseRaid* baseraid, U32 raidid)
{
    char buf[200];
    printf("\t%i: %i vs. %i (of %i)  %s v. %s \t %s %s %s\n", baseraid->id, 
        BaseRaidNumParticipants(baseraid, 1),
        BaseRaidNumParticipants(baseraid, 0),
        baseraid->max_participants,
        dbSupergroupNameFromId(baseraid->attackersg),
        dbSupergroupNameFromId(baseraid->defendersg), 
        timerMakeDateStringFromSecondsSince2000(buf, baseraid->time),
        baseraid->complete_time? "Complete ": "",
        baseraid->complete_time? timerMakeDateStringFromSecondsSince2000(buf, baseraid->complete_time): "");
    return 1;
}
static void printraidAdapter(void* arg0, U32 arg1)
{
    printraid((ScheduledBaseRaid*)arg0, (U32)arg1);
}

static void ShowRaidList(void)
{
    printf("All raids:\n");
    cstoreForEach(g_ScheduledBaseRaidStore, printraidAdapter);
}
static void DetailRaid(char* raidstr)
{
    ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, atoi(raidstr));
    if (!baseraid)
        printf("  - Invalid id %s\n", raidstr);
    else
        printraid(baseraid, baseraid->id);
}

static int deleterandom(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if (!param_int--)
    {
        printf("Removing raid %i\n", raidid);
        RaidDestroy(raidid);
        return 0;
    }
    return 1;
}
static void deleterandomAdapter(void* arg0, U32 arg1)
{
    deleterandom((ScheduledBaseRaid*)arg0, (U32)arg1);
}

static void DeleteRandomRaid(void)
{
    int count = cstoreCount(g_ScheduledBaseRaidStore);
    if (!count)
    {
        printf("  - no more raids\n");
        return;
    }
    param_int = rand() % count;
    cstoreForEach(g_ScheduledBaseRaidStore, deleterandomAdapter);
}

static void RemoveRaid(char* param)
{
    ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, atoi(param));
    if (!baseraid)
        printf("  - Invalid id %s\n", param);
    else
    {
        printf("Removing raid %i\n", baseraid->id);
        RaidDestroy(baseraid->id);
    }
}

static void SetRaidNow(char* param)
{
    ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, atoi(param));
    if (!baseraid)
        printf("  - Invalid id %s\n", param);
    else
    {
        baseraid->time = dbSecondsSince2000();
        RaidUpdate(baseraid->id, 0);
    }
}

static void SetAttackersWon(char* param)
{
    ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, atoi(param));
    if (!baseraid)
        printf("  - Invalid id %s\n", param);
    else
    {
        baseraid->complete_time = dbSecondsSince2000();
        baseraid->attackers_won = 1;
        RaidUpdate(baseraid->id, 0);
    }
}

static void SwapRaidSides(char* param)
{
    ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, atoi(param));
    if (!baseraid)
        printf("  - Invalid id %s\n", param);
    else
    {
        U32 temp = baseraid->attackersg;
        baseraid->attackersg = baseraid->defendersg;
        baseraid->defendersg = temp;
        RaidUpdate(baseraid->id, 0);
    }
}

static void memPrompt(void) { printf("Type 'DUMP' to continue: "); }
static void memDump(char* param) { if (strcmp(param, "DUMP")==0) memCheckDumpAllocs(); }

ConsoleDebugMenu raidserverdebug[] =
{
    { 0,        "General commands:",            NULL,                            NULL        },
    { 'l',        "show raids",                    ShowRaidList,                    NULL        },
    { 'm',        "print memory usage",            memMonitorDisplayStats,            NULL        },
    { 'M',        "dump memory table",            memPrompt,                        memDump        },
    { 'p',        "pause the server",                ConsoleDebugPause,                NULL        },
    { 'r',        "delete a random raid",            DeleteRandomRaid,                NULL        },
    { 'v',        "print raid vars",                PrintRaidVars,                    NULL        },
    { 't',        "print current server time",    PrintCurrentTime,                NULL        },
    { 0,        "Raid-specific commands:",        NULL,                            NULL        },
    { 'A',        "complete & set attackers won",    RaidPrompt,                        SetAttackersWon    },
    { 'D',        "detailed info on a raid",        RaidPrompt,                        DetailRaid    },
    { 'N',        "set a raid to be now",            RaidPrompt,                        SetRaidNow  },
    { 'S',        "swap attacker/defender roles", RaidPrompt,                        SwapRaidSides },
    { 'R',        "remove a specific raid",        RaidPrompt,                        RemoveRaid    },
    { 0,        "Item of Power commands:",        NULL,                            NULL        },
    { 'i',        "Show All Items of Power",        ShowAllItemsOfPower,            NULL        },

    { 0, NULL, NULL, NULL },
};

// *********************************************************************************
//  main loop
// *********************************************************************************

void UpdateConsoleTitle(void)
{
    char buf[200];
    sprintf(buf, "RaidServer - %i raids, %i items of power, %i supergroups %s (%i)", 
        cstoreCount(g_ScheduledBaseRaidStore),
        cstoreCount(g_ItemOfPowerStore),
        cstoreCount(g_SupergroupRaidInfoStore),
        db_comm_link.connected? "": " DISCONNECTED",
        (g_tickCount / RAIDSERVER_TICK_FREQ) % 10);
    setConsoleTitle(buf);
}

int main(int argc,char **argv)
{
int        i,timer;

    EXCEPTION_HANDLER_BEGIN 
    setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);
    memMonitorInit();
    for(i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");

    setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'R', 0x8080ff);

    FolderCacheChooseMode();

    preloadDLLs(0);

    if (fileIsUsingDevData()) {
        bsAssertOnErrors(true);
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            (!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0));
    } else {
        // In production mode on the servers we want to save all dumps and do full dumps
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            ASSERTMODE_FULLDUMP |
            ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    }

    srand((unsigned int)time(NULL));
    consoleInit(110, 128, 0);
    UpdateConsoleTitle();

    logSetDir("raidserver");
    sockStart();
    serverCfgLoad();

    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-db")==0)
        {
            strcpy(server_cfg.db_server,argv[++i]);
            if(!server_cfg.log_server)
                strcpy(server_cfg.log_server,server_cfg.db_server);
        }
        else if(0==stricmp(argv[i], "-logserver"))
        {
            strcpy(server_cfg.log_server,argv[++i]);
        }
        else
        {
            Errorf("-----INVALID PARAMETER: %s\n", argv[i]);
        }
    }
    strcpy(db_state.server_name, server_cfg.db_server);
    Strncpyt(db_state.log_server, server_cfg.log_server); // copy the logserver
    Strncpyt(db_state.map_name, "RaidServer");

    MakeDummyDetailMP(); //For loading Supergroups without def files loaded
    LoadItemOfPowerInfos();

    loadstart_printf("Networking startup...");
    timer = timerAlloc();
    timerStart(timer);
    packetStartup(0,0);
    loadend_printf("");
    
    // container server startup
    loadstart_printf("Connecting to dbserver (%s)..", db_state.server_name);
    csvrInit(storedefs);
    csvrDbConnect();
    loadend_printf("");

    if(*server_cfg.log_server)
    {
        loadstart_printf("Connecting to logserver (%s)...", db_state.log_server);
        logNetInit();
        loadend_printf("done");
    }
    else
    {
        printf("no logserver specified. only printing logs locally.\n");
    }

    loadstart_printf("Registering with db...");
    if (!csvrRegisterSync())
        FatalErrorf("Could not register as container server");
    csvrGetNotifications((1 << CONTAINER_SUPERGROUPS)); // we only send stuff related to supergroups
    csvrLoadContainers();
    UpdateConsoleTitle();
    loadend_printf("");

    PrintRaidVars();
    printf("Ready.\n");
    CheckItemOfPowerGame(); //Get one tick to jump start game if needed

    for(;;)
    {
        dbComm();
        logFlush(false);
        NMMonitor(1);
        netIdle(&db_comm_link,1,10);
        DoConsoleDebugMenu(raidserverdebug);
        if (timerElapsed(timer) > (1.f / RAIDSERVER_TICK_FREQ))
        {
            // any period checks go here
            timerStart(timer);
            g_tickCount++;
            UpdateConsoleTitle();
            CheckOutdatedRaids();
            CheckForfeitPlayers();

            CheckItemOfPowerGame();
        }
    }
    EXCEPTION_HANDLER_END
}
