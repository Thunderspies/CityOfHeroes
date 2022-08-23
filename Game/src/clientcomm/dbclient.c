#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/netio.h>
#include "entity/costume.h"
#include "entity/costume_client.h"
#include "gameData/BodyPart.h"
#include "comm_backend.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/network/sock.h>
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/version/AppVersion.h>
#include <stdio.h>
#include "testClient/TestClient.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/utils.h>
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/version/AppRegCache.h>
#include "auth/authUserData.h"
#include "render/renderUtil.h"
#include <utilitieslib/network/netcomp.h>
#include "cmdparse/cmdgame.h"
#include "entity/classes.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiLogin.h"
#include "UI/uiServerTransfer.h"
#include "account/AccountData.h"
#include "account/AccountCatalog.h"
#include "UI/uiNet.h"
#include "UI/uiConsole.h"
#include <utilitieslib/components/HashFunctions.h>
#include "UI/uiDialog.h"
#include <utilitieslib/utils/file.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "player/inventory_client.h"
#include "clientcomm/authclient.h"
#include "clientcomm/clientcomm.h"
#include "graphics/font.h"


#if defined(TEST_CLIENT)
#include "testAccountServer.h"
#endif

DbInfo db_info;
static char db_err_msg[1000];
static int db_rename_status;
static int db_can_create_static_map_response = 0;

static int s_queue_position = -1;
static int s_queue_count = 0;

NetLink db_comm_link; // JE: made unstatick so that TestClients can foribly kill the link without sending a disconnect

int dbclient_getQueueStatus(int *position, int *count)
{
    if(position)
        *position = s_queue_position;
    if(count)
        *count = s_queue_count;
    return 1;
}

int dbclient_getTimeSinceMessage(U32 currentTime)
{
    U32 deltaTime;

    if( !(db_comm_link.connected && db_comm_link.lastRecvTime) )
        return -1;

    deltaTime = currentTime - db_comm_link.lastRecvTime;
        
    //xyprintf(2, 6, "Last Db comm: %d secs ago", deltaTime);
    if(deltaTime > DB_CONNECTION_TIMEOUT/2) {
        // set early warning indicator
        commSetTroubledMode(TroubledComm_Dbserver);
    }

    return deltaTime;
}

void dbclient_resetQueueStatus()
{
    s_queue_position = -1;
    s_queue_count = 0;
}

static int verifyPlayerReceived(PlayerSlot * player)
{
    int invalid = 0;
    if(player->name &&!stricmp(player->name, "empty"))
        return 0;
    invalid |= (!player->name || !strlen(player->name));
    invalid |= (!player->playerslot_class || !strlen(player->playerslot_class));
    invalid |= (!player->origin || !strlen(player->origin));
    return invalid;
}

static AccountInventorySet* getCurrrentPlayerInv( void )
{
    Entity* e = playerPtr();
    if (( e ) && ( e->pl ))
    {
        return &e->pl->account_inventory;
    }
    return NULL;
}

#if !defined(TEST_CLIENT)
extern LoginStage s_loggedIn_serverSelected;
extern LoginStage s_PostNDALoginStage;
#endif

static void freePlayers()
{
    int i;
    // Clean up db_info, free costumes
    if (db_info.players) {
        for (i=0; i<db_info.max_slots; i++) {
            if (db_info.players[i].costume) {
                costume_destroy(db_info.players[i].costume);
                db_info.players[i].costume = NULL;
            }
        }

        SAFE_FREE(db_info.players);
        db_info.player_count = 0;
    }
}

