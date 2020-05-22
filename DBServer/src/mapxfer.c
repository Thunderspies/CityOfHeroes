#include "mapxfer.h"
#include "comm_backend.h"
#include "launchercomm.h"
#include "dbinit.h"
#include <utilitieslib/utils/error.h>
#include "dbserver/servercfg.h"
#include "dblog.h"
#include <stdio.h>
#include "waitingEntities.h"
#include "clientcomm.h"
#include "dbdispatch.h"
#include <utilitieslib/utils/mathutil.h>
#include "queueservercomm.h"
#include "dbcomm/StaticMapInfo.h"
#include "entity/gametypes.h"
#include <utilitieslib/utils/log.h>
#include "accountservercomm.h"
#include "account\AccountData.h"
#include "overloadProtection.h"

// id is db_id of entity
void sendMapLoadingMessage(NetLink *link, int is_gameclient, int id, int on)
{
    Packet *pak_out;

    if (is_gameclient)
        return;

    pak_out = pktCreateEx(link, DBSERVER_MAP_XFER_LOADING);
    pktSendBitsAuto(pak_out, id);
    pktSendBits(pak_out, 1, on);
    pktSend(&pak_out, link);
}

// id is db_id of entity that failed to transfer
void sendLoginFailure(NetLink *link,int is_gameclient,int id,char *errmsg)
{
    Packet    *pak_out;
    int        pkt_types[] = {DBSERVER_MAP_XFER_FAIL,DBGAMESERVER_MSG};

    pak_out = pktCreateEx(link,pkt_types[is_gameclient]);
    pktSendBitsPack(pak_out, 1, id);
    pktSendString(pak_out,errmsg);
    pktSend(&pak_out,link);
}

U32 getMatchingIpType(U32 ip_list[2],U32 client_ip)
{
    int        ret = dbIsLocalIp(client_ip);

    if (dbIsLocalIp(ip_list[1]) == ret)
        return ip_list[1];
    else
        return ip_list[0];
}

static char *linkIpStr(NetLink *link)
{
    return makeIpStr(link->addr.sin_addr.S_un.S_addr);
}

void sendLoginInfoToGameClient(EntCon *ent_con,int login_cookie)
{
    Packet    *pak_out = 0;
    MapCon    *map_con = 0;
    GameClientLink *game =0;
    int        pkt_types[] = {DBSERVER_MAP_XFER_OK,DBGAMESERVER_MAP_CONNECT};
    U32        ip;

    if (!ent_con || !ent_con->callback_link)
    {
        if (ent_con)
        {
            LOG_OLD_ERR( "missing callback link for entity (%d) (player disconnected while logging in or relay command?)\n", ent_con->id);
        } 
        else 
        {
            LOG_OLD_ERR( "clientcomm.log","missing callback link for entity (player disconnected while logging in or relay command?)\n");
        }
        return;
    }
    // Check to make sure this went through setNewMap
    // It's possible that the user connected, his entity was sent to a MapServer, and he disconnected and re-connected before
    // the cookie made it back to us.  In this case, he's sitting up on the stack in setNewMap right now, waiting for
    // for some map to start, so we don't want to send him *this* ack, we will send him the next one
    // we receive after his entity is re-sent to the mapserver...
    if (!ent_con->handed_to_mapserver && ent_con->is_gameclient) {
        LOG_OLD_ERR("clientcomm.log","player apparently disconencted and reconnected while his entity was en route to a mapserver (db_id %d)\n", ent_con->id);
        return;
    }

    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),ent_con->map_id);
    if(ent_con->is_gameclient)
    {
        game = dbClient_getGameClientByName(ent_con->account, ent_con->callback_link);
        if(game)
        {
            pak_out = dbClient_createPacket(pkt_types[ent_con->is_gameclient],game);
        }
        
        if(!pak_out)
            return;
    }
    else
        pak_out = pktCreateEx(ent_con->callback_link,pkt_types[ent_con->is_gameclient]);
    pktSendBitsPack(pak_out,1,ent_con->id);
    pktSendBitsPack(pak_out,1,map_con->id);
    if (ent_con->is_gameclient)
    {
        if (server_cfg.advertisedIp != 0)
        {
            // ignore what mapserver says, use value configured in servers.cfg
            ip = server_cfg.advertisedIp;
        }
        else
        {
            if (server_cfg.queue_server && game)
            {
                ip = getMatchingIpType(map_con->ip_list, game->nonQueueIP);
            }
            else
            {
                ip = getMatchingIpType(map_con->ip_list, ent_con->callback_link->addr.sin_addr.S_un.S_addr);
            }
        }
        
        pktSendBitsPack(pak_out,1,ip);
        pktSendBitsPack(pak_out,1,ip);
    }
    else
    {
        if (server_cfg.advertisedIp != 0)
        {
            // ignore what mapserver says, use value configured in servers.cfg
            ip = server_cfg.advertisedIp;
        }
        else
        {
            ip = getMatchingIpType(map_con->ip_list, ent_con->client_ip);
        }
        pktSendBitsPack(pak_out,1,ip);
        pktSendBitsPack(pak_out,1,ip);
    }
    pktSendBitsPack(pak_out,1,map_con->udp_port);
    pktSendBitsPack(pak_out,1,map_con->tcp_port);
    pktSendBitsPack(pak_out,1,login_cookie);

    if(!mpReclaimed(ent_con->callback_link)) // hack, need to fix properly
        pktSend(&pak_out,ent_con->callback_link);

    ent_con->handed_to_mapserver = 0;
    ent_con->logging_in = 0;
    ent_con->is_gameclient = 0;
    ent_con->callback_link = 0;
    ent_con->active = 1;
}

