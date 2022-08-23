/*\
 *
 *    raidmapserver.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Mainly networking functions on the mapserver side for communicating
 *  about base raids
 *
 */

#include "raidmapserver.h"
#include "entity/entity.h"
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include "comm_game.h"
#include "arenamapserver.h"
#include "entity/entVarUpdate.h"
#include "svr/svr_base.h"
#include <utilitieslib/network/net_packet.h>
#include "dbcomm/dbnamecache.h"
#include "gameData/raidstruct.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbcontainer.h"
#include "container/baseloadsave.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include "container/container_util.h"
#include "container/containercallbacks.h"
#include "entity/entPlayer.h"
#include "dooranim.h"
#include "dbcomm/dbmapxfer.h"
#include "sgraid.h"
#include "gameComm/sendToClient.h"
#include "language/langServerUtil.h"
#include "entity/Supergroup.h"
#include "auth/authUserData.h"
#include "svr/svr_player.h"
#include "bases/basedata.h"
#include "entity/SgrpServer.h"
#include <utilitieslib/utils/timing.h>
#include "bases/bases.h"
#include "sgraid_V2.h"
#include "door.h"

// *********************************************************************************
//  data sync
// *********************************************************************************

void RaidUpdateClient(Entity* ent, U32 id, int deleting)
{
    U32 othersg;
    ScheduledBaseRaid* baseraid;

    if (!ent || !ent->client || !ent->client->ready) return; // it will update when it connects fully
    baseraid = cstoreGetAdd(g_ScheduledBaseRaidStore, id, 0);
    if (!baseraid && !deleting)
    {
        Errorf("failed to find container in RaidUpdateClient");
        return;
    }

    START_PACKET(pak, ent, SERVER_SGRAID_UPDATE)
        pktSendBitsPack(pak, 1, id);
        pktSendBits(pak, 1, deleting? 1:0);
        if (!deleting)
        {
            char* data = cstorePackage(g_ScheduledBaseRaidStore, baseraid, id);
            pktSendString(pak, data);
            free(data);

            // send id/name pair
            if (baseraid->attackersg != ent->supergroup_id)
                othersg = baseraid->attackersg;
            else
                othersg = baseraid->defendersg;
            pktSendBitsPack(pak, 1, othersg);
            pktSendString(pak, dbSupergroupNameFromId(othersg));
        }
    END_PACKET
}

void RaidFullyUpdateClient(Entity* ent)
{
    int i, count = eaiSize(&ent->pl->baseRaids);
    if (!ent || !ent->client || !ent->client->ready) return; // it will update when it connects fully
    for (i = 0; i < count; i++)
        RaidUpdateClient(ent, ent->pl->baseRaids[i], 0);
}

void RaidEntityReflection(int* ids, int deleting, Entity* ent)
{
    int i, count = eaiSize(&ids);
    if (!ent->pl->baseRaids)
        eaiCreate(&ent->pl->baseRaids);
    for (i = 0; i < count; i++)
    {
        if (deleting)
        {
            // if it was previously in list..
            if (eaiFindAndRemove(&ent->pl->baseRaids, ids[i]) != -1)
                RaidUpdateClient(ent, ids[i], 1);
        }
        else
        {
            if (eaiFind(&ent->pl->baseRaids, ids[i]) == -1)
            {
                eaiPush(&ent->pl->baseRaids, ids[i]);
            }
            RaidUpdateClient(ent, ids[i], 0);
        }
    }
}

void RaidHandleReflection(int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo)
{
    int i;

    cstoreHandleReflectUpdate(g_ScheduledBaseRaidStore, ids, deleting, pak);
    SendReflectionToEnts(ids, deleting, reflectinfo, RaidEntityReflection);
    for (i = 0; i < eaSize(&reflectinfo); i++)
    {
        if (reflectinfo[i]->target_list == CONTAINER_MAPS ||
            reflectinfo[i]->target_list == 0) // response
            RaidHandleUpdate(ids, deleting);
    }
}


// *********************************************************************************
//  commands
// *********************************************************************************

void RaidHandleReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str, Packet* pak)
{
    // the receipt for a sgraidlist request contains the list of infos to forward to client
    if (!err && user_cmd == CLIENTINP_REQ_SGRAIDLIST)
    {
        Entity* ent = entFromDbId(user_data);
        if (ent)
        {
            SupergroupRaidInfo info = {0};
            START_PACKET(ret, ent, SERVER_SGRAID_INFO)
                while (pktGetBits(pak, 1))
                {
                    SupergroupRaidInfoGet(pak, &info);
                    pktSendBits(ret, 1, 1);
                    SupergroupRaidInfoSend(ret, &info);
                }
                pktSendBits(ret, 1, 0);
            END_PACKET
        }
    }

    // this was actually a transfer request
    if (user_cmd == CLIENTINP_BASERAID_PARTICIPATE_TRANSFER)
    {
        Entity* ent = entFromDbId(user_data);
        if (ent)
        {
            if (err) // failed the transfer
            {
                sendDoorMsg( ent, DOOR_MSG_FAIL, str );
                return; // avoid printing error
            }
            else // ok to complete transfer
            {
                Vec3 zero = {0};
                DoorAnimEnter(ent, NULL, zero, 0, pktGetString(pak), "PlayerSpawn", XFER_BASE, NULL);
                sendDoorMsg( ent, DOOR_MSG_OK, 0 );
                return; // avoid printing error
            }
        }
    }

    // if there was an error or not, if a string was sent, send to client
    if (str && str[0] && user_data)
    {
        RaidClientError(user_data, err, str);
    }
}

void RaidClientError(U32 dbid, U32 err, char* str)
{
    Entity* ent = entFromDbId(dbid);
    if (ent)
    {
        START_PACKET(ret, ent, SERVER_SGRAID_ERROR)
            pktSendBitsAuto(ret, err);
            pktSendString(ret, str);
        END_PACKET
    }
}

static int g_supergroupid = 0, g_userid = 0;

int RaidClientCallback(Packet *pak, int cmd, NetLink *link)
{
    switch (cmd)
    {
        //xcase RAIDSERVER_RAIDLIST:
        //handleRaidList(pak, link);
xdefault:
        printf("Unknown command %d\n",cmd);
        return 0;
    }
    return 1;
}

int RaidChallenge(Entity* e, U32 sgid)
{
    Packet* pak;

    if (!e || !e->ready || !e->supergroup)
        return 0;

    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_CHALLENGE, CONTAINER_BASERAIDS, 0, 0, e->db_id);
    pktSendBitsPack(pak, 1, e->supergroup_id);
    pktSendBitsPack(pak, 1, sgid);
    pktSend(&pak, &db_comm_link);

    return 1;
}

int RaidAcceptInstantInvite(Entity* e, U32 inviter_dbid, U32 inviter_sgid)
{
    Packet* pak;

    if (!e || !e->ready || !e->supergroup)
        return 0;

    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_CHALLENGE, CONTAINER_BASERAIDS, 0, 0, inviter_dbid);
    pktSendBitsPack(pak, 1, inviter_sgid);
    pktSendBitsPack(pak, 1, e->supergroup_id);
    pktSend(&pak, &db_comm_link);
    return 1;
}

void RaidRequestList(Entity* e)
{
    Packet* pak;

    if (!e || !e->ready || !e->supergroup)
        return;

    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_REQ_LIST, CONTAINER_BASERAIDS, 0, CLIENTINP_REQ_SGRAIDLIST, e->db_id);
    pktSendBitsAuto(pak, e->db_id);
    pktSendBitsAuto(pak, e->supergroup_id);
    pktSend(&pak, &db_comm_link);
}

void RaidSetBaseInfo(U32 sgid, int max_raid_size, int open_mount)
{
    Packet* pak = dbContainerRelayCreate(RAIDCLIENT_RAID_BASE_SETTINGS, CONTAINER_BASERAIDS, 0, 0, 0);
    pktSendBitsAuto(pak, sgid);
    pktSendBitsAuto(pak, max_raid_size);
    pktSendBits(pak, 1, open_mount?1:0);
    pktSend(&pak, &db_comm_link);
}

void RaidNotifyPlayerSG(Entity* e)
{
    if (e && e->supergroup_id && e->supergroup)
    {
        Packet* pak = dbContainerRelayCreate(RAIDCLIENT_NOTIFY_VILLAINSG, CONTAINER_BASERAIDS, 0, 0, 0);
        pktSendBitsAuto(pak, e->supergroup_id);
        pktSendBits(pak, 1, e->supergroup->playerType);
        pktSend(&pak, &db_comm_link);
    }
}

