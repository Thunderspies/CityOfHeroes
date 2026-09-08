/*\
 *
 *    missiongeo.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles loading geometry objects related to missions and
 *  tracking info about them.  Includes information about
 *    logical mission rooms.
 *
 */

#include "group/grouputil.h"

#include "entity/entity.h"
#include "group/group.h"
#include "group/groupProperties.h"

#include "comm_game.h"
#include "storyarc/missionMapCommon.h"
#include "storyarc/missionMapCommon.h"
#include "storyarc/missiongeoCommon.h"

#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>

#if SERVER
//#include "groupnetdb.h" // for attemptToCheckOut
#include "storyarc/missionspec.h"
//#include "svr_base.h"
#include "group/groupnetdb.h"
//#include "groupnetsend.h"
//#include "dbcomm.h"
//#include "mission.h"
//#include "storyarcprivate.h"
#endif

// *********************************************************************************
//  Mission markers
// *********************************************************************************
#ifndef MAX_EXTENTS
#define MAX_EXTENTS 20
#endif
int extentIds[MAX_EXTENTS];

// location types
StaticDefineInt ParseLocationTypeEnum[] =
{
    DEFINE_INT
    { "Person",            LOCATION_PERSON },
    { "ItemAgainstWall", LOCATION_ITEMWALL },
    { "ItemWall",        LOCATION_ITEMWALL },
    { "ItemOnFloor",    LOCATION_ITEMFLOOR },
    { "ItemFloor",        LOCATION_ITEMFLOOR },
    { "ItemOnRoof",        LOCATION_ITEMROOF },
    { "ItemRoof",        LOCATION_ITEMROOF },

    // terms that were in scripts previously
    { "Item",            LOCATION_LEGACY },
    { "HiddenItem",        LOCATION_LEGACY },
    { "WallItem",        LOCATION_LEGACY },
    { "HiddenWallItem",    LOCATION_LEGACY },

    // terms that were used on maps
    { "Hidden",            LOCATION_LEGACY },
    { "ItemHidden",        LOCATION_LEGACY },
    { "Obvious",        LOCATION_LEGACY },
    { "ItemObvious",    LOCATION_LEGACY },
    DEFINE_END
};

RoomInfo** g_rooms = 0;
int g_lowpace = 1;
int g_highpace = 1;
int g_encountersoutsidetray = 0; // count of how many encounters weren't even in a tray peice
int g_logmapload = 0;
int g_sectioncount = 0;

char * g_mapname = 0;

void RoomAdd(DefTracker* tracker, int pace, Vec3 markerPos);
static void LogMapLoad(GroupDefTraverser* traverser, DefTracker* tracker, char* description, ...);

// create the rooms array and add a NULL entry to it if needed
void RoomInit()
{
    Vec3 zero = {0};
    if (g_rooms) return;
    eaCreate(&g_rooms);
    RoomAdd(NULL, 1, zero);
}

void RoomDestroy()
{
    int i, n;
    n = eaSize(&g_rooms);
    for (i = 0; i < n; i++)
    {
        eaClearEx(&g_rooms[i]->items, NULL);
        eaiDestroy(&g_rooms[i]->objectiveMarkers);
        free(g_rooms[i]);
    }
    eaDestroy(&g_rooms);
    g_rooms = 0;
}

// add info for one room
void RoomAdd(DefTracker* tracker, int pace, Vec3 markerPos)
{
    RoomInfo* info;
    RoomInit();
    info = calloc(sizeof(RoomInfo),1);
    info->tracker = tracker;
    info->roompace = pace;
    if (pace > g_highpace) g_highpace = pace;
    if (pace < g_lowpace) g_lowpace = pace;
    copyVec3(markerPos, info->markerPos);
    eaPush(&g_rooms, info);
}


// just a convenience for debugging purposes: sorting rooms by pacing number
static int compareRooms(const void* lhsData, const void* rhsData)
{
    RoomInfo** lhs = (RoomInfo**)lhsData;
    RoomInfo** rhs = (RoomInfo**)rhsData;
    if ((*lhs)->roompace < (*rhs)->roompace) return -1;
    if ((*lhs)->roompace > (*rhs)->roompace) return 1;
    return 0;
}
static void SortRooms(void)
{
    if (eaSize(&g_rooms))
        qsort(&g_rooms[1], eaSize(&g_rooms)-1, sizeof(g_rooms[0]), compareRooms);
}

DefTracker* g_lasttracker; // used when debugging RoomGetInfo

// guaranteed a result, check tracker on result to see if you really just
// got the NULL room
RoomInfo* RoomInfoFromTracker(DefTracker* tracker)
{
    int i, n;
    n = eaSize(&g_rooms);
    for (i = 1; i < n; i++)
    {
        if (g_rooms[i]->tracker == tracker)
            return g_rooms[i];
    }
    return g_rooms[0];
}

