//
//
// cursor.c - basically a massive function to handle everything the cursor controls
//------------------------------------------------------------------------------------

#include "win/input.h"
#include "UI/uiStoredSalvage.h"
#include "player/player.h"
#include "UI/uiUtil.h"
#include "win/win_init.h"
#include "UI/uiGame.h"
#include "UI/uiWindows.h"
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include "clientcomm/clientcomm.h"
#include "win/win_cursor.h"
#include "UI/uiTray.h"
#include "entity/entclient.h"
#include "UI/uiConsole.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "UI/uiInput.h"
#include "UI/uiTarget.h"
#include "UI/uiTeam.h"
#include "UI/uiGift.h"
#include "UI/uiPet.h"
#include "UI/uiDialog.h"

#include "entity/powers.h"
#include "entity/character_base.h"
#include "entity/character_animfx_client.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/FX/fx.h"
#include "UI/uiInspiration.h"
#include "UI/uiContextMenu.h"
#include "group/grouputil.h"    // for groupDefFindPropertyValue
#include <utilitieslib/components/earray.h>
#include "player/pmotion.h"
#include "storyarc/contactClient.h"
#include "graphics/textureatlas.h"
#include "language/langClientUtil.h"
#include "entity/entDebug.h"
#include <utilitieslib/utils/timing.h>
#include "gameComm/dooranimclient.h"
#include "graphics/gfxwindow.h"
#include "graphics/camera.h"
#include "player/playerSticky.h"
#include "graphics/font.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "gameComm/itemselect.h"
#include "bases/baseedit.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiCursor.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "uiBaseStorage.h"
#include "UI/uiOptions.h"
#include "arena/arenagame.h"
#include "entity/character_target.h"

#define BASIC_SELECTION_DISTANCE (300.0f)
#define TELEPORT_SELECTION_DISTANCE (610.0f)
#define PLACE_ENT_SELECTION_DISTANCE (10000.0f)
#define CLICK_TO_MOVE_INTERSECT_DISTANCE (15000.0f)
#define DOUBLE_CLICK_TIME (0.5f)

Cursor    cursor;
Vec3    current_location_target;
Entity    *current_target;
Entity    *current_target_under_mouse;
EntityRef current_pet;
F32 gClickToMoveDist = 150.0f;
int gClickToMoveButton = 0;

int useClickToMove()
{
    int use = optionGet(kUO_ClickToMove);
    if (gClickToMoveButton)
        return !use;
    return use;
}

static StashTable cursorHashTable;
static StashTable cursorScaleHashTable;

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

// Notes on cursor:
// every frame anything that want to change the cursor will check for its special conditions (whatever they may be)
// if it sets the cursor, the cursor will only stay set for that one frame (it resets every frame) so if nothing
// decides to change the cursor, it will just draw the default cursor.  Trying to keep track of entry-exit states
// for wildy differnt cursor conditions became way too much of a hassle

void setCursor( char *base, char *overlay,  int software, int hotX, int hotY )
{
    if( base )
        cursor.base = atlasLoadTexture( base );
     else
        cursor.base = atlasLoadTexture( "cursor_select.tga" );

    if( overlay )
        cursor.overlay = atlasLoadTexture( overlay );
    else
        cursor.overlay = NULL;

    cursor.software = software;
    cursor.hotX = hotX;
    cursor.hotY = hotY;

}

static void destroyHCursor(HCURSOR cursor)
{
    DestroyIcon(cursor);
}

void destroyCursorHashTable()
{
    if (cursorHashTable)
    {
        stashTableDestroyEx(cursorHashTable, NULL, destroyHCursor);
        cursorHashTable = 0;
    }

    if (cursorScaleHashTable)
    {
        stashTableDestroy(cursorScaleHashTable);
        cursorScaleHashTable = 0;
    }
}

#define    TIME_FOR_ZOOM_OUT    ( 4 )
#define    TIME_FOR_ZOOM_DOWN    ( 10 )
#define DRAG_ICON_SC        0.8f

// returns 1 if def is a visitlocation, but not one we care about
static int visitLocationIgnored(GroupDef* def)
{
    char* groupName;
    int i;

     if( groupDefFindPropertyValue(def, "MinimapTextureName") )
        return 1;

    if(groupDefFindPropertyValue(def, "Kiosk")==NULL
         && groupDefFindPropertyValue(def, "Plaque")==NULL
        && groupDefFindPropertyValue(def, "ArenaKiosk")==NULL
        && groupDefFindPropertyValue(def, "Interact")==NULL
        && (groupName=groupDefFindPropertyValue(def, "VisitLocation"))!=NULL)
    {
        // Is this location one of the locations I want to visit?
        for(i = eaSize(&visitLocations)-1; i >= 0; i--)
        {
            if(stricmp(visitLocations[i], groupName) == 0)
                return 0;
        }
        // otherwise, it's not a location I care about
        return 1;
    }
    return 0; // not a VisitLocation or is a VisitLocation AND a plaque
}

static int isInteractLocation(GroupDef* def)
{
    char *doorval = groupDefFindPropertyValue( def, "Door" );
    if( groupDefFindPropertyValue(def, "Kiosk")
        || groupDefFindPropertyValue(def, "Plaque") 
        || (groupDefFindPropertyValue(def, "Zowie") && def->pulse_glow)    // Zowies are only considered interesting if their pulse_glow flag is set
        || groupDefFindPropertyValue(def, "Interact") 
        || groupDefFindPropertyValue(def, "ArenaKiosk") 
        || groupDefFindPropertyValue(def, "Workshop") 
        || groupDefFindPropertyValue(def, "Portal") 
        || (doorval && (stricmp(doorval,"Door") == 0 || stricmp(doorval,"ReturnDoor") == 0))
        || groupDefFindPropertyValue(def, "personal_storage")
        || groupDefFindPropertyValue(def, "ArchitectKiosk") )
    {
        return 1;
    }
    return 0;
}

