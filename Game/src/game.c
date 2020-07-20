// support functions for main.c 

#include "win/win_init.h"
#include <utilitieslib/utils/winutil.h>
#include "UI/uiConsole.h"
#include "win/input.h"
#include "graphics/camera.h"
#include "graphics/gfxwindow.h"
#include "graphics/fontdata.h"
#include <utilitieslib/utils/timing.h>
#include "sound/sound.h"
#include "graphics/sun.h"
#include "edit/edit.h"
#include "clientcomm/clientcomm.h"
#include "entity/entrecv.h"
#include "player/player.h"
#include "edit/edit_cmd.h"
#include "graphics/FX/fx.h"
#include "render/render.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include "UI/uiGame.h"
#include "UI/uiQuit.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/fileutil.h>
#include "win/win_cursor.h"
#include "filter/profanity.h"
#include "gameData/store.h"
#include <utilitieslib/version/AppRegCache.h>
#include "edit/edit_net.h"
//#include ".\cerrno" // Hacky way to check to make sure someone doesn't add an invalid include path
#include "graphics/light.h"
#include "entity/entclient.h"
#include "gameComm/initCommon.h"
#include "gameComm/initClient.h"
#include <utilitieslib/network/sock.h>
#include "UI/sprite/sprite_font.h"
#include "entity/load_def.h" // for load_AllDefs
#include "gameData/costume_data.h" // for loadCostume
#include "gameData/menudef.h"  // for load_MenuDefs
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/FolderCache.h>
#include "UI/uiLogin.h"
#include <utilitieslib/language/AppLocale.h>
#include "language/langClientUtil.h"
#include "clientcomm/autoResumeInfo.h"
#include "clientcomm/dbclient.h"
#include "gameData/randomCharCreate.h"
#include "UI/uiReticle.h" // for neighborhoodCheck
#include "UI/uiAutomap.h"
#include <utilitieslib/version/AppVersion.h>
#include "demo.h"
#include "clientError.h"
#include "gameComm/dooranimclient.h"
#include "UI/uiKeybind.h"
#include "UI/uiKeymapping.h"
#include <utilitieslib/utils/sysutil.h>
#include "UI/uiHelp.h"
#include "filter/titles.h"
#include "graphics/seqgraphics.h"
#include "edit/edit_drawlines.h" //for sphere drawing
#include "graphics/clothnode.h"
#include "gridcoll/gridcache.h"
#include "UI/uiChat.h"
#include "gameComm/chatClient.h"
#include "UI/uiWindows_init.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uidialog.h"
#include "render/texWords.h"
#include "graphics/textureatlas.h"
#include "render/tex.h"
#include "render/texUnload.h"
#include "UI/uiGender.h" // for disabling of bone scale sliders
#include "imageServer.h"
#include "render/renderprim.h"
#include "seq/modelReload.h"
#include "entity/entity.h"
#include "cmdparse/cmdcontrols.h"
#include "seq/anim.h"
#include <utilitieslib/components/SharedMemory.h>
#include "NovodeX/NwWrapper.h"
#include "bases/baseedit.h"
#include "bases/basedata.h"
#include "graphics/gfx.h"
#include "graphics/gfxDebug.h"
#include "UI/sprite/sprite_base.h"
#include "graphics/gfxLoadScreens.h"
#include "auth/authUserData.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiBaseInput.h"
#include "seq/seqstate.h"
#include "group/groupfilelib.h"
#include <utilitieslib/utils/strings_opt.h>
#include "render/dxtlibwrapper.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/utils/cpu_count.h>
#include <utilitieslib/utils/osdependent.h>
#include "entity/character_inventory.h"
#include "entity/powers.h"
#include "MissionControl.h"
#include "graphics/FX/fxlists.h"
#include "fxinfo.h"
#include "editorUI.h"
#include "group/group.h"
#include <utilitieslib/utils/mutex.h>
#include <utilitieslib/utils/filewatch.h>
#include "graphics/FX/fxdebris.h"
#include "UI/uiWindows.h"
#include "seq/AutoLOD.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "account/AccountCatalog.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiOptions.h"
#include "UI/uiPlayerNote.h"
#include "UI/uiLoadingTip.h"
#include "storyarc/missionMapCommon.h"
#include "storyarc/pnpcCommon.h"
#include "UI/uiMissionMakerScrollSet.h"
#include "render/renderSSAO.h"
#include "render/rendershadowmap.h"
#include "render/cubemap.h"
#include "render/renderstats.h"
#include "render/perfcounter.h"
#include "ui/uiPopHelp.h"
#include "render/rendershadowmap.h"
#include <utilitieslib/UtilsNew/profiler.h>
#include <utilitieslib/utils/ssemath.h>
#include "UI/uiAuction.h"
#include "UI/uiSalvageOpen.h"
#include "storyarc/zowieClient.h"
#include "win/hwlight.h"
#include "player/inventory_client.h"
#include <utilitieslib/utils/log.h>
#include "LWC.h"
#include <utilitieslib/utils/RegistryReader.h>
#include "UI/uiCursor.h"
#include <utilitieslib/network/crypt.h>
#include "game.h"
#include "entity/costume_client.h"
#include "entity/character_eval.h"
#include "entity/character_combat_eval.h"

extern int    g_win_ignore_popups;
extern int do_map_xfer;
int globMapLoadedLastTick = 0;
//int globPastStartUp = 0;
bool consoleStarted = false;
static progressStringUpdatingEnabled = false; // starts off disabled so we don't overwrite the previous strings
static char testGameProgressString[100] = "";
static bool forceCrashCheck = false;

ParticleSystemInfo sysInfo;

#define MAX_HANDLES 65535 //currently can't be set any higher
#define UPDATE_PROGRESS_STRING(X) loadstart_printf(X); game_setProgressString(X, NULL, PROGRESSDIALOGTYPE_OK);

void parseArgs(int argc,char **argv)
{
    int    i;
    char    buf[1000];

    if (isDevelopmentMode())
        cmdAccessOverride(9);
    for(i=1;i<argc;)
    {
        if (strEndsWith(argv[i],".cohdemo") || strEndsWith(argv[i],".cohdemo\""))
        {
            sprintf(buf,"demoplay %s",argv[i]);
            cmdParse(buf);
            i++;
            continue;
        }
        if (strEndsWith(argv[i],".texture") || strEndsWith(argv[i],".texture\""))
        {
            sprintf(buf,"texWordEditor %s",argv[i]);
            cmdParse(buf);
            i++;
            continue;
        }
        if (argv[i][0] != '-' && argv[i][0] != '+')
        {
            i++;
            continue;
        }
        strcpy(buf,&argv[i][1]);
        for(i=i+1;i<argc;i++)
        {
            if (!argv[i][0] || argv[i][0] == '-' || argv[i][0] == '+')
                break;
            strcat(buf," ");
            strcat(buf,argv[i]);
        }
        if (argv[i-1][0] == '-')
        {
            //if(argv[i-1][1] == '!')
            //{
            //    continue;
            //}
                
            strcat(buf," 1");
        }
        //printf("%s\n",buf);
        if (!cmdParse(buf)) {
            //winMsgAlert(textStd("CommandLineErr", buf));
        }
    }
    cmdAccessOverride(0);

    //    fileAddSearchPath("./data");
    //    if(!game_state.nodebug){
    //        fileAddSearchPath("c:/game_edit/data");
    //    }
}


void enter_3d_game()
{
    game_state.fov_custom = 0;
    camLockCamera( false );
}

/*
Toggles Between
SHOW_GAME (show everything),
SHOW_TEMPLATE (show only the player with special camera and lighting),
SHOW_NONE (show nothing)
Moves player from shell to game and back, while in the shell, it remembers the
player's old pyr (position shouldn't change) and returns him there when returned to the game.
(Much of the work is done when gfx TreeDraw checks game_state.game_mode, which is set in this function)
*/

Vec3 cam_pos =  {3.4, 3.1, 20.5};

void changeShellCamera( float x, float y, float z )
{
    setVec3( cam_pos, x, y, z );
}


