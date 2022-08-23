#include "clientcomm/clientcomm.h"
#include <stdio.h>
#include <float.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_socket.h>
#include <utilitieslib/utils/error.h>
#include "comm_game.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/net_link.h>

#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdcontrols.h"
#include "cmdparse/cmdgame.h"
#include "cmdparse/cmdDebug.h"

#include "graphics/font.h"
#include <utilitieslib/utils/memcheck.h>
#include "entity/entrecv.h"
#include "group/groupnetrecv.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "win/win_init.h"
//#include "../connserver/account.h"
#include "clientcomm/clientcommreceive.h"
#include <utilitieslib/language/MessageStore.h>
#include "entity/entclient.h"
#include "player/playerState.h"
#include "edit/edit_net.h"
#include "entity/entVarUpdate.h"
#include "entity/entDebug.h"
#include "utils/position.h"
#include <utilitieslib/assert/assert.h>
#include "clientcomm/dbclient.h"
#include "sound/sound.h"
#include "storyarc/contactClient.h"
#include "storyarc/storyarcClient.h"
#include "storyarc/missionClient.h"
#include "entity/scriptDebugClient.h"

#include "testClient/TestClient.h"
#include "entity/entity_power_client.h"
#include "clientError.h"
#include "gameComm/dooranimclient.h"
#include "demo.h"
#include "player/player.h"
#include "render/texUnload.h"
#include "render/tex.h"

#include "UI/uiWindows.h"
#include "UI/uiConsole.h"
#include "UI/uiNet.h"
#include "UI/uiTarget.h"
#include "UI/uiCursor.h"
#include "UI/uiLogin.h"
#include "UI/uiQuit.h"
#include "UI/uiDialog.h"
#include "UI/uiBrowser.h"
#include "UI/uiComment.h"
#include "UI/uiCostumeSelect.h"
#include "gameComm/initClient.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiMapSelect.h"
#include "UI/uiReSpec.h"
#include "gameComm/chatClient.h"
#include "UI/uiChat.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiSGList.h"
#include "UI/uiSGRaidList.h"
#include "UI/uiRaidResult.h"

#include "edit/edit_net.h"
#include "arena/arenagame.h"
#include "gameData/arenastruct.h"
#include "gameData/sgraidClient.h"

#include "clientcomm/clientcomm.h"
#include "UI/uiTabControl.h"
#include "clientcomm/authclient.h"

#include "seq/anim.h"
#include "entity/entity.h"

#include "player/pmotion.h"
#include <utilitieslib/utils/file.h>

#include "edit/MapGenerator.h"
#include "MissionControl.h"
#include "edit/DebugLocation.h"
#include "graphics/sun.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiAuction.h"
#include "UI/uiMissionReview.h"
#include "UI/uiAutomap.h"
#include "UI/uiScript.h"
#include "player/badges.h"
#include "player/badges_client.h"
#include "UI/uiTurnstile.h"
#include "entity/entPlayer.h"
#include "graphics/gfxLoadScreens.h"
#include "UI/uiGame.h"
#include "UI/uiConvertEnhancement.h"
#include "player/playerSticky.h"

#include <utilitieslib/components/EString.h>

#define DEFAULT_TIMEOUT 60 // Default timeout to use on each of the phases of connecting

NetLink    comm_link;
Packet*    client_input_pak;
MessageStore* AuthServerMsgs;
int g_showMapLoadingMessage = false;

int gPrintShardCommCmds = false;

NetStat recv_hist[RECV_HISTORY_SIZE];
int recv_idx = 0;
static int lastSibPartsCount = 0;
static int lastTotalBytesRead = 0;
static U32 lastRecvId = 0;
static int lastLostPacketSent = 0;
static int logPackets = 0;

#define LOG_TROUBLED_MODE 0

void commInit()
{
    MessageStoreLoadFlags flags = MSLOAD_DEFAULT;

    char *files[] = {
        "texts\\English\\AuthServerMsgs.txt", NULL,
        NULL
    };

    if(game_state.create_bins)
        flags |= MSLOAD_FORCEBINS;

    LoadMessageStore(&AuthServerMsgs,files,NULL,0,NULL,"authmsg",NULL,flags); // apparently these are English only
}

void commSendPkt(Packet *pak)
{
    pktSend(&pak,&comm_link);
}

U32 commID()
{
//    return comm_link.id;
    return 0;
}

U32 commAck()
{
//    return comm_link.id_ack;
    return 0;
}

void commResetControlState()
{
    // Turn off the slave control when connecting to a server.

    setMySlave(NULL, NULL, 0);
    setPlayerIsSlave(0);

    playerResetControlState();    
    
    cmdOldSetSends(control_cmds,1);
}

int commConnected()
{
    return comm_link.socket > 0;
}

void commSendQuitGame(int abort_quit)
{
    //For the editor : if you haven't hit save lately. Just pop a warning
    //if( abort_quit == 0 && fileIsDevelopmentMode() )
    //{
    //    abort_quit = groupPromptForMapSave();
    //}
    if (game_state.cryptic)
        saveOnExit();

     if (commCheckConnection() == CS_CONNECTED)
    {
        Packet    *pak;
        pak = pktCreate();
        pktSendBitsPack(pak,1,CLIENT_DISCONNECT);
        pktSendBitsPack(pak,1,abort_quit);    // 0 means do it, 1 means abort from disconnect
        pktSendBitsPack(pak,2,SAFE_MEMBER(playerPtr(),logout_login));
        pktSend(&pak,&comm_link);
        lnkBatchSend(&comm_link);
    }
    else
    {
        if( !playerPtr() || !playerPtr()->logout_login )
            windowExit(0);
        else // if 1 or 2, quit to login... if I don't have a connection, I can't quit to character select
            quitToLogin(0);
    }

    if( playerPtr() && !playerPtr()->logout_login )
        dialogStd( DIALOG_SINGLE_RESPONSE, "QuitNow", "QuitNowButton", NULL, windowExitDlg, NULL, 0 );
}

int commDisconnect()
{

    if (comm_link.socket <= 0)
        return 0;
    netSendDisconnect(&comm_link,2.f);
    lastTotalBytesRead = 0;
    commResetControlState();
    return 1;
}

void commAddInput(char *s)
{
    START_INPUT_PACKET(pak, CLIENTINP_SVRCMD);
    pktSendString(pak, s);
    END_INPUT_PACKET
}

#include "graphics/camera.h"

void commFreeInputPak()
{
    pktFree(client_input_pak);
    client_input_pak = NULL;
}

void commNewInputPak()
{
    commFreeInputPak();
    client_input_pak = pktCreate();
}

static struct {
    U32                    net_packet_order;
    ControlStateChange* last;
    U32                    new_last_send_ms;
    U32                    last_time_stamp_ms;
    S32                    delta_bits;
    S32                    sent_run_physics;
    U32                    last_client_abs;
    U32                    last_client_abs_slow;
} csc_send;

