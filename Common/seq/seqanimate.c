#include <utilitieslib/utils/wininclude.h>  // JS: Can't find where this file includes <windows.h> so I'm just sticking this include here.
#include <string.h>
#include <time.h>
#include "seq/seq.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include "seq/anim.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h> 
#include "seq/gfxtree.h"

#include "seq/animtrack.h"
#include "seq/animtrackanimate.h"
#include "graphics/font.h"
#include "gridcoll/gridcoll.h"
#include "graphics/gfx.h" //for debug info
#include "player/player.h" //for debug info
#include "seq/seqregistration.h"
#include "seq/tricks.h"
#include "entity/entity.h"
#include <utilitieslib/utils/Quat.h>

#ifdef CLIENT 
#include "cmdparse/cmdgame.h"
#include "UI/uiCursor.h"
#include "fxutil.h"
#endif

#define NO_CHANGE         (0)   //used by interpolation code
#define CHANGE_ALL         (1<<0)
#define CHANGE_SINGLE     (1<<1)

#define CLOSE_ENOUGH_ROT    0.0003
#define CLOSE_ENOUGH_XLATE    0.001

#ifdef CLIENT
extern GameState game_state;
#endif

//CRAZYREG
static Mat4 globUIMats[BONEID_COUNT];

//################################################################################
//1. Update the Animation each frame##############################################
// animPlayInst and all its helper functions




/*Blend newrot and newpos with the last position and rotation based on percent new.  set change to one 
if this had any effect on the newrot and newpos*/
static INLINEDBG void animInterpNodeWithLastPosition(SeqInst * seq, int anim_id, Quat newrot, Vec3 newpos, F32 percent_new, int * change)
{
    F32            cos_theta;

    //Check that old rotation and translation is valid (if this node has been updated this move or last move it is)
    //Set current interpolation state if necessary (clunky for debug purposes)
    if( !seqIsBoneAnimationStored(seq, anim_id) || percent_new >= 1.0 )
    {
        seqSetBoneNeedsRotationBlending(seq, anim_id, 0 );
        seqSetBoneNeedsTranslationBlending(seq, anim_id, 0 );
    }

    //Interpolate between old and new if called for
    /*TO DO look into: if the cos_theta or dist is below threshhold, boost percent new instead of 
    just turning it off or some such scheme*/
    if( seqBoneNeedsRotationBlending(seq, anim_id)  )
    {    
        Quat qLastRot;
        seqGetLastRotations(seq, anim_id, qLastRot);
           cos_theta = quatInterp( percent_new, qLastRot, newrot, newrot );
        *change |= CHANGE_SINGLE;
        if( cos_theta > 1 - (CLOSE_ENOUGH_ROT * TIMESTEP) ) //TO DO test
            seqSetBoneNeedsRotationBlending(seq, anim_id, 0 );
    }
    if( seqBoneNeedsTranslationBlending(seq, anim_id) )
    {
        posInterp( percent_new, seqLastTranslations(seq, anim_id), newpos, newpos ); 
        *change |= CHANGE_SINGLE;
        if( nearSameVec3Tol( newpos, seqLastTranslations(seq, anim_id), (CLOSE_ENOUGH_XLATE * TIMESTEP)) ) //TO DO test
            seqSetBoneNeedsTranslationBlending(seq, anim_id, 0 );
    }
}

void matrixFromAxisAngle(Vec3 a1, F32 angle, Mat3 m) 
 {
    F32 c,s, t, tmp1, tmp2;

    mathutil_sincosf(angle, &s, &c);

    t = 1.0 - c;

    m[0][0] = c + a1[0]*a1[0]*t;
    m[1][1] = c + a1[1]*a1[1]*t;
    m[2][2] = c + a1[2]*a1[2]*t;

    tmp1 = a1[0]*a1[1]*t;
    tmp2 = a1[2]*s;
    m[1][0] = tmp1 + tmp2;
    m[0][1] = tmp1 - tmp2;

    tmp1 = a1[0]*a1[2]*t;
    tmp2 = a1[1]*s;
    m[2][0] = tmp1 - tmp2;
    m[0][2] = tmp1 + tmp2;
    tmp1 = a1[1]*a1[2]*t;
    tmp2 = a1[0]*s;
    m[2][1] = tmp1 + tmp2;
    m[1][2] = tmp1 - tmp2;
}


