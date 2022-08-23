/*
 *
 *    endgameraid.c - Copyright 2010 NC Soft
 *        All Rights Reserved
 *        Confidential property of NC Soft
 *
 *    Setup and handling of end game raid instances
 *
 */

#include <utilitieslib/utils/error.h>
#include "dbcomm/dbcomm.h"
#include "entity/entity.h"
#include "container/team.h"
#include "container/league.h"
#include "storyarc/storyarcprivate.h"
#include "generator/encounterPrivate.h"
#include "generator/encounter.h"
#include "script/ZoneEvents/ScriptedZoneEvent.h"
#include "cmdparse/cmdserver.h"
#include "EndGameRaid.h"
#include "Turnstile.h"
#include "dbcomm/logcomm.h"
#include "svr/svr_base.h" // START_PACKET and END_PACKET
#include "comm_game.h"
#include "gameComm/svr_chat.h"
#include "door.h"

static char s_zoneEvent[256];
static int s_playerCount;
static int s_mobLevel;

#ifdef _snprintf
#undef _snprintf
#endif
#define _snprintf(buff, size, fmt, ...) _snprintf_s((buff), (size), (size) - 1, (fmt), __VA_ARGS__);

static StoryTask s_endGameRaidDummyStorytask = { 0 };

static MissionDef s_endGameRaidDummyMissionDef = { 
    "EndGameRaid.c",
    0,
    0,
    0,
    0,
    0,
    "",
    "5thcolumn",
    //char*                cityzone;                // not var
    //char*                doortype;                // not var
    //char*                locationname;            // not var, localized
    //char*                foyertype;                // not var
    //char*                doorNPC;                // not var
    //char*                doorNPCDialog;            // not var
    //char*                doorNPCDialogStart;        // not var
    //VillainPacingEnum    villainpacing;            // not var
    //int                    missiontime;        // not var
    //int                    missionLevel;        // not var
    //U32                    flags;
};

static MissionInfo s_endGameRaidDummyMission =
{
    &s_endGameRaidDummyStorytask,
    &s_endGameRaidDummyMissionDef,
    0,
};

typedef struct EndGameRaidKickAttempts
{
    U32    lastKickTime;
    int kicked_dbid;
    int initiator_dbid;
}EndGameRaidKickAttempts;

typedef struct EndGameRaidKickVote
{
    int voter_dbid;
    int vote;
}EndGameRaidKickVote;
typedef struct EndGameRaidKickTable
{
    U32 eventStartTime;
    U32 lastKickTime;    
    EndGameRaidKickAttempts **pastKickList;
    EndGameRaidKickAttempts **currentKickList;
    EndGameRaidKickVote voteTable[MAX_LEAGUE_MEMBERS];
    int totalCount;
}EndGameRaidKickTable;

EndGameRaidKickMods g_EndGameRaidKickMods;
static EndGameRaidKickTable s_EndGameRaidKickTable;
// DGNOTE 9/10/2010 - refilling an event
// Update this call to accept some identifier that guarantees to identify the TurnstileMission data structure for this mission.  Identify it by name,
// since this is a mapserver and will therefore have that data loaded by default.
char *EndGameRaidConstructInfoString(char *zoneEvent, int playerCount, int mobLevel, int eventID, int missionID, int eventLocked)
{
    static char buf[512];
    _snprintf(buf, sizeof(buf), "E:%s:%d:%d;%d;%d;%d", zoneEvent, playerCount, mobLevel, eventID, missionID, eventLocked);
    return buf;
}

// DGNOTE 9/10/2010 - refilling an event
// Extract the TurnstileMission identifier and use it to get a pointer to the mission we're running.
int EndGameRaidDeconstructInfoString(char *mapinfo)
{
    int eventID;
    int missionID;
    int eventLocked;
    int numArgs = sscanf(mapinfo, "E:%[^:]:%d:%d;%d;%d;%d", s_zoneEvent, &s_playerCount, &s_mobLevel, &eventID, &missionID,&eventLocked);
    turnstileMapserver_setMapInfo(eventID, missionID, eventLocked);
    return (numArgs == 6);
}

void EndGameRaidSetInfoString(char *info)
{
    if (!EndGameRaidDeconstructInfoString(info))
    {
        LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "EndGameRaidSetInfoString, got bad info %s", info);
        dbAsyncReturnAllToStaticMap("EndGameRaidSetInfoString() bad info", info);
        return;
    }
    db_state.is_endgame_raid = 1;
}

