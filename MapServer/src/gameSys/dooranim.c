/*\
 *  dooranim.c - server functions to handle animating players through doors
 *
 *  Copyright 2003 Cryptic Studios, Inc.
 */

#include "gameSys/dooranim.h"
#include "gameComm/dooranimcommon.h"
#include "svr/svr_base.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "svr/svr_player.h"
#include "dbcomm/dbdoor.h"
#include "gameComm/sendToClient.h"
#include "comm_game.h"
#include "ai/entai.h"
#include <utilitieslib/utils/Breakpoint.h>
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbmapxfer.h"
#include "player/parseClientInput.h"
#include <utilitieslib/components/earray.h>
#include "gridcoll/gridcoll.h"
#include <utilitieslib/utils/error.h>
#include "gridcoll/megaGrid.h"
#include "ai/entaivars.h"
#include "storyarc/storyarcinterface.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "gridcoll/gridfind.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include <utilitieslib/utils/utils.h>
#include "entity/teamCommon.h"
#include "bases/baseserver.h"
#include "gameData/raidstruct.h"
#include "entity/mapHistory.h"
#include "door.h"
#include "storyarc/storyarcutil.h"
#include <utilitieslib/components/StashTable.h>
#include "storyarc/taskforce.h"
#include "entity/character_pet.h"
#include "LWC_common.h"
#include "language/langServerUtil.h"

#define MAX_DOORANIM_BEACON_DIST        15.0    // dist for everything to and from EnterMe/EmergeFromMe

#define MAX_DIST_LOOK_FOR_DOOR            15.0    // distance from DoorEntry to entity
#define MAX_DIST_LOOK_FOR_ENTER_POINT    10.0        // distance from EnterMe to door beacon
#define MAX_DIST_LOOK_FOR_ENTRY_POINT    10.0    // distance from EnterMe to EntryPoint
#define MAX_DIST_LOOK_FOR_EMERGE_POINT    20.0    // distance from spawn point to EmergeFromMe
#define MAX_DIST_LOOK_FOR_EXIT_POINTS    5.0    // distance from EmergeFromMe to ExitPoint's

#define MAX_DOOROPEN_WAIT                11.0    // seconds to wait for door animation

static void DoorAnimStartClientAnim(Entity* player);

/////////////////////////////////////////////////////////////// points necessary for door animations

#define DA_GRID_DXYZ            MAX_DOORANIM_BEACON_DIST
#define DA_GRID_NODETYPE        3
#define POINT_FIND_LIMIT        100


// grid for DoorAnimPoint's
Grid        g_dooranimgrid        = {0};
DoorAnimPoint** g_dooranimlist = 0;

// temp for now
#define NUM_WARNINGS 10
int g_warnings = 0;

// the result of preloading is this call
//   gives single structure for the corresponding door anim points from a Vec3
DoorAnimPoint* DoorAnimFindPoint(const Vec3 pos, DoorAnimPointType type, F32 radius)
{
    DoorAnimPoint* points[POINT_FIND_LIMIT];
    F32 dist, bestdist = radius * radius + 0.0001;
    DoorAnimPoint* bestpoint = NULL;
    int gridCount, i;

    gridCount = gridFindObjectsInSphere(&g_dooranimgrid, (void**)points, ARRAY_SIZE(points), pos, radius);
    for (i = 0; i < gridCount; i++)
    {
        if (points[i]->type != type)
            continue;
        dist = distance3Squared(points[i]->centerpoint, pos);
        if (dist < bestdist)
        {
            bestdist = dist;
            bestpoint = points[i];
        }
    }
    return bestpoint;
}

int getBestEntryPointFromEntry( DoorAnimPoint* entry, const Vec3 pos )
{
    F32 dist, bestdist = 100000.0;
    int i, bestEntryPoint = 0;
    int bestEntryPointThatsCloserToDoorThanMe = -1;
    F32 bestEntryPointThatsCloserToDoorThanMeDist = 100000.0;

    // the aux point is optional
    if (entry->numauxpoints < 1)
    {
        copyVec3(entry->centerpoint, entry->auxpoints[0]);
        entry->numauxpoints = 1;
    }

    if( !pos )
        return 0;
 
    //Find the closest entry point of the closest door
    for( i = 0 ; i < entry->numauxpoints ; i++ )
    {
        dist = distance3Squared(entry->auxpoints[i], pos);
        if ( dist < bestdist )
        {
            bestdist = dist;
            bestEntryPoint = i;
        }
        //Make a preferred list of entry points between me and the entry point 
        if(        (distance3Squared(entry->centerpoint, entry->auxpoints[i] ) < distance3Squared(entry->centerpoint, pos) )
            &&  (distance3Squared(pos, entry->auxpoints[i] ) < distance3Squared(pos, entry->centerpoint ) ) )
        {
            if ( dist < bestEntryPointThatsCloserToDoorThanMeDist )
            {
                bestEntryPointThatsCloserToDoorThanMeDist = dist;
                bestEntryPointThatsCloserToDoorThanMe = i;
            }
        }
    }

    if( bestEntryPointThatsCloserToDoorThanMe >= 0 )
        bestEntryPoint = bestEntryPointThatsCloserToDoorThanMe;

    return bestEntryPoint;
}

