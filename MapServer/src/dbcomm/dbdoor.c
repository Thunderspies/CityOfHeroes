#include "dbdoor.h"
#include "dbcomm.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "storyarc/mission.h"
#include <utilitieslib/utils/strings_opt.h>
#include "gameComm/svr_chat.h"
#include "svr/svr_base.h"
#include "entity/entVarUpdate.h"
#include "gameComm/sendToClient.h"
#include "beacon/beaconConnection.h"
#include <utilitieslib/network/netcomp.h>
#include "staticMapInfo.h"
#include <utilitieslib/components/earray.h>
#include "storyarc/storyarcutil.h"
#include "beacon/beaconPrivate.h"
#include "beacon/beaconPath.h"
#include "gameSys/dooranim.h"
#include "entity/entity.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "comm_backend.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include "storyarc/storySend.h"
#include "bases/basedata.h"
#include "bases/bases.h"
#include "storyarc/contact.h"
#include "gameSys/sgraid_V2.h"

typedef struct
{
    DoorEntry    *entries;
    int            count;
    int            max;
} DoorList;

static DoorList local_doors,db_doors;
static    char    *cb_search_str;

// Callback function used by gatherDoors() to determine which groupdefs to extract from map.
static ExtractionAction mapObjFind_cb(GroupDefTraverser *def_trav)
{
    if (def_trav->def->has_properties)
    {
        if (stashFindPointerReturnPointer(def_trav->def->properties,cb_search_str ) )
            return     EA_EXTRACT | EA_NO_BRANCH_EXPLORE;
    }
    if (!def_trav->def->child_properties)
        return EA_NO_BRANCH_EXPLORE;
    return EA_NO_EXTRACT;
}

DoorEntry *dbFindLocalDoorWithName(DoorType type, char *name)
{
    int i;

    DoorEntry* findTarget = NULL;

    if(!name || !name[0])
        return NULL;

    for(i = 0; i < db_doors.count; i++)
    {
        DoorEntry* door = &local_doors.entries[i];
        if(door->type != type)
            continue;
        if(door->name && stricmp(door->name, name) == 0)
        {
            findTarget = door;
            break;
        }
    }

    return findTarget;
}

DoorEntry *dbFindLocalMapObj(const Vec3 pos,int type,F32 max_dist)
{
    int            i;
    DoorEntry    *door,*best=0;
    DoorList    *doors = &local_doors;
    F32            sq,dist, maxDistDefault;

    dist = -1.0f;    //door not yet found at -1
    maxDistDefault = SQR(max_dist);
    for(i=0;i<doors->count;i++)
    {
        door = &doors->entries[i];
        if (type == 0) // looking for any door
        {
            // these aren't actually doors
            if (door->type == DOORTYPE_SPAWNLOCATION ||
                door->type == DOORTYPE_PERSISTENT_NPC ||
                door->type == DOORTYPE_TASK_VISIT_LOCATION)
                continue;
        }
        else if (type != door->type)
            continue;
        sq = distance3Squared(door->mat[3],pos);

        //is this distance within the radius of the door we clicked on and the closest one we've seen so far?
        {
            F32 minSuccessDist; //the distance you have to beat in order to be considered.

            if(door->radius_override)
            {
                minSuccessDist = SQR(door->radius_override);
            }
            else
            {
                minSuccessDist = maxDistDefault;
            }

            //if we've already found another door that is even closer, we've got to beat the other door.
            if((dist>=0.0f) &&    //if dist < 0 then no doors have been found yet, and we can skip this.
                dist < minSuccessDist)
            {
                minSuccessDist = dist;
            }

            if( sq < minSuccessDist)
            {
                best = door;
                dist = sq;
            }
        }
    }
    return best;
}

DoorEntry* dbFindReachableLocalMapObj(Entity* e, int type, F32 max_dist)
{
    int            i;
    DoorEntry    *door,*best=0;
    DoorList    *doors = &local_doors;
    F32            sq,dist;
    Beacon        *startBeacon, *doorBeacon;
    BeaconBlock* cluster;
    Entity* doorEnt;

    beaconSetPathFindEntity(e, ENTAI(e)->canFly ? 1000000 : aiGetJumpHeight(e));

    startBeacon = beaconGetClosestCombatBeacon(ENTPOS(e), 1, NULL, GCCB_PREFER_LOS, NULL);

    if(!startBeacon)
        return NULL;

    cluster = startBeacon->block->galaxy->cluster;

    beacon_state.galaxySearchInstance++;

    dist = SQR(max_dist);
    for(i=0;i<doors->count;i++)
    {
        F32 * doorAnimPointPos; //us this thing's position if available;
        Vec3 doorPositionToConsider;

        door = &doors->entries[i];
        doorEnt = getTheDoorThisGeneratorSpawned(door->mat[3]);

        if(!doorEnt && door->type != DOORTYPE_MISSIONLOCATION)
            continue;    // this is probably a warp volume so it doesn't actually have a door

        if (type == 0) // looking for any door
        {
            // these aren't actually doors usable for disappearing
            if (door->type == DOORTYPE_SPAWNLOCATION ||
                door->type == DOORTYPE_PERSISTENT_NPC ||
                door->type == DOORTYPE_TASK_VISIT_LOCATION)
                continue;

            if(door->type == DOORTYPE_FREEOPEN ||
                (door->type == DOORTYPE_KEY && !door->special_info))
            {

                if (doorEnt && (ENTAI(doorEnt)->door.notDisappearInDoor || ENTAI(doorEnt)->door.open))
                    continue;
            }
        }
        else if (type != door->type)
            continue;

        //doorEnt = getTheDoorThisGeneratorSpawned( door->mat[3] );
        doorAnimPointPos = DoorAnimFindEntryPoint(door->mat[3], DoorAnimPointEnter, MAX_DOORANIM_BEACON_DIST);

        //Prefer to look for the doorAnimPoint's entry point as a spot to run to
        if( doorAnimPointPos )
            copyVec3( doorAnimPointPos, doorPositionToConsider );
        else if(doorEnt)
            copyVec3( ENTPOS(doorEnt), doorPositionToConsider );
        else
            copyVec3( door->mat[3], doorPositionToConsider );
    
        
        sq = distance3Squared(doorPositionToConsider,ENTPOS(e));
        if( sq < dist )
        {
            doorBeacon = beaconGetClosestCombatBeacon(doorPositionToConsider, 1, startBeacon, GCCB_PREFER_LOS, NULL);
            if(!doorBeacon)
                continue;

            best = door;
            dist = sq;
        }
    }
    return best;
}

