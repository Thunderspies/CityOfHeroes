/*\
 *
 *    arenamap.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Code to run an instanced arena map.
 *
 */

#include "arenamapserver.h"
#include <utilitieslib/components/EString.h>
#include "arenamap.h"
#include "dbcomm/dbcomm.h"
#include "comm_game.h"
#include "svr/svr_base.h"
#include "svr/svr_player.h"
#include <utilitieslib/utils/error.h>
#include "stdio.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"
#include "entity/character_combat.h"
#include "entity/character_tick.h"
#include "entity/entGameActions.h"
#include "entity/entserver.h"
#include "entity/entPlayer.h"
#include "container/team.h"
#include "dbcomm/dbdoor.h"
#include "entity/powers.h"
#include "entity/PowerInfo.h"
#include "entity/character_animfx.h"
#include "player/playerState.h"
#include <utilitieslib/utils/timing.h>
#include "player/parseClientInput.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "beacon/beaconPrivate.h"
#include "gridcoll/gridcoll.h"
#include "entity/character_target.h"
#include "petarena.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/log.h>

#define ARENAMAP_FINISH_TIME_NORMAL    300
#define ARENAMAP_FINISH_TIME_MULTIROUND    60
#define ARENAMAP_CLIENT_MIN_WAIT    15
#define ARENAMAP_CLIENT_MAX_WAIT    60
#define ARENA_SUDDENDEATH_TIME        60

#define ARENAMAP_SPACED_OUT_START    15    // apparently the first 15 spawns on a map are spread out well

ArenaEvent* g_arenamap_event;
U32 g_arenamap_eventid = 0;
int g_arenamap_seat = 0;
int g_arenamap_phase = ARENAMAP_INIT; // The current ArenaMapPhase
U32 start_time;
U32 phase_time; // The time that the current ArenaMapPhase began

U32 g_matchstarttime;

// translate from 'event side' to 'map side', weird to do this but then we can talk
// about the 1st and 2nd teams on the map, and not deal with the 126 other teams on other maps
U32 g_mapsides[ARENA_MAX_SIDES+1] = {0};
U32 g_mapside_rotate[ARENA_MAX_SIDES+1][2] = {0};
U32 g_last_mapside = 0;

// results of match thus far
int g_scores[ARENA_MAX_SIDES+1] = {0};
int g_is_still_playing[ARENA_MAX_SIDES+1] = {0};
int g_winningmapside = 0;

int g_team_lives_stock[ARENA_MAX_SIDES+1] = {0};
int g_use_team_stock = 0;

int GetMapSide(int eventside)
{
    if (!g_mapsides[eventside])
        g_mapsides[eventside] = ++g_last_mapside;
    return g_mapsides[eventside];
}

int GetEventSide(int mapside)
{
    int i;
    for (i = 1; i <= ARENA_MAX_SIDES; i++)
    {
        if (g_mapsides[i] == mapside)
            return i;
    }
    return 0;
}

void ArenaSetInfoString(char* info)
{
    int eventid, seat;
    if (!ArenaDeconstructInfoString(info, &eventid, &seat))
    {
        LOG_OLD_ERR( "arenadoor ArenaSetInfoString, got bad info %s", info);
        dbAsyncReturnAllToStaticMap("ArenaSetInfoString() bad info",info);
        return;
    }

    if (!ArenaSyncConnect())
    {
        g_arenamap_eventid = 0;
        LOG_OLD_ERR( "arenadoor ArenaSetInfoString, couldn't connect to arena server");
        dbAsyncReturnAllToStaticMap("ArenaSetInfoString() no connect",info);
        return;
    }
    
    phase_time = start_time = dbSecondsSince2000();
    g_arenamap_eventid = eventid;
    g_arenamap_seat = seat;
    g_arenamap_event = ArenaEventCreate();
    g_arenamap_event->eventid = eventid;
    ArenaSyncUpdateEvent(g_arenamap_event);

    // set correct PvP settings here!
    server_state.enablePowerDiminishing = !g_arenamap_event->no_diminishing_returns;
    server_state.enableTravelSuppressionPower = !g_arenamap_event->no_travel_suppression;
    server_state.enableHealDecay = !g_arenamap_event->no_heal_decay;
    server_state.enablePassiveResists = true;
    server_state.enableSpikeSuppression = false;
    server_state.inspirationSetting = g_arenamap_event->inspiration_setting;

    // set to go..

}

int OnArenaMap(void)
{
    return g_arenamap_eventid? 1: 0;
}

/**********************************************************************func*
* character_RemoveAllInspirations
*
*/
void ArenaGiveStdInspirations(Entity* e)
{
    character_RemoveAllInspirations(e->pchar, "arena");
    character_AddInspirationByName(e->pchar, "Medium", "Good_Luck", "arena");
    character_AddInspirationByName(e->pchar, "Medium", "Keen_Insight", "arena");
    character_AddInspirationByName(e->pchar, "Medium", "Take_a_Breather", "arena");
    character_AddInspirationByName(e->pchar, "Medium", "Dramatic_Improvement", "arena");
    character_AddInspirationByName(e->pchar, "Medium", "Focused_Rage", "arena");
    character_AddInspirationByName(e->pchar, "Medium", "Emerge", "arena");
    //character_AddInspirationByName(e->pchar, "Medium", "Bounce_Back");
}

