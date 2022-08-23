#include "render/cubemap.h"

#include <utilitieslib/assert/assert.h>
#include "graphics/viewport.h"
#include "graphics/gfx.h"
#include "cmdparse/cmdgame.h"
#include "render/tex.h"
#include "render/thread/ogl.h"
#include "render/thread/rt_init.h"
#include "UI/sprite/sprite_base.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/camera.h"
#include "render/renderUtil.h"
#include "render/thread/wcw_statemgmt.h"
#include "graphics/failtext.h"
#include "seq/seq.h" //MAX_LODS
#include "render/renderssao.h"
#include "graphics/gfxSettings.h"
#include "render/thread/rt_cubemap.h"
#include "demo.h"
#include "player/player.h"
#include "entity/entity.h"
#include "win/win_init.h"
#include "graphics/gfxwindow.h"
#include "graphics/gfxDebug.h"
#include <utilitieslib/utils/file.h>
#include "shlobj.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/sun.h" //isIndoors()
#include "render/renderprim.h"
#include "cubemap_filter.h"
#include <utilitieslib/utils/tga.h>
#include "group/group.h"
#include "graphics/font.h"
#include "cmdparse/cmdcommon.h"
#include "edit/edit_cmd.h"
#include "edit/edit_select.h"

#define CUBEMAP_FOV            90

static CubemapFaceData s_faces[NUM_CUBEMAP_FACES];
static bool s_bInitedCubemap = false;
static int s_forceCubeFace = 0;
static int s_update4Faces = 0;
static int s_update3Faces = 0;
static const char* s_face_names[] =  { "x_pos", "x_neg", "y_pos", "y_neg", "z_pos", "z_neg" };
static int s_usingInvCamMatrix = -1;
static Vec3 s_savedPYR;
static bool s_bForcedDynamicLastFrame;
static bool s_bNeedDynamic;
static void *s_capturedCubemapFaces[NUM_CUBEMAP_FACES];
static int s_currentFace = -1;

#define DEFAULT_STATIC_CUBEMAP "generic_cubemap_face0.tga" // use this if none specified in scene file

static INLINEDBG const char* GetDefaultStaticCubemapName()
{
    if( scene_info.cubemap && strlen(scene_info.cubemap) )
    {
#ifndef FINAL
        // require that name ends in 0.tga, so that other faces can be found at 1.tga, 2.tga, etc.
        if( !(strEndsWith(scene_info.cubemap, "0.tga") || strEndsWith(scene_info.cubemap, "0")) ) {
            failmsgf( 0, "Scene file's cubemap value \"%s\" not valid, needs to end in 0.tga", scene_info.cubemap );
        }
#endif
        return scene_info.cubemap;
    }

    return DEFAULT_STATIC_CUBEMAP;
}

CubemapFaceData* cubemapGetFace( int nFaceIndex )
{
    assert(( nFaceIndex >= 0 ) && ( nFaceIndex < NUM_CUBEMAP_FACES ));
    return ( s_faces + nFaceIndex );
}

static bool cubemapFacePreRenderCB(ViewportInfo *pViewportInfo)
{
    Vec3 pyr;

    // dont render dynamic cubemap if in GUI or forcing no face update
    if (game_state.game_mode != SHOW_GAME || game_state.forceCubeFace > NUM_CUBEMAP_FACES)
        return false;

    // dont render dynamic cubemap if static is active or forcing no face update
    if( (game_state.static_cubemap_terrain && game_state.static_cubemap_chars) && !s_bForcedDynamicLastFrame )
        return false;

    zeroVec3(pyr);

    // set camera matrix for this face
    switch( pViewportInfo->target )
    {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB:    pyr[1] = -HALFPI;        break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB:    pyr[1] = HALFPI;        break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB:    pyr[0] = -HALFPI; pyr[2] = HALFPI * 2;        break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB:    pyr[0] = HALFPI;  pyr[2] = HALFPI * 2;        break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB:    pyr[1] = HALFPI * 2;    break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB:    /* no rotation */        break;
        default: assert(!"unknown cubemap face id"); return false;
    }

    // create the viewport camera matrix at the current player location with the appropriate rotation
    createMat3PYR(pViewportInfo->cameraMatrix, pyr);

    if (game_state.cubemapUpdate3Faces)
    {
        Mat3 temp1, temp2, temp3;

        if (pViewportInfo->target == GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB)
            copyVec3(cam_info.targetPYR, s_savedPYR);

        pyr[0] =  0.5f * HALFPI;
        pyr[1] = -0.5f * HALFPI;
        pyr[2] =  0.0f * HALFPI;

        createMat3PYR(temp2, pyr);

        mulMat3(temp2, pViewportInfo->cameraMatrix, temp3);

        zeroVec3(pyr);
        pyr[0] = s_savedPYR[0];
        pyr[1] = 0.0f;
        pyr[2] = 0.0f;

        createMat3PYR(temp1, pyr);
        
        mulMat3(temp1, temp3, temp2);

        zeroVec3(pyr);
        pyr[0] = 0.0f;
        pyr[1] = s_savedPYR[1];
        pyr[2] = 0.0f;

        createMat3PYR(temp3, pyr);

        mulMat3(temp3, temp2, pViewportInfo->cameraMatrix);
    }

    if (pViewportInfo->callbackData) {
        DefTracker* tracker = pViewportInfo->callbackData;
        copyVec3(tracker->mat[3], pViewportInfo->cameraMatrix[3]);
    } else if (demoIsPlaying() || !playerPtr() )
        copyVec3( cam_info.cammat[3], pViewportInfo->cameraMatrix[3] );
    else {
        copyVec3( ENTPOS(playerPtr()), pViewportInfo->cameraMatrix[3] );
        pViewportInfo->cameraMatrix[3][1] += playerHeight()/2; // move to middle of player, not bottom of feet
    }

    // save the cubemap camera position to calculate cubemap reflection attenuation per model
    copyVec3(pViewportInfo->cameraMatrix[3] , gfx_state.cubemap_origin);

    // update each frame since game_state.charsInCubemap can be changed on they fly
    pViewportInfo->renderCharacters = !game_state.gen_static_cubemaps &&
        (game_state.cubemapMode >= CUBEMAP_HIGHQUALITY || game_state.charsInCubemap != 0);

    // Don't render characters into static cubemaps
    if (game_state.cubemap_flags&kGenerateStatic)
        pViewportInfo->renderCharacters = false;

    // finish up on the render thread.
    // Note: Passing a POINTER, not a copy of the data struct.
    rdrQueue( DRAWCMD_CUBEMAP_PRECALLBACK, &pViewportInfo, sizeof(ViewportInfo*) );
    return true;
}

