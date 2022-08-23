#include <utilitieslib/assert/assert.h>
#include "dbcomm.h"
#include "dbmapxfer.h"
#include "entity/entity.h"
#include <utilitieslib/network/netio.h>
#include "svr/svr_base.h"
#include <utilitieslib/network/net_packetutil.h>

#include "entity/entPlayer.h"
#include "container/containerloadsave.h" // packageEntAll()
#include "auction/AuctionClient.h" // ent_XactReqShardJump()
#include "dbquery.h" // dbQueryGetFilter()
#include <utilitieslib/components/EString.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/crypt.h>

#include "dbcontainer.h"
#include "comm_game.h"
#include <utilitieslib/utils/utils.h>
#include "group/grouputil.h"
#include <utilitieslib/utils/error.h>
#include "entity/entserver.h"
#include "svr/svr_player.h"
#include "entity/entVarUpdate.h"
#include "gameSys/dooranim.h"
#include "gameComm/sendToClient.h"
#include "language/langServerUtil.h"
#include "storyarc/storyarcinterface.h"
#include "container/team.h"
#include "gameSys/arenamap.h"
#include "script/scriptengine.h"
#include "entity/pvp.h"
#include "turnstile/TurnstileServerCommon.h"
#include <utilitieslib/utils/file.h>
#include "logcomm.h"
#include "LWC_common.h"

static bool s_UseTwoStageMapXfer = true;

static void sendClientNewMapLoading(Entity *e, int on)
{
    ClientLink *clientLink = clientFromEnt(e);
    Packet *pak = pktCreate();
    pktSendBitsPack(pak, 1, SERVER_MAP_XFER_LOADING);
    pktSendBits(pak, 1, on);
    pktSend(&pak, clientLink->link);
}

// get the entity back to life (failed map xfer)
int dbUndoMapXfer(Entity *e, char *errmsg)
{
    DbCommState    *dbstate;
    dbstate = &e->dbcomm_state;
    teamlogPrintf(__FUNCTION__, "%s:%i", e->name, e->db_id);

    // Verify that we still own the entity, if not things are bad!
    if (!e->owned) {
        assert(!"Undoing a map transfer on an entity we no longer own!  Bad things will happen.");
        //Kill the client?  Gracefully recover *shudder*?
        return 0;
    }

    e->adminFrozen = 0;

    dbstate->map_xfer_step = 0;
    dbstate->in_map_xfer = 0;
    dbstate->waiting_map_xfer = 0;
    entity_state[ e->owner ] = ENTITY_IN_USE;
    prepEntForEntryIntoMap(e, EE_USE_EMERGE_LOCATION_IF_NEARBY, 0 );

    // send client a message explain failure?
    sendClientNewMapLoading(e, 0);
    sendChatMsg( e, localizedPrintf(e,errmsg), INFO_SVR_COM, 0 );
    return 1;
}

