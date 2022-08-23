/*\
 *
 *    ArenaDb.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *         Confidential property of Cryptic Studios
 *
 *    Stub functions so arena can use dbcomm, dbcontainer.
 *  Also any other arena-specific db client functions.
 *
 */

#include "dbcomm/dbcomm.h"
#include "dbcomm/dbcontainer.h"
#include "arenaevent.h"
#include "arenaplayer.h"
#include <utilitieslib/utils/error.h>
#include "entity/entity.h"
#include "entity/entsend.h"
#include "comm_backend.h"
#include <utilitieslib/utils/log.h>

void containerHandleArena(ContainerInfo* ci)
{
    ArenaEvent* ae = EventGetAdd(ci->id, 1);
    if (!ae)
    {
        LOG_OLD_ERR("containerHandleArena: Couldn't create event?");
        return;
    }
    unpackArenaEvent(ae, ci->data, ci->id);
    // MAK- this was a bad idea..
    //ae->uniqueid = 0;    // arenaserver resets all unique id's on initial container load
}

void containerHandleArenaPlayers(ContainerInfo* ci)
{
    ArenaPlayer* ap = PlayerGetAdd(ci->id, 1);
    if (!ap)
    {
        LOG_OLD_ERR("containerHandleArena: Couldn't create player?");
        return;
    }
    unpackArenaPlayer(ap, ci->data, ci->id);
}

// arena server only pays attention to arena events, players
U32 dealWithContainer(ContainerInfo *ci,int type)
{
    if (type == CONTAINER_ARENAEVENTS)
        containerHandleArena(ci);
    else if (type == CONTAINER_ARENAPLAYERS)
        containerHandleArenaPlayers(ci);
    else
        LOG_OLD_ERR("dealWithContainer: got update for type %i", type);
    return 0;
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
void logSendDisconnect(F32 timeout) {}
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
void SGBaseSetInfoString(char* info) {;}
void RaidSetInfoString(char *info) {}
void EndGameRaidSetInfoString(char *info) {}
void dealWithLostMembership(int list_id, int container_id, int ent_id) {;}
void dealWithBroadcast(int container,int *members,int count,char *msg,int msg_type,int senderID,int container_type) {;}
void dealWithEntAck(int list_id,int id) {;}
void dealWithShardAccountAck(int list_id,int id) {;}
void mapGroupInit(char* uniquename) {;}
void dbUpdateAllOnlineInfo() {;}
void dbUpdateAllOnlineInfoComments() {;}
void handleOnlineEnts(Packet *pak) {;}
void handleOnlineEntComments(Packet *pak) {;}
void dbListDeletedChars_HandleRslt(Packet* pak) {;}
void dbRestoreDeletedChar_HandleRslt(Packet* pak) {;}
void dbOfflineCharacter_HandleRslt(Packet* pak) {;}
void dbQueryReceiveInfo(Packet *pak) {;}
void logDisconnectFlush() {;}
void handleArenaAddress(Packet* pak) {;}
void getAuthId(Entity *e) {;}
void handleSgChannelInviteHelper(Packet *pak) {;}
void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid) {;}
void dealWithContainerReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str, Packet* pak) {;}
void dealWithContainerReflection(int list_id, int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo) {;}
void beaconRequestSetMasterServerAddress(char* serverAddress){;}
void handleDbBackupSearch(Packet *pak){;}
void handleDbBackupApply(Packet *pak){;}
void handleDbBackupView(Packet *pak){;}
void dbLog(char const *cmdname,Entity *e,char const *fmt, ...){}