static bool cubemapFacePostRenderCB(ViewportInfo *pViewportInfo)
{
    if (s_currentFace >= 0 && game_state.cubemap_flags&kSaveFacesOnGen)
    {
        int w, h;
        windowSize(&w, &h);
        rdrGetFrameBuffer(0, 0, w, h, GETFRAMEBUF_RGBA, s_capturedCubemapFaces[s_currentFace]);
    }

    // finish up on the render thread
    // Note: Passing a POINTER, not a copy of the data struct.
    rdrQueue( DRAWCMD_CUBEMAP_POSTCALLBACK, &pViewportInfo, sizeof(ViewportInfo*) );
    return true;
}


static void initTuningMenu()
{
    rdrQueue( DRAWCMD_INITCUBEMAPMENU,NULL,0);
}    

static bool cubemapInitEx( int size, float lod_scale, bool bUpdateAllPerFrame, bool bRenderChars, bool bRenderAlphaPass, bool bRenderParticles )
{
    int i;
    bool bSuccess = true;
    
    assert(!s_bInitedCubemap);

    // init each face of the cubemap.  The first face serves as the master cubemap texture and other 
    //    faces share its pbuffer for rendering.
    for(i=0; i<NUM_CUBEMAP_FACES && bSuccess; i++)
    {
        CubemapFaceData* pface = &s_faces[i];
        ViewportInfo* vp = &pface->viewport;
        BasicTexture* pTex = &pface->texture;

        sprintf(pface->name, "cubemap_face_%d_%s", i, s_face_names[i]);

        viewport_InitStructToDefaults(vp);
        vp->renderPass = RENDERPASS_CUBEFACE;
        vp->lod_model_override = game_state.reflection_cubemap_lod_model_override;
        vp->lod_scale = lod_scale;
        vp->vis_limit_entity_dist_from_player = game_state.reflection_cubemap_entity_vis_limit_dist_from_player;

        vp->name = pface->name;
        vp->target = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i; // unique id within this module only (so can identify in callback)
        vp->fov = CUBEMAP_FOV;
        vp->bFlipYProjection = true; // flip y axis when rendering, to deal with opengl cubemap texture format

        // are we updating every face in one frame or only doing one face per frame?
        if (bUpdateAllPerFrame)
        {
            vp->update_interval = 1;
            vp->next_update = 0;
        }
        else
        {
            vp->update_interval = 6;
            vp->next_update = i;
        }

        vp->width = size;
        vp->height = size;
        vp->pPreCallback = cubemapFacePreRenderCB;
        vp->pPostCallback = cubemapFacePostRenderCB;
        vp->pSharedPBufferTexture = (i == 0 ? NULL : &s_faces[0].viewport.PBufferTexture);

        //vp->isMipmapped = true;
        //vp->maxMipLevel = 4;

        // limit what renders in cubemap
        vp->renderCharacters = bRenderChars;
        vp->renderPlayer = false; // never render player since camera located at player position
        vp->renderParticles = bRenderParticles;    // static cubemap gen may want particles for War Walls, etc.
        vp->renderAlphaPass = bRenderAlphaPass; // static cubemap gen may want alpha for War Walls, etc.
        vp->renderWater = false;

        vp->lod_entity_override = MAX_LODS;    // Not currently used on aux viewports because of performance problem

        bSuccess = viewport_InitRenderToTexture(vp);
        if( !bSuccess )
        {
            int j;
            failText("A cubemap viewport failed to initialize.  Disabling dynamic cubemaps.\n");
            viewport_Delete(vp);

            // remove and delete any faces that were successfully initialized before this one
            for(j=i-1; j>=0; j--)
            {
                CubemapFaceData* pface = &s_faces[i];
                viewport_Remove( &pface->viewport );
                viewport_Delete( &pface->viewport );
            }
            break;
        }


        // Fill in BasicTexture fields -- based on texGenNew()
        pTex->id = (vp->pSharedPBufferTexture ? vp->pSharedPBufferTexture->color_handle[vp->pSharedPBufferTexture->curFBO] : vp->PBufferTexture.color_handle[vp->PBufferTexture.curFBO]);
        pTex->target = (i == 0 ? GL_TEXTURE_CUBE_MAP_ARB : vp->target);
        pTex->actualTexture = pTex;
        pTex->load_state[0] = TEX_LOADED;
        pTex->gloss = 1;
        pTex->name = vp->name;
        pTex->dirname = "";
        pTex->width = pTex->realWidth = vp->width;
        pTex->height = pTex->realHeight = vp->height;
        pTex->memory_use[0] = 0;

        bSuccess = viewport_Add(vp);
        assert(bSuccess);
    }

    s_bInitedCubemap = bSuccess;
    return s_bInitedCubemap;
}