static void sendControlStateChangeData(Packet* pak, ControlStateChange* csc)
{
    if(csc->control_id < CONTROLID_BINARY_MAX)
    {
        pktSendBits(pak, 1, csc->state);

        if(csc->state)
            control_state.dir_key.send |= (1 << csc->control_id);
        else
            control_state.dir_key.send &= ~(1 << csc->control_id);
    }
    else if(csc->control_id < CONTROLID_ANGLE_MAX)
    {
        pktSendBits(pak, CSC_ANGLE_BITS, csc->angleU32);
        
        control_state.pyr.send[csc->control_id - CONTROLID_PITCH] = csc->angleU32;
    }
    else if(csc->control_id == CONTROLID_RUN_PHYSICS)
    {
        pktSendBits(pak, 1, csc->controls_input_ignored ? 1 : 0);

        if(csc_send.sent_run_physics)
        {
            U32 diff;

            diff = csc->client_abs - csc_send.last_client_abs;
            pktSendBitsPack(pak, 8, diff);

            diff = csc->client_abs_slow - csc_send.last_client_abs_slow;
            pktSendBitsPack(pak, 8, diff);
        }
        else
        {
            csc_send.sent_run_physics = 1;

            pktSendBits(pak, 32, csc->client_abs);
            pktSendBitsPack(pak, 10, csc->client_abs - csc->client_abs_slow);
        }

        csc_send.last_client_abs = csc->client_abs;
        csc_send.last_client_abs_slow = csc->client_abs_slow;

        if(csc->inp_vel_scale_U8 < 255)
        {
            pktSendBits(pak, 1, 1);
            pktSendBits(pak, 8, csc->inp_vel_scale_U8);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
    }
    else if(csc->control_id == CONTROLID_APPLY_SERVER_STATE)
    {
        pktSendBits(pak, 8, csc->applied_server_state_id);
    }
    else if(csc->control_id == CONTROLID_NOCOLL)
    {
        pktSendBits(pak, 2, csc->state2);
    }
    else
    {
        assert(0);
    }
}

void sendControlStateChangeList(Packet* pak, ControlStateChange* csc)
{
    // If this was previously sent, use the previous last_send_ms time, otherwise use the current last_send_ms.
    
    U32 last_send_ms = csc->sent ? csc->last_send_ms : control_state.time.last_send_ms;
    
    while(csc)
    {
        S32 diff_ms;

        if(csc_send.last)
        {
            // Only send the "1" bit if this isn't the first CSC.

            pktSendBits(pak, 1, 1);
        }

        if(csc->control_id == CONTROLID_RUN_PHYSICS)
        {
            pktSendBits(pak, 1, 1);
        }
        else
        {
            pktSendBits(pak, 1, 0);
            pktSendBits(pak, 4, csc->control_id);
        }

        csc_send.new_last_send_ms = csc->time_stamp_ms;

        //assert(abs(new_last_send_ms - curTime) < 2000);

        if(!csc_send.last)
        {
            csc_send.last_time_stamp_ms = last_send_ms;

            //if(csc->control_id < CONTROLID_BINARY_MAX)
            //    printf("sent %dms change\n", csc->time_stamp - control_state.time.last_send_ms);
        }

        diff_ms = csc->time_stamp_ms - csc_send.last_time_stamp_ms;

        if(diff_ms < 32 || diff_ms > 32 + 3)
        {
            diff_ms = MINMAX(diff_ms, 0, 1000);

            pktSendBits(pak, 1, 0);
            pktSendBits(pak, csc_send.delta_bits, diff_ms);
        }
        else
        {
            pktSendBits(pak, 1, 1);
            pktSendBits(pak, 2, diff_ms - 32);
        }

        // Send the CSC data.
        
        sendControlStateChangeData(pak, csc);

        csc_send.last_time_stamp_ms = csc->time_stamp_ms;
        csc_send.last = csc;

        if(!csc->sent)
        {
            csc->sent = 1;
            csc->net_id = control_state.net_id.cur++;
            csc->net_packet_order = csc_send.net_packet_order;
            csc->last_send_ms = control_state.time.last_send_ms;

            #if !TEST_CLIENT
                pmotionUpdateNetID(csc);
            #endif
        }

        csc = csc->next;
    }
    
    #if !TEST_CLIENT
        pmotionUpdateNetID(NULL);
    #endif
}

static void commSendControlUpdate(Packet* pak, int sendUpdate)
{
    static U32 lastSendCheckTime = 0;

    const U32 curTime = timeGetTime();

    ControlStateChange* csc_send_head = NULL;
    ControlStateChange* csc;
    S32 i;

    csc_send.new_last_send_ms = control_state.time.last_send_ms;

    if(!control_state.first_packet_sent)
    {
        control_state.time.last_send_ms = curTime;
        control_state.first_packet_sent = 1;
        sendUpdate = 0;
    }

    if(!sendUpdate)
    {
        pktSendBits(pak, 1, 0);
        return;
    }

    pktSendBits(pak, 1, 1);

    // Increment the net packet order.
    
    csc_send.net_packet_order++;
    
    // Find the starting CSC from x packets ago.
    
    control_state.net_error_correction = MINMAX(control_state.net_error_correction, 0, 2);
    
    for(csc = control_state.csc_processed_list.head; csc && csc->sent; csc = csc->next)
    {
        U32 diff = csc_send.net_packet_order - csc->net_packet_order;

        if(diff <= (U32)(control_state.net_error_correction + 1))
        {
            break;
        }
    }
    
    csc_send_head = csc;
    
    // Send the control state change list.

    if(csc_send_head)
    {
        ControlStateChange* last;
        S32 max_time_diff_ms = 0;

        // Send the first "1" bit to indicate that some control stuff is coming up.

        pktSendBits(pak, 1, 1);
        
        // Calculate the maximum time delta.
        
        for(csc = csc_send_head, last = NULL; csc; last = csc, csc = csc->next)
        {
            S32 diff_ms = csc->time_stamp_ms - (last ? last->time_stamp_ms : csc->sent ? csc->last_send_ms : control_state.time.last_send_ms);
            
            //assert(diff >= 0 && diff <= 1000);

            if(diff_ms < 32 || diff_ms > 32 + 3)
            {
                diff_ms = MINMAX(diff_ms, 0, 1000);
                
                if(diff_ms > max_time_diff_ms)
                {
                    max_time_diff_ms = diff_ms;
                }
            }
        }

        // Get the number of bits in max_time_diff_ms.

        assert(max_time_diff_ms >= 0);

        //printf("max_time_diff_ms: %d", max_time_diff_ms);

        for(csc_send.delta_bits = 1, max_time_diff_ms >>= 1; max_time_diff_ms; csc_send.delta_bits++){
            max_time_diff_ms >>= 1;
        }

        assert(csc_send.delta_bits <= 10);

        //printf("\tdelta bits: %d\n", delta_bits);

        pktSendBits(pak, 5, csc_send.delta_bits - 1);

        // Send the physics position id.

        pktSendBits(pak, 16, csc_send_head->sent ? csc_send_head->net_id : control_state.net_id.cur);

        // Send the control state changes.

        csc_send.last = NULL;
        csc_send.sent_run_physics = 0;

        //assert(abs(new_last_send_ms - curTime) < 2000);

        sendControlStateChangeList(pak, csc_send_head);

        #ifdef TEST_CLIENT
            while(control_state.csc_processed_list.head != csc_send_head)
            {
                destroyControlStateChangeListHead(&control_state.csc_processed_list);
            }
        #endif
    }
    
    // End of control-list bit.

    pktSendBits(pak, 1, 0);

    // Store the last send time.

    control_state.time.last_send_ms = csc_send.new_last_send_ms;

    if(abs(control_state.time.last_send_ms - timeGetTime()) > 1000)
    {
        control_state.time.last_send_ms = timeGetTime();
    }

    // Send the current binary control states.

    for(i = 0; i < CONTROLID_BINARY_MAX; i++)
    {
        pktSendBits(pak, 1, control_state.dir_key.down_now & (1 << i) ? 1 : 0);
    }

    // Send the current angles.

    #if TEST_CLIENT
        lastSendCheckTime = curTime + 1000;
    #endif

    if(abs(lastSendCheckTime - curTime) >= 1000)
    {
        lastSendCheckTime = curTime;

        pktSendBits(pak, 1, 1);
        pktSendBits(pak, CSC_ANGLE_BITS, control_state.pyr.prev[0]);
        pktSendBits(pak, CSC_ANGLE_BITS, control_state.pyr.prev[1]);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    #if TEST_CLIENT
        for(i = 0; i < 3; i++)
        {
            control_state.pyr.prev[i] = control_state.pyr.send[i] = CSC_ANGLE_F32_TO_U32(control_state.pyr.cur[i]);
        }
    #else
        copyVec3(control_state.pyr.prev, control_state.pyr.send);
    #endif
}

static void sendBits64(Packet* pak, U64 value64)
{
    U32* value32 = (U32*)&value64;
    U64 value = value64;
    int byte_count;

    for(byte_count = 1; value > 255 && byte_count < 9; value >>= 8, byte_count++);

    assert(byte_count <= 8);

    pktSendBits(pak, 3, byte_count - 1);

    if(byte_count > 4)
    {
        pktSendBits(pak, 32, *value32);
        value32++;
        byte_count -= 4;
    }

    pktSendBits(pak, byte_count * 8, *value32);
}

void commSendInput()
{
    static int         timer_id = -1;
    static F32         leftover_time;

    Packet*            pak;
    char            buf[100];
    int                reliable = 0;
    extern int        glob_client_pos_id,glob_plrstate_valid;
    F32                dt;
    Entity*         player = controlledPlayerPtr();
    int                i;

    #define MINSENDTIME (3.0f/30.f)

    if (timer_id < 0)
    {
        timer_id = timerAlloc();
        timerStart(timer_id);
    }

    // See if it's time to send a control update.

    dt = timerElapsed(timer_id) + leftover_time;

    if (dt < MINSENDTIME)
        return;

    dt = timerElapsedAndStart(timer_id) + leftover_time;

    leftover_time = fmod(dt, MINSENDTIME);

    if (control_state.client_pos_id != glob_client_pos_id && !glob_plrstate_valid)
    {
        sprintf(buf,"client_pos_id %d",glob_client_pos_id);
        cmdParse(buf);
    }

    // Send the client input packet.

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_INPUT);
    
    START_BIT_COUNT(pak, "CLIENT_INPUT");

    // Send control update.

    //printf("before:\t%d, %d\n", pak->stream.cursor.byte, pak->stream.cursor.bit);

    PERFINFO_AUTO_START("sendControlUpdate", 1);
        START_BIT_COUNT(pak, "sendControlUpdate");
            #if TEST_CLIENT
                commSendControlUpdate(pak, 1);
            #else
                commSendControlUpdate(pak, game_state.game_mode != SHOW_LOAD_SCREEN);
            #endif
        STOP_BIT_COUNT(pak);
    PERFINFO_AUTO_STOP();
    //printf("after:\t%d, %d\n", pak->stream.cursor.byte, pak->stream.cursor.bit);

    // Send selected entity.
    // Why isn't this sent with the other commands in cmdOldSetSends() ?
    pktSendBits(pak, 1, current_target ? 1 : 0 );
    pktSendBitsPack(pak, 14, current_target ? current_target->svr_idx : gSelectedDBID);

    // Send control log.

    for(i = 0; i < control_state.controlLog.cur; i++)
    {
        ControlLogEntry* entry = control_state.controlLog.entries + i;

        pktSendBits(pak, 1, 1);

        if(!i)
        {
            pktSendBits(pak, 32, entry->actualTime);
            pktSendBits(pak, 32, entry->mmTime);
            pktSendF32(pak, entry->timestep);
            if(entry->timestep == entry->timestep_noscale){
                pktSendBits(pak, 1, 0);
            }else{
                pktSendBits(pak, 1, 1);
                pktSendF32(pak, entry->timestep_noscale);
            }
            sendBits64(pak, entry->qpc);
            sendBits64(pak, entry->qpcFreq);
        }
        else
        {
            ControlLogEntry* lastEntry = control_state.controlLog.entries + i - 1;

            pktSendBitsPack(pak, 1, entry->actualTime - lastEntry->actualTime);
            pktSendBitsPack(pak, 1, entry->mmTime - lastEntry->mmTime);
            pktSendF32(pak, entry->timestep);
            if(entry->timestep == entry->timestep_noscale){
                pktSendBits(pak, 1, 0);
            }else{
                pktSendBits(pak, 1, 1);
                pktSendF32(pak, entry->timestep_noscale);
            }
            sendBits64(pak, entry->qpc - lastEntry->qpc);
            if(entry->qpcFreq == lastEntry->qpcFreq){
                pktSendBits(pak, 1, 0);
            }else{
                pktSendBits(pak, 1, 1);
                sendBits64(pak, entry->qpcFreq - lastEntry->qpcFreq);
            }
        }
    }

    control_state.controlLog.cur = 0;

    pktSendBits(pak, 1, 0);

    // Send control commands.

    START_BIT_COUNT(pak, "cmdSend");
        reliable = cmdOldSend(pak,control_cmds,0,0);
    STOP_BIT_COUNT(pak);    
    cmdOldSetSends(control_cmds,0);
    
    // Send hacker detection *DO NOT USE START_INPUT_PACKET*
    if (comm_link.nextID+1 == 37) {
        pktSendBitsPack(client_input_pak, 1, CLIENTINP_NOTAHACKER);
        pktSendBitsPack(client_input_pak, 1, comm_link.nextID+1);
    }

    START_BIT_COUNT(pak, "send:client_input_pak");
        //pktAppend(pak, client_input_pak); // Can't use this because it re-aligns the bytes boundary, messing up any internally sent pktSendBitsArrays
        pktSendBitsArray(pak,bsGetBitLength(&client_input_pak->stream),client_input_pak->stream.data);
    STOP_BIT_COUNT(pak);

    //printf("after:\t%d, %d\n", pak->stream.cursor.byte, pak->stream.cursor.bit);

    // Assume any data not related to movement needs reliable

    if (client_input_pak->stream.cursor.byte + client_input_pak->stream.cursor.bit > 0)
        reliable = 1;
    pak->reliable = reliable;

    //printf("sent:\t%d, %d\n", pak->stream.cursor.byte, pak->stream.cursor.bit);

    //printf("end:    %d, %d\n\n", pak->stream.cursor.byte, pak->stream.cursor.bit);
    
    STOP_BIT_COUNT(pak); // Matches START_BIT_COUNT right after sending CLIENT_INPUT.
    
    PERFINFO_AUTO_START("pktSend", 1);
        pktSend(&pak,&comm_link);
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("simulateNetConditions", 1);
        lnkSimulateNetworkConditions(    &comm_link,
                                        control_state.snc_client.lag,
                                        control_state.snc_client.lag_vary,
                                        control_state.snc_client.pl,
                                        control_state.snc_client.oo_packets);
    PERFINFO_AUTO_STOP();

    commNewInputPak();
}


void commGetStats(NetLink *link, Packet* pak)
{
    // Get the index to record the new stats.
    if (++recv_idx >= RECV_HISTORY_SIZE)
        recv_idx = 0;

    // Are there any missing incoming packets?
    //    If ID being processed does not follow the last seen ID, some incoming packet has been dropped.
    //    Denote each dropped packet in the receive history array as an entry with 0 received bytes.
    {
        unsigned int i, j, expectedPakID;

        expectedPakID = lastRecvId + lastSibPartsCount + 1;
        if(pak->id > expectedPakID)
        {
            // add missed packets to recv_hist
            for(i = expectedPakID; i < pak->id; i++)
            {
                recv_hist[recv_idx].type = NST_LostIncomingPacket;
                recv_hist[recv_idx].bytes = 0;
                recv_hist[recv_idx].time = 1000000;
                if (++recv_idx >= RECV_HISTORY_SIZE)
                    recv_idx = 0;
            }

            if( (pak->id - expectedPakID) > 1)
            {
                // lost multiple packets this frame, go into troubled mode
                commSetTroubledMode(TroubledComm_DroppedPackets);
            }
            else 
            {
                // Check recent received packet history to see if we are over threshold we consider for TroubledMode.
                //    For our purposes, we look to see if there is a second lost packet within a fixed packet count.
                #define MISSED_PACKET_THRESHOLD  60
                for(i=0, j=recv_idx + RECV_HISTORY_SIZE - 2; i<MISSED_PACKET_THRESHOLD; i++,j--)
                {
                    int prevIndex = j & (RECV_HISTORY_SIZE-1);
                    if( recv_hist[prevIndex].type == NST_LostIncomingPacket)
                    {
                        commSetTroubledMode(TroubledComm_DroppedPackets);
                        break;
                    }
                }        
            }
        }

        if (pak->id > lastRecvId) {
            lastRecvId = pak->id;
            lastSibPartsCount = pak->sib_count;
        }
    }

    // Are there any missing outgoing packets?
    {
        int i;
        for(i = lastLostPacketSent; i < link->lost_packet_sent_count; i++)
        {
            recv_hist[recv_idx].type = NST_LostOutgoingPacket;
            recv_hist[recv_idx].time = 1000000;
            recv_hist[recv_idx].bytes = 0;
            if (++recv_idx >= RECV_HISTORY_SIZE)
                recv_idx = 0;
        }
    }
    lastLostPacketSent = link->lost_packet_sent_count;

    recv_hist[recv_idx].type = NST_ReceivedUnreliable;
    recv_hist[recv_idx].bytes = link->totalBytesRead - lastTotalBytesRead;
    recv_hist[recv_idx].time = ((float)pingInstantRate(&link->pingHistory))/1000;
    lastTotalBytesRead = link->totalBytesRead;
}

void commServerWaitOK(char* mapname)
{
    Packet    *pak;
#ifndef TEST_CLIENT
    int index;
#endif

    // show load screen immediately
    g_showMapLoadingMessage = false;
    game_state.game_mode = SHOW_LOAD_SCREEN;
#ifndef TEST_CLIENT
    forceLoginMenu();
#endif
    loadScreenResetBytesLoaded();

#ifndef TEST_CLIENT
    for (index = eaSize(&g_comicLoadScreen_pageNames) - 1; index >= 0; index--)
    {
        free(g_comicLoadScreen_pageNames[index]);
    }
    eaDestroy(&g_comicLoadScreen_pageNames);
    g_comicLoadScreen_pageIndex = 0;
#endif

    gfxSetBGMapname(NULL);

      demoStop(); // we don't record demos across multiple maps
    respec_ClearData(0); // clear any respec info if we move maps

    window_closeAlways();

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_MAP_XFER_WAITING);
    pktSend(&pak,&comm_link);
}