// Hook player entry to kickstart everything.
void EndGameRaidPlayerEnteredMap(Entity *e)
{
    static int scriptIsRunning = 0;
    if (!db_state.is_endgame_raid)
    {
        // Bail out if we're not an end game raid
        return;
    }
    if (!scriptIsRunning)
    {
        scriptIsRunning = 1;
        // last sanity check on values
        if (s_zoneEvent[0] == 0)
        {
            LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0,  "EndGameRaidStartEvent, startupdata was bad: \"%s\"", s_zoneEvent);
            dbAsyncReturnAllToStaticMap("EndGameRaidStartEvent() bad data", "");
            return;
        }
        // Start the script
        if (ScriptedZoneEventStartFile(s_zoneEvent, 1) == 0)
        {
            // and kill everything if that failed
            LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "EndGameRaidStartEvent, script failed to start: \"%s\"", s_zoneEvent);
            dbAsyncReturnAllToStaticMap("EndGameRaidStartEvent() script failure", s_zoneEvent);
            return;
        }

        s_EndGameRaidKickTable.eventStartTime = dbSecondsSince2000();

        // If we get here, everything is up and running as it should be.
        // Override a few things in the encounter spawn code:
        // Team size
        EncounterOverrideTeamSize(s_playerCount);
        // Moblevel.  The code that uses this is 1 based, while this is 0 based.  So we add 1.
        EncounterOverrideMobLevel(s_mobLevel + 1);
        // And flag that "ManualSpawn" groups are not to be included
        EncounterRespectManualSpawn(1);
        // Fire everything else
        EncounterForceSpawnAll();
        // Disable the overrides
        EncounterOverrideTeamSize(0);
        EncounterOverrideMobLevel(0);
        EncounterRespectManualSpawn(0);

        // Fill in a few things in the dummy mission
        s_endGameRaidDummyMission.baseLevel = s_mobLevel + 1;
        s_endGameRaidDummyMission.missionLevel = s_mobLevel + 1;
        s_endGameRaidDummyMissionDef.missionLevel = s_mobLevel + 1;
        difficultySet(&s_endGameRaidDummyMission.difficulty, 0, 1, 1, 1);

        g_activemission = &s_endGameRaidDummyMission;
        sendIncarnateTrialStatus(e, GetTrialStatus());
        AllowHeroAndVillainPlayerTypesToTeamUpOnThisMap(1);
        AllowPrimalAndPraetorianPlayersToTeamUpOnThisMap(1);
    }

    // Make sure we're not in a team or league before trying to join the one we care about here.
    quitLeagueAndTeam(e, 1);
    if (e && !ENTHIDE(e))
    {
        if (e->pl->lastTurnstileEventID)
        {
            league_AcceptOfferTurnstile(e->pl->lastTurnstileEventID, e->db_id, e->name, e->pl->desiredTeamNumber, e->pl->isTurnstileTeamLeader);
            e->pl->desiredTeamNumber = -2;

            // Put us on gang 1 so we view everyone as frinedly
            if (EAINRANGE((int)e->pl->lastTurnstileMissionID, turnstileConfigDef.missions))
            {
                entSetOnGang(e, turnstileConfigDef.missions[e->pl->lastTurnstileMissionID]->playerGang);
            }
            else
            {
                entSetOnGang(e, 1);
            }
        }
        else
        {
            if (!e->access_level)
            {
                // This shouldn't be possible!  We used to assert that this never happened!
                LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "This player got onto an endgame map without a lastTurnstileEventID!  Crazy things may happen...");
            }
            entSetOnGang(e, 1);
        }
    }
    else
    {
        entSetOnGang(e, 1);
    }
    START_PACKET(pak_out, e, SERVER_EVENT_START_STATUS);
    pktSendBits(pak_out, 1, 1);
    pktSendBits(pak_out, 1, 0);
    END_PACKET;
}

void EndGameRaidPlayerExitedMap(Entity *e)
{
    if (db_state.is_endgame_raid)
    {
        // Only do this if we're in an end game raid
        entSetOnGang(e, 0);
        sendIncarnateTrialStatus(e, trialStatus_None);

        if (IncarnateTrialComplete())
        {
            turnstileMapserver_playerLeavingRaid(e, 1);
        }
        else
        {
            quitLeagueAndTeam(e, 0);
            turnstileMapserver_playerLeavingRaid(e, 0);
        }
    }
}

