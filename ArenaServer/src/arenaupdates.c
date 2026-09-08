/*\
 *
 *    ArenaUpdates.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Functions to send updates to events as required to mapservers.
 *  Mapservers register dbid's that they care about, and receive
 *  full updates on events those dbid's belong to.
 *
 */

#include "arenaupdates.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/MemoryPool.h>
#include "arenaevent.h"
#include "arenaplayer.h"
#include "arenaserver.h"
#include "entity/entVarUpdate.h"
#include "dbcomm/dbcomm.h"

// *********************************************************************************
//  Hash of dbid's and listeners
// *********************************************************************************

typedef struct DbidListeners 
{
    U32 dbid;
    ArenaClientLink** clients;
} DbidListeners;
MP_DEFINE(DbidListeners);

static StashTable g_dbidhashes;

static void AddRegistration(ArenaClientLink* client, U32 dbid)
{    
    char id[20];
    DbidListeners* list;

    // set up hash
    sprintf(id, "%i", dbid);
    if (!g_dbidhashes)
        g_dbidhashes = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys | StashCaseSensitive );
    stashFindPointer( g_dbidhashes, id, &list );
    if (!list)
    {
        MP_CREATE(DbidListeners, 100);
        list = MP_ALLOC(DbidListeners);
        list->dbid = dbid;
        eaCreate(&list->clients);
        stashAddPointer(g_dbidhashes, id, list, false);
        PlayerSetConnected(dbid, 1);
    }
    assert(list->dbid == dbid);
    if (eaFind(&list->clients, client) == -1)
    {
        eaPush(&list->clients, client);
        ClientUpdateParticipant(ClientListFromClient(client, 0), dbid, 1);
    }

    // add to client link
    if (eaiFind(&client->registered_dbids, dbid) == -1)
        eaiPush(&client->registered_dbids, dbid);

    // disable TTL
    SetPlayerTTL(dbid, 1);
}

static void RemoveRegistration(ArenaClientLink* client, U32 dbid, int logout)
{
    char id[20];
    DbidListeners* list;

    // remove from hash
    sprintf(id, "%i", dbid);
    if (!g_dbidhashes)
        g_dbidhashes = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys | StashCaseSensitive );
    stashFindPointer( g_dbidhashes, id, &list );
    if (list)
    {
        eaRemove(&list->clients, eaFind(&list->clients, client)); // error ok
        if (!eaSize(&list->clients))
        {
            stashRemovePointer(g_dbidhashes, id, NULL);
            eaDestroy(&list->clients);
            MP_FREE(DbidListeners, list);
            PlayerSetConnected(dbid, 0);

            // I'm last connection to this dbid
            if (logout)
                HandlePlayerDisconnect(dbid);
            else
                SetPlayerTTL(dbid, 0);
        }
    }

    // remove from client link
    eaiRemove(&client->registered_dbids, eaiFind(&client->registered_dbids, dbid));
}

static void RemoveAllRegistrations(ArenaClientLink* client)
{
    while (eaiSize(&client->registered_dbids))
    {
        RemoveRegistration(client, client->registered_dbids[0], 0);
    }
}

// *********************************************************************************
//  Hold on to registered dbid's by client link
// *********************************************************************************

int ArenaClientConnect(NetLink* link)
{
    ArenaClientLink* client = link->userData;
    client->link = link;
    link->cookieEcho = 1;
    eaiCreate(&client->registered_dbids);
    UpdateArenaTitle();
    return 1;
}

int ArenaClientDisconnect(NetLink* link)
{
    ArenaClientLink* client = link->userData;
    RemoveAllRegistrations(client);
    eaiDestroy(&client->registered_dbids);
    UpdateArenaTitle();
    return 1;
}

// *********************************************************************************
//  Arena client list
// *********************************************************************************

ArenaClientList g_clientlist = {0};

void ClientListInit(int add)
{
    if (!g_clientlist.clients)
        eaCreate(&g_clientlist.clients);
    if (!add)
        eaSetSize(&g_clientlist.clients, 0);
}

ArenaClientList* ClientListFromClient(ArenaClientLink* client, int add)
{
    ClientListInit(add);
    if (-1 == eaFind(&g_clientlist.clients, client))
        eaPush(&g_clientlist.clients, client);
    return &g_clientlist;
}

ArenaClientList* ClientListFromDbid(int dbid, int add)
{
    char id[20];
    DbidListeners* list;
    int c;

    ClientListInit(add);
    sprintf(id, "%i", dbid);
    stashFindPointer( g_dbidhashes, id, &list );
    if (list)
        for (c = eaSize(&list->clients)-1; c >= 0; c--)
        {
            if (-1 == eaFind(&g_clientlist.clients, list->clients[c]))
                eaPush(&g_clientlist.clients, list->clients[c]);
        }
    return &g_clientlist;
}

