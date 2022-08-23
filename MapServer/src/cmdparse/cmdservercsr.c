/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/strings_opt.h>

#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include "cmdparse/cmdserver.h"
#include "player/clientEntityLink.h"
#include "gameComm/sendToClient.h"
#include "entity/entserver.h"
#include "svr/svr_base.h"
#include "svr/svr_player.h"
#include "gameComm/svr_chat.h"
#include "generator/entgen.h"
#include "dbcomm/dbquery.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/buddy_server.h"
#include "storyarc/task.h"
#include "container/team.h"
#include "entity/entPlayer.h"
#include "gameData/store.h"
#include "storyarc/storyarcprivate.h"
#include "container/containerloadsave.h"
#include "player/badges.h"
#include "player/badges_server.h"
#include "dbghelper.h"
#include "dbcomm/shardcomm.h"
#include "gameComm/svr_chat.h"
#include "gameComm/trayCommon.h"
#include "entity/SgrpServer.h"
#include "container/containercallbacks.h"
#include "cmdparse/cmdservercsr.h"
#include "dbcomm/dbnamecache.h"
#include "entity/entity.h"
#include "entity/pvp.h"
#include "entity/PowerInfo.h"
#include "entity/character_level.h"
#include "gameData/raidstruct.h"
#include "entity/costume.h"
#include "comm_game.h"
#include "entity/character_net.h"
#include "entity/boostset.h"
#include "entity/character_combat.h"
#include "dbcomm/logcomm.h"
#include "MARTY.h"

static void findEntOrRelayInternal(ClientLink *admin,int db_id,char *orig_cmd,int force,int *result)
{
    // This is called if the player is not on this map, it asks the dbserver to relay the message if either he's online or force==1
    if (isBadEntity(db_id)) {
        // This is the db id of the most recent entity that was sent to this map, and it
        // was found to be bad/corrupted, so we didn't load it.  We must discard
        // the relay command otherwise it would loop forever, pingponging with the
        // DbServer
        if (result) {
            *result = 0;
        }
    } else {
        static PerformanceInfo* perfInfo;
        Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD_BYENT);
        static char    *buf = NULL;
        int        ret;

        estrClear(&buf);
        pktSendBitsPack(pak,1,db_id);
        pktSendBitsPack(pak,1,force);
        estrConcatf(&buf,"cmdrelay\n%s",orig_cmd);
        pktSendString(pak,buf);
        pktSend(&pak,&db_comm_link);
        // clear variables that will hold response
        relay_response_map_id = relay_response_succeeded = 0;
        if (result) {
            // Wait for response (only if synchronous), then return whether the guy was online or offline
            ret = dbMessageScanUntil(__FUNCTION__, &perfInfo);
            if (ret)
            {
                // Got a response
                if (relay_response_succeeded) {
                    *result = 1;
                } else {
                    *result = 0;
                }
            } else {
                *result = 0;
            }
        }
    }
}

// Passing a pointer to "result" will cause it to synchronously do the operation and give feedback if the entity isn't online
Entity *findEntOrRelayOfflineById(ClientLink *admin,int db_id,char *orig_cmd, int *result)
{
    Entity    *e;

    e = entFromDbIdEvenSleeping(db_id);
    if (e && e->owned) {
        return e;
    }

    findEntOrRelayInternal(admin,db_id,orig_cmd, 1, result);
    return NULL;
}

// Passing a pointer to "result" will cause it to synchronously do the operation and give feedback if the entity isn't online
Entity *findEntOrRelayById(ClientLink *admin,int db_id,char *orig_cmd, int *result)
{
    Entity    *e;

    e = entFromDbIdEvenSleeping(db_id);
    if (e && e->owned) {
        if(result)
            *result = true;
        return e;
    }

    findEntOrRelayInternal(admin,db_id,orig_cmd, 0, result);
    return NULL;
}

// Passing a pointer to "result" will cause it to synchronously do the operation and give feedback if the entity isn't online
Entity *findEntOrRelay(ClientLink *admin,char *player_name,char *orig_cmd, int *result)
{
    int        db_id;
    Entity    *e;

    db_id = dbPlayerIdFromName(player_name);
    if (db_id < 0)
    {
        conPrintf(admin, clientPrintf(admin,"PlayerDoesntExist",player_name));
        return 0;
    }
    e = findEntOrRelayById(admin,db_id,orig_cmd,result);
    if (result && !*result) // We sychonously got a result back
    {
        conPrintf(admin,clientPrintf(admin,"PlayerOffline",player_name));
    }
    return e;
}

int relayCommandStart(ClientLink *admin, int db_id, char *orig_cmd, Entity** ep, int* online)
{
    assert(db_id > 0);
    *ep = findEntOrRelayOfflineById(admin, db_id, orig_cmd, NULL);
    *online = 1; // We don't know at this point whether he was demand loaded for a relay command or not
    return *ep != NULL;
}

void relayCommandEnd(Entity** ep, int* online)
{
    assert(*online);
}

void sendToServer(int map_id, char* msg)
{
    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD);

    pktSendBitsPack(pak,1,map_id);
    pktSendString(pak,msg);
    pktSend(&pak,&db_comm_link);
}

void sendToAllServers(char *msg)
{
    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD);

    pktSendBitsPack(pak,1,-1);
    pktSendString(pak,msg);
    pktSend(&pak,&db_comm_link);
}

void sendToAllGroupMembers(int db_id, int group_type, char *msg)
{
    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_RELAY_CMD_TOGROUP);

    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,group_type);
    pktSendString(pak,msg);
    pktSend(&pak,&db_comm_link);
}

static void spawnCalcEntityPosition(ClientLink* client, Mat4 mat)
{
    float y;
    Vec3 direction;
    copyMat4(ENTMAT(clientGetViewedEntity(client)), mat);

    // find direction the player is pointing
    getMat3YPR(mat, direction);
    y = direction[1];
    direction[0] = 0;
    direction[1] = 0;
    direction[2] = 0;

    // rotate along xz plane
    direction[0] = sin(y);
    direction[2] = cos(y);

    scaleVec3(direction, 20, direction);

    addVec3(mat[3], direction, mat[3]);

    {
        static int offby = 0.0;
        offby = (offby + 1) % (11 * 11);

        mat[3][0] += offby%11 - 5;
        mat[3][2] += offby/11 - 5;
    }
}


void spawnVillain(char* spawnParams, ClientLink* client, int count){
    Mat4 mat;
    float angle = 0, distance = 0;

    // Copy the string out of spawnParams, because that's just tmp_str in serverExecCmd,
    // and we're going to recurse into that function later, where the tmp_str variables will
    // be overwritten.
    char *spawnParamsEString = estrCloneCharString(spawnParams);

    spawnCalcEntityPosition(client, mat);

    while(count-- > 0){
        // TODO!!!
        // This is dumb.  It knows exactly what to do now.  Move the actual functionality out of cmdDebug.c so that it can be called
        // directly.
        char buffer[1024];
        sprintf(buffer,
            "entcontrol -1 spawnVillain %f %f %f %s",
            mat[3][0] + distance * sin(angle),
            mat[3][1],
            mat[3][2] + distance * cos(angle),
            spawnParamsEString);

        serverParseClient(buffer, client);

        angle += RAD(30 + rand() % 20);
        distance += 1;
    }

    estrDestroy(&spawnParamsEString);
}

void spawnPet(char* spawnParams, ClientLink* client, int count){
    Mat4 mat;
    float angle = 0, distance = 0;

    spawnCalcEntityPosition(client, mat);

    while(count-- > 0){
        // TODO!!!
        // This is dumb.  It knows exactly what to do now.  Move the actual functionality out of cmdDebug.c so that it can be called
        // directly.
        char buffer[1024];
        sprintf(buffer,
            "entcontrol -1 spawnPet %f %f %f %s",
            mat[3][0] + distance * sin(angle),
            mat[3][1],
            mat[3][2] + distance * cos(angle),
            spawnParams);

        serverParseClient(buffer, client);

        angle += RAD(30 + rand() % 20);
        distance += 1;
    }
}

void spawnNPC(char* spawnParams, ClientLink* client, int count){
    Mat4 mat;
    float angle = 0, distance = 0;

    spawnCalcEntityPosition(client, mat);

    while(count-- > 0){
        // TODO!!!
        // This is dumb.  It knows exactly what to do now.  Move the actual functionality out of cmdDebug.c so that it can be called
        // directly.
        char buffer[1024];
        sprintf(buffer,
            "entcontrol -1 spawnNPC %f %f %f \"%s\"",
            mat[3][0] + distance * sin(angle),
            mat[3][1],
            mat[3][2] + distance * cos(angle),
            spawnParams);

        serverParseClient(buffer, client);

        angle += RAD(30 + rand() % 20);
        distance += 1;
    }

}

static const char* pickRandomVillain(int level, int attempts)
{
    int numVillains = eaSize(&villainDefList.villainDefs);
    do 
    {
        int random_i = rand()%numVillains;
        bool matched = false;
        int j;

        for(j = 0; j < eaSize(&villainDefList.villainDefs[random_i]->levels); j++)
        {
            if(level == villainDefList.villainDefs[random_i]->levels[j]->level)
            {
                matched = true;
                break;
            }
        }

        if ( matched )
        {
            return villainDefList.villainDefs[random_i]->name;
            break;
        }

    } while ( attempts-- > 0 );

    return NULL; // in case couldn't find acceptable in number of match attempts
}

// spawn a mob of random villains within the given level range
int  spawnVillainMob(int count, int min_level, int max_level, ClientLink* client){
    Mat4 mat;
    float angle = 0, distance = 0;
    int num_spawns = 0;

    min_level = CLAMP( min_level, 1, 60 );
    max_level = CLAMP( max_level, 1, 60 );
    if (min_level > max_level)
    {
        int tmp = max_level;
        max_level = min_level;
        min_level = tmp;
    }

    spawnCalcEntityPosition(client, mat);

    while(count-- > 0){
        const char* name;
        int random_lvl = min_level;
        if(max_level > min_level) random_lvl += rand() % (max_level-min_level);
        name = pickRandomVillain(random_lvl, 1000);
        if (name)    // didn't find an acceptable villain in the number of attempts, @todo should just retry?
        {
            // TODO!!!
            // This is dumb.  It knows exactly what to do now.  Move the actual functionality out of cmdDebug.c so that it can be called
            // directly.
            char buffer[1024];
            sprintf(buffer,
                "entcontrol -1 spawnVillain %f %f %f %s %d",
                mat[3][0] + distance * sin(angle),
                mat[3][1],
                mat[3][2] + distance * cos(angle),
                name,
                random_lvl);

//            printf("%s\n", buffer);    // leave a record in the console for testing purposes
            serverParseClient(buffer, client);

            angle += RAD(30 + rand() % 20);
            distance += 1;
            num_spawns++;
        }
    }
    return num_spawns;
}

void spawn(char* spawnParams, ClientLink* client, int count){
    Mat4 mat;
    float angle = 0, distance = 0;

    spawnCalcEntityPosition(client, mat);

    while(count-- > 0){
        // TODO!!!
        // This is dumb.  It knows exactly what to do now.  Move the actual functionality out of cmdDebug.c so that it can be called
        // directly.
        char buffer[1024];
        sprintf(buffer,
                "entcontrol -1 spawn %f %f %f \"%s\"",
                mat[3][0] + distance * sin(angle),
                mat[3][1],
                mat[3][2] + distance * cos(angle),
                spawnParams);

        serverParseClient(buffer, client);

        angle += RAD(30 + rand() % 20);
        distance += 1;
    }
}