MapCon *startInstancedMap(NetLink* link, EntCon* ent_con, char* mapinfo, char* errmsg, const char* source)
{
    U32                force_launcher_ip = 0;
    DBClientLink    *client = link->userData;
    DbList            *map_list = dbListPtr(CONTAINER_MAPS);
    MapCon            *map_con;
    int                i, sgid, sgid2, userid;
    int                raidid1, raidid2, raidtype;
    char            buf[5000];
    char            mapname[MAX_PATH];

    // find the existing instanced map if possible
    for (i = 0; i < map_list->num_alloced; i++)
    {
        map_con = (MapCon*)map_list->containers[i];
        if (map_con && stricmp(map_con->mission_info, mapinfo) == 0)
            return map_con;
    }

    // otherwise, create the container
    // immediately tell the player that we're going to start up a map
    if (link)
        sendMapLoadingMessage(link, ent_con->is_gameclient, ent_con->id, 1);

    // Generate a name for bases
    if (2 == sscanf(mapinfo, "B:%i:%i", &sgid, &userid) && (sgid || userid)) 
    {
        if(sgid && userid)
        {
            sprintf(errmsg, "launchfail: dually owned base (sgid %d userid %d)", sgid, userid);
            return 0;
        }
        sprintf(mapname, "Base(supergroupid=%d,userid=%d)", sgid, userid);
     }
    else if (5 == sscanf(mapinfo, "R:%i:%i:%i:%i:%i", &sgid, &sgid2, &raidid1, &raidid2, &raidtype) && sgid && sgid2 && raidid1 && raidid2)
    {
        sprintf(mapname, "Raid(%d vs %d)", MIN(sgid, sgid2), MAX(sgid, sgid2));
    }
    else
    {
        strcpy(mapname, "");
    }
    sprintf(buf,"MapName \"%s\"\nMissionInfo \"%s\"\n",mapname,mapinfo);
    map_con = handleContainerUpdate(CONTAINER_MAPS, -1, 1, buf);
    if (!map_con)
    {
        sprintf(errmsg, "launchfail: creating map container %s", mapinfo);
        return 0;
    }

    // and start the map
    if (client->static_link)
        force_launcher_ip = link->addr.sin_addr.S_un.S_addr;
    if (!launcherCommStartProcess(myHostname(), force_launcher_ip, map_con, false))
    {
        if (overloadProtection_DoNotStartMaps())
        {
            // Don't delete the map_con on overload protection
            sprintf(errmsg,"MapFailedOverloadProtection");
        }
        else
        {
            printf("launchfail: deleting mapcontainer %s\n",mapinfo);
            containerDelete(map_con);
            sprintf(errmsg,"NoLaunchersRunning");
        }
        return 0;
    }
    LOG_OLD( "dbserver starting instanced map %s (id: %d)", mapinfo, map_con->id);
    map_con->callback_link = link;
    map_con->callback_idx = 0; // ?
    map_con->starting = 1;
    getMapName(map_con->raw_data,map_con->map_name);
    return map_con;
}

