/*\
 *
 *    StatDb.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *         Confidential property of Cryptic Studios
 *
 *    Stub functions so statserver can use dbcomm, dbcontainer.
 *  Also any other statserver-specific db client functions.
 *
 */

#include "dbcomm/dbcomm.h"
#include "cmdparse/cmdstatserver.h"
#include "gameData/sgrpstatsstruct.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/file.h>
#include "dbcomm/dbcontainer.h"
#include <utilitieslib/utils/error.h>
#include "entity/entity.h"
#include "entity/entsend.h"
#include "comm_backend.h"
#include <utilitieslib/components/StashTable.h>
#include "statserver.h"
#include "entity/Supergroup.h"
#include <utilitieslib/utils/utils.h>
#include "container/containerSupergroup.h"
#include <utilitieslib/utils/textparser.h>
#include "entity/SgrpStats.h"
#include <utilitieslib/components/earray.h>
#include "Entity/stat_SgrpBadges.h"
#include "player/BadgeStats.h"
#include "entity/SgrpBadges.h"
#include <utilitieslib/utils/fileutil.h>
#include "cmdparse/cmdenum.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "gameData/miningaccumulator.h"
#include <utilitieslib/utils/timing.h>
#include "statgroups.h"
#include <utilitieslib/utils/log.h>

#define STATDB_FILENAME "StatsSgrp.def"
#define STATDB_BAK_FILENAME "StatsSgrp.bak"

typedef struct StatInfo
{
     StashTable sgstatFromIdSg;
} StatInfo;

static StatInfo stat = {0};

void statdb_Save(bool saveAllDirty)
{
    static U32 counter = 0;
#define COUNTER_MASK 511
#define ID_MATCHES_COUNTER(ID) (((ID)&COUNTER_MASK) == (counter&COUNTER_MASK))

    char **strs = NULL;
    int *ids = NULL;
    int i;
    stat_SgrpStats **statsToSend = NULL;

    StashTableIterator hi;
    StashElement he = NULL;

    // --------------------
    // figure out which stats to send

    for(stashGetIterator( stat.sgstatFromIdSg,&hi); stashGetNextElement(&hi,&he);)
    {
        stat_SgrpStats *pStat = stashElementGetPointer( he );

        if(verify(pStat))
        {
            if((saveAllDirty || ID_MATCHES_COUNTER(pStat->db_id)) && pStat->dirty)
            {
                eaPush(&statsToSend,pStat);
            }
        }
        else
        {
            int iKey = stashElementGetIntKey(he);
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": NULL pStat for idSgrp(%d)", iKey );
            stashIntRemovePointer(stat.sgstatFromIdSg, iKey, NULL);
        }
    }

    // -------------------
    // gather up the data to send

    for( i = eaSize( &statsToSend ) - 1; i >= 0; --i)
    {
        stat_SgrpStats *pStat = statsToSend[i];
        char *str;

        assert(pStat);
        str = stat_SgrpStats_Package(pStat);

        if( pStat->idSgrp > 0 && str )
        {
            eaPush(&strs, str);
            eaiPush(&ids, pStat->db_id);
        }
        else
        {
            LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": stat %d has invalid supergroup %d for container '%s'", pStat->db_id, pStat->idSgrp, str?str:"<invalid container>");
            free(str);
        }

        pStat->dirty = false;
    }

    // --------------------
    // send it

    if( strs )
    {
        dbContainerSendList( CONTAINER_SGRPSTATS, strs, ids, eaSize(&strs), CONTAINER_CMD_CREATE_MODIFY );
    }

    // --------------------
    // cleanup 

    eaDestroyEx(&strs, NULL); // uses free by default
    eaDestroy(&statsToSend);
    counter++;
}

void miningdata_Save(float duration)
{
    static int timer;
    int i;

    if(!timer)
        timer = timerAlloc();
    else
        timerStart(timer);

    if(MiningAccumulatorWeekendUpdate())
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "Performed data mining week changeover.");

    for(i = MiningAccumulatorCount(); i > 0 && (!duration || timerElapsed(timer) < duration); i--)
    {
        int dbid;
        char *name;
        char *data = MiningAccumulatorPackageNext(&name,&dbid);

        if(!data) // doesn't need persistance
            continue;

        if(dbid < 0)
        {
            dbSyncContainerCreate(CONTAINER_MININGACCUMULATOR, &dbid);
            MiningAccumulatorSetDBID(name,dbid);
        }
        dbAsyncContainerUpdate(CONTAINER_MININGACCUMULATOR, dbid, CONTAINER_CMD_CREATE_MODIFY, data, 0);

        free(data);
    }
}