/*Get prospective rotation and translation Quat newrot and Vec3 newpos set change if they might have 
changed from last frame*/
static INLINEDBG void animGetNextFrame( const BoneAnimTrack * bt, int intTime, F32 remainderTime, Vec3 newpos, Quat newrot, int interpolate)
{
    animGetRotationValue( intTime, remainderTime, bt, newrot, interpolate );
    animGetPositionValue( intTime, remainderTime, bt, newpos, interpolate );
}

F32 computeHipCorrectionScale(SeqInst * seq)
{
    return 1.0 + (seq->type->legScaleRatio * seq->legScale);
}

//HEADTURN
Mat4 globNeckWorldMat;

/* new way: get the new pos and rot from the anim track, interpolate it with the move last frame if you are 
given a percent new and the last frame had this node. (This is not perfectly efficient for clarity: maybe make 
tighter after i do the blending*/
static void animateGfxNodeBone(SeqInst * seq, GfxNode * gfxnode, int intTime, F32 remainderTime, F32 percent_new, int * change)
{
    Quat newrot;
    Vec3 newpos;
    BoneId anim_id;
    int interpolate;

    if( !gfxnode->bt )
        return;

    anim_id = gfxnode->anim_id;
    assert(bone_IdIsValid(anim_id));
  
    //figure out whether to be doing interpolation //move up one func
#ifdef CLIENT
    if( (seq->animation.move->flags & SEQMOVE_NOINTERP) || game_state.nointerpolation )
        interpolate = 0;
    else  
        interpolate = 1;
#endif
#ifdef SERVER 
    interpolate = 0;
#endif
    //Get the newpos and newrot                                                           
    animGetNextFrame( gfxnode->bt, intTime, remainderTime, newpos, newrot, interpolate ); 

    //Cheesy bit of head, neck, shoulder & hip movement (development only)
    if(1)  
    {
        Vec3 dv;     //0.08 -0.05
        switch(gfxnode->anim_id)
        {
            xcase BONEID_COL_R: case BONEID_COL_L:
                if(seq->shoulderScale <= 0)
                    scaleVec3(seq->type->shoulderScaleSkinny, (-seq->shoulderScale), dv );
                else
                    scaleVec3( seq->type->shoulderScaleFat, seq->shoulderScale, dv );
                if(gfxnode->anim_id == BONEID_COL_L)
                    dv[0] *= -1; //x axis is symmetrical
                addVec3( newpos, dv, newpos );

            xcase BONEID_NECK: case BONEID_HEAD:
                scaleVec3( seq->type->neckScale, seq->neckScale, dv );
                dv[0] = 0;
                dv[1] = -dv[1]; // vertical only
                dv[2] = 0;
                addVec3( newpos, dv, newpos );

            xcase BONEID_ULEGR: case BONEID_ULEGL:
                scaleVec3( seq->type->hipScale, seq->hipScale, dv );
                if(gfxnode->anim_id == BONEID_ULEGL)
                    dv[0] *= -1; //x axis is symmetrical
                addVec3( newpos, dv, newpos );

//             xcase BONEID_HIPS:
//                 scaleVec3( seq->type->hipScale, seq->hipScale, dv );
//                 dv[1] *= -1;
//                 addVec3( newpos, dv, newpos );
        }
    }
    //End cheesy shoulder movement

    //MULTITYPES
    subVec3( newpos, gfxnode->animDelta, newpos );

    //If you are interpolating, then go all the way
    if( interpolate )
    {
        *change |= CHANGE_SINGLE; //PTOR need to figure out new way to know whether the anim changed materailly from last frame

        if( ( seqBoneNeedsRotationBlending(seq, anim_id) || seqBoneNeedsTranslationBlending(seq, anim_id) ) && percent_new < 1.0 )
            animInterpNodeWithLastPosition( seq, anim_id, newrot, newpos, percent_new, change );

        //Assign the newpos and newrot
        if( *change != NO_CHANGE) 
        {
            copyVec3( newpos, seqLastTranslations(seq, anim_id) );
            copyVec3( newpos, gfxnode->mat[3] );

            quatToMat( newrot, gfxnode->mat );   
            seqSetLastRotations(seq, anim_id, newrot);

            *change = (*change) & ~CHANGE_SINGLE;
        }

        //Mark this node as recently updated
        seqSetBoneAnimationStored(seq, anim_id);
    }
    else 
    {
        quatToMat(newrot, gfxnode->mat );
        copyVec3( newpos, gfxnode->mat[3] );
    }

    // scale and unscale along the nodes
    switch(gfxnode->anim_id)
    {
        xcase BONEID_HIPS:
            if(seq->legScale)
                unscaleBone(gfxnode, 1, 1.0f / computeHipCorrectionScale(seq), 1);

#ifdef CLIENT
        xcase BONEID_WAIST:
            if(seq->currTargetingPitchVec[0])
                pitchMat3(seq->currTargetingPitchVec[0], gfxnode->mat);
            if(seq->currTargetingPitchVec[1])
                rollMat3(seq->currTargetingPitchVec[1], gfxnode->mat);
#endif

#ifdef CLIENT
        xcase BONEID_CHEST:
            if(seq->chestYaw) //chest turn to look
                yawMat3(seq->chestYaw, gfxnode->mat);
#endif

        xcase BONEID_HEAD:
            scaleBone(gfxnode, (1 + seq->headScales[0]), (1 + seq->headScales[1]), (1 + seq->headScales[2]));
#ifdef CLIENT
            if(seq->headYaw)  //head turn to look     
                yawMat3( seq->headYaw, gfxnode->mat );
#endif

        xcase BONEID_ULEGR: case BONEID_ULEGL:
            if(seq->legScale)
                scaleBone(gfxnode, 1, (1 + seq->legScale), 1);

        xcase BONEID_LLEGR: case BONEID_LLEGL:
            if(seq->legScale)
            {
                unscaleBone(gfxnode, 1, (1.0f / (1 + seq->legScale)), 1);
                scaleBone(gfxnode, 1, (1 + seq->legScale), 1);
            }

        xcase BONEID_FOOTR: case BONEID_FOOTL:
            if(seq->legScale)
                unscaleBone(gfxnode, 1, (1.0f / (1 + seq->legScale)), 1);

        xcase BONEID_LARMR: case BONEID_LARML:    // LARM
            if(seq->armScale)
            {
                F32 armscale = 1.0 + seq->armScale;
                scaleBone(gfxnode, armscale, armscale, armscale);
            }
            // this will trickle down the skeleton
            // it should be okay since it's scaled evenly in all directions
            // we just have to unscale where we want to quit scaling (like at WEP below)

        xcase BONEID_WEPR: case BONEID_WEPL:
            if(seq->armScale)
            {
                F32 armunscale = 1.0f / (1.0 + seq->armScale);
                unscaleBone(gfxnode, armunscale, armunscale, armunscale);
            }
    }

//        int y = 30; //debug
//        Mat4 headWorldMat;
//        //Vec3 targetPYR;
//        //Vec3 bodyPYR; //For figuring whether to do this at all...Maybe use the chest instead?
//
//        mulMat4( globNeckWorldMat, gfxnode->mat, headWorldMat );
//
//        if( anim_id == 4 ) //THis is the head
//        {
//            Vec3 headPyr;
//            F32 len;
//            Vec3 dv;
//            Vec3 temp;
//            Vec3 headVec;
//            Vec3 goalPyr;
//            Vec3 neckPyr;
//            Vec3 neckVec;
//            //Vec3 deltaPyr;
//            Mat4 goalMat;
//            //Mat4 neckMat;
//            Mat4 invNeck;
//            Mat4 goalMatLocal;
//
////            GfxNode * neckNode    = getNodeFromName(seq, "NECK" );
//
//            //Debug
//            temp[0] = 0;
//            temp[1] = 0;
//            temp[2] = 1;
//            mulVecMat3( temp, headWorldMat, headVec );
//            mulVecMat3( temp, globNeckWorldMat, neckVec );
//            //End debug
//            //End debug
//
//            //Get Head PYR and HeadMat
//            getMat3YPR(headWorldMat,headPyr);  
//
//            //Get Goal PYR and GoalMat
//            subVec3( seq->headTargetPoint, headWorldMat[3], dv );  
//            len = normalVec3( dv );
//            getVec3YP(dv,&goalPyr[1],&goalPyr[0]); 
//            goalPyr[2] = 0;  
//
//            createMat3YPR(goalMat, goalPyr);  
//            copyVec3( headWorldMat[3], goalMat[3] ); //same x lation
//
//            //Get Neck PYR
//            getMat3YPR(globNeckWorldMat, neckPyr); 
//            //globNeckWorldMat
//
//            // globNeck * X = goalMat
//            invertMat4Copy(globNeckWorldMat,invNeck);
//            mulMat4( invNeck, goalMat, goalMatLocal ); 
//            copyMat4( goalMatLocal, gfxnode->mat ); 
//
//            //Get diff between Neck and Goal, create a mat for that
//            //No, not right. TO DO make this always right, and I'm a long way to there.
//            //subVec3( goalPyr, neckPyr, deltaPyr );
//            //createMat3PYR(goalMat, deltaPyr); 
//               
//
//            if( playerPtr()->seq == seq )  
//            {
//            //    Vec3 end;  
//            //    addVec3( headWorldMat[3], headVec, end);
//            //    addDebugLine( headWorldMat[3], end, 255, 255, 255, 255, 2 );
//            //    addVec3( globNeckWorldMat[3], neckVec, end);
//            //    addDebugLine( globNeckWorldMat[3], end, 100, 100, 255, 255, 3 );
//            //    addVec3( headWorldMat[3], dv, end);
//            //    addDebugLine( headWorldMat[3], end, 255, 100, 100, 255, 2 ); 
// 
//                xyprintf( 30, y++, "HEAD PYR %f %f %f", headPyr[0], headPyr[1], headPyr[2]  );
//            //    y++;            
//                xyprintf( 30, y++, "NECK PYR %f %f %f", neckPyr[0], neckPyr[1], neckPyr[2]  );
//                 xyprintf( 30, y++, "GOAL PYR %f %f %f", goalPyr[0], goalPyr[1], goalPyr[2]  );
//            //    //xyprintf( 30, y++, "DELTA PYR %f %f %f", deltaPyr[0], deltaPyr[1], deltaPyr[2]  );
//    
//            }
//        }
//        else
//        {
//            copyMat4( headWorldMat, globNeckWorldMat ); //by the time this gets to the head, these names are right
//        }

    //if( seq->currTargetingPitch && anim_id == 0 )  //waist bend  
    //    pitchMat3( seq->currTargetingPitch*0.2, gfxnode->mat );
    //seq->currTargetingPitch = 0.8;
    //seq->animation.typeGfx->pitchAngle = ABS(game_state.splatShadowBias);  
 
                /*
                //copyMat3( unitmat, gfxnode->parent->mat );    
        //yawMat3( 3.505, gfxnode->parent->mat);
            F32 targetingPitch = game_state.splatShadowBias;  
            Mat3 temp1, temp2; 

            copyMat3( unitmat, gfxnode->mat );   
            yawMat3( 1.505, gfxnode->mat );

            copyMat3( unitmat, temp1 ); 
            pitchMat3( targetingPitch, temp1 );
            mulMat3( temp1, gfxnode->mat, temp2);
            copyMat3( temp2, gfxnode->mat );
            */
    //END AIM PITCH


    //CRAZYREG
    /*
    if( anim_id == getBoneNumberFromName( "HIPS" ) || 
        anim_id == getBoneNumberFromName( "ULEGL" ) || 
        anim_id == getBoneNumberFromName( "ULEGR" ) || 
        anim_id == getBoneNumberFromName( "LLEGL" ) || 
        anim_id == getBoneNumberFromName( "LLEGR" ) || 
        anim_id == getBoneNumberFromName( "FOOTL" ) || 
        anim_id == getBoneNumberFromName( "FOOTR" ) || 
        anim_id == getBoneNumberFromName( "TOEL" ) || 
        anim_id == getBoneNumberFromName( "TOER" )
        )
    {
        Mat4Ptr mat;
        mat = globUIMats[anim_id];
        quatToMat( &newrot, mat );
        copyVec3( newpos, mat[3] );
    }
    */

        //ENd aim up or down
}

