#include "entity/entserver.h"
#include "entity/SgrpStats.h"
#include "entity/SgrpRewards.h"
#include "cmdparse/cmdserver.h"
#include "cmdparse/cmdcontrols.h"
#include "svr_base.h"
#include "player/clientEntityLink.h"
#include "player/parseClientInput.h"
#include "svr_player.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/network/sock.h>
#include "dbcomm/dbcomm.h"
#include "gameComm/sendToClient.h"
#include <utilitieslib/assert/assert.h>
#include "group/groupnetdb.h"
#include <utilitieslib/utils/timing.h>
#include <process.h>
#include <utilitieslib/utils/utils.h>
#include "gloophook.h"
#include "dbcomm/dbmapxfer.h"
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/utils/FolderCache.h>
#include "dbcomm/dbquery.h"
#include "entity/friends.h"
#include "container/containerEmail.h"
#include "gameComm/trading.h"
#include <utilitieslib/network/net_version.h>
#include "entity/entPlayer.h"
#include "entity/entsend.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include "entity/character_level.h"
#include "script/scriptengine.h"
#include "script/scriptdebug.h"
#include "gameComm/debugCommon.h"
#include <utilitieslib/components/StringTable.h>
#include "serverError.h"
#include "language/langServerUtil.h"
#include "entity/character_base.h"
#include "gameComm/automapServer.h"
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/EString.h>
#include "storyarc/TeamTask.h"
#include "beacon/beaconDebug.h"
#include "beacon/beaconClientServerPrivate.h"
#include "entity/origins.h"
#include "svr_bug.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "dbcomm/shardcomm.h"
#include "dbcomm/logcomm.h"
#include "group/groupnetdb.h"
#include "group/groupfileloadutil.h"
#include "group/groupfilesave.h"
#include "gameSys/arenamapserver.h"
#include "seq/modelReload.h"
#include "seq/tricks.h"
#include "seq/anim.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gameSys/cutScene.h"
#include "gameSys/sgraid.h"
#include "bases/basesystems.h"
#include "NovodeX/NwWrapper.h"
#include "entity/teamCommon.h"
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/components/SharedMemory.h>
#include "beacon/beaconFile.h"
#include "auth/authUserData.h"
#include "svr_init.h"
#include "gameData/miningaccumulator.h"
#include "entity/character_net_server.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "gameSys/arenamap.h"
#include "entity/pvp.h"
#include "door.h"
#include "storyarc/missionServerMapTest.h"
#include "entity/character_inventory.h"
#include "player/badges_server.h"
#include "cmdparse/autoCommands.h"
#include "svr_tick.h"
#include "entity/salvage.h"
#include "gameSys/EndGameRaid.h"
#include "entity/powers.h"
#include "entity/costume.h"

int catch_ugly_entsend_bug_hack=0;
int server_error_sent_count=0;

static int frame;
static U32 s_svr_last_active_time;    // for tracking amount of time server is idle

#define MAX_BUG_MSG_SIZE    2000    // used by CLIENT_BUG handler... max size of bug message written to file
#define ENT_SAVE_COOLDOWN    5.0f

static F32 fixTime(F32 a)
{
    if(a >= 24.f)
        a -= 24.f;
    if(a < 0)
        a += 24.f;
    return a;
}

static void updateTime(int force)
{
    U32        msec = timerMsecsSince2000() % (24 * 60 * 60 * 1000);
    F32        t_old,hours = msec / (F32)(60 * 60 * 1000);

    t_old = fmod(((hours-0.1) * server_visible_state.timescale) + server_visible_state.time_offset,24);
    server_visible_state.time = fmod((hours * server_visible_state.timescale) + server_visible_state.time_offset,24);
    //printf("msec %d  hours %f  oldgame %f  gametime %f\n",msec,hours,t_old,server_visible_state.time);

    if (force || (frame & 255) == 255)
    {
        char buf[100];

        sprintf(buf,"time %f",server_visible_state.time);
        cmdOldServerReadInternal(buf);
    }
}

void svrTimeSet(F32 new_time)
{
    server_visible_state.time_offset = 0;
    updateTime(0);
    server_visible_state.time_offset = new_time - server_visible_state.time;
    updateTime(1);
}

extern void worldSendUpdate(NetLink *link,int full_update);

static int TickCount = 0;

F32        makemovie_time = 1.f/30.f;

static void freeDupDbIds(Entity *e)
{
    int        i;
    ClientLink    *client;

    for(i=0;i<net_links.links->size;i++)
    {
        client = ((NetLink*)net_links.links->storage[i])->userData;
        if (client->entity && client->entity != e && client->entity->db_id == e->db_id){
            LOG_OLD_ERR( "\n\tMapserver on %s disconnect %s from %s\n\tReason: duplicate db_id\n", getComputerName(), client->entity->auth_name, stringFromAddr(&client->link->addr));
            netRemoveLink(client->link);
        }
            //svrDisconnectAndSleepClient(client,0);
    }
}


void svrHandleClientConnect(ClientLink *client)
{
    ServerControlState* scs;
    playerEntAdd(PLAYER_ENTS_ACTIVE,client->entity);
    playerEntDelete(PLAYER_ENTS_CONNECTING,client->entity,0);
    SET_ENTINFO(client->entity).sent_self = 0;
    scs = getQueuedServerControlState(&client->controls);

    scs->speed[0]        = BASE_PLAYER_SIDEWAYS_SPEED;
    scs->speed[1]        = BASE_PLAYER_VERTICAL_SPEED;
    scs->speed[2]        = BASE_PLAYER_FORWARDS_SPEED;
    scs->speed_back        = BASE_PLAYER_BACKWARDS_SPEED;
    memcpy(scs->surf_mods, client->entity->motion->input.surf_mods, sizeof(scs->surf_mods));
    scs->surf_mods[SURFPARAM_GROUND].max_speed = BASE_PLAYER_FORWARDS_SPEED;
    scs->surf_mods[SURFPARAM_AIR].max_speed = BASE_PLAYER_FORWARDS_SPEED;
    // MS: Set the default entity interpolation quality.

    client->interpDataLevel = DEFAULT_CLIENT_INTERP_DATA_LEVEL;
    client->interpDataPrecision = DEFAULT_CLIENT_INTERP_DATA_PRECISION;

    client->entity->pl->inviter_dbid = 0;
}

typedef struct CmdTimer {
    const char*            name;
    PerformanceInfo*    perfInfo;
} CmdTimer;

static CmdTimer cmdTimers[CLIENT_CMD_MAX - CLIENT_INPUT];

