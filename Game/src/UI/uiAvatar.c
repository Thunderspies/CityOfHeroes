/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "player/player.h"
#include "cmdparse/cmdcommon.h"        // for TIMESTEP
#include "UI/uiAvatar.h"
#include "UI/uiCostume.h"        // for gSelectedGeoSet
#include "gameData/costume_data.h"    // for CostumeGeoSet internals
#include "cmdparse/cmdgame.h"        // for toggle3Dmode enums
#include "UI/uiGame.h"            // for isMenu()
#include "graphics/FX/fx.h"                // for green blobby fx functions
#include "entity/costume_client.h" // for scaleHero
#include "graphics/camera.h"
#include "win/win_init.h"
#include "entity/entity.h"
#include "graphics/gfxwindow.h"        // gfx_window
#include "UI/sprite/sprite_base.h"
#include "game.h" // for changeShellCamera

//----------------------------------------------------------------------------------------------

static float interpPoliceHuge(float x)
{
       float a = 3.79968499136267;
    float b = 7.45859160654399E-02;
    return ((a * x) + b);
}

static float interpPoliceFem(float x)
{
    float a = -3.38143142634556;
    float b = -7.72042136639228E-02;
    return ((a * x) + b);
}

static float interpProfileMaleHuge(float x)
{
    float a = 1.56610978164688;
    float b = 8.11222020894158E-02;
    return ((a * x) + b);
}

static float interpProfileFem(float x)
{
    float a = 1.52064541602481;
    float b = 4.60943812914448E-02;
    return ((a * x) + b);
}

static float interpDefaultMale(float x)
{
    float a = 2.51080159387888;
    float b = 0.035749949103183;
    return ((a * x) + b);
}

static float interpDefaultOther(float x)
{
    float a = 2.52502349659209;
    float b = -5.1650616895064E-03;
    return ((a * x) + b);
}

#define LOGIN_AVATAR_CAMERA_X        1.3f

// Used for gender selection on the New Hero page.
//
// modes for move avatar
void moveAvatar( AvatarMode mode )
{
    Entity * e = playerPtr();
    float aspect;

    if (isLetterBoxMenu())
    {
        aspect = (F32) DEFAULT_SCRN_WD / DEFAULT_SCRN_HT;
    }
    else
    {
        int w, h;
        windowClientSizeThisFrame( &w, &h );
         aspect = (float)w/h;
    }

    switch ( mode )
    {

    case AVATAR_DEFAULT_MALE:
        {
            changeShellCamera( 3.5, 2.6, 13.5 );
            playerTurn( 0 );
            scaleHero( e, 0 );
        } break;

    case AVATAR_DEFAULT_FEMALE:
        {
            changeShellCamera( interpPoliceFem(aspect), 2.8, 24.25 );
            playerTurn( 0 );
            scaleHero( e, 0 );
        } break;

    case AVATAR_DEFAULT_HUGE:
        {
            changeShellCamera( 3.5, 2.6, 13.5 );
            playerTurn( 0 );
            scaleHero( e, 0 );
        } break;

    case AVATAR_CENTER_MALE:
        {
               changeShellCamera( 0, 2.6, 13.5 );
            playerTurn( 0 );
            scaleHero( e, 0 );
        } break;

    case AVATAR_CENTER_FEMALE:
        {
             changeShellCamera( 0, 2.8, 13.5 );
            playerTurn( 0 );
            scaleHero( e, 0 );
        } break;

    case AVATAR_CENTER_HUGE:
        {
            changeShellCamera( 0, 2.6, 13.5 );
            playerTurn( 0 );
            scaleHero( e, 0 );
        } break;

    case AVATAR_LOGIN:
        {
              changeShellCamera( LOGIN_AVATAR_CAMERA_X * aspect, 3.f, 14.f );
        } break;

    case AVATAR_E3SCREEN:
        {
            changeShellCamera( /*.80*aspect*/ 0, 2.6, 13 );
            playerTurn( 0 );
        }break;
    case AVATAR_KOREAN_COSTUMECREATOR_SCREEN:
        {
            changeShellCamera( /*.80*aspect*/ 0, 2.6, 100 );
            playerTurn( 0 );
        }break;

    case AVATAR_GS_MALE:
    case AVATAR_GS_BM:
        {
               changeShellCamera( 3.2*aspect, 3.5, 20 );
        } break;

     case AVATAR_GS_FEMALE:
    case AVATAR_GS_BF:
        {
               changeShellCamera( 3.2*aspect, 3.5, 20 );
        } break;

    case AVATAR_GS_HUGE:
    case AVATAR_GS_ENEMY:
    case AVATAR_GS_ENEMY2:
    case AVATAR_GS_ENEMY3:
        {
            changeShellCamera( 3.2*aspect, 3.5, 20 ); //3.09, 26.9 );
        } break;

    case AVATAR_PROFILE_MALE:
    case AVATAR_PROFILE_FEMALE:
    case AVATAR_PROFILE_HUGE:
        changeShellCamera( 2.25f*aspect, 2.8f, 15.f );
        playerTurn( 0.4f * TIMESTEP );
        break;
    default:
        break;
    }

    toggle_3d_game_modes(SHOW_TEMPLATE);

}

//----------------------------------------------------------------------------------------------