void RaidSchedule(Entity* e, U32 sgid, U32 time, U32 raidsize)
{
    Packet* pak;
    if (!e->supergroup_id)
    {
        RaidClientError(e->db_id, CONTAINER_ERR_CANT_COMPLETE, "RaidNotInSupergroup");
        return;
    }
    if (!sgroup_hasPermission(e, SG_PERM_RAID_SCHEDULE))
    {
        RaidClientError(e->db_id, CONTAINER_ERR_CANT_COMPLETE, "RaidNoPermission");
        return;
    }

    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_SCHEDULE, CONTAINER_BASERAIDS, 0, CLIENTINP_RAID_SCHEDULE, e->db_id);
    pktSendBitsAuto(pak, e->supergroup_id);
    pktSendBitsAuto(pak, sgid);
    pktSendBitsAuto(pak, time);
    pktSendBitsAuto(pak, raidsize);
    pktSend(&pak, &db_comm_link);
}

void RaidCancel(Entity* e, U32 raidid)
{
    Packet* pak;
    if (!e->supergroup_id)
    {
        RaidClientError(e->db_id, CONTAINER_ERR_CANT_COMPLETE, "RaidCancelWrongSG");
        return;
    }
    if (!sgroup_hasPermission(e, SG_PERM_RAID_SCHEDULE))
    {
        RaidClientError(e->db_id, CONTAINER_ERR_CANT_COMPLETE, "RaidCancelPermission");
        return;
    }
    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_CANCEL, CONTAINER_BASERAIDS, 0, CLIENTINP_RAID_CANCEL, e->db_id);
    pktSendBitsAuto(pak, e->supergroup_id);
    pktSendBitsAuto(pak, raidid);
    pktSend(&pak, &db_comm_link);
}

// would another raid prevent this player from scheduling an instant raid?
int RaidSGCanScheduleInstant(Entity* player)
{
    U32 curtime = dbSecondsSince2000();
    int i;

    for (i = 0; i < eaiSize(&player->pl->baseRaids); i++)
    {
        ScheduledBaseRaid* baseraid = cstoreGet(g_ScheduledBaseRaidStore, player->pl->baseRaids[i]);
        if (!baseraid) continue; // don't have update yet, safe to ignore
        if (baseraid->complete_time) continue;
        if (baseraid->time < curtime-MAX_BASERAID_LENGTH) continue;
        if (baseraid->time > curtime+MAX_BASERAID_LENGTH+BASERAID_INSTANT_DELAY) continue;
        return 0;
    }
    return 1;
}

void RaidRequestParticipate(Entity* player, U32 raidid, int join)
{
    Packet* pak;
    
    if (!player->supergroup || !player->superstat)
        return;
    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_PARTICIPATE, CONTAINER_BASERAIDS, raidid, 0, player->db_id);
    pktSendBitsAuto(pak, player->supergroup_id);
    pktSendBitsAuto(pak, player->db_id);
    pktSendBitsAuto(pak, player->superstat->member_since);
    pktSendBits(pak, 1, join?1:0);
    pktSendString(pak, "");
    pktSend(&pak, &db_comm_link);
}

void RaidRequestParticipateAndTransfer(Entity* player, U32 raidid, char* transfer_string)
{
    Packet* pak;
    
    if (!player->supergroup || !player->superstat)
        return;
    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_PARTICIPATE, CONTAINER_BASERAIDS, raidid, CLIENTINP_BASERAID_PARTICIPATE_TRANSFER, player->db_id);
    pktSendBitsAuto(pak, player->supergroup_id);
    pktSendBitsAuto(pak, player->db_id);
    pktSendBitsAuto(pak, player->superstat->member_since);
    pktSendBits(pak, 1, 1);
    pktSendString(pak, transfer_string);
    pktSend(&pak, &db_comm_link);
}