// Adjust players' teamups as they enter map.
// Drop all players from their previous teams,
// then add them to an arena-specific team.
void JoinMapSideTeamup(Entity* e)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    ArenaParticipant* part;
    int i;

    part = ArenaParticipantFind(g_arenamap_event, e->db_id);
    devassert(part);

    if (!part || !e || !e->pl)
    {
        return;
    }

    // team is invalid, so have him quit his current team so he can
    // be joined by someone else or be put on a valid team
    teamDelMember(e, CONTAINER_TEAMUPS, 0, 0); 

    for (i = 0; i < players->count; i++)
    {
        if (players->ents[i] != e && players->ents[i]->pl
            && players->ents[i]->pl->arenaMapSide == e->pl->arenaMapSide)
        {
            ArenaParticipant *other = ArenaParticipantFind(g_arenamap_event, players->ents[i]->db_id);
            if (other && other->arena_team_id && players->ents[i]->teamup
                && players->ents[i]->teamup->members.count < MAX_TEAM_MEMBERS)
            {
                teamAddMember(e, CONTAINER_TEAMUPS, other->arena_team_id, 0);
                part->arena_team_id = other->arena_team_id;
                break;
            }
        }
    }

    if (!part->arena_team_id)
    {
        // Couldn't fit into a current arena team, so make my own.
        part->arena_team_id = teamCreate(e, CONTAINER_TEAMUPS);
    }
}

static void ServeAllFloater(char* str)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        serveFloater(players->ents[i], players->ents[i], str, 0.0, kFloaterStyle_Attention, 0);
    }
}

void ArenaGiveTempPower(Entity* e, char* powerstr)
{
    const BasePower* ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", powerstr);

    if(ppowBase!=NULL)
    {
        Power *ppow;
        PowerSet *pset;

        pset = character_BuyPowerSet(e->pchar, ppowBase->psetParent);
        ppow = character_BuyPower(e->pchar, pset, ppowBase, 0); // checks for duplicates, etc.

        if(ppow!=NULL)
        {
            eaPush(&e->powerDefChange, powerRef_CreateFromPower(e->pchar->iCurBuild, ppow));

            character_EnterStance(e->pchar, ppow, false);
            character_ActivatePowerOnCharacter(e->pchar, e->pchar, ppow);
            if (ppow && ppow->ppowBase && ppow->ppowBase->bShowBuffIcon)
                e->team_buff_update = true;
        }
    }
}

void ArenaRemoveTempPower(Entity *e, char *powerstr)
{
    const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", powerstr);
    if(ppowBase!=NULL)
    {
        character_RemoveBasePower(e->pchar, ppowBase);
    }
}

static void ShowMatchResults(int event_id,int unique_id)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        Entity* e = players->ents[i];
        ArenaRequestResults(e,event_id,unique_id);
    }
}

static void SetAllPlayerReturnSpawns(int winningside)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    ArenaParticipant* part;
    int i;

    for (i = 0; i < players->count; i++)
    {
        Entity* e = players->ents[i];

        part = ArenaParticipantFind(g_arenamap_event, e->db_id);
        if (!part)
            continue;

        if(part->side == winningside)
            sprintf(e->spawn_target, "Arena_Winners");
        else
            sprintf(e->spawn_target, "Arena_Losers");

        e->db_flags |= DBFLAG_DOOR_XFER;
    }
}

static void DropPlayersWhoArentHere(void)
{
    int i, n;

    n = eaSize(&g_arenamap_event->participants);
    for (i = 0; i < n; i++)
    {
        int dbid = g_arenamap_event->participants[i]->dbid;
        Entity* e = entFromDbId(dbid);
        if (!e && g_arenamap_event->participants[i]->seats[g_arenamap_event->round] == g_arenamap_seat)
        {
            ArenaDropPlayer(dbid, g_arenamap_event->eventid, g_arenamap_event->uniqueid);
        }
    }
}

static U32 GetArenaDuration()
{
    if(g_arenamap_event->victoryType == ARENA_TIMED)
    {
        return (U32) g_arenamap_event->victoryValue * 60;
    }

    return 0;
}

static int GetKillLimit()
{
    if(g_arenamap_event->victoryType == ARENA_KILLS)
    {
        return g_arenamap_event->victoryValue;
    }

    return 0;
}

static int GetPlayerStock()
{
    if(g_arenamap_event->victoryType == ARENA_STOCK)
    {
        return g_arenamap_event->victoryValue;
    }

    return 0;
}

static int GetTeamStock()
{
    if(g_arenamap_event->victoryType == ARENA_TEAMSTOCK)
    {
        return g_arenamap_event->victoryValue;
    }

    return 0;
}