// void statdb_Backup(U32 intervalBackupSecs)
// {
//     char fnbak[MAX_PATH];
//     char fnZip[MAX_PATH];
//     char *backupFilename = fileLocateWrite(STATDB_FILENAME, fnbak);
//     U32 doubling = (60*60*24)/(intervalBackupSecs > 0 ? intervalBackupSecs : 1);

//     fileGZip(backupFilename); // creates zip file
//     sprintf(fnZip,"%s.gz",backupFilename);
//     verify(fileExponentialBackup( fnZip, intervalBackupSecs, doubling ));
// }

//----------------------------------------
//  init the db variables. 
// NOTE: called pre connect to database
//----------------------------------------
void statdb_Init()
{
    g_sgrpFromId = stashTableCreateInt(128);
    stat.sgstatFromIdSg = stashTableCreateInt(128);

    // init the command handler
    cmdOldInit(client_sgstat_cmds);
}

U32 stat_dealWithContainer(ContainerInfo *ci,int type)
{
    U32 res = 0;
    
    if( !verify(g_sgrpFromId))
    {
        // must call init before this
        return 0;
    }

    if (type == CONTAINER_SUPERGROUPS)
    {
        int idSgrp = ci->id;
        Supergroup *sg;
        if ( !stashIntFindPointer( g_sgrpFromId, idSgrp, &sg))
            sg = NULL;
        
        if( idSgrp <= 0 )
        {
            // bad supergroup, what happened here?
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "invalid supergroup id '%d' for container '%s'", idSgrp, ci->data);
        }
        if( ci->delete_me )
        {
            // deletes are handled in dealWithNotify
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " got delete for sgrp %d even though not listening to deletes for it", idSgrp);
        }
        else
        {
            // add supergroup ids
            stat_SgrpStats *pStats = NULL;
            Entity tmpEnt = {0};
            char *container_data = strdup(ci->data);
            bool send_to_client = !ci->got_lock;
            
            // --------------------
            // create or clear the supergroup

            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " updating sgrp %d", idSgrp);

            // a new supergroup not in the list
            if(!sg)
            {
                sg = createSupergroup();
                stashIntAddPointer(g_sgrpFromId, idSgrp, sg, false);
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " sgrp %d not in hash, adding", idSgrp);
            }

            tmpEnt.supergroup = sg;

            // --------------------
            // unpack

            // clear this before unpacking, otherwise it gets deleted.
            // classic memory ownership problem. it belongs to the hashtable
            sg->pBadgeStats = NULL;

            unpackSupergroup( &tmpEnt, container_data, send_to_client );

            // --------------------
            // give a BadgeStats (statserver only)

            // ----------
            // lookup the stats

            // try the hash
                
            // if not in the hash:
            // - create it locally
            // - init it
            // - send it to the db so it creates an entry, with the id as the callback.
            // - add it to the hash by sgrp id
            if( !stashIntFindPointer(stat.sgstatFromIdSg, idSgrp, &pStats) )
            {
                static PerformanceInfo* perfInfo;
                int cookie = 0;                
                char *str;

                pStats = stat_SgrpStats_Create( 0, idSgrp, sg->name );
                cookie = dbSetDataCallback(&pStats->db_id);
                str = stat_SgrpStats_Package(pStats);
                
                dbAsyncContainerUpdate(CONTAINER_SGRPSTATS, -1, CONTAINER_CMD_CREATE, str, cookie );
                dbMessageScanUntil(__FUNCTION__, &perfInfo);
                stashIntAddPointer(stat.sgstatFromIdSg, idSgrp, pStats, false);
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "created stats container '%d' for idSgrp '%d'", pStats->db_id, idSgrp );

                free(str);
            }
            else
            {
                // just for debugging, if the supergroup name didn't make it into
                // the stat (created without a name and then locked to change it, for example)
                // set it here
                if( !*pStats->name )
                {
                    Strncpyt( pStats->name, sg->name );
                }
            }
                
            if( verify(pStats) )
            {
                sg->pBadgeStats = &pStats->badgeStats;
            }
            else
            {
                dbLog("statdb_errors", NULL, "couldn't create sgrpstats row");
            }

            
            // --------------------
            // give the badge states (statserver only)

            sg->badgeStates.eaiStates =  sgrpbadge_eaiStatesFromId(idSgrp);

            // --------------------
            // trigger any *sgrp affected badges

            // don't do it if we're locked
            // the reason is that the lock the sgrpbadges_Tick puts on a supergroup
            // results in a call to this while it is being updated.
            // ab: should really just make this call on the unlock anyway.
            if( !sgrpbadges_Locked() )
            {
                supergroup_MarkModifiedSgrpBadges( sg, "*sgrp" );
            }

            // --------------------
            // finally

            free(container_data);
        }
        res = true;
    }
    else if( type == CONTAINER_SGRPSTATS )
    {
        char *container_data = strdup(ci->data);
        int idStat = ci->id;
        stat_SgrpStats* pStat;
        
        if(ci->delete_me)
        {
            if( stashIntRemovePointer(stat.sgstatFromIdSg, idStat, &pStat) )
            {
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " delete stat %d", idStat);
                // don't delete pointer itself, it still belongs to a supergroup that
                // will delete it.
                // (this delete shouldn't ever be called except for admin purposes)
            }
        }
        else
        {
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " updating sgrpstats %d", idStat);

            // --------------------
            // init hash

            if( !stat.sgstatFromIdSg )
                stat.sgstatFromIdSg = stashTableCreateInt(128);

            // --------------------
            // find or add the info

            if(!stashIntFindPointer(stat.sgstatFromIdSg, idStat, &pStat))
            {
                pStat = stat_SgrpStats_Create(CONTAINER_SGRPSTATS,0,0);
            }

            // --------------------
            // unpack 

            pStat = stat_SgrpStats_Create(CONTAINER_SGRPSTATS,0,0);
            stat_SgrpStats_Unpack(pStat, container_data);
            pStat->db_id = idStat;

            // ----------
            // add it to the sgrp lookup

            if(pStat->idSgrp > 0)
            {
                stat_SgrpStats *pStatExisting;
                if(!stashIntFindPointer(stat.sgstatFromIdSg,pStat->idSgrp, &pStatExisting))
                {
                    stashIntAddPointer(stat.sgstatFromIdSg,pStat->idSgrp, pStat, false);
                }
                else if( verify(pStatExisting != pStat) ) // redundant check
                {
                    int i;
                    for( i = 0; i < ARRAY_SIZE( pStatExisting->badgeStats.aiStats ); ++i ) 
                    {
                        pStatExisting->badgeStats.aiStats[i] += pStat->badgeStats.aiStats[i];
                    }
                    pStatExisting->dirty = true;

                    dbAsyncContainerUpdate(CONTAINER_SGRPSTATS, idStat, CONTAINER_CMD_DELETE, container_data, 0);
                    stat_SgrpStats_Destroy(pStat);

                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ":  removing duplicate stat %i", idStat);
                }
            }
            else
            {
                LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, "stat %d has invalid idSgrp '%d' for container '%s'", idStat, pStat->idSgrp, container_data );
            }

        }
        free(container_data);
        res = true;
    }
    else if(type == CONTAINER_LEVELINGPACTS)
    {
        if(ci->delete_me)
        {
            stat_LevelingPactDeleteMe(ci->id);
            // i don't think this ever actually happens... but just in case
        }
        else
        {
            char *container_data = estrTemp();
            estrPrintCharString(&container_data, ci->data);
            stat_LevelingPactUnpack(ci->data, ci->id, ci->members, ci->member_count);
            estrDestroy(&container_data);
        }
        res = true;
    }
    else if(type == CONTAINER_LEAGUES)
    {
        if (ci->delete_me)
        {
            stat_LeagueDeleteMe(ci->id);
        }
        else
        {
            char *container_data = estrTemp();
            estrPrintCharString(&container_data, ci->data);
            stat_LeagueUnpack(ci->data, ci->id, ci->members, ci->member_count);
            estrDestroy(&container_data);
        }
        res = true;
    }
    else if(type == CONTAINER_MININGACCUMULATOR)
    {
        char *container_data = strdup(ci->data);

        if(!ci->delete_me)
        // deletion only occurs when receiving redundant data,
        // in which case there's no need to delete anything locally as it's already been dealt with
        {
            int merged_container;
            char *name = NULL;

            if(container_data[0])
            {
                merged_container = MiningAccumulatorUnpackAndMerge(container_data, ci->id, &name);

                if(merged_container < 0)
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " ignoring duplicate mining accumulator for %s (%d)", name, ci->id);
                }
                else if(merged_container)
                {
                    if(name)
                    {
                        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " deleting redundant mining accumulator for %s (%d)", name, merged_container)
                    }
                    else
                    {
                        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " deleting nameless mining accumulator (%d)", merged_container)
                    }
                    dbAsyncContainerUpdate(CONTAINER_MININGACCUMULATOR,merged_container,CONTAINER_CMD_DELETE,"",0);
                }
            }
            else
            {
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " deleting zombie mining accumulator (%d)", ci->id);
                dbAsyncContainerUpdate(CONTAINER_MININGACCUMULATOR,ci->id,CONTAINER_CMD_DELETE,"",0);
            }
        }
        free(container_data);
        res = true;
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "dealWithContainer: got update for type %i", type);
    }

    return res;
}