// find the door entry for any door on any map, based on position
DoorEntry *dbFindGlobalMapObj(int mapid, Vec3 pos)
{
    DoorEntry*    doorlist;
    int            numdoors, i;
    DoorEntry    *door, *best = NULL;
    F32            sq,dist = SQR(90);
    StaticMapInfo* info = staticMapInfoFind(mapid);
    
    // Get the base map id.
    
    if(info && info->baseMapID)
        mapid = info->baseMapID;

    numdoors = db_doors.count;
    doorlist = db_doors.entries;

    // just interate and find closest door
    for (i = 0; i < numdoors; i++)
    {
        door = &doorlist[i];
        if (door->map_id != mapid || door->type == DOORTYPE_SPAWNLOCATION)
            continue;
        sq = distance3Squared(door->mat[3],pos);
        if( sq < dist )
        {
            best = door;
            dist = sq;
        }
    }
    return best;
}

// Scans the loaded map and adds all map objects with the "lookfor" property to a list of doors name "doors".
static void gatherDoors(DoorList *doors,char *lookfor,char *lookfor_special,int type)
{
    int                    i;
    int                    j;
    int                    totalFound = 0;
    char                *name;
    Array                *door_array;
    GroupDefTraverser    *def_trav;
    DoorEntry            *door;

    cb_search_str = lookfor;

    // Grab a list of groupDefs that has a property name that matches "lookfor".
    door_array = groupExtractSubtrees( &group_info, mapObjFind_cb );
    for(i=0;i<door_array->size;i++)
    {
        door = dynArrayAdd(&doors->entries,sizeof(doors->entries[0]),&doors->count,&doors->max,1);
        door->map_id = db_state.map_id;        // Record which door the map is on.
        def_trav = door_array->storage[i];    // Save a copy of the traverser in case we need info from this door.  door_array now owns traverser.
        copyMat4(def_trav->mat,door->mat);    // Record where in the world the door is.
        name = groupDefFindPropertyValue(def_trav->def,lookfor);
        door->type = type;                    // Record a user supplied token to ID this door type.
        if (name)                            // Record the value of "look_for" property as door name.
        {
            door->name = strdup(name);
            if (stricmp(door->name,"ArchitectGurney")==0)
                db_state.has_gurneys = db_state.has_villain_gurneys = 1;
            if (stricmp(door->name,"Gurney")==0)
                db_state.has_gurneys = 1;
            if (stricmp(door->name,"VillainGurney")==0)
                db_state.has_villain_gurneys = 1;
        }
        if (lookfor_special)                // Record the value of "lookfor_special" property.
        {
            name = groupDefFindPropertyValue(def_trav->def,lookfor_special);
            if (name)
                door->special_info = strdup(name);
        }
        if (def_trav->def && def_trav->def->file)
            door->filename = strdup(def_trav->def->file->fullname);
        else
            door->filename = strdup("UNKNOWN");

        name = groupDefFindPropertyValue(def_trav->def,"Requires");
        if (name)
            door->required = strdup(name);
        name = groupDefFindPropertyValue(def_trav->def,"LockedText");
        if (name)
            door->lockedText = strdup(name);

        // DGNOTE 1/7/2010
        // To handle the shenanigens of places like the Pocket D door to Port Oakes, we have a sequence of requires, with a matching sequence of flags
        // and another matching sequence of "locked text" strings.
        for (j = 0; j < MAX_DOOR_REQUIRES; j++)
        {
            char requiresName[32];
            char lockedTextName[32];
            char allowThroughName[32];
            char *required;
            char *lockedText;
            char *allowThrough;

            _snprintf(requiresName, 32, "Requires%d", j + 1);    // +1 so the requires names as seen by design start at 1.
            requiresName[31] = 0;
            _snprintf(lockedTextName, 32, "LockedText%d", j + 1);
            lockedTextName[31] = 0;
            _snprintf(allowThroughName, 32, "AllowThrough%d", j + 1);
            allowThroughName[31] = 0;
            required = groupDefFindPropertyValue(def_trav->def, requiresName);
            lockedText = groupDefFindPropertyValue(def_trav->def, lockedTextName);
            allowThrough = groupDefFindPropertyValue(def_trav->def, allowThroughName);

            if (required == NULL)
            {
                break;
            }
            door->doorRequires[j].required = strdup(required);
            if (lockedText)
            {
                door->doorRequires[j].lockedText = strdup(lockedText);
            }
            if (allowThrough == NULL || stricmp(allowThrough, "false") == 0 || stricmp(allowThrough, "no") == 0)
            {
                door->doorRequires[j].allowThrough = 0;
            }
            else if (stricmp(allowThrough, "true") == 0 || stricmp(allowThrough, "yes") == 0)
            {
                door->doorRequires[j].allowThrough = 1;
            }
            else
            {
                door->doorRequires[j].allowThrough = atoi(allowThrough);
            }
        }

        door->entry_min = INVALID_DOOR_ENTRY_LEVEL;    // Assign defaults and get overrides for entry_min and entry_max
        door->entry_max = INVALID_DOOR_ENTRY_LEVEL;
        name = groupDefFindPropertyValue(def_trav->def,"OverrideEntryMin");
        if (name)
            door->entry_min = atoi(name);
        name = groupDefFindPropertyValue(def_trav->def,"OverrideEntryMax");
        if (name)
            door->entry_max = atoi(name);
        name = groupDefFindPropertyValue(def_trav->def,"RadiusOverride");
        if (name)
        {
            door->radius_override = atoi(name);
            if(door->radius_override <= 0 || door->radius_override > 10000)    //10,000 is an arbitrarily large number.  You can make it whatever.
                ErrorFilenamef(saUtilCurrentMapFile(), "Map has a negative radius override, or an override greater than 10000.  Not allowed.  Talk to a programmer if you want to change this.");
        }
        name = groupDefFindPropertyValue(def_trav->def,"MonorailLine");
        if (name)
            door->monorail_line = strdup(name);

        name = groupDefFindPropertyValue(def_trav->def,"VillainOnly");
        if (name)
            door->villainOnly = 1;
        name = groupDefFindPropertyValue(def_trav->def,"HeroOnly");
        if (name)
            door->heroOnly = 1;
        name = groupDefFindPropertyValue(def_trav->def,"NoMissions");
        if (name)
            door->noMissions = 1;
        name = groupDefFindPropertyValue(def_trav->def,"NoFallbackMissions");
        if (name)
            door->noFallbackMissions = 1;
        // Check if we're on a mission map
        if (OnMissionMap())
        {
            // Check for a special "alternate exit" door
            if (type == DOORTYPE_EXITTOSPAWN && door->name != NULL && door->special_info != NULL)
            {
                // Have we already seen one?
                if (totalFound)
                {
                    // If so, throw an error
                    ErrorFilenamef(saUtilCurrentMapFile(), "Map has multiple alternate exit doors, only one permitted");
                }
                totalFound++;
            }
        }
    }
    destroyArray( door_array );
}