void reportReachedVisited(GroupDef* def, Vec3 coord, DefTracker *tracker)
{
    char* groupName;
    int i;

    if(!visitLocationIgnored(def)
        && (groupName=groupDefFindPropertyValue(def, "VisitLocation"))!=NULL)
    {
        // Is this location one of the locations I want to visit?
        for(i = eaSize(&visitLocations)-1; i >= 0; i--)
        {
            if(stricmp(visitLocations[i], groupName) == 0)
                break;
        }

        if(i<eaSize(&visitLocations))
        {
            START_INPUT_PACKET(pak, CLIENTINP_TASK_REACHED_VISIT_LOCATION);
            pktSendString(pak, groupName);
            pktSendF32(pak, coord[0]);
            pktSendF32(pak, coord[1]);
            pktSendF32(pak, coord[2]);
            END_INPUT_PACKET
        }
    }

    if((groupName = groupDefFindPropertyValue(def, "Workshop"))!=NULL)
    {
        START_INPUT_PACKET(pak, CLIENTINP_WORKSHOP_INTERACT);
        pktSendString(pak, groupName);
        pktSendBitsAuto(pak, tracker->ent_id);
        END_INPUT_PACKET
    }

    if((groupName = groupDefFindPropertyValue(def, "ArchitectKiosk"))!=NULL)
    {
        START_INPUT_PACKET(pak, CLIENTINP_ARCHITECTKIOSK_INTERACT);
        pktSendString(pak, groupName);
        pktSendF32(pak, coord[0]);
        pktSendF32(pak, coord[1]);
        pktSendF32(pak, coord[2]);
        END_INPUT_PACKET
    }

    if((groupName = groupDefFindPropertyValue(def, "Portal"))!=NULL)
    {
        START_INPUT_PACKET(pak, CLIENTINP_PORTAL_INTERACT);
        pktSendString(pak, groupName);
        END_INPUT_PACKET
    }

    if((groupName = groupDefFindPropertyValue(def, "Plaque"))!=NULL)
    {
        // CLIENTINP_READ_PLAQUE
        // string name of plaque
        // position:
        //   float vec3[0]
        //   float vec3[1]
        //   float vec3[2]

        START_INPUT_PACKET(pak, CLIENTINP_READ_PLAQUE);
        pktSendString(pak, groupName);
        pktSendF32(pak, coord[0]);
        pktSendF32(pak, coord[1]);
        pktSendF32(pak, coord[2]);
        END_INPUT_PACKET
    }

    // Zowies need special handling - we check the def's pulse_glow flag, and only allow
    // a click if the zowie is actually glowing.
    if((groupName = groupDefFindPropertyValue(def, "Zowie")) != NULL && def->pulse_glow)
    {
        // CLIENTINP_ZOWIE
        // string name of zowie
        // position:
        //   float vec3[0]
        //   float vec3[1]
        //   float vec3[2]

        START_INPUT_PACKET(pak, CLIENTINP_ZOWIE);
        pktSendString(pak, groupName);
        pktSendF32(pak, coord[0]);
        pktSendF32(pak, coord[1]);
        pktSendF32(pak, coord[2]);
        END_INPUT_PACKET
    }

    if((groupName = groupDefFindPropertyValue(def, "Kiosk"))!=NULL)
    {
        char achAnotherStaticString[256];
        strcpy(achAnotherStaticString, "kiosk ");
        strcat(achAnotherStaticString, groupName);
        strcat(achAnotherStaticString, " home");
        cmdParse(achAnotherStaticString);
    }

    if((groupName = groupDefFindPropertyValue(def, "Interact"))!=NULL)
    {
        int id = baseedit_GetDetailFromChild(tracker, NULL);
        if(id)
        {
            START_INPUT_PACKET(pak, CLIENTINP_BASE_INTERACT);
            pktSendBitsPack(pak, 8, id);
            pktSendString(pak, groupName);
            END_INPUT_PACKET
        }
    }

    if ((groupName = groupDefFindPropertyValue(def, "ArenaKiosk"))!=NULL)
    {
        START_INPUT_PACKET(pak, CLIENTINP_ARENAKIOSK_OPEN);
        END_INPUT_PACKET
    }

    if ((groupName = groupDefFindPropertyValue(def, "personal_storage"))!=NULL)
    {
        int id = baseedit_GetDetailFromChild(tracker, NULL);    // tracker is validated here
        RoomDetail * detail = baseGetDetail(&g_base,id,NULL);    // returns NULL if not found in base item list

        if (!detail ||                                                // if this is not a base item, let the players use it    
            (detail && detail->bPowered && detail->bControlled))    // if this is a base item, validates that this item is powered and controlled
            ShowStoredSalvage(TRUE,coord);
    }
}

// The rest of this file is handleCursor()
//---------------------------------------------------------------------------------------------------------------
//
//