int dbCheckMapXfer(Entity *e,NetLink *client_link)
{
    Packet        *pak;
    DbCommState    *dbstate;
    ClientLink    *client;

    dbstate = &e->dbcomm_state;
    if (!dbstate->in_map_xfer)
        return 0;

    if (s_UseTwoStageMapXfer && dbstate->waiting_map_xfer)
        return 0;

    switch(dbstate->map_xfer_step)
    {
        xcase MAPXFER_TELL_CLIENT_IDLE:
            if (!client_link)
                return 0; // Hasn't connected yet
            client = client_link->userData;
            if (client->ready < CLIENTSTATE_ENTERING_GAME)
                return 0; // Has connected but is still handshaking
            e->controlsFrozen = 1;
            e->untargetable |= UNTARGETABLE_TRANSFERING;
            e->invincible |= INVINCIBLE_TRANSFERING;
            pak = pktCreate();
            pktSendBitsPack(pak,1,SERVER_MAP_XFER_WAIT);
            if (dbstate->new_map_id == -1) {
                pktSendString(pak, dbGetMapName(e->static_map_id));
            } else {
                pktSendString(pak, dbGetMapName(dbstate->new_map_id));
            }
            pktSend(&pak,client_link);
            dbstate->map_xfer_step = MAPXFER_WAIT_MAP_STARTING;

        xcase MAPXFER_WAIT_MAP_STARTING:
            if ((dbstate->xfer_type & XFER_MISSION) && !e->teamup_id)
            {
                dbUndoMapXfer(e, "LostTeamup");
                return 1; // transfer cancelled
            }
            if (!dbstate->map_ready)
                return 0;
            dbstate->map_xfer_step = MAPXFER_WAIT_CLIENT_IDLE;
        // Intentional fall-through
        case MAPXFER_WAIT_CLIENT_IDLE:
            if ((dbstate->xfer_type & XFER_MISSION) && !e->teamup_id)
            {
                dbUndoMapXfer(e, "LostTeamup");
                return 1; // transfer cancelled
            }
            if (!dbstate->client_waiting)
                return 0;
            dbstate->client_waiting = 0;

            // changes made to entity past this point will not be permanent,
            // because it's been transferred to another mapserver.
            playerEntDelete(PLAYER_ENTS_OWNED,e,0);
            teamUpdatePlayerCount(e);
            svrSendEntListToDb(&e,1);
            teamlogPrintf(__FUNCTION__, "Sending entity %s:%i to db", e->name, e->db_id);

            // do the transfer
            entity_state[ e->owner ] = ENTITY_SLEEPING;
            pak = pktCreateEx(&db_comm_link,DBCLIENT_MAP_XFER);
            pktSendBitsPack(pak,1,dbstate->new_map_id);
            pktSendBitsPack(pak,1,1);
            pktSendBitsPack(pak,1,e->db_id);
            pktSend(&pak,&db_comm_link);
            dbstate->map_xfer_step = MAPXFER_WAIT_NEW_MAPSERVER_READY;
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Step 2: Ent %s moving to map %d",e->name,dbstate->new_map_id);
            teamlogPrintf(__FUNCTION__, "Sending DBCLIENT_MAP_XFER for %s:%i, to map %i", e->name, e->db_id, dbstate->new_map_id);

        xcase MAPXFER_WAIT_NEW_MAPSERVER_READY:
            //printf("waiting for dbserver to ack mapmove\n");
            ;
    }
    return 0;
}

void dbAsyncMapMoveEx(Entity *e, int map_id, char* mapinfo, MapTransferType xfer, U32 arccrc, char *arcstr, int arcstrlen)
{
    DbCommState    *dbstate;
    LWC_STAGE currentStage = LWC_GetCurrentStage(e);

    dbstate = &e->dbcomm_state;

    if( OnArenaMap() )
    {
        team_MinorityQuit(e,false,false);
        league_MinorityQuit(e, false, false);
    }

    if (dbstate->map_xfer_step != MAPXFER_NO_XFER)
    {
        LOG_OLD_ERR( "Error: Entity had a second dbAsyncMapMove queued up (already transfering to map %d%s, new request to %d%s)", dbstate->new_map_id, (dbstate->xfer_type & XFER_MISSION)?" (Mission)":"", map_id, (xfer & XFER_MISSION)?" (Mission)":"");
    }
    else if (!LWC_CheckMapIDReady(map_id, currentStage, 0))
    {
        // This is here to try and keep the game client from crashing or trying to run with a bunch of missing assets.
        // It may put the character in a bad state and therefore, should be caught at a higher level.
        LOG_OLD_ERR(  "Error: dbAsyncMapMoveEx(): Unable to move entity because LWC_CheckMapIDReady(%d, %d, 0) failed (not enough data)", map_id, currentStage);
    }
    else 
    {
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Step 1: Ent %s moving to map %d, \"%s\"",e->name,map_id,mapinfo);

        dbstate->client_waiting = 0;
        dbstate->in_map_xfer = 1;
        dbstate->new_map_id = map_id;
        dbstate->xfer_type = xfer;
        dbstate->map_xfer_step = MAPXFER_TELL_CLIENT_IDLE;
        dbstate->map_ready = 0;
        dbstate->waiting_map_xfer = 1;

        {
            // Moving to a static map or already started mission map, request that it be started/ask if it's ready
            Packet *pak = pktCreateEx(&db_comm_link,DBCLIENT_REQUEST_MAP_XFER);
            pktSendBitsPack(pak,1,dbstate->new_map_id);
            pktSendString(pak,mapinfo);
            pktSendBits(pak, 1, (dbstate->xfer_type & XFER_FINDBESTMAP) ? 1 : 0);
            pktSendBitsPack(pak,1,e->db_id);
            if(arcstr)
            {
                pktSendBits(pak, 1, 1);
                pktSendBitsAuto(pak, arccrc);
                pktSendZipped(pak, arcstrlen, arcstr);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
            pktSend(&pak,&db_comm_link);
            teamlogPrintf(__FUNCTION__, "Sending DBCLIENT_REQUEST_MAP_XFER for %s:%i, to map %i, \"%s\"", e->name, e->db_id, dbstate->new_map_id, mapinfo);
        }
    }
}