int svrHandleClientMsg(Packet *pak, int cmd, NetLink *link)
{
    static initTimers = 0;

    Packet    *pak_out;
    ClientLink    *client = link->userData;
    int timerStarted = 0;

    server_state.curClient = client;
    client->last_ack = frame;

    //printf("cmd: %d, %d\n", cmd, pak->id);

    if(!initTimers){
        initTimers = 1;

        #define CREATE_NAME(x) cmdTimers[x - CLIENT_INPUT].name = "cmd:"#x
        CREATE_NAME(CLIENT_INPUT);
        CREATE_NAME(CLIENT_REQSCENE);
        CREATE_NAME(CLIENT_REQSHORTCUTS);
        CREATE_NAME(CLIENT_REQALLENTS);
        CREATE_NAME(CLIENT_READY);
        CREATE_NAME(CLIENT_DISCONNECT);
        CREATE_NAME(CLIENT_QUITSERVER);
        CREATE_NAME(CLIENT_CONNECT);
        CREATE_NAME(CLIENT_WORLDCMD);
        CREATE_NAME(CLIENT_MAP_XFER_WAITING);
        CREATE_NAME(CLIENT_MAP_XFER_ACK);
        CREATE_NAME(CLIENT_BUG);
        CREATE_NAME(CLIENT_ACCOUNT_SERVER_CMD);
        CREATE_NAME(CLIENT_LOGPRINTF);
        CREATE_NAME(CLIENT_BASEPOWER_NOT_FOUND);
        CREATE_NAME(CLIENT_STOP_FORCED_MOVEMENT);
        #undef CREATE_NAME
    }

    PERFINFO_RUN(
        if(cmd >= CLIENT_INPUT && cmd < CLIENT_CMD_MAX){
            int i = cmd - CLIENT_INPUT;
            PERFINFO_AUTO_START_STATIC(cmdTimers[i].name, &cmdTimers[i].perfInfo, 1);
            timerStarted = 1;
        }
    );

    if (cmd!=CLIENT_INPUT) {
        netLog(link, "Received command %s\n", cmdTimers[cmd - CLIENT_INPUT].name);
    }

    switch(cmd)
    {
        xcase CLIENT_INPUT:
            receiveClientInput(pak,client);

        xcase CLIENT_CONNECT:
#define CLIENT_CONNECT_ERROR(msg)    \
    LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Refused connection from %s:%d: %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port, msg); \
    pktSendBitsPack(pak_out,1,0);    \
    pktSendString(pak_out,msg);        \
    pktSend(&pak_out,client->link);    \
    lnkBatchSend(client->link);        \
    if (client->entity) {            \
        svrForceLogoutLink(client->link,msg,KICKTYPE_BADDATA);    \
    } else {                        \
        netRemoveLink(client->link);\
    }                                \
    if(timerStarted){                \
        PERFINFO_AUTO_STOP();        \
    }                                \
    return 0;

            if(pak->hasDebugInfo && isDevelopmentMode()) {
                pktSetDebugInfo();
            }

            pak_out = pktCreateEx(client->link, SERVER_CONNECT_MSG);

            // Check network versions
            if (link->protocolVersion != getDefaultNetworkVersion()) {
                CLIENT_CONNECT_ERROR(clientPrintf(client,"BadNetProtocol"));
            }

            clientSetEntity(client, entFromLoginCookie(pktGetBitsPack(pak,1)));

            if (!client->entity)
            {
                CLIENT_CONNECT_ERROR(clientPrintf(client,"BadCookie"));
            }

            freeDupDbIds(client->entity);

            if (pktGetBits(pak, 1)!= server_state.cod)
            {
                CLIENT_CONNECT_ERROR(clientPrintf(client,"ServerCodMismatch"));
            }

            {
                int newCharacter = pktGetBits(pak, 1);
                if (newCharacter) {
                    if (character_IsInitialized(client->entity->pchar)) {
                        CLIENT_CONNECT_ERROR(clientPrintf(client,"CreateCharacterExists"));
                    }
                    svrHandleClientConnect(client); // This has to be before parseClientInput?  Why?  I don't know.  Physics gets messed up otherwise.
                    if (!parseClientInput(    pak, client )) {
                        CLIENT_CONNECT_ERROR(clientPrintf(client,"BadCharCreate"));
                    }
                    if (!character_IsInitialized(client->entity->pchar)) {
                        CLIENT_CONNECT_ERROR(clientPrintf(client,"BadCharCreate2"));
                    }
                } else {
                    if (!character_IsInitialized(client->entity->pchar)) {
                        CLIENT_CONNECT_ERROR(clientPrintf(client,"BadResumeChar"));
                    }
                    svrHandleClientConnect(client);
                }
            }


            pktSendBitsPack(pak_out,1,1);
            pktSendF32(pak_out, db_state.timeZoneDelta ); 

            pktSend(&pak_out,client->link);

            autoCommand_run(client, client->entity, 0);

            LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Connected client %s From %s:%d",
                character_IsInitialized(client->entity->pchar)?"and resuming character":"and creating character", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port);

        xcase CLIENT_REQSCENE:
            {
                int i, failed = false, dummy = -1;

                for(i = 0; i < net_links.links->size; i++){
                    NetLink* link;
                    link = net_links.links->storage[i];
                    if(link->userData == client)
                        break;
                }

                assert(i < net_links.links->size);

                if (!pktEnd(pak))
                { // Hacker detection (leaked source will not send this bit
                    dummy = pktGetBits(pak, 1);
                }
                if (dummy!=0) 
                {
                    LOG( LOG_CHEATING, LOG_LEVEL_VERBOSE, 0, "Missing stealth bit on CLIENT_REQSCENE packet, AuthName: %s, IP: %s", client->entity?client->entity->auth_name:"NoEntity", makeIpStr(client->link->addr.sin_addr.S_un.S_addr));
                }

                if (!client->entity || !character_IsInitialized(client->entity->pchar))
                {
                    if (client->entity) 
                        assert(!"Client tried to connect on character that is corrupt! (Internal error)");
                    failed = true;
                }

                if( failed )
                {
                    pak_out = pktCreateEx(client->link, SERVER_CONNECT_MSG);
                    pktSendBitsPack(pak_out,1,0);
                    pktSendString(pak_out,"Trying to resume an invalid character");
                    pktSend(&pak_out,client->link);
                    lnkBatchSend(client->link);
                    netRemoveLink(client->link);
                    if(timerStarted)
                        PERFINFO_AUTO_STOP();
                    return 0;
                }

                worldSendUpdate(client->link,1);
                client->ready = CLIENTSTATE_NOT_READY;
                if (client->entity)
                    client->entity->ready = 0;
            }
        xcase CLIENT_REQALLENTS:
            if (!client->entity) 
            {
                // Data received without an entity, drop them!
                netRemoveLink(client->link);
                break;
            }
            // Client has loaded the map and wants to get information on the ents it can see, so it can load that
            client->ready = CLIENTSTATE_REQALLENTS;
            SET_ENTINFO(client->entity).sent_self = 0;
            memset(client->entNetLink, 0, sizeof(EntNetLink));
        xcase CLIENT_READY:
            if (!client->entity) {
                // Data received without an entity, drop them!
                netRemoveLink(client->link);
                return 0;
            }
            if (!client->entity->owned) {
                // Error resuming character!
                Packet *pak_out = pktCreateEx(client->link, SERVER_CONNECT_MSG);
                // Someone's connecting on an entity that isn't owned?  What horrible things have happened here?
                assert(0);
                pktSendBitsPack(pak_out,1,0);
                pktSendString(pak_out,"Connection refused: internal error");
                pktSend(&pak_out,client->link);
                lnkBatchSend(client->link);
                logDisconnect(client, "unowned ent");
                netRemoveLink(client->link);
                return 0;
            }

            // Client has loaded the map, loaded relevant textures/costumes/geometry, and now wants
            //   to be entered into the normal message loop of the game.  It will then send
            //   CLIENTINP_RECEIVED_CHARACTER when it's notified the server it's received itself
            // The state will instantly be set to CLIENTSTATE_IN_GAME as soon as a full update is sent to it
            client->ready = CLIENTSTATE_ENTERING_GAME;
            SET_ENTINFO(client->entity).sent_self = 0; // This triggers the entity update to be sent
            memset(client->entNetLink, 0, sizeof(EntNetLink));
            mapperSend(client->entity, 0);

            // Try to advance all compound tasks of the player if he is playing by himself.
            if(!client->entity->teamup || (client->entity->teamup && client->entity->teamup->members.count == 1)) {
                TaskAdvanceCompleted(client->entity);
            }
            MissionTeamupAdvanceTask(client->entity); // advance my teamup's task when I load onto new map

            {
                // This stuff used to be in CLIENTINP_RESUME_CHARACTER, which wasn't sent until a while after the
                // player actually started receiving entity updates
                int     ent_idx_cookie;
                int     client_console;
                U32        passwordMD5[4];
                char *trackedItemStr = NULL;
                int i;
                int isInitialLogin = 0;

                ent_idx_cookie      = pktGetBitsPack(pak,1);
                client_console      = pktGetBitsPack( pak, 1 );
                pktGetBitsArray(pak, 128, passwordMD5);
                isInitialLogin        = pktGetBitsAuto(pak);

                if(client->entity)
                {
                    int resumeResult;
                    client->entity->client_console = client_console;
                    memcpy(client->entity->passwordMD5, passwordMD5, sizeof(passwordMD5));

                    entSendPlrPosQrot(client->entity);

                    resumeResult = resumeCharacter( client, ent_idx_cookie );

                    if (!resumeResult)
                    {
                        // Error resuming character!
                        Packet *pak_out = pktCreateEx(client->link, SERVER_CONNECT_MSG);
                        pktSendBitsPack(pak_out,1,0);
                        pktSendString(pak_out,"Cannot find match for login cookie");
                        pktSend(&pak_out,client->link);
                        lnkBatchSend(client->link);
                        netRemoveLink(client->link);
                        return 0;
                    }
                    else if (resumeResult == -1)
                    {
                        Packet *pak_out = pktCreateEx(client->link, SERVER_CONNECT_MSG);
                        pktSendBitsPack(pak_out,1,0);
                        pktSendString(pak_out,"Cannot login this character");
                        pktSend(&pak_out,client->link);
                        lnkBatchSend(client->link);
                        netRemoveLink(client->link);
                        printf("Character disallowed, kicking...\n");
                        return 0;
                    }
                }
                LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Connection:ResumeCharacter from %s:%d AuthName \"%s\"",makeIpStr(client->link->addr.sin_addr.S_un.S_addr),client->link->addr.sin_port, client->entity->auth_name);

                for (i = 0; i < eaSize(&g_SalvageTrackedList.ppTrackedSalvage); ++i)
                {
                    SalvageTrackedByEnt *salvageItem = g_SalvageTrackedList.ppTrackedSalvage[i];
                    int count = 0;
                    if (salvageItem->item)
                        count = character_SalvageCount(client->entity->pchar, salvageItem->item);
                    estrConcatf(&trackedItemStr, " %s: %i", salvageItem->displayName, count);
                }

                LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Entity:Resume:Character: %s %s, Level:%d XPLevel:%d XP:%d HP:%.2f End:%.2f Debt: %d Inf:%d AccessLvl: %d WisLvl: %d Wis: %d%s",
                    client->entity->pchar->porigin->pchName,
                    client->entity->pchar->pclass->pchName,
                    client->entity->pchar->iLevel+1,
                    character_CalcExperienceLevel(client->entity->pchar)+1,
                    client->entity->pchar->iExperiencePoints,
                    client->entity->pchar->attrCur.fHitPoints,
                    client->entity->pchar->attrCur.fEndurance,
                    client->entity->pchar->iExperienceDebt,
                    client->entity->pchar->iInfluencePoints,
                    client->entity->access_level,
                    client->entity->pchar->iWisdomPoints,
                    client->entity->pchar->iWisdomLevel,
                    trackedItemStr);

                if (isInitialLogin)
                {
                    PrintCostumeToLog(client->entity);
                }

                estrDestroy(&trackedItemStr);
                if(client->entity->pl)
                {
                    LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "SuperGroup:Mode %s %d", client->entity->pl->supergroup_mode ? "On" : "Off", client->entity->pl->timeInSGMode);
                }
            }

        xcase CLIENT_DISCONNECT:
        {
            int        abort, logout_login;
            abort = pktGetBitsPack(pak,1);
            logout_login = pktGetBitsPack(pak,2);
            if (abort)
            {
                if (client->entity) {
                    client->entity->logout_timer = 0;
                    client->entity->logout_update = 1;
                    client->entity->logout_login = 0;
                }
            }
            else
            {
                if (client->entity) 
                {
                    svrPlayerSetDisconnectTimer(client->entity,0);
                    client->entity->logout_login = logout_login;
                } 
                else 
                {
                    netRemoveLink(client->link);
                }
            }

            //svrDisconnectAndSleepClient(client);
        }
        xcase CLIENT_REQSHORTCUTS:
            pak_out = pktCreate();
            pktSendBitsPack(pak_out,1,SERVER_CMDSHORTCUTS);
            cmdOldServerSendShortcuts(pak_out);
            if ( client->entity->access_level > 0 )
                cmdOldServerSendCommands(pak_out, client->entity->access_level);
            else
                pktSendBitsPack(pak_out, 1, 0);
            pktSend(&pak_out,client->link);

        xcase CLIENT_MAP_XFER_WAITING:
            if (!client->entity) {
                // Data received without an entity, drop them!
                netRemoveLink(client->link);
                break;
            }
            client->entity->dbcomm_state.client_waiting = 1;

        xcase CLIENT_MAP_XFER_ACK:
            if (!client->entity) {
                // Data received without an entity, drop them!
                netRemoveLink(client->link);
                break;
            }
            client->entity->dbcomm_state.map_xfer_step = MAPXFER_DONE;
            client->entity->dbcomm_state.client_waiting = 1;

        xcase CLIENT_WORLDCMD:
            if (!client->entity) {
                // Data received without an entity, drop them!
                netRemoveLink(client->link);
                break;
            }
            if (client->entity->access_level < ACCESS_DEBUG)
                break;
            worldHandleMsg(client->link,pak);

        xcase CLIENT_BUG:
        {
            char *mapName, *desc, *clientVersion;
            char * playerName = client->entity?client->entity->name:"no entity";
            char * report = NULL;
            char * specs;
            char * memStats;
            Vec3 pos, pyr;
            F32 camdist;
            int mode, third, buf_idx = 0;

            mode = pktGetBitsPack(pak, 2);  // whether log is normal (stored on server), cryptic-internal (stored on server), or local
            desc    = strdup(pktGetString(pak));
            clientVersion = strdup(pktGetString(pak));
            mapName    = strdup(pktGetString(pak)); // Will need to duplicate if more strings are ever added!!!
            specs    = strdup(pktGetString(pak));
            memStats = strdup(pktGetString(pak));
            pos[0]  = pktGetF32(pak);
            pos[1]  = pktGetF32(pak);
            pos[2]  = pktGetF32(pak);
            pyr[0]  = pktGetF32(pak);
            pyr[1]  = pktGetF32(pak);
            pyr[2]  = pktGetF32(pak);
            camdist = pktGetF32(pak);
            third   = pktGetBitsPack(pak, 1);

            bugStartReport(&report, mode, desc);

            bugAppendVersion(&report, clientVersion);
            bugAppendMissionLine(&report, client->entity);
            bugAppendPosition(&report, mapName, pos, pyr, camdist, third);
            bugAppendClientInfo(&report, client, playerName);
            bugAppendServerInfo(&report);
            estrConcatf(&report, "\n%s\n", memStats);
            estrConcatf(&report, "\n%s\n", specs);

            bugEndReport(&report);

            bugLogBug(&report, mode, client->entity);

            free(desc);
            free(clientVersion);
            free(mapName);
            free(specs);
            free(memStats);

        }

        xcase CLIENT_ACCOUNT_SERVER_CMD:
        {
            AccountServerCmdType acmd;
            int auth_id;
            int db_id;

            auth_id = SAFE_MEMBER(client->entity, auth_id);
            db_id = SAFE_MEMBER(client->entity, db_id);

            acmd = pktGetBitsAuto(pak);

            switch (acmd)
            {
                case ACCOUNT_SERVER_CMD_CHAR_COUNT:
                    if (client->entity && client->entity->pl)
                    {
                        bool vip = AccountIsVIP(ent_GetProductInventory(client->entity), client->entity->pl->account_inventory.accountStatusFlags) != 0;
                        dbRelayGetAccountServerCharacterCounts(auth_id, vip);
                    }
                    break;

                case ACCOUNT_SERVER_CMD_GET_CATALOG:
                    dbRelayGetAccountServerCatalog(client);
                    break;

                case ACCOUNT_SERVER_CMD_GET_INVENTORY:
                    {
                        dbRelayGetAccountServerInventory(auth_id);
                        break;
                    }

                default:
                    dbLog("cheater", client->entity, "Player with authname %s sent an invalid account server command (%d)", client->entity ? client->entity->auth_name : "UNKNOWN", acmd);
                    break;
            }
        }

        xcase CLIENT_LOGPRINTF:
        {
            char *logString = pktGetString(pak);
            if (logString)
            {
                dbLog("CLIENT_LOGPRINTF", client->entity, logString);
            }
        }

        xcase CLIENT_BASEPOWER_NOT_FOUND:
        {
            int receivingEntityDBID = pktGetBitsAuto(pak);
            int receivingEntityID = pktGetBitsAuto(pak);
            int crcFullName = pktGetBitsAuto(pak);
            Entity *receivingEntity = entFromDbIdEvenSleeping(receivingEntityDBID); // This message can be sent during ent load on the client, while ent is still sleeping
            const BasePower *power = powerdict_GetBasePowerByCRC(&g_PowerDictionary, crcFullName);
            if (!receivingEntity)
                receivingEntity = entFromId(receivingEntityID);


            LOG_ENT( receivingEntity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "BasePower<%s>:NotFound received on client<%d>", power ? power->pchFullName : "unknown", crcFullName );
        }

        xcase CLIENT_LWC_STAGE:
        {
            int entityDBID = pktGetBitsAuto(pak);
            int entityID = pktGetBitsAuto(pak);
            int lwc_stage = pktGetBitsAuto(pak);

            Entity *entity = entFromDbIdEvenSleeping(entityDBID);
            if (!entity)
                entity = entFromId(entityID);

            if (entity && entity->pl)
            {
                entity->pl->lwc_stage = lwc_stage;
                //printf("CLIENT_LWC_STAGE received %d, %d\n", lwc_stage, entity->pl->lwc_stage);
            }
        }

        xcase CLIENT_STOP_FORCED_MOVEMENT:
        {
            if (client->entity && client->entity->pchar && client->entity->pchar->bForceMove)
            {
                client->entity->pchar->bForceMove = false;
            }
        }

        xdefault:
            LOG_ENT( client->entity, LOG_CHEATING, LOG_LEVEL_IMPORTANT, 0, "Player with authname %s sent an unknown command to svrHandleClientMsg()", client->entity?client->entity->auth_name:"UNKNOWN");
    }

    if(timerStarted){
        PERFINFO_AUTO_STOP();
    }

    return 1;
}