static bool sphereLineIntersection(float radius, const Vec3 center, const Vec3 start, const Vec3 end, Vec3 result)
{
    // I saw sphereLineCollision and thought it should work, but it didn't
    // seem to give me the right answer.

    // This returns the point FARTHEST from start, which is what we want for
    // targetting.

    //
    // Full derivation at http://astronomy.swin.edu.au/~pbourke/geometry/sphereline/
    //
    // P = start + u(end-start)
    // S = (x-centerX)^2+(y-centerY)^2+(z-centerZ)^2 = radius
    //
    // Substituting P into S gives a quadratic of the form of:
    //    au^2 + bu + c = 0
    //       (I won't bother with defining a, b, and c here since I'm going
    //        to do it in code below.)
    //
    // Then use quadratic formula to solve for the parameter and substitute
    // back in.
    //
#define X 0
#define Y 1
#define Z 2
    float u = 0;

    float a = SQR(end[X]-start[X])+SQR(end[Y]-start[Y])+SQR(end[Z]-start[Z]);

    float b = 2*((end[X]-start[X])*(start[X]-center[X])
        +(end[Y]-start[Y])*(start[Y]-center[Y])
        +(end[Z]-start[Z])*(start[Z]-center[Z]));

    float c =
        SQR(center[X])+SQR(center[Y])+SQR(center[Z])
        +SQR(start[X])+SQR(start[Y])+SQR(start[Z])
        -2*(center[X]*start[X]+center[Y]*start[Y]+center[Z]*start[Z])
        -SQR(radius);

    float m = b*b-4*a*c;

    if(m<0)
    {
        // No intersection.
        copyVec3(center, result);
        return false;
    }
    else if(m==0)
    {
        // tangent
        u = -b/2*a;
    }
    else
    {
        // 2 answers, choose the farthest.
        u = (-b + sqrt(m))/(2*a);
    }

    result[X] = start[X] + u*(end[X]-start[X]);
    result[Y] = start[Y] + u*(end[Y]-start[Y]);
    result[Z] = start[Z] + u*(end[Z]-start[Z]);

    return true;

#undef X
#undef Y
#undef Z
}

static void classifyEntity(Entity *ent, int *selectEntity, int *canClickEntity)
{
     EntType entType;
    *selectEntity = 0;
     *canClickEntity = 1;

     if( mouseDown( MS_LEFT) && cursor.target_world==kWorldTarget_None)
    {
        *selectEntity = 1;
    }

    entType = ENTTYPE(ent);

    // The user has just pressed the left button.
    // If the user is selecting an entity type that they are supposed to be able to target,
    // then select the entity.
    if(!canSelectAnyEntity())
    {
        if(    entType != ENTTYPE_NPC            &&
            entType != ENTTYPE_PLAYER        &&
            entType != ENTTYPE_HERO            &&
            entType != ENTTYPE_CRITTER        &&
            entType != ENTTYPE_MISSION_ITEM
            || ent->noDrawOnClient
            || ent->seq && ent->seq->type->notselectable
            || game_state.edit_base)
        {
            *selectEntity = 0;
            current_target_under_mouse = NULL;

            if(    entType != ENTTYPE_DOOR &&
                entType != ENTTYPE_MISSION_ITEM &&
                !baseedit_IsClickableDetailEnt(ent))
            {
                *canClickEntity = 0;
            }
        }
        if (!entity_TargetIsInVisionPhase(playerPtr(), ent))
        {
            *selectEntity = 0;
            *canClickEntity = 0;
        }
    }
}

// If this triggers, fix the shenanigans I'm pulling below where I cast F32s to ints to store in the stash...
STATIC_ASSERT(sizeof(int) == sizeof(F32));
extern int g_cursor_size;