void csrBanChat(char *player_name,char *orig_cmd,int min, int admin_auth_id, int admin_db_id)
{
    const char * handle = getHandleFromString(player_name);
    char buf[1000];
    if(handle)
    {
        if(chatServerRunning())
        {
            sprintf(buf, "CsrSilence \"%s\" %d", escapeString(handle), min);
            shardCommSendInternal(admin_auth_id, buf);
        }
        else
            chatSendToPlayer(admin_db_id, "ChatServerDownCannotBan", INFO_USER_ERROR, 0);
    }
    else
    {
        int result = 0;
        Entity *e = findEntOrRelay(NULL,player_name,orig_cmd,&result);
        if (e)
        {
            int time;
            char * unit;

            e->chat_ban_expire = dbSecondsSince2000() + 60 * min;

            if(min < (24 * 60))
            {
                time = min;
                unit = "MinuteString";
            }
            else
            {
                time = min / (24 * 60);
                unit = "DayString";
            }

            chatSendToPlayer(e->db_id, localizedPrintf(e,"YouAreChatBanned", time, unit),INFO_ADMIN_COM,0);
            chatSendToPlayer(admin_db_id, localizedPrintf(e,"PlayerChatBanned", e->name, time, unit), INFO_SVR_COM,0);

            if(chatServerRunning())
            {
                sprintf(buf, "CsrSilence \"%s\" %d", escapeString(e->pl->chat_handle), min);
                shardCommSendInternal(admin_auth_id, buf);
            }
            else
                chatSendToPlayer(admin_db_id, "ChatServerDownCannotBan", INFO_USER_ERROR, 0);
        }
    }
}
void csrKick(ClientLink *admin,char *player_name,char *orig_cmd,char *reason,int ban)
{
    ClientLink    *kickclient;
    int result;
    Entity *e = findEntOrRelay(admin,player_name,orig_cmd,&result);

    if (e && e->client)
    {
        kickclient = clientFromEnt(e);
        svrForceLogoutLink(kickclient->link,reason,ban ? KICKTYPE_BAN : KICKTYPE_KICK);
    }
}


void csrBan(ClientLink *admin,char *player_name,char *orig_cmd,char *reason)
{
    Entity *e;
    int online;
    int db_id = dbPlayerIdFromName(player_name);

    if(db_id<=0)
    {
        conPrintf(admin, clientPrintf(admin, "PlayerDoesntExist", player_name));
        return;
    }

    // send message to player issuing the command
    // Do this *before* the kick since doing the kick may invalidate admin*
    if(    admin
        && admin->entity
        && admin->entity->db_id != db_id)
    {
        conPrintf(admin, clientPrintf(admin,"PlayerBanned", player_name));
    }

    if(relayCommandStart(admin, db_id, orig_cmd, &e, &online))
    {
        if(e && e->pl)
        {
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:Ban %s", reason);
            e->pl->banned = 1;
        }

        relayCommandEnd(&e, &online);

        if(online)
            csrKick(admin, player_name, orig_cmd, reason, 1);
    }
}


void csrUnban(ClientLink *admin,char *player_name,char *orig_cmd)
{
    Entity *e;
    int online;
    int db_id = dbPlayerIdFromName(player_name);

    if(db_id<=0)
    {
        conPrintf(admin, "Player '%s' isn't in the database", player_name);
        return;
    }

    if(relayCommandStart(admin, db_id, orig_cmd, &e, &online))
    {
        if(e && e->pl)
        {
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:Unban");
            e->pl->banned = 0;
        }

        relayCommandEnd(&e, &online);
    }

    // send message to player issuing the command
    if(    admin
        && admin->entity
        && admin->entity->db_id != db_id)
    {
        conPrintf(admin, clientPrintf(admin,"PlayerUnBanned", player_name));
    }
}

void csrInvincible(ClientLink *client,int invincible_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e){
        e->invincible = invincible_state;

        conPrintf(client, clientPrintf(client,"YouAreInvincible", e->invincible ? "NowString" : "NoLonger"));

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf(client->slave,"YouAreInvincible", e->invincible ? "NowString" : "NoLonger"));
        }
    }
}

void csrUnstoppable(ClientLink *client,int unstoppable_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e){
        e->unstoppable = unstoppable_state;

        conPrintf(client, clientPrintf(client,"YouAreUnstoppable", (e->unstoppable?"NowString" : "NoLonger")));

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf(client->slave,"YouAreUnstoppable", e->unstoppable ? "NowString" : "NoLonger"));
        }
    }
}

void csrDoNotTriggerSpawns(ClientLink *client,int donottriggerspawns_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e)
    {
        e->doNotTriggerSpawns = donottriggerspawns_state;

        conPrintf(client, clientPrintf(client,"YouDoNotTriggerSpawns", (e->doNotTriggerSpawns?"NowString" : "NoLonger")));

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf(client->slave,"YouDoNotTriggerSpawns", e->doNotTriggerSpawns ? "NowString" : "NoLonger"));
        }
    }
}

void csrAlwaysHit(ClientLink *client,int alwayshit_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e){
        e->alwaysHit = alwayshit_state;

         conPrintf(client, localizedPrintf( e,(e->alwaysHit? "AlwaysHitOn" : "AlwaysHitOff") ));

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf( e,e->alwaysHit? "AlwaysHitOn" : "AlwaysHitOff"));
        }
    }
}

void csrUntargetable(ClientLink *client,int untargetable_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e){
        e->untargetable = untargetable_state;
        if( e->untargetable & UNTARGETABLE_DBFLAG )
            e->admin = 1;
        else
            e->admin = 0;

        conPrintf(client, localizedPrintf(e,e->untargetable ? "YouAreUnTargetable" : "YouAreTargetable" ));

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf(e,e->untargetable ? "YouAreUnTargetable" : "YouAreTargetable"));
        }
    }
}

void csrPvPSwitch(ClientLink *client,int pvpswitch_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e && e->pl){
        pvp_SetSwitch(e, pvpswitch_state);

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf(e,e->pl->pvpSwitch ? "YourPvPSwitchIsOn" : "YourPvPSwitchIsOff" ));
        }
    }
}

void csrPvPActive(ClientLink *client,int pvpactive_state)
{
    Entity* e = clientGetControlledEntity(client);

    if(e && e->pchar){
        e->pchar->bPvPActive = pvpactive_state;
        e->pvp_update = 1;

        conPrintf(client, localizedPrintf(e,e->pchar->bPvPActive ? "YouAreNowPvPActive" : "YouAreNowPvPInactive" ));

        if(client->slave){
            conPrintf(client->slave->client, localizedPrintf(e,e->pchar->bPvPActive ? "YouAreNowPvPActive" : "YouAreNowPvPInactive" ));
        }
    }
}

void csrServerWho(ClientLink *client,int status_server_id)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_WHO);

    pktSendString(pak,"serverwho");
    pktSendBitsPack(pak,1,status_server_id);
    pktSend(&pak,&db_comm_link);
    dbMessageScanUntil("serverwho", &perfInfo);
    conPrintf(client,"%s",db_state.who_msg);
}

void csrWho(ClientLink *client,char *player_name)
{
    static PerformanceInfo* perfInfo;

    Entity *e;
    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_WHO);

    pktSendString(pak,"who");
    pktSendString(pak,(char*)escapeString(player_name));
    pktSend(&pak,&db_comm_link);
    dbMessageScanUntil("who", &perfInfo);

    conPrintf(client,"%s",db_state.who_msg);
    e = entFromDbId(dbPlayerIdFromName(player_name));
    if (e) {
        ClientLink *client_target= clientFromEnt(e);
        if (client_target) {
            conPrintf(client, "%s %s\n", localizedPrintf(e,"IPAddress"), makeIpStr(client_target->link->addr.sin_addr.S_un.S_addr));
            conPrintf(client, "%1.2f %1.2f %1.2f\n", ENTPOSPARAMS(e));
        }
    }
}

void csrWhoAll(ClientLink *client)
{
    int        i;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];

     conPrintf(client,"\n%s", clientPrintf(client,"PlayersOnMap") );
    for(i=0;i<players->count;i++)
    {
        Entity    *e = players->ents[i];

        if( e->pl->hidden&(1<<HIDE_SEARCH))
        {
            if( client->entity->access_level > 0 )
                conPrintf(client, clientPrintf(client,"HiddenBracket", e->name,e->pchar->iLevel+1,character_CalcCombatLevel(e->pchar)+1,
                            e->pchar->porigin->pchDisplayName,e->pchar->pclass->pchDisplayName) );
        }
        else
            conPrintf(client,clientPrintf(client,"WhoAllChar",e->name,e->pchar->iLevel+1,character_CalcCombatLevel(e->pchar)+1,
                        e->pchar->porigin->pchDisplayName,e->pchar->pclass->pchDisplayName) );
    }
}

void csrSupergroupWho(ClientLink *client,char *group_name)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_WHO);

    pktSendString(pak,"supergroupwho");
    pktSendString(pak,(char*)escapeString(group_name));
    pktSend(&pak,&db_comm_link);
    dbMessageScanUntil("supergroupwho", &perfInfo);
    conPrintf(client,"%s",db_state.who_msg);
}

void csrGroupWho(ClientLink *client, char *whocmd, int group_id)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak = pktCreateEx(&db_comm_link, DBCLIENT_WHO);
    pktSendString(pak, whocmd);
    pktSendBitsPack(pak, 1, group_id);
    pktSend(&pak, &db_comm_link);

    dbMessageScanUntil(whocmd, &perfInfo);
    conPrintf(client, "%s", db_state.who_msg);
}

void csrCsr(ClientLink *admin, int effective_accesslevel, int return_dbid, char *player_name, char *cmd, char *orig_cmd)
{
    // admin is most likely (certainly?) NULL
    Entity* ent = findEntOrRelay( NULL, player_name, orig_cmd, NULL);

    if (ent) {
        int old_accesslevel;
        if (!ent->client) {
            // No clientlink?  Do nothing
            return;
        }
        old_accesslevel = ent->access_level;
        ent->access_level = effective_accesslevel;
        if (ent->client->message_dbid == 0)
            ent->client->message_dbid = return_dbid;
        serverParseClient(cmd, ent->client);
        if (ent->client->message_dbid == return_dbid)
            ent->client->message_dbid = 0;
        if (ent->access_level == effective_accesslevel) {
            // If it didn't change (note that this doesn't allow you to up someone's access level
            //  to your access level, only 1 less than your level)
            ent->access_level = old_accesslevel;
        }
    }
}

void csrCsrOffline(ClientLink *admin, int effective_accesslevel, int return_dbid, int target_dbid, char *cmd, char *orig_cmd)
{
    // admin is most likely (certainly?) NULL
    int result = 0;
    Entity *ent;
    int online;

    if (relayCommandStart(NULL, target_dbid, orig_cmd, &ent, &online))
    {
        int old_accesslevel;
        bool forced_client=false;
        ClientLink clientLink = {0};
        old_accesslevel = ent->access_level;
        ent->access_level = effective_accesslevel;
        if (!ent->client) {
            // Need to have a client with a controlled entity in order to run serverParseClient
            svrClientLinkInitialize(&clientLink, NULL);
            clientLink.entity = ent;
            ent->client = &clientLink;
            forced_client = true;
        }
        if (ent->client->message_dbid == 0)
            ent->client->message_dbid = return_dbid;
        serverParseClient(cmd, ent->client);
        if (ent->client->message_dbid == return_dbid)
            ent->client->message_dbid = 0;
        if (ent->access_level == effective_accesslevel) {
            // If it didn't change (note that this doesn't allow you to up someone's access level
            //  to your access level, only 1 less than your level)
            ent->access_level = old_accesslevel;
        }
        if (forced_client) {
            clientLink.entity = NULL;
            ent->client = NULL;
            svrClientLinkDeinitialize(&clientLink);
        }
        relayCommandEnd(&ent, &online);
    }
}

