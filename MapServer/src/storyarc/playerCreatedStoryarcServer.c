#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/log.h>
#include "dbcomm/logcomm.h"
#include "storyarc/playerCreatedStoryarc.h"
#include "playerCreatedStoryarcServer.h"
#include "storyarcprivate.h"
#include "storyarc.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"
#include "entity/character_level.h"
#include <utilitieslib/utils/timing.h>
#include "gameComm/svr_chat.h"
#include "dbcomm/dbcomm.h"
#include "storyarc/missionMapCommon.h"
#include <utilitieslib/components/StashTable.h>
#include "Reward.h"
#include "cmdparse/cmdserver.h"
#include "entity/PCC_Critter.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include "player/badges_server.h"
#include "entity/character_inventory.h"
#include "entity/entGameActions.h"
#include "entity/CustomVillainGroup.h"
#include "entity/character_eval.h"
#include "playerCreatedStoryArcServer.h"
#include "comm_game.h"
#include "mission/MissionSearch.h"
#include "comm_backend.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/UtilsNew/meta.h>
#include "mission/missionserver_meta.h"
#include "entity/buddy_server.h"
#include <utilitieslib/language/AppLocale.h>
#include "TeamReward.h"
#include <utilitieslib/network/net_packetutil.h>
#include "container/team.h"
#include "missionServerMapTest.h"
#include "filter/validate_name.h"
#include "door.h"
#include <zlib/zlib.h>


typedef struct PlayerArc
{
    U32 uID;
    StoryArc *pArc;
    PlayerCreatedStoryArc *pPCArc;
    char * str;
    int inUse;
}PlayerArc;

StashTable stPlayerArcs;
static int s_devPublishTest = MAPSERVER_LOADTESTING;

static U32 s_authidFromName(char *name)
{
    U32 hash[4] = {0};
    if(name)
    {
        cryptMD5Init();
        cryptMD5Update(name, strlen(name));
        cryptMD5Final(hash);
    }
    return hash[0];
}

static U32 s_authid(Entity *e)
{
    if(e->auth_id)
    {
        return e->auth_id;
    }
    else
    {
        return s_authidFromName(e->auth_name);
    }
}

static void playerCreatedStoryArc_Stash( int id, char * str, StoryArc * pArc, PlayerCreatedStoryArc *pPCArc )
{
    PlayerArc *pStruct;

    if(!stPlayerArcs)
        stPlayerArcs = stashTableCreateInt(8);

    if(!stashIntFindPointer(stPlayerArcs, id, &pStruct))
    {
        pStruct = malloc(sizeof(PlayerArc));
        pStruct->uID = id;
        pStruct->str = strdup(str);
        pStruct->pArc = pArc;
        pStruct->pPCArc = pPCArc;
        pStruct->inUse = 1;
        stashIntAddPointer(stPlayerArcs, id, pStruct, 1);
    }
}



void playerCreatedStoryArc_Update( Entity *e )
{
    PlayerArc *pStruct;
    if( e && e->taskforce_id && stPlayerArcs && stashIntFindPointer(stPlayerArcs, e->taskforce_id, &pStruct) )
        pStruct->inUse = 1;
}

static void playerCreatedStoryArc_DeleteStruct(PlayerArc *pStruct)
{        
    extern TokenizerParseInfo ParseStoryArc[];

    stashIntRemovePointer( stPlayerArcs, pStruct->uID, 0 );
    SAFE_FREE(pStruct->str);
    StructDestroy(ParseStoryArc, pStruct->pArc );
    StructDestroy(ParsePlayerStoryArc, pStruct->pPCArc );
    SAFE_FREE(pStruct);
}

static int playerCreatedStoryArc_RemoveStruct(StashElement elem)
{
    PlayerArc *pStruct = (PlayerArc *)stashElementGetPointer(elem);

    if( !pStruct->inUse && !g_ArchitectTaskForce ) // don't remove player created arcs when we are on a mission map
    {
        playerCreatedStoryArc_DeleteStruct(pStruct);
    }
    else
        pStruct->inUse = 0;

    return 1;
}

void playerCreatedStoryArc_CleanStash(void)
{
    if(stPlayerArcs)
        stashForEachElement( stPlayerArcs, playerCreatedStoryArc_RemoveStruct );
}


StoryArc* playerCreatedStoryArc_GetStoryArc( int id )
{
    PlayerArc *pStruct;
    if(stPlayerArcs && stashIntFindPointer(stPlayerArcs, id, &pStruct))
    {
        return pStruct->pArc;
    }

    return 0;
}

PlayerCreatedStoryArc* playerCreatedStoryArc_GetPlayerCreatedStoryArc( int id )
{
    PlayerArc *pStruct;
    if(stPlayerArcs && stashIntFindPointer(stPlayerArcs, id, &pStruct))
    {
        return pStruct->pPCArc;
    }

    return 0;
}

bool playerCreatedStoryarc_Available(Entity *e)
{
    if( e && e->pl && e->taskforce_id )  
        return 1;
    else
        return 0;
}

int playerCreatedStoryarc_GetTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, ContactTaskPickInfo* pickinfo)
{
    StoryEpisode* episode;
    StoryArcInfo* storyarc;

    // see if we are on a story arc or can get on one
    storyarc = StoryArcGetInfo(info, contactInfo);
    if (!storyarc)
    {
        storyarc = StoryArcTryAdding(player, info, contactInfo, 1, &pickinfo->minLevel, 0);
    }
    if (!storyarc)
        return 0;    // not on story arc

    // select a task from the ones remaining in this episode
    episode = storyarc->def->episodes[storyarc->episode];
    pickinfo->taskDef = episode->tasks[0];
    pickinfo->sahandle.bPlayerCreated = 1;
    pickinfo->sahandle.context = storyarc->sahandle.context;
    pickinfo->sahandle.subhandle = storyarc->episode;
    return 1;
}


int playerCreatedStoryarc_TaskAdd(Entity *player, StoryContactInfo* contactInfo, ContactTaskPickInfo *pickInfo)
{
    int i, n, max;
    StoryInfo* info;
    StoryTaskInfo* task;

    if (!player) 
        return 0;

    info = StoryInfoFromPlayer(player);

    // check and see if we already have the tasks
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if (info->tasks[i]->def == pickInfo->taskDef)
            return 0;
    }
    if (n && info->taskforce) // if on a task force, we can only have a single task
        return 0;

    // otherwise add it
    max = StoryArcMaxTasks(info);
    if (n >= max)
    {
        log_printf("storyerrs.log", "TaskAdd: Attempted to add a task which would have put the player over the limit!\n");
        return 0;
    }

    task = storyTaskInfoAlloc();
    task->def = pickInfo->taskDef;
    task->compoundParent = 0;
    task->state = TASK_ASSIGNED;
    task->seed = 0;
    task->assignedDbId = player->db_id;
    task->assignedTime = dbSecondsSince2000();
    task->level = character_GetExperienceLevelExtern(player->pchar);
    difficultySetFromPlayer( &task->difficulty, player );
    task->sahandle.context = pickInfo->sahandle.context;
    task->sahandle.bPlayerCreated = 1;
    task->sahandle.compoundPos = 0;

    //handle random assignment of villain groups and map sets
    if (getVillainGroupFromTask(task->def) == villainGroupGetEnum("Random") || getMapSetFromTask(task->def)==MAPSET_RANDOM)
    {
        VillainGroupEnum vge;
        MapSetEnum mse;
        char * filename;
        getRandomFields(player,&vge,&mse,&filename,character_GetExperienceLevelExtern(player->pchar),task->def,0);
        task->villainGroup=vge;
        task->mapSet=mse;
    }

    eaPush(&info->tasks, task);

    // Update the contact from which the task was issued.
    ContactMarkDirty(player, info, contactInfo);
    TaskMarkIssued(info, contactInfo, task, 1);
    TaskMarkSelected(info, task);

    // check token count tasks at launch time to see if they have already been completed.
    if (task->def->type == TASK_TOKENCOUNT)
    {
        TokenCountTasksModify(player, task->def->tokenName);
    }

    if (TaskIsBroken(task))
    {
        TaskComplete(player, info, task, NULL, 1);
        chatSendToPlayer(player->db_id, saUtilLocalize(player, "TaskCompleteDueToError"), INFO_SVR_COM, 0);
    }
    return 1;
}

#define MISSIONSERVER_SEARCHREQUESTS_ALLOWED    4
#define MISSIONSERVER_SEARCHREQUESTS_TIMESPAN    1

#define s_searchRateLimiter(pl) (pl)?playerCreatedStoryArc_rateLimiter(pl->architectRecentSearches,\
    MISSIONSERVER_SEARCHREQUESTS_ALLOWED,\
    MISSIONSERVER_SEARCHREQUESTS_TIMESPAN,\
    dbSecondsSince2000()):0

#define MISSIONSERVER_PUBLISHREQUESTS_ALLOWED    4
#define MISSIONSERVER_PUBLISHREQUESTS_TIMESPAN    60

#define s_publishRateLimiter(pl) (pl)?playerCreatedStoryArc_rateLimiter(pl->architectRecentPublishes,\
    MISSIONSERVER_PUBLISHREQUESTS_ALLOWED, \
    MISSIONSERVER_PUBLISHREQUESTS_TIMESPAN,\
    dbSecondsSince2000()):0



typedef struct PlayerCreatedDetailToSpawnDefMapping
{
    char * varName;
    int    detailOffset;
    int decodeHTML;

}PlayerCreatedDetailToSpawnDefMapping;

PlayerCreatedDetailToSpawnDefMapping BossMappings[] = 
{
    { "VILLAIN_GROUP",            offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "BOSS_DISPLAYNAME",         offsetof(PlayerCreatedDetail, pchName), 1 },
    { "BOSS_DISPLAYINFO",          offsetof(PlayerCreatedDetail, pchDisplayInfo), },
    { "BOSS_INACTIVE_DIALOG",     offsetof(PlayerCreatedDetail, pchDialogInactive), },
    { "BOSS_ATTACK_DIALOG",       offsetof(PlayerCreatedDetail, pchDialogActive), },
    { "BOSS_1STBLOOD_DIALOG",     offsetof(PlayerCreatedDetail, pchDialogAttack), },
    { "BOSS_75HEALTH_DIALOG",     offsetof(PlayerCreatedDetail, pchDialog75), },
    { "BOSS_50HEALTH_DIALOG",     offsetof(PlayerCreatedDetail, pchDialog50), },
    { "BOSS_25HEALTH_DIALOG",     offsetof(PlayerCreatedDetail, pchDialog25), },
    { "BOSS_DEAD_DIALOG",          offsetof(PlayerCreatedDetail, pchDeath), },
    { "BOSS_HELD_DIALOG",          offsetof(PlayerCreatedDetail, pchHeld), },
    { "BOSS_FREED_DIALOG",          offsetof(PlayerCreatedDetail, pchFreed), },
    { "BOSS_AFRAID_DIALOG",          offsetof(PlayerCreatedDetail, pchAfraid), },
    { "BOSS_KILLEDHERO_DIALOG",   offsetof(PlayerCreatedDetail, pchKills), },
    { "BOSS_RUNAWAY_DIALOG",      offsetof(PlayerCreatedDetail, pchRunAway), },
    { "RunWhenHealthIsBelow",      offsetof(PlayerCreatedDetail, pchRunAwayHealthThreshold), },
    { "RunWhenTeamSizeIsBelow",   offsetof(PlayerCreatedDetail, pchRunAwayTeamThreshold), },
    { 0    },
};

PlayerCreatedDetailToSpawnDefMapping AmbushMappings[] = 
{
    { "VILLAIN_GROUP",            offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "WaveShout",                  offsetof(PlayerCreatedDetail, pchDialogActive), },
    { 0    },
};


PlayerCreatedDetailToSpawnDefMapping PatrolMappings[] = 
{
    { "VILLAIN_GROUP",                    offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "PATROLLER1_SEARCHING_DIALOG",    offsetof(PlayerCreatedDetail, pchPatroller1), },
    { "PATROLLER2_SEARCHING_DIALOG",    offsetof(PlayerCreatedDetail, pchPatroller2), },
    { 0    },
};