// Does the actual setup
void handleCursorInternal()
{
    static float lastCursorScale = 0.0f;
    float cursorScale = optionGetf(kUO_CursorScale);
    if (!(cursorScale > 0.9f) // this is here because it can be zero or NaN sometimes...
        || g_cursor_size == 32) // if this computer doesn't support > 1.0...
    {
        cursorScale = 1.0f;
        optionSetf(kUO_CursorScale, 1.0f, false);
    }

    PERFINFO_AUTO_START("change cursor", 1);
    // sometimes we draw a software cursor to prevent ugly detaching from window frame
    if( cursor.software )
    {
        int x, y;
        inpMousePos( &x, &y );
        display_sprite( cursor.base, x, y, 5000, cursorScale, cursorScale, CLR_WHITE );

        if( cursor.overlay )
            display_sprite( cursor.overlay, x, y, 5001, cursorScale, cursorScale, CLR_WHITE );

        if( cursor.HWvisible )
        {
            ShowCursor( FALSE );
            cursor.HWvisible = FALSE;
        }
    }
    else // most of the time we just use the hardware cursor
    {
        if( cursor.dragging )
        {
            cursor.base = atlasLoadTexture( "cursor_hand_closed.tga" );

            if ( cursor.dragged_icon && iconIsNew( cursor.base, cursor.dragged_icon ) )
            {
                hwcursorClear();
                hwcursorSetHotSpot(cursor.dragged_icon->width*DRAG_ICON_SC/2+2,cursor.dragged_icon->height*DRAG_ICON_SC/2+2);

                if( cursor.overlay )
                {
                    hwcursorBlit( cursor.overlay, 1 << log2i( cursor.overlay->width*DRAG_ICON_SC ),
                        1 << log2i( cursor.overlay->height*DRAG_ICON_SC ), 0, 0, cursor.overlay_scale*cursorScale*DRAG_ICON_SC, CLR_WHITE );
                }
                hwcursorBlit( cursor.dragged_icon, 1 << log2i( cursor.dragged_icon->width*DRAG_ICON_SC ),
                    1 << log2i( cursor.dragged_icon->height*DRAG_ICON_SC ), 0, 0, cursor.dragged_icon_scale*cursorScale*DRAG_ICON_SC, CLR_WHITE );
                hwcursorBlit( cursor.base, 1 << log2i( cursor.base->width ), 1 << log2i( cursor.base->width ),
                    cursor.dragged_icon->width*cursor.dragged_icon_scale*DRAG_ICON_SC/2-12, cursor.dragged_icon->height*cursor.dragged_icon_scale*DRAG_ICON_SC/2-12, cursorScale, CLR_WHITE );
                hwcursorReload(NULL, cursor.dragged_icon->width*DRAG_ICON_SC/2,cursor.dragged_icon->height*DRAG_ICON_SC/2);
            }

            // --------------------
            // special case for concept inventory dragging

            if( cursor.drag_obj.type == kTrayItemType_ConceptInvItem )
            {
                if( verify( cursor.drag_obj.displayCb ))
                {
                    cursor.drag_obj.displayCb( &cursor.drag_obj, gMouseInpCur.x, gMouseInpCur.y, cursor.drag_obj.z, cursorScale*cursor.drag_obj.scale );
                }

            }
        }
        else if (!cursor.dragging && (iconIsNew(cursor.base, cursor.overlay) || lastCursorScale != cursorScale))
        {
            StashElement element;
            char cursor_name[1000] = {0};
            char* pos = cursor_name;
            HCURSOR hCursor;
            F32 lastCursorScale;
            bool regenerated = false;

            if (!cursorHashTable)
            {
                cursorHashTable = stashTableCreateWithStringKeys(100,  StashDeepCopyKeys | StashCaseSensitive );
            }

            if (!cursorScaleHashTable)
            {
                cursorScaleHashTable = stashTableCreateWithStringKeys(100,  StashDeepCopyKeys | StashCaseSensitive );
            }

            pos += sprintf(pos, "%s(%d,%d)", cursor.base->name, cursor.hotX, cursor.hotY);

            if( cursor.overlay )
                pos += sprintf(pos, "%s", cursor.overlay->name);

            stashFindElement(cursorHashTable, cursor_name, &element);

            if (element)
            {

                int tempInt;
                hCursor = stashElementGetPointer(element);
                stashFindElement(cursorScaleHashTable, cursor_name, &element);
                tempInt = stashElementGetInt(element);
                lastCursorScale = *(F32*)&tempInt;
            }
            else
            {
                hCursor = NULL;
            }

            if (!hCursor || lastCursorScale != cursorScale)
            {
                hCursor = NULL;
                hwcursorClear();
                hwcursorSetHotSpot(cursor.hotX,cursor.hotY);
                hwcursorBlit( cursor.base, 1 << log2i( cursor.base->width ), 1 << log2i( cursor.base->width ), 0, 0, cursorScale, CLR_WHITE );
                if( cursor.overlay )
                    hwcursorBlit( cursor.overlay, 1 << log2i( cursor.overlay->width ), 1 << log2i( cursor.overlay->width ), 0, 0, cursorScale, CLR_WHITE );
                regenerated = true;
            }

            hwcursorReload(game_state.cursor_cache ? &hCursor : NULL, cursor.hotX, cursor.hotY);

            if(game_state.cursor_cache && regenerated)
            {
                stashAddPointer(cursorHashTable, cursor_name, hCursor, true);
                stashAddInt(cursorScaleHashTable, cursor_name, *(int*)&cursorScale, true);
            }
        }

        if( !cursor.HWvisible )
        {
            ShowCursor( TRUE );
            cursor.HWvisible = TRUE;
        }
    }

    setCursor( NULL, NULL, FALSE, 2, 2 ); // reset cursor for next pass

    lastCursorScale = cursorScale;

    PERFINFO_AUTO_STOP();
}

void ctmHandleClickDef(GroupDef *def, Mat4 mat)
{
    if (!useClickToMove() || !def || !mouseDown(MS_LEFT) || isDown(MS_RIGHT))
        return;

    // background object, we want to go towards it and activate it
    if (timerElapsedAndStart(cursor.click_move_timer) < DOUBLE_CLICK_TIME && cursor.click_move_last_def == def)
    {
        playerSetFollowMode(NULL, mat, def, 1, 1);
    }

    cursor.click_move_last_ent = 0;
    cursor.click_move_last_def = def;
}

static void ctmHandleClickEnt(Entity *ent)
{
    int selectEntity;
    int canClickEntity;

    if (!useClickToMove() || !ent || !mouseDown(MS_LEFT) || isDown(MS_RIGHT))
        return;

    classifyEntity(ent, &selectEntity, &canClickEntity);

    if (selectEntity || canClickEntity)
    {
        // CLICKTOMOVE - This is the case when double clicking
        if (timerElapsedAndStart(cursor.click_move_timer) < DOUBLE_CLICK_TIME && erGetEnt(cursor.click_move_last_ent) == ent)
        {
            playerSetFollowMode(ent, NULL, NULL, 1, 1);
        }

        cursor.click_move_last_ent = erGetRef(ent);
        cursor.click_move_last_def = 0;
    }
}