static void CalcScores(void)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;
    ArenaParticipant* part;
    int has_kill_limit = GetKillLimit(), duration = GetArenaDuration();

    memset(g_scores, 0, sizeof(g_scores));
    memset(g_is_still_playing, 0, sizeof(g_is_still_playing));
    for (i = 0; i < players->count; i++)
    {
        Entity* e = players->ents[i];

        part = ArenaParticipantFind(g_arenamap_event, e->db_id);
        if (!part || part->dropped)
            continue;

        if(e->pchar->attrCur.fHitPoints <= 0 && !part->death_time[g_arenamap_event->round])
        {
            part->death_time[g_arenamap_event->round] = ABS_TIME - g_matchstarttime;
        }

        if (!has_kill_limit && !e->pl->arena_lives_stock && !duration && !g_team_lives_stock[e->pl->arenaMapSide] &&
            (!e->pl->arenaMapSide || e->pl->arenaMapDefeated || e->pchar->attrCur.fHitPoints <= 0 || e->pl->arenaAmCamera))
            continue;

        //If you are in a pet arena and are out of pet arena pets, then you are out of the game
        if( g_arenamap_event->use_gladiators )
        {
            int i;
            int petCount = 0;
            for( i = 0 ; i < e->petList_size ; i++ )
            {
                Entity * pet = erGetEnt(e->petList[i]);
                if( pet && pet->owner > 0 && pet->erOwner == erGetRef( e ) && pet->commandablePet ) 
                    petCount++;
            }
            if( 0 == petCount )
                continue;
        }
 
        g_is_still_playing[e->pl->arenaMapSide]++;

        if(has_kill_limit || duration)
        {
            part = ArenaParticipantFind(g_arenamap_event, e->db_id);
            g_scores[e->pl->arenaMapSide] += part->kills[g_arenamap_event->round];
        }
    }
}

static int WinningMapSide(int want_random_winner)
{
    int i, count, winner;
    int* valid_sides;
    int random_winner;
    int last_side_here;
    int kill_limit = GetKillLimit();
    int highest_kills = -1, duration = GetArenaDuration();

    if (g_winningmapside)
        return g_winningmapside;

    if(want_random_winner)
        eaiCreate(&valid_sides);

    // count teams remaining
    CalcScores();
    winner = count = 0;
    
    for (i = 1; i <= ARENA_MAX_SIDES; i++)
    {
        if (g_is_still_playing[i])
        {
            last_side_here = i;
            if(kill_limit)
            {
                if(g_scores[i] >= kill_limit)
                    winner = i;
            }
            else if(duration && timerSecondsSince2000() > phase_time + duration)
            {
                if(g_scores[i] > highest_kills)
                {
                    highest_kills = g_scores[i];
                    winner = i;
                }
                else if (g_scores[i] == highest_kills)
                {
                    winner = 0;
                }
            }
            else if (duration && g_arenamap_phase == ARENAMAP_SUDDEN_DEATH)
            {
                if (highest_kills == -1)
                {
                    highest_kills = g_scores[i];
                }
                else if (g_scores[i] > highest_kills)
                {
                    winner = i;
                }
                else if (g_scores[i] < highest_kills)
                {
                    winner = 1;
                }
            }

            count++;
            if(want_random_winner)
                eaiPush(&valid_sides, i);
        }
    }

    if(want_random_winner)
    {
         random_winner = valid_sides[eaiSize(&valid_sides) * rand() / (RAND_MAX+1)];
         eaiDestroy(&valid_sides);
         return random_winner;
    }

    if(count <= 1)
        g_winningmapside = last_side_here;
    else if (winner && (kill_limit || duration))
        g_winningmapside = winner;

    return g_winningmapside;
}

void ArenaMapAdvanceAllPlayersToFinishPhase();

void ArenaEndEvent(char* floaterstr, int winningside)
{
    int time = timerSecondsSince2000();
    ArenaSetAllClientsCompassString(g_arenamap_event, "BeTeleportedBack",
        g_arenamap_event->tournamentType != ARENA_TOURNAMENT_NONE ? ARENAMAP_FINISH_TIME_MULTIROUND : ARENAMAP_FINISH_TIME_NORMAL);
    ServeAllFloater(floaterstr);
    ArenaReportMapResults(g_arenamap_event, g_arenamap_seat, winningside, time - start_time);
    ShowMatchResults(g_arenamap_event->eventid, g_arenamap_event->uniqueid);
    SetAllPlayerReturnSpawns(winningside);
    ArenaMapAdvanceAllPlayersToFinishPhase();
    g_arenamap_phase = ARENAMAP_FINISH;
    phase_time = time;
    {
        char *tmp = NULL;
        int i;
        int n = eaSize(&g_arenamap_event->participants);
        for( i = 0; i < n; ++i ) 
        {
            ArenaParticipant *p = g_arenamap_event->participants[i];
            estrConcatf(&tmp,"teammate=%s,sgid=%i,side=%i;",p->name,p->sgid,p->side); 
        }
        dbLog("Arena:EndEvent", NULL, "eventid=%i;uid=%i,info=\"%s\"",g_arenamap_event->eventid,g_arenamap_event->uniqueid,tmp);
        estrDestroy(&tmp);
    }
}

void ArenaMapSetTeam(Entity* e)
{
    if (e->pl->arenaMapSide)
        entSetOnTeam(e, e->pl->arenaMapSide + kAllyID_FirstArenaSide - 1);
}

