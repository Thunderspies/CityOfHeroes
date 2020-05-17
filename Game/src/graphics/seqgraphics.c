/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 */
#include "graphics/seqgraphics.h"
#include <utilitieslib/utils/wininclude.h>
#include <string.h>
#include <time.h>
#include "seq/seq.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "assert.h"
#include "seq/anim.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/HashFunctions.h>
#include "graphics/font.h"
#include "seq/animtrack.h"
#include "seq/seqanimate.h"
#include "seq/animtrackanimate.h"
#include <utilitieslib/utils/strings_opt.h>
#include "entity/costume.h"
#include "seq/tricks.h"
#include "render/tex.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/entity.h"
#include "entity/motion.h"
#include "graphics/gfxwindow.h"
#include "graphics/FX/fxlists.h"
#include "graphics/camera.h"
#include "render/render.h"
#include "sound/sound.h"
#include "graphics/font.h"
#include "cmdparse/cmdgame.h"
#include "graphics/FX/fx.h"
#include "graphics/light.h"
#include <utilitieslib/utils/timing.h>
#include "render/model_cache.h"
#include "win/win_init.h" //moving
#include "graphics/gfx.h"
#include "render/tex_gen.h"
#include "graphics/textureatlas.h"
#include "player/player.h"
#include "graphics/sun.h"
#include <utilitieslib/utils/FolderCache.h>
#include "seq/gfxtree.h"
#include "gameComm/npc.h"        // For NPC structure defintion
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "seq/seqregistration.h"
#include "render/renderprim.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"
#include "group/group.h"
#include "graphics/groupMiniTrackers.h"
#include <utilitieslib/components/estring.h>
#include "graphics/imageCapture.h"
#include "UI/uiPictureBrowser.h"
#include "entity/EntPlayer.h"
#include "graphics/viewport.h"
#include "graphics/jpeg.h"
#include "render/thread/rt_state.h"

// Used by takePicture()
#define NUM_TAKEPICTURE_INFOS 16

typedef struct 
{
    bool inited;
    bool inUse;
    ViewportInfo     viewport_1;
    ViewportInfo     viewport_2;

    // Initialized in takePicture
    Entity *entity;
    SeqInst * seq;
    int freeEntity;

    // Set in precallback, used in postcallback
    Mat4 savedMat;
    int heldBackgroundLoaderState;
    int hidePlayer;
    int game_mode;
    int saveClipper;
    int disable2d;
    F32 preservedLegScale;
    F32 timestep_save;
    F32 timestep_noscale_save;

    F32 entity_y_offset;
    bool newImage;

} TakePictureInfo;

static int s_next_takepicture_index = 0;
static TakePictureInfo    s_takepicture_info[NUM_TAKEPICTURE_INFOS];

//Set the GFX_USE_CHILD bit, recurse up

// A version of texNeedsAlphaSort that takes into account that Boned stuff doesn;t explicitly set
// BLENDMODE_COLORBLEND_DUAL in the texture ( TO DO, rationalize this so all textures know what their
// blend mode is without needing context


//Figure out if this thing has any alpha on it.
//Check for loading objects flag makes LOADING work
void animCalculateSortFlags( GfxNode * gfxnode )
{
    GfxNode *   node;
    Model   *    model;

    for( node = gfxnode ; node ; node = node->next )
    {
        if (node->clothobj)
            continue; // Cloth objects set their own flags

        model = node->model;

        if( model )
        {    
            node->flags &= ~GFXNODE_ALPHASORT;
            //If you have a customtex, check second texture.
            if( node->customtex.base )
            {
                if( node->customtex.generic && ( node->customtex.generic->flags & TEX_ALPHA ) && 
                    !(node->model && (node->model->flags & OBJ_TREEDRAW)) &&
                    !(node->tricks && (node->tricks->flags1 & TRICK_ALPHACUTOUT)))
                {
                    node->flags |= GFXNODE_ALPHASORT;
                }
            }
            else //otherwise check the model for any textures with alpha
            {
                if (modelNeedsAlphaSort(model) &&
                    !(node->tricks && (node->tricks->flags1 & TRICK_ALPHACUTOUT)))
                    node->flags |= GFXNODE_ALPHASORT;
            }

            gfxTreeSetBlendMode(node);
            gfxNodeFreeMiniTracker(node);
            gfxNodeBuildMiniTracker(node);
        }


        if( node->child )
            animCalculateSortFlags( node->child );        
    }
}



//###Set Sequencer State and Sequencer LOD stuff

void minMaxVec3(Vec3 newvec, Vec3 min, Vec3 max)
{
    min[0] = MIN(min[0], newvec[0]);
    min[1] = MIN(min[1], newvec[1]);
    min[2] = MIN(min[2], newvec[2]);
    max[0] = MAX(max[0], newvec[0]);
    max[1] = MAX(max[1], newvec[1]);
    max[2] = MAX(max[2], newvec[2]);
}

static BoneId extentbones[] = // This list is for bones we check when calculating extents.  Some bones like WepR need to be ignored 
{
    BONEID_HIPS,
    BONEID_WAIST,
    BONEID_CHEST,
    BONEID_NECK,
    BONEID_HEAD,
    BONEID_COL_R,
    BONEID_COL_L,
    BONEID_UARMR,
    BONEID_UARML,
    BONEID_LARMR,
    BONEID_LARML,
    BONEID_HANDR,
    BONEID_HANDL,
    BONEID_ULEGR,
    BONEID_ULEGL,
    BONEID_LLEGR,
    BONEID_LLEGL,
    BONEID_FOOTR,
    BONEID_FOOTL,
    BONEID_TOER,
    BONEID_TOEL,
    BONEID_HIND_ULEGL,
    BONEID_HIND_LLEGL,
    BONEID_HIND_FOOTL,
    BONEID_HIND_TOEL,
    BONEID_HIND_ULEGR,
    BONEID_HIND_LLEGR,
    BONEID_HIND_FOOTR,
    BONEID_HIND_TOER,
    BONEID_FORE_ULEGL,
    BONEID_FORE_LLEGL,
    BONEID_FORE_FOOTL,
    BONEID_FORE_TOEL,
    BONEID_FORE_ULEGR,
    BONEID_FORE_LLEGR,
    BONEID_FORE_FOOTR,
    BONEID_FORE_TOER,
}; 

static void s_destroyTempEnt(Entity *e)
{
    if(!e)
        return;

    e->waitingForFadeOut =1;
    e->currfade = 0;
}

int isExtentBone(BoneId bone)
{
    int i;
    for(i = 0; i < ARRAY_SIZE(extentbones); i++)
        if(extentbones[i] == bone)
            return 1;
    return 0;
}

//Finds view space top, bottom, left and right extremes of an anim
void seqGetExtremes( GfxNode * node, int seqHandle, SeqGfxData * seqGfxData, Vec3 ul, Vec3 lr, Vec3 worldMin, Vec3 worldMax )
{
    Vec3 vec;
    Vec3 vecScreen;
  
    for(; node ; node = node->next)
    {
        if( node->seqHandle == seqHandle && node->anim_id != BONEID_MYSTIC && node->anim_id != BONEID_BENDMYSTIC && node->useFlags ) //mystic bones can be anywhere...
        { 
            if(bone_IdIsValid(node->anim_id) && isExtentBone(node->anim_id))
            {
                mulVecMat4( seqGfxData->btt[node->anim_id], seqGfxData->bpt[node->anim_id], vec );
                gfxWindowScreenPos( vec, vecScreen );
                ul[0] = MIN( ul[0], vecScreen[0] ); 
                ul[1] = MAX( ul[1], vecScreen[1] );
                lr[0] = MAX( lr[0], vecScreen[0] );
                lr[1] = MIN( lr[1], vecScreen[1] );
                minMaxVec3(vec, worldMin, worldMax);
            }
            if (node->child)
            {
                seqGetExtremes( node->child, seqHandle, seqGfxData, ul, lr, worldMin, worldMax );
            }
        }
    }
    return;
}

void findMinMax( Vec2 ul, Vec2 lr, Vec3 corner )
{
    Vec3 cornerX;

    //DEBUG
    if(0){  
        Vec3 cornerup;
        copyVec3( corner, cornerup );
        cornerup[1] += 1;
        drawLine3D( cornerup, corner, 0xffffffff);
    }
    //END DEBUG
 
    mulVecMat4( corner, cam_info.viewmat, cornerX );    

    ul[0] = MIN( ul[0], cornerX[0] );   
    ul[1] = MAX( ul[1], cornerX[1] );
    ul[2] = MAX( ul[2], cornerX[2] );

    lr[0] = MAX( lr[0], cornerX[0] );
    lr[1] = MIN( lr[1], cornerX[1] );
    lr[2] = MAX( lr[2], cornerX[2] );
}

///Build a box around the world object and find the camera space extremes
int seqGetWorldGroupDimensions( SeqInst * seq, Vec3 ul, Vec3 lr )
{
    Mat4 mat;

    copyMat4( seq->gfx_root->mat, mat );
    zeroVec3( ul );
    zeroVec3( lr );
 
    if( seq->worldGroupPtr )  
    {
        GroupDef * def = seq->worldGroupPtr;

        Vec3 min;
        Vec3 max;
        Vec3 corner;
        Vec3 mid;
        Vec3 midx;

        copyVec3( mat[3], min );
        copyVec3( mat[3], max );

        if ( 1 || seq->type->flags & SEQ_USE_DYNAMIC_LIBRARY_PIECE )
        {
            // This def is probably base geometry, give it special treatment
            groupGetBoundsMinMaxNoVolumes( def, mat, min, max );
        }
        else
        {
            groupGetBoundsMinMax( def, mat, min, max );   
        }

        ul[0] = +1000000;      
        ul[1] = -1000000;
        ul[2] = -1000000; 
        lr[0] = -1000000;
        lr[1] = +1000000;
        lr[2] = -1000000;

        findMinMax( ul, lr, min );  
        findMinMax( ul, lr, max );
        corner[0] = min[0]; 
        corner[1] = min[1];
        corner[2] = max[2];
        findMinMax( ul, lr, corner );
        corner[0] = min[0];
        corner[1] = max[1];
        corner[2] = max[2];
        findMinMax( ul, lr, corner ); 
        corner[0] = min[0]; 
        corner[1] = max[1];
        corner[2] = min[2];
        findMinMax( ul, lr, corner );
        corner[0] = max[0];
        corner[1] = max[1];
        corner[2] = min[2];
        findMinMax( ul, lr, corner );
        corner[0] = max[0];
        corner[1] = min[1];
        corner[2] = max[2];
        findMinMax( ul, lr, corner );
        corner[0] = max[0]; 
        corner[1] = min[1];
        corner[2] = min[2];
        findMinMax( ul, lr, corner ); 

        //Use ther middle of the box for the z to minimize parallax
        addVec3( min, max, mid );
        scaleVec3( mid, 0.5, mid );
        mulVecMat4( mid, cam_info.viewmat, midx );
        ul[2] = lr[2] = midx[2];
        return 1;  
    }

    return 0;
}