// look up room based on position.  guaranteed a result, check tracker on
// result to see if you really just got the NULL room
RoomInfo* RoomGetInfo(const Vec3 pos)
{
    DefTracker* tracker;
    Vec3 point;
    RoomInfo * room;

    RoomInit();

    g_lasttracker = tracker = groupFindInside(pos, FINDINSIDE_TRAYONLY,0,0);
    room = RoomInfoFromTracker(tracker);

    //Look a little higher, maybe we're stuck in the ground a little
    if( room == g_rooms[0] ) //Check the actual position
    {
        copyVec3(pos, point);
        point[1] += 2.0;
        g_lasttracker = tracker = groupFindInside(point, FINDINSIDE_TRAYONLY,0,0);
        room = RoomInfoFromTracker(tracker);

        //Look a little lower, maybe we're stuck in the ceiling a little
        if( room == g_rooms[0] ) //Check the actual position
        {
            copyVec3(pos, point);
            point[1] -= 2.0;
            g_lasttracker = tracker = groupFindInside(point, FINDINSIDE_TRAYONLY,0,0);
            room = RoomInfoFromTracker(tracker);

            if( room == g_rooms[0] ) 
                g_rooms[0]->hadtracker = tracker != 0;  //Don't know why this exists, just keeping the logic the same
        }
    }

    return room;    // return the NULL room
}

RoomInfo* RoomFindClosest(const Vec3 findPos)
{
    int i, n;
    F32 closestDist = 0;
    int whichRoom = 0;
    RoomInit();
    n = eaSize(&g_rooms);
    for (i = 1; i < n; i++)
    {
        if(g_rooms[i]->tracker)
        {
            F32 currDist = distance3SquaredXZ(findPos, g_rooms[i]->tracker->mat[3]);
            if (!whichRoom || currDist < closestDist)
            {
                whichRoom = i;
                closestDist = currDist;
            }
        }
    }
    return g_rooms[whichRoom];
}

// guaranteed a result, check tracker on result to see if you really just
// got the NULL room
RoomInfo* RoomInfoFromPace(int pace)
{
    int i, n;
    n = eaSize(&g_rooms);
    for (i = 1; i < n; i++)
    {
        if (g_rooms[i]->roompace == pace)
            return g_rooms[i];
    }
    return g_rooms[0];
}

// guaranteed a result, check tracker on result to see if you really just
// got the NULL room
RoomInfo* StartRoomInfo()
{
    int i, n;
    n = eaSize(&g_rooms);
    for (i = 1; i < n; i++)
    {
        if ( g_rooms[i]->startroom )
            return g_rooms[i];
    }
    return g_rooms[0];
}

// guaranteed a result, check tracker on result to see if you really just
// got the NULL room
RoomInfo* EndRoomInfo()
{
    int i, n;
    n = eaSize(&g_rooms);
    for (i = 1; i < n; i++)
    {
        if ( g_rooms[i]->endroom )
            return g_rooms[i];
    }
    return g_rooms[0];
}



RoomInfo* RoomGetInfoEncounter(EncounterGroup* group)
{
    RoomInfo* room = 0;
#if SERVER    
    EncounterPoint* child;
    room = RoomGetInfo(group->mat[3]);
    if (room == g_rooms[0] && group->children.size) // if group center isn't in tray, try first child
    {
        child = group->children.storage[0];
        room = RoomGetInfo(child->mat[3]);
    }
    LogMapLoad(NULL, g_lasttracker, "Encounter %s in room %i.%i", group->groupname, eaFind(&g_rooms, room), room->roompace);
#endif
    return room;
}

const ItemPosition* GetClosestItemPosition(RoomInfo* room, const Mat4 mat);

void ObjectiveAddMarker(RoomInfo* room, char* obj_num)
{
    int num = MAX_OBJECTIVE_MARKER;
    int i, n;

    // verify the objective number
    if (obj_num) num = atoi(obj_num);
    if (num < 1 || num > MAX_OBJECTIVE_MARKER)
    {
        ErrorFilenamef( g_mapname, "Objective marker %s outside the range of 1-%i", 
            obj_num, MAX_OBJECTIVE_MARKER);
        num = MAX_OBJECTIVE_MARKER;
    }
    
    n = eaiSize(&room->objectiveMarkers);
    for (i = 0; i < n; i++)
    {
        if (room->objectiveMarkers[i] == num)
            return;
    }
    if (!room->objectiveMarkers) 
        eaiCreate(&room->objectiveMarkers);
    eaiPush(&room->objectiveMarkers, num);
}

// info passed to ItemAdd, the current room and item group
// that the item positions refer to
RoomInfo* g_curroom = NULL;
LocationTypeEnum g_grouptype = 0;
char * g_customName = 0;