static void ArenaMapAdvancePlayerToBuffPhase(Entity *e)
{
//    dbLog("ArenaMapAdvancePlayerToBuffPhase(", e, ") called.");

    if(e->pl->arenaAmCamera)
    {
        return;
    }

    if (g_arenamap_event->no_end)
    {
        ArenaGiveTempPower(e, "Unlimited_Endurance");
    }
    if (g_arenamap_event->no_pool)
    {
        ArenaGiveTempPower(e, "Disable_Pool");
    }
    if (g_arenamap_event->no_travel)
    {
        ArenaGiveTempPower(e, "Disable_Travel");
    }
    if (g_arenamap_event->no_stealth)
    {
        ArenaGiveTempPower(e, "Disable_Stealth");
    }
    switch (g_arenamap_event->inspiration_setting)
    {
        case ARENA_NO_INSPIRATIONS:
            ArenaGiveTempPower(e, "Disable_Small_Inspiration");
            // fall through
        case ARENA_SMALL_INSPIRATIONS:
            ArenaGiveTempPower(e, "Disable_Medium_Inspiration");
            // fall through
        case ARENA_SMALL_MEDIUM_INSPIRATIONS:
            ArenaGiveTempPower(e, "Disable_Large_Inspiration");
            // fall through
        case ARENA_NORMAL_INSPIRATIONS:
            ArenaGiveTempPower(e, "Disable_Special_Inspiration");
            // fall through
        case ARENA_ALL_INSPIRATIONS:
            ; // do nothing
    }
    if (!g_arenamap_event->enable_temp_powers)
    {
        ArenaGiveTempPower(e, "Disable_Temp");
        ArenaGiveTempPower(e, "Disable_Temp_Dead");
    }
    ArenaGiveTempPower(e, "Enable_Arena_Temp");

    if (g_arenamap_event->use_gladiators)
    {
        ArenaCreateArenaPets( e );

        e->untargetable |= UNTARGETABLE_GENERAL;
        e->invincible |= INVINCIBLE_GENERAL;
        character_Reset(e->pchar, false);
    }

    if (g_arenamap_phase <= ARENAMAP_BUFF)
    {
        ArenaGiveTempPower(e, "Buff_Period");
    }

    if (g_arenamap_event->use_gladiators)
    {
        // Give player Camera powers, because they need to fly around like an observer
        ArenaGiveTempPower(e, "Remote_Camera");

        //e->pl->disablePowers = 1;

        START_PACKET(pak, e, SERVER_ARENA_UPDATE_TRAY_DISABLE)
        END_PACKET
    }

    // Unfreeze
    e->controlsFrozen = 0;
    e->doorFrozen = 0;
}

static void ArenaMapAdvanceAllPlayersToBuffPhase()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        if (g_arenamap_event->use_gladiators)
        {
            ArenaStartClientCountdown(players->ents[i], "WaitPlayers", ARENA_COUNTDOWN_TIMER);
            ArenaSetClientCompassString(players->ents[i], "WaitPlayers", 0, 0, ARENA_GLOBAL_RESPAWN_PERIOD);
        }
        else
        {
            ArenaStartClientCountdown(players->ents[i], "BuffPlayers", ARENA_BUFF_TIMER);
            ArenaSetClientCompassString(players->ents[i], "BuffPlayers", 0, 0, ARENA_GLOBAL_RESPAWN_PERIOD);
        }
        ArenaMapAdvancePlayerToBuffPhase(players->ents[i]);
    }
}

static void ArenaMapAdvancePlayerToFightPhase(Entity* e)
{
    char *compassString;

    // Unfreeze
    e->controlsFrozen = 0;
    e->doorFrozen = 0;
    ArenaRemoveTempPower(e, "Buff_Period");
    
    serveFloater(e, e, "ArenaFight", 0.0, kFloaterStyle_PriorityAlert, 0);

    if (e->pl->arenaAmCamera)
    {
        compassString = "ClickToDrop";

        if (g_arenamap_event->victoryType == ARENA_TIMED)
            compassString = "MatchTimeRemaining";

        ArenaSetClientCompassString(e, compassString, phase_time + GetArenaDuration() - dbSecondsSince2000(), GetArenaDuration(), ARENA_GLOBAL_RESPAWN_PERIOD);
        return;
    }

    if (g_arenamap_event->tactical_start)
        e->pchar->attrCur.fEndurance = 0;

    // Update stock lives display in compass
    if (GetPlayerStock() || GetTeamStock() || GetKillLimit() || GetArenaDuration())
    {
        int inf_respawn = GetKillLimit() || GetArenaDuration();
        int required_kill_count = GetKillLimit();
        int is_team_stock = GetTeamStock();
        int stock = is_team_stock ? is_team_stock : GetPlayerStock();

        if(stock || inf_respawn)
            ArenaSendVictoryInformation(e, stock, required_kill_count, inf_respawn);
    }

    compassString = "ArenaFight";

    if (g_arenamap_event->victoryType == ARENA_TIMED)
        compassString = "MatchTimeRemaining";
    else if (g_arenamap_event->victoryType == ARENA_STOCK)
        compassString = "LivesRemaining";
    else if (g_arenamap_event->victoryType == ARENA_TEAMSTOCK)
        compassString = "TeamLivesRemaining";
    else if (g_arenamap_event->victoryType == ARENA_KILLS)
        compassString = "KillsRemaining";
    
    ArenaSetClientCompassString(e, compassString, phase_time + GetArenaDuration() - dbSecondsSince2000(), GetArenaDuration(), ARENA_GLOBAL_RESPAWN_PERIOD);
}