ArenaClientList* ClientListFromEvent(ArenaEvent* event, int add)
{
    int i;
    ClientListInit(add);
    if (event)
        for (i = eaSize(&event->participants)-1; i >= 0; i--)
        {
            ClientListFromDbid(event->participants[i]->dbid, 1);
        }
    return &g_clientlist;
}

void DbidsFromEvent(int** dbids, ArenaEvent* event)
{
    int i, n;

    eaiCreate(dbids);
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
        eaiPush(dbids, event->participants[i]->dbid);
}


// *********************************************************************************
//  Registration, updates
// *********************************************************************************

void handleRegisterPlayers(Packet* pak, NetLink* link)
{
    ArenaClientLink* client = link->userData;
    int add, fullupdate, count, i, logout;
    U32 dbid;

    fullupdate = pktGetBits(pak, 1);
    add = pktGetBits(pak, 1);
    logout = pktGetBits(pak, 1);
    count = pktGetBitsPack(pak, 1);

    if (fullupdate) 
        RemoveAllRegistrations(client);

    for (i = 0; i < count; i++)
    {
        dbid = pktGetBitsPack(pak, 1);
        if (add) 
            AddRegistration(client, dbid);
        else
            RemoveRegistration(client, dbid, logout);
    }
}

void ClientUpdateEvent(ArenaClientList* list, U32 eventid)
{
    ArenaEvent* event = EventGet(eventid);
    int i, n;

    n = eaSize(&list->clients);
    for (i = 0; i < n; i++)
    {
        Packet* ret = pktCreateEx(list->clients[i]->link, ARENASERVER_EVENTUPDATE);
        pktSendBitsPack(ret, 1, eventid);
        pktSendBits(ret, 1, event? 0 : 1);    // deleted
        if (event)
            ArenaEventSend(ret, event, 1);
        pktSend(&ret, list->clients[i]->link);
    }
}

void ClientRequestReadyMessages(ArenaEvent *event)
{
    int dbIdIndex, clientIndex;
    int *dbids;

    DbidsFromEvent(&dbids, event);
    
    for (dbIdIndex = 0; dbIdIndex < eaiSize(&dbids); dbIdIndex++)
    {
        ArenaClientList *list = ClientListFromDbid(dbids[dbIdIndex], 0);

        if (list)
        {
            for (clientIndex = eaSize(&list->clients) - 1; clientIndex >= 0; clientIndex--)
            {
                if (list->clients[clientIndex] && list->clients[clientIndex]->link)
                {
                    Packet *pak = pktCreateEx(list->clients[clientIndex]->link, ARENASERVER_REQREADY);
                    pktSendBitsAuto(pak, dbids[dbIdIndex]);
                    pktSend(&pak, list->clients[clientIndex]->link);
                }
            }
        }
    }
}

void ClientSendNotReadyMessage(U32 eventid, char *playername)
{
    ArenaEvent* event = EventGet(eventid);
    ArenaClientList* list = ClientListFromEvent(event, 0);
    int i, n;

    n = eaSize(&list->clients);
    for (i = 0; i < n; i++)
    {
        Packet* ret = pktCreateEx(list->clients[i]->link, ARENASERVER_SENDNOTREADYMESSAGE);
        pktSendBitsAuto(ret, eventid);
        pktSendString(ret, playername);
        pktSend(&ret, list->clients[i]->link);
    }
}

void ClientSendNextRoundMessage(U32 eventid, int numseconds)
{
    ArenaEvent* event = EventGet(eventid);
    ArenaClientList* list = ClientListFromEvent(event, 0);
    int i, n;

    n = eaSize(&list->clients);
    for (i = 0; i < n; i++)
    {
        Packet* ret = pktCreateEx(list->clients[i]->link, ARENASERVER_SENDNEXTROUNDMESSAGE);
        pktSendBitsAuto(ret, eventid);
        pktSendBitsAuto(ret, numseconds);
        pktSend(&ret, list->clients[i]->link);
    }
}


// send updates for this participant.  scans events to see
// what events this dbid belongs to.  if fullupdate is set, 
// event details are sent for each event.
static ArenaRef** ecup_refs;
static int ecup_dbid;
int ecup_Iter(ArenaEvent* event)
{
    ArenaRef* ref;
    int i, n;
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid == ecup_dbid)
        {
            ref = ArenaRefCreate();
            ref->eventid = event->eventid;
            ref->uniqueid = event->uniqueid;
            eaPush(&ecup_refs, ref);
            break;
        }
    }
    return 1;
}
static void ArenaRefDestroyCallback(void* arg0)
{
    ArenaRefDestroy((ArenaRef*)arg0);
}