void dbAsyncMapMove(Entity *e, int map_id, char* mapinfo, MapTransferType xfer)
{
    dbAsyncMapMoveEx(e, map_id, mapinfo, xfer, 0, NULL, 0);
}

static int prefixof(char *string, char *prefix)
{
    return strnicmp(string, prefix, strlen(prefix)) == 0;
}

static bool prepareShardXfer(char **container_txt, Entity *e, int type, int homeShard, char *homeShardName)
{
    char *ent_str;
    char *ent_in;
    static int debug_xfer_out = 0;
    static int debug_xfer_back = 0;

    ent_str = ent_in = packageEntAll(e);

    if(!ent_str)
    {
        LOG_OLD_ERR(  "Error: Unable to package entity for a shardxfer request.");
        return false;
    }

    // Do the switch () outside the loops, that way we only switch once, rather that for every member of ent_in
    // The other two options are to switch inside the loop, which is a waste of time, or test type in the if / else tests.
    // I just don't like this last idea, because it risks turning the test block into an incomprehensible rats nest.
    switch (type)
    {
    xcase XFER_TYPE_VISITOR_OUTBOUND:
        for(ent_in = strtok(ent_in,"\r\n"); ent_in; ent_in = strtok(NULL,"\r\n"))
        {
            if(prefixof(ent_in, "MapId"))
                estrConcatStaticCharArray(container_txt,"MapId 0\n"); // Hack to set the mapid to 0 so that an older buggy DbServer can log him out successfully.
            else if(prefixof(ent_in, "FriendCount "))
                estrConcatStaticCharArray(container_txt,"FriendCount 0\n");
            else if(prefixof(ent_in, "Name "))
            {
                int i;
                char *src;
                char *dst;
                char name[256];

                dst = name;
                src = &ent_in[strlen("Name ")];
                i = 0;
                while ((*dst = *src) != 0)
                {
                    src++;
                    if (*dst != '"' && i < sizeof(name) - 4)
                    {
                        dst++;
                        i++;
                    }
                }

                estrConcatf(container_txt,"Name \"%s : %s\"\n", name, homeShardName);
            }
            else if(prefixof(ent_in, "ContainerID ") || prefixof(ent_in, "Ents2[0].RemoteDBID ") || prefixof(ent_in, "Ents2[0].RemoteShard "))
                ;    // Ditch the containerID, RemoteDBID and RemoteShard.  RemoteDBID and RemoteShard should never be set,
                    // but if a visiting character ever has a valid value in either of these, it will be a "Bad Thing". (tm)
                    // Thus we nuke them to be absolutely certain.
            else if (prefixof(ent_in, "Ents2[0].HomeShard "))
                ;    // Dump whevever's in HomeShard, it'll get filled in later
            else if (prefixof(ent_in, "SupergroupsId "))
                    // Transfer SupergroupID to HomeSGID cache, and then clear SupergroupID
                estrConcatf(container_txt,"SupergroupsId 0\nEnts2[0].HomeSGID %s\n", &ent_in[strlen("SupergroupsId ")]);
            else if (prefixof(ent_in, "Ents2[0].LevelingPactsId"))
                    // Repeat the above for the leveling pact ID
                estrConcatf(container_txt,"Ents2[0].LevelingPactsId 0\nEnts2[0].HomeLPID %s\n", &ent_in[strlen("Ents2[0].LevelingPactsId ")]);
            else if(dbQueryGetFilter(ent_in)) // don't bother keeping these lines around
                ;    // estrConcatf(container_txt,"// %s\n",ent_in);
            else
                estrConcatf(container_txt,"%s\n",ent_in);
        }
        estrConcatf(container_txt,"Ents2[0].HomeDBID %d\n", e->db_id);
        estrConcatf(container_txt,"Ents2[0].HomeShard %d\n", homeShard);
        if (debug_xfer_out)
        {
            FILE *fp;

            if ((fp = fopen("c:\\game\\scratch\\xfer_out.txt", "w")) != NULL)
            {
                fprintf(fp, "%s", *container_txt);
                fclose(fp);
            }
        }
    xcase XFER_TYPE_VISITOR_BACK:
        for(ent_in = strtok(ent_in,"\r\n"); ent_in; ent_in = strtok(NULL,"\r\n"))
        {
            // if you change this, you may need to change dbQueryGetCharacter() as well
            if(prefixof(ent_in, "MapId"))
                estrConcatStaticCharArray(container_txt,"MapId 0\n"); // Hack to set the mapid to 0 so that an older buggy DbServer can log him out successfully.
            else if(prefixof(ent_in, "FriendCount "))
                estrConcatStaticCharArray(container_txt,"FriendCount 0\n");
            else if(dbQueryGetFilter(ent_in)) // don't bother keeping these lines around
                ; // estrConcatf(container_txt,"// %s\n",ent_in);
            else
                estrConcatf(container_txt,"%s\n",ent_in);
        }
    xdefault:
        for(ent_in = strtok(ent_in,"\r\n"); ent_in; ent_in = strtok(NULL,"\r\n"))
        {
            // if you change this, you may need to change dbQueryGetCharacter() as well
            if(prefixof(ent_in, "MapId"))
                estrConcatStaticCharArray(container_txt,"MapId 0\n"); // Hack to set the mapid to 0 so that an older buggy DbServer can log him out successfully.
            else if(prefixof(ent_in, "FriendCount "))
                estrConcatStaticCharArray(container_txt,"FriendCount 0\n");
            else if(dbQueryGetFilter(ent_in)) // don't bother keeping these lines around
                ; // estrConcatf(container_txt,"// %s\n",ent_in);
            else
                estrConcatf(container_txt,"%s\n",ent_in);
        }
    }
    free(ent_str);

    return true;
}

