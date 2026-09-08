/*\
 *
 *    sgraidClient.h/c - Copyright 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Client UI stuff for supergroup raids
 *
 */

#include "sgraidClient.h"
#include "UI/uiSGRaidList.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/netio.h>
#include "UI/sprite/sprite_text.h"
#include "gameData/raidstruct.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiDialog.h"
#include "UI/uiChat.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "player/player.h"
#include "UI/uiFx.h"
#include "graphics/textureatlas.h"
#include "UI/uiStatus.h"
#include <utilitieslib/language/MessageStoreUtil.h>

static int g_showRaidString = 0;
static char* g_raidString;
static U32 g_raidTimeout;
static int g_raidTrophies;

StashTable g_sgdict;

void receiveSGRaidCompassString(Packet *pak)
{
    char* str = pktGetString(pak);

    free(g_raidString);
    if (str && str[0])
    {
        g_showRaidString = 1;
        g_raidString = strdup(str);
    }
    else
    {
        g_showRaidString = 0;
        g_raidString = NULL;
    }
    g_raidTimeout = pktGetBitsPack(pak, 1);    
    g_raidTrophies = pktGetBitsPack(pak, 4);
}

char* getSGRaidCompassString(void)
{
    static char raidstring[500];

    if (!g_showRaidString || !g_raidString)
        return NULL;
    strcpy(raidstring, textStd(g_raidString));

    // show the seconds after the raid string if required
    if (g_raidTimeout)
    {
        static char buf[20];
        U32 diff;
        U32 curtime = timerSecondsSince2000WithDelta();

        if (g_raidTimeout > curtime)
            diff = g_raidTimeout - curtime;
        else
            diff = 0;

        timerMakeOffsetStringFromSeconds(buf, diff);
        strcat(raidstring, " - ");
        strcat(raidstring, buf);
    }
    return raidstring;
}

char* getSGRaidInfoString(void)
{
    if (!g_showRaidString)
        return NULL;
    if (g_raidTrophies)
        return textStd(g_raidTrophies==1?"SGRaidTrophiesRemainSingle":"SGRaidTrophiesRemain", g_raidTrophies);
    return "";
}

// builds a dictionary of sg names
void addSGName(U32 sgid, char* sgname)
{
    char id[20];
    char* value;

    if (!g_sgdict)
        g_sgdict = stashTableCreateWithStringKeys(10, StashDeepCopyKeys | StashCaseSensitive );
    sprintf(id, "%i", sgid);
    stashFindPointer( g_sgdict, id, &value );
    if (value)
    {
        if (stricmp(value, sgname))
        {
            free(value);
            stashRemovePointer(g_sgdict, id, NULL);
            stashAddPointer(g_sgdict, id, strdup(sgname), false);
        }
        // otherwise, we're done
    }
    else
        stashAddPointer(g_sgdict, id, strdup(sgname), false);
}

char* getSGName(U32 sgid)
{
    char id[20];

    if (!g_sgdict) return NULL;
    sprintf(id, "%i", sgid);
    return stashFindPointerReturnPointer(g_sgdict, id);
}

void receiveSGRaidUpdate(Packet* pak)
{
    U32 id = pktGetBitsPack(pak, 1);
    int deleting = pktGetBits(pak, 1);
    U32 othersg;
    char* othersgname;

    if (!deleting)
    {
        ScheduledBaseRaid* baseraid = cstoreGetAdd(g_ScheduledBaseRaidStore, id, 1);
        char* data = pktGetString(pak);
        cstoreUnpack(g_ScheduledBaseRaidStore, baseraid, data, id);
        othersg = pktGetBitsPack(pak, 1);
        othersgname = pktGetString(pak);
        addSGName(othersg, othersgname);
    }
    else
        cstoreDelete(g_ScheduledBaseRaidStore, id);

    // refresh our info list whenever one of our supergroup raids changes
    sgRaidListRefresh();
}

void receiveSGRaidError(Packet* pak)
{
    int err = pktGetBitsAuto(pak);
    char* str = pktGetString(pak);

    if (err)
        dialogStd( DIALOG_OK, str, NULL, NULL, NULL, NULL, 0);
    else // just informational message
        addSystemChatMsg( textStd(str), INFO_SVR_COM, 0 );
}

// raid offer variables
static int raidOfferingDbId = 0;
static int raidOfferingSgId = 0;
static char offererName[64];

static void sendRaidResponse(char* cmd)
{
    char buf[128];
    sprintf( buf, "%s \"%s\" %d %d", cmd, offererName, raidOfferingDbId, raidOfferingSgId);
    cmdParse( buf );
    raidOfferingDbId = 0;
    raidOfferingSgId = 0;
}
static void sendRaidAccept(void *data) { sendRaidResponse("raid_accept"); }
static void sendRaidDecline(void *data) { sendRaidResponse("raid_decline"); }