//return the position entryPoint of the nearest DoorAnimPoint
F32 * DoorAnimFindEntryPoint(Vec3 pos, DoorAnimPointType type, F32 radius)
{
    DoorAnimPoint* entry = DoorAnimFindPoint(pos, type, radius);
    
    if( entry )
    {
        return entry->auxpoints[ getBestEntryPointFromEntry( entry, pos ) ];
    }
    return 0;
}


int DoorAnimFindGroupFromPoint(DoorAnimPoint** doorlist, DoorAnimPoint* sourcedoor, int* numdoors, int maxdoors)
{
    DoorAnimPoint* points[POINT_FIND_LIMIT];
    F32 *dist;
    F32 newdist;
    F32 farthest = MAX_DOORANIM_BEACON_DIST * MAX_DOORANIM_BEACON_DIST + 0.01;
    int gridCount, i, j;

    if(!sourcedoor)
        return 0;
    *numdoors = 1;

    ZeroMemory(doorlist, maxdoors * sizeof(DoorAnimPoint*));
    dist = malloc(maxdoors * sizeof(F32));
    dist[0] = 0;
    for(i = 1; i < maxdoors; i++)
        dist[i] = farthest;

    doorlist[0] = sourcedoor;
    gridCount = gridFindObjectsInSphere(&g_dooranimgrid, (void**)points, ARRAY_SIZE(points),
        sourcedoor->centerpoint, MAX_DOORANIM_BEACON_DIST);
    for(i = 0; i < gridCount; i++)
    {
        if (points[i]->type != sourcedoor->type)
            continue;
        newdist = distance3Squared(points[i]->centerpoint, sourcedoor->centerpoint);
        if ( (newdist || !(*numdoors)) && newdist < dist[maxdoors-1])    // avoid adding again if distance is zero (i.e. == sourcedoor)
        {
            (*numdoors)++;
            for(j = maxdoors - 1; j > 0; j--)
            {
                if(newdist < dist[j-1])
                {
                    dist[j] = dist[j-1];
                    doorlist[j] = doorlist[j-1];
                }
                else
                {
                    dist[j] = newdist;
                    doorlist[j] = points[i];
                    break;
                }
            }
        }
    }
    if(*numdoors > maxdoors)
        *numdoors = maxdoors;
    free(dist);

    return *numdoors;
}

void DestroyDoorAnimPoint(DoorAnimPoint* point)
{
    if (!point)
        return;
    if (point->animation)
        free(point->animation);
    free(point);
}

void DoorAnimUnload()
{
    eaDestroyEx(&g_dooranimlist, DestroyDoorAnimPoint);
    g_dooranimlist = 0;
    gridFreeAll(&g_dooranimgrid);
    memset(&g_dooranimgrid, 0, sizeof(g_dooranimgrid));
}

void DoorAnimGridAdd(DoorAnimPoint* point)
{
    Vec3 dv = { DA_GRID_DXYZ, DA_GRID_DXYZ, DA_GRID_DXYZ };
    Vec3 min, max;

    subVec3(point->centerpoint,dv,min);
    addVec3(point->centerpoint,dv,max);
    gridAdd(&g_dooranimgrid, point, min, max, DA_GRID_NODETYPE, &point->grid_idx);
}

int centerpointLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* pointProp;
    PropertyEnt* otherProp;

    // EnterMe
    if (traverser->def->properties && stashFindPointer( traverser->def->properties, "EnterMe", &pointProp ))
    {
        DoorAnimPoint* newInst = calloc(sizeof(DoorAnimPoint), 1);
        newInst->type = DoorAnimPointEnter;
        copyVec3(traverser->mat[3], newInst->centerpoint);

        // optional animation
        stashFindPointer( traverser->def->properties, "EnterAnimationBits", &otherProp );
        if (otherProp)
        {
            newInst->animation = _strdup(otherProp->value_str);
        }

        // optional animation
        stashFindPointer( traverser->def->properties, "DontRunIntoMe", &otherProp );
        if (otherProp)
        {
            newInst->dontRunIntoMe = 1;
        }

        eaPush(&g_dooranimlist, newInst);
        DoorAnimGridAdd(newInst);
    }

    // EmergeFromMe
    
    if (traverser->def->properties && stashFindPointer( traverser->def->properties, "EmergeFromMe", &pointProp ))
    {
        DoorAnimPoint* newInst = calloc(sizeof(DoorAnimPoint), 1);
        newInst->type = DoorAnimPointExit;
        copyVec3(traverser->mat[3], newInst->centerpoint);

        // optional animation
        stashFindPointer( traverser->def->properties, "EmergeAnimationBits", &otherProp );
        if (otherProp)
        {
            newInst->animation = _strdup(otherProp->value_str);
        }

        eaPush(&g_dooranimlist, newInst);
        DoorAnimGridAdd(newInst);
    }

    // if none of our children have props, don't go further
    if (!traverser->def->child_properties)
        return 0;
    return 1; // traverse children
}

int auxpointLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* pointProp;

    // EntryPoint
    if (traverser->def->properties && stashFindPointer( traverser->def->properties, "EntryPoint", &pointProp ))
    {
        DoorAnimPoint* parent = DoorAnimFindPoint(traverser->mat[3], DoorAnimPointEnter, MAX_DOORANIM_BEACON_DIST);
        if (!parent) 
        {
            g_warnings++;
            if (g_warnings <= NUM_WARNINGS)
                printf("Warning: orphaned EntryPoint at %0.2f, %0.2f, %0.2f (needs EnterMe)\n",
                    traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
            ErrorFilenamef(saUtilCurrentMapFile(), "Warning: orphaned EntryPoint at %0.2f, %0.2f, %0.2f (needs EnterMe)\n",
                    traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
            return 1;
        }

        if (parent->numauxpoints < MAX_DOOR_AUX_POINTS)
        {
            copyVec3(traverser->mat[3], parent->auxpoints[parent->numauxpoints]);
            stashFindPointer( traverser->def->properties, "Radius", &pointProp );
            if (pointProp)
                parent->auxpointRadius[parent->numauxpoints] = atof(pointProp->value_str);
            parent->numauxpoints++;
        }
    }

    // ExitPoint
    if (traverser->def->properties && stashFindPointer( traverser->def->properties, "ExitPoint", &pointProp ))
    {
        DoorAnimPoint* parent = DoorAnimFindPoint(traverser->mat[3], DoorAnimPointExit, MAX_DOORANIM_BEACON_DIST);

        if (!parent)
        {
            g_warnings++;
            if (g_warnings <= NUM_WARNINGS)
                printf("Warning: orphaned ExitPoint at %0.2f, %0.2f, %0.2f (needs EmergeFromMe)\n",
                    traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
            ErrorFilenamef(saUtilCurrentMapFile(), "Warning: orphaned ExitPoint at %0.2f, %0.2f, %0.2f (needs EmergeFromMe)\n",
                traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
            return 1;
        }
        if (parent->numauxpoints < MAX_DOOR_AUX_POINTS)
        {
            copyVec3(traverser->mat[3], parent->auxpoints[parent->numauxpoints]);
            stashFindPointer( traverser->def->properties, "Radius", &pointProp );
            if (pointProp)
                parent->auxpointRadius[parent->numauxpoints] = atof(pointProp->value_str);
            parent->numauxpoints++;
        }
        // just discarding point if we fail this check..
    }

    // if none of our children have props, don't go further
    if (!traverser->def->child_properties)
        return 0;
    return 1; // traverse children
}

// all EmergeFromMe points need at least one ExitPoint
void VerifyExitPoints()
{
    int i, n;

    n = eaSize(&g_dooranimlist);
    for (i = 0; i < n; i++)
    {
        if (g_dooranimlist[i]->type == DoorAnimPointExit && g_dooranimlist[i]->numauxpoints == 0)
        {
            g_warnings++;
            if (g_warnings <= NUM_WARNINGS)
            printf("Warning: orphaned EmergeFromMe point at %0.2f, %0.2f, %0.2f (needs at least one ExitPoint)\n",
                g_dooranimlist[i]->centerpoint[0], g_dooranimlist[i]->centerpoint[1], g_dooranimlist[i]->centerpoint[2]);
        }
    }
}

// Make sure all doors have something for the user to click on to enter
void VerifyMissionDoorsAreAttached()
{
    int i, n;
    DoorEntry* doors = dbGetLocalDoors(&n);
    
    for (i = 0; i < n; i++)
    {
        DoorEntry* door = &doors[i];
        if (door->type == DOORTYPE_MISSIONLOCATION)
        {
            DoorAnimPoint* point = DoorAnimFindPoint(door->mat[3], DoorAnimPointEnter, MAX_DOOR_BEACON_DIST);
            if (!point && !groupFindDoorVolumeWithinRadius(door->mat[3], MAX_DOOR_BEACON_DIST))
                ErrorFilenamef(saUtilCurrentMapFile(), "Door does not have a physical location(%.2f, %.2f, %.2f)\n", door->mat[3][0], door->mat[3][1], door->mat[3][2]);
        }
    }
}

// load all the points from the map we will need to do animations
void DoorAnimLoad()
{
    GroupDefTraverser traverser = {0};

    DoorAnimUnload();
    loadstart_printf("Load door animation points.. ");
    groupProcessDefExBegin(&traverser, &group_info, centerpointLoader);
    memset(&traverser, 0, sizeof(traverser));
    groupProcessDefExBegin(&traverser, &group_info, auxpointLoader);
    VerifyExitPoints();
    VerifyMissionDoorsAreAttached();
    loadend_printf("%i found", eaSize(&g_dooranimlist));
}

////////////////////////////////////////////////////////// find a nearby door

Entity *getTheDoorThisGeneratorSpawned(Vec3 pos)
{
    Entity * door = 0;
    int doorEntIds[10000];
    int doorEntsCount;
    int i, idx;
    Entity *bestdoor = 0;
    F32 bestdist = MAX_DIST_LOOK_FOR_DOOR * MAX_DIST_LOOK_FOR_DOOR;
    F32 dist;
    Vec3 diff;

    //Find me the nearest door, (this way seems dumb, but it works ok.)
    doorEntsCount = mgGetNodesInRange(0, pos, (void **)doorEntIds, 0);
    //doorEntsCount = gridFindObjectsInSphere(&ent_grid,(void **)doorEntIds,ARRAY_SIZE(doorEntIds),pos,10.f);
    for( i = 0 ; i < doorEntsCount ; i++ )
    {
        EntType entType;
        idx = doorEntIds[i];
        door = entities[idx];
        if (!door)
            continue;
        entType = ENTTYPE_BY_ID(idx);
        if (entType != ENTTYPE_DOOR)
            continue;
        subVec3(ENTPOS(door), pos, diff);
        dist = lengthVec3Squared(diff);
        if( dist < bestdist )
        {
            bestdoor = door;
            bestdist = dist;
        }
    }
    return bestdoor;
}

//////////////////////////////////////////////////// entering doors

void DoorAnimFreezePlayer(Entity* player)
{
    if (player->client)
        svrResetClientControls(player->client, 0);
    player->controlsFrozen = 1;
    player->doorFrozen = 1;
    player->untargetable |= UNTARGETABLE_TRANSFERING;
    player->invincible |= INVINCIBLE_TRANSFERING;
    player->move_type = MOVETYPE_NOCOLL;
}


void DoorAnimUnfreezePlayer(Entity* player)
{
    if (player->client)
        svrResetClientControls(player->client, 0);
    player->controlsFrozen = 0;
    player->doorFrozen = 0;
    player->untargetable &= ~UNTARGETABLE_TRANSFERING;
    player->invincible &= ~INVINCIBLE_TRANSFERING;
    player->move_type = MOVETYPE_WALK;
}

// entity is done with animation, and needs to be actually moved
void DoorAnimDoMovement(Entity* player, DoorAnimState* anim, int map_id, char* mapinfo, char* spawnTarget, MapTransferType xfer)
{
    int sgid = 0, userid = 0;
    // special case for local mapservers and "MissionReturn"
    if (mapinfo)
        SGBaseDeconstructInfoString(mapinfo,&sgid,&userid);
    if (db_state.local_server && spawnTarget && !stricmp(spawnTarget, "MissionReturn")) 
    {
        if(anim)
        {
            entUpdateMat4Instantaneous(player, anim->initialpos);
        }

        prepEntForEntryIntoMap(player, EE_USE_EMERGE_LOCATION_IF_NEARBY, xfer);
    }
    // set up map move if needed
    else if( map_id != db_state.map_id &&
            !((xfer & XFER_FINDBESTMAP) && map_id == db_state.base_map_id) &&
            !((xfer & XFER_BASE) && db_state.map_supergroupid && sgid == db_state.map_supergroupid) &&
            !((xfer & XFER_BASE) && db_state.map_userid && userid == db_state.map_userid) )
    {
        if ((xfer & XFER_MISSION) && (!player->teamup || player->teamup->map_id != map_id)) {
            // Player lost his team's mission map while doing the animation
            prepEntForEntryIntoMap(player, EE_USE_EMERGE_LOCATION_IF_NEARBY, xfer);
        } else {
            if (spawnTarget)    
                strcpy(player->spawn_target, spawnTarget);
            else
                strcpy(player->spawn_target,"");

            if (xfer & XFER_MISSION)
                player->db_flags |= DBFLAG_MISSION_ENTER;            
            else if (!stricmp(spawnTarget, "MissionReturn"))
                player->db_flags |= DBFLAG_MISSION_EXIT;            
            else if (!stricmp(spawnTarget, "BaseReturn"))
                player->db_flags |= DBFLAG_BASE_EXIT;
            else if (xfer & XFER_BASE)
                player->db_flags |= DBFLAG_BASE_ENTER;
            else if (xfer & XFER_FROMINTRO)
                player->db_flags |= DBFLAG_INTRO_TELEPORT;                        
            else
                player->db_flags |= DBFLAG_DOOR_XFER;
            

            dbAsyncMapMove(player,map_id,mapinfo,xfer);
        }
    }
    else if (spawnTarget) // intra-map move
    {
        if (!stricmp(spawnTarget, "DebugEnter")) // just doing a debug enter animation
        {
            if (anim)
                entUpdateMat4Instantaneous(player, anim->initialpos);
 
            //TestArenaMove
            //seqSetStateFromString( player->seq->state, "ARENAEMERGE" );
            
            prepEntForEntryIntoMap(player, EE_USE_EMERGE_LOCATION_IF_NEARBY, xfer);
        }
        else if (!strnicmp(spawnTarget, "coord:",6))
        {
            Vec3 pos;
            Mat4 newMat;
            int unsued = 0;

            sscanf(spawnTarget+6, "%f:%f:%f:%d", &pos[0], &pos[1], &pos[2], &unsued);
            copyMat4(ENTMAT(player), newMat); // dooranim will reset back to here
            copyVec3(pos, newMat[3]);

            entUpdateMat4Instantaneous(player, newMat);
            prepEntForEntryIntoMap(player, EE_USE_EMERGE_LOCATION_IF_NEARBY, xfer);
        }
        else
        {
            DoorEntry *target = NULL;
            Entity* exitdoor = anim ? erGetEnt(anim->exitdoor) : NULL;
            
            if (exitdoor) // if we already chose a specific door, use that exit point
            {
                target = dbFindLocalMapObj(ENTPOS(exitdoor), DOORTYPE_SPAWNLOCATION, MAX_DIST_LOOK_FOR_DOOR);
            }
            if (!target)
                target = dbFindSpawnLocation(spawnTarget);
            if (!target && (xfer & XFER_BASE))
            { //If we're porting into a base gurney and its not there, don't crash but don't revive either
                target = dbFindSpawnLocation("PlayerSpawn");
            }
            //assert(target && "DoorAnimDoMovement - invalid target? Should already have caught this");
            if (target)
            {                
                entUpdateMat4Instantaneous(player, target->mat);

                if (xfer & XFER_GURNEY) {
                    if (xfer & XFER_BASE && target && target->detail_id)
                        completeBaseGurneyXferWork(player,target->detail_id);
                    else 
                        completeGurneyXferWork(player);
                }
            }
            // get client unfrozen and faded in
            DoorAnimUnfreezePlayer(player); 
            prepEntForEntryIntoMap(player, EE_USE_EMERGE_LOCATION_IF_NEARBY, xfer);
        }
    }
}

// set up map_id, and validate spawnTarget
// needs to happen before we start animating
int DoorAnimValidateMove(Entity* player, int* map_id, char* mapinfo, char* spawnTarget, char* errmsg)
{
    LWC_STAGE mapStage;

    if (!LWC_CheckMapIDReady(*map_id, LWC_GetCurrentStage(player), &mapStage))
    {
        // TODO: log this?
        //printf("DoorAnimValidateMove: LWC_CheckMapIDReady(%d, %d, %p) failed\n", *map_id, currentStage, &mapStage);
        // Should this just set the errmsg string?
        sendDoorMsg(player, DOOR_MSG_FAIL, localizedPrintf(player, "LWCDataNotReadyMap", mapStage));
        return 0;
    }

    // we assume if we have a mapinfo, the dbserver will take care of starting the map
    // as needed - it should not be possible to fail the transfer
    if (mapinfo)
        return 1; // ok

    if (*map_id <= 0)
        *map_id = db_state.base_map_id;
    //if (spawnTarget && !stricmp(spawnTarget, "MissionReturn"))
    //    *map_id = -1; // last static map
    if (*map_id == db_state.map_id || *map_id == db_state.base_map_id)
    {
        DoorEntry *target;

        if (!stricmp(spawnTarget, "DebugEnter"))
            return 1; // ok
        if (!strnicmp(spawnTarget, "coord:",6))
            return 1;
        target = dbFindSpawnLocation(spawnTarget);
        if (!target)
        {
            if (errmsg)
                sprintf(errmsg,"can't find spawn target: %s on current map",spawnTarget);
            return 0;
        }
    }
    return 1; // ok
}

// called every tick for each player, runs animation
void DoorAnimCheckMove(Entity* player)
{
    DoorAnimState* anim;
    
    if(player->pl)
        anim = player->pl->door_anim;
    else
        anim = ENTAI(player)->doorAnim;

    // for any player who is being moved
    if (anim)
    {
        // start here. don't do animation until doors are open.
        // timeout after 5 seconds of waiting for an open animation
        if (anim->state == DOORANIM_WAITDOOROPEN)
        {
            int doorsopen = 1;
            Entity* entrydoor = erGetEnt(anim->entrydoor);
            Entity* exitdoor = erGetEnt(anim->exitdoor);

            //TO DO a flag to check if door is actually trying to open?

            if(entrydoor)
            {
                aiDoorSetOpen(entrydoor, 1);
                if (!getMoveFlags(entrydoor->seq, SEQMOVE_WALKTHROUGH))
                    doorsopen = 0;
            }
            if (exitdoor)
            {
                aiDoorSetOpen(exitdoor, 1);
                if (!getMoveFlags(exitdoor->seq, SEQMOVE_WALKTHROUGH))
                    doorsopen = 0;
            }
            if (doorsopen || (ABS_TIME_SINCE(anim->starttime) > SEC_TO_ABS(MAX_DOOROPEN_WAIT)))
            {
                if( anim->dontRunIntoMe )
                {
                    //Just set the bits and wait for the animation to finish, no orientation or moving
                    anim->state = DOORANIM_ANIMATION;
                }
                else
                {
                    //Do full turn and run in 
                    anim->state = DOORANIM_BEGIN;
                }
                if(player->pl)
                    DoorAnimStartClientAnim(player);
            }
        }
        // when animation is done, wait for client to ack that it is done with animation too
        else if (anim->state == DOORANIM_DONE)
        {
            if (anim->clientack)
            {
                anim->state = DOORANIM_ACK;
            }
        }
        // totally done now, clean up animation and move player
        else if (anim->state == DOORANIM_ACK)
        {
            // now actually do the move (find best map if not returning from a mission)
            if(player->pl)
                DoorAnimDoMovement(player, anim, anim->map_id, anim->mapinfo, anim->spawnTarget, anim->xfer);

            if (anim->animation)
                free(anim->animation);
    
            free(anim);
            if(player->pl)
                player->pl->door_anim = NULL;
            else
                ENTAI(player)->doorAnim = NULL;
            // done with animation
        }
        // otherwise, we're in part of the animation
        else
        {
            DoorAnimMove(player, anim);
        }
    }
}

void DoorAnimClientDone(Entity* player)
{
    if (player->pl->door_anim)
        player->pl->door_anim->clientack = 1;
}

static void DoorAnimStartServerAnim(Entity* player, DoorEntry* door, const Vec3 doorPos, int map_id, char* mapinfo, char* spawnTarget, MapTransferType xfer)
{
    static DoorAnimPoint arenaVirtualEntryPoint;
    DoorAnimState* anim;
    int haveEntryPoint = 1;
    DoorAnimPoint* entryPoint = NULL;

    // if we don't have any animation points, don't do an animation
    if (door && player->client)
    {
        entryPoint = DoorAnimFindPoint(doorPos, DoorAnimPointEnter, MAX_DOORANIM_BEACON_DIST);
    }

    //When you are leave this map to go to the Arena, play an animation first; there's no door nearby to run into or to tell you what animation to do, so make a door here.  
    //(To add another virtualDoor, add it here and to the check in DoorAnimEnter() so it doesn't get short-circuited by not having a door.) Everything else is handled.
    if ( xfer & XFER_ARENA ) 
    {
        arenaVirtualEntryPoint.dontRunIntoMe = 1;
        arenaVirtualEntryPoint.animation = "ARENA WARP";
        entryPoint = &arenaVirtualEntryPoint;
    }

    if (!entryPoint)
    {
        DoorAnimDoMovement(player, NULL, map_id, mapinfo, spawnTarget, xfer);
        return; // no animation possible
    }

    anim = DoorAnimInitState( entryPoint, ENTPOS(player) );
    //Entry point to use is the one closest to the player
    

    // destination info
    anim->map_id = map_id;
    anim->xfer = xfer;
    if (mapinfo)
        strncpyt(anim->mapinfo, mapinfo, ARRAY_SIZE(anim->mapinfo));
    if (spawnTarget)
    {
        DoorEntry *exitdoor;
        strncpyt(anim->spawnTarget, spawnTarget, ARRAY_SIZE(anim->spawnTarget));
        exitdoor = dbFindSpawnLocation(spawnTarget);
        if (exitdoor)
            anim->exitdoor = erGetRef(getTheDoorThisGeneratorSpawned(exitdoor->mat[3]));
    }

    // animation will start next clock tick
    player->pl->door_anim = anim;
}

// tell client to start animating through door.  will cause a
// CLIENTINP_DOORANIM_DONE response
// must be called after DoorAnimStartServerAnim
static void DoorAnimStartClientAnim(Entity* player)
{
    if (!player->pl->door_anim)
        return; // ok, not doing animation

    START_PACKET(pak1, player, SERVER_DOORANIM)
    pktSendF32(pak1, player->pl->door_anim->entrypos[0]);
    pktSendF32(pak1, player->pl->door_anim->entrypos[1]);
    pktSendF32(pak1, player->pl->door_anim->entrypos[2]);
    pktSendF32(pak1, player->pl->door_anim->targetpos[0]);
    pktSendF32(pak1, player->pl->door_anim->targetpos[1]);
    pktSendF32(pak1, player->pl->door_anim->targetpos[2]);
    pktSendF32(pak1, player->pl->door_anim->entryPointRadius);

    pktSendBits(pak1, 1, player->pl->door_anim->animation? 1: 0);
    if (player->pl->door_anim->animation)
        pktSendString(pak1, player->pl->door_anim->animation);

    pktSendBits(pak1, 32, player->pl->door_anim->state );
    END_PACKET
}

// top-level function.  handles animation and getting player to destination
int DoorAnimEnter(Entity* player, DoorEntry* door, const Vec3 doorPos, int map_id, char* mapinfo, char* spawnTarget, 
                  MapTransferType xfer, char* errmsg)
{
    assert(player->pl);    // door animations are only for players

    // validate spawnTarget first & fix map_id
    if (!CanEnterDoor(player) || !DoorAnimValidateMove(player, &map_id, mapinfo, spawnTarget, errmsg))
        return 0;

    { // save our last position before animating or transferring
        int mapType = getMapType();
        if (mapType == MAPTYPE_STATIC)
        { // We're leaving a static map, clear the history
            mapHistoryClear(player->pl);
            copyVec3(ENTPOS(player), player->pl->last_static_pos);
            getMat3YPR(ENTMAT(player), player->pl->last_static_pyr);
            if (spawnTarget && strstri(spawnTarget,"Gurney"))
            { //if we're doing a gurney transfer, note it.
                MapHistory *hist = mapHistoryCreate();
                hist->mapType = MAPTYPE_STATIC;
                hist->mapID = getMapID();
                copyVec3(ENTPOS(player), hist->last_pos);
                getMat3YPR(ENTMAT(player), hist->last_pyr);
                mapHistoryPush(player->pl,hist);
            }
        }
        else 
        { //We're leaving a non-static map: add to history
            if (spawnTarget && (stricmp(spawnTarget, "MissionReturn") == 0 || stricmp(spawnTarget, "BaseReturn") == 0)) 
            { //If we're doing a return, remove from stack, don't add to it
                MapHistory *hist = mapHistoryLast(player->pl);
                if (hist && hist->mapType == mapType && hist->mapID == getMapID())
                { //only pop stack if the map we're leaving is the top one
                    hist = mapHistoryPop(player->pl);
                    mapHistoryDestroy(hist);
                }
            }
            else 
            {            
                MapHistory *hist = mapHistoryCreate();
                hist->mapType = mapType;
                hist->mapID = getMapID();
                copyVec3(ENTPOS(player), hist->last_pos);
                getMat3YPR(ENTMAT(player), hist->last_pyr);
                mapHistoryPush(player->pl,hist);
            }
        }
    }

    // allow clients to use this as a transfer method without a door
    //IF you are going to the arena, we have a virtual door for you to go through
    if (!door && !(xfer & XFER_ARENA))
    {
        DoorAnimState fakeanim = {0};
        copyMat4(ENTMAT(player), fakeanim.initialpos);
        DoorAnimFreezePlayer(player);  
        DoorAnimDoMovement(player, &fakeanim, map_id, mapinfo, spawnTarget, xfer);
        return 1;
    }

    // don't allow animation to be queued twice
    if (player->pl->door_anim)
    {
        if (errmsg)
            sprintf(errmsg, "Already moving through door"); // localize?  shouldn't happen
        return 0;
    }
    DoorAnimFreezePlayer(player);  
    DoorAnimStartServerAnim(player, door, doorPos, map_id, mapinfo, spawnTarget, xfer);

    // have pets follow through door
    if( !(xfer&XFER_STATIC) )
        character_AllPetsRunIntoDoor(player->pchar);


    return 1;
}

// run enter animation to nearest door
void DoorAnimDebugEnter(ClientLink* client, Entity* player, int toArena)
{
    // find a nearby door
    DoorEntry* door = dbFindLocalMapObj(ENTPOS(player), 0, 20);
    if ( !toArena && !door)
    {
        conPrintf(client, "No map link nearby");
        return;
    }

    // run the entry animation
    if( toArena ) //TestArenaMove
        DoorAnimEnter(player, 0, ENTPOS(player), 0, NULL, "DebugEnter", XFER_ARENA, NULL);
    else
        DoorAnimEnter(player, door, door->mat[3], 0, NULL, "DebugEnter", XFER_STATIC, NULL);

    if (player->pl->door_anim)
        copyMat4(ENTMAT(player), player->pl->door_anim->initialpos); // dooranim will reset back to here
    // otherwise, we didn't do an animation
}

// run exit animation from nearest door
void DoorAnimDebugExit(ClientLink* client, Entity* player, int toArena )
{
    if( toArena )
    {
        seqSetStateFromString( player->seq->state, "ARENAEMERGE" );
        prepEntForEntryIntoMap(player, 0, 0 );
    }
    else
    {
        prepEntForEntryIntoMap(player, EE_USE_EMERGE_LOCATION_IF_NEARBY, 0);
    }
}

////////////////////////////////////////////////////////////////////// exiting doors (just running an animation)

void prepEntForEntryIntoMap(Entity *e, int flags, MapTransferType xfer )
{
    int sentNewLocation = 0;
    DoorAnimPoint* exitPoint = NULL;

    // make sure to clear animations if we started one on other side of door
    SETB(e->seq->state, STATE_EMERGE);

    // get client unfrozen and faded in
    DoorAnimUnfreezePlayer(e);

    // get the nearest emerge point
    if( flags & EE_USE_EMERGE_LOCATION_IF_NEARBY )
    {
         exitPoint = DoorAnimFindPoint(ENTPOS(e), DoorAnimPointExit, 2*MAX_DOORANIM_BEACON_DIST);
        if (exitPoint && exitPoint->numauxpoints)
        {
            sentNewLocation = 1; // Tell the client they should be expecting a warp to a new location, and to load textures appropriately
            EntRunOutOfDoor(e, exitPoint, xfer);
        }
    }

    START_PACKET(pak1, e, SERVER_DOOREXIT); // fade in again
    pktSendBits(pak1, 1, sentNewLocation);
    END_PACKET
}

void EntRunOutOfDoor(Entity *e, DoorAnimPoint* runoutdoor, MapTransferType xfer)
{
    Entity* door;
    Mat4 result;
    Vec3 dir, pyr;
    F32 yaw;

    // choose one of the corresponding exit points
    int i;
        
    if(!runoutdoor)
    {
        static int numErrors = 0;

        numErrors++;
        if(numErrors < 5)
            Errorf("No door passed into EntRunOutOfDoor");
        return;
    }

    if(runoutdoor->numauxpoints)
        i = rand() % runoutdoor->numauxpoints;
    else
    {
        i = 0;
        copyVec3(runoutdoor->centerpoint, runoutdoor->auxpoints[0]);
        runoutdoor->numauxpoints = 1;
    }

    // make sure to clear animations if we started one on other side of door
    SETB(e->seq->state, STATE_EMERGE);

    // get client unfrozen and faded in
    DoorAnimUnfreezePlayer(e);

    // move player to exit point and orient away from emerge point
    copyMat4(ENTMAT(e), result);
    copyVec3(runoutdoor->auxpoints[i], result[3]);
    subVec3(runoutdoor->auxpoints[i], runoutdoor->centerpoint, dir);
    yaw = getVec3Yaw(dir);
    getMat3YPR(result, pyr);
    pyr[1] = yaw;
    createMat3YPR(result, pyr);
    entUpdateMat4Instantaneous(e, result);
    character_AllPetsRunOutOfDoor(e->pchar, result[3], xfer );

    // play the exit animation plus any additional bits the EmergeFromMe has for you
    SETB(e->seq->state, STATE_RUNOUT);
    if (runoutdoor->animation) 
    {
        seqSetStateFromString(e->seq->state, runoutdoor->animation);
    }

    // get the nearest door that was generated
    door = getTheDoorThisGeneratorSpawned(runoutdoor->centerpoint);
    if (door)
        aiDoorSetOpen(door, 1);
}



DoorAnimState *DoorAnimInitState(DoorAnimPoint *entryPoint, const Vec3 pos)
{
    DoorAnimState *anim;
    int bestEntryPoint;

    if(!entryPoint)
        return NULL;

    bestEntryPoint = getBestEntryPointFromEntry( entryPoint, pos );

    // set up the animation structure
    anim = calloc(sizeof(DoorAnimState), 1);
    anim->clientack = 0;
    anim->state = DOORANIM_WAITDOOROPEN;
    anim->dontRunIntoMe = entryPoint->dontRunIntoMe; 
    anim->entryPointRadius = entryPoint->auxpointRadius[bestEntryPoint];
    anim->starttime = ABS_TIME;
    anim->entrydoor = erGetRef(getTheDoorThisGeneratorSpawned(entryPoint->centerpoint));
    copyVec3(entryPoint->auxpoints[bestEntryPoint], anim->entrypos);
    copyVec3(entryPoint->centerpoint, anim->targetpos);
    if (entryPoint->animation)
        anim->animation = strdup(entryPoint->animation);

    return anim;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/*
void DoorAnimSetLastTrickOrTreat(DoorAnimPoint *entryPoint, U32 time)
{
    entryPoint->lastTrickOrTreat = time;
}

U32 DoorAnimGetLastTrickOrTreat(DoorAnimPoint *entryPoint)
{
    return entryPoint->lastTrickOrTreat;
}
*/

void DoorAnimAddTrickOrTreat(DoorAnimPoint *entryPoint, Entity *pEnt)
{
    eaiPush(&entryPoint->lastTrickOrTreat, pEnt->db_id);
}

int DoorAnimFindTrickOrTreat(DoorAnimPoint *entryPoint, Entity *pEnt)
{
    return (eaiFind(&entryPoint->lastTrickOrTreat, pEnt->db_id) != -1);
}

void DoorAnimClearTrickOrTreat()
{
    int i, n;

    n = eaSize(&g_dooranimlist);
    for (i = 0; i < n; i++)
    {
        if (g_dooranimlist[i]->lastTrickOrTreat)
        {
            eaiDestroy(&g_dooranimlist[i]->lastTrickOrTreat);
            g_dooranimlist[i]->lastTrickOrTreat = NULL;
        }
    }
}