static void receivePlayersCommon(Packet *pak)
{
    int        i;

    db_info.player_count = 0;
    db_info.time_recvd = timerSecondsSince2000();
     db_info.max_slots = pktGetBitsPack(pak,1);
    db_info.bonus_slots = pktGetBitsPack(pak,1);
    db_info.vip = pktGetBitsPack(pak,1);
    db_info.showPremiumSlotLockNag = pktGetBitsPack(pak,1);

    freePlayers();
    db_info.players = calloc(db_info.max_slots,sizeof(db_info.players[0]));
    for(i=0;i<db_info.max_slots;i++)
    {
        int tmp;

        db_info.players[i].idx = i;
        db_info.players[i].level = pktGetBitsPack(pak,1);
        Strncpyt(db_info.players[i].name,pktGetString(pak));
        Strncpyt(db_info.players[i].playerslot_class,pktGetString(pak));
        Strncpyt(db_info.players[i].origin,pktGetString(pak));
        db_info.players[i].db_flags = pktGetBitsPack(pak,1);
        db_info.players[i].type = pktGetBitsPack(pak, 1);
        db_info.players[i].subtype = pktGetBitsPack(pak, 1);
        db_info.players[i].typeByLocation = pktGetBitsAuto(pak);
        db_info.players[i].praetorian = pktGetBitsPack(pak, 1);
        Strncpyt(db_info.players[i].primaryPower,pktGetString(pak));
        Strncpyt(db_info.players[i].secondaryPower,pktGetString(pak));

        if(!db_info.players[i].costume) // This will always be NULL, it was calloc'd above!
        {
            db_info.players[i].costume = costume_create(GetBodyPartCount());
        }
        costume_init(db_info.players[i].costume);

        costume_SetBodyType(db_info.players[i].costume, pktGetBitsPack(pak, 1));
        costume_SetScale(db_info.players[i].costume, pktGetF32(pak));
        costume_SetBoneScale(db_info.players[i].costume, pktGetF32(pak));
        tmp = pktGetBits(pak, 32);
        costume_SetSkinColor(db_info.players[i].costume, COLOR_CAST(tmp));
        Strncpyt(db_info.players[i].map_name,pktGetString(pak));
        db_info.players[i].map_seconds = pktGetBitsPack(pak,1);
        db_info.players[i].seconds_till_offline = pktGetBits(pak,32);
        db_info.players[i].last_online = pktGetBits(pak,32);
        db_info.players[i].slot_lock = pktGetBits(pak,2);
        db_info.players[i].invalid = verifyPlayerReceived(&db_info.players[i]);
        if (strchr(db_info.players[i].name, ':') != NULL)
        {
            // Name contains a ':' - this is a visitor.  Throw them away.
            // The costume should never be NULL, but even so ...
            if (db_info.players[i].costume != NULL)
            {
                costume_destroy(db_info.players[i].costume);
            }
            // Since db_info.players was calloc'ed above, I'm going to take off and nuke it from orbit.
            // It's the only way to be sure.
            memset(&db_info.players[i], 0, sizeof(db_info.players[0]));
            i--;
            db_info.max_slots--;
        }
        else if (stricmp(db_info.players[i].name, "empty") != 0)
        {
            db_info.player_count++;
        }
        else
        {
            db_info.players[i].invalid = 0;
        }
    }
    STATIC_INFUNC_ASSERT(sizeof(db_info.auth_user_data) == sizeof(g_overridden_auth_bits));

    pktGetBitsArray(pak,sizeof(db_info.auth_user_data) * 8,db_info.auth_user_data);
    pktGetBitsArray(pak,sizeof(g_overridden_auth_bits) * 8,g_overridden_auth_bits);

#if !defined(TEST_CLIENT)
    charSelectRefreshCharacterList();
    // Make sure we do not skip the NDA screen
    if (s_loggedIn_serverSelected == LOGIN_STAGE_NDA)
    {
        s_PostNDALoginStage = LOGIN_STAGE_CHARACTER_SELECT;
    }
    else
    {
        s_loggedIn_serverSelected = LOGIN_STAGE_CHARACTER_SELECT;
    }
#endif
}

#ifndef TEST_CLIENT
extern __int64 s_retrievingCharacterList_startMS;
#endif

static void handleSendQueueInfo(Packet *pak)
{
    int new_queue_postion = pktGetBitsAuto(pak);
    s_queue_count = pktGetBitsAuto(pak);

    if (s_queue_position == -1 || s_queue_position > new_queue_postion)
        s_queue_position = new_queue_postion;

#ifndef TEST_CLIENT
    if (!s_queue_position)
        s_retrievingCharacterList_startMS = timerMsecsSince2000();
#endif
}


static void handleSendPlayers(Packet *pak)
{
    char * command_str = 0;    
    //db_info.time_remaining = pktGetBitsAuto(pak);
    //db_info.reservations = pktGetBitsAuto(pak);

    auth_info.uid = pktGetBitsAuto(pak);
    command_str = pktGetString(pak);
    if( command_str && *command_str )
        cmdParse(command_str);
    cfg_setNagGoingRoguePurchase(pktGetBits(pak, 1));
    cfg_setIsBetaShard(pktGetBits(pak, 1));
    cfg_setIsVIPShard(pktGetBits(pak, 1));
    db_info.base_slots = pktGetBitsPack(pak,1);

    receivePlayersCommon(pak);

#if !defined(TEST_CLIENT)
    renameCharacterRightAwayIfPossible();
#endif
}

// void fakeSendPlayers(void)
// {
//     int        i;

//     db_info.player_count = 8;
//  freePlayers();
//     db_info.players = calloc(db_info.player_count,sizeof(db_info.players[0]));
//     for(i=0;i<db_info.player_count;i++)
//     {
//         int tmp;

//         db_info.players[i].idx = i;
//         db_info.players[i].level = 99;
//         if (i>0) {
//             Strncpyt(db_info.players[i].name,"EMPTY");
//         } else {
//             Strncpyt(db_info.players[i].name,"test");
//         }
//         Strncpyt(db_info.players[i].playerslot_class,"Class_Blaster");
//         Strncpyt(db_info.players[i].origin,"Origin_Science");

//         if(!db_info.players[i].costume) // This will always be NULL, it was calloc'd above!
//         {
//             db_info.players[i].costume = costume_create(GetBodyPartCount());
//         }
//         costume_init(db_info.players[i].costume);