#ifdef CLIENT
void calcHeadTurn(SeqInst * seq)
{
    F32 delta = distance3Squared(seq->gfx_root->mat[3], seq->posLastFrame);
    xyprintf(40, 8,  "Pos Delta: %.2f", delta);

          if(SQR(delta) > SQR(0.05))
    {
        Vec3 ypr;

        getMat3YPR(seq->gfx_root->mat, ypr);

        seq->headTargetPoint[0] = ypr[1];

        // TODO: do smoothing & return
    }
}
#endif

#ifdef CLIENT
#include "UI/uiUtil.h" // debug only
void drawWorldLine(Vec3 a, Vec3 b, UINT color, float width);

void doHeadTurn(SeqInst * seq)
{
    //HEADTURN 
#define MAX_HEAD_TURN     ( 105  * (PI / 180))

     if( playerPtr() && playerPtr()->seq == seq )
    {
        Mat4 temp, mat;
        Vec3 rootYPR, ypr;
        int sign;
        F32 headTurn, waistTurn, hipTurn;
        F32 turn;

         GfxNode * hipNode    = seqFindGfxNodeGivenBoneNum(seq, BONEID_HIPS);
        GfxNode * neckNode    = seqFindGfxNodeGivenBoneNum(seq, BONEID_HEAD);
        GfxNode * waistNode    = seqFindGfxNodeGivenBoneNum(seq, BONEID_WAIST);

        // determine body turn &  reverse rotate back
        getMat3YPR(seq->gfx_root->mat, rootYPR);
        xyprintf(40, 10, "Body YPR: %.3f, %.3f, %.3f", rootYPR[0], rootYPR[1], rootYPR[2]);
        xyprintf(40, 11, "hipOrient: %.3f", seq->headTargetPoint[0]);
        xyprintf(40, 11, "Max Head Turn: %.3f", MAX_HEAD_TURN);

        // compute angle between vectors
 
        turn = rootYPR[1] - seq->headTargetPoint[0];
        sign = (turn >= 0) ? 1 : -1;
        turn = ABS(turn);

         if(turn > 0.02)
        {                
            // compute body rotation
             if(turn > MAX_HEAD_TURN)
            {    
                // turn body... for now, snap to new position
                turn = 0;
                seq->headTargetPoint[0] = rootYPR[1];
            }

             // compute head & waist rotation    
               headTurn   = sign * min(turn * .4, MAX_HEAD_TURN);
              waistTurn  = sign * min(turn * .6, MAX_HEAD_TURN);
            hipTurn    = - (sign * turn);

             xyprintf(40, 15, "Head  Turn: %.2f", headTurn);
            xyprintf(40, 16, "Waist Turn: %.2f", waistTurn);
            xyprintf(40, 17, "Hip   Turn: %.2f", hipTurn);

            // head rotation
            setVec3(ypr, 0, headTurn, 0);
            createMat3YPR(mat, ypr);
             mulMat3(neckNode->mat, mat, temp);
            copyMat3(temp, neckNode->mat);

            // rotate waist
            setVec3(ypr, 0, waistTurn, 0);
            createMat3YPR(mat, ypr);
             mulMat3(waistNode->mat, mat, temp);
             copyMat3(temp, waistNode->mat);    

            // overall correction (reverse direction)
             setVec3(ypr, 0, hipTurn, 0);
            createMat3YPR(mat, ypr);
            mulMat3(  mat, hipNode->mat,temp);         
            copyMat3(temp, hipNode->mat);
        }

        {
            // debug lines
            Vec3 v1, v2, a;
            Mat4 headMat, mat;

             gfxTreeFindWorldSpaceMat(headMat, neckNode);

             setVec3(v1, 0, 0, 2); 
            setVec3(ypr, 0, rootYPR[1], 0);
            createMat3YPR(mat, ypr);
            mulVecMat3(v1, mat, v2);
            addVec3(headMat[3], v2, a);
            drawWorldLine(headMat[3], a, CLR_RED, 5);

            setVec3(v1, 0, 0, 1); 
            setVec3(ypr, 0, seq->headTargetPoint[0], 0);
            createMat3YPR(mat, ypr);
            mulVecMat3(v1, mat, v2);
            addVec3(headMat[3], v2, a);
            drawWorldLine(headMat[3], a, CLR_WHITE, 8);
        }
    }
}
#endif

