//
// compass.c - draws a compass on the screen
//
//-------------------------------------------------------------------

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/wininclude.h>

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiTeam.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "UI/uiContact.h"
#include "UI/uiTarget.h"
#include "UI/uiMission.h"
#include "UI/uiAutomap.h"
#include "UI/uiWindows.h"
#include "UI/uiToolTip.h"
#include "UI/uiCompass.h"
#include "UI/uiUtilGame.h"
#include "UI/uiGroupWindow.h"
#include "UI/uiContextMenu.h"
#include "UI/uiOptions.h"
#include "UI/uiScript.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "language/langClientUtil.h"
#include "storyarc/contactClient.h"
#include "gameComm/dooranimclient.h"
#include "entity/entVarUpdate.h"
#include "group/groupnetrecv.h"
#include "cmdparse/cmdcommon.h"
#include "graphics/gfxwindow.h"
#include "clientcomm/clientcomm.h"
#include "win/win_init.h"
#include <utilitieslib/utils/timing.h>
#include "player/player.h"
#include "graphics/camera.h"
#include "UI/uiBox.h"
#include "cmdparse/cmdgame.h"
#include "graphics/ttFontUtil.h"
#include "entity/entity.h"
#include "UI/uiMissionSummary.h"
#include "win/input.h"
#include "entity/teamCommon.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/mathutil.h>
#include "storyarc/zowieClient.h"

#include "arena/arenagame.h"
#include "gameData/sgraidClient.h"
#include "bases/bases.h"

#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/entPlayer.h"

#include "UI/uiAutomap.h"



static int destUID = 0;

Destination waypointDest = {0};            // Where is the user selected waypoint?
Destination activeTaskDest = {0};        // Where is the active task?
Destination** missionDest = 0;            // Where are the important locations within a mission?
Destination** keydoorDest = 0;            // Where are the keydoors within a mission?

ContextMenu *compassContext = 0;

extern int g_display_exit_button;

void MissionUpdateAll();

//this stuff is for handling the script driven UIs
ScriptUIClientWidget** scriptUIWidgetList;
ScriptUIClientWidget** scriptUIKarmaWidgetList;
int scriptUIUpdated;

static TrialStatus s_TrialStatus = trialStatus_None;

//------------------------------------------------------------------------------------------------------
// Destination functions ///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------

//
//
static void compass_UpdateWaypoint(Destination* ref, int hideUntilServerResponse)
{
    if(ref->type == ICON_PLAYER || ref->type == ICON_TEAMMATE || ref->type == ICON_LEAGUEMATE)
        return;

    if(++destUID >= (1 << 24) - 2)
        destUID = 1;

    ref->uid = destUID;
    ref->showWaypoint = !hideUntilServerResponse;

    START_INPUT_PACKET(pak, CLIENTINP_REQUEST_WAYPOINT);
    pktSendF32(pak, ref->orig_world_pos[0]);
    pktSendF32(pak, ref->orig_world_pos[1]);
    pktSendF32(pak, ref->orig_world_pos[2]);
    pktSendBitsPack(pak, 1, ref->uid);
    END_INPUT_PACKET
}

//
//
void compass_SetDestination(Destination* ref, Destination* newDest)
{
    memcpy(ref, newDest, sizeof(Destination));
    ref->navOn = TRUE;
    ref->showWaypoint = 1;
    copyVec3(ref->world_location[3], ref->orig_world_pos);

    compass_UpdateWaypoint(ref, 1);
}

// MAK - I'm changing things so that waypoint selection and task 
// selection are one and the same.  The id fields in Destination
// let you know what your selection is.
void compass_SetTaskDestination(Destination* ref, TaskStatus* task)
{
    // DGNOTE 7/22/2010
    // The second parameter to updateAutomapZowies(...) determines if the first is to be cached or not.  By setting it to 1 in these two calls we force
    // the cached value to be replaced
    updateAutomapZowies(task && task->isZowie ? task : NULL, SET_CACHED);

    // complete tasks aren't allowed to set locations any more
    if (task && task->hasLocation && !task->isComplete)
    {
        compass_SetDestination(ref, &task->location);
        //waypointDest.icon = atlasLoadTexture("map_enticon_mission_dot.tga");
        ref->icon = atlasLoadTexture("map_enticon_mission_a.tga");
        ref->iconB = atlasLoadTexture("map_enticon_mission_b.tga");
        ref->iconBInCompass = 1;
        ref->angle = PI/4;
    }
    else
        clearDestination(ref);

    if (task)
    {
        ref->type = ICON_MISSION;
        ref->dbid = task->ownerDbid;
        ref->context = task->context;
        ref->subhandle = task->subhandle;
    }
}

void compass_SetContactDestination(Destination* ref, ContactStatus* contact)
{
    if (contact && contact->hasLocation)
    {
        compass_SetDestination(ref, &contact->location);
    }
    else
        clearDestination(ref);

    if (contact)
    {
        ref->handle = contact->handle;
        assignContactIcon(contact);
    }
}

//
//
void compass_SetDestinationWaypoint(int uid, Vec3 pos)
{
    if(waypointDest.uid == uid)
    {
        copyVec3(pos, waypointDest.world_location[3]);
        waypointDest.showWaypoint = 1;
    }
    else if(activeTaskDest.uid == uid)
    {
        copyVec3(pos, activeTaskDest.world_location[3]);
        activeTaskDest.showWaypoint = 1;
    }
}

//
//
void compass_UpdateWaypoints(int hideUntilServerResponse)
{
    if(waypointDest.uid)
    {
        compass_UpdateWaypoint(&waypointDest, hideUntilServerResponse);
    }

    if(activeTaskDest.uid)
    {
        compass_UpdateWaypoint(&activeTaskDest, hideUntilServerResponse);
    }
    MissionUpdateAll();
}

//
//
void clearDestination( Destination * dest )
{
    memset( dest, 0, sizeof(Destination) );
}

void compass_ClearAllMatchingId( int id )
{
    if (activeTaskDest.id == id)
        clearDestination(&activeTaskDest);
    if (waypointDest.id == id)
        clearDestination(&waypointDest);
}

// equivalent is only really defined for missions and contacts right now
int destinationEquiv(Destination* lhs, Destination* rhs)
{
    return    (lhs->type == ICON_MISSION &&
            lhs->type == rhs->type &&
            lhs->dbid == rhs->dbid &&
            lhs->context == rhs->context &&
            lhs->subhandle == rhs->subhandle) ||

            (DestinationIsAContact(lhs)
            && lhs->type == rhs->type &&
            lhs->handle && lhs->handle == rhs->handle) ||

             (memcmp(lhs, rhs, sizeof(*lhs)) == 0);
}

//
//
int isDestination(Destination* ref, Destination *dest)
{
    if( !ref->name[0] ) // not set
        return FALSE;

    if (dest->type == ref->type && dest->id == ref->id)
        return TRUE;

    return FALSE;
}

//
//
void checkNavigation()
{
    // De-select our mission destination once we're on the mission map.
    if( activeTaskDest.type == ICON_MISSION && game_state.mission_map)
        activeTaskDest.navOn = 0;

    // De-select the gate destination once we're on the map the gate leads to.
    if( waypointDest.type == ICON_GATE && stricmp( waypointDest.name, gMapName ) == 0 )
        clearDestination( &waypointDest );

    // De-select the teammate destination of the teammate is not on the team anymore.
    if( waypointDest.type == ICON_TEAMMATE )
    {
        int i;
        Entity *e = playerPtr();
        int foundPlayer = FALSE;

        if(e && e->teamup)
        {
            for(i = 0; i < e->teamup->members.count; i++)
            {
                if(e->teamup->members.ids[i] == waypointDest.id)
                {
                    if(e->teamup->members.onMapserver[i])
                    {
                        Entity *mate = entFromDbId( e->teamup->members.ids[i] );
                        copyMat4( ENTMAT(mate), waypointDest.world_location );
                        foundPlayer = TRUE;
                    }
                }
            }
        }

        if( !foundPlayer )
            clearDestination( &waypointDest );
    }

    // De-select the leaguemate destination of the leaguemate is not on the league anymore.
    if( waypointDest.type == ICON_LEAGUEMATE )
    {
        int i;
        Entity *e = playerPtr();
        int foundPlayer = FALSE;

        if(e && e->league)
        {
            for(i = 0; i < e->league->members.count; i++)
            {
                if(e->league->members.ids[i] == waypointDest.id)
                {
                    if(e->league->members.onMapserver[i])
                    {
                        Entity *mate = entFromDbId( e->league->members.ids[i] );
                        copyMat4( ENTMAT(mate), waypointDest.world_location );
                        foundPlayer = TRUE;
                    }
                }
            }
        }

        if( !foundPlayer )
            clearDestination( &waypointDest );
    }
}