//------------------------------------------------------------
//  This is plugged indirectly through a hierarchy of function
// calls:
// MapServer/dbcomm/dbcomm.c/void dbComm() calls
// "/dbcomm.c:dbMessageScan which passes to netLinkMonitor
// "/dbcomm.c:dbMessageCallback when it gets 'DBSERVER_CONTAINERS' calls
// "/dbcontainer.c:dbReceiveContainers calls
// "/dbcontainer.c:dbReceiveGroups calls 
// this
//----------------------------------------------------------
U32 dealWithContainer(ContainerInfo *ci,int type)
{
    return stat_dealWithContainer( ci, type );
}

//------------------------------------------------------------
//  cribbed from MapServer/cmdparse/cmdservercsr.c:sendToAllGroupMembers
//----------------------------------------------------------
void stat_sendToAllSgrpMembers(int db_id, char *msg)
{
    int group_type = CONTAINER_SUPERGROUPS;
    char *buf = NULL;

    if( verify( msg ))
    {
        Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD_TOGROUP);
        
        estrPrintf( &buf, "cmdrelay\n%s", msg ); 
        
        pktSendBitsPack(pak,1,db_id);
        pktSendBitsPack(pak,1,group_type);
        pktSendString(pak,buf);
        pktSend(&pak,&db_comm_link);

        estrDestroy(&buf);
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__": stat_sendToAllSgrpMembers", "NULL msg for id %d", db_id);
    }
}