int totalBonesAnimated;
int totalBonesRejected;

int totalNodesProcessed;
int totalNodesRejected;

//Debug function for counting rejected nodes
static int countNonHidChildren(GfxNode * pNode, int seqHandle)
{
    GfxNode * node;
    int total = 0; 
    for( node = pNode ; node ; node = node->next )
    {
        if( node->seqHandle == seqHandle )
        {
            total++;
            if( node->child )
                total+=countNonHidChildren( node->child, seqHandle );
        }
    }
    return total;
}

//Another debug only function
#include "player/player.h"
#ifdef CLIENT
#include "entity/entclient.h"
void printBonesAnimated( SeqInst * seq, GfxNode * node )
{
    Entity * e;
    SeqInst * mySeq = 0;


    if( game_state.animdebug & ANIM_SHOW_MY_ANIM_USE )
    {
        mySeq = playerPtr()->seq;
    }
    else if( game_state.animdebug & ANIM_SHOW_SELECTED_ANIM_USE )
    {
        e = current_target;
        if( e )
            mySeq = e->seq;
    }

    if( seq == mySeq )
        xyprintfcolor( 2, 2+node->anim_id, 255, 255, 255, "Animated: %d %s", node->anim_id, bone_NameFromId(node->anim_id) );
}
#endif