void handleWorldTargetingCursor( Cursor * cursor, Mat4 matWorld, int hitWorld, mouse_input * gMouseInpCur, Entity *player, Mat4 matCursor, Vec3 probe )
{
    bool bOutOfRange = false;

    if(!hitWorld || distance3(ENTPOS(player), matWorld[3]) > cursor->target_distance )
    {
        Vec3 start;
        Vec3 end2;
        Vec3 rpy;
        Vec3 result;

        cursor->target_world = kWorldTarget_OffGround;

        gfxCursor3d((float)gMouseInpCur->x, (float)gMouseInpCur->y, cursor->target_distance+100, start, end2);
        if(sphereLineIntersection(cursor->target_distance-1, ENTPOS(player), start, end2, result))
        {
            fxLookAt(result, ENTPOS(player), matCursor);
        }
        else
        {
            // Didn't even intersect the sphere. Just put a cursor
            // on the screen.
            bOutOfRange = true;
            cursor->target_world = kWorldTarget_OutOfRange;

            gfxCursor3d((float)gMouseInpCur->x, (float)gMouseInpCur->y, cam_info.targetZDist, start, result);
            fxFaceTheCamera(result, matCursor);
        }

        copyVec3(result, matCursor[3]);

        rpy[0]=-PI/2;
        rpy[1]=0;
        rpy[2]=0;

        rotateMat3(rpy, matCursor);
    }
    else
    {
        copyMat4(matWorld, matCursor);

        cursor->target_world = kWorldTarget_Normal;
    }

    tray_HandleWorldCursor(player, matCursor, probe);
    fxChangeWorldPosition(cursor->s_iCursorID, matCursor);
}

void resetWorldTargetCursor( Cursor * cursor )
{
    fxDelete(cursor->s_iCursorID, HARD_KILL);
    cursor->s_iCursorID = 0;
    cursor->s_iLastWorldTarget = cursor->target_world;

    if(cursor->target_world)
    {
        FxParams fxp;

        fxInitFxParams(&fxp);
        copyMat4(unitmat, fxp.keys[0].offset);
        fxp.keycount++;
        fxp.power = 10; //# from 1 to 10
        fxp.fxtype = FX_ENTITYFX; // We don't want this FX to go away if there are a lot onscreen

        switch(cursor->target_world)
        {
        case kWorldTarget_Normal:
        case kWorldTarget_Blocked:
        case kWorldTarget_OffGround:
            cursor->s_iCursorID = fxCreate( "GENERIC/TARGETINGFXOUTOFRANGE.FX", &fxp );
            break;

        case kWorldTarget_InRange:
            cursor->s_iCursorID = fxCreate( "GENERIC/TARGETINGFX.FX", &fxp );
            break;

        case kWorldTarget_OutOfRange:
            cursor->s_iCursorID = fxCreate( "GENERIC/TARGETINGFXWAYOUT.FX", &fxp );
            break;
        }
    }
}

void initCursor( Cursor * cursor )
{
    memset(    cursor, 0, sizeof( Cursor ) );
    cursor->initted = TRUE;
    cursor->HWvisible = TRUE;
    setCursor( "cursor_select.tga", NULL, FALSE, 2, 2 );

    cursor->click_move_timer = timerAlloc();
    timerStart(cursor->click_move_timer);
}

void doClickToMove( mouse_input * gMouseInpCur, Entity * player, Mat4 matCursor ) 
{
    // CLICKTOMOVE - This is the case when clicking on the ground, on a background object, or in the distance to move to a point
    Vec3 start;
    Vec3 end2;
    Vec3 rpy;
    Vec3 result;
    Mat4 matWorld2;
    bool hitWorld2;
    ItemSelect item_sel2;

    // first, shoot out a longer ray from the camera
    cursorFind((float)gMouseInpCur->x, (float)gMouseInpCur->y, CLICK_TO_MOVE_INTERSECT_DISTANCE, &item_sel2, end2, matWorld2, &hitWorld2, BASIC_SELECTION_DISTANCE, NULL);

    if (item_sel2.ent_id > 0)
    {
        // cursorFind converted the door def to an ent_id, or door was too far away for first call to cursorFind
        Entity *ent = entFromId(item_sel2.ent_id);
        if (ent && (ENTTYPE(ent) == ENTTYPE_DOOR))
            ctmHandleClickEnt(ent);
    }
    else if (item_sel2.def && isInteractLocation(item_sel2.def))
    {
        ctmHandleClickDef(item_sel2.def, item_sel2.mat);
    }
    else if(!hitWorld2 || distance3(ENTPOS(player), matWorld2[3]) > gClickToMoveDist)
    {
        if (hitWorld2) // distance is too far
        {
            // if it hit something, we want to have the player go some distance towards the point it hit
            float scale;
            subVec3(matWorld2[3], ENTPOS(player), result);
            scale = ((float)(gClickToMoveDist-1)) / lengthVec3(result);
            scaleVec3(result, scale, result);
            addVec3(ENTPOS(player), result, result);

            fxLookAt(result, ENTPOS(player), matCursor);
        }
        else
        {
            // if it hit nothing, intersect the camera->cursor ray with the sphere around the player.
            gfxCursor3d((float)gMouseInpCur->x, (float)gMouseInpCur->y, gClickToMoveDist, start, end2);
            if(sphereLineIntersection(gClickToMoveDist-1, ENTPOS(player), start, end2, result))
            {
                fxLookAt(result, ENTPOS(player), matCursor);
            }
            else
            {
                // Didn't even intersect the sphere. Just put a cursor
                // on the screen.
                gfxCursor3d((float)gMouseInpCur->x, (float)gMouseInpCur->y, cam_info.targetZDist, start, result);
                fxFaceTheCamera(result, matCursor);
            }
        }

        copyVec3(result, matCursor[3]);

        rpy[0]=-PI/2;
        rpy[1]=0;
        rpy[2]=0;

        rotateMat3(rpy, matCursor);

        playerSetFollowMode(NULL, matCursor, NULL, 1, 0);
    }
    else
    {
        Vec3 towardPlayer;

        copyMat4(matWorld2, matCursor);

        subVec3(ENTPOS(player), matWorld2[3], towardPlayer);
        normalVec3(towardPlayer);

        if (dotVec3(matCursor[1], towardPlayer) < 0.6f)
        {
            // point cursor z axis toward player
            copyVec3(towardPlayer, matCursor[2]);

            // orthonormalize, keeping Y axis the same
            crossVec3(matCursor[1], matCursor[2], matCursor[0]);
            crossVec3(matCursor[0], matCursor[1], matCursor[2]);
        }

        playerSetFollowMode(NULL, matCursor, NULL, 1, 0);
    }
}