void commServerMapXferLoading(Packet *pak_in)
{
    g_showMapLoadingMessage = pktGetBits(pak_in, 1);
}

int do_map_xfer;

void commServerMapXfer(Packet *pak_in)
{
    Packet    *pak;

    dbOrMapConnect(pak_in);
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_MAP_XFER_ACK);
    pktSend(&pak,&comm_link);
    lnkBatchSend(&comm_link);
    netSendDisconnect(&comm_link,5.f);
    lastTotalBytesRead = 0;

    {
        extern int glob_client_pos_id;

        glob_client_pos_id = 0;
        control_state.client_pos_id = 0;
    }
    do_map_xfer = 1;
}


void handleForcedLogout(Packet *pak)
{
    char    *reason;

    reason = pktGetString(pak);

    commDisconnect();
#if !defined(TEST_CLIENT)

    winMsgAlert( textStd("ForciblyDisconnected", reason) );
    windowExit(0);
#else
    printf( textStd("ForciblyDisconnected", reason) );
#endif
}

void handleEnableControlLog(Packet* pak)
{
    control_state.controlLog.duration = pktGetBitsPack(pak, 1);

    control_state.controlLog.start_time = time(NULL);

    control_state.controlLog.enabled = 1;
}

Vec3 curr_file_pyr,old_file_pyr,file_pyrvel,smooth_file_pyr;
Vec3 curr_file_xyz,old_file_xyz,file_xyzvel,smooth_file_xyz;

#define _Ch(cmd,func) xcase cmd: func(pak);

#ifndef TEST_CLIENT
    //#define SHOW_ALL_SERVER_CMDS
#endif

// this creates getServerCmdName
#define SERVERCMDNAME_FUNCTION
#include "comm_game.h"
#undef SERVERCMDNAME_FUNCTION

static int parseFmt(const char *fmt,char *str)
{
    const char    *sc;
    int            len=0,count=0;

    for(sc=fmt;*sc;sc++)
    {
        if (sc[0] == '%' && sc[1] != '%')
        {
            if (!count)
                len = sc - fmt;
            count++;
        }
    }
    //if (!len)
    //    strcat(str,fmt);
    //else
    //    strncat(str,fmt,len);
    return count;
}

void sendShardCmd(char * cmd, char *fmt,...)
{
    va_list ap;
    char    *p,str[10000] = "shardcomm ";

    if(!UsingChatServer())
    {
        addSystemChatMsg( textStd("NotConnectedToChatServer"), INFO_USER_ERROR, 0 );
        return;
    }

    strcatf(str, "%s ", cmd);

    va_start(ap, fmt);

    for(p=fmt;*p;++p)
    {
        if (p[0] == '%' && p[1] == 's')
        {
            char * s = va_arg(ap,char *);
            if (!s)
                break;
        
            strcatf(str," \"%s\"",escapeString(s));
            ++p;
        }
        else if (p[0] == '%' && p[1] == 'd')
        {
            int i = va_arg(ap,int);

            strcatf(str," \"%d\"",i);
            ++p;
        }
        else 
        {
            strcatf(str, "%c", *p);
        }
    }
    va_end(ap);

    if(gPrintShardCommCmds)
    {    
        conPrintf("SEND--> %s", str);
    }

    cmdParse(str);
}

void receiveShardComm(Packet *pak)
{
    char *originalStr = pktGetString(pak);
    char    *str, *args[100];
    int        i,count;

    estrCreate(&str);
    estrConcatCharString(&str, originalStr);
    count = tokenize_line(str,args,0);
    for(i=0;i<count;i++)
        strcpy(args[i],unescapeString(args[i])); // This strcpy is safe since the result will be smaller than the input

    if(count <= 0) // used to assert but can fail silently
        return;

    if(gPrintShardCommCmds)
    {    
        char buf[10000] = "";
        
        for(i=0;i<count;i++)
            strcatf(buf,"%s ",args[i]);

         conPrintf("RECV--> %s\n",buf);
        printf("shardcomm: %s\n", buf);
    }
    
    if (!processShardCmd(args, count))
    {
        // if it didn't process correctly, send a message back to the logserver.
        Packet *newPacket;
        char *estr;
        estrCreate(&estr);
        estrConcatf(&estr, "Bad Packet received by client: %s", originalStr);
        newPacket = pktCreateEx(&comm_link, CLIENT_LOGPRINTF);
        pktSendString(newPacket, estr);
        commSendPkt(newPacket);
        estrDestroy(&estr);
    }
    estrDestroy(&str);

    //sendShardCmd("usermode %s %s %s %s","test","bruce","+operator","+join");
}

