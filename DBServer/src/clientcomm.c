#include <utilitieslib/network/netio.h>
#include "accountservercomm.h"
#include "account\accountdata.h"
#include <utilitieslib/utils/textparser.h>
#include "comm_backend.h"
#include <utilitieslib/utils/error.h>
#include "authcomm.h"
#include "container.h"
#include "dbserver/servercfg.h"
#include "mapxfer.h"
#include <utilitieslib/Version/AppVersion.h>
#include "dbdispatch.h"
#include <stdio.h>
#include <sqlext.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/timing.h>
#include "namecache.h"
#include "waitingEntities.h"
#include "clientcomm.h"
#include <utilitieslib/network/crypt.h>
#include "entity/costume.h"
#include <utilitieslib/network/netcomp.h>
#include "container_sql.h"
#include "sql_fifo.h"
#include "container_tplt.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include "statagg.h"
#include "container/container_util.h"
#include "auth/authUserData.h"
#include "offline.h"
#include <utilitieslib/components/earray.h>
#include "dbcomm/staticMapInfo.h"
#include <utilitieslib/stdtypes.h>
#include "entity/gametypes.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/estring.h>
#include "dbrelay.h"
#include <utilitieslib/components/HashFunctions.h>

#include "../Common/ClientLogin/clientcommlogin.h"
#include "queueservercomm.h"
#include <utilitieslib/utils/StringUtil.h>
#include "statservercomm.h"
#include <utilitieslib/utils/log.h>
#include "turnstileDb.h"
#include "overloadProtection.h"

NetLinkList    gameclient_links;

bool authnameAllowedLogin(char *authname, U32 ent_access_level);
bool authnameAutoLogin(char *authname, int removeSingle);
char *getAuthDeniedMsg();

static void sendMsg(NetLink *link,char *msg, GameClientLink *client)
{
    Packet    *pak        = dbClient_createPacket(DBGAMESERVER_MSG, client);
    if(pak)
    {
        pktSendString(pak,msg);
        pktSend(&pak,link);
    }
}


bool clientcommKickLogin(U32 auth_id,char *authname, int reason)
{
    ShardAccountCon *container = containerPtr(shardaccounts_list, auth_id);
    if (container)
        container->logged_in = 0;

    if(server_cfg.queue_server)
    {
        GameClientLink *game = 0;
        char buf[1000];
        getGameClientOnQueueServer(auth_id, &game);        
        if (!game)
            getGameClientOnQueueServerByName(authname, &game); // This is probably not necessary
        if (game)
        {
            sprintf(buf,"DBKicked %d",reason);
            sendMsg(game->link, buf, game);
            sendClientQuit(authname, game->linkId);
        }
        return !!game;
    }
    else
        return clientKickLogin(auth_id, reason, &gameclient_links);
}

int netGameClientLinkDelCallback(NetLink *link)
{
    assert(!server_cfg.queue_server);    //note that this is a no queue servre only function
    return clientcomm_quitClient(link, link->userData);
}


void clientCommInit()
{
    assert(!server_cfg.queue_server);
    clientCommLoginInit(&gameclient_links);
}
void clientCommInitStartListening()
{
    assert(!server_cfg.queue_server);
    clientCommLoginInitStartListening(&gameclient_links,processGameClientMsg, netGameClientLinkDelCallback);
}

static int getNumPlayerSlotsVIP()
{
    // We always make both heroes and villains available and so everyone should
    // have the 'both games' number of slots.
    return server_cfg.dual_player_slots;
}

const char *getAttribString(int idx)
{
    extern AttributeList *var_attrs;
    if (idx > 0 && idx < eaSize(&var_attrs->names)) {
        return var_attrs->names[idx];
    }
    return "";
}

GameClientLink *clientcomm_getGameClient(NetLink *lnk, Packet *pak)
{
    GameClientLink *game = 0;
    if(server_cfg.queue_server)
    {
        if(pak && pak->userData)
        {
            getGameClientOnQueueServerByName((char *) pak->userData, &game);
        }
    }
    else
    {
        devassert(lnk);
        game = lnk->userData;
    }
    return game;
}