PlayerCreatedDetailToSpawnDefMapping DestroyObjectMappings[] = 
{
    { "VILLAIN_GROUP",               offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "OBJECT_DISPLAYNAME",          offsetof(PlayerCreatedDetail, pchName), 1 },
    { "OBJECT_INACTIVE_DIALOG",      offsetof(PlayerCreatedDetail, pchDialogInactive), },
    { "OBJECT_ACTIVE_DIALOG",        offsetof(PlayerCreatedDetail, pchDialogActive), },
    { "OBJECT_DISPLAYINFO",             offsetof(PlayerCreatedDetail, pchDisplayInfo), },
    { "BOSS_1STBLOOD_DIALOG",        offsetof(PlayerCreatedDetail, pchDialogAttack), },
    { "BOSS_75HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog75), },
    { "BOSS_50HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog50), },
    { "BOSS_25HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog25), },
    { "BOSS_DEAD_DIALOG",             offsetof(PlayerCreatedDetail, pchDeath), },
    { "BOSS_HELD_DIALOG",             offsetof(PlayerCreatedDetail, pchHeld), },
    { "BOSS_FREED_DIALOG",             offsetof(PlayerCreatedDetail, pchFreed), },
    { "BOSS_AFRAID_DIALOG",             offsetof(PlayerCreatedDetail, pchAfraid), },
    { "BOSS_KILLEDHERO_DIALOG",      offsetof(PlayerCreatedDetail, pchKills), },
    { "GUARD_INACTIVE_DIALOG",       offsetof(PlayerCreatedDetail, pchGuard1), },
    { "GUARD_ACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchGuard2), },
    { 0 },
};



PlayerCreatedDetailToSpawnDefMapping DefendObjectMappings[] = 
{
    { "VILLAIN_GROUP",               offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "OBJECT_DISPLAYNAME",          offsetof(PlayerCreatedDetail, pchName), 1 },
    { "OBJECT_DISPLAYINFO",             offsetof(PlayerCreatedDetail, pchDisplayInfo), },
    { "OBJECT_INACTIVE_DIALOG",      offsetof(PlayerCreatedDetail, pchDialogInactive), },
    { "OBJECT_ACTIVE_DIALOG",        offsetof(PlayerCreatedDetail, pchDialogActive), },
    { "BOSS_1STBLOOD_DIALOG",        offsetof(PlayerCreatedDetail, pchDialogAttack), },
    { "BOSS_75HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog75), },
    { "BOSS_50HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog50), },
    { "BOSS_25HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog25), },
    { "BOSS_DEAD_DIALOG",             offsetof(PlayerCreatedDetail, pchDeath), },
    { "BOSS_HELD_DIALOG",             offsetof(PlayerCreatedDetail, pchHeld), },
    { "BOSS_FREED_DIALOG",             offsetof(PlayerCreatedDetail, pchFreed), },
    { "BOSS_AFRAID_DIALOG",             offsetof(PlayerCreatedDetail, pchAfraid), },
    { "BOSS_KILLEDHERO_DIALOG",      offsetof(PlayerCreatedDetail, pchKills), },
    { "BOSS_RUNAWAY_DIALOG",         offsetof(PlayerCreatedDetail, pchRunAway), },
    { "ATTACKER1_ATTACKING_DIALOG",  offsetof(PlayerCreatedDetail, pchAttackerDialog1), },
    { "ATTACKER2_ATTACKING_DIALOG",  offsetof(PlayerCreatedDetail, pchAttackerDialog2), },
    { "ATTACKER1_HERO_ALERT_DIALOG", offsetof(PlayerCreatedDetail, pchAttackerHeroAlertDialog), },
    { 0    },
};

PlayerCreatedDetailToSpawnDefMapping RescueMappings[] = 
{
    { "VILLAIN_GROUP",               offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "CAPTOR_INACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchDialogInactive), },
    { "CAPTOR_ACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchDialogActive), },
    { "HOSTAGE_NAME",                 offsetof(PlayerCreatedDetail, pchName ),  1 },
    { "HOSTAGE_DISPLAYINFO",         offsetof(PlayerCreatedDetail, pchDisplayInfo), },
    { "HOSTAGE_INACTIVE_DIALOG",     offsetof(PlayerCreatedDetail, pchPersonInactive), },
    { "HOSTAGE_ACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchPersonActive), },

    { "SayOnRescue",                 offsetof(PlayerCreatedDetail, pchPersonSayOnRescue), },
    { "SayOnArrival",                offsetof(PlayerCreatedDetail, pchPersonSayOnArrival), },
    { "SayOnReRescue",               offsetof(PlayerCreatedDetail, pchPersonSayOnReRescue), },
    { "SayOnStranded",               offsetof(PlayerCreatedDetail, pchPersonSayOnStranded), },
    { "SayOnClickAfterRecapture",    offsetof(PlayerCreatedDetail, pchPersonSayOnClickAfterRescue), },
    { "SayOnClickAfterRescue",       offsetof(PlayerCreatedDetail, pchPersonSayOnClickAfterRescue), },

    { "RescueObjectiveText",         offsetof(PlayerCreatedDetail, pchObjectiveDescription), },
    { "RescueObjectiveTextSingular", offsetof(PlayerCreatedDetail, pchObjectiveSingularDescription), },

    { "BOSS_1STBLOOD_DIALOG",         offsetof(PlayerCreatedDetail, pchDialogAttack), },
    { "BOSS_75HEALTH_DIALOG",         offsetof(PlayerCreatedDetail, pchDialog75), },
    { "BOSS_50HEALTH_DIALOG",         offsetof(PlayerCreatedDetail, pchDialog50), },
    { "BOSS_25HEALTH_DIALOG",         offsetof(PlayerCreatedDetail, pchDialog25), },
    { "BOSS_DEAD_DIALOG",             offsetof(PlayerCreatedDetail, pchDeath), },
    { "BOSS_KILLEDHERO_DIALOG",      offsetof(PlayerCreatedDetail, pchKills), },

    { "BetrayalObjectiveComplete",    offsetof(PlayerCreatedDetail, pchBetrayalTrigger), },
    { "SayOnBetrayal",                offsetof(PlayerCreatedDetail, pchSayOnBetrayal), },

    { 0    },
};

PlayerCreatedDetailToSpawnDefMapping EscortMappings[] = 
{
    { "VILLAIN_GROUP",               offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "CAPTOR_INACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchDialogInactive), },
    { "CAPTOR_ACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchDialogActive), },
    { "HOSTAGE_NAME",                 offsetof(PlayerCreatedDetail, pchName ), 1 },
    { "HOSTAGE_DISPLAYINFO",         offsetof(PlayerCreatedDetail, pchDisplayInfo), },
    { "HOSTAGE_INACTIVE_DIALOG",     offsetof(PlayerCreatedDetail, pchPersonInactive), },
    { "HOSTAGE_ACTIVE_DIALOG",         offsetof(PlayerCreatedDetail, pchPersonActive), },
    { "PlaceToTakeRescuee",             offsetof(PlayerCreatedDetail, pchEscortDestination), },

    { "BOSS_1STBLOOD_DIALOG",         offsetof(PlayerCreatedDetail, pchDialogAttack), },
    { "BOSS_75HEALTH_DIALOG",        offsetof(PlayerCreatedDetail, pchDialog75), },
    { "BOSS_50HEALTH_DIALOG",         offsetof(PlayerCreatedDetail, pchDialog50), },
    { "BOSS_25HEALTH_DIALOG",         offsetof(PlayerCreatedDetail, pchDialog25), },
    { "BOSS_DEAD_DIALOG",             offsetof(PlayerCreatedDetail, pchDeath), },
    { "BOSS_KILLEDHERO_DIALOG",      offsetof(PlayerCreatedDetail, pchKills), },

    { "SayOnRescue",                 offsetof(PlayerCreatedDetail, pchPersonSayOnRescue), },
    { "SayOnArrival",                offsetof(PlayerCreatedDetail, pchPersonSayOnArrival), },
    { "SayOnRecapture",              offsetof(PlayerCreatedDetail, pchPersonSayOnReRescue), },
    { "SayOnReRescue",               offsetof(PlayerCreatedDetail, pchPersonSayOnReRescue), },
    { "SayOnStranded",               offsetof(PlayerCreatedDetail, pchPersonSayOnStranded), },
    { "SayOnClickAfterRecapture",    offsetof(PlayerCreatedDetail, pchPersonSayOnClickAfterRescue), },
    { "SayOnClickAfterRescue",       offsetof(PlayerCreatedDetail, pchPersonSayOnClickAfterRescue), },

    { "WayPointText",                offsetof(PlayerCreatedDetail, pchEscortWaypoint ), },
    { "StatusString",                offsetof(PlayerCreatedDetail, pchEscortStatus), },
    { "RescueObjectiveText",         offsetof(PlayerCreatedDetail, pchObjectiveDescription), },
    { "RescueObjectiveTextSingular", offsetof(PlayerCreatedDetail, pchObjectiveSingularDescription), },
    { "LeadToObjectiveText",         offsetof(PlayerCreatedDetail, pchEscortObjective), },
    { "LeadToObjectiveTextSingular", offsetof(PlayerCreatedDetail, pchEscortObjectiveSingular), },

    { "BetrayalObjectiveComplete",    offsetof(PlayerCreatedDetail, pchBetrayalTrigger), },
    { "SayOnBetrayal",                offsetof(PlayerCreatedDetail, pchSayOnBetrayal), },

    { 0    },
};

PlayerCreatedDetailToSpawnDefMapping RumbleMappings[] = 
{
    { "VILLAIN_GROUP_1",                offsetof(PlayerCreatedDetail, pchVillainGroupName), },
    { "VILLAIN_GROUP_2",                offsetof(PlayerCreatedDetail, pchVillainGroup2Name), },
    { "GROUP_1_INACTIVE",                offsetof(PlayerCreatedDetail, pchDialogInactive), },
    { "GROUP_2_INACTIVE",                offsetof(PlayerCreatedDetail, pchAttackerDialog1), },
    { "GROUP_1_ACTIVE",                    offsetof(PlayerCreatedDetail, pchDialogActive), },
    { "GROUP_2_ACTIVE",                    offsetof(PlayerCreatedDetail, pchAttackerHeroAlertDialog), },
    { 0    },
};



int placementMapping[] = { MISSION_ANY, MISSION_FRONT, MISSION_MIDDLE, MISSION_BACK };
int pacingMapping[] = { PACING_FLAT, PACING_BACKLOADED, PACING_SLOWRAMPUP, PACING_STAGGERED };


// OnRescue
//  DoNothing(AnimList(%s),Timer(5)),Gang(%i),
//        Aggressive
//      DoAttackAI(0),Combat(Untargetable,Invincible)
//        RunIntoDoor
//      // ONLY RESCUE (NO DELIVERY)
//      Wander
//      FleeAwayNearest
//      DoNothing(AnimList(%s))

// OnRerescue
//  DoNothing(AnimList(%s),Timer(5)),Gang(%i),
//        Aggressive
//      DoAttackAI(0),Combat(Untargetable,Invincible)
//      // ONLY RESCUE (NO DELIVERY)
//      FleeToNearestDoor
//      Wander
//      FleeAwayNearest

// OnStranded
//  DoNothing(AnimList(%s))

// OnArrive
// DoNothing(AnimList(%s),Timer(5)),Gang(%i),
//    Aggressive
//    DoNothing(AnimList(%s))
//    Wander
//    FleeToNearestDoor
//    FleeAwayNearest

static spawnAddVar( SpawnDef * pSpawn, char * varName, char * varValue )
{
    TokenizerParams *pParam = StructAllocRaw(sizeof(TokenizerParams));
    if( varValue && stricmp( varValue, "none" ) == 0 )
        varValue = 0;
    eaPush(&pParam->params, StructAllocString( varName ));
    eaPush(&pParam->params, StructAllocString( "=" ));
    eaPush(&pParam->params, StructAllocString( varValue?varValue:"" ));
    eaPush(&pSpawn->vs.vars, pParam);
}

char * combatTypes[] = 
{
    "DoAttackAI(0), DoNotUseAnyPowers(1), Untargetable, Invincible,",    //    If you add any thing here, make sure to disable it when a betrayal happens
    "DoNotUseAnyPowers(1)",
    "CombatOverride(Aggressive)",
    "Pet.Follow(CombatOverride(Defensive))",
};

char * defaultBehaviorStrings[] = 
{
    "", // Follow - Script Controlled
    "FleeToNearestDoor",
    "RunAwayFar(Timer(90),Radius(300)),DestroyMe",
    "WanderAround",
    "Combat(DoNotMove)", // Do Nothing
};

static void spawnSetUpAI( SpawnDef * pSpawn, PlayerCreatedDetail * pDetail )
{
    char * behavior, *behaviorStr, *combat = combatTypes[pDetail->ePersonCombatType];
    int gang; 

    if( pDetail->eAlignment == kAlignment_Enemy ) // normal
        gang = ARCHITECT_GANG_ALLY;
    if( pDetail->eAlignment == kAlignment_Ally)
        gang = ARCHITECT_GANG_ENEMY;
    else if ( pDetail->eAlignment == kAlignment_Rogue )
        gang = ARCHITECT_GANG_ROGUE;


    if( pDetail->eDetailType == kDetail_Rescue )
    {
        char * rescueBehavior;

        estrCreate(&rescueBehavior);
        estrConcatf(&rescueBehavior, "%s,%s", combatTypes[0], defaultBehaviorStrings[1] );

        if( pDetail->pchRescueAnimation )
        {
            estrCreate(&behavior); 
            estrConcatf(&behavior, "DoNothing(AnimList(%s),Timer(5)),Gang(%i),%s,%s", pDetail->pchRescueAnimation, gang, combatTypes[0], defaultBehaviorStrings[1] );
            spawnAddVar( pSpawn, "PL_OnRescue", behavior );
            estrClear(&behavior);
        }
        else
            spawnAddVar( pSpawn, "PL_OnRescue", rescueBehavior );

        spawnAddVar( pSpawn, "PlaceToTakeRescuee", "marker:" );
        spawnAddVar( pSpawn, "PL_OnStranded", rescueBehavior );
        spawnAddVar( pSpawn, "PL_OnReRescue", rescueBehavior );
        spawnAddVar( pSpawn, "PL_OnReCapture", rescueBehavior );
        spawnAddVar( pSpawn, "PL_OnArrival", rescueBehavior );

        estrDestroy(&rescueBehavior);

        return;
    }

    if( pDetail->eDetailType == kDetail_Escort )
    {
        if( pDetail->pchEscortDestination && stricmp(pDetail->pchEscortDestination,"none") != 0 )
        {
            char buff[256];
            sprintf_safe(buff, 255, "objective:%s", pDetail->pchEscortDestination );
            //sprintf_safe(buff, 255, "spawndef:%s", pDetail->pchEscortDestination );
            spawnAddVar( pSpawn, "PlaceToTakeRescuee", buff );
        }
        else
            spawnAddVar( pSpawn, "PlaceToTakeRescuee", "Door:MissionReturn" );
        behaviorStr = defaultBehaviorStrings[0];
    }
    else
    {
        spawnAddVar( pSpawn, "PlaceToTakeRescuee", "marker:" );
        behaviorStr = defaultBehaviorStrings[pDetail->ePersonRescuedBehavior]; // can only chose this for allies
    }

    estrCreate(&behavior);  

    // On Rescue, ReRescue
    if( pDetail->pchRescueAnimation )
        estrConcatf(&behavior, "DoNothing(AnimList(%s),Timer(5)),Gang(%i),%s,%s", pDetail->pchRescueAnimation, gang, combat, behaviorStr );
    else
        estrConcatf(&behavior, "Gang(%i),%s,%s", gang, combat, behaviorStr );
    spawnAddVar( pSpawn, "PL_OnRescue", behavior );
    estrClear(&behavior);

    if( pDetail->ePersonRescuedBehavior == kPersonBehavior_Nothing )
        estrConcatf(&behavior, "DoNothing(AnimList(%s),Timer(5)),Gang(%i),%s", pDetail->pchRescueAnimation?pDetail->pchRescueAnimation:"None", gang, combat);
    else if( pDetail->pchRescueAnimation )
        estrConcatf(&behavior, "DoNothing(AnimList(%s),Timer(5)),Gang(%i),%s,%s", pDetail->pchRescueAnimation, gang, combat, behaviorStr );
    else
        estrConcatf(&behavior, "Gang(%i),%s,%s", gang, combat, behaviorStr );
    spawnAddVar( pSpawn, "PL_OnReRescue", behavior );
    estrClear(&behavior);

    // On Stranded
    estrConcatf(&behavior, "DoNothing(AnimList(%s), DoNotUseAnyPowers(1), Invincible)", pDetail->pchStrandedAnimation?pDetail->pchStrandedAnimation:"None" );
    spawnAddVar( pSpawn, "PL_OnStranded", behavior );
    estrClear(&behavior);

    // On RecaptureStranded
    {
        char * pchAnimList = "HandsOnHead_Kneel";
        if( pDetail->pchPersonStartingAnimation && !stricmp(pDetail->pchPersonStartingAnimation, "None") == 0  )
            pchAnimList = pDetail->pchPersonStartingAnimation;
        estrConcatf(&behavior, "DoNothing(AnimList(%s),Untargetable,Invincible,DoNotUseAnyPowers(1),)", pchAnimList );
        spawnAddVar( pSpawn, "PL_OnReCapture", behavior );
        estrClear(&behavior);
    }

    // On Arrive
    behaviorStr = defaultBehaviorStrings[pDetail->ePersonArrivedBehavior];
    if( pDetail->pchArrivalAnimation )
        estrConcatf(&behavior, "DoNothing(AnimList(%s),Timer(5)),Gang(%i),%s, %s", pDetail->pchArrivalAnimation?pDetail->pchArrivalAnimation:"None", gang, combat, behaviorStr );
    else
        estrConcatf(&behavior, "Gang(%i),%s, %s", gang, combat, behaviorStr );
    spawnAddVar( pSpawn, "PL_OnArrival", behavior );
    estrDestroy(&behavior);
}

static void spawnAddVars( SpawnDef * pSpawn, PlayerCreatedDetail * pDetail, PlayerCreatedDetailToSpawnDefMapping * mapping, int addAmbushObjectives )
{
    while( mapping->varName )
    {
        int i, skip=0;
        char * value = "";

        // don't add existing varibles (not sure it even matters, but maybe we'll want to overwrite here later)
        for( i=eaSize(&pSpawn->vs.vars)-1; i>=0; i-- )
        {
            if( stricmp(pSpawn->vs.vars[0]->params[0], mapping->varName ) == 0 )
                skip = 1;
        }

        if(!skip)
        {
            value = (*(char**)((char*)pDetail+mapping->detailOffset));

            if( mapping->decodeHTML )
                spawnAddVar( pSpawn, smf_DecodeAllCharactersGet(mapping->varName), value );    
            else
                spawnAddVar( pSpawn, mapping->varName, value );
        }
        mapping++;
    }

    if( addAmbushObjectives )
    {
        char * estr;

        estrCreate(&estr);
        estrConcatf(&estr, "%s 75_HEALTH", smf_DecodeAllCharactersGet(pDetail->pchName) );
        spawnAddVar( pSpawn, "BOSS_75HEALTH_Objective", estr );

        estrClear(&estr);
        estrConcatf(&estr, "%s 50_HEALTH", smf_DecodeAllCharactersGet(pDetail->pchName) );
        spawnAddVar( pSpawn, "BOSS_50HEALTH_Objective", estr );

        estrClear(&estr);
        estrConcatf(&estr, "%s 25_HEALTH", smf_DecodeAllCharactersGet(pDetail->pchName) );
        spawnAddVar( pSpawn, "BOSS_25HEALTH_Objective", estr );

        estrDestroy(&estr);
    }
}

static int getSpawnDef( SpawnDef *pSpawn, char * pchFile, char * pchExact, int difficulty )
{
    char spawnFile[MAX_PATH];
    const SpawnDef* pInclude;

    if(pchExact)
        pInclude = SpawnDefFromFilename( pchExact );
    else
    {
        sprintf( spawnFile, "Player_Created/%s_V%i.spawndef", pchFile, difficulty );
        pInclude = SpawnDefFromFilename( spawnFile );

        if(!pInclude) // try default difficulty
        {
            Errorf( "Boss SpawnInclude  Difficulty %i Not Found", difficulty );
            sprintf( spawnFile, "Player_Created/%s_V0.spawndef", pchFile );
            pInclude = SpawnDefFromFilename( spawnFile  );
        }
    }

    if( pInclude ) 
    {
        StructCopy(ParseSpawnDef, pInclude, pSpawn,0,0);
        return 1;
    }

    Errorf( "%s SpawnInclude Not Found", pchFile );
    return 0;
}

void playerCreatedStoryArc_SetContact( PlayerCreatedStoryArc *pArc, TaskForce * pTaskforce )
{
    pTaskforce->architect_contact_costume = NULL;
    pTaskforce->architect_contact_npc_costume = NULL;
    if( pArc->eContactType == kContactType_Default )
    {
        npcFindByName("Mission_Arch_Hologram_01", &pTaskforce->architect_contact_override);
    }
    else if( pArc->eContactType == kContactType_Contact || pArc->eContactType == kContactType_Critter || pArc->eContactType == kContactType_Object  )
    {
        if (pArc->customNPCCostumeId)
        {
            if (EAINRANGE(pArc->customNPCCostumeId-1, pArc->ppNPCCostumes) && validateCustomNPCCostume(pArc->ppNPCCostumes[pArc->customNPCCostumeId-1]) )
            {
                pTaskforce->architect_contact_npc_costume = pArc->ppNPCCostumes[pArc->customNPCCostumeId-1];
                pTaskforce->architect_useCachedCostume = 0;
            }
        }
        pTaskforce->architect_contact_override = playerCreatedStoryArc_NPCIdByCostumeIdx(pArc->pchContact, pArc->costumeIdx, 0 );
    }
    else if(pArc->customContactId)
    {
        if( EAINRANGE( pArc->customContactId-1, pArc->ppCustomCritters ) )
        {
            npcFindByName("Mission_Arch_Hologram_01", &pTaskforce->architect_contact_override);
            playerCreatedStoryArc_PCCSetCritterCostume( pArc->ppCustomCritters[pArc->customContactId-1], pArc );
            pTaskforce->architect_contact_costume = pArc->ppCustomCritters[pArc->customContactId-1]->pccCostume;
            pTaskforce->architect_useCachedCostume = 0;
            strncpyt(pTaskforce->architect_contact_name, pArc->ppCustomCritters[pArc->customContactId-1]->name, 31);
        }
    }
    if( pArc->pchContactDisplayName )
        strncpyt(pTaskforce->architect_contact_name, smf_DecodeAllCharactersGet(pArc->pchContactDisplayName), 31);
}

static void storyArcAddClue( StoryArc *pArc, char *pchName, char *pchDisplayName, char *pchText  )
{
    StoryClue * pClue = StructAllocRaw(sizeof(StoryClue));
    pClue->name = StructAllocString(pchName);

    if( pchDisplayName )
        pClue->displayname = StructAllocString(pchDisplayName);
    if( pchText )
        pClue->detailtext = StructAllocString(pchText);

    //filename
    //introstring
    //iconfile

    eaPush( &pArc->clues, pClue );
}

PlayerCreatedStoryArc * playerCreatedStoryarc_Add(char *str, int id, int validate, char **failErrors, char **playableErrors, int *hasErrors)
{
    PlayerCreatedStoryArc *pPCArc;
    StoryArc *pArc;
    PlayerArc *pStruct;
    StoryTask *pLastTask = 0;
    int i, j;

    if( stPlayerArcs && stashIntFindPointer(stPlayerArcs, id, &pStruct) )
    {
        return pStruct->pPCArc;
    }

    pPCArc = playerCreatedStoryArc_FromString(str, NULL, validate, failErrors, playableErrors, hasErrors);
    if(!pPCArc )
        return 0;

    pArc = StructAllocRaw(sizeof(StoryArc));
    pArc->name = StructAllocString(pPCArc->pchName);
    pArc->uID = id;

    if( pPCArc->pchAboutContact && strlen(pPCArc->pchAboutContact) )
        pArc->architectAboutContact = StructAllocString( pPCArc->pchAboutContact );

    // Each Episode contains one Story Task
    for( i = 0; i < eaSize(&pPCArc->ppMissions); i++ )
    {
        StoryEpisode * pEpisode = StructAllocRaw(sizeof(StoryEpisode));
        StoryTask *pTask = StructAllocRaw(sizeof(StoryTask));
        PlayerCreatedMission *pPCMission = pPCArc->ppMissions[i];
        MissionDef * pMission = StructAllocRaw(sizeof(MissionDef));
        ScriptDef * pScript = StructAllocRaw(sizeof(ScriptDef));
        StoryReward *pRewardSuccess;

        if( pPCMission->pchStartClueName || pPCMission->pchStartClueDesc )
        {
            char * pchClueName;
            StoryReward *pStartClue = StructAllocRaw(sizeof(StoryReward));
            estrCreate(&pchClueName);
            estrConcatf(&pchClueName, "MissionClueStart_%i", i );
            storyArcAddClue( pArc, pchClueName,  pPCMission->pchStartClueName, pPCMission->pchStartClueDesc );
            eaPushConst(&pStartClue->addclues, StructAllocString(pchClueName) );
            estrDestroy(&pchClueName);
            eaPushConst(&pTask->taskBegin, pStartClue);
        }

        pTask->logicalname = StructAllocString(pPCMission->pchTitle);
        pTask->type = TASK_MISSION;
        pTask->flags = TASK_PLAYERCREATED;

        pTask->minPlayerLevel = pPCMission->iMinLevel?pPCMission->iMinLevel-1:pPCMission->iMinLevelAuto-1;
        pTask->maxPlayerLevel = pPCMission->iMaxLevel?pPCMission->iMaxLevel-1:pPCMission->iMaxLevelAuto-1;

        if( pTask->minPlayerLevel > pTask->maxPlayerLevel )
        {
            if(pPCMission->iMinLevel)
                pTask->maxPlayerLevel = pTask->minPlayerLevel;
            else
                pTask->minPlayerLevel = pTask->maxPlayerLevel;
        }

        pTask->villainGroup = villainGroupGetEnum(pPCMission->pchVillainGroupName);
        pTask->villainGroupType = VGT_ANY;

        pTask->taskTitle = StructAllocString(pPCMission->pchTitle);
        if(pPCMission->pchSubTitle)
            pTask->taskSubTitle = StructAllocString(pPCMission->pchSubTitle);            

        if(pPCMission->pchIntroDialog)
            pTask->detail_intro_text = StructAllocString(pPCMission->pchIntroDialog);
        if(pPCMission->pchActiveTask)
            pTask->headline_text = StructAllocString(pPCMission->pchActiveTask);
        if(pPCMission->pchAcceptDialog)
            pTask->yes_text = StructAllocString(pPCMission->pchAcceptDialog);
        if(pPCMission->pchDeclineDialog)
            pTask->no_text = StructAllocString(pPCMission->pchDeclineDialog);
        if(pPCMission->pchSendOff)
            pTask->sendoff_text = StructAllocString(pPCMission->pchSendOff);    
        //pTask->decline_sendoff_text = StructAllocString(pPCMission->pchDeclineSendOff);    

        if(pPCMission->pchActiveTask)
            pTask->inprogress_description = StructAllocString(pPCMission->pchActiveTask);
        else
            pTask->inprogress_description = StructAllocString("RequiredString");


        if(pPCMission->pchTaskSuccess)
        {
            pTask->taskComplete_description = StructAllocString(localizedPrintf(0,"FloatMissionComplete"));
            pMission->exittextsuccess = StructAllocString(pPCMission->pchTaskSuccess);
        }
        if(pPCMission->pchTaskFailed)
        {
            pTask->taskFailed_description = StructAllocString(localizedPrintf(0,"FloatMissionFailed")); 
            pMission->exittextfail = StructAllocString(pPCMission->pchTaskFailed);
        }
        if(pPCMission->pchStillBusyDialog)
            pTask->busy_text = StructAllocString(pPCMission->pchStillBusyDialog);                
        pTask->timeoutMins =  pPCMission->fTimeToComplete;

        // MISSION
        if( pPCMission->pchMapName && stricmp( "Random", pPCMission->pchMapName) != 0 )
            pMission->mapfile = StructAllocString(pPCMission->pchMapName);
        else
        {
            pMission->mapset = StaticDefineIntGetInt(ParseMapSetEnum, pPCMission->pchMapSet);
            pMission->maplength = pPCMission->eMapLength;    
        }

        pMission->fFogDist = pPCMission->fFogDist;
        if( pPCMission->pchFogColor)
        {
            char * temp = strdup( pPCMission->pchFogColor );
            pMission->fogColor[0] = atof( strtok( temp, " " ) );
            pMission->fogColor[1] = atof( strtok( NULL, " " ) );
            pMission->fogColor[2] = atof( strtok( NULL, " " ) );
            SAFE_FREE(temp);
        }

        pMission->showCritterThreshold = 1;
        pMission->showItemThreshold = 1;

        pMission->cityzone = StructAllocString("WhereIssued");
        pMission->doortype = StructAllocString("architect"); // architect  
        if(pPCMission->pchEnterMissionDialog)
            pMission->entrytext = StructAllocString(pPCMission->pchEnterMissionDialog);            
        if(pPCMission->pchVillainGroupName)
            pMission->villaingroup = StructAllocString(pPCMission->pchVillainGroupName);

        pMission->villainpacing = pacingMapping[pPCMission->ePacing];    
        pMission->missionLevel = -1;         
        pMission->villainGroupType = VGT_ANY;    

        pMission->CVGIdx = pPCMission->CVGIdx;

        pMission->flags = MISSION_COEDTEAMSALLOWED;
        if(  pPCMission->bNoTeleportExit )
            pMission->flags |= MISSION_NOTELEPORTONCOMPLETE;
        pScript->scriptname = StructAllocString("PlayTogetherMission");
        eaPushConst(&pMission->scripts, pScript);

        // objectives
        for( j = 0; j < eaSize(&pPCMission->ppDetail); j++ )
        {
            PlayerCreatedDetail * pDetail = pPCMission->ppDetail[j];
            StoryReward * pReward;
            int k, bRequired = 0;

            for( k = 0; k < eaSize(&pPCMission->ppMissionCompleteDetails); k++ )
            {
                if( stricmp( pDetail->pchName,  pPCMission->ppMissionCompleteDetails[k] ) == 0 )
                    bRequired = 1;
            }

            if( pDetail->eDetailType == kDetail_Collection )
            {
                MissionObjective *pObjective = StructAllocRaw(sizeof(MissionObjective));

                pObjective->name = StructAllocString(pDetail->pchName);
                pObjective->modelDisplayName = StructAllocString(pDetail->pchName);

                if( pDetail->pchObjectiveDescription )
                    pObjective->description = StructAllocString(pDetail->pchObjectiveDescription);
                else if( pDetail->pchObjectiveSingularDescription )
                    pObjective->description = StructAllocString(pDetail->pchObjectiveSingularDescription);
                else if( bRequired )
                    pObjective->description = StructAllocString(pDetail->pchName);

                if( pDetail->pchObjectiveSingularDescription )
                    pObjective->singulardescription = StructAllocString(pDetail->pchObjectiveSingularDescription);
                else if( bRequired )
                    pObjective->description = StructAllocString(pDetail->pchName);

                pObjective->model = StructAllocString(pDetail->pchVillainDef);
                pObjective->count = pDetail->iQty;
                pObjective->room = placementMapping[pDetail->ePlacement];

                pObjective->locationtype = pDetail->eObjectLocation; //LOCATION_ITEMFLOOR;
                pObjective->interactOutcome = pDetail->bObjectiveRemove;
                pObjective->interactDelay = pDetail->iInteractTime;
                if( pDetail->bObjectiveFadein )
                    pObjective->effectInactive = StructAllocString("ObjectiveObjectFx/ghosted.fx");

                if(pDetail->pchBeginText) 
                    pObjective->interactBeginString = StructAllocString(pDetail->pchBeginText);
                if(pDetail->pchInterruptText)
                    pObjective->interactInterruptedString = StructAllocString(pDetail->pchInterruptText);
                if(pDetail->pchCompleteText)
                    pObjective->interactCompleteString = StructAllocString(pDetail->pchCompleteText);
                if(pDetail->pchInteractBarText)
                    pObjective->interactActionString = StructAllocString(pDetail->pchInteractBarText);
                if(pDetail->pchBeginText)
                    pObjective->interactWaitingString = StructAllocString(pDetail->pchBeginText);
                if(pDetail->pchBeginText)
                    pObjective->interactResetString = StructAllocString(pDetail->pchBeginText);

                if( pDetail->pchDetailTrigger )
                {
                    eaPush(&pObjective->activateRequires, StructAllocString( pDetail->pchDetailTrigger ) );
                    eaPush(&pObjective->descRequires, StructAllocString( pDetail->pchDetailTrigger ) );
                }

                pReward = StructAllocRaw(sizeof(StoryReward));

                if( bRequired )
                    pReward->architectBadgeStat = StructAllocString("Architect.Click");
                else
                    pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                pReward->architectBadgeStatTest = StructAllocString("Architect.ClickTest");

                if(    pDetail->pchRewardDescription )
                    pReward->rewardDialog = StructAllocString(pDetail->pchRewardDescription);
                if(pDetail->pchClueName || pDetail->pchClueDesc )
                {
                    char * pchClueName;
                    estrCreate(&pchClueName);
                    estrConcatf(&pchClueName, "DetailClue_%i_%i", i, j );
                    storyArcAddClue( pArc, pchClueName,  pDetail->pchClueName, pDetail->pchClueDesc );
                    eaPushConst(&pReward->addclues, StructAllocString(pchClueName) );
                    estrDestroy(&pchClueName);
                }

                eaPush( &pObjective->reward, pReward );
                eaPushConst( &pMission->objectives, pObjective );
            }
            else
            {
                MissionObjective *pObjective = StructAllocRaw(sizeof(MissionObjective));
                StoryReward * pReward = StructAllocRaw(sizeof(StoryReward));
                SpawnDef *pSpawn =  StructAllocRaw(sizeof(SpawnDef));
                int k;
                int vg_match = 0;

                
                if( pDetail->eDetailType == kDetail_Boss )
                {
                    devassert(getSpawnDef( pSpawn, "Boss", 0, pDetail->eDifficulty ));

                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);
                    pSpawn->logicalName = StructAllocString(pDetail->pchName);  

                    if( !pDetail->pchVillainGroupName && pDetail->pchEntityGroupName && 
                        (pDetail->customCritterId==0) && stricmp(pDetail->pchEntityGroupName, DOPPEL_GROUP_NAME) )
                    {
                        //    custom critters and doppels have custom villain groups so don't use them
                        pDetail->pchVillainGroupName = StructAllocString( pDetail->pchEntityGroupName );
                        pDetail->iVillainSet = pDetail->iEntityVillainSet;
                    }
                    spawnAddVars( pSpawn, pDetail, BossMappings, 1 );

                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                    else
                        pReward->architectBadgeStat = StructAllocString("Architect.Boss");
                    pReward->architectBadgeStatTest = StructAllocString("Architect.TestBoss");
                }
                else if( pDetail->eDetailType == kDetail_GiantMonster )
                {
                    devassert(getSpawnDef( pSpawn, "Boss", 0, pDetail->eDifficulty ));

                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);
                    pSpawn->logicalName = StructAllocString(pDetail->pchName);  
                    pSpawn->spawntype = StructAllocString("GiantMonster");
                    if( !pDetail->pchVillainGroupName && pDetail->pchEntityGroupName &&
                        (pDetail->customCritterId==0)  && stricmp(pDetail->pchEntityGroupName, DOPPEL_GROUP_NAME) )
                    {
                        //    custom critters and doppels have custom villain groups so don't use them
                        pDetail->pchVillainGroupName = StructAllocString( pDetail->pchEntityGroupName );
                    }
                    spawnAddVars( pSpawn, pDetail, BossMappings, 1 );
                }
                else if( pDetail->eDetailType == kDetail_Ambush )  // have to make two spawns here, one is the trigger, the other is the wave attack
                {
                    SpawnDef *pTrigger =  StructAllocRaw(sizeof(SpawnDef));
                    char nameBuff[MAX_PATH];

                    devassert(getSpawnDef( pTrigger, 0, "Player_Created/Ambush_Trigger_V0.spawndef", 0 ));
                    devassert(getSpawnDef( pSpawn, "Ambush", 0, pDetail->eDifficulty ));

                    pSpawn->logicalName = StructAllocString(pDetail->pchName); 
                    spawnAddVars( pSpawn, pDetail, AmbushMappings, 0 );
                    pSpawn->missionplace = MISSION_SCRIPTCONTROLLED;
                    pSpawn->missioncount = 1;
                    pSpawn->playerCreatedDetailType = 0;            //    exception because ambush's can be double upped on one location.

                    pTrigger->logicalName = StructAllocString("AmbushTrigger");
                    sprintf(nameBuff, "MissionEncounter:%s", pDetail->pchName );
                    spawnAddVar( pTrigger, "SpawnDefWave1", nameBuff );

                    if( pDetail->pchDialogActive )
                        spawnAddVar( pTrigger, "WaveShout", pDetail->pchDialogActive );
                    pTrigger->flags |= SPAWNDEF_AUTOSTART;
                    if( pDetail->pchDetailTrigger )
                        eaPushConst(&pTrigger->activateOnObjectiveComplete, StructAllocString( pDetail->pchDetailTrigger ) );
                    eaPushConst( &pMission->spawndefs, pTrigger );
                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                }
                else if(pDetail->eDetailType == kDetail_Patrol)
                {
                    devassert(getSpawnDef( pSpawn, "Patrol", 0, pDetail->eDifficulty ));

                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);
                    pSpawn->logicalName = StructAllocString("BasicPatrol"); 
                    spawnAddVars( pSpawn, pDetail, PatrolMappings, 0 );
                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                }
                else if(pDetail->eDetailType == kDetail_DestructObject)
                {
                    devassert(getSpawnDef( pSpawn, "Destroy", 0, pDetail->eDifficulty ));
                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);
                    pSpawn->logicalName = StructAllocString(pDetail->pchName);  
                    spawnAddVars( pSpawn, pDetail, DestroyObjectMappings, 1 );

                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                    else
                        pReward->architectBadgeStat = StructAllocString("Architect.Destroy");
                    pReward->architectBadgeStatTest = StructAllocString("Architect.TestDestroy");
                }
                else if(pDetail->eDetailType == kDetail_DefendObject)
                {
                    SpawnDef * pAmbush = StructAllocRaw(sizeof(SpawnDef));
                    char nameBuff[MAX_PATH];
                    devassert(getSpawnDef( pSpawn, "Defend", 0, pDetail->eDifficulty ));
                    devassert(getSpawnDef( pAmbush, "Ambush", 0, pDetail->eDifficulty ));

                    sprintf(nameBuff, "DefendAmbush%s", pDetail->pchName);
                    pAmbush->logicalName = StructAllocString(nameBuff); 
                    spawnAddVars( pAmbush, pDetail, AmbushMappings, 0 );
                    pAmbush->missionplace = MISSION_SCRIPTCONTROLLED;
                    pAmbush->missioncount = 1;
                    eaPushConst( &pMission->spawndefs, pAmbush );

                    pSpawn->spawntype = StructAllocString("Around"); 
                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);
                    pSpawn->logicalName = StructAllocString(pDetail->pchName); 

                    sprintf(nameBuff, "MissionEncounter:DefendAmbush%s", pDetail->pchName);
                    spawnAddVar( pSpawn, "SpawnDefWave1", nameBuff );
                    spawnAddVars( pSpawn, pDetail, DefendObjectMappings, 0 );

                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                    pReward->architectBadgeStat = StructAllocString("Architect.Defend");
                    pReward->architectBadgeStatTest = StructAllocString("Architect.TestDefend");
                }
                else if(pDetail->eDetailType == kDetail_Rumble)
                {
                    devassert(getSpawnDef( pSpawn, "Rumble", 0, pDetail->eDifficulty ));
                    pSpawn->spawntype = StructAllocString("Around"); 
                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);

                    pSpawn->logicalName = StructAllocString("Rumble"); 
                    spawnAddVars( pSpawn, pDetail, RumbleMappings, 0 );
                    pSpawn->CVGIdx2 = pDetail->CVGIdx2;
                    if( stricmp( pDetail->pchVillainGroupName, pPCMission->pchVillainGroupName ) == 0 )
                        vg_match = 1;
                    else if ( stricmp( pDetail->pchVillainGroup2Name, pPCMission->pchVillainGroupName ) == 0 )
                        vg_match = 2;

                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                }
                else if(pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Rescue || pDetail->eDetailType == kDetail_Ally)
                {

                    devassert(getSpawnDef( pSpawn, "Person", 0, pDetail->eDifficulty ));
                    pSpawn->spawntype = StructAllocString("Around"); 
                    pSpawn->missionplace = placementMapping[pDetail->ePlacement];
                    pSpawn->missioncount = MAX(1,pDetail->iQty);

                    spawnSetUpAI(pSpawn, pDetail);
                    spawnAddVar( pSpawn, "AutoWaveAttackCount", "0" ); // Disable ambushes 
                    if( pDetail->eDetailType == kDetail_Escort )
                    {
                        char ambushTriggerBuff[MAX_PATH];
                        sprintf(ambushTriggerBuff, "%s Rescue", pDetail->pchName);

                        spawnAddVars( pSpawn, pDetail, EscortMappings, 0 );
                        spawnAddVar( pSpawn, "SucceedOnRescueOrDelivery", "Delivery" );
                        spawnAddVar( pSpawn, "RescueObjective", ambushTriggerBuff );
                    }
                    else
                    {
                        spawnAddVars( pSpawn, pDetail, RescueMappings, 0 );
                        spawnAddVar( pSpawn, "SucceedOnRescueOrDelivery", "Rescue" );
                    }
                    spawnAddVar( pSpawn, "BetrayalGangId", "2" );
                    spawnAddVar( pSpawn, "PL_OnBetrayal", "DoAttackAI(1), DoNotUseAnyPowers(0), Untargetable(0), Invincible(0), Combat" );    //    Undo Rescue flags
                    

                    pSpawn->logicalName = StructAllocString(pDetail->pchName); 

                    if( !bRequired )
                        pReward->architectBadgeStat = StructAllocString("Architect.NonRequiredObjective");
                    else
                        pReward->architectBadgeStat = StructAllocString("Architect.Rescue");
                    pReward->architectBadgeStatTest = StructAllocString("Architect.TestRescue");
                }

                pSpawn->CVGIdx = pDetail->CVGIdx;

                // Loop over actors and make individual adjustments
                for( k = eaSize(&pSpawn->actors)-1; k>=0; k-- )
                {
                    Actor *pActor = cpp_const_cast(Actor*)(pSpawn->actors[k]);

                    if(!pActor->gang)
                    {
                        if( pDetail->eAlignment == kAlignment_Ally )
                            pActor->gang = StructAllocString( "1" );
                        else if ( pDetail->eAlignment == kAlignment_Enemy )
                            pActor->gang = StructAllocString( "2" );
                        else if( pDetail->eAlignment == kAlignment_Rogue )
                            pActor->gang = StructAllocString( "3" );
                    }

                    //    kRumble_RealFight -- The groups hate each other and me
                    //    kRumble_Ally1 -- Group one is on my side
                    //    kRumble_Ally2 -- Group two is on my side
                    // Complication: If either group matches the villain group of the mission, then
                    // they should be on that team instead.
                    if( pDetail->eDetailType == kDetail_Rumble )
                    {
                        if( pActor->gang[0] == '4' )
                        {
                            if( pDetail->eRumbleType == kRumble_Ally1 )
                            {
                                StructFreeString(pActor->gang);
                                pActor->gang = StructAllocString( "1" );
                            }
                            else if( vg_match == 1 )
                            {
                                StructFreeString(pActor->gang);
                                pActor->gang = StructAllocString( "2" );
                            }
                        }
                        else // gang 5
                        {
                            if( pDetail->eRumbleType == kRumble_Ally2 )
                            {
                                StructFreeString(pActor->gang);
                                pActor->gang = StructAllocString( "1" );
                            }
                            else if( vg_match == 2 )
                            {
                                StructFreeString(pActor->gang);
                                pActor->gang = StructAllocString( "2" );
                            }
                        }
                    }


                    if( stricmp(pActor->actorname, "B_BOSS") == 0 )
                    {
                        pActor->succeedondeath = pDetail->bBossOnly;
                        //    Override using the pcc
                        if (pDetail->customCritterId && EAINRANGE(pDetail->customCritterId-1,pPCArc->ppCustomCritters))
                        {
                            pActor->customCritterIdx = pDetail->customCritterId;
                            if ( pPCArc->ppCustomCritters[pDetail->customCritterId-1]->description && !pDetail->pchDisplayInfo )
                            {
                                if (pActor->displayinfo)    StructFreeString(pActor->displayinfo);
                                pActor->displayinfo = StructAllocString( smf_DecodeAllCharactersGet(pPCArc->ppCustomCritters[pDetail->customCritterId-1]->description) );
                            }
                        }
                        else
                        {
                            if( pDetail->pchVillainDef && stricmp( "random", pDetail->pchVillainDef ) !=0 ) // only put villain in if its not random
                            {
                                pActor->villain = StructAllocString( pDetail->pchVillainDef ); 
                                pActor->npcDefOverride = playerCreatedStoryArc_NPCIdByCostumeIdx(pDetail->pchVillainDef, pDetail->costumeIdx, 0);
                            }
                            else if( pDetail->pchEntityGroupName && stricmp(DOPPEL_GROUP_NAME, pDetail->pchEntityGroupName) )
                            {
                                //    random and doppels have custom villain groups so don't use them
                                pActor->villain = 0;
                                pActor->villaingroup = StructAllocString(pDetail->pchEntityGroupName);
                            }
                        }
                        if( pDetail->pchBossStartingAnim && !stricmp(pDetail->pchBossStartingAnim, "None") == 0 )
                        {
                            char aiString[MAX_PATH];
                            sprintf( aiString, "<<PL_%s>>", pDetail->pchBossStartingAnim );
                            pActor->ai_states[ACT_WORKING] = StructAllocString(aiString);
                            pActor->ai_states[ACT_INACTIVE] = StructAllocString(aiString);
                        }
                        if (pDetail->pchCustomNPCCostumeIdx && EAINRANGE(pDetail->pchCustomNPCCostumeIdx-1, pPCArc->ppNPCCostumes) &&
                            validateCustomNPCCostume(pPCArc->ppNPCCostumes[pDetail->pchCustomNPCCostumeIdx-1]))
                        {
                            pActor->customNPCCostumeIdx = pDetail->pchCustomNPCCostumeIdx;
                        }
                        if (pDetail->pchOverrideGroup && (ValidateCustomObjectName(pDetail->pchOverrideGroup, 50, 0) == VNR_Valid ))
                        {
                            if (pActor->displayGroup)
                            {
                                StructFreeString(pActor->displayGroup);
                            }
                            pActor->displayGroup = StructAllocString(pDetail->pchOverrideGroup);
                        }
                    }
                    else if( stricmp(pActor->actorname, "M_PERSON") == 0 || stricmp(pActor->actorname, "M_Object") == 0 )
                    {
                        char * pchAnimList = "HandsOnHead_Kneel";
                        char aiString[MAX_PATH];

                        // Flip Person Alignment
                        if( pDetail->eDetailType != kDetail_DestructObject )
                        {
                            if( pDetail->eAlignment == kAlignment_Ally )
                                pActor->gang = StructAllocString( "2" );
                            else if( pDetail->eAlignment == kAlignment_Enemy )
                                pActor->gang = StructAllocString( "1" );
                        }

                        if (pDetail->customCritterId && EAINRANGE(pDetail->customCritterId-1,pPCArc->ppCustomCritters))
                        {
                            pActor->customCritterIdx = pDetail->customCritterId;
                            //    Should this be awarded on each M_PERSON or only once per spawn def or only once per file?
                            if ( pPCArc->ppCustomCritters[pDetail->customCritterId-1]->description && !pDetail->pchDisplayInfo)
                            {
                                if (pActor->displayinfo)    StructFreeString(pActor->displayinfo);
                                pActor->displayinfo = StructAllocString( smf_DecodeAllCharactersGet(pPCArc->ppCustomCritters[pDetail->customCritterId-1]->description) );
                            }

                        }
                        else if( pDetail->pchVillainDef && stricmp( "random", pDetail->pchVillainDef ) !=0 ) // only put villain in if its not random
                        {
                            pActor->villain = StructAllocString( pDetail->pchVillainDef ); 
                            pActor->npcDefOverride = playerCreatedStoryArc_NPCIdByCostumeIdx(pDetail->pchVillainDef, pDetail->costumeIdx, 0);
                        }
                        else if( pDetail->pchEntityGroupName )
                        {
                            pActor->villain = 0;
                            pActor->villaingroup = StructAllocString(pDetail->pchEntityGroupName);
                        }

                        if( pDetail->pchPersonStartingAnimation && !stricmp(pDetail->pchPersonStartingAnimation, "None") == 0  )
                            pchAnimList = pDetail->pchPersonStartingAnimation;

                        sprintf( aiString, "DoNothing(FindTarget(0),Untargetable,AnimList(%s),DoNotUseAnyPowers(1),)", pchAnimList );
                        pActor->ai_states[ACT_WORKING] = StructAllocString(aiString);

                        if(pDetail->eDetailType == kDetail_DestructObject )
                        {
                            sprintf( aiString, "DoNothing" );
                            pActor->ai_states[ACT_INACTIVE] = StructAllocString(aiString);
                            pActor->ai_states[ACT_ALERTED] = StructAllocString(aiString);
                        }
                        else
                        {
                            sprintf( aiString, "DoNothing(Untargetable,AnimList(%s),DoNotUseAnyPowers(1))", pchAnimList );
                            pActor->ai_states[ACT_INACTIVE] = StructAllocString(aiString);

                            if( pDetail->eDetailType == kDetail_DefendObject )
                            {
                                sprintf( aiString, "DoNothing(AnimList(%s))", pchAnimList );
                                pActor->ai_states[ACT_ACTIVE] = StructAllocString(aiString);
                                pActor->ai_states[ACT_ALERTED] = StructAllocString(aiString);
                            }
                            else
                            {
                                pActor->ai_states[ACT_ALERTED] = StructAllocString(aiString);
                                pActor->ai_states[ACT_ACTIVE] = StructAllocString(aiString);
                            }
                        }
                        if (pDetail->pchCustomNPCCostumeIdx && EAINRANGE(pDetail->pchCustomNPCCostumeIdx-1, pPCArc->ppNPCCostumes) &&
                            validateCustomNPCCostume(pPCArc->ppNPCCostumes[pDetail->pchCustomNPCCostumeIdx-1]) )
                        {
                            pActor->customNPCCostumeIdx = pDetail->pchCustomNPCCostumeIdx;
                        }
                        if (pDetail->pchOverrideGroup && (ValidateCustomObjectName(pDetail->pchOverrideGroup, 50, 0) == VNR_Valid ) )
                        {
                            if (pActor->displayGroup)
                            {
                                StructFreeString(pActor->displayGroup);
                            }
                            pActor->displayGroup = StructAllocString(pDetail->pchOverrideGroup);
                        }
                    }
                    else // all other actors
                    {
                        if(pDetail->pchSupportVillainDef && stricmp( "none", pDetail->pchSupportVillainDef ) != 0 )
                        {
                            pActor->villain = StructAllocString(pDetail->pchSupportVillainDef);
                        }

                        if( pDetail->pchDefaultStartingAnim && !stricmp(pDetail->pchDefaultStartingAnim, "None") == 0 )
                        {
                            char aiString[MAX_PATH];
                            sprintf( aiString, "<<PL_%s>>", pDetail->pchDefaultStartingAnim  );
                            pActor->ai_states[ACT_WORKING] = StructAllocString(aiString);
                            pActor->ai_states[ACT_INACTIVE] = StructAllocString(aiString);
                        }
                    }
                }


                if( pDetail->pchName )
                    pObjective->name = StructAllocString(pDetail->pchName);

                if( pDetail->pchObjectiveDescription)
                    pObjective->description = StructAllocString(pDetail->pchObjectiveDescription);
                else if( pDetail->pchObjectiveSingularDescription )
                    pObjective->description = StructAllocString(pDetail->pchObjectiveSingularDescription);
                else if( pDetail->pchName && bRequired )
                    pObjective->description = StructAllocString(pDetail->pchName);


                if( pDetail->pchObjectiveSingularDescription )
                    pObjective->singulardescription = StructAllocString(pDetail->pchObjectiveSingularDescription);
                else if( pDetail->pchName && bRequired )
                    pObjective->singulardescription = StructAllocString(pDetail->pchName);

                if(    pDetail->pchRewardDescription )
                    pReward->rewardDialog = StructAllocString(pDetail->pchRewardDescription);

                if( pDetail->pchDetailTrigger && (pDetail->eDetailType != kDetail_Ambush) )
                {
                    eaPush(&pObjective->descRequires, StructAllocString( pDetail->pchDetailTrigger ) ); 
                    eaPushConst(&pSpawn->createOnObjectiveComplete, StructAllocString( pDetail->pchDetailTrigger ) );
                }

                if( pDetail->pchClueName || pDetail->pchClueDesc )
                {
                    char * pchClueName;
                    estrCreate(&pchClueName);
                    estrConcatf(&pchClueName, "DetailClue_%i_%i", i, j );
                    storyArcAddClue( pArc, pchClueName,  pDetail->pchClueName, pDetail->pchClueDesc );
                    eaPushConst(&pReward->addclues, StructAllocString(pchClueName) );
                    estrDestroy(&pchClueName);
                }

                if( pDetail->eDetailType == kDetail_Ambush ) // have to make two spawns here, one is the trigger, the other is the wave attack
                    pSpawn->playerCreatedDetailType = 0;
                if( pDetail->eDetailType == kDetail_Boss || pDetail->eDetailType == kDetail_Patrol || pDetail->eDetailType == kDetail_DestructObject )
                    pSpawn->playerCreatedDetailType = 1;
                else if(pDetail->eDetailType == kDetail_GiantMonster) 
                    pSpawn->playerCreatedDetailType = 2;
                else
                    pSpawn->playerCreatedDetailType = 3;

                eaPush(&pObjective->reward, pReward);
                eaPushConst(&pSpawn->objective, pObjective);
                eaPushConst(&pMission->spawndefs, pSpawn);
            }
        }

        for( j = 0; j < eaSize(&pPCMission->ppMissionCompleteDetails); j++ )
        {
            if(!pMission->objectivesets)
                eaPushConst(&pMission->objectivesets, StructAllocRaw(sizeof(MissionObjectiveSet)) );
            eaPushConst( &pMission->objectivesets[0]->objectives, StructAllocString(pPCMission->ppMissionCompleteDetails[j]) );

            if( stricmp(pPCMission->ppMissionCompleteDetails[j], "DefeatAllVillainsInEndRoom" ) == 0 )
            {
                if( pPCMission->pchDefeatAll )
                    pMission->pchDefeatAllText = StructAllocString(pPCMission->pchDefeatAll);
                else
                    pMission->pchDefeatAllText = StructAllocString(localizedPrintf(0,"MMDefeatAllInEndRoomText"));
            }

            if( stricmp(pPCMission->ppMissionCompleteDetails[j], "DefeatAllVillains" ) == 0 )
            {
                if( pPCMission->pchDefeatAll )
                    pMission->pchDefeatAllText = StructAllocString(pPCMission->pchDefeatAll);
                else
                    pMission->pchDefeatAllText = StructAllocString(localizedPrintf(0,"MMDefeatAllText"));
            }
        }
        for( j = 0; j < eaSize(&pPCMission->ppMissionFailDetails); j++ ) 
        {
            MissionObjectiveSet *pSet = StructAllocRaw(sizeof(MissionObjectiveSet));
            eaPush( &pSet->objectives, StructAllocString(pPCMission->ppMissionFailDetails[j]) );
            eaPushConst(&pMission->failsets, pSet);
        }


        if( pPCMission->pchTaskReturnSuccess)
        {
            pRewardSuccess = StructAllocRaw(sizeof(StoryReward));
            pRewardSuccess->rewardDialog = StructAllocString(pPCMission->pchTaskReturnSuccess);
            eaPushConst(&pTask->returnSuccess, pRewardSuccess);
        }

        if( pPCMission->pchTaskReturnFail )
        {
            pRewardSuccess = StructAllocRaw(sizeof(StoryReward));
            pRewardSuccess->rewardDialog = StructAllocString(pPCMission->pchTaskReturnFail);
            eaPushConst(&pTask->returnFailure, pRewardSuccess);
        }

        if(pPCMission->pchClueName || pPCMission->pchClueDesc)
        {
            char * pchClueName;
            StoryReward *pRewardClue = StructAllocRaw(sizeof(StoryReward));
            estrCreate(&pchClueName);
            estrConcatf(&pchClueName, "MissionClue_%i", i );
            storyArcAddClue( pArc, pchClueName,  pPCMission->pchClueName, pPCMission->pchClueDesc );
            eaPushConst(&pRewardClue->addclues, StructAllocString(pchClueName) );
            estrDestroy(&pchClueName);
            eaPushConst(&pTask->taskSuccess, pRewardClue);
        }

        pLastTask = pTask;

        eaPushConst(&pTask->missiondef, pMission);
        eaPush(&pEpisode->tasks, pTask);
        eaPush(&pArc->episodes, pEpisode);
    }

    if( pLastTask && ( pPCArc->pchSouvenirClueName || pPCArc->pchSouvenirClueDesc ) )
    {
        StoryReward *pReward = 0;
        char * pchClueName;
        estrCreate(&pchClueName);
        estrConcatf(&pchClueName, "SouvenirID_%i", id );

        storyArcAddClue( pArc, pchClueName, pPCArc->pchSouvenirClueName,  pPCArc->pchSouvenirClueDesc );

        if( pLastTask->taskSuccess && pLastTask->taskSuccess[0] )
            pReward = cpp_const_cast(StoryReward*)(pLastTask->taskSuccess[0]);
        else
        {
            pReward = StructAllocRaw(sizeof(StoryReward));
            eaPushConst(&pLastTask->taskSuccess, pReward);
        }
        eaPushConst(&pReward->souvenirClues, StructAllocString(pchClueName) );

        estrDestroy(&pchClueName);
    }

    playerCreatedStoryArc_Stash( id, str, pArc, pPCArc );

    return pPCArc;
}

void testStoryArc(Entity *e, char *str)
{
    if( ArchitectCanEdit(e) ) 
        TaskForceArchitectStart( e, str, 0, 1, ARCHITECT_NORMAL, s_authid(e) );
}

// Summary of How Architect Tickets Work
//--------------------------------------------------------------
// Players earn architect tickets in place of salvage/enhancements/recipes when killing mobs.
// There is a cap on how many tickets can be earned per mission.
// When the mission is completed, the player will receive a bonus tickets, a percentage of how many tickets were earned already during the mission.
extern StuffBuff s_sbRewardLog;
void playerCreatedStoryArc_RewardTickets(Entity *e, int iAmount, int bEndOfMission )
{
    const SalvageItem *si = salvage_GetItem( "S_ArchitectTicket" );
    MissionMapStats *pStat = missionMapStat_Get(db_state.map_name);

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    if( (e && e->taskforce && e->taskforce->architect_flags&ARCHITECT_TEST) || (g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST)  )
    {
        badge_StatAddArchitectAllowed(e, "Architect.TestTickets", iAmount, 0 );
        return;
    }

    if( !e || !e->pl || !AccountCanGainAllMissionArchitectRewards(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags) )
    {
        return;
    }

    if (e && e->pl && si && pStat)
    {
        int ticket_count = character_SalvageCount(e->pchar,si);
        int max_ticket_count = gMMRewards.TicketCap;
        int missionCap = 0;

        if( bEndOfMission )
        {
            if( INRANGE( g_activemission->sahandle.subhandle, 0, 5) )
            {
                F32 fBonus = 1.f;

                if (chareval_Eval(e->pchar, gMMRewards.ppBonus2Requires, "scripts/Player_Created/PC_Rewards.txt"))
                    fBonus = gMMRewards.MissionCompleteBonus2[g_activemission->sahandle.subhandle];
                else if (chareval_Eval(e->pchar, gMMRewards.ppBonus1Requires, "scripts/Player_Created/PC_Rewards.txt"))
                    fBonus = gMMRewards.MissionCompleteBonus1[g_activemission->sahandle.subhandle];
                else
                    fBonus = gMMRewards.MissionCompleteBonus[g_activemission->sahandle.subhandle];

                iAmount = e->pl->pendingArchitectTickets * fBonus;

                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv:Tick Auth: %s  Arc ID: %d  Creator: %d  Flags: 0x%08x  Count: %d", e->auth_name, e->taskforce->architect_id, e->taskforce->architect_authid, e->taskforce->architect_flags, iAmount);
            }
            else
                Errorf( "Invalid Subhandle when trying to reward mission complete tickets");
        }

        // inventory cap
        if( ticket_count + iAmount > max_ticket_count )
        {
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Arch]:Rcv:Tick Overflow Auth: %s  Count: %d", e->auth_name, (ticket_count + iAmount) - max_ticket_count);
            missionserver_map_grantOverflowTickets(e, (ticket_count + iAmount) - max_ticket_count);
            iAmount = max_ticket_count - ticket_count;
            sendInfoBox(e, INFO_REWARD, "ArchitectTicketInventoryCap");
            addStringToStuffBuff(&s_sbRewardLog, "Architect: Tickets Award Hit inventory cap.  Inventory: %i, Attempted Award: %i\n", ticket_count, iAmount );
        }

        // Mission cap 
        {
            MissionPlaceStats * pPlace = pStat->places[0];
            int x;

            missionCap += pPlace->genericEncounterOnlyCount;
            for( x = 0; x < eaSize(&pPlace->customEncounters); x++ )
            {
                if( stricmp( "Around", pPlace->customEncounters[x]->type ) == 0)
                {
                    missionCap += pPlace->customEncounters[x]->countOnly;
                    missionCap += pPlace->customEncounters[x]->countGrouped;
                    break;
                }
            }

            max_ticket_count = MIN(missionCap * gMMRewards.TicketsPerSpawnCap, gMMRewards.MissionCap);

            if( e->pl->pendingArchitectTickets + iAmount > max_ticket_count )
            {
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Arch]:Rcv:Tick Overflow Auth: %s  Count: %d", e->auth_name, (e->pl->pendingArchitectTickets + iAmount) - max_ticket_count);
                missionserver_map_grantOverflowTickets(e, (e->pl->pendingArchitectTickets + iAmount) - max_ticket_count);
                iAmount = max_ticket_count - e->pl->pendingArchitectTickets;
                sendInfoBox(e, INFO_REWARD, "ArchitectTicketMissionCap");
                addStringToStuffBuff(&s_sbRewardLog, "Architect: Tickets Award Hit Mission Map cap.  Cap: %i, Attempted Award: %i\n", max_ticket_count, iAmount );
            }
        }

        //    this was moved after because otherwise the e->pl->pendingArchitectTickets + iAmount would be added twice (since we are incrementing it here)
        if( g_ArchitectTaskForce && (iAmount > 0) )
            e->pl->pendingArchitectTickets += iAmount;    //    still increment pendingArchitectTickets after the mission completes, because this value is used to check if it goes over the max ticket amount
                                                        //    this gets set to 0, when the the leader sets a new task

        if( iAmount > 0 )
        {
            if(bEndOfMission)
            {
                if( iAmount > 1 )
                    sendInfoBox(e, INFO_REWARD, "ArchitectMissionCompleteBonus", iAmount );
                else
                    sendInfoBox(e, INFO_REWARD, "ArchitectMissionCompleteBonusSingle" );
            }
            else
            {
                if( iAmount > 1 )
                    sendInfoBox(e, INFO_REWARD, "ArchitectTicketsRewarded", iAmount);
                else
                    sendInfoBox(e, INFO_REWARD, "ArchitectTicketRewarded");
            }

            serveFloater(e, e, si->pchDisplayDropMsg, 0.0f, kFloaterStyle_Attention, 0);
            character_AdjustSalvage( e->pchar, si, iAmount, "architectreward", true );
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Tick Auth: %s  Count: %d", e->auth_name, iAmount);
            addStringToStuffBuff(&s_sbRewardLog, "Architect: Tickets Awarded %i\n", iAmount );
            badge_StatAddArchitectAllowed(e, "Architect.Tickets", iAmount, 0 );
        }
    }
}

void playerCreatedStoryArc_Reward(Entity *e, int storyarc_complete)
{
    if( e && e->pl && e->taskforce_id && e->pl->taskforce_mode )
    {
        PlayerCreatedStoryArc *pArc = playerCreatedStoryArc_GetPlayerCreatedStoryArc(e->taskforce_id);
        if(pArc)
        {
            const SalvageItem *si = salvage_GetItem( "S_ArchitectTicket" );
            int mission_num = e->taskforce->storyInfo->tasks[0]->sahandle.subhandle;

            if( storyarc_complete )
            {
                if( e->taskforce->architect_flags&ARCHITECT_TEST )
                {
                    badge_StatAddArchitectAllowed(e, "Architect.TestPlay", 1, 0 );
                    if( e->teamup && e->teamup->members.count == 8 )
                        badge_Award( e, "ArchitectTest8", 1 );
                    if( e->taskforce->architect_authid == s_authid(e) )
                        badge_Award( e, "ArchitectTestOwnMission", 1 );
                }
                else
                {
                    badge_StatAddArchitectAllowed(e, "Architect.Play", 1, 0 );
                    missionserver_recordArcCompleted( e->taskforce->architect_id, s_authid( e ) );
                }

                if( pArc->pchSouvenirClueDesc || pArc->pchSouvenirClueName )
                {
                    START_PACKET( pak, e, SERVER_ARCHITECT_SOUVENIR )
                        pktSendBitsAuto(pak, e->taskforce_id );
                    pktSendString( pak, pArc->pchSouvenirClueName?saUtilTableAndEntityLocalize(pArc->pchSouvenirClueName,0,e):"" );
                    pktSendString( pak, pArc->pchSouvenirClueDesc?saUtilTableAndEntityLocalize(pArc->pchSouvenirClueDesc,0,e):"" );
                    END_PACKET
                }

                if( e->taskforce->architect_flags&ARCHITECT_DEVCHOICE || e->taskforce->architect_flags&ARCHITECT_DEVCHOICE_NORMALREWARD )
                    badge_StatAddArchitectAllowed(e, "Architect.PlayDevChoice", 1, 0 );
                if( e->taskforce->architect_flags&ARCHITECT_HALLOFFAME )
                    badge_StatAddArchitectAllowed(e, "Architect.PlayHallofFame", 1, 0 );
                if( e->taskforce->architect_flags&ARCHITECT_FEATURED )
                    badge_StatAddArchitectAllowed(e, "Architect.PlayFeatured", 1, 0 );

                switch( pArc->eMorality )
                {
                    xcase kMorality_Good: badge_StatAddArchitectAllowed(e, "Architect.HeroMissions", 1, 0 );
                    xcase kMorality_Vigilante: badge_StatAddArchitectAllowed(e, "Architect.VigilanteMissions", 1, 0 );
                    xcase kMorality_Rogue: badge_StatAddArchitectAllowed(e, "Architect.RogueMissions", 1, 0 );
                    xcase kMorality_Neutral: badge_StatAddArchitectAllowed(e, "Architect.NeutralMissions", 1, 0 );
                    xcase kMorality_Evil: badge_StatAddArchitectAllowed(e, "Architect.VillainMissions", 1, 0 );
                }

                // count missions the player participated in
                if( e->taskforce->architect_flags&ARCHITECT_DEVCHOICE )
                {
                    int i, missions_played = 1;
                    char * estr;

                    for( i = 0; i < mission_num; i++ )
                    {
                        if( e->pl->architectMissionsCompleted&(1<<i) )
                            missions_played++;
                    }

                    rewardTeamMember( e, "DevChoiceMissionBonus", (F32)missions_played, e->pchar->iLevel, true, 1.f, 1.f, 1.f, VG_NONE, 1, 1, character_CalcRewardLevel(e->pchar), REWARDSOURCE_DEVCHOICE, CONTAINER_TEAMUPS, 0, NULL );

                    estrCreate(&estr);
                    estrConcatf(&estr, "DevChoice%iMissionArcBonus", missions_played);
                    rewardTeamMember( e, estr, 1.f, e->pchar->iLevel, true, 1.f, 1.f, 1.f, VG_NONE, 1, 1, character_CalcRewardLevel(e->pchar), REWARDSOURCE_DEVCHOICE, CONTAINER_TEAMUPS, 0, NULL ); 
                    estrDestroy(&estr);
                }
            }

            if( e->taskforce->architect_flags&ARCHITECT_DEVCHOICE ) 
            {
                e->pl->architectMissionsCompleted |= (1<<mission_num);
            }
            else if( !(e->taskforce->architect_flags&ARCHITECT_TEST ) )
            {
                playerCreatedStoryArc_RewardTickets(e, 0, 1 );
            }
        }
    }
}

void playerCreatedStoryArc_CalcReward( Entity *e, int iVictimLevel, const char *pchRank, F32 fScale )
{
    F32 fLevelMod = 1.f;
    int iCharLevel;
    int i,j;
    MMRewardRank *pRank = 0;

    if(!e || !e->taskforce || !e->pchar )
        return;

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    iCharLevel = character_CalcCombatLevel(e->pchar);

    if( e->taskforce->architect_flags&ARCHITECT_DEVCHOICE )
        return;

    if( iCharLevel < iVictimLevel )
    {
        int idx = MIN( iVictimLevel-iCharLevel-1, eafSize(&gMMRewards.highLevelBonus)-1);
        fLevelMod = gMMRewards.highLevelBonus[idx];
    }
    else if( iVictimLevel < iCharLevel)
    {
        int idx = MIN( iCharLevel-iVictimLevel-1, eafSize(&gMMRewards.lowLevelPenalty)-1);
        fLevelMod = gMMRewards.lowLevelPenalty[idx];
    }

    for( i = eaSize(&gMMRewards.ppRewardRank)-1; i >= 0; i-- )
    {
        for( j = eaSize(&gMMRewards.ppRewardRank[i]->pchRanks)-1; j >= 0; j--)
        {
            if( stricmp(gMMRewards.ppRewardRank[i]->pchRanks[j], pchRank) == 0 )
                pRank = gMMRewards.ppRewardRank[i];
        }
    }

    if(pRank)
    {
        F32 fRand = (float)rand()/(float)RAND_MAX;
        F32 fTally = 0;
        int iAmount = 0;

        for( i = 0; i < eafSize(&pRank->chances); i++ )
        {
            fTally += pRank->chances[i];
            if( fRand < fTally )
            {
                if (server_state.ticketscale != 1.0f)
                {
                    fScale *= server_state.ticketscale;
                }
                iAmount = round(pRank->values[i] * fLevelMod * fScale);
                break;
            }
        }

        if( iAmount )
        {
            playerCreatedStoryArc_RewardTickets( e, iAmount, 0 );
            addStringToStuffBuff(&s_sbRewardLog, "Architect: Tickets roll succeeded: %4.3f.  Amount: %i, LevelMod: %3.2f, Scale: %3.2f, Total: %i\n", fRand, pRank->values[i], fLevelMod, fScale, iAmount );
        }
        else
            addStringToStuffBuff(&s_sbRewardLog, "Architect: Tickets roll failed: %4.3f\n", fRand );
    }
    else
        addStringToStuffBuff(&s_sbRewardLog, "Architect: Tickets not found for rank %s\n", pchRank );
}

static void s_sendInventoryToClient(Entity *e)
{
    int count;

    if(e && e->mission_inventory)
    {
        START_PACKET(pak_out, e, SERVER_ARCHITECT_INVENTORY)
            pktSendBitsAuto(pak_out, e->mission_inventory->banned_until);
        pktSendBitsAuto(pak_out, e->mission_inventory->tickets);
        pktSendBitsAuto(pak_out, e->mission_inventory->publish_slots);
        pktSendBitsAuto(pak_out, e->mission_inventory->publish_slots_used);
        pktSendBitsAuto(pak_out, e->mission_inventory->permanent);
        pktSendBitsAuto(pak_out, e->mission_inventory->invalidated);
        pktSendBitsAuto(pak_out, e->mission_inventory->playableErrors);
        pktSendBitsAuto(pak_out, e->mission_inventory->uncirculated_slots);
        pktSendBitsAuto(pak_out, eaSize(&e->mission_inventory->unlock_keys));
        for(count = 0; count < eaSize(&e->mission_inventory->unlock_keys); count++)
        {
            pktSendString(pak_out, e->mission_inventory->unlock_keys[count]);
        }
        END_PACKET
    }
}

void playerCreatedStoryArc_BuyUpgrade(Entity *e, char *pchUnlockKey, int iPublishSlots, int iTicketCost)
{
    if (e)
    {
        int flags = 0;
        const SalvageItem *si = salvage_GetItem( "S_ArchitectTicket" );

        // TODO: if we have it already stop the order
        if( pchUnlockKey && strlen(pchUnlockKey) > 0 )
        {
            if( playerCreatedStoryArc_CheckUnlockKey(e, pchUnlockKey ) )
            {
                // already owned, abort transaction
                chatSendToPlayer( e->db_id, localizedPrintf(e,"ArchitectTokenOwned", pchUnlockKey), INFO_ARCHITECT, 0 );
                s_sendInventoryToClient(e);
                return;
            }
            flags |= kPurchaseUnlockKey;
        }

        if( iPublishSlots > 0 )
            flags |= kPurchasePublishSlots;



        if( character_AdjustSalvage( e->pchar, si, -iTicketCost, "ArchitectTokenPurchase", false ) < 0 )
        {
            // how did we get here!?
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ArchitectTransactionFailed"), INFO_ARCHITECT, 0 );
            s_sendInventoryToClient(e);
            return;
        }
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ArchitectTransactionMessage", iTicketCost, pchUnlockKey), INFO_ARCHITECT, 0 );

        if( flags )
        {
            MissionInventoryOrder *order = calloc( 1, sizeof( MissionInventoryOrder ) );
            Packet *pak = pktCreateEx( &db_comm_link, DBCLIENT_MISSIONSERVER_BUY_ITEM );

            pktSendBitsAuto( pak, e->db_id );
            pktSendBitsAuto( pak, s_authid( e ) );
            pktSendBitsAuto( pak, iTicketCost );
            pktSendBitsAuto( pak, flags );

            if( flags & kPurchaseUnlockKey )
            {
                pktSendString( pak, pchUnlockKey );
            }

            if( flags & kPurchasePublishSlots )
                pktSendBitsAuto( pak, iPublishSlots );

            pktSend( &pak, &db_comm_link );

            order->flags = flags;
            order->tickets = iTicketCost;
            order->time = timerSecondsSince2000();

            if( !e->mission_inventory)
            {
                e->mission_inventory = calloc(1, sizeof(MissionInventory));
                e->mission_inventory->stKeys = stashTableCreateWithStringKeys(8,StashDefault);
            }
            eaPush( &e->mission_inventory->pending_orders, order );
        }
    }
}

U32 playerCreatedStoryArc_GetPublishSlots(Entity *e)
{
    U32 retval = 0;

    if( e && e->mission_inventory )
    {
        retval = e->mission_inventory->publish_slots;
    }

    return retval;
}

void InventoryOrderTick(Entity *e)
{
    if( e && e->mission_inventory )
    {
        int count;
        U32 now = timerSecondsSince2000();
        const SalvageItem *si = salvage_GetItem( "S_ArchitectTicket" );

        // Working backwards means we don't lose our place if we remove
        // things from the list.
        for( count = eaSize( &e->mission_inventory->pending_orders ) - 1; count >= 0; count-- )
        {
            if( ( now - e->mission_inventory->pending_orders[count]->time ) > INVENTORY_ORDER_TIMEOUT_SECS )
            {
                MissionInventoryOrder *order;
                int num = e->mission_inventory->pending_orders[count]->tickets;
                character_AdjustSalvage( e->pchar, si, num, "ArchitectTicketsRefund", false );
                order = eaRemove( &e->mission_inventory->pending_orders, count );
                chatSendToPlayer( e->db_id, localizedPrintf(e, "ArchitectTicketRefundMessage", num ), INFO_ARCHITECT, 0 );
                SAFE_FREE( order );
            }
        }
    }
}

// Mission Server Communication ////////////////////////////////////////////////

static void s_missionserver_map_sendClientError(Entity *e, char *errorMsg)
{
    char *text = estrTemp();
    if(e && errorMsg)
    {
        estrConcatf(&text, "%s: %s", localizedPrintf(e,"MMPublishError"), errorMsg);
        START_PACKET( pak, e, SERVER_SEND_DIALOG)
            pktSendString( pak, text );
        END_PACKET
    }
    estrDestroy(&text);
}

void missionserver_map_publishArc(Entity *e, int arcid, const char *arcstr)
{
    PlayerCreatedStoryArc arc = {0};
    MissionSearchHeader header = {0};
    int arcstrlen = strlen(arcstr);
    char *headerstr = estrTemp();
    char *estr = estrTemp();
    char *estrname = estrTemp();
    char *s = (char*)arcstr;
    MissionServerPublishFlags publishflags = 0;
    char *errors;

    if(e && !s_publishRateLimiter(e->pl) &&!(s_devPublishTest && e->access_level == 10))
    {
        dbLog("missionmaker", e, "Cheater: Attempted to publish more frequently than allowed");
        chatSendToPlayer(e->db_id, saUtilLocalize(e, "MissionArchitectSpamPublish"), INFO_SVR_COM, 0);
        return;
    }

    if(!ParserReadTextEscaped(&s, ParsePlayerStoryArc, &arc))
    {
        if(e)
            s_missionserver_map_sendClientError(e, localizedPrintf(e,"ArchitectCanNotParse"));
        return;
    }

    estrCreate(&errors);
    if(playerCreatedStoryarc_GetValid( &arc, s, &errors, 0, e, 0 ) != kPCS_Validation_OK &&!(s_devPublishTest && e->access_level == 10))
    {
        if(e)
            s_missionserver_map_sendClientError(e, errors);
        estrDestroy(&errors);
        return;
    }
    estrDestroy(&errors);

    arc.locale_id = getCurrentLocale(); // override with the map's locale
    arc.european = locIsEuropean(arc.locale_id);

    estrConcatCharString(&estrname, e->name);
    arc.pchAuthor = StructAllocString(estrname);

    ParserWriteTextEscaped(&estr, ParsePlayerStoryArc, &arc, 0, 0);
    playerCreatedStoryArc_FillSearchHeader(&arc, &header,estrname);
    if (eaSize(&arc.ppCustomCritters) > 0 )
    {
        publishflags |= kPublishCustomBoss;
    }

    ParserWriteText(&headerstr, parse_MissionSearchHeader, &header, 0, 0);

    {
        Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_PUBLISHARC);
        pktSendBitsAuto(pak, e->db_id);
        pktSendBitsAuto(pak, arcid);
        pktSendBitsAuto(pak, s_authid(e));
        pktSendString(pak, e->auth_name);
        pktSendBitsAuto(pak, e->access_level);
        pktSendBitsAuto(pak, arc.keywords);
        pktSendBitsAuto(pak, publishflags);
        pktSendString(pak, headerstr);
        pktSendZipped(pak, strlen(estr), (char*)estr);
        pktSend(&pak, &db_comm_link);
    }

    StructClear(ParsePlayerStoryArc, &arc);
    estrDestroy(&headerstr);
    estrDestroy(&estr);
}

void missionserver_map_startArc(Entity *e, int arcid)
{
    if (e)
    {
        //    if arcid is 0, the player is testing an arc, do we want to track that as an arc start?
        if (arcid)
        {
            missionserver_recordArcStarted(arcid, s_authid(e));
        }
    }
}

// these are passthroughs to get at s_authid() and enforce that it's from the calling client
void missionserver_map_setKeywordsForArc(Entity *e, int arcid, MissionRating vote)    { missionserver_setKeywordsForArc_remote(NULL, e->db_id, e->access_level, arcid, s_authid(e), vote); }
void missionserver_map_voteForArc(Entity *e, int arcid, MissionRating vote)    { missionserver_voteForArc_remote(NULL, e->db_id, e->access_level, arcid, s_authid(e), vote); }
void missionserver_map_comment(Entity *e, int arcid, const char *comment)    { missionserver_comment_remote(NULL, e->db_id, arcid, s_authid(e), comment); }

void missionserver_map_printUserInfo(Entity *e, int message_id, char *authname, char *region)
{
    missionserver_map_printUserInfoById(e, message_id, (authname && authname[0]) ? s_authidFromName(authname) : s_authid(e), region);
}

void missionserver_map_printUserInfoById(Entity *e, int message_id, U32 authid, char *region)
{
    meta_console(NULL, message_id ? message_id : e->db_id, missionserver_printUserInfo(authid, region && region[0] ? region : ""));
}

void missionserver_map_reportArc(Entity *e, int arcid, int type, const char *comment, const char *global)
{
    if(!e->access_level && type != kComplaint_Comment && type != kComplaint_Complaint)
        dbLog("missionmaker", e, "Cheater: Attempted to use an unauthorized complaint type %d", type);
    else
        missionserver_reportArc_remote(NULL, e->db_id, arcid, s_authid(e), type, comment, global);
}


void missionserver_map_unpublishArc(Entity *e, int arcid)                    
{
    if(    SAFE_MEMBER(e->taskforce,architect_id) == arcid &&
        SAFE_MEMBER(e->taskforce,architect_authid) == s_authid(e) &&
        !(e->taskforce->architect_flags & ARCHITECT_TEST) )
    {
        TaskForceDestroy(e);
        dbLog("missionmaker", e, "Automatically disbanded taskforce whose member unpublished the running arc");
        sendChatMsg(e, localizedPrintf(e, "ArchitectUnpublishDisband"), INFO_ARCHITECT, 0);
    }

    missionserver_unpublishArc_remote(NULL, e->db_id, arcid, s_authid(e), e->access_level);
}

static void s_pktSendGetArcInfo(Entity *e, Packet *pak_out, Packet *pak_in)
{
    int comments;
    char *globalName = estrTemp();
    char *comment = estrTemp();
    pktSendGetBool(pak_out, pak_in);        // owned
    pktSendGetBool(pak_out, pak_in);        // played
    pktSendGetBitsAuto(pak_out, pak_in);    // myVote
    pktSendGetBitsAuto(pak_out, pak_in);    // ratingi
    pktSendGetBitsAuto(pak_out, pak_in);    // total_votes
    pktSendGetBitsAuto(pak_out, pak_in);    // keyword_votes
    pktSendGetBitsAuto(pak_out, pak_in);    // date
    pktSendGetString(pak_out, pak_in);        // header
    comments = pktSendGetBitsAuto(pak_out, pak_in);
    while(comments--)
    {
        int authid = pktGetBitsAuto(pak_in);
        AuthRegion authregion = pktGetBitsAuto(pak_in);
        int type = pktGetBitsAuto(pak_in);
        estrPrintCharString(&globalName, pktGetString(pak_in));
        estrPrintCharString(&comment, pktGetString(pak_in));

        pktSendBitsAuto( pak_out, type );
        if( type == kComplaint_Comment )
        {
            pktSendString(pak_out, globalName);
            pktSendString(pak_out, comment);
        }
        else
        {
            if(e->access_level)
                pktSendStringf(pak_out, "%s.%d %s: %s", authregion_toString(authregion), authid, globalName, comment);
            else
                pktSendString(pak_out, comment);
        }
    }
    estrDestroy(&globalName);
    estrDestroy(&comment);
}

void missionserver_map_requestSearchPage(Entity *e, MissionSearchPage category, const char *context, int page, int viewFilter, int levelFilter, int authorArcId )
{
    char *context2 = NULL;
    int cheater = 0;
    MissionSearchParams searchParameters = {0};

    if(!s_searchRateLimiter(e->pl) &&!(s_devPublishTest && e->access_level == 10))
    {
        dbLog("missionmaker", e, "Cheater: Attempted to search more frequently than allowed");
        chatSendToPlayer(e->db_id, saUtilLocalize(e, "MissionArchitectSpamSearch"), INFO_SVR_COM, 0);
        return;
    }

    if(    !INRANGE0(category, MISSIONSEARCH_PAGETYPES) ||
        e->access_level < g_missionsearchpage_accesslevel[category])
    {
        dbLog("missionmaker", e, "Cheater: tried to access an invalid missionsearch page %d", category);
        return;
    }

    switch(category)
    {
        STATIC_ASSERT(MISSIONSEARCH_PAGETYPES ==7);

        xcase MISSIONSEARCH_ALL:
        {
            if(!ParserReadText(context, -1, parse_MissionSearchParams, &searchParameters))
            {
                dbLog("missionmaker", e, "Cheater: invalid missionsearch page context (%d: %s)", category, context);
                return;
            }

            if(!e->access_level)
            {
                U32 old = searchParameters.ban;
                int cheater = 0;

                //banned missions
                missionban_removeInvisible(&searchParameters.ban);
                if(old != searchParameters.ban || !searchParameters.ban)
                {
                    if(!searchParameters.ban)
                        missionban_setVisible(&searchParameters.ban);
                    cheater++;
                }

                //search string format
                if(searchParameters.text)
                {
                    char *textTemp = estrTemp();
                    estrPrintCharString(&textTemp, searchParameters.text);
                    missionsearch_formatSearchString(&textTemp, MSSEARCH_QUOTED_ALLOWED);
                    if(strcmp(textTemp, searchParameters.text))
                    {
                        dbLog("missionmaker", e, "Cheater: Attempted to search with unformatted search string");
                        StructReallocString(&searchParameters.text, textTemp);
                        cheater++;
                    }
                    estrDestroy(&textTemp);
                }

                if(cheater)
                {
                    context2 = estrTemp();
                    ParserWriteText(&context2, parse_MissionSearchParams, &searchParameters, 0, 0);
                    context = context2;
                }
            }
        }

        xcase MISSIONSEARCH_MYARCS:
        xcase MISSIONSEARCH_ARC_AUTHOR:
        context = "";

        xcase MISSIONSEARCH_CSR_AUTHORED:
    case  MISSIONSEARCH_CSR_COMPLAINED:
    case  MISSIONSEARCH_CSR_VOTED:
    case  MISSIONSEARCH_CSR_PLAYED:
        // they have accesslevel, i'm not going to bother verifying their context

xdefault:
        dbLog("missionmaker", e, "Unhandled mission search page %d", category);
        return;
    }

    {
        Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_SEARCHPAGE);
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendBitsAuto(pak_out, s_authid(e));
        pktSendBitsAuto(pak_out, e->access_level);
        pktSendBitsAuto(pak_out, category);
        pktSendString(pak_out, context);
        pktSendBitsAuto(pak_out, page);
        pktSendBitsAuto(pak_out, viewFilter);
        pktSendBitsAuto(pak_out, levelFilter);
        pktSendBitsAuto(pak_out, authorArcId);
        pktSend(&pak_out, &db_comm_link);
    }

    estrDestroy(&context2);
    StructClear(parse_MissionSearchParams, &searchParameters);
}

void missionserver_map_receiveSearchPage(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    if(e)
    {
        int arcid;
        START_PACKET(pak_out, e, SERVER_MISSIONSERVER_SEARCHPAGE)
            if(pktSendGetBitsAuto(pak_out, pak_in) == MISSIONSEARCH_ALL)    // category
            {
                pktSendGetString(pak_out, pak_in);                            // context
            }
            else
            {
                pktGetString(pak_in); // eat context for non-search pages
                pktSendString(pak_out, NULL);                                // context
            }
            pktSendGetBitsAuto(pak_out, pak_in);                            // page
            pktSendGetBitsAuto(pak_out, pak_in);                            // page count
            while(arcid = pktSendGetBitsAuto(pak_out, pak_in))
            {
                if(arcid < 0)
                    pktSendGetString(pak_out, pak_in);                        // section
                else
                    s_pktSendGetArcInfo(e, pak_out, pak_in);
            }
            END_PACKET
    }
}

void missionserver_map_requestArcInfo(Entity *e, int arcid)
{
    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_ARCINFO);
    pktSendBitsAuto(pak_out, e->db_id);
    pktSendBitsAuto(pak_out, s_authid(e));
    pktSendBitsAuto(pak_out, e->access_level);
    pktSendBitsAuto(pak_out, arcid);
    pktSendBitsAuto(pak_out, 0);            // arcid (finished)
    pktSend(&pak_out, &db_comm_link);
}

void missionserver_map_requestBanStatus(int arcid)
{
    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_BANSTATUS);
    pktSendBitsAuto(pak_out, arcid);
    pktSend(&pak_out, &db_comm_link);
}