void EndGameRaidReceiveVoteKickOpinion(int voter_dbid, int opinion)
{
    //    make sure a vote is happening
    if (s_EndGameRaidKickTable.lastKickTime)
    {
        if (dbSecondsSince2000() <= s_EndGameRaidKickTable.lastKickTime)
        {
            //    update the status
            int i;
            for (i = 0; i < s_EndGameRaidKickTable.totalCount; ++i)
            {
                if (s_EndGameRaidKickTable.voteTable[i].voter_dbid == voter_dbid)
                {
                    s_EndGameRaidKickTable.voteTable[i].vote = opinion;
                    break;
                }
            }
        }
    }
}

void EndGameRaidCompleteVoteKick()
{
    int i;
    int count = 0;
    int success =0;
    char *playerName = NULL;
    //    tally up the votes
    for (i = 0; i < s_EndGameRaidKickTable.totalCount; ++i)
    {
        Entity *e = entFromDbId(s_EndGameRaidKickTable.voteTable[i].voter_dbid);
        if (s_EndGameRaidKickTable.voteTable[i].vote)
            count++;
    }
    //    if over threshhold kick player

    {
        int lastIndex = eaSize(&s_EndGameRaidKickTable.pastKickList) - 1;
        EndGameRaidKickAttempts *currentAttempt = s_EndGameRaidKickTable.pastKickList[lastIndex];
        Entity *e = entFromDbId(currentAttempt->kicked_dbid);
        if (e)
        {
            playerName = e->name;
            if ((count*2) > s_EndGameRaidKickTable.totalCount)
            {
                //    send a ban list update to the ts so this player doesn't requeue into the same event
                turnstileMapserver_addBanID(e->auth_id);

                //    remove the player
                free(currentAttempt);
                eaRemove(&s_EndGameRaidKickTable.pastKickList, lastIndex);
                if (e->league)
                {
                    league_MemberQuit(e, 1);
                }
                else
                {
                    leaveMission(e, e->static_map_id, 1);
                }
                success = 1;
            }

            for (i = 0; i < s_EndGameRaidKickTable.totalCount; ++i)
            {
                Entity *e = entFromDbId(s_EndGameRaidKickTable.voteTable[i].voter_dbid);
                if (e)
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e, success ? "EGVRK_VoteSuccess" : "EGVRK_VoteFail", playerName), INFO_SVR_COM, 0);
                }
            }
        }
    }

    //    clear relevant stats
    s_EndGameRaidKickTable.totalCount = 0;
}

static void EndGameRaidInitiateVoteKick(Entity *seconder, EndGameRaidKickAttempts *currentAttempt)
{
    int i;

    if (!seconder || !currentAttempt)
        return;

    //    set last kick time for this player
    //    set last kick time
    //    set timer for this kick
    currentAttempt->lastKickTime = s_EndGameRaidKickTable.lastKickTime = dbSecondsSince2000() + g_EndGameRaidKickMods.kickDurationSeconds;
    eaPush(&s_EndGameRaidKickTable.pastKickList, currentAttempt);

    //    broadcast to all players requesting their view on the kick
    devassert(seconder && seconder->league);
    if (seconder && seconder->league)
    {
        for (i = 0; i < seconder->league->members.count; ++i)
        {
            Entity *e = entFromDbId(seconder->league->members.ids[i]);
            if (e)
            {
                s_EndGameRaidKickTable.voteTable[s_EndGameRaidKickTable.totalCount].voter_dbid = e->db_id;
                if ((currentAttempt->initiator_dbid == e->db_id) || (seconder->db_id == e->db_id))
                {
                    s_EndGameRaidKickTable.voteTable[s_EndGameRaidKickTable.totalCount].vote = 1;
                    chatSendToPlayer(e->db_id, localizedPrintf(NULL, "EGRVK_VoteStarted"), INFO_USER_ERROR, 0);
                }
                else
                {
                    START_PACKET(pak, e, SERVER_ENDGAMERAID_VOTE_KICK);
                    pktSendBitsAuto(pak, currentAttempt->kicked_dbid);
                    END_PACKET;
                    s_EndGameRaidKickTable.voteTable[s_EndGameRaidKickTable.totalCount].vote = 0;
                }
                s_EndGameRaidKickTable.totalCount++;
            }
        }
    }

    //    clear out the current kick attempt
    for (i = eaSize(&s_EndGameRaidKickTable.currentKickList) - 1; i >= 0; --i)
    {
        free(s_EndGameRaidKickTable.currentKickList[i]);
    }
    eaDestroy(&s_EndGameRaidKickTable.currentKickList);
}