void dbAsyncShardJump(Entity *e, const char *dest)
{
    DbCommState    *dbstate;
    dbstate = &e->dbcomm_state;

    if( OnArenaMap() )
    {
        team_MinorityQuit(e,false,false);
        league_MinorityQuit(e, false, false);
    }

    if(dbstate->map_xfer_step != MAPXFER_NO_XFER)
    {
        LOG_OLD_ERR( "Error: Entity already had a dbAsyncMapMove queued up (transfering to map %d%s, new request to shardjump to %s)", dbstate->new_map_id, (dbstate->xfer_type & XFER_MISSION)?" (Mission)":"", dest);
    }
    else if(!e->owned)
    {
        LOG_OLD_ERR("Error: Entity is not owned, but requesting a shardjump.")
    }
    else
    {
        char *ent_out = NULL;

        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Step 1: Ent %s moving to shard %s",e->name,dest);

        if(!prepareShardXfer(&ent_out, e, XFER_TYPE_PAID, 0, NULL))
            return;

        dbstate->client_waiting = 0;
        dbstate->in_map_xfer = 1;
        dbstate->new_map_id = 0;
        dbstate->xfer_type = XFER_SHARD;
        dbstate->map_xfer_step = MAPXFER_TELL_CLIENT_IDLE;
        dbstate->map_ready = 0;

        ent_XactReqShardJump(e,dest,e->static_map_id,ent_out);
        estrDestroy(&ent_out);
    }
}