void doPlaceEntity( int hitWorld, Mat4 mat, Vec3 end )
{
    if(hitWorld && mouseDown(MS_LEFT))
    {
        char buffer[1000];
        Vec3 offset;

        scaleVec3( mat[1], 2, offset);
        offset[1] = 0;
        addVec3(end, offset, end);

        sprintf(buffer,
            "entcontrol %d setpos %f %f %f%s",
            current_target->svr_idx,
            end[0],
            end[1],
            end[2],
            game_state.place_entity == 2 ? " setspawn" : "");

        cmdParse(buffer);
    }
}

int dragTarget = -1;
TrayObj dragCopy;

void drag_ConfirmSendToTarget(void *data)
{
    gift_SendAndRemove( dragTarget, &dragCopy );
    dragTarget = -1;
}

void drag_DenySendToTarget(void *data)
{
    dragTarget = -1;
}

void handleDroppingWhatYouWereDragging( Cursor * cursor, ItemSelect* item_sel, Entity * player )
{
    static int frameDelay = 0;
    if( frameDelay )
    {
        if( current_target_under_mouse && 
            (ENTTYPE(current_target_under_mouse) == ENTTYPE_PLAYER ||    // player gift
            (cursor->drag_obj.type == kTrayItemType_Inspiration && entIsPet(current_target_under_mouse) && playerIsMasterMind() && !amOnPetArenaMap()))) // make pet use
        {
            if ( cursor->drag_obj.type == kTrayItemType_SpecializationInventory && player && player->pchar)
            {
                Boost *pBoost = player->pchar->aBoosts[cursor->drag_obj.ispec];

                if (pBoost != NULL && pBoost->iNumCombines > 0 && dragTarget < 0)
                {
                    memcpy(&dragCopy, &cursor->drag_obj, sizeof(TrayObj));
                    dragTarget = current_target_under_mouse->svr_idx;
                    dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyTradeCombinedEnhancement", NULL, drag_ConfirmSendToTarget, NULL, drag_DenySendToTarget, 0, NULL, NULL,0,0,0, NULL );
                    trayobj_stopDragging();
                    frameDelay = 0;
                    return;
                }
            }

            gift_SendAndRemove( current_target_under_mouse->svr_idx, &cursor->drag_obj );
            trayobj_stopDragging();
            frameDelay = 0;
        }
        else if(item_sel->def && item_sel->tracker && groupDefFindPropertyValue(item_sel->def, "Interact") )
        { 
            int id = baseedit_GetDetailFromChild(item_sel->tracker, NULL);
            RoomDetail * detail = baseGetDetail(&g_base,id,NULL);

            if(detail) // dragging itoms onto storage containers
            {
                if( playerPtr()->pchar && playerPtr()->pchar->salvageInv && 
                    cursor->drag_obj.type == kTrayItemType_Salvage &&
                    detail->info->eFunction == kDetailFunction_StorageSalvage &&
                    playerPtr()->pchar->salvageInv[cursor->drag_obj.invIdx] )
                {
                    if (!windowUp(WDW_BASE_STORAGE) || detail != getStorageWindowDetail())
                    {
                        reportReachedVisited(item_sel->def, item_sel->mat[3], item_sel->tracker);
                        basestorageSetDetail(detail);
                    }
//                    if( !windowUp(WDW_BASE_STORAGE) )
//                        reportReachedVisited(item_sel->def, item_sel->mat[3], item_sel->tracker);

                    salvageAddToStorageContainer(playerPtr()->pchar->salvageInv[cursor->drag_obj.invIdx],10000);
                }
                if( cursor->drag_obj.type == kTrayItemType_Inspiration &&
                    detail->info->eFunction == kDetailFunction_StorageInspiration )
                {
                    TrayObj *obj = &cursor->drag_obj;
                    const BasePower *ppow = obj?player->pchar->aInspirations[obj->iset][obj->ipow]:NULL;
                    if( ppow )
                    {
                        if (!windowUp(WDW_BASE_STORAGE) || detail != getStorageWindowDetail())
//                        if( !windowUp(WDW_BASE_STORAGE) )
                        {
                            reportReachedVisited(item_sel->def, item_sel->mat[3], item_sel->tracker);
                            basestorageSetDetail(detail);
                        }
                        inspirationAddToStorageContainer(ppow,detail->id,obj->iset,obj->ipow,1);
                    }
                }
                if( cursor->drag_obj.type == kTrayItemType_SpecializationInventory &&
                    detail->info->eFunction == kDetailFunction_StorageEnhancement )
                {
                    TrayObj *obj = &cursor->drag_obj;
                    Boost *boost = player->pchar->aBoosts[obj->ispec]?player->pchar->aBoosts[obj->ispec]:NULL;
                    const BasePower *ppow = boost?boost->ppowBase:NULL;

                    if( ppow ) 
                    {
//                        if( !windowUp(WDW_BASE_STORAGE) )
                        if (!windowUp(WDW_BASE_STORAGE) || detail != getStorageWindowDetail())
                        {
                            reportReachedVisited(item_sel->def, item_sel->mat[3], item_sel->tracker);
                            basestorageSetDetail(detail);
                        }
                        enhancementAddToStorageContainer(boost,detail->id,obj->ispec,1);
                    }
                }
                trayobj_stopDragging();
                frameDelay = 0;
            }
        }
        else
        {
            if (cursor->drag_obj.type == kTrayItemType_Power || IS_TRAY_MACRO(cursor->drag_obj.type))
            {
                // Possible code snippet OTD here.  I have no clue whatsoever as to why we're calling playerPtr() here, since the
                // last parameter to this routine is <quote> Entity *player </quote>
                Entity *e = playerPtr();
                if (e && e->pchar && e->pl)
                {
                    TrayObj *last = getTrayObj(e->pl->tray, cursor->drag_obj.icat, cursor->drag_obj.itray, cursor->drag_obj.islot, e->pchar->iCurBuild, 1);
    
                    if(last)
                        trayobj_copy( last, &cursor->drag_obj, TRUE );
                }
            }

            trayobj_stopDragging();
            frameDelay = 0;
        }
    }
    else
        frameDelay = 1;
}

