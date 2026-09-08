/*\
 *
 *    arenakiosk.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Just handles players clicking on arena kiosks
 *
 */

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include "group/grouputil.h"
#include <utilitieslib/language/MultiMessageStore.h>
#include "comm_game.h"
#include "svr/svr_base.h"
#include "entity/entity.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include "group/groupnetsend.h"
#include <utilitieslib/utils/textparser.h>
#include "gameComm/dooranimcommon.h"
#include "entity/entPlayer.h"
#include "entity/entVarUpdate.h"
#include "arenakiosk.h"
#include "arenamapserver.h"
#include "container/mapgroup.h"

StaticDefineInt ParseKioskType[] =
{
    DEFINE_INT
    {"Duel",            ARENAKIOSK_DUEL},
    {"Team",            ARENAKIOSK_TEAM},
    {"Supergroup",        ARENAKIOSK_SUPERGROUP},
    {"Tournament",        ARENAKIOSK_TOURNAMENT},
    DEFINE_END
};

typedef struct ArenaKiosk
{
    ArenaKioskType type;
    Vec3 pos;
} ArenaKiosk;

static ArenaKiosk** g_kiosks = NULL;

// structures
MP_DEFINE(ArenaKiosk);
ArenaKiosk* ArenaKioskCreate(void)
{
    MP_CREATE(ArenaKiosk, 100);
    return MP_ALLOC(ArenaKiosk);
}
void ArenaKioskDestroy(ArenaKiosk* ap)
{
    MP_FREE(ArenaKiosk, ap);
}

static int ArenaKioskLoader(GroupDefTraverser* traverser)
{
    ArenaKiosk* kiosk;
    char* str;
    ArenaKioskType type;

    if(!traverser->def->properties)
    {
        if(!traverser->def->child_properties)
            return 0;
        return 1;
    }

    // Does this thing have a kiosk?
    str = groupDefFindPropertyValue(traverser->def, "ArenaKiosk");
    if (str)
    {
        type = StaticDefineIntGetInt(ParseKioskType, str);
        if (type == -1)
        {
            ErrorFilenamef(currentMapFile(), "ArenaKiosk at (%0.2f,%0.2f,%0.2f) has invalid type %s", 
                traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2], str);
        }
        else
        {
            kiosk = ArenaKioskCreate();
            copyVec3(traverser->mat[3], kiosk->pos);
            kiosk->type = type;
            eaPush(&g_kiosks, kiosk);
        }
    }

    return 1;
}

static void ArenaKioskDestroyAdapter(void* arg0)
{
    ArenaKioskDestroy((ArenaKiosk*)arg0);
}

void ArenaKioskLoad(void)
{
    GroupDefTraverser traverser = {0};

    loadstart_printf("Loading arena kiosk locations.. ");

    // If we're doing a reload, clear out old data first.
    if (g_kiosks)
    {
        eaClearEx(&g_kiosks, ArenaKioskDestroyAdapter);
        eaSetSize(&g_kiosks, 0);
    }
    else
    {
        eaCreate(&g_kiosks);
    }
    groupProcessDefExBegin(&traverser, &group_info, ArenaKioskLoader);

    loadend_printf("%i found", eaSize(&g_kiosks));
}

ArenaKiosk* ArenaKioskFind(Vec3 pos, int* id)
{
    int i, n;

    n = eaSize(&g_kiosks);
    for (i = 0; i < n; i++)
    {
        if (nearSameVec3(pos, g_kiosks[i]->pos))
        {
            if (id) *id = i+1;
            return g_kiosks[i];
        }
    }
    if (id) *id = 0;
    return NULL;
}

ArenaKiosk* ArenaKioskFromId(int id)
{
    if (id <= 0 || id > eaSize(&g_kiosks))
        return NULL;
    return g_kiosks[id-1];
}