int csrRadius(ClientLink *admin, float radius, char *cmd)
{
    char buf[1000];
    char cmdBuf[1000];
    Entity * target;
    Entity * nearbyEnts[MAX_ENTITIES_PRIVATE];
    int nearbyCount, i;

    if (!admin || !admin->entity) return 0;

    sprintf(cmdBuf, "%s", cmd);

    nearbyCount = entWithinDistance(ENTMAT(admin->entity), radius, nearbyEnts, ENTTYPE_PLAYER, 0);

    for(i = 0; i < nearbyCount; i++)
    {
        target = nearbyEnts[i];

        if(target && !target->logout_timer)
        {
            conPrintf(admin, "%s", clientPrintf(admin,"CSRRadiusApply", cmdBuf, target->name));
            sprintf(buf, "csr_long %d %d \"%s\" %s", admin->entity->access_level, admin->entity->db_id, target->name, cmdBuf);
            serverParseClient(buf, NULL); // NULL to get internal access level
        }
    }

    return nearbyCount;
}

int csrMap(ClientLink *admin, char *cmd)
{
    char buf[1000];
    char cmdBuf[1000];
    Entity * target;
    int i;

    if (!admin || !admin->entity) return 0;

    sprintf(cmdBuf, "%s", cmd);

    for(i = 0; i < player_ents[PLAYER_ENTS_ACTIVE].count; i++)
    {
        target = player_ents[PLAYER_ENTS_ACTIVE].ents[i];

        if(target && !target->logout_timer)
        {
            conPrintf(admin, "%s", clientPrintf(admin,"CSRRadiusApply", cmdBuf, target->name));
            sprintf(buf, "csr_long %d %d \"%s\" %s", admin->entity->access_level, admin->entity->db_id, target->name, cmdBuf);
            serverParseClient(buf, NULL); // NULL to get internal access level
        }
    }

    return player_ents[PLAYER_ENTS_ACTIVE].count;
}

// utility function used by csrPlayerInfo and others... It obtains a Entity* given a player name.
// If player does not exist, it will output a message to the client's console
Entity * debugEntFromNameOrNotify(ClientLink * client, char * player_name)
{
    Entity * e = entFromDbIdEvenSleeping(dbPlayerIdFromName(player_name));
    if(!e)
    {
        conPrintf(client, "\n%s", clientPrintf(client,"PlayerNotOnMapServer", player_name));
    }
    return e;
}

// get rid of trailing comma (each list element is followed by a comma and a space,
// the last element needs to have these removed
void tidyDebugTextList(char ** estr)
{

    int len = estrLength(estr);
    int pos = len - 2; // location where comma would be
    if((pos >= 0) && ((*estr)[pos] == ','))
    {
        estrRemove(estr, pos, 2);
    }
}


// print out information about a given player (xp level, powers, etc)
char * csrPlayerInfo(char ** estr, ClientLink * client, char *player_name, int detailed)
{
    int i, j, count, category, counter;
    Entity *e        = NULL;
    PowerSet *pPowerSet        = NULL;
    const BasePower *pBasePower    = NULL;
    StoryTaskInfo * pSTI    = NULL;
    char time[128];    // holds time string


    e = debugEntFromNameOrNotify(client, player_name);
    if(e)
    {
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo1"), player_name );
        if(e->corrupted_dont_save)
            estrConcatf(estr, "\n%s", localizedPrintf(e, "entCorrupted", player_name));
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo1a"), e->pl ? e->pl->chat_handle : "N/A" );
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo2"), e->auth_name );
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo3"), (e->pchar && e->pchar->porigin) ? e->pchar->porigin->pchName : "N/A" );
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo4"), (e->pchar && e->pchar->pclass) ? e->pchar->pclass->pchName : "N/A" );
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo14"), e->pl ? (e->pl->playerType ? localizedPrintf(e,"CSRInfo14V") : localizedPrintf(e,"CSRInfo14H")) : "N/A");
        if(e->pl)
            estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo16"), 
                e->pl->praetorianProgress == kPraetorianProgress_Tutorial ? localizedPrintf(e,"CSRInfo16A") :
                e->pl->praetorianProgress == kPraetorianProgress_Praetoria ? localizedPrintf(e,"CSRInfo16B") :
                e->pl->praetorianProgress == kPraetorianProgress_PrimalEarth ? localizedPrintf(e,"CSRInfo16C") :
                e->pl->praetorianProgress == kPraetorianProgress_TravelHero ? localizedPrintf(e, "CSRInfo16E") :
                e->pl->praetorianProgress == kPraetorianProgress_TravelVillain ? localizedPrintf(e, "CSRInfo16F") :
                e->pl->praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial ? localizedPrintf(e, "CSRInfo16G") :
                localizedPrintf(e,"CSRInfo16D"));
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo5"), timerMakeOffsetStringFromSeconds(time, e->total_time) );
        estrConcatf(estr, "\n%s%d/%d ", localizedPrintf(e,"CSRInfo6"), e->pchar ? (e->pchar->iLevel + 1) : 0, (e && e->pchar) ? (e->pchar->iCombatLevel + 1) : 0 );
        estrConcatf(estr, "\n%s%d / %d / %d", localizedPrintf(e,"CSRInfo7"), e->pchar ? e->pchar->iExperiencePoints : 0, (e && e->pchar) ? e->pchar->iExperienceDebt : 0, (e && e->pchar) ? e->pchar->iExperienceRest : 0 );
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo15"), e->pchar ? (e->pchar->playerTypeByLocation ? localizedPrintf(e,"CSRInfo15V") : localizedPrintf(e,"CSRInfo15H")) : "N/A");
        estrConcatf(estr, "\n%s%d ", localizedPrintf(e,"CSRInfo8a"), e->pchar ? e->pchar->iInfluencePoints : 0);
        estrConcatStaticCharArray(estr, "(Influence)");

        estrConcatf(estr, "\n%s ", localizedPrintf(e,"CSRInfo13"));
        if(e->pl)
        {
            for (i = 0; i < 12; i++)
            {
                if (e->pl->respecTokens & (1 << (i * 2)))
                    estrConcatf(estr, " %d", i);
                if (e->pl->respecTokens & (1 << ((i * 2) + 1)))
                    estrConcatStaticCharArray(estr, "(used)");
            }
            counter = (e->pl->respecTokens >> 24);
            estrConcatf(estr, " c %d", counter);
        }
        estrConcatf(estr, "\n%s%s ", localizedPrintf(e,"CSRInfo9"), dbGetMapName(e->map_id) );
        estrConcatf(estr, "\n%s ", localizedPrintf(e,"CSRInfo10") );

        for (i = 0; pSTI = TaskGetMissionTask(e, i); i++)
        {
            estrConcatf(estr, "\t%s\n", pSTI->def->logicalname);
        }
        if (i == 0)
        {
            estrConcatf(estr, "\t%s\n", localizedPrintf(e,"NoneParen"));
        }

        //print out current story arc
        StoryArcDebugShowMineHelper(estr, StoryInfoFromPlayer(e));

        estrConcatf(estr, "\n%s\t", localizedPrintf(e,"CSRInfo11") );

        // build up powerset names... this method assures that powers are listed in order
        // of importance ... ie "primary", "secondary", followed by the power pools
        for (category = 0; e->pchar && category < kCategory_Count; category++)
        {
            int iSizeSets = eaSize(&e->pchar->ppPowerSets);
            // find the power set corresponding to this category
            for( i = 0; i < iSizeSets; i++)
            {
                pPowerSet = e->pchar->ppPowerSets[i];

                if( pPowerSet->psetBase->pcatParent == e->pchar->pclass->pcat[category] )
                {
                    const BasePowerSet *pPowerSetBase = pPowerSet->psetBase;

                    if(!detailed)
                    {
                        // just build up list of power sets
                        estrConcatf(estr, "%s, ", pPowerSetBase->pchName);
                    }
                    else
                    {
                        // build up list of power sets and actual powers
                        int iSizePows = eaSize( &pPowerSetBase->ppPowers );

                        estrConcatf(estr, "\n\t%s: ", pPowerSetBase->pchName);

                        // loop though powers
                        for( j = 0; j < iSizePows; j++ )
                        {
                            if ( character_OwnsPower( e->pchar, pPowerSetBase->ppPowers[j] ) )
                            {
                                estrConcatf(estr, "%s, ", pPowerSetBase->ppPowers[j]->pchName);
                            }
                        }
                        tidyDebugTextList(estr);
                    }
                }
            }
        }

        tidyDebugTextList(estr);

        if(detailed && e->pchar)
        {
        // print out more detailed information

            estrConcatf(estr, "\n\n%s\t", localizedPrintf(e,"CSRInfo11") );

            // build up inventory list
            count = 0;
            for(i = 0; i < CHAR_INSP_MAX_COLS; ++i)
            {
                for(j = 0; j < CHAR_INSP_MAX_ROWS; ++j)
                {
                    pBasePower = e->pchar->aInspirations[i][j];
                    if(pBasePower)
                    {
                        count++;
                        estrConcatf(estr, "%s, ", pBasePower->pchName);
                    }
                }
            }

            if(count == 0)
            {
                estrConcatCharString(estr, localizedPrintf(e, "NoneParen"));
            }

            tidyDebugTextList(estr); // remove trailing comma
        }
    }

    return *estr;
}


// debug only (used in Visual Studio command window)
static int strlenwrap(char * s)
{
    return (int)strlen(s);
}


// prints out a player's specialization/boost/enhancement info
char * csrPlayerSpecializationInfo(char ** estr, ClientLink * client, char * player_name)
{
    int i,j,k;
    int count = 0;
    BOOL bFirst;
    PowerSet **ppPowerSets;
    Power *pPower;
    Boost *pBoost;

    Entity * e = debugEntFromNameOrNotify(client, player_name);

    if(e)
    {
        int iSizeSets;
        estrConcatf(estr, "\n\n%s", localizedPrintf(e,"SpecInfo1") );

        ppPowerSets = e->pchar->ppPowerSets;
        iSizeSets = eaSize(&ppPowerSets);
        for( i = 0; i < iSizeSets; i++)
        {
            int iSizePows = eaSize(&ppPowerSets[i]->ppPowers);
            for( j = 0; j < iSizePows; j++)
            {
                pPower = ppPowerSets[i]->ppPowers[j];
                bFirst = TRUE;
                if(pPower)
                {
                    int iSizeBoosts = eaSize(&pPower->ppBoosts);
                    for(k = 0; k < iSizeBoosts; k++)
                    {
                        pBoost = pPower->ppBoosts[k];
                        if(pBoost)
                        {
                            int icost = 0;
                            const StoreItem *psi = store_FindItem(pBoost->ppowBase, pBoost->iLevel+pBoost->iNumCombines);
                            if(psi)
                            {
                                icost = psi->iSell;
                            }

                            if(bFirst)
                            {
                                estrConcatf(estr, "\n\t%s.%s.%s:\n",
                                    pPower->ppowBase->psetParent->pcatParent->pchName,
                                    pPower->ppowBase->psetParent->pchName,
                                    pPower->ppowBase->pchName);
                                bFirst = FALSE;
                            }

                            count++;
                            estrConcatf(estr, "\t\t%s.%s.%d+%d (cost: %d),\n",
                                pBoost->ppowBase->psetParent->pchName,
                                pBoost->ppowBase->pchName,
                                pBoost->iLevel+1,
                                pBoost->iNumCombines,
                                icost);
                        }
                    }
                    tidyDebugTextList(estr);
                }
            }
        }

        // now, interate through all unassigned boosts
        for(i = 0; i < CHAR_BOOST_MAX; i++)
        {
            pBoost = e->pchar->aBoosts[i];
            if(pBoost)
            {
                count++;
                estrConcatf(estr, "%s  %s.%s.%s.%d",
                localizedPrintf(e,"Unassigned"),
                pBoost->ppowBase->psetParent->pcatParent->pchName,
                pBoost->ppowBase->psetParent->pchName,
                pBoost->ppowBase->pchName,
                pBoost->iLevel);
            }
        }

        //if no boosts exist, then set string accordingly
        if(count == 0)
        {
            estrConcatf(estr, "\n\t%s", localizedPrintf(e,"NoneParen"));
        }
    }

    return *estr;
}