// Steady top is the top of the character's collision box * his geometry scale in camera space
//
int seqGetSteadyTop( SeqInst * seq, Vec3 steady_top )
{
    if( !seq )
    {
        zeroVec3(steady_top);
        return FALSE;
    }
        
    if( seq->type->selection == SEQ_SELECTION_LIBRARYPIECE ) 
    {
        Vec3 ul, lr;
        if( seqGetWorldGroupDimensions( seq, ul, lr ) )
        {
            steady_top[0] = ul[0];
            steady_top[1] = ( ul[1] - lr[1] ) / 2;
            steady_top[2] = ul[2];
            return TRUE;
        }
    }

    //SEQ_SELECTION_CAPSULE uses this by default
    //SELECTION_LIBRARYPIECE falls through to here, 
    //SELECTION_BONES uses capsule for steady top -- since it's supposed to be steady
    {
        Mat4 mat;
        Mat4 mat2;
        Vec3 size;
        Vec3 offset;
        bool bScaleCapsuleByGeomScale;

        seqGetCapsuleSize(seq, size, offset);

        //MW changed seq->basegeomscale[1] to seq->type->geomscale[1] because randomly scaled monsters were looking weird
        // I'm assuming that the collision box is designed for the geomScale even if it is the small end of the spectrum.
        // fpe 10/30/2010 -- steadytop location is used to draw debug text and chat text.  To fix a bug where chat text appears WAY over the head
        //    of some large monsters (which have costume scale of 3.5x or greater), I am removing the geomscale here by default but leaving a debug
        //    way to revert to old behavior so Colin can see if this is causing any problems.  The other solution would be to have them enter capsule
        //    sizes in the ent type definition, which seems to be the correct fix (since the same ent type can have varying scales when used with different
        //    costumes), but that change is more far reaching and all capsules would have to be resized.
        bScaleCapsuleByGeomScale = (game_state.ent_debug_client & 512) != 0;
        if(bScaleCapsuleByGeomScale)
            size[1] *= seq->currgeomscale[1] / seq->type->geomscale[1]; // Only want to scale this by how much the guy is currently scaled up
        copyMat4(seq->gfx_root->mat, mat);
        mat[3][1] += size[1];
        mat[3][1] += seq->type->reticleHeightBias;
        mulMat4(cam_info.viewmat, mat, mat2);
        copyVec3(mat2[3],steady_top);
        return TRUE;
    }

}

////////// Get Capsule and get Entity Matrix in the correct position for the capsule
int seqGetCollisionBrackets( SeqInst * seq, const Mat3 entMat, Vec3 ul, Vec3 lr, Vec2 screenUL, Vec2 screenLR, Vec3 worldMin, Vec3 worldMax  )
{
    Mat4 collMat;
    F32 bias, basebias;
    EntityCapsule cap;

    bias = seq->type->reticleHeightBias; 
    basebias = seq->type->reticleBaseBias;  

    getCapsule(seq, &cap ); 
    positionCapsule( entMat, &cap, collMat ); 

    /////////////// Now we have right entMat and a capsule, find the screen coordinate extremes
    {
        Vec3 point1;
        Vec3 point2;
        Vec3 point1x;
        Vec3 point2x;

        Vec3 point1ul;
        Vec3 point1ur;
        Vec3 point1ll;
        Vec3 point1lr;

        Vec3 point2ul;
        Vec3 point2ur;
        Vec3 point2ll;
        Vec3 point2lr;

        int i;

        Vec2 pointScreen[8]; //By here I'm more interested in easy looping than keeping their status straight

        point1[0]=point1[1]=point1[2]=0;
        point2[0]=point2[1]=point2[2]=0;

        // Find point 1 & 2 and put them in camera space
        point2[1] += cap.length;
        mulVecMat4( point1, collMat, point1x ); 
        mulVecMat4( point2, collMat, point2x ); 

        //drawLine3DWidth( point1x, point2x, 0xffffffff, 20); 

        mulVecMat4( point1x, cam_info.viewmat, point1 );
        mulVecMat4( point2x, cam_info.viewmat, point2 );

        //mulVecMat4( entMat[3], cam_info.viewmat, point1 );//DEBUG
        //mulVecMat4( entMat[3], cam_info.viewmat, point2 );//DEBUG

        // Use point1x and point2x find front facing square of each capsule sphere
        //( Use reticle height bias to lift top of both front facing squares (making rectangles) )
        copyVec3( point1, point1ul );
        point1ul[0] -= cap.radius;
        point1ul[1] += cap.radius + bias; 
        point1ul[2] += cap.radius;
        copyVec3( point1, point1ur );
        point1ur[0] += cap.radius;
        point1ur[1] += cap.radius + bias; 
        point1ur[2] += cap.radius; 
        copyVec3( point1, point1ll );
        point1ll[0] -= cap.radius;
        point1ll[1] -= cap.radius + basebias; 
        point1ll[2] -= cap.radius; 
        copyVec3( point1, point1lr );
        point1lr[0] += cap.radius;
        point1lr[1] -= cap.radius + basebias; 
        point1lr[2] -= cap.radius; 

        copyVec3( point2, point2ul );
        point2ul[0] -= cap.radius;
        point2ul[1] += cap.radius + bias; 
        point2ul[2] += cap.radius; 
        copyVec3( point2, point2ur );
        point2ur[0] += cap.radius;
        point2ur[1] += cap.radius + bias; 
        point2ur[2] += cap.radius; 
        copyVec3( point2, point2ll );
        point2ll[0] -= cap.radius;
        point2ll[1] -= cap.radius + basebias;
        point2ll[2] -= cap.radius;
        copyVec3( point2, point2lr );
        point2lr[0] += cap.radius;
        point2lr[1] -= cap.radius + basebias;
        point2lr[2] -= cap.radius;

        // Find screenspace coords of all eight corners
        gfxWindowScreenPos( point1ul, pointScreen[0] );
        gfxWindowScreenPos( point1ll, pointScreen[1] );
        gfxWindowScreenPos( point1ur, pointScreen[2] );
        gfxWindowScreenPos( point1lr, pointScreen[3] );
        gfxWindowScreenPos( point2ul, pointScreen[4] );
        gfxWindowScreenPos( point2ll, pointScreen[5] );
        gfxWindowScreenPos( point2ur, pointScreen[6] );
        gfxWindowScreenPos( point2lr, pointScreen[7] );

        // Find extremes in world space
        setVec3(worldMin, 1000000, 1000000, 1000000);
        setVec3(worldMax, -1000000, -1000000, -1000000);
        minMaxVec3(point1ul, worldMin, worldMax);
        minMaxVec3(point1ll, worldMin, worldMax);
        minMaxVec3(point1ur, worldMin, worldMax);
        minMaxVec3(point1lr, worldMin, worldMax);
        minMaxVec3(point2ul, worldMin, worldMax);
        minMaxVec3(point2ll, worldMin, worldMax);
        minMaxVec3(point2ur, worldMin, worldMax);
        minMaxVec3(point2lr, worldMin, worldMax);

        //Find extremes of the corner points to find the screenUL and screenUL 
        screenUL[0] = +1000000; 
        screenUL[1] = -1000000;
        screenLR[0] = -1000000;
        screenLR[1] = +1000000;

        for( i = 0 ; i < 8 ; i++ ) 
        {
            F32 * pnt = pointScreen[i];

            //Is it leftest?
            if( pnt[0] < screenUL[0] )
                screenUL[0] = pnt[0];

            //Is it rightest?
            if( pnt[0] > screenLR[0] )
                screenLR[0] = pnt[0];

            //Is it highest? 
            if( pnt[1] > screenUL[1] )
                screenUL[1] = pnt[1];

            //Is it lowest?
            if( pnt[1] < screenLR[1] )
                screenLR[1] = pnt[1];
        }

        i++;
    }

    return TRUE;
}




int GetHeadScreenPos( SeqInst * seq, Vec3 HeadScreenPos )
{
    int boneNum;
    GfxNode * node;
    Vec3 vec;

    boneNum = BONEID_HAIR;
    node = gfxTreeFindBoneInAnimation( boneNum, seq->gfx_root->child, seq->handle, 1 );
    if( !node || !node->useFlags )
    {
        boneNum = BONEID_HEAD;
        node = gfxTreeFindBoneInAnimation( boneNum, seq->gfx_root->child, seq->handle, 1 );
    }
    if( !node || !node->useFlags )
    {
        boneNum = BONEID_HIPS;
        node = gfxTreeFindBoneInAnimation( boneNum, seq->gfx_root->child, seq->handle, 1 );
    }
    if( !node ) //Should be assert
        return 0;

    mulVecMat4( seq->seqGfxData.btt[boneNum], seq->seqGfxData.bpt[boneNum], vec );
    gfxWindowScreenPos( vec, HeadScreenPos );

    return 1;
}

bool seqGetScreenAreaOfBones( SeqInst * seq, Vec2 screenUL, Vec2 screenLR, Vec3 worldMin, Vec3 worldMax )
{
    GfxNode * node;
    SeqGfxData * seqGfxData;
    Vec3 t1;
    Vec3 ul;
    Vec3 lr;
    Vec3 tempa, tempb;

    if (!worldMin)
        worldMin = tempa;
    if (!worldMax)
        worldMax = tempb;

    seqGfxData = &seq->seqGfxData;
    node = seq->gfx_root->child; 

    //If this thing is off screen or just messed up, bail
    if( seq->gfx_root->flags & GFXNODE_HIDE || !seq->gfx_root->child || seq->gfx_root->child->anim_id != 0 )
        return false;

    //Restore the pretranslated bonepositions back to original spots 
    mulVecMat4( seqGfxData->btt[node->anim_id], seqGfxData->bpt[node->anim_id], t1 );

    if (t1[0] == 0.0f || t1[1] == 0.0f || t1[2] == 0.0f)
        return false;//failure //if hips are at 0, something is wrong

    {
        gfxWindowScreenPos( t1, ul );
        copyVec2(ul, lr);
        copyVec3(t1, worldMin);
        copyVec3(t1, worldMax);

        seqGetExtremes( node, seq->handle, &(seq->seqGfxData), ul, lr, worldMin, worldMax );

        {
            Vec3 pos;
            Vec2 s1, s2;

            //Trick to add a little height to the reticle
            mulVecMat4( seqGfxData->btt[node->anim_id], seqGfxData->bpt[node->anim_id], pos );
            gfxWindowScreenPos( pos, s1 );
            pos[1] += seq->type->reticleHeightBias;
            gfxWindowScreenPos( pos, s2 );
            ul[1] += s2[1] - s1[1] ;

            //Trick to add a little room to the base of the to the reticle
            mulVecMat4( seqGfxData->btt[node->anim_id], seqGfxData->bpt[node->anim_id], pos );
            gfxWindowScreenPos( pos, s1 );
            pos[1] += seq->type->reticleBaseBias;
            gfxWindowScreenPos( pos, s2 );
            lr[1] += s2[1] - s1[1];
        }

        copyVec2( ul, screenUL );
        copyVec2( lr, screenLR );

        return true;
    }
}