// debug way of challenging for a raid
int RaidDebugChallenge(Entity* e, char* sgname)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i, time = dbSecondsSince2000();

    for (i = 0; i < players->count; i++)
    {
        Entity* player = players->ents[i];
        if (player && player->supergroup)
        {
            if (stricmp(sgname, player->supergroup->name)==0)
            {
                return RaidChallenge(e, player->supergroup_id);
            }
        }
    }
    return 0;
}

void RaidDebugSetVar(ClientLink* client, char* var, char* value)
{
    Packet* pak = dbContainerRelayCreate(RAIDCLIENT_SETVAR, CONTAINER_BASERAIDS, 0, 0, client->entity? client->entity->db_id: 0);
    pktSendString(pak, var);
    pktSendString(pak, value);

    pktSend(&pak, &db_comm_link);
}

void RaidSetWindow(Entity* e, U32 daybits, U32 hour)
{
    Packet* pak;

    if (!e) return;
    if (!e->supergroup_id)
    {
        RaidClientError(e->db_id, CONTAINER_ERR_CANT_COMPLETE, "RaidNotInSupergroup");
        return;
    }
    if (!sgroup_hasPermission(e, SG_PERM_RAID_SCHEDULE))
    {
        RaidClientError(e->db_id, CONTAINER_ERR_CANT_COMPLETE, "RaidNoPermission");
        return;
    }
    pak = dbContainerRelayCreate(RAIDCLIENT_RAID_SET_WINDOW, CONTAINER_BASERAIDS, 0, 0, e->db_id);
    pktSendBitsAuto(pak, e->supergroup_id);
    pktSendBitsAuto(pak, daybits);
    pktSendBitsAuto(pak, hour);
    pktSend(&pak, &db_comm_link);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Item of Power Game /////////////////////////////////////////////////////////////

ItemOfPowerGame * g_ItemOfPowerGame = 0; //Poor man's parameters

//Updates Item Of Power Game State
int ItemOfPowerGameSetState( Entity * e, char * newState )
{
    Packet* pak;

    if (!e || !e->ready )
        return 0;

    if( !newState )
        return 0;

    pak = dbContainerRelayCreate(RAIDCLIENT_ITEM_OF_POWER_GAME_SET_STATE, CONTAINER_ITEMOFPOWERGAMES, 0, 0, e->db_id);
    pktSendString( pak,  newState );
    pktSend(&pak, &db_comm_link);

    return 1;
}

//Called on MapServer start up to request a Reflection of the ItemOfPowerGame
void requestItemOfPowerGameStateUpdate()
{
    Packet* pak = dbContainerRelayCreate(RAIDCLIENT_ITEM_OF_POWER_GAME_REQUEST_UPDATE, CONTAINER_ITEMOFPOWERGAMES, 0, 0, db_state.map_id);
    pktSend(&pak, &db_comm_link);
}

//Expect a reflection after above update request, and any time the game's state changes, so it should always be fully up to date
void ItemOfPowerGameHandleReflection(int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo)
{
    cstoreHandleReflectUpdate(g_ItemOfPowerGameStore, ids, deleting, pak);
}

int getItemOfPowerGameCallback(ItemOfPowerGame* iopGame, U32 iopGameID)
{  
    assert( !g_ItemOfPowerGame ); //Only one IOP game should every be running, and it 
    assert( iopGameID == 1 ); //Should alway be game one
    assert( iopGameID == iopGame->id );
    g_ItemOfPowerGame = iopGame;

    return 1;
}

int IsCathedralOfPainOpen()
{
    g_ItemOfPowerGame = 0;
    cstoreForEach(g_ItemOfPowerGameStore, getItemOfPowerGameCallback );

    if( g_ItemOfPowerGame && ( g_ItemOfPowerGame->state & (IOP_GAME_CATHEDRAL_OPEN | IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS) ) )
        return 1;

    return 0;
}

ClientLink* g_clientLinkParam; //Poor man's parameter
static int printIOP( ItemOfPower* iop, U32 iopId)
{
    char buf[400];
    int sgid;

    sgid = g_clientLinkParam->entity->supergroup_id;

    timerMakeDateStringFromSecondsSince2000(buf, iop->creationTime); 
    conPrintf( g_clientLinkParam, "\t%s%s (id %d) \tOwned by SG %d \tCreated at %i:%s\n", (sgid&&sgid==iop->superGroupOwner)?"*Yours*":"      ",
               iop->name, iop->id, iop->superGroupOwner, 
               iop->creationTime, buf );
    return 1;
}

//Handles cmd "showIopGame"
void ShowItemOfPowerGameStatus( ClientLink* client )
{
    char buf[200];

    g_ItemOfPowerGame = 0;
    cstoreForEach(g_ItemOfPowerGameStore, getItemOfPowerGameCallback );

    if( !g_ItemOfPowerGame )
    {
        conPrintf( client, "No RaidServer running when this map started. If one has started since, re-do showIOP" );
        requestItemOfPowerGameStateUpdate();
    }
    else
    {     
        timerMakeDateStringFromSecondsSince2000(buf, g_ItemOfPowerGame->startTime);
        conPrintf( client, "\nItem Of Power Game\nStarted: %s State: %s\n", buf, stringFromGameState(g_ItemOfPowerGame->state) );
        conPrintf( client, "All Items Of Power currently tracked on this server:\n");
        g_clientLinkParam = client;
        cstoreForEach( g_ItemOfPowerStore, printIOP );
    }
}

//////////////////////////////////////////////////////////////////////////////////
/////////////////////// N E W   I o P s //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
SpecialDetail *ItemOfPowerFind(Supergroup *pSG, const char *pName)
{
    int i;
    int iSize = eaSize(&pSG->specialDetails);

    for(i=0; i<iSize; i++)
    {
        if(pSG->specialDetails[i] != NULL &&
            pSG->specialDetails[i]->pDetail != NULL &&
            stricmp(pSG->specialDetails[i]->pDetail->pchName, pName) == 0)
        {
            return pSG->specialDetails[i];
        }
    }

    return NULL;
}

int ItemOfPowerGrant2(Entity* e, char *iopName, int expireTime)
{
    const Detail *pDetail = basedata_GetDetailByName(iopName);

    if (!e || !e->supergroup || !pDetail)
        return false;

    // can't have two of the same details
    if (ItemOfPowerFind(e->supergroup, iopName) == NULL)
    {
        sgroup_AddSpecialDetail(e, pDetail, dbSecondsSince2000(), dbSecondsSince2000() + expireTime, DETAIL_FLAGS_AUTOPLACE);
        return true;
    } else {
        return false;
    }
}

int ItemOfPowerRevoke2( Entity* e, const char *itemOfPowerName )
{
    SpecialDetail *pSpecialDetail = NULL; 

    if (!e || !e->supergroup)
        return false;

    pSpecialDetail = ItemOfPowerFind(e->supergroup, itemOfPowerName);
    
    if (pSpecialDetail != NULL)
    {
        sgroup_RemoveSpecialDetail( e, pSpecialDetail->pDetail, pSpecialDetail->creation_time );
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////////
/////////////////// Items of Power //////////////////////

void ItemOfPowerEntityReflection(int* ids, int deleting, Entity* ent)
{
}

void ItemOfPowerHandleReflection(int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo)
{
    cstoreHandleReflectUpdate(g_ItemOfPowerStore, ids, deleting, pak);
    //SendReflectionToEnts( ids, deleting, reflectinfo, ItemOfPowerEntityReflection );
}

int ItemOfPowerGrantNew(Entity* e, int sgid)
{
    Packet* pak;

    if (!e || !e->ready )
        return 0;

    pak = dbContainerRelayCreate(RAIDCLIENT_ITEM_OF_POWER_GRANT_NEW, CONTAINER_ITEMSOFPOWER, 0, 0, e->db_id);
    pktSendBitsPack(pak, 1, sgid);
    pktSend(&pak, &db_comm_link);

    return 1;
}

int ItemOfPowerGrant( Entity* e, int sgid, char * itemOfPowerName )
{
    Packet* pak;

    if (!e || !e->ready )
        return 0;
    if( !itemOfPowerName )
        return 0;

    pak = dbContainerRelayCreate(RAIDCLIENT_ITEM_OF_POWER_GRANT, CONTAINER_ITEMSOFPOWER, 0, 0, e->db_id);
    pktSendBitsPack(pak, 1, sgid);
    pktSendString(pak, itemOfPowerName);
    pktSend(&pak, &db_comm_link);

    return 1;
}


int ItemOfPowerTransfer( Entity* e, int sgid, int newSgid, char * itemOfPowerName, int timeCreated )
{
    Packet* pak;

    if (!e || !e->ready )
        return 0;
    if( !itemOfPowerName )
        return 0;

    pak = dbContainerRelayCreate(RAIDCLIENT_ITEM_OF_POWER_TRANSFER, CONTAINER_ITEMSOFPOWER, 0, 0, e->db_id);
    pktSendBitsPack(pak, 1, sgid);
    pktSendBitsPack(pak, 1, newSgid);
    pktSendString(pak, itemOfPowerName );
    pktSendBitsPack(pak, 1, timeCreated );
    pktSend(&pak, &db_comm_link);

    return 1;
}


int ItemOfPowerSGSynch( int sgid )
{
    Packet* pak;

    pak = dbContainerRelayCreate(RAIDCLIENT_ITEM_OF_POWER_SG_SYNCH, CONTAINER_ITEMSOFPOWER, 0, 0, 0);
    pktSendBitsPack(pak, 1, sgid);
    pktSend(&pak, &db_comm_link);

    return 1;
}


int ItemOfPowerRevoke( Entity* e, int sgid, char * itemOfPowerName, int timeCreated )
{
    return ItemOfPowerTransfer( e, sgid, 0, itemOfPowerName, timeCreated );
}


//////////////////////// End Item of Power Game ///////////////////////////////////////////

void SGBaseSetInfoString(char* info)
{
    if (!SGBaseDeconstructInfoString(info, &g_supergroupid, &g_userid))
    {
        g_supergroupid = g_userid = 0;
        log_printf("sgbase.log", "SGBaseSetInfoString, got bad info %s", info);
        dbAsyncReturnAllToStaticMap("SGBaseSetInfoString() bad info",info);
        return;
    }

    // load/save map to database:
    sprintf(db_state.map_name, "supergroupid=%i,userid=%i", g_supergroupid, g_userid);

    // set to go..
}

static int g_raidsg1;
static int g_raidsg2;
void RaidSetInfoString(char *info)
{
    if(!RaidDeconstructInfoString(info, &g_raidsg1, &g_raidsg2))
    {
        g_raidsg1 = g_raidsg2 = 0;
        log_printf("sgbase.log", "RaidSetInfoString, got bad info %s", info);
        dbAsyncReturnAllToStaticMap("RaidSetInfoString() bad info",info);
        return;
    }
    strncpy(raidInfoString, info, MAX_PATH - 1);
    raidInfoString[MAX_PATH - 1] = 0;
    sprintf(db_state.map_name, "%i vs %i", g_raidsg1, g_raidsg2);
}

// participating_test - testing for attacking & defending supergroups, valid on any mapserver
int g_attacktest = 0;
int g_parttest = 0;
U32 g_thissg = 0;
U32 g_thisdbid = 0;
U32 g_othersg = 0;
U32 g_raidid = 0;
static int participating_test(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if ((g_attacktest && baseraid->attackersg == g_thissg) ||
        (!g_attacktest && baseraid->defendersg == g_thissg))
    {
        U32 time = dbSecondsSince2000();
        if (time >= baseraid->time && !baseraid->complete_time)
        {
            // verify if this player is a participant
            if (g_parttest && !BaseRaidIsParticipant(baseraid, g_thisdbid, g_attacktest))
                return 1;

            if (g_attacktest)
                g_othersg = baseraid->defendersg;
            else
                g_othersg = baseraid->attackersg;
            g_raidid = raidid;
            return 0;
        }
    }
    return 1;
}

static int scheduled_test(ScheduledBaseRaid* baseraid, U32 raidid)
{
    if ((g_attacktest && baseraid->attackersg == g_thissg) ||
        (!g_attacktest && baseraid->defendersg == g_thissg))
    {
        g_raidid = raidid;

        if (g_attacktest)
            g_othersg = baseraid->defendersg;
        else
            g_othersg = baseraid->attackersg;

        return 0;
    }
    return 1;
}


U32 RaidSGIsAttacking(U32 sgid)
{
    g_attacktest = 1;
    g_thissg = sgid;
    g_parttest = g_thisdbid = g_othersg = g_raidid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, participating_test);
    return g_raidid;
}

U32 RaidSGIsDefending(U32 sgid)
{
    g_attacktest = 0;
    g_thissg = sgid;
    g_parttest = g_thisdbid = g_othersg = g_raidid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, participating_test);
    return g_raidid;
}

U32 RaidHasOpenSlot(U32 sgid)
{
    ScheduledBaseRaid* raid;
    U32 raidid;

    raidid = RaidSGIsDefending(sgid);
    if (raidid)
    {
        raid = cstoreGet(g_ScheduledBaseRaidStore, raidid);
        if (BaseRaidNumParticipants(raid, 0) < raid->max_participants)
            return 1;
        else
            return 0;
    }
    raidid = RaidSGIsAttacking(sgid);
    if (raidid)
    {
        raid = cstoreGet(g_ScheduledBaseRaidStore, raidid);
        if (BaseRaidNumParticipants(raid, 1) < raid->max_participants)
            return 1;
        else
            return 0;
    }
    return 0;
}

U32 RaidPlayerIsAttacking(Entity* player)
{
    g_attacktest = 1;
    g_thissg = player->supergroup_id;
    g_parttest = 1;
    g_thisdbid = player->db_id;
    g_othersg = g_raidid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, participating_test);
    return g_raidid;
}