#define PRINT_SKELETON_HIERARCHY 0
#if PRINT_SKELETON_HIERARCHY
static void printnode(GfxNode * gfxnode, int depth)
{
    static char buf[256];
    int i;
    GfxNode * node;
    for(node = gfxnode ; node ; node = node->next)       
    {  
        for(i=0; i<depth; i++) OutputDebugString("\t");
        sprintf(buf, "%s(%d)\n", bone_NameFromId(node->anim_id), node->anim_id );
        OutputDebugString(buf);
        if(node->child)
            printnode(node->child, depth+1);
    }
}
#endif

static void animateSkeleton(SeqInst * seq, GfxNode * gfxnode, int intTime, F32 remainderTime, F32 percent_new, int * change) 
{
    GfxNode * node;

#if PRINT_SKELETON_HIERARCHY // debug code 
    static bool bPrintHier = false;
    if( bPrintHier )
    {
        bPrintHier = false;
        printnode(gfxnode, 0);
        OutputDebugString("END\n");
    }
#endif

    for(node = gfxnode ; node ; node = node->next)       
      {  
        
        if( seq->handle == node->seqHandle && node->useFlags ) //Dont try to animate things that aren't part of this anim, or aren't used by anything right now
        {  
            if( !( node->flags & GFXNODE_DONTANIMATE ) )
                animateGfxNodeBone( seq, node, intTime, remainderTime, percent_new, change );

            if(node->child)
                animateSkeleton(seq, node->child, intTime, remainderTime, percent_new, change);

#ifdef CLIENT 
            //Debug
            if( game_state.animdebug )
            {
                totalBonesAnimated++;
                printBonesAnimated( seq, node );
            }
            //End debug
#endif
        }
#ifdef CLIENT 
        //Debug
        else if( game_state.animdebug && seq->handle == node->seqHandle && !node->useFlags)
        { 
            totalBonesRejected++;
            totalBonesRejected += countNonHidChildren(node->child, node->seqHandle);
        }
        //End Debug
#endif
    }
}