static void svrTimedBackups()
{
    U32            mask_get_online;
    int            i,count=0;
    static        int        max_count;
    static        Entity    **ents;
    static        U32        mask_get_online_info_static = 15,    // static maps get online info every 16 seconds
                        mask_get_online_info_mission = 63,    // mission maps get online info every 64 seconds
                        mask_charsave = 1023;                // Saves each character once every 17.05 minutes
    static        F32        fTimeBetweenSaves = 1.0f;
    static        U32        counter=0;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];
    static        int timer=-1;
    static      U32 periodic_timer = 0;
    BOOL        log_periodic_info = FALSE;
    
    if (timer < 0)
        timer = timerAlloc();
    if (timerElapsed(timer) < fTimeBetweenSaves)
        return;

    // Restart the timer
    timerAdd(timer, -fTimeBetweenSaves);
    counter = (counter+1) & mask_charsave;
    if (db_state.is_static_map)
        mask_get_online = mask_get_online_info_static;
    else
        mask_get_online = mask_get_online_info_mission;
    if ((counter & mask_get_online)==0 && players->count) {
        dbUpdateAllOnlineInfo();
    }

    // periodic timer
    if(timerSecondsSince2000CheckAndSet(&periodic_timer,30))
        log_periodic_info = TRUE;

    for(i=0;i<players->count;i++)
    {
        Entity *e = players->ents[i];
        if (e && !e->dbcomm_state.map_xfer_step && e->ready)
        {
            if ((e->db_id & mask_charsave)==counter ||
                (e->auctionPersistFlag && e->auctionPersistTimer <= 0.0f))
            {
                dynArrayAddp(&ents, &count, &max_count, e);
                e->auctionPersistFlag = false;
                e->auctionPersistTimer = ENT_SAVE_COOLDOWN;
                if(e->auc_trust_gameclient)
                    e->auctionPersistTimer /= 5; // each test clients should act like 5 players

                // check for pending transactions
                checkPendingTransactions(e);
            } 
            else if (e->auctionPersistTimer > 0.0f)
            {
                e->auctionPersistTimer -= fTimeBetweenSaves;
            } 
        }

        // currently we get here once per second
        // seems like a good place to log periodic entity info
        if(log_periodic_info)
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "PeriodicInfo pos=<%.2f,%.2f,%.2f>",ENTPOSPARAMS(e));
    }

    if (count) {
        PERFINFO_AUTO_START("svrSendEntListToDb", 1);
            //printf("saving %d characters...\n", count);
            svrSendEntListToDb(ents,count);
        PERFINFO_AUTO_STOP();
    }
}