//gets upper left and lower right extremes of a character's position in screen coords
//only correct after gfx treedraw has been called.
int seqGetSides( SeqInst * seq, const Mat4 entmat, Vec2 screenULVec2, Vec2 screenLRVec2, Vec3 worldMin, Vec3 worldMax )
{
    eSelection actualSelectionUsed = 0;
    Vec3 tempa, tempb;
    Vec3 screenUL;
    Vec3 screenLR;
    int screenW, screenH;

    if (!worldMin)
        worldMin = tempa;
    if (!worldMax)
        worldMax = tempb;

    if( !seq || !seq->gfx_root ) 
    {
        //Only way we can get here is an unformed entity, just bail
        screenUL[0] = screenUL[1] = 0;
        screenLR[0] = screenLR[1] = 0;
        setVec3(worldMin, 0, 0, 0);
        setVec3(worldMax, 0, 0, 0);
        return false;
    }

    if( seq->type->selection == SEQ_SELECTION_LIBRARYPIECE )
    {
        if( seqGetWorldGroupDimensions( seq, worldMin, worldMax ) )
        {
            gfxWindowScreenPos( worldMin, screenUL );
            gfxWindowScreenPos( worldMax, screenLR );
            actualSelectionUsed = SEQ_SELECTION_LIBRARYPIECE;
        }
    }
    else if( seq->type->selection == SEQ_SELECTION_BONES )
    {
        if( seqGetScreenAreaOfBones( seq, screenUL, screenLR, worldMin, worldMax ) )
            actualSelectionUsed = SEQ_SELECTION_BONES;
    }

    //If neither of the first two worked, get the capsule's reticle -- it will always work
    if( !actualSelectionUsed || seq->type->selection == SEQ_SELECTION_CAPSULE )
    {
        Vec3 ul;
        Vec3 lr;
        seqGetCollisionBrackets( seq, entmat, ul, lr, screenUL, screenLR, worldMin, worldMax );
        actualSelectionUsed = SEQ_SELECTION_CAPSULE;
    }

    windowSize(&screenW, &screenH);

    /////////// Now we have values in screenUL and screenLR: Interpolate if needed
    {
        #define FRAMES_TO_INTERP_RETICLE (15.0)
        Mat4 meInCamSpace;
        Vec3 root;

        //Find root, so cached reticle coords are relative to ent position -- feels a little better, no reticle lag as you turn camera
        mulMat4( cam_info.viewmat, seq->gfx_root->mat, meInCamSpace );
        gfxWindowScreenPos( meInCamSpace[3], root );   

        //If it was selected last frame, and selected in a different way now, then start the interpolation 
        if( 0 && seq->selectionUsedLastFrame && actualSelectionUsed != seq->selectionUsedLastFrame
            && seq->visStatusLastFrame == ENT_OCCLUDED ) //Dont interoplate if you couldn't see the reticle 
            seq->reticleInterpFrame = FRAMES_TO_INTERP_RETICLE; //Do interpolation between reticles

        //Interpolate to the new reticle if needed
        if( seq->reticleInterpFrame > 0.0 )  
        {
            Vec3 screenULOld = {0};
            Vec3 screenLROld = {0};
            F32 percent_new;

            percent_new = 1.0 - (seq->reticleInterpFrame / FRAMES_TO_INTERP_RETICLE);

            //Get cached reticle, relative to current position
            addVec2( root, seq->ulFromRoot, screenULOld );
            addVec2( root, seq->lrFromRoot, screenLROld );

            //Interpolate
            interpVec2( percent_new, screenULOld, screenUL, screenULVec2 );
            interpVec2( percent_new, screenLROld, screenLR, screenLRVec2 );
        }
        else //No interpolation
        {
            copyVec2( screenUL, screenULVec2 ); 
            copyVec2( screenLR, screenLRVec2 );

            //Cache current reticle values in case you need them next frame
            subVec3( screenULVec2, root, seq->ulFromRoot ); 
            subVec3( screenLRVec2, root, seq->lrFromRoot );
        }

        //selectionUsedLastFrame set and reticleInterpFrame incremented in ent ClientUpdate so seqGetSides can be called
        //several times a frame on the same entity
        //TO DO since entClientUpdate can also be called multiple times per frame, theoretically, this is probably not 
        //the best solution.  Maybe switch to absolute timestamps.  
        seq->selectionUsedThisFrame = actualSelectionUsed; 
    }

    return true;
}

/*assumes MAX_LODS 4 && loddist[0] == 0
  does a setheader (move out to entrecv for consistency?)
  note if dist == 0 for loddist 1, 2, or 3, that is
*/
#define MAX_CAMDIST_BEFORE_UPPING_LODS 40.0
void seqSetLod(SeqInst * seq, F32 len)
{
    F32 d1, d2, d3;
    int lod, oldlod = -1;

    Entity *e = playerPtr();

    d1 = MAX( 0, seq->type->loddist[1] * game_state.LODBias ); //distance to go from lod0 to lod1
    d2 = MAX( 0, seq->type->loddist[2] * game_state.LODBias ); //                     lod1 to lod2
    d3 = MAX( 0, seq->type->loddist[3] * game_state.LODBias ); //                     lod2 to lod3
    //see seqtype load for how unset lods are set to the one higher

    if( game_state.camdist > MAX_CAMDIST_BEFORE_UPPING_LODS )
    {
        d1 += (game_state.camdist - MAX_CAMDIST_BEFORE_UPPING_LODS);
        d2 += (game_state.camdist - MAX_CAMDIST_BEFORE_UPPING_LODS);
        d3 += (game_state.camdist - MAX_CAMDIST_BEFORE_UPPING_LODS);
    }

    lod = seq->lod_level;    
    assert(lod >= 0 && lod < MAX_LODS);

    oldlod = -1;
    while(oldlod != lod)
    {
        oldlod = lod;
        if( lod == 0 )
        {
            if( len > d1+9 )
                lod++;
        }
        else if( lod == 1 )
        {
            if( len < d1 )
                lod--;
            else if( len > d2+9 )
                lod++;
        }
        else if( lod == 2 )
        {
            if( len < d2 )
                lod--;
            else if( len > d3+9 )
                lod++;
        }
        else if( lod == 3 )
        {
            if( len < d3 )
                lod--;
        }
    }
    
    //Hack so character looks good in the UI
    if( game_state.game_mode == SHOW_TEMPLATE )
        lod = 0;
    //End Hack

    if (e && e->seq == seq)
        lod = 0;

    if (game_state.lod_entity_override)
        lod = game_state.lod_entity_override - 1;

    if(lod != seq->lod_level)
    {
        seq->lod_level                = lod;
        seq->updated_appearance        = 1;
        //printToScreenLog(1, "lod %d", lod );
    }
}



///#################################################
//### I don't know where these last things go


#ifdef CLIENT
void printSeqType( SeqInst * seq)
{
    int x = 50;
    int y = 60;
    const SeqType * type;

    type = seq->type;

    //print enttype info
    xyprintfcolor( x, y++,255,100,100," EntType (ent_types/%s.txt) ", type->name);
    xyprintf( x, y++,"%-55s ", "");
    xyprintf( x, y++,"Name           %-40s ", type->name );
    xyprintf( x, y++,"Graphics       %-40s ", type->graphics );
    xyprintf( x, y++,"LOD1_Dist      %6.0f                                   ", type->loddist[1] );
    xyprintf( x, y++,"LOD2_Dist      %6.0f                                   ", type->loddist[2] );
    xyprintf( x, y++,"LOD3_Dist      %6.0f                                   ", type->loddist[3] );
    xyprintf( x, y++,"FadeOutStart   %6.0f                                   ", type->fade[0] );
    xyprintf( x, y++,"FadeOutFinish  %6.0f                                   ", type->fade[1] );
    xyprintf( x, y++,"VisSphereRadius%6.0f                                   ", type->vissphereradius );
    xyprintf( x, y++,"%-55s ", "");
}

int printGeometry( GfxNode * node, int seqHandle, int * y )
{
    GfxNode * mynode;
    int totalTris = 0;

    mynode = node;
    for( mynode = node ; node ; node = node->next )
    {
        if( node->seqHandle == seqHandle )
        {
            if( node->model )
            {
                xyprintf( 2, (*y)++,"  %-25s %4d tris", node->model->name, node->model->tri_count );
                totalTris += node->model->tri_count;
            }
            if( node->child )
            {
                totalTris += printGeometry( node->child, seqHandle, y );
            }
        }
    }

    return totalTris;
}

void printLod(SeqInst * seq)
{
    int x = 2;
    int y = 60;
    int totalTris;
    Vec3 ent_pos_dv;
    F32 ent_dist;
    const SeqType * type;

    printSeqType( seq );

    type = seq->type;

    subVec3(  seq->gfx_root->mat[3], cam_info.cammat[3], ent_pos_dv );
    ent_dist = lengthVec3( ent_pos_dv );

    xyprintf( x, y++,"%-20s ", "");
    xyprintf( x, y++,"Distance From Camera: %0.0f       ", ent_dist );
    xyprintf( x, y++,"Current LOD level:    %d          ", seq->lod_level );
    xyprintf( x, y++,"Geometry:                         ");
    
    totalTris = printGeometry( seq->gfx_root->child, seq->handle, &y );

    xyprintfcolor( x, y++,255, 200, 200,"                Total Tris: %d     ", totalTris );
    //print tri count

}

