#include <float.h>
#include "entity/SgrpRewards.h"
#include <limits.h>
#include "entity/entserver.h"
#include <utilitieslib/network/netio.h>
#include "comm_game.h"
#include "svr_base.h"
#include "container/containerloadsave.h"
#include <utilitieslib/assert/assert.h>
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbmapxfer.h"
#include <utilitieslib/utils/timing.h>
#include "varutils.h"
#include <utilitieslib/utils/error.h>
#include "entity/motion.h"
#include "player/clientEntityLink.h"
#include "player/pmotion.h"
#include "generator/encounter.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "group/groupnetdb.h"
#include <utilitieslib/utils/breakpoint.h>
#include "entity/entVarUpdate.h"
#include "svr_player.h"
#include "container/team.h"
#include "entity/entPlayer.h"
#include <utilitieslib/network/net_socket.h>
#include "storyarc/storyarcinterface.h"
#include "storyarc/mission.h"
#include "player/pl_stats.h" // for stat_*
#include "player/parseClientInput.h"
#include "cmdparse/cmdcontrols.h"
#include "storyarc/TeamTask.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "dbcomm/logcomm.h"
#include "dbcomm/shardcomm.h"
#include "player/badges_server.h"
#include "entity/buddy_server.h"
#include "gameSys/arenamapserver.h"
#include <utilitieslib/language/AppLocale.h>
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "utils/position.h"
#include "player/pmotion.h"
#include "script/scriptengine.h"
#include "group/group.h"
#include "gameSys/raidmapserver.h"
#include "gameSys/sgraid.h"
#include "bases/bases.h"
#include "bases/baseserver.h"
#include "entity/character_inventory.h"
#include "gameSys/endgameraid.h"
#include "container/league.h"
#include <utilitieslib/utils/log.h>
#include "svr_tick.h"


void checkPlayerPowers(Entity* e, int reset)
{
    static int asserted = 0;
    int character_CheckAllPowersPresent(Character *p);

    if(reset)
    {
        asserted = 0;
    }
    else
    {
        if(!asserted && e && e->pchar && !character_CheckAllPowersPresent(e->pchar))
        {
            asserted = 1;
            assertmsg(0, "I don't have all my powers!");
        }
    }
}

#define CONNECTING_PLAYER_TIMEOUT (15*60) // In seconds

PlayerEnts    player_ents[PLAYER_ENTTYPE_COUNT];

int player_count;

int svr_hero_count=0;
int svr_villain_count=0;

void svrSendPlayerError(Entity *player,const char *fmt, ...)
{
    char    buf[1000];
    va_list arglist;
    ClientLink *client = clientFromEnt(player);

    if (!client)
        return;
    va_start(arglist, fmt);
    vsprintf(buf, fmt, arglist);
    va_end(arglist);

    sendEditMessage(client->link, 1, "%s",buf);
}

void svrCompletelyUnloadPlayerAndMaybeLogout(Entity *e, const char * reason, int logout)
{
    ClientLink    *client;
    U32 ulNow = dbSecondsSince2000();

    if(!e)
        return;

    client = clientFromEnt(e);

    assert(e->db_id>0);

    logDisconnect(client, reason);

    if (client && client->link && client->link->parentNetLinkList)
    {// disconnect via timeout - the networking code may not have decided to remove the link yet.
        netRemoveLink(client->link);
    }

    if (e->db_id==-1) 
    {
        // This entity was freed in svrDisconnectCallback, which can only
        //  happen if the entity was waiting for a map transfer, so most likely
        //  the person was waiting for a map to be spawned or ready, but it crashed,
        //  and then they disconnected from this map because they got tired of waiting
        //  and then they timed out and got auto-disconnected?
        return;
    }

    shardCommLogout(e);
    

    e->map_id = e->static_map_id;
    if (e->league)
    {
        league_MemberQuit(e, 0);
    }
    // Player should only be saved it we own the entity!
    if (e->owned) 
    {
        playerEntDelete(PLAYER_ENTS_OWNED,e,1); // This must be *before* UpdatePlayerCount
        if (character_IsInitialized(e->pchar)) 
        {
            teamUpdatePlayerCount(e);
            svrSendEntListToDb(&e,1);
        }
    }

    playerEntDelete(PLAYER_ENTS_ACTIVE,e,1);
    playerEntDelete(PLAYER_ENTS_CONNECTING,e,1);

    teamlogPrintf(__FUNCTION__, "logout %s:%i", e->name, e->db_id);
    dbSendPlayerDisconnect(e->db_id, logout, e->logout_login);
    TeamTaskClearMemberTaskList(e); 

    entFree(e);
}

void svrForceLogoutLink(NetLink *link,const char *reason,KickReason kicktype)
{
    Packet        *pak;
    ClientLink    *client;

    if (!link)
        return;

    client = link->userData;
    pak = pktCreate();
    pktSendBitsPack(pak,1,SERVER_FORCE_LOGOUT);
    pktSendString(pak,reason);
    pktSend(&pak,link);
    lnkBatchSend(link);

    link->logout_disconnect = 1;

    // removing old link during containerHandleEntity
    if (kicktype == KICKTYPE_OLDLINK)
        return;

    if (kicktype != KICKTYPE_FROMDBSERVER && client->entity)
    {
        if (kicktype == KICKTYPE_BAN)
        {
            dbPlayerKicked(client->entity->db_id,1);
            LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Banned: %s", reason?reason:"(no reason given)");
        }
        else
        {
            dbPlayerKicked(client->entity->db_id,0);
            LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Kicked %s", reason?reason:"(no reason given)");
        }
    }

    if (kicktype == KICKTYPE_BADDATA)
    {
        // Remove the entity so that when the link is removed, it doesn't save the entity
        if (client->entity) {
            LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Kicked Bad Data");

            stat_AddTimeInZone(client->entity->pl, dbSecondsSince2000()-client->entity->on_since);

            playerEntDelete(PLAYER_ENTS_CONNECTING,client->entity,0);
            playerEntDelete(PLAYER_ENTS_ACTIVE,client->entity,0);
            if (client->entity->owned) {
                playerEntDelete(PLAYER_ENTS_OWNED,client->entity,0);
                teamUpdatePlayerCount(client->entity);
            }
            TeamTaskClearMemberTaskList(client->entity);
            teamlogPrintf(__FUNCTION__, "Kicking due to bad data %s:%i", client->entity->name, client->entity->db_id);
            client->entity->client = NULL;
            entFree(client->entity);
            client->entity = NULL;
            netRemoveLink(link);
        }
    }
    else
        svrCompletelyUnloadPlayerAndMaybeLogout(client->entity, reason, 1);
}

void svrForceLogoutAll(char *reason)
{
    int        i;

    for(i=net_links.links->size-1;i>=0;i--)
        svrForceLogoutLink((NetLink*)net_links.links->storage[i],reason,KICKTYPE_FROMDBSERVER);
}

int svrSendEntListToDb(Entity **ents,int count)
{
    int            i, newindex;
    Entity        *e;
    int *ids = NULL;
    char **data = NULL;
    if (!count)
        return 0;

    PERFINFO_AUTO_START("alloc data array", 1);
    eaSetSize(&data,count);
    eaiSetSize(&ids,count);
    PERFINFO_AUTO_STOP();

    for(i=0, newindex = 0; i<count; i++, newindex++)
    {
        e = ents[i];
        if (!e->corrupted_dont_save
            && !(e->pl && e->pl->isBeingCritter) ) // don't send if being a critter
        {
            PERFINFO_AUTO_START("packageEnt", 1);
            data[newindex] = packageEnt(e);
            PERFINFO_AUTO_STOP();

            if(data[newindex]==0)
            {
                ClientLink *client = clientFromEnt(e);
                dbLog("EntSave",e, "Character did not pack correctly. Not saving.");

                if(client)
                {
                    svrForceLogoutLink(client->link, "S_DATABASE_FAIL", KICKTYPE_BADDATA);
                }

                newindex--;                
                continue;
            }
        }
        else
        {
            if( e->corrupted_dont_save )
            {
                ClientLink *client = clientFromEnt(e);
                if(client)
                {
                    svrForceLogoutLink(client->link, "S_DATABASE_FAIL", KICKTYPE_BADDATA);
                }
                LOG_OLD("entity %s:%s had corrupted_dont_save set",e->auth_name,e->name);
            }
            
            newindex--;
            continue;
        }
        assert(e->db_id > 0);
        ids[newindex] = e->db_id;
    }

    PERFINFO_AUTO_START("dbContainerSendList", 1);

    dbContainerSendList(CONTAINER_ENTS,data,ids,newindex, 0);

    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("free data", 1);

    for(i=0; i<newindex; i++)
        SAFE_FREE(data[i]);
    eaDestroy(&data);
    eaiDestroy(&ids);

    PERFINFO_AUTO_STOP();

    return count;
}

int svrSendAllPlayerEntsToDb()
{
    int            i,count=0;
    static        int        max_count;
    static        Entity    **ents;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];

    PERFINFO_AUTO_START("svrSendAllPlayerEntsToDb", 1);
        for(i=0;i<players->count;i++)
        {
            if (!players->ents[i]->dbcomm_state.map_xfer_step && players->ents[i]->ready)
                dynArrayAddp(&ents,&count,&max_count,players->ents[i]);
        }
        PERFINFO_AUTO_START("svrSendEntListToDb", 1);
            count = svrSendEntListToDb(ents,count);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
    return count;
}

void svrCheckQuitting(void)
{
    int            i,count=0;
    ClientLink    *client;
    //char    buf[1000];

    if (!db_state.server_quitting)
        return;
    // going to need logic in here to make sure no new players get accepted?
    for(i=0;i<net_links.links->size;i++)
    {
        client = ((NetLink*)net_links.links->storage[i])->userData;
        if (client->entity)
        {
            Entity *ent = client->entity;
            if (!ent->dbcomm_state.in_map_xfer && ent->ready && !(ent->pl->lastTurnstileEventID || ent->pl->lastTurnstileMissionID))
                dbAsyncMapMove(ent,-1,NULL,XFER_STATIC | XFER_FINDBESTMAP);
            count++;
        }

    }

    // ----------
    // flush pending sgrp prestige updates

    sgrprewards_Tick(true);

    // ----------
    // shutdown

    if (!count && !svrAnyOwnedOrActiveEnts())
    {
        extern bool disconnected_from_dbserver;
        LOG_OLD("serverquit trying to disconnect on map %d via %s",db_state.map_id,
            (db_state.server_quitting_msg ? db_state.server_quitting_msg : "(error: no quit msg)") );
        netSendDisconnect(&db_comm_link,4.f);
        disconnected_from_dbserver = true;
        LOG_OLD( "svrCheckQuitting() said to shut down");
        //groupCleanUp(); // Clean up ctris that might be in shared memory
        svrLogPerf();
        logSendDisconnect(30);    // remote logs (logserver)
        logShutdownAndWait();    // local logs
        exit(0);
    }
}