static bool cubemapInit()
{
    return cubemapInitEx( game_state.cubemap_size, game_state.cubemapLodScale, false,
        !game_state.gen_static_cubemaps && 
            (game_state.cubemapMode >= CUBEMAP_HIGHQUALITY || game_state.charsInCubemap != 0),
        true, false );
}

static void cubemapRelease()
{
    int i;

    if ( !s_bInitedCubemap )
        return;
    
    // release in reverse order since first entry contains the shared pbuffer
    for(i=NUM_CUBEMAP_FACES-1; i>=0; i--)
    {
        CubemapFaceData* pface = &s_faces[i];
        viewport_Remove( &pface->viewport );
        viewport_Delete( &pface->viewport );
    }

    s_bInitedCubemap = false;
    s_update4Faces = 0;
    s_update3Faces = 0;
}

static void cubemap_UpdateFaceRenderOrder()
{
    if (game_state.cubemapUpdate3Faces && !s_update3Faces)
    {
        viewport_Remove( &s_faces[1].viewport );
        viewport_Remove( &s_faces[5].viewport );
//        viewport_Remove( &s_faces[2].viewport );
    }
    else if (!game_state.cubemapUpdate3Faces && s_update3Faces)
    {
        viewport_Add( &s_faces[1].viewport );
        viewport_Add( &s_faces[5].viewport );
//        viewport_Add( &s_faces[2].viewport );
    }

    if (game_state.cubemapUpdate3Faces)
    {
        if (game_state.forceCubeFace)
        {
            s_faces[0].viewport.update_interval = 1;
            s_faces[0].viewport.next_update = 0;

            s_faces[4].viewport.update_interval = 1;
            s_faces[4].viewport.next_update = 0;

            s_faces[3].viewport.update_interval = 1;
            s_faces[3].viewport.next_update = 0;
        }
        else
        {
            s_faces[0].viewport.update_interval = 3;
            s_faces[0].viewport.next_update = 0;

            s_faces[4].viewport.update_interval = 3;
            s_faces[4].viewport.next_update = 1;

            s_faces[3].viewport.update_interval = 3;
            s_faces[3].viewport.next_update = 2;
        }

        s_faces[2].viewport.update_interval = 100;
        s_faces[2].viewport.next_update = 0;

        s_update3Faces = 1;
        s_update4Faces = 0;
    }
    else if (game_state.cubemapUpdate4Faces)
    {
        // Pick the three faces which point toward/behind the camera and have them
        // update every 4th frame.  Have each of the remaining three faces
        // update every 12th frame.
        // So: 1 2 3 (4) 1 2 3 (5) 1 2 3 (6)
        float dotproduct[6];
        bool picked[6];
        int i, j;

        static Vec3 face_normals[6] = { { 1,  0,  0},
                                        {-1,  0,  0},
                                        { 0,  1,  0},
                                        { 0, -1,  0},
                                        { 0,  0,  1},
                                        { 0,  0, -1} };

        for (i = 0; i < 6; i++)
        {
            dotproduct[i] = dotVec3(face_normals[i], cam_info.cammat[2]);
            picked[i] = false;
        }

        for (j = 0; j < 3; j++)
        {
            float best = -1;
            int best_index = -1;

            for (i = 0; i < 6; i++)
            {
                if (!picked[i] && dotproduct[i] > best)
                {
                    best = dotproduct[i];
                    best_index = i;
                }
            }

            picked[best_index] = true;

            s_faces[best_index].viewport.update_interval = 4;
            s_faces[best_index].viewport.next_update = j;
        }

        // j should already be 3 here
        //j = 3;
        for (i = 0; i < 6; i++)
        {
            if (!picked[i])
            {
                s_faces[i].viewport.update_interval = 12;
                s_faces[i].viewport.next_update = j;
                j += 4;
            }
        }

        s_update3Faces = 0;
        s_update4Faces = 1;
    }
    else
    {
        int i;

        // restore all faces to render one face per frame
        for(i=0; i<NUM_CUBEMAP_FACES; i++)
        {
            CubemapFaceData* pface = &s_faces[i];
            pface->viewport.update_interval = 6;
            pface->viewport.next_update = i;
        }

        s_update3Faces = 0;
        s_update4Faces = 0;
    }
}