//         costume_SetBodyType(db_info.players[i].costume, 0);
//         costume_SetScale(db_info.players[i].costume, 0);
//         costume_SetBoneScale(db_info.players[i].costume, 0);
//         tmp = 0xffffffff;
//         costume_SetSkinColor(db_info.players[i].costume, COLOR_CAST(tmp));
//         Strncpyt(db_info.players[i].map_name,"Nowheres");
//         db_info.players[i].map_seconds = 0;
//     }
// #if !defined(TEST_CLIENT)
//     charSelectRefreshCharacterList();
// #endif
// }

static void handleSendCostume(Packet *pak)
{
    int i, j;
    int idx;
    int numparts,num3Dscales,num2DScales;
    bool bSaving = false;
    CostumePart part = {0};

    idx = pktGetBitsPack(pak,1);
    if(idx>=0)
        bSaving = true;

    numparts = pktGetBitsPack(pak, 1);
    for(i=0; i<numparts; i++)
    {
        int subId = pktGetBits(pak,32);
         part.pchGeom = allocAddString(pktGetString(pak));
        part.pchTex1 = allocAddString(pktGetString(pak));
        part.pchTex2 = allocAddString(pktGetString(pak));
        part.pchName = allocAddString(pktGetString(pak));
        part.pchFxName = allocAddString(pktGetString(pak));
        
        for (j=0; j<ARRAY_SIZE(part.color); j++)
            part.color[j].integer = pktGetBits(pak, 32);

        if(bSaving)
        {
            StructCopy(ParseCostumePart, &part, db_info.players[idx].costume->parts[subId],0,0);
        }
    }

    if(bSaving)
    {
        db_info.players[idx].costume->appearance.iNumParts = GetBodyPartCount();
    }

    for( i = 0; i<db_info.players[idx].costume->appearance.iNumParts; i++ )
    {
         if(!db_info.players[idx].costume->parts[i]->pchGeom || !db_info.players[idx].costume->parts[i]->pchGeom[0] )
            db_info.players[idx].costume->parts[i]->pchGeom = allocAddString("none");
        if(!db_info.players[idx].costume->parts[i]->pchTex1 || !db_info.players[idx].costume->parts[i]->pchTex1[0] )
            db_info.players[idx].costume->parts[i]->pchTex1 = allocAddString("none");
        if(!db_info.players[idx].costume->parts[i]->pchTex2 || !db_info.players[idx].costume->parts[i]->pchTex2[0] )
            db_info.players[idx].costume->parts[i]->pchTex2 = allocAddString("none");
        if(!db_info.players[idx].costume->parts[i]->pchFxName || !db_info.players[idx].costume->parts[i]->pchFxName[0] )
            db_info.players[idx].costume->parts[i]->pchFxName  = allocAddString("none");
    }

    // get appearance (bone scale) info
    if (pktGetBits(pak, 1))
    {
        int currentBodyType, skinColor;

        currentBodyType = pktGetBitsPack(pak, 1);
        costume_SetBodyType(db_info.players[idx].costume, currentBodyType);

        skinColor = pktGetBits(pak, 32);
        costume_SetSkinColor(db_info.players[idx].costume, COLOR_CAST(skinColor) );

        num2DScales = pktGetBitsPack(pak, 1);
        num3Dscales = pktGetBitsPack(pak, 1);

        if(num2DScales && num3Dscales)
        {
            for(i=0;i<NUM_2D_BODY_SCALES && i<num2DScales;i++)
            {
                db_info.players[idx].costume->appearance.fScales[i] = pktGetF32(pak);
            }    

            db_info.players[idx].costume->appearance.convertedScale = pktGetBits(pak, 1);

            for(i=0;i<NUM_3D_BODY_SCALES && i<num3Dscales;i++)
            {
                db_info.players[idx].costume->appearance.compressedScales[i] = pktGetBitsPack(pak, 1);
            }

            decompressAppearanceScales(&db_info.players[idx].costume->appearance);
        }
    }    
    StructClear(ParseCostumePart, &part);
}


static void handleSendPowersetNames(Packet *pak)
{
    int idx;

    idx = pktGetBitsPack(pak,1);

    strcpy_s(db_info.players[idx].primaryPower, ARRAY_SIZE_CHECKED(db_info.players[idx].primaryPower), pktGetString(pak));
    strcpy_s(db_info.players[idx].secondaryPower, ARRAY_SIZE_CHECKED(db_info.players[idx].secondaryPower), pktGetString(pak));
}

void dbOrMapConnect(Packet *pak)
{
    int        link_id,map_id,ip_list[2],udp_port,tcp_port,login_cookie;
    MapServerConn    *mapserver = &db_info.mapserver;

    link_id        = pktGetBitsPack(pak,1); // only used by mapservers
    map_id        = pktGetBitsPack(pak,1);
    ip_list[0]    = pktGetBitsPack(pak,1);
    ip_list[1]    = pktGetBitsPack(pak,1);
    udp_port    = pktGetBitsPack(pak,1);
    tcp_port    = pktGetBitsPack(pak,1);
    login_cookie = pktGetBitsPack(pak,1);

    mapserver->ip        = ip_list[0];
    mapserver->port        = udp_port;
    mapserver->cookie    = login_cookie;
    mapserver->user_id    = 0;
}