// do we have a player IN_USE?
Entity* firstActivePlayer(void)
{
    int i;
    for(i = 1; i < entities_max; i++){
        if(entity_state[i] & ENTITY_IN_USE){
            if(ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER){
                return entities[i];
            }
        }
    }
    return NULL;
}

void svrCheckMissionEmpty(void)
{
    static int player_ever_connected = 0;
    static U32 last_connected_time;
    static int first_time = 1;
    if (!OnMissionMap()) return;
    if (player_count) // have a player connected right now
    {
        if (!player_ever_connected && firstActivePlayer()) // the first time a player connects
        {
            MissionInitMap();
            player_ever_connected = 1;
        }
        last_connected_time = ABS_TIME;
    }
    else if (svrAnyOwnedOrActiveEnts())
    {
        // no one is fully connected, but we still have a link or an ent,
        // nothing to see here..
    }
    else if (player_ever_connected) // we actually had a player log on and start mission
    {
        if (MissionCompleted() || // if mission is complete, close immediately
            IncarnateTrialComplete() || // also close now if the zone event is complete
            ABS((int)ABS_TIME - (int)last_connected_time) >=
            SEC_TO_ABS(MISSION_SHUTDOWN_DELAY_MINS * 60)) // otherwise, we have a delay
            MissionShutdown("svrCheckMissionEmpty() empty and complete or timeout");
    }
    else // never had a player log on - do a sanity check of 3 minutes here
    {
        if (first_time)
        {
            first_time = 0;
            last_connected_time = ABS_TIME;
        }
        if (ABS((int)ABS_TIME - (int)last_connected_time) >=
            SEC_TO_ABS(MISSION_NOLOGIN_DELAY_MINS * 60))
        {
            MissionShutdown("svrCheckMissionEmpty() empty, nologin");
        }
    }
}

void svrCheckBaseEmpty(void)
{
    if (!OnSGBase()) return;
    if (RaidIsRunning()) return; // don't close while a raid is running
    if (isDevelopmentMode()) return; //don't kick them out in development mode

    if (svrGetSecondsIdle() > SGBASE_SHUTDOWN_TIMEOUT)
    {
        LOG_OLD("Base:Empty %i shutting base down", g_base.supergroup_id);
        dbRequestShutdown();
        svrResetSecondsIdle();    // reset the idle time so that we don't generate another request immediately
    }
}

// this is an instance map of a master base map
static  bool isStaticMapInstance(void)
{
    return db_state.is_static_map && db_state.map_id != db_state.base_map_id;
}

/*
    Is this map a candidate for idle exit?
    Mission maps and super group bases are handled elsewhere.

    Don't want to idle exit master static maps, but instances of those static
    maps can be delinked.

    Reasons why we don't want to idle exit an auto start static map:

     1.    With the current design/implementation whenever a new static maps starts up
        it has to tell the dbserver about the doors that are defined in its assets
        (there is no master list of doors). Any time the dbserver gets any new door
        information it updates EVERY running map (even mission and base maps) with
        the complete door list. That is a very large amount of network traffic and
        processing for the maps to do (when most of them don't care).
    2. Some static maps that need to be already running for scripted events (apparently like Boomtown an the holiday event)
    3. They autostart so that players can quickly transfer into their current maps as they login.

*/
void svrCheckIdleShutdown(void)
{
    // local servers for development and editing don't ever idle exit.
    // also only idle out static map instances, not the master started with AutoStart at shard startup (see comments above)
    if ( !db_state.local_server && (server_state.idle_exit_timeout > 0 || server_state.preload_transient) && (server_state.transient || isStaticMapInstance()))
    {
        // is the server currently idle
        if (!player_count)
        {
            // server is idle, has it been idle long enough that it should be shutdown?
            static bool s_warned = false;
            const U32 kWarnIdleMinutes = 5;

            U32 idle_seconds = svrGetSecondsIdle();
            U32 idle_minutes = idle_seconds/60;
            
            // log a warning a few minutes before idle exit
            if ( !s_warned && server_state.idle_exit_timeout > kWarnIdleMinutes )
            {
                if (idle_minutes > server_state.idle_exit_timeout - kWarnIdleMinutes)
                {
                    printf("server has been idle %d minutes and will shutdown in %d minutes if it remains unused.\n",
                        (server_state.idle_exit_timeout - kWarnIdleMinutes), kWarnIdleMinutes );
                    s_warned = true;
                }
            }

            if (idle_minutes > server_state.idle_exit_timeout)
            {
                LOG_OLD("mapserver: server has exceeded idle exit timeout [%d minutes]. shutting down.",  server_state.idle_exit_timeout);
                printf("server has exceeded idle exit timeout [%d minutes]. shutting down.\n",  server_state.idle_exit_timeout);
                dbRequestShutdown();
                svrResetSecondsIdle();    // reset the idle time so that we don't generate another request immediately
            }
        }

        if (server_state.preload_transient && (svrGetSecondsIdle() > 5))
        {
            server_state.preload_transient = false;
            if (!player_count)
            {
                LOG_OLD("mapserver: transient server preloaded. shutting down until it is required again");
                printf("transient server preloaded. shutting down until it is required again\n");
                dbRequestShutdown();
                svrResetSecondsIdle(); // reset the idle time so that we don't generate another request immediately
            }
        }
    }
}

// Test if this server is just sitting around unused
// and empty. If so we we terminate to save shard resources
void svrCheckUnusedAndEmpty(void)
{
    svrCheckMissionEmpty();    // custom shutdown cleanup for missions
    svrCheckBaseEmpty();    // custom shutdown cleanup for supergroup bases
    if (!OnSGBase() && !OnMissionMap())    // these two cases are already handled
        svrCheckIdleShutdown();    // generic shutdown cleanup for normal maps
}

void playerEntAdd(PlayerEntType type,Entity *e)
{
    int            i;
    PlayerEnts    *players = &player_ents[type];

    for(i=0;i<players->count;i++)
    {
        if (players->ents[i] == e) {
            if (type == PLAYER_ENTS_OWNED) {
                assert(e->owned);
            }
            return;
        }
    }
    dynArrayAddp(&players->ents,&players->count,&players->max,e);
    if (type == PLAYER_ENTS_OWNED) {
        e->owned = 1;
        ArenaRegisterPlayer(e, 1, 0);
    }
}

void playerEntDelete(PlayerEntType type,Entity *e, int logout)
{
    int            i;
    PlayerEnts    *players = &player_ents[type];

    for(i=0;i<players->count;i++)
    {
        if (players->ents[i] == e)
            break;
    }
    if (i >= players->count) {
        if (type == PLAYER_ENTS_OWNED) {
            assert(!e->owned);
        }
        return;
    }
    memmove(&players->ents[i],&players->ents[i+1],(players->count-i-1) * sizeof(Entity **));
    players->count--;
    if (type == PLAYER_ENTS_OWNED) {
        int i;
        for( i = 0 ; i < e->volumeList.volumeCount ; i++ )
            pmotionIJustExitedAVolume( e, &e->volumeList.volumes[i] );
        EndGameRaidPlayerExitedMap(e);
        e->owned = 0;
        ScriptPlayerExitingMap(e);
        ArenaPlayerLeavingMap(e,logout);
        
    }
}

int playerEntCheckMembership(PlayerEntType type,Entity *e)
{
    int            i;
    PlayerEnts    *players = &player_ents[type];

    for(i=0;i<players->count;i++)
    {
        if (players->ents[i] == e)
            return 1;
    }
    return 0;
}

int svrAnyOwnedOrActiveEnts(void)
{
    int i;
    for (i = 0; i < PLAYER_ENTTYPE_COUNT; i++)
    {
        if (player_ents[i].count) return 1;
    }
    return 0;
}

void playerCheckFailedConnections()
{
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_CONNECTING];
    int            i,dt;

    for(i=players->count-1;i>=0;i--)
    {
        dt = dbSecondsSince2000() - players->ents[i]->on_since;
        if (dt > CONNECTING_PLAYER_TIMEOUT) // clients get 15 minutes to connect
        {
            svrCompletelyUnloadPlayerAndMaybeLogout(players->ents[i], "Transfer Timed Out", 0);
        }
    }
}

void playerEntCheckDisconnect()
{
    int        i;
    Entity    *e;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];

    for(i=players->count-1;i>=0;i--)
    {
        e = players->ents[i];
        if(SAFE_MEMBER2(e,pl,accountServerLock[0]))
        {
            // Don't log out if shardVisitorData contains something
            // We know it's safe to test this because the SAFE_MEMBER2 macro above guarantees e->pl is valid
            if (e->pl->shardVisitorData[0] == 0)
            {
                // TODO: this could be less obnoxious
                svrForceLogoutLink(SAFE_MEMBER(e->client,link), "AccountServerKick", KICKTYPE_KICK);
            }
        }
        else if (e->logout_timer)
        {
            e->logout_timer -= TIMESTEP;
            // logging out from auth server early to ensure "log out to character select"
            // doesn't hit a race condition where the client tries to log in before the
            // server logs out.
            if (e->logout_timer <= 30 || // 30 = one second
                e->dbcomm_state.in_map_xfer) // just logout players who requested to quit while in map transfer
            {
                svrCompletelyUnloadPlayerAndMaybeLogout(e, "Logout timer expired", 1);
            }
            //printf("logout timer: %f\n",e->logout_timer / 30);
        }
        else if (e->client==NULL) 
        {
            assert(!"WTF? #2");
            svrCompletelyUnloadPlayerAndMaybeLogout(e,"WTF? #2", 1);
        }
    }
}

NetLink *findLinkById(int id)
{
    int        i;
    ClientLink    *client;

    assert(net_links.links);
    for(i=0;i<net_links.links->size;i++)
    {
        client = ((NetLink*)net_links.links->storage[i])->userData;
        if (client->entity && client->entity->db_id == id)
            return net_links.links->storage[i];
    }
    return 0;
}

Entity *playerCreate()
{
    Entity    *e;
    char gfxfile[200];

    e = entAlloc();

    //TODO: this gets reloaded when the player character is actually created, and we know what kind it is (I think)
    //    get rid of this load, perhaps?  Initing to NULL doesn't work though.
    strcpy(gfxfile, "male");
    verbose_printf("loading: %s\n",gfxfile);

#ifdef RAGDOLL // ragdoll requires all seq info on server
     entSetSeq(e, seqLoadInst(gfxfile, 0, SEQ_LOAD_FULL_SHARED, 0, e));
#else
    entSetSeq(e, seqLoadInst(gfxfile, 0, SEQ_LOAD_HEADER_ONLY, 0, e));
#endif
    e->randSeed = rand(); //for synchronizing client gfx
    e->move_type = MOVETYPE_WALK;
    SET_ENTTYPE(e) = ENTTYPE_PLAYER;
    e->motion->falling = 1;
    e->birthtime = dbSecondsSince2000();

    // setting default player locale from server
    e->playerLocale = getCurrentLocale();

    //entSetPos(e,spawn_pos);
    playerVarAlloc(e, ENTTYPE(e));

    e->pl->uiSettings = 1; // defaults mouse look to inverted
    e->pl->uiSettings2 = 0;
    e->pl->uiSettings3 = 0;
    e->pl->uiSettings4 = 0;
    e->pl->mouse_speed  = 1.f;
    e->pl->turn_speed   = 3.f;
    e->pl->chatSendChannel = INFO_NEARBY_COM;

    e->fRewardScaleOverride = -1.0f;

    return e;
}