//CRAZYREG
#ifdef CLIENT
void doCrazy( SeqInst * seq )
{
    //Only if you are on the ground
        Mat4 temp1;
        Mat4 temp2;
        Mat4 footL;
        Mat4 footR;
        Mat4 toeL;
        Mat4 toeR;
        Mat4Ptr leftLow;
        Mat4Ptr rightLow;

        int hitL;
        Mat4 hitPosL;
        int hitR;
        Mat4 hitPosR;

        //return; 
        
        copyMat4( seq->gfx_root->mat, temp1 );
        mulMat4( temp1, globUIMats[BONEID_HIPS], temp2 );
        mulMat4( temp2, globUIMats[BONEID_ULEGL], temp1 );
        mulMat4( temp1, globUIMats[BONEID_LLEGL], temp2 );
        mulMat4( temp2, globUIMats[BONEID_FOOTL], footL );
        mulMat4( footL, globUIMats[BONEID_TOEL], toeL );

        copyMat4( seq->gfx_root->mat, temp1 );
        mulMat4( temp1, globUIMats[BONEID_HIPS], temp2 );
        mulMat4( temp2, globUIMats[BONEID_ULEGR], temp1 );
        mulMat4( temp1, globUIMats[BONEID_LLEGR], temp2 );
        mulMat4( temp2, globUIMats[BONEID_FOOTR], footR );
        mulMat4( footR, globUIMats[BONEID_TOER], toeR );

        leftLow = (footL[3][1]) < (toeL[3][1]) ? footL : toeL;  
        rightLow = (footR[3][1]) < (toeR[3][1]) ? footR : toeR; 

        
        {
            Mat4Ptr low;
            Vec3 start;
            Vec3 end;
            CollInfo coll  = {0};

            low = leftLow;
            copyVec3( low[3], start );
            copyVec3( low[3], end );
            start[1] += 2.0; 
            end[1] -= 3.0;
            hitL = collGrid(0, start, end, &coll, 0.3, COLL_CYLINDER | COLL_IGNOREINVISIBLE | COLL_DISTFROMSTART | COLL_BOTHSIDES);
            
            if( hitL )
                copyMat4( coll.mat, hitPosL );
        }

        {
            Mat4Ptr low;
            Vec3 start;
            Vec3 end;
            CollInfo coll  = {0};

            low = rightLow;
            copyVec3( low[3], start );
            copyVec3( low[3], end );
            start[1] += 3.0; 
            end[1] -= 3.0;
            hitR = collGrid(0, start, end, &coll, 0.3, COLL_CYLINDER | COLL_IGNOREINVISIBLE | COLL_DISTFROMSTART | COLL_BOTHSIDES);

            if( hitR )
                copyMat4( coll.mat, hitPosR );
        }

        
        if( seq == playerPtr()->seq )
        {
            int y = 10;
            xyprintf( 20, y++, "leftLow  %f", leftLow[3][1] );
            if( hitL ) 
                xyprintf( 20, y++, "hitPosL  %f", hitPosL[3][1] );  else y++;
            xyprintf( 20, y++, "rightLow %f", rightLow[3][1] ); 
            if( hitR )
                xyprintf( 20, y++, "hitPosR  %f", hitPosR[3][1] ); else y++;
        }
         
#define LEFTLEG 1
#define RIGHTLEG 2
#define THESAME 3
        //Now you know what you need to know
        {
            int lowLeg; 
            F32 lowPos, drop;

            lowLeg = THESAME;

            if( hitR && hitL ) 
            {
                lowLeg = (hitPosL[3][1] < hitPosR[3][1]) ? LEFTLEG : RIGHTLEG;
                if( hitPosL[3][1] == hitPosR[3][1] )
                    lowLeg = THESAME;

                lowPos = MIN( hitPosL[3][1], hitPosR[3][1] );
            }
            else if ( hitR )
                lowPos = hitPosR[3][1];
            else if ( hitL )
                lowPos = hitPosL[3][1];
            else
                lowPos = 0;

            drop = seq->gfx_root->mat[3][1] - lowPos;

            if( lowPos )
                seq->gfx_root->mat[3][1] = lowPos;

            if( seq == playerPtr()->seq )
            {
                int y = 20;
                xyprintf( 20, y++, "LOWPOS %f", lowPos );
                xyprintf( 20, y++, "LOWLEG %d", lowLeg );
                xyprintf( 20, y++, "DROP   %f", drop );
                xyprintf( 20, y++, "DROP   %f", drop );
                xyprintf( 20, y++, "DROP   %f", drop );
            }

            if( lowLeg == LEFTLEG ) //RAISE THE RIGHT LEG
            {
                F32 raiseDist, ULegRot, LLegRot, FootRot;
                GfxNode * node;
                raiseDist = hitPosR[3][1] - (  rightLow[3][1] - drop );

                ULegRot = raiseDist * 0.5;
                LLegRot = raiseDist * 0.5;
                FootRot = raiseDist * -0.5;

                node = seqFindGfxNodeGivenBoneNum(seq, BONEID_ULEGR);
                pitchMat3( ULegRot, node->mat );
                node = seqFindGfxNodeGivenBoneNum(seq, BONEID_LLEGR);
                pitchMat3( ULegRot, node->mat );
                node = seqFindGfxNodeGivenBoneNum(seq, BONEID_FOOTR);
                pitchMat3( ULegRot, node->mat );
            }
            else if( lowLeg == RIGHTLEG ) //RAISE THE LEFT LEG
            {

            }
            else {} //DO NOTHING

        }
}
#endif