static void sqlGetPlayersCallback(Packet *pak,U8 *cols,int col_count,ColumnInfo **field_ptrs, void * foo)
{
    const char        *name,*class,*origin;
    float            bodyscale, bonescale;
    int                i,idx,level,map_id,bodytype,playertype,playersubtype,playerTypeByLocation,praetorianprogress;
    char            *primarySet,*secondarySet;
    U32                dbFlags, colorskin, slotLock;
    GameClientLink    *client = clientcomm_getGameClient(pak->link, pak);
    U32                maxLastActive=0,secondsTillOffline;
    static U32        server_started=0;
    bool            kheldian_unlocked = false;
    bool            arachnos_unlocked = false;
    bool            tutorial_finished = false;
//    int                player_count = MAX(MIN(col_count, MAX_PLAYER_SLOTS), getNumPlayerSlots());
    int                player_count = MAX(col_count, MAX_PLAYER_SLOTS);
    ShardAccountCon*    shardaccount_con;
    if(!client)    //we've lost our connecting with the client since the time that the original request was made.
    {
        pktFree(pak);
        return;
    }
    pktSendBitsPack( pak, 1, player_count ); // maybe they have more than max
    shardaccount_con = (ShardAccountCon*)containerPtr(dbListPtr(CONTAINER_SHARDACCOUNT), client->auth_id);
    pktSendBitsPack( pak, 1, shardaccount_con ? shardaccount_con->slot_count : 0 ); // db_info.bonus_slots
    pktSendBitsPack( pak, 1, client->vip ); // db_info.vip
    pktSendBitsPack( pak, 1, shardaccount_con->showPremiumSlotLockNag ); // db_info.showPremiumSlotLockNag
    for(idx=0,i=0;i<col_count && i < MAX_PLAYER_SLOTS;i++)
    {
        int idxfld = 0;
        SQL_TIMESTAMP_STRUCT lastActiveStruct;
        U32 lastActiveSeconds;
        EntCatalogInfo *catalogEntry = NULL;

        playersubtype = kPlayerSubType_Normal;
        praetorianprogress = kPraetorianProgress_PrimalBorn;
        playerTypeByLocation = kPlayerType_Hero;
        primarySet = NULL;
        secondarySet = NULL;

        client->players_offline[i] = 0;
        client->players[i] = *(int *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        name = &cols[idx];
        idx += field_ptrs[idxfld++]->num_bytes;

        class = getAttribString(*(int *)(&cols[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        origin = getAttribString(*(int *)(&cols[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        dbFlags = *(U32 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        level = *(int *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        map_id = *(U16 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        bodytype = *(U8 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        playertype = *(U8 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        // Things which are stored in Ents2 can't be grabbed with this
        // SQL query so we go get them from the cache instead.
        catalogEntry = entCatalogGet(client->players[i], false);

        if(catalogEntry)
        {
            playersubtype = catalogEntry->playerSubType;
            praetorianprogress = catalogEntry->praetorianProgress;
            playerTypeByLocation = catalogEntry->playerTypeByLocation;
            primarySet = catalogEntry->primarySet;
            secondarySet = catalogEntry->secondarySet;
        }

        bodyscale = *(F32 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        bonescale = *(F32 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        colorskin = *(U32 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        lastActiveStruct = *(SQL_TIMESTAMP_STRUCT *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;
        lastActiveSeconds = timerGetSecondsSince2000FromSQLTimestamp(&lastActiveStruct);
        maxLastActive = MAX(lastActiveSeconds, maxLastActive);

        slotLock = *(U32 *)(&cols[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        idxfld++;

        pktSendBitsPack(pak,1,level);
        pktSendString(pak,unescapeString(name));
        pktSendString(pak,unescapeString(class));
        pktSendString(pak,unescapeString(origin));
        pktSendBitsPack(pak,1,dbFlags);
        pktSendBitsPack(pak,1,playertype);
        pktSendBitsPack(pak,1,playersubtype);
        pktSendBitsAuto(pak, playerTypeByLocation);
        pktSendBitsPack(pak,1,praetorianprogress);
        pktSendString(pak,primarySet?primarySet:"");
        pktSendString(pak,secondarySet?secondarySet:"");
        pktSendBitsPack(pak,1,bodytype);
        pktSendF32(pak,bodyscale);
        pktSendF32(pak,bonescale);
        pktSendBits(pak,32,colorskin);

        if (name[0])
        {
            int            id = pnameFindByName(name);
            EntCon    *e;

            e = (EntCon*)    containerPtr(dbListPtr(CONTAINER_ENTS),id);
            if (e && server_cfg.auth_server[0]) {
                if (e->access_level >= 1) {
                    // Don't kick them!
                } else {
                    dbForceLogout(e->id,-1);
                }
            }
        }
        {
            MapCon    *map;
            char    *name = "unknown";
            int        last_tick = -1;

            map = containerPtr(map_list,map_id);
            if (map)
            {
                if (map->map_running && map->link) { //JE: somehow map->map_running was true, yet map->link was NULL here
                    last_tick = timerCpuSeconds() - map->link->lastRecvTime;
                }
                name = map->map_name;
            }
            pktSendString(pak,getFileName(name));
            pktSendBitsPack(pak,1,last_tick);
        }

        // Characters with both flags set will unlock both epics, but that
        // doesn't normally happen and it's safer to do this than lock out
        // both epics in that circumstance.
        if (level >= 19) {
            if (dbFlags & DBFLAG_UNLOCK_HERO_EPICS)
                kheldian_unlocked = true;

            if (dbFlags & DBFLAG_UNLOCK_VILLAIN_EPICS)
                arachnos_unlocked = true;
        }

        // Any character which is not in the tutorial
        // finished it itself, or was created after another character finished
        // it.
        if (praetorianprogress != kPraetorianProgress_Tutorial
            && praetorianprogress != kPraetorianProgress_NeutralInPrimalTutorial)
            tutorial_finished = true;

        if (level < server_cfg.offline_protect_level)
            secondsTillOffline = server_cfg.offline_idle_days * 3600 * 24 - lastActiveSeconds;
        else
            secondsTillOffline = 0x7fffffff;
        pktSendBits(pak,32,secondsTillOffline);
        pktSendBits(pak,32,lastActiveSeconds);
        pktSendBits(pak,2,slotLock);
    }
    {
        OfflinePlayer    *offline,*p;

        p = offline = _alloca(player_count * sizeof(offline[0]));
        memset(offline,0,player_count * sizeof(offline[0]));
        offlinePlayerList(client->auth_id,client->account_name,offline,player_count-col_count,client->players,col_count);
        for(i=col_count;i<player_count;i++,p++)
        {
            int eLocked = SLOT_LOCK_NONE;
            extern AttributeList *var_attrs;

            client->players[i] = p->db_id;
            if (p->db_id)
            {
                client->players_offline[i] = 1;
                eLocked = SLOT_LOCK_OFFLINE;
            }
            pktSendBitsPack(pak,1,p->level);
            if (p->db_id)
            {
                pktSendString(pak,p->name);
                pktSendString(pak,var_attrs->names[p->archetype]); // class
                pktSendString(pak,var_attrs->names[p->origin]); // origin
            }
            else
            {
                pktSendString(pak,"EMPTY");
                pktSendString(pak,""); // class
                pktSendString(pak,""); // origin
            }

            pktSendBitsPack(pak,1,0); // dbFlags
            pktSendBitsPack(pak,1,0); // playertype
            pktSendBitsPack(pak,1,0); // playersubtype
            pktSendBitsAuto(pak, 0); // playerTypeByLocation
            pktSendBitsPack(pak,1,0); // praetorianprogress
            pktSendString(pak,""); // primaryPower
            pktSendString(pak,""); // secondaryPower
            pktSendBitsPack(pak,1,0); // bodytype
            pktSendF32(pak,1.0f); // bodyscale
            pktSendF32(pak,1.0f); // bonescale
            pktSendBits(pak,32,0); // colorskin

            pktSendString(pak,p->db_id ? "OFFLINE" : "");
            pktSendBitsPack(pak,1,-1);
            pktSendBits(pak,32,0);
            pktSendBits(pak,32,0);
            pktSendBits(pak,2,eLocked);
        }
    }
    // No auth server specified, fake the authbits so new characters can
    // be created.
    if (server_cfg.fake_auth)
    {
        authUserSetHeroAccess((U32*)client->auth_user_data,1);
        authUserSetVillainAccess((U32*)client->auth_user_data,1);

        // Pre-purchase means the authbit can be set while the shard overall
        // doesn't activate the features.
        authUserSetRogueAccess((U32*)client->auth_user_data,server_cfg.ownsGR);

        if (kheldian_unlocked)
        {
            authUserSetKheldian((U32*)(client->auth_user_data),1);
        }

        if (arachnos_unlocked)
        {
            authUserSetArachnosAccess((U32*)(client->auth_user_data),1);
        }

        authUserSetIsRetailAccount((U32*)(client->auth_user_data),1);
    }

    pktSendBitsArray(pak,sizeof(client->auth_user_data)*8,client->auth_user_data);
    pktSendBitsArray(pak,sizeof(g_overridden_auth_bits)*8,g_overridden_auth_bits);

    pktSend(&pak,pak->link);

    if (!server_started) {
        server_started = timerSecondsSince2000();
    }
    // Only log system specs if they have not logged in within this instance of the Dbserver
    if (client->systemSpecs) 
    {
        if (maxLastActive < server_started) 
        {
            LOG( LOG_SYSTEM_SPECS, 0, 0, "IP,%s,AuthName,%s,SystemSpecs,%s", makeIpStr(client->link->addr.sin_addr.S_un.S_addr), client->account_name, client->systemSpecs);
        }
        free(client->systemSpecs);
        client->systemSpecs = 0;
    }
}

// Both player sending functions use the same columns in their SQL query
// so it's best to have it in one place.
#define SEND_PLAYERS_COLUMNS "ContainerId, Name, Class, Origin, DbFlags, Level, StaticMapId, BodyType, PlayerType, BodyScale, BoneScale, ColorSkin, LastActive, IsSlotLocked"

MP_DEFINE(VipSqlCbData);

static void s_VipChangedSqlCb(Packet *pak, U8 *cols, int col_count, ColumnInfo **field_ptrs, VipSqlCbData *vip_data)
{
    char buf[200];
    int i;
    ShardAccountCon *shardaccount_con = vip_data->shardaccount_con;
    int vip = AccountIsVIP(NULL, shardaccount_con->accountStatusFlags);

    // vip -> free transition for non-VIP-only shards
    if (!server_cfg.isVIPServer && !vip)
    {
        for (i = 0; i < col_count; ++i)
        {
            int dbid = *(int*)&cols[i * field_ptrs[0]->num_bytes];
            sqlFifoTickWhileReadPending(CONTAINER_ENTS, dbid);
            dbForceLogout(dbid, -2);
            sprintf(buf, "UPDATE dbo.Ents SET IsSlotLocked=1 WHERE ContainerId=%d;", dbid);
            sqlExecAsyncEx(buf, (int)strlen(buf), dbid, false);
        }

        shardaccount_con->showPremiumSlotLockNag = 1;
    }

    sprintf(buf, "WasVIP %d\nAccountStatusFlags %d\nShowPremiumSlotLockNag %d\n", vip, shardaccount_con->accountStatusFlags, shardaccount_con->showPremiumSlotLockNag);
    containerUpdate(shardaccounts_list, shardaccount_con->id, buf, 1);

    vip_data->updateCallback(vip_data);
    MP_FREE(VipSqlCbData, vip_data);
}

void updateVip(void (*onVipUpdateCallback)(VipSqlCbData*), GameClientLink *client, ShardAccountCon *shardaccount_con, Packet *pak_out)
{
    int vip = AccountIsVIP(NULL, shardaccount_con->accountStatusFlags) != 0;
    VipSqlCbData *vip_data;

    MP_CREATE(VipSqlCbData,64);

    TODO(); // this will leak if the client link is dropped while the sql command is in flight?
    vip_data = MP_ALLOC(VipSqlCbData);
    assert(vip_data);
    vip_data->updateCallback = onVipUpdateCallback;
    vip_data->client = client;
    vip_data->shardaccount_con = shardaccount_con;
    vip_data->pak_out = pak_out;

    if (shardaccount_con->was_vip != vip)
    {
        char *restrict = NULL;
        estrPrintf(&restrict,"WHERE AuthId=%d AND IsSlotLocked IS NULL",shardaccount_con->id);
        sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restrict,s_VipChangedSqlCb,vip_data,0,0);
        estrDestroy(&restrict);
    }
    else
    {
        // don't need to do any SQL hackery, call callback directly
        vip_data->updateCallback(vip_data);
        MP_FREE(VipSqlCbData, vip_data);
    }    
}

void getEnts_sendPlayers(GameClientLink *client, ShardAccountCon* shardaccount_con)
{
    char restrict[100];

    Packet *pak = dbClient_createPacket(DBGAMESERVER_SEND_PLAYERS, client);
    pktSendBitsAuto(pak, shardaccount_con->id);
    pktSendString( pak, server_cfg.client_commands);
    pktSendBits(pak, 1, server_cfg.GRNagAndPurchase);
    pktSendBits(pak, 1, server_cfg.isBetaShard);
    pktSendBits(pak, 1, server_cfg.isVIPServer);
    pktSendBitsPack(pak,1,getNumPlayerSlotsVIP());

    sprintf_s(restrict, ARRAY_SIZE_CHECKED(restrict), "WHERE AuthId=%d", client->auth_id);
    sqlFifoBarrier();
    sqlReadColumnsAsyncWithDebugDelay(dbListPtr(CONTAINER_ENTS)->tplt->tables,0,
        SEND_PLAYERS_COLUMNS,restrict,sqlGetPlayersCallback,NULL,pak,0,server_cfg.debugSendPlayersDelayMS);
}

static void onVipUpdateLogin(VipSqlCbData *data)
{
    getEnts_sendPlayers(data->client, data->shardaccount_con);
}

static void updateVipStuff_getEnts_sendPlayers(GameClientLink *client, ShardAccountCon *shardaccount_con)
{
    if (!client || !shardaccount_con)
        return;

    if (!client->vipFlagReady) // this will only be 0 for fake_auth
    {
        client->vip = AccountIsVIP(NULL, shardaccount_con->accountStatusFlags) != 0;
        client->vipFlagReady = 1;
    } else {
        if (client->vip)
            shardaccount_con->accountStatusFlags |= ACCOUNT_STATUS_FLAG_IS_VIP;
        else
            shardaccount_con->accountStatusFlags = ACCOUNT_STATUS_FLAG_DEFAULT;
    }

    updateVip(onVipUpdateLogin, client, shardaccount_con, NULL);
}

static void finishSendingPlayers(GameClientLink *client, U32 auth_id)
{
    ShardAccountCon* shardaccount_con;    
    shardaccount_con = containerPtr(dbListPtr(CONTAINER_SHARDACCOUNT), auth_id);
    if (shardaccount_con)
    {
        updateVipStuff_getEnts_sendPlayers(client, shardaccount_con);
    }
    else if (!sqlIsAsyncReadPending(CONTAINER_SHARDACCOUNT, auth_id))
    {
        sqlReadContainerAsync(CONTAINER_SHARDACCOUNT, auth_id, client->link, client, NULL);
    }
}

static void sendPlayers(GameClientLink *client, U32 auth_id)
{
    // The server is not currently issuing a sqlFifoBarrier() here because
    // this is a hot path after a shard restart.  In some cases the player list
    // could be a few seconds out of date.

    finishSendingPlayers(client, auth_id);
}

static void resendPlayers(GameClientLink *client, U32 auth_id)
{
    // Insert a barrier to make sure that we wait for any pending character
    // changes before asking for the list.  Because this is slow, we need to
    // make sure that the login sequence never hits this path for normal
    // logins.
    sqlFifoBarrier();

    finishSendingPlayers(client, auth_id);
}

static void handleSqlAppearanceCallback(Packet *pak,U8 *appearance,int col_count,ColumnInfo **field_ptrs,void *data)
{
    int container_id = (int)(intptr_t)data;
    if(col_count)
    {    
        int i,val,idx=0,idxfld=0;
        int currentBodyType;
        int skinColor;

        pktSendBits(pak, 1, 1);

        currentBodyType = *(U8 *)(&appearance[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;
        pktSendBitsPack(pak, 1, currentBodyType);

        skinColor = *(U32 *)(&appearance[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;
        pktSendBits(pak, 32, skinColor);

        pktSendBitsPack(pak,1,NUM_2D_BODY_SCALES-1); // FIXME: hackish, no persistant arm scaling
        pktSendBitsPack(pak,1,NUM_3D_BODY_SCALES);

        for(i=0;i<NUM_2D_BODY_SCALES-1;i++) // FIXME: hackish, no persistant arm scaling
        {
            F32 scale = *(F32 *)(&appearance[idx]);
            idx += field_ptrs[idxfld++]->num_bytes;
            pktSendF32(pak, scale);
        }

        val = *(int *)(&appearance[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;
        pktSendBits(pak,1,val);

        for(i=0;i<NUM_3D_BODY_SCALES;i++)
        {
            int scale = *(int *)(&appearance[idx]);
            idx += field_ptrs[idxfld++]->num_bytes;
            pktSendBitsPack(pak, 1, scale);
        }

    }
    else
    {
        pktSendBits(pak,1,0);
    }
    pktSend(&pak, pak->link);
}


// adds data to the sendCostume() packet
static void sendAppearance(Packet * pak, int container_id, int currentCostume)
{
    DbList        *ent_list = dbListPtr(CONTAINER_ENTS);
    TableInfo    *table;
    char        restrict[400];

    table = tpltFindTable(ent_list->tplt, "Appearance");
    sprintf(restrict,"WHERE ContainerId = %d AND SubId = %d", container_id, currentCostume); // use first set of scales for each char
    sqlReadColumnsAsync(table,0,"BodyType, ColorSkin, BodyScale, BoneScale, HeadScale, ShoulderScale, ChestScale, WaistScale, HipScale, LegScale, ConvertedScale, HeadScales, BrowScales, CheekScales, ChinScales, CraniumScales, JawScales, NoseScales",
                        restrict,handleSqlAppearanceCallback,(void*)(intptr_t)container_id, pak, container_id);
}


#define TOTAL_COSTUME_PARTS (MAX_COSTUME_PARTS)
static void handleSqlCostumeCallback(Packet *pak,U8 *costume,int col_count,ColumnInfo **field_ptrs,void *data)
{
    int j, idx, total, currentCostume = 0;
    int container_id = (int)(intptr_t)data;

    // find actual number of costume parts belonging to costume 0
    total = 0;
    for(idx=0, j=0; j<col_count;j++)
    {
        U32 subId;
        int idxfld = 0;

        idx += field_ptrs[idxfld++]->num_bytes;        //    geom
        idx += field_ptrs[idxfld++]->num_bytes;        //    tex1
        idx += field_ptrs[idxfld++]->num_bytes;        //    tex2
        idx += field_ptrs[idxfld++]->num_bytes;        //    name
        idx += field_ptrs[idxfld++]->num_bytes;        //    fxname
        idx += field_ptrs[idxfld++]->num_bytes;        //    color1
        idx += field_ptrs[idxfld++]->num_bytes;        //    color2
        idx += field_ptrs[idxfld++]->num_bytes;        //    color3
        idx += field_ptrs[idxfld++]->num_bytes;        //    color4

        subId = *(U32 *)(&costume[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;        //    subid

        total++;
        currentCostume = subId / TOTAL_COSTUME_PARTS;        //    we are inferring the current costume from the subId
                                                            //    since the query already does the filtering, there should only be costume parts from the current costume
    }

    pktSendBitsPack(pak,1,total);
    for(idx=0, j=0; j<col_count;j++)
    {
        const char *geom;
        const char *tex1;
        const char *tex2;
        const char *name;
        const char *fxname;
        U32 color1, color2, color3, color4, subId;
        int idxfld = 0;

        geom = getAttribString(*(int *)(&costume[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        tex1 = getAttribString(*(int *)(&costume[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        tex2 = getAttribString(*(int *)(&costume[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        name = getAttribString(*(int *)(&costume[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        fxname = getAttribString(*(int *)(&costume[idx]));
        idx += field_ptrs[idxfld++]->num_bytes;

        color1 = *(U32 *)(&costume[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        color2 = *(U32 *)(&costume[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        color3 = *(U32 *)(&costume[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        color4 = *(U32 *)(&costume[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        subId = (*(U32 *)(&costume[idx])) % MAX_COSTUME_PARTS;
        idx += field_ptrs[idxfld++]->num_bytes;

            pktSendBits(pak, 32, subId);
            pktSendString(pak, geom);
            pktSendString(pak, tex1);
            pktSendString(pak, tex2);
            pktSendString(pak, name);
            pktSendString(pak, fxname);
            pktSendBits(pak, 32, color1);
            pktSendBits(pak, 32, color2);
            pktSendBits(pak, 32, color3);
            pktSendBits(pak, 32, color4);
    }

    sendAppearance(pak, container_id, currentCostume);
}


static void sendCostume(Packet *pak, int container_id, int currentCostume)
{
    DbList        *ent_list = dbListPtr(CONTAINER_ENTS);
    TableInfo    *table;
    char        restrict[400];

    table = tpltFindTable(ent_list->tplt, "CostumeParts");
    sprintf(restrict,"WHERE ContainerId = %d AND SubId >= %d AND SubId < %d", container_id, 
        currentCostume*TOTAL_COSTUME_PARTS, (currentCostume+1)*TOTAL_COSTUME_PARTS);
    sqlReadColumnsAsync(table,0,"Geom, Tex1, Tex2, Name, FxName, Color1, Color2, Color3, Color4, SubId",
                        restrict,handleSqlCostumeCallback,(void*)(intptr_t)container_id, pak, container_id);
}

static void handleSqlCurrentCostumeCallback(Packet *pak,U8 *ent,int col_count,ColumnInfo **field_ptrs,void *data)
{
    int container_id = (int)(intptr_t)data;
    int currentCostume = 0;

    if (col_count > 0 && ent)
        currentCostume = *(int*)(&ent[0]);

    sendCostume(pak, container_id, currentCostume);
}

static void sendCurrentCostume(GameClientLink *client, int slot_idx)
{
    Packet        *pak;
    NetLink        *link = client->link;
    DbList        *ent_list = dbListPtr(CONTAINER_ENTS);
    int            container_id;
    TableInfo    *table;
    char        restrict[400];

    if (slot_idx < 0 || slot_idx >= MAX_PLAYER_SLOTS )
    {
        // player sent us bad data. tricksy player is!
        if(!server_cfg.queue_server)
        {
            netRemoveLink(link);
        }
        else if(client->account_name || client->linkId)
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        return;
    }
    container_id = client->players[slot_idx];
    if (!container_id) {
        // The player probably recently deleted the character, but the UI in
        // the client had not finished refreshing yet and sent an invalid
        // request.  The player's client will likely be stuck for 60 seconds,
        // but we can return a message indicating why here.
        sendMsg(client->link, "CantResumeEmptyChar", client);
        return;
    }

    pak = dbClient_createPacket(DBGAMESERVER_SEND_COSTUME, client);
    pktSendBitsPack(pak, 1, slot_idx);

    table = tpltFindTable(ent_list->tplt, "Ents");
    sprintf(restrict,"WHERE ContainerId = %d", container_id);
    sqlReadColumnsAsync(table,0, "CurrentCostume", restrict, handleSqlCurrentCostumeCallback, (void*)(intptr_t)container_id, pak, container_id);
}

static void handleSqlCurrentPowersetNamesCallback(Packet *pak, U8 *ent, int col_count, ColumnInfo **field_ptrs, void *data)
{
    int container_id = (int)(intptr_t)data;
    char *primary = NULL;
    char *secondary = NULL;
    int idx=0,idxfld=0;

    if (col_count)
    {
        primary = (char *)(&ent[idx]);
        idx += field_ptrs[idxfld++]->num_bytes;

        secondary = (char *)(&ent[idx]);
//        idx += field_ptrs[idxfld++]->num_bytes;
    }

    pktSendString(pak,unescapeString(primary));
    pktSendString(pak,unescapeString(secondary));
    pktSend(&pak, pak->link);
}

static void sendCurrentPowersetNames(GameClientLink *client, int slot_idx)
{
    Packet        *pak;
    NetLink        *link = client->link;
    DbList        *ent_list = dbListPtr(CONTAINER_ENTS);
    int            container_id;
    TableInfo    *table;
    char        restrict[400];

    if (slot_idx < 0 || slot_idx >= MAX_PLAYER_SLOTS )
    {
        // player sent us bad data. tricksy player is!
        if(!server_cfg.queue_server)
        {
            netRemoveLink(link);
        }
        else if(client->account_name || client->linkId)
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        return;
    }
    container_id = client->players[slot_idx];
    pak = dbClient_createPacket(DBGAMESERVER_SEND_POWERSET_NAMES, client);
    pktSendBitsPack(pak, 1, slot_idx);

    table = tpltFindTable(ent_list->tplt, "Ents2");
    sprintf(restrict,"WHERE ContainerId = %d", container_id);
    sqlReadColumnsAsync(table,0, "originalPrimary, originalSecondary", restrict, handleSqlCurrentPowersetNamesCallback, (void*)(intptr_t)container_id, pak, container_id);
}

static int s_skipQueue(GameClientLink *client)
{
    int online = queueservercomm_getNLoggingIn() + inUseCount(dbListPtr(CONTAINER_ENTS));

    // always allow GMs and Relogging players
    if(authnameAutoLogin(client->account_name, 1))
        return 1;

    // Block free players if the account server is down
    if (server_cfg.block_free_if_no_account && !accountServerResponding() && !AccountIsVIPByPayStat(client->payStat))
        return 0;

    // Queue players while overload protection is enabled
    if (overloadProtection_IsEnabled())
        return 0;

    // if the server isn't full, let them in
    if((server_cfg.max_players && !queueservercomm_getNWaiting()) && (online <= server_cfg.max_players-1))
        return 1;

    if(server_cfg.enqueue_auth_limiter && //we're testing, let allowed people into the game as long as there's room, regardless of queue size.
        server_cfg.authname_limiter && 
        authnameAllowedLogin(client->account_name, 0) && 
        online <= server_cfg.max_players-1)
        return 1;

    return 0;
}

/**
* Generate a fake auth id when fake_auth mode is enabled.
*/
int genFakeAuthId(char * auth_name)
{
    char lowered_auth_name[64];
    char *target = lowered_auth_name;
    while (*auth_name)
    {
        *target = tolower(*auth_name);

        ++auth_name;
        ++target;
    }

    return hashCalc((void*)lowered_auth_name, target - lowered_auth_name, 0xa74d94e3) & 0x7fffffff;
}

static int handleLogin(Packet *pak,GameClientLink *client)
{
    U32        cookie,auth_id;
    U8        test_auth_data[AUTH_BYTES];

    {
        int            dont_check_version,protocol_version,no_version_check_from_client;
        char        game_version[100];
        const char*    correct_version;
        U32            game_checksum[4];

        strcpy(client->account_name,pktGetString(pak));
        auth_id    = pktGetBitsPack(pak,1);
        protocol_version    = pktGetBitsPack(pak,1);
        if (protocol_version != DBSERVER_CLIENT_PROTOCOL_VERSION) // date of last change
        {
            LOG_OLD_ERR("IP: %s connected with protocol %d", makeIpStr(client->link->addr.sin_addr.S_un.S_addr), protocol_version);
            sendMsg(client->link,"WrongProtocol", client);
            return 0;
        }
        pktGetBitsArray(pak,sizeof(test_auth_data)*8,test_auth_data); // only used if no auth server (for testing)
        no_version_check_from_client = dont_check_version    = pktGetBitsPack(pak,1);
        Strncpyt(game_version, pktGetString(pak));
        correct_version = getCompatibleGameVersion();

        pktGetBitsArray(pak,8*sizeof(game_checksum),game_checksum);
        cookie    = pktGetBits(pak,32);

        if (clientCommLoginVerifyChecksum(game_checksum))
            strcpy(game_version, "Bad client checksum - reverify files");
        if (!server_cfg.fake_auth && strnicmp(correct_version,"dev:",4)!=0)
            dont_check_version = 0;
        if (!dont_check_version && stricmp(game_version,correct_version) != 0)
        {
            char    buf[1000];

            if (strnicmp(correct_version,"dev",3)==0)
                sprintf(buf,"WrongVersion \"%s\" \"%s\"",correct_version,game_version);
            else
                sprintf(buf,"WrongVersionPatcher \"%s\" \"%s\"",correct_version,game_version);
            LOG_OLD_ERR("IP: %s, AuthName %s, connected with version %s checksum: %08x %08x %08x %08x", makeIpStr(client->link->addr.sin_addr.S_un.S_addr), client->account_name, game_version, game_checksum[0], game_checksum[1], game_checksum[2], game_checksum[3]);
            sendMsg(client->link,buf, client);
            return 0;
        }
        if (!no_version_check_from_client) 
        {
            char *client_branch = strstri(game_version, "Branch ");
            const char *correct_branch = strstriConst(correct_version, "Branch ");
            if (client_branch && correct_branch && stricmp(client_branch, correct_branch) != 0) 
            {
                char    buf[1000];

                if (strnicmp(correct_version,"dev",3)==0)
                    sprintf(buf,"Wrong game version\nserver %s\nclient %s\nYou are on a different branch than the server is running!",correct_version,game_version);
                else
                    sprintf(buf,"WrongVersionPatcher \"%s\" \"%s\"",correct_version,game_version);
                sendMsg(client->link,buf, client);
                return 0;
            }
        }
    }

    // DGNOTE - Edison  If running in fake auth is going to fail anywhere, it'll be really close to here.
    if (server_cfg.fake_auth || authValidLogin(auth_id,cookie,client->account_name,client->auth_user_data,&client->loyalty,&client->loyaltyLegacy, &client->payStat))
    {
        if (server_cfg.fake_auth)
        {
            if (!auth_id)
                auth_id = genFakeAuthId(client->account_name);

            memcpy(client->auth_user_data,test_auth_data,sizeof(client->auth_user_data));
            client->loyalty = server_cfg.defaultLoyaltyPointsFakeAuth;
            client->loyaltyLegacy = server_cfg.defaultLoyaltyLegacyPointsFakeAuth;
            client->vip = client->vipFlagReady = 1;
        }
        else
        {
            client->vip = AccountIsVIPByPayStat(client->payStat) != 0;
            client->vipFlagReady = 1;
        }

        if (!auth_id)
        {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "%s invalid login for \"%s\" provided no AuthId\n", linkIpStr(client->link), client->account_name);
            sendMsg(client->link,"DBInvalidLogin", client);
            return 0;    //why would we continue?
        }

        LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, LOG_DEFAULT, "%s successful login for \"%s\" AuthID %d Cookie %d\n", linkIpStr(client->link), client->account_name, auth_id, cookie);
        client->auth_id = auth_id;
        client->valid = 1;
        authSendPlayGame(client->auth_id);
    }
    else
    {
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "%s invalid login for \"%s\" provided AuthId %d Cookie %d\n", linkIpStr(client->link), client->account_name, auth_id, cookie);
        sendMsg(client->link,"DBInvalidLogin", client);
        return 0;    //why would we continue?
    }
    if (!pktEnd(pak)) { // Just doing a !pktEnd so I don't have to increase the DbServer-GameClient version number again...
        client->link->notimeout = pktGetBits(pak, 1);
        if (isProductionMode())
        {
            client->link->notimeout = 0; // Don't allow in production
        }
    }
    if (!pktEnd(pak))
    { // Just doing a !pktEnd so I don't have to increase the DbServer-GameClient version number again and to allow "hacked"/leaked clients to connect
        char *patchValue = pktGetString(pak);
        if (patchValue && patchValue[0] && stricmp(patchValue, "0")!=0) 
        {
            U32 patchValueInt = atoi(patchValue);
            LOG_OLD( "IP: %s, AuthName %s, sent PatchValue of %u", makeIpStr(client->link->addr.sin_addr.S_un.S_addr), client->account_name, (U32)patchValueInt);
        }
    }
    if (!pktEnd(pak))
    {
        pktGetString(pak); // No longer used
    }
    if (!pktEnd(pak)) 
    {
        U32 slen;
        client->systemSpecs = pktGetZipped(pak, &slen);
        if (client->systemSpecs && !client->systemSpecs[0])
        {
            SAFE_FREE(client->systemSpecs);
            client->systemSpecs = 0;
        }
    }
    if (!pktEnd(pak)) 
    {
        U32 keyed_access_level;
        keyed_access_level = pktGetBitsPack(pak, 1);
        if (keyed_access_level)
        {
            char *issuedTo = pktGetString(pak);
            LOG( LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0, "IP: %s, AuthName %s, has access level key of %d issued to \"%s\"", makeIpStr(client->link->addr.sin_addr.S_un.S_addr), client->account_name, keyed_access_level, issuedTo);
        }
    }

    if(server_cfg.queue_server && !authnameAllowedLogin(client->account_name, 0))    //for the queue server, we ignore level
    {
        //if we enqueue on failure, do that.  Otherwise, just deny the client.
        if(server_cfg.enqueue_auth_limiter)
        {
            //enqueue the client until the queue server says otherwise.
            queueservercomm_switchgamestatus(client, GAMELLOGIN_QUEUED);
            return 1;
        }
        else
        {
            static char        msg[1000];
            char *denied_msg = getAuthDeniedMsg();
            LOG_OLD_ERR("account \"%s\" authname not allowed\n",client->account_name);
            sprintf(msg,"AccountNotAllowed %s \"%s\"", client->account_name, denied_msg?denied_msg:"");
            sendMsg(client->link,msg, client);
            return 0;        //we can't force quit them or they won't get our disconnect message.
        }
    }

    if(!server_cfg.queue_server)
    {
        clientcomm_PlayerLogin(client);
    }
    else
    {
        int readyToPlay = s_skipQueue(client);
        //this has been validated, so stick it in the name hash.  If there's anyone else there, this will remove them.
        queueservercomm_switchgamestatus(client, readyToPlay?GAMELLOGIN_LOGGINGIN:GAMELLOGIN_QUEUED);
    }

    return 1;
}

void clientcomm_PlayerLogin(GameClientLink *client)
{
    int vip;

    ShardAccountCon *con = containerPtr(shardaccounts_list, client->auth_id);
    if (con)
        con->logged_in = 1;

    queueservercomm_switchgamestatus(client, GAMELLOGIN_LOGGINGIN);

    // send catalog information out to the client
    account_handleProductCatalog(client);

    // register the account
    account_handleAccountRegister(client->auth_id, client->account_name);

    // update loyalty information on account server
    if (client->vipFlagReady) // vipFlagReady is 1 for real authserver
    {
        vip = client->vip;
    }
    else if (con) // fake auth, use debug flag
    {
        vip = AccountIsVIP(NULL, con->accountStatusFlags) != 0;
    }
    else // fake auth, and debug flag isn't available yet, assume not vip
    {
        vip = 0;
    }
    account_handleLoyaltyAuthUpdate(client->auth_id, client->payStat, client->loyalty, client->loyaltyLegacy, vip);

    // request account inventory from account server
    account_handleAccountInventoryRequest(client->auth_id);

    // finally, send characters to game.
    sendPlayers(client,client->auth_id);
}

int totalPlayersOnMap(MapCon* map)
{
    return    map->num_players +
            map->num_players_connecting +
            map->num_players_waiting_for_start +
            (map->active ? map->num_players_sent_since_who_update : 0);
}

static int mapWithinSafePlayersLow(MapCon* map)
{
    return !map->safePlayersLow || totalPlayersOnMap(map) < map->safePlayersLow;
}

static int mapWithinSafePlayersHigh(MapCon* map)
{
    return !map->safePlayersHigh || totalPlayersOnMap(map) < map->safePlayersHigh;
}

int mapIsRunning(MapCon* map)
{
    return map->active || map->starting;
}

int findBestMapByBaseId(EntCon* ent_con, int map_id)
{
    MapCon* map;
    char log_buffer[10000] = "";
    char* bufpos = log_buffer;
    char* ent_name;
    int ent_id;
    int i;

    map = containerPtr(map_list,map_id);

    if(!map)
        return 0;

    if(ent_con)
    {
        ent_name = ent_con->ent_name;
        ent_id = ent_con->id;
    }
    else
    {
        ent_name = "NewPlayer";
        ent_id = -1;
    }

    if(eaSize(&server_cfg.blocked_map_keys))
    {
        StaticMapInfo* map_info = staticMapInfoFind(map->base_map_id);
        bool blocked = false;
        int i, j;

        if(map_info)
        {
            for(i = eaSize(&server_cfg.blocked_map_keys)-1; !blocked && i >= 0; i--)
            {
                for(j = eaSize(&map_info->mapKeys)-1; !blocked && j >= 0; j--)
                {
                    if(!stricmp(server_cfg.blocked_map_keys[i], map_info->mapKeys[j]))
                    {
                        blocked = true;
                    }
                }
            }

            // the map that we want has been blocked in the servers.cfg
            // put them in the starting map for their playertype
            if(blocked)
            {
                char cmd_buf[1000];
                char db_flags_text[200];
                int cmd_idx = 0;
                int db_flags;
                int new_map_id;

                // new players won't have a type or praetorian status so we
                // default them to hero
                if(ent_con && (ent_con->praetorianProgress == kPraetorianProgress_Tutorial || ent_con->praetorianProgress == kPraetorianProgress_Praetoria))
                    new_map_id = MAPID_NOVA_PRAETORIA;
                else if(ent_con && ent_con->playerType == kPlayerType_Villain)
                    new_map_id = MAPID_MERCY_ISLAND;
                else
                    new_map_id = MAPID_ATLAS_PARK;

                map = containerPtr(map_list, new_map_id);

                cmd_idx += sprintf(&cmd_buf[cmd_idx],"SpawnTarget NewPlayer\n");

                if(ent_con)
                {
                    findFieldTplt(ent_list->tplt, &ent_con->line_list,"DbFlags",db_flags_text);
                    db_flags = atoi(db_flags_text);
                    db_flags &= ~DBFLAG_TELEPORT_XFER;
                    db_flags |= DBFLAG_DOOR_XFER;
                    db_flags &= ~DBFLAG_MAPMOVE;
                    sprintf(&cmd_buf[cmd_idx],"DbFlags %u\n",db_flags);

                    containerUpdate(dbListPtr(CONTAINER_ENTS),ent_con->id,cmd_buf,0);
                }
            }
        }
    }

    if(map->base_map_id)
    {
        int lowestCount;
        MapCon* lowestMap = NULL;
        MapCon* allClones[1000];
        int cloneCount = 0;

        // Compile list of clone maps.

        for(i = 0; i < map_list->num_alloced; i++)
        {
            MapCon* same_map = (MapCon*)map_list->containers[i];

            if(cloneCount < ARRAY_SIZE(allClones) && same_map->base_map_id == map->base_map_id)
            {
                bufpos += sprintf(    bufpos,
                                    "%s%d:%d%s%s",
                                    bufpos == log_buffer ? "" : ", ",
                                    same_map->id,
                                    totalPlayersOnMap(same_map),
                                    mapIsRunning(same_map) ? "" : "(off)",
                                    map_id == same_map->id ? "(**ORIG**)" : "");

                allClones[cloneCount++] = same_map;
            }
        }

        if(!mapIsRunning(map) || !mapWithinSafePlayersLow(map))
        {
            // Find a running map that's within LOW range.

            for(i = 0; i < cloneCount; i++)
            {
                MapCon* same_map = allClones[i];

                if(!mapIsRunning(same_map))
                    continue;

                if(mapWithinSafePlayersLow(same_map))
                {
                    LOG_OLD( "Step 1: Character %d(%s) sent to map %d (%s)\n", ent_id, ent_name, same_map->id, log_buffer);
                    return same_map->id;
                }
            }

            // Find a non-running map.

            for(i = 0; i < cloneCount; i++)
            {
                MapCon* same_map = allClones[i];

                if(mapWithinSafePlayersLow(same_map))
                {
                    LOG_OLD("Step 2: Character %d(%s) sent to map %d (%s)\n", ent_id, ent_name, same_map->id, log_buffer);
                    return same_map->id;
                }
            }

            // Find a running map that's within HIGH range.

            for(i = 0; i < cloneCount; i++)
            {
                MapCon* same_map = allClones[i];

                if(!mapIsRunning(same_map))
                    continue;

                if(mapWithinSafePlayersHigh(same_map))
                {
                    LOG_OLD("Step 3: Character %d(%s) sent to map %d (%s)\n", ent_id, ent_name, same_map->id, log_buffer);
                    return same_map->id;
                }
            }

            // No luck so far, just find a matching base ID with the least players.

            for(i = 0; i < cloneCount; i++)
            {
                MapCon* same_map = allClones[i];
                int playerCount;

                playerCount = totalPlayersOnMap(same_map);

                if(!lowestMap || playerCount < lowestCount)
                {
                    lowestMap = same_map;
                    lowestCount = playerCount;
                }
            }

            if(lowestMap)
            {
                LOG_OLD( "Step 4: Character %d(%s) sent to map %d (%s)\n", ent_id, ent_name, lowestMap->id, log_buffer);
                return lowestMap->id;
            }
        }
    }

    LOG_OLD( "Step 5: Character %d(%s) sent to map %d (%s)\n", ent_id, ent_name, map->id, log_buffer);
    return map->id;
}

 
int determineStartMapId(int createLocation)
{
	int i;
	int base_map_id;
	int best_map_id = 0;
	int max_players = 30;	// Default if SafePlayersLow is not set.
    MapCon *map;

	if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO)
		base_map_id = 1;	// Atlas Park
	else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO_TUTORIAL)
		base_map_id = 24;	// Outbreak
    else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN)
		base_map_id = 71;	// Mercy Island
	else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN_TUTORIAL)
		base_map_id = 70;	// Breakout
	else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_TUTORIAL)
		base_map_id = 40;	// Precint 5
    else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_LOYALIST
            || createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_RESISTANCE)
		base_map_id = 36;	// Nova Praetoria
	else
		base_map_id = 41;	// Destroyed Galaxy City

	map = containerPtr(map_list, base_map_id); 
	if (map)
    {
		if (map->safePlayersLow)
			max_players = map->safePlayersLow;

		if (totalPlayersOnMap(map) < max_players)
			return base_map_id;
    }

	// Base map ID is full, find another instance.
	for(i = 0; i < map_list->num_alloced; i++)
    {
        map = (MapCon *) map_list->containers[i];

		if (base_map_id != map->base_map_id)
            continue;

		// The map is not running, but we don't have a better option yet.
		if (!map->map_running && !best_map_id)
			best_map_id = map->id;

		// The map is running, see if it's not full.
		if (map->map_running && totalPlayersOnMap(map) < max_players)
        { 
			best_map_id = map->id;
			break;
        }
    }

	// Return the first non-full instance, or the base map if we didn't find any.
	return best_map_id ? best_map_id : base_map_id;
}

static int determinePlayerType(int createLocation)
{
    if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_TUTORIAL
        || createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO
		|| createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO_TUTORIAL
        || createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_RESISTANCE)
    {
        return kPlayerType_Hero;
    }
    else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN
            || createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_TUTORIAL
			|| createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN_TUTORIAL
            || createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_LOYALIST)
    {
        return kPlayerType_Villain;
    }
    else // default, shouldn't ever happen
    {
        return kPlayerType_Hero;
    }
}

static int determinePraetorianProgress(int createLocation)
{
    if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_TUTORIAL)
        return kPraetorianProgress_NeutralInPrimalTutorial;
    else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO
			|| createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO_TUTORIAL
			|| createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN
			|| createLocation == DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN_TUTORIAL)
        return kPraetorianProgress_PrimalBorn;
    else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_TUTORIAL)
        return kPraetorianProgress_Tutorial;
    else if (createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_LOYALIST
            || createLocation == DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_RESISTANCE)
        return kPraetorianProgress_Praetoria;
    else // default, shouldn't ever happen
        return kPraetorianProgress_PrimalBorn;
}

static bool entConAuthUserDataIsEmpty(EntCon *ent_con)
{
    bool retval = false;
    char ent_auth[AUTH_BYTES * 2 + 128] = "unset";
    char *auth_pos;

    if (devassert(ent_con))
    {
        findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt, &ent_con->line_list, "Ents2[0].AuthUserDataEx", ent_auth);
        retval = true;

        if (stricmp(ent_auth, "unset") != 0)
        {
            auth_pos = &(ent_auth[0]);

            // Go through the hex string and look for any non-zero digits.
            // If it's all zeroes, the authbits are unset, otherwise they
            // have been previously set either from the auth server, from
            // the test data or by hand.
            while (*auth_pos && retval)
            {
                if (*auth_pos != '0')
                {
                    retval = false;
                }

                auth_pos++;
            }
        }
    }

    return retval;
}

static void updateAuthUserData(GameClientLink *client,EntCon *ent_con)
{
    int i;
    char *hex_str;
    char auth_user_data_hex[AUTH_BYTES * 2 + 1024];
    char auth_user_data_orig_hex[AUTH_BYTES * 2 + 1024];
    unsigned char auth_user_data[AUTH_BYTES];
    unsigned char auth_user_data_orig[AUTH_BYTES];
    U32 *auth_words;
    U32 *auth_words_orig;
    char old_hex[AUTH_BYTES * 2 + 1024] = "unset";

    // Don't replace existing auth data with the default if there is no
    // auth server. If there is a server, make all characters be consistent
    // for the auth (which stores it in the client link).
    if (!server_cfg.fake_auth || entConAuthUserDataIsEmpty(ent_con))
    {
        // This is a total anachronism, we could maybe dispense with it
        hex_str = binStrToHexStr(client->auth_user_data,sizeof(client->auth_user_data));
        sprintf(auth_user_data_hex,"Ents2[0].AuthUserDataEx \"%s\"\n",hex_str);

        findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt,&ent_con->line_list,"Ents2[0].AuthUserDataEx",old_hex);
        if (stricmp(old_hex,hex_str)!=0)
        {
            LOG_OLD( "%s 0x%s updating entity %d %s", ent_con->account, hex_str, ent_con->id, ent_con->ent_name);
            containerUpdate(dbListPtr(CONTAINER_ENTS),ent_con->id,auth_user_data_hex,TRUE);
        }
    }

    // If we determine that this change is also needed when there's an auth server, just remove this test
    if (server_cfg.fake_auth)
    {
        strcpy(auth_user_data_hex, "unset");
        strcpy(auth_user_data_orig_hex, "unset");
        findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt, &ent_con->line_list, "Ents2[0].AuthUserDataEx", auth_user_data_hex);
        findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt, &ent_con->line_list, "AuthUserData", auth_user_data_orig_hex);
        if (stricmp(auth_user_data_hex, "unset" ) != 0 && stricmp(auth_user_data_orig_hex, "unset" ) != 0)
        {
            // This seems extremely inefficient, however it'll only ever do work in the case of an error.
            // Pad out a short string to the expected length
            while (strlen(auth_user_data_hex) < AUTH_BYTES * 2)
            {
                strcat(auth_user_data_hex, "0");
            }
            // and truncate a long one
            auth_user_data_hex[AUTH_BYTES * 2] = 0;

            memcpy(auth_user_data, hexStrToBinStr(auth_user_data_hex, AUTH_BYTES * 2), AUTH_BYTES);

            while (strlen(auth_user_data_orig_hex) < AUTH_BYTES_ORIG * 2)
            {
                strcat(auth_user_data_orig_hex, "0");
            }
            auth_user_data_orig_hex[AUTH_BYTES_ORIG * 2] = 0;

            // This memcpy is technically unnecessary.  However since I will be changing the binary data, I prefer to do
            // the changes in a local buffer
            memcpy(auth_user_data_orig, hexStrToBinStr(auth_user_data_orig_hex, AUTH_BYTES_ORIG * 2), AUTH_BYTES_ORIG);

            auth_words = (U32 *) auth_user_data;
            auth_words_orig = (U32 *) auth_user_data_orig;
            for (i = 0; i < AUTH_DWORDS_ORIG; i++)
            {
                *auth_words |= *auth_words_orig;
                // zero out the original data, so that if someone uses "AuthUserDatSet xyzzy 0" to clear an auth bit
                // it'll stay clear
                *auth_words_orig = 0;

                auth_words++;
                auth_words_orig++;
            }

            hex_str = binStrToHexStr(auth_user_data, AUTH_BYTES);
            sprintf(auth_user_data_hex, "Ents2[0].AuthUserDataEx \"%s\"\n", hex_str);

            containerUpdate(dbListPtr(CONTAINER_ENTS), ent_con->id, auth_user_data_hex, TRUE);

            // And write back the zeroed out original data.
            hex_str = binStrToHexStr(auth_user_data_orig, AUTH_BYTES_ORIG);
            sprintf(auth_user_data_orig_hex, "AuthUserData \"%s\"\n", hex_str);

            containerUpdate(dbListPtr(CONTAINER_ENTS), ent_con->id, auth_user_data_orig_hex, TRUE);
        }
    }

    ent_con->auth_user_data_valid = 1;
}

//----------------------------------------
//  get/create player
//----------------------------------------
typedef struct ChoosePlayerCreateCbData 
{
    GameClientLink *client;
    char name[64];
    int start_map_id;
    int playerType;
    int playerSubType;
    int praetorian;
    int playerTypeByLocation;
} ChoosePlayerCreateCbData;

static void s_finishHandleChoosePlayer(Packet *pak, U8 *cols, int col_count, ColumnInfo **field_ptrs, void *data)
{
    ChoosePlayerCreateCbData *cbData = data;
    GameClientLink *client = cbData->client;
    NetLink *link = client->link;
    int    unlocked_character_count = col_count;
    int owned_slots;

    owned_slots = client->vip ? getNumPlayerSlotsVIP() : 0;
    owned_slots += accountInventoryFindBonusCharacterSlots(client->auth_id);

    if (unlocked_character_count + 1 > owned_slots)
    {
        LOG_OLD_ERR( "%s account \"%s\" tried to create more characters than they have slots for.\n",linkIpStr(client->link),client->account_name);
        sendMsg(link,"NotEnoughSlots", client);
    }
    else
    {
        EntCon *ent_con;
        int idx;
        char cmd_buf[1000];

        ent_con = containerAlloc(dbListPtr(CONTAINER_ENTS),-1);
        playerNameCreate(cbData->name,ent_con->id, ent_con->auth_id);
        strcpy(ent_con->account,client->account_name);
        ent_con->map_id = ent_con->static_map_id = cbData->start_map_id;
        idx = sprintf(cmd_buf,"AuthId %d\n",client->auth_id);
        idx += sprintf(cmd_buf + idx,"AuthName \"%s\"\n",client->account_name);
        idx += sprintf(cmd_buf + idx,"StaticMapId %d\n",ent_con->static_map_id);
        idx += sprintf(cmd_buf + idx,"Name \"%s\"\n",cbData->name);
        idx += sprintf(cmd_buf + idx,"AccessLevel %d\n",server_cfg.default_access_level);
        idx += sprintf(cmd_buf + idx,"PlayerType %d\n",cbData->playerType);
        idx += sprintf(cmd_buf + idx,"Ents2[0].PlayerSubType %d\n",cbData->playerSubType);
        idx += sprintf(cmd_buf + idx,"Ents2[0].PraetorianProgress %d\n",cbData->praetorian);
        idx += sprintf(cmd_buf + idx,"Ents2[0].InfluenceType %d\n",cbData->playerTypeByLocation);
        containerUpdate(dbListPtr(CONTAINER_ENTS),ent_con->id,cmd_buf,0);
        assert(ent_con->just_created);
        LOG_OLD_ERR("%s account \"%s\" created \"%s\"\n",linkIpStr(client->link),client->account_name,cbData->name);
        handlePlayerContainerLoaded(link,ent_con->id, client);
    }

    free(cbData);
}

// the opposite of offlining a character, this terminology is confusing
// copy/pasted from handleChoosePlayer
static void handleMakePlayerOnline(Packet *pak, GameClientLink *client)
{
    int            slot_idx,resume_map_id=-1;
    static char    msg[1000]; // Static to reduce stack size
    NetLink        *link = client->link;

    slot_idx = pktGetBitsPack(pak,1);
    if (slot_idx < 0 || slot_idx >= MAX_PLAYER_SLOTS )
    {
        // player sent us bad data. tricksy player is!
        LOG_OLD_ERR( "%s account \"%s\" sent bad data\n",linkIpStr(client->link),client->account_name);
        if(!server_cfg.queue_server)
        {
            netRemoveLink(link);
        }
        else if(client->account_name || client->auth_id)
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        return;
    }

    client->char_id = client->players[slot_idx];

    devassert(client->vipFlagReady);
    if (server_cfg.isVIPServer && !client->vip)
    {
        sendMsg(link,"CantResumeVIPShard",client);
        return;
    }

    if (client->char_id)
    {
        EntCon    *c = (EntCon*) containerPtr(dbListPtr(CONTAINER_ENTS),client->char_id);
        if (c || sqlIsAsyncReadPending(CONTAINER_ENTS, client->char_id))
        {
            return;
        }
        if (client->players_offline[slot_idx])
        {
            if ( offlinePlayerRestoreOfflined(client->auth_id,client->account_name,client->char_id) != kOfflineRestore_SUCCESS )
            {
                LOG(LOG_OFFLINE, LOG_LEVEL_ALERT, 0, "could not restore offline character %d account %s",client->char_id,client->account_name);
                sprintf(msg,"CantFindPlayer %d",client->char_id);
                sendMsg(link,msg, client);
                return;
            }

            resendPlayers(client, client->auth_id);
        }
    }    
}

// copy/pasted to handleMakePlayerOnline
static void handleChoosePlayer(Packet *pak, GameClientLink *client)
{
    int            slot_idx,resume_map_id=-1;
    static char    msg[1000]; // Static to reduce stack size
    NetLink        *link = client->link;
    int            createLocation;
    char        name[64];

    slot_idx = pktGetBitsPack(pak,1);
    if (slot_idx < 0 || slot_idx >= MAX_PLAYER_SLOTS )
    {
        // player sent us bad data. tricksy player is!
        LOG_OLD_ERR( "%s account \"%s\" sent bad data\n",linkIpStr(client->link),client->account_name);
        if(!server_cfg.queue_server)
        {
            netRemoveLink(link);
        }
        else if(client->account_name || client->auth_id)
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        return;
    }
    client->local_map_ip = pktGetBitsPack(pak,1);
    client->char_id = client->players[slot_idx];
    strncpy(name,escapeString(pktGetString(pak)),sizeof(name));
    name[sizeof(name)-1]=0; // Just in case we get a string longer than 64, we need to null-terminate
    createLocation = pktGetBitsAuto(pak);

    devassert(client->vipFlagReady);
    if (server_cfg.isVIPServer && !client->vip)
    {
        sendMsg(link,"CantResumeVIPShard",client);
        return;
    }

    if (!client->char_id) // client wants us to make a new entry
    {
        int        isempty;
        int        start_map_id = determineStartMapId(createLocation);
        int        playerType = determinePlayerType(createLocation);
        int        playerSubType = kPlayerSubType_Normal; // can't create a rogue or vigilante
        int        praetorian = determinePraetorianProgress(createLocation);
        int        playerTypeByLocation = playerType; // can't create someone who's touristing
        ChoosePlayerCreateCbData    *cbData;
        char    buf[100];
        int        tempLockResult;

        // Skipped tutorial and chose an alternate starting contact
        if (!playerNameValid(name, &isempty, client->auth_id))
        {
            if (isempty)
            {
                LOG_OLD_ERR( "%s account \"%s\" tried to resume an empty character\n",linkIpStr(client->link),client->account_name);
                sendMsg(link,"CantResumeEmptyChar",client);
            }
            else
            {
                sprintf(msg,"DuplicateName \"%s\"",name);
                LOG_OLD_ERR( "%s account \"%s\" tried to create %s\n",linkIpStr(client->link),client->account_name, msg);
                sendMsg(link,msg, client);
            }

            return;
        }
        //use the name reserve feature to hold this name, since the name is created in s_finishHandleChoosePlayer after asynchronous request
        tempLockResult = tempLockAdd( name, client->auth_id, timerSecondsSince2000() );
        devassertmsg(tempLockResult, "Problem reserving the name, %s.", name);

        sqlFifoBarrier(); // make sure IsSlotLocked is fully up to date

        cbData = calloc(1, sizeof(ChoosePlayerCreateCbData));
        cbData->client = client;
        strcpy(cbData->name, name);
        cbData->start_map_id = start_map_id;
        cbData->playerType = playerType;
        cbData->playerSubType = playerSubType;
        cbData->praetorian = praetorian;
        cbData->playerTypeByLocation = playerTypeByLocation;

        sprintf(buf, "WHERE AuthId='%u' AND IsSlotLocked IS NULL", client->auth_id);
        sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",buf,s_finishHandleChoosePlayer,cbData,NULL,0);
    }
    else
    {
        EntCon    *c = (EntCon*) containerPtr(dbListPtr(CONTAINER_ENTS),client->char_id);
        if (c || sqlIsAsyncReadPending(CONTAINER_ENTS, client->char_id))
        {
            sprintf(msg,"CharacterLoggingOut \"%s\"",c ? c->ent_name : "???");
            dbForceLogout(client->char_id,-1);
            sendMsg(link,msg, client);
            return;
        }
        if (client->players_offline[slot_idx])
        {
            dbForceLogout(client->char_id,-1);
            sendMsg(link, "InternalErrorCantChooseOfflineCharacter", client);
            return;
        }
        sqlReadContainerAsync(CONTAINER_ENTS, client->char_id, link, client, NULL);
    }    
}

//----------------------------------------
//  see if we can start a static map
//----------------------------------------
static void handleCanStartStaticMap(Packet *pak,GameClientLink *client)
{
    int    start_map_id;
    int response;

    start_map_id = pktGetBitsAuto(pak);
    start_map_id = determineStartMapId(start_map_id);

    response = canStartStaticMap(start_map_id, NULL) ? 1 : 0;

    {
        Packet* pak_out = dbClient_createPacket(DBGAMESERVER_CAN_START_STATIC_MAP_RESPONSE, client);
        if( pak_out )
        {
            pktSendBitsPack(pak_out,1,response);
            pktSend(&pak_out, client->link);
        }
    }
}

//----------------------------------------
//  get visiting player
//----------------------------------------
static void handleChooseVisitingPlayer(Packet *pak, GameClientLink *client)
{
    int    dbid;
    int resume_map_id = -1;
    char msg[128];
    EntCon *c;
    NetLink *link;

    link = client->link;
    dbid = pktGetBitsAuto(pak);
    client->local_map_ip = 0;
    client->char_id = dbid;

    c = (EntCon *) containerPtr(dbListPtr(CONTAINER_ENTS),client->char_id);
    if (c || sqlIsAsyncReadPending(CONTAINER_ENTS, client->char_id))
    {
        sprintf(msg,"CharacterLoggingOut \"%s\"",c ? c->ent_name : "???");
        dbForceLogout(client->char_id,-1);
        sendMsg(link, msg, client);
        return;
    }
    sqlReadContainerAsync(CONTAINER_ENTS, client->char_id, link, client, NULL);
}

// *********************************************************************************
// Authname Limiter
// Purpose: for limitng access to training room for invention beta testing
// - load a file of names allowed
// - deny everybody else access
// *********************************************************************************
typedef struct AllowedAuthnames
{
    __time32_t file_last_changed;
    char *denied_msg;
    char **allowed_names;

    // not parsed 
    StashTable allowed_stash;
} AllowedAuthnames;

TokenizerParseInfo  parse_AllowedAuthnames[] =
{
    {    "DeniedMessage", TOK_STRING(AllowedAuthnames, denied_msg, "")},
    {    "Allowed", TOK_STRINGARRAY(AllowedAuthnames, allowed_names)},
    {    "", 0, 0 }
};
static AllowedAuthnames g_allowed_authnames = {0};
static AllowedAuthnames g_auto_allowed_authnames = {0};
static AllowedAuthnames g_auto_allowed_once_authnames = {0};

char *getAuthDeniedMsg()
{
    return g_allowed_authnames.denied_msg;
}

static __time32_t s_updateAuthAllowNames()
{
    __time32_t tmp = 0;
    char *filename = "server/db/auto_allowed_authnames.cfg";
    tmp = fileLastChanged(filename);

    if(tmp != g_auto_allowed_authnames.file_last_changed)
    {
        StructClear(parse_AllowedAuthnames,&g_auto_allowed_authnames);
        if(g_auto_allowed_authnames.file_last_changed)
            printf("%s changed. reloading\n", filename);
        ParserReadTextFile(filename,parse_AllowedAuthnames,&g_auto_allowed_authnames);

        //we need to keep them ordered for sending updates and matching with the access level list.
        eaQSort(g_auto_allowed_authnames.allowed_names, strcmp);

        g_auto_allowed_authnames.file_last_changed = tmp;
    }
    return g_auto_allowed_authnames.file_last_changed ;
}

static __time32_t s_updateAuthNames()
{
    __time32_t tmp = 0;
    char *filename = "server/db/allowed_authnames.cfg";
    tmp = fileLastChanged(filename);

    if(tmp != g_allowed_authnames.file_last_changed)
    {
        StructClear(parse_AllowedAuthnames,&g_allowed_authnames);
        if(g_allowed_authnames.file_last_changed) 
            printf("%s changed. reloading\n", filename);
        ParserReadTextFile(filename,parse_AllowedAuthnames,&g_allowed_authnames);

        //we need to keep them ordered for sending updates and matching with the access level list.
        eaQSort(g_allowed_authnames.allowed_names, strcmp);

        g_allowed_authnames.file_last_changed = tmp;
    }
    return g_allowed_authnames.file_last_changed ;
}

__time32_t getAllowedAuthNames(char ***names)
{
    __time32_t ret;

    if(!names)
        return 0 ;
    ret = s_updateAuthNames();
    *names = g_allowed_authnames.allowed_names;
    return ret;
}

void addSingleAutoPass(char *authname)
{
    int key;

    if (!authname)
        return;

    key = hashString(authname, false);
    if (!key)
        key = 1; // hack, little chance of a zero-one hash collide

    if (!g_auto_allowed_once_authnames.allowed_stash)
        g_auto_allowed_once_authnames.allowed_stash=stashTableCreateInt(128);

    stashIntAddInt(g_auto_allowed_once_authnames.allowed_stash,key,1,0);
}

bool authnameAutoLogin(char *authname, int removeSingle)
{
    static __time32_t lastUpdate = 0;

    if(!authname)
        return FALSE;

    if(lastUpdate != s_updateAuthAllowNames())
    {
        int i;
        char *authname = NULL;
        lastUpdate = g_auto_allowed_authnames.file_last_changed;
        if(!g_auto_allowed_authnames.allowed_stash)
            g_auto_allowed_authnames.allowed_stash=stashTableCreateWithStringKeys(128, StashDefault);
        else
            stashTableClear(g_auto_allowed_authnames.allowed_stash);
        for( i = eaSize(&g_auto_allowed_authnames.allowed_names) - 1; i >= 0; --i)
        {
            char *name = g_auto_allowed_authnames.allowed_names[i];
            stashAddInt(g_auto_allowed_authnames.allowed_stash,name,i,true);
        }
    }

    if (g_auto_allowed_once_authnames.allowed_stash)
    {
        int returnMe = false;
        int hash = hashString(authname, false);
        if (!hash)
            hash = 1; // hack, little chance of a zero-one hash collide

        if (removeSingle)
        {
            returnMe = stashIntRemoveInt(g_auto_allowed_once_authnames.allowed_stash, hash, NULL);
        }
        else
        {
            returnMe = stashIntFindInt(g_auto_allowed_once_authnames.allowed_stash, hash, NULL);
        }

        if (returnMe)
            return returnMe;
    }

    return g_auto_allowed_authnames.allowed_stash && stashFindInt(g_auto_allowed_authnames.allowed_stash, authname, NULL);
}

bool authnameAllowedLogin(char *authname, U32 ent_access_level)
{
    static __time32_t lastUpdate = 0;
    
    if(!server_cfg.authname_limiter || !authname)
        return TRUE;

    if(server_cfg.authname_limiter_access_level < ent_access_level)
        return TRUE;

    //check if we're in the auto-allow list.  You should only have to be in one list or the other, not both.
    if(authnameAutoLogin(authname, 0))
        return TRUE;

    if(lastUpdate != s_updateAuthNames())
    {
        int i;
        char *authname = NULL;
        lastUpdate = g_allowed_authnames.file_last_changed;
        if(!g_allowed_authnames.allowed_stash)
            g_allowed_authnames.allowed_stash=stashTableCreateWithStringKeys(128, StashDefault);
        else
            stashTableClear(g_allowed_authnames.allowed_stash);
        for( i = eaSize(&g_allowed_authnames.allowed_names) - 1; i >= 0; --i)
        {
            char *name = g_allowed_authnames.allowed_names[i];
            stashAddInt(g_allowed_authnames.allowed_stash,name,i,true);
        }
    }

    return stashFindInt(g_allowed_authnames.allowed_stash,authname,NULL);
}

void handlePlayerContainerUpdated(int container_id)
{
    EntCon *ent_con = containerPtr(ent_list, container_id);

    // Container was unloaded before SQL finished
    if (!ent_con)
        return;

    accountMarkSaved(ent_con->auth_id, ent_con->completedOrders);
}

void handlePlayerContainerLoaded(NetLink *link,int container_id, GameClientLink *client)
{
    int                ret;
    MapCon            *map_con;
    int                ent_id; // save ent_id so we can look up the EntCon later to see if it was unloaded
    static char        msg[1000];
    char            errmsg[128]={0};
    EntCon            *ent_con;

    ent_con = containerLoad(ent_list,container_id);
    
    if(!client && ent_con)
        linkFromAuthName(ent_con->account, &client);

    if (!ent_con || !devassertmsg(client, "Client with authname %s unable to be found in lookup.", ent_con->account))
    {
        sprintf(msg,"CantFindPlayer %d",client?client->char_id:-1);
        LOG( LOG_ERROR, LOG_LEVEL_ALERT, 0, "%s account \"%s\" error: %s\n",client?linkIpStr(client->link):"[no client]",client?client->account_name:"[no client]", msg);
        sendMsg(link,msg,client);
        return;
    }

    updateAuthUserData(client,ent_con);

    if (ent_con->banned)
    {
        LOG_OLD_ERR( "%s account \"%s\" attempted login of banned character (%d)\n",linkIpStr(client->link),client->account_name, ent_con->id);
        sprintf(msg,"CharacterBlocked \"%s\"", ent_con->ent_name);
        dbForceLogout(ent_con->id,-1);
        sendMsg(link,msg, client);
        return;
    }

    if (ent_con->acc_svr_lock[0])
    {
        LOG_OLD_ERR("%s account \"%s\" attempted login of accountserver-locked character (%d)\n",linkIpStr(client->link),client->account_name, ent_con->id);
        sprintf(msg,"CharacterAccLocked \"%s\"", ent_con->ent_name);
        dbForceLogout(ent_con->id,-1);
        sendMsg(link,msg, client);
        return;
    }

    // Repair characters broken from their use of dbquery, which does not set the authid
    if (ent_con->auth_id != client->auth_id)
    {
        char fix_auth_buf[100] = "";
        sprintf(fix_auth_buf, "AuthId %d\n", client->auth_id);

        // Required because "AuthId" has the CMD_SETIFNULL flag
        ent_con->auth_id = 0;
        containerUpdate(ent_list, container_id, fix_auth_buf, 1);

        devassert(ent_con->auth_id == client->auth_id);
    }

    if(server_cfg.queue_server && client->nonQueueIP)
        ent_con->client_ip = client->nonQueueIP;
    else
        ent_con->client_ip = client->link->addr.sin_addr.S_un.S_addr;
    if (ent_con->active || ent_con->logging_in)
    {
        LOG_OLD( "%s account \"%s\" force logging out previous entity? (%d)\n",linkIpStr(client->link),client->account_name, ent_con->id);
        sprintf(msg,"AccountLogging %s \"%s\"", ent_con->account, ent_con->ent_name);
        dbForceLogout(ent_con->id,-1);
        sendMsg(link,msg, client);
        return;
    }

    if(!authnameAllowedLogin(client->account_name, ent_con->access_level))    //if the queue server is up, this check happens twice.  This is probably a good safeguard.
    {
        char *denied_msg = g_allowed_authnames.denied_msg;
        LOG_OLD_ERR("%s account \"%s\" authname not allowed (%d)\n",linkIpStr(client->link),client->account_name, ent_con->id);
        sprintf(msg,"AccountNotAllowed %s \"%s\"", ent_con->account, denied_msg?denied_msg:"");
        dbForceLogout(ent_con->id,-1);
        sendMsg(link,msg, client);
        return;        
    }

    ent_con->map_id = findBestMapByBaseId(ent_con, ent_con->static_map_id);

    if (client->local_map_ip)
    {
        map_con = containerFindByIp(dbListPtr(CONTAINER_MAPS),client->local_map_ip);
        if (!map_con)
        {
            sprintf(msg,"CantFindLocalMapServer %s",makeHostNameStr(client->local_map_ip));
            LOG_OLD_ERR("%s account \"%s\" error: %s\n",linkIpStr(client->link),client->account_name, msg);
            dbForceLogout(ent_con->id,-1);
            sendMsg(link,msg, client);
            return;
        }
        if (map_con)
        {
            ent_con->map_id = map_con->id;
        }
    }
    client->container_id = ent_con->id;
    if (ent_con->callback_link && ent_con->is_gameclient)
    {
        static char    *temp_err_msg = "DuplicateLogin";
        int the_same = link==ent_con->callback_link;

        assert(((NetLinkList*)link->parentNetLinkList)->links); // Essentially to assert that parentNetLinkList is a valid pointer

        LOG_OLD_ERR("%s account \"%s\" error: %s\n",linkIpStr(client->link),client->account_name, temp_err_msg);
        sendMsg(link,temp_err_msg, client);
        lnkBatchSend(link);
        if(!server_cfg.queue_server)
        {
            netRemoveLink(link);
        }
        else if(client->account_name || client->auth_id)
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        if (!the_same) 
        {
            LOG_OLD("%s account \"%s\" kicking old link, (%s)\n",linkIpStr(client->link),client->account_name, temp_err_msg);
            sendMsg(ent_con->callback_link,temp_err_msg, client);
            lnkBatchSend(ent_con->callback_link);
            netRemoveLink(ent_con->callback_link);
        }
        return;
    }
    ent_con->callback_link = 0;
    // setNewMap can take a *long* time to complete, send an idle packet while we're waiting, so that they get an Ack back
    netSendIdle(link);
    ent_id = ent_con->id; // save ent_id so we can look up the EntCon later to see if it was unloaded
    // Add to waiting entities list (in setNewMap)
    ret = setNewMap(ent_con,ent_con->map_id,link,1,errmsg,client->char_id != 0);
    if (!ret) {
        // handle immediate failure
        doneWaitingChoosePlayerFailed(ent_con, errmsg);
    }
    else
    {
        stashIntAddPointer(ent_list->other_hashes, ent_con->auth_id, ent_con, true);
    }
}

ShardAccountCon *createShardAccountContainer(U32 auth_id, const char *auth_name)
{
    ShardAccountCon *shardaccount_con = containerAlloc(shardaccounts_list, auth_id);
    char buf[100];
    sprintf(buf, "AuthName \"%s\"\nWasVIP 1\n", auth_name);
    containerUpdate(shardaccounts_list, auth_id, buf, 1);
    return shardaccount_con;
}

void handleShardAccountContainerLoaded(GameClientLink *client, Packet *pak, DbContainer *container)
{
    ShardAccountCon *shardaccount_con = (ShardAccountCon *)container;
    if (!shardaccount_con)
    {
        shardaccount_con = createShardAccountContainer(client->auth_id, client->account_name);
    }
    if (shardaccount_con)
    {
        shardaccount_con->logged_in = 1;

        // sometimes there are old account records where the account name is not set correctly
        if (stricmp(shardaccount_con->account, client->account_name) != 0)
        {
            char buf[100];
            sprintf(buf, "AuthName \"%s\"\n", client->account_name);
            containerUpdate(shardaccounts_list, client->auth_id, buf, 1);
        }

        stashAddPointer(shardaccounts_list->other_hashes,shardaccount_con->account,shardaccount_con,true);

        updateVipStuff_getEnts_sendPlayers(client, shardaccount_con);

        // send cached version to client
        accountInventoryRelayToClient(shardaccount_con->id, 0, 0, 0, 0, 0, false);
    }
}

void doneWaitingChoosePlayerFailed(EntCon *ent_con, char *errmsg)
{
    NetLink *link = ent_con?ent_con->callback_link:NULL;
    GameClientLink *game = NULL;

    // This code needs to be ran on an entity after its map has started if it got into setNewMap from handleChoosePlayer
    if (containerPtr(dbListPtr(CONTAINER_ENTS),ent_con->id)!=ent_con)
    {
        assert(0); // This shouldn't happen here anymore?
        return;
    }
    LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "%s dbid %d name \"%s\" setNewMap failed %s",linkIpStr(link), ent_con->id, ent_con->ent_name, errmsg);
    assert(ent_con->is_gameclient); // Make sure this link is pointing to a client not a mapserver

    // Get the game client link before we clear out the ent_con
    if (link)
    {
        game = dbClient_getGameClientByName(ent_con->account, link);
    }
    
    // if we created a new ent container before this error, cleanup
    if (ent_con->just_created) {
        ent_con->callback_link = NULL;
        ent_con->is_gameclient = 0;
        playerNameDelete(ent_con->ent_name, ent_con->id);
        containerDelete(ent_con);
    } else {
        ent_con->callback_link = NULL;
        ent_con->is_gameclient = 0;
        containerUnload(dbListPtr(ent_con->type),ent_con->id);
    }

    if(game)
        sendMsg(link,errmsg,game);
}

void doneWaitingChoosePlayerSucceded(EntCon *ent_con)
{
    NetLink *link = ent_con->callback_link;

    // At this point our entity has just been sent to a map, and we will notify the client as soon as the cookie is received
    assert(ent_con->is_gameclient);
    LOG_OLD( "%s resuming \"%s\" (%d)\n", linkIpStr(link), ent_con->ent_name, ent_con->id);
    
    ent_con->logging_in = 1;
    ent_con->connected = 1;
    playerNameLogin(ent_con->ent_name, ent_con->id, ent_con->just_created, ent_con->auth_id);
}

bool deletePlayer(int db_id, bool log, char *player_name, char *ipstr, char *accountname)
{
    EntCon    *container;
    int tsLeaderId = 0;
    container = containerLoad(dbListPtr(CONTAINER_ENTS),db_id);
    if (!container)
    {
        LOG_OLD_ERR("deletePlayer not able to find ent %d",db_id);
        return true;
    }

    if (player_name) // this is coming from a client
    {
        if (stricmp(container->ent_name, escapeString(player_name)) != 0 &&
            stricmp(container->ent_name, player_name) != 0 ) // For some reasons player names have made it into the DataBase without being escaped, let them delete them!
        {
            LOG_OLD_ERR("deletePlayer tried to delete %s(%d), but given name %s", container->ent_name, db_id, player_name);
            return false;
        }

        if (container->acc_svr_lock[0])
        {
            LOG_OLD_ERR("deletePlayer tried to delete %s(%d), but it has an AccountServer lock (%s)",container->ent_name,db_id,container->acc_svr_lock);
            return false;
        }
    }
    // else this is an internal call

    // log deletion:
    if (log) // Don't log internal deletes (player created character with invalid name)
    {
        extern void logPutMsgSub(char *msg_str,int len,char *filename,int zip_file,int zipped,int text_file, char *delim);

        char datestr[100], *logbuf, *chardata;
        int logbuflen;
        char *eff_player_name=player_name;

        timerMakeDateString(datestr);
        chardata = strdup(escapeString(containerGetText(container)));

        if (!chardata)
            chardata = "";
        if (!accountname)
            accountname = "";
        if (!eff_player_name)
            eff_player_name = "";
        if (!ipstr)
            ipstr = "0.0.0.0";

        // date, ip, account, character name, character data
        logbuflen = (int)(5 + strlen(datestr) + strlen(ipstr) + strlen(accountname) + strlen(eff_player_name) + strlen(chardata));
        logbuf = malloc(logbuflen+1);
        sprintf(logbuf, "%s\t%s\t%s\t%s\t%s\n", datestr, ipstr?ipstr:"0.0.0.0", accountname?accountname:"", eff_player_name, chardata);
        logPutMsgSub(logbuf, logbuflen+1, "deletion.log", 1, 0, 0, NULL);
        free(logbuf);
        free(chardata);
    }

    if (container->league_id)
    {
        GroupCon *group = containerPtr(dbListPtr(CONTAINER_LEAGUES), container->league_id);
        tsLeaderId = group ? group->leader_id : 0;
    }
    if (!tsLeaderId && container->teamup_id)
    {
        GroupCon *group = containerPtr(dbListPtr(CONTAINER_TEAMUPS), container->teamup_id);
        tsLeaderId = group ? group->leader_id : 0;
    }
    if (!tsLeaderId)
    {
        tsLeaderId = db_id;
    }

    turnstileDBserver_removeFromQueueEx(db_id, tsLeaderId, 1);
    containerRemoveMember(dbListPtr(CONTAINER_TEAMUPS),container->teamup_id,db_id,0,CONTAINERADDREM_FLAG_NOTIFYSERVERS);
    containerRemoveMember(dbListPtr(CONTAINER_SUPERGROUPS),container->supergroup_id,db_id,0,CONTAINERADDREM_FLAG_NOTIFYSERVERS);
    containerRemoveMember(dbListPtr(CONTAINER_TASKFORCES),container->taskforce_id,db_id,0,CONTAINERADDREM_FLAG_NOTIFYSERVERS);
    containerRemoveMember(dbListPtr(CONTAINER_RAIDS),container->raid_id,db_id,0,CONTAINERADDREM_FLAG_NOTIFYSERVERS);
    containerRemoveMember(dbListPtr(CONTAINER_LEVELINGPACTS),container->levelingpact_id,db_id,0,CONTAINERADDREM_FLAG_NOTIFYSERVERS);
    if (container->league_id)
    {
        NetLink *linkStat = statLink();
        if( linkStat )
        {
            Packet* pak_out = pktCreateEx(linkStat,DBSERVER_SEND_STATSERVER_CMD);
            if( pak_out )
            {
                char cmd[512];
                _snprintf_s(cmd, sizeof(cmd), sizeof(cmd)-1, "statserver_league_quit %d 0 0", db_id);
                pktSendBitsPack(pak_out,1,db_id);
                pktSendBitsPack(pak_out,1, 0);
                pktSendString(pak_out, cmd);

                pktSend(&pak_out, linkStat);
            }
        }
    }
    containerRemoveMember(dbListPtr(CONTAINER_LEAGUES), container->league_id, db_id, 0,CONTAINERADDREM_FLAG_NOTIFYSERVERS);

    playerNameDelete(container->ent_name, db_id);
    containerDelete(container);

    return true;
}

static void handleDeletePlayer(Packet *pak,GameClientLink *client)
{
    int        slot_id;
    char    *player_name;
    Packet    *pak_out;

    slot_id            = pktGetBitsPack(pak,1);
    player_name        = (char*)unescapeString(pktGetString(pak));
    if (slot_id < 0 || slot_id >= MAX_PLAYER_SLOTS )
    {
        if(!server_cfg.queue_server)
        {
            netRemoveLink(client->link);
        }
        else if(client->account_name || client->auth_id)
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        return;
    }
    LOG( LOG_DELETE, LOG_LEVEL_IMPORTANT, 0, "%s account \"%s\" deleted \"%s\" (%d)\n",linkIpStr(client->link),client->account_name,player_name, client->players[slot_id]);
    dbForceLogout(client->players[slot_id],-1);
    offlinePlayerDelete(client->auth_id,client->account_name,client->players[slot_id]);
    pak_out = dbClient_createPacket(DBGAMESERVER_DELETE_OK, client);
    if(deletePlayer(client->players[slot_id],true,player_name,linkIpStr(client->link),client->account_name))
    {
        client->players[slot_id] = 0;
        pktSendBits(pak_out,1,1);
    }
    else
    {
        pktSendBits(pak_out,1,0);
    }
    pktSend(&pak_out,client->link);
}


static void handleGetCostume(Packet *pak, GameClientLink *client)
{
    int slot_idx;

    slot_idx = pktGetBitsPack(pak, 1);
    sendCurrentCostume(client, slot_idx);
}

static void handleGetPowersetNames(Packet *pak, GameClientLink *client)
{
    int slot_idx;

    slot_idx = pktGetBitsPack(pak, 1);
    sendCurrentPowersetNames(client, slot_idx);
}


static void handleGetCharacterCounts(Packet *pak, GameClientLink *client)
{
    if(client)
    {
        devassert(client->vipFlagReady);
        account_handleGameClientCharacterCounts(client->auth_id, client->vip);
    }
}

static void handleGetProductCatalog(Packet *pak, GameClientLink *client)
{
    if(client)
        account_handleProductCatalog(client);
}

static void handleAccountLoyaltyBuy(Packet *pak, GameClientLink *client)
{
    char *auth_name;
    int node_index;

    strdup_alloca(auth_name, pktGetString(pak));
    node_index = pktGetBitsAuto(pak);

    account_handleLoyaltyChange(client->auth_id, node_index, 1);
}

static void handleAccountLoyaltyRefund(Packet *pak, GameClientLink *client)
{
    char *auth_name;
    int node_index;

    strdup_alloca(auth_name, pktGetString(pak));
    node_index = pktGetBitsAuto(pak);

    account_handleLoyaltyChange(client->auth_id, node_index, 0);
}

static void handleShardXferTokenRedeem(Packet *pak, GameClientLink *client)
{
    int auth_id = pktGetBitsAuto(pak);
    char *client_name = strdup(pktGetString(pak));
    int slot_idx = pktGetBitsAuto(pak);
    char *destshard = strdup(pktGetString(pak));
    if (client)
    {
        char *db_name = pnameFindById(client->players[slot_idx]);
        if (client_name && db_name && stricmp(client_name, db_name) == 0)
        {
            devassert(client->vipFlagReady);
            account_handleShardXferTokenClaim(auth_id, client->players[slot_idx], destshard, client->vip);
        }
    }
    free(client_name);
    free(destshard);
}

static void handleRenameCharacter(Packet *pak, GameClientLink *client)
{
    char *old_name;
    char *new_name;
    char *msg = NULL;
    U32 ent_id;
    int count = 0;
    bool found = false;

    old_name = strdup(pktGetString(pak));
    new_name = pktGetString(pak);

    ent_id = pnameFindByName(old_name);

    //    how did you get an ent_id of 0? you're old name.. aka, your current name, isn't in the name cache??? 
    //    how did you select your character to rename??
    if (ent_id > 0)
    {
        estrPrintf(&msg, "cmdrelay_dbid %d\nplayerrename_paid %s %d \"%s\" \"%s\"\n",
            ent_id, client->account_name, ent_id, unescapeString(old_name),
            new_name);

        while (!found && count < MAX_PLAYER_SLOTS)
        {
            if (client->players[count] == ent_id)
            {
                found = true;
            }
            else
            {
                count++;
            }
        }
    }

    if (found)
    {
        sendToEnt(NULL, ent_id, TRUE, msg);
    }
    else
    {
        Packet *pak;

        pak = dbClient_createPacket(DBGAMESERVER_RENAME_RESPONSE, client);
        pktSendString(pak, "PaidRenameInvalidCharacter");
        pktSend(&pak, client->link);
    }

    estrDestroy(&msg);
    free(old_name);
}

static void handleRenameTokenRedeem(Packet *pak, GameClientLink *client)
{
    U32 auth_id = pktGetBitsAuto(pak);
    char *client_name = strdup(pktGetString(pak));
    int slot_idx = pktGetBitsAuto(pak);
    if (client)
    {
        char *db_name = pnameFindById(client->players[slot_idx]);
        if (client_name && db_name && stricmp(client_name, db_name) == 0)
        {
            account_handleGenericClaim(auth_id, kAccountRequestType_Rename, client->players[slot_idx]);
        }
    }
    free(client_name);
}

static void handleCheckName(Packet *pak, GameClientLink *client)
{
    char *name;
    U32 ent_id;
    int tempLockName = 0;
    int locked = 0;

    name = pktGetString(pak);
    tempLockName = pktGetBitsAuto(pak);
    ent_id = client->players[0];

    
    if (pnameFindByName(name) > 0 || (locked = tempLockFindbyName(name)))
    {
        Packet *pak_out;
        pak_out = dbClient_createPacket(DBGAMESERVER_CHECK_NAME_RESPONSE, client);

        if (locked && tempLockCheckAccountandRefresh(name, client->auth_id, timerSecondsSince2000()))
        {
            pktSendBitsAuto(pak_out, 1); //CheckNameSuccess
            pktSendBitsAuto(pak_out, 1); //name locked
        }
        else
        {
            pktSendBitsAuto(pak_out, 0); //CheckNameFail
            pktSendBitsAuto(pak_out, 0); //name locked
        }
        pktSend(&pak_out, client->link);
    }
    else if (ent_id)
    {
        char *msg = NULL;
        estrPrintf(&msg, "cmdrelay_dbid %d\ncheck_player_name %d %d \"%s\"\n",
            ent_id, client->auth_id, tempLockName, unescapeString(name));

        sendToEnt(NULL, ent_id, true, msg);

        estrDestroy(&msg);
    }
    else
    {
        int i, pktWasSent = 0;
        static int lastUsedMap = 0;            //    remember the last map used, and try not to the next one for load balancing purposed

        for(i=0;i<map_list->num_alloced;i++)
        {
            MapCon *map = (MapCon *) map_list->containers[(i+lastUsedMap)%map_list->num_alloced];
            if( map->is_static && map->active)
            {
                char *msg = NULL;
                Packet *pak_out = pktCreateEx(map->link,DBSERVER_RELAY_CMD);

                lastUsedMap = i;
                estrPrintf(&msg, "cmdrelay_db_no_ent\ncheck_player_name %d %d \"%s\"\n",
                    client->auth_id, tempLockName, unescapeString(name));            
                pktSendString(pak_out,msg);
                pktSend(&pak_out,map->link);
                estrDestroy(&msg);
                pktWasSent = 1;
                break;
            }
        }
        
        if (!pktWasSent)
        {
            Packet *pak_out;
            pak_out = dbClient_createPacket(DBGAMESERVER_CHECK_NAME_RESPONSE, client);
            pktSendBitsAuto(pak_out, 0); //CheckNameFail
            pktSendBitsAuto(pak_out, 0); //name locked
            pktSend(&pak_out, client->link);
        }
    }
}

static void handleSlotTokenRedeem(Packet *pak, GameClientLink *client)
{
    if(client)
        account_handleSlotTokenClaim(makeIpStr(client->link->addr.sin_addr.S_un.S_addr), client->account_name, client->auth_id);
}

typedef struct UnlockCharacterCbData 
{
    GameClientLink *client;
    U32 ent_id;
} UnlockCharacterCbData;

static void s_finishHandleUnlockCharacter(Packet *pak, U8 *cols, int col_count, ColumnInfo **field_ptrs, void *data)
{
    UnlockCharacterCbData *cbData = data;
    GameClientLink *client = cbData->client;
    int unlocked_character_count = col_count;
    int owned_slots;
    char *msg = NULL;

    owned_slots = client->vip ? getNumPlayerSlotsVIP() : 0;
    owned_slots += accountInventoryFindBonusCharacterSlots(client->auth_id);

    if (unlocked_character_count < owned_slots)
    {
        estrPrintf(&msg, "cmdrelay_dbid %d\nunlock_character", cbData->ent_id);
        sendToEnt(NULL, cbData->ent_id, TRUE, msg);
    }
    else
    {
        LOG_OLD_ERR( "%s account \"%s\" tried to create more characters than they have slots for.\n",linkIpStr(client->link),client->account_name);
        sendMsg(client->link, "NotEnoughSlots", client);
    }

    estrDestroy(&msg);
    free(cbData);
}

static void handleUnlockCharacter(Packet *pak, GameClientLink *client)
{
    char *name;
    U32 ent_id;
    int count = 0;
    bool found = false;

    name = pktGetString(pak);
    ent_id = pnameFindByName(name);

    //    how did you get an ent_id of 0? you're old name.. aka, your current name, isn't in the name cache??? 
    //    how did you select your character to unlock??
    if (ent_id > 0)
    {
        while (!found && count < MAX_PLAYER_SLOTS)
        {
            if (client->players[count] == ent_id)
            {
                found = true;
            }
            else
            {
                count++;
            }
        }
    }

    if (found)
    {
        UnlockCharacterCbData *cbData;
        char buf[100];

        sqlFifoBarrier(); // make sure IsSlotLocked is fully up to date

        cbData = calloc(1, sizeof(UnlockCharacterCbData));
        cbData->client = client;
        cbData->ent_id = ent_id;
        sprintf(buf, "WHERE AuthName='%s' AND IsSlotLocked IS NULL", client->account_name); // can't use AuthId in dev environment
        sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",buf,s_finishHandleUnlockCharacter,cbData,NULL,0);
    }
}

int clientcomm_quitClient(NetLink *link, GameClientLink *client)
{
    EntCon            *ent;

    if (!client->container_id && client->auth_id)
    {
        authSendQuitGame(client->auth_id,1,0);
    }
    else
    {
        ent = containerPtr(dbListPtr(CONTAINER_ENTS),client->container_id);
        if (!ent)
        {
            authSendQuitGame(client->auth_id,2,0);
        }
        else if (ent->callback_link == link)
        {
            if (ent->handed_to_mapserver) 
            {
                int id = ent->id;
                dbForceLogout(ent->id,-1);
                ent = containerPtr(dbListPtr(CONTAINER_ENTS),id);
                if (ent) 
                { // Sometimes gets unloaded
                    ent->callback_link = NULL;
                }
            } 
            else 
            {
                sendPlayerLogout(ent->id,3,false);
            }
        }
    }

    return 1;
}

static void handleSignNDA(Packet *pak, GameClientLink *client)
{
    U32 subtype   = pktGetBitsAuto(pak);
    U32 newValue  = pktGetBitsAuto(pak);

    if(!client)
        return;

    if (subtype == 1)
        account_handleAccountSetInventory(client->auth_id, SKU("nda_beta"), newValue, 1);
    else
        account_handleAccountSetInventory(client->auth_id, SKU("nda_test"), newValue, 1);
}

static void handleGetPlayNCAuthKey(Packet *pak, GameClientLink *client)
{
    int request_key;
    char* auth_name;
    char* digest_data;
    
    request_key = pktGetBitsAuto(pak);
    strdup_alloca(auth_name,pktGetString(pak)); 
    strdup_alloca(digest_data,pktGetString(pak)); 

    if (!client)
    {
        return;
    }
    account_handleGetPlayNCAuthKey( client->auth_id, request_key, auth_name, digest_data );
}

int processGameClientMsg(Packet *pak,int cmd, NetLink *link)
{
    GameClientLink *client = 0;
    client = clientcomm_getGameClient(link, pak);
    if(!devassert(client || (cmd == DBGAMECLIENT_LOGIN && server_cfg.queue_server)))
        return 0;

    if (dbIsShuttingDown()) {
        if(client)
            sendMsg(link,"ServerShutdown", client);
        lnkBatchSend(link);
        if(!server_cfg.queue_server)
        {
            netRemoveLink(link);
        }
        else if(client && (client->account_name || client->linkId))
        {
            sendClientQuit(client->account_name, client->linkId);
        }
        return 0;
    }

    if (cmd == DBGAMECLIENT_LOGIN)
    {
        if(server_cfg.queue_server)
        {
            //the client may have been filled out above as we do a lookup by auth name, but if this is someone
            //else connecting with the same auth, we want to use a new game client link
            //we destroy the old one if this succeeds at login, we destroy the new one if it fails.
            client = queueservercomm_getLoginClient(pak);
            if(!client)
                return 0;
        }
        if(!handleLogin(pak,client) && server_cfg.queue_server)
        {
            sendClientQuit(client->account_name, client->linkId);
            //destroy the gameclientlink since its login was unsuccessful
            if(server_cfg.queue_server)
                queueservercomm_gameclientDestroy(client);
        }
        else if(server_cfg.queue_server)
        {
            Packet *pak =0;
            //hash the client so that we can refer back to it and destroy it later.
            setGameClientOnQueueServer(client);
            //tell the queue server what the enqueuing status is.
            pak = dbClient_createPacket(QUEUESERVER_SVR_ENQUEUESTATUS, client);
            pktSendBitsAuto(pak, client->loginStatus);
            pktSendBitsAuto(pak, AccountIsVIPByPayStat(client->payStat));
            pktSendBitsAuto(pak, client->loyalty);
            pktSend(&pak,client->link);
        }
    }
    else
    {
        if (!client->valid)
        {
            sendMsg(link,"NotLogged", client);
            lnkBatchSend(link);
            if(!server_cfg.queue_server)
            {
                netRemoveLink(link);
            }
            else if(client->account_name || client->auth_id)
            {
                sendClientQuit(client->account_name, client->linkId);
            }
            return 0;
        }
        switch ( cmd )
        {
        case DBGAMECLIENT_CHOOSE_PLAYER:
            handleChoosePlayer(pak,client);
            break;
        case DBGAMECLIENT_MAKE_PLAYER_ONLINE:
            handleMakePlayerOnline(pak,client);
            break;
        case DBGAMECLIENT_CAN_START_STATIC_MAP:
            handleCanStartStaticMap(pak,client);
            break;
        case DBGAMECLIENT_CHOOSE_VISITING_PLAYER:
            handleChooseVisitingPlayer(pak,client);
            break;
        case DBGAMECLIENT_DELETE_PLAYER:
            handleDeletePlayer(pak,client);
            break;
        case DBGAMECLIENT_GET_COSTUME:
            handleGetCostume(pak,client);
            break;
        case DBGAMECLIENT_GET_POWERSET_NAMES:
            handleGetPowersetNames(pak,client);
            break;
        case DBGAMECLIENT_ACCOUNTSERVER_UNSECURE_CMD:
            // careful! these commands should be considered accesslevel 0!
            // @todo -AB: implement :05/09/07
            break;
        case DBGAMECLIENT_ACCOUNTSERVER_CHARCOUNT:
            handleGetCharacterCounts(pak,client);
            break;
        case DBGAMECLIENT_ACCOUNTSERVER_CATALOG:
            handleGetProductCatalog(pak,client);
            break;
        case DBGAMECLIENT_ACCOUNTSERVER_LOYALTY_BUY:
            handleAccountLoyaltyBuy(pak,client);
            break;
        case DBGAMECLIENT_ACCOUNTSERVER_LOYALTY_REFUND:
            handleAccountLoyaltyRefund(pak,client);
            break;
        case DBGAMECLIENT_SHARD_XFER_TOKEN_REDEEM:
            handleShardXferTokenRedeem(pak,client);
            break;
        case DBGAMECLIENT_RENAME_CHARACTER:
            handleRenameCharacter(pak,client);
            break;
        case DBGAMECLIENT_RENAME_TOKEN_REDEEM:
            handleRenameTokenRedeem(pak,client);
            break;
        case DBGAMECLIENT_RESEND_PLAYERS:
            resendPlayers(client, client->auth_id);
            break;
        case DBGAMECLIENT_CHECK_NAME:
            handleCheckName(pak,client);
            break;
        case DBGAMECLIENT_SLOT_TOKEN_REDEEM:
            handleSlotTokenRedeem(pak, client);
            break;
        case DBGAMECLIENT_UNLOCK_CHARACTER:
            handleUnlockCharacter(pak, client);
            break;
        case DBCLIENT_ACCOUNTSERVER_CMD: // Testing only! this could be anybody.
            // TODO: replace with access level check later: if(isDevelopmentMode())
            handleSendAccountCmd(pak,link);
            break;
        case DBGAMECLIENT_QUITCLIENT:
            clientcomm_quitClient(link, client);
            break;
        case DBGAMECLIENT_SIGN_NDA:
            handleSignNDA(pak, client);
            break;
        case DBGAMECLIENT_GET_PLAYNC_AUTH_KEY:
            handleGetPlayNCAuthKey(pak, client);
            break;
        default:
            LOG_OLD_ERR("%s unknown command %d from client\n",linkIpStr(link),cmd);
            break;
        };
    }
    return 0;
}

int clientConnectedCount()
{
    if(server_cfg.queue_server)
        return queueservercomm_getNConnected();
    assert(!server_cfg.queue_server);
    return gameclient_links.links->size;
}

int clientLoginCount()
{
    if(server_cfg.queue_server)
        return queueservercomm_getNLoggingIn();
    assert(!server_cfg.queue_server);
    return gameclient_links.links->size;
}

int clientQueuedCount()
{
    if(server_cfg.queue_server)
        return queueservercomm_getNWaiting();
    assert(!server_cfg.queue_server);
    return 0;
}

void clientClearDeadLinks()
{
    assert(!server_cfg.queue_server);
    clientCommLoginClearDeadLinks(gameclient_links.links);
}

void reconnectClientCommPlayersToAuth()
{
    int                i;
    GameClientLink    *client;
    assert(!server_cfg.queue_server);

    for(i=0;i<gameclient_links.links->size;i++)
    {
        client = ((NetLink*)gameclient_links.links->storage[i])->userData;

        if (client->valid)
            authReconnectOnePlayer(client->account_name,client->auth_id);
    }
}

NetLink *linkFromAuthId(U32 auth_id, GameClientLink **link)
{
    int        i;

    if(server_cfg.queue_server)
    {
        GameClientLink    *client = 0;
        getGameClientOnQueueServer(auth_id,&client);
        if(client)
        {
            NetLink *queueLink = queueserver_getLink();
            if(link)
                *link = client;
            return queueLink;
        }
    }
    else
    {
        for(i=0;i<gameclient_links.links->size;i++)
        {
            GameClientLink    *client = ((NetLink*)gameclient_links.links->storage[i])->userData;
            if(client && link)
                *link = client;
            if(!client)
                continue;
            if(client->auth_id == auth_id)
                return client->link;
        }
    }
    return NULL;
}

NetLink *linkFromAuthName(char *authname, GameClientLink **link)
{
    int        i;

    if(server_cfg.queue_server)
    {
        GameClientLink    *client = 0;
        getGameClientOnQueueServerByName(authname,&client);
        if(client)
        {
            NetLink *queueLink = queueserver_getLink();
            if(link)
                *link = client;
            return queueLink;
        }
    }
    else
    {
        for(i=0;i<gameclient_links.links->size;i++)
        {
            GameClientLink    *client = ((NetLink*)gameclient_links.links->storage[i])->userData;
            if(client && link)
                *link = client;
            if(!client)
                continue;
            if(0==stricmp(client->account_name,authname))
                return client->link;
        }
    }
    return NULL;
}

NetLink *findLinkToClient(U32 auth_id, bool *is_direct_dest, int *dbid_dest)
{
    NetLink *retval = NULL;

    devassert(auth_id);
    devassert(is_direct_dest);
    devassert(dbid_dest);

    if (is_direct_dest)
    {
        *is_direct_dest = false;
    }

    if (dbid_dest)
    {
        *dbid_dest = 0;
    }

    if (auth_id)
    {
        retval = linkFromAuthId(auth_id, 0);

        if (retval)
        {
            if (is_direct_dest)
            {
                *is_direct_dest = true;
            }
        }
        else
        {
            EntCon *ent_con = containerFindByAuthId(auth_id);

            if (ent_con)
            {
                retval = dbLinkFromLockId(ent_con->lock_id);

                if (dbid_dest)
                {
                    *dbid_dest = ent_con->id;
                }
            }
        }
    }

    return retval;
}

extern StashTable gameClientHashByName;
void clientLinkDoForAll( ClientLinkDoForAllCB cb, void* userData )
{
    //
    //    Performs the callback for all clients directly connected to the
    //    dbserver -- i.e., not in maps. 
    //
    //    To access clients in maps, ask each map to do the work, via
    //    mapLinkDoForAll().
    //

    StashTableIterator stashIter = { 0 };
    int arrayI = -1;
    bool bMore = true;
    int dbid = 0;

    if(server_cfg.queue_server)
    {
        if(gameClientHashByName)
            stashGetIterator( gameClientHashByName, &stashIter );
    }
    else
    {
        arrayI = 0;
    }
    
    while ( bMore )
    {
        GameClientLink* gameClient = NULL;
        bMore = false;
        if ( arrayI >= 0 )
        {
            // Stepping through array
            if ( arrayI < gameclient_links.links->size )
            {
                gameClient = ((NetLink*)gameclient_links.links->storage[arrayI])->userData;
                arrayI++;
                bMore = true;
            }
        }
        else if(gameClientHashByName)
        {
            // iterating through stash table
            StashElement elem;
            if ( stashGetNextElement( &stashIter, &elem ) )
            {
                gameClient = (GameClientLink*)stashElementGetPointer(elem);
                bMore = true;
            }
        }

        if ( gameClient != NULL )
        {
            NetLink* client = gameClient->link;
            if ( client != NULL )
            {
                (*cb)( client, gameClient, true, gameClient->auth_id, gameClient->container_id, userData );
            }
        }
    }
}

void mapLinkDoForAll( MapLinkDoForAllCB cb, void* userData )
{
    //
    //    Does work for each map connected to this dbserver.
    //    HINT: If you need to send an update to every client in every map
    //    your callback can send a request to the mapserver and let the
    //    mapserver do the work.
    //

    int i;
    for(i = 0; i < map_list->num_alloced; i++)
    {
        MapCon* map = (MapCon*)map_list->containers[i];
        if ( map->link )
        {
            (*cb)( map->link, userData );
        }
    }
}