void svrClientLinkInitialize(ClientLink* client, NetLink* link)
{
    client->controls.server_state = createServerControlState();
    client->link = link;
    client->ready = CLIENTSTATE_NOT_READY;
    client->entNetLink = createEntNetLink();
}

void svrClientLinkDeinitialize(ClientLink* client)
{
    int logout_disconnect = 0;

    if(client->link)
    {
        NetLink* link = client->link;
        U32 addr = link->addr.sin_addr.s_addr;
        int reliableSize = 0;
        int i;
        
        logDisconnect(client, "NetLink was closed");


        for(i = 0; i < link->reliablePacketsArray.size; i++)
        {
            Packet* pak = link->reliablePacketsArray.storage[i];
            reliableSize += pak->stream.size;
        }

        LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,
                        "Packets Recv/Sent %-6d/%-6d Dropped %-3d/%-4d Dup Recv %-3d Rel %4dB/%-3d",
                        link->last_recv_id,
                        link->nextID,
                        link->lost_packet_recv_count,
                        link->lost_packet_sent_count,
                        link->duplicate_packet_recv_count,
                        reliableSize,
                        link->reliablePacketsArray.size);

        logout_disconnect = link->logout_disconnect;
        client->link = NULL;
    }

    if (client->entity)
    {
        if(client->slave){
            freePlayerControl(client);
        }

        if( logout_disconnect ) // if they disconnected in the middle of a transfer end this flag so the don't get inadvertantly warped somewhere
        {
            client->entity->db_flags &= ~DBFLAG_DOOR_XFER;
        }



        assertmsg(client->entity->client == client, "Entity whose client is not the same as the client pointing to the entity!");

        if (client->entity->dbcomm_state.in_map_xfer && client->entity->dbcomm_state.map_xfer_step >= MAPXFER_WAIT_NEW_MAPSERVER_READY)
        {
            // Only do this if we've already told the db server about the map move, otherwise, we need
            //  to let the dbserver know the client disconnected!
            //dbSendEntList(&client->entity,1);
            playerEntDelete(PLAYER_ENTS_ACTIVE,client->entity,0);
            if (client->entity->owned) {
                assert(!"Shouldn't happen?"); // JE/MK tracking down teamLock bugs
                playerEntDelete(PLAYER_ENTS_OWNED,client->entity,0);
                teamUpdatePlayerCount(client->entity);
            }
            teamlogPrintf(__FUNCTION__, "Freeing ent that was in map xfer %s:%i", client->entity->name, client->entity->db_id);
            TeamTaskClearMemberTaskList(client->entity);
            client->entity->client = NULL;
            entFree(client->entity);
            client->entity = NULL;
        }
        else
        {
            // svrSendEntListToDb will also get called when the entity is actually freed (30 seconds or so later)
            // It's here so that in development, you can kill your local mapserver right after you quit the game
            // and your player state won't lose the last 1-60 seconds -BER
            if (client->entity->ready) // entity->ready is a bool... was this supposed to be client->ready > CLIENTSTATE_NOT_READY??
                svrSendEntListToDb(&client->entity,1);
            if (!client->entity->logout_timer)
            {
                LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "svrNetDisconnectCallback setting logout_timer");
                svrPlayerSetDisconnectTimer(client->entity,0);
            }
            client->entity->client = NULL;
        }
    }

    // Free control data.
    destroyControlState(&client->controls);

    // Free net data.
    if (client->entNetLink) {
        destroyEntNetLink(client->entNetLink);
        client->entNetLink = 0;
    }

    // Free debug hash table.
    if ( client->hashDebugVars ) 
        stashTableDestroy(client->hashDebugVars);
    client->hashDebugVars = NULL;
}