void updateLocalBaseDoor( int type, char * name, char * special, int detailID, Mat4 newmat )
{
    DoorEntry * door;
    int i;
    if (!detailID) 
    { //we're not bound to a detail, wrong function
        return;
    }
    // first check the array for an exact match
    for( i = 0; i < local_doors.count; i++ )
    {
        door = &local_doors.entries[i];

        if( door->type == type &&
            (!name || stricmp(door->name, name) == 0) &&
            (!special || stricmp(door->special_info, special) == 0) &&
            door->detail_id == detailID )
        {
            // found one, just update the position
            copyMat4(newmat, door->mat);
        }
    } 

    // door hasn't been added yet, update it.
    door = dynArrayAdd(&local_doors.entries,sizeof(local_doors.entries[0]),&local_doors.count,&local_doors.max,1);

    door->type = type;
    door->detail_id = detailID;
    if( name )
        door->name = strdup(name);
    if( special )
        door->special_info = strdup(special);

    door->entry_min = door->entry_max = INVALID_DOOR_ENTRY_LEVEL;
    copyMat4(newmat, door->mat);

    DoorAnimLoad();

}
void deleteLocalBaseDoor( int type, char * name, char * special, int detailID )
{
    DoorEntry * door;
    int i;
    if (!detailID) 
    { //we're not bound to a detail, wrong function
        return;
    }
    // first check the array for an exact match
    for( i = 0; i < local_doors.count; i++ )
    {
        door = &local_doors.entries[i];

        if( door->type == type &&
            (!name || stricmp(door->name, name) == 0) &&
            (!special || stricmp(door->special_info, special) == 0) &&
            door->detail_id == detailID )
        {
            // found one, remove it from the array
            SAFE_FREE(local_doors.entries[i].name);
            SAFE_FREE(local_doors.entries[i].special_info);
            if (local_doors.count - 1 != i)
            { //copy over the deleted door
                memcpy(&local_doors.entries[i],&local_doors.entries[local_doors.count - 1],sizeof(DoorEntry));
            }
            memset(&local_doors.entries[local_doors.count - 1],0,sizeof(DoorEntry));
            local_doors.count--;
        }
    }

    DoorAnimLoad();
    //Not found, don't do anything
    return;
    
}