void handleCanStartStaticMapResponse(Packet *pak)
{
    db_can_create_static_map_response = pktGetBitsPack(pak, 1);
}

void handleSendCharacterCounts(Packet *pak)
{
    int shards = pktGetBitsAuto(pak);
    if(shards)
    {
        int i;

        for(i = 0; i < shards; ++i)
        {
            char *shard = pktGetString(pak);
            int unlockedCount = pktGetBitsAuto(pak);
            int totalCount = pktGetBitsAuto(pak);
            int accountSlotCount = pktGetBitsAuto(pak); // excludes VIP slots
            int serverSlotCount = pktGetBitsAuto(pak);
            int vipOnlyStatus = pktGetBitsAuto(pak);
#if defined(TEST_CLIENT)
            testAccountAddCharacterCount(shard,unlockedCount);
#else
            if (!vipOnlyStatus || db_info.vip)
            {
                characterTransferAddCharacterCount(shard,unlockedCount,totalCount,accountSlotCount,serverSlotCount);
            }
#endif
        }

#if defined(TEST_CLIENT)
        testAccountReceiveCharacterCountsDone();
#else
        characterTransferReceiveCharacterCountsDone();
#endif
    }
    else
    {
        char *errmsg = pktGetString(pak);

#if defined(TEST_CLIENT)
        testAccountCharacterCountsError(errmsg);
#else
        characterTransferReceiveError(errmsg);
#endif
    }
}

void handleSendAccountServerCatalog(Packet *pak)
{
    accountCatalog_CacheAcctServerCatalogUpdate( pak );
    accountCatalogDone();
}

void handleAccountPlayNCAuthKeyUpdate(Packet *pak)
{
    int request_key = pktGetBitsAuto(pak);
    char * auth_key = pktGetString(pak);
    #if ! defined(TEST_CLIENT)        
        //PlaySpanStoreLauncher_ReceiveAuthKeyResponse(request_key, auth_key);
    #endif
}

void handleClientAuth(Packet *pak)
{
    U32 timeStamp = pktGetBitsAuto(pak);
    char* digest = pktGetString(pak);
    #if ! defined(TEST_CLIENT)    
        //PlaySpanStoreLauncher_SetDigest( timeStamp, digest );
    #endif
}

void receiveAccountServerUnavailable(Packet *pak)
{
    accountCatalogDone();
}

void receiveAccountServerInventoryDB(Packet *pak)
{
    int count, i;
    AccountInventory *pItem;
    Entity *e = playerPtr();
    bool authoritative = pktGetBits(pak, 1);
    if (db_info.accountInformationAuthoritative && !authoritative)
    {
        return;
    }

    // clean any old items in inventory
    AccountInventorySet_Release( &db_info.account_inventory );
    
    db_info.shardXferFreeOnly = pktGetBits(pak, 1);
    db_info.shardXferTokens = pktGetBitsAuto(pak);
    strncpy(db_info.shardname, pktGetString(pak), 100);
    db_info.accountInformationAuthoritative = authoritative;
    if (db_info.accountInformationAuthoritative == 0)
        db_info.accountInformationCacheTime = timerSecondsSince2000();
    else
        db_info.accountInformationCacheTime = 0;

    count = pktGetBitsAuto(pak);
    for (i = 0; i < count; i++)
    {
        pItem = (AccountInventory *) calloc(1,sizeof(AccountInventory));
        AccountInventoryReceiveItem(pak,pItem);
        eaPush(&db_info.account_inventory.invArr, pItem);
    }
    AccountInventorySet_Sort( &db_info.account_inventory );

    pktGetBitsArray(pak, LOYALTY_BITS, db_info.loyaltyStatus);
    db_info.loyaltyPointsUnspent = pktGetBitsAuto(pak);
    db_info.loyaltyPointsEarned = pktGetBitsAuto(pak);
    db_info.virtualCurrencyBal = pktGetBitsAuto(pak);
    db_info.lastEmailTime = pktGetBitsAuto(pak);
    db_info.lastNumEmailsSent = pktGetBitsAuto(pak);
    db_info.account_inventory.accountStatusFlags = pktGetBitsAuto(pak);
    db_info.bonus_slots = pktGetBitsAuto(pak);

    // need to update dummy entity
    if (e)
    {
        costumereward_clear( 0 );
        costumeAwardAuthParts(e);
        costumeAwardPowersetParts(e, 0, getPCCEditingMode() > 0);
    }
}

// This is a little hacky, but there is no easy way to create a dynamically generated
// va_list.  If you can do better feel free
static char * translateDbString( char * str )
{
    int count = 0;
    char * args[6] = {0};
    char * strtmp = strdup(str);
    char * out = NULL;

    count = tokenize_line_safe( strtmp, args, ARRAY_SIZE(args), 0);

#ifndef TEST_CLIENT
    if( !count || count >= 6 ) // too many args, don't try to translate
#endif
    {
        free(strtmp);
        return str;
    }

    switch( count )
    {
        case 1:
            out = textStd( args[0] );
            break;
        case 2:
            out =  textStd( args[0], args[1] );
            break;
        case 3:
            out =  textStd( args[0], args[1], args[2] );
            break;
        case 4:
            out =  textStd( args[0], args[1], args[2], args[3] );
            break;
        case 5:
            out =  textStd( args[0], args[1], args[2], args[3], args[4] );
            break;
        case 6:
            out =  textStd( args[0], args[1], args[2], args[3], args[4], args[5]  );
            break;
        default:
            break;
    }

    free(strtmp);

    return out;
}