U32 RaidPlayerIsDefending(Entity* player)
{
    g_attacktest = 0;
    g_thissg = player->supergroup_id;
    g_parttest = 1;
    g_thisdbid = player->db_id;
    g_othersg = g_raidid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, participating_test);
    return g_raidid;
}

U32 RaidSGIsScheduledAttacking(U32 sgid)
{
    g_attacktest = 1;
    g_thissg = sgid;
    g_parttest = g_thisdbid = g_othersg = g_raidid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, scheduled_test);
    return g_raidid;
}
U32 RaidSGIsScheduledDefending(U32 sgid)
{
    g_attacktest = 0;
    g_thissg = sgid;
    g_parttest = g_thisdbid = g_othersg = g_raidid = 0;
    cstoreForEach(g_ScheduledBaseRaidStore, scheduled_test);
    return g_raidid;
}


U32 RaidGetDefendingSG(U32 raidid)
{
    ScheduledBaseRaid* raid;
    if (!raidid)
        return 0;
    raid = cstoreGet(g_ScheduledBaseRaidStore, raidid);
    if (raid)
        return raid->defendersg;
    return 0;
}

int OnSGBase(void)
{
    // TODO: rename this function 'OnBase' or make a separate 'OnUserBase'
    extern int g_raidsg1, g_raidsg2;
    return g_supergroupid || g_userid || g_raidsg1 || g_raidsg2 ? 1 : 0;
}