void printScreenSeq(SeqInst * seq)
{
    int i,j;
    char empty[1];
    int x = 2;
    int y = 2;
    char alpha[] = "(A)";
    char noalpha[] = "(O)";
    char *s;

    if(!seq)
    {
        xyprintf( x, y++,"\n\nBad Entity with no sequencer\n\n");
        return;
    }

    empty[0] = 0;
    
    xyprintf( x, y++,"%-100s ", "");
    xyprintf( x, y++,"Custom Graphics                                                                                      ");
    xyprintf( x, y++,"%-100s ", "");
    xyprintf( x, y++,"GEOMETRY: name of the .geo (.wrl file) . name of the model in the .geo file                          ");
    xyprintf( x, y++,"          'default' = use 'GEO_thisbone' in %-57s", seq->type->graphics);
    xyprintf( x, y++,"TEXTURE:  .tgas. If nothing is listed use default textures on the geometry                           ");
    xyprintf( x, y++,"COLORS:   color1 rgba / color2 rgba                                                                  ");
    xyprintf( x, y++,"( if a bone isn't listed, it has no custom anything )                                                ");
    xyprintf( x, y++,"%-100s ", "");
    xyprintf( x, y++,"EntType: %s  (info on this type is in data/ent_types/%s.txt)                                          ", seq->type->name, seq->type->name);
    xyprintf( x, y++,"GeomScale: %f %f %f  BoneScaleRatio: %f                                                            ", seq->currgeomscale[0],seq->currgeomscale[1],seq->currgeomscale[2], seq->bonescale_ratio);
    xyprintf( x, y++,"%-140s ", "");
    xyprintfcolor( x, y++, 255,101,100,"BONE    GEOMETRY                                TEXTURE1                        TEXTURE2                       COLORS                           ");
    xyprintf( x, y++,"%-140s ", "");
    for(i = 0; i < BONEID_COUNT; i++)
    {
        bool needtoshow=seq->newgeometry[i] || seq->newtextures[i].base || seq->newtextures[i].generic || (*(U32 *)seq->newcolor1[i]) || (*(U32 *)seq->newcolor2[i]);
        if( 1 || needtoshow )
        {
            char buf2[100];
            char buf[1000];
            char bigbuf[1000];
            GfxNode * node;
            node = seqFindGfxNodeGivenBoneNum(seq, i);

            memset( bigbuf, 0, 1000 );
            sprintf( buf, "%-7s ", bone_NameFromId( i ) );
            strcat( bigbuf, buf );

            //Geometry
            if( seq->newgeometry[i])
            {
                //Make sure this bone exists
                if( seq->newgeometryfile[i] )
                { //not NONE
                    if( !node ) {
                        Errorf( "The costume includes the piece %s, but the skeleton has no %s bone. (Assuming this is the highest LOD )",
                            seq->newgeometry[i],
                            bone_NameFromId( i ) );
                    } else {
                        if (strStartsWith(seq->newgeometryfile[i], "player_library/")) {
                            sprintf( buf2, "%s/%s", seq->newgeometryfile[i] + strlen("player_library/"),seq->newgeometry[i]);
                        } else {
                            sprintf( buf2, "%s/%s", seq->newgeometryfile[i],seq->newgeometry[i]);
                        }
                        sprintf(buf, "%-40s", buf2);
                    }
                } else {
                    if (node && node->model) {
                        if (s=strchr(node->model->gld->name, '/'))
                            s++;
                        else
                            s = node->model->gld->name;
                        strcpy(buf2, s);
                        if (s=strchr(buf2, '.'))
                            *s=0;
                        strcat( buf2, "/");
                        strcat( buf2, node->model->name);
                        sprintf( buf, "%-40s", buf2);
                    } else
                        sprintf( buf, "%-40s", "NONE");
                }
            }
            else
            {
                if (!needtoshow && (!node || !node->model))
                    continue;

                if (node && node->model) {
                    if (s=strchr(node->model->gld->name, '/'))
                        s++;
                    else
                        s = node->model->gld->name;
                    strcpy(buf2, s);
                    if (s=strchr(buf2, '.'))
                        *s=0;
                    strcat( buf2, "/");
                    strcat( buf2, node->model->name);
                    sprintf( buf, "%-40s", buf2);
                } else {
                    sprintf( buf, "%-40s", "default");
                }
            }
            strcat( bigbuf, buf );


            //Textures
            if( seq->newtextures[i].base )
            {
                char * a;
                if( seq->newtextures[i].base->tex_layers[TEXLAYER_BASE1]->flags & TEX_ALPHA )
                    a = alpha;
                else
                    a = noalpha;

                sprintf( buf, "%s%-28s", a, seq->newtextures[i].base->name);
            }
            else
            {
                if(seq->newtextures[i].generic)
                    sprintf( buf, "   %-28s", "white");
                else {
                    if (node && node->model && node->model->tex_count) {
                        strcpy(buf2, node->model->tex_binds[0]->name);
                        for (j=1; j<node->model->tex_count; j++) {
                            strcat(buf2, ",");
                            strcat(buf2, node->model->tex_binds[j]->name);
                        }
                        sprintf( buf, "   %-28s", buf2);
                    } else {
                        sprintf( buf, "   %-28s", "none?");
                    }
                }
            }
            strcat( bigbuf, buf );

            if( seq->newtextures[i].generic )
            {
                char * a;
                if( seq->newtextures[i].generic->flags & TEX_ALPHA )
                    a = alpha;
                else
                    a = noalpha;

                sprintf( buf, " %s%-28s", a, seq->newtextures[i].generic->name);
            }
            else
            {
                if(seq->newtextures[i].base)
                    sprintf( buf, " %s%-28s", noalpha, "white");
                else {
                    sprintf( buf, " %s%-28s", "(?)", "default");
                }
            }
            strcat( bigbuf, buf );

            //Colors
            if( (*(U32 *)seq->newcolor1[i]) || (*(U32 *)seq->newcolor2[i])  )
            {
                U8 * c1 = seq->newcolor1[i];
                U8 * c2 = seq->newcolor2[i];
                sprintf( buf,  "%-4u%-4u%-4u%-4u/%-4u%-4u%-4u%-4u",
                    c1[0],c1[1],c1[2],c1[3],c2[0],c2[1],c2[2],c2[3]);
            }
            else
            {
                sprintf( buf, "%-40s", "no custom colors");
            }
            strcat( bigbuf, buf );

            xyprintf( x, y++, "%s", bigbuf);
        }
    }
}
#endif

//#################################################################
//##########Load SeqInst
static int getBonesThatNeedStaticLight(GfxNode * node, GfxNode * bones[BONEID_COUNT], int seqHandle)
{
    int hits = 0;

    for(; node ; node = node->next)
    {
        if( node->seqHandle == seqHandle )
        {
            if( bone_IdIsValid(node->anim_id) && node->model && (node->model->flags & OBJ_STATICFX) )
            {    
                bones[node->anim_id] = node;
                hits++;
            }
            if(node->child)
            {
                hits += getBonesThatNeedStaticLight(node->child, bones, seqHandle);
            }
        }
    }
    return hits;
}

#ifdef CLIENT


int seqSetStaticLight(SeqInst * seq, DefTracker * lighttracker)
{
    GfxNode *bones[BONEID_COUNT];
    int static_bone_count,i;
    Mat4 mat;
    int success;

    if(!seq || !seq->gfx_root || !seq->gfx_root->child)
        return 1;

    ZeroArray(bones);

    static_bone_count = getBonesThatNeedStaticLight(seq->gfx_root->child, bones, seq->handle);

    if(static_bone_count)
    {
        if(!lighttracker)
        {
            lighttracker = groupFindLightTracker(seq->gfx_root->mat[3]);
            if (isDevelopmentMode() && lighttracker && lighttracker->def && lighttracker->def->has_ambient && !lighttracker->light_grid) // For Trick reloading
                lightTracker(lighttracker);
        }

        if (lighttracker && lighttracker->light_grid)
        {
            for(i = 0; i < ARRAY_SIZE(bones); i++)
            {
                if(bones[i])
                {
                    U8 *buf;
                    int buffer_size;
                    assert(bones[i]->anim_id == i && !bones[i]->rgbs && bone_IdIsValid(i) && bones[i]->model && (bones[i]->model->flags & OBJ_STATICFX));
                    gfxTreeFindWorldSpaceMat(mat, bones[i]);
                    buffer_size = bones[i]->model->vert_count * 4;
                    if(rdr_caps.use_vbos)
                        buf = _alloca(buffer_size);
                    else
                        buf = malloc(buffer_size);
                    lightModel( bones[i]->model, mat, buf, lighttracker, 1.f );
                    seq->rgbs_array[i] = modelConvertRgbBuffer(buf,bones[i]->model->vert_count);
                    bones[i]->rgbs = seq->rgbs_array[i];
                }
            }
            success = 1;
        }
        else
        {
            success = 0;
        }
    }
    else
    {
        success = 1;
    }
    return success;
}

//#############################################################################################
//#############################################################################################
//#############################################################################################
//#############################################################################################
//######## Where should this live? ###################################

#include <utilitieslib/utils/tga.h>
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "graphics/gfxwindow.h"
#include "entity/entrecv.h"
#include "render/pbuffer.h"
#include "render/renderprim.h"
#include "render/renderUtil.h"

extern int glob_have_camera_pos;
extern void engine_update();

bool g_doingHeadShot=false;
bool doingHeadShot(void)
{
    return g_doingHeadShot;
}

PBuffer pbufHeadShot;
static PBuffer debug_pbufOld;

void initHeadShotPbuffer(int xres, int yres, int desiredMultisample, int requiredMultisample)
{
    // This pbuffer is now shared by the texWords system
    static int numCalls=0;
    static int numRecreates=0;
    numCalls++;
    debug_pbufOld = pbufHeadShot; // struct copy for debugging backup
    if (pbufHeadShot.software_multisample_level) {
        xres *= pbufHeadShot.software_multisample_level;
        yres *= pbufHeadShot.software_multisample_level;
    }
    if (xres > pbufHeadShot.virtual_width || yres > pbufHeadShot.virtual_height || game_state.imageServerDebug ||
        desiredMultisample != pbufHeadShot.desired_multisample_level || requiredMultisample != pbufHeadShot.required_multisample_level)
    {
        PBufferFlags flags = PB_COLOR_TEXTURE|PB_ALPHA|PB_RENDER_TEXTURE;
        if(rdr_caps.supports_fbos)
            flags |= PB_FBO;
        numRecreates++;

        pbufInit(&pbufHeadShot, xres, yres, flags, desiredMultisample,requiredMultisample,0,24,0,0);
    } else {
        pbufHeadShot.width = xres;
        pbufHeadShot.height = yres;
    }
    pbufMakeCurrent(&pbufHeadShot);
    rdrInit();
    rdrSetFog(2000000,2000000,0);
}

#define FRAME_TIMESTEP 0.5
#define FRAME_TIMESTEP_INIT 150
#define FRAMES_BEFORE_CAPTURE (hasFx?20:2)

void debugGetUpperLeftScreenPixels()
{
    int pixbuf[16*16];
    int i, j;

    rdrGetFrameBuffer(0,0,16,16, GETFRAMEBUF_RGBA, pixbuf);
    for (j=0; j<16; j++) {
        for (i=0; i<16; i++) {
            printf("%X ", pixbuf[i + 16*j]);
        }
        printf("\n");
    }
}