void missionserver_map_receiveBanStatus(Packet *pak_in)
{
    int arcid = pktGetBitsAuto(pak_in);
    int ban = pktGetBitsAuto(pak_in);
    int honors = pktGetBitsAuto(pak_in);
    DoorArchitectBanUpdate(arcid, ban, honors);
}

void missionserver_map_receiveArcInfo(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    if(e)
    {
        START_PACKET(pak_out, e, SERVER_MISSIONSERVER_ARCINFO)
            if(pktSendGetBitsAuto(pak_out, pak_in))            // arcid
                if(pktSendGetBool(pak_out, pak_in))            // exists
                    s_pktSendGetArcInfo(e, pak_out, pak_in);
        // this is designed to be a while loop, but we only ever ask for one right now
        // so, drop the rest of pak_in
        END_PACKET
    }
}

void missionserver_map_requestArcData_mapTest(int arcid)
{

    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_ARCDATA);
    pktSendBitsAuto(pak_out, 0);
    pktSendBitsAuto(pak_out, arcid);
    pktSendBitsAuto(pak_out, 0);
    pktSendBitsAuto(pak_out, 11);
    pktSendBitsAuto(pak_out, MISSIONSEARCH_ARCDATA_MAPTEST);
    pktSendBitsAuto(pak_out, 0);
    pktSend(&pak_out, &db_comm_link);
}