static void showTitleBarStats()
{
    int        seconds;
    char    buf[1000];

    seconds = timerCpuSeconds() - db_comm_link.lastRecvTime;
    sprintf(buf,
        "%d:%d(%s%d): %s Port=%d Ents=%d Players=%d (H:%d/V:%d) Connecting=%d %s",
            db_state.map_id,
            _getpid(),
            db_comm_link.lastRecvTime ? "" : "LOST DB CONNECTION: ",
            db_comm_link.lastRecvTime ? seconds : 0,
            db_state.map_name,
            server_state.udp_port,
            entCount(),
            net_links.links->size,
            svr_hero_count, svr_villain_count,
            player_ents[PLAYER_ENTS_CONNECTING].count,
            pktGetDebugInfo()?"(PACKET DEBUG ON)":"");
    if(server_state.mission_server_test)
    {
        int checked, total, invalids, uniques;
        missionServerMapTest_getStatus(&checked, &total, &invalids, &uniques);
        sprintf(buf+strlen(buf), "Mission Server Validation: (%d/%d) complete.  %d total new invalidations, %d unique",
            checked, total, invalids, uniques);
    }
    setConsoleTitle(buf);
}

static void sendPerformanceUpdateLayout(Packet* pak, PerformanceInfo* info)
{
    PerformanceInfo* child;

    pktSendBits(pak, 1, 1);

    if(isDevelopmentMode())
        assert(info->uid < timing_state.autoTimerCount);

    pktSendBitsPack(pak, 7, info->uid);
    pktSendString(pak, info->locName);
#if defined(_M_X64)
    #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBits(pak, 32, (U32)(uintptr_t)info->rootStatic);

    // There's currently 3 info types, and it's usually set to 0, thus this piece of code.
    pktSendBits(pak, 1, info->infoType ? 1 : 0);

    if(info->infoType)
        pktSendBits(pak, 1, info->infoType - 1);

    for(child = info->child.head; child; child = child->nextSibling)
        sendPerformanceUpdateLayout(pak, child);

    pktSendBits(pak, 1, 0);
}

static void sendBits64(Packet* pak, U64 value64)
{
    U32* value32 = (U32*)&value64;
    U64 value = value64;
    int byte_count;

    for(byte_count = 1; byte_count < 9; byte_count++){
        if(value <= 255)
            break;

        value >>= 8;
    }

    assert(byte_count <= 8);

    pktSendBits(pak, 3, byte_count - 1);

    if(byte_count > 4){
        pktSendBits(pak, 32, *value32);
        value32++;
        byte_count -= 4;
    }

    pktSendBits(pak, byte_count * 8, *value32);
}

static void sendPerformanceUpdate(ClientLink* client, int clear)
{
    int i;
    int count;
    __int64 timePassed = 0;
    __int64 timeUsed = 0;
    int sendRunCount = 0;
    int sendTotalTicks = 0;
    Packet* pak = pktCreate();
    PerformanceInfo* info;
    int old_runperfinfo;

    pktSetCompression(pak, 1);

    pktSendBitsPack(pak, 1, SERVER_PERFORMANCE_UPDATE);

    if(clear)
    {
        pktSendBits(pak, 32, 0);
        pktSend(&pak, client->link);
        return;
    }

    // Turn off the timer so that it doesn't get changed while it's being sent.

    old_runperfinfo = timing_state.runperfinfo;
    timing_state.runperfinfo = 0;

    // Send the layout if it is different since I sent it last.

    pktSendBits(pak, 32, timing_state.autoTimersLayoutID);

    if(client->autoTimersLayoutID == timing_state.autoTimersLayoutID)
    {
        pktSendBits(pak, 1, 0);
    }
    else
    {
        pktSendBits(pak, 1, 1);

        pktSendBits(pak, 1, timing_state.send_reset ? 1 : 0);

        client->autoTimersLayoutID = timing_state.autoTimersLayoutID;

        pktSendBitsPack(pak, 1, timing_state.autoTimerCount);

        for(info = timing_state.autoTimerRootList; info; info = info->nextSibling)
        {
            sendPerformanceUpdateLayout(pak, info);
        }

        pktSendBits(pak, 1, 0);
    }

    // Send the updates for all the timers.

    count = global_state.global_frame_count - client->autoTimersLastSentFrame;

    count = max(0, min(10, count));

    client->autoTimersLastSentFrame = global_state.global_frame_count;

    // Send the history update count.

    pktSendBitsPack(pak, 4, 1);

    // Send the total server time.

    sendBits64(pak, timing_state.totalTime);

    // Send the CPU speed.

    sendBits64(pak, getRegistryMhz());

    // Send the last CPU time used and passed.

    for(i = 0; i < ARRAY_SIZE(timing_state.totalCPUTimePassed) - 1; i++)
    {
        int j = (timing_state.totalCPUTimePos + ARRAY_SIZE(timing_state.totalCPUTimePassed) - 1 - i) % ARRAY_SIZE(timing_state.totalCPUTimePassed);

        timePassed += timing_state.totalCPUTimePassed[j];
        timeUsed += timing_state.totalCPUTimeUsed[j];
    }

    sendBits64(pak, timePassed);
    sendBits64(pak, timeUsed);

    // Send the types of data that will be sent.

    client->autoTimersSentCount++;

    if(client->autoTimersSentCount % 4 == 0)
    {
        sendRunCount = 1;
        sendTotalTicks = 1;
    }

    pktSendBits(pak, 1, sendRunCount ? 1 : 0);
    pktSendBits(pak, 1, sendTotalTicks ? 1 : 0);

    for(info = timing_state.autoTimerMasterList.head; info; info = info->nextMaster)
    {
        int j;
        S64 cyclesTotal = 0;
        U32 countTotal = 0;

        // Get the total of the last "count" history counts.

        for(j = 0; j < count; j++)
        {
            int k = (info->historyPos - (j + 1) + ARRAY_SIZE(info->history)) % ARRAY_SIZE(info->history);

            cyclesTotal += info->history[k].cycles;
            countTotal += info->history[k].count;
        }

        //if(stricmp(timing_state.autoTimers[i].name, "entPrepareUpdate") == 0){
        //    printf("%s - %I64u\n", timing_state.autoTimers[i].name, value64);
        //}

        if(!countTotal){
            pktSendBits(pak, 1, 0);
        }else{
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, 4, countTotal);
            sendBits64(pak, cyclesTotal);
        }

        if(sendRunCount)
        {
            sendBits64(pak, info->opCountInt);
        }

        if(sendTotalTicks)
        {
            sendBits64(pak, info->totalTime);
            sendBits64(pak, info->maxTime);
        }
    }

    // Turn the timer back on.

    timing_state.runperfinfo = old_runperfinfo;

    pktSend(&pak, client->link);
}