void updateAllBaseDoorTypes( RoomDetail *detail, Mat4 door_pos )
{
    if (stricmp("Entrance", detail->info->pchCategory)==0 )
    {
        updateLocalBaseDoor( DOORTYPE_GOTOSPAWN, "BaseReturn", NULL, detail->id, door_pos );
        updateLocalBaseDoor( DOORTYPE_SPAWNLOCATION, "PlayerSpawn", NULL, detail->id, door_pos );
    }
    else if (detail->info->eFunction == kDetailFunction_Teleporter)
    {
        updateLocalBaseDoor(DOORTYPE_BASE_PORTER,"Teleporter",NULL,detail->id,door_pos);
        updateLocalBaseDoor(DOORTYPE_SPAWNLOCATION,"TeleporterSpawn",NULL,detail->id,door_pos);
    }
    else if (detail->info->eFunction == kDetailFunction_RaidTeleporter)
    {
        updateLocalBaseDoor(DOORTYPE_BASE_PORTER,"RaidTeleporter",NULL,detail->id,door_pos);
        updateLocalBaseDoor(DOORTYPE_SPAWNLOCATION,"RaidTeleporterSpawn",NULL,detail->id,door_pos);
    }
    else if (detail->info->eFunction == kDetailFunction_Medivac)
    {
        updateLocalBaseDoor(DOORTYPE_SPAWNLOCATION,"Gurney",NULL,detail->id,door_pos);
    }
}

void deleteAllBaseDoorTypes( RoomDetail * detail, int include_entrance)
{
    if( include_entrance && stricmp("Entrance", detail->info->pchCategory)==0 )
    {
        deleteLocalBaseDoor( DOORTYPE_GOTOSPAWN, "BaseReturn", NULL, detail->id);
        deleteLocalBaseDoor( DOORTYPE_SPAWNLOCATION, "PlayerSpawn", NULL, detail->id);
    }
    else if (detail->info->eFunction == kDetailFunction_Teleporter)
    {
        deleteLocalBaseDoor(DOORTYPE_BASE_PORTER,"Teleporter",NULL,detail->id);
        deleteLocalBaseDoor(DOORTYPE_SPAWNLOCATION,"TeleporterSpawn",NULL,detail->id);
    }
    else if (detail->info->eFunction == kDetailFunction_RaidTeleporter)
    {
        deleteLocalBaseDoor(DOORTYPE_BASE_PORTER,"RaidTeleporter",NULL,detail->id);
        deleteLocalBaseDoor(DOORTYPE_SPAWNLOCATION,"RaidTeleporterSpawn",NULL,detail->id);
    }
    else if (detail->info->eFunction == kDetailFunction_Medivac)
    {
        deleteLocalBaseDoor(DOORTYPE_SPAWNLOCATION,"Gurney",NULL,detail->id);
    }
}

static void freeDoorList(DoorList *doors)
{
    int        i;

    for(i=0;i<doors->count;i++)
    {
        free(doors->entries[i].name);
        free(doors->entries[i].filename);
        free(doors->entries[i].special_info);
        if (doors->entries[i].required)
            free(doors->entries[i].required);
        if (doors->entries[i].lockedText)
            free(doors->entries[i].lockedText);
    }
    if (doors->entries)
        free(doors->entries);
    memset(doors,0,sizeof(*doors));
}

void VerifyRequiredMissionDoors()
{
    StashTable missionDoorList = 0;
    const char** requiredDoors;
    char buf[2000];
    char* types[200];
    int i, d, n = 0;

    // record all mission doors on this map
    missionDoorList = stashTableCreateWithStringKeys(64, StashDeepCopyKeys);
    for (i = 0; i < local_doors.count; i++)
    {
        DoorEntry* door = &local_doors.entries[i];
        if (door->type != DOORTYPE_MISSIONLOCATION) continue;
        if (!door->name) continue;

        memset(types, 0, sizeof(char*)*200);
        strcpy(buf, door->name);
        tokenize_line(buf, types, NULL);
        for (d = 0; types[d]; d++)
        {
            stashAddPointer(missionDoorList, types[d], door, false);
        }
    }

    // verify I have all the doors I'm required to
    requiredDoors = MapSpecDoorList(saUtilCurrentMapFile());
    if (requiredDoors)
        n = eaSize(&requiredDoors);
    for (i = 0; i < n; i++)
    {
        if (!stashFindPointerReturnPointer(missionDoorList, requiredDoors[i]))
        {
            ErrorFilenamef(saUtilCurrentMapFile(), "Map required to have a %s mission door, but does not", requiredDoors[i]);
        }
    }

    stashTableDestroy(missionDoorList);
}

void dbDoorCreateBeaconConnections()
{
    beaconCreateAllClusterConnections(local_doors.entries, local_doors.count);
}