static int dbHandleMessage(Packet *pak,int cmd,NetLink *link)
{
    char    buf[1000];

    switch(cmd)
    {
        xcase DBGAMESERVER_SEND_PLAYERS:
            handleSendPlayers(pak);
            return 1;
        xcase DBGAMESERVER_QUEUE_POSITION:
            handleSendQueueInfo(pak);
            return 1;
        xcase DBGAMESERVER_SEND_COSTUME:
            handleSendCostume(pak);
            return 1;
        xcase DBGAMESERVER_SEND_POWERSET_NAMES:
            handleSendPowersetNames(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_CHARCOUNT:
            handleSendCharacterCounts(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_CATALOG:
            handleSendAccountServerCatalog(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_CLIENTAUTH:
            receiveAccountServerClientAuth(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY:
            receiveAccountServerPlayNCAuthKey(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_NOTIFYREQUEST:
            receiveAccountServerNotifyRequest(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_INVENTORY:
            receiveAccountServerInventoryDB(pak);
            return 1;
        xcase DBGAMESERVER_ACCOUNTSERVER_UNAVAILABLE:
            receiveAccountServerUnavailable(pak);
            return 1;
        xcase DBGAMESERVER_MAP_CONNECT:
            dbOrMapConnect(pak);
            return 1;
        xcase DBGAMESERVER_CAN_START_STATIC_MAP_RESPONSE:
            handleCanStartStaticMapResponse(pak);
            return 1;
        xcase DBGAMESERVER_DELETE_OK:
            db_rename_status = pktGetBits(pak,1);
            return 1;
        xcase DBGAMESERVER_MSG:
            Strncpyt(db_info.error_msg,translateDbString(pktGetString(pak)));
            return -1;
        xcase DBGAMESERVER_CONPRINT:
            conPrintf("%s",pktGetString(pak));
            return -1;
        xcase DBGAMESERVER_RENAME_RESPONSE:
            receiveRenameResponse(pak);
            return 1;
        xcase DBGAMESERVER_CHECK_NAME_RESPONSE:
            receiveCheckNameResponse(pak);
            return 1;
        xcase DBGAMESERVER_UNLOCK_CHARACTER_RESPONSE:
            receiveUnlockCharacterResponse(pak);
            return 1;
        xcase DBGAMESERVER_LOGIN_DIALOG_RESPONSE:
            receiveLoginDialogResponse(pak);
            return 1;
        xdefault:
            sprintf(buf,"Unknown csserver command: %d\n",cmd);
            printf("%s\n", buf);
            return 0;
    }
    return 1;
}

int dbWaitForStartOrQueue(F32 timeout)
{
    int queuePosition;
    int        ret, timer = 0;
    dbclient_resetQueueStatus();
    if(dbhasError())
        dbGetError();    //clears the error string.

    timer = timerCpuTicks();
    while((dbclient_getQueueStatus(&queuePosition, 0) && queuePosition == -1) &&
        !dbhasError() &&    //we haven't received a new error.
        !(ret=netLinkMonitor(&db_comm_link,DBGAMESERVER_SEND_PLAYERS,dbHandleMessage))&& (timeout==NO_TIMEOUT || timerSeconds(timerCpuTicks() - timer) < timeout) )
    {
        netIdle(&db_comm_link,1,10);
    }

    if(ret < 0)
        ret = DBGAMESERVER_MSG;
    else if(ret)
        ret = DBGAMESERVER_SEND_PLAYERS;
    else if (queuePosition >= 0)
        ret = DBGAMESERVER_QUEUE_POSITION;
    else ret = 0;
    return ret;
}

static int dbWaitFor(int wait_for_cmd, F32 timeout)
{
    int        ret, timer = 0;

    timer = timerCpuTicks();
    while(!(ret=netLinkMonitor(&db_comm_link,wait_for_cmd,dbHandleMessage))&& (timeout==NO_TIMEOUT || timerSeconds(timerCpuTicks() - timer) < timeout) )
    {
        netIdle(&db_comm_link,1,10);
        FakeYieldToOS();
    }
    return ret >= 1;
}

void dbSendQuit()
{
    if (db_comm_link.connected)
    {
        Packet    *pak;

        pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_QUITCLIENT);
        pktSend(&pak,&db_comm_link);
        netSendDisconnect(&db_comm_link,1);
        lnkBatchSend(&db_comm_link);
    }
}

int dbCheckCanStartStaticMap(int createLocation, F32 timeout)
{
    Packet    *pak;
    int ret = 0;

    pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_CAN_START_STATIC_MAP);
    pktSendBitsAuto(pak, createLocation);
    pktSend(&pak,&db_comm_link);
    lnkBatchSend(&db_comm_link);
    testClientRandomDisconnect(TCS_dbCheckCanStartStaticMap_1);
    
    db_can_create_static_map_response = 0;

    if (dbWaitFor(DBGAMESERVER_CAN_START_STATIC_MAP_RESPONSE, timeout))
    {
        ret = db_can_create_static_map_response;
    }

    testClientRandomDisconnect(TCS_dbCheckCanStartStaticMap_2);

    return ret;
}

void dbMakePlayerOnline(int slot_number)
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_MAKE_PLAYER_ONLINE);
    pktSendBitsPack(pak,1,slot_number);
    pktSend(&pak,&db_comm_link);
    lnkBatchSend(&db_comm_link);
}

int dbChoosePlayer(int slot_number, U32 local_map_ip, int createLocation, F32 timeout)
{
    Packet    *pak;
    int ret;

    pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_CHOOSE_PLAYER);
    pktSendBitsPack(pak,1,slot_number);
    pktSendBitsPack(pak,1,local_map_ip);
    pktSendString(pak,db_info.players[slot_number].name);
    pktSendBitsAuto(pak,createLocation);
    pktSend(&pak,&db_comm_link);
    lnkBatchSend(&db_comm_link);
    testClientRandomDisconnect(TCS_dbChoosePlayer_1);

    if (!dbWaitFor(DBGAMESERVER_MAP_CONNECT, timeout)) {
        ret = 0;
    } else {
        if (db_info.mapserver.cookie==0)
        {
            Strncpyt(db_info.error_msg,"InvalidName");
            ret = 0;
        }
        else if (db_info.mapserver.cookie==1)
        {
            Strncpyt(db_info.error_msg,"S_DATABASE_FAIL");
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }
    testClientRandomDisconnect(TCS_dbChoosePlayer_2);
    if (db_comm_link.connected) {
        // Make sure to acknowledge the packet the DbServer just sent us!
        netSendIdle(&db_comm_link);
    }

    return ret;
}

int dbChooseVisitingPlayer(int dbid, F32 timeout)
{
    Packet    *pak;
    int ret;

    pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_CHOOSE_VISITING_PLAYER);
    pktSendBitsAuto(pak,dbid);
    pktSend(&pak,&db_comm_link);
    lnkBatchSend(&db_comm_link);

    if (!dbWaitFor(DBGAMESERVER_MAP_CONNECT, timeout))
    {
        ret = 0;
    }
    else
    {
        if (db_info.mapserver.cookie == 0)
        {
            Strncpyt(db_info.error_msg,"InvalidName");
            ret = 0;
        }
        else if (db_info.mapserver.cookie == 1)
        {
            Strncpyt(db_info.error_msg,"S_DATABASE_FAIL");
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }
    if (db_comm_link.connected)
    {
        // Make sure to acknowledge the packet the DbServer just sent us!
        netSendIdle(&db_comm_link);
    }

    return ret;
}

int dbGetPowersetNames(int slot_number)
{
    Packet *pak;

    if (db_info.players && stricmp(db_info.players[slot_number].name, "EMPTY")==0 )
        return 1;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_GET_POWERSET_NAMES); 
    pktSendBitsPack(pak, 1, slot_number);
    pktSend(&pak, &db_comm_link);

    
    if (!dbWaitFor(DBGAMESERVER_SEND_POWERSET_NAMES, 60))
        return 0;
    

    return 1;
}

int dbGetCostume(int slot_number)
{
    Packet *pak;

    if (db_info.players && stricmp(db_info.players[slot_number].name, "EMPTY")==0 )
        return 1;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_GET_COSTUME); 
    pktSendBitsPack(pak, 1, slot_number);
    pktSend(&pak, &db_comm_link);

    if (!dbWaitFor(DBGAMESERVER_SEND_COSTUME, 60))
        return 0;

    dbGetPowersetNames(slot_number);

    return 1;
}