// prints out a player's current tray contents
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
char * csrPlayerTrayInfo(char ** estr, ClientLink * client, char *player_name)
{
    int i;

    Entity * e = debugEntFromNameOrNotify(client, player_name);

    if(e && e->pchar && e->pl)
    {
        estrConcatf(estr, "\n\n%s", localizedPrintf(e,"TrayInfo"));

        for(i=0; i < TRAY_SLOTS; i++)
        {
            TrayObj * pObj = getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, e->pl->tray->current_trays[kCurTrayType_Primary], i, e->pchar->iCurBuild, 0);

            if(!pObj || pObj->type == kTrayItemType_None)
            {
                estrConcatf(estr, "\n\t%d\t%s", i+1, localizedPrintf(e,"EmptyParen"));
            }            
            else if( IS_TRAY_MACRO(pObj->type) )
            {
                estrConcatf(estr, "\n\t%d\t%s %s", i+1, localizedPrintf(e,"MacroParen"), pObj->command);
            }
            else
            {
                estrConcatf(estr, "\n\t%d\t%s", i+1, pObj->pchPowerName);
            }
        }
    }

    return *estr;
}


// prints out a player's current tray contents
char * csrPlayerTeamInfo(char ** estr, ClientLink * client, char *player_name)
{
    Entity * e = debugEntFromNameOrNotify(client, player_name);
    StoryTaskInfo * pSTI = NULL;

    if(e)
    {
        estrConcatf(estr, "\n\n%s", localizedPrintf(e,"TeamInfo"));

        if(e->teamup_id == 0)
        {
            estrConcatf(estr, "\n\t%s", localizedPrintf(e,"Soloer"));
        }
        else
        {
            static PerformanceInfo* perfInfo;

            // teamup info
            Packet * pak = pktCreateEx(&db_comm_link,DBCLIENT_WHO);
            pktSendString(pak,"teamupwho");
            pktSendBitsPack(pak,1,e->teamup_id);
            pktSend(&pak,&db_comm_link);
            dbMessageScanUntil("teamupwho2", &perfInfo);
            estrConcatCharString(estr, db_state.who_msg);


            // additional teamup mission info
            pSTI = e->teamup->activetask;
            if(pSTI->sahandle.context)
            {
                const StoryTask * pST = TaskDefinition(&pSTI->sahandle);
                if(pST)
                {
                    estrConcatf(estr, "%s %s\n", localizedPrintf(e,"TeamInfo2"), pST->logicalname );
                    estrConcatf(estr, "%s %d\n", localizedPrintf(e,"TeamInfo3"), e->teamup->map_id );
                    estrConcatf(estr, "%s %s\n", localizedPrintf(e,"TeamInfo4"), e->teamup->mission_status );
                    estrConcatf(estr, "%s %d\n", localizedPrintf(e,"TeamInfo5"), e->teamup->map_playersonmap );
                }
            }
        }
    }
    return *estr;
}

void csrPetition(Entity *e, char *msg)
{
    char    *str;
    char *pch;
    char *pchDesc;
    char *pchEnd;
    int iCat;
    char *errorMsg = "InternalPetitionCommand";
    char *successMsg = "PetitionSent";

    // "<cat=5> <summary=xxx> <desc=yyy>"
    // xxx and yyy won't have any \r, \n, <, or >s in them.
    if(!(pch=strstr(msg, "<cat="))) {
        sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_USER_ERROR, 0);
        return;
    }
    iCat=atoi(pch+5);

    if(!(pch=strstr(msg, "<summary="))) {
        sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_USER_ERROR, 0);
        return;
    }
    pch+=strlen("<summary=");
    if(!(pchEnd=strchr(pch, '>'))) {
        sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_USER_ERROR, 0);
        return;
    }
    *pchEnd = 0;

    if(!(pchDesc=strstr(pchEnd+1, "<desc="))) {
        sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_USER_ERROR, 0);
        return;
    }
    pchDesc+=strlen("<desc=");
    if(!(pchEnd=strchr(pchDesc, '>'))) {
        sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_USER_ERROR, 0);
        return;
    }
    *pchEnd = 0;

    str = packagePetition(e,iCat,pch,pchDesc);
    dbAsyncContainerUpdate(CONTAINER_PETITIONS,-1,CONTAINER_CMD_CREATE,str,0);
    free(str);

    sendChatMsg(e, localizedPrintf(e, successMsg), INFO_SVR_COM, 0);
}

void csrPowersCashInEnhancements(ClientLink *client)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        int i, j, k;
        PowerSet **ppPowerSets;
        int iSizeSets;
        int icnt=0;
        int icost=0;

        ppPowerSets = e->pchar->ppPowerSets;
        iSizeSets = eaSize(&ppPowerSets);
        for(i = 0; i < iSizeSets; i++)
        {
            int iSizePows = eaSize(&ppPowerSets[i]->ppPowers);
            for(j = 0; j < iSizePows; j++)
            {
                Power *pPower = ppPowerSets[i]->ppPowers[j];
                if(pPower)
                {
                    int iSizeBoosts = eaSize(&pPower->ppBoosts);
                    for(k = 0; k < iSizeBoosts; k++)
                    {
                        Boost *pBoost = pPower->ppBoosts[k];
                        if(pPower->ppBoosts[k])
                        {
                            const StoreItem *psi = store_FindItem(pBoost->ppowBase, pBoost->iLevel+pBoost->iNumCombines);
                            if(psi)
                            {
                                ent_AdjInfluence(e, psi->iSell, NULL);
                                icnt++;
                                icost += psi->iSell;
                            }
                            else
                            {
                                conPrintf(client, localizedPrintf(e,"CantFindPrice", pBoost->ppowBase->pchDisplayName) );
                            }
                            power_DestroyBoost(pPower, k, "cashin");
                        }
                    }
                }
            }
        }
        eaPush(&e->powerDefChange, (void *)-1);
        conPrintf(client, "%s\n", localizedPrintf(e,"CashedIn",icnt, icost));
        conPrintf(client, "%s\n", localizedPrintf(e,"SendPlayerToStore") );

        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:CashIn Cashed in %d enhancements for %d influence.\n", icnt, icost);
    }
}

static void DisplayPowerSetChoices(ClientLink *client, Entity *e, char *cmd)
{
    int iset;
    int i;

    // Tell them their choices.
    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions1"));
    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions2"));
    for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets)-1; iset>=0; iset--)
    {
        const BasePowerSet *pbaseset = e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets[iset];

        for(i=eaSize(&pbaseset->ppPowers)-1; i>=0; i--)
        {
            if(pbaseset->piAvailable[i]==0)
            {
                conPrintf(client, "    %s %s\n",
                    pbaseset->pchName,
                    pbaseset->ppPowers[i]->pchName);
            }
        }
    }

    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions3"));
    for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets)-1; iset>=0; iset--)
    {
        conPrintf(client, "    %s (%s)\n",
            e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets[iset]->pchName,
            localizedPrintf(e,e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets[iset]->pchDisplayName));
    }
    conPrintf(client, "\n%s\n   %s %s %s %s\n",
        localizedPrintf(client->entity,"client->entity,ResetInstructions4"),
        cmd,
        e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets[0]->pchName,
        e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets[0]->ppPowers[0]->pchName,
        e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets[0]->pchName);
}

void csrPowersReset(ClientLink *client, const char *pchPrimary, const char *pchFirst, const char *pchSecondary)
{
    Entity *e = clientGetControlledEntity(client);
    const char *pchPrimaryFound = NULL;
    const char *pchSecondaryFound = NULL;
    const char *pchFirstFound = NULL;
    const BasePowerSet *psetPrimary;
    const BasePowerSet *psetSecondary;
    const BasePower *ppowFirst = NULL;
    PowerSet *pset;

    int iset;
    int ibuild;

    if(e == NULL || ENTTYPE(e) != ENTTYPE_PLAYER)
        return;

    conPrintf(client, "%s\n",  localizedPrintf(e,"ResetInstructions5",e->name));
    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions6",e->name));
    for(iset=0; iset<eaSize(&e->pchar->ppPowerSets); iset++)
    {
        int ipow;

        if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Inherent")!=0
            && stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Temporary_Powers")!=0)
        {
            conPrintf(client, "   %s (%s)\n", e->pchar->ppPowerSets[iset]->psetBase->pchName, localizedPrintf(e,e->pchar->ppPowerSets[iset]->psetBase->pchDisplayName));
            if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
            {
                if(pchPrimaryFound)
                {
                    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions7"));
                }
                else
                {
                    pchPrimaryFound = e->pchar->ppPowerSets[iset]->psetBase->pchName;
                }
            }
            else if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary])
            {
                if(pchSecondaryFound)
                {
                    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions8"));
                }
                else
                {
                    pchSecondaryFound = e->pchar->ppPowerSets[iset]->psetBase->pchName;
                }
            }
        }


        if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
        {
            for(ipow=eaSize(&e->pchar->ppPowerSets[iset]->ppPowers)-1; ipow>=0; ipow--)
            {
                // Figure out what the first power the bought is.
                if(e->pchar->ppPowerSets[iset]->ppPowers[ipow]->iLevelBought==0)
                {
                    pchFirstFound = e->pchar->ppPowerSets[iset]->ppPowers[ipow]->ppowBase->pchName;
                }
            }
        }
    }

    if(!pchPrimaryFound || !pchSecondaryFound || !pchFirstFound)
    {
        if(pchPrimary==NULL || *pchPrimary=='\0'
            || pchFirst==NULL || *pchFirst=='\0'
            || pchSecondary==NULL || *pchSecondary=='\0')
        {
            DisplayPowerSetChoices(client, e, "powers_reset");
            return;
        }
    }
    else
    {
        conPrintf(client, "\n");

        if(pchPrimary==NULL || *pchPrimary=='\0')
        {
            pchPrimary = pchPrimaryFound;
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions9", pchPrimary));
        }
        else
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions10", pchPrimary));
        }

        if(pchFirst==NULL || *pchFirst=='\0')
        {
            pchFirst = pchFirstFound;
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions11", pchFirst));
        }
        else
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions12",  pchFirst));
        }

        if(pchSecondary==NULL || *pchSecondary=='\0')
        {
            pchSecondary = pchSecondaryFound;
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions13",  pchSecondary));
        }
        else
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions14",  pchSecondary));
        }
    }

    conPrintf(client, "\n");

    if(!(psetPrimary=powerdict_GetBasePowerSetByName(&g_PowerDictionary, e->pchar->pclass->pcat[kCategory_Primary]->pchName, pchPrimary)))
    {
        conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions15",  pchPrimary));
        pchPrimary = NULL;
    }
    else if(!(ppowFirst=baseset_GetBasePowerByName(psetPrimary, pchFirst)))
    {
        conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions16",  pchFirst));
        pchFirst = NULL;
    }

    if(!(psetSecondary=powerdict_GetBasePowerSetByName(&g_PowerDictionary, e->pchar->pclass->pcat[kCategory_Secondary]->pchName, pchSecondary)))
    {
        conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions18",  pchSecondary));
        pchSecondary = NULL;
    }

    if(!pchPrimary || !pchSecondary || !pchFirst)
    {
        DisplayPowerSetChoices(client, e, "powers_reset");
        return;
    }

    //
    // OK, everything checks out. Do the work.
    //

    e->pchar->ppowTried=NULL;
    e->pchar->ppowQueued=NULL;
    e->pchar->ppowDefault=NULL;
    e->pchar->ppowCurrent=NULL;
    //character_EnterStance(e->pchar, NULL, 1); // should this be called instead of just NULL'ing the pointer?  Some code does, some doesn't...
    e->pchar->ppowStance=NULL;

    for (ibuild = 0; ibuild < MAX_BUILD_NUM; ibuild++)
    {
        character_SelectBuildLightweight(e, ibuild);
        csrPowersCashInEnhancements(client);
    }

    character_SelectBuildLightweight(e, 0);

    // First destroy all the old stuff
    costumeUnawardPowersetParts(e, 0);
    character_CancelPowers(e->pchar);

    for (ibuild = 0; ibuild < MAX_BUILD_NUM; ibuild++)
    {
        int stophere = ibuild == 0 ? 0 : g_numSharedPowersets;
        character_SelectBuildLightweight(e, ibuild);
        for(iset = eaSize(&e->pchar->ppPowerSets) - 1; iset >= stophere; iset--)
        {
            powerset_Destroy(e->pchar->ppPowerSets[iset],"reset");
        }
        eaDestroy(&e->pchar->ppPowerSets);

        eaCreate(&e->pchar->ppPowerSets);
        eaSetCapacity(&e->pchar->ppPowerSets, 4);

        // Drop their level back to 0 so they can level up again, AND so buying the following sets occurs at
        //  the proper level
        e->pchar->iLevel = 0;
        e->pchar->iBuildLevels[ibuild] = 0;

        if (ibuild == 0)
        {
            character_GrantAutoIssueBuildSharedPowerSets(e->pchar);
        }
        else
        {
            int j;
            for (j = 0; j < g_numSharedPowersets; j++)
            {
                pset = e->pchar->ppBuildPowerSets[0][j];
                eaPush(&e->pchar->ppPowerSets, pset);
            }
        }

        character_GrantAutoIssueBuildSpecificPowerSets(e->pchar);

        pset = character_BuyPowerSet(e->pchar, psetPrimary);
        character_BuyPower(e->pchar, pset, ppowFirst, 0);

        pset = character_BuyPowerSet(e->pchar, psetSecondary);
        character_BuyPower(e->pchar, pset, psetSecondary->ppPowers[0], 0);

        if (ibuild == 0)
        {
            costumeAwardPowersetParts(e, 1, 0);
        }

        // Calculate vars and fill in arrays as appropriate.
        character_LevelFinalize(e->pchar, true);

        e->pchar->ppBuildPowerSets[ibuild] = e->pchar->ppPowerSets;
    }
    e->pchar->iCurBuild = 0;
    e->pchar->iActiveBuild = 0;
    e->pchar->ppPowerSets = e->pchar->ppBuildPowerSets[0];

    // Populate the the character's attributes with his initial values.
    character_UncancelPowers(e->pchar);

    // Send down a complete power description.
    eaPush(&e->powerDefChange, (void *)-1);

    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions19"));
    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions20"));
    conPrintf(client, "%s\n\n", localizedPrintf(e,"ResetInstructions21"));

    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:PowersReset Primary: %s, First: %s, Secondary: %s", pchPrimary, pchFirst, pchSecondary);

    // Removes the badge that granted the origin power; will be regranted immediately by the badge system
    //  re-rewarding the power.
    badge_Revoke(e, "earlypowergrantbadge");
}