void toggle_3d_game_modes(int mode)
{
    static int player_state = PLAYER_IS_IN_UNKNOWN;
    static Mat4 old_plr_pos;

    Vec3    plr_pos;
    Entity  * player;
    int        i;

    game_state.game_mode = mode;

    player = playerPtr();
    if(!player)
        return;

    if(mode == SHOW_GAME)
    {
        //return to old pyr
        //PLAYER_IS_IN_SHELL is a cheesy way of knowing you just came out of the shell
        if( player_state == PLAYER_IS_IN_SHELL )
        {
            Entity *pfs = playerPtrForShell(0);

            //copyMat4(old_plr_pos, player->mat);

            //show all entities
            for( i = 1 ; i  < entities_max ; i++)
                if(entity_state[i] & ENTITY_IN_USE )
                    SET_ENTHIDE_BY_ID(i) = 0;
            if (pfs) {
                SET_ENTHIDE(pfs) = 1;
                entFree(pfs);
            }

            //turn off special camera
            camLockCamera( false );
            game_state.fov_custom = 0;

            player_state = PLAYER_IS_IN_GAME;
        }

        //engine = draw all (done in gfx TreeDraw)
    }
    if(mode == SHOW_TEMPLATE)
    {
        Vec3 cam_pos_internal;
        Entity *pfs = playerPtrForShell(1);
        Vec3 newpfspos;

        //show only the player
        for( i = 1 ; i  < entities_max ; i++) //only kinda works...
            if(entity_state[i] & ENTITY_IN_USE && !entities[i]->showInMenu )
                SET_ENTHIDE_BY_ID(i) = 1;
        //playerHide(0);
        SET_ENTHIDE(pfs) = 0;

        //get old pyr - JE: doesn't work since it does it every frame
        copyMat4(ENTMAT(player), old_plr_pos);
        //copyMat4(player->mat, pfs->mat);
        setVec3(newpfspos, 20000, 20000, 20000);
        entUpdatePosInterpolated(pfs, newpfspos);

        //set pyr to 0
        if ( player_state != PLAYER_IS_IN_SHELL )
            playerTurn(0);

        //turn on special camera
        //playerGetPos(plr_pos);
        copyVec3(ENTPOS(pfs), plr_pos);
        addVec3( cam_pos, plr_pos, cam_pos_internal ) ;

        // FIXME
        // I know this dumb, but too lazy to do it right! - BR
        // We should make it so that cam* functions don't implicitly move the player
        copyVec3(cam_pos_internal,cam_info.cammat[3]);
        copyMat3(unitmat,cam_info.cammat);

        camLockCamera( true );
        game_state.fov_custom = 30;

        player_state = PLAYER_IS_IN_SHELL;

    }
}

void updateGameTimers()
{
    game_state.client_frame++;
    updateClientFrameTimestamp();

    game_state.client_age += TIMESTEP;

    game_state.client_loop_timer += TIMESTEP;
    if (game_state.client_loop_timer > 65536.f)
        game_state.client_loop_timer -= 65536.f;
}

void engine_update_net() {
    int ent_count;

    //Connection stuff (Must be outside the renderer)
    dbPingIfConnected();
    commTickTroubledMode();

    if (commConnected() || control_state.alwaysmobile)
    {
        PERFINFO_AUTO_START("commConnected",1);
        //if(game_state.game_mode == SHOW_GAME)
        //{
        PERFINFO_AUTO_START("playerGetInput",1);
        playerGetInput();
        PERFINFO_AUTO_STOP();
        //}

        if(control_state.alwaysmobile && !commConnected())
        {
            destroyControlStateChangeList(&control_state.csc_processed_list);
        }

        PERFINFO_AUTO_START("commSendInput",1);
        if(control_state.mapserver_responding)
        {
            commSendInput();
        }
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("commCheck",1);
        commCheck(0);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("entNetUpdate",1);
        ent_count = entNetUpdate(); //mw moved this out of display(). Hopefully nothing breaks
        if(game_state.info)
        {
            int entCount[ENTTYPE_COUNT];
            int entTotal = 0;
            int entCountFade[ENTTYPE_COUNT];
            int entTotalFade = 0;
            int i;
            int y;

            memset(entCount, 0, sizeof(entCount));
            memset(entCountFade, 0, sizeof(entCountFade));

            for(i = 1; i < entities_max; i++){
                if(entities[i] && entInUse(i)){
                    if(entities[i]->fadedirection == ENT_FADING_OUT)
                    {
                        entCountFade[ENTTYPE_BY_ID(i)]++;
                        entTotalFade++;
                    }
                    else
                    {
                        entCount[ENTTYPE_BY_ID(i)]++;
                        entTotal++;
                    }
                }
            }

            y = 12;
            xyprintf(5,y++,"%3d repro states",control_state.csc_processed_list.count);
            xyprintf(5,y++,"%3d ents    live   fadeout",ent_count);
            xyprintf(7,y++,"players:  %3d    %d",entCount[ENTTYPE_PLAYER],entCountFade[ENTTYPE_PLAYER]);
            xyprintf(7,y++,"critters: %3d    %d",entCount[ENTTYPE_CRITTER],entCountFade[ENTTYPE_CRITTER]);
            xyprintf(7,y++,"npcs:     %3d    %d",entCount[ENTTYPE_NPC],entCountFade[ENTTYPE_NPC]);
            xyprintf(7,y++,"cars:     %3d    %d",entCount[ENTTYPE_CAR],entCountFade[ENTTYPE_CAR]);
            xyprintf(7,y++,"doors:    %3d    %d",entCount[ENTTYPE_DOOR],entCountFade[ENTTYPE_DOOR]);
            xyprintf(7,y++,"total:    %3d    %d",entTotal,entTotalFade);
        }
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
    }
    else
    {
        control_state.mapserver_responding = 0;
        if (game_state.game_mode == SHOW_LOAD_SCREEN) {
            commCheck(0); // Get the boot to login screen code called
        }
    }
}

// If we're in development mode, and we're working on the Release branch, let them know!
void showBranchWarning()
{
    if (isDevelopmentMode()) {
        char    titleBuffer[1000];
        sprintf(titleBuffer, "City of Heroes : PID: %d", _getpid());
        winSetText(titleBuffer);
    }
}