static void ArenaMapAdvanceAllPlayersToFightPhase()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        ArenaMapAdvancePlayerToFightPhase(players->ents[i]);
    }
}

void ArenaMapAdvancePlayerToSuddenDeathPhase(Entity *e)
{
    int i;

    if (!e)
    {
        return;
    }

    if (entAlive(e))
    {
        ArenaGiveTempPower(e, "PvP_SuddenDeath");
    }

    for (i = 0; i < e->petList_size; i++)
    {
        ArenaMapAdvancePlayerToSuddenDeathPhase(erGetEnt(e->petList[i]));
    }
}

static void ArenaMapAdvanceAllPlayersToSuddenDeathPhase()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        ArenaMapAdvancePlayerToSuddenDeathPhase(players->ents[i]);
    }
}

void ArenaMapAdvancePlayerToFinishPhase(Entity *e)
{
    int i;

    if (!e)
    {
        return;
    }

    ArenaRemoveTempPower(e, "PvP_SuddenDeath");

    ArenaNotifyFinish(e);

    for (i = 0; i < e->petList_size; i++)
    {
        ArenaMapAdvancePlayerToFinishPhase(erGetEnt(e->petList[i]));
    }
}

static void ArenaMapAdvanceAllPlayersToFinishPhase()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        ArenaMapAdvancePlayerToFinishPhase(players->ents[i]);
    }
}

void ArenaMapInitPlayer(Entity* e)
{
    ArenaParticipant* part;

    if (!OnArenaMap())
    {
        ArenaSendScheduledMessages(e);
        return;
    }

    part = ArenaParticipantFind(g_arenamap_event, e->db_id);
    if (!part || part->dropped)
    {
        // make him an observer if not in the tourney
        ArenaSwitchCamera(e, 1);
    }
    else
    {
        // put him on the right side 
        e->pl->arenaMapSide = GetMapSide(part->side);
        if(GetTeamStock())
            g_team_lives_stock[e->pl->arenaMapSide] = GetTeamStock();
        ArenaMapSetTeam(e);
        JoinMapSideTeamup(e);
        e->pl->arena_lives_stock = GetPlayerStock();

        // Reset health and powers etc
        character_Reset(e->pchar, false);
    }
    
    if(g_arenamap_phase <= ARENAMAP_HOLD)
    {
        serveFloater(e, e, "WaitPlayers", 0.0, kFloaterStyle_Attention, 0);
        ArenaSetClientCompassString(e, "WaitPlayers", 0, 0, ARENA_GLOBAL_RESPAWN_PERIOD);
    }
    else if(g_arenamap_phase == ARENAMAP_BUFF)
    {
        if (g_arenamap_event->use_gladiators)
        {
            ArenaStartClientCountdown(e, "WaitPlayers", ARENA_COUNTDOWN_TIMER - (dbSecondsSince2000() - phase_time));
            ArenaSetClientCompassString(e, "WaitPlayers", 0, 0, ARENA_GLOBAL_RESPAWN_PERIOD);
        }
        else
        {
            ArenaStartClientCountdown(e, "BuffPlayers", ARENA_BUFF_TIMER - (dbSecondsSince2000() - phase_time));
            ArenaSetClientCompassString(e, "BuffPlayers", 0, 0, ARENA_GLOBAL_RESPAWN_PERIOD);
        }
        ArenaMapAdvancePlayerToBuffPhase(e);
    }
    else
    {
        ArenaMapAdvancePlayerToBuffPhase(e); // must go through all phases
        ArenaMapAdvancePlayerToFightPhase(e);
    }

    ArenaRequestAllCurrentResults(e);
}

#define PLAYERS_PER_RING    5
#define RING_RADIUS            5
#define SEARCH_HEIGHT        10.0
#define MAX_DROP_DIST        15.0
#define MAX_RINGS            5
void RotateSpawn(Entity* e, U32 rotate[2], int* result)
{
    CollInfo info;
    Vec3 newpos, entitypos;
    float dy, angle;
    int ring, rotation, maxrot;
    bool found_pos = false;
    int i;

    copyVec3(ENTPOS(e), entitypos);
    entitypos[1] += 1.0;
    entitypos[1] += beaconGetDistanceYToCollision(entitypos, SEARCH_HEIGHT);

    for(i = 0; i < 500 && !found_pos; i++)
    {
        found_pos = true;
        ring = rotate[0];
        rotation = rotate[1];
        if(rotate[0])
            maxrot = (2 * (ring - 1) + 1) * PLAYERS_PER_RING;
        else
            maxrot = 1;

        angle = 2 * rotation * PI / maxrot;

        newpos[0] = ring * RING_RADIUS * cos(angle);
        newpos[1] = 0;
        newpos[2] = ring * RING_RADIUS * sin(angle);

        addVec3(entitypos, newpos, newpos);

        if(collGrid(NULL, entitypos, newpos, &info, 1, COLL_NOTSELECTABLE | COLL_DISTFROMSTART | COLL_BOTHSIDES))
            found_pos = false;

        dy = beaconGetDistanceYToCollision(newpos, -1 * (MAX_DROP_DIST + SEARCH_HEIGHT));

        if(dy >= MAX_DROP_DIST + SEARCH_HEIGHT)
            found_pos = false;

        if(found_pos)
        {
            newpos[1] -= dy;
            entUpdatePosInstantaneous(e, newpos);
        }
        
        if(++rotate[1] == maxrot)
        {
            rotate[0]++;
            rotate[1] = 0;
        }

        if(rotate[0] >= MAX_RINGS)
            rotate[0] = 0;
    }

    if(result)
        *result = found_pos;
}
#undef MAX_RINGS
#undef MAX_DROP_DIST
#undef SEARCH_HEIGHT
#undef PLAYERS_PER_RING
#undef RING_RADIUS

