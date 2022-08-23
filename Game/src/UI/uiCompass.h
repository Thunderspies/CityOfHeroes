#ifndef _COMPASS_H
#define    _COMPASS_H

#include "UI/uiInclude.h"
#include <utilitieslib/stdtypes.h>
#include "storyarc/ScriptUIEnum.h"
#include "storyarc/pnpcCommon.h"
#include "storyarc/contactCommon.h"

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef enum TrialStatus TrialStatus;

// destination stuff - (functions are used in uiAutomap.c )
//---------------------------------------------------------------------

// structure to hold destination information

typedef struct Destination
{
    int        navOn;            // is it on
    int        type;            // type of destination
    ContactDestType contactType;
    int        id;                // identifier
    AtlasTex *icon;            // icon
    AtlasTex *iconB;        // second background icon
    int        color;            // the color to draw it
    int        colorB;            // the color to draw the background

    float    angle;
    Vec2    map_location;    // 2d map coordinates, based on actual position
    Vec2    sep_location;    // coordinates, separated from nearby tacks
    Vec2    view_location;    // map coordinates user is going to see (between map & sep)
    Mat4    world_location; // world matix (for when orientation matters)
    Vec3    orig_world_pos;    // the position when it was first set
    bool    bHiddenInFog;    // Only show if the fog is revealed above it.

    char    name[256];        // der
    char    mapName[512];    // the map the destination is on
    char    *required;    // if the destination has a requires to show on the minimap, there's a copy here.
    bool    dontTranslateName;    // don't translate the name field.  Show as-is (mainly for player names)

    int        uid;            // the UID to match with when the server sends a waypoint

    unsigned int    iconBInCompass: 1;        // show iconB in compass
    unsigned int    showWaypoint: 1;        // show in compass and 3d (turned off by UpdateWaypoint)
    unsigned int    pulseBIcon: 1;            // pulse iconB in the minimap

    unsigned int    shownLastFrame: 1;        // for map separation code

    U32        creationTime;    // Used by the mission waypoint system to determine when we got a certain waypoint

    const PNPCDef *destPNPC;
    Entity    *destEntity;

    char    *pchSourceFile;

    union
    {
        struct { // ICON_MISSION
            U32        dbid;            
            int        context;
            int        subhandle;
        };
        struct { // DestinationIsAContact
            int        handle;
        };
    };
} Destination;

typedef struct ToolTip ToolTip;

typedef struct ScriptUIClientWidget
{
    ScriptUIType type;
    ToolTip* tooltip;
    char** varList;
    unsigned int* packetOrder;
    unsigned int uniqueID;
    unsigned int showHours : 1;        // Special var just for the timers
    unsigned int hidden    : 1;        //    hidden status
} ScriptUIClientWidget;

// get the destination name
//
char *compassGetDestName();

// set the destination
//
void compass_SetDestination(Destination* ref, Destination* newDest);

typedef struct TaskStatus TaskStatus;
typedef struct ContactStatus ContactStatus;
void compass_SetTaskDestination(Destination* ref, TaskStatus* task);
void compass_SetContactDestination(Destination* ref, ContactStatus* contact);

// set the destination waypoint from the server
//
void compass_SetDestinationWaypoint(int uid, Vec3 pos);

// ask the mapserver for waypoint updates
//
void compass_UpdateWaypoints(int hideUntilServerResponse);

// wipe the destination
//
void clearDestination( Destination * dest );

// wipe any destinations with given id #
void compass_ClearAllMatchingId( int id );

typedef struct miniMapIcon miniMapIcon;

// returns true if the given minimap icon is the current destination
//
int isDestination(Destination* ref, Destination *dest);
int destinationEquiv(Destination* lhs, Destination* rhs);    // are these equivalent?

// Mission Waypoint and Keydoor system
void MissionClearAll();
void MissionWaypointReceive(Packet* pak);
void MissionKeydoorReceive(Packet* pak);

// Notify compass with team-up teleporter status
void updateTrialStatus(TrialStatus status);

//------------------------------------------------------------------------
// Server-set waypoint
//------------------------------------------------------------------------

extern Destination serverDest;
extern StashTable serverDestList;
void ServerWaypointReceive(Packet* pak);

// compass drawing stuff
//--------------------------------------------------------------------------------------------

// draw the compass
int compassWindow();

extern Destination   activeTaskDest;    // Where is the active task? Should only be set to active task location.
extern Destination   waypointDest;        // User selected waypoint.  All other locations should be marked as a waypoint.
extern Destination** missionDest;        // List of important locations within a mission map.
extern Destination** keydoorDest;        // Where are the keydoors within a mission?

void compass_Draw3dBlip(Destination *dest, int color);
void compass_setTarget(void *foo);

//this stuff is for handling the script driven UIs
extern ScriptUIClientWidget** scriptUIWidgetList;
extern ScriptUIClientWidget** scriptUIKarmaWidgetList;
extern int scriptUIUpdated;
/************************************************/

// script support routines
//--------------------------------------------------------------------------------------------
void scriptUIAddToolTip(ScriptUIClientWidget* widget, double x, double y, double width, double height, char* message);

#define ARCHITECT_NAV_WD 100

#endif