int dbGetCharacterCountsAsync(void)
{
    int retval = 0;

    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet *pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_ACCOUNTSERVER_CHARCOUNT);
        pktSend(&pak, &db_comm_link);
        retval = 1;
    }

    return retval;
}
int dbGetAccountServerCatalogAsync(void)
{
    int retval = 0;

    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet *pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_ACCOUNTSERVER_CATALOG);
        pktSend(&pak, &db_comm_link);
        retval = 1;
    }

    return retval;
}

int dbLoyaltyRewardBuyItem(char *nodeName)
{
    int retval = 0;
    
    LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(nodeName);

    // check to make sure this is a valid node to buy
    if (!node || !accountLoyaltyRewardCanBuyNode(getCurrrentPlayerInv(), db_info.account_inventory.accountStatusFlags, db_info.loyaltyStatus, db_info.loyaltyPointsUnspent, node))
        return 0;

    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet *pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_ACCOUNTSERVER_LOYALTY_BUY);
        pktSendString(pak, g_achAccountName);
        pktSendBitsAuto(pak, node->index);
        pktSend(&pak, &db_comm_link);
        retval = 1;
    }
    return retval;
}

int dbLoyaltyRewardRefundItem(char *nodeName)
{

    int retval = 0;

    LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(nodeName);

    // check to make sure this is a valid node to refund
    if (!node || !accountLoyaltyRewardHasNode(db_info.loyaltyStatus, nodeName))
        return 0;

    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet *pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_ACCOUNTSERVER_LOYALTY_REFUND);
        pktSendString(pak, g_achAccountName);
        pktSendBitsAuto(pak, node->index);
        pktSend(&pak, &db_comm_link);
        retval = 1;
    }
    return retval;
}