void csrPowersBuyDev(ClientLink *client, char *pchCat, char *pchSet, char *pchPow)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        Power *ppow;
        PowerSet *pset;
        const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, pchCat, pchSet, pchPow);
        if(!ppowBase)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructionsPbuy",pchCat, pchSet, pchPow));
            return;
        }

        if((pset=character_OwnsPowerSet(e->pchar, ppowBase->psetParent))==NULL)
        {
            if((pset=character_BuyPowerSet(e->pchar, ppowBase->psetParent))==NULL)
            {
                return;
            }
        }

        if(character_OwnsPower(e->pchar, ppowBase))
        {
            return;
        }

        if ((ppow = character_BuyPower(e->pchar, pset, ppowBase, 0)) != NULL)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions26",
                ppow->ppowBase->psetParent->pcatParent->pchName,
                ppow->ppowBase->psetParent->pchName,
                ppow->ppowBase->pchName,
                e->name));
            dbLog("CSR:PowersBuyDev", e, "%s.%s.%s",
                ppow->ppowBase->psetParent->pcatParent->pchName,
                ppow->ppowBase->psetParent->pchName,
                ppow->ppowBase->pchName);
            // This forces a full redefine to be sent.
            eaPush(&e->powerDefChange, (void *)-1);

            if (ppow->ppowBase->eType == kPowerType_Auto || ppow->ppowBase->eType == kPowerType_GlobalBoost)
            {
                character_ActivateAllAutoPowers(e->pchar);
            }
        }
    }
}

void csrPowersDebug(ClientLink *client, char *pchCat, char *pchSet, char *pchPow)
{
    const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, pchCat, pchSet, pchPow);
    Entity *e = clientGetControlledEntity(client);

    if( ppowBase && e )
    {
        START_PACKET(pak, e, SERVER_SETDEBUGPOWER );
        basepower_Send(pak, ppowBase);
        END_PACKET;
    }
}


void csrPowersBuy(ClientLink *client, char *pchCat, char *pchSet, char *pchPow)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        Power *ppow;
        PowerSet *pset;
        const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, pchCat, pchSet, pchPow);
        if(!ppowBase)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructionsPbuy",pchCat, pchSet, pchPow));
            return;
        }

        if((pset=character_OwnsPowerSet(e->pchar, ppowBase->psetParent))==NULL)
        {
            csrPowersBuySet(client, pchCat, pchSet);

            if((pset=character_OwnsPowerSet(e->pchar, ppowBase->psetParent))==NULL)
            {
                return;
            }
        }

        if(character_CanBuyPower(e->pchar))
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions22", e->name, e->pchar->iLevel+1));
            return;
        }

        if(character_OwnsPower(e->pchar, ppowBase))
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions24",
                e->name,
                ppowBase->psetParent->pcatParent->pchName,
                ppowBase->psetParent->pchName,
                ppowBase->pchName));
            return;
        }

        if(!character_IsAllowedToHavePowerHypothetically(e->pchar, ppowBase))
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions25",
                e->name,
                ppowBase->psetParent->pcatParent->pchName,
                ppowBase->psetParent->pchName,
                ppowBase->pchName));
            return;
        }

        if ((ppow = character_BuyPower(e->pchar, pset, ppowBase, 0)) != NULL)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions26",
                ppow->ppowBase->psetParent->pcatParent->pchName,
                ppow->ppowBase->psetParent->pchName,
                ppow->ppowBase->pchName,
                e->name));
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:PowersBuy %s.%s.%s",
                ppow->ppowBase->psetParent->pcatParent->pchName,
                ppow->ppowBase->psetParent->pchName,
                ppow->ppowBase->pchName);
        }
    }
}

void csrPowersBuySet(ClientLink *client, char *pchCat, char *pchSet)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        int iset;
        const BasePowerSet *psetPrimary = NULL;
        const BasePowerSet *psetSecondary = NULL;

        PowerSet *pset;
        const BasePowerSet *psetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, pchCat, pchSet);

        for(iset=eaSize(&e->pchar->ppPowerSets)-1; iset>=0; iset--)
        {
            if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Inherent")!=0
                && stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Temporary_Powers")!=0)
            {
                if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
                {
                    psetPrimary = e->pchar->ppPowerSets[iset]->psetBase;
                }
                else if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary])
                {
                    psetSecondary = e->pchar->ppPowerSets[iset]->psetBase;
                }
            }
        }

        if(!psetBase)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions27", pchCat, pchSet));
            if(!psetPrimary)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions28"));
                for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets)-1; iset>=0; iset--)
                {
                    conPrintf(client, "   /powers_buyset %s %s\n",
                        e->pchar->pclass->pcat[kCategory_Primary]->pchName,
                        e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets[iset]->pchName);
                }
            }
            if(!psetSecondary)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions53") );
                for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets)-1; iset>=0; iset--)
                {
                    conPrintf(client, "   /powers_buyset %s %s\n",
                        e->pchar->pclass->pcat[kCategory_Secondary]->pchName,
                        e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets[iset]->pchName);
                }
            }
            if(character_CanBuyPoolPowerSet(e->pchar))
            {
                conPrintf(client,"%s\n", localizedPrintf(e,"ResetInstructions29"));
                for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Pool]->ppPowerSets)-1; iset>=0; iset--)
                {
                    conPrintf(client, "   /powers_buyset %s %s\n",
                        e->pchar->pclass->pcat[kCategory_Pool]->pchName,
                        e->pchar->pclass->pcat[kCategory_Pool]->ppPowerSets[iset]->pchName);
                }
            }
            if(character_CanBuyEpicPowerSet(e->pchar))
            {
                conPrintf(client,"%s\n", localizedPrintf(e,"ResetInstructions29_5"));
                for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Epic]->ppPowerSets)-1; iset>=0; iset--)
                {
                    conPrintf(client, "   /powers_buyset %s %s\n",
                        e->pchar->pclass->pcat[kCategory_Epic]->pchName,
                        e->pchar->pclass->pcat[kCategory_Epic]->ppPowerSets[iset]->pchName);
                }
            }
        }

        if(psetPrimary
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Primary])
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions30", e->name, psetPrimary->pchName));
            return;
        }

        if(psetSecondary
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Secondary])
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions31", e->name, psetSecondary->pchName));
            return;
        }

        if(character_CanBuyPoolPowerSet(e->pchar)
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Pool])
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions32", e->name, e->pchar->iLevel+1));
            return;
        }

        if(character_CanBuyEpicPowerSet(e->pchar)
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Epic])
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions32_5", e->name, e->pchar->iLevel+1));
            return;
        }

        if(character_OwnsPowerSet(e->pchar, psetBase))
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions33",
                e->name,
                psetBase->pcatParent->pchName,
                psetBase->pchName));
            return;
        }

        if(psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Primary]
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Secondary]
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Pool]
            && psetBase->pcatParent != e->pchar->pclass->pcat[kCategory_Epic])
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions34",
                e->name, e->pchar->pclass->pchName, pchCat, pchSet));
            return;
        }

        if((pset=character_BuyPowerSet(e->pchar, psetBase))!=NULL)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions35",
                psetBase->pcatParent->pchName,
                psetBase->pchName,
                e->name));
            dbLog("CSR:PowersBuySet", e, "%s.%s",
                psetBase->pcatParent->pchName,
                psetBase->pchName);
        }
    }
}


