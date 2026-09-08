/*\
 *
 *    raidinfo.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Some information about the raid game saved to supergroups
 *
 */

#include "raidinfo.h"
#include "gameData/raidstruct.h"
#include "comm_backend.h"
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/error.h>
#include "iopserver.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include "raidserver.h"

static Packet* g_pak;
static U32 g_excludesgid;

void RaidInfoUpdate(U32 sgid, int deleting)
{
    SupergroupRaidInfo* raidinfo = cstoreGet(g_SupergroupRaidInfoStore, sgid);
    if (!raidinfo)
    {
        Errorf("RaidInfoUpdate called with invalid id %i", sgid);
        return;
    }

    if (!deleting)
    {
        char* data = cstorePackage(g_SupergroupRaidInfoStore, raidinfo, sgid);
        dbAsyncContainerUpdate(CONTAINER_SGRAIDINFO, sgid, CONTAINER_CMD_CREATE_MODIFY, data, 0);
        free(data);
    }
    else
        dbAsyncContainerUpdate(CONTAINER_SGRAIDINFO, sgid, CONTAINER_CMD_DELETE, "", 0);
}

void RaidInfoDestroy(U32 sgid)
{
    RaidInfoUpdate(sgid, 1);
    cstoreDelete(g_SupergroupRaidInfoStore, sgid);
}

SupergroupRaidInfo* RaidInfoGetAdd(U32 sgid)
{
    SupergroupRaidInfo* raidinfo = cstoreGet(g_SupergroupRaidInfoStore, sgid);
    if (!raidinfo)
    {
        raidinfo = cstoreGetAdd(g_SupergroupRaidInfoStore, sgid, 1);
        if (!raidinfo) return NULL;
        raidinfo->max_raid_size = BASERAID_MIN_RAID_SIZE;
        raidinfo->window_hour = BASERAID_DEFAULT_WINDOW;
        raidinfo->window_days = BASERAID_DEFAULT_WINDOW_DAYS;
        RaidInfoUpdate(sgid, 0);
    }
    return raidinfo;
}

void handleRequestSetWindow(Packet* pak, U32 listid, U32 cid)
{
    U32 sgid;
    U32 daybits;
    U32 hour;
    int dayofweek, numbits = 0;
    U32 daymask;
    SupergroupRaidInfo* info;

    sgid = pktGetBitsAuto(pak);
    daybits = pktGetBitsAuto(pak);
    hour = pktGetBitsAuto(pak);

    // check we have two bits exactly
    daymask = (1 << 7)-1;
    daybits &= daymask;
    for (dayofweek = 0; dayofweek < 7; dayofweek++)
    {
        if (daybits & (1 << dayofweek))
            numbits++;
    }
    if (numbits != 2)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "ServerError");
        return;
    }

    // TODO - check window hour against correct server cycle
    hour = hour % 24;    

    info = RaidInfoGetAdd(sgid);
    info->window_days = daybits;
    info->window_hour = hour;
    RaidInfoUpdate(sgid, 0);

    dbContainerSendReceipt(0, "RaidWindowSet");
}

void handleBaseSettings(Packet* pak, U32 listid, U32 cid)
{
    U32 sgid;
    SupergroupRaidInfo* info;
    int oldraid, oldmount;

    sgid = pktGetBitsAuto(pak);
    info = RaidInfoGetAdd(sgid);
    if (!info) 
    { //Error but don't crash
        dbContainerSendReceipt(0, NULL);
        return;
    }
    oldraid = info->max_raid_size;
    oldmount = info->open_mount;
    info->max_raid_size = pktGetBitsAuto(pak);
    info->open_mount = pktGetBits(pak, 1);

    // small optimization to avoid sending extra updates to dbserver
    if (oldraid != info->max_raid_size || oldmount != info->open_mount)
    {
        //If you don't have a mount, delete all you raids
        if( !info->open_mount )
        {
            RemoveSGsScheduledRaids( sgid, true, false, true ); //rm attacks w refund
        }
        RaidInfoUpdate(sgid, 0);
    }
    dbContainerSendReceipt(0, NULL);
}

int RaidListSend(SupergroupRaidInfo* info, U32 id)
{
    int length, first, second;
    SupergroupRaidInfo sendinfo;

    if (id == g_excludesgid)    // skip excluded sg
        return 1;
    if (!SGCanBeRaided(info->id, 0, 0, 0, NULL))
        return 1;

    // calculate raid window length
    length = NumItemsOfPower(info->id) * BASERAID_WINDOW_PER_ITEM;
    if (length > BASERAID_MAX_WINDOW)
        length = BASERAID_MAX_WINDOW;
    info->window_length = length;
    if (!info->window_length)    // no items
        return 1;

    // determine what windows are open
    sendinfo = *info;
    SGGetOpenRaidWindows(sendinfo.id, &first, &second);
    if (!first && !second)
        return 1;                // no windows open
    sendinfo.window_days = SupergroupRaidInfoWindowMask(sendinfo.window_days, !first, !second);

    pktSendBits(g_pak, 1, 1);    // another info coming
    SupergroupRaidInfoSend(g_pak, &sendinfo);
    return 1;
}
static void RaidListSendAdapter(void* arg0, U32 arg1)
{
    RaidListSend((SupergroupRaidInfo*)arg0, (U32)arg1);
}