void updateTrialStatus(TrialStatus status)
{
    s_TrialStatus = status;
}

#ifndef TEST_CLIENT // Put all graphics-related compass functions below this point =)

#include "formatter/smf_util.h"

static TextAttribs s_taCompassTask =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};
 // Put all graphics-related compass functions below this point =)

//-----------------------------------------------------------------------------------------------
// Context Menu Functions ///////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------

void compass_setTarget(void *foo)
{
    int i;
    Entity *e = playerPtr();

    if( current_target )
    {
        if (e->teamup)
        {
            for( i = 0; i < e->teamup->members.count; i++ )
            {
                if( e->teamup->members.ids[i] == current_target->db_id )
                {
                    createZoneIcon(&waypointDest, 0, ICON_TEAMMATE, ENTMAT(current_target), 0, i, current_target);
                    processIcons( &waypointDest, 1, TRUE );
                    waypointDest.navOn = 1;
                    waypointDest.showWaypoint = 1;
                    return;
                }
            }
        }
        if (e->league)
        {
            for( i = 0; i < e->league->members.count; i++ )
            {
                if( e->league->members.ids[i] == current_target->db_id )
                {
                    createZoneIcon(&waypointDest, 0, ICON_LEAGUEMATE, ENTMAT(current_target), 0, i, current_target);
                    processIcons( &waypointDest, 1, TRUE );
                    waypointDest.navOn = 1;
                    waypointDest.showWaypoint = 1;
                    return;
                }
            }
        }
    }
}


//--------------------------------------------------------------------------------------------------------
// Compass functions /////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------
#define    FEET_IN_A_YARD    ( 3.f )
#define    FEET_IN_A_MILE    ( 5280.f )
#define FEET_IN_A_METER ( 3.281f )

#define COMPASS_SHRINK_DIST    .45f // where to start shrinking the destination icon
#define COMPASS_RANGE        .50f // where to pint the destination icon

//
//
static float compass_CalcDiff( float plyr_yaw, float x, float z )
{
     F32    dest_yaw, temp_yaw;

    dest_yaw = atan2( x, z );

    // keep yaw between 0 and 2PI
    if( dest_yaw < 0 )
        dest_yaw += 2*PI;
    if( dest_yaw > 2*PI )
        dest_yaw -= 2*PI;

    temp_yaw = plyr_yaw;
    if( temp_yaw < 0 )
        temp_yaw += 2*PI;
    if( temp_yaw > 2*PI )
        temp_yaw -= 2*PI;

    return ((temp_yaw) - (dest_yaw))/(2*PI);
}

//
//
static void compass_DisplayDestText( char * map, Vec3 loc, float x, float y, float z, int color )
{
    char *distance;

    // see if its on the same map as us
      if( stricmp(map, gMapName ) == 0 )
    {
        F32        dist = ( float ) sqrt( loc[0]*loc[0] + loc[1]*loc[1] + loc[2]*loc[2] );

        if( !getCurrentLocale() )
        {
             if( dist < 100 )
                distance = textStd( "FeetString", (int)dist );
            else if ( dist < ( FEET_IN_A_MILE / 2 ) )
                distance = textStd( "YardsString", (int)(dist / FEET_IN_A_YARD) );
            else
                distance = textStd( "MilesString", (dist / FEET_IN_A_MILE ) );
        }
        else
        {
            // convert feet to meters
            dist /= FEET_IN_A_METER;

            if( dist < 1000 )
                distance = textStd( "MetersString", dist );
            else
                distance = textStd( "KiloMetersString", dist/1000 );
        }
    }
    else // destination is not on our map
    {     // tell what map it is on (if we know that)
        if ( strlen(map) > 0 )
            distance = textStd( "GoToMapName", map );
        else
            distance = textStd( "DestinationNotOnMap", map );
    }

    // print the distance to target
    font( &game_9 );
    font_color( color, color );

    cprnt( x, y, z+1, 1.f, 1.f, distance );
}

//
//
static void compass_DrawBlip( AtlasTex *icon, AtlasTex *iconB, float diff, float compassWd, float x, float y, float z, float wd, float ht, float scale, int clr, int clrB, float rot )
{
    float xPos; // actual x location
    float dsc, sc = 1.f;
    int height;

    // keep destination_diff centered
    if( diff > .5f )
        diff -= 1.f;
    if( diff < -.5f )
        diff += 1.f;

    // keep blip in bounds
    if ( diff > COMPASS_RANGE )
    {
        diff    = COMPASS_RANGE;
        sc        = .5f;
    }
    else if ( diff < -COMPASS_RANGE)
    {
        diff    = -COMPASS_RANGE;
        sc        = .5f;
    }
    // shrink blip when near the edges
    else if ( diff > COMPASS_SHRINK_DIST )
    {
        sc = .5f + .5f*(COMPASS_RANGE - diff)/(COMPASS_RANGE-COMPASS_SHRINK_DIST);
    }
    else if ( diff < -COMPASS_SHRINK_DIST)
    {
        sc = .5f + .5*(COMPASS_RANGE + diff)/(COMPASS_RANGE-COMPASS_SHRINK_DIST);
    }

    xPos = x + wd/2 - diff*(compassWd-1); // get x position now

    // scale to fit in compass border
    height = iconB? iconB->height: icon->height;
    dsc = ( ht-(2*PIX3 - 2)*scale)/height * sc;

    if (iconB)
        display_rotated_sprite( iconB, xPos - (iconB->width/2)*dsc, y + ht/2 - (iconB->height/2)*dsc, z+2, dsc, dsc, clrB, rot, 0);
    display_rotated_sprite( icon, xPos - (icon->width/2)*dsc, y + ht/2 - (icon->height/2)*dsc, z+2, dsc, dsc, clr, rot, 0);
}

void compass_Draw3dBlip(Destination *dest, int color)
{
    AtlasTex * blip = atlasLoadTexture("waypoint_marker.tga");
    Vec3    camToEnt;
    float    scale;
    float   minScale = 0;
    float   maxScale = 255;
    Entity *e = playerPtr();

    if( stricmp( dest->mapName, gMapName ) != 0 )
        return;

     scale = distance3(cam_info.cammat[3], dest->world_location[3]);

    if(scale > 25.f )
        scale = maxScale;
    else if( scale < 5.f )
        scale = minScale;
    else
            scale = ((scale-5.f)/20.0f) * (maxScale - minScale);

    if(scale > maxScale)
        scale = maxScale;
    else if(scale < minScale)
        scale = minScale;

     subVec3(dest->world_location[3], cam_info.cammat[3], camToEnt);

    // MS: This is backwards because the cammat is backwards, will fix later.
    if(dotVec3(cam_info.cammat[2], camToEnt) < 0)
    {
        int  w,h;
        Mat4 loc = {0};
        Mat4 screen = {0};
        Vec3 tmp = {0};
        Vec2 pix = {0};

         copyVec3( dest->world_location[3], loc[3] );
        mulMat4(cam_info.viewmat, loc, screen);
        copyVec3(screen[3],tmp);
         gfxWindowScreenPos( tmp, pix );

        windowSize(&w,&h);
        subVec3( dest->world_location[3], ENTPOS(e), tmp );

           display_sprite( blip, pix[0] - blip->width / 2, h - pix[1] - blip->height, 30.f, 1.0, 1.0, (color + (int)scale));
        compass_DisplayDestText( dest->mapName, tmp, pix[0], h - pix[1] - blip->height - 4, 30.f, (color + (int)scale) );
    }
}