int dbRedeemShardXferToken(U32 auth_id, char *name, U32 slot_idx, char *destshard)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_SHARD_XFER_TOKEN_REDEEM);
    pktSendBitsAuto(pak, auth_id);
    pktSendString(pak, escapeString(name));
    pktSendBitsAuto(pak, slot_idx);
    pktSendString(pak, destshard);
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbRenameCharacter(char *old_name, char *new_name)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_RENAME_CHARACTER);
    pktSendString(pak, escapeString(old_name));
    pktSendString(pak, new_name);
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbRedeemRenameToken(U32 auth_id, char *name, U32 slot_idx)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_RENAME_TOKEN_REDEEM);
    pktSendBitsAuto(pak, auth_id);
    pktSendString(pak, escapeString(name));
    pktSendBitsAuto(pak, slot_idx);
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbCheckName(char *name, int fromLoginScreen, int tempLockName)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_CHECK_NAME);
    pktSendString(pak, escapeString(name));
    pktSendBitsAuto(pak, tempLockName);
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbRedeemSlot(void)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_SLOT_TOKEN_REDEEM);
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbUnlockCharacter(char *name)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_UNLOCK_CHARACTER);
    pktSendString(pak, escapeString(name));
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbSignNDA(U32 NDAVersion, U32 versionSubtype)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_SIGN_NDA);
    pktSendBitsAuto(pak, versionSubtype);                        // subtype
    pktSendBitsAuto(pak, NDAVersion);                            // newValue
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbGetPlayNCAuthKey( int request_key, char* auth_name,char* digest_data)
{
    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet* pak;

        pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_GET_PLAYNC_AUTH_KEY);
        pktSendBitsAuto(pak, request_key);
        pktSendString(pak, auth_name);
        pktSendString(pak, digest_data);
        pktSend(&pak, &db_comm_link);
        return 1;
    }
    return 0;
}

int dbResendPlayers(void)
{
    Packet* pak;

    pak = pktCreateEx(&db_comm_link, DBGAMECLIENT_RESEND_PLAYERS);
    pktSend(&pak, &db_comm_link);

    return 0;
}

int dbConnect(char *server,int port,int user_id,int cookie,char *auth_name,int no_version_check,F32 timeout,int no_timeout)
{
    Packet    *pak;
    int        ret;
    static U32 checksum[4] = {0, 0, 0, 0};
    int commandFound;

    PERFINFO_AUTO_START("dbConnect", 1);

        writeConsole(OUTPUT_INFO, "Connecting to DBServer %s:%d (UDP) Cookie: %x", server, port, cookie);
        Strncpyt(db_info.address, makeIpStr(ipFromString(server)));
        if (db_comm_link.socket)
            netSendDisconnect(&db_comm_link,1);
        ret = netConnectEx(&db_comm_link,server,port,NLT_UDP,timeout,NULL,1);
        if (!ret)
        {
            Strncpyt(db_info.error_msg,"CantConnectDbServer");
            writeConsole(OUTPUT_ERROR, "Failed to connect to DBServer");
            PERFINFO_AUTO_STOP();
            return 0;
        }
        db_comm_link.alwaysProcessPacket = 1;
        testClientRandomDisconnect(TCS_dbConnect_1);

        {
            char *exename = getExecutableName();  // This is a static, we can't safely overwrite it
            char exenamecopy[MAX_PATH];
            Strncpyt(exenamecopy,exename);
            exename = exenamecopy;
            #ifdef TEST_CLIENT
                // Have TestClient checksum CityOfHeroes.exe
                if (strEndsWith(exenamecopy, "TestClient.exe")) {
                    strcpy(strstri(exenamecopy, "TestClient.exe"), "Ouroboros.exe");
                }
            #endif
            #ifdef TEXT_CLIENT
                // Have TextClient checksum CityOfHeroes.exe
                if (strEndsWith(exenamecopy, "TextClient.exe")) {
                    strcpy(strstri(exenamecopy, "TextClient.exe"), "Ouroboros.exe");
                }
            #endif
            // Hack for running development build executables
            //exename = "C:\\program files\\city of heroes\\cityofheroes.exe";
            if (!checksum[0]) // keep the checksum around, only calculate the first time
                checksumFile(exename,checksum);
            
            #ifdef TEST_CLIENT
            printf("\nCHECKSUM (%s):\n  %x %x %x %x\n",exenamecopy,checksum[0],checksum[1],checksum[2],checksum[3]);
            #endif
        }

        pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_LOGIN);
        pktSendString(pak,auth_name);
        pktSendBitsPack(pak,1,user_id);
        pktSendBitsPack(pak,1,DBSERVER_CLIENT_PROTOCOL_VERSION);
        pktSendBitsArray(pak,8*sizeof(db_info.auth_user_data),db_info.auth_user_data);
        pktSendBitsPack(pak,1,no_version_check||game_state.local_map_server);
        pktSendString(pak,getExecutablePatchVersion(NULL));
        pktSendBitsArray(pak,8*sizeof(checksum),checksum);
        pktSendBits(pak,32,cookie);
        pktSendBits(pak,1,no_timeout);
        // Check for registry value saying the Updater has something to say
        pktSendString(pak, regGetPatchValue());
        {
#if defined(DISABLE_RENDER_TELEMETRY)
            char *systemSpecString = "Disabled";
#else
            char *systemSpecString = rdrGetSystemSpecCSVString();
#endif
            pktSendString(pak, "");
            pktSendZipped(pak, strlen(systemSpecString)+1, systemSpecString);
        }
        pktSendBitsPack(pak, 1, encryptedKeyedAccessLevel());
        if (encryptedKeyedAccessLevel()) 
        {
            extern char *encrypedKeyIssuedTo;
            pktSendString(pak, encrypedKeyIssuedTo);
        }
        pktSend(&pak,&db_comm_link);
        testClientRandomDisconnect(TCS_dbConnect_2);

        if (!(commandFound=dbWaitForStartOrQueue(timeout)))
        {
            writeConsole(OUTPUT_ERROR, "Failed to connect to DBServer");
            PERFINFO_AUTO_STOP();
            return 0;
        }

        testClientRandomDisconnect(TCS_dbConnect_3);

    PERFINFO_AUTO_STOP();

    return commandFound;
}