void stat_sendToEnt(int idEnt, char *msg)
{
    char *buf = NULL;

    if( verify( idEnt && msg ))
    {
        Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD_BYENT);
        estrPrintf( &buf, "cmdrelay\n%s", msg ); 
        
        pktSendBitsPack(pak,1,idEnt);
        pktSendBitsPack(pak,1, 0);   // don't force
        pktSendString(pak,buf);
        pktSend(&pak,&db_comm_link);

        estrDestroy(&buf);
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__": stat_sendToAllSgrpMembers", "NULL msg for id %d", idEnt);
    }
}


// *********************************************************************************
//  container handling
// *********************************************************************************

void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid)
{
    switch (cmd)
    {
    xdefault:
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

// get notification when players logon to send them the stats
void dealWithNotify(U32 list_id, U32 cid, U32 dbid, int add)
{
    switch(list_id)
    {
    xcase CONTAINER_SUPERGROUPS:
    {
        int idSgrp = cid;
        // get the supergroup. need to load to send the correct badges down
        Supergroup *sg = stat_sgrpFromIdSgrp(idSgrp,true);

        if (!dbid) // supergroup itself is added or deleted
        {
            if( add )
            {
                // request the full info
                //dbAsyncContainerRequest( CONTAINER_SUPERGROUPS, idSgrp, 0, 0 );
            }
            else
            {
                // deleting a supergroup
                stat_SgrpStats *pStat;
                
                // ----------
                // also delete stats from db
                
                if( stashIntRemovePointer(stat.sgstatFromIdSg, idSgrp, &pStat) )
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, " delete stat %d", pStat->db_id);
                    dbAsyncContainerUpdate( CONTAINER_SGRPSTATS, pStat->db_id, CONTAINER_CMD_DELETE, "", 0 );
                    stat_SgrpStats_Destroy(pStat);
                    pStat = NULL;
                }
                
                // ----------
                // finally

                if(sg)
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "remove sgrp %d from hash", idSgrp);
                    destroySupergroup(sg);
                    stashIntRemovePointer(g_sgrpFromId, idSgrp, NULL);
                }
            }

            // ab: I think this is a race condition of sorts. ten
            // times in four months this log msg has been hit which
            // makes me think the notify might get here
            // first(?)... every one has been a delete of the
            // supergroup.
            if(!verify( sg ))
            {
                LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__": notified of sgrp %d that isn't in hashtable. dbid=%i add=%i", idSgrp, dbid, add );
            }
        }
        else // player adding or exiting supergroup, or logging on
        {
            int idEnt = dbid;
            int n;
            U32 *eaiStates;

            eaiStates = sgrpbadge_eaiStatesFromId(idSgrp);
            n = eaiSize( &eaiStates);

            if( n && add )
            {
                // clearing this will cause an update automatically
                // ab: redundant send to all logged in ents. This fix
                // should work, but we're shipping tommorow, so I'll wait.
                ZeroStructs( eaiStates, n );
                /*
                char *esCmd = badgestates_estrCmdFromStates( eaiStates );
                char *buf = NULL;
                Packet    *pak;
                
                estrCreateEx(&buf, estrLength(&esCmd) + 16);
                estrPrintf( &buf, "cmdrelay\n%s %i", esCmd, idEnt ); 
                
                pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD_BYENT);
                pktSendBitsPack( pak, 1, idEnt );
                pktSendBitsPack( pak, 1, 0 ); // don't force load
                pktSendString( pak, buf );
                pktSend(&pak,&db_comm_link);
                
                // cleanup
                estrDestroy(&esCmd);
                estrDestroy(&buf);
                */
            }
        }
    } // CONTAINER_SUPERGROUPS

    xcase CONTAINER_LEVELINGPACTS:
        if(!dbid) // notification for the leveling pact itself
        {
            if(!add)
                stat_LevelingPactDeleteMe(cid);
            // else creating a new pact
        }
        // else notification of a member logging in or out
    xcase CONTAINER_LEAGUES:
        if(!dbid)
        {
            if (!add)
                stat_LeagueDeleteMe(cid);
        }

    }
}

