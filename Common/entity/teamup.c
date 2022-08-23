#include <utilitieslib/stdtypes.h>
#include "entity/teamup.h"
#include "entity/Supergroup.h"
#include <memory.h>
#include <utilitieslib/components/MemoryPool.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/stdtypes.h>
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "entity/RewardToken.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"



#ifdef SERVER
#include "storyarc/storyinfo.h"
#include "storyarc/storyarcprivate.h"
#include <utilitieslib/components/EString.h>
#include "entity/entityRef.h"
#endif

#ifdef SERVER
#define DEFAULT_SIZE 16
#else
#define DEFAULT_SIZE 1
#endif

MP_DEFINE(Teamup);

Teamup *createTeamup(void)
{
    MP_CREATE(Teamup, DEFAULT_SIZE);
    return MP_ALLOC(Teamup);
}

int team_IsMember( Entity* e, int db_id  )
{
    int i;

    if(!e || !e->teamup_id )
        return FALSE;

    for( i = 0; i < e->teamup->members.count; i++ )
    {
        if( db_id == e->teamup->members.ids[i] )
            return TRUE;
    }

    return FALSE;
}

#if defined(CLIENT) || defined(SERVER) || defined(TEST_CLIENT)
// Covers player teams or pets of players on player teams
bool entsAreTeamed( Entity* a, Entity* b, bool checkLeague )
{
    Entity *a_root = NULL;
    Entity *b_root = NULL;
    int a_db_id = 0;
    int b_db_id = 0;
    
    if(!a || !b)
        return false;

    if(a->erOwner)
    {
        a_root = erGetEnt(a->erOwner);
        a_db_id = erGetDbId(a->erOwner);
    }
    else
    {
        a_root = a;
        a_db_id = a->db_id;
    }

    if(b->erOwner)
    {
        b_root = erGetEnt(b->erOwner);
        b_db_id = erGetDbId(b->erOwner);
    }
    else
    {
        b_root = b;
        b_db_id = b->db_id;
    }

    if(a_db_id > 0 || b_db_id > 0) //player or player pet, check player teams which could be off map
    {
        if(a_db_id == b_db_id)
            return true;
        else if(a_root)
            return checkLeague ? league_IsMember(a_root, b_db_id) : team_IsMember(a_root, b_db_id); // a or a's owner is on map
        else if(b_root)
            return checkLeague ? league_IsMember(b_root, a_db_id) : team_IsMember(b_root, a_db_id); // b or b's owner is on map
        else
            return false; // in the case of both a's owner and b's owner off map, we need to fail as there's no possible teamup to check
    }

    return false;
}
#endif

void destroyTeamup(Teamup *teamup)
{
    if (!teamup)
        return;

    destroyTeamMembersContents(&teamup->members);

#ifdef SERVER
    eaiDestroy(&teamup->taskSelect.memberValid);
    eaClearEx(&teamup->activePlayerRewardTokens, rewardtoken_Destroy);
    storyTaskInfoDestroy(teamup->activetask);
#endif
    MP_FREE(Teamup, teamup);
}

void clearTeamup(Teamup *teamup)
{
#ifdef SERVER
    StoryTaskInfo* active = teamup->activetask;
    int saved_rotors[3];
    memcpy(saved_rotors, teamup->members.rotors, 3*sizeof(int));
    eaClearEx(&teamup->activePlayerRewardTokens, rewardtoken_Destroy);
#endif

    destroyTeamMembersContents(&teamup->members);
    memset(teamup, 0, sizeof(Teamup));

#ifdef SERVER
    memset(active, 0, sizeof(StoryTaskInfo));
    teamup->activetask = active;
    memcpy(teamup->members.rotors, saved_rotors, 3*sizeof(int));
#endif
}