int dbDeletePlayer(int player_slot, F32 timeout)
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBGAMECLIENT_DELETE_PLAYER);
    pktSendBitsPack(pak,1,player_slot);
    pktSendString(pak,db_info.players[player_slot].name);
    pktSend(&pak,&db_comm_link);
    if (!dbWaitFor(DBGAMESERVER_DELETE_OK, timeout))
        return 0;
    return db_rename_status;
}

void dbDisconnect()
{
    netSendDisconnect(&db_comm_link,5.f);
    testClientRandomDisconnect(TCS_dbDisconnect);
    freePlayers();
}

char *dbGetError()
{
    Strncpyt(db_err_msg,db_info.error_msg);
    db_info.error_msg[0] = 0;

    if( db_err_msg[0] == '\0' )
    {
        dbDisconnect();
        Strncpyt( db_err_msg, "LostConnToDB");
    }
    return db_err_msg;
}

int dbhasError()
{
    return !!db_info.error_msg[0];
}

void dbPingIfConnected()
{
    static int timer;
    Packet    *pak;

    if (!db_comm_link.connected)
        return;
    if (!timer)
        timer = timerAlloc();

    // Read packets from the dbserver.
    //    This used to only get idles.
    netLinkMonitor(&db_comm_link, 0, dbHandleMessage);

    if (timerElapsed(timer) < 2)
        return;

    pak = pktCreateControlCmd(&db_comm_link,COMMCONTROL_IDLE);
    pktSend(&pak,&db_comm_link);
    lnkBatchSend(&db_comm_link);
    timerStart(timer);
}

int waitForRedeemSlot(F32 timeout)
{
    return dbWaitFor(DBGAMESERVER_ACCOUNTSERVER_NOTIFYREQUEST, timeout);
}

int AccountIsReallyVIPAfterServerSelected(void)
{
    // AccountIsVIP only checks the AccountServer. If the AccountServer is down, we can get the info from the DbServer.
    // In theory, we'd only need the info from the DbServer, except we might force the VIP debug flag on, in which case we'd still want to use it.
    return db_info.vip || AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags());
}

int getVIPAdjustedBaseSlots(void)
{
    return AccountIsReallyVIPAfterServerSelected() ? db_info.base_slots : 0;
}

int getTotalServerSlots(void)
{
    return db_info.bonus_slots + getVIPAdjustedBaseSlots();
}

int getSlotUnlockedPlayerCount()
{
    int display_slots = db_info.base_slots + MAX_SLOTS_PER_SHARD;
    int unlockedCount = db_info.player_count;
    int i;

    for (i = 0; i < display_slots; ++i)
    {
        if (db_info.players[i].slot_lock != SLOT_LOCK_NONE)
        {
            --unlockedCount;
        }
    }

    devassert(unlockedCount >= 0);
    return unlockedCount;
}

int AvailableUnallocatedCharSlots(void)
{
    return AccountGetStoreProductCount(inventoryClient_GetAcctInventorySet(), SKU("svcslots"), true);
}

int CanAllocateCharSlots()
{
    return AvailableUnallocatedCharSlots() > 0 && db_info.bonus_slots < MAX_SLOTS_PER_SHARD;
}