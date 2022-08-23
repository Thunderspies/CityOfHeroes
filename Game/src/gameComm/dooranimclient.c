/*\
 *  dooranimclient.c - client functions to run door animations for player
 *
 *  Copyright 2003-2005 Cryptic Studios, Inc.
 */

#include "gameComm/dooranimclient.h"
#include "gameComm/dooranimcommon.h"
#include <utilitieslib/network/netio.h>
#include "clientcomm/clientcomm.h"
#include "player/player.h"
#include "entity/entPlayer.h"
#include "entity/entVarUpdate.h"
#include "UI/uiCursor.h"
#include "win/win_init.h"
#include "entity/entDebug.h"
#include "render/renderUtil.h"
#include <utilitieslib/utils/timing.h>
#include "UI/uiWindows.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/gfx.h"
#include "gameComm/initClient.h"
#include "render/tex.h"
#include "render/texWords.h"
#include "entity/entity.h"
#include "LWC.h"

static int dooranim_animfadeout = 0;
int dooranim_doorfade = 1; // whether the client will do fading on door entry/exit
#define FADEOUT_TIME 15
#define FADEIN_TIME 15

void DoorAnimClientCheckMove()
{
    Entity* player = playerPtr();
    if (!player || !player->pl || !player->pl->door_anim)
        return;
    DoorAnimMove(player, player->pl->door_anim);
    if (player->pl->door_anim->state == DOORANIM_DONE)
    {
        player->pl->door_anim->state = DOORANIM_ACK;
        EMPTY_INPUT_PACKET(CLIENTINP_DOORANIM_DONE);
    }
    if (player->pl->door_anim->pastthreshold && dooranim_animfadeout)
    {
        dooranim_animfadeout = 0;
        DoorAnimFadeOut(FADEOUT_TIME);
    }
}

int DoorAnimIgnorePositionUpdates()
{
    Entity* player = playerPtr();
    if (!player || !player->pl || !player->pl->door_anim)
        return 0;
    return 1; // if we are updating, we don't pay attention to position updates
}

int DoorAnimFrozenCam()
{
    Entity* player = playerPtr();
    if (!player || !player->pl || !player->pl->door_anim)
        return 0;
    return 1; // camera is frozen iff we are animating
}

// start an animation locally
void DoorAnimReceiveStart(Packet *pak)
{
    DoorAnimState* anim;
    Entity* player = playerPtr();

    // allocate a door animation structure and start running asyncronously
    assert(player);
    assert(!player->pl->door_anim);
    anim = calloc(sizeof(DoorAnimState), 1);
    anim->entrypos[0] = pktGetF32(pak);
    anim->entrypos[1] = pktGetF32(pak);
    anim->entrypos[2] = pktGetF32(pak);
    anim->targetpos[0] = pktGetF32(pak);
    anim->targetpos[1] = pktGetF32(pak);
    anim->targetpos[2] = pktGetF32(pak);
    anim->entryPointRadius = pktGetF32(pak);
    if (pktGetBits(pak, 1))
        anim->animation = strdup(pktGetString(pak));

    anim->state = pktGetBits( pak, 32 ); //TO DO, 8 or fewer?
    player->pl->door_anim = anim;
    dooranim_animfadeout = 1;
}

enum {
    DALT_NONE,
    DALT_WAITINGFORMOVE,
    DALT_WAITINGFORTEXLOAD,
} dooranim_loadingtextures=DALT_NONE;

U32 last_forced_move_pakid=0;

void DoorAnimReceiveExit(Packet *pak)
{
    Entity* player = playerPtr();
    int sentNewLocation = 0;
    int wasFadedOut;

    if (pak) {
        sentNewLocation = pktGetBits(pak, 1);
        if (sentNewLocation && pak->id <= last_forced_move_pakid) {
            // We seem to have already received the forced move!  Just start to fade in
            sentNewLocation = 0;
        }
    }
    doneWaitForDoorMode();
    wasFadedOut = DoorAnimIsFadedOut();
    if (wasFadedOut) {
        if (sentNewLocation) {
            verbose_printf("Exiting door, waiting for force move...");
            // The character has warped to a new location, wait for the position resync and load textures!
            dooranim_fadePercent = 100.0;
            dooranim_loadingtextures = DALT_WAITINGFORMOVE;
        } else {
            verbose_printf("Force exiting door...\n");
            // We just want to exit, no questions asked
            dooranim_loadingtextures = DALT_NONE;
            DoorAnimFadeIn(FADEIN_TIME);
        }
    }

     if(player )
    {
        if (okayToChangeGameMode() || wasFadedOut) {
            //start_menu( MENU_GAME );
            toggle_3d_game_modes( SHOW_GAME );
        }

        if (player->pl->door_anim)
        {
            if (player->pl->door_anim->animation)
                free(player->pl->door_anim->animation);
            free(player->pl->door_anim);
            player->pl->door_anim = NULL;
        }
    }
}

//////////////////////////////////////////////////////////////// fading

F32 dooranim_fadePercent = 0.0;
static F32 targetPercent = 0.0;
static F32 startPercent = 0.0;
static F32 endTime = 0.0;
static F32 curTime = 0.0;
static U32 fadedOutTime=0;

void DoorAnimCheckForcedMove()
{
    if (dooranim_loadingtextures==DALT_WAITINGFORMOVE) {
        verbose_printf("received forced move.\n");
        dooranim_fadePercent = 100.0;
        gfxLoadRelevantTextures();
        texWordsFlush();
        dooranim_loadingtextures = DALT_WAITINGFORTEXLOAD;
    }
    last_forced_move_pakid=control_state.forced_move_pkt_id;
}