void missionserver_map_requestArcData_playArc(Entity *e, int arcid, int test, int devchoice)
{
    if(e)
    {
        // TODO: this probably needs more checking
        // FIXME: add error messages
        if(e->taskforce_id)
        {
            // already on a taskforce
        }
        else
        {
            Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_ARCDATA);
            pktSendBitsAuto(pak_out, e->db_id);
            pktSendBitsAuto(pak_out, arcid);
            pktSendBitsAuto(pak_out, s_authid(e));
            pktSendBitsAuto(pak_out, e->access_level);
            pktSendBitsAuto(pak_out, !!test);
            pktSendBitsAuto(pak_out, !!devchoice);
            pktSend(&pak_out, &db_comm_link);
        }
    }
}

void missionserver_map_requestArcData_viewArc(Entity *e, int arcid)
{
    if(e)
    {
        Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_ARCDATA);
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendBitsAuto(pak_out, arcid);
        pktSendBitsAuto(pak_out, s_authid(e));
        pktSendBitsAuto(pak_out, e->access_level);
        pktSendBitsAuto(pak_out, MISSIONSEARCH_ARCDATA_VIEWONLY);
        pktSendBitsAuto(pak_out, 0); // not relevant for viewing
        pktSend(&pak_out, &db_comm_link);
    }
}

