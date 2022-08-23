/*\
 *
 *    ArenaGame.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *         Confidential property of Cryptic Studios
 *
 *    Game client logic for arena events.  Mainly networking stuff
 *
 */

#include "bases/DetailRecipe.h"
#include "arenagame.h"
#include "UI/uiConsole.h"
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include "player/player.h"
#include "entity/entPlayer.h"
#include "UI/uiArenaList.h"
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiArena.h"
#include "UI/uiArenaResult.h"
#include <utilitieslib/components/EString.h>
#include "sound/sound.h"
#include "UI/uiArenaJoin.h"
#include <utilitieslib/utils/timing.h>
#include "player/pmotion.h"
#include "UI/uiDialog.h"
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "gameData/arenastruct.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiInfo.h"
#include "UI/uiChat.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "clientcomm/clientcommreceive.h"
#include "UI/uiNet.h"

ArenaEventList g_kiosk;
ArenaEvent* g_event; // for now, just the last event we received full details about...

bool arenaCountdown = false; // should I be displaying the countdown?
unsigned long arenaCountdownMomentInitial = 0.0; // the (unknown big number) that defines the moment that the countdown began.
int arenaCountdownTimerInitial = 0; // the number of seconds that the timer should countdown in total (the current time in the countdown at the moment the countdown began.)
int arenaCountdownTimerCurrent = 0; // the current time in the countdown (this is the big red number)

char* g_arenastring;
U32 g_arena_run_phase_end_time = 0;
U32 g_arena_run_phase_length = 0;
U32 g_arena_respawn_period = 0;
int g_playstatsloop = 0;
int g_onarenamap = 0;
int g_arena_stock = 0;
int g_arena_kills = 0;
int g_resurrect_allowed = 0;
int g_display_exit_button = 0;

int g_eventid = 0;

void DebugPrintKiosk(void)
{
    int i, n;

    conPrintf("events:");
    n = eaSize(&g_kiosk.events);
    for (i = 0; i < n; i++)
    {
        conPrintf("\t%i %s", g_kiosk.events[i]->eventid, g_kiosk.events[i]->description);
    }
    conPrintf("--end--");
}

void receiveArenaKiosk(Packet *pak)
{
    int ok = pktGetBits(pak, 1);
    if (ok)
    {
        int count = 0;
        int initial = pktGetBits(pak, 1);
        if (initial)
        {
            ArenaEventListDestroyContents(&g_kiosk);
            eaCreate(&g_kiosk.events);
        }
        while (pktGetBits(pak, 1)) // have additional data
        {
            ArenaEvent* event = ArenaEventCreate();
            ArenaEventReceive(pak, event);
            eaPush(&g_kiosk.events, event);
            count++;
        }
        arenaRebuildListView();
    }
    else
    {
        addSystemChatMsg( textStd("NoArenaServer"), INFO_USER_ERROR, 0 );
    }
}

void receiveArenaKioskOpen(Packet* pak)
{
    ArenaKioskType type;
    int open;

     open = pktGetBits(pak, 1);
    if (open)
    {
        type = pktGetBits(pak, ARENAKIOSK_NUMBITS);
        window_setMode( WDW_ARENA_LIST, WINDOW_GROWING );
        arenaListSelectTab( ARENA_TAB_ALL ); //arena select tab now requests the arena kiosk on its own
        gArenaInvited = false;
    }
    else
    {
        window_setMode( WDW_ARENA_LIST, WINDOW_SHRINKING );
    }
}

ArenaEventList* getArenaKiosk(void)
{
    return &g_kiosk;
}