void ArenaMapSetPosition(Entity* e)
{
    ArenaParticipant* part;
    DoorEntry* door;
    char buf[20];

    if (!OnArenaMap()) return;

    // make him an observer if not in the tourney
    part = ArenaParticipantFind(g_arenamap_event, e->db_id);
    if (!part)
    {
        door = dbFindSpawnLocation("ArenaObserver");
        if (door)
            entUpdateMat4Instantaneous(e, door->mat);
        return;
    }

    // otherwise, choose the right start location
    e->pl->arenaMapSide = GetMapSide(part->side);
    sprintf(buf, "ArenaStart%i", e->pl->arenaMapSide);
    door = dbFindSpawnLocation(buf);
    if (door)
    {
        entUpdateMat4Instantaneous(e, door->mat);
        RotateSpawn(e, g_mapside_rotate[e->pl->arenaMapSide], NULL);
        if( e->seq )
            seqSetStateFromString( e->seq->state, "ARENAEMERGE" );
    }
}

// just need to freeze people as they come onto map
void ArenaMapReceivedCharacter(Entity* e)
{
    ArenaParticipant* part;

    if (!OnArenaMap()) return;

    part = ArenaParticipantFind(g_arenamap_event, e->db_id);
    if (g_arenamap_phase <= ARENAMAP_HOLD && part)
    {
        e->controlsFrozen = 1;
        e->doorFrozen = 1;
    }
}

static void UpdateMapSideStockLives(int mapside)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for (i = 0; i < players->count; i++)
    {
        Entity* e = players->ents[i];
        if(e->pl->arenaMapSide == mapside)
        {
            ArenaSendVictoryInformation(e, g_team_lives_stock[mapside], 0, 0);
        }
    }
}

void ArenaMapHandleKill(Entity* killer, Entity* victim)
{
    ArenaParticipant* part = ArenaParticipantFind(g_arenamap_event, killer ? killer->db_id : 0);
    int side_kills = 0, mapside = GetMapSide(part ? part->side : 0);
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];

    if (!g_arenamap_event || !part || !killer || !victim || !victim->pl)
        return;

    if (victim->pl->arenaAmCamera)
        return;

    if (g_arenamap_phase != ARENAMAP_FIGHT && g_arenamap_phase != ARENAMAP_SUDDEN_DEATH)
        return;

    part->kills[g_arenamap_event->round]++;
    ArenaReportKill(g_arenamap_event, part->dbid);

    if (GetKillLimit())
    {
        if (part->kills[g_arenamap_event->round] >= GetKillLimit())
        {
            ArenaEndEvent("HaveWinner", part->side);
        }
        else
        {
            ArenaSendVictoryInformation(killer, 0, GetKillLimit() - part->kills[g_arenamap_event->round], 1);
        }
    }
}

void ArenaMapHandleDeath(Entity* e)
{
    ArenaParticipant* part = ArenaParticipantFind(g_arenamap_event, e->db_id);

    if(!part)
        return;

    e->pl->arenaDeathProcessed = 1;
    e->pl->last_arena_death = timerSecondsSince2000();
    part->death_time[g_arenamap_event->round] = ABS_TIME - g_matchstarttime;

    if(GetTeamStock())
        --g_team_lives_stock[e->pl->arenaMapSide];
}