void dbShardXferInit(Entity *e, OrderId order_id, int type, int homeShard, char *shardVisitorData)
{
    char lock_str[ORDER_ID_STRING_LENGTH];
    char *container_txt = NULL;
    char *homeShardName;

    if(!devassert(e && e->pl))
        return;

    strcpy(lock_str, orderIdAsString(order_id));

    if(e->dbcomm_state.map_xfer_step != MAPXFER_NO_XFER)
    {
        dbLog("ShardXfer", e, "Error: Entity has a dbAsyncMapMove queued up (transfering to map %d%s), accountserver should reattempt",
                e->dbcomm_state.new_map_id, (e->dbcomm_state.xfer_type & XFER_MISSION)?" (Mission)":"");
    }
    else if(!e->owned)
    {
        dbLog("ShardXfer", e, "Error: Entity is not owned, accountserver should reattempt");
    }
    else if(e->pl->accountServerLock[0] && strcmp(e->pl->accountServerLock,lock_str))
    {
        dbLog("ShardXfer", e, "Error: Entity is already locked by another AccountServer transaction (found %s, want %s)",e->pl->accountServerLock,lock_str);
    }
    else
    {
        strcpy(e->pl->accountServerLock,lock_str);
        if (type == XFER_TYPE_VISITOR_OUTBOUND || type == XFER_TYPE_VISITOR_BACK)
        {
            homeShardName = strchr(shardVisitorData, ';');
            if (homeShardName != NULL)
            {
                *homeShardName++ = 0;
            }
            else
            {
                homeShardName = "Unknown";
            }
            devassert(strlen(shardVisitorData) < sizeof(e->pl->shardVisitorData));
            strncpyt(e->pl->shardVisitorData, shardVisitorData, sizeof(e->pl->shardVisitorData));
        }
        else
        {
            homeShardName = NULL;
        }

        if (prepareShardXfer(&container_txt, e, type, homeShard, homeShardName))
        {
            Packet *pak_out = pktCreateEx(&db_comm_link,DBCLIENT_ACCOUNTSERVER_SHARDXFER);
            pktSendBitsAuto2(pak_out, order_id.u64[0]);
            pktSendBitsAuto2(pak_out, order_id.u64[1]);
            pktSendBool(pak_out,ENT_IS_VILLAIN(e));
            pktSendString(pak_out,container_txt);
            pktSend(&pak_out,&db_comm_link);

            estrDestroy(&container_txt);

            dbLog("ShardXfer", e, "Entity locked and packaged for transfer");
        }
        else
        {
            e->pl->accountServerLock[0] = '\0';
            e->pl->shardVisitorData[0] = '\0';
            dbLog("ShardXfer", e, "Error: Problem packing entity for transfer");
        }
    }
}

void dbShardXferComplete(Entity *e, int dbid, int cookie, int addr, int port)
{
    if (e && e->pl && e->pl->shardVisitorData[0])
    {
        START_PACKET(pak, e, SERVER_VISITOR_SHARD_XFER_DATA)
        pktSendString(pak, e->pl->shardVisitorData);
        pktSendBitsAuto(pak, dbid);
        pktSendBitsAuto(pak, e->auth_id);
        pktSendBitsAuto(pak, cookie);
        pktSendBitsAuto(pak, addr);
        pktSendBitsAuto(pak, port);
        END_PACKET
    }
}

void dbShardXferJump(Entity *e)
{
    if (e && e->pl && e->pl->shardVisitorData[0])
    {
        START_PACKET(pak, e, SERVER_VISITOR_SHARD_XFER_JUMP)
        END_PACKET
    }
}