void EndGameRaidRequestVoteKick(Entity *initiator, Entity *kicked)
{
    int i;
    int initiator_dbid;
    int kicked_dbid;
    EndGameRaidKickAttempts *successfulKickRequest = NULL;

    if (!initiator || !kicked)
        return;

    initiator_dbid = initiator->db_id;
    kicked_dbid = kicked->db_id;
    if (s_EndGameRaidKickTable.totalCount > 0)
    {
        //    send error
        chatSendToPlayer(initiator_dbid, localizedPrintf(NULL, "EGRVK_VoteInProgress"), INFO_USER_ERROR, 0);
        return;
    }

    //    if last kick time < X s
    if (s_EndGameRaidKickTable.lastKickTime)
    {
        if (((int)(dbSecondsSince2000() - s_EndGameRaidKickTable.lastKickTime)) < g_EndGameRaidKickMods.lastKickTimeSeconds)
        {
            //    send error
            chatSendToPlayer(initiator_dbid, localizedPrintf(NULL, "EGRVK_KickSpam"), INFO_USER_ERROR, 0);
            return;
        }
    }

    //    if last kick time for this player > X min
    for (i = eaSize(&s_EndGameRaidKickTable.pastKickList) -1; i >= 0; --i)
    {
        EndGameRaidKickAttempts *pastAttempts = s_EndGameRaidKickTable.pastKickList[i];
        if (pastAttempts->kicked_dbid == kicked_dbid)
        {
            if (((int)(dbSecondsSince2000() - pastAttempts->lastKickTime)) < (g_EndGameRaidKickMods.lastPlayerKickTimeMins * SECONDS_PER_MINUTE))
            {
                //    send error
                chatSendToPlayer(initiator_dbid, localizedPrintf(NULL, "EGRVK_KickingSamePlayer"), INFO_USER_ERROR, 0);
                return;
            }
            else
            {
                free(pastAttempts);
                eaRemove(&s_EndGameRaidKickTable.pastKickList, i);
                break;
            }
        }
    }

    //    if raid time > X min
    if (((int)(dbSecondsSince2000() - s_EndGameRaidKickTable.eventStartTime)) < (g_EndGameRaidKickMods.minRaidTimeMins *SECONDS_PER_MINUTE))
    {
        //    send error
        chatSendToPlayer(initiator_dbid, localizedPrintf(NULL, "EGRVK_MinRaidTime"), INFO_USER_ERROR, 0);
        return;
    }

    //    register a vote kick
    for (i = eaSize(&s_EndGameRaidKickTable.currentKickList) -1 ; i >= 0; --i)
    {
        EndGameRaidKickAttempts *currentAttempt = s_EndGameRaidKickTable.currentKickList[i];
        if (currentAttempt->kicked_dbid == kicked_dbid)
        {
            if (currentAttempt->initiator_dbid == initiator_dbid)
            {
                //    same person is spamming request
                chatSendToPlayer(initiator_dbid, localizedPrintf(NULL, "EGRVK_RequestRecievedNotSeconded"), INFO_USER_ERROR, 0);
                return;
            }
            else
            {
                //    if (registered vote kicks > 1)
                //        start vote kick
                eaRemove(&s_EndGameRaidKickTable.currentKickList, i);
                successfulKickRequest = currentAttempt;
                break;
            }
        }
    }

    if (successfulKickRequest)
    {
        EndGameRaidInitiateVoteKick(initiator, successfulKickRequest);
    }
    else
    {
        //    not enough to start a kick, store it in the list
        EndGameRaidKickAttempts *currentAttempt = malloc(sizeof(EndGameRaidKickAttempts));
        currentAttempt->initiator_dbid = initiator_dbid;
        currentAttempt->kicked_dbid = kicked_dbid;
        currentAttempt->lastKickTime = dbSecondsSince2000();
        eaPush(&s_EndGameRaidKickTable.currentKickList, currentAttempt);
        chatSendToPlayer(initiator_dbid, localizedPrintf(NULL, "EGRVK_RequestReceivedWaitingForSecond"), INFO_USER_ERROR, 0);
    }
}

// DGNOTE 9/10/2010 - refilling an event
// Call this every so often.  Probably fomr somewhere near the code that calls the script engine tick.  An easy cop-out would be to call
// this from ScriptedZoneEvent, but that's a hideously poor architectural choice
void EndGameRaidTick()
{
    if (db_state.is_endgame_raid)
    {
        if (s_EndGameRaidKickTable.lastKickTime && s_EndGameRaidKickTable.totalCount)
        {
            if (dbSecondsSince2000() > s_EndGameRaidKickTable.lastKickTime)
            {
                EndGameRaidCompleteVoteKick();
            }
        }
    }
}