// received a detail update from the mapserver
void receiveArenaUpdate(Packet* pak)
{
    int updateDetail = 0;
    int haveEvent = 0;

    // the server will only allow us to be in a single Active event
    if (!g_event)
        g_event = ArenaEventCreate();

    haveEvent = pktGetBits(pak, 1);
    if (!haveEvent)
    {
        ArenaEventDestroyContents(g_event);
        ArenaRefZero(g_event);
        window_setMode(WDW_ARENA_CREATE, WINDOW_SHRINKING);
        window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
        ArenaRefZero(&gArenaDetail);
        return;
    }

    pktGetBitsPack(pak, 1); // event id.. don't need
     pktGetBitsPack(pak, 1); // unique id..
    ArenaEventReceive(pak, g_event);

    // notice if this update was in response to a request of mine
    if (playerPtr() && g_event->lastupdate_dbid == playerPtr()->db_id)
    {
        switch (g_event->lastupdate_cmd)
        {
        case CLIENTINP_ARENA_REQ_CREATE:
        case CLIENTINP_ARENA_REQ_JOIN:
            if( g_event->phase == ARENA_NEGOTIATING)
            {
                // open the create screen
                window_setMode(WDW_ARENA_CREATE, WINDOW_GROWING);
                window_setMode(WDW_ARENA_LIST, WINDOW_SHRINKING);
                window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
            }
//            else
//                addArenaChatMsg( "ScheduledEvent", INFO_ARENA_ERROR );
            updateDetail = 1;
            break;
        case CLIENTINP_ARENA_REQ_JOIN_INVITE:
             if( g_event->phase != ARENA_SCHEDULED)
            {
                window_setMode(WDW_ARENA_CREATE, WINDOW_GROWING);
                window_setMode(WDW_ARENA_LIST, WINDOW_SHRINKING);
                window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
            }
            updateDetail = 1;
            break;
        case CLIENTINP_ARENA_DROP:
            window_setMode(WDW_ARENA_CREATE, WINDOW_SHRINKING);
            window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
            break;
        }
    }

    // whether it was miy doing or not execute these
    switch (g_event->lastupdate_cmd)
    {
        case CLIENTINP_ARENA_REQ_JOIN:
        case CLIENTINP_ARENA_REQ_JOIN_INVITE:
            {
                if( g_event->lastupdate_name[0] )
                    addArenaChatMsg( textStd("ArenaMemberJoined", g_event->lastupdate_name), INFO_ARENA_ERROR );
            }break;
        case CLIENTINP_ARENA_DROP:
            {
                if( g_event->lastupdate_name[0] )
                    addArenaChatMsg( textStd("ArenaMemberLeft", g_event->lastupdate_name), INFO_ARENA_ERROR );
            }break;
        default:
            break;
    }

    if (g_event->lastupdate_cmd == CLIENTINP_ARENA_REQ_CREATE)
    {
        // I just created it, push my last event's information back to the server
        g_event->teamStyle = gArenaDetail.teamStyle;
        g_event->teamType = gArenaDetail.teamType;
        g_event->weightClass = gArenaDetail.weightClass;
        g_event->victoryType = gArenaDetail.victoryType;
        g_event->victoryValue = gArenaDetail.victoryValue;
        g_event->tournamentType = gArenaDetail.tournamentType;
        g_event->verify_sanctioned = gArenaDetail.verify_sanctioned;
        g_event->no_observe = gArenaDetail.no_observe;
        g_event->numsides = gArenaDetail.numsides;
        g_event->maxplayers = gArenaDetail.maxplayers;
        g_event->tactical_start = gArenaDetail.tactical_start;
        g_event->no_end = gArenaDetail.no_end;
        g_event->no_pool = gArenaDetail.no_pool;
        g_event->no_travel = gArenaDetail.no_travel;
        g_event->no_stealth = gArenaDetail.no_stealth;
        g_event->no_travel_suppression = gArenaDetail.no_travel_suppression;
        g_event->no_diminishing_returns = gArenaDetail.no_diminishing_returns;
        g_event->no_heal_decay = gArenaDetail.no_heal_decay;
        g_event->inspiration_setting = gArenaDetail.inspiration_setting;
        g_event->enable_temp_powers = gArenaDetail.enable_temp_powers;
        ArenaEventCopy(&gArenaDetail, g_event);
        arenaApplyEventToUI();
        arenaSendCreatorUpdate(&gArenaDetail);
    }
    else
    {
        ArenaEventCopy(&gArenaDetail, g_event);
        arenaApplyEventToUI();
    }
}