//--------------------------------------------------------------------------------------------------------
// Compass Master /////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------
//
//
static void drawDestination(UIBox box, F32 sc, F32 z, F32 currentYaw, const Vec3 currentPos, Destination* dest)
{
    F32    diff = -100.f;
    static AtlasTex    *compass;
    static int texBindInit=0;
    if (!texBindInit) {
        texBindInit = 1;
        compass = atlasLoadTexture( "compass_ticks.tga" );
    }

    if(dest->navOn && dest->showWaypoint)
    {
        Vec3 tmp;
        subVec3(dest->world_location[3], currentPos, tmp);

        // find angle to point at destination
        if(stricmp(gMapName, dest->mapName) == 0)
            diff = compass_CalcDiff(currentYaw, tmp[0], tmp[2]);
    }
    if(diff != -100.f) // if the diff has been set, then we have a destination to point to
    {
        TaskUpdateColor(dest);
        compass_DrawBlip(dest->icon, dest->iconBInCompass? dest->iconB: NULL,
            diff, compass->width*sc/2, box.x, box.y, z+1, box.width, box.height, sc,
            dest->color? dest->color: CLR_WHITE, dest->colorB? dest->colorB: CLR_WHITE, dest->angle);
    }
}

static void drawCompass( float x, float y, float z, float wd, float ht, float scale )
{
      Entity        *e = playerPtr();

    float    vec, xsc;

    char *s = gMapName;

    int i;

    Vec3    plyr_pyr;
    F32        plyr_yaw;
    F32        destination_diff    = -100.f;
    F32        teamleader_diff        = -100.f;
    F32        sidekick_diff        = -100.f;
    UIBox    compassBox;
    static AtlasTex        *compass;
    static AtlasTex        *leaderIcon;
    static int texBindInit=0;
    if (!texBindInit) {
        texBindInit = 1;
        compass = atlasLoadTexture( "compass_ticks.tga" );
        leaderIcon = atlasLoadTexture( "MissionPicker_icon_star.tga" );
    }

    xsc = 0.5f;

    if(!e)
        return;

    uiBoxDefine(&compassBox, x, y, wd, ht);

    // Display compass ticks
    //    get the players yaw
    getMat3YPR(    ENTMAT(e), ( float * )&plyr_pyr );
    plyr_yaw = plyr_pyr[1];

    // keep vec positive between 0 and 1
    if( plyr_yaw > 0 )
        vec = plyr_yaw / ( 2.f * PI );
    else
        vec = (( 2.f * PI ) + plyr_yaw ) / ( 2.f * PI );

    // left
    display_sprite_UV_4Color( compass, x + wd/2 - (compass->width/4)*scale, y + PIX3, z-1, xsc/3 * scale, scale, 0xffffff00, 0xffffffff, 0xffffffff, 0xffffff00,
        vec*( xsc ), 0.f, vec*(xsc) + xsc/3, 1.f );
    // middle
    display_spriteUV( compass, x + wd/2 - (compass->width/4)*scale + compass->width*(xsc/3)*scale, y + PIX3, z-1, xsc/3 * scale, scale, 0xffffffff,
        vec*(xsc) + xsc/3, 0.f, xsc/3 + vec*( xsc ) + xsc/3, 1.f );
    // right
    display_sprite_UV_4Color( compass, x + wd/2 - (compass->width/4)*scale  + compass->width*2*(xsc/3)*scale, y + PIX3, z-1, xsc/3 * scale, scale, 0xffffffff, 0xffffff00, 0xffffff00, 0xffffffff,
        vec*( xsc ) + 2*xsc/3, 0.f, xsc/3 + vec*( xsc ) + 2*xsc/3, 1.f );

    // De-select destination if we've reached it.
    checkNavigation();

    // Draw selected destination icon
    drawDestination(compassBox, scale, z, plyr_yaw, ENTPOS(e), &activeTaskDest);
    if (!destinationEquiv(&activeTaskDest, &waypointDest))
         drawDestination(compassBox, scale, z, plyr_yaw, ENTPOS(e), &waypointDest);

    // Draw the keydoor icon if it is selected
    for (i = 0; i < eaSize(&keydoorDest); i++)
        if (keydoorDest[i]->navOn)
            drawDestination(compassBox, scale, z, plyr_yaw, ENTPOS(e), keydoorDest[i]);

    // Draw leader destination icon.
    if( e->teamup_id && e->teamup && e->teamup->members.leader != e->db_id )
    {
        int i;
        for( i = 0; i < e->teamup->members.count; i++ )
        {
            if( e->teamup->members.ids[i] == e->teamup->members.leader && e->teamup->members.onMapserver[i] )
            {
                Entity *leader = entFromDbId( e->teamup->members.leader );

                if( leader )
                {
                    Vec3 tmp;
                    subVec3( ENTPOS(leader), ENTPOS(e), tmp );

                    // find angle to point at destination
                    teamleader_diff = compass_CalcDiff( plyr_yaw, tmp[0], tmp[2] );
                }
            }
        }
    }
    if( teamleader_diff != -100 ) // if the destination_diff has been set, then we have a destination to point to
        compass_DrawBlip( leaderIcon, NULL, teamleader_diff, compass->width*scale/2, x, y, z+1, wd, ht, scale, CLR_YELLOW, CLR_YELLOW, 0);
}

//--------------------------------------------------------------------------------------------------------
// Context Menu Functions ////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------

static int compasscm_isMapOpen( void * not )
{
    if( window_getMode(WDW_MAP) == WINDOW_DISPLAYING )
    {
        if( *(int*)not )
            return CM_AVAILABLE;
        else
            return CM_HIDE;
    }
    else
    {
        if( *(int*)not )
            return CM_HIDE;
        else
            return CM_AVAILABLE;
    }
}

static void compasscm_OpenMap( void *close )
{
    if( *(int*)close )
        window_setMode( WDW_MAP, WINDOW_SHRINKING );
    else
        window_setMode( WDW_MAP, WINDOW_GROWING );
}