void receivePackagedEnt(Packet *pak)
{
    char *str = strdup(pktGetString(pak));
    if(str)
    {
        char *pname = playerPtr()?playerPtr()->name:NULL;
        char filename[MAX_PATH];
        FILE *file;

        sprintf(filename, "n:/nobackup/resource/characters/common/%s/%s.txt","unknown",pname?pname:"unknown");
        mkdirtree(filename);
        if(file = fopen(filename, "wt"))
        {
            char *tok = strtok(str, "\n");
            printf("packaged ent:\n");
            while(tok)
            {
                int len = strlen(tok);
                if(strncmp(tok, "CurrentCostume", strlen("CurrentCostume"))
                   && strncmp(tok, "InfluencePoints", strlen("InfluencePoints"))
                   && strncmp(tok, "KeyBind", strlen("KeyBind"))
                   && (strncmp(tok, "Badges[", strlen("Badges[")) || len<2 || strcmp(&tok[len-2]," 0")))
                {
                    fprintf(file, "%s\n", tok);
                }
                tok = strtok(NULL, "\n");
            }
            fclose(file);
            printf("Saved ent to %s\n",filename);
        }
        free(str);
    }
}

static void receiveByteAlignmentTag(Packet* pak)
{
    pktAlignBitsArray(pak);
}

static void logPacketInfo(char const *fmt, ...)
{
    if (logPackets)
    {
        va_list ap;
        va_start(ap, fmt);
        log_vprintf("packets.log", fmt, ap);
        va_end(ap);
    }
}