// Should only be used by imageserver_SaveShot()
// Most in-game code should now call takePicture()
U8 * getThePixelBuffer( SeqInst * seq, Vec3 headshotCameraPos, F32 headShotFovMagic, int sizeOfPictureX,
                       int sizeOfPictureY, BoneId centerBone, int shouldIFlip, int bodyshot,
                       AtlasTex *bgimage, int bgcolor, int isImageServer, char *caption)
{
    //////////////////Start the subfunction
    //Generic inputs
    Mat4 temp;
    U8 * pixbuf;
    int i;
    int desired_aa=bodyshot?8:4;
    int required_aa=bodyshot?2:2;
    F32 preservedLegScale;
    CameraInfo oldCamInfo;
    int        disable2d;
    int        game_mode;
    int        fov_custom;
    Mat4    headMat;
    int        hidePlayer;
    int heldBackgroundLoaderState;
    int hasFx;
    int saveClipper;
    //Figure out the middle of the screen //TO DO: may need to force set it to minimum size
    //and get those pixels
    int ULx, ULy;
    int screenX, screenY;
    F32 timestep_save, timestep_noscale_save;
    int old_disableGeometryBackgroundLoader;

    PERFINFO_AUTO_START("getThePixelBuffer", 1);

    old_disableGeometryBackgroundLoader = game_state.disableGeometryBackgroundLoader;
    game_state.disableGeometryBackgroundLoader = 1;

    g_doingHeadShot = true;

    //Preserve the entpos if this is a guy that won't be destroyed immediately
    copyMat4( seq->gfx_root->mat, temp );
    copyMat4( unitmat, seq->gfx_root->mat );

    if (rdr_caps.use_pbuffers) {
        initHeadShotPbuffer(sizeOfPictureX, sizeOfPictureY, desired_aa, required_aa);
        sizeOfPictureX *= pbufHeadShot.software_multisample_level;
        sizeOfPictureY *= pbufHeadShot.software_multisample_level; 
        // Move back camera so that the same scene is rendered twice as big
        //  we should really do this via fov or something less tweaky... as it changes the aspect of things...
        headshotCameraPos[2] = headshotCameraPos[2] / (pow(pbufHeadShot.software_multisample_level,0.95));
    }
    windowSize( &screenX, &screenY );
    ULx = ((F32)screenX)/2.0 - (F32)sizeOfPictureX / 2.0; //TO DO UL really LL?
    ULy = ((F32)screenY)/2.0 - (F32)sizeOfPictureY / 2.0;

    heldBackgroundLoaderState = game_state.disableGeometryBackgroundLoader;
    game_state.disableGeometryBackgroundLoader = 1;  //tells the background loader to finish

    seq->seqGfxData.alpha = 255; 
    preservedLegScale = seq->legScale; 
    seq->legScale = 0.0;

    //Make sure we're in the right LOD and that all textures and geometry are loaded
    seqProcessClientInst( seq, 1.0, 0, 1 ) ; //Ready move
    seqSetLod( seq, 1.0 );  //?? nEEDED?
    seq->seqGfxData.alpha = 255;
    animSetHeader( seq, 0 );  
    animPlayInst( seq );   

    //Preserve the old params
    disable2d = game_state.disable2Dgraphics;
    game_state.disable2Dgraphics = 1;
    saveClipper = do_scissor;
    if (saveClipper) set_scissor(0);
    //init_display_list();
    fov_custom = game_state.fov_custom; // I do fovMagic below, so don't need to set this
    game_mode = game_state.game_mode;
    oldCamInfo = cam_info; //copy off the old camera
    timestep_save = TIMESTEP;
    timestep_noscale_save = TIMESTEP_NOSCALE;
    sunPush();

    //Set us to template mode to not draw world or baddies
    toggle_3d_game_modes(SHOW_TEMPLATE);
    seq->gfx_root->flags &= ~GFXNODE_HIDE;

    game_state.fov_custom = headShotFovMagic;
//    gfxWindowReshapeForHeadShot( headShotFovMagic );  //Undone by gfxWindowReshape

//    lightEnt( &seq->seqGfxData.light, seq->gfx_root->mat[3] , seq->type->minimumambient, 0.2 );
//
//    seqProcessClientInst( seq, 1.0, 0, 1 ) ; //Ready move
//    seqSetLod( seq, 1.0 );  //?? nEEDED?
//    seq->seqGfxData.alpha = 255;
//    animSetHeader( seq, 0 );
//    animPlayInst( seq );

    //Figure out where this guys head is and put it in the middle of the screen
    if( centerBone != BONEID_INVALID )
    {
        GfxNode *headNode = gfxTreeFindBoneInAnimation( centerBone, seq->gfx_root->child, seq->handle, 1 );
        gfxTreeFindWorldSpaceMat(headMat, headNode);
        if (!headNode || seq->type->fullBodyPortrait) {
            Vec2 ul, ur;
            Vec3 worldMin, worldMax;
            Vec3 diag;
            F32 mag;

            // Sorta hacky code to fix issues with guys who don't have heads,
            //   just do a full body shot.
            // Good models to test changes on:
            //  spawnvillain Arachnos_Dual_Turret
            //  spawnvillain Arachnos_Tarantula_Queen
            //  spawnvillain Arachnos_Summoned_Spiderling
            //    spawnvillain Arachnos_Arachnos_Flyer

            copyMat4(unitmat, cam_info.cammat);
            addVec3( cam_info.cammat[3], headshotCameraPos, cam_info.cammat[3] );
            gfxSetViewMat(cam_info.cammat, cam_info.viewmat, NULL);

            seqGetSides(seq, seq->gfx_root->mat, ul, ur, worldMin, worldMax);
            addVec3(worldMin, worldMax, headMat[3]);
            scaleVec3(headMat[3], 0.5, headMat[3]);
            subVec3(worldMax, worldMin, diag);
            mag = lengthVec3(diag);
            headMat[3][0]-=headshotCameraPos[0];
            headMat[3][1]+=headshotCameraPos[1];
            headMat[3][2]+=headshotCameraPos[2];
            headMat[3][1] = MAX(3, headMat[3][1]);

            scaleVec3(headshotCameraPos, MAX(mag / 2, 5.0), headshotCameraPos);
        }
    }
    else
    {
        copyMat4( seq->gfx_root->mat, headMat );
    }

    //Set up the camera
    copyMat3( unitmat, cam_info.mat );
    copyVec3( headMat[3],cam_info.mat[3] );

    copyMat3( unitmat, cam_info.cammat );
    copyVec3( headMat[3], cam_info.cammat[3] );

    addVec3( cam_info.cammat[3], headshotCameraPos, cam_info.cammat[3] );

    gfxSetViewMat(cam_info.cammat, cam_info.viewmat, cam_info.inv_viewmat);

    camLockCamera( true );

    lightEnt( &seq->seqGfxData.light, seq->gfx_root->mat[3] , seq->type->minimumambient, 0.2 );

    seqProcessClientInst( seq, 1.0, 0, 1 ) ; //Ready move
    seqSetLod( seq, 1.0 );  //?? nEEDED?
    seq->seqGfxData.alpha = 255;
    animSetHeader( seq, 0 );
    animPlayInst( seq );

    //hide everything but the sequence we're drawing.
    {
        GfxNode * node;

        for( node = gfx_tree_root ; node ; node = node->next )
        {
            if( node->child && node->child->seqHandle != seq->handle  )
                node->flags |= GFXNODE_HIDE;
        }
    }

    //Hide the player
    {
        Entity * player = playerPtr();
        hidePlayer = 0;
        if( player && player->seq && player->seq->gfx_root )
        {
            hidePlayer = (player->seq->gfx_root->flags & GFXNODE_HIDE) ? 1 : 0;
            player->seq->gfx_root->flags |= GFXNODE_HIDE;
        }
    }

    //move the character around a little for a more dramatic pose
    yawMat3( -0.2, seq->gfx_root->mat );

    sunGlareDisable();

    if (bgimage)
    {
        // Background image
        F32 xscale = (sizeOfPictureX+2) / (F32)bgimage->width;
        F32 yscale = (sizeOfPictureY+2) / (F32)bgimage->height;

        init_display_list(); // clears the sprite list
        display_sprite(bgimage, ULx-1, ULy-1, 0, xscale, yscale, (bgcolor << 8) | 0xff);
    }

    if (caption && caption[0])
    {
        float sc = 1;
        int text_width = 0;
        font_set(&game_12, 0, 0, 1, 1, 0xffffffff, 0xffffffff);
        text_width = str_wd_notranslate(&game_12, sc, sc, caption);
        if (text_width > sizeOfPictureX)
        {
            sc = str_sc(&game_12, sizeOfPictureX - 2, caption);
            text_width = str_wd_notranslate(&game_12, sc, sc, caption);
        }
        cprntEx((sizeOfPictureX - text_width) / 2, sizeOfPictureY - 5, 100, sc, sc, NO_MSPRINT, caption);
    }

    if (game_state.imageServerDebug) {
        extern int glob_force_buffer_flip;
        if (rdr_caps.use_pbuffers)
            glob_force_buffer_flip = 0;
        else
            glob_force_buffer_flip = 1;
    }

    //Yuck
//        rdrClearScreen();
    gfxUpdateFrame( 0, 1, 0 ); //need this one to set up loader
    forceGeoLoaderToComplete();
    texForceTexLoaderToComplete(0);

    //Sleep(5); //still getting problems, don't know why...
    // Jump ahead a bunch in case the FX has a longer startup time
    TIMESTEP_NOSCALE = TIMESTEP = FRAME_TIMESTEP_INIT;
//        rdrClearScreen();
    //gfxUpdateFrame( 0, 1, 0 );
    fxRunEngine();
    partRunEngine();
    // Check to see if we have any FX on us
    hasFx = eaiSize(&seq->seqfx);
    for( i = 0 ; i < MAX_SEQFX ; i++ )
    {
        if( seq->const_seqfx[i] )
            hasFx = 1;
    }
    for( i = 0 ; i < eaiSize(&seq->seqcostumefx) ; i++ )
    {
        if( seq->seqcostumefx[i] )
            hasFx = 1;
    }
    // Run the particle systems for a bit
    for (i=0; i<FRAMES_BEFORE_CAPTURE; i++) {
        //Fill the buffer
        TIMESTEP_NOSCALE = TIMESTEP = FRAME_TIMESTEP;
        //gfxUpdateFrame( 0, 1, 1 );
        fxRunEngine();
        partRunEngine();
    }

//        rdrClearScreen();
    gfxUpdateFrame( 0, 1, 0 );

    //Restore the old gfx settings
    //Un Hide the player
    if( !hidePlayer )
    {
        Entity * player = playerPtr();
        if( player && player->seq && player->seq->gfx_root )
            player->seq->gfx_root->flags &= ~GFXNODE_HIDE;
    }
    toggle_3d_game_modes(game_mode);
    game_state.game_mode = game_mode;
    game_state.disable2Dgraphics = disable2d;

    if (saveClipper) set_scissor(1);
    cam_info = oldCamInfo; //also restored camera Func
    game_state.fov_custom = fov_custom;
    init_display_list();
    TIMESTEP = timestep_save;
    TIMESTEP_NOSCALE = timestep_noscale_save;

    seq->legScale = preservedLegScale;

    game_state.disableGeometryBackgroundLoader = heldBackgroundLoaderState;

    //Read the pixels
    if (rdr_caps.use_pbuffers) {
        pixbuf = pbufFrameGrab( &pbufHeadShot, NULL, 0 );
        sizeOfPictureX /= pbufHeadShot.software_multisample_level;
        sizeOfPictureY /= pbufHeadShot.software_multisample_level;
    } else {
        pixbuf = malloc( sizeOfPictureX * sizeOfPictureY * 4 );
        rdrGetFrameBuffer(ULx, ULy, sizeOfPictureX, sizeOfPictureY, GETFRAMEBUF_RGBA, pixbuf);
    }
    
    //invert picture, I don't know why, am I stupid or is GL?  JE: We're all stupid.
    if( shouldIFlip )
    {
        U8 * invertedPixBuf;

        invertedPixBuf = calloc(sizeOfPictureX * sizeOfPictureY * 4, 1);

        for( i = 0 ; i < sizeOfPictureY ; i++ )
        {
            memcpy(
                &invertedPixBuf[ (sizeOfPictureY - (i+1)) * (sizeOfPictureX * 4) ],
                &pixbuf[ i * (sizeOfPictureX * 4) ],
                sizeOfPictureX * 4 );//*/
        }
        free( pixbuf );
        pixbuf = invertedPixBuf;
    }

    // Remove alpha
    {
        int max = sizeOfPictureY*sizeOfPictureX*4;
        for( i = 3 ; i < max ; i+=4 )
        {
            pixbuf[ i ] |= 0xff;
        }
    }

    copyMat4( temp, seq->gfx_root->mat );
    gfxWindowReshape(); //Restore

    if (rdr_caps.use_pbuffers) {
        winMakeCurrent();
    }

    sunPop();

    g_doingHeadShot = false;
    rdrNeedGammaReset(); // Because people were reporting gamma going screwy after headshots taken.
    
    game_state.disableGeometryBackgroundLoader = old_disableGeometryBackgroundLoader;
    PERFINFO_AUTO_STOP();

    return pixbuf;
}