// Pretends to have a list of doors from the server, used for local mapserver
void matchDoorsFromLocal()
{
    int            i, j, k, count, baseMapId;
    DoorList    *doors;
    DoorEntry    *door;

    assert(db_state.local_server);

    doors = &db_doors;
    freeDoorList(doors);
    count = local_doors.count;
    db_state.base_map_id = db_state.map_id;
    baseMapId = staticMapInfoMatchNames(saUtilCurrentMapFile());
    if (baseMapId == -1) baseMapId = 0;

    for(i=0;i<count;i++)
    {
        DoorEntry* localdoor = &local_doors.entries[i];

        door = dynArrayAdd(&doors->entries,sizeof(doors->entries[0]),&doors->count,&doors->max,1);
        door->db_id = baseMapId*10000 + (i+1);
        door->map_id = localdoor->map_id;
        for(j=0;j<4;j++)
            for(k=0;k<3;k++)
                door->mat[j][k] = localdoor->mat[j][k];
        door->type = localdoor->type;
        door->name = strdup(localdoor->name);
        if (door->type == DOORTYPE_PERSISTENT_NPC)
        {
            char* extension;
            fileFixName(door->name, door->name);
            extension = door->name + (strlen(door->name) - 4);
            if(stricmp(extension, ".npc") == 0)
                *extension = '\0';
        }
        door->special_info = strdup(localdoor->special_info);
    }
    // Clear the cached locations of the contacts
    ContactDestroyLocations();
}

void dbGatherMapLinkInfo()
{
    int i;
    int j;
    freeDoorList(&local_doors);
    gatherDoors(&local_doors,"SpawnLocation",0,DOORTYPE_SPAWNLOCATION);
    gatherDoors(&local_doors,"LairType","LocationName",DOORTYPE_MISSIONLOCATION);
    gatherDoors(&local_doors,"GotoSpawn","GotoMap",DOORTYPE_GOTOSPAWN);
    gatherDoors(&local_doors,"GotoSpawnAbsPos","GotoMap",DOORTYPE_GOTOSPAWNABSPOS);
    gatherDoors(&local_doors,"ExitToSpawn","ExitToMap",DOORTYPE_EXITTOSPAWN);
    gatherDoors(&local_doors,"OpenDoor",0,DOORTYPE_FREEOPEN);
    gatherDoors(&local_doors,"LockedDoor",0,DOORTYPE_LOCKED);
    gatherDoors(&local_doors,"KeyDoor", "GotoLocation", DOORTYPE_KEY);
        // The second parameter is basically GotoSpawn, but we can't use that word without confusing the system
    gatherDoors(&local_doors,"SGBase",0,DOORTYPE_SGBASE);

    gatherDoors(&local_doors,"FlashbackDoor",0,DOORTYPE_FLASHBACK_CONTACT);

    //JS:    Something similar is already being done in pnpc.c and locationTask.c.
    //        They also include error checking.  Maybe this info should be grabbed from
    //        those modules instead.
    gatherDoors(&local_doors,"PersistentNPC",0,DOORTYPE_PERSISTENT_NPC);
    //gatherDoors(&local_doors,"LocationGroup",0,DOORTYPE_TASK_VISIT_LOCATION);
    ContactGatherScriptContacts();
    
    dbDoorCreateBeaconConnections();
    
    VerifyRequiredMissionDoors();
    // If this is a local mapserver, we just an update, modify dbdoor list
    if (db_state.local_server)
        matchDoorsFromLocal();
    if (g_base.rooms) //reload base doors
        baseForceUpdate(&g_base);
    if (i = eaSize(&g_raidbases))
    {
        for (j = 0; j < i; j++)
        {
            baseForceUpdate(g_raidbases[j]);
        }
    }
}