static int handleGameCmd(Packet *pak, int cmd)
{
    #if USE_NEW_TIMING_CODE
        static PerfInfoStaticData* cmdBitCounts[SERVER_CMD_COUNT];
        static PerfInfoStaticData* cmdTimers[SERVER_CMD_COUNT];
    #else
        static PerformanceInfo* cmdBitCounts[SERVER_CMD_COUNT];
        static PerformanceInfo* cmdTimers[SERVER_CMD_COUNT];
    #endif

    int cmds[1000];
    int cmd_count = 0;

#ifdef SHOW_ALL_SERVER_CMDS
    consoleSetFGColor(COLOR_BLUE | COLOR_BRIGHT);
    printf("Handling game commands. pak %4d\n", pak->id);
    consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
#endif


    for(;;)
    {
        int timer_started = 0;

        if (cmd==-1)
            cmd = pktGetBitsPack(pak,1);
        if (!cmd)
            break;

        if(cmd_count < ARRAY_SIZE(cmds))
            cmds[cmd_count++] = cmd;

        PERFINFO_RUN(
            if(cmd >= 0 && cmd < ARRAY_SIZE(cmdBitCounts))
            {
                const char* name = getServerCmdName(cmd);
                
                timer_started = 1;
                
                START_BIT_COUNT_STATIC(pak, &cmdBitCounts[cmd], name);
                PERFINFO_AUTO_START_STATIC(name, &cmdTimers[cmd], 1);
            }
        );
//#define SHOW_ALL_SERVER_CMDS
#ifdef SHOW_ALL_SERVER_CMDS
        consoleSetFGColor(COLOR_BLUE | COLOR_BRIGHT);
        printf("  pak %4d: handleGameCmd of [%d] %s (%d bytes)\n", pak->id, cmd, getServerCmdName(cmd), pak->stream.size);
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
#endif

        logPacketInfo("SGC: %d, %s", cmd, getServerCmdName(cmd));
        switch(cmd)
        {
            xcase SERVER_GAME_CMDS:break;                                        
            _Ch(SERVER_RECEIVE_DIALOG,                    receiveDialog)
            _Ch(SERVER_RECEIVE_DIALOG_POWREQ,            receiveDialogPowerRequest)
            _Ch(SERVER_RECEIVE_DIALOG_TEAMLEVEL,        receiveTeamChangeDialog)
            _Ch(SERVER_RECEIVE_DIALOG_NAG,                receiveDialogNag)
            _Ch(SERVER_RECEIVE_DIALOG_IGNORABLE,        receiveDialogIgnorable)
            _Ch(SERVER_RECEIVE_DIALOG_WIDTH,            receiveDialogWithWidth)
            _Ch(SERVER_RECEIVE_INFO_BOX,                receiveInfoBox)
            _Ch(SERVER_DOOR_CMD,                        receiveDoorMsg)
            _Ch(SERVER_SEND_CONSOLE_OUTPUT,                receiveConsoleOutput)
            _Ch(SERVER_SET_CLIENT_STATE,                receiveServerSetClientState)
            _Ch(SERVER_CON_PRINTF,                        receiveConPrintf)
            _Ch(SERVER_SEND_CHAT_MSG,                    receiveChatMsg)
            _Ch(SERVER_SEND_FLOATING_DAMAGE,            receiveFloatingDmg)
            _Ch(SERVER_CONTROL_PLAYER,                    receiveControlPlayer)
            _Ch(SERVER_CONTACT_DIALOG_OPEN,                receiveContactDialog)
            _Ch(SERVER_CONTACT_DIALOG_CLOSE,            receiveContactDialogClose)
            _Ch(SERVER_CONTACT_DIALOG_OK,                receiveContactDialogOk)
            _Ch(SERVER_CONTACT_DIALOG_YESNO,            receiveContactDialogYesNo)
            _Ch(SERVER_CONTACT_STATUS,                    ContactStatusListReceive)
            _Ch(SERVER_ACCESSIBLE_CONTACT_STATUS,        ContactStatusAccessibleListReceive)
            _Ch(SERVER_CONTACT_SELECT,                    ContactSelectReceive)
            _Ch(SERVER_TASK_SELECT,                        TaskSelectReceive)
            _Ch(SERVER_TASK_STATUS,                        TaskStatusListReceive)
            _Ch(SERVER_TASK_REMOVE_TEAMMATE_TASKS,        TaskStatusListRemoveTeammates)
            _Ch(SERVER_MISSION_ENTRY,                    receiveMissionEntryText)
            _Ch(SERVER_MISSION_EXIT,                    receiveMissionExitText)
            _Ch(SERVER_MISSION_KICKTIMER,                MissionKickReceive)
            _Ch(SERVER_TEAMCOMPLETE_PROMPT,                receiveTeamCompletePrompt)
            _Ch(SERVER_BROKER_CONTACT,                    receiveBrokerAlert)
            _Ch(SERVER_CLUE_UPDATE,                        ClueListReceive)
            _Ch(SERVER_KEYCLUE_UPDATE,                    KeyClueListReceive)
            _Ch(SERVER_SOUVENIRCLUE_UPDATE,                scReceiveAllHeaders)
            _Ch(SERVER_SOUVENIRCLUE_DETAIL,                scReceiveDetails)
            _Ch(SERVER_SEND_PLAYER_INFO,                receiveEntityInfo)
            _Ch(SERVER_SEND_COMBAT_STATS,                receiveClientCombatStats)
            _Ch(SERVER_MISSION_OBJECTIVE_TIMER_UPDATE,    MissionReceiveObjectiveTimer)
            _Ch(SERVER_TASK_DETAIL,                        TaskDetailReceive)
            _Ch(SERVER_BROWSER_TEXT,                    browserReceiveText)
            _Ch(SERVER_BROWSER_CLOSE,                    browserReceiveClose)
            _Ch(SERVER_STORE_OPEN,                        storeReceiveOpen)
            _Ch(SERVER_STORE_CLOSE,                        storeReceiveClose)

            _Ch(SERVER_DEAD_NOGURNEY,                    receiveDeadNoGurney)
            _Ch(SERVER_DOORANIM,                        DoorAnimReceiveStart)
            _Ch(SERVER_DOOREXIT,                        DoorAnimReceiveExit)
            _Ch(SERVER_FACE_ENTITY,                        receiveFaceEntity)
            _Ch(SERVER_FACE_LOCATION,                    receiveFaceLocation)
            _Ch(SERVER_SEND_FLOATING_INFO,                receiveFloatingInfo)
            _Ch(SERVER_SEND_PLAY_SOUND,                    receivePlaySound)
            _Ch(SERVER_SEND_FADE_SOUND,                    receiveFadeSound)
            _Ch(SERVER_SEND_DESATURATE_INFO,            receiveDesaturateInfo)
            _Ch(SERVER_SET_STANCE,                        entity_StanceReceive)
            _Ch(SERVER_MAP_XFER_LIST,                    mapSelectReceiveText)
            _Ch(SERVER_MAP_XFER_LIST_INIT,                mapSelectReceiveInit)
            _Ch(SERVER_MAP_XFER_LIST_APPEND,            mapSelectReceiveUpdate)
            _Ch(SERVER_MAP_XFER_LIST_CLOSE,                mapSelectReceiveClose)
            _Ch(SERVER_ENABLE_CONTROL_LOG,                handleEnableControlLog)

            _Ch(SERVER_VISITED_MAP_CELLS,                receiveVisitMapCells)
            _Ch(SERVER_ALL_STATIC_MAP_CELLS,            receiveStaticMapCells)
            _Ch(SERVER_RESEND_WAYPOINT_REQUEST,            receiveResendWaypointRequest);
            _Ch(SERVER_UPDATE_WAYPOINT,                    receiveWaypointUpdate)
            _Ch(SERVER_SET_WAYPOINT,                    ServerWaypointReceive)
            _Ch(SERVER_LEVEL_UP,                        receiveLevelUp)
            _Ch(SERVER_LEVEL_UP_RESPEC,                    receiveLevelUpRespec)
            _Ch(SERVER_NEW_TITLE,                        receiveNewTitle)

            // team commands
            _Ch(SERVER_TEAM_OFFER,                        receiveTeamOffer)

            // taskforce commands
            _Ch(SERVER_TASKFORCE_KICK,                    receiveTaskforceKick)
            _Ch(SERVER_TASKFORCE_QUIT,                    receiveTaskforceQuit)

            // supergroup commands
            _Ch(SERVER_SUPERGROUP_OFFER,                receiveSuperGroupOffer)
            _Ch(SERVER_SGROUP_CREATE_REPLY,                receiveSuperResponse)
            _Ch(SERVER_HIDE_EMBLEM_UPDATE,                receiveHideEmblemChange)
            _Ch(SERVER_SUPERGROUP_COSTUME,                receiveSuperCostume)
            _Ch(SERVER_REGISTER_SUPERGROUP,                receiveRegisterSupergroup)
            _Ch(SERVER_SUPERGROUP_LIST,                    receiveSupergroupList)
            xcase SERVER_SEND_FRIENDS:
                receiveFriendsList(pak, playerPtr() );

            // alliance commands
            _Ch(SERVER_ALLIANCE_OFFER,                    receiveAllianceOffer)

            // league commands
            _Ch(SERVER_LEAGUE_OFFER,                    receiveLeagueOffer)
            _Ch(SERVER_LEAGUE_TEAM_KICK,                recieveLeagueTeamKick)
            _Ch(SERVER_LEAGUE_ER_QUIT,                    recieveLeagueERQuit)

            // cutscene commands
            _Ch(SERVER_CUTSCENE_UPDATE,                    receiveCutSceneUpdate)

            // trade commands
            _Ch(SERVER_TRADE_OFFER,                        receiveTradeOffer)
            _Ch(SERVER_TRADE_INIT,                        recieveTradeInit)
            _Ch(SERVER_TRADE_CANCEL,                    receiveTradeCancel)
            _Ch(SERVER_TRADE_UPDATE,                    receiveTradeUpdate)
            _Ch(SERVER_TRADE_SUCESS,                    receiveTradeSuccess)

            _Ch(SERVER_TASK_VISITLOCATION_UPDATE,        receiveVisitLocations)
            _Ch(SERVER_SEND_EMAIL_HEADERS,                receiveEmailHeaders)
            _Ch(SERVER_SEND_EMAIL_MESSAGE,                receiveEmailMessage)
            _Ch(SERVER_SEND_EMAIL_MESSAGE_STATUS,        receiveEmailMessageStatus)

            _Ch(SERVER_BUG_REPORT,                        receiveBugReport)
            _Ch(SERVER_MISSION_SURVEY,                    receiveMissionSurvey)
            _Ch(SERVER_SEND_TRAY_ADD,                    receiveTrayObjAdd )

            _Ch(SERVER_COMBINE_RESPONSE,                combineSpec_receiveResult )
            _Ch(SERVER_BOOSTER_RESPONSE,                combineSpec_receiveBoosterResult )
            _Ch(SERVER_CATALYST_RESPONSE,                combineSpec_receiveCatalystResult )

            // Arena
            _Ch(SERVER_ARENA_KIOSK,                        receiveArenaKiosk)
            _Ch(SERVER_ARENA_EVENTS,                    receiveArenaEvents )
            _Ch(SERVER_ARENA_EVENTUPDATE,                receiveArenaUpdate )
            _Ch(SERVER_ARENA_UPDATE_TRAY_DISABLE,        receiveArenaUpdateTrayDisable)
            _Ch(SERVER_ARENAKIOSK_SETOPEN,                receiveArenaKioskOpen)
            _Ch(SERVER_ARENA_START_COUNT,                receiveArenaStartCountdown)
            _Ch(SERVER_ARENA_COMPASS_STRING,            receiveArenaCompassString)
            _Ch(SERVER_ARENA_VICTORY_INFORMATION,        receiveArenaVictoryInformation)
            _Ch(SERVER_ARENA_SCHEDULED_TELEPORT,        receiveArenaScheduledTeleport)
            _Ch(SERVER_ARENA_RUN_EVENT_WINDOW,            receiveServerRunEventWindow)
            _Ch(SERVER_ARENA_NOTIFY_FINISH,                receiveArenaFinishNotification)
            _Ch(SERVER_ARENA_ERROR,                        receiveArenaDialog)

            _Ch(SERVER_SGRAID_COMPASS_STRING,            receiveSGRaidCompassString)
            _Ch(SERVER_SGRAID_UPDATE,                    receiveSGRaidUpdate)
            _Ch(SERVER_SGRAID_ERROR,                    receiveSGRaidError)
            _Ch(SERVER_SGRAID_OFFER,                    receiveSGRaidOffer)
            _Ch(SERVER_SGRAID_INFO,                        receiveSGRaidInfo)

            _Ch(SERVER_TAILOR_OPEN,                        receiveTailorOpen );
            _Ch(SERVER_RESPEC_OPEN,                        receiveRespecOpen );
            _Ch(SERVER_AUCTION_OPEN,                    receiveAuctionOpen );
            _Ch(SERVER_SEARCH_RESULTS,                    receiveSearchResults );
            _Ch(SERVER_SCRIPT_DEBUGINFO,                receiveScriptDebugInfo );
            _Ch(SERVER_SHARDCOMM,                        receiveShardComm );
            _Ch(SERVER_SEND_REWARD_CHOICE,                receiveRewardChoice );
            _Ch(SERVER_VALIDATE_RESPEC,                    respec_validate );
            _Ch(SERVER_ARENA_UPDATE_PLAYER,                handleClientArenaPlayerUpdate);
            _Ch(SERVER_ARENA_REQRESULTS,                receiveArenaResults );
            _Ch(SERVER_ARENA_OFFER,                        receiveArenaInvite );
            
            _Ch(SERVER_SALVAGE_IMMEDIATE_USE_STATUS,    receiveSalvageImmediateUseStatus );

            _Ch(SERVER_SEND_PACKAGEDENT,                receivePackagedEnt );

            _Ch(SERVER_SCRIPT_UI,                        ScriptUIReceive);
            _Ch(SERVER_SCRIPT_UI_UPDATE,                ScriptUIReceiveUpdate );

            _Ch(SERVER_MISSION_WAYPOINT,                MissionWaypointReceive);
            _Ch(SERVER_MISSION_KEYDOOR,                    MissionKeydoorReceive);

            _Ch(SERVER_PET_UI_UPDATE,                    receivePetUpdateUI);

            _Ch(SERVER_ARENA_MANAGEPETS,                receiveArenaManagePets);
            _Ch(SERVER_UPDATE_PET_STATE,                receiveUpdatePetState);
            _Ch(SERVER_UPDATE_BASE_INTERACT,            receiveBaseInteract);
            _Ch(SERVER_UPDATE_BASE_MODE,                receiveBaseMode);
            _Ch(SERVER_UPDATE_RAID_STATE,                receiveRaidState);
            _Ch(SERVER_UPDATE_NAME,                        receiveUpdateName);
            _Ch(SERVER_UPDATE_PRESTIGE,                    receivePrestigeUpdate);
            //map generation stuff
            _Ch(SERVER_MAP_GENERATION,                    mapGeneratorPacketHandler);

            _Ch(SERVER_MISSION_CONTROL,                    MissionControlHandleResponse);
            _Ch(SERVER_DEBUG_LOCATION,                    DebugLocationReceive);

            _Ch(SERVER_EDIT_VILLAIN_COSTUME,            receiveEditCritterCostume);
            _Ch(SERVER_PLAQUE_DIALOG,                    receivePlaqueDialog);
            _Ch(SERVER_RAID_RESULT,                        uiRaidResultReceive);
            _Ch(SERVER_AUCTION_HISTORY,                    uiAuctionHouseHistoryReceive);
            _Ch(SERVER_AUCTION_ITEMSTATUS,                uiAuctionHouseItemInfoReceive);
            _Ch(SERVER_AUCTION_BATCH_ITEMSTATUS,        uiAuctionHouseBatchItemInfoReceive);
            _Ch(SERVER_AUCTION_LIST_ITEMSTATUS,            uiAuctionHouseListItemInfoReceive);
            _Ch(SERVER_AUCTION_BANNED_UPDATE,            uiAuctionHandleBannedUpdate);
            _Ch(SERVER_ACCOUNTSERVER_CHARCOUNT,            handleSendCharacterCounts);
            _Ch(SERVER_ACCOUNTSERVER_CATALOG,            handleSendAccountServerCatalog);
            _Ch(SERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY,    handleAccountPlayNCAuthKeyUpdate);
            _Ch(SERVER_ACCOUNTSERVER_CLIENTAUTH,        handleClientAuth);
            _Ch(SERVER_ACCOUNTSERVER_NOTIFYREQUEST,        receiveAccountServerNotifyRequest);
            _Ch(SERVER_ACCOUNTSERVER_INVENTORY,            receiveAccountServerInventory);

            _Ch(SERVER_FLASHBACK_LIST,                    receiveFlashbackListResponse);
            _Ch(SERVER_FLASHBACK_DETAILS,                receiveFlashbackDetailResponse);
            _Ch(SERVER_FLASHBACK_IS_ELIGIBLE,            receiveFlashbackEligibilityResponse);
            _Ch(SERVER_FLASHBACK_CONFIRM,                receiveFlashbackConfirmResponse);

            _Ch(SERVER_TASKFORCE_TIME_LIMITS,            receiveTaskforceTimeLimits);
            _Ch(SERVER_SETDEBUGPOWER,                    receiveDebugPower);
            _Ch(SERVER_AUTH_BITS,                        receiveAuthBits);
            _Ch(SERVER_SELECT_BUILD,                    receiveSelectBuild);
            _Ch(SERVER_MISSIONSERVER_SEARCHPAGE,        missionserver_game_receiveSearchPage);
            _Ch(SERVER_MISSIONSERVER_ARCINFO,            missionserver_game_receiveArcInfo);
            _Ch(SERVER_MISSIONSERVER_ARCDATA,            missionserver_game_receiveArcData);
            _Ch(SERVER_MISSIONSERVER_ARCDATA_OTHERUSER,    missionserver_game_receiveArcData_otherUser);
            _Ch(SERVER_ARCHITECTKIOSK_SETOPEN,            receiveArchitectKioskSetOpen);
            
            _Ch(SERVER_SG_COLOR_DATA,                    receiveSGColorData);
            _Ch(SERVER_RENAME_BUILD,                    receiveRenameBuild);
            _Ch(SERVER_COSTUME_CHANGE_EMOTE_LIST,        receiveCostumeChangeEmoteList);

            _Ch(SERVER_CONFIRM_SG_PROMOTE,                receiveConfirmSGPromote);
            
            _Ch(SERVER_LEVELINGPACT_INVITE,                receiveLevelingPactInvite);
            _Ch(SERVER_ARCHITECT_COMPLETE,                receiveArchitectComplete);
            _Ch(SERVER_ARCHITECT_SOUVENIR,                receiveArchitectSouvenir);
            _Ch(SERVER_ARCHITECT_INVENTORY,                receiveArchitectInventory);
            _Ch(SERVER_UPDATE_BADGE_MONITOR_LIST,        badgeMonitor_ReceiveFromServer);
            _Ch(SERVER_SEND_DIALOG,                        receiveServerDialog);
            _Ch(SERVER_SEND_TOKEN,                        receiveRewardToken);

            _Ch(SERVER_SG_PERMISSIONS,                    srHandlePermissionsSetBit);
            // Not yet
            //_Ch(SERVER_ENABLE_ASSERT_ON_BS_ERRORS,        receiveBSErrorsFlag);

            _Ch(SERVER_BEGIN_BYTE_ALIGNED_DATA,            receiveByteAlignmentTag);
            _Ch(SERVER_SEND_MORAL_CHOICE,                receiveMoralChoice);

            _Ch(SERVER_SEND_ITEMLOCATIONS,                receiveItemLocations);
            _Ch(SERVER_SEND_CRITTERLOCATIONS,            receiveCritterLocations);

            _Ch(SERVER_DETACH_SCRIPT_UI,                scriptUIReceiveDetach);

            _Ch(SERVER_SEND_POP_HELP_TAG,                receivePopHelpTag);
            _Ch(SERVER_SEND_POP_HELP_RESET,                receivePopHelpReset);
            _Ch(SERVER_SEND_POP_HELP_N,                    receivePopHelpN);
            _Ch(SERVER_SEND_KARMA_STATS,                receiveKarmaStats);

            _Ch(SERVER_EVENT_LIST,                        turnstileGame_handleEventList);
            _Ch(SERVER_QUEUE_STATUS,                    turnstileGame_handleQueueStatus);
            _Ch(SERVER_EVENT_READY,                        turnstileGame_handleEventReady);
            _Ch(SERVER_EVENT_READY_ACCEPT,                turnstileGame_handleEventReadyAccept);
            _Ch(SERVER_EVENT_START_STATUS,                turnstileGame_HandleEventStartStatus);
            _Ch(SERVER_EVENT_STATUS,                    turnstileGame_handleEventStatus);
            _Ch(SERVER_EVENT_JOIN_LEADER,                turnstileGame_handleJoinLeader);
        
            _Ch(SERVER_GOINGROGUE_NAG,                    receiveGoingRogueNag);
            _Ch(SERVER_INCARNATE_TRIAL_STATUS,            receiveIncarnateTrialStatus);
            _Ch(SERVER_POPHELP_EVENT_HAPPENED_BY_TAG,    receivePopHelpEventHappenedByTag);
            _Ch(SERVER_CHANGE_WINDOW_COLORS,            receiveChangeWindowColors);

            _Ch(SERVER_VISITOR_SHARD_XFER_DATA,            receiveShardVisitorData);
            _Ch(SERVER_VISITOR_SHARD_XFER_JUMP,            receiveShardVisitorJump);
            _Ch(SERVER_UPDATE_TURNSTILE_STATUS,            turnstileGame_handleTurnstileGeneralStatus);
            _Ch(SERVER_ENDGAMERAID_VOTE_KICK,            turnstileGame_handleVotekickRequest);
            _Ch(SERVER_COMBAT_MESSAGE,                    receiveCombatMessage);

            _Ch(SERVER_SHOW_CONTACT_FINDER_WINDOW,        receiveShowContactFinderWindow);
            _Ch(SERVER_OPEN_SALVAGE_REQUEST,            receiveSalvageOpenRequest);
            _Ch(SERVER_OPEN_SALVAGE_RESULTS,            receiveSalvageOpenResults);
            _Ch(SERVER_WEB_STORE_OPEN_PRODUCT,            receiveWebStoreOpenProduct);
            _Ch(SERVER_CONVERT_ENHANCEMENT_RESULT,        receiveConvertEnhancement);

            _Ch(SERVER_SUPPORT_HOME,                    displaySupportHome);
            _Ch(SERVER_SUPPORT_KB,                        displaySupportKB);
            _Ch(SERVER_DISPLAY_PRODUCT_PAGE,            displayProductPage);
            _Ch(SERVER_FORCE_QUEUE_FOR_EVENT,            turnstileGame_handleForceQueue);
            _Ch(SERVER_START_FORCED_MOVEMENT,            playerStartForcedFollow);

            xdefault:
                {
                    char assert_buffer[10000];
                    char* pos = assert_buffer;
                    int i;

                    assert_buffer[0] = 0;

                    if(cmd < SERVER_CMD_COUNT)
                        pos += sprintf(pos, "Unhandled Command %d\nPrevious Commands: ", cmd);
                    else
                        pos += sprintf(pos, "Invalid Command %d\nPrevious Commands: ", cmd);

                    for(i = 0; i < cmd_count - 1; i++)
                    {
                        pos += sprintf(pos, "%d, ", cmds[i]);
                    }

                    assertmsg(0, assert_buffer );
                }
                break;
        }

        PERFINFO_RUN(
            if(timer_started)
            {
                PERFINFO_AUTO_STOP();
                STOP_BIT_COUNT(pak);
            }
        );

        cmd = -1;
    }

    return FALSE;
}