/*given a animation, update the gfx_node's heirarchy*/
void animPlayInst( SeqInst * seq )
{
    F32            percent_new;
    int            change = NO_CHANGE;    
    Animation * animation;

    animation = &seq->animation;

    //1a. Set up interpolation rate from last frame
    if( seq->curr_interp_frame == 0 )
        change |= CHANGE_ALL;

    seq->curr_interp_frame += TIMESTEP;

    if( seq->curr_interp_frame < (F32)seq->animation.move->interpolate )
        percent_new = 1 / (1 + (F32)seq->animation.move->interpolate - seq->curr_interp_frame);//true linear
    else
        percent_new = 1.0;


    //1b. compute interpolation factor for fixing arm registration 
     if(animation->move->flags & (SEQMOVE_FIXARMREG|SEQMOVE_FIXWEAPONREG))
        seq->curr_fixInterp += TIMESTEP;
    else
          seq->curr_fixInterp -= TIMESTEP;

    seq->curr_fixInterp = MINMAX(seq->curr_fixInterp, 0.0, 1.0);
    seq->bFixWeaponReg = !!(animation->move->flags & SEQMOVE_FIXWEAPONREG);


    //2. Update anim node by node
    {
        int intTime = 0;
        F32 remainderTime;

        intTime =(int)animation->frame;
        remainderTime = animation->frame - (F32)intTime; 

        //copyMat4( seq->gfx_root->mat, globNeckWorldMat); //HEADTURN
        
        // moved to once per frame update in entClientUpdate -- fixes issue with LOD changing between viewports
         //if(seq->legScale)
        //    scaleBone(seq->gfx_root, 1, computeHipCorrectionScale(seq), 1);

         animateSkeleton(seq, seq->gfx_root->child, intTime, remainderTime, percent_new, &change);

    }

    //CRAZYREG
    //if you are on the ground
    //doCrazy( seq );
}


#ifdef CLIENT
void showAnimationDebugInfo() 
{
    xyprintf( 2, 70, "BonesAnimated: %d  NodesProcessed: %d ", totalBonesAnimated, totalNodesProcessed );
    xyprintf( 2, 71, "BonesRejected: %d  NodesRejected:  %d ", totalBonesRejected, totalNodesRejected );

    totalNodesProcessed = 0;
    totalBonesAnimated = 0;
    totalBonesRejected = 0;
    totalNodesRejected = 0;
}
#endif