static void cubemap_ChangeRenderSingleFaceMode(int prevFaceNum, int newFaceNum)
{
    int i;

    assert(s_bInitedCubemap);

    // First, restore to default rendering if a previous face was set
    if(prevFaceNum)
    {
        // restore all faces to render one face per frame
        for(i=0; i<NUM_CUBEMAP_FACES; i++)
        {
            CubemapFaceData* pface = &s_faces[i];
            if( prevFaceNum != -1 && i != (prevFaceNum-1) ) {
                viewport_Add( &pface->viewport );
            }
            pface->viewport.update_interval = 6;
            pface->viewport.next_update = i;
        }
    }

    if(newFaceNum)
    {
        // force this face to render each frame, and other faces not to render at all
        for(i=0; i<NUM_CUBEMAP_FACES; i++)
        {
            CubemapFaceData* pface = &s_faces[i];
            if( i == (newFaceNum-1) || newFaceNum == -1) { // -1 to update all faces every frame (slow, just for testing)
                pface->viewport.update_interval = 1;
                pface->viewport.next_update = 0;
                continue;
            }

            // remove other faces from viewports list
            viewport_Remove( &pface->viewport );
        }
    }

    s_update4Faces = 0;
    s_update3Faces = 0;
}

enum
{
    CMGENSTATE_NONE,
    CMGENSTATE_MOVING,
    CMGENSTATE_WAITING_NEXT_MOVE
};


static float sLodScaleMapping[] =
{
    0.0f,        // CUBEMAP_OFF
    0.05f,        // CUBEMAP_LOWQUALITY
    0.08f,        // CUBEMAP_MEDIUMQUALITY
    0.12f,        // CUBEMAP_HIGHQUALITY
    0.15f,        // CUBEMAP_ULTRAQUALITY
    1.0            // CUBEMAP_DEBUG_SUPERHQ
};

static void saveCubemap(const char* dirName, const char *name, void *filteredFaces[], int size) 
{
    int i;
    char the_date[32] = "";
    time_t now;
    char parentDir[MAX_PATH];
    static int counter;

    // add date/frame to filename so don't overwite old results
    now = time(NULL);
    if (now != -1)
        strftime(the_date, 32, "%Y%m%d", gmtime(&now));

    // create parent dir if it doesn't exist
    if( dirName )
        sprintf(parentDir, "c:/game/src/texture_library/static_cubemaps/%s", dirName);
    else
        strcpy(parentDir, "c:/temp");

    if (!dirExists(parentDir))
        SHCreateDirectoryExA(NULL, parentDir, NULL);

    for (i = 0; i < 6; ++i)
    {
        char filename[1000];

        if(dirName)
        {
            assert(name && "Specified dirName but not name!");
            sprintf(filename, "%s/%s_%s_face%d.tga", parentDir, dirName, name, i);
        }
        else
        {
            sprintf(filename, "%s/coh_%s_%02d_cubemap_face%d.tga", parentDir, the_date, counter, i);
        }

        if (!dirName) {
            tgaSave(filename, filteredFaces[i], size, size, 4);
        }
    }

    if (!dirName)
        ++counter;
}