static bool takePicture_viewport_precallback1(ViewportInfo *pViewportInfo)
{
    TakePictureInfo    *takepicture_info = (TakePictureInfo*) pViewportInfo->callbackData;

    SeqInst * seq = takepicture_info->seq;

    g_doingHeadShot = true;

    //Preserve the entpos if this is a guy that won't be destroyed immediately
    copyMat4( seq->gfx_root->mat, takepicture_info->savedMat );
    copyMat4( unitmat, seq->gfx_root->mat );

    seq->gfx_root->mat[3][1] += takepicture_info->entity_y_offset;

    takepicture_info->heldBackgroundLoaderState = game_state.disableGeometryBackgroundLoader;
    game_state.disableGeometryBackgroundLoader = 1;  //tells the background loader to finish

    seq->seqGfxData.alpha = 255; 
    takepicture_info->preservedLegScale = seq->legScale; 
    seq->legScale = 0.0;

    //Make sure we're in the right LOD and that all textures and geometry are loaded
    seqProcessClientInst( seq, 1.0, 0, 1 ) ; //Ready move
    seqSetLod( seq, 1.0 );  //?? nEEDED?
    seq->seqGfxData.alpha = 255;
    animSetHeader( seq, 0 );  
    animPlayInst( seq );   

    //Preserve the old params
    takepicture_info->saveClipper = do_scissor;
    if (takepicture_info->saveClipper) set_scissor(0);
    takepicture_info->game_mode = game_state.game_mode;
    takepicture_info->timestep_save = TIMESTEP;
    takepicture_info->timestep_noscale_save = TIMESTEP_NOSCALE;
    takepicture_info->disable2d = game_state.disable2Dgraphics;
    game_state.disable2Dgraphics = 1;

    sunPush();

    //Set us to template mode to not draw world or baddies
    toggle_3d_game_modes(SHOW_TEMPLATE);
    seq->gfx_root->flags &= ~GFXNODE_HIDE;

    lightEnt( &seq->seqGfxData.light, seq->gfx_root->mat[3] , seq->type->minimumambient, 0.2 );

    seqProcessClientInst( seq, 1.0, 0, 1 ) ; //Ready move
    seqSetLod( seq, 1.0 );  //?? nEEDED?
    seq->seqGfxData.alpha = 255;
    if (takepicture_info->newImage)
    {
        animSetHeader( seq, 0 );
        seqProcessClientInst( seq, 30.0, 1, 1 ) ; //Ready move

        //the following is necessary so that destructible objects like the Rikti bomb aren't in their startup position.
        seq->curr_interp_frame = 30;
        seqSetMove(seq, seq->info->moves[seq->animation.move->raw.nextMove[0]], 1);

    }
    animPlayInst( seq );

    // Hide other stuff
    {
        GfxNode * node;

        for( node = gfx_tree_root ; node ; node = node->next )
        {
            if( node->child && node->child->seqHandle != seq->handle  )
                node->flags |= GFXNODE_HIDE;
        }
    }

    //Hide the player
    {
        Entity * player = playerPtr();
        takepicture_info->hidePlayer = 0;
        if( player && player->seq && player->seq->gfx_root )
        {
            takepicture_info->hidePlayer = (player->seq->gfx_root->flags & GFXNODE_HIDE) ? 1 : 0;
            player->seq->gfx_root->flags |= GFXNODE_HIDE;
        }
    }

    //move the character around a little for a more dramatic pose
    yawMat3( -0.2, seq->gfx_root->mat );

    sunGlareDisable();

    return true;
}


static bool takePicture_viewport_precallback2(ViewportInfo *pViewportInfo)
{
    TakePictureInfo    *takepicture_info = (TakePictureInfo*) pViewportInfo->callbackData;
    SeqInst * seq = takepicture_info->seq;
    int i;
    int hasFx;

    forceGeoLoaderToComplete();
    texForceTexLoaderToComplete(0);
    
    // Jump ahead a bunch in case the FX has a longer startup time
    TIMESTEP_NOSCALE = TIMESTEP = FRAME_TIMESTEP_INIT;
    fxRunEngine();
    partRunEngine();

    // Check to see if we have any FX on us
    hasFx = eaiSize(&seq->seqfx);
    for( i = 0 ; i < MAX_SEQFX ; i++ )
    {
        if( seq->const_seqfx[i] )
            hasFx = 1;
    }
    for( i = 0 ; i < eaiSize(&seq->seqcostumefx) ; i++ )
    {
        if( seq->seqcostumefx[i] )
            hasFx = 1;
    }
    // Run the particle systems for a bit
    for (i=0; i<FRAMES_BEFORE_CAPTURE; i++) {
        //Fill the buffer
        TIMESTEP_NOSCALE = TIMESTEP = FRAME_TIMESTEP;
        fxRunEngine();
        partRunEngine();
    }

    return true;
}


static bool takePicture_viewport_postcallback2(ViewportInfo *pViewportInfo)
{
    TakePictureInfo    *takepicture_info = (TakePictureInfo*) pViewportInfo->callbackData;

    //Restore the old gfx settings
    //Un Hide the player
    if( !takepicture_info->hidePlayer )
    {
        Entity * player = playerPtr();
        if( player && player->seq && player->seq->gfx_root )
            player->seq->gfx_root->flags &= ~GFXNODE_HIDE;
    }
    toggle_3d_game_modes(takepicture_info->game_mode);
    game_state.game_mode = takepicture_info->game_mode;
    game_state.disable2Dgraphics = takepicture_info->disable2d;

    if (takepicture_info->saveClipper) set_scissor(1);
    //init_display_list();
    TIMESTEP = takepicture_info->timestep_save;
    TIMESTEP_NOSCALE = takepicture_info->timestep_noscale_save;
    sunPop();

    takepicture_info->seq->legScale = takepicture_info->preservedLegScale;

    game_state.disableGeometryBackgroundLoader = takepicture_info->heldBackgroundLoaderState;

    copyMat4( takepicture_info->savedMat, takepicture_info->seq->gfx_root->mat );

    g_doingHeadShot = false;
    rdrNeedGammaReset(); // Because people were reporting gamma going screwy after headshots taken.
    
    viewport_Remove(&takepicture_info->viewport_2);
    viewport_Remove(&takepicture_info->viewport_1);

    if (takepicture_info->freeEntity)
    {
        s_destroyTempEnt(takepicture_info->entity);
        entFree( takepicture_info->entity );
    }

    takepicture_info->inUse = false;

    return true;
}

// New version of getThePixelBuffer
TakePictureInfo * takePicture(SeqInst *seq, Mat4 cameraMatrix, F32 FovMagic,
                              int sizeOfPictureX, int sizeOfPictureY, BoneId centerBone,
                              AtlasTex *bgimage, int bgcolor, bool freeEntity, Entity *entity,
                              int desired_multisample_level, int required_multisample_level,
                              F32 entity_y_offset, bool newImage)
{
    TakePictureInfo    *takepicture_info;
   
    // Find a free takepicture_info
    {
        if (s_takepicture_info[s_next_takepicture_index].inUse)
        {
            assert("May need to bump up NUM_TAKEPICTURE_INFOS" == 0);
            return NULL;
        }

        takepicture_info = &s_takepicture_info[s_next_takepicture_index];

        s_next_takepicture_index = (s_next_takepicture_index + 1) % NUM_TAKEPICTURE_INFOS;
    }

    assert(!takepicture_info->inUse);

    if(!seq)    //if we don't have a sequence, we can't do anything.
        return NULL;

    takepicture_info->inUse = true;

    if (!takepicture_info->inited)
    {
        takepicture_info->inited = true;

        viewport_InitStructToDefaults(&takepicture_info->viewport_1);

        // do this so viewport_InitRenderToTexture() will properly get called
        takepicture_info->viewport_1.width = 0;
        takepicture_info->viewport_1.height = 0;

        takepicture_info->viewport_1.renderSky = false;
        takepicture_info->viewport_1.renderCharacters = true;
        takepicture_info->viewport_1.renderOpaquePass = true;
        takepicture_info->viewport_1.renderLines = false;
        takepicture_info->viewport_1.renderUnderwaterAlphaObjectsPass = false;
        takepicture_info->viewport_1.renderWater = false;
        takepicture_info->viewport_1.renderAlphaPass = true;
        takepicture_info->viewport_1.renderParticles = true;
        takepicture_info->viewport_1.render2D = false;

        takepicture_info->viewport_1.doHeadshot = true;

        takepicture_info->viewport_1.pPreCallback = takePicture_viewport_precallback1;

        takepicture_info->viewport_1.name = "takePicture 1";
        takepicture_info->viewport_1.renderPass = RENDERPASS_TAKEPICTURE;
    }

    takepicture_info->viewport_1.fov = FovMagic;
    takepicture_info->viewport_1.desired_multisample_level = desired_multisample_level;
    takepicture_info->viewport_1.required_multisample_level = required_multisample_level;
    takepicture_info->viewport_1.callbackData = takepicture_info;

    takepicture_info->viewport_1.width = sizeOfPictureX;
    takepicture_info->viewport_1.height = sizeOfPictureY;
    takepicture_info->viewport_1.backgroundimage = bgimage;
    viewport_InitRenderToTexture(&takepicture_info->viewport_1);

    takepicture_info->entity = entity;
    takepicture_info->seq = seq;
    takepicture_info->freeEntity = freeEntity;

    takepicture_info->entity_y_offset = entity_y_offset;
    takepicture_info->newImage = newImage;

    copyMat4( cameraMatrix, takepicture_info->viewport_1.cameraMatrix );

    memcpy(&takepicture_info->viewport_2, &takepicture_info->viewport_1, sizeof(takepicture_info->viewport_2));
    takepicture_info->viewport_2.pPreCallback = takePicture_viewport_precallback2;
    takepicture_info->viewport_2.pPostCallback = takePicture_viewport_postcallback2;
    takepicture_info->viewport_2.pSharedPBufferTexture = &takepicture_info->viewport_1.PBufferTexture;
    takepicture_info->viewport_2.name = "takePicture 2";

    viewport_Add(&takepicture_info->viewport_1);
    viewport_Add(&takepicture_info->viewport_2);

    return takepicture_info;
}

/*
AtlasTex * takePictureMMSeqImage(SeqInst * seq, Vec3 cameraPos, Vec3 lookat, float yOffset, float rot, F32 FOV,
                         unsigned int * desiredWidth, unsigned int *desiredHeight,
                         int * px1, int * px2, int * py1, int * py2, int newImage, AtlasTex *bgimage, int square,
                         F32 entity_y_offset,
                         int desired_multisample_level, int required_multisample_level)
{
    return NULL;
}
*/