int svrClientCallback(NetLink *link)
{
    ClientLink    *client = link->userData;

    svrClientLinkInitialize(client, link);

    // If we're running a local mapserver, don't bother imposing data queue limits.
    // The mapserver can send and receive as much data as it wants.
    if(db_state.local_server){
        link->sendQueuePacketMax = 0;
        qSetMaxSizeLimit(link->receiveQueue, 0);
    }
    //lnkSimulateNetworkConditions(client->link,250,50,15,1);

    {
        // All clients have the same socket, so we only do this once
        static int inited=0;
        if (!inited) {
            // Set initial and max UDP buffer sizes (Shared between all clients
            if (db_state.is_static_map) {
                netLinkListSetMaxBufferSize(link->parentNetLinkList, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
                netLinkListSetBufferSize(link->parentNetLinkList, BothBuffers, 1*1024*1024);
            } else {
                netLinkListSetMaxBufferSize(link->parentNetLinkList, BothBuffers, 1*1024*1024); // Set max size to auto-grow to
                netLinkListSetBufferSize(link->parentNetLinkList, BothBuffers, 64*1024);
            }
            inited = 1;
        }
    }
    link->notimeout = server_state.notimeout;
    return 1;
}

int svrNetDisconnectCallback(NetLink *link)
{
    ClientLink *client = (ClientLink *)link->userData;

    extern int catch_ugly_entsend_bug_hack;
    assert(catch_ugly_entsend_bug_hack==0);

    svrClientLinkDeinitialize(client);

    return 1;
}

#define DUMP_CONTROLS 0

void svrResetClientControls(ClientLink* client, int full_reset)
{
    ControlState* controls = &client->controls;
    Entity* e = clientGetControlledEntity(client);
    ServerControlState* scs;

    if(controls->controldebug)
    {
        printf("************************************* RESETTING CONTROLS: %d **********************************\n", full_reset);
    }

    scs = getQueuedServerControlState(controls);

    scs->physics_disabled = 0;

    controls->connStats.conncheck_count = 0;

    if(full_reset)
    {
        if(controls->controldebug)
        {
            printf("full resettting client controls -------------------------------------------------------\n");
        }

        controls->time.playback.ms = 0;
        controls->time.playback.ms_acc = -100;

        controls->connStats.received_controls = 0;

        destroyFuturePushList(controls);
    }

    controls->time.total_ms = 0;

    destroyControlStateChangeList(&controls->csc_list);

    controls->phys.latest.net_id = -1;
    controls->phys.acc = 1;

    controls->dir_key.down_now = 0;
    controls->dir_key.affected_last_frame = 0;

    if(e)
    {
        copyVec3(ENTPOS(e), controls->start_pos);
        copyVec3(ENTPOS(e), controls->end_pos);
    }
}

void svrPrintControlQueue(ClientLink* client)
{
    ControlStateChange* csc;
    ControlState* controls = &client->controls;

    for(csc = controls->csc_list.head; csc; csc = csc->next)
    {
        int id = csc->control_id;

        if(id < CONTROLID_BINARY_MAX)
        {
            printf(    "%5d:  type %s(%d) @ %dms\n",
                    csc->net_id,
                    pmotionGetBinaryControlName(id),
                    csc->state,
                    csc->time_diff_ms);
        }
        else
        {
            printf(    "%5d:  type %d(%1.3f) @ %dms\n",
                    csc->net_id,
                    id,
                    csc->angleF32,
                    csc->time_diff_ms);
        }
    }
}

#include "entity/entPlayer.h"
static void svrRecordLastGoodPosition(Entity *e)
{
    #define        GOOD_POS_MINSTEP 4
    #define        GOOD_POS_MAX ARRAY_SIZE(pl->last_good_pos)
    int            i;
    EntPlayer    *pl = e->pl;
    const F32    *pos = ENTPOS(e);

    for(i=0;i<GOOD_POS_MAX;i++)
    {
        if (distance3Squared(pos,pl->last_good_pos[i]) < SQR(GOOD_POS_MINSTEP))
            return;
    }
    memmove(&pl->last_good_pos[1],&pl->last_good_pos[0],(GOOD_POS_MAX-1) * sizeof(pl->last_good_pos[0]));
    copyVec3(pos,pl->last_good_pos[0]);
}

#include "gridcoll/gridcoll.h"
#include "entity/character_target.h"
#include "dbcomm/dbdoor.h"

void svrPlayerFindLastGoodPos(Entity *e)
{
    int            i;
    F32            d2;
    EntPlayer    *pl = e->pl;
    const F32    *pos = ENTPOS(e);
    CollInfo    coll;
    Vec3        start,end;

    if( g_base.rooms && !RaidIsRunning() )
    {
        teleportEntityToNearestDoor(e);
        return;
    }

    copyVec3(ENTPOS(e),start);
    copyVec3(ENTPOS(e),end);
    start[1] += 1.5;
    end[1] -= 10000;

    // PvP Exploit - Siren's Call: /stuck at -1773.2 -178.0 2173.8
    // transports Heroes into Villain Hospital - Can kill respawning
    // players
    //
    // doors aren't marked properly in hospitals as hero or villain 
    // only, so just don't do the door hopping in PvP if you get stuck
    if (!server_visible_state.isPvP &&
        (!collGrid(0,start,end,&coll,0,0) || IsTargetOutOfBounds(start)))
    {
        DoorEntry    *door;

        door = dbFindLocalMapObj(ENTPOS(e),DOORTYPE_SPAWNLOCATION,1000000);
        if (door &&
            ((!door->villainOnly && !door->heroOnly)
            || (door->villainOnly && ENT_IS_VILLAIN(e)) 
             || (door->heroOnly && ENT_IS_HERO(e))))
        {
            entUpdateMat4Instantaneous(e,door->mat);
            return;
        }
    }

    for(i=0;i<GOOD_POS_MAX;i++)
    {
        d2 = distance3Squared(pos,pl->last_good_pos[i]);
        if (d2 > SQR(GOOD_POS_MINSTEP) && d2 < SQR(GOOD_POS_MINSTEP*2))
        {
            entUpdatePosInstantaneous(e,pl->last_good_pos[i]);
            for(i=0;i<GOOD_POS_MAX;i++)
                pl->last_good_pos[i][1] = -1000000;
            return;
        }
    }
}

struct
{
    Entity*            e;
    ControlState*    controls;
    MotionState*    motion;
    int                movementAllowed;
} controlInstance;

static void svrUpdateMotionState(Entity* e, ControlState* controls)
{
    MotionState* motion = controlInstance.motion;
    ServerControlState* scs = controls->server_state;

    if(motion->input.flying != scs->fly)
    {
        motion->input.flying = scs->fly;

        if(!motion->input.flying)
        {
            motion->falling = 1;
        }
        else
        {
            motion->jumping = 0;
            motion->falling = 0;
        }
    }

    if(motion->input.no_ent_collision != scs->no_ent_collision)
    {
        motion->input.no_ent_collision = scs->no_ent_collision;
        e->collision_update = 1;
    }

    if( scs->hit_stumble )
    {
        motion->hit_stumble = 1;
        motion->hitStumbleTractionLoss = 1.0; //TO DO base on strength of hit
        SETB( e->seq->state, STATE_STUMBLE );
    }

    memcpy(motion->input.surf_mods, scs->surf_mods, sizeof(motion->input.surf_mods));
}

static void svrDestroyServerControlStateHead(ControlState* controls)
{
    ServerControlState* scs = controls->queued_server_states.head;

    controls->queued_server_states.head = scs->next;

    destroyServerControlState(scs);

    if(!--controls->queued_server_states.count)
    {
        controls->queued_server_states.tail = NULL;
    }
}

static void svrDestroyControlStateChangeHead(ControlState* controls)
{
    ControlStateChange* csc = controls->csc_list.head;

    if(!csc)
        return;

    controls->csc_list.head = csc->next;

    controls->time.total_ms -= csc->time_diff_ms;

    destroyControlStateChange(csc);

    if(!--controls->csc_list.count)
    {
        controls->csc_list.tail = NULL;

        assert(!controls->csc_list.head);
    }
    else
    {
        controls->csc_list.head->prev = NULL;
    }
}

static void svrApplyServerState(Entity* e, ControlState* controls, MotionState* motion, ControlStateChange* csc)
{
    ServerControlState* server_state = controls->server_state;
    char max_diff = controls->sent_update_id - controls->received_update_id;
    char diff = csc->applied_server_state_id - controls->received_update_id;

    if(!controls->cur_server_state_sent)
        max_diff--;

    if(diff > 0 && diff <= max_diff)
    {
        ServerControlState* scs;
        int updateMotion = 0;

        controls->received_update_id = csc->applied_server_state_id;

        for(scs = controls->queued_server_states.head; scs; scs = controls->queued_server_states.head)
        {
            // Check if this ID is valid.

            diff = controls->received_update_id - scs->id;

            if(diff < 0)
                break;

            updateMotion = 1;

            //printf("%5dms: applying server state: %d\n", controls->time.playback.ms, scs->id);

            if(server_state->physics_disabled && !scs->physics_disabled)
            {
                controls->connStats.hit_end_count = 0;
                controls->time.playback.ms_acc = -100;
                controls->phys.acc = 1;

                //printf("re-enabled physics!!!!!!!!!\n");

                //svrPrintControlQueue(e->client);
            }

            memcpy(server_state, scs, sizeof(*scs));

            svrDestroyServerControlStateHead(controls);
        }

        if(updateMotion)
        {
            svrUpdateMotionState(e, controls);
        }
    }
}

static void svrApplyExpiredServerStates(Entity* e, ControlState* controls)
{
    int updateMotion = 0;
    int cur_server_state_sent = controls->cur_server_state_sent;
    ServerControlState* scs;

    for(scs = controls->queued_server_states.head; scs; scs = controls->queued_server_states.head)
    {
        if(    !cur_server_state_sent && !scs->next ||
            ABS_TIME_SINCE(scs->abs_time_sent) < SEC_TO_ABS(1.5))
        {
            break;
        }

        updateMotion = 1;

        controls->received_update_id = scs->id;

        //printf("applying expired server state: %d\n", scs->id);

        //if(controls->server_state.physics_disabled && !scs->physics_disabled)
        //    printf("re-enabled physics!!!!!!!!!\n");

        memcpy(controls->server_state, scs, sizeof(*scs));

        svrDestroyServerControlStateHead(controls);
    }

    if(updateMotion)
    {
        svrUpdateMotionState(e, controls);
    }
}

static F32 clampF32abs(F32 value, F32 max_abs)
{
    if(value > max_abs)
        return max_abs;
    else if(value < -max_abs)
        return -max_abs;
    else
        return value;
}

static void svrAdjustBodyPYR(Entity* e, ControlState* controls)
{
    ServerControlState* scs = controls->server_state;
    MotionState*        motion = e->motion;
    int                    usePYR;
    Vec3                body_pyr;
    int                    forcedTurn = controls->forcedTurn.hold_pyr || controls->forcedTurn.timeRemaining > 0;

    // Figure out if my body should be turned.

    usePYR =    forcedTurn
                ||
                e->pchar->attrCur.fHitPoints > 0 &&
                (    controls->nocoll ||
                    controls->alwaysmobile ||
                    (    !scs->controls_input_ignored &&
                        !controls->recover_from_landing_timer));

    if(usePYR)
    {
        int use_pitch = 0;
        Mat3 newmat;

        if (!scs->fly && !controls->nocoll)
        {
            body_pyr[0] = 0;
        }
        else
        {
            if(    scs->fly &&
                abs(controls->dir_key.total_ms[CONTROLID_FORWARD] - controls->dir_key.total_ms[CONTROLID_BACKWARD]) < 500 &&
                abs(controls->dir_key.total_ms[CONTROLID_UP] - controls->dir_key.total_ms[CONTROLID_DOWN]) < 500)
            {
                body_pyr[0] = 0;
            }
            else
            {
                body_pyr[0] = controls->pyr.cur[0];
                use_pitch = 1;
            }
        }

        if(use_pitch != controls->using_pitch){
            controls->using_pitch = use_pitch;

            if(!use_pitch)
            {
                Vec3 temp_vec;

                getMat3YPR(ENTMAT(e), temp_vec);

                controls->pitch_offset = temp_vec[0];

                controls->pitch_scale = 0;
            }
            else
            {
                controls->pitch_offset = 0;

                controls->pitch_scale = 1.0f - controls->pitch_scale;
            }
        }

        controls->pitch_scale += (1.0 - controls->pitch_scale) * 0.1 * TIMESTEP;

        if(controls->pitch_scale > 1)
            controls->pitch_scale = 1;

        if(controls->pitch_offset)
            body_pyr[0] = controls->pitch_offset * (1.0 - controls->pitch_scale);
        else
            body_pyr[0] *= controls->pitch_scale;

        if(forcedTurn)
        {
            Entity* faceEnt = erGetEnt(controls->forcedTurn.erEntityToFace);
            Vec3 pos;
            Vec3 diff;

            controls->forcedTurn.timeRemaining -= e->timestep;

            if(controls->forcedTurn.timeRemaining <= 0)
            {
                controls->last_vel_yaw = 0;
                controls->forcedTurn.timeRemaining = 0;
                controls->forcedTurn.hold_pyr = getMoveFlags(e->seq, SEQMOVE_CANTMOVE) ? 1 : 0;
            }

            if(faceEnt)
                copyVec3(ENTPOS(faceEnt), pos);
            else
                copyVec3(controls->forcedTurn.locToFace, pos);

            subVec3(pos, ENTPOS(e), diff);

            if(lengthVec3SquaredXZ(diff)){
                getVec3YP(diff, &body_pyr[1], &body_pyr[0]);
            }else{
                getVec3YP(ENTMAT(e)[2], &body_pyr[1], &body_pyr[0]);
            }

            if(scs->fly){
                if(body_pyr[0] < -RAD(45))
                    body_pyr[0] += RAD(45);
                else if(body_pyr[0] > RAD(45))
                    body_pyr[0] -= RAD(45);
                else
                    body_pyr[0] = 0;

                //if(faceEnt)
                //{
                //    printf("facing ent, pitch: %1.3f\n", body_pyr[0]);
                //}
            }else{
                body_pyr[0] = 0;
            }
        }
        else
        {
            F32 yaw_diff;

            if(controls->no_strafe || e->seq->type->noStrafe)
            {
                F32 air_scale = motion->input.flying ?
                                    0.05 :
                                    motion->falling || motion->jumping ?
                                        0.02 :
                                        vec3IsZero(controls->last_inp_vel) ?
                                            0.2 :
                                            0.4;
                F32 swap_threshold;
                F32 vel_yaw;

                // Calculate the yaw caused by the input velocity.

                vel_yaw = getVec3Yaw(controls->last_inp_vel);

                if(controls->last_inp_vel[2] < 0)
                {
                    vel_yaw = addAngle(vel_yaw, RAD(180));
                    swap_threshold = RAD(130);
                }
                else
                {
                    if(!controls->yaw_swap_backwards && controls->last_vel_yaw >= -RAD(90) && controls->last_vel_yaw <= RAD(90))
                    {
                        swap_threshold = RAD(175);
                    }
                    else
                    {
                        swap_threshold = RAD(130);
                    }
                }

                yaw_diff = fabs(subAngle(vel_yaw, controls->last_vel_yaw));

                if(yaw_diff > swap_threshold)
                {
                    if(controls->yaw_swap_timer > 0)
                    {
                        controls->yaw_swap_timer -= TIMESTEP;

                        if(controls->yaw_swap_timer < 1)
                            controls->yaw_swap_timer = 1;
                    }
                    else
                    {
                        if(controls->last_inp_vel[2] < 0)
                        {
                            controls->yaw_swap_timer = 15;
                            controls->yaw_swap_backwards = 1;
                        }
                        else
                        {
                            controls->yaw_swap_timer = 7;
                            controls->yaw_swap_backwards = 0;
                        }
                    }

                    if(controls->yaw_swap_timer > 1)
                    {
                        vel_yaw = addAngle(vel_yaw, RAD(180));
                    }
                }else{
                    controls->yaw_swap_timer = 0;
                }

                yaw_diff = clampF32abs(air_scale * subAngle(vel_yaw, controls->last_vel_yaw), TIMESTEP * RAD(10));

                controls->last_vel_yaw = addAngle(controls->last_vel_yaw, yaw_diff);
            }
            else
            {
                controls->last_vel_yaw = 0;
            }

            body_pyr[1] = addAngle(controls->pyr.cur[1], controls->last_vel_yaw);
        }

        body_pyr[2] = controls->pyr.cur[2];

        createMat3RYP(newmat,body_pyr);
        entSetMat3(e, newmat);
    }
    else
    {
        Mat3 newmat;
        // Not using PYR, so pitch to zero.

        getMat3YPR(ENTMAT(e), body_pyr);
        body_pyr[0]=0;
        createMat3RYP(newmat,body_pyr);
        entSetMat3(e, newmat);
    }
}

static void createKeyString(char* key_string, ControlState* controls)
{
    char* cur = key_string;
    int i;

    key_string[0] = 0;

    for(i = 0; i < CONTROLID_BINARY_MAX; i++)
    {
        if(controls->dir_key.total_ms[i] || controls->dir_key.down_now & (1 << i))
        {
            cur += sprintf(cur,    "%s%s (%dms), ",
                                controls->dir_key.down_now & (1 << i) ? "+" : "-",
                                pmotionGetBinaryControlName(i),
                                controls->dir_key.total_ms[i]);
        }
    }
}

static void hexDump(void* dataParam, U32 length)
{
    U8* data = dataParam;
    int odd = 1;

    printf("---------------------\n");
    while(length)
    {
        int writeLength = min(length, 16);
        int sum = 0;
        int i;

        odd = !odd;

        printf("%8.8x:   ", data - (U8*)dataParam);

        for(i = 0; i < writeLength; i++)
        {
            if(i % 4 == 0)
            {
                consoleSetColor((odd?COLOR_BRIGHT:0)|(i%8?COLOR_GREEN:0)|COLOR_BLUE, i%8?COLOR_BLUE:0);
            }

            sum += data[i];
            printf("%2.2x ", data[i]);
        }

        for(; i < 16; i++)
        {
            printf("   ");
        }

        consoleSetColor(1 + sum % 15, 0);
        printf(" = %8.8x\n", sum);
        consoleSetDefaultColor();

        data += writeLength;
        length -= writeLength;
    }
    printf("---------------------\n\n");
}

static void svrPlayerApplyFuturePushes(Entity* e, ControlState* controls, ControlStateChange* csc){
    MotionState* motion = e->motion;
    FuturePush* fp = controls->future_push_list;
    FuturePush* prev = NULL;
    FuturePush* next;

    for(; fp; fp = next)
    {
        next = fp->controls_next;

        if(!fp->net_id_used)
        {
            if(fp->phys_tick_remaining)
            {
                fp->phys_tick_remaining--;
            }

            if(!fp->phys_tick_remaining)
            {
                // Apply the push.

                if(fp->do_knockback_anim)
                {
                    SETB(e->seq->state, STATE_KNOCKBACK);
                    SETB(e->seq->state, STATE_HIT);
                }

                pmotionApplyFuturePush(motion, fp);

                fp->net_id_used = 1;

                // Destroy if this FuturePush was already sent.

                if(fp->was_sent)
                {
                    destroyFuturePush(fp);

                    if(prev)
                    {
                        prev->controls_next = next;
                    }
                    else
                    {
                        controls->future_push_list = next;
                    }

                    fp = NULL;
                }
            }
        }

        if(fp)
        {
            prev = fp;
        }
    }
}

static int svrRunPhysicsStep(    Entity* e,
                                ControlState* controls,
                                ControlStateChange* csc)
{
    MotionState* motion = controlInstance.motion;
    ServerControlState* scs = controls->server_state;
    Vec3 new_vel;
    char key_string[200];
    int do_physics = !scs->physics_disabled && !scs->no_physics;
    int controldebug = controls->controldebug;

    struct {
        Vec3    start_vel;
        float    gravity;
        int        jumping;
    } debug;

    //printf("pos %5d :\ttimes %8d\t%8d\n", csc->pos_id, csc->client_abs, csc->client_abs_slow);

    controls->controls_input_ignored_this_csc = csc && csc->controls_input_ignored && !controls->alwaysmobile;

    pmotionUpdateControlsPrePhysics(e, controls, controls->time.playback.ms);

    // Set the motion PYR.

    controls->pyr.cur[2] = 0;
    copyVec3(controls->pyr.cur, controls->pyr.motion);

    if(!scs->fly && !controls->nocoll)
    {
        controls->pyr.motion[0] = 0;
    }

    // Get the key string for debugging.

    if(controldebug && !server_visible_state.pause)
    {
        createKeyString(key_string, controls);
    }

    // Run the physics now.

    svrRecordLastGoodPosition(e);

    controls->inp_vel_scale = csc ? (float)csc->inp_vel_scale_U8 / 255.0 : 1.0;

    pmotionSetVel(e, controls, controls->inp_vel_scale);

    // Set speed scale and max jump.

    motion->input.max_speed_scale    = controls->max_speed_scale;
    motion->input.max_jump_height    = pmotionGetMaxJumpHeight(e, controls);
    motion->input.stunned            = controls->server_state->stun;

    if(!controls->controls_input_ignored_this_csc)
    {
        Mat3 control_mat;

        copyVec3(motion->input.vel, controls->last_inp_vel);

        createMat3RYP(control_mat, controls->pyr.motion);
        mulVecMat3(motion->input.vel, control_mat, new_vel);
    }
    else
    {
        zeroVec3(controls->last_inp_vel);

        zeroVec3(new_vel);
    }

    if(controldebug)
    {
        copyVec3(motion->vel, debug.start_vel);
        debug.gravity = motion->input.surf_mods[motion->input.flying].gravity;
        debug.jumping = motion->jumping;
    }

    if(do_physics){
        extern int dump_grid_coll_info;

        // Restore the last state.

        if(controlInstance.movementAllowed)
        {
            controls->start_abs_time = controls->stop_abs_time;
            copyVec3(controls->end_pos, controls->start_pos);
            entUpdatePosInterpolated(e, controls->end_pos);
            e->timestep = 1;
            copyVec3(new_vel, motion->input.vel);
        }

        if(controls->recover_from_landing_timer)
        {
            //printf("recover remaining: %d\n", controls->recover_from_landing_timer);
            controls->recover_from_landing_timer--;
        }

        // Run the physics.

        PERFINFO_AUTO_START("entMotion", 1);
            //if(csc)
            //    printf("id: %d\n", csc->net_id);

            if(csc)
            {
                setEntCollTimes(1, csc->client_abs, csc->client_abs_slow);
                controls->stop_abs_time = csc->client_abs;
            }
            else
            {
                controls->start_abs_time = controls->stop_abs_time = ABS_TIME;
                setEntCollTimes(1, ABS_TIME, ABS_TIME);
            }

            // Apply future pushes.

            svrPlayerApplyFuturePushes(e, controls, csc);

            // Run physics code.

            //dump_grid_coll_info = controldebug;

            if(controlInstance.movementAllowed)
            {
                static const char* trackName = "server";

                int record_motion = controls->record_motion;

                // Record motion BEFORE physics.

                if(record_motion)
                {
                    pmotionRecordStateBeforePhysics(e, trackName, csc);
                }

                // Run the physics.

                entMotion(e, unitmat);

                // Record motion AFTER physics.

                if(record_motion)
                {
                    pmotionRecordStateAfterPhysics(e, trackName);
                }
            }

            //dump_grid_coll_info = 0;
        PERFINFO_AUTO_STOP();

        copyVec3(ENTPOS(e), controls->end_pos);

        // Stuff the physics position array.

        if(controlInstance.movementAllowed && csc && !csc->no_ack)
        {
            //printf("pos %d: %1.3f, %1.3f, %1.3f\n", csc->net_id, vecParamsXYZ(e->mat[3]));

            // Store the ID, positions, and velocity.

            controls->phys.latest.net_id = csc->net_id;
            copyVec3(ENTPOS(e), controls->phys.latest.pos);
            copyVec3(motion->vel, controls->phys.latest.vel);
        }

        // Print out debugging stuff.

        if(controldebug)
        {
            if(key_string[0] || !sameVec3(controls->start_pos, controls->end_pos))
            {
                controls->print_physics_step_count++;

                if(controls->print_physics_step_count == 0)
                {
                    printf(    "\n\n\n\n"
                            "----------------------------------------------------------------------\n"
                            "------- BEGIN PHYSICS STEPS ------------------------------------------\n"
                            "----------------------------------------------------------------------\n"
                            "\n");
                }
                else if(controls->print_physics_step_count % 10 == 0)
                {
                    controls->print_physics_step_count = controls->print_physics_step_count % 1000;

                    printf(    "\n%d --------------------------------------------------------------------\n",
                            controls->print_physics_step_count);
                }

                #define TRIPLET "%1.8f, %1.8f, %1.8f"

                printf(    "\n"
                        "%4d.\t\tid=%5d, pak_id=%d:%d curTime=%dms\n"
                        "        keys:      %s\n"
                        "        pos:       ("TRIPLET")\n"
                        "      + vel:       ("TRIPLET")\n"
                        "      + inpvel:    ("TRIPLET") @ %f\n"
                        "      + pyr:       ("TRIPLET")\n"
                        "      + misc:      grav=%1.3f, %s%s\n"
                        "      + move_time: %1.3f\n"
                        "      = newpos:    ("TRIPLET")\n"
                        "        newvel:    ("TRIPLET")\n"
                        "%s"
                        "%s",
                        controls->print_physics_step_count,
                        csc ? csc->net_id : -1,
                        csc ? csc->pak_id : -1,
                        csc ? csc->pak_id_index : -1,
                        csc ? csc->time_diff_ms : -1,
                        key_string,
                        vecParamsXYZ(controls->start_pos),
                        vecParamsXYZ(debug.start_vel),
                        vecParamsXYZ(new_vel),
                        controls->inp_vel_scale,
                        vecParamsXYZ(controls->pyr.cur),
                        debug.gravity,
                        debug.jumping ? "Jumping, " : "",
                        scs->controls_input_ignored ? "ControlsInputIgnored, " : "",
                        motion->move_time,
                        vecParamsXYZ(controls->end_pos),
                        vecParamsXYZ(motion->vel),
                        scs->no_ent_collision ? "    ** NO ENT COLL **\n" : "",
                        scs->fly ? "    ** FLYING **\n" : "");

                #undef TRIPLET
            }
            else
            {
                if(controls->print_physics_step_count != -1)
                {
                    controls->print_physics_step_count = -1;

                    printf(    "\n"
                            "----------------------------------------------------------------------\n"
                            "-------- END PHYSICS STEPS -------------------------------------------\n"
                            "----------------------------------------------------------------------\n"
                            "\n");
                }
            }
        }
    }

    pmotionUpdateControlsPostPhysics(controls, controls->time.playback.ms);

    return do_physics;
}

static int svrRunControlsUntilPhysics(    ClientLink *client,
                                        Entity* e,
                                        ControlState* controls)
{
    U16 playback_ms = (U16)controls->time.playback.ms_acc;
    int controldebug = controls->controldebug;
    U8 use_prev = controls->dir_key.use_now;
    ControlStateChange* csc;

    if(controls->time.playback.ms_acc < 0)
    {
        return 0;
    }

    // Process controls that happened up to current point.

    for(csc = controls->csc_list.head; csc; csc = controls->csc_list.head)
    {
        int done = 0;
        int id;
        U8    dir_key_cur;

        if(csc->time_diff_ms >= playback_ms)
        {
            #if 0 && DUMP_CONTROLS
                printf("STOPPING, current queue:\n");

                svrPrintControlQueue(client);

                printf("  ----------------\n");
            #endif
            break;
        }


        #if 0 && DUMP_CONTROLS
            if(id < CONTROLID_BINARY_MAX)
            {
                printf(    "USING       type %d(%d) @ %dms, [%d]\n",
                        id,
                        csc->state,
                        csc->time_diff_ms);
            }
            else if(id < CONTROLID_ANGLE_MAX)
            {
                printf(    "USING       type %d(%1.3f) @ %dms, [%d]\n",
                        id,
                        csc->angle,
                        csc->time_diff_ms);
            }
        #endif

        playback_ms -= csc->time_diff_ms;

        controls->time.playback.ms_acc -= csc->time_diff_ms;

        controls->time.playback.ms += csc->time_diff_ms;

        id = csc->control_id;

        dir_key_cur = controls->dir_key.down_now;

        controls->dir_key.down_now = csc->last_key_state;

        if(id < CONTROLID_BINARY_MAX)
        {
            // Directional control update.

            if(controlInstance.movementAllowed)
            {
                if(((dir_key_cur >> id) & 1) != csc->state)
                {
                    int bit = 1 << id;

                    if(csc->state)
                    {
                        // Start pressing a key.

                        controls->dir_key.start_ms[id] = controls->time.playback.ms;

                        // If changed from previous step, enable for this step.

                        if(!(use_prev & bit))
                        {
                            controls->dir_key.use_now |= bit;
                            controls->dir_key.affected_last_frame |= bit;
                        }

                        if(controldebug)
                        {
                            printf(    "\n%d:%d KeyDown: %s @ %dms%s\n",
                                    csc->pak_id,
                                    csc->pak_id_index,
                                    pmotionGetBinaryControlName(id),
                                    controls->time.playback.ms,
                                    csc->initializer ? " ** INITIALIZER **" : "");
                        }
                    }
                    else
                    {
                        // Key being released.

                        int add_ms;

                        // If changed from previous step, disable for this step.

                        if(use_prev & bit)
                        {
                            controls->dir_key.use_now &= ~bit;
                            controls->dir_key.affected_last_frame |= bit;
                        }

                        if(id == CONTROLID_UP)
                        {
                            controlInstance.motion->input.jump_released = 1;
                        }

                        if(controls->dir_key.down_now & (1 << opposite_control_id[csc->control_id]))
                        {
                            // Add nothing if the opposite key is held down on release.

                            add_ms = 0;
                        }
                        else
                        {
                            add_ms = controls->time.playback.ms - controls->dir_key.start_ms[id];

                            if(add_ms > 0)
                            {
                                controls->dir_key.total_ms[id] += add_ms;

                                if(controls->dir_key.total_ms[id] > 1000)
                                {
                                    controls->dir_key.total_ms[id] = 1000;
                                }
                            }
                            else if(!controls->dir_key.total_ms[id])
                            {
                                // Add at least 1ms on key release.

                                controls->dir_key.total_ms[id] = 1;
                            }
                        }

                        if(controldebug)
                        {
                            printf(    "\n%d:%d KeyUp: %s @ %dms, %dms added, %dms total%s\n",
                                    csc->pak_id,
                                    csc->pak_id_index,
                                    pmotionGetBinaryControlName(id),
                                    controls->time.playback.ms,
                                    add_ms,
                                    controls->dir_key.total_ms[id],
                                    csc->initializer ? " ** INITIALIZER **" : "");
                        }
                    }
                }
            }
        }
        else if(id < CONTROLID_ANGLE_MAX)
        {
            // Angle changing.

            if(controlInstance.movementAllowed)
            {
                //if(id == CONTROLID_YAW){
                //    printf("applying yaw (%d): %1.3f\n", csc->net_id, csc->angleF32);
                //}

                controls->pyr.cur[id - CONTROLID_BINARY_MAX] = csc->angleF32;
            }
        }
        else if(id == CONTROLID_NOCOLL)
        {
            // Toggling nocoll, if I have better than "user" access level, or if I'm turning it off.

            if(    client->entity->access_level > ACCESS_USER ||
                controls->nocoll && !csc->state2)
            {
                controls->nocoll = csc->state2;

                // Disable falling damage if you nocoll-drop yourself.

                if(!controls->nocoll)
                {
                    e->noDamageOnLanding = 1;
                }
            }
        }
        else if(id == CONTROLID_APPLY_SERVER_STATE)
        {
            // Apply the acknowledged server state.

            svrApplyServerState(e, controls, controlInstance.motion, csc);
        }
        else if(id == CONTROLID_RUN_PHYSICS)
        {
            // Run physics.

            int did_physics;

            did_physics = svrRunPhysicsStep(e, controls, csc);

            svrDestroyControlStateChangeHead(controls);

            return did_physics;
        }

        svrDestroyControlStateChangeHead(controls);
    }

    return 0;
}

static void svrGetNextFuturePosition(    ClientLink* client,
                                        Entity* e,
                                        ControlState* controls)
{
    int physics_disabled = controls->server_state->physics_disabled;
    Vec3 diff;

    while(controls->phys.acc >= 1)
    {
        F32 prev_acc = controls->phys.acc;

        if(!physics_disabled)
        {
            controls->phys.acc -= 1;
        }

        if(!svrRunControlsUntilPhysics(client, e, controls))
        {
            if(!physics_disabled)
            {
                controls->connStats.hit_end_count++;

                //printf("hit end: %1.3f->%1.3f, %1.3f, %d ==> %d\n", prev_acc, controls->phys.acc, controls->time.playback.ms_acc, ABS_TIME, controls->connStats.hit_end_count);

                //svrPrintControlQueue(client);
            }

            break;
        }
    }

    if(controlInstance.movementAllowed)
    {
        Vec3 newpos;
        F32 factor = min(1, controls->phys.acc);
        U32 time_diff = controls->stop_abs_time - controls->start_abs_time;

        subVec3(controls->end_pos, controls->start_pos, diff);
        scaleVec3(diff, factor, diff);
        addVec3(controls->start_pos, diff, newpos);
        entUpdatePosInterpolated(e, newpos);

        controls->cur_abs_time = controls->start_abs_time + factor * time_diff;

        pmotionSetState(e, controls);
    }
}

void svrTickUpdatePlayerFromControls(ClientLink *client)
{
    Entity*            e = clientGetControlledEntity(client);
    ControlState*    controls = &client->controls;
    F32                time_step = TIMESTEP_NOSCALE + global_state.frame_time_30_discarded;

    if(!e)
    {
        return;
    }

    if(!controls->connStats.received_controls)
    {
        // Can't advance time until something has been received from the client.

        time_step = 0;
    }

    if(time_step > 30)
    {
        //printf("time_step clamping to 30, losing %1.2f\n", time_step - 30);

        time_step = 30;
    }

    controlInstance.e = e;
    controlInstance.controls = controls;
    controlInstance.motion = e->motion;

    if(    !e->adminFrozen &&
        !e->doorFrozen &&
        controls->server_pos_id == controls->client_pos_id)
    {
        controlInstance.movementAllowed = 1;
    }
    else
    {
        controlInstance.movementAllowed = 0;
    }

    //if(!controlInstance.movementAllowed){
    //    printf("frozen yaw: %1.3f\n", controls->pyr.cur[1]);
    //}

    //if(global_state.frame_time_30_discarded)
    //{
    //    printf(    "discarding %1.2f timesteps -------------------------------------------------------------------------\n",
    //            global_state.frame_time_30_discarded);
    //}

    // Update how many milliseconds have passed based on TIMESTEP.

    controls->time.playback.ms_acc += 1000.0 * time_step / 30.0;
    controls->phys.acc += time_step;

    svrGetNextFuturePosition(client, e, controls);

    // Update PYR.

    if(controlInstance.movementAllowed)
    {
        svrAdjustBodyPYR(e, controls);
    }

    // Check for expired server states.

    svrApplyExpiredServerStates(e, controls);

    // Update the connection stats.

    if(controls->time.total_ms > controls->connStats.max_total)
    {
        controls->connStats.max_total = controls->time.total_ms;
    }

    if(controls->time.total_ms < controls->connStats.min_total)
    {
        controls->connStats.min_total = controls->time.total_ms;
    }

    controls->connStats.check_acc -= time_step;

    if(controls->connStats.check_acc <= 0)
    {
        if(controls->connStats.conncheck_count < 2)
        {
            controls->connStats.conncheck_count++;
        }
        else
        {
            //printf(    "min_total: %d, "
            //        "max_total: %d, "
            //        "hit_end: %d, "
            //        "\n",
            //        controls->connStats.min_total,
            //        controls->connStats.max_total,
            //        controls->connStats.hit_end_count);

            if(controls->connStats.max_total >= 1500)
            {
                svrResetClientControls(client, 0);
            }
            else if(controls->connStats.hit_end_count)
            {
                if(controls->connStats.hit_end_count > 10)
                {
                    svrResetClientControls(client, 0);
                }
                else if(controls->phys.acc >= 0)
                {
                    //printf(    "\n\n************* HIT END %dx ***************\n\n\n",
                    //        controls->connStats.hit_end_count);

                    controls->phys.acc -= 1;
                }
            }

            if(controls->connStats.min_total > 50)
            {
                S32 diff_ms = (controls->connStats.min_total - 50) / 10;

                //printf("min_total: %d\n", controls->connStats.min_total);

                if(diff_ms)
                {
                    controls->phys.acc += diff_ms * 30.0 / 1000.0;
                    controls->time.playback.ms_acc += diff_ms;
                }
            }
        }

        if(controls->time.playback.ms_acc > 2000)
        {
            controls->time.playback.ms_acc = 2000;
        }

        controls->connStats.check_acc = 30;
        controls->connStats.hit_end_count = 0;
        controls->connStats.min_total = 10000;
        controls->connStats.max_total = 0;
    }

    //if(0)
    //{
    //    printf(    "%5dms\t%5d queued (%dms)\tphys %2.2f\n",
    //            controls->time.playback.ms,
    //            controls->csc_list.count,
    //            controls->time.total_ms,
    //            controls->phys.acc);
    //}

    // Player could have logged in inside a volume which is not supposed to
    // inhibit reject-teleport. If so all we can really do is teleport them
    // to their login position.
    if(client->entity->pl && (!client->entity->pl->rejectPosVolume || !client->entity->pl->rejectPosInited))
    {
        copyVec3(ENTPOS(client->entity), client->entity->pl->volume_reject_pos);
        client->entity->pl->rejectPosInited = 1;
    }

    if(ENTTYPE_PLAYER == ENTTYPE(client->entity))
    {
        player_count++;
    }
}

static void receiveControlStateAngleInit(    Packet *pak,
                                            ClientLink *client,
                                            U8 last_key_state,
                                            Vec3 last_pyr,
                                            int index,
                                            int oo_packet,
                                            U16* pak_id_index)
{
    U32 angleU32 = pktGetBits(pak, CSC_ANGLE_BITS);
    F32 angleF32 = CSC_ANGLE_U32_TO_F32(angleU32);

    if(!oo_packet)
    {
        ControlId id = CONTROLID_PITCH + index;

        if(angleF32 != last_pyr[index])
        {
             ControlStateChange* csc = createControlStateChange();

            csc->control_id = id;
            csc->time_diff_ms = 0;
            csc->angleF32 = angleF32;
            csc->pak_id = pak->id;
            csc->pak_id_index = (*pak_id_index)++;
            csc->last_key_state = last_key_state;
            csc->initializer = 1;

            last_pyr[index] = angleF32;

            csc->last_pyr[0] = last_pyr[0];
            csc->last_pyr[1] = last_pyr[1];

            if(client->controls.csc_list.count < 100)
            {
                addControlStateChange(&client->controls.csc_list, csc, 1);

                //if(id == CONTROLID_YAW){
                //    printf("received yaw (0), %1.3f\n", angleF32);
                //}
            }
            else
            {
                destroyControlStateChange(csc);
            }

            //printf("  received angle init %d: %dms, %1.3f --> %1.3f\n", csc->control_id, csc->time_stamp, client->controls.pyr_last_send[index], csc->angle);
        }
    }
}

static void receiveControlUpdates(Packet* pak, ClientLink* client)
{
    ControlState* controls = &client->controls;
    ControlStateChange* insert_here;
    S32     pak_id = pak->id;
    S32     oo_packet = 0;
    S32     latest_packet = 0;
    U16     pak_id_index = 0;
    U8        last_key_state;
    Vec3    last_pyr;
    S32     i;

    if(!pktGetBits(pak, 1))
        return;

    controls->connStats.received_controls = 1;

    // Get the position to insert the new control changes.

    for(insert_here = controls->csc_list.tail; insert_here; insert_here = insert_here->prev)
    {
        int id_diff = pak_id - insert_here->pak_id;

        //assert(pak_id && insert_here->pak_id);

        if(1 || id_diff > 0)
        {
            last_key_state = insert_here->last_key_state;
            copyVec3(insert_here->last_pyr, last_pyr);
            break;
        }
    }

    if(!insert_here)
    {
        last_key_state = controls->dir_key.down_now;
        copyVec3(controls->pyr.cur, last_pyr);
    }

    if(pak_id - controls->connStats.highest_packet_id > 0)
    {
        controls->connStats.highest_packet_id = pak_id;

        latest_packet = 1;
    }
    else
    {
        oo_packet = 1;
    }

    // Receive all the changes.

    if(pktGetBits(pak, 1))
    {
        S32 totalTime = 0;
        S32 delta_bits = pktGetBits(pak, 5) + 1;
        U16 cur_net_id = pktGetBits(pak, 16);
        S32 first_run_physics = 1;
        U32 last_client_abs;
        U32 last_client_abs_slow;
        U16 last_net_id = controls->last_net_id;

        for(i = 0; !i || pktGetBits(pak, 1); i++)
        {
            ControlStateChange* csc;
            ControlStateChange    temp_csc;
            int keeping_csc;
            U16 new_net_id = cur_net_id++;
            S16 net_id_diff = new_net_id - last_net_id;

            if (pktEnd(pak)) {
                // dbLog(cheater) happens in caller
                return;
            }

            // Figure out if this is a CSC we need.

            keeping_csc = net_id_diff > 0;

            if(keeping_csc)
                csc = createControlStateChange();
            else
                csc = &temp_csc;

            // Read the CSC.

            csc->net_id = new_net_id;

            #if DUMP_CONTROLS
                if(!i && !server_visible_state.pause)
                    printf("\n*********** CONTROL UPDATE (id %d, %d bytes) *****************\n", pak_id, pak->stream.size);
            #endif

            csc->control_id = pktGetBits(pak, 1) ? CONTROLID_RUN_PHYSICS : pktGetBits(pak, 4);
            csc->pak_id = pak_id;
            csc->pak_id_index = pak_id_index++;
            csc->last_key_state = last_key_state;
            csc->last_pyr[0] = last_pyr[0];
            csc->last_pyr[1] = last_pyr[1];

            if(pktGetBits(pak, 1))
            {
                csc->time_diff_ms = 32 + pktGetBits(pak, 2);
            }
            else
            {
                csc->time_diff_ms = pktGetBits(pak, delta_bits);
            }

            if(keeping_csc)
            {
                totalTime += csc->time_diff_ms;
            }

            if(csc->control_id < CONTROLID_BINARY_MAX)
            {
                // Binary key change.

                csc->state = pktGetBits(pak, 1);

                if(keeping_csc)
                {
                    if(csc->state)
                        last_key_state |= (1 << csc->control_id);
                    else
                        last_key_state &= ~(1 << csc->control_id);

                    csc->last_key_state = last_key_state;
                }

                #if DUMP_CONTROLS
                    if(!server_visible_state.pause)
                        printf(    "  %d. received control update: control %d, %dms, state %d\n",
                                i,
                                csc->control_id,
                                csc->time_diff_ms,
                                csc->state);
                #endif
            }
            else if(csc->control_id < CONTROLID_ANGLE_MAX)
            {
                // Angle change.

                U32 angleU32 = pktGetBits(pak, CSC_ANGLE_BITS);

                if(keeping_csc)
                {
                    S32 index = csc->control_id - CONTROLID_PITCH;
                    F32 angleF32 = CSC_ANGLE_U32_TO_F32(angleU32);

                    //if(csc->control_id == CONTROLID_YAW){
                    //    printf("received yaw (%d): %1.3f\n", csc->net_id, angleF32);
                    //}

                    last_pyr[index] = csc->last_pyr[index] = csc->angleF32 = angleF32;
                }

                #if DUMP_CONTROLS
                    if(!server_visible_state.pause)
                        printf(    "  %d. received angle update: control %d, %dms, %1.3f\n",
                                i,
                                csc->control_id,
                                csc->time_diff_ms,
                                csc->angle);
                #endif
            }
            else if(csc->control_id == CONTROLID_RUN_PHYSICS)
            {
                csc->controls_input_ignored = pktGetBits(pak, 1);
                csc->no_ack = !latest_packet;

                if(first_run_physics)
                {
                    S32 diff;

                    first_run_physics = 0;

                    csc->client_abs = pktGetBits(pak, 32);
                    diff = pktGetBitsPack(pak, 10);
                    csc->client_abs_slow = csc->client_abs - diff;
                }
                else
                {
                    csc->client_abs =        last_client_abs +        pktGetBitsPack(pak, 8);
                    csc->client_abs_slow =    last_client_abs_slow +    pktGetBitsPack(pak, 8);
                }

                if(pktGetBits(pak, 1))
                {
                    csc->inp_vel_scale_U8 = pktGetBits(pak, 8);
                }
                else
                {
                    csc->inp_vel_scale_U8 = 255;
                }

                last_client_abs = csc->client_abs;
                last_client_abs_slow = csc->client_abs_slow;

                #if 0 && DUMP_CONTROLS
                    if(!server_visible_state.pause)
                        printf(    "  %d. received run physics: control %d, %dms\n",
                                i,
                                csc->control_id,
                                csc->time_stamp_ms);
                #endif
            }
            else if(csc->control_id == CONTROLID_APPLY_SERVER_STATE)
            {
                csc->applied_server_state_id = pktGetBits(pak, 8);
            }
            else if(csc->control_id == CONTROLID_NOCOLL)
            {
                csc->state2 = pktGetBits(pak, 2);
            }

            if(keeping_csc)
            {
                if(controls->csc_list.count < 100)
                {
                    // Insert the CSC into the list.

                    if(insert_here)
                    {
                        csc->prev = insert_here;
                        csc->next = insert_here->next;

                        insert_here->next = csc;
                    }
                    else
                    {
                        csc->prev = NULL;
                        csc->next = controls->csc_list.head;

                        controls->csc_list.head = csc;
                    }

                    if(csc->next)
                    {
                        csc->next->prev = csc;
                    }
                    else
                    {
                        controls->csc_list.tail = csc;
                    }

                    insert_here = csc;

                    controls->csc_list.count++;
                }
                else
                {
                    destroyControlStateChange(csc);
                }
            }
        }

        if(!oo_packet)
        {
            controls->last_net_id = --cur_net_id;
        }

        // Add the total time received in this packet.

        controls->time.total_ms += totalTime;
    }

    // Receive the final state of the binary controls.
    //   If no packets were lost, this should be the same as what's in last_key_state.

    for(i = 0; i < CONTROLID_BINARY_MAX; i++)
    {
        S32 state = pktGetBits(pak, 1);

        if(latest_packet)
        {
            if(state != ((last_key_state >> i) & 1))
            {
                ControlStateChange* csc = createControlStateChange();

                csc->control_id = i;
                csc->time_diff_ms = 0;
                csc->state = state;
                csc->pak_id = pak_id;
                csc->pak_id_index = pak_id_index++;
                csc->last_pyr[0] = last_pyr[0];
                csc->last_pyr[1] = last_pyr[1];
                csc->initializer = 1;

                if(state)
                    last_key_state |= (1 << i);
                else
                    last_key_state &= ~(1 << i);

                csc->last_key_state = last_key_state;

                if(client->controls.csc_list.count < 100)
                {
                    addControlStateChange(&controls->csc_list, csc, 1);
                }
                else
                {
                    destroyControlStateChange(csc);
                }

                #if DUMP_CONTROLS
                    printf("  received control check %d, %d\n", i, state);
                #endif
            }
        }
    }

    // Get the angle initializer states if present.

    if(pktGetBits(pak, 1))
    {
        // Receive the current initial state of the angle controls.

        receiveControlStateAngleInit(pak, client, last_key_state, last_pyr, 0, oo_packet, &pak_id_index);
        receiveControlStateAngleInit(pak, client, last_key_state, last_pyr, 1, oo_packet, &pak_id_index);
    }
}

static U64 getBits64(Packet* pak){
    int byte_count = pktGetBits(pak, 3) + 1;
    U64 value64 = 0;
    U32* value32 = (U32*)&value64;

    if(byte_count > 4){
        *value32 = pktGetBits(pak, 32);
        value32++;
        byte_count -= 4;
    }

    *value32 = pktGetBits(pak, byte_count * 8);

    return value64;
}

//
// Receive commands from the client.
//
void receiveClientInput(Packet *pak,ClientLink *client)
{
    ControlLogEntry* lastLogEntry = NULL;
    ControlState* controls = &client->controls;
    int    old_notimeout;
    CmdContext context = {0};

    static CmdList cmd_list =
    {
        {{ control_cmds },
        { server_visible_cmds },
        { 0, }},
    };

    //if(0 && client->controls.controldebug){
    //    static last_second = -1;
    //    static second_packet = 0;
    //    int cur_second = time(NULL) % 60;

    //    if(cur_second != last_second)
    //        second_packet = 0;

    //    printf(    "%s%2d. %2d: client pak %d: %d bytes - %d\n",
    //            cur_second != last_second ? "--------------------------------------\n" : "",
    //            second_packet++,
    //            cur_second,
    //            pak->id,
    //            pak->stream.size,
    //            client->controls.snc_server.pl);

    //    last_second = cur_second;
    //}

    // Receive control updates.

    //printf("ms_total: %d -> ", client->controls.time.total_ms);

    receiveControlUpdates(pak, client);

    //printf("%d\n", client->controls.time.total_ms);

    // Receive selected entity.

    if(pktGetBits(pak, 1))
    {
        controls->selected_ent_server_index = pktGetBitsPack(pak, 14);
        controls->selected_ent_db_id = 0;
    }
    else
    {
        controls->selected_ent_server_index = 0;
        controls->selected_ent_db_id = pktGetBitsPack(pak, 14);
    }

    //TO DO this probably doesn't go here, nor should lastSelectedPet be on he entity, probably
    {
        Entity * eTarget = validEntFromId( controls->selected_ent_server_index );
        if( eTarget && eTarget->erOwner && eTarget->erOwner == erGetRef( client->entity ) )
            client->entity->lastSelectedPet = erGetRef( eTarget );
    }
    // Receive control log.

    while(pktGetBits(pak, 1) && !pktEnd(pak))
    {
        ControlLogEntry* entry = controls->controlLog.entries + controls->controlLog.cur;

        controls->controlLog.cur = (controls->controlLog.cur + 1) % ARRAY_SIZE(controls->controlLog.entries);

        if(!lastLogEntry)
        {
            entry->actualTime = pktGetBits(pak, 32);
            entry->mmTime = pktGetBits(pak, 32);
            entry->timestep = pktGetF32(pak);
            entry->timestep_noscale = pktGetBits(pak, 1) ? pktGetF32(pak) : entry->timestep;
            entry->qpc = getBits64(pak);
            entry->qpcFreq = getBits64(pak);
        }
        else
        {
            entry->actualTime = lastLogEntry->actualTime + pktGetBitsPack(pak, 1);
            entry->mmTime = lastLogEntry->mmTime + pktGetBitsPack(pak, 1);
            entry->timestep = pktGetF32(pak);
            entry->timestep_noscale = pktGetBits(pak, 1) ? pktGetF32(pak) : entry->timestep;
            entry->qpc = lastLogEntry->qpc + getBits64(pak);
            entry->qpcFreq = pktGetBits(pak, 1) ? getBits64(pak) : lastLogEntry->qpcFreq;
        }

        lastLogEntry = entry;
    }

    if (pktEnd(pak)) {
        dbLog("cheater",client->entity,"Authname %s, End of packet inside receiveClientInput()", client->entity?client->entity->auth_name:"NULL");
        return;
    }

    // Receive control commands.

    PERFINFO_AUTO_START("cmdReceive", 1);
        old_notimeout = client->controls.notimeout;

        cmdOldAddDataSwap(&context,&control_state,sizeof(ControlState),controls);

        context.access_level = client->entity?client->entity->access_level:ACCESS_USER;

        cmdOldReceive(pak, &cmd_list, &context);

        if (old_notimeout != client->controls.notimeout) { // Only change it if it changed (in case of mapserver -notimeout)
            client->link->notimeout = !!client->controls.notimeout;
        }

        cmdOldClearDataSwap(&context);

    PERFINFO_AUTO_STOP();

    //if(client->debugMenuFlags == -1)
    //{
    //    client->debugMenuFlags = 0;
    //    checkPlayerPowers(NULL, 1);
    //}
    //checkPlayerPowers(client->entity, 0);

    // Align the packet data to match up with what was sent
    pktAlignBitsArray(pak);

    PERFINFO_AUTO_START("parseClientInput", 1);
        parseClientInput(pak, client);
    PERFINFO_AUTO_STOP();

    //checkPlayerPowers(client->entity, 0);

    // Simulate network conditions.

    lnkSimulateNetworkConditions(    client->link,
                                    controls->snc_server.lag,
                                    controls->snc_server.lag_vary,
                                    controls->snc_server.pl,
                                    controls->snc_server.oo_packets);
}


Entity* svrFindPlayerInTeamup(int teamID, PlayerEntType type)
{
    int i;
    PlayerEnts* ents = &player_ents[type];
    for(i = 0; i < ents->count; i++)
    {
        Entity* e = ents->ents[i];
        if(e->teamup_id == teamID)
            return e;
    }

    return NULL;
}

void svrPlayerSetDisconnectTimer(Entity *e,int bad_connection)
{
    F32    seconds = server_state.client_logout_time;

    if (e->access_level > 0)
    {
        int debug_seconds = (int)(intptr_t)clientLinkGetDebugVar(e->client, "LogoutTimer");

        if(debug_seconds > 0)
        {
            seconds = debug_seconds;
        }
        else
        {
            seconds = 2;
        }
    }
    e->logout_timer = 30 * seconds;
    if (bad_connection)
    {
        e->logout_bad_connection = 1;
        e->logout_update = 2;
    }
    else
    {
        e->logout_update = 1;
    }
}

static void svrPlayerSendFuturePushList(Packet* pak, Entity* e){
    ControlState* controls = &e->client->controls;
    FuturePush* fp = controls->future_push_list;
    FuturePush* prev = NULL;

    while(fp && !fp->was_sent){
        FuturePush* next = fp->controls_next;

        fp->was_sent = 1;

        pktSendBits(pak, 1, 1);
        pktSendBits(pak, 16, fp->net_id_start);
        pktSendBitsPack(pak, 5, fp->phys_tick_wait);
        pktSendBits(pak, 32, fp->abs_time);
        pktSendF32(pak, fp->add_vel[0]);
        pktSendF32(pak, fp->add_vel[1]);
        pktSendF32(pak, fp->add_vel[2]);
        pktSendBits(pak, 1, fp->do_knockback_anim);

        if(fp->net_id_used)
        {
            if(prev)
            {
                prev->controls_next = next;
            }
            else
            {
                controls->future_push_list = next;
            }

            destroyFuturePush(fp);
        }
        else
        {
            prev = fp;
        }

        fp = next;
    }

    pktSendBits(pak, 1, 0);
}

static void svrPlayerSendPhysicsPositions(Packet* pak, Entity* e, ClientLink* client)
{
    ControlState* controls = &client->controls;

    // Send the physics positions.

    if(    controls->phys.latest.net_id >= 0 &&
        (    1 ||
            !sameVec3(controls->phys.latest.pos, controls->phys.sent.pos) ||
            !sameVec3(controls->phys.latest.vel, controls->phys.sent.vel)))
    {
        //printf("pos %d: %1.3f, %1.3f, %1.3f\n", controls->phys.latest.net_id, vecParamsXYZ(controls->phys.latest.pos));

        pktSendBits(pak, 1, 1);
        pktSendBits(pak, 16, controls->phys.latest.net_id);
        pktSendF32(pak, controls->phys.latest.pos[0]);
        pktSendF32(pak, controls->phys.latest.pos[1]);
        pktSendF32(pak, controls->phys.latest.pos[2]);
        pktSendIfSetF32(pak, controls->phys.latest.vel[0]);
        pktSendIfSetF32(pak, controls->phys.latest.vel[1]);
        pktSendIfSetF32(pak, controls->phys.latest.vel[2]);

        copyVec3(controls->phys.latest.pos, controls->phys.sent.pos);
        copyVec3(controls->phys.latest.vel, controls->phys.sent.vel);
    }
    else
    {
        pktSendBits(pak, 1, 0);

        if(    controls->phys.latest.net_id >= 0 &&
            (!e->pl || !e->pl->door_anim))
        {
            pktSendBits(pak, 1, 1);
            pktSendBits(pak, 16, controls->phys.latest.net_id);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
    }

    controls->phys.latest.net_id = -1;

    // Send motion recording.

    if(controls->record_motion)
    {
        pktSendBits(pak, 1, 1);
        pmotionSendPhysicsRecording(pak, e, "server");
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

int svrPlayerSendControlState(Packet* pak, Entity* e, NetLink* link, int full_state)
{
    int                    i;
    int                    reliable = 0;
    ClientLink*            client = link->userData;
    ServerControlState* scs;

    // Send future pushes.

    svrPlayerSendFuturePushList(pak, e);

    // Send physics positions.

    svrPlayerSendPhysicsPositions(pak, e, client);

    // If a full update, force-send the control state update.

    if(full_state)
    {
        ServerControlState* scs = getQueuedServerControlState(&client->controls);

        // Restore states that may have been lost.

        scs->stun = e->pchar->attrCur.fStunned > 0.0;
    }

    // Send the control update.

    scs = client->controls.queued_server_states.tail;

    if(scs && !client->controls.cur_server_state_sent)
    {
        pktSendBits(pak,1,1);

        pktSendBits(pak, 8, client->controls.sent_update_id);

        client->controls.cur_server_state_sent = 1;
        scs->abs_time_sent = ABS_TIME;

        reliable = 1;

        pktSendF32(pak, scs->speed[0]);
        pktSendF32(pak, scs->speed[1]);
        pktSendF32(pak, scs->speed[2]);
        pktSendF32(pak, scs->speed_back);
        pktSendBitsArray(pak, sizeof(scs->surf_mods)*8, &scs->surf_mods);

        pktSendF32(pak, scs->jump_height);

        pktSendBits(pak, 1, scs->fly);
        pktSendBits(pak, 1, scs->stun);
        pktSendBits(pak, 1, scs->jumppack);
        pktSendBits(pak, 1, scs->glide);
        pktSendBits(pak, 1, scs->ninja_run);
        pktSendBits(pak, 1, scs->walk);
        pktSendBits(pak, 1, scs->beast_run);
        pktSendBits(pak, 1, scs->steam_jump);
        pktSendBits(pak, 1, scs->hover_board);
        pktSendBits(pak, 1, scs->magic_carpet);
        pktSendBits(pak, 1, scs->parkour_run);
        pktSendBits(pak, 1, scs->no_physics);
        pktSendBits(pak, 1, scs->hit_stumble);

        pktSendBits(pak, 1, scs->controls_input_ignored);
        pktSendBits(pak, 1, scs->shell_disabled);
        pktSendBits(pak, 1, scs->no_ent_collision);

        pktSendBits(pak, 1, scs->no_jump_repeat);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    if(    client->controls.server_pos_id != client->controls.client_pos_id &&
        client->controls.server_pos_id_sent + 10 <= timerCpuSeconds() )
    {
        MotionState* motion = e->motion;

        //printf(    "sent:\t%1.2f\t%1.2f\t%1.2f\n",
        //        e->client->controls.pyr.cur[0],
        //        e->client->controls.pyr.cur[1],
        //        e->client->controls.pyr.cur[2]);

        if(client->controls.server_pos_id_sent)
            client->controls.server_pos_id++; // the client hasn't confirmed that it got a position update in 10 seconds, perhaps we missed it. tell the client that this is a new position.
        client->controls.server_pos_id_sent = timerCpuSeconds();

        pktSendBits(pak, 1, 1);

        pktSendBitsPack(pak, 1, client->controls.server_pos_id);

        for(i=0;i<3;i++)
            pktSendF32(pak, ENTPOS(e)[i]);

        for(i=0;i<3;i++)
            pktSendIfSetF32(pak, motion->vel[i]);

        for(i=0;i<3;i++)
            pktSendIfSetF32(pak, client->controls.pyr.cur[i]);

        pktSendBitsPack(pak, 1, motion->falling);

        link->lost_packet_recv = 0;
    }
    else
    {
        pktSendBits(pak,1,0);
    }

    return reliable;
}