// This function reconfigures the dynamic cubemap system to generate a high quality
// static cubemap texture.
//  Should be able to specify:
//        resolution
//        vis scale
//        post filtering
//        etc.
// We will need to restore the system to the dynamic runtime settings completely
// when we are done
static void cubemap_GenerateStatic(int genSize, int captureSize, float radius, float lod_scale,
                                   const char* dirName, const char *name, DefTracker *captureNode)
{
#ifndef FINAL
    int prev_static_cubemap = game_state.static_cubemap_chars;
    int prev_chars = game_state.charsInCubemap;
    game_state.static_cubemap_chars = 0;    // enable dynamic cubemap rendering
    game_state.charsInCubemap = 0;

    // release current dynamic viewports and recreate with desired size/settings
    cubemapRelease();

    // We render particles/alpha pass into static gen cubemaps to capture effects like the
    // War Walls. Best to capture using just the geometry layer to avoid any entity FX

    if ( cubemapInitEx(captureSize, lod_scale, true, false, 
        game_state.cubemap_flags&kCubemapStaticGen_RenderAlpha, 
        game_state.cubemap_flags&kCubemapStaticGen_RenderParticles) )
    {
        int i;
        void *filteredFaces[6];

        // NOTE: We order here such they the output filenames can be used directly as inputs
        //    to our cubemaps, which expect the following order:
        //    +x, -x, +y, -y, +z, -z
        for (i = 0; i < 6; i++)
        {
            s_currentFace = i;
            filteredFaces[i] = malloc(genSize * genSize * 4);
            s_capturedCubemapFaces[i] = malloc(captureSize * captureSize * 4);
            s_faces[i].viewport.callbackData = captureNode;
            viewport_RenderOneUtility(&s_faces[i].viewport);
        }

        s_currentFace = -1;
        filterCubemap(filteredFaces, genSize, s_capturedCubemapFaces, captureSize, radius);
        saveCubemap(dirName, name, filteredFaces, genSize);

        for (i = 0; i < NUM_CUBEMAP_FACES; ++i)
        {
            free(s_capturedCubemapFaces[i]);
            free(filteredFaces[i]);
            s_capturedCubemapFaces[i] = NULL;
        }
    }

#if 0 // buggy
    // Repopulate our generic static cubemap with the newly created one?
    if (game_state.cubemap_flags&kReloadStaticOnGen)
    {
        BasicTexture *pTestCubemap = texFind( GetDefaultStaticCubemapName() );
        
        if(game_state.debugCubemapTexture)
            assert(pTestCubemap);

        if( game_state.cubemapMode && pTestCubemap)
        {
            PBuffer* pbuf = viewport_GetPBuffer(&s_faces[0].viewport);

            // free the old one
            rdrTexFree(pTestCubemap->id);

            // detach the one we just created and make it the static
            // why this ??  -- assert(0);
            pTestCubemap->id = pbuf->color_handle[pbuf->curFBO];
            pbuf->color_handle[pbuf->curFBO] = 0;
            pTestCubemap->width = pbuf->width;
            pTestCubemap->height = pbuf->height;
            // hack: render thread should be flushed, so we're issuing gl commands
            //            pTestCubemap->id = dbg_CopyCubemapTexture( pbuf->color_handle, pbuf->width, pbuf->height );
        }
    }
#endif

    // restore previous settings on next update
    cubemapRelease();

    game_state.static_cubemap_chars = prev_static_cubemap;
    game_state.charsInCubemap = prev_chars;

    winMakeCurrent();
    gfxWindowReshape();

#endif //!FINAL
}

static void findCaptureNodes(DefTracker ***foundNodes, DefTracker *root)
{
    int i;

    if (!root)
        return;

    for (i = 0; i < root->count; ++i)
        findCaptureNodes(foundNodes, &root->entries[i]);

    if (root->def && root->def->has_cubemap)
        eaPush(foundNodes, root);
}

typedef struct  
{
    DefTracker **nodes;
    int index;
    U32 lastMoveTime;
    OptionCubemap savedMode;
    F32 savedTime;
    F32 savedTimescale;
    int savedEditMode;
    int savedBackgroundGeometryLoadState;
    int savedCubemapLodBias;
} CubemapGenerationContext;

static void setUpNextStaticCubemapCapture(CubemapGenerationContext *context)
{
    static int sNodeWasInvisible = 1;

    if (context->index >= 0 && context->index < eaSize(&context->nodes))
        context->nodes[context->index]->parent->invisible = sNodeWasInvisible;

    do
    {
        ++(context->index);
    } while (context->index < eaSize(&context->nodes) && !context->nodes[context->index]->def->has_cubemap);

    if (context->index < eaSize(&context->nodes))
    {
        char buffer[64];
        DefTracker *captureNode = context->nodes[context->index];
        sNodeWasInvisible = captureNode->parent->invisible;
        captureNode->parent->invisible = 1;
        context->lastMoveTime = ABS_TIME;
        sprintf_s(buffer, sizeof(buffer), "timeset %f", captureNode->def->cubemapCaptureTime);
        cmdParse(buffer);
    }
}

const char *findAnyDuplicateCubemapName(DefTracker ***nodes)
{
    int i;

    for (i = 1; i < eaSize(nodes); ++i)
    {
        int j;
        for (j = 0; j < i; ++j)
        {
            if (stricmp((*nodes)[i]->def->name, (*nodes)[j]->def->name) == 0)
                return (*nodes)[i]->def->name;
        }
    }

    return NULL;
}

static void endStaticCubemapGeneration(CubemapGenerationContext *context) 
{
    char commandBuffer[64];
    game_state.gen_static_cubemaps = 0;
    game_state.cubemapMode = context->savedMode;
    context->index = -1;
    eaDestroy(&context->nodes);
    context->nodes = NULL;
    texEnableThreadedLoading();
    game_state.disableGeometryBackgroundLoader = context->savedBackgroundGeometryLoadState;
    game_state.reflection_cubemap_lod_model_override = context->savedCubemapLodBias;
    game_state.edit = context->savedEditMode;
    sprintf_s(commandBuffer, sizeof(commandBuffer), "timeset %f", context->savedTime);
    cmdParse(commandBuffer);
    sprintf_s(commandBuffer, sizeof(commandBuffer), "timescale %f", context->savedTimescale);
    cmdParse(commandBuffer);
}