static float interpZoomMale(float x)
{
    float a = 1.1699144463331;
    float b = -6.13707976646018E-02;
    return ((a * x) + b);
}

static float interpZoomFemale(float x)
{
    float a = 1.26977763397973;
    float b = 1.69580073123207E-03;
    return ((a * x) + b);;
}

static float interpZoomHuge(float x)
{
    float a = 0.970188071039847;
    float b = 1.24960055437276E-02;
    return ((a * x) + b);
}

int gZoomed_in = 0;

F32* avatar_getDefaultPosition(int costumeEditMode, int centered )
{
    static Vec3 defaultPos = { .7015, 2.6, 13.5};
    static Vec3 returnVec;
    const CostumeGeoSet* selectedGeoSet = getSelectedGeoSet(costumeEditMode);

    if (selectedGeoSet && !vec3IsZero(selectedGeoSet->defaultPos))
    {
        copyVec3(selectedGeoSet->defaultPos, returnVec);
    }
    else
        copyVec3(defaultPos, returnVec);

    if( centered )
        returnVec[0] = 0;

    return returnVec;
}

F32* avatar_getZoomedInPosition(int costumeEditMode, int centered)
{
    static Vec3 zoomPos[2][3]    = {    {    { .6991, 4.2, 6},        // zoom is slightly different for each character type because of the height
                                        { .6777, 4.0, 7},        // differences, these numbers were basically found through trial&error
                                        { .7312, 4.3, 5},    },
                                    {    { 0, 4.2, 6},
                                        { 0, 4.0, 7},
                                        { 0, 4.3, 5},    }};

    const CostumeGeoSet* selectedGeoSet = getSelectedGeoSet(costumeEditMode);

    if (selectedGeoSet && !vec3IsZero(selectedGeoSet->defaultPos))
        return (cpp_const_cast(CostumeGeoSet*)(selectedGeoSet))->zoomPos;

    devassert(centered >=0 && centered <=1);
    switch (playerPtr()->costume->appearance.bodytype)
    {
        xcase kBodyType_Huge:
            return zoomPos[centered][2];

        xcase kBodyType_Female:
            return zoomPos[centered][1];

        xcase kBodyType_Male:
        default:
            return zoomPos[centered][0];
    }
}

void zoomAvatar(bool init, Vec3 destPos)
{
    static Vec3 curPos;  // the current camera position
    static Vec3 diffs;
    static Vec3 lastPos;

    // instantly init for first entry to screen
    if(init || !lastPos)
    {
        copyVec3(destPos,curPos);
    }
    else // otherwise interp
    {
        // if state changed calc the diffs, this is used so x/y/z speeds will all reach dest at same time
        if( !sameVec3(lastPos, destPos) )
            subVec3(destPos,lastPos,diffs);

        if(!sameVec3(curPos,destPos))
        {
            float dist = distance3Squared(curPos,destPos);

            curPos[0] += TIMESTEP/15 * diffs[0];
            curPos[1] += TIMESTEP/15 * diffs[1];
            curPos[2] += TIMESTEP/15 * diffs[2];

            if(distance3Squared(curPos,destPos) > dist) // end condition
                copyVec3(destPos,curPos);
        }
#ifndef FINAL
        else if( game_state.editnpc && gZoomed_in && GetForegroundWindow() == hwnd )
        {
            // when zoomed in and not interpolating, allow arrow keys to move around (shift arrows controls z)
            #define DELTA_X        0.003
            #define DELTA_Y        0.02
            #define DELTA_Z        0.15
            bool bLeft    = GetAsyncKeyState(VK_LEFT);
            bool bRight = GetAsyncKeyState(VK_RIGHT);
            bool bUp    = GetAsyncKeyState(VK_DOWN);
            bool bDown    = GetAsyncKeyState(VK_UP);
            if(bLeft || bRight || bUp || bDown)
            {
                float dx=0, dy=0, dz=0;
                float multiplier = GetAsyncKeyState(VK_CONTROL) ? 10.0 : 1.0;
                if( !GetAsyncKeyState(VK_SHIFT) )
                {
                    dx += bLeft ? -DELTA_X : (bRight ? DELTA_X : 0); 
                    dy += bUp ?   DELTA_Y :  (bDown  ? -DELTA_Y : 0); 
                }
                else
                {
                    dz += bLeft ? DELTA_Z   : (bRight  ? -DELTA_Z : 0); 
                }
                destPos[0] += dx * multiplier;
                destPos[1] += dy * multiplier;
                destPos[2] += dz * multiplier;
                copyVec3(destPos,curPos); // no interpolation, go straight there
            }
        }
#endif
    }

    copyVec3(destPos, lastPos);
    changeShellCamera( curPos[0]*curPos[2]*gfx_window.hvam, curPos[1], curPos[2] );
    toggle_3d_game_modes(SHOW_TEMPLATE);
}

void scaleAvatar()
{
    Entity* e = playerPtr();
    if (e && e->costume)
    {
        switch(e->costume->appearance.bodytype)
        {
        case kBodyType_Huge:
            scaleHero(e, -10.f);
            break;
        case kBodyType_Female:
            scaleHero(e, 0.f);
            break;
        case kBodyType_Male:
        default:
            scaleHero(e, -10.f);
            break;
        }
    }
}
