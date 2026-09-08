#include <utilitieslib/stdtypes.h>
#include "graphics/gfxLoadScreens.h"
#include <utilitieslib/utils/mathutil.h>
#include "cmdparse/cmdgame.h"
#include "render/tex.h"
#include <utilitieslib/utils/timing.h>
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/utils.h>
#include "win/win_init.h"
#include "clientcomm/clientcomm.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiStatus.h"
#include "graphics/font.h"
#include "render/renderprim.h"
#include "render/sprite.h"
#include "UI/uiLoadingTip.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include <utilitieslib/utils/Cbox.h>
#include "UI/uiInput.h"
#include "gameComm/initClient.h"
#include "entity/entrecv.h"
#include "UI/uiGame.h"
#include "win/input.h"
#include "UI/uiKeybind.h"
#include <utilitieslib/components/earray.h>
#include "UI/Hybrid/uiHybridMenu.h"
#include <utilitieslib/language/AppLocale.h>
#include "group/group.h"
#include "UI/uiAutomap.h"

//##########################################################################
//Loading Screen stuff 

#define VOLUME_FADE_SPEED 0.5

int max_bytes_to_load = 200000000;

static int show_bg=-1;
static int total_bytes_loaded, last_update_bytes=0;
static bool load_bar_full=false;
int load_bar_color=0xffffffff;
static unsigned long sLastOpaqueLoadScreenTime;
static unsigned long sFirstOpaqueLoadScreenTime;
static int sLastOpaqueLoadScreenFrame;
static bool sIsLoadingTipStale = true;

int loadingScreenVisible()
{
    return show_bg;
}

F32 getLoadScreenVolumeAdjustment(bool isMusic) {
    unsigned long now = timerCpuTicks();
    F32 fadeIn, fadeOut, fade;

    // Assume that a load screen is still being shown if it was shown last frame.
    if (global_state.global_frame_count - sLastOpaqueLoadScreenFrame <= 1)
        sLastOpaqueLoadScreenTime = now;

    // The music and SFX volumes differ in that SFX must never be played during a load screen, since
    //  SFX may be incorrectly spatialized during a map load.
    if (isMusic)
    {
        fadeOut = 1.0f - VOLUME_FADE_SPEED * (now - sFirstOpaqueLoadScreenTime) / timerCpuSpeed();
        fadeIn = VOLUME_FADE_SPEED * (MAX(1,(now - sLastOpaqueLoadScreenTime))) / timerCpuSpeed();
    }
    else
    {
        fadeOut = 0.0f;
        fadeIn = VOLUME_FADE_SPEED * (now - sLastOpaqueLoadScreenTime) / timerCpuSpeed();
    }

    
    fade = MAX(fadeOut, fadeIn);
    return MINMAX(fade, 0.0f, 1.0f);
}

const char *getDefaultLoadScreenName(void)
{
    if (getCurrentRegion() == REGION_EU)
    {
        switch (locGetIDInRegistry())
        {
            case LOCALE_ID_ENGLISH:
                return "COH_LogInScreen_Background_UK";
            case LOCALE_ID_FRENCH:
                return "COH_LogInScreen_Background_French";
            case LOCALE_ID_GERMAN:
                return "COH_LogInScreen_Background_German";
        }
    }

    return "COH_LogInScreen_Background";
}

static AtlasTex * bg_sprite;

int g_comicLoadScreen_pageIndex = 0;
char **g_comicLoadScreen_pageNames = 0;

bool comicLoadScreenEnabled(void)
{
    return eaSize(&g_comicLoadScreen_pageNames) > 0;
}