void initializeStaticCubemapGeneration(CubemapGenerationContext *context) 
{
    int i;
    const char* duplicateName;

    context->savedMode = game_state.cubemapMode;
    context->savedTime = server_visible_state.time;
    context->savedTimescale = server_visible_state.timescale;
    context->savedEditMode = game_state.edit;
    unSelect(1);

    if (game_state.edit)
        editSetMode(0, 0);

    context->savedCubemapLodBias = game_state.reflection_cubemap_lod_model_override;
    game_state.reflection_cubemap_lod_model_override = 0;
    context->savedBackgroundGeometryLoadState = game_state.disableGeometryBackgroundLoader;
    game_state.disableGeometryBackgroundLoader = 1;
    texDisableThreadedLoading();

    cmdParse("timescale 0");

    game_state.cubemapMode = CUBEMAP_DEBUG_SUPERHQ;

    for (i = 0; i < group_info.ref_count; ++i)
        findCaptureNodes(&context->nodes, group_info.refs[i]);

    duplicateName = findAnyDuplicateCubemapName(&context->nodes);

    if (duplicateName)
    {
        ErrorFilenamef(game_state.world_name, "Duplicate cubemap name: %s.\nPlease rename the def.",
            duplicateName);
        endStaticCubemapGeneration(context);
    }
    else
        setUpNextStaticCubemapCapture(context);
}

static void getMapName(char * mapName)
{
    char *namePtr = game_state.world_name;
    int len;

    if (strchr(namePtr, '/'))
        namePtr = strrchr(namePtr, '/') + 1;
    if (strchr(namePtr, '\\'))
        namePtr = strrchr(namePtr, '\\') + 1;
    strcpy(mapName, namePtr);
    len = strlen(mapName);

    if (len > 4 && stricmp(mapName + len - 4, ".txt") == 0)
        mapName[len - 4] = '\0';
}

static void handleStaticCubemapGeneration() 
{
    static CubemapGenerationContext sContext = { NULL, -1, 0, CUBEMAP_OFF };

    if (!game_state.gen_static_cubemaps)
        return;

    xyprintf(30, 20, "Generating static cubemap: %s", 
        (sContext.index >= 0 && sContext.index < eaSize(&sContext.nodes))
            ? sContext.nodes[sContext.index]->def->name
            : "");

    if (sContext.index < 0)
        initializeStaticCubemapGeneration(&sContext);
    else if (sContext.index < eaSize(&sContext.nodes) &&
        ABS_TIME_SINCE(sContext.lastMoveTime) > SEC_TO_ABS(3))
    {
        char mapName[MAX_PATH];
        DefTracker *cubemapNode = sContext.nodes[sContext.index];
        getMapName(mapName);
        cubemap_GenerateStatic(cubemapNode->def->cubemapGenSize, 
            cubemapNode->def->cubemapCaptureSize, cubemapNode->def->cubemapBlurAngle,
            game_state.cubemapLodScale_gen,  mapName, cubemapNode->def->name, cubemapNode);
        setUpNextStaticCubemapCapture(&sContext);
    }
    else if (sContext.index >= eaSize(&sContext.nodes))
    {
        endStaticCubemapGeneration(&sContext);
    }
}