static void sendPerformanceUpdates(ClientLink** clients, int count)
{
    int i;
    
    for(i = 0; i < count; i++)
    {
        ClientLink* client = clients[i];
        int clear;

        if(timing_state.runperfinfo && client->sendAutoTimers)
        {
            clear = 0;
        }
        else
        {
            client->autoTimersLayoutID = 0;
            clear = 1;
        }

        sendPerformanceUpdate(client, clear);
    }
}

enum {
    STAT_RELIABLE_SIZE,
    STAT_LOST_IN,
    STAT_LOST_OUT,
    STAT_MAX
};

static struct {
    int var;
    char *name;
} mapping[] = {
    {STAT_RELIABLE_SIZE, "reliablePacketsArray.size"},
    {STAT_LOST_IN, "lost_packet_recv_count"},
    {STAT_LOST_OUT, "lost_packet_send_count"},
};



#include <conio.h>

void svrDumpNetworkStats(int all)
{
    int total[STAT_MAX];
    int max[STAT_MAX];
    int min[STAT_MAX];
    bool first=true;
    int i;
    char buf[256];
    int count=0;

    for(i=net_links.links->size-1;i>=0;i--)
    {
        ClientLink *client = ((NetLink*)net_links.links->storage[i])->userData;
        if (!client->entity)
            continue;

        count++;
        if (first) {
            max[STAT_RELIABLE_SIZE] = min[STAT_RELIABLE_SIZE] = client->link->reliablePacketsArray.size;
            max[STAT_LOST_IN] = min[STAT_LOST_IN] = client->link->lost_packet_recv_count;
            max[STAT_LOST_OUT] = min[STAT_LOST_OUT] = client->link->lost_packet_sent_count;
            total[STAT_RELIABLE_SIZE] = total[STAT_LOST_IN] = total[STAT_LOST_OUT] = 0;
            first = false;
        }
#define COUNT(id, var) \
    max[id] = MAX(max[id], var); \
    min[id] = MIN(min[id], var); \
    total[id] += var;
        COUNT(STAT_RELIABLE_SIZE, client->link->reliablePacketsArray.size);
        COUNT(STAT_LOST_IN, client->link->lost_packet_recv_count);
        COUNT(STAT_LOST_OUT, client->link->lost_packet_sent_count);
    }
    timerMakeDateString(buf);
    if (all) {
        printf("%-30s***%10s **%10s **%10s **%10s **\n", buf, "Total", "Min", "Max", "Average");
    }
    if (first) {
        if (all)
            printf("   No active connections\n");
    } else {
        for (i=0; i< (int)(all?ARRAY_SIZE(mapping):1); i++) {
            if (max[mapping[i].var] > 10 || all) {
                if (!all) {
                    printf("%s ", buf);
                }
                printf("%-30s   %10d   %10d   %10d   %10f\n", mapping[i].name, total[mapping[i].var], min[mapping[i].var], max[mapping[i].var], total[mapping[i].var] / (float)count);
            }
        }
        if (all)
            printf("\n");
    }

}

static void svrShowBeaconInfo()
{
    int i;
    
    beaconPrintDebugInfo();
    
    consoleSetFGColor(COLOR_BRIGHT|COLOR_RED);
    
    printf("Press 'V' to verify beacon file ('F' for full info): ");
    
    for(i = 10; i > 0; i--)
    {
        int key;
        
        printf("%d...", i);
        
        if(_kbhit() && strchr("vfr", key = tolower(_getch())))
        {
            if(key == 'r'){
                beaconRequestBeaconizing(db_state.map_name);
            }else{
                consoleSetFGColor(COLOR_BRIGHT|COLOR_GREEN);
                printf("Verifing: \n");
                consoleSetDefaultColor();
                if(beaconDoesTheBeaconFileMatchTheMap(key == 'f'))
                {
                    consoleSetFGColor(COLOR_BRIGHT|COLOR_GREEN);
                    printf("Beacon file Matches!");
                }
                else
                {
                    consoleSetFGColor(COLOR_BRIGHT|COLOR_RED);
                    printf("Beacon file DOES NOT MATCH!!!");
                }
            }
                        
            break;
        }
        
        if(i > 1)
        {
            Sleep(200);
        }
    }
    
    if(!i)
    {
        printf("CANCELED!");
    }
    consoleSetDefaultColor();
    printf("\n");
}

static void svrDebugHook()
{
    static int debug_enabled=0;
    static float timestep_acc;
    static float time_interval = 30;
    static float no_key_time_acc;
    extern int world_modified;
    extern char world_name[MAX_PATH];

    timestep_acc += TIMESTEP_NOSCALE;

    if(timestep_acc >= time_interval){
        if (_kbhit()) {
            no_key_time_acc = 0;

            if (debug_enabled<2) {
                switch (_getch()) {
                    case 'd':
                        debug_enabled++;
                        time_interval = 5;
                        if (debug_enabled==2) {
                            printf("Debugging hotkeys enabled (Press '?' for a list).\n");
                            time_interval = 0;
                        }
                        break;
                    
                    case 's':
                        if (isDevelopmentMode()) {
                            int numBytes=groupSave(world_name);    
                            int i;
                            for(i=net_links.links->size-1;i>=0;i--)
                            {
                            ClientLink * client = ((NetLink*)net_links.links->storage[i])->userData;
                             if (!client->entity)
                                     continue;
                                    consoleSave(client->link,numBytes);
                            }
                            if (numBytes)
                                printf("Map saved.\n");
                            break;
                        }

                    default:
                    debug_enabled = 0;
                }
            } else {
                time_interval = 0;

                switch(_getch()) {
                    xcase 'a':
                        printSharedMemoryUsage();
                    xcase 'b':
                        svrShowBeaconInfo();
                    xcase 'c':
                        relayCommandLogToggle();
                    xcase 'd':
                        // Don't use this letter, since you tap D to get into debugging mode
                    xcase 'e':
                        ParserDumpStructAllocs();
                    xcase 'f':
                        {
                            int result;
                            loadstart_printf("Trying to reconnect... ");
                            result = connectToDbserver(5.f);
                            loadend_printf(result?"success":"failed");
                        }
                    xcase 'h':
                        heapInfoLog();
                    xcase 'l':
                    {
                        int doDump = 0;
                        printf("Are you sure you want to dump to memlog (type \"yes\")?  (This can take a LONG time): ");
                        if(_getch() == 'y'){
                            printf("y");
                            if(_getch() == 'e'){
                                printf("e");
                                if(_getch() == 's'){
                                    printf("s");
                                    doDump = 1;
                                }
                            }
                        }
                        if(doDump){
                            printf("\nDumping memory to memlog.txt...");
                            memCheckDumpAllocs();
                            printf("done.\n");
                        }else{
                            printf("Canceled!\n");
                        }
                    }
                    xcase 'm':
                        memMonitorDisplayStats();
                    xcase 'n':
                        svrDumpNetworkStats(1);
                    xcase 'p':
                        printf("SERVER PAUSED FOR 2 SECONDS PRESS 'p' AGAIN TO PAUSE INDEFINITELY...\n");
                        Sleep(2000);
                        if (_kbhit() && _getch()=='p') {
                            printf("SERVER PAUSED\n");
                            _getch();
                        }
                        printf("Server resumed\n");
                    xcase 'r':
                        {
                            int doDump = 0;
                            printf("Confirm write of shared heap log. This can impact other host processes (type \"yes\"): ");
                            if(_getch() == 'y'){
                                printf("y");
                                if(_getch() == 'e'){
                                    printf("e");
                                    if(_getch() == 's'){
                                        printf("s\n");
                                        doDump = 1;
                                    }
                                }
                            }
                            if(doDump){
                                printSharedHeapInfo();
                            }else{
                                printf("Canceled!\n");
                            }
                        }
                    xcase 's': 
                        if (isDevelopmentMode()) {
                            int numBytes=groupSave(world_name);    
                            int i;
                            for(i=net_links.links->size-1;i>=0;i--)
                            {
                            ClientLink * client = ((NetLink*)net_links.links->storage[i])->userData;
                             if (!client->entity)
                                     continue;
                                    consoleSave(client->link,numBytes);
                            }
                            if (numBytes)
                                printf("Map saved.\n");
                        }
                    xcase 't':
                        printStringTableMemUsage();
                    xcase 'x':
                        printStashTableMemDump();
                    xcase 'D':
                            DumpSpawndefs();
                    xcase 'H':
                        heapValidateAll();
                    xdefault:
                        printf("Keyboard shortcuts for mapserver debugging:\n");
                        printf("  a - print shared memory usage\n");
                        printf("  b - print beacon info\n");
                        printf("  c - toggle logging of relay commands\n");
                        printf("  e - print ParserAllocStruct memory usage\n");
                        printf("  f - try to reconnect to the DbServer if disconnected\n");
                        printf("  h - print some Heap info\n");
                        printf("  l - dump memlog.txt\n");
                        printf("  m - print memory usage\n");
                        printf("  n - print some network stats\n");
                        printf("  p - pause server\n");
                        printf("  r - dump shared heap info\n");
                        printf("  s - save\n");
                        printf("  H - validate heaps\n");
#ifdef STRING_TABLE_TRACK
                        printf("  t - string table dump\n");
#endif
#ifdef HASH_TABLE_TRACK
                        printf("  x - hash table dump\n");
#endif

                }
            }
        }
        else if (time_interval < 30) {
            if(no_key_time_acc < 30 * 10){
                no_key_time_acc += timestep_acc;

                if(no_key_time_acc >= 30 * 10){
                    time_interval = 30;
                    
                    printf("Enabling slow-debug-key-check mode (press ? for command list).\n");
                }
            }
        }

        timestep_acc = 0;
    }
}