AtlasTex * takePicturHeadBodyShot(Entity *entity, int bodyshot, int villain, int freeEntity)
{
    Vec3 cameraPos;
    F32 FovMagic;
    int sizeOfPictureX;
    int sizeOfPictureY;
    int desired_multisample_level;
    int required_multisample_level;
    BoneId centerBone;
    AtlasTex *bgimage;
    BasicTexture *basic_texture = NULL;
    Mat4    headMat;
    Mat4    cameraMatrix;
    SeqInst * seq;

    TakePictureInfo    *takepicture_info;
   
    if(!entity || !entity->seq)    //if we don't have a sequence, we can't do anything.
        return NULL;

    seq = entity->seq;

    if (bodyshot)
    {
        sizeOfPictureX = 128; //Size of shot on screen.
        sizeOfPictureY = 256; //Powers of two!!! ARgh.
        FovMagic = 15;   //This is a weird magic thing
        centerBone = BONEID_HIPS;
        cameraPos[0] = -0.05;
        cameraPos[1] = -0.7;
        cameraPos[2] = 22.0;
        bgimage = atlasLoadTexture("Portrait_bkgd_contact.tga");
    }
    else
    {
        sizeOfPictureX = 128; //Size of shot on screen.
        sizeOfPictureY = 128; //Powers of two!!! ARgh.
        FovMagic = 28;   //This is a weird magic thing
        centerBone = BONEID_HEAD;
        cameraPos[0] = -0.05;
        cameraPos[1] = 0.0;
        cameraPos[2] = 15.0;
        bgimage = atlasLoadTexture(villain?"v_Portrait_bkgd_red.tga":"Portrait_bkgd_contact.tga");
    }


    desired_multisample_level = bodyshot?8:4;
    required_multisample_level = bodyshot?2:2;

    //Figure out where this guys head is and put it in the middle of the screen
    if( centerBone != BONEID_INVALID )
    {
        GfxNode * headNode = gfxTreeFindBoneInAnimation( centerBone, seq->gfx_root->child, seq->handle, 1 );

        seqProcessClientInst( seq, 1.0, 0, 1 ) ; //Ready move
        animPlayInst( seq );
        gfxTreeFindWorldSpaceMat(headMat, headNode);

        if (!headNode || seq->type->fullBodyPortrait) {
            Vec2 ul, ur;
            Vec3 worldMin, worldMax;
            Vec3 diag;
            F32 mag;

            // Sorta hacky code to fix issues with guys who don't have heads,
            //   just do a full body shot.
            // Good models to test changes on:
            //  spawnvillain Arachnos_Dual_Turret
            //  spawnvillain Arachnos_Tarantula_Queen
            //  spawnvillain Arachnos_Summoned_Spiderling
            //    spawnvillain Arachnos_Arachnos_Flyer

            copyMat4(unitmat, cameraMatrix);
            addVec3( cameraMatrix[3], cameraPos, cameraMatrix[3] );

            seqGetSides(seq, unitmat, ul, ur, worldMin, worldMax);
            addVec3(worldMin, worldMax, headMat[3]);
            scaleVec3(headMat[3], 0.5, headMat[3]);
            subVec3(worldMax, worldMin, diag);
            mag = lengthVec3(diag);
            headMat[3][0] -= cameraPos[0];
            headMat[3][1] += cameraPos[1];
            headMat[3][2] += cameraPos[2];
            headMat[3][1]  = MAX(3, headMat[3][1]);

            scaleVec3(cameraPos, MAX(mag / 2, 5.0), cameraPos);
        }
    }
    else
    {
        copyMat4( unitmat, headMat );
    }

    copyMat3( unitmat, cameraMatrix );
    copyVec3( headMat[3], cameraMatrix[3] );
    addVec3( cameraMatrix[3], cameraPos, cameraMatrix[3] );

    takepicture_info = takePicture(entity->seq, cameraMatrix, FovMagic, sizeOfPictureX, sizeOfPictureY, centerBone,
                                   bgimage, 0xffffff, freeEntity, entity, desired_multisample_level, required_multisample_level,
                                   0, true);

    basic_texture = texGenNew(sizeOfPictureX, sizeOfPictureY, "take picture texture");

    pbufSetupCopyTexture(&takepicture_info->viewport_1.PBufferTexture, basic_texture->id);

    return atlasGenTextureFromBasic(basic_texture, false, true);
}

#define BODYSHOT_TEX_STARTING_RESOLUTION_X 128
#define BODYSHOT_TEX_STARTING_RESOLUTION_Y 256
AtlasTex * takeMMPicture( AtlasTex **tex, SeqInst * seq, int * x1, int * x2, 
                         int * y1, int * y2, Vec3 headshotCameraPos, Vec3 lookatPos, 
                         int newImage, BoneId centerBone, int autoResize)
{
    U8 * pixbuf = 0;
    //Settable params
//    int screenWidth, screenHeight;
    int sizeOfPictureX = BODYSHOT_TEX_STARTING_RESOLUTION_Y; //Size of shot on screen.
    int sizeOfPictureY = BODYSHOT_TEX_STARTING_RESOLUTION_Y; //Powers of two!!! ARgh.
    F32 headShotFovMagic = 15;//, height;   //This is a weird magic thing
    F32 scale = 1.5;
    F32 rotate = 0;
    int shouldIFlip = newImage;
    //BoneId centerBone = BONEID_HIPS;

    if(!seq)    //if we don't have a sequence, we can't do anything.
        return NULL;


    //setup lookat position
    if(newImage) 
    {
        int headNum;
        F32 distance;
        GfxNode * headNode;
        Mat4 lookAtMatrix;
        Mat4 collMat;
        Vec3 headPosition;
        EntityCapsule cap;
        F32 height, width;
        F32 rotateRadians = 0;

        headshotCameraPos[0] = 0.0;
        headshotCameraPos[1] = 0.0;
        headshotCameraPos[2] = 0.0;

        lookatPos[0] = 0;//(worldMax[0]-worldMin[0])/2.0;
        lookatPos[1] = 0;//seq->type->capsuleSize[1] + seq->type->reticleHeightBias;//(worldMax[1]-worldMin[1])/2.0;
        lookatPos[2] = 0;//(worldMax[2]-worldMin[2])/2.0;
        gfxTreeFindWorldSpaceMat(lookAtMatrix, seq->gfx_root);

        if( seq->type->selection == SEQ_SELECTION_LIBRARYPIECE )
        {
            autoResize = 1;        //always resize on library piece
            getCapsule(seq, &cap ); 
            positionCapsule( seq->gfx_root->mat, &cap, collMat );

            if(cap.dir == 0)
                lookatPos[1] =seq->curranimscale*((cap.radius*2+ seq->type->reticleWidthBias)/2.0 + cap.offset[0]);
            else if(cap.dir == 1)
                lookatPos[1] =seq->curranimscale*((cap.length+ cap.radius*2+ seq->type->reticleHeightBias)/2.0 + cap.offset[1]);
            else if(cap.dir == 2)
            {
                lookatPos[1] =seq->curranimscale*((cap.radius*2+ seq->type->reticleBaseBias)/2.0 + cap.offset[2]);
                
            }
            else
                lookatPos[1] =seq->curranimscale*((cap.radius*2+ seq->type->reticleBaseBias)/2.0 + cap.offset[2]);

            if(cap.dir == 1)
            {
                height = seq->curranimscale*((cap.length+ cap.radius*2+ seq->type->reticleHeightBias));
                width = seq->curranimscale*((cap.radius*2+ seq->type->reticleWidthBias)/2.0 + cap.offset[0]);
            }
            else
            {
                height = seq->curranimscale*((cap.radius*2));
                width = seq->curranimscale*((cap.length+ cap.radius*2+ seq->type->reticleHeightBias));
            }
            height *= scale;
            width *= scale;

            headshotCameraPos[1] += height*.5;
        }
        else if( seq->type->selection == SEQ_SELECTION_BONES )
        {
            //HIPS
            /*headNum = centerBone;  
            headNode = gfxTreeFindBoneInAnimation( headNum, seq->gfx_root->child, seq->handle, 1 );
            gfxTreeFindWorldSpaceMat(lookAtMatrix, headNode);
            copyVec3(lookAtMatrix[3], lookatPos);*/

            //HEAD
            headNum = BONEID_HEAD;  
            headNode = gfxTreeFindBoneInAnimation( headNum, seq->gfx_root->child, seq->handle, 1 );
            if(!headNode)
                autoResize = 1;        //always resize if there's no head
            gfxTreeFindWorldSpaceMat(lookAtMatrix, headNode);
            copyVec3(lookAtMatrix[3], headPosition);

            height = seq->curranimscale*(seq->type->capsuleSize[1]+ seq->type->reticleHeightBias);
            width = seq->curranimscale*(seq->type->capsuleSize[0]+ seq->type->reticleWidthBias);

            width *= headPosition[1]*scale/height;
            height = headPosition[1]*scale;//seq->curranimscale*seq->type->reticleHeightBias;//headPosition[1]+ seq->type->reticleHeightBias;
        }
        else
        {
            autoResize = 1;        //always resize if this is a capsule only entity
            getCapsule(seq, &cap ); 
            positionCapsule( seq->gfx_root->mat, &cap, collMat );

            if(cap.dir == 1)
                lookatPos[1] =seq->curranimscale*((cap.length+ cap.radius*2+ seq->type->reticleHeightBias)/2.0 + cap.offset[1] );
            else if(cap.dir == 2)
                lookatPos[1] =seq->curranimscale*((cap.radius*2)/2);

            if(cap.dir == 1)
            {
                height = seq->curranimscale*((cap.length+ cap.radius*2+ seq->type->reticleHeightBias));
                width = seq->curranimscale*((cap.radius*2+ seq->type->reticleWidthBias));
            }
            else
            {
                height = seq->curranimscale*((cap.radius*2));
                width = seq->curranimscale*((cap.length+ cap.radius*2+ seq->type->reticleHeightBias));
            }

            //these tend to be a little more flush--move them out by adding height
            height *= scale;
            width *= scale;
        }

        //Match so that the full height of the image fits into the image.
        //if the width is larger than the height, change the height so that both fit.
        if(width > height)
            height = width;

        if(autoResize)
        {
            distance = 10.0/3.0;
            lookatPos[2] = distance*height;
        }
        else
        {
            rotate = .2;
            headshotCameraPos[1] = -.2;
            copyVec3(headPosition, lookatPos);
            lookatPos[2] = height;
            headShotFovMagic = 22;
        }
    }


    getMMSeqImage(&pixbuf, seq, headshotCameraPos, lookatPos, 0, rotate, (F32) headShotFovMagic,
        &sizeOfPictureX, &sizeOfPictureY, x1, x2, y1, y2, newImage, NULL,!autoResize);


    if(newImage && autoResize)
    {

        if(sizeOfPictureX > BODYSHOT_TEX_STARTING_RESOLUTION_Y || 
            sizeOfPictureY > BODYSHOT_TEX_STARTING_RESOLUTION_Y)
        {
            //if this is ridiculously large, it's because the model information is unhelpful.
            //we need to try again
            if(sizeOfPictureX/(F32)BODYSHOT_TEX_STARTING_RESOLUTION_Y < 1.5 &&
                sizeOfPictureY/(F32)BODYSHOT_TEX_STARTING_RESOLUTION_Y< 1.5)
                lookatPos[2] *= 1.5;
            else
                lookatPos[2] *= 2.5;
            sizeOfPictureX = sizeOfPictureY = BODYSHOT_TEX_STARTING_RESOLUTION_Y;
            pixbuf = getMMSeqImage(&pixbuf,seq, headshotCameraPos, lookatPos, 0, 0, (F32) headShotFovMagic,
                &sizeOfPictureX, &sizeOfPictureY, x1, x2, y1, y2, newImage, NULL,0);
        }
    }



    //Bind this into a Texture

    if(*tex)
    {
        //texGenUpdate((*tex)->data->basic_texture, pixbuf);
        atlasUpdateTexture(*tex, pixbuf, sizeOfPictureX, sizeOfPictureY, PIX_RGBA|PIX_DONTATLAS|PIX_NO_VFLIP);
    }
    else
        *tex = atlasGenTexture(pixbuf, sizeOfPictureX, sizeOfPictureY, PIX_RGBA|PIX_DONTATLAS|PIX_NO_VFLIP, "mmShot");

    estrDestroy(&pixbuf);
    return *tex;
}



//////////////////End the subfunction

////////////// If we need any more of these, I need to generalize it some more ////////////////////////////

//#########################################################################################
//#### Body Texture #################################################################################


static int s_isVillainous()
{
    return (SAFE_MEMBER2(playerPtr(),pl,playerType)==kPlayerType_Villain);
}