void cubemap_Update()
{
    // debug stuff first, need to run whether cubemaps enabled or not
    static bool once = false;
    static int lastCubemapSize = 0;
    int i;

    if(!once ) {
        initTuningMenu();
        once = true;
    }

    s_usingInvCamMatrix = -1;

    s_bForcedDynamicLastFrame = s_bNeedDynamic;
    s_bNeedDynamic = false;

    if( !game_state.cubemapMode )
    {
        if(s_bInitedCubemap)
            cubemapRelease();
        return;
    }

    if(s_bInitedCubemap && !(game_state.static_cubemap_terrain && game_state.static_cubemap_chars) && game_state.cubemap_size != lastCubemapSize ) 
    {
        // changed cubemap size, so re-initialize cubemaps now
        cubemapRelease();
    }

    if( !s_bInitedCubemap )
    {
        if( !cubemapInit() )
        {
            // failed to init, disable feature permanently
            rdr_caps.features &= ~GFXF_CUBEMAP;
            rdr_caps.allowed_features &= ~GFXF_CUBEMAP;
            game_state.cubemapMode = CUBEMAP_OFF;
            return;
        }
        s_forceCubeFace = 0; // to re-synch forced face
        lastCubemapSize = game_state.cubemap_size;
    }

    // update lodscale from cubemapmode
    assert(game_state.cubemapMode >= CUBEMAP_OFF && game_state.cubemapMode <= CUBEMAP_DEBUG_SUPERHQ);
    game_state.cubemapLodScale = sLodScaleMapping[game_state.cubemapMode];

    // Update viewport LOD scale
    for (i = 0; i < 6; i++)
    {
        CubemapFaceData* pface = &s_faces[i];
        ViewportInfo* vp = &pface->viewport;
        vp->lod_scale = game_state.cubemapLodScale;
    }

#if 0 // cubemap attenuation only used for dynamic cubemap
    if (game_state.static_cubemap)
    {
        // save the cubemap camera position to calculate cubemap reflection attenuation per model
        // TODO: possibly set this to something more appropriate to the static cubemap
        copyVec3( cam_info.cammat[3], gfx_state.cubemap_origin );
    }
#endif

    // viewport module takes care of rendering each face when its turn comes up, so no
    //    non-debug rendering to do here    

#ifndef FINAL
    // debug rendering:
    if( game_state.display_cubeface>=1 && game_state.display_cubeface<=6 )
    {
        BasicTexture* ptex;
        if( game_state.static_cubemap_chars ) {
            char buf[256];

            // temp hack for static cubemap
            strcpy(buf, GetDefaultStaticCubemapName());
            buf[strlen(buf)-5] = '0' + game_state.display_cubeface-1;
            ptex = texFind(buf);
        } else {
            ptex = &s_faces[game_state.display_cubeface-1].texture;
        }
        if(game_state.debugCubemapTexture)
            assert(ptex);

        display_sprite_ex(0, ptex, 640, 128, 1, 1.f, 1.f,
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
            0, 0, 1, 1, // flip v coords so face appears right side up
            0, 0, clipperGetCurrent(), 1, H_ALIGN_LEFT, V_ALIGN_TOP);
    }

    // did user request one-time static cubemap gen?
    if (game_state.cubemap_flags&kGenerateStatic)
    {
        cubemap_GenerateStatic(game_state.cubemap_size_gen, game_state.cubemap_size_capture, 0.0,
            game_state.cubemapLodScale_gen, NULL, NULL, NULL);
        game_state.cubemap_flags &= ~kGenerateStatic; // we're done generating, clear the flag
    }

    handleStaticCubemapGeneration();

#endif //!FINAL

    // if user requested a single face to be rendered since last frame, invoke
    //    single face rendering mode
    if( game_state.forceCubeFace != s_forceCubeFace )
    {
        if( game_state.forceCubeFace >= -1 && game_state.forceCubeFace <= NUM_CUBEMAP_FACES )
            cubemap_ChangeRenderSingleFaceMode(s_forceCubeFace, game_state.forceCubeFace);
        s_forceCubeFace = game_state.forceCubeFace;
    }

    if (game_state.forceCubeFace < 1 && game_state.cubemapUpdate3Faces)
    {
        // Don't combine this if() with the one above
        if (!s_update3Faces)
        {
            cubemap_UpdateFaceRenderOrder();
        }
    }
    // Don't do this else if game_state.cubemapUpdate3Faces is true
    else
    {
        if (s_update3Faces)
        {
            cubemap_UpdateFaceRenderOrder();
        }
        else if (game_state.forceCubeFace == 0 && game_state.cubemapUpdate4Faces)
        {
            if ((!s_update4Faces || (global_state.global_frame_count % 12) == 0))
            {
                cubemap_UpdateFaceRenderOrder();
            }
        }
    }
}

// further than this distance, models use default static cubemap for level
//    instead of nearest cubemap beacon location
#define USE_DEFAULT_CUBEMAP_THRESHOLD_DIST    500
#define ENABLE_TEST_BEACONLIST    0