void handleDbNewMapLoading(Packet *pak)
{
    int        link_id,on;
    Entity    *e;

    link_id    = pktGetBitsAuto(pak);
    on = pktGetBits(pak, 1);

    e = entFromDbIdEvenSleeping(link_id);
    if (!e)
        return;

    sendClientNewMapLoading(e, on);
}

void handleDbNewMapReady(Packet *pak) // A static map is ready for an entity who's waiting for it
{
    int        link_id,map_id;
    NetLink    *link;
    Entity    *e;

    link_id        = pktGetBitsPack(pak,1);
    map_id        = pktGetBitsPack(pak,1);

    link = findLinkById(link_id);
    if (!link)
        return;

    e = entFromDbIdEvenSleeping(link_id);
    e->dbcomm_state.map_ready = 1;
    e->dbcomm_state.new_map_id = map_id;
    e->dbcomm_state.waiting_map_xfer = 0;
}


void sendClientNewMap(NetLink *link, int map_id, int ip, int udp_port, int tcp_port, int login_cookie)
{
    Packet    *pak_out;

    pak_out = pktCreate();
    pktSendBitsPack(pak_out,1,SERVER_MAP_XFER);
    pktSendBitsPack(pak_out,1,0);
    pktSendBitsPack(pak_out,1,map_id);
    pktSendBitsPack(pak_out,1,ip);
    pktSendBitsPack(pak_out,1,ip);
    pktSendBitsPack(pak_out,1,udp_port);
    pktSendBitsPack(pak_out,1,tcp_port);
    pktSendBitsPack(pak_out,1,login_cookie);
    pktSend(&pak_out,link);
}

void handleDbNewMap(Packet *pak)
{
    int        link_id,map_id,ip_list[2],udp_port,tcp_port,login_cookie;
    NetLink    *link;
    Entity    *e;

    link_id        = pktGetBitsPack(pak,1);
    map_id        = pktGetBitsPack(pak,1);
    ip_list[0]    = pktGetBitsPack(pak,1);
    ip_list[1]    = pktGetBitsPack(pak,1);
    udp_port    = pktGetBitsPack(pak,1);
    tcp_port    = pktGetBitsPack(pak,1);
    login_cookie = pktGetBitsPack(pak,1);

    link = findLinkById(link_id);
    if (!link)
        return;

    sendClientNewMap(link,map_id,ip_list[0],udp_port,tcp_port,login_cookie);

    e = entFromDbIdEvenSleeping(link_id);
    LOG_OLD( "Sending map %d, %s:%d, cookie: %x to client",map_id,makeIpStr(ip_list[0]),udp_port, login_cookie);
    if (e->dbcomm_state.map_xfer_step == MAPXFER_WAIT_NEW_MAPSERVER_READY)
        e->dbcomm_state.map_xfer_step = MAPXFER_DONE;
}

void handleDbNewMapFail(Packet *pak)
{
    int        id;
    Entity    *e;
    char    *errmsg;

    id = pktGetBitsPack(pak,1);
    errmsg = pktGetString(pak);

    e = entFromDbIdEvenSleeping(id);
    if (!e)
        return;
    // reclaim ownership of entity since dbserver refused to hand it off
    teamlogPrintf(__FUNCTION__, "reclaiming %s:%i", e->name, e->db_id);
    playerEntAdd(PLAYER_ENTS_OWNED,e);

    dbUndoMapXfer(e, errmsg);
    LOG_OLD_ERR("mapserver map xfer err: %s\n",errmsg);

    pvp_ApplyPvPPowers(e);
    ScriptPlayerEnteringMap(e); // Map thought player left, but it failed

    // Let the player know that the map failed to start
    START_PACKET(pak, e, SERVER_DOOR_CMD)
    pktSendBitsPack( pak, 1, DOOR_MSG_FAIL );
    pktSendString(pak, errmsg);
    END_PACKET
}

void dbUseTwoStageMapXfer(bool bTwoStage)
{
    s_UseTwoStageMapXfer = bTwoStage;
}