void newSceneFile()
{
/*    sceneLoad(server_state.newScene);
#if CLIENT
    texCheckSwapList();
    lightReapplyColorSwaps();

#endif
    free(server_state.newScene);
    server_state.newScene = 0;*/
}

static void svrTickUpdateNetSend()
{
    S32 i;

    if(server_state.net_send.update_period <= 0)
    {
        // Initialize to 4 updates per second.

        server_state.net_send.update_period = 30.0 / 4.0;
    }

    server_state.net_send.acc += TIMESTEP_NOSCALE + global_state.frame_time_30_discarded;

    if(server_state.net_send.acc >= server_state.net_send.update_period)
    {
        server_state.net_send.send_on_this_tick = 1;

        server_state.net_send.acc = fmod(server_state.net_send.acc, server_state.net_send.update_period);
    }
    else
    {
        server_state.net_send.send_on_this_tick = 0;
    }

    // Reset counter based on previous type.

    for(i = 0; i < ARRAY_SIZE(server_state.process.timestep_acc); i++)
    {
        if(i <= server_state.process.max_rate_this_tick)
            server_state.process.timestep_acc[i] = 0;

        server_state.process.timestep_acc[i] += TIMESTEP;
    }

    if(!server_state.net_send.send_on_this_tick)
    {
        server_state.process.max_rate_this_tick = ENT_PROCESS_RATE_ALWAYS;
    }
    else
    {
        server_state.net_send.count++;

        server_state.net_send.is_odd_send = server_state.net_send.count & 1;

        if(server_state.net_send.is_odd_send)
        {
            server_state.process.max_rate_this_tick = ENT_PROCESS_RATE_ON_ODD_SEND;
        }
        else
        {
            server_state.process.max_rate_this_tick = ENT_PROCESS_RATE_ON_SEND;
        }
    }
}

static void executePlayerControls()
{
    int i;
    
    player_count = 0;
    for(i=0;i<net_links.links->size;i++)
    {
        NetLink* link = net_links.links->storage[i];
        ClientLink* client = link->userData;
        Entity* e = client->entity;
        
        if(e && entity_state[e->owner] & ENTITY_IN_USE)
        {
            svrTickUpdatePlayerFromControls(client);
        }
    }
}

static void checkDbReconnect(void)
{
    // Only in local mapserver mode, and when no one is connected (lest it crash because of a protocol
    //  version change while a user is editing things).
    if (!db_comm_link.connected && db_state.local_server && !net_links.links->size && isDevelopmentMode())
    {
        static int timer;
        int result;
        F32 elapsed;
        if (!timer) {
            timer = timerAlloc();
            elapsed = 100000;
        } else {
            elapsed = timerElapsed(timer);
        }
        if (elapsed > 15) {
            loadstart_printf("Lost connection to DbServer, trying to reconnect... ");
            db_state.map_id = -1;
            result = connectToDbserver(2.5f);
            loadend_printf(result?"success":"failed");
            if (!result) { // Second one needed
                loadend_printf("failed");
            }
            timerStart(timer);
        }
    }
}

void logDisconnect(ClientLink *client, const char * reason)
{
    const char * msg = "";
    if( reason )
        msg = reason;

    if(client && client->entity && client->entity->pchar)
    {
        static const SalvageItem *pMerits = NULL;
        static const SalvageItem *pTickets = NULL;
        int        tickets, merits;

        if (pMerits == NULL)
        {
            pMerits = salvage_GetItem("S_MeritReward");
        }
        if (pTickets == NULL)
        {
            pTickets = salvage_GetItem("S_ArchitectTicket");
        }
        if (pMerits)
        {
            merits = character_SalvageCount(client->entity->pchar, pMerits);
        } else {
            merits = 0;
        }
        if (pTickets)
        {
            tickets = character_SalvageCount(client->entity->pchar, pTickets);
        } else {
            tickets = 0;
        }

        LOG_ENT( client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "[Disconnect:%s] %s %s, Level:%d XPLevel:%d XP:%d HP:%.2f End:%.2f Debt: %d Inf:%d AccessLvl: %d WisLvl: %d Wis: %d Merits: %d Tickets: %d",
            msg,
            client->entity->pchar->porigin->pchName,
            client->entity->pchar->pclass->pchName,
            client->entity->pchar->iLevel+1,
            character_CalcExperienceLevel(client->entity->pchar)+1,
            client->entity->pchar->iExperiencePoints,
            client->entity->pchar->attrCur.fHitPoints,
            client->entity->pchar->attrCur.fEndurance,
            client->entity->pchar->iExperienceDebt,
            client->entity->pchar->iInfluencePoints,
            client->entity->access_level,
            client->entity->pchar->iWisdomPoints,
            client->entity->pchar->iWisdomLevel,
            merits, 
            tickets);
    }
}

static int svrTickCheckDisconnect(NetLink* link, ClientLink* client, int max_buffered_packets)
{
    if(!client->ready || link->reliablePacketsArray.size < max_buffered_packets)
    {
        return 0;
    }
    
    if (db_state.local_server)
    {
        int ret;
        while(link->reliablePacketsArray.size >= max_buffered_packets)
        {
            char    buf[1000];

            sprintf(buf,"Waiting for client %s(%s) to accept packets",makeIpStr(link->addr.sin_addr.s_addr),client->entity->name);
            setConsoleTitle(buf);
            ret = netLinkMonitor(client->link,0,0);
            if (ret == LINK_DISCONNECTED)
                break;
            Sleep(1);
        }
        if (ret == LINK_DISCONNECTED){
            return 1;
        }
        return 0;
    }
    else
    {
        logDisconnect(client, "TickCheck");
        netRemoveLink(client->link);
        return 1;
    }
}

static void svrTickSendQueuedErrors(NetLink* link, ClientLink* client)
{
    char *s,*mem;

    while(s = (char*)errorGetQueued())
    {
        if (server_error_sent_count < 3) 
        {
            mem = malloc(strlen(s) + 100);
            sprintf(mem,"Message from mapserver %d\n%s",db_state.map_id,s);
            sendEditMessage(link, 3, "%s",mem);
            free(mem);
            server_error_sent_count++;
        } else if (server_error_sent_count==3)
        {
            mem = malloc(strlen(s) + 150);
            sprintf(mem,"FINAL Message from mapserver %d\nNO MORE MESSAGES WILL BE SENT\nCheck the mapserver console for more messages.\n%s",db_state.map_id,s);
            sendEditMessage(link, 3, "%s",mem);
            free(mem);
            server_error_sent_count++;
        }
    }
}