// Update the doors array to mirror the data contained in the packet.
void handleServerDoorUpdate(Packet *pak)
{
    int            i,j,count;
    DoorList    *doors;
    DoorEntry    *door;
    char        *s,*args[10],*doors_str,*doors_args[10],*curr_doors_str;
    char        buf[1024];

    doors = &db_doors;
    freeDoorList(doors);
    count = pktGetBitsPack(pak,1);
    curr_doors_str = doors_str = pktGetZipped(pak,0);

    for(i=0;i<count;i++)
    {
        Vec3    pyr;

        door = dynArrayAdd(&doors->entries,sizeof(doors->entries[0]),&doors->count,&doors->max,1);
        tokenize_line(curr_doors_str,doors_args,&curr_doors_str);
#if 0
        door->db_id = pktGetBitsPack(pak,1);
        s = strcpy(buf, unescapeString(pktGetString(pak)));
#else
        door->db_id = atoi(doors_args[0]);
        s = strcpy(buf, unescapeString(doors_args[1]));
        s = strcpy(buf, unescapeString(buf));    // The string was escaped twice, to unescape it twice
#endif
        tokenize_line(s,args,&s);
        door->map_id = atoi(args[1]);
        for(j=0;j<3;j++)
        {
            tokenize_line(s,args,&s);
            door->mat[3][j] = atof(args[1]);
        }
        for(j=0;j<3;j++)
        {
            tokenize_line(s,args,&s);
            pyr[j] = atof(args[1]);
        }
        createMat3YPR(door->mat,pyr);
        tokenize_line(s,args,&s);
        if (stricmp(args[1],"MissionLocation")==0)
            door->type = DOORTYPE_MISSIONLOCATION;
        else if (stricmp(args[1],"SpawnLocation")==0)
            door->type = DOORTYPE_SPAWNLOCATION;
        else if (stricmp(args[1],"GotoSpawn")==0)
            door->type = DOORTYPE_GOTOSPAWN;
        else if (stricmp(args[1],"GotoSpawnAbsPos")==0)
            door->type = DOORTYPE_GOTOSPAWNABSPOS;
        else if (stricmp(args[1],"ExitToSpawn")==0)
            door->type = DOORTYPE_EXITTOSPAWN;
        else if (stricmp(args[1], "PersistentNPC")==0)
            door->type = DOORTYPE_PERSISTENT_NPC;
        else if (stricmp(args[1], "TaskVisitLocation")==0)
            door->type = DOORTYPE_TASK_VISIT_LOCATION;
        else if (stricmp(args[1], "OpenDoor")==0)
            door->type = DOORTYPE_FREEOPEN;
        else if (stricmp(args[1], "LockedDoor")==0)
            door->type = DOORTYPE_LOCKED;
        else if (stricmp(args[1], "KeyDoor")==0)
            door->type = DOORTYPE_KEY;
        else if (stricmp(args[1], "SGBase")==0)
            door->type = DOORTYPE_SGBASE;
        else if (stricmp(args[1], "FlashbackDoor")==0)
            door->type = DOORTYPE_FLASHBACK_CONTACT;
        else
            FatalErrorf("unknown door type: %s.%s",args[0],args[1]);

        if (tokenize_line(s,args,&s) == 2 && stricmp(args[0],"Name")==0)
        {
            door->name = strdup(args[1]);
            if(door->type == DOORTYPE_PERSISTENT_NPC)
            {
                char* extension;
                fileFixName(door->name, door->name);
                extension = door->name + (strlen(door->name) - 4);
                if(stricmp(extension, ".npc") == 0)
                    *extension = '\0';
            }
        }

        if (tokenize_line(s,args,&s) == 2 && stricmp(args[0],"SpecialInfo")==0) 
        {
            door->special_info = strdup(args[1]);
            tokenize_line(s,args,&s);
        }

        if (args[0] && stricmp(args[0],"NoMissions")==0)
        {
            door->noMissions = atoi(args[1]);
        }
        if (args[0] && stricmp(args[0],"NoFallbackMissions")==0)
        {
            door->noFallbackMissions = atoi(args[1]);
        }


        if (!door->name || !door->name[0])
        {
            Errorf("Door with null name in map!\n");
            continue;
        }
    }
    free(doors_str);

    // If this is a local mapserver, we just got garbage, replace it with the local doorlist
    if (db_state.local_server)
        matchDoorsFromLocal();

    // Cleanup the contact locations cache
    ContactDestroyLocations();
}

// Send out a packet containing all information it the doors array.
void sendDoorsToDbServer(int is_static)
{
    int            i,idx;
    Packet        *pak;
    DoorList    *doors;
    char        buf[10000];
    DoorEntry    *door;

    doors = &local_doors;
    pak = pktCreateEx(&db_comm_link,DBCLIENT_SEND_DOORS);
    pktSendBitsPack(pak,1,db_state.map_id);
    if (!is_static)
    {
        pktSendBitsPack(pak,1,0 );
        pktSend(&pak,&db_comm_link);
        return;
    }

    pktSendBitsPack(pak,1,doors->count);
    for(i=0;i<doors->count;i++)
    {
        StaticMapInfo* info;
        Vec3    pyr;
        
        door = &doors->entries[i];
        info = staticMapInfoFind(door->map_id);
        getMat3YPR(door->mat,pyr);
        idx = sprintf(buf,"MapId %d\n",info && info->baseMapID ? info->baseMapID : door->map_id);
        idx += sprintf(buf+idx,"PosX %f\nPosY %f\nPosZ %f\n",door->mat[3][0],door->mat[3][1],door->mat[3][2]);
        idx += sprintf(buf+idx,"RotX %f\nRotY %f\nRotZ %f\n",pyr[0],pyr[1],pyr[2]);
        if (door->type == DOORTYPE_MISSIONLOCATION)
            idx += sprintf(buf+idx,"DoorType MissionLocation\n");
        else if (door->type == DOORTYPE_SPAWNLOCATION)
            idx += sprintf(buf+idx,"DoorType SpawnLocation\n");
        else if (door->type == DOORTYPE_GOTOSPAWN)
            idx += sprintf(buf+idx,"DoorType GotoSpawn\n");
        else if (door->type == DOORTYPE_GOTOSPAWNABSPOS)
            idx += sprintf(buf+idx,"DoorType GotoSpawnAbsPos\n");
        else if (door->type == DOORTYPE_EXITTOSPAWN)
            idx += sprintf(buf+idx,"DoorType ExitToSpawn\n");
        else if (door->type == DOORTYPE_PERSISTENT_NPC)
            idx += sprintf(buf+idx,"DoorType PersistentNPC\n");
        else if (door->type == DOORTYPE_TASK_VISIT_LOCATION)
            idx += sprintf(buf+idx,"DoorType TaskVisitLocation\n");
        else if (door->type == DOORTYPE_FREEOPEN)
            idx += sprintf(buf+idx,"DoorType OpenDoor\n");
        else if (door->type == DOORTYPE_LOCKED)
            idx += sprintf(buf+idx,"DoorType LockedDoor\n");
        else if (door->type == DOORTYPE_KEY)
            idx += sprintf(buf+idx,"DoorType KeyDoor\n");
        else if (door->type == DOORTYPE_SGBASE)
            idx += sprintf(buf+idx,"DoorType SGBase\n");
        else if (door->type == DOORTYPE_FLASHBACK_CONTACT)
            idx += sprintf(buf+idx,"DoorType FlashbackDoor\n");
        if (door->name)
            idx += sprintf(buf+idx,"Name \"%s\"\n",escapeString(door->name));
        if (door->special_info)
            idx += sprintf(buf+idx,"SpecialInfo \"%s\"\n",escapeString(door->special_info));
        if (door->noMissions)
            idx += sprintf(buf+idx,"NoMissions %d\n", door->noMissions);
        if (door->noFallbackMissions)
            idx += sprintf(buf+idx,"NoFallbackMissions %d\n", door->noFallbackMissions);
        pktSendString(pak,buf);
    }
    pktSend(&pak,&db_comm_link);
}