// use areMembersAlignmentMixed in team.h for server-side checks
int isTeamupAlignmentMixedClientside(Teamup *teamup)
{
    Entity *teammate;
    int i;
    int alliance = -99;

    if (!teamup)
        return false;

    for (i = 0; i < teamup->members.count; i++)
    {
        teammate = entFromDbId(teamup->members.ids[i]);

        if (teammate && teammate->pl)
        {
            if (alliance < 0)
                alliance = teammate->pchar->playerTypeByLocation;
            else if (teammate->pchar->playerTypeByLocation != alliance)
                return true;
        }
        else
        {
            // can't tell, so err on the side of caution
            return true;
        }
    }

    return false;
}

void destroyTeamMembersContents(TeamMembers* members) {
    if (!members)
        return;
    SAFE_FREE(members->onMapserver);
    SAFE_FREE(members->mapIds);
    SAFE_FREE(members->oldmapIds);
    SAFE_FREE(members->ids);
    SAFE_FREE(members->names);
}


TaskForce *createTaskForce(void)
{
    return calloc(sizeof(TaskForce),1);
}

void destroyTaskForce(TaskForce *taskforce)
{
    if (!taskforce)
        return;
    destroyTeamMembersContents(&taskforce->members);
#ifdef SERVER
    storyInfoDestroy(taskforce->storyInfo);
    estrDestroy(&taskforce->playerCreatedArc);
#endif
    free(taskforce);
}

void clearTaskForce(TaskForce *taskforce)
{
    destroyTeamMembersContents(&taskforce->members);
#ifdef SERVER
    storyInfoDestroy(taskforce->storyInfo);
    estrDestroy(&taskforce->playerCreatedArc);
#endif
    memset(taskforce, 0, sizeof(TaskForce));
}

MP_DEFINE(LevelingPact);

LevelingPact *createLevelingpact()
{
    MP_CREATE(LevelingPact, DEFAULT_SIZE);
    return MP_ALLOC(LevelingPact);
}

void destroyLevelingpact(LevelingPact *levelingpact)
{
    if (!levelingpact)
        return;

    destroyTeamMembersContents(&levelingpact->members);

    MP_FREE(LevelingPact, levelingpact);
}

void clearLevelingPact(LevelingPact *levelingpact)
{
    destroyTeamMembersContents(&levelingpact->members);
    memset(levelingpact, 0, sizeof(LevelingPact));
}

MP_DEFINE(League);

League *createLeague(void)
{
    MP_CREATE(League, DEFAULT_SIZE);
    return MP_ALLOC(League);
}

int league_IsMember( Entity *e, int db_id )
{
    int i;

    if( !e || !e->league_id )
        return FALSE;

    devassert(e->league);
    if (e->league)
    {
        for( i = 0; i < e->league->members.count; i++ )
        {
            if( db_id == e->league->members.ids[i] )
                return TRUE;
        }
    }
    return FALSE;
}

void destroyLeague(League *league)
{
    if (league)
    {
        destroyTeamMembersContents(&league->members);
#if CLIENT
        {
            int i;
            for (i = 0; i < eaSize(&league->teamRoster); ++i)
            {
                eaiDestroy(&league->teamRoster[i]);
            }
        }
        eaDestroy(&league->teamRoster);
        eaiDestroy(&league->focusGroupRosterIndex);
#endif
        MP_FREE(League, league);
    }
}

void clearLeague(League *league)
{
    devassert(league);
    destroyTeamMembersContents(&league->members);
#if CLIENT
    {
        int i;
        for (i = 0; i < eaSize(&league->teamRoster); ++i)
        {
            eaiDestroy(&league->teamRoster[i]);
        }
    }
    eaDestroy(&league->teamRoster);
    eaiDestroy(&league->focusGroupRosterIndex);
#endif
    memset(league, 0, sizeof(League));
}

int league_isMemberTeamLocked(Entity *e, int db_id)
{
    if (!e)
        return 0;

    if ( e->league )
    {
        int i;
        for( i = 0; i < e->league->members.count; i++ )
        {
            if( db_id == e->league->members.ids[i] )
            {
                return e->league->lockStatus[i];
            }
        }
    }

    return 0;
}