// This is only called when you know the entity must respawn.
int ArenaMapHandleEntityRespawn(Entity* e)
{
    char buf[20];
    DoorEntry* door;
    U32 time = timerSecondsSince2000();
    ArenaParticipant* part = ArenaParticipantFind(g_arenamap_event, e->db_id);

    if(entAlive(e))
        return 0;

    if(e->pl->arenaAmCamera)
    {    
        // player is a camera already
        ArenaSwitchCamera(e, 1);    // have to cycle the camera power to turn on again
    }
    else
    {
        if(GetTeamStock())
        {
            if(!g_team_lives_stock[e->pl->arenaMapSide])
            {
                PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
                int i;

                for (i = 0; i < players->count; i++)
                {
                    Entity* ent = players->ents[i];
                    if(ent->pl->arenaMapSide == e->pl->arenaMapSide)
                    {
                        ArenaSwitchCamera(ent, 1);
                        ArenaSetAllClientsCompassString(g_arenamap_event, "ClickToDrop", 0);
                    }
                }
            }
            else
                UpdateMapSideStockLives(e->pl->arenaMapSide);
//            g_team_lives_stock[e->pl->arenaMapSide]--;    // already being updated by the death handler now
//            UpdateMapSideStockLives(e->pl->arenaMapSide);
        }
        else if(e->pl->arena_lives_stock)
        {
            e->pl->arena_lives_stock--;
            ArenaSendVictoryInformation(e, e->pl->arena_lives_stock, 0, 0);
            if (!e->pl->arena_lives_stock)
            {
                ArenaSwitchCamera(e, 1);
            }
        }
        else if(GetKillLimit() || GetArenaDuration())
            ;    // if there is a kill limit, players can respawn without having to keep track of anything
        else
        {
            ArenaSwitchCamera(e, 1);
        }
    }

    if(e->pl->arenaAmCamera)
        door = dbFindSpawnLocation("ArenaObserver");
    else
    {
        part->death_time[g_arenamap_event->round] = 0;
        sprintf(buf, "ArenaStart%i", e->pl->arenaMapSide);
        door = dbFindSpawnLocation(buf);
    }

    // Clear the target of any player who has me targeted
    {
        int idxpl;
        Entity *curpl;
        EntityRef erDead = erGetRef(e);

        for (idxpl = 0; idxpl < player_ents[PLAYER_ENTS_ACTIVE].count; idxpl++)
        {
            curpl = player_ents[PLAYER_ENTS_ACTIVE].ents[idxpl];
            if (curpl 
                && curpl->pchar 
                && curpl->pchar->erTargetInstantaneous == erDead 
                && character_TargetIsFoe(e->pchar, curpl->pchar))
            {
                // Throw away their target
                curpl->pchar->erTargetInstantaneous = erGetRef(NULL);
                curpl->pchar->entParent->target_update = 1;
                curpl->pchar->entParent->clear_target = 1;
            }
        }
    }

    ArenaGiveTempPower(e, "Respawn");

    if (door)
        entUpdateMat4Instantaneous(e, door->mat);

    e->pl->arenaDeathProcessed = 3;
    completeGurneyXferWork(e);
    return 1;
}

static void RunRoundProcessAllParticipants()
{
    static U32 timeOfLastRespawnWave = 0;
    bool respawnWave = false;
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;
    U32 time = timerSecondsSince2000();

    if(!players->count)
        ArenaEndEvent("Where is everyone??", -1);

    if (!timeOfLastRespawnWave)
    {
        timeOfLastRespawnWave = g_matchstarttime;
    }

    if (timeOfLastRespawnWave + ARENA_GLOBAL_RESPAWN_PERIOD <= time || g_arenamap_phase == ARENAMAP_FINISH)
    {
        timeOfLastRespawnWave = time;
        respawnWave = true;
    }

    for (i = 0; i < players->count; i++)
    {
        if (players->ents[i]->pl->arenaDeathProcessed == 2)
        {
            ArenaMapHandleEntityRespawn(players->ents[i]);    // respawn the player
        }

        if (respawnWave && !entAlive(players->ents[i]) && 
            players->ents[i]->pl->arenaDeathProcessed == 1)
        {
            ArenaGiveTempPower(players->ents[i], "Respawn_Dead");
            players->ents[i]->pl->arenaDeathProcessed = 2;
        }
    }
}