static void KioskOpen(Entity* e, int type)
{
    int sent = 0;

    if( e->pl->arenaActiveEvent.eventid) // There is an active event, req an update
    {
        ArenaEvent* event = FindEventDetail(e->pl->arenaActiveEvent.eventid, 0, NULL);
        ArenaParticipant* part = ArenaParticipantFind(event, e->db_id);

        if (event && part &&
            event->uniqueid == e->pl->arenaActiveEvent.uniqueid &&
            event->phase != ARENA_SCHEDULED &&  // scheduled events are not ready to go yet
            !part->dropped)    // players who drop an event can join another event
        {
            if (event->phase == ARENA_NEGOTIATING)
            {
                event->lastupdate_dbid = e->db_id;
                event->lastupdate_cmd = CLIENTINP_ARENA_REQ_JOIN;
                strncpy(event->lastupdate_name, e->name, 32);
                SendActiveEvent(e, event);
            }
            else
            {
                ArenaRequestResults(e, event->eventid, event->uniqueid);
            }
            sent = 1;
        }
    }
    if (!sent)
    {
        START_PACKET(pak, e, SERVER_ARENAKIOSK_SETOPEN)
            pktSendBits(pak, 1, 1);
            pktSendBits(pak, ARENAKIOSK_NUMBITS, type);
        END_PACKET
    }
}

// verify client input of kiosk click
// DEPRECATED!!!!
bool ArenaKioskReceiveClick(Packet *pak, Entity *e)
{
    //ArenaKiosk* kiosk;
    //Vec3 pos;
    MapDataToken *pTok = NULL;

    //int id;

    // We got a lot looser with kiosk restricitons,
    // we just want them to be in the arena lobby now
    // CLIENTINP_ARENAKIOSK_OPEN
    //pos[0] = pktGetF32(pak);
    //pos[1] = pktGetF32(pak);
    //pos[2] = pktGetF32(pak);

//    kiosk = ArenaKioskFind(pos, &id);
//    if (kiosk && distance3Squared(e->mat[3], kiosk->pos) < SQR(SERVER_INTERACT_DISTANCE))
    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    pTok = getMapDataToken("pvpDisabled");
    if( e->pl->arenaLobby && (!pTok || pTok->val == 0))
    {
        e->pl->arenaKioskOpen = 1;
        e->pl->arenaKioskCheckDist = 1;
        //e->pl->arenaKioskId = id;

          KioskOpen(e, 0 ); //kiosk->type);
        return true;
    }
    return false;
}

// set the player as interacting with kiosk, without requiring one
void ArenaKioskOpen(Entity* e)
{
    e->pl->arenaKioskOpen = 1;
    e->pl->arenaKioskCheckDist = 0;
    KioskOpen(e, 0);
}

// notice if the player walked away from the kiosk
void ArenaKioskCheckPlayer(Entity* e)
{
    int failed = 0;
    //ArenaKiosk* kiosk;

    if (!e->pl || !e->pl->arenaKioskCheckDist)
        return;

    // players now only have to stay in the lobby volume
    if (!e->pl->arenaLobby)
        failed = 1;

    //kiosk = ArenaKioskFromId(e->pl->arenaKioskId);
    //if (!kiosk)
    //    failed = 1;        // map was re-inited?
    //else
    //{
    //    if (distance3Squared(e->mat[3], kiosk->pos) > SQR(SERVER_INTERACT_DISTANCE))
    //        failed = 1;
    //}

    if (failed)
    {
        e->pl->arenaKioskOpen = 0;
        e->pl->arenaKioskCheckDist = 0;
        e->pl->arenaKioskId = 0;
        ArenaPlayerLeftKiosk(e);
        START_PACKET(pak, e, SERVER_ARENAKIOSK_SETOPEN)
            pktSendBits(pak, 1, 0);
        END_PACKET
    }
}

void GetFirstKioskPos(Vec3 dest)
{
    copyVec3(g_kiosks[0]->pos, dest);
}