void gfxSetBGMapname(char* mapname)
{
    static char bg_mapname[MAX_PATH]="";
    char    *s,buf[1000],buf2[1000],*names[128],*start;
    int        count=0;
    static    char    last_mapname[MAX_PATH],last_loadname[MAX_PATH];
    int        show_bg_save = show_bg;

    if (!bg_mapname[0])
        strcpy(bg_mapname, getDefaultLoadScreenName());

    show_bg = 0; // So that the old loading screen is not shown while this loading screen is loading!

    if( !mapname )
    {
        strcpy(bg_mapname, getDefaultLoadScreenName());
        bg_sprite = atlasLoadTexture(bg_mapname);
        return;
    }

    if( strstri( mapname, "supergroupid=" ) )
    { // loading onto a base
        strcpy(bg_mapname, "base_loading_screen");
        bg_sprite = atlasLoadTexture(bg_mapname);
    }

    if(group_info.loadscreen[0])
        strncpy(buf, group_info.loadscreen, 1000);
    else
        strncpy(buf, mapname, 1000);
    forwardSlashes(buf);
    s = strrchr(buf,'.');
    if (s)
        *s = 0;
    s = buf;
    s = strrchr(buf,'/');

    if (s)
        s++;
    else
        s = buf;

    if (comicLoadScreenEnabled())
    {
        // index reset in receiveLoadScreenPages()
        gfxSetBG(g_comicLoadScreen_pageNames[g_comicLoadScreen_pageIndex]);
    }
    else if (texFind(s))
    {
        gfxSetBG(s);
        strcpy(bg_mapname, s);
    }
    else
    {
        strcpy(bg_mapname, getDefaultLoadScreenName());

        if (strlen(buf) > strlen("maps/") && strnicmp(buf, "maps/", 5)==0)
        {
            int num_up = 0;
            start = buf + strlen("maps/");
            sprintf(buf2, "loading_screens/%s", start);
            start = buf2;
            for(;;)
            {
                count = texFindByFullPathName(start,names,ARRAY_SIZE(names));
                
                if (count)
                    break;

                num_up++;

                s = strrchr(buf2,'/');
                if (!s)
                    break;
                *s = 0;

                if( num_up > 2 || // save art from themselves
                    stricmp(start, "loading_screens/missions/outdoor_missions")==0 || 
                    stricmp(start, "loading_screens/missions/unique")==0 || 
                    stricmp(start, "loading_screens/missions")==0 )
                {
                    if( !game_state.edit )
                        ErrorFilenamef( mapname, "No loading screen match found!" );
                    strcpy(bg_mapname, "def_ldg_scrn" );
                    bg_sprite = atlasLoadTexture(bg_mapname);
                    return;
                }

                if ( stricmp(start, "loading_screens")==0 )
                    break;
            }
        }

        if (count)
        {
            if (stricmp(last_mapname,mapname)!=0)
            {
                rule30Seed(timerCpuTicks64());
                strcpy(bg_mapname, names[rule30Rand() % count]);
                gfxSetBG(bg_mapname);
            }
        }
        else
        {
            gfxSetBG(bg_mapname);
        }
    }

    strcpy(last_mapname,mapname);
    show_bg = show_bg_save;
}

void gfxSetBG(const char *texname)
{
    AtlasTex *bind;

    sIsLoadingTipStale = true;
    texLoadQueueDefer();
    bind = atlasLoadTextureEx(texname, 1);
    if (!bind || bind == white_tex_atlas)
        bind = atlasLoadTextureEx(getDefaultLoadScreenName(), 1);
    bg_sprite = bind;
    texLoadQueueResume();
}

void loadBG()
{
    gfxSetBG(getDefaultLoadScreenName());
}

static AtlasTex *mapDebug[3];

void setBackGroundMapDebug( AtlasTex * t1, AtlasTex *t2, AtlasTex *t3 )
{
    mapDebug[0] = t1;
    mapDebug[1] = t2;
    mapDebug[2] = t3;
}

static void updateLoadScreenTimes()
{
    sLastOpaqueLoadScreenTime = timerCpuTicks();

    if (global_state.global_frame_count - sLastOpaqueLoadScreenFrame > 1)
        sFirstOpaqueLoadScreenTime = sLastOpaqueLoadScreenTime;

    sLastOpaqueLoadScreenFrame = global_state.global_frame_count;
}

static void drawLoadingTipIfNecessary(int z) 
{
    if (game_state.mission_map && !comicLoadScreenEnabled() 
        && bg_sprite != atlasLoadTexture(getDefaultLoadScreenName()))
    {
        if (sIsLoadingTipStale)
        {
            nextLoadingTip();
            sIsLoadingTipStale = false;
        }

        displayLoadingTip( z+11 );
    }
}

extern int glob_have_camera_pos;