void ArenaMapProcess(void)
{
    U32 time = dbSecondsSince2000();
    if (!OnArenaMap()) return;

    // check for phase changes
    switch (g_arenamap_phase)
    {
    case ARENAMAP_INIT:    // this phase is only to get a meaningful phase_time for ARENAMAP_HOLD
        srand(timerSecondsSince2000());
        g_arenamap_phase = ARENAMAP_HOLD;
        phase_time = time;
        // fall through
    case ARENAMAP_HOLD:
        {
            // Wait for all participants to be ready,
            // and at least ARENAMAP_CLIENT_MIN_WAIT seconds 
            // but no longer than ARENAMAP_CLIENT_MAX_WAIT seconds.
            int i;
            Entity* e;
            bool waitforme = false;

            if(time <= phase_time + ARENAMAP_CLIENT_MAX_WAIT)
            {
                for(i = eaSize(&g_arenamap_event->participants)-1; i >= 0; i--)
                {
                    if(g_arenamap_event->participants[i]->dbid &&
                        g_arenamap_event->participants[i]->seats[g_arenamap_event->round] == g_arenamap_seat)
                    {
                        e = entFromDbId(g_arenamap_event->participants[i]->dbid);
                        if(!e || !e->client || !e->ready)
                        {
                            waitforme = true;
                        }
                    }
                }
            }

            if((time > phase_time + ARENAMAP_CLIENT_MIN_WAIT && !waitforme) || time > phase_time + ARENAMAP_CLIENT_MAX_WAIT)
            {
                ArenaMapAdvanceAllPlayersToBuffPhase();
                phase_time = time;
                g_arenamap_phase = ARENAMAP_BUFF;
            }
        }
        break;
    case ARENAMAP_BUFF:
        if(time > phase_time + (g_arenamap_event->use_gladiators ? ARENA_COUNTDOWN_TIMER : ARENA_BUFF_TIMER))
        {
            DropPlayersWhoArentHere();
            g_arenamap_phase = ARENAMAP_FIGHT;
            phase_time = time;
            g_matchstarttime = ABS_TIME;
            ArenaMapAdvanceAllPlayersToFightPhase();
        }
        break;
    case ARENAMAP_FIGHT:
        {
            int duration = GetArenaDuration();
            int winningMapSide = WinningMapSide(0);
            RunRoundProcessAllParticipants();
            if (winningMapSide)
            {
                ArenaEndEvent("HaveWinner", GetEventSide(winningMapSide));
            }
            else if(duration && time > phase_time + duration)
            {
                ArenaMapAdvanceAllPlayersToSuddenDeathPhase();
                ServeAllFloater("EnterSuddenDeath");
                ArenaSetAllClientsCompassString(g_arenamap_event, "SuddenDeath", ARENA_SUDDENDEATH_TIME);
                g_arenamap_phase = ARENAMAP_SUDDEN_DEATH;
                phase_time = time;
            }
        }
        break;
    case ARENAMAP_SUDDEN_DEATH:
        {
            int winningMapSide = WinningMapSide(0);
            RunRoundProcessAllParticipants();
            if (winningMapSide)
            {
                ArenaEndEvent("HaveWinner", GetEventSide(winningMapSide));
            }
            else if(time > phase_time + ARENA_SUDDENDEATH_TIME)
            {
                if(g_arenamap_event->tournamentType != ARENA_TOURNAMENT_NONE
                    && g_arenamap_event->roundtype[g_arenamap_event->round] == ARENA_ROUND_SINGLE_ELIM)
                {
                    ArenaEndEvent("ArenaTimeUp", GetEventSide(WinningMapSide(1)));
                }
                else
                {
                    ArenaEndEvent("ResultDraw", -1);
                }
            }
        }
        break;
    case ARENAMAP_FINISH:
        RunRoundProcessAllParticipants();
        if (!player_ents[PLAYER_ENTS_ACTIVE].count ||
            time > phase_time + (g_arenamap_event->tournamentType != ARENA_TOURNAMENT_NONE ? ARENAMAP_FINISH_TIME_MULTIROUND : ARENAMAP_FINISH_TIME_NORMAL))
        {
            dbAsyncReturnAllToStaticMap("ArenaMapProcess() case ARENAMAP_FINISH",NULL);
            phase_time = time;
        }
        break;
    }
}

void ArenaSwitchCamera(Entity* e, int on)
{
    int changed = 0;
    if(on)
    {
        if(!e->pl->arenaAmCamera)
            changed = 1;

        modStateBits(e, ENT_DEAD, 0, FALSE );
        e->pl->arenaAmCamera = 1;
//        e->pchar->attrCur.fHitPoints = 1.0;

        if(!changed)
            character_UnCamera(e);

        character_BecomeCamera(e);

        e->untargetable |= UNTARGETABLE_GENERAL;
        e->invincible |= INVINCIBLE_GENERAL;
        SET_ENTHIDE(e) |= 1;
        e->draw_update = 1;
        entSetOnTeam(e, kAllyID_Good);

        ArenaSetClientCompassString(e, "ClickToDrop", 0, 0, ARENA_GLOBAL_RESPAWN_PERIOD);
        ArenaSendVictoryInformation(e, 0, 0, 0);
    }
    else    // this is not currently used but just kinna here for completeness
    {
        if(e->pl->arenaAmCamera)
            changed = 1;

        character_UnCamera(e);
        e->pl->arenaAmCamera = 0;
        e->untargetable &= ~UNTARGETABLE_GENERAL;
        e->invincible &= ~INVINCIBLE_GENERAL;
        SET_ENTHIDE(e) &= ~1;
        e->draw_update = 1;
        ArenaMapSetTeam(e);
    }

    setClientState(e, STATE_RESURRECT);

    if(changed)
    {
        START_PACKET(pak, e, SERVER_ARENA_UPDATE_TRAY_DISABLE)
        END_PACKET
    }
}

ArenaEvent* GetMyEvent(void)
{
    return g_arenamap_event;
}

// p->iCombatLevel is set to experience level before calling this function.
// returns true if I'm going to handle setting this players combat level.
int ArenaMapLevelOverride(Character* p)
{
    ArenaParticipant* part;
    Entity* e;

    if (!OnArenaMap()) 
        return 0;
    e = p->entParent;
    if (!e || !e->pl) 
        return 0;

    part = ArenaParticipantFind(g_arenamap_event, e->db_id);
    if (!part) // he's an observer
        return 0;

    // ok, we're under the combat domain of an event now
    if (g_arenamap_event->weightClass && 
        g_arenamap_event->weightClass < ARENA_NUM_WEIGHT_CLASSES)
    {
        // according to design, all we do is limit the upper bound.
        // lower doesn't matter (combat mods are off anyway)
        int max = g_weightRanges[g_arenamap_event->weightClass].maxLevel;
        if (max < p->iCombatLevel)
            p->iCombatLevel = max;
    }
    // else, level's aren't restricted, so he should play as his experience level

    return 1;
}

int ArenaMapIsParticipant(Entity* e)
{
    ArenaParticipant* part;

    if (!OnArenaMap()) return 0;
    if (!e || !e->pl) return 0;

    part = ArenaParticipantFind(g_arenamap_event, e->db_id);
    return part? 1: 0;
}

