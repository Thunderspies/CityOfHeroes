/*
 *
 *    uiTurnstile.c - Copyright 2010 NC Soft
 *        All Rights Reserved
 *        Confidential property of NC Soft
 *
 *    Turnstile system for matching players for large raid content
 *
 */

#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/timing.h>
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include "entity/character_eval.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/teamCommon.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "formatter/smf_main.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "uiListView.h"
#include "UI/uiClipper.h"
#include <utilitieslib/utils/Cbox.h>
#include "gameComm/wdwbase.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiTurnstile.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiDialog.h"
#include "sound/sound.h"
#include "UI/uiLeague.h"
#include "UI/uiContextMenu.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "UI/uiChat.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiTabControl.h"

#ifndef TEST_CLIENT
#include "UI/uiInput.h"
#endif

#ifdef _snprintf
#undef _snprintf
#endif
#define _snprintf(buff, size, fmt, ...) _snprintf_s((buff), (size), (size) - 1, (fmt), __VA_ARGS__);

// NOTE:  This SDI is copy/pasted from TurnstileCommon.c!
// TurnstileCommon.c is NOT actually common!  It is server-only, but across multiple servers!
StaticDefineInt ParseTurnstileCategory[] =
{
    DEFINE_INT
    { "Trial",            kTurnstileCategory_Trial            },
    { "IncarnateTrial",    kTurnstileCategory_IncarnateTrial    },
    { "HolidayTrial",    kTurnstileCategory_HolidayTrial        },
    { "TaskforceContact",    kTurnstileCategory_TaskforceContact    },    
    DEFINE_END
};

static TurnstileMissionClient **turnstileMissions;
static float currentWait;

// values to handle message throttling
static U32 requestTick;
static U32 queueTick;
static int inQueue;
static int eventResponseExpected;

static char event_name[256];
static int event_acceptedCount;
static int event_declinedCount;
static int event_inProgress;
static int event_playerCount;
static U32 event_instanceID;

static int mainWindowEscrowedState = WINDOW_UNINITIALIZED;    // Use WINDOW_UNINITIALIZED because it's guaranteed to be different from any legal state we might see

static int status_playerCount;
static U32 status_replied1;
static U32 status_replied2;
static U32 status_status1;
static U32 status_status2;

static uiTabControl *eventCategories = NULL;
static UIListView *eventListView;

typedef enum WFD_types
{
    WAITING_FOR_DATA_DATA_RECIEVED,
    WAITING_FOR_DATA_REQUESTING_DATA,
    WAITING_FOR_DATA_OLD_DATA,
    WAITING_FOR_DATA_COUNT,
}WFD_types;

static int waitingForData = WAITING_FOR_DATA_OLD_DATA;

static TurnstileMissionClient s_firstAvailableDummyMission;
static TurnstileMissionClient s_firstAvailableCategoryDummyMission[kTurnstileCategory_Count];

static U32 windowColor;
static int tsJoinFlags = 0;
static char sForceQueueEvent[MAX_NAME_LEN] = "";

static TextAttribs smfTSTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0,
    /* piScale       */  (int *) (int) (SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};
// TEST CLIENT hackery
// I could just wrap the offending portions of this in an #ifndef TEST_CLIENT block, but that has a great tendancy to make the text go grey and for
// "right click - go to definition" to stop working.  I dunno about anybody else, but that infuriates me, however that is a different rant for a
// different time and a different place.  So instead I punt the test client problem in a rather different direction.  Not 100% pretty,
// but a very small price to pay to prevent massive globs of greyed out text.

// Of course, if Viz Stew got the sense of what's really elided by a #ifdef block correct, I would not need to bugger around like this.  GJ MS, lrn 2 code, nubs.

#ifdef TEST_CLIENT
#define drawFrame (void)
#define window_getDims (int)
#define window_getMode (int)
#define smf_SetRawText (void)
#define smf_Display (int)
#define window_GetMode (int)
#define smf_SetFlags (void)
#define smfBlock_Create() NULL
#define drawStdButton (int)
#define clipperPop() /* */
#define clipperPushRestrict (void)
#define prnt (void) 
#define uiCHIGetIterator (void) 
#define uiCHIGetNextColumn (int) 
#define uiDrawBox (void) 
#define uiLVAddItem (void)
#define uiLVClear (void)
#define uiLVHandleInput (void)
#define uiLVDisplay (void)
#define uiLVSetScale (void)
#define uiLVHFinalizeSettings (void)
#define uiLVHSetWidth (void)
#define uiLVSetHeight (void)
#define uiLVSetDrawColor (void)
#define window_GetColor (void)
#define uiLVGetFullDrawHeight (int)
#define uiBoxAlter (void)
#define uiLVSetClickable (void)
#define uiLVEnableSelection (void)
#define uiLVEnableMouseOver (void)
#define uiLVFinalizeSettings (void)
#define uiLVHAddColumn (void)
#define uiLVHAddColumnEx (void)
#define uiLVCreate() NULL
#define addToolTip(void)
#define mouseCollision(void) 1
#define league_CanOfferMembership(void) 1
#define gSelectedName 0
#ifdef display_sprite
#undef display_sprite
#endif
#define display_sprite (void)
#ifdef setToolTip
#undef setToolTip
#endif
#define setToolTip (void)
#endif

#define CHECKBOX_HT        16
#define LINE_HT            24