static int commHandleMessage(Packet *pak,int cmd,NetLink *link)
{
    unsigned int cursorStart = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit;
    int ret = 1;
    
    logPacketInfo(
        //"Cmd, Id, Compress, Creation, Debug, RetransmitQueue, SendQueue, Ordered, OrderedId, Reliable, RetransCount, SibCount, SibId, SibPart, TruncId, Uid, XferTime"
        "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
        cmd,
        pak->id,
        pak->compress,
        pak->creationTime,
        pak->hasDebugInfo,
        pak->inRetransmitQueue,
        pak->inSendQueue,
        pak->ordered,
        pak->ordered_id,
        pak->reliable,
        pak->retransCount,
        pak->sib_count,
        pak->sib_id,
        pak->sib_partnum,
        pak->truncatedID,
        pak->UID,
        pak->xferTime,
        pak->stream);

    commGetStats(link, pak);

#ifdef SHOW_ALL_SERVER_CMDS
    if (cmd!=SERVER_UPDATE) {
        consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
        printf("pak %4d: commHandleMessage of %s (%d bytes)\n", pak->id, getServerCmdName(cmd), pak->stream.size);
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
    }
#endif
    switch(cmd)
    {
        xcase SERVER_GAME_CMDS:
            START_BIT_COUNT(pak, "handleGameCmd");
                //assert(0); // not sent individually anymore
                {
                    int pos = bsGetCursorBitPosition(&pak->stream);
                    int cmd = pktGetBitsPack(pak, 1);
                    bsSetCursorBitPosition(&pak->stream, pos);
                    if (cmd && cmd != SERVER_SEND_CHAT_MSG) {
                        writeConsole(OUTPUT_VERBOSE, "Message from server, %d ('%s'), was sent before receiving entity updates", cmd, getServerCmdName(cmd));
                    }
                    handleGameCmd(pak, -1);
                }
            STOP_BIT_COUNT(pak);
        xcase SERVER_ALLENTS:{
            // See comment for SERVER_GROUPS
            
            U32 ackFullUpdateID = pktGetBitsPack(pak, 1);
            
            START_INPUT_PACKET(pak, CLIENTINP_ACK_FULL_UPDATE);
            pktSendBitsPack(pak, 1, ackFullUpdateID);
            END_INPUT_PACKET
            
            netSendIdle(link);
            lnkBatchSend(link);
            
            // Intentional fall-through
        }
        case SERVER_UPDATE:
            PERFINFO_AUTO_START("entReceiveUpdate", 1);
                entReceiveUpdate(pak,cmd==SERVER_ALLENTS);

                PERFINFO_AUTO_START("handleGameCmd", 1);
                    START_BIT_COUNT(pak, "handleGameCmd");
                        if (!pktEnd(pak))
                        {
                            S32 gameCmd = pktGetBitsPack(pak, 1);
                            if (!gameCmd) {
                                assert(pktEnd(pak));
                            } else {
                                //assert(gameCmd==SERVER_GAME_CMDS);
                                handleGameCmd(pak, gameCmd);
                            }
                        }
                    STOP_BIT_COUNT(pak);
                PERFINFO_AUTO_STOP();
            PERFINFO_AUTO_STOP();
        xcase SERVER_MAP_XFER_WAIT:
            START_BIT_COUNT(pak, "mapXferWait");
            {
                char* mapname = pktGetString(pak);
                commServerWaitOK(mapname);
            }
            STOP_BIT_COUNT(pak);
        xcase SERVER_CMDSHORTCUTS:
            START_BIT_COUNT(pak, "receiveShortcuts");
                cmdReceiveShortcuts(pak);
                cmdReceiveServerCommands(pak);
            STOP_BIT_COUNT(pak);
        xcase SERVER_GROUPS:
            // This particular operation will take awhile to complete.
            // Send back packet acks so that the server knows not to continue
            // retransmitting packets.
            //
            // FIXME!!!
            //    The idle/ack packet might be lost in transit.  In this case,
            //    the server will continue to retransmit the packets while the
            //    game loads the game world as before.  A good way to get rid
            //    of this problem is to move the networking code into another
            //    thread.  This way, the game client will be able to continue
            //    acknowledging + receiving any packets while the main game
            //    thread is busy loading stuff.
            //
            START_BIT_COUNT(pak, "worldReceiveGroups");
                netSendIdle(link);
                lnkBatchSend(link);
                worldReceiveGroups(pak);
                editNetUpdate();
            STOP_BIT_COUNT(pak);
        xcase SERVER_DYNAMIC_DEFS:
            receiveDynamicDefChanges(pak);
        xcase SERVER_LARGE_PACKET:
            conPrintf("Received large packet of size %d bytes",pak->stream.size);
        xcase SERVER_EDITMSG:
            START_BIT_COUNT(pak, "editMsg");
            {
                EditorMessageType msgType;
                char    *msg;

                msgType = pktGetBits(pak,2);
                msg = pktGetString(pak);

                if ( msgType == EDITOR_MESSAGE_ERROR ) //Error from the editor
                    winErrorDialog(msg, "Program Error", 0, 0);
                else if( msgType == EDITOR_MESSAGE_HIGH_PRIORITY )  
                    winErrorDialog(msg, "The Editor Says", 0, 0); //Editor has a an important message like checkout
                
                if( msgType == EDITOR_MESSAGE_LOW_PRIORITY || msgType == EDITOR_MESSAGE_HIGH_PRIORITY )  
                    status_printf("%s",msg); //run of the mill message

                editNetUpdate();
            }
            STOP_BIT_COUNT(pak);
        xcase SERVER_DEBUGCMD:
            START_BIT_COUNT(pak, "cmdDebugHandle");
                cmdOldDebugHandle(pak);
            STOP_BIT_COUNT(pak);
        xcase SERVER_SENDENTDEBUGMENU:
            START_BIT_COUNT(pak, "entDebugReceiveCommands");
                entDebugReceiveCommands(pak);
            STOP_BIT_COUNT(pak);
        xcase SERVER_MAP_XFER_LOADING:
            commServerMapXferLoading(pak);
        xcase SERVER_MAP_XFER:
            START_BIT_COUNT(pak, "commServerMapXfer");
                commServerMapXfer(pak);
            STOP_BIT_COUNT(pak);
            ret = 0;
        xcase SERVER_FORCE_LOGOUT:
            START_BIT_COUNT(pak, "handleForcedLogout");
                handleForcedLogout(pak);
            STOP_BIT_COUNT(pak);
        xcase SERVER_CONNECT_MSG:
            START_BIT_COUNT(pak, "connectMsg");
            if (pktGetBitsPack(pak,1))
            {
                game_state.dbTimeZoneDelta = pktGetF32(pak); 
                STOP_BIT_COUNT(pak);
            }
            else
            {
                FatalErrorf("%s",pktGetString(pak));
                STOP_BIT_COUNT(pak);
                windowExit(0);
                ret = 0;
            }
        xcase SERVER_PERFORMANCE_UPDATE:
            PERFINFO_AUTO_START("receivePerfInfo", 1);
                START_BIT_COUNT(pak, "receivePerfInfo");
                    entDebugReceiveServerPerformanceUpdate(pak);
                STOP_BIT_COUNT(pak);
            PERFINFO_AUTO_STOP();
        xcase SERVER_CSR_BUG_REPORT:
            START_BIT_COUNT(pak, "csrBugReport");
            {
                char * summary = strdup(pktGetString(pak));
                char * info    = strdup(pktGetString(pak));
                StuffBuff sb;
                initStuffBuff(&sb, 1024);
                addStringToStuffBuff(&sb, "bug_internal 0 <cat=99> <summary=%s> <desc=%s>", summary, info);
                cmdParse(sb.buff);
                freeStuffBuff(&sb);
                free(summary);
                free(info);
            }
            STOP_BIT_COUNT(pak);
    }
    logPacketInfo("Size: %d", pak->stream.cursor.byte * 8 + pak->stream.cursor.bit - cursorStart);
    return ret;
}

