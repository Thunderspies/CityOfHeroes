/*\
 *
 *    scripthooklocation.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to locations
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "generator/encounterPrivate.h"
#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "svr/svr_player.h"
#include "entity/entPlayer.h"
#include "entity/entGameActions.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_target.h"
#include "entity/character_tick.h"
#include "entity/entity.h"
#include <utilitieslib/components/StashTable.h>
#include "group/group.h"
#include "group/groupProperties.h"

#include "door.h"

#include "storyarc/missiongeoCommon.h"
#include "gridcoll/gridcoll.h"
#include "entity/motion.h"

#include "script/Scripthook/ScriptHookInternal.h"

// *********************************************************************************
//  Location references
// *********************************************************************************

//Takes name after "missionroom:"
RoomInfo* GetRoomInfo( STRING missionroom )
{
    RoomInfo* info = 0;

    if( 0 == stricmp( "end", missionroom ) )
    {
        info = EndRoomInfo( missionroom );
    }
    else if( 0 == stricmp( "start", missionroom ) )
    {
        info = StartRoomInfo( missionroom );
    }
    else
    {
        int roomNum;
        roomNum = atoi(missionroom);
        if (!roomNum)
        {
            ScriptError("MissionRoomName given invalid format 'missionroom:%s", missionroom);
            return 0;
        }
        info = RoomInfoFromPace(roomNum);
    }

    return info;
}

static const int LOCATION_RADIUS_2 = LOCATION_RADIUS * LOCATION_RADIUS;


// generic system for locations - GetPointFromLocation & PointInArea define these
//                                    POINT                AREA
//        location:none                (none)                (none)
//        marker:name                    exact                approx (30' radius)
//        scriptlocation:id            exact                approx (30' radius)
//        coord:x,y,z                    exact                 approx (30' radius)
//        missionroom:room_number        approx (marker loc)    exact
//        trigger:name                approx (group loc)    exact
//        encountergroup:name            exact                approx (30' radius)
//        encounterid:idnum            exact                approx (30' radius)
//            note: if multiple encountergroups have same name, one is chosen at random

//referencePoint: if the more than one location is found -- return the one closest to this reference point
//TO DO : add reference point to things besides doors
int GetMatFromLocation( LOCATION location, Mat4 mat, const Vec3 referencePoint )
{
    int i;
    if( !location || !mat )
        return 0;
    if (stricmp(location, LOCATION_NONE)==0)
        return 0;

    copyMat4( unitmat, mat );
    
    if (strnicmp(location, "marker:", strlen("marker:"))==0)
    {
        ScriptMarker* marker = MarkerFind(location + strlen("marker:"), 0);
        if (!marker || !marker->locs)
            return 0;

        i = rand() % eaSize(&marker->locs);
        copyMat4(marker->locs[i]->mat, mat);
        return 1;
    }
    else if (strnicmp(location, "scriptlocation:", strlen("scriptlocation:"))==0)
    {
        ScriptLocation *script = g_scriptLocations[atoi(location + strlen("scriptlocation:"))];
        if (!script)
            return 0;

        copyMat4(script->mat, mat);
        return 1;
    }
    else if ( (strnicmp(location, "ent:", strlen("ent:"))==0) || (strnicmp(location, "player:", strlen("player:"))==0) )
    {
        Entity * e = EntTeamInternal(location, 0, NULL);
        if (!e)
            return 0;

        copyMat4(ENTMAT(e), mat);
        return 1;
    }
    else if ( (strnicmp(location, "random:player", strlen("random:player"))==0) )
    {
        NUMBER n;
        n = NumEntitiesInTeam(ALL_PLAYERS);
        
        if(!n)
            return 0;
        else
        {
            Entity * e = EntTeamInternal(GetEntityFromTeam(ALL_PLAYERS, RandomNumber(1, n) ), 0, NULL);
    
            if (!e)
                return 0;

            copyMat4(ENTMAT(e), mat);
            return 1;
        }
    }
    else if (strnicmp(location, "coord:", strlen("coord:"))==0)
    {
        Vec3 v;
        if (sscanf(location + strlen("coord:"), "%f,%f,%f", &v[0], &v[1], &v[2]) != 3)
        {
            ScriptError("GetPointFromLocation given invalid format %s", location);
            return 0;
        }
        copyVec3(v, mat[3]);
        return 1;
    }
    else if (strnicmp(location, "matrix:", strlen("matrix:"))==0)
    {
        Mat4 m;
        if (sscanf(location + strlen("matrix:"), "%f,%f,%f/%f,%f,%f/%f,%f,%f/%f,%f,%f", &m[0][0], &m[0][1], &m[0][2], 
                                                                                        &m[1][0], &m[1][1], &m[1][2],
                                                                                        &m[2][0], &m[2][1], &m[2][2],
                                                                                        &m[3][0], &m[3][1], &m[3][2]) != 12)
        {
            ScriptError("GetPointFromLocation given invalid format %s", location);
            return 0;
        }
        copyMat4(m, mat);
        return 1;
    }
    else if (strnicmp(location, "missionroom:", strlen("missionroom:"))==0)
    {
        RoomInfo* info;
        
        info = GetRoomInfo( (location + strlen("missionroom:") ) );

        if (!info || !info->tracker)
            return 0;

        copyVec3(info->markerPos, mat[3]);
        return 1;
    }
    else if (strnicmp(location, "trigger:", strlen("trigger:"))==0)
    {
        ScriptMarker* marker = MarkerFind(location + strlen("trigger:"), 1);
        if (!marker || !marker->locs)
            return 0;

        i = rand() % eaSize(&marker->locs);
        copyMat4(marker->locs[i]->mat, mat);
        return 1;
    }
    else if (strnicmp(location, "encountergroup:", strlen("encountergroup:"))==0)
    {
        EncounterGroup* group = EncounterGroupByName(location + strlen("encountergroup:"), NULL, 0);
        if (!group)
            return 0;

        copyMat4(group->mat, mat);
        return 1;
    }
    else if (strnicmp(location, "encounterid:", strlen("encounterid:"))==0)
    {
        EncounterGroup* group = EncounterGroupFromHandle(atoi(location + strlen("encounterid:")));
        if (!group)
        {
            // this should be error, because it should be impossible for scripts to
            // get a hold of an invalid handle
            ScriptError("GetPointFromLocation passed an invalid encounter handle %s", location);
            return 0;
        }
        copyMat4(group->mat, mat);
        return 1;
    }
    //These next two are synonyms : the field spawndef->name is the missionencounter name for task included spawns 
    //and spawndef name for non included encounters
    else if ( strnicmp(location, "missionencounter:", strlen("missionencounter:"))==0 )
    {
        EncounterGroup *results[ENT_FIND_LIMIT];
        int count;
        count = FindEncounterGroups( results, 
            0,                //Center point to start search from
            0,                //No nearer than this
            0,                //No farther than this
            0,                //inside this area
            0,                //Layout required in the EncounterGroup
            0,                //Name of the encounter group ( Property GroupName )
            SEF_FULL_LIST,    //Flags 
            location + strlen("missionencounter:")    //missionEncounterName
            );

        if (!count)
            return 0;
        copyMat4(results[ RandomNumber(0, count-1) ]->mat, mat);
        return 1;
    }
    //Synonym for missionencounter
    else if ( strnicmp(location, "spawndef:", strlen("spawndef:"))==0 )
    {
        EncounterGroup *results[ENT_FIND_LIMIT];
        int count;
        count = FindEncounterGroups( results, 
            0,                //Center point to start search from
            0,                //No nearer than this
            0,                //No farther than this
            0,                //inside this area
            0,                //Layout required in the EncounterGroup
            0,                //Name of the encounter group ( Property GroupName )
            SEF_FULL_LIST,    //Flags 
            location + strlen("spawndef:")    //missionEncounterName    
            );

        if (!count)
            return 0;
        copyMat4(results[ RandomNumber(0, count-1) ]->mat, mat);
        return 1;
    }
    else if ( strnicmp(location, "missionencounterpos:", strlen("missionencounterpos:"))==0 )
    {
        char encounterName[100];
        int encounterPos;
        EncounterGroup *results[ENT_FIND_LIMIT];
        int count;

        if (sscanf(location + strlen("missionencounterpos:"), "%[^:]:%d", &encounterName, &encounterPos) != 2)
        {
            ScriptError("GetPointFromLocation given invalid format %s", location);
            return 0;
        }

        count = FindEncounterGroups( results, 
            0,                //Center point to start search from
            0,                //No nearer than this
            0,                //No farther than this
            0,                //inside this area
            0,                //Layout required in the EncounterGroup
            0,                //Name of the encounter group ( Property GroupName )
            SEF_FULL_LIST,    //Flags 
            encounterName    //missionEncounterName
            );

        if (!count)
            return 0;

        if (!EncounterMatFromPos(results[ RandomNumber(0, count-1) ], encounterPos, mat))
            return 0;
        return 1;
    }
    else if ( strnicmp(location, "missionencounterlayoutpos:", strlen("missionencounterlayoutpos:"))==0 )
    {
        char encounterName[100];
        char encounterLayout[100];
        int encounterPos, count;
        EncounterGroup *results[ENT_FIND_LIMIT];

        if (sscanf(location + strlen("missionencounterlayoutpos:"), "%[^:]:%[^:]:%d", &encounterName, &encounterLayout, &encounterPos) != 3)
        {
            ScriptError("GetPointFromLocation given invalid format %s", location);
            return 0;
        }

        count = FindEncounterGroups( results, 
            0,                //Center point to start search from
            0,                //No nearer than this
            0,                //No farther than this
            0,                //inside this area
            0,                //Layout required in the EncounterGroup
            0,                //Name of the encounter group ( Property GroupName )
            SEF_FULL_LIST,    //Flags 
            encounterName    //missionEncounterName
            );

        if (!count)
            return 0;

        if (!EncounterMatFromLayoutPos(results[ RandomNumber(0, count-1) ], encounterLayout, encounterPos, mat))
            return 0;
        return 1;
    }
    else if (strnicmp(location, "objective:", strlen("objective:"))==0)
    {
        // Process all mission objectives of the specified name.
        int foundObj = 0;
        int objCursor;
        const char * objective = location + strlen("objective:");

        for( objCursor = eaSize(&objectiveInfoList)-1; objCursor >= 0; objCursor-- )
        {
            MissionObjectiveInfo* obj = objectiveInfoList[objCursor];
            if(stricmp(obj->name, objective) != 0)
                continue;

            foundObj = 1;

            if( obj->encounter && obj->encounter->mat )
            {
                copyMat4( obj->encounter->mat, mat );
                return 1;
            }
            else if( obj->objectiveEntity )
            {
                Entity * objEnt = erGetEnt( obj->objectiveEntity );
                if( objEnt )
                {
                    copyMat4( ENTMAT(objEnt), mat );
                    return 1;
                }
            }
        }

        if( foundObj )
            ScriptError("BAD LOCATION: The objective %s exists, but doesn't have an encounter or objective associated with it, so has no location", location);
        else
            ScriptError("BAD LOCATION: The objective %s is not an objective on this mission.", location);

        return 0;
    }
    //If there are multiple doors with this name, return the location of the nearest one
    else if (strnicmp(location, "door:", strlen("door:"))==0)
    {
        DoorEntry * doors;
        DoorEntry * door;
        int count, i;
        DoorEntry * bestDoor = 0;
        F32 bestDist = 100000.0;
        const char * name = location + strlen("door:");

        // Gets all doors found on this map
        doors = dbGetLocalDoors(&count);
        for( i = 0 ; i < count ; i++ )
        {
            door = &doors[i];

            //Since Mission Return can be in special_info or in name, check both...
            //Conceivably could be buggy in the rare case the wrong one has the right value coincidentally
            //but I'd need to research when each has either 
            //Separate from point from Location because there can be more than one, and point in area returns the nearest one.
            if ( ( door->special_info && 0 == stricmp( door->special_info, name ) ) ||
                ( door->name         && 0 == stricmp( door->name,         name ) )  )
            {
                if( !bestDoor || (referencePoint && distance3Squared( referencePoint, door->mat[3] ) < bestDist ) )
                {
                    bestDoor = door;
                    if( referencePoint )
                        bestDist = distance3Squared( referencePoint, door->mat[3] );
                }
            }
        }

        if( bestDoor )
        {
            copyMat4( bestDoor->mat, mat );
            return 1;
        }
        return 0;
    }
    else
    {
        ScriptError("GetMatFromLocation given invalid location string %s", location);
        return 0;
    }
}


int GetPointFromLocation(LOCATION location, Vec3 vec)
{
    Mat4 mat;
    if( GetMatFromLocation( location, mat, 0 ) )
    { 
        copyVec3( mat[3], vec );
        return 1;
    }
    return 0;
}

// other function that defines generic system for locations
//        entity is optional, but used to optimize lookup of triggered volumes, etc.
int PointInArea(const Vec3 vec, AREA area, Entity* e)
{
    int i;
    if( !vec || !area || !e )
        return 0;
    if (stricmp(area, LOCATION_NONE)==0)
        return 0;
    if (stricmp(area, LOCATION_ANY)==0)
        return 1;

    if (strnicmp(area, "missionroom:", strlen("missionroom:"))==0)
    {
        RoomInfo* room;
        int pace = atoi(area + strlen("missionroom:"));

        // if I have an entity, I should have a cached volume lookup
        if (e &&
            (ENTTYPE(e) == ENTTYPE_PLAYER || ENTTYPE(e) == ENTTYPE_CRITTER) &&
            e->roomImIn)
        {
            room = RoomInfoFromTracker(e->roomImIn);
        }
        else
        {
            room = RoomGetInfo(vec);
        }
        if (pace == room->roompace)
            return 1;
        return 0;
    }
    else if (strnicmp(area, "trigger:", strlen("trigger:"))==0)
    {
        VolumeList* volumeList;
        const char* triggername = area + strlen("trigger:");

        // if I have an entity, I should have a cached volume lookup
        if (e && (ENTTYPE(e) == ENTTYPE_PLAYER || ENTTYPE(e) == ENTTYPE_CRITTER) )
        {
            volumeList = &e->volumeList;
        }
        else
        {
            groupFindInside(vec, FINDINSIDE_VOLUMETRIGGER, &sharedVolumeList, NULL);
            volumeList = &sharedVolumeList;
        }
        for (i = 0; i < volumeList->volumeCount; i++)
        {
            DefTracker* volume = volumeList->volumes[i].volumePropertiesTracker;
            PropertyEnt* prop = volume? stashFindPointerReturnPointer(volume->def->properties, "NamedVolume"): 0;
            if (prop && stricmp(prop->value_str, triggername)==0)
                return 1;
        }

        return 0;
    }
    else
    {
        //All other checks start as a location you're checking against
        Mat4 areaMat;

        //Mat because I only added the reference point code to the GetMatFromLocation
        if( GetMatFromLocation( area, areaMat, vec ) )
        {
            if (distance3Squared( areaMat[3], vec ) <= LOCATION_RADIUS_2)
                return 1;
        }
        return 0;
    }
}

AREA RoomFromEntity(ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    RoomInfo* room;

    if (!e)
        return LOCATION_NONE;

    // find what tray the ent is in, and then get the mission room from that..
    room = RoomGetInfo(ENTPOS(e));
    if (!room)
        return LOCATION_NONE;
    if (!room->roompace)
        return LOCATION_NONE;
    return StringAdd("MissionRoom:", NumberToString(room->roompace));
}

LOCATION PointFromEntity(ENTITY ent)
{
    char buf[2000];
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if (!e)
        return LOCATION_NONE;

    sprintf(buf, "coord:%f,%f,%f", ENTPOSPARAMS(e));
    return StringDupe(buf);
}

int EntityInArea(ENTITY ent, AREA area)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if (!e)
        return 0;
    return PointInArea(ENTPOS(e), area, e);
}

int LocationExists(LOCATION location)
{
    Vec3 vec;
    return GetPointFromLocation(location, vec);
}

LOCATION MyLocation(void)
{
    char buf[200];
    ScriptLocation* loc = g_currentScript->location;
    if (!loc)
    {
        ScriptError("MyLocation called from non-location script %s", g_currentScript->function->name);
        return LOCATION_NONE;
    }
//    sprintf(buf, "coord:%f,%f,%f", loc->pos[0], loc->pos[1], loc->pos[2]);
    sprintf(buf, "scriptlocation:%d", loc->locationid);
    return StringDupe(buf);
}

FRACTION DistanceBetweenEntities(ENTITY ent1, ENTITY ent2)
{
    Entity* e1 = EntTeamInternal(ent1, 0, NULL);
    Entity* e2 = EntTeamInternal(ent2, 0, NULL);

    if( e1 && e2 ) {
        return distance3( ENTPOS(e1), ENTPOS(e2) );
    } else {
        return 0.0f;
    }

}

FRACTION DistanceBetweenLocations(LOCATION loc1, LOCATION loc2)
{

    Vec3 vloc1, vloc2;

    if( GetPointFromLocation( loc1, vloc1) && GetPointFromLocation( loc2, vloc2))
    {
        return distance3( vloc1, vloc2 );
    }
    return 0.0f;
}

STRING GetXYZStringFromLocation(LOCATION loc)
{
    Vec3 vloc;
    char buf[2000];

    if( GetPointFromLocation( loc, vloc))
    {
        sprintf(buf, "%f,%f,%f", vloc[0], vloc[1], vloc[2]);
        return StringDupe(buf);
    }
    else
        return LOCATION_NONE;

}

NUMBER CountMarkers(STRING name)
{
    int i;
    char index[4];

    // Count up the number of locations.
    for (i = 0; i < MAX_MARKER_COUNT; i++)
    {
        index[0] = '_';
        index[1] = (i + 1) / 10 + '0';
        index[2] = (i + 1) % 10 + '0';
        index[3] = 0;
        if (!LocationExists(StringAdd(StringAdd("marker:", name), index)))
        {
            break;
        }
    }
    return i;
}