void csrPowersCheck(ClientLink *client)
{
    bool bUseTrainer = false;
    bool bUsePowersReset = false;
    bool bUseBoostReset = false;
    bool bFixedInherents = false;

    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        int i;
        int iset;
        int iInfluenceCost = 0;
        PowerSet *psetInherent = NULL;
        PowerSet *psetTemp = NULL;
        PowerSet *psetAccolades = NULL;
        PowerSet *psetPrimary = NULL;
        PowerSet *psetSecondary = NULL;
        Power *ppowPrimaryFirst = NULL;

         conPrintf(client, "\n\n%s\n*****\n", localizedPrintf(e,"ResetInstructions36", e->name, e->db_id, db_state.server_name) );
        if(e->corrupted_dont_save) conPrintf(client, "   %s\n", localizedPrintf(e, "entCorrupted", e->name));

        for(iset=eaSize(&e->pchar->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pchName, "Inherent")==0)
            {
                psetInherent = e->pchar->ppPowerSets[iset];
            }
            else if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Inherent")==0)
            {
                // TODO: Test something about other inherent powersets (fitness) powers here?
            }
            else if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pchName, "Temporary_Powers")==0)
            {
//                 if(iset!=0)
//                 {
//                     conPrintf(client, localizedPrintf(e,"ResetInstructions37"));
//                     bUsePowersReset = true;
//                 }
                psetTemp = e->pchar->ppPowerSets[iset];
            }
            else if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pchName, "Accolades")==0)
            {
                if(psetAccolades) 
                {
                    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions62"));
                    bUsePowersReset = true;
                }
                else 
                {
                    psetAccolades = e->pchar->ppPowerSets[iset];
                }
            }
            else if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
            {
                if(psetPrimary)
                {
                    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions38",
                        e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName,
                        e->pchar->ppPowerSets[iset]->psetBase->pchName));
                    bUsePowersReset = true;
                }
                else
                {
                    psetPrimary = e->pchar->ppPowerSets[iset];
                }
            }
            else if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary])
            {
                if(psetSecondary)
                {
                    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions39",
                        e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName,
                        e->pchar->ppPowerSets[iset]->psetBase->pchName));
                    bUsePowersReset = true;
                }
                else
                {
                    psetSecondary = e->pchar->ppPowerSets[iset];
                }
            }
            else if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Pool])
            {
                // TODO: Test something about pools here?
            }
            else if(e->pchar->ppPowerSets[iset]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Epic])
            {
                // TODO: Test something about epic power sets here?
            }
            else if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Temporary_Powers")==0)
            {
                // TODO: Test something about random temp powers here?
            }
            else if(stricmp(e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName, "Set_Bonus")==0)
            {
                // TODO: Test something about set bonus powers here?
            }
            else
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions40",
                    e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName,
                    e->pchar->ppPowerSets[iset]->psetBase->pchName,
                    e->name,
                    e->pchar->pclass->pchName));

                bUsePowersReset = true;
            }

            for(ipow=eaSize(&e->pchar->ppPowerSets[iset]->ppPowers)-1; ipow>=0; ipow--)
            {
                int iboost;
                Power *ppow = e->pchar->ppPowerSets[iset]->ppPowers[ipow];

                if (!ppow)
                {
                    // This is a valid scenario.  So sayeth the Vince.
                    continue;
                }

                if(!character_IsAllowedToHavePowerHypothetically(e->pchar, ppow->ppowBase))
                {
                    conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions41",
                        ppow->ppowBase->psetParent->pcatParent->pchName,
                        ppow->ppowBase->psetParent->pchName,
                        ppow->ppowBase->pchName));
                    bUsePowersReset = true;
                }

                // Figure out what the first power the bought is.
                if(ppow->ppowBase->psetParent->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
                {
                    if(ppow->iLevelBought==0)
                    {
                        ppowPrimaryFirst = ppow;
                    }
                }

                for(iboost=eaSize(&ppow->ppBoosts)-1; iboost>=0; iboost--)
                {
                    if(ppow->ppBoosts[iboost]
                        && !power_IsValidBoost(ppow, ppow->ppBoosts[iboost]))
                    {
                        int icost = 0;
                        const StoreItem *psi = store_FindItem(ppow->ppBoosts[iboost]->ppowBase, ppow->ppBoosts[iboost]->iLevel+ppow->ppBoosts[iboost]->iNumCombines);

                        if(psi)
                        {
                            iInfluenceCost += psi->iSell;
                            icost = psi->iSell;
                        }

                        conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions42",
                            ppow->ppBoosts[iboost]->ppowBase->psetParent->pchName,
                            ppow->ppBoosts[iboost]->ppowBase->pchName,
                            ppow->ppBoosts[iboost]->iLevel+1,
                            ppow->ppBoosts[iboost]->iNumCombines,
                            ppow->ppowBase->psetParent->pcatParent->pchName,
                            ppow->ppowBase->psetParent->pchName,
                            ppow->ppowBase->pchName,
                            icost));

                        bUseBoostReset = true;
                    }
                }
            }
        }

        if(!psetTemp)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions43"));
            bUsePowersReset = true;
        }
        if(!psetInherent)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions44"));
            bUsePowersReset = true;
        }
        if(!psetPrimary)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions45"));
            bUsePowersReset = true;
        }
        if(!psetSecondary)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions46"));
            bUsePowersReset = true;
        }


        // Give them all of their inherent and auto-grant powers
        {
            // This uses internal knowledge of the GrantAutoIssuePowers function.
            int iSize = eaSize(&e->powerDefChange);

            character_GrantAutoIssueBuildSpecificPowerSets(e->pchar);
            character_GrantAutoIssuePowers(e->pchar);

            if(iSize != eaSize(&e->powerDefChange))
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions47"));
                bFixedInherents = true;
            }
        }

        {
            char *pchType = "PowerType";
            char *pchXPType = "PowerXPType";

            // Check the total number of powers they have
            i = CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piPower) - character_CountPowersBought(e->pchar);
            if(i<0)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions48",
                    character_CountPowersBought(e->pchar), CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piPower),
                    character_GetLevel(e->pchar)+1,
                    pchType, pchXPType));

                bUsePowersReset = true;
            }
            else if(i>0)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions49",
                    character_CountPowersBought(e->pchar),
                    CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piPower),
                    character_GetLevel(e->pchar)+1,
                    pchType, pchXPType));

                bUseTrainer = true;
            }

            // Check the total number of enhancement slots
            i = CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piAssignableBoost) - character_CountBoostsBought(e->pchar);

            if(i<0)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions50",
                    character_CountBoostsBought(e->pchar),
                    CountForLevel(character_GetLevel(e->pchar ), g_Schedules.aSchedules.piAssignableBoost),
                    character_GetLevel(e->pchar )+1,
                    pchType, pchXPType));

                bUsePowersReset = true;
            }
            else if(i>0)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions51",
                    character_CountBoostsBought(e->pchar),
                    CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piAssignableBoost),
                    character_GetLevel(e->pchar)+1,
                    pchType, pchXPType));

                bUseTrainer = true;
            }

            // Check the total number of power pool sets
            i = CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piPoolPowerSet) - character_CountPoolPowerSetsBought(e->pchar);
            if(i<0)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions52",
                    character_CountPoolPowerSetsBought(e->pchar),
                    CountForLevel(character_GetLevel(e->pchar), g_Schedules.aSchedules.piPoolPowerSet),
                    character_GetLevel(e->pchar)+1, pchType, pchXPType));
                bUsePowersReset = true;
            }
        }


        if(bUsePowersReset)
        {
            conPrintf(client, localizedPrintf(e,"ResetInstructionsLong"));

            if(psetPrimary && ppowPrimaryFirst && psetSecondary)
            {
                conPrintf(client, "\n     %s\n\n", localizedPrintf(e,"ResetInstructions54"));
            }
            else if(psetPrimary && ppowPrimaryFirst && !psetSecondary)
            {
                conPrintf(client, "\n     %s\n\n", localizedPrintf(e,"ResetInstructions55", psetPrimary->psetBase->pchName, ppowPrimaryFirst->ppowBase->pchName));
            }
            else if(psetPrimary && !ppowPrimaryFirst && !psetSecondary)
            {
                conPrintf(client, "\n     %s\n\n", localizedPrintf(e,"ResetInstructions56", psetPrimary->psetBase->pchName));
            }
            else if(!psetPrimary && psetSecondary)
            {
                conPrintf(client, "\n    %s\n\n", localizedPrintf(e,"ResetInstructions57", psetSecondary->psetBase->pchName));
            }
            else
            {
                conPrintf(client, "\n     %s\n\n", localizedPrintf(e,"ResetInstructions58"));
            }

            if(!psetPrimary)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions59"));
                for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets)-1; iset>=0; iset--)
                {
                    const BasePowerSet *pbaseset = e->pchar->pclass->pcat[kCategory_Primary]->ppPowerSets[iset];

                    for(i=eaSize(&pbaseset->ppPowers)-1; i>=0; i--)
                    {
                        if(pbaseset->piAvailable[i]==0)
                        {
                            conPrintf(client, "    %s %s\n",
                                pbaseset->pchName,
                                pbaseset->ppPowers[i]->pchName);
                        }
                    }
                }
            }
            else if(!ppowPrimaryFirst)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions60"));
                for(i=eaSize(&psetPrimary->psetBase->ppPowers)-1; i>=0; i--)
                {
                    if(psetPrimary->psetBase->piAvailable[i]==0)
                    {
                        conPrintf(client, "    %s\n",
                            psetPrimary->psetBase->ppPowers[i]->pchName);
                    }
                }
            }

            if(!psetSecondary)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"ResetInstructions61"));
                for(iset=eaSize(&e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets)-1; iset>=0; iset--)
                {
                    conPrintf(client, "    %s %s\n",
                        e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets[iset]->pchName,
                        localizedPrintf(e,e->pchar->pclass->pcat[kCategory_Secondary]->ppPowerSets[iset]->pchDisplayName));
                }
            }

            conPrintf(client, localizedPrintf(e,"ResetInstructionsLong2"));
        }
        else if(bUseTrainer)
        {
            conPrintf(client, localizedPrintf(e,"ResetInstructionsLong3"));
        }
        else if(bFixedInherents)
        {
            conPrintf(client, localizedPrintf(e,"ResetInstructionsLong4"));
        }
        else if(!bUseBoostReset)
        {
            conPrintf(client, localizedPrintf(e,"ResetInstructionsLong5"));
        }


        if(bUseBoostReset && !bUsePowersReset)
        {
            conPrintf(client, localizedPrintf(e,"ResetInstructionsLong6",iInfluenceCost));
        }

        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:PowersCheck UsePowerReset=%s UseTrainer=%s FixedInherents=%s UseBoostReset=%s",
            bUsePowersReset?"true":"false", bUseTrainer?"true":"false",
            bFixedInherents?"true":"false", bUseBoostReset?"true":"false");
    }
}


void csrBadgesShow(ClientLink *client, int all)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        int i, n = eaSize(&g_BadgeDefs.ppBadges);
        bool bGotOne = false;

        conPrintf(client, "%s\n", localizedPrintf(e,"BadgesFor", e->name, e->db_id, db_state.server_name));
        for(i=0; i<n; i++)
        {
            int iIdx = g_BadgeDefs.ppBadges[i]->iIdx;
            if(iIdx>=0)
            {
                int owned = BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, iIdx);
                if(all || owned)
                {
                    conPrintf(client, "   %s%s\n", g_BadgeDefs.ppBadges[i]->pchName, all?(owned?"  (owned)":""):"");
                    bGotOne = true;
                }
            }
        }

        if(!bGotOne)
        {
            conPrintf(client, "   %s\n", localizedPrintf(e,"NoneParen"));
        }
    }
}

void csrBadgeGrantAll(Entity *e)
{
    int i, count = eaSize(&g_BadgeDefs.ppBadges);

    for( i = 0; i < count; i++ )
    {
        badge_Award(e, g_BadgeDefs.ppBadges[i]->pchName, 0);
    }

    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:BadgeGrant Granted all badges");
}

void csrBadgeRevokeAll(Entity *e)
{
    int i, count = eaSize(&g_BadgeDefs.ppBadges);

    for( i = 0; i < count; i++ )
    {
        badge_Revoke(e, g_BadgeDefs.ppBadges[i]->pchName);
    }

    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:BadgeRevoke Revoked all badges");
}