int league_teamCount(League *league)
{
    if (league)
    {
        int *uniqueTeams = NULL;
        int retVal = 0;
        int i;
        for (i = 0; i < league->members.count; ++i)
        {
            int j;
            int matchFound = 0;
            devassert(league->teamLeaderIDs[i]);
            if (league->teamLeaderIDs[i])
            {
                for (j = 0; j < eaiSize(&uniqueTeams) && !matchFound; ++j)
                {
                    if (league->teamLeaderIDs[i] == uniqueTeams[j])
                    {
                        matchFound = 1;
                    }
                }
                if (!matchFound)
                {
                    eaiPush(&uniqueTeams, league->teamLeaderIDs[i]);
                }
            }
        }
        retVal = eaiSize(&uniqueTeams);
        eaiDestroy(&uniqueTeams);
        return retVal;
    }
    return 0;
}

int league_getTeamLeadId(League *league, int teamIndex)
{
    if (league)
    {
        int i;
        int *uniqueTeams = NULL;
        int retVal = 0;
        for (i = 0; i < league->members.count; ++i)
        {
            int j;
            int matchFound = 0;
            devassert(league->teamLeaderIDs[i]);
            if (league->teamLeaderIDs[i])
            {
                for (j = 0; j < eaiSize(&uniqueTeams) && !matchFound; ++j)
                {
                    if (league->teamLeaderIDs[i] == uniqueTeams[j])
                    {
                        matchFound = 1;
                    }
                }
                if (!matchFound)
                {
                    eaiPush(&uniqueTeams, league->teamLeaderIDs[i]);
                }
            }
        }
        retVal = (eaiSize(&uniqueTeams) > teamIndex) ? uniqueTeams[teamIndex] : 0;
        eaiDestroy(&uniqueTeams);
        return retVal;
    }
    return 0;
}

int league_getTeamMemberCount(League *league, int teamIndex)
{
    if (league)
    {
        int i;
        int *uniqueTeams = NULL;
        int *teamMemberCount = NULL;
        int retVal = 0;
        for (i = 0; i < league->members.count; ++i)
        {
            int j;
            int matchFound = 0;
            devassert(league->teamLeaderIDs[i]);
            if (league->teamLeaderIDs[i])
            {
                for (j = 0; j < eaiSize(&uniqueTeams) && !matchFound; ++j)
                {
                    if (league->teamLeaderIDs[i] == uniqueTeams[j])
                    {
                        matchFound = 1;
                        teamMemberCount[j]++;
                    }
                }
                if (!matchFound)
                {
                    eaiPush(&uniqueTeams, league->teamLeaderIDs[i]);
                    eaiPush(&teamMemberCount, 1);
                }
            }
        }
        retVal = (eaiSize(&teamMemberCount) > teamIndex) ? teamMemberCount[teamIndex] : 0;
        eaiDestroy(&teamMemberCount);
        eaiDestroy(&uniqueTeams);
        return retVal;
    }
    else
    {
        return 0;
    }
}



int league_getTeamLockStatus(League *league, int teamIndex)
{
    if (league)
    {
        int i;
        int *uniqueTeams = NULL;
        for (i = 0; i < league->members.count; ++i)
        {
            int j;
            int matchFound = 0;
            for (j = 0; j < eaiSize(&uniqueTeams) && !matchFound; ++j)
            {
                if (league->teamLeaderIDs[i] == uniqueTeams[j])
                {
                    matchFound = 1;
                }
            }
            if (!matchFound)
            {
                if (eaiSize(&uniqueTeams) == teamIndex)
                {
                    eaiDestroy(&uniqueTeams);
                    return league->lockStatus[i];
                }
                else
                {
                    eaiPush(&uniqueTeams, league->teamLeaderIDs[i]);
                }
            }
        }
        eaiDestroy(&uniqueTeams);
        devassert(0);    //    shouldn't get to here, it means we were given a bad index
        return 0;
    }
    return 0;
}