void interactWithInterestingGroupDef( Entity * player, GroupDef * def, Vec3 collisionPos, DefTracker * tracker )
{
    if(player && distance3Squared(ENTPOS(player), collisionPos) < SQR(INTERACT_DISTANCE))
    {
        char *doorval = groupDefFindPropertyValue( def, "Door" );

        if (doorval && (stricmp(doorval,"Door") == 0 || stricmp(doorval,"ReturnDoor") == 0))
        {
            enterDoorClient( collisionPos, 0 );
        }
        else
        {        
            reportReachedVisited( def, collisionPos, tracker );
        }
        playerStopFollowMode();
    }
}

void handleMouseOverClickableGroupDef( Entity * player, GroupDef * def, DefTracker * tracker, Mat4 collisionMat )
{
    if(isInteractLocation( def ) )
    {
        setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2);
    }

    if( mouseDown( MS_LEFT ) && !entMode(player, ENT_DEAD) )
    {
        interactWithInterestingGroupDef( player, def, collisionMat[3], tracker );
    }
}

int critter_and_friend(Entity *target_ent, Entity *player)
{
    if (!target_ent || !player)
        return 0;
    if (ENTTYPE(target_ent) != ENTTYPE_CRITTER)
        return 0;

    return !character_TargetIsFoe(player->pchar, target_ent->pchar);
}

void handleMouseOverEntity( int mousedOverEnt, Entity * player )
{
    int selectEntity;
    int canClickEntity;
    Entity*        target_ent = ( Entity *)NULL;

    target_ent = entFromId( mousedOverEnt );
    assert( target_ent );

    current_target_under_mouse = target_ent;

    classifyEntity(target_ent, &selectEntity, &canClickEntity);

    //If you have selected this entity ( Confusingly redundant with mouseDown block below )
    if( selectEntity )
    {
        current_target = target_ent;
        gSelectedDBID = 0;
        gSelectedHandle[0] = 0;
        if ( entIsMyHenchman(target_ent) ) 
            current_pet = erGetRef(target_ent);

        if( mouseRightClick() )
            contextMenuForTarget( target_ent );
    }

    //Draw correct cursor for clickable entities
    if( canClickEntity )
    {
        if( ENTTYPE(target_ent) == ENTTYPE_MISSION_ITEM && player ) //TO DO this for doors and contacts, too
        {
            if( closeEnoughToInteract( player, target_ent, INTERACT_DISTANCE ) )
                setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2);
        }
        else if(( ENTTYPE(target_ent) == ENTTYPE_DOOR || baseedit_IsClickableDetailEnt(target_ent) ) && player)
        {
            if( distance3Squared( ENTPOS(player), ENTPOS(target_ent) ) < SQR(INTERACT_DISTANCE ) )
                setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2);
        }
        else if( ENTTYPE(target_ent) == ENTTYPE_CRITTER )
        {
            setCursor( "cursor_select_enemy.tga", NULL, FALSE, 2, 2);
        }
        else
        {
            setCursor( "cursor_friendly.tga", NULL, FALSE, 2, 2 );
        }
    }

    //Strange
    ctmHandleClickEnt( target_ent ); 

    // If you just clicked on this entity
    if( mouseDown( MS_LEFT ) && player && !entMode(player, ENT_DEAD))
    {
        bool interactWithNPC = false;

        if ( target_ent->seq->type->onClickFx) //Currently unused
        {
            seqTriggerOnClickFx(target_ent->seq, target_ent);
        }

        if( ENTTYPE(target_ent) == ENTTYPE_MISSION_ITEM )
        {
            if(player && closeEnoughToInteract( player, target_ent, INTERACT_DISTANCE ) )
            {
                interactWithNPC = true;
            }
        }
        else if( ENTTYPE(target_ent) == ENTTYPE_NPC || baseedit_IsClickableDetailEnt(target_ent) || critter_and_friend(target_ent, player) )
        {
            if( distance3Squared(ENTPOS(player), ENTPOS(target_ent)) < SQR(INTERACT_DISTANCE) )
            {
                interactWithNPC = true;
            }
        }
        else if( ENTTYPE(target_ent) == ENTTYPE_DOOR )
        {
            if ( distance3Squared(ENTPOS(player), ENTPOS(target_ent)) < SQR(INTERACT_DISTANCE) )
            {
                playerStopFollowMode();
                enterDoorClient( ENTPOS(target_ent), 0 );
            }
        }

        if (!entity_TargetIsInVisionPhase(player, target_ent))
        {
            interactWithNPC = false;
        }

        if( interactWithNPC && !game_state.pendingTSMapXfer)
        {
            playerStopFollowMode();
            START_INPUT_PACKET(pak, CLIENTINP_TOUCH_NPC);
            pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, target_ent->svr_idx);
            END_INPUT_PACKET
        }

    } 
} 