MapCon *startMapIfStatic(NetLink *link, EntCon* ent_con, int map_id, char *errmsg, const char* source)
{
    MapCon *map_con;

    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),map_id);
    if (!map_con)
    {
        if(errmsg)
        {
            sprintf(errmsg,"CantFindMap (%d)",map_id);
        }
        return 0;
    }
    if (!map_con->active && !map_con->starting)
    {
        extern char    my_hostname[];
        char log_buffer[10000] = "";
        char* bufpos = log_buffer;
        int i;

        if (!map_con->is_static)
        {
            if(errmsg)
            {
                sprintf(errmsg,"MapNotRunning (%d)",map_id);
            }
            return 0;
        }

        if(map_con->base_map_id)
        {
            for(i = 0; i < map_list->num_alloced; i++)
            {
                MapCon* same_map = (MapCon*)map_list->containers[i];
                
                if(same_map->base_map_id == map_con->base_map_id)
                {
                    bufpos += sprintf(    bufpos,
                                        "%s%d:%d%s",
                                        bufpos == log_buffer ? "" : ", ",
                                        same_map->id,
                                        totalPlayersOnMap(same_map),
                                        mapIsRunning(same_map) ? "" : "(off)");
                }
            }
        }
        else
        {
            strcpy(log_buffer, "no clones");
        }
        
        LOG_OLD( "Sending char %d:\"%s\" to map %d (%s), Source: %s\n", ent_con ? ent_con->id : -1,
                    ent_con ? ent_con->ent_name : "<none>", map_con->id, log_buffer, source ? source : "");
        
        if (overloadProtection_DoNotStartMaps())
        {
            // Do not delete the map container
            if(errmsg)
            {
                sprintf(errmsg,"MapFailedOverloadProtection");
            }
            return 0;
        }

        // Map isn't started yet, get it going!
        // immediately tell the player that we're going to start up a map
        if (link)
            sendMapLoadingMessage(link, ent_con->is_gameclient, ent_con->id, 1);

        if (!launcherCommStartProcess(my_hostname, 0, map_con, false))
        {
            // trying to start a map could initiate the overload protection
            // So, check here again
            if (overloadProtection_DoNotStartMaps())
            {
                // Do not delete the map container
                if(errmsg)
                {
                    sprintf(errmsg,"MapFailedOverloadProtection");
                }
            }
            else
            {
                containerDelete(map_con);
                if(errmsg)
                {
                    sprintf(errmsg,"NoLaunchersRunning");
                }
            }
            return 0;
        }
        map_con->starting = 1;

        /*
        while(map_con->starting && containerPtr(dbListPtr(CONTAINER_ENTS),ent_id)==ent_con)
            msgScan();
        map_con->num_players_waiting_for_start--;
        if (ent_con!=containerPtr(dbListPtr(CONTAINER_ENTS),ent_id)) // client could disconnect while waiting for static map to start
        {
            if(errmsg)
            {
                strcpy(errmsg,"ClientDisconnect");
            }
            return 0;
        }
        */
    }
    return map_con;
}

int checkMapIDForOverrides(int map_id, EntCon *ent_con)
{
    int retval;
    StaticMapInfo *map_info;
    
    retval = map_id;
    map_info = staticMapInfoFind(map_id);

    if (ent_con && !(ent_con->access_level) && map_info)
    {
        // If another map substitutes for non-Praetorians, point all
        // non-Praetorians at that map instead of the original one.
        if (map_info->overriddenBy[kOverridesMapFor_PrimalEarth] &&
            (ent_con->praetorianProgress == kPraetorianProgress_PrimalBorn ||
            ent_con->praetorianProgress == kPraetorianProgress_PrimalEarth ||
            ent_con->praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial))
        {
            retval = map_info->overriddenBy[kOverridesMapFor_PrimalEarth];
        }
    }

    return retval;
}

bool canStartStaticMap(int map_id, EntCon *ent_con)
{
    MapCon *map_con;

    map_id = checkMapIDForOverrides(map_id, ent_con);

    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),map_id);
    if (!map_con)
    {
        return false;
    }

    if (!mapIsRunning(map_con))
    {
        if (overloadProtection_DoNotStartMaps())
            return false;
    }

    return true;
}

int setNewMap(EntCon *ent_con,int map_id,NetLink *link,int is_gameclient,char *errmsg,int write_sql)
{
    MapCon *map_con;
    int cmd_idx;
    char cmd_buf[128];
    int dest_map_id;

    dest_map_id = checkMapIDForOverrides(map_id, ent_con);

    dbMemLog(__FUNCTION__, "Ent %i moving to map %i, was map %i", ent_con->id, dest_map_id, ent_con->map_id);

    ent_con->callback_link = link;
    ent_con->is_gameclient = is_gameclient;

    errmsg[0]=0;
    map_con = startMapIfStatic(link, ent_con, dest_map_id, errmsg, "setNewMap");
    if (!map_con) {
        assert(errmsg[0]!=0);
        dbMemLog(__FUNCTION__, "failed: %s", errmsg);
        return 0;
    }

    if (!map_con->active && !is_gameclient) {
        // MapServer requested a transfer to a non-running map, reject it!  Otherwise the entity is in a no-owner limbo state
        sprintf(errmsg,"MapNotRunning (%d)",dest_map_id);
        dbMemLog(__FUNCTION__, "failed: %s", errmsg);
        return 0;
    }

    cmd_idx = sprintf(cmd_buf,"MapId %d\n",dest_map_id);
    if (map_con->is_static)
    {
        sprintf(&cmd_buf[cmd_idx],"StaticMapId %d\n",dest_map_id);
        ent_con->static_map_id = dest_map_id;
    }
    containerUpdate(dbListPtr(CONTAINER_ENTS),ent_con->id,cmd_buf,write_sql);

    if (is_gameclient) {
        // Whether it's running or not, put it in the queue, it might flush immediately
        waitingEntitiesAddEnt(ent_con, dest_map_id, !is_gameclient, map_con);
    } else {
        // This is a mapxfer
        assert(map_con->active);
        // MapServer is running, pass them through! (it should complete immediately)
        finishSetNewMap(ent_con, ent_con->id, dest_map_id, !is_gameclient);
    }
    return 1;
}