static int compasscm_isWaypoint( void *unused )
{
    if(waypointDest.navOn)
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

//------------------------------------------------------------------------
// Server-set waypoint
//------------------------------------------------------------------------

Destination serverDest = {0};

StashTable serverDestList = NULL;

void ServerWaypointReceive(Packet* pak)
{
    int                hasLocation = pktGetBits(pak, 1);
    int                id = pktGetBitsPack(pak, 1);
    int                setCompass = false;
    char            key[20];
    Destination        *dest;

    sprintf(key, "%i", id);

    if (serverDestList == NULL)
        serverDestList = stashTableCreateWithStringKeys(20, StashDeepCopyKeys | StashCaseSensitive );

    if (id > 0) {
        stashFindPointer( serverDestList, key, &dest );

        if (hasLocation)
        {
            if (dest == NULL) {
                dest = malloc(sizeof(Destination));
                memset(dest, 0, sizeof(Destination));
                stashAddPointer(serverDestList, key, dest, false);
            }

            dest->navOn = 1;
            dest->type = ICON_SERVER;
            dest->id = -id;
            setCompass = pktGetBits(pak, 1);
            dest->pulseBIcon = pktGetBits(pak, 1);
            if (pktGetBits(pak, 1))
                dest->icon = atlasLoadTexture(pktGetString(pak));
            if (pktGetBits(pak, 1))
                dest->iconB = atlasLoadTexture(pktGetString(pak));
            if (pktGetBits(pak, 1))
                strncpyt(dest->name, pktGetString(pak), ARRAY_SIZE(dest->name));
            if (pktGetBits(pak, 1)) {
                strncpyt(dest->mapName, pktGetString(pak), ARRAY_SIZE(dest->mapName));
                forwardSlashes(dest->mapName);
            }
            if (pktGetBits(pak, 1))
                dest->angle = pktGetF32(pak);

            dest->world_location[3][0] = pktGetF32(pak);
            dest->world_location[3][1] = pktGetF32(pak);
            dest->world_location[3][2] = pktGetF32(pak);

            // need to do this after the position has been updated
            if (setCompass || isDestination(&waypointDest, dest))
                compass_SetDestination(&waypointDest, dest);

        }
        else // we may have lost this location, make sure we don't have waypoint
        {
            if (dest) {
                compass_ClearAllMatchingId((dest->id));
                clearDestination(dest);
                stashRemovePointer(serverDestList, key, NULL);
                free(dest);
            }
        }
    } else {
        // remove all server waypoints for this player
        StashElement            waypoint;
        StashTableIterator    iter;

        stashGetIterator(serverDestList, &iter);
        while (stashGetNextElement(&iter, &waypoint))
        {
            Destination *dest = stashElementGetPointer(waypoint);
            if (dest)
            {
                compass_ClearAllMatchingId((dest->id));
                clearDestination(dest);
                stashRemovePointer(serverDestList, stashElementGetStringKey(waypoint), NULL);
                free(dest);
            }
        }

    }
}

void ServerWaypointSetByName(char *name)
{
    StashElement            waypoint;
    StashTableIterator        iter;

    stashGetIterator(serverDestList, &iter);
    while (stashGetNextElement(&iter, &waypoint))
    {
        Destination *dest = stashElementGetPointer(waypoint);
        if (dest)
        {
            if (dest->name && !stricmp(name, dest->name))
            {
                compass_SetDestination(&waypointDest, dest);
            }
        }
    }
}



//--------------------------------------------------------------------------------------------------------
// Compass Window ////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------
static ToolTip compassTip = {0};

#define MIN_OPACITY 77

// The exit button needs to persist even when the task is cleared
static int compass_showExitButton(float x, float y, float z, float wd, float ht, float *used_wd, float scale)
{
    static int exitMissionButton = 0;
    static int exitButtonUpdated = 0;
    static int exitMissionShow = 0;
    static int s_lastMap;
    TaskStatus* task = PlayerGetActiveTask();
    Entity* player = playerPtr();

    if (!game_state.mission_map || (s_lastMap != game_state.base_map_id) || eaSize(&g_base.rooms))
    {
        s_TrialStatus = trialStatus_None;
        exitMissionShow = exitButtonUpdated = 0;
    }
    else if (!exitButtonUpdated && task)
    {
        exitMissionButton = task->teleportOnComplete;
        exitButtonUpdated = 1;
        exitMissionShow = 0;
    }
    else if (!exitMissionShow && exitButtonUpdated && exitMissionButton && ((!task && player && player->teamup) || (task && task->isComplete)))
    {
        exitMissionShow = 1;
    }
    else if (s_TrialStatus != trialStatus_None)
    {
        exitMissionShow = 1;
        task = NULL;
    }

    s_lastMap = game_state.base_map_id;

    if (exitMissionShow)
    {
           if(D_MOUSEHIT == drawStdButton(x + wd - PIX3*2*scale - 18*scale, y + ht/2, z+10, 30 * scale, 30*scale, CLR_BLUE, "NavExitButton",scale, 0))
        {
            cmdParse("requestexitmission 0");
        }
        if (!task)
        {
            font(&game_12);
            font_color(CLR_WHITE, CLR_WHITE);
            if(s_TrialStatus == trialStatus_Failure)
                cprntEx(x+wd*0.3*scale-PIX2*scale, y + 45*scale, z, scale, scale, 0, "TrialFailed" );
            else if(s_TrialStatus == trialStatus_Success)
                cprntEx(x+wd*0.3*scale-PIX2*scale, y + 45*scale, z, scale, scale, 0, "TrialCompleted" );
            else
                cprntEx(x+wd*0.3*scale-PIX2*scale, y + 45*scale, z, scale, scale, 0, "MissionCompleted" );
            return 1;
        }
           *used_wd = 35*scale;
    }
    else
        *used_wd = 0;
    return 0;
}

// show text in compass window
int compass_DrawHeaderText( float x, float y, float z, float ht, float wd, float scale, Destination* dest, int active )
{
    TaskStatus* task;
    char *line1, *line2;
    char * estr;
    F32 info_button_wd=0, exit_button_wd=0;
    static SMFBlock smfBlock = {0};

    // let other systems intercede here
     if (compass_showExitButton(x, y, z, wd, ht, &exit_button_wd, scale) || (!scriptUIIsDetached && eaSize(&scriptUIWidgetList)))
        return 0;
    else
    {
        estrCreate(&estr);

        if( getArenaCompassString() )
        {
            line1 = getArenaCompassString();
            line2 =  getArenaInfoString();

            if (!line1 && dest->name[0])
                line1 = dest->name;
            if (!line2 && dest->mapName[0] && stricmp(gMapName, dest->mapName) != 0)
                line2 = dest->mapName;

            estrClear(&estr);
            estrConcatf(&estr, "<span align=center>%s</span>", textStd(line1));
            estrConcatStaticCharArray(&estr, "<br>");
            estrConcatf(&estr, "<span align=center>%s</span>", textStd(line2));

            if(g_display_exit_button)
            {
                if(D_MOUSEHIT == drawStdButton(x + wd - 25, y + ht + 3 - (60 - 20*scale)/2, z+5, 25 - PIX3*2*scale, 53 - 25*scale, CLR_BLUE, "NavScoreButton",scale, 0))
                {
                    window_setMode(WDW_ARENA_RESULT,WINDOW_GROWING);
                }
            }

            smf_SetFlags(&smfBlock, 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
        }
        else if( getSGRaidCompassString() )
        {
            line1 = getSGRaidCompassString();
            line2 = getSGRaidInfoString();

            if (!line1 && dest->name[0])
                line1 = dest->name;
            if (!line2 && dest->mapName[0] && stricmp(gMapName, dest->mapName) != 0)
                line2 = dest->mapName;

            estrClear(&estr);
            estrConcatf(&estr, "<span align=center>%s</span>", textStd(line1));
            estrConcatStaticCharArray(&estr, "<br>");
            estrConcatf(&estr, "<span align=center>%s</span>", textStd(line2));

            smf_SetFlags(&smfBlock, 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
        }
        else if(dest->type == ICON_MISSION )
        {
            task = TaskStatusGet(dest->dbid, dest->context, dest->subhandle);
             mission_displayCompass( x + wd/2, y + 40*scale, z+5, wd - exit_button_wd - PIX3*2*scale, ht, scale, task, active, &estr, &info_button_wd);

            smf_SetFlags(&smfBlock, 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);
        }
        else
        {
            if( dest->name[0] )
            {
                estrConcatf(&estr, "<span align=center>%s</span>", textStdEx(TRANSLATE_NOPREPEND, dest->name));
                if( dest->mapName[0] && stricmp(gMapName,dest->mapName)!=0 )
                    estrConcatf(&estr, "<scale .8><span align=center>%s</span></scale>", textStd(dest->mapName));
            }
        }

        smf_SetRawText(&smfBlock, estr, false);
        estrDestroy(&estr);

           return 15*scale + smf_Display(&smfBlock, x+(info_button_wd+exit_button_wd)+R10*scale, y+25*scale, z, wd-exit_button_wd*2-info_button_wd*2-2*R10*scale, 0, 0, 0, &s_taCompassTask, 0);
    }
}

/* Functions for handling the script driven UI stuff */

static void scriptUIShowIcon(float x, float y, float z,float mscale,char * filename,char * text,int hasItem)
{
    AtlasTex         *icon = atlasLoadTexture(filename);
    int color;
    float dist,max,scale;
    if (mouseActive()) {
        max=1;
    } else {
        int xp,yp;
        int xm,ym;
        xp = gMouseInpCur.x;
        yp = gMouseInpCur.y;
        xm = x+(icon->width*mscale)/2;
        ym = y+(icon->height*mscale)/2;
        dist=(xp-xm)*(xp-xm)+(yp-ym)*(yp-ym);
        max=2-(dist/(((icon->width*mscale)/2.0)*((icon->width*mscale)/2.0)));
    }
    scale=max;
    scale=(scale-1)*.5+1;
    if (scale<1)
        scale=1;
    if (hasItem)
        color=CLR_WHITE;
    else
        color=CLR_BLACK;
    if (scale>1.001)
        z+=5;
    scale*=mscale;
    display_sprite(icon,x-icon->width*(scale-mscale)/2.0,y-icon->height*(scale-mscale)/2.0,z,scale,scale,color);
    if (max>1) {
        font_color(CLR_PARAGON,CLR_PARAGON);
        font(&game_12);
        cprnt(x+icon->width*mscale/2,y+icon->height*mscale,z,mscale,mscale,"%s",text);
    }
}

void scriptUIAddToolTip(ScriptUIClientWidget* widget, double x, double y, double width, double height, char* message)
{
    CBox box;
    if (!message || message[0]=='\0')
        return;

    if (!widget->tooltip)
    {
        widget->tooltip = malloc(sizeof(ToolTip));
        memset(widget->tooltip, 0, sizeof(ToolTip));
    }
    BuildCBox(&box, x, y, width, height);
    setToolTip(widget->tooltip, &box, message, 0, MENU_GAME, WDW_COMPASS);
    addToolTip(widget->tooltip);
}

float scriptUIDrawTitleBar(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    int totalHeight=15;
    char* title = widget->varList[0];
    char* info = widget->varList[1];
    extern TaskStatus zoneEvent;

    if (draw)
    {
        float iscale = .7*scale;
        AtlasTex *ptex = atlasLoadTexture("map_enticon_mission_b.tga");
        UIBox buttonBounds;
        PointFloatXY buttonCenter;

        font_color(CLR_WHITE,CLR_WHITE);
        font(&game_12);

        buttonCenter.x = x+140*scale-str_wd(&game_12,scale,scale,"%s",title)/2-8*scale;
        buttonCenter.y = y+4*scale-1;

        uiBoxDefine(&buttonBounds, 0, 0, ptex->width*iscale, ptex->height*iscale);
        buttonBounds.x = buttonCenter.x - buttonBounds.width/2;
        buttonBounds.y = buttonCenter.y - buttonBounds.height/2;

        if(uiMouseCollision(&buttonBounds))
            iscale=.8*scale;

//        display_sprite(ptex, buttonCenter.x-(ptex->width*scale)/2, buttonCenter.y-(ptex->height*scale)/2, z, scale, scale, 0x0000f0ff);
        iscale*=1.25;
        display_rotated_sprite(ptex, buttonCenter.x-(ptex->width*iscale)/2, buttonCenter.y-(ptex->height*iscale)/2, z, iscale, iscale, 0x0000f0ff, PI/4, 0);
        iscale/=1.25;

        ptex = atlasLoadTexture("MissionPicker_icon_info_overlay.tga");
        display_sprite(ptex, buttonCenter.x-(ptex->width*iscale)/2, buttonCenter.y-(ptex->height*iscale)/2, z, iscale, iscale, CLR_WHITE);

        if(uiMouseUpHit(&buttonBounds, MS_LEFT)) {
                        zoneEvent.detailInvalid=0;
                        strcpy(zoneEvent.detail,info);
                        missionSummaryShow(-1,-1,-1);
        }
        cprnt( x+140*scale, y+totalHeight*scale/2.0 , z , scale, scale, "%s",title);
        scriptUIAddToolTip(widget,buttonCenter.x-(ptex->width*iscale)/2,buttonCenter.y-(ptex->height*iscale)/2,ptex->width,ptex->height,"missionInfoTip");
    }
    return totalHeight;
}

void scriptUIDrawBasicMeter(double x,double y,double z,double width,double filled,int color,int color2,double scale) {
    double height=8*scale;

    display_sprite(white_tex_atlas,x+1                    ,y+1,z,(width-1)*filled        /8.0,(height-2)    /8.0,color);
    //the unfilled part of the meter overlaps the filled part by one pixel to give it a nice soft edge
    if (color2)
    {
        display_sprite(white_tex_atlas,x+(width-1)*filled+1    ,y+1,z,(width-2)*(1-filled)    /8.0,(height-2)    /8.0, color2);
        // bevel
        display_sprite(white_tex_atlas,x+(width-1)*filled+1    ,y+1,z,(width-2)*(1-filled)        /8.0,(height-2)/32.0,0xffffff7f);
        display_sprite(white_tex_atlas,x+(width-1)*filled+1    ,y+1,z,(width-2)*(1-filled)        /8.0,(height-2)/16.0,0xffffff4f);
        display_sprite(white_tex_atlas,x+(width-1)*filled+1    ,y+height-(height-2)/2,z,(width-2)*(1-filled)/8.0,(height-2)/16.0,0x4f);
        display_sprite(white_tex_atlas,x+(width-1)*filled+1    ,y+height-(height-2)/4,z,(width-2)*(1-filled)/8.0,(height-2)/32.0,0x7f);
    }
    else
    {
        display_sprite(white_tex_atlas,x+(width-1)*filled+1    ,y+1,z,(width-2)*(1-filled)    /8.0,(height-2)    /8.0,0x7f);
    }

    display_sprite(white_tex_atlas,x        ,y            ,z,width    /8.0,1.0    /8.0,0xff);    // top
    display_sprite(white_tex_atlas,x        ,y+height-1    ,z,width    /8.0,1.0    /8.0,0xff);    // bottom
    display_sprite(white_tex_atlas,x        ,y            ,z,1.0        /8.0,height    /8.0,0xff);    // left edge
    display_sprite(white_tex_atlas,x+width-1,y            ,z,1.0        /8.0,height    /8.0,0xff); // right edge

    // bevel
    display_sprite(white_tex_atlas,x+1                ,y+1,z,(width-2)*filled        /8.0,(height-2)/32.0,0xffffff7f);
    display_sprite(white_tex_atlas,x+1                ,y+1,z,(width-2)*filled        /8.0,(height-2)/16.0,0xffffff4f);
    display_sprite(white_tex_atlas,x+1                ,y+height-(height-2)/2,z,(width-1)*filled/8.0,(height-2)/16.0,0x4f);
    display_sprite(white_tex_atlas,x+1                ,y+height-(height-2)/4,z,(width-1)*filled/8.0,(height-2)/32.0,0x7f);

    // display values over the meter
}

// [MAX] = max value; [MIN] = min value; [VAL] = current value
static char* meterTooltip(char* meterTip, int min, int max, int val)
{
    static char* tooltip = 0;
    char* last;
    char* tracker = meterTip;
    estrClear(&tooltip);
    while (tracker)
    {
        last = tracker;
        tracker = strchr(tracker, '{');
        if (tracker)
        {
            estrConcatFixedWidth(&tooltip, last, tracker-last);
            if (!strncmp(tracker, "{MAX}", 5))
            {
                estrConcatf(&tooltip, "%i", max);
                tracker += 5;
            }
            else if (!strncmp(tracker, "{MIN}", 5))
            {
                estrConcatf(&tooltip, "%i", min);
                tracker += 5;
            }
            else if (!strncmp(tracker, "{VAL}", 5))
            {
                estrConcatf(&tooltip, "%i", val);
                tracker += 5;
            }
            else // just a regular '{'
            {
                estrConcatChar(&tooltip, *tracker);
                tracker++;
            }

        }
        else
            estrConcatCharString(&tooltip, last);
    }
    return tooltip;
}

// Draws a script UI meter based
float scriptUIDrawMeter(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    static int widestText[3] = {0};
    static int widestTextCalculated = 0;
    int value = atoi(widget->varList[0]);
    char* name = widget->varList[1];
    int showValues = !(!(atoi(widget->varList[2])));
    int min = atoi(widget->varList[3]);
    int max = atoi(widget->varList[4]);
    double widthLeft = str_wd(&game_9, scale, scale, widget->varList[3]);
    double widthRight = str_wd(&game_9, scale, scale, widget->varList[4]);
    double valWidth = str_wd(&game_9, scale, scale, widget->varList[2]);
    double width = str_wd(&game_9, scale, scale, name);
    double meterWidth;

    // Calculate the appropriate width, based on max width so that all the meters line up
    if (!draw && widestTextCalculated)
        widestText[0] = widestText[1] = widestText[2] = widestTextCalculated = 0;
    widestText[0] = MAX(width, widestText[0]);
    widestText[1] = MAX(widthLeft, widestText[1]);
    widestText[2] = MAX(widthRight, widestText[2]);
     meterWidth = 280*scale - 2*margin - widestText[0] - (!showValues)*5*scale - showValues*(widestText[1]+widestText[2]);

    // Second pass actually draws the meters
    if (draw)
    {
        // Colors are in hex, parse them out
        int color1, color2;
        double percent;
        sscanf(widget->varList[5], "%x", &color1);
        sscanf(widget->varList[6], "%x", &color2);

        widestTextCalculated = 1;
        font_color(CLR_WHITE,CLR_WHITE);
        font(&game_9);
        prnt(x+margin, y+8*scale, z, scale, scale, name);
        if (showValues)
        {
            prnt(x+margin+widestText[0]+5*scale+widestText[1]-widthLeft, y+8*scale, z, scale, scale, widget->varList[3]);
//            prnt(x+margin+widestText[0]+meterWidth+widestText[2], y+8*scale, z, scale, scale, widget->varList[4]);
            prnt(x+margin+widestText[0]+5*scale+meterWidth+(widestText[2]/2), y+8*scale, z, scale, scale, widget->varList[4]);
        }
        if (value > max && max > min)
            value = max;
        if (value < min && min < max)
            value = min;
        percent = (double)(value-min)/(double)(max-min);
        if (color1)
            scriptUIDrawBasicMeter(x+margin+widestText[0]+7*scale+showValues*widestText[1], y-1, z, meterWidth, percent, color1, color2, scale);
        else
            scriptUIDrawBasicMeter(x+margin+widestText[0]+7*scale+showValues*widestText[1], y-1, z, meterWidth, percent, 0x2255ffff, 0x0, scale);
        scriptUIAddToolTip(widget,x+margin+widestText[0]+7*scale+showValues*widestText[1],y-2,meterWidth,11*scale,meterTooltip(widget->varList[7], min, max, value));
        if (showValues)
             prnt(x+margin+widestText[0]+7*scale+meterWidth*.5-.5*valWidth+widestText[1], y+8*scale, z, scale, scale, "%i", value);
    }
    return 11*scale;
}

float scriptUIDrawCollectItems(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    int i, n = eaSize(&widget->varList);
    int numItems = n/3;
    double width = 0;
    double height = 0;
    double spacing;
    UIBox buttonBounds;

    // Calculate height, width, and spacing
    for (i = 0; i < n; i += 3)
    {
        AtlasTex* image = atlasLoadTexture(widget->varList[i+2]);
        if (image->height*scale > height)
            height = image->height*scale;
        width += image->width*scale;
    }
    if (numItems > 1)
    {
        spacing = (280.0*scale-width-margin*2)/((float)numItems);
        x += spacing / 2.0f;
    }
    else
    {
        x += scale*146.0-width;
        spacing = 0;
    }

    if (draw)
    {
        x += margin;
        for (i = 0; i < n; i += 3)
        {
            int tokenOwned = atoi(widget->varList[i]);
            char* text = widget->varList[i+1];
            AtlasTex* image = atlasLoadTexture(widget->varList[i+2]);
            scriptUIShowIcon(x, y, z, scale, widget->varList[i+2], text, tokenOwned);

            uiBoxDefine(&buttonBounds, 0, 0, image->width*scale, image->height*scale);
            buttonBounds.x = x;
            buttonBounds.y = y;
            if(uiMouseUpHit(&buttonBounds, MS_LEFT)) {
                ServerWaypointSetByName(text);
//                printf("blah %s\n", text);
            }

            x += (image->width*scale + spacing);
        }
    }
    return height;
}

// Helper function that takes a number of seconds left and turns it into a string
char* calcTimerString(int seconds, int forceShowHours)
{
    static char timer[9];
    timer[0] = 0;
    // Put a limit on what we display xx:xx:xx
    seconds = MAX(seconds, 0);
    seconds = MIN(seconds, 359999);
    if (seconds >= 3600)
        strcatf(timer, "%i:", seconds/3600);
    else if (forceShowHours)
        strcat(timer, "0:");
    seconds %= 3600;
    strcatf(timer, "%d" , seconds/600);
    seconds %= 600;
    strcatf(timer, "%d:", seconds/60);
    seconds %= 60;
    strcatf(timer, "%d", seconds/10);
    seconds %= 10;
    strcatf(timer, "%d", seconds);
    return timer;
}

float scriptUIDrawTimer(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    if (draw)
    {
        int endTime = atoi(widget->varList[0]);
        char* timerText = widget->varList[1];
        char* tooltip = widget->varList[2];
        int timeLeft = endTime - timerSecondsSince2000WithDelta();
        char* timerString = calcTimerString(timeLeft, widget->showHours);
        int size = strlen(timerString);
        int width = str_wd(&game_9, scale, scale, timerString);

        // if negative, it should probably be a count up!
        if (timeLeft < 0)
        {
            timeLeft = timerSecondsSince2000WithDelta() - endTime;
            timerString = calcTimerString(timeLeft, widget->showHours);
        }

        // Draw the timer and add the tooltip
        font_color(CLR_WHITE,CLR_WHITE);
        font(&game_9);
        prnt(x+margin, y+8*scale, z, scale, scale, timerText);
        prnt(x+280*scale-margin-width, y+8*scale, z, scale, scale, timerString);
        scriptUIAddToolTip(widget,x+280*scale-margin-width, y-2, width, 10*scale, tooltip);
    }
    return 11*scale;
}

// Draws a List that formats like this
// -----------------------------------
// Element                         Value
float scriptUIDrawList(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    char* tooltip = widget->varList[0];
    int i, leftAligned = 1;
    float height = 0;

    for (i = 1; i < eaSize(&widget->varList); i += 2)
    {
        char* item = widget->varList[i];
        int color;
        sscanf(widget->varList[i+1], "%x", &color);
        font(&game_9);
        font_color(color, color);
        if (draw && leftAligned)
            prnt(x+margin, y+8*scale, z, scale, scale, item);
        if (draw && !leftAligned)
            prnt(x+280*scale-margin-str_wd(&game_9, scale, scale, item), y-4*scale, z, scale, scale, item);

        // Increase the height after drawing one on the left, the right print has this taken into account already
        if (leftAligned)
        {
            height += 12*scale;
            y += 12*scale;
        }
        leftAligned = !leftAligned;
    }
    if (draw)
        scriptUIAddToolTip(widget,x+margin, y-height-1, 280*scale-margin*2, height+2*scale, tooltip);

    return height;
}

// Draws a "Distance to" that formats like this
// -----------------------------------
// Element                         Value
float scriptUIDrawDistance(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    char *tooltip = widget->varList[0];
    float height = 0;
    float dist;
    char *item;
    int color;
    Vec3 targetPos;
    Entity *player = playerPtr();
    char *distance;

    item = widget->varList[1];
    sscanf(widget->varList[2], "%x", &color);
    font(&game_9);
    font_color(color, color);
    prnt(x+margin, y+8*scale, z, scale, scale, item);
    sscanf(widget->varList[3], "%f,%f,%f", &targetPos[0], &targetPos[1], &targetPos[2]);
    sscanf(widget->varList[4], "%x", &color);
    dist = player ? distance3(ENTPOS(player), targetPos) : 0.0f;


    if (getCurrentLocale())
    {
        // convert feet to meters
        dist /= FEET_IN_A_METER;

        if (dist < 1000)
        {
            distance = textStd("MetersString", dist);
        }
        else
        {
            distance = textStd("KiloMetersString", dist / 1000);
        }
    }
    else
    {
        if (dist < 100)
        {
            distance = textStd("FeetString", (int) dist);
        }
        else if (dist < FEET_IN_A_MILE / 2)
        {
            distance = textStd("YardsString", (int)(dist / FEET_IN_A_YARD));
        }
        else
        {
            distance = textStd("MilesString", (dist / FEET_IN_A_MILE));
        }
    }

    prnt(x+280*scale-margin-str_wd(&game_9, scale, scale, distance), y+8*scale, z, scale, scale, distance);

    // Increase the height
    height += 12*scale;
    y += 12*scale;

    if (draw)
        scriptUIAddToolTip(widget,x+margin, y-height-1, 280*scale-margin*2, height+2*scale, tooltip);

    return height;
}

// New Zone event widgets

// Draws a zone event script UI meter
float scriptUIDrawMeterEx(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float sc)
{
    char *name;
    int current;
    int target;
    char *color;
    char *tooltip;

    AtlasTex *default_L;
    AtlasTex *default_M;
    AtlasTex *default_R;
    AtlasTex *glow_L;
    AtlasTex *glow_M;
    AtlasTex *glow_R;
    AtlasTex *edge;
    char numbers[120];    // safe on anything up to and including a 64 bit machine to hold %d/%d

    name = widget->varList[0];
    current = atoi(widget->varList[2]);
    target = atoi(widget->varList[3]);
    color = widget->varList[4];
    tooltip = widget->varList[5];
    
    if (stricmp(color, "red") == 0)
    {
        default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Red_L");
        default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Red_Mid");
        default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Red_R");
        glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_L");
        glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_Mid");
        glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_R");
        edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_Edge");
    }
    else if (stricmp(color, "gold") == 0)
    {
        default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Gold_L");
        default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Gold_Mid");
        default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Gold_R");
        glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_L");
        glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_Mid");
        glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_R");
        edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_Edge");
    }
    else
    {
        default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_L");
        default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_Mid");
        default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_R");
        glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_L");
        glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_Mid");
        glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_R");
        edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_Edge");
    }

    if (draw)
    {
        display_sprite(glow_L, x + margin, y + 12 * sc, z, sc, sc, CLR_WHITE);
        display_sprite(glow_R, x + margin + 10 * sc, y + 12 * sc, z, sc, sc, CLR_WHITE);

        font(&game_9);
        font_color(CLR_WHITE, CLR_WHITE);

        prnt(x + margin + 20 * sc, y + 12 * sc, z, sc, sc, name);
        _snprintf(numbers, sizeof(numbers), "%d/%d", current, target);
        numbers[sizeof(numbers) - 1] = 0;
        prnt(x + 280 * sc - margin - str_wd(&game_9, sc, sc, numbers), y + 12 * sc, z, sc, sc, numbers);
    }

    return 22 * sc;
}

float scriptUIDrawText(ScriptUIClientWidget* widget, int draw, float x, float y, float z, float margin, float scale)
{
    char *text;
    char *format;
    char *tooltip;
    int centered;
    float tx;

    text = widget->varList[0];
    format = widget->varList[1];
    tooltip = widget->varList[2];
    centered = strnicmp(format, "center", 6 /* == strlen("center") */) == 0;

    font(&game_9);
    font_color(0xffffffff, 0xffffffff);
    if (draw)
    {
        if (centered)
        {
            tx = x + 280 * scale - str_wd(&game_9, scale, scale, text);
            if (tx < x + margin)
            {
                tx = x + margin;
            }
        }
        else
        {
            tx = x + margin;
        }
        prnt(tx, y + 8 * scale, z, scale, scale, text);
        if (tooltip[0])
        {
            scriptUIAddToolTip(widget, x + margin, y - 9 * scale, 280 * scale - margin * 2, 10 * scale, tooltip);
        }
    }

    return 12 * scale;
}

float scriptUIDraw(float x, float y, float z, float margin, int draw, float scale)
{
    int i, numWidgets = eaSize(&scriptUIWidgetList);
    int height = 1*scale;

    for (i = 0; i < numWidgets; i++)
    {
        ScriptUIClientWidget* widget = scriptUIWidgetList[i];
        if (widget->hidden)
            continue;
        switch (widget->type)
        {
            xcase MeterWidget:
                height += scriptUIDrawMeter(widget, draw, x, y + height, z, margin, scale);
            xcase TimerWidget:
                height += scriptUIDrawTimer(widget, draw, x, y + height, z, margin, scale);
            xcase CollectItemsWidget:
                height += scriptUIDrawCollectItems(widget, draw, x, y + height, z, margin, scale);
            xcase TitleWidget:
                height += scriptUIDrawTitleBar(widget, draw, x, y + height, z, margin, scale);
            xcase ListWidget:
                height += scriptUIDrawList(widget, draw, x, y + height, z, margin, scale);
            xcase DistanceWidget:
                height += scriptUIDrawDistance(widget, draw, x, y + height, z, margin, scale);
            xcase MeterExWidget:
                height += scriptUIDrawMeterEx(widget, draw, x, y + height, z, margin, scale);
            xcase TextMeterWidget:
                height += scriptUIDrawText(widget, draw, x, y + height, z, margin, scale);
            xcase ProgressWidget:
            xcase TextWidget:
                break;
        }            
    }
    return (height+5*scale);    //add a little buffer at the bottom
}

void MissionUpdateAll()
{
    int i, n = eaSize(&missionDest);
    int numKeydoors = eaSize(&keydoorDest);
    for (i = 0; i < n; i++)
        compass_UpdateWaypoint(missionDest[i], 1);
    for (i = 0; i < numKeydoors; i++)
    {
        int navOn = keydoorDest[i]->navOn;
        compass_UpdateWaypoint(keydoorDest[i], 1);
        keydoorDest[i]->showWaypoint = navOn;
    }
}

void MissionClearAll()
{
    int i, n = eaSize(&missionDest);
    int numKeydoors = eaSize(&keydoorDest);
    for (i = 0; i < n; i++)
        free(missionDest[i]);
    for (i = 0; i < numKeydoors; i++)
        free(keydoorDest[i]);
    eaDestroy(&missionDest);
    eaDestroy(&keydoorDest);
}

// Rrturns the mission location matching the id
static Destination* DestinationFind(Destination** destList, int id)
{
    int i;
    for (i = eaSize(&destList)-1; i >= 0; i--)
        if (destList[i]->id == id)
            return destList[i];
    return NULL;
}

#define WAYPOINT_TIMETHRESHOLD    3
// All waypoints received within the WAYPOINT_TIMETHRESHOLD since the most recent are treated as a group
// Because the waypoints are being sent when the objective is created, we are grouping them based on timestamp
// Within a group, priority is given to newly received waypoints, then closest
// If creationTime is not set, then we ignore it when finding closest
Destination* DestinationFindClosest(Destination** destList, U32 creationTime)
{
    int i, n = eaSize(&destList);
    U32 maxTime = creationTime + WAYPOINT_TIMETHRESHOLD;
    U32 minTime = creationTime - WAYPOINT_TIMETHRESHOLD;
    Destination* closeDest = NULL;
    float closest = -1;
    Entity* player = playerPtr();

    // If we don't have a player, then it doesnt matter, bail out
    if (!player)
        return NULL;

    // Search through the list and find the closest based on players distance and time(if specified)
    for (i = 0; i < n; i++)
    {
        U32 destTime = destList[i]->creationTime;
        float dist = distance3SquaredXZ(ENTPOS(player), destList[i]->world_location[3]);
        if ((closest == -1 || dist < closest) && (!destTime || (destTime >= minTime && destTime <= maxTime)))
        {
            closest = dist;
            closeDest = destList[i];
        }
    }
    return closeDest;
}

void MissionWaypointReceive(Packet* pak)
{
    int newWaypoints = pktGetBits(pak, 1);
    Destination* closest;
    while (pktGetBits(pak, 1))
    {
        int id = pktGetBitsPack(pak, 1) + MISSION_WAYPOINT_LOCATION;
        if (newWaypoints)
        {
            static int lastReceived = 0;
            Destination* newDest = DestinationFind(missionDest, id);
            if (!newDest)
            {
                newDest = calloc(sizeof(Destination), 1);
                eaPush(&missionDest, newDest);
            }

            // We want to know the time so we know whether or not we should change waypoints
            newDest->creationTime = pktGetBitsAuto(pak);

            // Get the position from the server
            newDest->world_location[3][0] = pktGetF32(pak);
            newDest->world_location[3][1] = pktGetF32(pak);
            newDest->world_location[3][2] = pktGetF32(pak);
            copyVec3(newDest->world_location[3], newDest->orig_world_pos);

            // Destination stuff
            newDest->navOn = 1;
            newDest->showWaypoint = 1;
            newDest->iconBInCompass = 1;
            newDest->id = id;
            newDest->type = ICON_MISSION_WAYPOINT;
            newDest->icon = atlasLoadTexture("map_enticon_mission_a.tga");
            newDest->iconB = atlasLoadTexture("map_enticon_mission_b.tga");
            newDest->color = 0xffffffff;
            newDest->colorB = 0xff0000ff;
            strcpy(newDest->mapName, gMapName);
            compass_UpdateWaypoint(newDest, 1);

            // We prioritize which waypoint is next by the received
            closest = DestinationFindClosest(missionDest, newDest->creationTime);
            if (!closest)
                closest = newDest;
            compass_SetDestination(&waypointDest, closest);
        }
        else
        {
            // Clear out the old waypoint, remove it from the list
            int i;
            for (i = eaSize(&missionDest)-1; i >= 0; i--)
            {
                if (missionDest[i]->id == id)
                {
                    free(missionDest[i]);
                    eaRemove(&missionDest, i);
                }
            }
            compass_ClearAllMatchingId(id);
        }
    }

    // If we just deleted a waypoint, change our target to be the next closest waypoint to our current location
    if (!newWaypoints && (closest = DestinationFindClosest(missionDest, 0)))
        compass_SetDestination(&waypointDest, closest);
}

// returns 1 if any keydoor destination is shown on the nav
static void MissionKeydoorTurnOffAll(void)
{
    int i, n = eaSize(&keydoorDest);
    for (i = 0; i < n; i++)
        keydoorDest[i]->navOn = keydoorDest[i]->showWaypoint = 0;
}

void MissionKeydoorReceive(Packet* pak)
{
    int i, n = pktGetBitsPack(pak, 1);
    for (i = 0; i < n; i++)
    {
        int id = pktGetBitsPack(pak, 1) + MISSION_KEYDOOR_LOCATION;
        Destination* newDest = DestinationFind(keydoorDest, id);

        // We are getting a new door, read in its position and add it to our list
        if (pktGetBits(pak, 1))
        {
            if (!newDest)
            {
                newDest = calloc(sizeof(Destination), 1);
                eaPush(&keydoorDest, newDest);
            }

            newDest->world_location[3][0] = pktGetF32(pak);
            newDest->world_location[3][1] = pktGetF32(pak);
            newDest->world_location[3][2] = pktGetF32(pak);
            copyVec3(newDest->world_location[3], newDest->orig_world_pos);

            newDest->iconBInCompass = 1;
            newDest->id = id;
            newDest->type = ICON_MISSION_KEYDOOR;
            newDest->icon = atlasLoadTexture("map_enticon_mission_a.tga");
            newDest->iconB = atlasLoadTexture("map_enticon_mission_b.tga");
            newDest->color = 0xffffffff;
            newDest->colorB = 0xffff00ff;
            strcpy(newDest->mapName, gMapName);
            compass_UpdateWaypoint(newDest, 1);

            // Always display the most recently received keydoor
            MissionKeydoorTurnOffAll();
            newDest->showWaypoint = newDest->navOn = 1;
        }
        else // Turn off the door
        {
            if (newDest)
                newDest->navOn = newDest->showWaypoint = 0;
        }
    }
}

static void compass_drawArchitect( F32 x, F32 y, F32 z, F32 ht, F32 sc )
{
    Entity * e = playerPtr();

 }

/******************************w***********************/

// Resizing of the compass window by the objective string
static int objectiveStringResize = 0;
static int objectiveStringSize = 0;

// controls the compass
//
int compassWindow()
{
    float x, y, z, wd, ht, scale;
     int color, bcolor;
    CBox box;
    Entity * e = playerPtr();
    static float timer = 0;
       static AtlasTex * top;
    static AtlasTex * bot;
    static AtlasTex * left_arrow;
     static AtlasTex * right_arrow;
    static int texBindInit=0;
    F32 default_wd = 280.f, default_ht = 60.f;

    if (!e)
        return 0;

    if (!texBindInit)
    {
        texBindInit = 1;
        top = atlasLoadTexture("compass_facing_bottom.tga");
        bot = atlasLoadTexture("compass_facing_top.tga");
        left_arrow            = atlasLoadTexture( "teamup_arrow_contract.tga" );
        right_arrow           = atlasLoadTexture( "teamup_arrow_expand.tga" );
    }

    addToolTip( &compassTip );
      if ( !window_getDims( WDW_COMPASS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    // Compass resizing for SCriptUI and objective string and Architect
    if (window_getMode(WDW_COMPASS) == WINDOW_DISPLAYING)
     {
        // DGNOTE 10/13/2009 - Script UI now trumps AE UI.
        // ScriptUI resizes the window its own special way
        if (!scriptUIIsDetached && eaSize(&scriptUIWidgetList))
        {
            if (scriptUIUpdated)
            {
                 float scriptUIHeight = scriptUIDraw(x, y+30*scale, z, 15*scale, 0, scale) - 30*scale;
                ht = 60*scale + scriptUIHeight;
                window_setDims(WDW_COMPASS, x, y, wd, ht);
                scriptUIUpdated = 0;
            }
            scriptUIDraw(x, y+30*scale, z, 15*scale, 1, scale);
        }
        else if( e->onArchitect ) //
        {
            ht = MAX( default_ht*scale,  objectiveStringResize + 50*scale);
            window_setDims(WDW_COMPASS, -1, -1, -1, ht);

               if( drawMMButton( "ArchitectOptions", 0, 0, x + wd/2, y + ht - 18*scale, z, 180*scale, .75*scale, MMBUTTON_SMALL, 0 ) )
                window_openClose( WDW_MISSIONREVIEW );
        }
        else // ScriptUI Not Running
        {
            // If there are no scriptUI widgets, resize to normal
            if (scriptUIUpdated)
            {
                if (!scriptUIIsDetached)
                {
                    ht = default_ht*scale;
                    window_setDims(WDW_COMPASS, x, y, wd, ht);
                }
                scriptUIUpdated = 0;
            }
            // Handle objective string resizing
            else if (objectiveStringResize)
            {
                ht = MAX( default_ht*scale,  objectiveStringResize + 30*scale);
                window_setDims(WDW_COMPASS, x, y, wd, ht);
                objectiveStringSize = objectiveStringResize;
            }
            // No size, then resize to normal
            else if (!objectiveStringResize && (ht != default_ht*scale || wd != default_wd*scale) )
            {
                ht = default_ht*scale;
                wd = default_wd*scale;
                window_setDims(WDW_COMPASS, x, y, wd, ht);
            }
            // If there is a size and we haven't resized, fix it
            else if (objectiveStringResize && ht == default_ht*scale)
            {
                ht = 60*scale + objectiveStringResize;
                window_setDims(WDW_COMPASS, x, y, wd, ht);
            }
        }
    }


    if( !compassContext )
    {
        compassContext = contextMenu_Create( NULL );
        contextMenu_addTitle( compassContext, "CMCompass" );
        contextMenu_addCode( compassContext, compasscm_isMapOpen, &boolOffOn[0], compasscm_OpenMap, &boolOffOn[0], "CMViewMap", 0 );
        contextMenu_addCode( compassContext, compasscm_isMapOpen, &boolOffOn[1], compasscm_OpenMap, &boolOffOn[1], "CMCloseMap", 0 );
        contextMenu_addCode( compassContext, compasscm_isWaypoint, 0,  clearDestination, (void*)&waypointDest, "CMClearWaypoint", 0 );
        contextMenu_addCode( compassContext, alwaysVisible, 0, 0, 0, "CMHelp", 0 );
    }

     BuildCBox( &box, x, y, wd, ht );
    setToolTip( &compassTip, &box, "compassTip", NULL, MENU_GAME, WDW_COMPASS );

    if( mouseClickHit( &box, MS_RIGHT ) )
    {
        int xp, yp;
        rightClickCoords( &xp, &yp );
        contextMenu_set( compassContext, xp, yp, NULL );
    }

    // draw the frame and arrows
      if( window_getMode( WDW_COMPASS) == WINDOW_DISPLAYING )
           display_sprite( bot, x + wd/2 - (top->width-4)*scale/2, y + (PIX3 - top->height + 4)*scale, z+13, scale, scale, (color | 0xff) );

    // draw the internal compass
       drawCompass( x, y, z+5, wd, 27*scale, scale );

     drawFrame( PIX3, R22, x, y, z-1, wd, ht, scale, color, bcolor );

    set_scissor( 1 );
    scissor_dims( x + PIX3*scale, y+PIX3*scale, wd-2*PIX3*scale, ht-2*PIX3*scale );

    //the mission text
    if (activeTaskDest.type == ICON_MISSION)
        objectiveStringResize = compass_DrawHeaderText( x, y, z, ht, wd, scale, &activeTaskDest, 1 );
    else
        objectiveStringResize = compass_DrawHeaderText( x, y, z, ht, wd, scale, &waypointDest, 0 );

    set_scissor( 0 );

    // compass frame

    return 0;
}

#endif