void ItemAdd(RoomInfo* room, Mat4 pos, LocationTypeEnum loctype, char * customName)
{
    ItemPosition* item = NULL;

    if (!room || !loctype)
        return; // got an ItemPosition without a correct group

    if (!room->items)
        eaCreateConst(&room->items);

    item = calloc(sizeof(ItemPosition), 1);
    copyMat4(pos, item->pos);
    item->loctype = loctype;
    if( customName )
    {
        item->customName = strdup( customName );
    }

    eaPushConst(&room->items, item);
}

static int missionSectionsLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* pairProp;

    // find the group
    if (traverser->def->properties)
    {

        stashFindPointer( traverser->def->properties, "extentsMarkerGroup", &pairProp );
        // if we found marker
        if( pairProp )    //we found another section.
        {
            int i;
            int id = atoi(pairProp->value_str);
            int found = false;
            for(i = 0; i < g_sectioncount && !found; i++)
            {
                if(extentIds[i] == id)
                    found = 1;
            }
            if(!found)
            {
                extentIds[g_sectioncount] = id;
                g_sectioncount++;
            }

            return 0;
        }
    }

    if (!traverser->def->child_properties)  // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

int RoomLoader(GroupDefTraverser* traverser)
{
    Vec3 roomPos;
    PropertyEnt* roomProp;
    DefTracker* tracker;
    int pace;

    // find the mission room marker
    if (traverser->def->properties)
    {
        stashFindPointer( traverser->def->properties, "MissionRoom", &roomProp );
        if (roomProp)
        {
            copyVec3(traverser->mat[3], roomPos);
            roomPos[1] += 2.0;
            tracker = groupFindInside(roomPos, FINDINSIDE_TRAYONLY,0,0);
            pace = 1;
            if (roomProp->value_str) pace = atoi(roomProp->value_str);
            if (!tracker)
            {
                ErrorFilenamef(g_mapname, "mission.c: Room %s marker isn't in a room", roomProp->value_str);
            }
            RoomAdd(tracker, atoi(roomProp->value_str), traverser->mat[3]);
            LogMapLoad(traverser, tracker, "Room marker %s", roomProp->value_str);
        }
    }

    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

int ItemPosLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* itemProp;

    // find an ItemPosition marker
    if (traverser->def->properties)
    {
        // Is this thing an item at all?
        stashFindPointer( traverser->def->properties, "ItemPosition", &itemProp );
        if (!itemProp)
            return 1; // traverse children

        ItemAdd(g_curroom, traverser->mat, g_grouptype, g_customName );
    }

    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

int MarkerLoader(GroupDefTraverser* traverser)
{
    RoomInfo* info;
    PropertyEnt* roomProp;

    // find the group
    if (!traverser->def->properties) {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }

    // find any mission marker
    stashFindPointer( traverser->def->properties, "MissionStart", &roomProp );
    if (roomProp)
    {
        info = RoomGetInfo(traverser->mat[3]);
        if (info == g_rooms[0])
        {
            ErrorFilenamef(g_mapname, "mission.c: MissionStart marker isn't in a room");
        }
        info->startroom = 1;
        LogMapLoad(traverser, g_lasttracker, 
            "Mission start marker in room %i.%i", eaFind(&g_rooms, info), info->roompace);
    }

    stashFindPointer( traverser->def->properties, "MissionEnd", &roomProp );
    if (roomProp)
    {
        info = RoomGetInfo(traverser->mat[3]);
        if (info == g_rooms[0])
        {
            ErrorFilenamef(g_mapname, "mission.c: MissionEnd marker isn't in a room");
        }
        info->endroom = 1;
        LogMapLoad(traverser, g_lasttracker, 
            "Mission end marker in room %i.%i", eaFind(&g_rooms, info), info->roompace);
    }

    stashFindPointer( traverser->def->properties, "SpawnLocation", &roomProp );
    if (roomProp)
    {
        if (roomProp->value_str && stricmp(roomProp->value_str, "_Lobby")== 0)
        {
            info = RoomGetInfo(traverser->mat[3]);
            info->lobbyroom = 1;
            LogMapLoad(traverser, g_lasttracker, 
                "Mission spawn marker in room %i.%i", eaFind(&g_rooms, info), info->roompace);
        }
    }

    stashFindPointer( traverser->def->properties, "MissionObjective", &roomProp );
    if (roomProp)
    {
        info = RoomGetInfo(traverser->mat[3]);
        if (info == g_rooms[0])
        {
            ErrorFilenamef(g_mapname, "mission.c: Objective %s marker isn't in a room", roomProp->value_str);
        }
        ObjectiveAddMarker(info, roomProp->value_str);
        LogMapLoad(traverser, g_lasttracker, 
            "Objective marker %s in room %i.%i", roomProp->value_str, eaFind(&g_rooms, info), info->roompace);
    }

    // look for ItemGroup's property prefixed with ItemGroup
    stashFindPointer( traverser->def->properties, "ItemGroup", &roomProp );
    if (roomProp)
    {
        g_curroom = RoomGetInfo(traverser->mat[3]);
        g_grouptype = StaticDefineIntGetInt(ParseLocationTypeEnum, roomProp->value_str);
        g_customName = 0;
        if (g_grouptype == -1)
        {
            ErrorFilenamef(g_mapname, "mission.c:MarkerLoader - incorrect Mission Item group type %s", roomProp->value_str);
        }
        else if (g_grouptype == LOCATION_LEGACY)
        {
            //printf("Legacy location group %s, ignoring\n", traverser->def->name);
        }
        else
        {
            groupProcessDefExContinue(traverser, ItemPosLoader);
        }
        LogMapLoad(traverser, g_lasttracker, 
            "Objective group %s in room %i.%i", roomProp->value_str, 
            eaFind(&g_rooms, g_curroom), g_curroom->roompace);
        g_curroom = NULL;
        g_grouptype = 0;
    }

    // look for ItemGroup's with just the Location type as a property name ("ItemAgainstWall")
    // (this sucks)
    {
        int curtype = DM_NOTYPE;
        StaticDefine* cur = (StaticDefine*)ParseLocationTypeEnum;

        // from StaticDefineLookup
        while (1)
        {
            // look for key markers first
            int keymarker = (int)(intptr_t)cur->key;
            if (keymarker == DM_END)
                break; // failed lookup
            else if (keymarker == DM_INT)
                curtype = DM_INT;
            else if (keymarker == DM_STRING)
                curtype = DM_STRING;
            else
            {
                // looking through all the possible item types in ParseLocationTypeEnum for markers
                stashFindPointer( traverser->def->properties, cur->key, &roomProp );
                if (roomProp)
                {
                    g_curroom = RoomGetInfo(traverser->mat[3]);
                    g_grouptype = (int)(intptr_t)cur->value;
                    g_customName = roomProp->value_str;
                    if( g_customName && g_customName[0] == '0' ) //string NULL is not customname
                        g_customName = 0;

                    if ((int)(intptr_t)cur->value == LOCATION_LEGACY)
                    {
                        //printf("Ignoring legacy item set under %s", traverser->def->name);
                    }
                    else
                    {
                        groupProcessDefExContinue(traverser, ItemPosLoader);
                    }
                    LogMapLoad(traverser, g_lasttracker, 
                        "Objective group %s (Custom Name %s) in room %i.%i", roomProp->name_str, roomProp->value_str, 
                        eaFind(&g_rooms, g_curroom), g_curroom->roompace);
                    g_curroom = NULL;
                    g_grouptype = 0;
                }
            }
            // keep looking for keys
            cur++;
        }
    }

    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

// verify that we at least have a start room and an end room
void VerifyRoomMarkers()
{
    int i, n;
    int havestart = 0;
    int haveend = 0;
    n = eaSize(&g_rooms);
    for (i = 0; i < n; i++)
    {
        if (g_rooms[i]->startroom) havestart = 1;
        if (g_rooms[i]->endroom) haveend = 1;
    }
    
    if (!havestart)
        ErrorFilenamef(g_mapname, "Mission has no start room\n");
    if (!haveend)
        ErrorFilenamef(g_mapname, "Mission has no end room\n");
}

void ClearRoomMarkers()
{
    g_encountersoutsidetray = 0;
    RoomDestroy();
}

void MissionLoad(int forceload, int map_id, char * mapname)
{
    GroupDefTraverser traverser = {0};

#if SERVER
    if (!forceload && !OnMissionMap()) 
        return;
#endif

    ClearRoomMarkers();
    groupProcessDefExBegin(&traverser, &group_info, RoomLoader);    // first rooms
    SortRooms();
    groupProcessDefExBegin(&traverser, &group_info, MarkerLoader);    // then, other markers
    g_sectioncount = 0;
    groupProcessDefExBegin(&traverser, &group_info, missionSectionsLoader);    //get the number of sections
    //if there are no sections, this is probably an outdoor map.  In that case, we still produce an image
    if(!g_sectioncount)
        g_sectioncount = 1;

    if( map_id )
        log_printf("missiondoor.log", "Map %i MissionLoad called\n", map_id);

    //we don't always have a start or end room since we apply this to non-mission maps
    //only do this check if we have an indication that we're on a mission map.  OnMissionMap is not
    //reliable when we're working from a local mapserver, so we actually have to go to the map's name.
    SAFE_FREE(g_mapname);
    if( mapname )
        g_mapname = strdup(mapname);
    else
        g_mapname = strdup("Undefined");

#if SERVER
    {
        if(mapname)
        {
            char *isInMissionDirectory = strstr(mapname, "maps/Missions");
            if(isInMissionDirectory || OnMissionMap())
            {
                VerifyRoomMarkers();
            }
        }
    }
    EncounterSetMissionMode(1);
#endif
}

// this is a hostage encounter group.  Verify all enclosed spawns are marked as
// hostage encounters (PersonObjective)

void VerifyAllHostageEncounters(EncounterGroup* group)
{
#if SERVER
    int i;
    for (i = 0; i < group->children.size; i++)
    {
        EncounterPoint* point;
        point = group->children.storage[i];
        if (stricmp(point->name, "PersonObjective"))
        {
            ErrorFilenamef(g_mapname, "Hostage encounter group %s contains a spawn of type %s (should be PersonObjective)",
                group->geoname, point->name);
            break;
        }
    }
    if (group->children.size == 0)
    {
        ErrorFilenamef(g_mapname, "Hostage encounter group %s is empty", group->geoname);
    }
#endif
}

// we get called by encounter system when the group gets loaded.
// we need to attach this group to the correct room so we can
// find groups to attach encounters to later.
// we mark the group as visited if it shouldn't have to go off (hostage encounters)
void MissionProcessEncounterGroup(EncounterGroup* group)
{
#if SERVER
    RoomInfo* room;

    if (!OnMissionMap()) 
        return;

    room = RoomGetInfoEncounter(group);

    // record how many encounters aren't in a tray at all
    if (room == g_rooms[0] && !room->hadtracker)
        g_encountersoutsidetray++;

    if (group->personobjective)    // we are a hostage group
    {
        if (!room->hostageEncounters)
            eaCreate(&room->hostageEncounters);
        eaPush(&room->hostageEncounters, group);
        VerifyAllHostageEncounters(group);
    }
    else    // normal encounter
    {
        if (!room->standardEncounters)
            eaCreate(&room->standardEncounters);
        eaPush(&room->standardEncounters, group);
    }
    // hostage groups don't go off unless we specifically attach a spawn to them
    if (group->personobjective)
    {
        group->conquered = 1;    // shouldn't go off
    }
#endif
}

// *********************************************************************************
//  Mission marker debugging
// *********************************************************************************

RoomInfo* g_itemTestingRoom = NULL;

// this is just for testing item objectives.  it's called from a
// static map to load item objective locations for testing.
// it sets up a fake room to put them all in
RoomInfo* ItemTestingLoad(ClientLink* client)
{
    GroupDefTraverser traverser = {0};

    if (g_itemTestingRoom)
        eaClearEx(&g_itemTestingRoom->items, NULL);
    else
        g_itemTestingRoom = calloc(sizeof(RoomInfo), 1);

    g_curroom = g_itemTestingRoom;
    g_grouptype = LOCATION_ITEMFLOOR;
    groupProcessDefExBegin(&traverser, &group_info, ItemPosLoader);
    return g_itemTestingRoom;
}

// called on each object that we are trying to put into a tray
static void LogMapLoad(GroupDefTraverser* traverser, DefTracker* tracker, char* description, ...)
{
    va_list arg;
    char buf[1000];
    if (!g_logmapload)
        return;

    va_start(arg, description);
    vsprintf(buf, description, arg);
    va_end(arg);

    filelog_printf( "missionmapstats", "%s. group %s (%0.0f,%0.0f,%0.0f), in tray %s",
        buf, traverser? traverser->def->name: "(unknown)", 
        traverser? traverser->mat[3][0]: 0.0, 
        traverser? traverser->mat[3][1]: 0.0, 
        traverser? traverser->mat[3][2]: 0.0,
        tracker?tracker->def->name: "(no tray found)");
}
#if SERVER 

// just a little dumb function, print to both missionmapstats.log, and to console
static void logconPrintf(ClientLink* client, char* s, ...)
{

    va_list arg;
    va_start(arg, s);
    filelog_vprintf("missionmapstats", s, arg);
    convPrintf(client, s, arg);
    va_end(arg);

}

static void CountMissionEncounters(EncounterGroup **groups, MissionPlaceStats *placeStats)
{
    int groupIndex, groupCount, isMission, childrenIndex, customIndex, customCount;
     groupCount = eaSize(&groups); 
    for (groupIndex = 0; groupIndex < groupCount; groupIndex++)
    {
        int addedCustom = 0;
        isMission = 0;
        
        // record all the non-Mission layouts
        for (childrenIndex = 0; childrenIndex < groups[groupIndex]->children.size; childrenIndex++)
        {
            EncounterPoint* point = (EncounterPoint*)groups[groupIndex]->children.storage[childrenIndex];
            if (!stricmp(point->name, "Mission"))
            {
                isMission = 1;
            }
        }

        for (childrenIndex = 0; childrenIndex < groups[groupIndex]->children.size; childrenIndex++)
        {
            EncounterPoint* point = (EncounterPoint*)groups[groupIndex]->children.storage[childrenIndex];
            if (!stricmp(point->name, "Mission"))
                 continue;

            customCount = eaSize(&placeStats->customEncounters);
            if( customCount > 0 )
                addedCustom = 1;

            for (customIndex = 0; customIndex < customCount; customIndex++)
            {
                if (!strcmp(placeStats->customEncounters[customIndex]->type, point->name))
                {
                    if( isMission )
                        placeStats->customEncounters[customIndex]->countGrouped++;
                    else
                        placeStats->customEncounters[customIndex]->countOnly++;
                    break;
                }
            }
            if (customIndex == customCount) // didn't find the name in the list of types
            {
                MissionCustomEncounterStats *newCustomStats;
                newCustomStats = StructAllocRaw(sizeof(MissionCustomEncounterStats));
                newCustomStats->type = StructAllocString(point->name);
                if( isMission )
                    newCustomStats->countGrouped = 1;
                else
                    newCustomStats->countOnly = 1;
                eaPush(&placeStats->customEncounters, newCustomStats);
            }
        }

        if (isMission)
        {
            if( addedCustom )
                placeStats->genericEncounterGroupedCount++;
            else
                placeStats->genericEncounterOnlyCount++;
        }
    }
}


void MissionMapStats_gather(MissionMapStats *mapStats, Entity *player)
{
    int roomCount, roomIndex, placeIndex, itemCount, itemIndex;
    MissionPlaceEnum place;
    MissionPlaceStats *placeStats;

    // load all the map objects again
    filelog_printf("missionmapstats", "----- generating mission map stats");
    MissionSpecReload();
    MissionLoad(1, 0, 0);
    if (g_activemission && player)  
    {
        MissionForceUninit();
        MissionForceReinit(player);
    }
    else
    {
        MissionForceLoad(1);    // have to fake out the encounters if we are editing a mission map
        EncounterLoad();        // usually covered in the missionreinit
        MissionForceLoad(0);
    }

    //get the number of floors
    mapStats->floorCount =  g_sectioncount;

    mapStats->roomCount = eaSize(&g_rooms) - 1; // roomCount doesn't count the room where the index is 0.

    roomCount = eaSize(&g_rooms);
    for (roomIndex = 0; roomIndex < roomCount; roomIndex++)
    {
        if (g_rooms[roomIndex]->objectiveMarkers && roomIndex != 0)
        {
            mapStats->objectiveRoomCount++;
        }

        for (placeIndex = 0; placeIndex < mapStats->placeCount; placeIndex++)
        {
            placeStats = mapStats->places[placeIndex];
            place = placeStats->place;

            if ((place == MISSION_NONE && roomIndex != 0) ||
                (place != MISSION_NONE && roomIndex == 0) ||
                (place == MISSION_FRONT && !g_rooms[roomIndex]->startroom) ||
                (place == MISSION_BACK && !g_rooms[roomIndex]->endroom) ||
                (place == MISSION_MIDDLE && (g_rooms[roomIndex]->endroom || g_rooms[roomIndex]->startroom)) ||
                ((place == MISSION_LOBBY) ^ (g_rooms[roomIndex]->lobbyroom)) )
            {
                continue;
            }

            placeStats->hostageLocationCount += eaSize(&g_rooms[roomIndex]->hostageEncounters);
            itemCount = eaSize(&g_rooms[roomIndex]->items);
            for (itemIndex = 0; itemIndex < itemCount; itemIndex++)
            {
                switch (g_rooms[roomIndex]->items[itemIndex]->loctype)
                {
                case LOCATION_ITEMWALL:
                    placeStats->wallObjectiveCount++;
                xcase LOCATION_ITEMFLOOR:
                    placeStats->floorObjectiveCount++;
                xcase LOCATION_ITEMROOF:
                    placeStats->roofObjectiveCount++;
                }
            }
            CountMissionEncounters(g_rooms[roomIndex]->standardEncounters, placeStats);
        }
    }

    // we lied to the encounters to say we were on a mission map before, and have to clean up now
    if (!g_activemission)
    {
        EncounterUnload();
        EncounterSetMissionMode(0);
    }
}

static void MissionMapStats_print(MissionMapStats *mapStats, ClientLink *client)
{
    int placeIndex, customCount, customIndex;
    MissionPlaceStats *placeStats;

    filelog_printf("missionmapstats", "------ map loaded");
    logconPrintf(client, "Mission map stats for %s", g_mapname);
    logconPrintf(client, "%i rooms, %i of these are marked as objective rooms", mapStats->roomCount, mapStats->objectiveRoomCount);

    for (placeIndex = 0; placeIndex < mapStats->placeCount; placeIndex++)
    {
        switch (mapStats->places[placeIndex]->place)
        {
        case MISSION_ANY:
            logconPrintf(client, "Total:");
        xcase MISSION_LOBBY:
            logconPrintf(client, "Lobby Room:");
        xcase MISSION_FRONT:
            logconPrintf(client, "Front Room:");
        xcase MISSION_MIDDLE:
            logconPrintf(client, "Middle Rooms:");
        xcase MISSION_BACK:
            logconPrintf(client, "Back Room:");
        xcase MISSION_NONE:
            logconPrintf(client, "Not in a mission room (these won't spawn):");
        xdefault:
            logconPrintf(client, "Undefined room. Alert a programmer.");
        }

        placeStats = mapStats->places[placeIndex];

        logconPrintf(client, "   %i grouped generic mission encounters", placeStats->genericEncounterGroupedCount);
        logconPrintf(client, "   %i generic only mission encounters", placeStats->genericEncounterOnlyCount);
        logconPrintf(client, "   %i hostage locations", placeStats->hostageLocationCount);
        logconPrintf(client, "   %i wall objectives, %i floor objectives, and %i roof objectives", 
            placeStats->wallObjectiveCount, placeStats->floorObjectiveCount, placeStats->roofObjectiveCount);
        
        customCount = eaSize(&placeStats->customEncounters);
        for (customIndex = 0; customIndex < customCount; customIndex++)
        {
            logconPrintf(client, "   %i custom grouped encounters (%s)", placeStats->customEncounters[customIndex]->countGrouped, placeStats->customEncounters[customIndex]->type);
            logconPrintf(client, "   %i custom only encounters (%s)", placeStats->customEncounters[customIndex]->countOnly, placeStats->customEncounters[customIndex]->type);
        }
    }

    filelog_printf("missionmapstats", "------ done");
}


// debug only - reinitializes mission and counts map objects
void MissionDebugPrintMapStats(ClientLink *client, Entity *player, char * mapname)
{
    MissionMapStats *mapStats = missionMapStats_initialize(mapname);
    MissionMapStats_gather(mapStats, player);
    MissionMapStats_print(mapStats, client);
    missionMapStats_destroy(mapStats);

}

void missionMapStats_SaveToFile(MissionMapStats *mapStats, int checkin)
{
    char fileName[1000];
    //sprintf(fileName, "%s/%s.mapstats", fileDataDir(), mapStats->pchMapName);
    sprintf(fileName, "%s.mapstats", forwardSlashes(mapStats->pchMapName) );
    missionMapStats_Save(mapStats, fileName);
}

// used on map save
void MissionDebugSaveMapStatsToFile(Entity *player, char * mapname)
{
    char mapName[512];
    char *s;
    
    //get the position of maps in the filename.  This is the root of our filename.
    s = strstr(mapname, "maps");

    if (!s)
        s = strstr(mapname, "MAPS"); // mapname isn't guaranteed to be all-lower...

    if (s)
    {
        MissionMapStats *mapStats;

        strcpy(mapName, s);
        mapStats = missionMapStats_initialize(mapName);//(dbGetMapName(getMapID()));//
        g_logmapload = 1;
        MissionMapStats_gather(mapStats, player);
        g_logmapload = 0;
        missionMapStats_SaveToFile(mapStats, 0);
        missionMapStats_destroy(mapStats);
    }
}

void MissionMapStats_MakeFile( char * pchFile )
{
    MissionMapStats *mapStats = missionMapStats_initialize(pchFile);
    MissionMapStats_gather(mapStats, 0);
    missionMapStats_SaveToFile(mapStats, 1);
    missionMapStats_destroy(mapStats);
}


static int CheckMapStatsPlace(ClientLink *client, MissionMapStats *mapStats, MissionPlaceEnum place, const MissionSpec *spec)
{
    const MissionAreaSpec *mas = NULL;
    MissionPlaceStats *placeStats = NULL;
    int placeIndex, groupIndex, groupCount, customIndex, customCount, layoutIndex, layoutCount;

    if (place == MISSION_FRONT)
    {
        mas = &spec->front;
    }
    else if (place == MISSION_MIDDLE)
    {
        mas = &spec->middle;
    }
    else if (place == MISSION_BACK)
    {
        mas = &spec->back;
    }
    else if (place == MISSION_LOBBY)
    {
        mas = &spec->lobby;
    }
    else
    {
        return 0;
    }

    for (placeIndex = 0; placeIndex < mapStats->placeCount; placeIndex++)
    {
        if (mapStats->places[placeIndex]->place == place)
        {
            placeStats = mapStats->places[placeIndex];
        }
    }
    if (placeStats == NULL)
    {
        return 0;
    }

    if (placeStats->genericEncounterOnlyCount < mas->missionSpawns || 
        placeStats->hostageLocationCount < mas->hostages ||
        placeStats->wallObjectiveCount < mas->wallItems ||
        placeStats->floorObjectiveCount < mas->floorItems ||
        placeStats->roofObjectiveCount < mas->roofItems)
    {
        return 0;
    }

    groupCount = eaSize(&mas->encounterGroups);
    customCount = eaSize(&placeStats->customEncounters);
    for (groupIndex = 0; groupIndex < groupCount; groupIndex++)
    {
        const EncounterGroupSpec *group = mas->encounterGroups[groupIndex];
        layoutCount = eaSize(&group->layouts);
        for (layoutIndex = 0; layoutIndex < layoutCount; layoutIndex++)
        {
            for (customIndex = 0; customIndex < customCount; customIndex++)
            {
                if (placeStats->customEncounters[customIndex]->type == group->layouts[layoutIndex] && 
                    placeStats->customEncounters[customIndex]->countOnly < group->count)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}


// debug only - checks to make sure this map meets the mapspecs requirements
void MissionDebugCheckMapStats(ClientLink *client, Entity *player, int sendpopup, char * mapname)
{
    MissionMapStats *mapStats = missionMapStats_initialize(mapname);
    int i;
    char buf[256];
    int time;
    const MissionSpec *spec = NULL;
    if (world_name == NULL || strrchr(world_name,'_') == NULL || 
        strrchr(world_name,'.') == NULL || strstr(world_name,"_R_") == NULL)
    {
        if (sendpopup)
        {
            sendEditMessage(client->link, 1, "world_name is not valid (%x)\n", world_name);
        }
        START_PACKET(pak, player, SERVER_MAP_GENERATION);
            pktSendBitsPack(pak, 1, 0);
        END_PACKET;
        return;
    }
    strcpy(buf, strstr(world_name,"_R_") + 3);        //_R_caves30_12345.txt becomes
    (*strrchr(buf,'_')) = 0;                        //caves30

    i = (int)strlen(buf) - 1;
    while (i >= 0 && buf[i] >= '0' || buf[i] <= '9') 
    {
        i--;
    }
    if (i == strlen(buf) - 1)
    {
        if (sendpopup)
        {
            sendEditMessage(client->link, 1, "world_name is not valid (%x)\n", world_name);
        }
        START_PACKET(pak, player, SERVER_MAP_GENERATION);
            pktSendBitsPack(pak, 1, 0);
        END_PACKET;
        return;
    }
    for (i = 0; i < (int) strlen(buf); i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
        {
            sscanf(buf + i, "%d", &time);                //and time=30
            buf[i] = 0;                                //now we have "caves"
            break;
        }
    }

    
    MissionMapStats_gather(mapStats, player);

    //find which missionSpecs matches these values

    for (i = 0; i < eaSize(&missionSpecFile.missionSpecs); i++)
    {
        if (missionSpecFile.missionSpecs[i]->mapSetTime == time &&
            !stricmp(StaticDefineIntRevLookup(ParseMapSetEnum, missionSpecFile.missionSpecs[i]->mapSet), buf))
        {
            spec = missionSpecFile.missionSpecs[i];
            break;
        }
    }

    if (spec == NULL)
    {
        if (sendpopup)
        {
            sendEditMessage(client->link, 1, "No mapspec found for %s%d", buf, time);
        }
        START_PACKET(pak, player, SERVER_MAP_GENERATION);
            pktSendBitsPack(pak, 1, 0);
        END_PACKET;
        return;
    }

    if (!CheckMapStatsPlace(client, mapStats, MISSION_FRONT, spec) ||
        !CheckMapStatsPlace(client, mapStats, MISSION_MIDDLE, spec) ||
        !CheckMapStatsPlace(client, mapStats, MISSION_BACK, spec) ||
        !CheckMapStatsPlace(client, mapStats, MISSION_LOBBY, spec))
    {
        if (sendpopup)
        {
            sendEditMessage(client->link, 1, "Map %s is NOT valid.\n", world_name);
        }
        START_PACKET(pak, player, SERVER_MAP_GENERATION);
            pktSendBitsPack(pak, 1, 0);
        END_PACKET;
    }
    else
    {
        if (sendpopup)
        {
            sendEditMessage(client->link, 1, "Map %s is valid.\n", world_name);
        }
        START_PACKET(pak, player, SERVER_MAP_GENERATION);
        pktSendBitsPack(pak, 1, 1);
        END_PACKET;
    }

    missionMapStats_destroy(mapStats);
}

#endif