void missionserver_map_requestArcData_otherUser(Entity *e, int arcid)
{
    // This goes down a separate command path because it gets arcs without
    // regard to the owner or the status of it. It can't be used by ordinary
    // players.
    if(e && e->access_level > 0)
    {
        Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_ARCDATA_OTHERUSER);
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendBitsAuto(pak_out, arcid);
        pktSend(&pak_out, &db_comm_link);
    }
}

void missionserver_map_requestInventory(Entity *e)
{
    if(e)
    {
        Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_INVENTORY);
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendBitsAuto(pak_out, s_authid(e));
        pktSend(&pak_out, &db_comm_link);
    }
}

void missionserver_map_grantTickets(Entity *e, int tickets)
{
    if(e)
    {
        missionserver_grantTickets(tickets, e->db_id, s_authid(e));
    }
}

void missionserver_map_claimTickets(Entity *e, int tickets)
{
    int ticketsToClaim = tickets;

    if(e)
    {
        const SalvageItem *ticketSalvage = salvage_GetItem("S_ArchitectTicket");
        int ticketsOwned = character_SalvageCount(e->pchar, ticketSalvage);

        if(ticketsOwned + ticketsToClaim > 9999)
        {
            ticketsToClaim = 9999 - ticketsOwned;
            sendChatMsg(e, localizedPrintf(e, "ExcessTicketClaimReduced", ticketsToClaim), INFO_ARCHITECT, 0);
        }

        if(ticketsToClaim > 0)
        {
            Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_CLAIM_TICKETS);

            pktSendBitsAuto(pak_out, e->db_id);
            pktSendBitsAuto(pak_out, s_authid(e));
            pktSendBitsAuto(pak_out, ticketsToClaim);

            pktSend(&pak_out, &db_comm_link);
        }
    }
}