// debug function to print event details
void receiveArenaEvents(Packet* pak)
{
    char* estr;
    int i, n;
    
    n = pktGetBitsPack(pak, 1);
    conPrintf("You belong to %i events", n);
    estrCreate(&estr);
    for (i = 0; i < n; i++)
    {
        ArenaEvent* ae = ArenaEventCreate();
        ArenaEventReceive(pak, ae);
        ArenaEventDebugPrint(ae, &estr, timerSecondsSince2000WithDelta());
        conPrintf("%s", estr);
        ArenaEventDestroy(ae);
    }
    estrDestroy(&estr);
}

void receiveArenaUpdateTrayDisable(Packet* pak)
{
    g_display_exit_button = 1;
}

void receiveServerRunEventWindow(Packet* pak)
{
    if(!g_event)
        g_event = ArenaEventCreate();
    ArenaEventReceive(pak, g_event);
    window_setMode(WDW_ARENA_CREATE, WINDOW_GROWING);
    window_setMode(WDW_ARENA_LIST, WINDOW_SHRINKING);
    window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
}

void receiveArenaFinishNotification(Packet *pak)
{
    g_playstatsloop = 1;
    window_setMode(WDW_ARENA_RESULT, WINDOW_GROWING);
}

void requestArenaKiosk(int radioButtonFilter,int tabFilter)
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_REQKIOSK);
    pktSendBitsPack(pak, 1, radioButtonFilter);
    pktSendBitsPack(pak, 1, tabFilter);
    END_INPUT_PACKET
    ArenaEventListDestroyContents(&g_kiosk);
    eaCreate(&g_kiosk.events);
    arenaRebuildListView();
}

static void requestArenaTeleport(void *data)
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_REQ_TELEPORT);
    pktSendBitsPack(pak, 1, g_eventid);
    END_INPUT_PACKET
}

void receiveArenaResults(Packet * pak) {
    ArenaRankingTable *art = ArenaRankingTableCreate();
    ArenaRankingTableReceive(pak,art);
    arenaRebuildResultView(art);
    ArenaRankingTableDestroy(art);
}

void arenaStartCountdown(int timer)
{
    arenaCountdown = true;
    arenaCountdownMomentInitial = timerCpuTicks();
    arenaCountdownTimerInitial = arenaCountdownTimerCurrent = timer;
}

void receiveArenaStartCountdown(Packet *pak)
{
    int timer = pktGetBitsPack(pak, 1);

    if(timer)
    {
        arenaStartCountdown(timer);
    }
}

void receiveArenaCompassString(Packet *pak)
{
    g_onarenamap = 1;
    estrDestroy(&g_arenastring);
    estrCreate(&g_arenastring);
    estrConcatCharString(&g_arenastring,pktGetString(pak));
    g_arena_run_phase_end_time = timerSecondsSince2000() + pktGetBitsAuto(pak);
    g_arena_run_phase_length = pktGetBitsAuto(pak);
    g_arena_respawn_period = pktGetBitsAuto(pak);
    g_display_exit_button = 1;
}

void receiveArenaVictoryInformation(Packet *pak)
{
    g_arena_stock = pktGetBitsPack(pak, 1);
    g_arena_kills = pktGetBitsPack(pak, 1);
    g_resurrect_allowed = pktGetBitsPack(pak, 1);

    if (g_arena_stock < 0)
        g_arena_stock = 0;

    if (g_arena_kills < 0)
        g_arena_kills = 0;
}