int commStart(char *ip_str,int port,int connect_cookie)
{
    Packet    *pak;
    int        timer;
    int        result;

    if (!netConnectEx(&comm_link,ip_str,port,NLT_UDP,control_state.notimeout?NO_TIMEOUT:120,NULL,1)) // Leave this over a minute, TestClients often take this long to connect
        return 0;

    netLinkSetBufferSize(&comm_link, SendBuffer, 128*1024);
    netLinkSetBufferSize(&comm_link, ReceiveBuffer, 128*1024);

    comm_link.notimeout = !!control_state.notimeout;

    comm_link.alwaysProcessPacket = 1; // So we can call netLinkMonitor and have it scan for a specific packet, but still process all the ones in between

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_CONNECT);
    pktSendBitsPack(pak,1,connect_cookie);
    pktSendBits(pak, 1, game_state.cod);    

    sendCharacterCreate(pak);

    pktSend(&pak,&comm_link);

    timer = timerAlloc();
    timerStart(timer);

    while(1)
    {
        int link_state = netLinkMonitor(&comm_link, SERVER_CONNECT_MSG, commHandleMessage);

        if(link_state == LINK_DISCONNECTED) {
            result = 0;
            break;
        } else if(link_state) {
            result = 1;
            break;
        }

        if (!control_state.notimeout && timerElapsed(timer) > DEFAULT_TIMEOUT) {
            result = 0;
            break;
        }

        netIdle(&comm_link,1,2);
    }

    timerFree(timer);
    return result;
}

typedef struct
{
    Vec3    pyr;
    Vec3    pos;
    U8        third;
    F32        camdist;
    F32        fov_1st;
    F32        fov_3rd;
} ClientRecord;

F32        old_pkt_time,curr_pkt_time;

void commGetData()
{
    if (commConnected())
        lnkBatchReceive(&comm_link);
}

int commLinkLooksDead(NetLink *link)
{
    int        dead = 0;
    int        deadlinkThreshold;

    if (!commConnected())
        return 1;

    if(loadingScreenVisible())
        deadlinkThreshold = 120;
    else
        deadlinkThreshold = 60;
#ifdef TEST_CLIENT
    deadlinkThreshold = 120;
#endif

    dead |= linkLooksDead(link, deadlinkThreshold);

    return dead;
}

int commCheck(int lookfor)
{
    extern int glob_have_camera_pos;

    int got_it=0;
    int dead_link;

    PERFINFO_AUTO_START("top", 1);
        if(CS_NOT_RESPONSIVE == commCheckConnection())
        {
            // The mapserver may have gone down or is trying hard to keep up with the
            // server load.
            //
            // Stop pounding the server with network traffic.
            //        Stop the player from generating more movement commands and discard
            //        any queued movements.
            control_state.mapserver_responding = 0;
            //destroyControlStateChangeList(&control_state.csc_send_list);
            //destroyControlStateChangeList(&control_state.csc_to_process_list);
        }
        else
        {
            control_state.mapserver_responding = 1;
        }

        // If we're not connected, but we think we're playing the game, boot back to login!
        if (!commConnected() && glob_have_camera_pos != PLAYING_GAME && game_state.game_mode != SHOW_LOAD_SCREEN)
        {
            PERFINFO_AUTO_STOP();
            return 0;
        }
    PERFINFO_AUTO_STOP();
    
    // If we're in the middle of a map transfer, the net link to the map server
    // will have been destroyed.  Do not try to use it.
    if(do_map_xfer)
        return 0;

    PERFINFO_AUTO_START("commGetdata", 1);
        commGetData();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("commLinkLooksDead", 1);
        dead_link = commLinkLooksDead(&comm_link);
    PERFINFO_AUTO_STOP();
    
    if (dead_link)
    {
        if (glob_have_camera_pos != PLAYING_GAME && game_state.game_mode != SHOW_LOAD_SCREEN)
        {
            PERFINFO_AUTO_START("weird stuff", 1);
                showBgAdd(2);
                loadUpdate("Lost link to mapserver!",-1);
                showBgAdd(-2);
            PERFINFO_AUTO_STOP();
        }
        else
        {
            // Disconnect from server and go back to the login screen if the game isn't
            // in some sort of debugging mode.
            if(!game_state.local_map_server && !control_state.notimeout)
            {
                PERFINFO_AUTO_START("quitToLogin", 1);
                    // Go back to the login screen if it doesn't seem likely that the mapserver
                    // is coming back.

                    // Disconnect from the server.
                    //        At this point, it is fairly certain that the mapserver is dead.
                    //        This is just marking the link as disconnected in a nice way.
                    
                    PERFINFO_AUTO_START("commDisconnect()", 1);
                        commDisconnect();
                    PERFINFO_AUTO_STOP();

                    PERFINFO_AUTO_START("quitToLogin", 1);
                        // make sure to fade back in if we were in a door transfer
                        DoorAnimReceiveExit(NULL);
                    PERFINFO_AUTO_STOP();

                    PERFINFO_AUTO_START("quitToLogin", 1);
                        // Go back to the login screen.
                        printf("quitToLogin: %s\n", textStd("LostConn")); // For TestClient
                        quitToLogin(0);
                        //restartLoginScreen();
                    PERFINFO_AUTO_STOP();

                    PERFINFO_AUTO_START("dialogStd", 1);
                        // Tell the player the connection has been lost.
                        dialogStd(DIALOG_OK, textStd("LostConn"), NULL, NULL, NULL, NULL, 0);
                    PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();
                
                return 0;
            }
        }
    }

    {
        int old_retransmit = comm_link.disableRetransmits;

        // Disable *sending* of retransmits (will still queue up new packets for sending later)
        comm_link.disableRetransmits = !control_state.mapserver_responding;

        PERFINFO_AUTO_START("netLinkMonitor", 1);
            got_it = netLinkMonitor(&comm_link, lookfor, commHandleMessage);
        PERFINFO_AUTO_STOP_CHECKED("netLinkMonitor");
        
        PERFINFO_AUTO_START("lnkBatchSend", 1);
            lnkBatchSend(&comm_link);
        PERFINFO_AUTO_STOP();

        comm_link.disableRetransmits = old_retransmit;
    }

    return got_it;
}