void csrBadgeStatsShow(ClientLink *client, char *pchStat)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        StashTableIterator iter;
        StashElement elem;

        conPrintf(client, "%s\n", localizedPrintf(e,"BadgesStatsFor", e->name, e->db_id, db_state.server_name));
        if(*pchStat!='\0')
            conPrintf(client, "   %s\n", localizedPrintf(e,"Searchingfor", pchStat));

        stashGetIterator(g_hashBadgeStatNames, &iter);
        while(stashGetNextElement(&iter, &elem))
        {
            const char *pchName = stashElementGetStringKey(elem);
            if(*pchName!='*' // Skip internal names
                && (*pchStat=='\0' || strstriConst(pchName, pchStat)!=NULL))
            {
                int iVal = badge_StatGet(e, pchName);
                int iIdx = badge_StatGetIdx(pchName);
                conPrintf(client, "   %6d %s (%d)\n", iVal, pchName, iIdx);
            }
        }
    }
}

void csrPowerInfo(ClientLink *client)
{
    Entity *e = clientGetControlledEntity(client);

    if(e)
    {
        int n = eaSize(&e->pchar->ppPowerSets);
        int i;

        conPrintf(client, "%s\n", localizedPrintf(e,"PowInfoStatFor", e->name, e->db_id, db_state.server_name));
        if(e->corrupted_dont_save) conPrintf(client, "   %s\n", localizedPrintf(e, "entCorrupted", e->name));
        conPrintf(client, "   %s\n", localizedPrintf(e,"SecurityCombat", e->pchar->iLevel+1, e->pchar->iCombatLevel+1));

        for(i=0; i<n; i++)
        {
            PowerSet *pset = e->pchar->ppPowerSets[i];
            int m, j;

            if (pset) 
            {
                m = eaSize(&pset->ppPowers);

                conPrintf(client, "   %s %s (lvl %d)\n", pset->psetBase->pcatParent->pchName, pset->psetBase->pchName, pset->iLevelBought+1);
                for(j=0; j<m; j++)
                {
                    Power *ppow = pset->ppPowers[j];

                    if (ppow)
                        conPrintf(client, "      %s (lvl %d)\n", ppow->ppowBase->pchName, ppow->iLevelBought+1);
                }
            }
        }
    }
}

void csrPowerInfoBuild(ClientLink *client, int build)
{
    Entity *e = clientGetControlledEntity(client);

    if (build < 0 || build >= MAX_BUILD_NUM)
    {
        conPrintf(client, "Invalid build number %d, range is 0 through %d\n", build, MAX_BUILD_NUM - 1);
        return;
    }

    if(e)
    {
        int n = eaSize(&e->pchar->ppBuildPowerSets[build]);
        int i;

        conPrintf(client, "%s\n", localizedPrintf(e,"PowInfoStatFor", e->name, e->db_id, db_state.server_name));
        if(e->corrupted_dont_save) conPrintf(client, "   %s\n", localizedPrintf(e, "entCorrupted", e->name));
        conPrintf(client, "   %s\n", localizedPrintf(e,"SecurityCombat", e->pchar->iLevel+1, e->pchar->iCombatLevel+1));

        for(i=0; i<n; i++)
        {
            PowerSet *pset = e->pchar->ppBuildPowerSets[build][i];
            int m, j;

            if (pset) 
            {
                m = eaSize(&pset->ppPowers);

                conPrintf(client, "   %s %s (lvl %d)\n", pset->psetBase->pcatParent->pchName, pset->psetBase->pchName, pset->iLevelBought+1);
                for(j=0; j<m; j++)
                {
                    Power *ppow = pset->ppPowers[j];

                    if (ppow)
                        conPrintf(client, "      %s (lvl %d)\n", ppow->ppowBase->pchName, ppow->iLevelBought+1);
                }
            }
        }
    }
}

void csrPowerChangeLevelBought(ClientLink *client, char *pchCat, char *pchSet, char *pchPow, int iNewLevel)
{
    Entity *e = clientGetControlledEntity(client);

    if(e && e->pchar)
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, pchCat, pchSet, pchPow);

        if(iNewLevel<1)
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"LevelRange") );
            return;
        }

        if(ppowBase)
        {
            Power *ppow = character_OwnsPower(e->pchar, ppowBase);
            if(ppow)
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"PowerChanged",
                    dbg_BasePowerStr(ppowBase), ppow->iLevelBought+1, iNewLevel));

                dbLog("CSR:PowerChangeLevel", e, "%s changed from %d to %d",
                    dbg_BasePowerStr(ppowBase), ppow->iLevelBought+1, iNewLevel);

                ppow->iLevelBought = iNewLevel-1;
            }
            else
            {
                conPrintf(client, "%s\n", localizedPrintf(e,"DoesntOwnPower", dbg_BasePowerStr(ppowBase)));
            }
        }
        else
        {
            conPrintf(client, "%s\n", localizedPrintf(e,"PowerDeosntExist", pchCat, pchSet, pchPow));
        }
    }
}


void csrCritterList( ClientLink *client )
{
    int i;
    for( i = 1 ; i  < entities_max ; i++)
    {
         Entity *e = entities[i];

        if( entInUse(i) && ENTTYPE(e) == ENTTYPE_CRITTER && e->pchar )
        {
            conPrintf( client, "%s\n", localizedPrintf(e,"EntityList", i, e->name, e->pchar->pclass->pchDisplayName ));
        }
    }
}

void csrNextCritterById( ClientLink *client, int id )
{
    Entity *e;

    if( id <= 0 || id >= entities_max )
    {
        conPrintf( client, clientPrintf(client,"InvalidId") );
        return;
    }

    e = entities[id];
    if(!e || !entInUse(id) )
    {
        conPrintf(client, "%s\n", clientPrintf(client,"EntityNoLonger", id) );
        return;
    }

    entUpdatePosInstantaneous(client->entity, ENTPOS(e));
    conPrintf( client, "%s\n", clientPrintf(client,"WentTo", id, e->name) );
}

void csrNextCritterByName( ClientLink *client, char *name )
{
    Entity* e = NULL;

    static int last_id = 1;
    bool bFound = 0;
    int cur_id = last_id;

      while( !bFound )
    {
         if( cur_id < entities_max-1 )
            cur_id++;
        else
            cur_id = 1;

        if( cur_id == last_id )
        {
            conPrintf( client, clientPrintf(client,"NoEntitesNamed", name) );
            return;
        }

        e = entities[cur_id];
        if( e && entInUse(cur_id) && stricmp(name,clientPrintf(client,e->name)) == 0  )
        {
            last_id = cur_id;
            bFound = true;
        }
    }

     entUpdatePosInstantaneous(client->entity, ENTPOS(e));
    conPrintf( client, "%s\n", clientPrintf(client,"WentTo", last_id, e->name) );
}

void csrAuthKick( ClientLink *client, char *player_name )
{

}

void csrNextArchVillain( ClientLink *client )
{
    Entity* e = NULL;

    static int last_id = 1;
    bool bFound = 0;
    int cur_id = last_id;

    while( !bFound )
    {
        if( cur_id < entities_max-1 )
            cur_id++;
        else
            cur_id = 1;

        if( cur_id == last_id )
        {
            conPrintf( client, clientPrintf(client,"NoArchVillains") );
            return;
        }

        e = entities[cur_id];
        if( e && entInUse(cur_id) && e->pchar && stricmp("Class_Boss_ArchVillain",clientPrintf(client,e->pchar->pclass->pchName)) == 0  )
        {
            last_id = cur_id;
            bFound = true;
        }
    }

    entUpdatePosInstantaneous(client->entity, ENTPOS(e));
    conPrintf( client, "%s\n", clientPrintf(client,"WentTo", last_id, e->name) );
}

void csrListen( ClientLink *client, char * player_name )
{
    int id            = dbPlayerIdFromName(player_name);
    Entity *player    = entFromDbId( id );

    if( id && player )
    {
        player->pl->csrListener = client->entity->db_id;
         conPrintf( client, clientPrintf(client,"ListenStatus", player->name) );
    }
}

void conPrintAuthUserData(ClientLink *client, U32 data[4])
{
    int i;
    char *estr;
    U8 *puch = (U8*)data;

    estrCreate(&estr);


    estrConcatStaticCharArray(&estr, "   as bytes: 0x");
    for(i=0; i<16; i++)
    {
        estrConcatf(&estr, "%02x", (int)puch[i]);
    }

    estrConcatStaticCharArray(&estr, "\n   as words:");
    for(i=0; i<4; i++)
    {
        estrConcatf(&estr, " 0x%08x", data[i]);
    }
    estrConcatStaticCharArray(&estr, "\n");

    conPrintf(client, estr);
    estrDestroy(&estr);
}

void csrEditDescription( Entity * e, char * str )
{
    if(str)
        strncpyt( e->strings->playerDescription, str, 1023 );
    else
        strcpy( e->strings->playerDescription, "" );

    sendChatMsg( e, localizedPrintf(e,"SetPlayerDescription", e->name, e->strings->playerDescription ), INFO_DEBUG_INFO, 0 );
}

void csrDestroyBase( int db_id, int sg_id )
{
    Packet        *pak;
    pak = pktCreateEx(&db_comm_link,DBCLIENT_DESTROY_BASE);

    pktSendBits(pak, 32, db_id);
    pktSendBits(pak, 32, sg_id);
    pktSendString(pak, SGBaseConstructInfoString(sg_id));

    pktSend(&pak,&db_comm_link);
}


void csrJoinSupergroup( Entity *e, int super_id )
{
    if( e->supergroup_id )
        sgroup_MemberQuit(e);

    if( teamAddMember( e, CONTAINER_SUPERGROUPS, super_id, 0) )
    {
        int gmRank = NUM_SG_RANKS-1;
        sgroup_initiate(e);
        sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &e->db_id, &gmRank, "SetRank");
        sgroup_refreshStats(e->db_id);
    }
}

static Boost * boost_Get( const char * pchSet, const char * pchName, int iLevel )
{
    const BasePower *ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Boosts", pchSet, pchName);
    if(ppow!=NULL)
    {
        return boost_Create(ppow, 0, NULL, iLevel, 0, NULL, 0, NULL, 0);
    }

    return 0;
}


#define AUTOENHANCE_IO  (1<<0)
#define AUTOENHANCE_SET (1<<1)