void receiveArenaScheduledTeleport(Packet *pak)
{
    int onarenamap;
    int eventid;

    onarenamap = pktGetBits(pak, 1);
    eventid = pktGetBitsPack(pak,1);

    if(eventid == g_eventid)
        return;

    g_eventid = eventid;

    if(onarenamap)
        dialogTimed(DIALOG_YES_NO, "DropAndTeleport", "TeleportYes", "StayCurrentEvent", requestArenaTeleport, NULL, 0, ARENA_SCHEDULED_TELEPORT_WAIT-1);
    else
    {
        if(playerPtr()->pl->taskforce_mode)
            dialogTimed(DIALOG_YES_NO, "TeleportToArenaTaskforce", "TeleportYes", "DropEventNow", requestArenaTeleport, NULL, 0, ARENA_SCHEDULED_TELEPORT_WAIT-1);
        else
            dialogTimed(DIALOG_YES_NO, "TeleportToArena", "TeleportYes", "DropEventNow", requestArenaTeleport, NULL, 0, ARENA_SCHEDULED_TELEPORT_WAIT-1);
    }
}

char* getArenaCompassString()
{
    if(g_arenastring)
        return g_arenastring;
    else
        return NULL;
}

char* getArenaInfoString()
{
    U32 curtime = timerSecondsSince2000();
    static char buf[20];
    if (g_arena_run_phase_end_time && (g_arena_run_phase_end_time > curtime))
    {
        int diff = g_arena_run_phase_end_time - curtime;
        timerMakeOffsetStringFromSeconds(buf, diff);
        if (diff <= 25 && !arenaCountdown)
        {
            arenaStartCountdown(diff);
        }
    }
    else if (g_arena_stock)
    {
        sprintf(buf, "%d", g_arena_stock);
    }
    else if (g_arena_kills)
    {
        sprintf(buf, "%d", g_arena_kills);
    }
    else
    {
        *buf = 0;
    }
    return buf;
}

char* getArenaRespawnTimerString()
{
    U32 curtime = timerSecondsSince2000();
    static char buf[20];
    if (g_arena_respawn_period && g_arena_run_phase_end_time)
    {
        timerMakeOffsetStringFromSeconds(buf, g_arena_respawn_period - (curtime - (g_arena_run_phase_end_time - g_arena_run_phase_length)) % g_arena_respawn_period);
    }
    else
    {
        *buf = 0;
    }
    return buf;
}

int getArenaStockLives()
{
    return g_arena_stock || g_resurrect_allowed;
}

void playArenaStatsLoop()
{
    if(g_playstatsloop)
        sndPlay("stats_loop", SOUND_MUSIC);
}

void resetArenaVars()
{
    g_playstatsloop = 0;
    estrDestroy(&g_arenastring);
    g_arena_run_phase_end_time = 0;
    g_arena_run_phase_length = 0;
    g_arena_respawn_period = 0;
    g_onarenamap = 0;
    g_arena_stock = 0;
    g_arena_kills = 0;
    g_resurrect_allowed = 0;
    g_display_exit_button = 0;
    leftArenaLobby();
}

int amOnArenaMap()
{
    return g_onarenamap;
}

int amOnPetArenaMap()
{
    if( g_onarenamap &&    g_event && g_event->use_gladiators )
        return 1;

    //DEBUG if you have any arena pets in the regular world, pretend like this is an arena map
    if( game_state.pretendToBeOnArenaMap )
        return 1;

    return 0;
}

int arenaAllowReturnStaticMap()
{
    Entity* e = playerPtr();
    return g_onarenamap && entMode(e, ENT_DEAD);
}

int activeEvent(void)
{
    return (g_event && g_event->eventid);
}