// Creates a new door for a contact that was created on the fly
void dbRegisterNewContact(char* name, Mat4 location)
{
    Vec3 pyr;
    DoorEntry* door = dynArrayAdd(&local_doors.entries,sizeof(local_doors.entries[0]),&local_doors.count,&local_doors.max,1);
    door->map_id = db_state.map_id;
    door->type = DOORTYPE_PERSISTENT_NPC;
    door->name = strdup(name);
    getMat3YPR(location,pyr);
    copyMat4(location, door->mat);
}

DoorEntry *dbFindSpawnLocation(char *target_name)
{
    int            i,count=0;
    DoorEntry    *door;
    DoorList    *doors = &local_doors;
    static        DoorEntry **choices;
    static        int max_choices;

    for(i=0;i<doors->count;i++)
    {
        door = &doors->entries[i];
        if (door->type != DOORTYPE_SPAWNLOCATION)
            continue;
        if (stricmp(door->name,target_name)==0)
            dynArrayAddp(&choices,&count,&max_choices,door);
    }
    if (!count)
        return 0;
    return choices[randInt(count)];
}

DoorEntry *dbFindSpawnLocationForBaseRaid(char *target_name, Entity *e)
{
    int            i;
    int            found = 0;
    DoorEntry    *door;
    DoorList    *doors = &local_doors;
    static        DoorEntry **choices = NULL;
    static        int max_choices = 0;
    int            count = 0;

    for(i = 0; i < doors->count; i++)
    {
        door = &doors->entries[i];
        if (door->type != DOORTYPE_SPAWNLOCATION)
            continue;
        if (stricmp(door->name, target_name) == 0)
            dynArrayAddp(&choices,&count,&max_choices,door);
    }
    devassertmsg(count, "Looking for doors in a base raid, but there aren't any");
    devassertmsg(g_isBaseRaid && eaSize(&g_raidbases),"Trying to place player for a base raid when there isn't one going on");

    devassertmsg(e->pl, "Trying to place a non-player in a base raid");
    for (i = eaSize(&g_raidbases) - 1; i >= 0; i--)
    {
        // TODO - fix this when we go raid vs raid (as opposed to the current sg vs sg)
        // When that happens, g_raidbases[i]->supergroup_id willl actually hold the raidID
        // Which in turn will probably cause the devassertmsg at the bottom to trigger
        if (e->supergroup_id == g_raidbases[i]->supergroup_id)
        {
            found = 1;
            break;
        }
    }
    // If this triggers, it's because you forgot to adjust the if() statement above to check the entity's
    // raidID instead of their sgid.  Once that change is made, this comment should be removed.
    devassertmsg(found, "Intruder detected in base raid");
    return choices[i];
}

// Gets all doors found on this map
DoorEntry *dbGetLocalDoors(int *count)
{
    *count = local_doors.count;
    return local_doors.entries;
}

// Gets all doors tracked by the dbserver
// Note that mission maps never have any doors in this list
DoorEntry *dbGetTrackedDoors(int *count)
{
    *count = db_doors.count;
    return db_doors.entries;
}

// looks through local doors and global doors and returns the
// proper name of the door.  Only useful for mission entrances.
// returns NULL on error
char* dbGetDoorName(int door_id)
{
    int j;
    DoorList* doors = &db_doors;

    for (j = 0; j < doors->count; j++)
    {
        if (doors->entries[j].db_id == door_id)
            return doors->entries[j].name;
    }
    return NULL;
}

char* g_doortypenames[] = 
{
    "Mission door",            // DOORTYPE_MISSIONLOCATION
    "SpawnLocation",        // DOORTYPE_SPAWNLOCATION
    "GotoSpawn door",        // DOORTYPE_GOTOSPAWN
    "PersistentNPC",        // DOORTYPE_PERSISTENT_NPC
    "TaskVisitLocation",    // DOORTYPE_TASK_VISIT_LOCATION
    "Open door",            // DOORTYPE_FREEOPEN
    "Locked door",            // DOORTYPE_LOCKED
    "Keyed door",            // DOORTYPE_KEY
    "SG Base door",            // DOORTYPE_SGBASE
    "SG Raid Teleporter",    // DOORTYPE_SGRAID
    "Base Teleporter",        // DOORTYPE_BASE_PORTER
    "Ouroboros Door",        // DOORTYPE_FLASHBACK_CONTACT
    "ExitToSpawn door",        // DOORTYPE_EXITTOSPAWN
    "GotoSpawnAbsPos door",    // DOORTYPE_GOTOSPAWNABSPOS
};