int commReqScene(int isInitialLogin)
{
    extern int load_bar_color;
    Packet    *pak;
    int        timeout=0;
    int        timer = timerAlloc();
#ifndef TEST_CLIENT
    int index;

    for (index = eaSize(&g_comicLoadScreen_pageNames) - 1; index >= 0; index--)
    {
        free(g_comicLoadScreen_pageNames[index]);
    }
    eaDestroy(&g_comicLoadScreen_pageNames);
    g_comicLoadScreen_pageIndex = 0;
#endif

    PERFINFO_AUTO_START("commReqScene", 1);
    
        clearWaitingForFullUpdate();

        //sndVolumeControl( SOUND_VOLUME_FX, 0.0 );
        loadScreenResetBytesLoaded();
        showBgAdd(2);
        game_state.game_mode = SHOW_LOAD_SCREEN;
        texDynamicUnload(0);
        texLoadQueueStart();

        //loadstart_printf("Requesting scene...");
        pak = pktCreateEx(&comm_link, CLIENT_REQSCENE);
        pktSendBits(pak, 1, 0);
        pak->reliable = 1;
        pktSend(&pak,&comm_link);

        testClientRandomDisconnect(TCS_commReqScene_1);

        loadUpdate("Requesting scene",-1);

        timerStart(timer);
        while (!timeout && !commCheck(SERVER_GROUPS) && commConnected()) {
            if (!control_state.notimeout && timerElapsed(timer) > DEFAULT_TIMEOUT) {
                timeout=1;
            } else {
                netIdle(&comm_link, 1, 0.5);
                loadUpdate("Waiting for response from server...",-1);
            }
        }
        testClientRandomDisconnect(TCS_commReqScene_2);
        if (timeout || !commConnected()){
            printf("Server timeout\n");
            texLoadQueueFinish();
            quitToLogin(0);
            timerFree(timer);
            PERFINFO_AUTO_STOP();
            return 0;
        }
        //loadend_printf("");


        //loadstart_printf("Pre-requesting entities...");
        pak = pktCreateEx(&comm_link,CLIENT_REQALLENTS); // Request all ents be sent
        pak->reliable = 1;
        pktSend(&pak,&comm_link);
        testClientRandomDisconnect(TCS_commReqScene_3);

        loadUpdate("Requesting entities",-1);
        timerStart(timer);
        while (!timeout && !commCheck(SERVER_ALLENTS) && commConnected()) {
            if (!control_state.notimeout && timerElapsed(timer) > DEFAULT_TIMEOUT) {
                timeout=1;
            } else {
                netIdle(&comm_link, 1, 0.5);
                loadUpdate("Waiting for response from server...",-1);
            }
        }
        testClientRandomDisconnect(TCS_commReqScene_4);
        if (timeout || !commConnected()){
            //FatalErrorf("Server timeout");
            texLoadQueueFinish();
            quitToLogin(0);
            timerFree(timer);
            PERFINFO_AUTO_STOP();
            return 0;
        }

        PERFINFO_AUTO_START("load stuff", 1);
            gfxLoadRelevantTextures();
            forceGeoLoaderToComplete(); // This doesn't seem to do anything?
            texLoadQueueFinish();
            texDynamicUnload(1);
        PERFINFO_AUTO_STOP();

        pak = pktCreateEx(&comm_link,CLIENT_READY); // Request all ents be sent
        pktSendBitsPack( pak, 1, db_info.mapserver.cookie );
        pktSendBitsPack( pak, 1, isConsoleOpen() );
        pktSendBitsArray( pak, 128, auth_info.passwordMD5);
        pktSendBitsAuto(pak, isInitialLogin);
        pak->reliable = 1;
        pktSend(&pak,&comm_link);

        setWaitingForFullUpdate(); // Tells us we're waiting for the next full entity update packet before fading in

        rdrNeedGammaReset(); // 'Cause I'm paranoid and people were seeing gamma bugs when mapmoving, and this is free.

        testClientRandomDisconnect(TCS_commReqScene_5);
        timerFree(timer);

    PERFINFO_AUTO_STOP_CHECKED("commReqScene");

    return 1;
}

int commReqShortcuts()
{
    Packet    *pak;
    int        timeout=0;
    int        timer = timerAlloc();

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_REQSHORTCUTS);
    pktSend(&pak,&comm_link);

    loadUpdate("Requesting shortcuts",-1);
    while (!timeout && !commCheck(SERVER_CMDSHORTCUTS) && commConnected()) {
        if (!control_state.notimeout && timerElapsed(timer) > DEFAULT_TIMEOUT) {
            timeout=1;
        } else {
            netIdle(&comm_link, 1, 0.5);
        }
    }
    if (timeout || !commConnected())
    {
        //FatalErrorf("Server timeout");
        quitToLogin(0);
        timerFree(timer);
        return 0;
    }

    loadUpdate("got shortcuts",-1);

    timerFree(timer);

    return 1;
}

void clearCutScene( void )
{
    game_state.viewCutScene = 0;
    game_state.cutSceneDepthOfField = 0;
    unsetCustomDepthOfField(DOFS_SNAP, &g_sun);
}

int commConnect(char *addr,int port,int cookie)
{
    int result;
    
    writeConsole(OUTPUT_INFO, "Connecting to MapServer %s:%d (UDP) Cookie: %x", addr, port, cookie);

    PERFINFO_AUTO_START("commStart", 1);
        result = commStart(addr,port,cookie);
    PERFINFO_AUTO_STOP();
    
    if (!result)
    {
        return 0;
    }

    clearCutScene();
    
    entDebugClearServerPerformanceInfo();
    resetArenaVars();
    commResetControlState();

    lastRecvId = comm_link.last_recv_id;

    PERFINFO_AUTO_START("commReqShortcuts", 1);
        result = commReqShortcuts();
    PERFINFO_AUTO_STOP();
    
    if(!result)
    {
        return 0;
    }

    lastRecvId = comm_link.last_recv_id;

    // Tell the server to enable file checking if the client has it enabled.

    if(global_state.no_file_change_check_cmd_set && isDevelopmentMode())
    {
        cmdParse("nofilechangecheck_server 1");
    }

    return 1;
}

int tryConnect()
{
    if(!commConnect(makeIpStr(db_info.mapserver.ip),db_info.mapserver.port,db_info.mapserver.cookie))
        return 0;

    testClientRandomDisconnect(TCS_tryConnect);

    return 1;
}

int commTimeSinceLastReceive()
{
    if(!commConnected())
        return -1;
    return timerCpuSeconds() - comm_link.lastRecvTime;
}

ConnectionStatus commCheckConnection()
{
    int seconds = commTimeSinceLastReceive();
    if(seconds > 5)
    {
        return CS_NOT_RESPONSIVE;
    }

    if(commConnected()) {
#if LOG_TROUBLED_MODE
        xyprintf(2, 7, "Last Mapserver comm: %d secs ago", seconds); // fpe for test
#endif
        if(seconds > 3 && !do_map_xfer) {
            // set early warning indicator
            commSetTroubledMode(TroubledComm_Mapserver);
        }

        return CS_CONNECTED;
    }

    return CS_DISCONNECTED;
}

int commGetCharacterCountsAsync(void)
{
    int retval = 0;

    if (comm_link.socket && NLT_NONE != comm_link.type)
    {
        Packet *pak = pktCreateEx(&comm_link, CLIENT_ACCOUNT_SERVER_CMD);
        pktSendBitsAuto(pak, ACCOUNT_SERVER_CMD_CHAR_COUNT);
        pktSend(&pak, &comm_link);
        retval = 1;
    }

    return retval;
}
int commGetAccountServerCatalogAsync(void)
{
    int retval = 0;

    if (comm_link.socket && NLT_NONE != comm_link.type)
    {
        Packet *pak = pktCreateEx(&comm_link, CLIENT_ACCOUNT_SERVER_CMD);
        pktSendBitsAuto(pak, ACCOUNT_SERVER_CMD_GET_CATALOG);
        pktSend(&pak, &comm_link);
        retval = 1;
    }

    return retval;
}

int commGetAccountServerInventory()
{
    int retval = 0;

    if (comm_link.socket && NLT_NONE != comm_link.type )
    {

        Packet *pak = pktCreateEx(&comm_link, CLIENT_ACCOUNT_SERVER_CMD);
        pktSendBitsAuto(pak, ACCOUNT_SERVER_CMD_GET_INVENTORY);
        pktSend(&pak, &comm_link);
        retval = 1;
    }

    return retval;
}

void commSendPopHelp(unsigned int pop_help_event, unsigned int new_state)
{
    START_INPUT_PACKET(pak, CLIENTINP_POP_HELP_N);
    pktSendBitsPack(pak, 30, pop_help_event);
    pktSendBitsPack(pak, 2, new_state);
    END_INPUT_PACKET
}

void commSendPopHelpLatest(EntPlayer *pl)
{
    int i;

    START_INPUT_PACKET(pak, CLIENTINP_POP_HELP_LATEST);
    for(i=0; i<POP_HELP_LATEST_SIZE; i++)
        pktSendBitsPack(pak, 32, pl->popHelpLatest[i]);
    END_INPUT_PACKET
}

void commLogPackets(int value)
{
    logPackets = value;
}



///////////////////////////////////////////////////////////////////////////////////////////////
// TroubledMode gets set when we notice a slow tick to mapserver/dbserver, or when excess 
//    packet loss is detected.
///////////////////////////////////////////////////////////////////////////////////////////////

static TroubledCommReason sTroubledCommReason = TroubledComm_None;
static U32 sTroubledExpireTime = 0; // timersecondsSince2000() at which to expire troubled mode
#define TROUBLE_DURATION_SECS 30 // how long to stay in troubled mode after the last slow tick or lost packet occurs

bool commIsInTroubledMode()
{
    return sTroubledExpireTime != 0;
}

void commTickTroubledMode()
{
    if(sTroubledCommReason == TroubledComm_None)
        return; // nothing to check

    // see if time has expired yet
    assert(sTroubledExpireTime != 0);
    if(timerSecondsSince2000() > sTroubledExpireTime)
        commSetTroubledMode(TroubledComm_None);
}

void commSetTroubledMode(TroubledCommReason reason)
{
    TroubledCommReason prevReason = sTroubledCommReason;
    sTroubledCommReason = reason;
    if(reason != TroubledComm_None)
        sTroubledExpireTime = timerSecondsSince2000() + TROUBLE_DURATION_SECS;
    else
        sTroubledExpireTime = 0;

#if LOG_TROUBLED_MODE
    if(prevReason == TroubledComm_None && reason != TroubledComm_None) {
        static const char* reasonNames[] = {"None", "Mapserver Long Tick", "DbServer Long Tick", "Mapserver dropped packets"};
        STATIC_ASSERT(ARRAY_SIZE(reasonNames) == TroubledComm_Count);
        printf("***** ENTERTING TROUBLED COMM MODE: reason %s\n", reasonNames[reason]);
    } else if(prevReason != TroubledComm_None && reason == TroubledComm_None) {
        printf("***** Leaving TROUBLED COMM MODE\n");
    }
#endif
}