// I think this is a bad way of doing things, but for the record:
// requester == 0: put only the rated info in a dialog box
// requester == 1: put only the rated info in the arena_tab of the info window
// requester == 2: put all the info except the rated in a dialog box
// otherwise     : do not display the info
void handleClientArenaPlayerUpdate(Packet * pak) {
    ArenaPlayer * ap=ArenaPlayerCreate();
    int requester=pktGetBitsPack(pak,1);
    int rating,provisional,wins,losses,draws;

    ArenaPlayerReceive(pak,ap);

    if (requester == 0 || requester == 1)
    {
        char text[2048];
        char temp[2048];
        
        text[0]='\0';
        sprintf(temp,"<span align=center>Rated Matches:</span><br>");
        strcat(text,temp);
        sprintf(temp,"<TABLE><TR>");
        strcat(text,temp);
        sprintf(temp,"<TD></TD>");
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("RatingString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("WinsString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("LossesString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("DrawsString"));
        strcat(text,temp);
        sprintf(temp,"</TR>");
        strcat(text,temp);

        //duels
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("DuelString"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);




        //battle royale
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("BattleRoyale"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);




        //star
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_STAR,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_STAR,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_STAR,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_STAR,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_STAR,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("5TARString"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);




        //team duel
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("TeamDuel"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);




        //team battle royale
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,3,0,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("TeamBattleRoyale"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);




        //supergroup rumble
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SUPERGROUP,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SUPERGROUP,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SUPERGROUP,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SUPERGROUP,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SUPERGROUP,ARENA_TOURNAMENT_NONE,2,0,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("SupergroupRumble"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);

        //Gladiator
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_SINGLE,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("GladiatorSingle"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);

        //Gladiator
        sprintf(temp,"<TR>");
        strcat(text,temp);
        rating        =ap->ratingsByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        provisional    =ap->provisionalByRatingIndex    [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        wins        =ap->winsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        losses        =ap->lossesByRatingIndex        [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        draws        =ap->drawsByRatingIndex            [EventGetRatingIndex(ARENA_FREEFORM,ARENA_TEAM,ARENA_TOURNAMENT_NONE,2,1,NULL)];
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("GladiatorTeam"));
        strcat(text,temp);
        if (provisional)
            sprintf(temp,"<TD align=right><color gray>");
        else
            sprintf(temp,"<TD align=right><color white>");
        strcat(text,temp);
        sprintf(temp,"%d</color></TD>",rating);
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",wins,losses,draws);
        strcat(text,temp);


        //overall
        sprintf(temp,"<TR>");
        strcat(text,temp);
        {
            int i;
            rating        =0;
            wins        =0;
            losses        =0;
            draws        =0;
            for (i=0;i<ARENA_NUM_RATINGS;i++) {
                rating        +=ap->ratingsByRatingIndex[i]*(ap->winsByRatingIndex[i]+ap->lossesByRatingIndex[i]+ap->drawsByRatingIndex[i]);
                wins        +=ap->winsByRatingIndex            [i];
                losses        +=ap->lossesByRatingIndex        [i];
                draws        +=ap->drawsByRatingIndex        [i];
            }
            if (wins+losses+draws!=0)
                rating/=(wins+losses+draws);
            else
                rating=0;
        }
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("OverallString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",rating,wins,losses,draws);
        strcat(text,temp);




        sprintf(temp,"</TABLE>");
        strcat(text,temp);

        if (requester==1)
            info_replaceTab(textStd("ArenaTabString"),text,-1);
        if (requester==0)
            dialogStd(DIALOG_OK,text,NULL,NULL,NULL,NULL,0);
    }
    else if (requester == 2)
    {
        char text[10240];
        char temp[2048];
        
        text[0]='\0';
        sprintf(temp,"<TABLE><TR>");
        strcat(text,temp);
        sprintf(temp,"<TD></TD>");
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("WinsString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("LossesString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right><color paragon>%s</color></TD>",textStd("DrawsString"));
        strcat(text,temp);
        sprintf(temp,"</TR>");
        strcat(text,temp);

        // overall
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("OverallString"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->wins,ap->losses,ap->draws);
        strcat(text,temp);

        // event type
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD></TR>",textStd("ArenaEventType"));
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeDuel"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[0],ap->lossesByEventType[0],ap->drawsByEventType[0]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeSwissDraw"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[1],ap->lossesByEventType[1],ap->drawsByEventType[1]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeFreeForAll"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[2],ap->lossesByEventType[2],ap->drawsByEventType[2]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeStar"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[3],ap->lossesByEventType[3],ap->drawsByEventType[3]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeVillainStar"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[4],ap->lossesByEventType[4],ap->drawsByEventType[4]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeVersusStar"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[5],ap->lossesByEventType[5],ap->drawsByEventType[5]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventType7Star"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[6],ap->lossesByEventType[6],ap->drawsByEventType[6]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeVillain7Star"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[7],ap->lossesByEventType[7],ap->drawsByEventType[7]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeVersus7Star"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[8],ap->lossesByEventType[8],ap->drawsByEventType[8]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaEventTypeGladiator"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByEventType[9],ap->lossesByEventType[9],ap->drawsByEventType[9]);
        strcat(text,temp);

        // team type
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD></TR>",textStd("ArenaTeamType"));
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaTeamTypeSolo"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByTeamType[0],ap->lossesByTeamType[0],ap->drawsByTeamType[0]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaTeamTypeTeam"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByTeamType[1],ap->lossesByTeamType[1],ap->drawsByTeamType[1]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaTeamTypeSupergroup"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByTeamType[2],ap->lossesByTeamType[2],ap->drawsByTeamType[2]);
        strcat(text,temp);

        // victory type
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD></TR>",textStd("ArenaVictoryType"));
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaTimed"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByVictoryType[0],ap->lossesByVictoryType[0],ap->drawsByVictoryType[0]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaTeamStock"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByVictoryType[1],ap->lossesByVictoryType[1],ap->drawsByVictoryType[1]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("LastManStanding"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByVictoryType[2],ap->lossesByVictoryType[2],ap->drawsByVictoryType[2]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaKills"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByVictoryType[3],ap->lossesByVictoryType[3],ap->drawsByVictoryType[3]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("ArenaStock"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByVictoryType[4],ap->lossesByVictoryType[4],ap->drawsByVictoryType[4]);
        strcat(text,temp);

        // weight class
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD></TR>",textStd("ArenaWeightClass"));
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("AnyLevel"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[0],ap->lossesByWeightClass[0],ap->drawsByWeightClass[0]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("StrawWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[1],ap->lossesByWeightClass[1],ap->drawsByWeightClass[1]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("FlyWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[2],ap->lossesByWeightClass[2],ap->drawsByWeightClass[2]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("BantamWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[3],ap->lossesByWeightClass[3],ap->drawsByWeightClass[3]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("FeatherWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[4],ap->lossesByWeightClass[4],ap->drawsByWeightClass[4]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("LightWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[5],ap->lossesByWeightClass[5],ap->drawsByWeightClass[5]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("WelterWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[6],ap->lossesByWeightClass[6],ap->drawsByWeightClass[6]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("MiddleWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[7],ap->lossesByWeightClass[7],ap->drawsByWeightClass[7]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("CruiserWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[8],ap->lossesByWeightClass[8],ap->drawsByWeightClass[8]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("HeavyWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[9],ap->lossesByWeightClass[9],ap->drawsByWeightClass[9]);
        strcat(text,temp);
        sprintf(temp,"<TR>");
        strcat(text,temp);
        sprintf(temp,"<TD align=left><color paragon>%s</color></TD>",textStd("SuperHeavyWeight"));
        strcat(text,temp);
        sprintf(temp,"<TD align=right>%d</TD><TD align=right>%d</TD><TD align=right>%d</TD></TR>",ap->winsByWeightClass[10],ap->lossesByWeightClass[10],ap->drawsByWeightClass[10]);
        strcat(text,temp);

        sprintf(temp,"</TABLE>");
        strcat(text,temp);

        dialogStd(DIALOG_OK,text,NULL,NULL,NULL,NULL,0);
    }
}