// *********************************************************************************
// cribbed from container_server.c until statserver does full port 
// - alternative would be to pretend to register for the CONTAINER_SUPERGROUP container
//   and use the update notify dispatch there 
// *********************************************************************************

void csvrNotify(Packet* pak)
{
    U32 list_id, cid, dbid;
    int add;

    list_id = pktGetBitsPack(pak, 1);
    cid = pktGetBitsPack(pak, 1);
    dbid = pktGetBitsPack(pak, 1);
    add = pktGetBits(pak, 1);
    dealWithNotify(list_id, cid, dbid, add);
}


void csvrDbMessage(int cmd,Packet* pak)
{
    switch (cmd)
    {
        case DBSERVER_CONTAINER_SERVER_ACK:
            // do nothing
        xcase DBSERVER_CONTAINER_SERVER_NOTIFY:
            csvrNotify(pak);
        xdefault:
            Errorf("Unknown server command: %i", cmd);
    }
}

// *********************************************************************************
//  stub functions for dbcomm, dbcontainer
// *********************************************************************************

#include "svr/svr_player.h"

// Externs
int last_db_error_code;
char last_db_error[256];
PlayerEnts player_ents[PLAYER_ENTTYPE_COUNT];
//Entity *entities[MAX_ENTITIES_PRIVATE];
U8 entity_state[MAX_ENTITIES_PRIVATE];
int entities_max;
EntityInfo entinfo[MAX_ENTITIES_PRIVATE];

char* teamTypeName(ContainerType type) { return ""; }
char *dbg_FillNameStr(char *pchDest, size_t iDestSize, Entity *e) { return ""; }
StaticMapInfo* staticMapInfoFind(int mapID) { return NULL; }
Entity *entFromDbId(int db_id) { return NULL; }