int OnEndgameRaidMap()
{
    return game_state.mission_map == 2;
}
int turnstileGameCM_EndgameRaidLeader(void *foo)
{
    if (!OnEndgameRaidMap())
        return CM_HIDE;
    return league_CanOfferMembership(foo);
}
int turnstileGameCM_onEndGameRaid(void *foo)
{
    return OnEndgameRaidMap() ? CM_AVAILABLE : CM_HIDE;
}
void turnstile_votekickTarget(void *foo)
{
    Entity * e = playerPtr();

    if( turnstileGameCM_onEndGameRaid(NULL) == CM_AVAILABLE )
    {
        char buf[128];

        if( current_target )
            sprintf( buf, "tut_votekick %s", current_target->name );
        else if ( gSelectedDBID )
            sprintf( buf, "tut_votekick %s", gSelectedName );
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// Dialog response callbacks

// Shim that just type casts the response and notes that we're no longer in the queue
static void AcceptDeclineCallback(void *response)
{
    inQueue = turnstileGame_generateEventResponse((char *) response);
    queueTick = 0;
    if (inQueue)
    {
        game_state.pendingTSMapXfer = 1;
        queueTick = timerSecondsSince2000() + CLIENT_RESPONSE_DELAY;
    }
}

// Shim that calls remove routine
static void ConfirmLeaveQueue(void *param)
{
    turnstileGame_generateRemoveFromQueue();
}

static void eventListHandleClick(TurnstileMissionClient *mission)
{
    int i;

    if (!mission->eligible)
        return;
    mission->selected = !mission->selected;
    // Special handling for first available
    if (mission->category == -1)
    {
        for(i = eaSize(&turnstileMissions) - 1; i >= 0; i--)
        {
            turnstileMissions[i]->selected = mission->selected;
        }
        for (i = 0; i < kTurnstileCategory_Count; ++i)
        {
            s_firstAvailableCategoryDummyMission[i].selected = mission->selected;
        }
    }
    else
    {
        int isAllCatMissionPick = 0;
        s_firstAvailableDummyMission.selected = 0;
        if (mission->index == 0)
        {
            //    Special handling for the per category "all" mission
            int i;
            for(i = eaSize(&turnstileMissions) - 1; i >= 0; i--)
            {
                if (turnstileMissions[i]->category == mission->category)
                {
                    turnstileMissions[i]->selected = mission->selected;
                }
            }
            isAllCatMissionPick = mission->selected;
        }
        else
        {
            s_firstAvailableCategoryDummyMission[mission->category].selected = 0;
            //    not a special mission
            if (mission->selected)
            {
                int areAllCatMissionPicked = 1;
                for(i = eaSize(&turnstileMissions) - 1; i >= 0; i--)
                {
                    if (turnstileMissions[i]->category == mission->category)
                    {
                        if (!turnstileMissions[i]->selected)
                        {
                            areAllCatMissionPicked = 0;
                        }
                    }
                }
                if (areAllCatMissionPicked)
                {
                    s_firstAvailableCategoryDummyMission[mission->category].selected = 1;
                    isAllCatMissionPick = 1;
                }
            }
        }

        if (isAllCatMissionPick)
        {
            int areAllMissionsPicked = 1;
            for (i = 0; i < kTurnstileCategory_Count; ++i)
            {
                if (!s_firstAvailableCategoryDummyMission[i].selected)
                {
                    areAllMissionsPicked = 0;
                    break;
                }
            }
            if (areAllMissionsPicked)
            {
                s_firstAvailableDummyMission.selected = 1;
            }
        }
    }
}

static int currentCategory = kTurnstileCategory_Trial;

static void turnstileGame_updateEventListView()
{
    int i;
    int n;
    int count = 0;

    uiLVClear(eventListView);

    n = eaSize(&turnstileMissions);
    for(i = 0; i < n; i++)
    {
        if (turnstileMissions[i]->category == currentCategory)
        {
            count++;
        }
    }

    if (count >= 2)
    {
        uiLVAddItem(eventListView, &s_firstAvailableDummyMission);
        uiLVAddItem(eventListView, &s_firstAvailableCategoryDummyMission[currentCategory]);
    }

    for(i = 0; i < n; i++)
    {
        if (turnstileMissions[i]->category == currentCategory)
        {
            uiLVAddItem(eventListView, turnstileMissions[i]);
        }
    }
    waitingForData = WAITING_FOR_DATA_DATA_RECIEVED;
}

// Handles SERVER_EVENT_LIST
//  Generated by void turnstileMapserver_handleRequestEventList(Entity *e) in Turnstile.c
void turnstileGame_handleEventList(Packet *pak)
{
    int i;
    //int seenFirst;
    int num_mission;
    float overallAverage;
    TurnstileMissionClient ** oldMissions = turnstileMissions;

    eaCreate(&turnstileMissions);
    overallAverage = pktGetF32(pak);
    num_mission = pktGetBitsAuto(pak);
    for (i = 0; i < num_mission; i++)
    {
        if (pktGetBits(pak, 1))
        {
            TurnstileMissionClient *mission;
            char *strMem;
            char name[4096];
            int category;
            int eligible;
            float avgWait;
            int isWeeklyTF;

            // Read into local buffers initially, then do a single malloc to get everything.
            // This allows destruction of the entry with a single call to free
            strncpyt(name, pktGetString(pak), sizeof(name));
            category = pktGetBitsAuto(pak);
            avgWait = pktGetF32(pak);
            eligible = pktGetBits(pak, 1);
            isWeeklyTF = pktGetBits(pak, 1);
            if (mission = malloc(sizeof(TurnstileMissionClient) + strlen(name) + 1))
            {
                strMem = (char *) mission;
                strMem = &strMem[sizeof(TurnstileMissionClient)];
                strncpyt(mission->name = strMem, name, strlen(name) + 1);

                mission->category = category;
                mission->avgWait = avgWait;
                mission->selected = 0;
                mission->index = i + 1;
                mission->eligible = eligible;
                mission->isWeeklyTF = isWeeklyTF;
                eaPush(&turnstileMissions, mission);
            }
        }
    }

    if (oldMissions != NULL)
    {
        // Copy the selected flag from the old array of missions to the new array of missions
        int numOldMissions = eaSize(&oldMissions);
        int numNewMissions = eaSize(&turnstileMissions);
        int oldMissionIndex = 0;

        for (i = 0; i < numNewMissions; i++)
        {
            for (;oldMissionIndex < numOldMissions; oldMissionIndex++)
            {
                if (turnstileMissions[i]->index == oldMissions[oldMissionIndex]->index)
                {
                    turnstileMissions[i]->selected = oldMissions[oldMissionIndex]->selected;
                    oldMissionIndex++;
                    break;
                }
            }
        }

        eaDestroyEx(&oldMissions, NULL);
    }

    if (s_firstAvailableDummyMission.name == NULL)
    {
        s_firstAvailableDummyMission.name = strdup("FirstAvailableStr");
    }

    if (s_firstAvailableCategoryDummyMission[kTurnstileCategory_Trial].name == NULL)
    {
        s_firstAvailableCategoryDummyMission[kTurnstileCategory_Trial].name = strdup("FirstAvailableTrialStr");
    }

    if (s_firstAvailableCategoryDummyMission[kTurnstileCategory_IncarnateTrial].name == NULL)
    {
        s_firstAvailableCategoryDummyMission[kTurnstileCategory_IncarnateTrial].name = strdup("FirstAvailableIncarnateTrialStr");
    }

    if (s_firstAvailableCategoryDummyMission[kTurnstileCategory_HolidayTrial].name == NULL)
    {
        s_firstAvailableCategoryDummyMission[kTurnstileCategory_HolidayTrial].name = strdup("FirstAvailableHolidayTrialStr");
    }

    if (s_firstAvailableCategoryDummyMission[kTurnstileCategory_TaskforceContact].name == NULL)
    {
        s_firstAvailableCategoryDummyMission[kTurnstileCategory_TaskforceContact].name = strdup("FirstAvailableTaskforceContactStr");
    }

    s_firstAvailableDummyMission.avgWait = overallAverage;
    s_firstAvailableDummyMission.selected = 0;
    s_firstAvailableDummyMission.category = -1;
    s_firstAvailableDummyMission.eligible = 1;

    for (i = 0; i < kTurnstileCategory_Count; i++)
    {
        int j;
        s_firstAvailableCategoryDummyMission[i].avgWait = overallAverage;
        s_firstAvailableCategoryDummyMission[i].selected = 0;
        s_firstAvailableCategoryDummyMission[i].index = 0;
        s_firstAvailableCategoryDummyMission[i].category = i;
        s_firstAvailableCategoryDummyMission[i].eligible = 1;
        for (j = 0; j < eaSize(&turnstileMissions); ++j)
        {
            if (turnstileMissions[j]->index && turnstileMissions[j]->category == i && !turnstileMissions[j]->eligible)
            {
                s_firstAvailableCategoryDummyMission[i].eligible = 0;
                s_firstAvailableDummyMission.eligible = 0;
                break;
            }
        }

    }


    turnstileGame_updateEventListView();
}

// Handles SERVER_QUEUE_STATUS
//  Generated by void turnstileMapserver_handleQueueStatus(Packet *pak) in Turnstile.c
void turnstileGame_handleQueueStatus(Packet *pak)
{
    int i;
    // TODO based on the presence or absence of inQueue, display a small status window with currentWait
    inQueue = pktGetBits(pak, 1);
    queueTick = 0;
    s_firstAvailableDummyMission.selected = 0;
    for (i = 0; i < kTurnstileCategory_Count; i++)
    {
        s_firstAvailableCategoryDummyMission[i].selected = 0;
    }
    for (i = 0; i < eaSize(&turnstileMissions); ++i)
    {
        if (!turnstileMissions[i]->eligible)
        {
            turnstileMissions[i]->selected = 0;
        }
    }
    if (inQueue)
    {
        int numEvent;
        int j;
        int eventList[MAX_EVENT];
        int allSelected = -1;

        currentWait = pktGetF32(pak);
        numEvent = pktGetBitsAuto(pak);
        for (i = 0; i < numEvent; ++i)
        {
            eventList[i] = pktGetBitsAuto(pak) + 1;        //    these come down as base 0, while the index is base 1
            for (j = 0; j < eaSize(&turnstileMissions); ++j)
            {
                if (turnstileMissions[j]->index == eventList[i])
                {
                    turnstileMissions[j]->selected = 1;
                }
            }
        }
        
        for (i = 0; i < kTurnstileCategory_Count; ++i)
        {
            int allCatMissionsAccepted = 0;
            for (j = 0; j < eaSize(&turnstileMissions); ++j)
            {
                if (turnstileMissions[j]->category == i && turnstileMissions[j]->index && turnstileMissions[j]->selected)
                {
                    //    at least 1 is accepted
                    allCatMissionsAccepted = 1;
                }
                if (turnstileMissions[j]->category == i && turnstileMissions[j]->index && !turnstileMissions[j]->selected)
                {
                    allCatMissionsAccepted = 0;
                    break;
                }
            }
            if (allCatMissionsAccepted)
            {
                s_firstAvailableCategoryDummyMission[i].selected = 1;
                if (allSelected)
                    allSelected = 1;
            }
            else
            {
                allSelected = 0;
            }
        }
        if (allSelected == 1)
        {
            s_firstAvailableDummyMission.selected = 1;
        }
    }
    else
    {
        currentWait = 0.0f;
        s_firstAvailableDummyMission.selected = 0;
    }
}

// Handles SERVER_EVENT_READY
//  Generated by void turnstileMapserver_handleEventReady(Packet *pak) in Turnstile.c
// Generates CLIENTINP_EVENT_READY_ACK
//  Handled by void turnstileMapserver_handleEventReadyAck(Entity *e, Packet *pak) in Turnstile.c
void turnstileGame_handleEventReady(Packet *pak)
{
    char title[256] = {0};
    char body[512] = {0};

    strncpyt(event_name, pktGetString(pak), sizeof(event_name));
    event_inProgress = pktGetBits(pak, 1);
    event_playerCount = pktGetBitsAuto(pak);
    event_instanceID = pktGetBits(pak, 32);

    eventResponseExpected = 1;

    // Ack this so that the dbserver gets to hear that we know the event is ready.
    START_INPUT_PACKET(pak, CLIENTINP_EVENT_READY_ACK);
    pktSendBits(pak, 32, event_instanceID);
    END_INPUT_PACKET;

#if TEST_CLIENT
    turnstileGame_generateEventResponse("1");
#elif CLIENT
    sndPlay("IT_Ready_01", SOUND_GAME);
    if (event_inProgress)
    {
        _snprintf(title, sizeof(title), "%s", textStd("LFGNeedsAssistance", event_name));
        _snprintf(body, sizeof(body), "%s", textStd("LFGEventInProgress", event_playerCount));
        turnstileDialogWindowActivate(title, body, NULL, CLIENT_RESPONSE_DELAY,
                                   "EnterEventStr", "DeclineAndWaitStr",
                                   AcceptDeclineCallback, "accept",
                                   AcceptDeclineCallback, "decline",
                                   1);


    }
    else
    {
        _snprintf(title, sizeof(title), "%s", textStd("LFGEventReady", event_name));
        _snprintf(body, sizeof(body), "%s", textStd("LFGEventNotStarted", event_playerCount));
        turnstileDialogWindowActivate(title, body, NULL, CLIENT_RESPONSE_DELAY,
                                   "EnterEventStr", "LeaveQueueStr",
                                   AcceptDeclineCallback, "accept",
                                   AcceptDeclineCallback, "decline",
                                   1);
    }
#endif
}

// Handles SERVER_EVENT_READY_ACCEPT
//  Generated by void turnstileMapserver_handleEventReadyAccept(Packet *pak) in Turnstile.c
void turnstileGame_handleEventReadyAccept(Packet *pak)
{
    char title[256] = {0};
    char body[512] = {0};

    event_acceptedCount = pktGetBitsAuto(pak);
    event_declinedCount = pktGetBitsAuto(pak);
    event_playerCount = pktGetBitsAuto(pak);

    _snprintf(title, sizeof(title), "%s", textStd("TSAcceptedWaitingToEnterEvent"));
    _snprintf(body, sizeof(body), "%s%s%s", textStd("TSAcceptedWaitingString", event_acceptedCount, event_playerCount),
        event_declinedCount ? textStd((event_declinedCount == 1) ? "TSDeclinedCountSingleString" : "TSDeclinedCountString", event_declinedCount): "",
        textStd("TSAdditionalStartInfoString"));
    turnstileDialogWindowActivate(NULL, title, body, 0, NULL, NULL,
        NULL, NULL, NULL, NULL, !eventResponseExpected);
}

// Handles SERVER_EVENT_START_STATUS
//  Generated by void turnstileMapserver_handleEventFailedStart(Packet *pak_in) in Turnstile.c
//  Generated by void turnstileMapserver_handleEventStart(Packet *pak_in) in Turnstile.c
void turnstileGame_HandleEventStartStatus(Packet *pak)
{
    window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_SHRINKING);
    if (pktGetBits(pak, 1))
    {
        if (!pktGetBits(pak,1))
            game_state.pendingTSMapXfer = 0;
    }
    else
    {
        game_state.pendingTSMapXfer = 0;
        if (pktGetBits(pak, 1))
        {
            dialogStd(DIALOG_OK, "EventFailedStartRemoved", NULL, NULL, NULL, NULL, 0);
        }
        else
        {
            dialogStd(DIALOG_OK, "EventFailedStartReadded", NULL, NULL, NULL, NULL, 0);
        }
    }
}

// Handles SERVER_EVENT_STATUS
//  Generated by void turnstileMapserver_handleEventStatus(Packet *pak) in Turnstile.c
void turnstileGame_handleEventStatus(Packet *pak)
{
    status_playerCount = pktGetBitsAuto(pak);
    status_replied1 = pktGetBits(pak, 32);
    status_replied2 = pktGetBits(pak, 32);
    status_status1 = pktGetBits(pak, 32);
    status_status2 = pktGetBits(pak, 32);
}

void turnstileGame_handleForceQueue(Packet *pak)
{
    const char * eventName = pktGetString(pak);

    window_setMode(WDW_TURNSTILE, WINDOW_GROWING);
    strcpy(sForceQueueEvent, eventName);
}

static int ts_inviter_db_id;
static int ts_inviter_mission_id;
static void sendJoinLeaderAccept(void *data)
{
    char msg[64];
    sprintf(msg, "turnstile_invite_player_accept %i 1 %i", ts_inviter_db_id, ts_inviter_mission_id);
    cmdParse( msg );
}
static void sendJoinLeaderDecline(void *data)
{
    char msg[64];
    sprintf(msg, "turnstile_invite_player_accept %i 0 %i", ts_inviter_db_id, ts_inviter_mission_id);
    cmdParse( msg );
}
// Handles SERVER_EVENT_JOIN_LEADER
//    Generated by "turnstile_invite_player_relay" in cmdchat.c
void turnstileGame_handleJoinLeader(Packet *pak)
{
    char *playerName = strdup(pktGetString(pak));
    char *missionName = strdup(pktGetString(pak));
    char *displayName = NULL;
    ts_inviter_db_id = pktGetBitsAuto(pak);
    ts_inviter_mission_id = pktGetBitsAuto(pak);
    estrConcatf(&displayName, "LFG_Mission_%s", missionName);
    dialogStd( DIALOG_YES_NO, textStd("IncarnateTrialJoinLeader", playerName, textStd(displayName)), NULL, NULL, sendJoinLeaderAccept, sendJoinLeaderDecline, 1 );
    estrDestroy(&displayName);
    free(missionName);
    free(playerName);
}

// Handle a request for the event list generated here in the game client.  Either from a slash command or user interaction with a window.
// This will only send a request if at least 15 secvonds have elapsed since the last request
// Generates CLIENTINP_REQUEST_EVENT_LIST
//  Handled by void turnstileMapserver_handleRequestEventList(Entity *e) in Turnstile.c
void turnstileGame_generateRequestEventList()
{
    if (timerSecondsSince2000() > requestTick)
    {
        START_INPUT_PACKET(pak, CLIENTINP_REQUEST_EVENT_LIST);
        END_INPUT_PACKET;
        requestTick = timerSecondsSince2000() + 5;
        waitingForData = WAITING_FOR_DATA_REQUESTING_DATA;
    }
    else
    {
        waitingForData = WAITING_FOR_DATA_OLD_DATA;
    }
}

// Handles SERVER_UPDATE_TURNSTILE_STATUS
//    Generated by static void sendTurnstileGeneralUpdate(Entity *e) in Turnstile.c
void turnstileGame_handleTurnstileGeneralStatus(Packet *pak)
{
    Entity *e = playerPtr();
    int inTurnstileQueue = pktGetBitsAuto(pak);
    int lastTurnstileEventID = pktGetBitsAuto(pak);
    int lastTurnstileMissionID = pktGetBitsAuto(pak);
    if (e && e->pl)
    {
        inQueue = inTurnstileQueue;
        queueTick = 0;
        e->pl->lastTurnstileEventID = lastTurnstileEventID;
        e->pl->lastTurnstileMissionID = lastTurnstileMissionID;
    }
}

static void turnstileGame_voteKickYes(void *foo)
{
    cmdParse("tut_votekick_opinion 1");
}
static void turnstileGame_voteKickNo(void *foo)
{
    cmdParse("tut_votekick_opinion 0");
}
// Handles SERVER_ENDGAMERAID_VOTE_KICK
//    Generated by void EndGameRaidInitiateVoteKick() in EndGameRaid.h
void turnstileGame_handleVotekickRequest(Packet *pak)
{
    int kicked_dbid = pktGetBitsAuto(pak);
    int i;
    Entity *e = playerPtr();
    if (e && e->league)
    {
        if (kicked_dbid == e->db_id)
        {
            dialogStd(DIALOG_OK, textStd("TUTVoteKickSelf"), NULL, NULL, turnstileGame_voteKickNo, 0, 0);
        }
        else
        {
            char *playerName = NULL;
            for (i = 0; i < e->league->members.count; ++i)
            {
                if (kicked_dbid == e->league->members.ids[i])
                {
                    playerName = e->league->members.names[i];
                }
            }
            if (playerName)
            {
                dialogStd(DIALOG_YES_NO, textStd("TUTVoteKickOpinion", playerName), NULL, NULL, turnstileGame_voteKickYes, turnstileGame_voteKickNo, 0);
            }
        }
    }
}

// Handle a request for the event list generated here in the game client.  Either from a slash command or user interaction with a window.
// This will only send a request if at least 15 secvonds have elapsed since the last request (tied in with the request tick becuase.. might as well)
// Generates CLIENTINP_REQUEST_REJOIN_INSTANCE
//  Handled by void turnstileMapserver_rejoinInstance(Entity *e) in Turnstile.c
void turnstileGame_generateRejoinRequest(int rejoin)
{
    if (timerSecondsSince2000() > queueTick)
    {
        START_INPUT_PACKET(pak, CLIENTINP_REQUEST_REJOIN_INSTANCE);
        pktSendBits(pak, 1, rejoin ? 1 : 0);
        END_INPUT_PACKET;
        queueTick = timerSecondsSince2000() + 15;
    }
}

// Send a "queue for events" message
// Generates CLIENTINP_QUEUE_FOR_EVENTS
//  Handled by void turnstileMapserver_handleQueueForEvents(Entity *e, Packet *pak) in Turnstile.c
void turnstileGame_generateQueueForEvents(int *events, int eventCount)
{
    int i;

    // Use this to throttle the rate at which join requests can be sent.
    if (timerSecondsSince2000() <= queueTick)
    {
        return;
    }

    if (eventCount != 0)
    {
        START_INPUT_PACKET(pak, CLIENTINP_QUEUE_FOR_EVENTS);
        pktSendBits(pak, 1, (tsJoinFlags & TS_GROUP_FLAGS_WILLING_TO_SUB) ? 1 : 0);
        pktSendBits(pak, 1, (tsJoinFlags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP) ? 1 : 0);
        pktSendBitsAuto(pak, eventCount);
        for (i = 0; i < eventCount; i++)
        {
            pktSendBitsAuto(pak, events[i]);
        }
        END_INPUT_PACKET;
        queueTick = timerSecondsSince2000() + 15;
    }
}

void turnstileGame_generateQueueForEventsByString(char *eventList)
{
    int    argc;
    char *argv[1024];
    int eventCount;
    int events[MAX_EVENT];
    int i;
    int eventNum;
    int firstAvailable = 0;

    argc = tokenize_line(eventList, argv, 0);
    eventCount = 0;
    for (i = 0; i < argc; i++)
    {
        // Players input these 1 based, so they are converted here.
        if ((eventNum = atoi(argv[i])) > 0)
        {
            events[eventCount++] = eventNum - 1;
        }
        else if (eventNum < 0)
        {
            firstAvailable = 1;
            break;
        }
    }

    if (firstAvailable)
    {
        eventCount = 0;
        for(i = eaSize(&turnstileMissions) - 1; i >= 0; i--)
        {
            events[eventCount++] = turnstileMissions[i]->index - 1;
        }
    }

    turnstileGame_generateQueueForEvents(events, eventCount);
}

// Send a "remove from queue" message
// Generates CLIENTINP_REMOVE_FROM_QUEUE
//  Handled by void turnstileMapserver_handleRemoveFromQueue(Entity *e, Packet *pak) in Turnstile.c
void turnstileGame_generateRemoveFromQueue()
{
    START_INPUT_PACKET(pak, CLIENTINP_REMOVE_FROM_QUEUE);
    END_INPUT_PACKET;
    queueTick = timerSecondsSince2000() + 15;
    game_state.pendingTSMapXfer = 0;
}

// Send the players response to the "Your event is ready" message
// Generates CLIENTINP_EVENT_RESPONSE
//  Handled by void void turnstileMapserver_handleEventResponse(Entity *e, Packet *pak) in Turnstile.c
int turnstileGame_generateEventResponse(char *response)
{
    int accept;
    
    if (eventResponseExpected)
    {
        accept = stricmp(response, "yes") == 0 || stricmp(response, "accept") == 0 || stricmp(response, "1") == 0;
        START_INPUT_PACKET(pak, CLIENTINP_EVENT_RESPONSE);
        pktSendBits(pak, 1, accept);
        pktSendBits(pak, 32, event_instanceID);
        END_INPUT_PACKET;
        eventResponseExpected = 0;
        return accept;
    }
    return 0;
}

#define EVENT_SELECT_COL        "Select"
#define EVENT_NAME_COL            "Name"
#define EVENT_WAIT_COL            "Wait"

// Leave this amount of space at the bottom of the window to display the "Join" / "Leave" buttons.
#define EVENT_BUTTON_AREA        40

// This converts the float second count to a wait time string.  This is the first half of the operation, it
// only produces a time string.  This is then fed to prnt(...) with a suitably chosen format string so that
// it'll localize correctly.
static char *waitTimeToRawStr(float waitTime)
{
    int hours;
    int fraction;
    int minutes;
    static char buff[32];

    // Under 60 seconds, this will unilaterally become "< 1 Min"
    if (waitTime < 60.0f)
    {
        return "1";
    }
    // Over an hour, becomes h.d i.e. the equivalent of %.1f
    if (waitTime >= 3600.0f)
    {
        // Add 1/20th hour, so that when we convert to a decimal, it'll round to the nearest tenth.
        waitTime += 180.0f;

        hours = (int) (waitTime / 3600.0f);
        // Get just the first decimal into fraction;
        fraction = (int) ((waitTime - 3600.0f * hours) / 360.0f);
        // This should never happen - just call me paranoid
        if (fraction > 9)
        {
            fraction = 9;
        }

        _snprintf(buff, sizeof(buff), "%d.%d", hours, fraction);
    }
    else
    {
        minutes = (int) (waitTime / 60.0f);
        // This should also never happen
        if (minutes > 59)
        {
            minutes = 59;
        }
        _snprintf(buff, sizeof(buff), "%d", minutes);
    }
    buff[sizeof(buff) - 1] = 0;
    return buff;
}

UIBox eventListViewDisplayItem(UIListView *list, PointFloatXYZ rowOrigin, void *userSettings, TurnstileMissionClient *item, int itemIndex)
{
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator = {0}; 
    int color;
    char *fmt;
    AtlasTex *check;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;
    box.height = (LINE_HT-2) * list->scale;
    box.width = list->header->width;

    if (item->isWeeklyTF)
    {
        color = CLR_CONSTRUCT(255, 100, 255, 32);
        uiDrawBox(&box, rowOrigin.z + 1.0f, color, color);
    }
    else if (itemIndex & 1)
    {
        color = CLR_CONSTRUCT(255, 255, 255, 32);
        uiDrawBox(&box, rowOrigin.z + 1.0f, color, color);
    }

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z += 2.0f;

    if (item->avgWait < 60.0f)
    {
        color = CLR_CONSTRUCT(0, 192, 0, 255);
        fmt = "LFGUnderOneMinute";
    }
    else if (item->avgWait >= 3600.0f)
    {
        color = CLR_RED;
        fmt = "LFGOverOneHour";
    }
    else
    {
        color = CLR_WHITE;
        fmt = "LFGNormal";
    }

    if (!item->eligible)
    {
        color = CLR_GREY;
    }

    font_color(color, color);

    // Iterate through each of the columns.
    while (uiCHIGetNextColumn(&columnIterator, list->scale))
    {
        UIColumnHeader *column = columnIterator.column;
        UIBox clipBox;
        pen.x = rowOrigin.x + columnIterator.columnStartOffset * list->scale;

        clipBox.origin.x = pen.x;
         clipBox.origin.y = pen.y;
        clipBox.height = 20 * list->scale;

        if (columnIterator.columnIndex >= eaSize(&columnIterator.header->columns) - 1)
        {
            clipBox.width = 1000 * list->scale;
        }
        else
        {
            clipBox.width = columnIterator.currentWidth * list->scale;
        }

        clipperPushRestrict(&clipBox);
        // Decide what to draw on on which column.
        if (stricmp(column->name, EVENT_SELECT_COL) == 0)
        {
            if (item->selected)
            {
                check = atlasLoadTexture("Context_checkbox_filled.tga");
                display_sprite(check, pen.x + 4.0f * list->scale, pen.y + 0.2f * CHECKBOX_HT * list->scale, pen.z + 2, list->scale, list->scale, windowColor | 0x000000ff);
            }

            check = atlasLoadTexture("Context_checkbox_empty.tga");
            display_sprite(check, pen.x + 4.0f * list->scale, pen.y + 0.2f * CHECKBOX_HT * list->scale, pen.z + 1, list->scale, list->scale, item->eligible ? (windowColor | 0x000000ff) : CLR_GREY);
        }
        else if (stricmp(column->name, EVENT_NAME_COL) == 0)
        {
            CBox cbox;
            char displayName[4096];
            BuildCBox(&cbox, clipBox.x, clipBox.y, clipBox.width-10*list->scale, clipBox.height);
            if (mouseCollision(&cbox))
            {
                static ToolTip eventToolTip;
                char displayHelp[4096];
                char *toolTipText = estrTemp();
                strncpyt(displayHelp, "LFG_Mission_Help_", 1024);
                strncatt(displayHelp, item->name);
                if (item->isWeeklyTF)
                {
                    estrConcatf(&toolTipText, "%s<br>%s", textStd("LFG_WeeklyTF_Text"), textStd(displayHelp));
                }
                else
                {
                    estrConcatCharString(&toolTipText, textStd(displayHelp));
                }
                addToolTip( &eventToolTip );
                setToolTip( &eventToolTip, &cbox, toolTipText, 0, MENU_GAME, WDW_TURNSTILE );
                estrDestroy(&toolTipText);
            }
            strncpyt(displayName, "LFG_Mission_", 1024);
            strncatt(displayName, item->name);
            prnt(pen.x, pen.y + 18 * list->scale, pen.z, list->scale, list->scale, "%s", textStd(displayName));
        }
        else
        {
            prnt(pen.x, pen.y + 18 * list->scale, pen.z, list->scale, list->scale, fmt, waitTimeToRawStr(item->avgWait));
        }

        clipperPop();
    }

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

static UIListView *eventList_initSettings()
{
    UIListView *listView;
    Wdw *turnstileWindow = wdwGetWindow(WDW_TURNSTILE);

    listView = uiLVCreate();

    uiLVHAddColumnEx(listView->header,  EVENT_SELECT_COL, "", 30, 30, 0);
    uiLVHAddColumnEx(listView->header,  EVENT_NAME_COL, "LFGNameString", 260, 500, 1);
    uiLVHAddColumn(listView->header,  EVENT_WAIT_COL, "LFGWaitString", 30);

    uiLVFinalizeSettings(listView);
    uiLVEnableMouseOver(listView, 0);
    uiLVEnableSelection(listView, 0);
    uiLVSetClickable(listView, 0);

    listView->displayItem = eventListViewDisplayItem;

    return listView;
}
static void drawTextFlagButton(float cx, float cy, float z, float sc, float wd, int flag, char *text)
{
#ifndef TEST_CLIENT
    AtlasTex *check = NULL;
    float checkWd = 0;
    float textSc = sc;
    CBox cbox;
    font(&game_12);
    if ((tsJoinFlags & flag))
    {
        check = atlasLoadTexture("Context_checkbox_filled.tga");
        if (check)
        {
            checkWd = check->width;
            display_sprite(check, cx - (checkWd*0.5f*sc), cy - (0.5*checkWd*sc), z + 2, sc, sc, windowColor | 0x000000ff);
        }
    }

    check = atlasLoadTexture("Context_checkbox_empty.tga");
    if (check)
    {
        checkWd = check->width;
        display_sprite(check, cx - (checkWd*0.5f*sc) * sc, cy - (0.5*checkWd*sc), z + 1, sc, sc, windowColor | 0x000000ff);
    }
    
    if (wd)
    {
        int fontWd = str_wd(&game_12, sc, sc, textStd(text));
        float remainingWd = wd - (checkWd*sc+10*sc);
        if (fontWd > remainingWd)
        {
            textSc = str_sc(&game_12, remainingWd, textStd(text));
        }
    }
    cprntEx(cx+ checkWd*0.5*sc+10*sc, cy, z, textSc, textSc, CENTER_Y, textStd(text));
    BuildCBox(&cbox, cx-(checkWd*0.5f*sc), cy-(checkWd*0.5f*sc), checkWd, checkWd);

    if (mouseClickHit(&cbox, MS_LEFT))
    {
        tsJoinFlags ^= flag;
    }
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Window handlers

int turnstileWindow()
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    int mode;
    int fullDrawHeight;
    int joinMode;
    int teamLead;
    int isSolo;
    UIBox windowDrawArea;
    PointFloatXYZ pen;
    Entity *player = playerPtr();
    char confirmText[1024];
    char leaveText[4096];
    static int lastMode = WINDOW_DOCKED;
    static ScrollBar eventListSb = { WDW_TURNSTILE };
    int waitingForResponse = 0;
#ifndef TEST_CLIENT
    int newCategory;
    char *curTab;
#endif

    // Silence the compiler complaint caused by test client shenanigens
#ifdef TEST_CLIENT
    x = y = z = wd = ht = sc = 0.0f;
    color = 0;
#endif

    if (!window_getDims(WDW_TURNSTILE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        return 0;
    }

    windowDrawArea.x = x;
    windowDrawArea.y = y + 20 * sc;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht - EVENT_BUTTON_AREA * sc - 20 * sc;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3 * sc);

    pen.x = windowDrawArea.x;
    pen.y = windowDrawArea.y;
    pen.z = z;

    windowColor = color;

    font(&game_12);

// This is the only UI file that uses tab controls that is in the test client...
#ifndef TEST_CLIENT
    if (eventCategories == NULL)  
    {
        eventCategories = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
        uiTabControlRemoveAll(eventCategories);
        uiTabControlAdd(eventCategories, "TrialString", (void*)kTurnstileCategory_Trial);
        uiTabControlAdd(eventCategories, "IncarnateTrialString", (void*)kTurnstileCategory_IncarnateTrial);
        uiTabControlAdd(eventCategories, "TaskforceContactString", (void*)kTurnstileCategory_TaskforceContact);
        uiTabControlAdd(eventCategories, "HolidayTrialString", (void*)kTurnstileCategory_HolidayTrial);
    }
#endif

    if (eventListView == NULL)
    {
        eventListView = eventList_initSettings();
    }

    mode = window_getMode(WDW_TURNSTILE);
    waitingForResponse = queueTick > timerSecondsSince2000() || (window_getMode(WDW_TURNSTILE_DIALOG) != WINDOW_DOCKED);
    // We don't have one time init here, we do however want to request the event list every time the window opens.
    // Thus if the current mode is WINDOW_GROWING and the previous mode isn't, request the list.  Note that
    // turnstileGame_generateRequestEventList() will throttle these in the event the user is opening and closing
    // the window quickly.
    if ((waitingForData == WAITING_FOR_DATA_OLD_DATA) || (mode == WINDOW_GROWING && lastMode != WINDOW_GROWING) || (!inQueue && (timerSecondsSince2000() > (requestTick + 30))))
    {
        turnstileGame_generateRequestEventList();
    }
    lastMode = mode;
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

// This is the only UI file that uses tab controls that is in the test client...
#ifndef TEST_CLIENT
    curTab = drawTabControl(eventCategories, x+R10*sc, y, z + 3.0f, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal);
    newCategory = (int)curTab;
    if (newCategory != currentCategory)
    {
        currentCategory = newCategory;
        turnstileGame_updateEventListView();
    }

    //force selection dictated by the mapserver
    if (sForceQueueEvent[0] != 0 && waitingForData == WAITING_FOR_DATA_DATA_RECIEVED)
    {
        int missionFound = false;
        int i, iSize = eaSize(&turnstileMissions);
        for (i = 0; i < iSize; ++i)
        {
            if (stricmp(turnstileMissions[i]->name, sForceQueueEvent) == 0)
            {
                if (turnstileMissions[i]->category == currentCategory)
                {
                    int j, jSize = eaSize(&eventListView->items);
                    for (j = 0; j < jSize; ++j)
                    {
                        TurnstileMissionClient * mission = (TurnstileMissionClient *) eventListView->items[j];
                        if (stricmp(mission->name, sForceQueueEvent) == 0)
                        {
                            //select if it's not already selected
                            if (!mission->selected)
                            {
                                eventListView->selectedItem = mission;
                                eventListView->newlySelectedItem = 1;
                            }
                            break;
                        }
                    }
                    sForceQueueEvent[0] = 0;
                }
                else
                {
                    //change category
                    uiTabControlSelect(eventCategories, (void*)turnstileMissions[i]->category);
                }
                missionFound = true;
                break;
            }
        }
        if (!missionFound)
        {
            // mission not found, maybe not eligible?
            sForceQueueEvent[0] = 0;
        }
    }
#endif

    if (eventListView)
    {
        static SMFBlock *tsWindowText = NULL;
        if (!tsWindowText)
        {
            tsWindowText = smfBlock_Create();

            smf_SetFlags(tsWindowText,
                SMFEditMode_Unselectable,
                SMFLineBreakMode_MultiLineBreakOnWhitespace,
                0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
        }
        if (OnEndgameRaidMap() || game_state.pendingTSMapXfer)
        {
            smf_SetRawText(tsWindowText, game_state.pendingTSMapXfer ? textStd("TurnstileWaitingStart") : textStd("TurnstileOnEndgameRaidMap"), 0);
            smf_Display(tsWindowText, x + 10*sc, y+20*sc, z, wd-10*sc, 0, 0, 0, &smfTSTextAttr, NULL);
        }
        else if (eaSize(&eventListView->items))
        {
            if (player->pl->lastTurnstileEventID)
            {
                int buttonClick = 0;
                smf_SetRawText(tsWindowText, textStd("TurnstileEventInProgress"), 0);
                smf_Display(tsWindowText, x + 10*sc, y+20*sc, z, wd-10*sc, 0, 0, 0, &smfTSTextAttr, NULL);

                if (drawStdButton(x + wd * 0.25, y + ht - 30.0f * sc, z, sc * 150.0f, sc * 30.0f, 0x00ff00ff, "ReJoinLastStr", 1.0f, waitingForResponse || (waitingForData == WAITING_FOR_DATA_REQUESTING_DATA)) == D_MOUSEHIT)
                {
                    turnstileGame_generateRejoinRequest(1);
                    buttonClick = 1;
                }
                if (drawStdButton(x + wd * 0.75, y + ht - 30.0f * sc, z, sc * 150.0f, sc * 30.0f, 0xff0000ff, "AbandonLastStr", 1.0f, waitingForResponse || buttonClick  || (waitingForData == WAITING_FOR_DATA_REQUESTING_DATA)) == D_MOUSEHIT)
                {
                    turnstileGame_generateRejoinRequest(0);
                    player->pl->lastTurnstileEventID = 0;
                    player->pl->lastTurnstileMissionID = 0;
                }
            }
            else 
            {
                if (waitingForData == WAITING_FOR_DATA_OLD_DATA)
                {
                    smf_SetRawText(tsWindowText, textStd("TurnstileOldEventList"), 0);
                    smf_Display(tsWindowText, x + 10*sc, y+ht-30*sc, z, wd-10*sc, 0, 0, 0, &smfTSTextAttr, NULL);
                    ht -= 20.f*sc;
                    waitingForResponse = 1;
                }

                fullDrawHeight = uiLVGetFullDrawHeight(eventListView);

                if (fullDrawHeight > (windowDrawArea.height-30))
                {
                    doScrollBar(&eventListSb, windowDrawArea.height-30, fullDrawHeight, wd, (R10 + PIX3) * sc, z + 3, 0, &windowDrawArea);
                    eventListView->scrollOffset = eventListSb.offset;
                }
                else
                {
                    eventListView->scrollOffset = 0;
                }

                uiLVSetDrawColor(eventListView, window_GetColor(WDW_TURNSTILE));
                uiLVSetHeight(eventListView, windowDrawArea.height-30);
                uiLVHSetWidth(eventListView->header, windowDrawArea.width);
                uiLVHFinalizeSettings(eventListView->header);

                // How tall and wide is the list view supposed to be?
                clipperPushRestrict(&windowDrawArea);
                uiLVSetScale(eventListView, sc);
                uiLVDisplay(eventListView, pen);
                clipperPop();

                // Can join if and only if we didn't send a join request in the last 15 seconds, and the inQueue flag is clear.
                // The fifteen second delay is to cover round trip to the server to process our join request - the returning ack
                joinMode =  !waitingForResponse && !inQueue;
                uiLVSetClickable(eventListView, joinMode);

                uiLVHandleInput(eventListView, pen);

                if (player->league_id)
                {
                    teamLead = player->league->members.leader == player->db_id;
                    isSolo = 0;
                }
                else if (player->teamup_id)
                {
                    teamLead = player->teamup->members.leader == player->db_id;
                    isSolo = 0;
                }
                else
                {
                    teamLead = 1;
                    isSolo = 1;
                }

                if (joinMode)
                {
                    if (teamLead)
                    {
                        int i;
                        int atLeastOneSelected = 0;
                        if (eventListView->newlySelectedItem)
                        {
                            eventListHandleClick((TurnstileMissionClient *) eventListView->selectedItem);
                        }
                        for(i = eaSize(&turnstileMissions) - 1; i >= 0; i--)
                        {
                            if (turnstileMissions[i]->selected && turnstileMissions[i]->index)
                            {
                                atLeastOneSelected = 1;
                                break;
                            }
                        }
                        if (i < 0)
                        {
                            atLeastOneSelected = 0;
                        }

                        //    draw willing to sub toggle
                        drawTextFlagButton(x+ 20*sc, y+ht-20*sc, z, sc, (wd-40)/2, TS_GROUP_FLAGS_WILLING_TO_SUB, "TSSubString");
                        drawTextFlagButton(x+ wd/2, y+ht-20*sc, z, sc, (wd-40)/2, TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP, "TSOwnString");
                        if (drawStdButton(x + wd * 0.5, y + ht - 50.0f * sc, z, sc * 150.0f, sc * 30.0f, 0x00ff00ff, "JoinQueueStr", 1.0f, waitingForResponse || !atLeastOneSelected || (waitingForData == WAITING_FOR_DATA_REQUESTING_DATA)) == D_MOUSEHIT)
                        {
                            // They hit "Join Queue".

                            int eventList[1024];
                            int eventCount = 0;

                            // run thru the missions, adding all numbers of selected missions to the event list
                            for(i = eaSize(&turnstileMissions) - 1; i >= 0; i--)
                            {
                                if (turnstileMissions[i]->selected && turnstileMissions[i]->index)
                                {
                                    eventList[eventCount++] = turnstileMissions[i]->index - 1;
                                }
                            }

                            if (eventCount)
                            {
                                // If we found anything, send the request.
                                turnstileGame_generateQueueForEvents(eventList, eventCount);
                            }
                        }
                    }
                    else
                    {
                        drawStdButton(x + wd * 0.5, y + ht - 30.0f * sc, z, sc * 150.0f, sc * 30.0f, color, "CantJoinQueueStr", 1.0f, BUTTON_LOCKED);
                    }
                }
                else
                {
                    if (drawStdButton(x + wd * 0.5, y + ht - 30.0f * sc, z, sc * 150.0f, sc * 30.0f, 0xff0000ff, "LeaveQueueStr", 1.0f, waitingForResponse || (waitingForData == WAITING_FOR_DATA_REQUESTING_DATA)) == D_MOUSEHIT)
                    {
                        if (teamLead)
                        {
                            strncpyt(leaveText, textStd(isSolo ? "LeaveQueueSoloStr" : (player->league_id ? "LeaveQueueLeagueLeaderStr" : "LeaveQueueTeamLeaderStr")), sizeof(leaveText));
                        }
                        else
                        {
                            strncpyt(leaveText, textStd(player->league_id ? "LeaveQueueLeagueStr" : "LeaveQueueTeamStr"), sizeof(leaveText));
                        }
                        // Make a local copy of textStd's output, since turnstileDialogWindowActivate also calls it and therefore could
                        // damage the current output.
                        strncpyt(confirmText, textStd("ConfirmLeaveQueueStr"), sizeof(confirmText));

                        turnstileDialogWindowActivate(NULL, leaveText, confirmText, 0, "LeaveQueueStr", "CancelLeaveStr",
                            ConfirmLeaveQueue, NULL, NULL, NULL, 0);
                    }
                }
                // Unselect the selected item, so it doesn't highlight
                eventListView->selectedItem = NULL;
                eventListView->mouseOverItem = NULL;
                eventListView->newlySelectedItem = 0;
            }
        }
        else
        {
            smf_SetRawText(tsWindowText, textStd("TurnstileNoValidEvents"), 0);
            smf_Display(tsWindowText, x + 10*sc, y+40*sc, z, wd-10*sc, 0, 0, 0, &smfTSTextAttr, NULL);
        }
    }
    return 0;
}

static int dialogTitle = 0;
static int dialogText = 0;
static int dialogFooter = 0;
static int dialogTimeout = 0;
static int dialogLastDisplayedTime = 0;
static char *dialogYesButton = NULL;
static char *dialogNoButton = NULL;
static void (*dialogYesFunc)(void *);
static void *dialogYesData;
static void (*dialogNoFunc)(void *);
static void *dialogNoData;

static SMFBlock *smfTitle = NULL;
static SMFBlock *smfText = NULL;
static SMFBlock *smfFooter = NULL;
static SMFBlock *smfTimeout = NULL;

static TextAttribs smfTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0,
    /* piScale       */  (int *) (int) (SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

int turnstileDialogWindow()
{
    float x, y, z, wd, ht, sc;
    float ypos;
    int w;
    int color, bcolor;
    int mode;
    char timeoutText[256];

    x = 0;
    y = 0;
    z = 0;
    wd = 0;
    sc = 1.0f;

    if (!window_getDims(WDW_TURNSTILE_DIALOG, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        return 0;
    }
    if (!inQueue)
    {
        window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_SHRINKING);
    }

    // We should never arrive here with smfTitle == NULL, but in the unlikely event we do, nuke the window
    if (smfTitle == NULL)
    {
        mode = window_getMode(WDW_TURNSTILE_DIALOG);
        if (mode != WINDOW_SHRINKING && mode != WINDOW_DOCKED)
        {
            window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_SHRINKING);
        }
        return 0;
    }

    if (mainWindowEscrowedState == WINDOW_UNINITIALIZED)
    {
        mode = window_getMode(WDW_TURNSTILE);
        if (mode == WINDOW_SHRINKING || mode == WINDOW_DOCKED)
        { 
            mainWindowEscrowedState = WINDOW_DOCKED;
        }
        else
        {
            mainWindowEscrowedState = WINDOW_DISPLAYING;
            window_setMode(WDW_TURNSTILE, WINDOW_SHRINKING);
        }
    }

    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    ypos = (int) (y + 10.0f * sc);
    w = (int) (wd - 20.0f * sc);

    // Title
    if (dialogTitle)
    {
        smfTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE * sc * 1.2f));
        ypos += smf_Display(smfTitle, x + 10, ypos, z, w, 0, 0, 0, &smfTextAttr, NULL) + 8.0f * sc;
    }

    // Body
    if (dialogText)
    {
        smfTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE * sc));
        ypos += smf_Display(smfText, x + 10, ypos, z, w, 0, 0, 0, &smfTextAttr, NULL) + 8.0f * sc;
    }

    // Footer
    if (dialogFooter)
    {
        smfTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE * sc * 1.2f));
        ypos += smf_Display(smfFooter, x + 10, ypos, z, w, 0, 0, 0, &smfTextAttr, NULL) + 8.0f * sc;
    }

    // Timer
    if (dialogTimeout)
    {
        if (dialogLastDisplayedTime != timerSecondsSince2000())
        {
            int tenSecondWarning = dialogLastDisplayedTime;
            dialogLastDisplayedTime = timerSecondsSince2000();
            if (((dialogTimeout-tenSecondWarning) > 10) && ((dialogTimeout-dialogLastDisplayedTime) <= 10))
            {
                sndPlay("IT_Warning_01", SOUND_GAME);
            }
            _snprintf(timeoutText, sizeof(timeoutText), "%s", textStd("LFGSecondsRemaining", dialogTimeout - dialogLastDisplayedTime));
            smf_SetRawText(smfTimeout, timeoutText, 0);
        }

        smfTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE * sc));
        ypos += smf_Display(smfTimeout, x + 10, ypos, z, w, 0, 0, 0, &smfTextAttr, NULL) + 8.0f * sc;

        if (dialogLastDisplayedTime >= dialogTimeout)
        {
            // Timeout - handle a negative response
            if (dialogNoFunc)
            {
                (* dialogNoFunc)(dialogNoData);
            }
            window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_SHRINKING);
            if (mainWindowEscrowedState == WINDOW_DISPLAYING)
            {
                window_setMode(WDW_TURNSTILE, WINDOW_GROWING);
            }
            mainWindowEscrowedState = WINDOW_UNINITIALIZED;
        }
    }

    if (dialogYesButton)
    {
        if (drawStdButton(x + wd * (3.0f / 12.0f), ypos + 20.0f * sc, z, wd * (1.0f / 3.0f), sc * 30.0f, 0x00ff00ff, dialogYesButton, 1.0f, 0) == D_MOUSEHIT)
        {
            if (dialogYesFunc)
            {
                (* dialogYesFunc)(dialogYesData);
            }
            window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_SHRINKING);
            if (mainWindowEscrowedState == WINDOW_DISPLAYING)
            {
                window_setMode(WDW_TURNSTILE, WINDOW_GROWING);
            }
            mainWindowEscrowedState = WINDOW_UNINITIALIZED;
        }
    }

    if (dialogNoButton)
    {
        if (drawStdButton(x + wd * (9.0f / 12.0f), ypos + 20.0f * sc, z, wd * (1.0f / 3.0f), sc * 30.0f, 0xff0000ff, dialogNoButton, 1.0f, 0) == D_MOUSEHIT)
        {
            if (dialogNoFunc)
            {
                (* dialogNoFunc)(dialogNoData);
            }
            window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_SHRINKING);
            if (mainWindowEscrowedState == WINDOW_DISPLAYING)
            {
                window_setMode(WDW_TURNSTILE, WINDOW_GROWING);
            }
            mainWindowEscrowedState = WINDOW_UNINITIALIZED;
        }
    }

    if (windowUp(WDW_TURNSTILE_DIALOG))
    {
        window_setDims(WDW_TURNSTILE_DIALOG, -1.0f, -1.0f, -1.0f, ypos - y + 50.0f * sc);
    }

    return 0;
}