void missionserver_map_grantOverflowTickets(Entity *e, int tickets)
{
    if(e && tickets)
    {
        badge_StatAddArchitectAllowed(e, "Architect.PendingOverflowTickets", tickets, 0);
        missionserver_grantOverflowTickets(tickets, e->db_id, s_authid(e));
    }
}

void missionserver_map_setBestArcStars(Entity *e, int stars)
{
    if(e && stars >= 0)
    {
        missionserver_setBestArcStars(stars, e->db_id, s_authid(e));
    }
}

void missionserver_map_setPaidPublishSlots(Entity *e, int paid_slots)
{
    if(e && paid_slots >= 0)
    {
        missionserver_setPaidPublishSlots(paid_slots, e->db_id, s_authid(e));
    }
}

void missionserver_map_handlePendingStatus(Entity *e)
{
    if(e)
    {
        int pendingTickets;

        pendingTickets = badge_StatGet(e, "Architect.PendingOverflowTickets");

        if(pendingTickets)
            missionserver_grantOverflowTickets(pendingTickets, e->db_id, s_authid(e));
    }
}

static bool s_findAndRemovePendingOrder(MissionInventory *inv, U32 cost, U32 flags)
{
    bool retval = false;
    MissionInventoryOrder *order;
    int count;

    if (inv)
    {
        for (count = 0; count < eaSize(&inv->pending_orders); count++)
        {
            if (inv->pending_orders && inv->pending_orders[count]->tickets == cost )
            {
                order = eaRemove(&inv->pending_orders, count);
                SAFE_FREE(order);
                retval = true;
                break;
            }
        }
    }

    return retval;
}