static int MMShotCount;
static StashTable MMShotHT;
//We're still using headshots for the time being.
//uniqueId : if you give me a uniqueId, I hash that, otherwise I hash the costume ptr
AtlasTex * seqGetMMShotStore( const cCostume * costume, int stillImage, Seqgraphics_PictureType pictureType,
    int *MMShotCount, StashTable *MMShotHT, int uniqueId)
{
    //setup for image capture
    MMPinPData data;
    AtlasTex *bind = 0;
    static Entity *e = 0;    //this is a hack to get multiple cameras working at the same time
    int screenX, screenY;
    int newImage = 0;
    static const cCostume *c = 0;    //this is a hack to get multiple cameras working at the same time
    BoneId centerbone;
    int autoResize;
    if(!MMShotCount || !MMShotHT)
        return NULL;

    
    if(pictureType == PICTURETYPE_BODYSHOT)
    {
        centerbone = BONEID_HIPS;
        autoResize = 1;
    }
    else
    {
        centerbone = BONEID_HEAD;
        autoResize = 0;
    }

    

    PERFINFO_AUTO_START("seqGetMMShot", 1);



    windowSize( &screenX, &screenY );
    //if we can't get a texture...
    if( !costume || ( screenX <= BODYSHOT_TEX_STARTING_RESOLUTION_X || screenY <= BODYSHOT_TEX_STARTING_RESOLUTION_Y ))
    {
        return white_tex_atlas;
    }
    else if(!(*MMShotHT))
    {
        (*MMShotHT) = stashTableCreateAddress(1000);
    }

    //if we've already recorded the image, return it.
    //if(stillImage)
    //{
        if(stashAddressFindPointer( (*MMShotHT), uniqueId ? S32_TO_PTR(uniqueId) : costume, &bind))
            return bind;
    //}

    memset(&data, 0, sizeof(MMPinPData));

    //Otherwise use the costume to generate a sequencer, take a picture, and free it
    //setup entity
    {
        //int npcIndex;
        //NPCDef* npcDef = NULL;  // What kind of NPC are we trying to create?
        if(costume != c || stillImage || !e || !e->seq)
        {
            newImage = 1;
            if(data.e)
            {
                s_destroyTempEnt(data.e);
                entFree( data.e );
            }
            if(e && e->costume)
            {
                s_destroyTempEnt(e);
                entFree(e);
            }

            e = entCreate("");
            c = costume;
            //e = data->e = entCreate("");
            //c = data->c = costume;

            //data->e->costume = costume;
            e->costume = costume;
            

            costume_Apply( e );
            entUpdateMat4Interpolated(e, unitmat);
            copyMat4( ENTMAT(e), e->seq->gfx_root->mat );
            animSetHeader( e->seq, 0 );

            manageEntityHitPointTriggeredfx(e);
            /*costume_Apply( data->e );
            entUpdateMat4Interpolated(data->e, unitmat);
            copyMat4( ENTMAT(data->e), data->e->seq->gfx_root->mat );
            animSetHeader( data->e->seq, 0 );

            manageEntityHitPointTriggeredfx(data->e);*/

        }
    }

    if(    pictureType == PICTURETYPE_HEADSHOT && 
        e->seq->type->selection == SEQ_SELECTION_BONES &&
        gfxTreeFindBoneInAnimation(BONEID_HEAD, e->seq->gfx_root->child, e->seq->handle, 1) )
    {
        //shouldn't need to worry about library piece stuff since this is only for bones.
        bind = takePicturHeadBodyShot( e, 0, s_isVillainous(), stillImage );

        if (stillImage)
        {
            // do this to prevent the code below from freeing the entity
            stillImage = false;
            e = 0;
        }
    }
    else
        bind = takeMMPicture(&bind, e->seq, &data.x1, &data.x2, &data.y1, &data.y2, data.headshotCameraPos, data.lookatPos, newImage, centerbone, autoResize);

    //if(!stillImage)
    //    MMatlas = bind;


    //Add to hash table

    
    

    if(bind)
    {
        (*MMShotCount)++; //debug
        stashAddressAddPointer( (*MMShotHT), uniqueId ? S32_TO_PTR(uniqueId) : costume, bind, true);
        if(stillImage)
        {
            s_destroyTempEnt(e);
            entFree( e );
            e = 0;
        //entFree( data->e );
        //data->e = 0;
        }
        PERFINFO_AUTO_STOP();

        return bind;
    }

    PERFINFO_AUTO_STOP();

    return white_tex_atlas;

    
}

AtlasTex * seqGetMMShot( const cCostume * costume, int stillImage, Seqgraphics_PictureType type)
{
    return seqGetMMShotStore( costume, stillImage, type,
        &MMShotCount, &MMShotHT, 0);
}

void seqClearMMCache()
{
    stashTableClear(MMShotHT);
}

//THis function is a cut and paste of the headshot with a word changed.  One more kind of shot, and I'll make it all more generic
int bodyShotCount;
StashTable bodyShotHT;

//uniqueId : if you give me a uniqueId, I hash that, otherwise I hash the costume ptr
AtlasTex * seqGetBodyshot( const cCostume * costume, int uniqueId )
{
    Entity * e;
    AtlasTex * bind;

    PERFINFO_AUTO_START("seqGetBodyshot", 1);
    
    bind = 0;
    if( !costume )
        bind =  white_tex_atlas;

    //If you've already taken this guys picture, return it.
    //TO DO make this a hash table with proper freeing when it gets to big
    /*Look in the hash table for this already loaded.  If so, return that.*/
    if( !bind )
    {
        bool bFoundElem;
        if( !bodyShotHT )
        {
            bodyShotHT = stashTableCreateAddress(1000);
        }
        //Use the uniqueId if provided, otherwise, use the ptr
        bFoundElem = stashAddressFindPointer( bodyShotHT, uniqueId ? S32_TO_PTR(uniqueId) : costume, &bind);

        if (!bFoundElem)
            bind = NULL;
            
        if( !bind )
        {
            //The thing chokes at les than 128 x 128, so wait till the screen is a decent size
            {
                int sizeOfPictureX = BODYSHOT_TEX_STARTING_RESOLUTION_X; //Size of shot on screen.
                int sizeOfPictureY = BODYSHOT_TEX_STARTING_RESOLUTION_Y; //Powers of two!!! ARgh.
                int screenX, screenY;
                windowSize( &screenX, &screenY );
                if( screenX <= sizeOfPictureX || screenY <= sizeOfPictureY )
                    bind = white_tex_atlas;
            }

            //if( game_state.game_mode != SHOW_GAME )
            //    bind = white_tex_atlas;
            
            //if( glob_have_camera_pos != PLAYING_GAME )
            //    bind = white_tex_atlas;

            if( !bind )
            {
                //Otherwise use the costume to generate a sequencer, take a picture, and free it
                //buildEntAndTakeAPicture( const cCostume * costume );
                e = entCreate("");
                e->costume = costume;
                costume_Apply( e );
                entUpdateMat4Interpolated(e, unitmat);
                copyMat4( ENTMAT(e), e->seq->gfx_root->mat );
                animSetHeader( e->seq, 0 );

                picturebrowser_clearEntities();
                bind = takePicturHeadBodyShot( e, 1, 0, 1 );

                //Add to hash table
                bodyShotCount++; //debug


                stashAddressAddPointer( bodyShotHT, uniqueId ? S32_TO_PTR(uniqueId) : costume, bind, true);
            }
        }
    }
    
    PERFINFO_AUTO_STOP();
    
    return bind;
}

//#########################################################################################
//#### Head Texture #################################################################################

typedef struct
{
    const cCostume *costume;
    int uniqueId;
} RequestedHeadshot;

int headShotCount;
StashTable headShotHTHero;
StashTable headShotHTVillain;
RequestedHeadshot **requestedHeadshots = NULL;

static StashTable * s_getHeadshotStashTable()
{
    int isVillain = s_isVillainous();
    return ((isVillain)?&headShotHTVillain:&headShotHTHero);
}

AtlasTex *seqMakeHeadshot( const cCostume * costume, int uniqueId )
{
    AtlasTex *bind;
    Entity *e = entCreate("");
    StashTable *ht = s_getHeadshotStashTable();
    e->costume = costume;
    costume_Apply( e );
    entUpdateMat4Interpolated(e, unitmat);
    copyMat4( ENTMAT(e), e->seq->gfx_root->mat );
    animSetHeader( e->seq, 0 );

    if (getRootHealthFxLibraryPiece( e->seq , (int)e->svr_idx ))
        manageEntityHitPointTriggeredfx(e);

    picturebrowser_clearEntities();
    bind = takePicturHeadBodyShot( e, 0, s_isVillainous(), 1 );

    //Add to hash table
    headShotCount++; //debug

    stashAddressAddPointer(*ht, uniqueId ? S32_TO_PTR(uniqueId) : costume, bind, false );

    return bind;
}

void seqClearHeadshotCache(void)
{
    StashTable *ht = s_getHeadshotStashTable();
    stashTableClear(*ht);
}

void seqRequestHeadshot( const cCostume * costume, int uniqueId )
{
    RequestedHeadshot *newRequest = (RequestedHeadshot *)malloc(sizeof(RequestedHeadshot));
    newRequest->costume = costume;
    newRequest->uniqueId = uniqueId;
    eaPush(&requestedHeadshots, newRequest);
}

void seqProcessRequestedHeadshots(void)
{
    RequestedHeadshot *curRequest = NULL;

    while ( eaSize(&requestedHeadshots) )
    {
        curRequest = eaPop(&requestedHeadshots);
        seqGetHeadshot( curRequest->costume, curRequest->uniqueId );
        //seqMakeHeadshot(curRequest->costume, curRequest->uniqueId);
        free(curRequest);
    }
}

//uniqueId : if you give me a uniqueId, I hash that, otherwise I hash the costume ptr
AtlasTex* seqGetHeadshotCached( const cCostume * costume, int uniqueId )
{
    //Entity * e;
    AtlasTex * bind;
    StashTable *ht = s_getHeadshotStashTable();

    PERFINFO_AUTO_START("seqGetHeadshot", 1);

    bind = 0;
    if( !costume )
        bind =  white_tex_atlas;

    //If you've already taken this guys picture, return it.
    //TO DO make this a hash table with proper freeing when it gets to big
    /*Look in the hash table for this already loaded.  If so, return that.*/
    if( !bind )
    {
        bool bBindFound;
        if( !(*ht) )
        {
            *ht = stashTableCreateAddress(1000);
        }
        //Use the uniqueId if provided, otherwise, use the ptr

        bBindFound = stashAddressFindPointer(*ht, uniqueId ? S32_TO_PTR(uniqueId) : costume, &bind);

        if( !bBindFound || game_state.imageServerDebug )
            bind = NULL;
    }

    return bind;
}

//uniqueId : if you give me a uniqueId, I hash that, otherwise I hash the costume ptr
int seqRemoveHeadshotCached( const cCostume * costume, int uniqueId )
{
    //Entity * e;
    int success = 0;
    AtlasTex * bind = 0;
    StashTable *ht = s_getHeadshotStashTable();

        if( !costume || !(*ht))
            return 0;

    success = stashAddressRemovePointer(*ht, uniqueId ? S32_TO_PTR(uniqueId) : costume, &bind);
    //can we free the texture?
    return success;
}
static int seqHeadshotUniqueCount = 0;
void updateSeqHeadshotUniqueID()
{
    seqHeadshotUniqueCount++;
}
int getSeqHeadshotUniqueID()
{
    return seqHeadshotUniqueCount;
}
AtlasTex* seqGetHeadshotForce( const cCostume * costume, int uniqueId )
{
    seqRemoveHeadshotCached( costume, uniqueId );
    return seqGetHeadshot( costume, uniqueId );
}
//uniqueId : if you give me a uniqueId, I hash that, otherwise I hash the costume ptr
AtlasTex* seqGetHeadshot( const cCostume * costume, int uniqueId)
{
    StashTable *ht = s_getHeadshotStashTable();
    return seqGetMMShotStore(costume, 1, PICTURETYPE_HEADSHOT,
        &headShotCount, ht, uniqueId);
}





#endif