void showBG(void)
{
     int        bytes_to_load = max_bytes_to_load;
    F32        wd=400, y = 720;
    int        w, h, z = 6000;
    float    screenScaleX, screenScaleY;
    static bool in_here=false;
    BasicTexture *testtex = 0;
    F32        finalXScale, finalYScale;

    if (in_here)
        return;

    if (!show_bg || !bg_sprite )
        return;

    in_here = true;
    PERFINFO_AUTO_START("showBG", 1);

    screenScaleX = screenScaleY = 1.f;
    w = DEFAULT_SCRN_WD;
    h = DEFAULT_SCRN_HT;
 
    scissor_dims(0, 0, w, h);

    commGetData(); 
    if (glob_have_camera_pos != MAP_LOADED_FINISHING_LOADING_BAR)
    {
        // mildly blatant hack to get input working on the loading screen
        inpUpdate();
    }
    windowProcessMessages();

    display_sprite(bg_sprite, 0.f, 0.f, z, (F32)w/bg_sprite->width, (F32)h/bg_sprite->height, CLR_WHITE);
    finalXScale = finalYScale = 1.f;

    if( mapDebug[0] )
    {
        display_sprite( mapDebug[0],0,0, z+1,2.f,2.f,0xffffffff);
        display_sprite( mapDebug[1],0,0, z+2,2.f,2.f,0xffffffff);
        display_sprite( mapDebug[2],0,0, z+3,2.f,2.f,0xffffffff);
    }

    drawLoadingTipIfNecessary(z);

    if (show_bg > 0)
    {
        int eff_bytes_loaded = total_bytes_loaded;
        int wereCollisionsOff = collisions_off_for_rest_of_frame;
        static HybridElement sButtonEnter = {0, "Neutral_PlayGameString", "Neutral_PlayGameString"};
        static HybridElement sButtonBack = {0, "BackString", "BackString"};
        static HybridElement sButtonNext = {0, "NextString", "NextString"};
        collisions_off_for_rest_of_frame = false; // YAY FOR HAX
        
        if (!load_bar_full)
            eff_bytes_loaded = MIN(eff_bytes_loaded, bytes_to_load*.98);
        else
            eff_bytes_loaded = MIN(eff_bytes_loaded, bytes_to_load);

        if (eff_bytes_loaded < bytes_to_load || !comicLoadScreenEnabled())
        {
            drawHealthBar((w - wd * finalYScale) / 2, h * ((1 + finalYScale) / 2) - 15, z+10, wd * finalYScale, eff_bytes_loaded, bytes_to_load, 1.f, load_bar_color, 0x0005ffff, HEALTHBAR_LOADING);
        }
        else if (!isMenu(MENU_GAME))
        {
            if (D_MOUSEHIT == drawHybridBarButton(&sButtonEnter, 512.f, 743.f, z+10, 150.f, 1.f, 0, 0))
            {
                hideLoadScreen();
            }
        }

        if (comicLoadScreenEnabled() && !isMenu(MENU_GAME))
        {
            if (g_comicLoadScreen_pageIndex > 0)
            {
                if (D_MOUSEHIT == drawHybridBarButton(&sButtonBack, 60.f, 743.f, z+10, 100.f, 1.f, 0, 0))
                {
                    g_comicLoadScreen_pageIndex--;
                    gfxSetBG(g_comicLoadScreen_pageNames[g_comicLoadScreen_pageIndex]);
                }
            }

            if (g_comicLoadScreen_pageIndex < eaSize(&g_comicLoadScreen_pageNames) - 1)
            {
                if (D_MOUSEHIT == drawHybridBarButton(&sButtonNext, 964.f, 743.f, z+10, 100.f, 1.f, 0, 0))
                {
                    g_comicLoadScreen_pageIndex++;
                    gfxSetBG(g_comicLoadScreen_pageNames[g_comicLoadScreen_pageIndex]);
                }
            }
        }

        collisions_off_for_rest_of_frame = wereCollisionsOff;
    }

    fontRenderGame();
    fontRenderEditor();
    init_display_list();
    windowUpdate();
    
    PERFINFO_AUTO_STOP();
    in_here = false;

    updateLoadScreenTimes();
}

void showBGAlpha(int coverAlpha)
{
    int        z = 15000.f;

    if (!bg_sprite )
        return;

    display_spriteFullscreenLetterbox(bg_sprite, z, coverAlpha, 0, 0);

    fontRenderGame();
    fontRenderEditor();
    init_display_list();

    if ((coverAlpha & 0xff) == 0xff)
        updateLoadScreenTimes();
}

#define MIN_TIME_BEFORE_UPDATE 20
void loadUpdate(char *msg,int num_bytes)
{
    static DWORD last_update = 0;

    if (!loadingScreenVisible() && game_state.game_mode != SHOW_LOAD_SCREEN)
        return;

    if (num_bytes>0)
    {
        total_bytes_loaded += num_bytes;
        last_update_bytes += num_bytes;
    }

    if (isDrawingSprites() || isDoingAtlasTex())
        return;
    
    if (GetTickCount() > last_update + MIN_TIME_BEFORE_UPDATE) 
    {
        static int in_here=0; // Prevent infinite recursion
        if (!in_here) 
        {
            last_update_bytes = 0;
            in_here = 1;
            last_update = GetTickCount();
            rdrClearScreen();
            showBG();
            in_here = 0;
        }
    }
}

// mild hack to get the loading bar to complete
void finishLoadScreen(void)
{
    load_bar_full = true;
    while(total_bytes_loaded < max_bytes_to_load)
    {
        loadUpdate("blarg",1000000);
        Sleep(1);
    }
}

// this fades out the load screen and prepares the client to function in game mode
void hideLoadScreen(void)
{
    startFadeInScreen(1);
    glob_have_camera_pos = PLAYING_GAME;
    start_menu( MENU_GAME );
    toggle_3d_game_modes( SHOW_GAME );
    init_map_optSettings();
}

void loadScreenResetBytesLoaded(void)
{
    total_bytes_loaded = 0;
    last_update_bytes = 1000001;
}

void showBgReset(void)
{
    show_bg = 0;
}

void showBgAdd(int val)
{
    show_bg += val;
    if (show_bg < 0)
        show_bg = 0;
}
//End Load Screen stuff