void ClientUpdateParticipant(ArenaClientList* list, int dbid, int fullupdate)
{
    Packet* pak;
    int c, i, n;

    eaCreate(&ecup_refs);
    ecup_dbid = dbid;
    EventIter(ecup_Iter);

    n = eaSize(&ecup_refs);
    for (c = eaSize(&list->clients)-1; c >= 0; c--)
    {
        pak = pktCreateEx(list->clients[c]->link, ARENASERVER_PARTICIPANTUPDATE);
        pktSendBitsPack(pak, 1, dbid);
        pktSendBitsPack(pak, 1, n);
        for (i = 0; i < n; i++)
        {
            pktSendBitsPack(pak, 1, ecup_refs[i]->eventid);
            pktSendBitsPack(pak, 1, ecup_refs[i]->uniqueid);
        }
        pktSend(&pak, list->clients[c]->link);

        if (fullupdate)
            for (i = 0; i < n; i++)
                ClientUpdateEvent(list, ecup_refs[i]->eventid);
    }
    eaDestroyEx(&ecup_refs, ArenaRefDestroyCallback);
}

void ClientUpdatePlayer(int dbid)
{
    Packet* pak;
    int i, n;
    ArenaClientList* list = ClientListFromDbid(dbid, 0);
    ArenaPlayer* ap = PlayerGet(dbid,1);

    if (!ap) 
        return;
    n = eaSize(&list->clients);
    for (i = 0; i < n; i++)
    {
        pak = pktCreateEx(list->clients[i]->link, ARENASERVER_PLAYERUPDATE);
        pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, dbid);
        pktSendBitsPack(pak, 1, -1);    //sending the -1 to signal that we don't want a window to pop up
        ArenaPlayerSend(pak, ap);
        pktSend(&pak, list->clients[i]->link);
    }
}

// *********************************************************************************
//  TTL list of dbid's - used for logout & disconnect detection
// *********************************************************************************

#define ARENA_DISCONNECT_TIMEOUT  60        // time in seconds to wait before disconnecting a participant

typedef struct DbidTTL
{
    U32 dbid;
    U32 expires;
} DbidTTL;
MP_DEFINE(DbidTTL);

static StashTable g_ttlhashes = 0;

// safe to call with zero dbid
void SetPlayerTTL(U32 dbid, int clear)
{
    DbidTTL* rec;
    char id[20];

    if (!dbid)
        return;
    if (!g_ttlhashes)
        g_ttlhashes = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys | StashCaseSensitive );
    sprintf(id, "%i", dbid);
    stashFindPointer( g_ttlhashes, id, &rec );
    if (clear)
    {
        if (!rec)
            return;
        stashRemovePointer(g_ttlhashes, id, NULL);
        MP_FREE(DbidTTL, rec);
    }
    else // set
    {
        if (!rec)
        {
            MP_CREATE(DbidTTL, 100);
            rec = MP_ALLOC(DbidTTL);
            stashAddPointer(g_ttlhashes, id, rec, false);
        }
        rec->dbid = dbid;
        rec->expires = dbSecondsSince2000() + ARENA_DISCONNECT_TIMEOUT;
    }
}

// give all players in the negotation phase of an event an expiration time
// before they will be disconnected (called on startup)
static int enpIter(ArenaEvent* event)
{
    if (event->phase == ARENA_NEGOTIATING)
    {
        int i;
        for (i = 0; i < eaSize(&event->participants); i++)
        {
            SetPlayerTTL(event->participants[i]->dbid, 0);
        }
    }
    return 1;
}
void SetTTLForNegotiatingPlayers(void)
{
    EventIter(enpIter);
}

// CheckForPlayerDisconnects: check all players with a disconnect TTL
static U32 cpdTime;
static int cpdHashIter(StashElement el)
{
    DbidTTL* rec = stashElementGetPointer(el);
    if (rec->expires <= cpdTime)
    {
        //printf("detected disconnect of %i\n", rec->dbid);
        HandlePlayerDisconnect(rec->dbid);
        stashRemovePointer(g_ttlhashes, stashElementGetStringKey(el), NULL);
    }
    return 1;
}
void CheckForPlayerDisconnects(void)
{
    if (g_ttlhashes)
    {
        cpdTime = dbSecondsSince2000();
        stashForEachElement(g_ttlhashes, cpdHashIter);
    }
}

// HandlePlayerDisconnect - get the player out of any negotiating events
static U32 hpdDbid;
static int hpdEventIter(ArenaEvent* event)
{
    if (event->phase == ARENA_NEGOTIATING &&
        EventGetParticipant(event, hpdDbid))
        EventRemoveParticipant(event, hpdDbid);
    return 1;
}
void HandlePlayerDisconnect(U32 dbid)
{
    hpdDbid = dbid;
    EventIter(hpdEventIter);
}