// Code to be ran on an entity after its map finished loading
void finishSetNewMap(EntCon *ent_con, U32 container_id, U32 new_map_id, int is_map_xfer)
{
    MapCon *map_con;
    int ret;

    assert(ent_con == containerPtr(dbListPtr(CONTAINER_ENTS),container_id));
    if (!ent_con) // client disconnected while waiting for static map to start
    {
        // This queue entry should have been cleared out when the client disconnects
        assert(0);
        //strcpy(errmsg,"ClientDisconnect");
        return;
    }
    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),new_map_id);
    if (!map_con->active)
    {
        // Checked in the queue code now
        assertmsgf(0, "Static map %d failed to start!"  );
        return;
    }
    ent_con->map_id = map_con->id;
    ent_con->is_map_xfer = is_map_xfer;
    if (server_cfg.log_relay_verbose) 
        LOG_OLD_ERR("finishSetNewMap Ent %d, demand_loaded: %d, is_map_xfer: %d", ent_con->id, ent_con->demand_loaded, ent_con->is_map_xfer);
    dbMemLog(__FUNCTION__, "had lock %i, setting to %i", ent_con->lock_id, dbLockIdFromLink(map_con->link));
    LOG_DEBUG( "mapxfer old lock: %d, new lock: %d",ent_con->lock_id,dbLockIdFromLink(map_con->link));

    assert(ent_con->lock_id == 0);
    ret = containerLock(ent_con,dbLockIdFromLink(map_con->link));
    assert(ret); // Shouldn't get here if the container is locked
    if(map_con->active && !map_con->starting)
        map_con->num_players_sent_since_who_update++;
    containerSendList(dbListPtr(CONTAINER_ENTS),&ent_con->id,1,0,map_con->link,0);
    containerServerNotify(CONTAINER_ENTS, ent_con->id, ent_con->id, 1);
    assert(!ent_con->handed_to_mapserver);
    ent_con->handed_to_mapserver = 1;

    // HYBRID
    // need to update mapserver with loyaltyStatus
    {
        NetLink *mapLink = map_con->link;
        U8 *loyalty = accountInventoryGetLoyaltyStatus(ent_con->auth_id);

        if (mapLink && loyalty)
        {
            Packet *pak_out = pktCreateEx(mapLink, DBSERVER_ACCOUNTSERVER_LOYALTY);

            pktSendBitsAuto(pak_out, ent_con->id);
            pktSendBitsArray(pak_out, LOYALTY_BITS, loyalty);
            pktSend(&pak_out, mapLink);
        }
    }

    //printf("id: %d  map: %d\n\n",ent_con->id,ent_con->map_id);
    return;

}

void dbForceLogout(int container_id,int reason)
{
    EntCon    *container;
    Packet    *pak;
    NetLink    *map_link;

    container = containerPtr(dbListPtr(CONTAINER_ENTS),container_id);
    if (!container)
        return;
    map_link = dbLinkFromLockId(container->lock_id);
//    Don't do this, it tells a mapserver who doesn't have this entity locked to unload him, which will just be ignored!
//    if (!map_link) { // Fall back case?  I don't think this should happen, though
//        MapCon    *map_con;
//        map_con = containerPtr(dbListPtr(CONTAINER_MAPS),container->map_id);
//        if (map_con)
//            map_link = map_con->link;
//    }
    if (map_link)
    {
        pak = pktCreateEx(map_link,DBSERVER_FORCE_LOGOUT);
        pktSendBitsPack(pak,1,container_id);
        pktSendBitsPack(pak,1,reason);
        pktSend(&pak,map_link);
    }
    else
    {
        LOG_OLD( "ent sendPlayerLogout\"%s\"",container->ent_name);
        sendPlayerLogout(container_id,6,false); // calls playerUnload
    }
}