void cmdOldServerHandleRelay(char *msg) {;}
void dbGatherMapLinkInfo() {;}
void dbNameTableInit() {;}
void forceLogout(int db_id,int reason) {;}
void svrLogPerf(void) {}
void handleClearPnameCacheEntry(Packet *pak) {;}
void handleDbNewMapLoading(Packet *pak) {;}
void handleDbNewMapReady(Packet *pak) {;}
void handleDbNewMap(Packet *pak) {;}
void handleDbNewMapFail(Packet *pak) {;}
void handleForceLogout(Packet *pak) {;}
void handleMapRegisterCallback(Packet *pak) {;}
void handleSendEntNames(Packet *pak) {;}
void handleSendGroupNames(Packet *pak) {;}
void handleServerDoorUpdate(Packet *pak) {;}
void MissionRequestShutdown() {;}
void returnAllPlayers(int disconnect,char *msg) {;}
void sendDoorsToDbServer(int is_static) {;}
void serverSynchTime(void) {;}
void staticMapInfoResetPlayerCount() {;}
void teamlogPrintf(const char* func, const char* s, ...) {;}
typedef struct StoryArc StoryArc;
void MissionSetInfoString(char* info, StoryArc *pArc) {;}
void ArenaSetInfoString(char* info) {;}
void EndGameRaidSetInfoString(char* info) {;}
void SGBaseSetInfoString(char* info) {;}
void RaidSetInfoString(char *info) {}
void dealWithLostMembership(int list_id, int container_id, int ent_id) {;}
void dealWithBroadcast(int container,int *members,int count,char *msg,int msg_type,int senderID,int container_type) {;}
void dealWithEntAck(int list_id,int id) {;}
void dealWithShardAccountAck(int list_id,int id) {;}
void mapGroupInit(char* uniquename) {;}
void dbUpdateAllOnlineInfo() {;}
void dbUpdateAllOnlineInfoComments() {;}
void handleOnlineEnts(Packet *pak) {;}
void dbListDeletedChars_HandleRslt(Packet* pak) {;}
void dbRestoreDeletedChar_HandleRslt(Packet* pak) {;}
void dbOfflineCharacter_HandleRslt(Packet* pak) {;}
void handleOnlineEntComments(Packet *pak) {;}
void dbQueryReceiveInfo(Packet *pak) {;}
void handleArenaAddress(Packet* pak) {;}
void getAuthId(Entity *e) {;}
void handleSgChannelInviteHelper(Packet *pak) {;}
void handleDbBackupSearch(Packet *pak){;}
void handleDbBackupApply(Packet *pak){;}
void handleDbBackupView(Packet *pak){;}
//void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid) {;}
//void dealWithContainerReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str) {;}
//void dealWithContainerReflection(int list_id, int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo) {;}
void beaconRequestSetMasterServerAddress(char* serverAddress){;}

// *********************************************************************************
//  for dblog
// *********************************************************************************
void shardCommReconnect() {} 
int gChatServerTestClients = 0;
void shardCommMonitor(){}

// *********************************************************************************
//  for supergroup missions
// *********************************************************************************


// *********************************************************************************
// for cmd messages
// *********************************************************************************
/*char *cmdTranslated(char * str)
{
    return str;
}

char *cmdHelpTranslated( char * str )
{
    return str;
}

char *cmdRevTranslated(char * str)
{
    return str;
}*/

void conPrintf( ClientLink* client, char *s, ... )
{
    va_list va;

    verify(0); // not really supported

    va_start( va, s );
    log_vprintf(LOGID, s, va);
    va_end(va);
}

char* localizedPrintf(Entity * e, char* messageID,  ...)
{
    static char *buf = NULL;
    char* msg;
    va_list    arg;
    int prepend = 0;

    verify(0); // not really supported

    va_start(arg, messageID);
    estrConcatfv(&msg, messageID, arg);
    va_end(arg);
    return msg;
}

//----------------------------------------
//  mark stats as dirty to save to db
//----------------------------------------
void statdb_SetSgStatDirty(int idSgrp)
{
    stat_SgrpStats *pStats;
    if (!stashIntFindPointer(stat.sgstatFromIdSg, idSgrp, &pStats))
        pStats = NULL;
    if(verify( pStats ))
    {
        pStats->dirty = true;
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": verify failed for id %d", idSgrp);
    }
}

//----------------------------------------
//  locks the passed supergroup. 
// different than mapserver one due to internals
// of statserver.
//----------------------------------------
Supergroup *stat_sgrpLockAndLoad(int idSgrp)
{
    Supergroup *res = NULL;
    
    if( dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 ) )
    {
        assert(stashIntFindPointer(g_sgrpFromId, idSgrp, &res));
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": lock and load failed for id %d", idSgrp);
    }
    return res;
}

//----------------------------------------
//  different than mapserver one, doesn't cleanup
// the supergroup because it wasn't allocated by the lock function.
//----------------------------------------
void stat_sgrpUpdateUnlock(int idSgrp, Supergroup *sg)
{
    char *container_str;
    container_str = packageSuperGroup(sg);
    if(!verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str)))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": failed to unlock sg %s, id %d. container %s",sg->name,idSgrp,container_str);
    }
    free(container_str);
}