void missionserver_map_itemBought(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int authid = pktGetBitsAuto(pak_in);
    int cost = pktGetBitsAuto(pak_in);
    int flags = pktGetBitsAuto(pak_in);

    if (e)
    {
        s_findAndRemovePendingOrder(e->mission_inventory, cost, flags);
    }
    s_sendInventoryToClient(e);
}

void missionserver_map_itemRefund(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int authid = pktGetBitsAuto(pak_in);
    int cost = pktGetBitsAuto(pak_in);
    int flags = pktGetBitsAuto(pak_in);

    if (e)
    {
        if (s_findAndRemovePendingOrder(e->mission_inventory, cost, flags))
        {
            const SalvageItem *si = salvage_GetItem("S_ArchitectTicket");
            character_AdjustSalvage(e->pchar, si, cost, "ArchitectTicketsRefund", true);
            chatSendToPlayer( e->db_id, localizedPrintf(e, "ArchitectTicketRefundMessage", cost ), INFO_ARCHITECT, 0 );
        }
    }
    s_sendInventoryToClient(e);
}

void missionserver_map_overflowGranted(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int authid = pktGetBitsAuto(pak_in);
    int granted = pktGetBitsAuto(pak_in);
    int overflowTotal = pktGetBitsAuto(pak_in);

    if (e)
    {
        badge_StatSet(e, "Architect.OverflowTickets", overflowTotal);
        granted = badge_StatGet(e, "Architect.PendingOverflowTickets") - granted;

        // At least some of the outstanding tickets have been awarded.
        badge_StatSet(e, "Architect.PendingOverflowTickets", granted);
    }
}