int cubemap_GetTextureID(bool bForTerrain, Vec3 modelLoc, bool bForceDynamic)
{
    int texID = 0;

    // force static cubemap in front end, otherwise use terrain/char preference 
    bool bUseStatic = ( game_state.game_mode != SHOW_GAME || 
            ( (bForTerrain && game_state.static_cubemap_terrain) || 
              (!bForTerrain && game_state.static_cubemap_chars) )   );
    
    if (bUseStatic && bForceDynamic && game_state.game_mode == SHOW_GAME)
    {
        bUseStatic = false;
        s_bNeedDynamic = true;
    }

    if( bUseStatic )
    {
        const char* cubeMapName;
        BasicTexture *pTestCubemap;

#if !ENABLE_TEST_BEACONLIST
        cubeMapName = GetDefaultStaticCubemapName();
#else
        if( game_state.game_mode != SHOW_GAME || !sPraetoriaCubemapLocs.numLocations || fabs(modelLoc[2]) > USE_DEFAULT_CUBEMAP_THRESHOLD_DIST )
        {
            cubeMapName = GetDefaultStaticCubemapName();
        }
        else
        {
            // This level has static cubemap beacons, use the nearest one.  Note that modelLoc is camera relative.
            //    TBD, change algorigthm to take into account model location, for now just getting nearest to camera
            F32* cameraPos = cam_info.cammat[3]; // operate in world coords
            F32 nearestDistSq = 9999999.f;
            static char buf[256];
            int i, nearestIndex = -1;

            for(i=0; i<sPraetoriaCubemapLocs.numLocations; i++)
            {
                F32* beaconLoc = sPraetoriaCubemapLocs.locations[i];
                F32 distSq = SQR(cameraPos[0] - beaconLoc[0]) + SQR(cameraPos[2] - beaconLoc[2]); // ignore y component
                if( distSq < nearestDistSq )
                {
                    nearestDistSq = distSq;
                    nearestIndex = i;
                    //printf("index %d distance = %.2f  (cameraPos {%.1f,%.1f,%.1f},  beaconLoc {%.1f,%.1f,%.1f})\n", 
                    //    nearestIndex, sqrt(distSq), cameraPos[0], cameraPos[1], cameraPos[2], beaconLoc[0], beaconLoc[1], beaconLoc[2] );
                }
            }

            assert(nearestIndex >= 0 );
            // tbd, store name with BeaconList data
            sprintf(buf, "%s_cubemap_%02d_face0", sPraetoriaCubemapLocs.mapName, nearestIndex);
            cubeMapName = &buf[0];
        }
#endif // ENABLE_TEST_BEACONLIST

        pTestCubemap = texFind(cubeMapName);
        if( pTestCubemap )
        {
            assertmsgf(pTestCubemap->flags & TEX_CUBEMAPFACE, "Texture \"%s\" is not a cubemap.  Make sure texture trick has Cubemap flag.", pTestCubemap->name);
            texID = texDemandLoad(pTestCubemap);
        }
        else if(game_state.debugCubemapTexture)
        {
            assert(0);
        }

        if (gfx_state.mainViewport)
            cubemap_UseStaticCubemap();
    }
    else
    {
        if(s_bInitedCubemap)
        {
            // first face of cubemap has the cubemap texture
            texID = s_faces[0].viewport.PBufferTexture.color_handle[s_faces[0].viewport.PBufferTexture.curFBO];

#if (defined(_FULLDEBUG)) && !defined(TEST_CLIENT)
            // 03-29-10: Made this assert active only in full debug.  It was being triggered in I17 Closed Beta.
            assert(texID);
#endif
        }

        if (gfx_state.mainViewport)
            cubemap_UseDynamicCubemap();
    }

    return texID;
}

// Calculates the cubemap attenuation
F32 cubemap_CalculateAttenuation(bool bForTerrain, const Vec3 modelLoc, F32 modelRadius, bool bForceDynamic)
{
    F32 cubemap_attenuation = 0.0f;

    bool bUseStatic = game_state.game_mode != SHOW_GAME ||
        ( ( (game_state.static_cubemap_chars && !bForTerrain) || 
            (game_state.static_cubemap_terrain && bForTerrain) ) && !bForceDynamic);

    if (!bUseStatic)
    {
        F32 distance;
        //F32 model_radius = 3;
        F32 distance_in_model_radius;
        F32 reflection_attenuation;
        F32 attenuation_start = game_state.cubemapAttenuationStart;
        F32 attenuation_end = game_state.cubemapAttenuationEnd;

        if (!bForTerrain)
        {
            attenuation_start = game_state.cubemapAttenuationSkinStart;
            attenuation_end = game_state.cubemapAttenuationSkinEnd;
        }

        // Attenuate cubemap reflectivity based on distance and model size
        distance = distance3(gfx_state.cubemap_origin_vs, modelLoc);
        distance_in_model_radius = distance / modelRadius;
        reflection_attenuation = (distance_in_model_radius - attenuation_start) / (attenuation_end - attenuation_start);
        reflection_attenuation = 1.0f - CLAMPF32(reflection_attenuation, 0.0f, 1.0f);
        cubemap_attenuation = reflection_attenuation;
    }
    else
    {
        // no attenuation when using static cubemap
        cubemap_attenuation = 1.0f;
    }

    // Attenuate attenuation based on time of day if outdoors
    if ( game_state.game_mode == SHOW_GAME && !isIndoors() )
    {
        cubemap_attenuation *= 1.0f - (g_sun.lamp_alpha * game_state.buildingReflectionNightReduction / 255.0f);
    }

    return cubemap_attenuation;
}

void cubemap_UseStaticCubemap()
{
    if (s_usingInvCamMatrix != 1)
    {
        rdrQueue( DRAWCMD_CUBEMAP_SETINVCAMMATRIX,NULL,0);
        s_usingInvCamMatrix = 1;
    }
}

void cubemap_UseDynamicCubemap()
{
    if (game_state.cubemapUpdate3Faces && s_usingInvCamMatrix != 0)
    {
        rdrQueue( DRAWCMD_CUBEMAP_SETSET3FACEMATRIX,NULL,0);
        s_usingInvCamMatrix = 0;
    }
    else if (!game_state.cubemapUpdate3Faces && s_usingInvCamMatrix != 1)
    {
        rdrQueue( DRAWCMD_CUBEMAP_SETINVCAMMATRIX,NULL,0);
        s_usingInvCamMatrix = 1;
    }
}