void DoorAnimCheckFade()
{
    if(!commConnected())
    {
        DoorAnimSetFade(0);
    }

    if (dooranim_loadingtextures == DALT_WAITINGFORTEXLOAD) {
        if (texLoadsPending(1)==0) {
            DoorAnimFadeIn(FADEIN_TIME);
            dooranim_loadingtextures = DALT_NONE;
        } else {
            // Give background loader priority!
            Sleep(100);
        }
    }

    if (dooranim_fadePercent != targetPercent)
    {
        curTime += TIMESTEP;
        if (curTime > endTime) curTime = endTime;
        dooranim_fadePercent = startPercent + curTime * (targetPercent - startPercent) / endTime;
        if (nearf(dooranim_fadePercent, targetPercent)) {
            dooranim_fadePercent = targetPercent;
            if (DoorAnimIsFadedOut()) {
                fadedOutTime = timerCpuSeconds();
            }
        }
    }

    if (DoorAnimIsFadedOut()) {
        U32 curTime = timerCpuSeconds();
        static U32 secondsBeforeForcedFadeIn = 30;
        if (fadedOutTime==0) {
            fadedOutTime = timerCpuSeconds();
        }
        if (curTime - fadedOutTime > secondsBeforeForcedFadeIn) {
            DoorAnimSetFade(0);
        }
    } else {
        fadedOutTime = 0;
    }

    if (dooranim_fadePercent)
    {
        int w, h, alpha;
        windowSize(&w, &h);
        alpha = (0xff * (dooranim_fadePercent)) / 100;
        displayEntDebugInfoTextBegin();
        drawFilledBox(0, 0, w, h, (alpha << 24) | 0x000000);
        displayEntDebugInfoTextEnd();
    }
}

void DoorAnimSetFade(F32 percent)
{
    if (percent < 0.0) percent = 0.0;
    if (percent > 100.0) percent = 100.0;
    targetPercent = startPercent = dooranim_fadePercent = percent;
}

void DoorAnimFadeIn(F32 fadeTime)
{
    if (!dooranim_doorfade) return;
    targetPercent = 0.0;
    startPercent = 100.0;
    endTime = fadeTime;
    curTime = 0.0;
}

void DoorAnimFadeOut(F32 fadeTime)
{
    if (!dooranim_doorfade) return;
    targetPercent = 100.0;
    startPercent = 0.0;
    endTime = fadeTime;
    curTime = 0;
}

int DoorAnimIsFadedOut()
{
    return nearf(targetPercent, 100.0);
}

//////////////////////////////////////////////////////////////// old door state functions

// this wait state will time out on the client in 10 seconds
static U32 waiting_for_door_time;
static int waiting_for_door;

void setWaitForDoorMode()
{
    waiting_for_door = 1;
    waiting_for_door_time = timerCpuSeconds();
}

int waitingForDoor()
{
    if (waiting_for_door)
    {
        U32 cur_time = timerCpuSeconds();
        if (cur_time - waiting_for_door_time > 10)
        {
            waiting_for_door = 0;
            return 0;
        }
        return 1;
    }
    return 0;
}

void doneWaitForDoorMode()
{
    waiting_for_door = 0;
}

void enterDoorClient( const Vec3 pos, char* map_id )
{
    char* nomap = "0";
    if (!map_id) map_id = nomap;

#if defined(CLIENT) && !defined(TEST_CLIENT)
    // The end of the Neutral Tutorial uses a door for both Heroes and Villains
    // to teleport to Atlas Park or Mercy Island
    // TODO: Additional checks to see which map we are going to
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_3))
    {
        return;
    }
#endif

    setWaitForDoorMode();
    START_INPUT_PACKET( pak, CLIENTINP_ENTER_DOOR );
    if(pos)
    {
        pktSendBits( pak, 1, 0 );
        pktSendF32( pak, pos[0] );
        pktSendF32( pak, pos[1] );
        pktSendF32( pak, pos[2] );
    }
    else
    {
        pktSendBits( pak, 1, 1 );
    }
    pktSendString( pak, map_id );
    END_INPUT_PACKET
}

void enterDoorFromSgid( int idSgrp, char* map_id )
{
    char* nomap = "0";
    if (!map_id) map_id = nomap;

    setWaitForDoorMode();
    START_INPUT_PACKET( pak, CLIENTINP_ENTER_BASE_CHOICE );
    pktSendBitsAuto( pak, idSgrp );
    pktSendString( pak, map_id );
    END_INPUT_PACKET
}



/////////////////////////////////////////////////////////////// mission kick functions
static U32 g_missionKickTime;    // 0 if not going to be kicked, server dbSeconds otherwise

void MissionKickReceive(Packet *pak)
{
    g_missionKickTime = pktGetBits(pak, 32);
}

void MissionKickClear()
{
    g_missionKickTime = 0;
}

// when player will be disconnected from mission, -1 if not being disconnected
int MissionKickSeconds()
{
    int seconds;

    if (!g_missionKickTime) return -1;
    seconds = g_missionKickTime - timerSecondsSince2000WithDelta();
    if (seconds < 0) seconds = 0;
    return seconds;
}

// show leaving mission message, add disconnecting to message if passed 1
void CheckMissionKickProgress(int disconnecting)
{
    char* str;
    int h, w;

    if (!g_missionKickTime) return;

    windowSize(&w,&h);
    font( &title_18 );
    font_color( CLR_WHITE, CLR_GREEN );
    str = disconnecting? "LeavingMissionTimeDisconnect": "LeavingMissionTime";
    cprnt( w / 2, h / 2 -30, 90, 1.f, 1.f, str, MissionKickSeconds());
}