void handlePlayerTargetingConfusion( Entity * player )
{
    static Entity *last_target = NULL;
    if(current_target != last_target)
    {
        if (player && !character_IsConfused(player->pchar))
        {
            tray_CancelQueuedPower();
        }
        last_target = current_target;
    }
}

void handleMouseOverAndOrClick( Entity * player, ItemSelect * item_sel )
{
    Mat4 matWorld;
    Mat4 matCursor;
    Vec3 end;
    F32 probe[6];    
    int x, y;
    bool hitWorld = false;
    float distance;

    /////// Cast Ray
    //Figure out how far to cast the selection ray
    if(cursor.target_world)
    {
        distance = TELEPORT_SELECTION_DISTANCE;
        tray_SetWorldCursorRange();
    }
    else if( game_state.place_entity && current_target )
    {
        distance = PLACE_ENT_SELECTION_DISTANCE;
    }
    else
    {
        distance = BASIC_SELECTION_DISTANCE;
    }

    //If the right click button is currently held down, use those buffered coords to do the ray cast, otherwise use the current 
    //mouse coords (TO DO what about left click buffered vals? maybe, there should be a ray cast for each (left, right, and curr)
    if( !rightClickCoords( &x, &y ) )
    {
        x = gMouseInpCur.x;
        y = gMouseInpCur.y;
    }

    //Cast selection ray and see what you get
    cursorFind(    x, y, distance, item_sel, end, matWorld, &hitWorld, INTERACT_DISTANCE, probe );
    // probe is really two Vec3's in tandem.  The first is the probe vector, the second is the player's location.
    copyVec3(ENTPOS(player), &probe[3]);

    // Don't pay attention to visit locations player shouldn't know about (TO DO belongs in CursorFind?)
    if(item_sel->def && visitLocationIgnored(item_sel->def))
        item_sel->def = NULL;

    /////// Handle selection, in order of precedence
    {
        if( game_state.place_entity && current_target) // Martin's entity placement debug mode
        {
            doPlaceEntity( hitWorld, item_sel->mat, end ); // The cursor is the targeting bullseye fx for targeting a place in the world
        }
        else if( cursor.target_world ) // The cursor is the targeting bullseye fx for targeting a place in the world
        {
            handleWorldTargetingCursor( &cursor, matWorld, hitWorld, &gMouseInpCur, player, matCursor, probe );
        }
        else if( item_sel->ent_id ) // The cursor is over an entity
        {
            handleMouseOverEntity( item_sel->ent_id, player );
        }
        else if( item_sel->def ) // The cursor is over a door black poly or other GroupDef that can be clicked on
        {
            handleMouseOverClickableGroupDef( player, item_sel->def, item_sel->tracker, item_sel->mat );
        }
        else if( useClickToMove() && mouseDown(MS_LEFT) && !isDown(MS_RIGHT) ) // The player has just clicked to move
        {
            doClickToMove( &gMouseInpCur, player, matCursor );
        }
    }
}

void handleCursor()
{
    Entity*        player = playerPtr();
    ItemSelect    item_sel = {0};

    if( !cursor.initted )
        initCursor( &cursor );

    ////// Reset globals and make sure the old target is still valid

    if( cursor.s_iLastWorldTarget != cursor.target_world )
        resetWorldTargetCursor( &cursor );

    current_target_under_mouse = NULL;
    
    if( current_target && !isEntitySelectable(current_target))
        current_target = NULL;
    
    if( !isMenu(MENU_GAME)  || !player )
        control_state.canlook = FALSE;

    // Can look shows no cursor at all, and nothing is moused over
    if ( control_state.canlook ) 
        return;

    ////// Decide what you are moused over, handle left and right clicks, and set cursor icon 

    if ( waitingForDoor() )
    {
        setCursor( "cursor_wait.tga", NULL, FALSE, 11, 10 );
    }
    else if( isMenu(MENU_GAME) 
        && !entDebugMenuVisible()             
        && !game_state.texWordEdit        
        && !game_state.viewCutScene             //Probably this should just set collisions_off_for_rest_of_frame
        && !collisions_off_for_rest_of_frame //Generally set by UI, saying don't click through
        && !game_state.edit_base
        && player )             
    {
        handleMouseOverAndOrClick( player, &item_sel );
    } 

    ////// Draw cursor
    handleCursorInternal(); 

    ////// Handle Drag and Drop ( A little strange for it to be here )
    if( player )
    {
        if( cursor.dragging && !isDown( MS_LEFT ) )
            handleDroppingWhatYouWereDragging( &cursor, &item_sel, player );

        ////// Mysterious Powers stuff ( Also a little strange for it to be here )
        // If the current target isn't the same, and we're not confused, clear out any queued up stuff.
        handlePlayerTargetingConfusion( player );
    }
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------