void power_AutoPopulateBoosts( Character * pchar, Power * ppow, int autoType )
{
    
    char buffer[1000];
    Boost * pBoost;

    char *Types[] = // Ordered by priority
    {
        "Accuracy",
        "Damage",

        "Recovery",
        "Heal",
        "Hold",
        "Stun",
        "Res_Damage",
        "ToHit_Buff",
        "Defense_Buff",

        "Endurance_Discount",
        "Recharge",

        "Drain_Endurance",
        "ToHit_DeBuff",
        "Defense_DeBuff",
        "Sleep",
        "Immobilize",
        "Knockback",
        "Confuse",
        "Fear",
        "Snare",
        "Intangible",
        "Interrupt",
        "Range",
        "Run",
        "Jump",
        "Fly",
    };
    int i = 0, iType = 0, iTypeCnt = 0, j, k, m, n;
    int iLevel = character_CalcExperienceLevel(pchar); 

       if( autoType&AUTOENHANCE_SET && ppow->iNumBoostsBought >= 1 )
    {
        BoostSet **ppBoostSets=0;
        int try_again = 1;

        eaCopy(&ppBoostSets, &g_BoostSetDict.ppBoostSets);
    
        for( k = eaSize(&ppBoostSets)-1; k>=0; k-- )
        {
            int icnt = 0;
            // if we've used a set remove it from list
            for( n = 0; n < eaSize(&pchar->ppPowerSets); n++ )
            {
                for(j = 0; j < eaSize(&pchar->ppPowerSets[n]->ppPowers); j++ )
                {
                    Power * pow = pchar->ppPowerSets[n]->ppPowers[j];

                    for( m = 0; pow && m < eaSize(&pow->ppBoosts); m++ )
                    {
                        if( pow->ppBoosts[m] && pow->ppBoosts[m]->ppowBase == ppBoostSets[k]->ppBoostLists[0]->ppBoosts[0] )
                            icnt++;
                    }
                }
            }

            if( icnt > 2)
                eaRemove(&ppBoostSets, k );
        }
second_attempt:
        for( j= 0; j < eaSize(&ppow->ppowBase->ppBoostSets); j++ )
        {
            for( k = 0; k < eaSize(&ppBoostSets); k++ )
            {
                if( (!try_again || eaSize(&ppBoostSets[k]->ppBoostLists) >= ppow->iNumBoostsBought+1) && // look for set that can fill us out
                    stricmp( ppow->ppowBase->ppBoostSets[j]->pchGroupName, ppBoostSets[k]->pchGroupName )==0 )
                {
                    int set_idx = 0;
                    while(i <= ppow->iNumBoostsBought && set_idx < eaSize(&ppBoostSets[k]->ppBoostLists) ) // attempt to fill power
                    {
                        const BasePower *pBase = ppBoostSets[k]->ppBoostLists[set_idx]->ppBoosts[0];
                        pBoost = boost_Get( pBase->pchName, pBase->pchName, iLevel );
                        if (pBoost && 
                            power_IsValidBoost( ppow, pBoost) && 
                            power_BoostCheckRequires(ppow, pBoost) && 
                            boost_IsValidToSlotAtLevel(pBoost, pchar->entParent, iLevel, pchar->entParent->erOwner)  )
                        {
                            power_ReplaceBoostFromBoost( ppow, i, pBoost, "AutoEnhance" );
                            i++;
                        }
                        boost_Destroy(pBoost);
                        set_idx++;
                    } 

                    // if we filled power, exit otherwise try again
                    if( i > ppow->iNumBoostsBought )
                        return;
                    else if( try_again )
                    {
                        i = 0;
                    }
                }
            }
        }

        if( try_again )
        {
            try_again = 0;
            goto second_attempt;
        }
    }

    // Couldn't find a set so fill via ordinary means
    if(autoType&AUTOENHANCE_IO)
    {
        int mult = (iLevel+1)/5;  
          iLevel = MAX(9,(mult*5)-1);
    }

    // long recharge powers need this more
    if( ppow->ppowBase->fRechargeTime >= 180 )
    {
        if( autoType&AUTOENHANCE_IO )
            sprintf(buffer, "crafted_Recharge" );
        else
            sprintf(buffer, "%s_Recharge", pchar->porigin->pchName );
        pBoost = boost_Get( buffer, buffer, iLevel );
        if (pBoost && 
            power_IsValidBoost( ppow, pBoost) && 
            power_BoostCheckRequires(ppow, pBoost) && 
            boost_IsValidToSlotAtLevel(pBoost, pchar->entParent, iLevel, pchar->entParent->erOwner)  )
        {
            while(i <= ppow->iNumBoostsBought && iTypeCnt < 3 )
            {    power_ReplaceBoostFromBoost( ppow, i, pBoost, "AutoEnhance" );
                iTypeCnt++;
                i++;
            }
        }
        boost_Destroy(pBoost);
        iTypeCnt = 0;
    }

    while( i <= ppow->iNumBoostsBought && i<ppow->ppowBase->iMaxBoosts && iType < ARRAY_SIZE(Types) && ppow->ppowBase->pBoostsAllowed )
    {
        // Skip stupid cases
        if( ppow->ppowBase->eType == kPowerType_Toggle && stricmp( Types[iType], "Recharge") == 0 || // toggle powers never need recharge
            ppow->ppowBase->eType == kPowerType_Auto && stricmp( Types[iType], "Endurance_Discount") == 0   ) // auto powers never need endurance
        {
            iType++;
            iTypeCnt = 0;
        }

        if( autoType&AUTOENHANCE_IO )
        {
            if(stricmp( Types[iType], "Drain_Endurance") == 0)
                iType++;
            sprintf(buffer, "crafted_%s", Types[iType] );
        }
        else
            sprintf(buffer, "%s_%s", pchar->porigin->pchName, Types[iType] );

        pBoost = boost_Get( buffer, buffer, iLevel );
        if (pBoost && 
            power_IsValidBoost( ppow, pBoost) && 
            power_BoostCheckRequires(ppow, pBoost) && 
            boost_IsValidToSlotAtLevel(pBoost, pchar->entParent, iLevel, pchar->entParent->erOwner)  )
        {
            power_ReplaceBoostFromBoost( ppow, i, pBoost, "AutoEnhance" );
            iTypeCnt++;
            i++;
            if( iTypeCnt >= 2 )
            {
                iType++;
                iTypeCnt = 0;
            }
        }
        else
        {
            iType++;
            iTypeCnt = 0;
        }
        boost_Destroy(pBoost);

        if(iType==ARRAY_SIZE(Types)) // restart list
            iType=0;
    }
}


void csrPowersAutoEnhance( Entity *e, int autoType )
{
    int i, j;
    if( !e || !e->pchar )
        return;

    if( !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn")) )
    {
        sendChatMsg( e, "You need the Invention License to use this command.", INFO_DEBUG_INFO, 0 );
        return;
    }

    for( i = eaSize(&e->pchar->ppPowerSets)-1; i>=0; i-- )
    {
        for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            Power *ppow = e->pchar->ppPowerSets[i]->ppPowers[j];
            if( ppow )
                power_AutoPopulateBoosts( e->pchar, ppow, autoType );
        }
    }
    eaPush(&e->powerDefChange, (void *)-1);
}

void csrPowersMaxSlots( Entity *e )
{
    int i, j;
    if( !e || !e->pchar )
        return;

    for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ ) 
    {
        for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            Power *ppow = e->pchar->ppPowerSets[i]->ppPowers[j];
            if( ppow )
            {
                while( ppow->iNumBoostsBought < ppow->ppowBase->iMaxBoosts-1)
                {
                    ppow->iNumBoostsBought++;
                    power_AddBoostSlot(ppow);
                }
            }
        }
    }
    eaPush(&e->powerDefChange, (void *)-1);
}
void csrClearMARTYHistory(ClientLink *admin, char *player_name, char *orig_cmd, char *reason)
{
    int result = 0;
    Entity *e = findEntOrRelay(NULL,player_name,orig_cmd,&result);
    if (e)
    {
        clearMARTYHistory(e);
    }
}

void csrPrintMARTYHistory(ClientLink *admin, char *player_name, char *orig_cmd)
{
    int result = 0;
    Entity *e = findEntOrRelay(NULL,player_name,orig_cmd,&result);
    if (e)
    {
        int i;
        char *estr = NULL;
        for (i = 0; i < MARTY_ExperienceTypeCount; ++i)
        {
            estrConcatf(&estr, "Experience Type: %s\n", StaticDefineIntRevLookup(ParseMARTYExperienceTypes,i));
            estrConcatf(&estr, "Running bucket(not added yet): %i\n", displayMARTYSum(e, i, 0));
            estrConcatf(&estr, "%i min interval sum: %i\n", 1, displayMARTYSum(e, i, 1));
            estrConcatf(&estr, "%i min interval sum: %i\n", 5, displayMARTYSum(e, i, 5));
            estrConcatf(&estr, "%i min interval sum: %i\n", 15, displayMARTYSum(e, i, 15));
            estrConcatf(&estr, "%i min interval sum: %i\n", 30, displayMARTYSum(e, i, 30));
            estrConcatf(&estr, "%i min interval sum: %i\n", 60, displayMARTYSum(e, i, 60));
            estrConcatf(&estr, "%i min interval sum: %i\n", 120, displayMARTYSum(e, i, 120));
        }
        conPrintf(admin, estr);
        estrDestroy(&estr);
    }
}
#define DBFLAG_PRINT(flag) conPrintf(client, "  %c : " #flag "\n", e->db_flags & DBFLAG_##flag ? '*' : ' ');
void csrPrintDbFlags( ClientLink *client, Entity *e )
{
    if( !client || !e )
        return;

    conPrintf(client, "DbFlags: %08lx\n", e->db_flags );
    DBFLAG_PRINT( TELEPORT_XFER );
    DBFLAG_PRINT( UNTARGETABLE );
    DBFLAG_PRINT( INVINCIBLE );
    DBFLAG_PRINT( DOOR_XFER );
    DBFLAG_PRINT( MISSION_ENTER );
    DBFLAG_PRINT( MISSION_EXIT );
    DBFLAG_PRINT( INVISIBLE );
    DBFLAG_PRINT( INTRO_TELEPORT );
    DBFLAG_PRINT( MAPMOVE );
    DBFLAG_PRINT( CLEARATTRIBS );
    DBFLAG_PRINT( NOCOLL );
    DBFLAG_PRINT( BASE_ENTER );
    DBFLAG_PRINT( RENAMEABLE );
    DBFLAG_PRINT( BASE_EXIT );
    DBFLAG_PRINT( ALT_CONTACT );
    DBFLAG_PRINT( ARCHITECT_EXIT );
    DBFLAG_PRINT( PRAET_SG_JOIN );
    DBFLAG_PRINT( HALF_MAX_HEALTH );
    DBFLAG_PRINT( UNLOCK_HERO_EPICS );
    DBFLAG_PRINT( UNLOCK_VILLAIN_EPICS );
}

#define DBFLAG_COMPARE(flag, bitval) if( stricmp( flag_name, #flag ) == 0 ) bit = bitval;
void csrSetDbFlag( ClientLink *client, Entity *e, char *flag_name, int flag_val )
{
    int bit = -1;

    if( !client || !e )
        return;

    DBFLAG_COMPARE( TELEPORT_XFER, 0 );
    DBFLAG_COMPARE( UNTARGETABLE, 1 );
    DBFLAG_COMPARE( INVINCIBLE, 2 );
    DBFLAG_COMPARE( DOOR_XFER, 3 );
    DBFLAG_COMPARE( MISSION_ENTER, 4 );
    DBFLAG_COMPARE( MISSION_EXIT, 5 );
    DBFLAG_COMPARE( INVISIBLE, 6 );
    DBFLAG_COMPARE( INTRO_TELEPORT, 7 );
    DBFLAG_COMPARE( MAPMOVE, 8 );
    DBFLAG_COMPARE( CLEARATTRIBS, 9 );
    DBFLAG_COMPARE( NOCOLL, 10 );
    DBFLAG_COMPARE( BASE_ENTER, 11 );
    DBFLAG_COMPARE( RENAMEABLE, 12 );
    DBFLAG_COMPARE( BASE_EXIT, 13 );
    DBFLAG_COMPARE( ALT_CONTACT, 14 );
    DBFLAG_COMPARE( ARCHITECT_EXIT, 15 );
    DBFLAG_COMPARE( PRAET_SG_JOIN, 16 );
    DBFLAG_COMPARE( HALF_MAX_HEALTH, 17 );

    if( bit < 0 )
    {
        conPrintf( client, "Unknown DbFlag '%s'\n", flag_name );
    }
    else
    {
        if( flag_val )
        {
            e->db_flags |= ( 1 << bit );
            conPrintf( client, "DbFlag set\n" );
        }
        else
        {
            e->db_flags &= ~( 1 << bit );
            conPrintf( client, "DbFlag unset\n" );
        }
    }
}

/* End of File */