// This is somewhat a singleshot case of the standard dialog system.  However there's a little functionality I want here that isn't present there,
// so I roll my own.
// Note that it's a bit inconsistent vis a vis where we do localization.  I'd like to do it all in here, but the title, body and footer are variable
// and would therefore require passing in some sort of varargs trickery.  That in turn leads to the problem of , ... only allowing one set of variable
// parameters, while we would have up to three separate calls.  Therefore the top three strings are localized by the caller, while the two button
// titles are localized here.
void turnstileDialogWindowActivate(char *title, char *text, char *footer, int seconds,
                                   char *yesButton, char *noButton,
                                   void (*yesFunc)(void *), void *yesData,
                                   void (*noFunc)(void *), void *noData,
                                   int overRide)
{
    static int init = 0;

    if (!init)
    {
        smfTitle = smfBlock_Create();
        smfText = smfBlock_Create();
        smfFooter = smfBlock_Create();
        smfTimeout = smfBlock_Create();

        smf_SetFlags(smfTitle,
                        SMFEditMode_Unselectable,
                        SMFLineBreakMode_MultiLineBreakOnWhitespace,
                        0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);

        smf_SetFlags(smfText,
                        SMFEditMode_Unselectable,
                        SMFLineBreakMode_MultiLineBreakOnWhitespace,
                        0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);

        smf_SetFlags(smfFooter,
                        SMFEditMode_Unselectable,
                        SMFLineBreakMode_MultiLineBreakOnWhitespace,
                        0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);

        smf_SetFlags(smfTimeout,
                        SMFEditMode_Unselectable,
                        SMFLineBreakMode_MultiLineBreakOnWhitespace,
                        0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);

        init = true;
    }

    if (overRide || !windowUp(WDW_TURNSTILE_DIALOG))
    {
        if (dialogTitle = title != NULL)
        {
            smf_SetRawText(smfTitle, title, 0);
        }

        if (dialogText = text != NULL)
        {
            smf_SetRawText(smfText, text, 0);
        }

        if (dialogFooter = footer != NULL)
        {
            smf_SetRawText(smfFooter, footer, 0);
        }

        dialogTimeout = seconds ? timerSecondsSince2000() + seconds : 0;
        dialogLastDisplayedTime = 0;

        if (dialogYesButton)
        {
            free(dialogYesButton);
        }
        dialogYesButton = strdup(textStd(yesButton));

        if (dialogNoButton)
        {
            free(dialogNoButton);
        }
        dialogNoButton = strdup(textStd(noButton));

        dialogYesFunc = yesFunc;
        dialogYesData = yesData;
        dialogNoFunc = noFunc;
        dialogNoData = noData;

        if (!windowUp(WDW_TURNSTILE_DIALOG))
        {
            window_setMode(WDW_TURNSTILE_DIALOG, WINDOW_GROWING);
        }
    }
}