static void updateAllBuffStashes(void)
{
    int i;

    for(i=1;i<entities_max;i++)
    {
        //if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];
            if( e && e->pchar && e->pchar->update_buffs)
                character_UpdateBuffStash(e->pchar);
        }
    }
}

static void sendWorldChanges(ClientLink* client, int world_update, NetLink *link, int fullWorld) 
{
    if (client->ready == CLIENTSTATE_IN_GAME)
    {
        if (world_update) 
        {  // Only send world updates if the player has already gotten the full world
            PERFINFO_AUTO_START("worldSendUpdate", 1);
                worldSendUpdate(link,fullWorld);
            PERFINFO_AUTO_STOP();
        }

        // Sent separately from worldSendUpdate, which is too slow for normal gameplay stuff.
        PERFINFO_AUTO_START("worldSendDynamicDefChanges", 1);
            sendDynamicDefChanges(link);
        PERFINFO_AUTO_STOP();
    }
}

static void svrTickDoNetSend(int world_update)
{
    ClientLink*    sendPerfInfoClients[100]; // Arbitrary max people to send this to, should never actually be more than about 1.
    int            clientPacketLogEnabled = clientPacketLogIsEnabled(0);
    int            sendPerfInfoCount = 0;
    int            i;
    int            fullWorld = 0;
    static int    netSendIterationNumber = 0;

    netSendIterationNumber++;

    PERFINFO_AUTO_START("entPrepareUpdate", 1);

        if(server_state.net_send.is_odd_send)
        {
            PERFINFO_AUTO_START("odd", 1);
        }
        else
        {
            PERFINFO_AUTO_START("even", 1);
        }

            entPrepareUpdate();

        PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP_START("updateAllFriendLists", 1);

        updateAllFriendLists();
    
    PERFINFO_AUTO_STOP_START("for all net_links.links", 1);
        for(i=net_links.links->size-1;i>=0;i--)
        {
            int            max_buffered_packets = 400;
            NetLink*    link = net_links.links->storage[i];
            ClientLink*    client = link->userData;
            Entity*        clientEnt = client->entity;
            
            if (!clientEnt)
                continue;

            if (db_state.local_server || client->controls.notimeout)
                max_buffered_packets = 10000;
                
            trade_CheckFinalize( clientEnt );

            PERFINFO_AUTO_START("top", 1);

                if(svrTickCheckDisconnect(link, client, max_buffered_packets))
                {
                    PERFINFO_AUTO_STOP();
                    continue;
                }

                // Needs to stay north of entSendUpdate, since this may append to e->pak
                
                dbCheckMapXfer(clientEnt, link);

            PERFINFO_AUTO_STOP_START("entSendUpdate", 1);

                if(    client->ready == CLIENTSTATE_IN_GAME ||
                    client->ready == CLIENTSTATE_RE_REQALLENTS ||
                    client->ready && !ENTINFO(clientEnt).sent_self)
                {
                    // Send script debug information
                    
                    if (client->scriptDebugFlags & SCRIPTDEBUGFLAG_DEBUGON)
                    {
                        ScriptSendDebugInfo(client, client->scriptDebugFlags, client->scriptDebugSelect);
                    }
                    
                    // Send the update.

                    catch_ugly_entsend_bug_hack=1;
                    entSendUpdate( link, clientGetViewedEntity(client), client->controls.repredict, client->ready!=CLIENTSTATE_IN_GAME, netSendIterationNumber);
                    if (client->ready == CLIENTSTATE_RE_REQALLENTS)
                        client->ready=CLIENTSTATE_IN_GAME;
                    if (client->ready == CLIENTSTATE_ENTERING_GAME)
                        client->ready=CLIENTSTATE_IN_GAME;
                    catch_ugly_entsend_bug_hack=0;
                    
                    client->controls.repredict = 0;
                    
                    // Log packet stats.
                    
                    if(clientPacketLogEnabled)
                    {
                        clientSetPacketLogsSent(CLIENT_PAKLOG_OUT, clientEnt->entityPacket, clientEnt);
                    }

                    // Queue sending CPU usage meter thingy update.

                    if(    (client->sendAutoTimers || client->autoTimersLayoutID) &&
                        sendPerfInfoCount < ARRAY_SIZE(sendPerfInfoClients))
                    {
                        sendPerfInfoClients[sendPerfInfoCount++] = client;
                    }

                    // Send queued error messages to non-user clients.

                    if(clientEnt->access_level >= ACCESS_DEBUG){
                        svrTickSendQueuedErrors(link, client);
                    }

                    if(clientEnt->world_update)
                    {
                        clientEnt->world_update = 0;
                        world_update = 1;
                        fullWorld = 1;
                    }
                }
            PERFINFO_AUTO_STOP();

            sendWorldChanges(client, world_update, link, fullWorld);

            PERFINFO_AUTO_START("lnkBatchSend", 1);
                lnkBatchSend(link);
            PERFINFO_AUTO_STOP();

            if (link->overflow && !link->notimeout){
                LOG_OLD_ERR( "MapserverDisconnects.log",
                            "\n\tMapserver on %s disconnect %s from %s\n\tReason: netlink overflow\n",
                            getComputerName(),
                            clientEnt->auth_name,
                            stringFromAddr(&link->addr));

                PERFINFO_AUTO_START("netRemoveLink", 1);
                    netRemoveLink(link);
                PERFINFO_AUTO_STOP();

                continue;
            }
            //printf("client %d, %d reliable queued\n",i,link->reliable_count);

            PERFINFO_AUTO_START("setLogoutTimer", 1);
                if (!link->notimeout)
                {
                    #define TIMEOUTSECS 5*60
                    #define FILELOADTIMEOUTSECS 600
                    U32 timeout,dt;
                    int packets_unacked;

                    if (client->ready == CLIENTSTATE_IN_GAME)
                        timeout = TIMEOUTSECS;
                    else
                        timeout = FILELOADTIMEOUTSECS + TIMEOUTSECS;
                    dt = timerCpuSeconds() - link->lastRecvTime;
                    packets_unacked = link->nextID - link->idAck;
                    if (dt > timeout)
                    {
                        if (!clientEnt->logout_timer)
                        {
                            svrPlayerSetDisconnectTimer(clientEnt,1);
                        }
                    }
                    else if (clientEnt->logout_timer && clientEnt->logout_bad_connection)
                    {
                        clientEnt->logout_timer = 0;
                        clientEnt->logout_bad_connection = 0;
                        clientEnt->logout_update = 2;
                    }
                }
            PERFINFO_AUTO_STOP();
        }
    PERFINFO_AUTO_STOP();

    if(sendPerfInfoCount)
    {
        PERFINFO_AUTO_START("sendPerformanceUpdates", 1);
            sendPerformanceUpdates(sendPerfInfoClients, sendPerfInfoCount);
        PERFINFO_AUTO_STOP();
    }

    //Used as a temp. place for entities to pile strings that will be sent @ end of every frame.
    //initStringsForTick();
    PERFINFO_AUTO_START("entFinishUpdate", 1);

        if(server_state.net_send.is_odd_send)
        {
            PERFINFO_AUTO_START("odd", 1);
        }
        else
        {
            PERFINFO_AUTO_START("even", 1);
        }

        entFinishUpdate();
        updateAllBuffStashes();
        playerCreatedStoryArc_CleanStash();

        PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP();

    clearDynamicDefChanges();
    timing_state.send_reset = 0;
}

static void svrTickCheckNetSend()
{
    int            world_update;

    world_update = worldModified();

    if(world_update)
    {
        server_state.net_send.send_on_this_tick = 1;
    }

    if(server_state.net_send.send_on_this_tick)
    {
        if(world_update)
        {
            PERFINFO_AUTO_START("doNetSend:worldUpdate", 1);
        }
        else
        {
            PERFINFO_AUTO_START("doNetSend", 1);
        }
        
        svrTickDoNetSend(world_update);
        
        PERFINFO_AUTO_STOP();
    }
}

// tracks how many seconds the server has been idle for (i.e. without players)
// returns 0 if the server is not idle
U32    svrGetSecondsIdle(void)
{
    U32 idle_seconds = 0;    // zero means server is not idle

    // is the server actively in use? If so record the most recent time in use
    if (!s_svr_last_active_time || player_count )
    {
        s_svr_last_active_time = dbSecondsSince2000();
    }
    else
    {
        // server is idle, how long has it been idle?
        idle_seconds = dbSecondsSince2000() - s_svr_last_active_time;
    }
    return idle_seconds;
}

// force reset for the idle time so that we don't generate multiple idle request immediately
void svrResetSecondsIdle(void)
{
    s_svr_last_active_time = 0;
}