static TFArchitectFlags s_ArchitectFlagsFromHonors(MissionHonors honors, int allow_devchoice)
{
    TFArchitectFlags retval = ARCHITECT_NORMAL;

    if( honors == MISSIONHONORS_HALLOFFAME )
        retval = ARCHITECT_HALLOFFAME;
    else if( honors == MISSIONHONORS_CELEBRITY )
        retval = ARCHITECT_FEATURED;
    else if( honors  == MISSIONHONORS_DEVCHOICE || honors  == MISSIONHONORS_DC_AND_HOF )
    {
        if( allow_devchoice )
            retval = ARCHITECT_DEVCHOICE;
        else
            retval = ARCHITECT_NORMAL|ARCHITECT_DEVCHOICE_NORMALREWARD;
    }

    return retval;
}

void missionserver_map_receiveArcData(Packet *pak_in)
{
    char *arcdata = NULL;
    int authorid = 0;
    MissionSearchInfo rating = {0};
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int arcid = pktGetBitsAuto(pak_in);
    int test = pktGetBitsAuto(pak_in);
    int devchoice = pktGetBitsAuto(pak_in);
    U32 arcdata_zip_bytes = 0;
    U32 arcdata_raw_bytes = 0;

    if(pktGetBool(pak_in))
    {
        authorid = pktGetBitsAuto(pak_in);
        rating.i = pktGetBitsAuto(pak_in);

        //arcdata = pktGetZipped(pak_in, NULL);
        arcdata = pktGetZippedInfo(pak_in,&arcdata_zip_bytes,&arcdata_raw_bytes);
    }

    if(test == MISSIONSEARCH_ARCDATA_MAPTEST && arcid)
    {
        missionServerMapTest_receiveArcs(arcid, arcdata);
    }
    else if(e && arcid)
    {
        if(test == MISSIONSEARCH_ARCDATA_VIEWONLY)
        {
            // this is a client request to view the arc
            START_PACKET(pak_out, e, SERVER_MISSIONSERVER_ARCDATA)
            pktSendBitsAuto(pak_out, arcid);
            if(pktSendBool(pak_out, arcdata))
            {
                //pktSendString(pak_out, arcdata); // FIXME: this should be a passthru!! (send bits array is unsafe for map<->game communication)
                pktSendZippedAlready(pak_out, arcdata_raw_bytes, arcdata_zip_bytes, arcdata );
            }
            END_PACKET
        }
        else
        {
            // TODO: this probably needs more checking
            // FIXME: add error messages
            if(e->taskforce_id)
            {
                // already on a taskforce
            }
            else if(!arcdata)
            {
                // arc does not exist
            }
            else
            {
                ClientLink *client = clientFromEnt(e);
                char* arcdata_unzipped = malloc(arcdata_raw_bytes+1);
                arcdata_unzipped[arcdata_raw_bytes] = 0;
                uncompress(arcdata_unzipped,&arcdata_raw_bytes,arcdata,arcdata_zip_bytes);
                TaskForceArchitectStart(e, arcdata_unzipped, arcid, test, s_ArchitectFlagsFromHonors(rating.honors, devchoice), authorid);
                if(client && isDevelopmentMode() )
                {
                    ContactDebugInteract(client, "Player_Created/MissionArchitectContact.contact", 0);
                }
                SAFE_FREE(arcdata_unzipped);
            }
        }
    }

    SAFE_FREE(arcdata);
}

void missionserver_map_receiveArcDataOtherUser(Packet *pak_in)
{
    char *arcdata = NULL;
    int authorid = 0;
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int arcid = pktGetBitsAuto(pak_in);
    U32 arcdata_zip_bytes = 0;
    U32 arcdata_raw_bytes = 0;

    if(pktGetBitsPack(pak_in, 1))
    {
        arcdata = pktGetZippedInfo(pak_in,&arcdata_zip_bytes,&arcdata_raw_bytes);

        if(arcdata)
        {
            START_PACKET(pak_out, e, SERVER_MISSIONSERVER_ARCDATA_OTHERUSER)
            pktSendBitsAuto(pak_out, arcid);
            pktSendZippedAlready(pak_out, arcdata_raw_bytes, arcdata_zip_bytes, arcdata);
            END_PACKET
        }
        else
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e, "GetArcZipDataCorrupt"), INFO_USER_ERROR, 0);
        }
    }
    else
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e, "GetArcNotFound"), INFO_USER_ERROR, 0);
    }

    SAFE_FREE(arcdata);
}

void missionserver_map_receiveInventory(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int authid = pktGetBitsAuto(pak_in);
    U32 bannedUntil = pktGetBitsAuto(pak_in);
    U32 tickets = pktGetBitsAuto(pak_in);
    U32 lifetimeTickets = pktGetBitsAuto(pak_in);
    U32 overflowTickets = pktGetBitsAuto(pak_in);
    U32 badgeFlags = pktGetBitsAuto(pak_in);
    U32 bestArcStars = pktGetBitsAuto(pak_in);
    U32 publishSlots = pktGetBitsAuto(pak_in);
    U32 publishSlotsUsed = pktGetBitsAuto(pak_in);
    U32 permanent = pktGetBitsAuto(pak_in);
    U32 invalid = pktGetBitsAuto(pak_in);
    U32 playableErrors = pktGetBitsAuto(pak_in);
    U32 uncirculatedSlots = pktGetBitsAuto(pak_in);
    U32 createdCompletions = pktGetBitsAuto(pak_in);
    int unlockKeys = pktGetBitsAuto(pak_in);
    int count;
    char *key;

    if(e)
    {
        if(!e->mission_inventory)
        {
            e->mission_inventory = calloc(1, sizeof(MissionInventory));
            e->mission_inventory->stKeys = stashTableCreateWithStringKeys(8,StashDefault);
        }

        stashTableClear(e->mission_inventory->stKeys );
        eaDestroyEx(&e->mission_inventory->unlock_keys, NULL);

        e->mission_inventory->banned_until = bannedUntil;
        e->mission_inventory->tickets = tickets;
        e->mission_inventory->lifetime_tickets = lifetimeTickets;
        e->mission_inventory->overflow_tickets = overflowTickets;
        e->mission_inventory->badge_flags = badgeFlags;
        e->mission_inventory->publish_slots = publishSlots;
        e->mission_inventory->publish_slots_used = publishSlotsUsed;
        e->mission_inventory->permanent = permanent;
        e->mission_inventory->uncirculated_slots = uncirculatedSlots;
        e->mission_inventory->created_completions = createdCompletions;
        e->mission_inventory->invalidated = invalid;
        e->mission_inventory->playableErrors = playableErrors;

        badge_StatSet(e, "Architect.AuthorTickets", lifetimeTickets);
        badge_StatSet(e, "Architect.OverflowTickets", overflowTickets);
        badge_StatSet(e, "Architect.Author", createdCompletions);
        badge_StatSet(e, "Architect.TotalStars", bestArcStars);

        if(badgeFlags & kBadgeArcDevChoice)
            badge_StatSet(e, "Architect.MarkedDevChoice", 1);
        if(badgeFlags & kBadgeArcHallOfFame)
            badge_StatSet(e, "Architect.MarkedHallOfFame", 1);

        if(badgeFlags & kBadgeArcFirstXPlayers)
            badge_StatSet(e, "Architect.FirstXPlayers", 1);
        if(badgeFlags & kBadgeArcCustomBossPublish)
            badge_Award(e, "ArchitectCustomBoss", true);

        if(badgeFlags & kBadgeArcVoteRating1)
            badge_StatSet(e, "Architect.VoteRating1", 1);
        if(badgeFlags & kBadgeArcVoteRating2)
            badge_StatSet(e, "Architect.VoteRating2", 1);
        if(badgeFlags & kBadgeArcVoteRating3)
            badge_StatSet(e, "Architect.VoteRating3", 1);
        if(badgeFlags & kBadgeArcVoteRating4)
            badge_StatSet(e, "Architect.VoteRating4", 1);
        if(badgeFlags & kBadgeArcVoteRating5)
            badge_StatSet(e, "Architect.VoteRating5", 1);


    }

    for(count = 0; count < unlockKeys; count++)
    {
        key = pktGetString(pak_in);

        if(e && e->mission_inventory)
        {
            char * pKey = strdup(key);
            eaPush(&e->mission_inventory->unlock_keys, pKey );
            stashAddPointer( e->mission_inventory->stKeys, pKey, pKey, 0 );
            playerCreatedStoryArc_AwardPCCCostumePart(e, pKey);
        }
    }

    s_sendInventoryToClient(e);
}

void missionserver_map_receiveTickets(Packet *pak_in)
{
    Entity *e = entFromDbId(pktGetBitsAuto(pak_in));
    int authid = pktGetBitsAuto(pak_in);
    int ticketsClaimed = pktGetBitsAuto(pak_in);
    int ticketsRemaining = pktGetBitsAuto(pak_in);
    int ticketsOwned;
    char *msg;

    strdup_alloca(msg, pktGetString(pak_in));

    if(e)
    {
        const SalvageItem *ticketSalvage = salvage_GetItem("S_ArchitectTicket");

        if(!e->mission_inventory)
        {
            e->mission_inventory = calloc(1, sizeof(MissionInventory));
            e->mission_inventory->stKeys = stashTableCreateWithStringKeys(8,StashDefault);
        }

        e->mission_inventory->tickets = ticketsRemaining;

        ticketsOwned = character_SalvageCount(e->pchar, ticketSalvage);

        // We can only hold 9999 of a given salvage item so any extra tickets
        // are lost.
        if(ticketsOwned + ticketsClaimed > 9999)
        {
            sendChatMsg(e, localizedPrintf(e, "ExcessTicketSalvageLost", ticketsClaimed, (ticketsOwned + ticketsClaimed) - 9999), INFO_ARCHITECT, 0);
            ticketsClaimed = 9999 - ticketsOwned;
        }

        character_AdjustSalvage(e->pchar, ticketSalvage, ticketsClaimed, "ArchitectTicketsClaim", true);

        if(msg)
        {
            sendChatMsg(e, localizedPrintf(e, msg, ticketsClaimed, ticketsRemaining), INFO_ARCHITECT, 0);
        }

        s_sendInventoryToClient(e);
    }
}

////////////////////////////////////////////////////////////////////////////////

void architectKiosk_SetOpen(Entity *e, int open)
{
    START_PACKET(pak, e, SERVER_ARCHITECTKIOSK_SETOPEN)
        pktSendBits(pak, 1, open);
    END_PACKET
}

// verify client input of kiosk click
void architectKiosk_recieveClick( Entity *e, Packet *pak)
{
    Vec3 pos;

    char * kioskName = pktGetString(pak);
    pos[0] = pktGetF32(pak);
    pos[1] = pktGetF32(pak);
    pos[2] = pktGetF32(pak);

    // Todo: bounds or range checking
    // Set flags on entity

    architectKiosk_SetOpen(e, 1);

}

void architectForceQuitTaskforce(Entity *e)
{
    if( e && e->pl && e->pl->taskforce_mode )
    {
        team_MemberQuit(e);
    }
}