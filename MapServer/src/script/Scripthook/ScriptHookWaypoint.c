/*\
 *
 *    scripthookwaypoint.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to dynamic waypoints
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "entity/entity.h"
#include "group/groupnetsend.h"
#include "gameComm/sendToClient.h"
#include "storyarc/storyarcprivate.h"

#include "script/Scripthook/ScriptHookInternal.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// W A Y P O I N T S
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ScriptWaypoint {
    char    *name;
    char    *icon;
    char    *iconB;
    int        compass;
    int        pulse;
    float    rotate;
    Vec3    location;
    Entity    **users;
} ScriptWaypoint;

static int WaypointIDs = 0;
static StashTable scriptWaypointList = NULL;

NUMBER CreateWaypoint(LOCATION location, STRING text, STRING icon, STRING iconB, NUMBER compass, NUMBER pulse, FRACTION rotate)
{
    ScriptWaypoint    *waypoint = NULL;
    int                waypointID = ++WaypointIDs;
    Vec3            loc = {0};
    char            key[20];

    if (scriptWaypointList == NULL)
        scriptWaypointList = stashTableCreateWithStringKeys(20, StashDeepCopyKeys | StashCaseSensitive );

    waypoint = malloc(sizeof(ScriptWaypoint));
    waypoint->users = NULL;
    waypoint->compass = compass;
    waypoint->pulse = pulse;
    if (text && text[0]) {
        waypoint->name = strdup(text);
    } else {
        waypoint->name = NULL;
    }

    if (icon) {
        waypoint->icon = strdup(icon);
    } else {
        waypoint->icon = strdup("map_enticon_mission_c");
    }

    if (iconB) {
        waypoint->iconB = strdup(iconB);
    } else {
        waypoint->iconB = NULL;
    }

    if (!location || !GetPointFromLocation(location, waypoint->location))
    {
        memset(waypoint->location, 0, sizeof(Vec3));
    }

    if (rotate >= 0.0f)
        waypoint->rotate = rotate;
    else
        waypoint->rotate = RAD(360.0f);

    sprintf(key, "%i", waypointID);
    stashAddPointer(scriptWaypointList, key, waypoint, false);
    return waypointID;
}

void DestroyWaypoint(NUMBER id)
{
    int                i, n;
     char            key[20];
    ScriptWaypoint    *waypoint = NULL;
    Vec3            none = {0};

    sprintf(key, "%i", id);

    if( !scriptWaypointList )
        return;

    stashRemovePointer(scriptWaypointList, key, &waypoint);
    if (waypoint) {

        // clean up all users of this waypoint
        n = eaSize(&waypoint->users);
        for (i = 0; i < n; i++)
        {
            Entity* e = waypoint->users[i];

            if (e != NULL)
                sendServerWaypoint(e, id, 0, 0, 0, NULL, NULL, NULL, NULL, none, -1.0f);
        }
        if (waypoint->users)
            eaDestroy(&waypoint->users);
    
        // free all memory associated with this waypoint
        if (waypoint->name)
            free(waypoint->name);
        if (waypoint->icon)
            free(waypoint->icon);
    }
}

void SetWaypoint(TEAM team, NUMBER id)
{
    int                i, n;
    char            key[20];

    ScriptWaypoint *waypoint = NULL;
    sprintf(key, "%i", id);

    stashFindPointer( scriptWaypointList, key, &waypoint );
    if (waypoint) {
        if (waypoint->users == NULL)
        {
            eaCreate(&waypoint->users);
        }

        n = NumEntitiesInTeam(team);
        for (i = 0; i < n; i++)
        {
            Entity* e = EntTeamInternal(team, i, NULL);
            if (e) 
            {
                if (eaFind(&waypoint->users, e) == -1)
                {
                    sendServerWaypoint(e, id, 1, waypoint->compass, waypoint->pulse, waypoint->icon, waypoint->iconB, saUtilScriptLocalize(waypoint->name), saUtilCurrentMapFile(), waypoint->location, waypoint->rotate);
                    eaPush(&waypoint->users, e);
                }
            }
        }
    }
}
 
void ClearAllWaypoints(TEAM team)
{
    StashTableIterator hashIter;
    StashElement elem;
    int                i, n;
    Vec3            none = {0};

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e) {
            sendServerWaypoint(e, -1, 0, 0, 0, NULL, NULL, NULL, NULL, none, -1.0f);
 
            if (scriptWaypointList) 
            {
                stashGetIterator(scriptWaypointList, &hashIter);
                while (stashGetNextElement(&hashIter, &elem))
                {
                    ScriptWaypoint *waypoint = stashElementGetPointer(elem);

                    if (waypoint && waypoint->users ) {
                        eaFindAndRemove(&waypoint->users, e);
                    }
                }
            }
        }
    }
}

void ClearWaypoint(TEAM team, NUMBER id)
{
    int                i, n;
    char            key[20];
    Vec3            none = {0};

    ScriptWaypoint *waypoint = NULL;
    sprintf(key, "%i", id);

    stashFindPointer( scriptWaypointList, key, &waypoint );
    if (waypoint) {
        if (waypoint->users == NULL)
        {
            eaCreate(&waypoint->users);
        }

        n = NumEntitiesInTeam(team);
        for (i = 0; i < n; i++)
        {
            Entity* e = EntTeamInternal(team, i, NULL);
            if (e) {
                sendServerWaypoint(e, id, 0, 0, 0, NULL, NULL, NULL, NULL, none, -1.0f);
                eaFindAndRemove(&waypoint->users, e);
            }
        }
    }
}

void UpdateWaypoint(NUMBER id, LOCATION location, STRING text, STRING icon, FRACTION rotate)
{
    char            key[20];
    int                i, n;

    ScriptWaypoint *waypoint = NULL;
    sprintf(key, "%i", id);
    stashFindPointer( scriptWaypointList, key, &waypoint );

    if (waypoint) {
        if (text && text[0]) {
            if (waypoint->name)
                free(waypoint->name);
            waypoint->name = strdup(text);
        }

        if (icon) {
            if (waypoint->icon)
                free(waypoint->icon);
            waypoint->icon = strdup(icon);
        }

        if (location)
            GetPointFromLocation(location, waypoint->location);

        if (rotate >= 0.0f)
            waypoint->rotate = rotate;
        else
            waypoint->rotate = 90.0f;

        n = eaSize(&waypoint->users);
        for (i = 0; i < n; i++)
        {
            Entity* e = waypoint->users[i];
            if (e) {
                sendServerWaypoint(e, id, 1, waypoint->compass, waypoint->pulse, waypoint->icon, waypoint->iconB, 
                                    saUtilScriptLocalize(waypoint->name), world_name, 
                                    waypoint->location, waypoint->rotate);
            }
        }
    }
}