static void displayTestValues(void)
{
    int y = 12;
    static F32 hist[10][10];
    int i;
#define FLOAT_FROMAT " %6.3f"
#define DISPLAY_COLOR (255-(i%2)*40)
#define DISPLAY_IT(num1)    \
    xyprintf(60,++y,"test" #num1 ":" FLOAT_FROMAT,game_state.test##num1);    \
    for (i=0; i<ARRAY_SIZE(hist[num1-1]); i++)                                \
    xyprintfcolor(60+16+i*2, y, DISPLAY_COLOR,DISPLAY_COLOR,DISPLAY_COLOR,"%1.f", hist[num1-1][i]);                        \
    memmove(&hist[num1-1][1], &hist[num1-1][0], sizeof(F32)*(ARRAY_SIZE(hist[num1-1])-1));    \
    hist[num1-1][0] = game_state.test##num1;
    DISPLAY_IT(1);
    DISPLAY_IT(2);
    DISPLAY_IT(3);
    DISPLAY_IT(4);
    DISPLAY_IT(5);
    DISPLAY_IT(6);
    DISPLAY_IT(7);
    DISPLAY_IT(8);
    DISPLAY_IT(9);
    DISPLAY_IT(10);
    xyprintf(60,++y,"testVec1:" FLOAT_FROMAT FLOAT_FROMAT FLOAT_FROMAT,game_state.testVec1[0], game_state.testVec1[1], game_state.testVec1[2]);
    xyprintf(60,++y,"testVec2:" FLOAT_FROMAT FLOAT_FROMAT FLOAT_FROMAT,game_state.testVec2[0], game_state.testVec2[1], game_state.testVec2[2]);
    xyprintf(60,++y,"testVec3:" FLOAT_FROMAT FLOAT_FROMAT FLOAT_FROMAT,game_state.testVec3[0], game_state.testVec3[1], game_state.testVec3[2]);
    xyprintf(60,++y,"testVec4:" FLOAT_FROMAT FLOAT_FROMAT FLOAT_FROMAT,game_state.testVec4[0], game_state.testVec4[1], game_state.testVec4[2]);
}

////////////////////////////////////////////////////////////////////////////
//

static void updateDesaturateSetting()
{
    float dt =  global_state.frame_time_30 / 30.0f;

    if (loadingScreenVisible())
        return;

    if (game_state.desaturateDelay > 0.0f)
    {
        game_state.desaturateDelay -= dt;
    } 
    else if (game_state.desaturateWeightTarget != game_state.desaturateWeight )
    {
        if (game_state.desaturateFadeTime <= 0.0f)
        {
            game_state.desaturateWeight = game_state.desaturateWeightTarget;
            game_state.desaturateFadeTime = 0.0f;
        } else {
            float desaturateIncrement = (game_state.desaturateWeightTarget - game_state.desaturateWeight) / game_state.desaturateFadeTime;
            if (ABS(game_state.desaturateWeightTarget - game_state.desaturateWeight) < desaturateIncrement * dt)
            {
                game_state.desaturateWeight = game_state.desaturateWeightTarget;
                game_state.desaturateFadeTime = 0.0f;
            } else {
                game_state.desaturateWeight +=  dt * desaturateIncrement;
                game_state.desaturateFadeTime -= dt;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////

static void runServeMenus()
{
    if(!demoIsPlaying() && !editMode() && !(game_state.stopInactiveDisplay && game_state.inactiveDisplay) && !(game_state.game_mode==SHOW_LOAD_SCREEN))
    {
        if(playerPtr() || (!isMenu(MENU_REGISTER) && !isMenu(MENU_GAME)))
        {
            PERFINFO_AUTO_START("serve_menus", 1);
            serve_menus();
            PERFINFO_AUTO_STOP_CHECKED("serve_menus");
        }
    }
}

static void runServeEditorMenus()
{
    if(!demoIsPlaying() && !(game_state.stopInactiveDisplay && game_state.inactiveDisplay) && !(game_state.game_mode==SHOW_LOAD_SCREEN))
    {
        if(playerPtr() || (!isMenu(MENU_REGISTER) && !isMenu(MENU_GAME)))
        {
            PERFINFO_AUTO_START("serve_editor_menus", 1);
            serve_menus();
            PERFINFO_AUTO_STOP_CHECKED("serve_editor_menus");
        }
    }
}

#include "group/groupfileload.h"


void engine_update()
{
    rdrBeginMarkerFrame();
    rdrBeginMarker(__FUNCTION__);
    PERFINFO_AUTO_START("top", 1);
        updateGameTimers();

        PERFINFO_AUTO_START("inp/winproc/keycheck",1);
            //Get all player input (keep this order)
            PERFINFO_AUTO_START("inpUpdate",1);
            inpUpdate();                // Mouse and keyboard input meant as commands, mostly
            PERFINFO_AUTO_STOP_START("windowProcessMessages",1);
            windowProcessMessages();    // Keyboard input for chat and other windows messages, mostly.

            gfxDebugInput();    // grab input for graphics debug services

            PERFINFO_AUTO_STOP_START("bindCheckKeysReleased",1);
            bindCheckKeysReleased();    // mw moved this out of display().  Hopefully nothing breaks
            PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("engine_update_net", 1);
            //Connection stuff (Must be outside the renderer)
            engine_update_net();
        PERFINFO_AUTO_STOP();

        // let the debug/dev systems update now with latest input before
        // we let the masses update
        gfxDebugUpdate();


        PERFINFO_AUTO_START("demoCheckPlay", 1);
            demoCheckPlay();
        PERFINFO_AUTO_STOP();

        if(game_state.testDisplayValues)
        {
            displayTestValues();
        }

        runServeMenus();

        // Check background loaders and tag loaded stuff as ready for use. This function times itself.
        
        texCheckThreadLoader();
        
        // texUnloadTexturesToFitMemory();
        
        PERFINFO_AUTO_START("texGatherStatistics",1);
            if (game_state.info || window_getMode(WDW_RENDER_STATS) == WINDOW_DISPLAYING)
                texGatherStatistics();
        PERFINFO_AUTO_STOP_START("geoCheckThreadLoader",1);
            geoCheckThreadLoader();
        PERFINFO_AUTO_STOP();

        gfxShowDebugIndicators();

        atlasDoFrame();

        updateDesaturateSetting();

        PERFINFO_AUTO_START("camUpdate",1);
        // Set the camera target.

         if(editMode() || !DoorAnimFrozenCam())
        {
            if(!editMode() && !control_state.detached_camera)
            {
                Entity* player = playerPtr();
                if (game_state.game_mode == SHOW_TEMPLATE)
                    player = playerPtrForShell(1);

                if(erGetEnt(game_state.camera_follow_entity))
                {
                    Entity* follow_ent = erGetEnt(game_state.camera_follow_entity);

                    camSetPos( ENTPOS(follow_ent), true );
                }
                else if(player)
                {
                    game_state.camera_follow_entity = 0;

                    camSetPos( ENTPOS(player), true );

                    copyMat3(ENTMAT(player), cam_info.mat);
                }
            }
            camUpdate();
            gfxSetViewMat(cam_info.cammat, cam_info.viewmat, cam_info.inv_viewmat);
        }
        PERFINFO_AUTO_STOP();

        // entClientProcess was here

        globMapLoadedLastTick = 0;

        PERFINFO_AUTO_START("neighborhoodCheck",1);

        neighborhoodCheck();

        PERFINFO_AUTO_STOP();

        if (MissionControlMode())
        {
            MissionControlMain();
        }

        if (baseedit_Mode())
        {
            Mat4 sound_mat;

            game_state.camera_shake = 0;

            PERFINFO_AUTO_START("sndUpdate - edit",1);
            copyMat4(cam_info.cammat, sound_mat);

            scaleMat3(sound_mat, sound_mat, -1);
            sndUpdate(sound_mat);
            PERFINFO_AUTO_STOP();

            sndVolumeControl( SOUND_VOLUME_FX, game_state.fxSoundVolume );

             baseedit_Process();
            inpLockMouse(control_state.canlook );
        }
        else if (editMode())
        {
            Mat4 sound_mat;

            game_state.camera_shake = 0;

            PERFINFO_AUTO_START("sndUpdate - edit",1);
            runServeEditorMenus();
            copyMat4(cam_info.cammat, sound_mat);
            scaleMat3(sound_mat, sound_mat, -1);
            sndUpdate(sound_mat);
            PERFINFO_AUTO_STOP();

            PERFINFO_AUTO_START("editProcess",1);
            editProcess();
            PERFINFO_AUTO_STOP_CHECKED("editProcess");
        }
        else
        {
            Mat4 sound_mat;

            PERFINFO_AUTO_START("sndUpdate - game",1);

            copyMat3(cam_info.cammat, sound_mat); 
            scaleMat3(sound_mat, sound_mat, -1);

            if( game_state.viewCutScene )
                copyVec3( game_state.cutSceneCameraMat[3], sound_mat[3] );
            else
            {
                copyVec3( cam_info.mat[3], sound_mat[3]);
                sound_mat[3][1] += playerHeight() / 2;
            }

            if( !isMenu(MENU_REGISTER) )
                sndUpdate(sound_mat);

            sndVolumeControl( SOUND_VOLUME_FX, game_state.fxSoundVolume );
            PERFINFO_AUTO_STOP();

            inpLockMouse(control_state.canlook || control_state.cam_rotate ); //TO DO rename canlook "mouse_look_on".  Needs to be south of playerGetInput, I don't know why
        }

        PERFINFO_AUTO_START("conProcess",1);
        conProcess(); //Handle the in-game ~ console
        PERFINFO_AUTO_STOP();
    
    PERFINFO_AUTO_STOP();// Ends "top"

#if NOVODEX
    if (nwEnabled() && TIMESTEP > 0.0000001f )
    {
        PERFINFO_AUTO_START("NovodeX Update", 1);
        nwStepScenes(TIMESTEP / 30.0f);
        PERFINFO_AUTO_STOP();
    }
#endif

/*    if(game_state.create_map_images)
    {
        //toggle_3d_game_modes( SHOW_GAME );
        groupLoadMakeAllMapImages("");
        game_state.create_map_images = 0;
    }*/

    if (!editMode())
    {
        void editCmdSearchVisible(void);
        if (game_state.search_visible)
            editCmdSearchVisible();
    }
    PERFINFO_AUTO_START("sunUpdate",1);
        sunUpdate(&g_sun, 0);
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("rdrAmbientUpdate",1);
    rdrAmbientUpdate();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("rdrShadowMapUpdate",1);
    rdrShadowMapUpdate();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("entClientProcess",1);
    entClientProcess();
    PERFINFO_AUTO_STOP();

    hwlightTick();

    PERFINFO_AUTO_START("gfxUpdateFrame",1);
        //Render (everything from here down should be non-destructive, so it could be called multiple times per frame)
        if(game_state.stopInactiveDisplay && game_state.inactiveDisplay)
            gfxDoNothingFrame();    // Don't draw if the window isn't visible. plus, go to sleep for a bit
        else
            gfxUpdateFrame(0, 0, 0);
    PERFINFO_AUTO_STOP_CHECKED("gfxUpdateFrame");

    PERFINFO_AUTO_START("bottom",1);
        PERFINFO_AUTO_START("seqClearRandomNumberUpdateFlag",1);
        //For rough synchronizing of world fx across clients
        seqClearRandomNumberUpdateFlag();
        PERFINFO_AUTO_STOP();

        if( game_state.animdebug & ANIM_SHOW_USEFLAGS )
            showAnimationDebugInfo();

        PERFINFO_AUTO_START("waitFps",1);
            //If the frame rate is locked, slow down to match it.
            if (game_state.maxInactiveFps && game_state.inactiveDisplay) {
                int fps = MIN(game_state.maxInactiveFps, game_state.maxfps);
				waitFps( fps ? fps : game_state.maxInactiveFps);
			} else if (!isMenu(MENU_GAME) && game_state.maxMenuFps) {
				int fps = MIN(game_state.maxMenuFps, game_state.maxfps);
				waitFps( fps ? fps : game_state.maxMenuFps);
            } else {
                waitFps( game_state.maxfps);
            }
        PERFINFO_AUTO_STOP();

        //See if you need to change maps, if so, do it.
        if (do_map_xfer)
        {
            do_map_xfer = 0;
            doMapXfer();
        }
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("seqProcessRequestedHeadshots", 1);
        seqProcessRequestedHeadshots();
    PERFINFO_AUTO_STOP();

    popHelpTick();
    zowieSoundTick();
    rdrEndMarker();
}

int engine_shutdown(void)
{
    return perfCounterShutdown();
}

//Not called right now...
static FileScanAction animLoadCallback(char* dir, struct _finddata32_t* data)
{
    char buffer[512];
    int len;
    int isanm = 0;
    int isseq = 0;

    if(!(data->name[0] == '_'))
    {

        len = strlen(data->name);
        if( len > 4 && !stricmp(".geo", data->name+len-4) )
            isanm = 1;

        sprintf(buffer, "%s/%s", dir, data->name);

        if( isanm )
        {
            sprintf(buffer, "player_library/%s", data->name);
            //TO DO: fix this hackery
            if(!stricmp(data->name, "male.geo") || !stricmp(data->name, "fem.geo") || !stricmp(data->name, "huge.geo"))
                geoLoad( buffer, LOAD_HEADER, (GeoUseType)(GEO_DONT_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE));
            else
                geoLoad( buffer, LOAD_HEADER, (GeoUseType)(GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE));
            printf(".a.");
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

//Not called right now
void animPreLoadCharacterModelHeaders()
{
    printf("PreLoading Player Library Headers");
    fileScanAllDataDirs("player_library", animLoadCallback);
    printf("done\n");
}


/* Function at_exit_func()
*    This function is registered as the "atexit" function for the program.
*    Release all system resources in here.
*/
void GameShutdown(void)
{
    engine_shutdown();
    packetShutdown();
    InputShutdown();
    rdrRestoreGammaRamp();
    //TO DO: Add a "game was shut down properly" registry flag.
    //autoTimerFlushFileBuffers();
}

void checkQuickLogin(void)
{
    char    err_msg[1000];

    if (game_state.quick_login && !game_state.editnpc) {
        //serve_menus(); // Needed to load the graphics used in the loading bar? //JE: removed because it was messing up resume_info.txt by catching keypresses
        if (loginToAuthServer(0) && loginToDbServer(0,err_msg) && db_info.players &&  (game_state.quick_login - 1) < db_info.player_count) {
            int newchar;
            gPlayerNumber = game_state.quick_login - 1;
            newchar = stricmp(db_info.players[gPlayerNumber].name, "empty")==0;
            if (!newchar) {
                if(!choosePlayerWrapper( gPlayerNumber, 0 )) {
                    Sleep(30);
                    if (!choosePlayerWrapper( gPlayerNumber, 0 )) {
                        return;
                    }
                }
                dbDisconnect();           //Character being resumed, no longer need DbServer.
                doCreatePlayer(0);
                player_being_created = 0;
                tryConnect();
                if( commConnected() ) {
                    commReqScene(0);
                }
            } else {
                // New character
                simulateCharacterCreate(0, 0);
            }
        }
        UPDATE_PROGRESS_STRING("Waiting for mapserver update.." );
    }
}

void checkFullScreenToggle(void)
{
    static bool init = false;
    static int cur_fullscreen;
    static int cur_screenX;
    static int cur_screenY;
    static int cur_refreshRate;
    bool doResize = false;

    if (!init) {
        game_state.fullscreen_toggle = game_state.fullscreen;
        cur_fullscreen = game_state.fullscreen;
        cur_screenX = game_state.screen_x;
        cur_screenY = game_state.screen_y;
        cur_refreshRate = game_state.refresh_rate;
        init = true;
    }
    if (cur_fullscreen != game_state.fullscreen) {
        // User changed fullscreen (via options, etc)
        game_state.fullscreen_toggle = cur_fullscreen = game_state.fullscreen;
    }

    if (game_state.fullscreen != game_state.fullscreen_toggle)
        doResize = true;

    if (game_state.fullscreen_toggle && (game_state.refresh_rate != cur_refreshRate ||
                                         game_state.screen_x != cur_screenX || 
                                         game_state.screen_y != cur_screenY))
        doResize = true;

    if (doResize)
    {
        PERFINFO_AUTO_START("windowResizeToggle", 1);
        windowResizeToggle(&game_state.screen_x,&game_state.screen_y,&game_state.refresh_rate,game_state.screen_x_pos,game_state.screen_y_pos, game_state.fullscreen_toggle, game_state.fullscreen);
        PERFINFO_AUTO_STOP_START("gfxWindowReshape", 1);
        gfxWindowReshape();
        PERFINFO_AUTO_STOP();
        cur_fullscreen = game_state.fullscreen = game_state.fullscreen_toggle;
        cur_screenX = game_state.screen_x;
        cur_screenY = game_state.screen_y;
        cur_refreshRate = game_state.refresh_rate;
    }
}

void checkOddball()
{
#ifndef FINAL
    if (game_state.spin360)
        gfxDump360();
#endif

    DoorAnimClientCheckMove();
    checkDoneResumingMapServerEntity();
    checkModelReload();
    checkTrickReload();
    checkLODInfoReload();
    reloadSounds();
    checkFullScreenToggle();

    if(game_state.reloadSequencers)
    {
        entReloadSequencersClient();
        fxReloadSequencers();
        game_state.reloadSequencers = 0;
    }
    
    // disabled 10/27/09, shouldn't need this -- checkForCorruptTexturesATI();

    mpCompactPools();

#ifdef ENABLE_LEAK_DETECTION
    GC_collect_a_little();
#endif
}

#include "UI/uiUtil.h"
#include "UI/sprite/sprite_text.h"
void checkLogoutProgress()
{
    Entity *p = playerPtr();
    int        i;

    if (game_state.cryptic) {
        //        saveOnExit();
    }

    if (p && p->logout_timer)
    {
        int quitSeconds = (int)(p->logout_timer / 30.f - 1.0f);
        int kickSeconds = MissionKickSeconds();
        extern void         commSendQuitGame(int abort_quit);
        //        saveOnExit();
        if (kickSeconds != -1 && kickSeconds < quitSeconds) // show kick text instead of quit text
        {
            CheckMissionKickProgress(1);
        }
        else
        {
            int h, w;
            windowSize(&w,&h);

            if( shell_menu() )
            {
                w = DEFAULT_SCRN_WD;
                h = DEFAULT_SCRN_WD;
            }

            font( &title_18 );
            font_color( CLR_WHITE, CLR_BLUE );
            cprnt( w / 2, h/4 - 30, 90, 1.f, 1.f, "DisconnectTime", quitSeconds > 0 ? quitSeconds : 0);
            if (quitSeconds >= 0)
            {
                cprnt( w / 2, h/4 + 30, 90, 1.f, 1.f, "DisconnectAbortPrompt");
            }
        }

        p->logout_timer -= TIMESTEP;
        if (p->logout_timer <= 0)
        {
            cursor.dragging = FALSE;
            if( !p->logout_login )
            {
                windowExit(0);
            }
            else if (p->logout_login == 2)
            {
                quitToCharacterSelect(0);
            }
            else // if (p->logout_login == 1)
            {
                quitToLogin(0);
                if (isProductionMode())
                {
                    // Encrypt an empty string
                    g_achPassword[0] = 0;
                    cryptStore(g_achPassword, g_achPassword, sizeof(g_achPassword));
                }
            }
        }
        if (quitSeconds >= 0 && GetFocus())
        {
            for(i=0;i<INP_KEY_LAST;i++)
            {
                if (inpEdge(i))
                {
                    printf("aborted by key: %d\n",i);
                    commSendQuitGame(1);
                    dialogRemove( NULL, windowExitDlg, NULL );
                    break;
                }
            }
        }
    }
    else if (g_showMapLoadingMessage)
    {
        int h, w;
        windowSize(&w,&h);

        if( shell_menu() )
        {
            w = DEFAULT_SCRN_WD;
            h = DEFAULT_SCRN_WD;
        }

        font( &title_18 );
        font_color( CLR_WHITE, CLR_BLUE );
        cprnt( w / 2, h/4 - 30, 90, 1.f, 1.f, "MapLoadingPleaseWait");
    }
    else
    {
        CheckMissionKickProgress(0);
    }
}

static void setAssertSystemInfo(void)
{
    char specBuf[SPEC_BUF_SIZE];
    rdrGetSystemSpecs( &systemSpecs );
    rdrGetSystemSpecString( &systemSpecs, specBuf );
    setAssertExtraInfo2(specBuf);
}

void game_setAssertSystemExtraInfo(void)
{
    char buffer[1000];

    if (comm_link.connected)
    {
        sprintf(buffer, "Map: %s\nServer IP: %s:%d\nClient IP: %s\nLWC Stage: %s", game_state.world_name, makeIpStr(comm_link.addr.sin_addr.S_un.S_addr), comm_link.addr.sin_port, makeIpStr(getHostLocalIp()), LWC_GetStageString());
    }
    else
    {
        sprintf(buffer, "Map: N/A\nServer IP: N/A\nClient IP: N/A\nLWC Stage: %s", LWC_GetStageString());
    }

    setAssertExtraInfo(buffer);
}

static void crashtest(void)
{
    // Remove the '!' to get one to go off
    if (!"access violation") {
        int *a=(int*)1;
        int b = *a;
    }
    if (!"stack corruption") {
        int a[4];
        a[-1] = -1;
        return; // Will trigger check on return
    }
    if (!"heap corruption") {
        char *data = (char*)malloc(5);
        data[-1] = -1;
        free(data);
    }
    if (!"heap overrun") {
        char *data = (char*)malloc(5);
        data[6] = -1;
        free(data);
    }
    if (!"assert") {
        assert(0);
        assertmsg(0, "die!");
    }
    if (!"exception") {
        int a = 0;
        int b = 1/a;
    }
}

#define CHECKARG(arg) (stricmp(argv[i],arg)==0)

int    totalCallsTo_listAddNewMember;

int getProjectKey(int argc, char **argv)
{
    int        i,ok_to_run=0;

    for(i=1;i<argc;i++)
    {
        if (CHECKARG("-project") && i < argc-1)
        {
            char* projName = argv[i+1];
            
            if (projName[0]=='\"') {
                strcpy(projName, projName+1);
            }
            if (strEndsWith(projName, "\"")) {
                projName[strlen(projName)-1] = 0;
            }

            regSetAppName(projName);

            // set training room if this is COHTEST (passed from updater)
            if(0 == stricmp(projName,"CohTest"))
            {
                game_state.trainingshard = 1;
                printf("trainingshard 1");
            }

            ok_to_run = 1;
        }
        else if (CHECKARG("-cryptic") || CHECKARG("-demoplay") || strEndsWith(argv[i],".cohdemo") || strEndsWith(argv[i],".cohdemo\""))
            ok_to_run = 1;
        else if (isDevelopmentMode() && (strEndsWith(argv[i],".texture") || strEndsWith(argv[i],".texture\"") || strEndsWith(argv[i],".tga") || strEndsWith(argv[i],".tga\"")))
            ok_to_run = 1;
    }
    return ok_to_run;
}

void parseArgsForCovFlag(int argc, char **argv)
{
    int        i;
    extern char *gameDataDirOverride;

    for(i=1;i<argc;i++)
    {
        if (CHECKARG("-cov"))
        {
            game_state.skin = UISKIN_VILLAINS;
            *argv[i] = '\0';
        }
        // Praetorian skin turned off for retail. We can't do the usual
        // GR lock check here because we haven't connected to anything
        // else that we could ask.
        else if (CHECKARG("-uiskin"))
        {
            *argv[i] = '\0';

            if (i < (argc - 1))
            {
                game_state.skin = atoi(argv[i + 1]);
                *argv[i + 1] = '\0';
                i++;

                if (game_state.skin < UISKIN_HEROES || game_state.skin > UISKIN_PRAETORIANS)
                    game_state.skin = UISKIN_HEROES;
            }

        }
        else if (CHECKARG("-setlocale"))
        {
            argv[i][0] = '\0';

            if (i < (argc - 1) && argv[i+1] && argv[i+1][0] != '-')
            {
                int locale = locGetIDByWindowsLocale(atoi(argv[i + 1]));
                if(locIDIsValidForPlayers(locale))
                    locSetIDInRegistry(locale);

                argv[i + 1][0] = '\0';
            }
        }
        else if (CHECKARG("-setregion"))
        {
            argv[i][0] = '\0';

            if (i < (argc - 1) && argv[i+1] && argv[i+1][0] != '-')
            {
                char *regionStr = argv[i + 1];
                if (stricmp(regionStr, "EU") == 0)
                    setCurrentRegion(REGION_EU);
                else if (stricmp(regionStr, "NA") == 0)
                    setCurrentRegion(REGION_NA);
                else
                    // Assume NA
                    setCurrentRegion(REGION_NA);

                argv[i + 1][0] = '\0';
            }
        }
        else if (CHECKARG("-testgameprogress"))
        {
            argv[i][0] = '\0';
            if (i < (argc - 1) && argv[i+1] && argv[i+1][0] != '-')
            {
                char *src = argv[i + 1];
                int length;

                // Skip leading quote
                if (src[0] == '"')
                    src++;

                length = strlen(src);

                if (length > 99)
                    length = 99;

                strncpy(testGameProgressString, src, length);

                // remove trailing quote
                if (testGameProgressString[length-1] == '"')
                    length--;

                testGameProgressString[length] = 0;

                argv[i + 1][0] = '\0';
            }
        }
        else if (CHECKARG("-forceCrashCheck"))
        {
            argv[i][0] = '\0';
            forceCrashCheck = true;
        }
    }
}

void parseArgs0(int argc, char **argv)
{
    int        i;

    // auto detect multi cpus to enable renderthread - needs to be before parseargs0 get processed so it can be disabled
     if (getNumRealCpus() > 1)
         renderThreadSetThreading(1);
     else
         renderThreadSetThreading(0);

    //writeConsole(OUTPUT_INFO, "num cpus = %d / %d", getNumRealCpus(), getNumVirtualCpus());

    for(i=1;i<argc;i++)
    {
        bool bSkipArgIfPresent = false;
        if (CHECKARG("-cod"))
        {
            printf("Using development data (d_).\n");
            FolderCacheRemoveIgnorePrefix("d_");
        }
        else if (CHECKARG("-renderthread"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
                renderThreadSetThreading(2); // Force it on regardless of driver version/CPUs
            else if (argv[i+1][0]=='0')
                renderThreadSetThreading(0);
        }
        else if (CHECKARG("-norenderthread"))
        {
            renderThreadSetThreading(0);
        }
        else if (CHECKARG("-novbos"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
                game_state.noVBOs = 1;
        }
        else if (CHECKARG("-noPBuffers"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
                game_state.noPBuffers = 1;
        }
        else if (CHECKARG("-texwordeditor"))
        {
            estrPrintCharString(&game_state.texWordEdit, "1");
            setCurrentLocale(LOCALE_ID_ENGLISH); // force english when in texwords mode
        }
        else if (CHECKARG("-nowinio"))
            fileDisableWinIO(1);
        else if (CHECKARG("-safemode") || CHECKARG("-safe_mode"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
            {
                game_state.safemode = 1;
                FolderCacheDisallowOverrides();
            }
        }
        else if (CHECKARG("-hidetrans"))
        {
            msSetHideTranslationErrors(1);
        }
        else if (CHECKARG("-nooverride"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
                FolderCacheDisallowOverrides();
        }
        else if (CHECKARG("-verbose"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
                errorSetVerboseLevel(1);
        }
        else if (CHECKARG("-createbins"))
        {
            if (i == argc-1 || argv[i+1][0]!='0')
                game_state.create_bins = 1;
        }
        else if (CHECKARG("-emailOnAssert"))
        {
            enableEmailOnAssert(1);
            // add all email addresses to email list
            if (argv[i+1][0]!='-')
            {
                argv[i][0] = 0;
                ++i;
                while (i <= argc-1 && argv[i][0] != '-')
                {
                    if ( argv[i][0] == '[' )
                    {
                        int len = strlen(argv[i]);
                        if ( argv[i][len-1] == ']' )
                            argv[i][len-1] = 0;
                        setMachineName(&argv[i][1]);
                    }
                    else
                        addEmailAddressToAssert(argv[i]);
                    argv[i][0] = 0;
                    if (argv[i+1] && argv[i+1][0] != '-')
                        ++i;
                    else
                        break;
                }
            }
        }
        else if (CHECKARG("-nopopups"))
        {
            g_win_ignore_popups = 1;
             bSkipArgIfPresent = true;
       }
        else if (CHECKARG("-skippraetoriantutorial") && isDevelopmentMode())
        {
            game_state.can_skip_praetorian_tutorial = 1;
            bSkipArgIfPresent = true;
        }
        else if (CHECKARG("-launcher"))
        {
            game_state.usedLauncher = 1;
            bSkipArgIfPresent = true;
        }
        else if (CHECKARG("-lwc"))
        {
            LWC_Enable();
            bSkipArgIfPresent = true;
        }
        else if (CHECKARG("-verify"))
        {
            game_state.doFullVerify = 1;
            bSkipArgIfPresent = true;
        }
        else if (CHECKARG("-noverify"))
        {
            game_state.noVerify = 1;
            bSkipArgIfPresent = true;
        }
        else if (CHECKARG("-ignoreBadDriver"))
        {
            game_state.ignoreBadDrivers = 1;
            bSkipArgIfPresent = true;
        }
        else if (CHECKARG("-patchDir"))
        {
            argv[i][0] = 0;
            if (i+1 < argc && argv[i+1])
            {
                forwardSlashes(argv[i+1]);
                PigSetAddPatchDir(unquote(argv[i+1]));

                argv[i+1][0] = 0;
                ++i;
            }
        }

        if(bSkipArgIfPresent)
        {
            // There's no slash command for this, and we don't want the later
            // arg parsers complaining. The arg parameter is not needed, but by
            // stripping it we gracefully handle people putting it in anyway.
            argv[i][0] = '\0';
            if (i < argc - 1 && argv[i + 1][0] != '-')
                argv[i + 1][0] = '\0';
        }
    }
}

void protectMemory(void* mem)
{
    MEMORY_BASIC_INFORMATION mbi;
    DWORD oldProtect;
    VirtualQuery(mem, &mbi, sizeof(mbi));
    VirtualProtect(mem, 1, mbi.Protect | PAGE_GUARD, &oldProtect);
}

static DWORD WINAPI keepMemoryThread(void* unusedParam){
    // MS: This is a crazy experiment to force all memory to stay in physical RAM and thus to not get paged.
    //     It does this by reading every page in the virtual page list.
    //     Games are supposed to be responsive, right?

    SYSTEM_INFO systemInfo;
    
    GetSystemInfo(&systemInfo);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL - 2);

    while(1){
        char* lastAddress = 0;
        char* address = 0;
        char* endAddress = (char*)(1 << 31);
        char* nextAddress;
        int acc = 0;
        int startTimer = 1;

        Sleep(3 * 1000);

        while(address != endAddress){
            MEMORY_BASIC_INFORMATION mbi;

            if(acc >= 64 * SQR(1024)){
                acc = 0;
                PERFINFO_AUTO_STOP();
                Sleep(100);
                startTimer = 1;
            }
            
            if(startTimer){
                startTimer = 0;
            }
        
            VirtualQuery(address, &mbi, sizeof(mbi));

            if(address != ((char*)mbi.BaseAddress)){
                break;
            }

            nextAddress = address + mbi.RegionSize;

            if(    mbi.Protect & (PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_READONLY|PAGE_READWRITE) &&
                !(mbi.Protect & (PAGE_GUARD)))
            {
                int stop = 0;

                PERFINFO_AUTO_START("keepMemory", 1);

                while(!stop && acc < 64 * SQR(1024) && address < nextAddress){
                    __try{
                        int x = *address;
                    }
                    __except(stop = 1, EXCEPTION_EXECUTE_HANDLER){}
                    address += systemInfo.dwPageSize;
                    acc += systemInfo.dwPageSize;
                }
                
                PERFINFO_AUTO_STOP();
                
                Sleep(0);
            }else{
                address = nextAddress;
            }
        }
    }
    return 0;
}

static void AttachConsoleIfAvail(void)
{
    HMODULE mod;
    mod = LoadLibraryA("kernel32.dll");
    if(mod) {
        FARPROC func = GetProcAddress(mod, "AttachConsole");
        if(func) {
            func((DWORD)-1); // ATTACH_PARENT_PROCESS
        }
    }
}

void parseArgsForConsole(int argc, char** argv) {
    int i;

    for (i = 1; i < argc; i++) {
        if (CHECKARG("-console")) { // Do this early!
            newConsoleWindow();
            consoleStarted = true;
        } else if (CHECKARG("-nogui")) {
            AttachConsoleIfAvail();
            newConsoleWindow();
            consoleStarted = true;
            ShowWindow(compatibleGetConsoleWindow(), SW_SHOW);

            setGuiDisable(true);
            g_win_ignore_popups = 1;
        } else if (CHECKARG("-showLoadMemUsage")) {
            setShowLoadMemUsage(true);
        }
    }
}

void startHiddenConsole()
{
    if (isDevelopmentMode() && !consoleStarted) // Must be after FolderCacheChooseMode
    {
        // Create and hide
        newConsoleWindow();
        ShowWindow(compatibleGetConsoleWindow(), SW_HIDE);
    }
}

static const char* cohCountMutexName = "COHCountMutex";

static void acquireCountMutex()
{
    if(!acquireMutexHandleByPID(cohCountMutexName, -1, NULL)){
        Sleep(1000);
        
        if(!acquireMutexHandleByPID(cohCountMutexName, -1, NULL)){
            assert(0);
        }
    }
}

S32 game_runningCohClientCount(void)
{
    return countMutexesByPID(cohCountMutexName, NULL);
}

void game_beforeFolderCacheIgnore(int timer, int argc, char **argv)
{
    parseArgsForConsole(argc, argv);

    parseArgsForCovFlag(argc, argv);

    windowInit();
    writeConsole(OUTPUT_INFO, "Project: Ouroboros");
    writeConsole(OUTPUT_INFO, "Git Commit Hash: %s", build_version);
    writeConsole(OUTPUT_INFO, "Running %s", argv[0]);

    acquireCountMutex();
    
    timeBeginPeriod(1);
    timerStart(timer);

    mpCompactPools();

    preloadDLLs(0);
//    if (isDevelopmentMode() && IsUsingWin2kOrXp())
//        trickGoogleDesktopDll();
}

static void checkForCrash()
{
    if (!isDevelopmentMode() || forceCrashCheck)
    {
        char progressUserString[100];
        PROGRESSDIALOGTYPE type = PROGRESSDIALOGTYPE_NONE;
        RegReader    rr;

        rr = createRegReader();
        initRegReader(rr, regGetAppKey());
        if (!rrReadString(rr, "GameProgressUserString", progressUserString, 100))
        {
            progressUserString[0] = 0;
        }

        if (!rrReadInt(rr, "GameProgressDialogType", (int *) &type))
        {
            type = PROGRESSDIALOGTYPE_NONE;
        }

        destroyRegReader(rr);

        if (progressUserString[0] != 0 && type != PROGRESSDIALOGTYPE_NONE)
        {
            int mbtype = MB_ICONEXCLAMATION | MB_SYSTEMMODAL;
            int mbresult;

            if (type == PROGRESSDIALOGTYPE_SAFEMODE)
                mbtype |= MB_YESNO;
            else
                mbtype |= MB_OK;


            mbresult = MessageBoxA(0, textStd(progressUserString), textStd("CrashDialogTitle"), mbtype );

            if (type == PROGRESSDIALOGTYPE_SAFEMODE && mbresult == IDYES)
            {
                game_state.safemode = 1;
            }
        }
    }

    // Now that we've checked the previous progress strings, allow them to be updated
    progressStringUpdatingEnabled = true;
}

void game_beforeParseArgs(int doLogging)
{
    if(game_state.create_bins)
        SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    else if(isProductionMode())
        SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    else
        SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

    FolderCacheChooseMode();
    FolderCacheEnableCallbacks(0);
    FolderCacheSetManualCallbackMode(1);
    sharedMemorySetMode(SMM_DISABLED);
    if (isGuiDisabled()) {
        bsAssertOnErrors(true);
        disableRtlHeapChecking(NULL);
        setAssertMode(ASSERTMODE_STDERR | ASSERTMODE_EXIT);
    } else if (fileIsUsingDevData()) {
        bsAssertOnErrors(true);
        disableRtlHeapChecking(NULL);
        setAssertMode((!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0) |
            ASSERTMODE_DEBUGBUTTONS);
    } 
    else 
    {
        // Production mode
        setAssertMode( ASSERTMODE_MINIDUMP | ASSERTMODE_ERRORREPORT | ASSERTMODE_CALLBACK );
        setAssertCallback(clientProductionCrashCallback);
    }
    setAssertProgramVersion(getExecutablePatchVersion(NULL));
    // This will setup the assert/report system with system information as we know
    // it up until this point. It will not however contain details from the GL context
    // at this point. We'll do the setup again once that information is available to make
    // the information more comprehensive.
    setAssertSystemInfo();

    startHiddenConsole();

    if(!locIDIsValidForPlayers(locGetIDInRegistry()))
        locSetIDInRegistry(DEFAULT_LOCALE_ID);

    // only allow default language (english) when in texWord mode
    if(!game_state.texWordEdit)
        setCurrentLocale(locGetIDInRegistry());

    // @todo ab: this should be enabled at some point, if it is safe.
    //setlocale(LC_CTYPE,"");
    writeConsole(OUTPUT_DEBUG, "Loading message stores");
    if (game_state.create_bins) {
        reloadClientMessageStores(LOCALE_ID_ENGLISH);
    } else {
        reloadClientMessageStores(getCurrentLocale());
    }
    writeConsole(OUTPUT_INFO, "Loaded message stores");

    // See if we crashed last time
    checkForCrash();

    // Look for known bad drivers and warn about old drivers
    rdrExamineDriversEarly(&systemSpecs);

    atexit(GameShutdown);
    //_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
    if (doLogging)
        logSetDir("game");

    errorLogStart();
    //pktSetDebugInfo();
    cmdOldCommonInit();
    gameStateInit();
    gameStateLoad(); // this must be before parseArgs
    gridCacheSetSize(12);

    determineIfSSEAvailable();

    game_setProgressString("INIT: Before renderThreadStart", "CrashPromptSafeMode", PROGRESSDIALOGTYPE_SAFEMODE);
    renderThreadStart();
    game_setProgressString("INIT: After renderThreadStart", NULL, PROGRESSDIALOGTYPE_OK);

    //Get last gfxSettings and last accountName from registry (done for everybody)
    {
        GfxSettings gfxSettings = {0};
        gfxGetInitialSettings( &gfxSettings );
        getAutoResumeInfoFromRegistry( &gfxSettings, g_achAccountName, &g_iDontSaveName );

        //Make sure the gfx settings are valid (TO DO do more validating...)
        if( gfxSettings.advanced.worldDetailLevel < 0.01f || gfxSettings.advanced.worldDetailLevel > 2.0f ||
            gfxSettings.advanced.entityDetailLevel < 0.01f || gfxSettings.advanced.entityDetailLevel > 2.0f ||
            gfxSettings.screenX == 0 || gfxSettings.screenY == 0 || 
            gfxSettings.advanced.mipLevel > 3 || gfxSettings.advanced.mipLevel < -1 ||
            gfxSettings.advanced.entityMipLevel > 4 || gfxSettings.advanced.entityMipLevel < 0 ||
            gfxSettings.gamma == 0)
        {
            winMsgAlert( textStd("RegistryMunged") );
            gfxGetInitialSettings( &gfxSettings );
        }

        gfxApplySettings( &gfxSettings, 0, false );
    }

#if NOVODEX
    nwSetNovodexDefaults();
#endif
}

void game_reapplyGfxSettings(void)
{
    GfxSettings gfxSettings = {0};

    gfxGetSettings(&gfxSettings);

    gfxApplySettings( &gfxSettings, 0, false );

    // Must be done after gfxApplySettings
    rdrShadowMapSetDefaults();    // setup the default shadowmapping mode settings
}

// Adjust rendering options and finalize initialization of the
// renderer including compiling and loading shaders
static void finalizeRenderer(void)
{
    int expected_allowed;

    // make any modifications to the supported hardware configuration
    // based on command line flags, etc.
    rdrSetChipOptions();

    setCgShaderMode(game_state.useCg);
    setUseDXT5nmNormals(game_state.use_dxt5nm_normal_maps);

    // At this point rdr_caps are for the most part finalized. However, if some
    // shaders fail to load then capabilities will be removed from rdr_caps.allowed_features
    // So, it is a good time to log the system specs and to update the data used by 
    // error/crash reporting in case we have any serious problems during the completion
    // of the renderer initialization.
    rdrPrintSystemSpecs(&systemSpecs);
    expected_allowed = rdr_caps.allowed_features; // save this so we can detect if options changed

    // Tell the render thread to complete initialization of the rendering system.
    // Initialization and fundamental extension collection etc was began during 
    // renderThreadStart which issued a DRAWCMD_RDRSETUP and blocked on completion
    // This will also compile and load shaders as appropriate.
    rdrQueueCmd(DRAWCMD_INIT);

    // *BLOCK* until renderer initialization completes.
    // DRAWCMD_INIT will finalize and set rdr_caps.filled_in and we want
    // subsequent access to that data to be sure to use the finalized data
    // (e.g. logged system specs and gfxSetting preset adjustments)
    
    // In addition, blocking is required when the game is using TEX_ENV_COMBINE mode.
    // If the OpenGL vanilla Tex Env Combiner stages are being used
    // the setup descriptors are stored in custom '.tec' text files
    // in the shader directory. These are parsed using struct parser in the render thread.
    // However, struct parser isn't multi-thread friendly so we need
    // to make sure we wait until that work completes before something else
    // tries to use struct parser in the main thread.

    rdrQueueFlush();    // *BLOCK* until renderer initialization completes.

    // Mark the render caps as FINALIZED!
    // Development console commands can still change the caps on the fly
    rdr_caps.filled_in = 1;

    // Now that render capabilities are finalized update the system spec data
    // used for logging, reporting, assert/crash reports, etc.
    setAssertSystemInfo();
    if ( rdr_caps.allowed_features != expected_allowed )
    {
        // there must have been a problem in the finalization that caused some features
        // to be disabled. Go ahead and log the new feature string to the console for
        // information.
        writeConsole(OUTPUT_WARNING, "Reduced Render Features: %s", rdrFeaturesString(false));
    }
    writeConsole(OUTPUT_INFO, "Renderer initialization complete");

    //**
    // Reapply graphics settings in case we forced something off
    // Do this after rdr_caps are finalized
    game_reapplyGfxSettings();
}

int game_loadSoundsTricksFonts(int argc, char **argv)
{
    int maximize;

    inpClear();

    finalizeRenderer();    // finalize renderer initialization, caps, compile/load shaders

    hdlInitHandles( MAX_HANDLES ); //fx and seqs use handles so this needs to be called before you do anything with either
    initBackgroundLoader();
    initBackgroundTexWordRenderer();
    initNVDXTCriticalSection();

    toggle_3d_game_modes(SHOW_NONE);

    groupInit();

    if (argc > 1)
        strcpy(game_state.scene,argv[1]);

    //11.7.03 moved this above InputStartUp so window size would be right
    //TO DO move up more to be next to gfxApplySettings to reduce confustion
    maximize = game_state.maximized;  //this here because windowResize clears out maximize

    if (!game_state.texWordEdit && FolderCacheGetMode() != FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC) { // these don't get loaded/used for the texWordEditor
        cacheRelevantFolders();
        writeConsole(OUTPUT_INFO, "Cached relevant folders");

        sndInit();
        writeConsole(OUTPUT_INFO, "Loaded sounds");
    }
    writeConsole(OUTPUT_DEBUG, "Loading tricks");
    trickLoad();
    writeConsole(OUTPUT_INFO, "Loaded tricks");
    conCreate();
    texWordsAllowLoading(true);
    writeConsole(OUTPUT_DEBUG, "Loading texWords");
    texWordsLoad(locGetName(getCurrentLocale())); // Before texLoadHeaders
    writeConsole(OUTPUT_INFO, "Loaded texWords");

    if (!game_state.create_bins) {
        game_setProgressString("INIT: before rdrInitTopOfFrame()", "CrashPromptSafeMode", PROGRESSDIALOGTYPE_SAFEMODE);
        rdrInitTopOfFrame();
        game_setProgressString("INIT: before rdrInitTopOfView()", "CrashPromptSafeMode", PROGRESSDIALOGTYPE_SAFEMODE);
        rdrInitTopOfView(cam_info.viewmat,cam_info.inv_viewmat); // Needed to send down anisotropy values to use
        game_setProgressString("INIT: after rdrInitTopOfView()", NULL, PROGRESSDIALOGTYPE_OK);
    }

    if (!texLoadHeaders()) {
        disableClientCrashReports();
        FatalErrorf("%s", textStd("BadInstall"));
    }

    if (!game_state.minimal_loading)
    {
        partEngineInitialize();
        fxEngineInitialize();
    }

    if (!game_state.create_bins) {
        texMakeWhite();
        texLoad("font.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);
        fontNew(&sysfont_info); // font #0

        // Must be before loadBG because the BG might be a composited image with a font
        loadFonts();
        writeConsole(OUTPUT_INFO, "Loaded fonts");
    }
    
    fontInitCriticalSection();

    return maximize;
}

void game_initWindow(int maximize)
{
    //Kinda cheesy.  Message boxes can force a window show before we've set the, that will be interpreted as as user resize
    //and set the window size wrong, so we say don't record the window size changes till here.
    //globPastStartUp = 1;

    //Moved this down here to keep the thing from changing resolution until splash screen comes up
    windowResize(&game_state.screen_x,&game_state.screen_y,&game_state.refresh_rate,game_state.screen_x_pos,game_state.screen_y_pos,game_state.fullscreen);
    gfxWindowReshape();

    if (InputStartup()) {
        // Input failed to initialize!
        if (isProductionMode()) {
            FatalErrorf("%s", textStd("DirectInputDied"));
        }
    }

    if (game_state.nodebug)
        inpDisableAltTab();

    if (!game_state.create_bins)
    {
        chatClientInit();
        initChatUIOnce();    // MJP: must be loaded at some point before networking startup

        loadBG(); // after windowResize
        windowShow(game_state.fullscreen,maximize);
        rdrInitTopOfFrame();
        rdrInitTopOfView(cam_info.viewmat,cam_info.inv_viewmat); // width and height were not getting inited

        rdrClearScreen();
        windowUpdate(); // JE: For some reason the first draw calls (in showBg()) did *nothing* unless the buffers have been swapped once... so I'm putting this line here... probably there's something else going wrong.
        rdrClearScreen();
        windowUpdate(); // Need to do it twice...MW

        gfxSetupLetterBoxViewport();

        showBG();
        showBG();

        hwcursorInit();
    }

    if (game_state.texWordEdit) {
        texWordsEditor(0);
    }

    setupUIColors();
}

void game_networkStart(void)
{
    packetStartup(game_state.packet_mtu,1);
    commNewInputPak();
    sockStart();
    writeConsole(OUTPUT_INFO, "Initialized network library");
    inpClear();
}

void game_loadData(int isCostumeCreator)
{
    bool bNewAttribs = 0; 
    PERFINFO_AUTO_START("top", 1);
    
    chareval_Init();
    combateval_Init();

    if (!isCostumeCreator)
    {
        PERFINFO_AUTO_STOP_START("middle", 1);
        groupLoadLibs();    // Must be before FX and Sequencers
        writeConsole(OUTPUT_DEBUG, "Loading LOD info");
        lodinfoLoad();
        writeConsole(OUTPUT_INFO, "Loaded LOD info");
        modelInitReload();
        makeBaseMakeReferenceArrays();
    }

    if (!game_state.minimal_loading)
    {
        //Preloading sequencers
        writeConsole(OUTPUT_DEBUG, "Loading sequencers");
        loadClothWindInfo(); // Must be before loading sequencers
        loadClothColInfo();

        PERFINFO_AUTO_STOP_START("middle3", 1);

        seqPreloadSeqInfos(); // Must be after groupLoadLibs
        writeConsole(OUTPUT_INFO, "Loaded sequencers");
    }

    PERFINFO_AUTO_STOP_START("middle4", 1);

    //Misc
    writeConsole(OUTPUT_DEBUG, "Loading message stores");
    commInit();

    PERFINFO_AUTO_STOP_START("middle5", 1);

    // Load the profanity filter words.
    LoadProfanity();
    writeConsole(OUTPUT_INFO, "Loaded message stores");

    if (!isCostumeCreator && !game_state.minimal_loading)
    {
        PERFINFO_AUTO_STOP_START("middle6", 1);

        PERFINFO_AUTO_STOP_START("middle7", 1);

        // Load the new classes, origins, and power stuff.
        writeConsole(OUTPUT_DEBUG, "Loading powers");
        load_AllDefs();
        writeConsole(OUTPUT_INFO, "Loaded powers");

        MissionMapPreload();
        PNPCPreload();

        PERFINFO_AUTO_STOP_START("middle8", 1);

        // store defs
        writeConsole(OUTPUT_DEBUG, "Loading items");
        load_Stores();
        writeConsole(OUTPUT_INFO, "Loaded items");
    }

    PERFINFO_AUTO_STOP_START("middle9", 1);

    if (!game_state.minimal_loading)
    {
        // Load title lists
        LoadTitleDictionaries();

        PERFINFO_AUTO_STOP_START("middle10", 1);

        init_menus();

        PERFINFO_AUTO_STOP_START("middle11", 1);

        partPreloadParticles();

        PERFINFO_AUTO_STOP_START("middle12", 1);

        // load all the costume data
        loadCostumes();
        loadPowerCustomizations();
        load_ChestGeoLinkList(); //some costume pieces are custom based on the chest piece (mapping stored in data)

        if(game_state.costume_diff)
            return;

        PERFINFO_AUTO_STOP_START("middle12.1", 1);

        LoadConfigs();

        PERFINFO_AUTO_STOP_START("middle13", 1);

        // Load menu definitions
        load_MenuDefs();
        load_MenuAnimations();
        
        if (!isCostumeCreator)
        {
            PERFINFO_AUTO_STOP_START("middle14", 1);

            loadHelp();
            loadLoadingTips();
            superPackDescriptionsLoad();

            PERFINFO_AUTO_STOP_START("middle15", 1);

            // parse keybinds
            ParseBindings();
            ParseCommandList();

            PERFINFO_AUTO_STOP_START("middle16", 1);

            if(isDevelopmentMode())
            {
                editStateInit();
            }
        }
    }

    srand(((unsigned int)time(NULL))|1);


    if(!game_state.minimal_loading)
    {
        MMScrollSet_Load();
    }


    if (game_state.create_bins)
        return;

    PERFINFO_AUTO_STOP_START("middle17", 1);

    loadUpdate("gfx Reload",-1);
    gfxReload(0);

    PERFINFO_AUTO_STOP_START("middle18", 1);

    if (!game_state.minimal_loading)
    {
        writeConsole(OUTPUT_DEBUG, "Preloading character animations");
        seqPreLoadPlayerCharacterAnimations( SEQ_LOAD_FULL ); //after PreloadSeqInfos
        writeConsole(OUTPUT_INFO, "Preloaded character animations");

        PERFINFO_AUTO_STOP_START("middle19", 1);

        writeConsole(OUTPUT_DEBUG, "Preloading FX geometry");
        fxPreloadGeometry();
        writeConsole(OUTPUT_INFO, "Preloaded FX geometry");
    }
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP_START("middle19", 1);
    writeConsole(OUTPUT_DEBUG, "Creating auction data");
    auction_Init();
    writeConsole(OUTPUT_INFO, "Created auction data");
    PERFINFO_AUTO_STOP();
}

void reShowWindow(void)
{
    // For some reason, on ATI cards, our window sometimes is not shown
    //  properly, so let's try re-showing it!
    ShowWindow(hwnd, SW_SHOW);
}

void someFilesHaveChanged(const char *relpath, int when)
{
    //Otherwise, only turn on file change checks when at least one file in the data dir has actually changed
    if( isDevelopmentMode() && global_state.no_file_change_check_cmd_set != 1 )
    {
        global_state.no_file_change_check = 0;
    }    
}

void checkForStartupExec()
{
    if (game_state.startupExec[0]) {
        char temp[1024];
        sprintf(temp, "exec %s", game_state.startupExec);
        cmdParse(temp);
    }
}

void game_beforeLoop(int isCostumeCreator, int timer)
{
    showBgReset();

    if (!isCostumeCreator)
    {
        commResetControlState();

        if (demo_state.demoname[0])
            demoPlay();
    }

    reShowWindow();

    showBranchWarning();

    texDynamicUnload(1);

    FolderCacheEnableCallbacks(1);
    FolderCacheQuery(NULL, NULL);

    //Don't try to do any more file change checks until something actually changes in the data dir
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "*", someFilesHaveChanged);
    global_state.no_file_change_check = 1;
    global_state.global_frame_count = 1;

    if (!isCostumeCreator)
        checkQuickLogin();

    server_visible_state.timestepscale = 1;

    writeConsole(OUTPUT_INFO, "Loaded all data!");

    if (!isCostumeCreator)
        windows_initDefaults(0);

    checkForStartupExec();

    if(0)
    {
        // MS: Experimental function to prevent memory from swapping to disk.

        _beginthreadex(NULL, 0, keepMemoryThread, NULL, 0, NULL);
    }

    rdrNeedGammaReset();

    // Do this here since we are about to go into the game main loop and the player will see
    // the old driver warning on the login page.  The dialog is there just in case the drivers
    // are bad and the game won't initialize.
    rdrClearOldDriverCheck();
}

void game_setProgressString(const char *progressString, const char *userMessageID, PROGRESSDIALOGTYPE type)
{
    RegReader    rr;

    // Don't overwrite the previous progress strings on startup.
    // Once we've checked for crashes, this will be enabled
    if (!progressStringUpdatingEnabled)
        return;

    // Set the string in the registry
    rr = createRegReader();
    initRegReader(rr, regGetAppKey());

    rrWriteString(rr, "GameProgress", progressString);

    if (userMessageID)
        rrWriteString(rr, "GameProgressUserString", userMessageID);
    else
        rrDelete(rr, "GameProgressUserString");

    rrWriteInt(rr, "GameProgressDialogType", type);

    destroyRegReader(rr);

    // Set the string for crash reports
    setAssertGameProgressString(progressString);

    // For testing
    if ((isDevelopmentMode() || forceCrashCheck) && strcmp(testGameProgressString, progressString) == 0)
    {
        // Force a crash
        int *a=(int*)1;
        int b = *a;
        // Make sure b is used so the compiler does not optimize out the above code
        printf("%d\n", b);
    }
}

// static void game_setAllThreadPriorities(U32 priority)
// {
//     SetThreadPriority(GetCurrentThread(), priority);
//     texWordsSetThreadPriority(priority);
//     backgroundLoaderSetThreadPriority(priority);
// }

#ifdef ENABLE_PROFILER
static int profiling = 0;
static int profileNumber = 0;
static U32 profilingTimer = 0;
#endif

static void startProfilingCPU()
{
#ifdef ENABLE_PROFILER
    if(!game_state.profile && !game_state.profile_spikes) {
        return;
    }

    if(!profilingTimer) {
        profilingTimer = timerAlloc();
    }

    BeginProfile(game_state.profiling_memory * 1024 * 1024);
    timerStart(profilingTimer);
    profiling = 1;
#endif
}

static void endProfilingCPU()
{
#ifdef ENABLE_PROFILER
    char profname[MAX_PATH];
    float ms;

    if(!profiling) {
        return;
    }

    ms = timerElapsed(profilingTimer) * 1000.0f;

    snprintf(profname, MAX_PATH, "profile_%d_%dms.txt", profileNumber, (long)ms);

    if(game_state.profile) {
        EndProfile(profname);
        profileNumber++;
        game_state.profile--;
    } else if(game_state.profile_spikes) {
        if(ms >= (float)game_state.profile_spikes) {
            EndProfile(profname);
            profileNumber++;
        } else {
            EndProfile(NULL);
        }
    } else {
        // user just turned it off
        EndProfile(NULL);
    }
    profiling = 0;
#endif
}

static void game_updateThreadPriority()
{
    // JE: Not changing thread priorities because we keep forgetting to add
    //  new threads to this list (render thread is not in the list), and
    //  just changing the process priority should do what we want.
    if(isProductionMode())
    {
        if(GetForegroundWindow() == hwnd)
        {
            SetPriorityClass(GetCurrentProcess(), 
                game_state.priorityBoost ? ABOVE_NORMAL_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS);
        }
        else
        {
            SetPriorityClass(GetCurrentProcess(),BELOW_NORMAL_PRIORITY_CLASS);
        }
    }
}

int game_mainLoop(int timer)
{
    //timing_state.runperfinfo_init = -1;
    //timing_state.autoStackMaxDepth = 100;

    if(isDevelopmentMode())
        SetPriorityClass(GetCurrentProcess(),BELOW_NORMAL_PRIORITY_CLASS);

    game_setProgressString("game_mainLoop", NULL, PROGRESSDIALOGTYPE_NONE);

    for(;;)
    {
        // Nothing goes above autoTimerTickBegin()!!!!!

        autoTimerTickBegin();

        startProfilingCPU();

        game_updateThreadPriority();

        if (isMenu(MENU_GAME))
        {
            static bool b=false;
            if (!b) {
                /* char buf[1024];
                sprintf(buf, "Time to load game and get into a map: %1.3gs\n", timerElapsed(timer));
                printf("%s", buf); */
                timerFree(timer);
                b=true;
            }
#ifdef PROFILING
            return 0;
#endif
        }
        global_state.global_frame_count++;

        //real client work: serve menus + engine update.

        rdrSetGamma( game_state.gamma );

        if( game_state.imageServer )
        {
            imageserver_Go();
        }


        PERFINFO_AUTO_START("engine_update", 1);
            engine_update();
        PERFINFO_AUTO_STOP_CHECKED("engine_update");
        
        demoRecordClientInfo();

        setAssertEntityName(controlledPlayerPtr() ? controlledPlayerPtr()->name : NULL);
        setAssertShardName(db_info.address);

        PERFINFO_AUTO_START("others", 1);
            PERFINFO_AUTO_START("FolderCacheQuery", 1);
            
                FolderCacheDoCallbacks(); // Check for directory changes
                fileFreeOldZippedBuffers();
            
            PERFINFO_AUTO_STOP_START("checkOddball", 1);
                
                //odd ball things that never happen
                 checkOddball();
                
            PERFINFO_AUTO_STOP_START("checkLogoutProgress", 1);
                
                checkLogoutProgress();
                
            PERFINFO_AUTO_STOP_START("LWC_Tick", 1);
                
                LWC_Tick();
                
        PERFINFO_AUTO_STOP_CHECKED("others");
        
        // HEY YOU: Don't put anything after autoTimerTickEnd()!!!!!!

//         if(isDevelopmentMode())
//             assert(heapValidateAll());

        endProfilingCPU();

        autoTimerTickEnd();
        
        // Only do a forced full relight for one frame
        game_state.forceFullRelight = 0;

        // Nothing goes down here.
    }
    return 0;
}