// prints a list of all tracked doors to console
void dbDebugPrintTrackedDoors(ClientLink* client, int missiononly)
{
    int i, found = 0;;

    conPrintf(client, "You are on map %i", db_state.map_id);
    for (i = 0; i < db_doors.count; i++)
    {
        DoorEntry* door = &db_doors.entries[i];
        if (door->map_id != db_state.map_id) continue;
        if (missiononly && door->type != DOORTYPE_MISSIONLOCATION) continue;
        found++;
        if (missiononly)
            conPrintf(client, "(%i) %s at (%1.0f,%1.0f,%1.0f): %s, %s", 
            door->db_id, g_doortypenames[door->type],
            door->mat[3][0], door->mat[3][1], door->mat[3][2], door->name, door->special_info);
        else
            conPrintf(client, "(%i) %s at (%1.0f,%1.0f,%1.0f)", 
            door->db_id, g_doortypenames[door->type],
            door->mat[3][0], door->mat[3][1], door->mat[3][2]);
    }
    if (!found)
        conPrintf(client, "(no doors)");
}

static ClientLink* dpmdt_client;
static int DebugPrintDoorType(StashElement el)
{
    conPrintf(dpmdt_client, "  %s\n", stashElementGetStringKey(el));
    return 1;
}

void dbDebugPrintMissionDoorTypes(ClientLink* client)
{
    StashTable missionDoorList = 0;
    char buf[2000];
    char* types[200];
    int i, d, n = 0;

    // record all mission doors on this map
    missionDoorList = stashTableCreateWithStringKeys(64, StashDeepCopyKeys);
    for (i = 0; i < local_doors.count; i++)
    {
        DoorEntry* door = &local_doors.entries[i];
        if (door->type != DOORTYPE_MISSIONLOCATION) continue;
        if (!door->name) continue;

        memset(types, 0, sizeof(char*)*200);
        strcpy(buf, door->name);
        tokenize_line(buf, types, NULL);
        for (d = 0; types[d]; d++)
        {
            stashAddPointer(missionDoorList, types[d], door, false);
        }
    }

    // print them
    dpmdt_client = client;
    conPrintf(client, "Mission doors on this map:\n");
    stashForEachElement(missionDoorList, DebugPrintDoorType);
}

DoorEntry *dbFindDoorWithName(DoorType type, const char* name)
{
    int i;

    for(i = 0; i < db_doors.count; i++)
    {
        DoorEntry* door = &db_doors.entries[i];

        if(door->type != type)
            continue;

        if(door->name && stricmp(door->name, name) == 0)
            return door;

        if (type == DOORTYPE_PERSISTENT_NPC)
        {
            // Sometimes we only have the name of the NPC, but the door name includes the containing
            // folders.
            char *lastSlash = strrchr(door->name, '/');

            if (!lastSlash)
                lastSlash = strrchr(door->name, '\\');

            if (lastSlash && stricmp(lastSlash + 1, name) == 0)
                return door;
        }
    }

    return NULL;
}

DoorEntry *dbFindDoorWithSpecialInfo(DoorType type, const char* special_info)
{
    int i;

    DoorEntry* findTarget = NULL;

    for(i = 0; i < db_doors.count; i++)
    {
        DoorEntry* door = &db_doors.entries[i];
        if(door->type != type)
            continue;
        if(door->special_info && stricmp(door->special_info, special_info) == 0)
        {
            findTarget = door;
            break;
        }
    }

    return findTarget;
}

DoorEntry *dbFindDoorWithDoorId(int db_id)
{
    int i;

    DoorEntry* findTarget = NULL;

    for(i = 0; i < db_doors.count; i++)
    {
        DoorEntry* door = &db_doors.entries[i];

        if(door->db_id == db_id)
        {
            findTarget = door;
            break;
        }
    }

    return findTarget;
}

// See if we can find a door in this mission that specifies an alternate mission exit
DoorEntry *dbFindAlternateMissionExit()
{
    int i;

    for(i = 0; i < local_doors.count; i++)
    {
        DoorEntry* door = &local_doors.entries[i];
        if (door->type == DOORTYPE_EXITTOSPAWN && door->name != NULL && door->special_info != NULL)
        {
            return door;
        }
    }
    return NULL;
}

// choose a random local door of this type
static DoorEntry* debugGetRandomDoor(DoorType type)
{
    DoorEntry** doors;
    DoorEntry* result = NULL;
    int i, sel;

    eaCreate(&doors);
    for (i = 0; i < local_doors.count; i++)
    {
        if (local_doors.entries[i].type == type)
            eaPush(&doors, &local_doors.entries[i]);
    }
    if (eaSize(&doors))
    {
        sel = rand() % eaSize(&doors);
        result = doors[sel];
    }

    eaDestroy(&doors);
    return result;
}

// trying to find a good location for walking out of a random door
DoorEntry* debugGetRandomExitDoor()
{
    DoorEntry* door;
    door = debugGetRandomDoor(DOORTYPE_MISSIONLOCATION);
    if (door) return door;
    return debugGetRandomDoor(DOORTYPE_SPAWNLOCATION);
}



