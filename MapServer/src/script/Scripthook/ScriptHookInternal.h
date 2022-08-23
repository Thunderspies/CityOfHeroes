/*\
 *
 *    scripthookinternal.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions that should only be called internally to the scripthook system
 *
 */

#ifndef SCRIPTHOOKINTERNAL_H
#define SCRIPTHOOKINTERNAL_H

#include "storyarc/missiongeoCommon.h"
#include "utils/position.h"

#define SCRIPTMARKER_NAMELEN    50
typedef struct ScriptMarker
{
    char        name[SCRIPTMARKER_NAMELEN];
    Position**    locs;
    U32            namedvolume : 1;            // otherwise a normal script marker
    StashTable    properties;
    char        *geoName;
} ScriptMarker;

// a scriptdef attached to a location on the map, with
// some attributes telling it when to start
typedef struct ScriptLocation
{
    Vec3        pos;
    Mat4        mat;
    const ScriptDef*    def;
    char*        name;            // optional logical name
    int            scriptid;        // iff running, this is set
    int            locationid;        // index into location list for this element
    F32            timerBegin;        // the script will start between Begin and Begin+Range hours from when it last ran
    F32            timerRange;
    U32            timer;            // dbSeconds when the script will run next (if timerControlled)

    unsigned    alwaysRun        :1;
    unsigned    timerControlled    :1;
} ScriptLocation;

extern ScriptLocation** g_scriptLocations;

// ScriptHookEntityteam.c
Entity* EntTeamInternal(TEAM team, int index, int* num);
Entity* EntTeamInternalEx(TEAM team, int index, int* num, int onlytargetable, int countDead);
ENTITY EntityNameFromEnt(Entity* e);
void ScriptTeamAddEnt(Entity* e, TEAM team);

// ScriptHookCallbacks.c
int npcClicked(void* func, Entity* player, Entity* npc);

// ScriptHookLocation.c
RoomInfo* GetRoomInfo( STRING missionroom );

// scripthook.c
ScriptMarker* MarkerFind(const char* name, int namedvolume);

// ScriptHookencounter.c
EncounterGroup* FindEncounterGroupInternal(AREA area, STRING layout, int inactiveonly, int deserted);


#endif