void receiveSGRaidOffer(Packet *pak)
{
    int        offererDbid, offererSgid;
    char    *offerer_name, *offerer_sg_name;

    offererDbid = pktGetBits(pak, 32);
    offererSgid = pktGetBits(pak, 32);
    offerer_name = strdup(pktGetString(pak));
    offerer_sg_name = pktGetString(pak);

    // if somone has already offered, send a busy signal back
    if ( raidOfferingDbId )
    {
        // ignoring for production mode should be fine
        devassert(!"Recieved a raid offer while we were already considering one!  Server-side check gone awry?");
        free(offerer_name);
        return;
    }
    raidOfferingDbId = offererDbid;
    raidOfferingSgId = offererSgid;
    strcpy(offererName, offerer_name);
      free(offerer_name);

    dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd( "OfferRaid", offererName, offerer_sg_name ), NULL, sendRaidAccept, NULL, sendRaidDecline, 
            DLGFLAG_GAME_ONLY, NULL, NULL, 2, 0, 0, 0 );
}

// *********************************************************************************
//  SupergroupRaidInfo
// *********************************************************************************

SupergroupRaidInfo** g_raidinfos;

// just replacing our global list wholesale
void receiveSGRaidInfo(Packet* pak)
{
    if (!g_raidinfos)
        eaCreate(&g_raidinfos);
    eaClearEx(&g_raidinfos, SupergroupRaidInfoDestroy);

    while (pktGetBits(pak, 1))
    {
        SupergroupRaidInfo* info = SupergroupRaidInfoCreate(0);
        SupergroupRaidInfoGet(pak, info);
        eaPush(&g_raidinfos, info);
    }
}

// *********************************************************************************
//  raidClientTick
// *********************************************************************************

// raids that we've shown reminders for already
static U32 last_raid_warning;
static U32 last_raid_start;
#define WARNING_TIME    60*5        // time before a raid to give a warning (5 minutes)

static U32 raidNotifyEnabled;
static U32 raidCurrentlyActive;

static void status_raidNotify()
{
    dialogStd(DIALOG_OK, "RaidCurrentlyActive", NULL, NULL, NULL, NULL, 1);
}

static void checkRaidTimes(ScheduledBaseRaid* raid, U32 raidid)
{
    U32 now = timerServerTime();
    Entity* player = playerPtr();
    if (!player) return;
    if (!player->supergroup_id) return;
    if (player->supergroup_id != raid->attackersg && player->supergroup_id != raid->defendersg)
        return;

    // show warning
    if (raid->time <= now + WARNING_TIME &&
        raid->time >= now &&
        raid->time > last_raid_warning &&
        raid->complete_time == 0)
    {
        last_raid_warning = raid->time;
        attentionText_add("RaidFloatStartingSoon", 0xffff00ff, WDW_STAT_BARS, NULL);
    }
    if (raid->time <= now &&
        raid->time >= now - WARNING_TIME &&
        raid->time > last_raid_start &&
        raid->complete_time == 0)
    {
        last_raid_start = raid->time;
        if (player->supergroup_id == raid->defendersg)
        {
            attentionText_add("RaidFloatStartingDefender", 0xffff00ff, WDW_STAT_BARS, NULL);
            attentionText_add("RaidFloatStartingDefender2", 0xffff00ff, WDW_STAT_BARS, NULL);
        }
        else
        {
            attentionText_add("RaidFloatStartingAttacker", 0xffff00ff, WDW_STAT_BARS, NULL);
            attentionText_add("RaidFloatStartingAttacker2", 0xffff00ff, WDW_STAT_BARS, NULL);
        }
    }
    if (raid->time <= now &&
        raid->complete_time == 0)
    {
        raidCurrentlyActive = 1;
    }
}

// Adds the notification to the xp bar if you are part of an active raid
static void raidShowNotify()
{
    if (raidCurrentlyActive && !raidNotifyEnabled)
    {
        AtlasTex* raidNotify = atlasLoadTexture("healthbar_notify_baseraid.tga");
        status_addNotification(raidNotify, status_raidNotify, NULL);
        raidNotifyEnabled = 1;
    }
    if (!raidCurrentlyActive && raidNotifyEnabled)
    {
        AtlasTex* raidNotify = atlasLoadTexture("healthbar_notify_baseraid.tga");
        status_removeNotification(raidNotify);
        raidNotifyEnabled = 0;
    }
}

// check for upcoming raids and pop messages
static void checkRaidTimesAdapter(void* arg0, U32 arg1)
{
    checkRaidTimes((ScheduledBaseRaid*)arg0, (U32)arg1);
}

void raidClientTick(void)
{
    raidCurrentlyActive = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, checkRaidTimesAdapter);
    raidShowNotify();
}