// periodic self maintenance when server has been idle
static void svrTickMaintenanceIdle(void)
{
    if (server_state.maintenance_idle)
    {
        static bool s_did_idle_maintenance;
        U32 idle_seconds = svrGetSecondsIdle();
        U32 idle_minutes = idle_seconds/60;
        bool busy = (idle_seconds==0);

        if (busy)
        {
            // if server is not idle schedule next idle upkeep,
            // should we become idle long enough
            s_did_idle_maintenance = false;
        }
        else
        {
            if (idle_minutes > server_state.maintenance_idle && !s_did_idle_maintenance)
            {
                printf("Performing idle maintenance.\n");
                // If we are idle we might as well trim our working
                // set and put our pages on the modified/standby lists
                // instead of having Windows guess if there is physical memory pressure
                compactWorkingSet(false);

                s_did_idle_maintenance = true;    // idle maintenance done, don't allow another one until we've been busy again
            }
        }
    }
}

// check if a daily maintenance window has been defined and perform full maintenance
// at a random point within the window

static void srvPerformDailyMaintenance(void)
{
    printf("Performing daily maintenance.\n");

    // clear struct link cache
    TextParser_ClearCaches();
}

static void svrTickMaintenanceDaily(void)
{
    // do we have an active service window on a static map?
    if (db_state.is_static_map && server_state.maintenance_daily_start < server_state.maintenance_daily_end)
    {
        static int s_last_daily_maintenance_day = 0;    // last day we did maintenance
        SYSTEMTIME tod;

        GetLocalTime(&tod);

        // new day
        if (tod.wDay != s_last_daily_maintenance_day)
        {
            static int s_maintenance_time = 0;    // target time of maintenance
            int minute_delta;

            if (!s_maintenance_time)
            {
                // select a random time within the maintenance window to
                // perform the maintenance so that all the servers don't do it at
                // the same time
                minute_delta = (server_state.maintenance_daily_end - server_state.maintenance_daily_start)*60;
                s_maintenance_time = randInt(minute_delta);
            }

            // have we reached the target time for maintenance?
            minute_delta = (tod.wHour - server_state.maintenance_daily_start)*60 + tod.wMinute;
            if ( minute_delta >= s_maintenance_time)
            {

                srvPerformDailyMaintenance();    // Do the maintenance

                s_last_daily_maintenance_day = tod.wDay;    // no maintenance until it's a new day
                s_maintenance_time = 0;    // force choice of a new maintenance time for next day
            }
        }
    }
}

// do any pending server maintenance
static void svrTickMaintenance(void)
{
    svrTickMaintenanceIdle();
    svrTickMaintenanceDaily();
    svrCheckUnusedAndEmpty();    // exit if we are no longer of use
}

static void svrTickTop()
{
    frame++;
    if(server_state.logServerTicks)
        LOG_OLD("----- Tick %i Began -----\n", TickCount++);

    if( server_state.newScene )
        newSceneFile();

    svrTickUpdateNetSend();

    if( server_state.reloadSequencers )
        entReloadSequencersServer();

    PERFINFO_AUTO_START("sgrpStats", 1);
    if( server_state.process.max_rate_this_tick == ENT_PROCESS_RATE_ON_ODD_SEND )
    {
        sgrpstats_FlushToDb( &db_comm_link );
        sgrprewards_Tick(false);
    }
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("MiningDataSend", 1);
    if( server_state.process.max_rate_this_tick == ENT_PROCESS_RATE_ON_ODD_SEND )
        MiningAccumulatorSendAndClear(&db_comm_link);
    PERFINFO_AUTO_STOP();

    checkDbReconnect();

    PERFINFO_AUTO_START("dbComm", 1);
        dbComm();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("shardCommMonitor", 1);
        shardCommMonitor();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("svrCheckQuitting", 1);
        svrCheckQuitting();
    PERFINFO_AUTO_STOP();

    if(server_state.net_send.send_on_this_tick && !(server_state.net_send.count & 3))
    {
        PERFINFO_AUTO_START("showTitleBarStats", 1);
            showTitleBarStats();
        PERFINFO_AUTO_STOP();
    }

    if (!server_state.enablePowerDiminishing && server_visible_state.isPvP)
        serverParseClientEx("setPowerDiminish 1", NULL, ACCESS_INTERNAL);


    //checkMission();

    global_state.global_frame_count++;

    PERFINFO_AUTO_START("updateTime", 1);
        updateTime(0);
    PERFINFO_AUTO_STOP();

    seqUpdateServerRandomNumber();

    PERFINFO_AUTO_START("geoCheckThreadLoader", 1);
        geoCheckThreadLoader();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("sharedHeapLazySync", 1);
        sharedHeapLazySync();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("FolderCacheDoCallbacks", 1);
        FolderCacheDoCallbacks();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("mpCompactPools", 1);
        mpCompactPools();
    PERFINFO_AUTO_STOP();

    if (global_state.global_frame_count > 10)
    {
        PERFINFO_AUTO_START("NMMonitor", 1);
            NMMonitor(POLL);
        PERFINFO_AUTO_STOP();
    }

    server_state.curClient = NULL;

    PERFINFO_AUTO_START("executePlayerControls", 1);
        executePlayerControls();
    PERFINFO_AUTO_STOP();

    // Overwrite the number of players currently connected to the mapserver.
    // This is here to test entity generator code.
    if(server_state.playerCount)
        player_count = server_state.playerCount;
}

void svrTick()
{
    PERFINFO_AUTO_START("svrTickTop", 1);
        svrTickTop();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("updateGameStateForEachEntity", 1);
        updateGameStateForEachEntity(TIMESTEP/30.0f);
    PERFINFO_AUTO_STOP_CHECKED("updateGameStateForEachEntity");

    if(!server_visible_state.pause)
    {
        PERFINFO_AUTO_START("nwStepScenes", 1);
#if NOVODEX // this has to be before execute entities, or at least not between that and the sending of updates
        if (nwEnabled())
        {
            nwStepScenes(TIMESTEP / 30.0f);
        }
#endif
        PERFINFO_AUTO_STOP_START("executeEntities", 1);
            executeEntities();
        PERFINFO_AUTO_STOP_START("executeGameLogic", 1);
            executeGameLogic();
        PERFINFO_AUTO_STOP_START("ArenaTick", 1);
            ArenaClientTick();
        PERFINFO_AUTO_STOP_START("baseSystemsTick", 1);
            baseSystemsTick(TIMESTEP/30.0f);
        PERFINFO_AUTO_STOP_START("RaidTick", 1);
            RaidTick();
        PERFINFO_AUTO_STOP_START("ScriptTick", 1);
            ScriptTick(TIMESTEP);
        PERFINFO_AUTO_STOP_START("EndgameRaidTick", 1);
            EndGameRaidTick();
        PERFINFO_AUTO_STOP_START("DoorArchitectEntryTick", 1);
            DoorArchitectEntryTick();
        PERFINFO_AUTO_STOP_START("MissionServerDatabaseFixup, only for dev mode", 1);
        if(server_state.mission_server_test)
            missionServerMapTest_tick();
        PERFINFO_AUTO_STOP_START("CutSceneCommands", 1);
            processCutScenes( TIMESTEP );
        PERFINFO_AUTO_STOP_START("PvPLogging", 1);
            pvp_LogTick();
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_START("svrTickBottom", 1);
        PERFINFO_AUTO_START("playerCheckFailedConnections", 1);
            playerCheckFailedConnections();
        PERFINFO_AUTO_STOP();

        svrTickCheckNetSend();

        PERFINFO_AUTO_START("odds and ends", 1);
            PERFINFO_AUTO_START("svrTimedBackups", 1);
                svrTimedBackups();
            PERFINFO_AUTO_STOP_START("playerEntCheckDisconnect", 1);
                playerEntCheckDisconnect();
            PERFINFO_AUTO_STOP_START("dbFlushLogs", 1);
                logFlush(0);
            PERFINFO_AUTO_STOP_START("emailCheckNewHeaders", 1);
                emailCheckNewHeaders();
            PERFINFO_AUTO_STOP_START("svrTickMaintenance", 1);
                svrTickMaintenance();
            PERFINFO_AUTO_STOP_START("svrDebugHook", 1);
                svrDebugHook();
            PERFINFO_AUTO_STOP_START("checkModel/TrickReload", 1);
                checkModelReload();
                checkTrickReload();
            PERFINFO_AUTO_STOP_START("netProfileStats", 1);
                netprofile_decrementTimer();
            PERFINFO_AUTO_STOP();
            beaconRequestUpdate();
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