void handleRequestRaidList(Packet* pak, U32 listid, U32 cid)
{
    U32 dbid, sgid;
    Packet* ret;
    SupergroupRaidInfo* info;

    dbid = pktGetBitsAuto(pak);
    sgid = pktGetBitsAuto(pak);

    // send list of all supergroup infos
    ret = dbContainerCreateReceipt(0, NULL);
    g_pak = ret;

    info = RaidInfoGetAdd(sgid);
    g_excludesgid = 0;
    RaidListSend(info, sgid); // send sgid first
    g_excludesgid = sgid;
    cstoreForEach(g_SupergroupRaidInfoStore, RaidListSendAdapter); // then every other sg
    
    pktSendBits(ret, 1, 0);        // done with infos
    pktSend(&ret, &db_comm_link);
}

void handleNotifyVillainSG(Packet* pak, U32 listid, U32 cid)
{
    U32 sgid;
    U32 playertype;
    SupergroupRaidInfo* info;

    sgid = pktGetBitsAuto(pak);
    playertype = pktGetBits(pak, 1);

    info = RaidInfoGetAdd(sgid);
    info->playertype = playertype;
    RaidInfoUpdate(sgid, 0);
    dbContainerSendReceipt(0, 0);
}

// returns next two open raid windows for supergroup (if there are any)
static int g_haveraid;
static U32 g_defendingsg;
static U32 g_windowstart;
static U32 g_windowend;
static int checkWindow(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if (baseraid->defendersg == g_defendingsg &&
        baseraid->time >= g_windowstart &&
        baseraid->time < g_windowend)
    {
        g_haveraid = 1;
        return 0; // stop checking
    }
    return 1;
}
static void checkWindowAdapter(void* arg0, U32 arg1)
{
    checkWindow((ScheduledBaseRaid*)arg0, (U32)arg1);
}

int SGWindowIsScheduled(U32 sgid, U32 windowstart, U32 windowend)
{
    g_defendingsg = sgid;
    g_windowstart = windowstart;
    g_windowend = windowend;
    g_haveraid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, checkWindowAdapter);
    return g_haveraid;
}
void SGGetOpenRaidWindows(U32 sgid, U32* first, U32* second)
{
    SupergroupRaidInfo* info = RaidInfoGetAdd(sgid);

    // basic checks first
    *first = *second = 0;
    if (!info->window_length)
        return;

    // check each window
    SupergroupRaidInfoGetWindowTimes(info, timerSecondsSince2000(), first, second);
    if (*first && SGWindowIsScheduled(sgid, *first, *first + info->window_length*3600))
        *first = 0;
    if (*second && SGWindowIsScheduled(sgid, *second, *second + info->window_length*3600))
        *second = 0;
}

int SGHasOpenMount(U32 sgid)
{
    SupergroupRaidInfo* info = RaidInfoGetAdd(sgid);
    return info->open_mount;
}

// This is only used to verify scheduled raids
int SGCanBeRaided(U32 sgid, U32 attackersg, U32 time, U32 raidsize, char* err)
{
#define FAIL(str) { if (err) strcpy(err, str); return 0; }
    U32 window1, window2;
    int sgiop = NumItemsOfPower(sgid);
    int attiop = attackersg? NumItemsOfPower(attackersg): 0;
    SupergroupRaidInfo* oppInfo = RaidInfoGetAdd(sgid);

    if (!sgiop)
        FAIL("CantRaidNoItems");
    if (attiop > sgiop)
        FAIL("CantRaidMoreItems");
    if(!isScheduledRaidsAllowed())
        FAIL("RaidTimeIsNotAllowed");

    if (time)
    {
        int found = 0;
        time = timerClampToHourSS2(time, 1);
        SGGetOpenRaidWindows(sgid, &window1, &window2);
        if (window1 && time >= window1 && time < window1 + oppInfo->window_length*3600)
            found = 1;
        if (window2 && time >= window2 && time < window2 + oppInfo->window_length*3600)
            found = 1;
        if (!found)
            FAIL("CantRaidTime");

        // LH: Design change, can't schedule a raid during a time that overlaps with your window
        if (attackersg)
        {
            SupergroupRaidInfo* attackerInfo = RaidInfoGetAdd(attackersg);
            int overlap = 0;

            SupergroupRaidInfoGetWindowTimes(attackerInfo, timerSecondsSince2000(), &window1, &window2);
            if (window1 && time >= window1 && time < window1 + attackerInfo->window_length*3600)
                overlap = 1;
            if (window2 && time >= window2 && time < window2 + attackerInfo->window_length*3600)
                overlap = 1;
            if (overlap)
                FAIL("CantRaidOverlap");
        }
    }
    if (raidsize)
    {
        if (raidsize > oppInfo->max_raid_size)
            FAIL("CantRaidSizeLarge");
    }
    if (err)
        err[0] = 0;
    return 1;
#undef FAIL
}