U32 SGBaseId(void)
{
    return g_supergroupid;
}

void SGBaseSave(void)
{
    if (OnSGBase() || db_state.local_server)
        baseSaveMapToDb();

}

void SGBaseEnter(Entity* ent, char* spawnlocation, char* errmsg)
{
    if (!ent->supergroup_id)
        strcpy(errmsg, localizedPrintf(ent,"SGDoesNotOwnBase"));
    else if (ent->access_level)
    {
        if (RaidSGIsDefending(ent->supergroup_id) && !RaidPlayerIsDefending(ent))
        {
            if (RaidHasOpenSlot(ent->supergroup_id))
                RaidRequestParticipateAndTransfer(ent, RaidSGIsDefending(ent->supergroup_id), SGBaseConstructInfoString(ent->supergroup_id));
            else
                strcpy(errmsg, localizedPrintf(ent,"SGNotParticipatingInRaid"));
        }
        else
            SGBaseEnterFromSgid(ent, spawnlocation, ent->supergroup_id, errmsg);
    }
    else
    {
        strcpy(errmsg, localizedPrintf(ent,"SGDoesNotOwnBase") );
    }
}

// this is the NO VERIFY version, please use SGBaseEnter for verification
void SGBaseEnterFromSgid(Entity* ent, char* spawnlocation, int idSgrp, char* errmsg)
{
    Vec3 zero = {0};
    if(verify(idSgrp))
    {
        DoorAnimEnter(ent, NULL, zero, 0, SGBaseConstructInfoString(idSgrp), spawnlocation, XFER_BASE, NULL);
    }
}



