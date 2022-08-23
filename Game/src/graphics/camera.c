#include "graphics/camera.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/font.h"
#include "win/win_init.h"
#include "seq/seq.h"
#include "cmdparse/cmdgame.h"
#include "cmdparse/cmdcommon.h"
#include "player/player.h"
#include "gridcoll/gridcoll.h"
#include <utilitieslib/utils/memcheck.h>
#include "edit/edit_cmd.h"
#include "entity/entclient.h"
#include <utilitieslib/utils/mathutil.h>
#include "render/model.h"
#include "group/group.h"
#include "edit/edit.h"
#include <utilitieslib/assert/assert.h>
#include "graphics/gfx.h"
#include "entity/entDebug.h"
#include "UI/uiNet.h"
#include "entity/character_base.h"
#include "entity/motion.h"
#include "cmdparse/cmdcontrols.h"
#include <utilitieslib/components/SharedMemory.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "bases/baseedit.h"
#include "render/renderprim.h"
#include "UI/uiUtilGame.h"
#include "graphics/splat.h"
#include "graphics/sun.h"

CameraInfo    cam_info;

void camSetPyr(Vec3 pyr)
{
    if(cam_info.camIsLocked == true) 
        return;

    copyVec3(pyr,cam_info.targetPYR);

    //Do CameraShake, if any
    if (!editMode())
    {
        F32 factor = game_state.camera_shake;
        cam_info.targetPYR[0] = fixAngle(cam_info.targetPYR[0] + factor * RAD((rand() % 31 - 15) / 10.f));
        cam_info.targetPYR[1] = fixAngle(cam_info.targetPYR[1] + factor * RAD((rand() % 31 - 15) / 10.f));
        cam_info.targetPYR[2] = fixAngle(cam_info.targetPYR[2] + factor * RAD((rand() % 91 - 45) / 10.f));    
    }
}


/* interpolate sets a position that where the camera should eventually move to.  The camera code may decide how fast to move to the given position.*/
void camSetPos(const Vec3 pos, bool interpolate )
{
    if(cam_info.camIsLocked == true)  
        return;

    copyVec3(pos,cam_info.mat[3]);

    if (!editMode())
    {
        copyVec3( pos, cam_info.cammat[3] );

        if( !interpolate )
            gfxSetViewMat(cam_info.cammat, cam_info.viewmat, cam_info.inv_viewmat);

        if( interpolate )
        {
            float factor = game_state.camera_shake;
            cam_info.mat[3][0] += factor * (rand() % 3 - 1) / 10.f;
            cam_info.mat[3][2] += factor * (rand() % 3 - 1) / 10.f;
        }
    }
}

void camLockCamera( bool isLocked )
{
    cam_info.camIsLocked = isLocked;
}

void camPrintInfo()
{
    if (game_state.info)
    {
        char buffer[1024];

        sprintf(buffer, "CAM% 3.1f % 3.1f % 3.1f Y=%3.0f P=%3.0f R=%3.0f",
            cam_info.cammat[3][0],cam_info.cammat[3][1],cam_info.cammat[3][2],
            DEG(cam_info.pyr[1]),DEG(cam_info.pyr[0]),DEG(cam_info.pyr[2]));
        printDebugString(buffer, 1, 14, 1, 11, -1, 0, 255, NULL);

    }
}

/////////////////////////////////////////////////////////////////////////////////////
///////// Below :CamUpdate and helper functions

static F32 camTimeStep()
{
    return 0 ? TIMESTEP : TIMESTEP_NOSCALE;
}

static int cameraCollisionCallback(DefTracker *tracker, CTri* triangle){
    // Do not collide against any portals that are not water.
    if(!tracker->def->water_volume && (triangle->flags & COLL_PORTAL)){
        return 0;
    }
    return 1;
}

static void camPushPull(CameraInfo* ci, float newZDist, int snap)
{
    float idealDelta;
    float lastDist = ci->lastZDist;

    if(snap)       
    {
        lastDist = newZDist;
    }
    else
    {
        float realDelta;

        idealDelta = newZDist - lastDist;
        realDelta = idealDelta * 0.1 * camTimeStep();

        // Clamp real delta to ideal delta in case timestep is large.
        if(fabs(realDelta) > fabs(idealDelta))
            realDelta = idealDelta;
        
        lastDist = lastDist + realDelta;
    }

    ci->lastZDist = lastDist;
}


static void camSetAlpha(CameraInfo* ci)
{
    Vec3 translucencyInterpPoints[] = { {1000, 255, 0},{10, 255, 0},{3, 255, 0}, {0.0, 16, 0}, {-1000, 0, 0} };
    int interpResult;

    interpResult = graphInterp(ci->lastZDist, translucencyInterpPoints); 
    //xyprintf( 20, 20, "cam SetAlpha %d", interpResult ); 
    entSetAlpha( playerPtr(), interpResult, SET_BY_CAMERA );    
}

float camGetPlayerHeight()
{
    Entity* player = playerPtr();
        
    float camRaiseHeight = 0.0;
    // Figure out new camera height based on player height.
    // Account for any geometry that may have cut through the player model.
    if(player){
        static F32 curHeight;

        CollInfo heightColl;
        Vec3 newCameraTarget;
        Vec3 playerPos;

        // Default camera height is the same as the player height.
        camRaiseHeight = playerHeight();

        // If some geometry is intersecting with the player model's upper body, placing the
        // camera at the default height will put the camera inside the intersecting geometry.
        // If this is the case, pull the camera down a little.


         if(!curHeight)
        {
            curHeight = camRaiseHeight;
        }
        
        if(player && player->pchar && player->pchar->attrCur.fHitPoints <= 0)
        {
            camRaiseHeight = 2.0;
        }
        
        curHeight += (camRaiseHeight - curHeight) * TIMESTEP / 30;
        
        camRaiseHeight = curHeight;

        if(!control_state.nocoll && player->pl && !baseedit_Mode()){
            copyVec3(ENTPOS(player), playerPos);
            copyVec3(ENTPOS(player), newCameraTarget);
            playerPos[1] += 1;
            newCameraTarget[1] += camRaiseHeight + 1.5; // 1.5 is to account for camera radius
            if(collGrid(0, playerPos, newCameraTarget, &heightColl, 0, COLL_DISTFROMSTART))
            {
                //xyprintf(10, 10, "coll %f", heightColl.mat[3][1]);
                camRaiseHeight = MAX(0, heightColl.mat[3][1] - ENTPOSY(player) - 1.5);
            }
        }

    }

    return camRaiseHeight;
}


float findProperZDistanceOld( CameraInfo * ci, Vec3 newCameraTarget, F32 maxZDist, int * instantPullPush )
/*********************************
* Move camera Phase 2: Pull back camera
*    The camera has been assigned a target where it is difficult for visul artifacts
*    to appear.  Pull the camera back as far as possible now (up to some distance cap).
*
*    This phase is very similar to phase 2.  We pull the camera back as far as it can go,
*    then check if various cushion points have collided against something.  If it any
*    of the cushion points hits something, pull figure out how close to pull the camera
*    in to avoid the collision.
*
*    Wierd variable names?
*    A "reverseFOV" is the angle between the vector that goes from the cam target to
*    a cushion point and the vector that goes from the cam target to the camera.  This
*    angle can be used to figure out how far to pull the camera in to avoid a collision
*    along the target to cushion point collision.
*
*/
{
    // Tweakable variables    
    F32 cameraRadius = 0.6;  // cushion radius around camera
    F32 dist;
    F32 reverseFOV;
    int camRadCollInfo = 0;
    Vec3 newCameraPos;
    Vec3 dv;
    int i;
    Entity *player = playerPtr();

    // Camera collision related variables
#define camRadDirections 9
    CollInfo camRadColl[camRadDirections];
    Vec3 camSrc[camRadDirections];
    Vec3 camDst[camRadDirections];
    char* camRadNames[] = {"top ", "bottom ", "left ", "right ", "t2cam ", "topright ", "topleft "};


    // Pull the camera as far back as it will go.
    dist = maxZDist;
    copyVec3(newCameraTarget, newCameraPos);
    moveVinZ(newCameraPos, ci->cammat, dist);

    if(!baseedit_Mode())
    {
        // Find "reverseFOV"
        //        Find the vector pointing from the camera target to the new camera position
        subVec3(newCameraTarget, newCameraPos, dv);
        //        Use magnitude of the vector and the "camera radius" to find the angle of the vector
        reverseFOV = atan(cameraRadius / fsqrt(SQR(dv[0]) + SQR(dv[1]) + SQR(dv[2])));

        // Find various cushion points around the camera
        // Top
        copyVec3(newCameraPos, camDst[0]);
        moveVinY(camDst[0], ci->cammat, cameraRadius);
        copyVec3(newCameraTarget, camSrc[0]);
        moveVinY(camSrc[0], ci->cammat, cameraRadius);

        // Bottom
        copyVec3(newCameraPos, camDst[1]);
        moveVinY(camDst[1], ci->cammat, -cameraRadius);
        copyVec3(newCameraTarget, camSrc[1]);
        moveVinY(camSrc[1], ci->cammat, -cameraRadius);

        // Left
        copyVec3(newCameraPos, camDst[2]);
        moveVinX(camDst[2], ci->cammat, cameraRadius);
        copyVec3(newCameraTarget, camSrc[2]);
        moveVinX(camSrc[2], ci->cammat, cameraRadius);

        // Right
        copyVec3(newCameraPos, camDst[3]);
        moveVinX(camDst[3], ci->cammat, -cameraRadius);
        copyVec3(newCameraTarget, camSrc[3]);
        moveVinX(camSrc[3], ci->cammat, -cameraRadius);

        // t2cam
        copyVec3(newCameraPos, camDst[4]);
        copyVec3(newCameraTarget, camSrc[4]);

        // Top right
        copyVec3(newCameraPos, camDst[5]);
        moveVinY(camDst[5], ci->cammat, cameraRadius);
        moveVinX(camDst[5], ci->cammat, -cameraRadius);
        copyVec3(newCameraTarget, camSrc[5]);
        moveVinY(camSrc[5], ci->cammat, cameraRadius);
        moveVinX(camSrc[5], ci->cammat, -cameraRadius);

        // Top left
        copyVec3(newCameraPos, camDst[6]);
        moveVinY(camDst[6], ci->cammat, cameraRadius);
        moveVinX(camDst[6], ci->cammat, cameraRadius);
        copyVec3(newCameraTarget, camSrc[6]);
        moveVinY(camSrc[6], ci->cammat, cameraRadius);
        moveVinX(camSrc[6], ci->cammat, cameraRadius);

        // Bottom right
        copyVec3(newCameraPos, camDst[7]);
        moveVinY(camDst[7], ci->cammat, -cameraRadius);
        moveVinX(camDst[7], ci->cammat, -cameraRadius);
        copyVec3(newCameraTarget, camSrc[7]);
        moveVinY(camSrc[7], ci->cammat, -cameraRadius);
        moveVinX(camSrc[7], ci->cammat, -cameraRadius);

        // Bottom left
        copyVec3(newCameraPos, camDst[8]);
        moveVinY(camDst[8], ci->cammat, -cameraRadius);
        moveVinX(camDst[8], ci->cammat, cameraRadius);
        copyVec3(newCameraTarget, camSrc[8]);
        moveVinY(camSrc[8], ci->cammat, -cameraRadius);
        moveVinX(camSrc[8], ci->cammat, cameraRadius);

        // Check points around the camera for collision
        for(i = 0; i < camRadDirections; i++){
            camRadColl[i].tri_callback = cameraCollisionCallback;
        }
        for(i = 0; i < camRadDirections; i++){
            if(collGrid(0,camSrc[i],camDst[i],&camRadColl[i], 0, COLL_PORTAL | COLL_DISTFROMSTART | COLL_TRICALLBACK)){
                addVec3(camRadColl[i].mat[3], camRadColl[i].mat[1], camRadColl[i].mat[3]);
                camRadCollInfo |= 1 << i;
            }
        }
    }
    // If there are any collisions...
    if(camRadCollInfo){
        int initCol = 1;
        int radCursor = 1;
        *instantPullPush = 1;

        // Check the various rays that were shot from the cam target to cam position
        for(i = 0; i < camRadDirections; i++){
            // If the current ray is reported to have collided against something...
            if(camRadCollInfo & radCursor){
                float tempDist;
                float scale;

                // Find a distance to place the camera to avoid the collision...
                subVec3(camRadColl[i].mat[3], newCameraTarget, dv);
                tempDist = normalVec3(dv);

                scale = (dist - tempDist)/dist;

                if(tempDist < 0.5)
                    tempDist = 0.5;

                if(tempDist < dist)
                    dist = tempDist;
            }

            // move cursor to the next bit...
            radCursor = radCursor << 1;
        }
    }    

    return dist;
}

typedef struct CamRay 
{
    Vec3 headPos;
    Vec3 camPos;
    CollInfo coll;
    F32 camZNeededToAvoidThis;
    int hit;
} CamRay;

int camRaySort( const CamRay * f1, const CamRay * f2 )
{
    if( f1->camZNeededToAvoidThis < f2->camZNeededToAvoidThis ) return -1;

    if( f1->camZNeededToAvoidThis == f2->camZNeededToAvoidThis ) return ((f1 > f2) ? 1 : 0);

    return 1;
}

// Tweakable Camera collision variables    
#define LENGTH_EXCLUSION_CYLINDER 5.0    //If, by freak geometry, something still manages to slip inside this cylinder in front 
#define RADIUS_EXCLUSION_CYLINDER 0.5    //of the camera, reject this camera dist
#define TOTAL_CAM_RAYS 30                //Cast this many rays
#define MAX_BLOCKERS 23
#define MAX_BLOCKERS_TO_MOVE_BACK 6
#define CAMERA_RADIUS_AT_HEAD 1.8       //Radius of ray circle at player's head
#define CAMERA_RADIUS_AT_CAMERA 1.8     //radius of ray circle at camera (same now as radius at head -- we're using a cylinder
#define JUMP_FORWARD_TO_STAY_OUT_OF_TROUBLE 1.0  //move up this much from a ray's collision point toward the head
#define AMOUNT_TO_SHORTEN_RAYS 4.0  //since we're using a cylinder now, the rays are around the camera instead of converging on them, soth

float findProperZDistanceNew( CameraInfo * ci, Vec3 newCameraTarget, F32 maxZDist, int * instantPullPush, F32 goodSpots[], int goodSpotCount )
{  
    int i;
    F32 dist = -1000; //return value (-1000=debug val)
    CamRay camRays[TOTAL_CAM_RAYS] = {0};    
    Entity *player = playerPtr();

    PERFINFO_AUTO_START("CalcRays",1);  
    // Calculate the rays around the camera
    {
        int i, s;
        F32 m, rad, rdm, xmove, ymove; 
        Vec3 newCameraPos;
 
        // Find newCameraPos (the camera pulled to the first candidate spot
        copyVec3(newCameraTarget, newCameraPos);       
        moveVinZ(newCameraPos, ci->cammat, goodSpots[ 0 ]);

        //Cone of collision
        for( i = 0 ; i < TOTAL_CAM_RAYS - 1 ; i++ )       
        {
            CamRay * camRay = &camRays[i];

            //This makes the rays spread inside the area of the camera circle
            s = i+37;   
            rdm = ABS( qfrandFromSeed( &s ) );  
            rdm = 1.0 - rdm * rdm * rdm ; 
 
            //Find rays
            m = (F32)(i*360) / (F32)(TOTAL_CAM_RAYS-1);

            rad = CAMERA_RADIUS_AT_CAMERA * rdm;
            ymove = rad * sin(RAD(m));
            xmove = rad * cos(RAD(m));

            copyVec3(newCameraPos, camRay->camPos);
            moveVinY( camRay->camPos, ci->cammat, ymove );
            moveVinX( camRay->camPos, ci->cammat, xmove );

            rad = CAMERA_RADIUS_AT_HEAD * rdm;
            copyVec3( newCameraTarget, camRay->headPos );
            moveVinY( camRay->headPos, ci->cammat, ymove );
            moveVinX( camRay->headPos, ci->cammat, xmove );
        }

        //Cast last one down the middle
        copyVec3(newCameraPos, camRays[TOTAL_CAM_RAYS-1].camPos );  
        copyVec3(newCameraTarget, camRays[TOTAL_CAM_RAYS-1].headPos );
    }

    PERFINFO_AUTO_STOP_START("CollGrids",1);
    // Find collision distances of each ray, and sort rays by distance of collision from head 
    {
        Mat4 camInv;
        Mat4 cammat;
        int needsBackFaceCheck; 

        //Find camInv -- inverted cammat at head
        copyMat4( ci->cammat, cammat );  
        copyVec3( newCameraTarget, cammat[3] );
        invertMat4Copy( cammat, camInv ); 

        //Get the disc around the head the rays emanate from.  Check that for obstruction. 
        //If any,
        needsBackFaceCheck = 1;
        
        for( i = 0 ; i < TOTAL_CAM_RAYS ; i++ )        
        {
            CamRay * camRay = &camRays[i];
            Vec3 dv; 
            bool insideAnObject = false;

            //If this line begins inside an object, it is automatically considered blocked at the source.
            if( needsBackFaceCheck )
            {
                CollInfo coll = {0};
                if( collGrid( 0, camRay->headPos, newCameraTarget, &coll, 0, COLL_PORTAL | COLL_CAMERA | COLL_BOTHSIDES ) )
                {
                    if( coll.backside )
                    {
                        insideAnObject = true;
                        camRay->camZNeededToAvoidThis = 0;

                        if( game_state.cameraDebug & 2 )
                            drawLine3DWidth( camRay->headPos, camRay->camPos, 0xffff5555, 2);
                    }
                }

            }

            //Otherwise find camZNeededToAvoidThis
            if( !insideAnObject )
            {
                if( collGrid( 0, camRay->headPos, camRay->camPos, &camRay->coll, 0, COLL_PORTAL | COLL_DISTFROMSTART | COLL_CAMERA ) )
                {
                    camRay->hit = 1;

                    // Find a place for the camera to avoid the collision...
                    mulVecMat4( camRay->coll.mat[3], camInv, dv ); //
                    camRay->camZNeededToAvoidThis = dv[2]; // Correctly positive since cam looks down negative Z

                    if( game_state.cameraDebug & 2 )
                        drawLine3DWidth( camRay->headPos, camRay->camPos, 0xf0000000, 2);
                }
                else
                {    
                    camRay->camZNeededToAvoidThis = goodSpots[0];

                    if( game_state.cameraDebug & 2 )
                        drawLine3DWidth( camRay->headPos, camRay->camPos, 0xffffffff, 2);
                }
            }
        }
        

        qsort( camRays, TOTAL_CAM_RAYS, sizeof( CamRay ), camRaySort );  
    }

    PERFINFO_AUTO_STOP_START("FindBest",1);

    //Check Rays against good spots
    for( i = 0 ; i < goodSpotCount ; i++ )    
    {
        dist = goodSpots[i] - JUMP_FORWARD_TO_STAY_OUT_OF_TROUBLE;
        if( dist - AMOUNT_TO_SHORTEN_RAYS < camRays[ MAX_BLOCKERS ].camZNeededToAvoidThis )    
            break;
    }
    
    //A little play in the system: If there's a spot close by that's good, and there are still some blocked rays, choose the close spot
    if( !ci->triedToScrollBackThisFrame ) //don't do this if the mouse wheel just ticked back, so you don't get stuck
    { 
        for( i = 0 ; i < goodSpotCount ; i++ )    
        {
            F32 closeGoodSpot = goodSpots[i] - JUMP_FORWARD_TO_STAY_OUT_OF_TROUBLE;

            //If it's closer to the camera than the spot you found
            if( dist > closeGoodSpot )
            {
                if( ABS( ci->zDistImMovingToward - closeGoodSpot ) < 2.0 ) //And it's close to where you wanted to be last frame
                {
                    //and the spot you found still has a fair number of blockers
                    if( dist - AMOUNT_TO_SHORTEN_RAYS > camRays[ MAX_BLOCKERS_TO_MOVE_BACK ].camZNeededToAvoidThis ) 
                    {
                        dist = closeGoodSpot;
                        break;
                    }
                }
            }
        }
    }

    //If the camera got moved forward by a collision, turn off smoothing, and jump forward
    if( dist < ci->zDistImMovingToward && dist < maxZDist - JUMP_FORWARD_TO_STAY_OUT_OF_TROUBLE - 0.001 ) //cuz floating point is a cruel mistress
        *instantPullPush = 1; 

    //finally if the camera tells you to move back between 1 and 4 feet because the good spot has changed, don't bother
    //TO DO 
 
    assert( dist > -1.1 );
    dist = MAX( 0, dist ); 

    //Debug
    if( game_state.cameraDebug & 4 )    
    {
        for( i = 0 ; i < TOTAL_CAM_RAYS ; i++ )     
        {
            CamRay * camRay = &camRays[i];
            if( i == MAX_BLOCKERS )
                xyprintfcolor( 20, 30+i, 150, 150, 255, "%d %f %d", i, camRays[ i ].camZNeededToAvoidThis, camRay->hit );
            else if( i == MAX_BLOCKERS_TO_MOVE_BACK )
                xyprintfcolor( 20, 30+i, 100, 200, 255, "%d %f %d", i, camRays[ i ].camZNeededToAvoidThis, camRay->hit );
            else
                xyprintf( 20, 30+i, "%d %f %d", i, camRays[ i ].camZNeededToAvoidThis, camRay->hit );
        }

        for( i = 0 ; i < goodSpotCount ; i++ )     
        {
            Vec3 x, y;
            F32 goodSpot = goodSpots[i];
            F32 goodSpotUsed = goodSpots[i] - JUMP_FORWARD_TO_STAY_OUT_OF_TROUBLE;
            copyVec3( newCameraTarget, x );
            copyVec3( newCameraTarget, y );
            moveVinZ(x, ci->cammat, goodSpot ); 
            moveVinZ(y, ci->cammat, goodSpotUsed ); 

            drawLine3DWidth( x, y, 0xf000ff00, 10);

            if( dist == goodSpotUsed )
                xyprintfcolor( 20, 20+i, 255, 100, 100, "%d %f Good Spot", i, goodSpotUsed );
            else
                xyprintfcolor( 20, 20+i, 255, 255, 255, "%d %f Good Spot", i, goodSpotUsed );
        }
    } //End Debug

    PERFINFO_AUTO_STOP();

    return dist;
}

static void swapInt (int * a, int * b )
{    
    int t;
    t=*a;
    *a=*b;
    *b=t;
}

static Vec3 splatVertsX[MAX_SHADOW_TRIS*3];

int sortTrisByNearestZ( const Triangle * t1, const Triangle * t2 )
{
    if( (splatVertsX[ t1->index[0] ])[2] < (splatVertsX[ t2->index[0] ])[2] ) 
        return -1;

    if( (splatVertsX[ t1->index[0] ])[2] == (splatVertsX[ t2->index[0] ])[2] )
        return ((t1 > t2) ? 1 : 0);

    return 1;
}

int findGoodSpots( CameraInfo * ci, F32 maxZDist, Vec3 newCameraTarget, F32 goodSpots[] )
{
    Mat4 cammat;
    Mat4 camInv;
    F32 zCurr;
    int goodSpotCnt = 0;
    Triangle newTris[MAX_SHADOW_TRIS];
    int i;
    Splat * splat = ci->simpleShadow->splat;

    //Xform all verts into camera space
    copyMat4( ci->cammat, cammat );
    copyVec3(newCameraTarget, cammat[3]);    
    moveVinZ(cammat[3], cammat, maxZDist);

    invertMat4Copy( cammat, camInv );

    for( i = 0 ; i < splat->vertCnt ; i++ )
    {
        mulVecMat4( splat->verts[i], camInv, splatVertsX[i] );
        splatVertsX[i][2] *= -1; //I did all the calculations as though looking down +z, not -z
    }

    //Build new array with points in each tri ordered smallest Z to largest Z
    memcpy( newTris, splat->tris, sizeof( Triangle ) * splat->triCnt );

    for( i = 0 ; i < splat->triCnt ; i++ )
    {
        Triangle * tri = &newTris[i];

        if( ( splatVertsX[ tri->index[0] ] )[2] > (splatVertsX[ tri->index[1] ])[2] )
            swapInt( &tri->index[0], &tri->index[1] );

        if( ( splatVertsX[ tri->index[1] ] )[2] > (splatVertsX[ tri->index[2] ])[2] )
            swapInt( &tri->index[1], &tri->index[2] );

        if( ( splatVertsX[ tri->index[0] ] )[2] > (splatVertsX[ tri->index[1] ])[2] )
            swapInt( &tri->index[0], &tri->index[1] );
    }

    //Sort all tris by each tri's nearest z (now the first index)
    qsort( newTris, splat->triCnt, sizeof( Triangle ), sortTrisByNearestZ );

    //Use the new sorted array to find all the spots with at least FREE_SPACE_LENGTH space with no tris
    zCurr = 0;
    for( i = 0 ; i < splat->triCnt ; i++ )
    {
        Triangle * tri = &newTris[i];
        F32 zNear      = ( splatVertsX[ tri->index[0] ] )[2];
        F32 zFar       = ( splatVertsX[ tri->index[2] ] )[2];

        if( zCurr + LENGTH_EXCLUSION_CYLINDER < zNear )
            goodSpots[ goodSpotCnt++ ] = maxZDist - zCurr;

        zCurr = MAX( zCurr, zFar);
    }

    //Get the last good spot
    goodSpots[ goodSpotCnt++ ] = maxZDist - zCurr;

    return goodSpotCnt;
}


void updateCameraSplat( CameraInfo * ci, F32 maxZDist, Vec3 newCameraTarget )
{
    static SplatParams sp;
    SplatParams * splatParams;
    Vec3 newCameraPos;
    Vec3 dir;
    Mat3 mat;

    splatParams = &sp;  

    //Make sure you have a good node   
    if( !gfxTreeNodeIsValid( ci->simpleShadow, ci->simpleShadowUniqueId ) )
        ci->simpleShadow = initSplatNode( &ci->simpleShadowUniqueId, "white", "white", 0 );

    splatParams->node = ci->simpleShadow;   
 
    copyMat3( ci->cammat, mat );   
 
    splatParams->shadowSize[0] = RADIUS_EXCLUSION_CYLINDER;  //looks wrong, but is right, because shadowSize is actually 2x in X and Z, for obscure reasons    
    splatParams->shadowSize[1] = maxZDist;
    splatParams->shadowSize[2] = RADIUS_EXCLUSION_CYLINDER; //looks wrong, see above
    splatParams->maxAlpha = 100;
     
    copyVec3(newCameraTarget, newCameraPos);    
    moveVinZ(newCameraPos, mat, maxZDist);
    copyVec3( newCameraPos, splatParams->projectionStart );   //Get Projection Start   
    copyMat3( mat, splatParams->mat ); 
    pitchMat3( RAD(-90), splatParams->mat ); 
    copyVec3( newCameraPos, splatParams->mat[3] );

    subVec3( newCameraPos, newCameraTarget, dir ); 
    normalVec3( dir );
    copyVec3( dir, splatParams->projectionDirection );

    //Debug
    splatParams->projectionDirection[0] = 0;
    splatParams->projectionDirection[1] = -1; 
    splatParams->projectionDirection[2] = 0;
    //End Debug

    splatParams->max_density = 1000.0; //TO DO make none 

    splatParams->rgb[0] = 0;  
    splatParams->rgb[1] = 0;
    splatParams->rgb[2] = 0;

    splatParams->falloffType = SPLAT_FALLOFF_NONE;
    splatParams->flags |= SPLAT_CAMERA;

    updateASplat( splatParams ); 
      
    if( game_state.cameraDebug & 1 )
    {
        updateSplatTextureAndColors(splatParams, 2.0f * splatParams->shadowSize[0], 2.0f * splatParams->shadowSize[2]);
    }
    else
    {
        splatParams->node->alpha = 0;
        ((Splat*)splatParams->node->splat)->drawMe = 0; //If the splat wasn't updated, don't draw it. 
        return;
    }
}


// Note that this function should only be used on one camera only since
// static variables are used to store some camera states.
static void camThirdPerson(    CameraInfo *ci )
{
#define    FIGHT_ZDIST        ( 2.f )
    Vec3    plr;
    Vec3 newCameraTarget;
    int instantPullPush = 0;
    Vec3 debugTemp;

PERFINFO_AUTO_START("Top",1);

    // Player model should be visible in 3rd person mode.  Do not hide it.
    playerHide(0); 

    // Extract player position and camera orientation + position from ci
    copyVec3(ci->mat[3], plr);

    
    /*********************************
     * Rotate camera
     */
    
    {
        int i;
        Vec3 camPYRScale;
        Vec3 camAirPYRScale = {0.6, 0.6, 1};
        Vec3 camGroundPYRScale = {0.6, 0.6, 1};

        if(ci->rotate_scale < 1){
            ci->rotate_scale *= 1 + 0.08 * camTimeStep();
        }
        
        if(ci->rotate_scale < 0.0001)
            ci->rotate_scale = 0.001;
        else if(ci->rotate_scale > 1)
            ci->rotate_scale = 1;
            
        if(control_state.server_state->fly){
            copyVec3(camAirPYRScale, camPYRScale);
            camPYRScale[1] = camAirPYRScale[1] * ci->rotate_scale;
        }else{
            copyVec3(camGroundPYRScale, camPYRScale);
            camPYRScale[1] = camGroundPYRScale[1] * ci->rotate_scale;
        }

        for(i = 0; i < 3; i++){
            // How much can the camera pyr actually change according to the
            // camera pyr velocity?
            float idealChange;
            float allowableChange;
            float changeRate;

            // What's the ideal change for this timestep?
            idealChange = subAngle(ci->targetPYR[i], ci->pyr[i]);

            // What's the camera rotational change rate allowed for this time step?
            changeRate = camPYRScale[i] * min(camTimeStep(), 1);
            changeRate = min(1, changeRate);

            // What's the actual cam rotational allowable change?
            allowableChange = idealChange * changeRate;

            // Apply the calculated change.
            ci->pyr[i] = addAngle(ci->pyr[i], allowableChange);
        }

        //printf("made:\t%1.2f\t%1.2f\t%1.2f\n", ci->pyr[0], ci->pyr[1], ci->pyr[2]);

        // Produce the new, rotated camera matrix
        createMat3YPR(ci->cammat, ci->pyr);
    }
  
    game_state.fov = game_state.fov_3rd;  
    copyVec3(plr, newCameraTarget);  
    newCameraTarget[1] += camGetPlayerHeight(); 
  
    copyVec3( newCameraTarget, debugTemp );       

PERFINFO_AUTO_STOP_START("YSpring",1);

    //Cushion Y changes  
    if( 0 && !(game_state.cameraDebug & 8) )        
    {
#define MAX_SPRING_LENGTH 1.0
#define SPRING_STRENGTH 0.03

        F32 tgt, curr, realDelta, idealDelta, numStepsF32;
        F32 fullGap, stepGap, newCurr, stepTgt;
        int i, numSteps;
         
        tgt        = newCameraTarget[1];
        curr    = ci->CameraYLastFrame;
        fullGap = tgt - curr;  
        numStepsF32 = 50.0 * TIMESTEP; //One step every 1/300th of a second 

        numSteps = (int)numStepsF32;
        stepGap = fullGap/numStepsF32;
        newCurr = 0;     
        
        for( i = 0 ; i <= numSteps; i++ )     
        {
            stepTgt = stepGap*(i+1);

            idealDelta = stepTgt - newCurr; 
            realDelta = idealDelta * SPRING_STRENGTH; 

            //MAX_SPRING_LENGTH
            if( ABS( (idealDelta-realDelta) ) > MAX_SPRING_LENGTH )
            {
                if( realDelta > 0 )
                    realDelta = idealDelta-MAX_SPRING_LENGTH;
                else
                    realDelta = idealDelta+MAX_SPRING_LENGTH;
            }

            //for the last one, 
            if( i == numSteps )
            {
                F32 pct = ( numStepsF32 - (F32)numSteps ); 
                realDelta *= pct;
            }
            newCurr += realDelta;
        }
        
        newCameraTarget[1] = curr + newCurr;    
  
        ci->CameraYLastFrame = newCameraTarget[1];

        if( game_state.cameraDebug & 1 ) 
        {
            xyprintf( 30, 28, "   TIMESTEP %f", TIMESTEP ); 
            xyprintf( 30, 29, "   numSteps %d", numSteps ); 
            xyprintf( 30, 30, "    fullGap %f", fullGap );  
            xyprintf( 30, 31, " bridgedGap %f", tgt - newCameraTarget[1] );
        }
    }

    if( game_state.cameraDebug & 1 ) 
    {
        Vec3 t1,t2;

        //Top of Head Bar
        copyVec3( debugTemp, t1 ); 
        copyVec3( debugTemp, t2 );  
        moveVinX( t1, ci->cammat, 0.25 );
        moveVinX( t2, ci->cammat, -0.25 );
        drawLine3DWidth( t1, t2, 0xff7070ff, 3 );

        //Actual Cam Pos Bar
        copyVec3( newCameraTarget, t1 ); 
        copyVec3( newCameraTarget, t2 );  
        moveVinX( t1, ci->cammat, 0.15 );
        moveVinX( t2, ci->cammat, -0.15 );
        drawLine3DWidth( t1, t2, 0xf77000f7, 3 );

        //Connect   
        drawLine3DWidth( debugTemp, newCameraTarget, 0xffffffff, 3 );
    }

PERFINFO_AUTO_STOP_START("DistFind",1);

    // Move camera  
    if( game_state.cameraDebug & 32 )
    {
        ci->zDistImMovingToward = findProperZDistanceOld( ci, newCameraTarget, game_state.camdist, &instantPullPush );
    }
    else
    {
        int goodSpotCnt;
        F32 goodSpots[50];

        PERFINFO_AUTO_START("updateCameraSplat",1);
        updateCameraSplat( ci, game_state.camdist, newCameraTarget );
        PERFINFO_AUTO_STOP_START("findGoodSpots",1);
        goodSpotCnt = findGoodSpots( ci, game_state.camdist, newCameraTarget, goodSpots );
        PERFINFO_AUTO_STOP_START("findProperZDistanceNew",1);
        ci->zDistImMovingToward = findProperZDistanceNew( ci, newCameraTarget, game_state.camdist, &instantPullPush, goodSpots, goodSpotCnt );
        PERFINFO_AUTO_STOP();
    }

PERFINFO_AUTO_STOP_START("Bottom",1);

    //Ignore all the stuff you did above if you are editing a base
    if( baseedit_Mode() )   
        ci->zDistImMovingToward = game_state.camdist; 

    {
        Vec3 newCameraPos;
        instantPullPush = instantPullPush && ci->zDistImMovingToward < ci->lastZDist; 
        ci->camDistLastFrame = game_state.camdist;
        ci->triedToScrollBackThisFrame = 0;
        camPushPull(ci, ci->zDistImMovingToward, instantPullPush); //sets ci->lastZDist from dist with lag (unless instant)
        copyVec3(newCameraTarget, newCameraPos); 
        moveVinZ(newCameraPos, ci->cammat, ci->lastZDist);
        camSetAlpha(ci);

        //If the camera went insane, then pretend like nothing happened...nice
        {
            int    i;
            for(i=0;i<3;i++)
            {
                if (!(newCameraPos[i]<=0) && !(newCameraPos[i]>=0))
                    newCameraTarget[i] = newCameraPos[i] = ci->cammat[3][i];
                if (newCameraPos[i]<-1000000 || newCameraPos[i]>1000000)
                    newCameraTarget[i] = newCameraPos[i] = ci->cammat[3][i];
            }
        }     
        copyVec3(newCameraPos,ci->cammat[3]);    
    }
   
    if( game_state.cameraDebug & 16 )  
    {       
        Mat4 mat;
        Vec3 pos = { -20, 20, -30 };
        Vec3 dir;

        copyMat4( ENTMAT(playerPtr()), mat );

        if( !vec3IsZero( game_state.cameraDebugPos ) )
            copyVec3( game_state.cameraDebugPos, pos );

        copyMat4( mat, ci->cammat); 

        addVec3( ci->cammat[3], pos, ci->cammat[3] );

        subVec3( ci->cammat[3], mat[3], dir );
        normalVec3( dir );
        orientMat3( ci->cammat, dir );  //Look at he player
    }

    if(0)
    { 
        Vec3 c,x,y,z;
  
        //Player
        copyVec3( ENTPOS(playerPtr()), c );
        copyVec3( ENTPOS(playerPtr()), x );
        copyVec3( ENTPOS(playerPtr()), y );
        copyVec3( ENTPOS(playerPtr()), z );
        moveVinX( x, ENTMAT(playerPtr()), 0.8);
        moveVinY( y, ENTMAT(playerPtr()), 0.8);
        moveVinZ( z, ENTMAT(playerPtr()), 0.8);
 
        drawLine3DWidth( c, x, 0xffffffff, 5 ); //X White
        drawLine3DWidth( c, y, 0xff777777, 5 ); //Y Light Gray
        drawLine3DWidth( c, z, 0xff333333, 5 ); //Z Dark Gray
 
        // World Coordinates 
        copyVec3( ENTPOS(playerPtr()), c );
        copyVec3( ENTPOS(playerPtr()), x );
        copyVec3( ENTPOS(playerPtr()), y );
        copyVec3( ENTPOS(playerPtr()), z );
        moveVinX( x, unitmat, 1.3);
        moveVinY( y, unitmat, 1.3);
        moveVinZ( z, unitmat, 1.3);
  
        drawLine3DWidth( c, x, 0xffffffff, 2 ); //X White
        drawLine3DWidth( c, y, 0xff777777, 2 ); //Y Light Gray
        drawLine3DWidth( c, z, 0xff333333, 2 ); //Z Dark Gray

        //cam_info.cammat
        copyVec3( newCameraTarget, c );
        copyVec3( newCameraTarget, x );
        copyVec3( newCameraTarget, y );
        copyVec3( newCameraTarget, z );
        moveVinX( x, ci->cammat, 1);
        moveVinY( y, ci->cammat, 1);
        moveVinZ( z, ci->cammat, 1);

        drawLine3DWidth( c, x, 0xffffffff, 2 ); //X White
        drawLine3DWidth( c, y, 0xff777777, 2 ); //Y Light Gray
        drawLine3DWidth( c, z, 0xff333333, 2 ); //Z Dark Gray
    }

PERFINFO_AUTO_STOP();

//    xyprintf(1, 13, "cam %f %f %f", newCameraPos[0], newCameraPos[1], newCameraPos[2]);
//    xyprintf(1, 14, "plr %f %f %f", ci->mat[3][0], ci->mat[3][1], ci->mat[3][2]);
//    xyprintf(1, 15, "ptarget %f %f %f", newCameraTarget[0], newCameraTarget[1], newCameraTarget[2]);
}

//Edit mode looking around
Vec3    view_pos;
F32        view_dist = 30;

static void look(ControlState *controls)
{
#define PITCH_SNAP RAD(10)
#define MPITCH_SNAP RAD(90)
extern int mouse_dx,mouse_dy;
F32        d;
#define DIST 1.f
Vec3    pyr;
int inverted = controls->editor_mouse_invert?-1:1;

    if(!mouse_dx && !mouse_dy)
        return;

    mouse_dy *= inverted;
    copyVec3(controls->pyr.cur,pyr);
    pyr[1] = addAngle(pyr[1], (F32)mouse_dx / 500.0F);
    if(controls->mlook)
    {
        pyr[0] = addAngle(pyr[0], (F32)mouse_dy / 500.0F);
        if (pyr[0] < -MPITCH_SNAP) pyr[0] = -MPITCH_SNAP;
        if (pyr[0] >  MPITCH_SNAP) pyr[0] =  MPITCH_SNAP;
    }
    else
    {
        d = subAngle(RAD(-2),pyr[0]);
        if (d > PITCH_SNAP) d = PITCH_SNAP;
        if (d <-PITCH_SNAP) d = -PITCH_SNAP;
        pyr[0] = addAngle(d,pyr[0]);
    }
    copyVec3(pyr,controls->pyr.cur);
    pyr[1] = addAngle(pyr[1], RAD(180));
    pyr[0] *= -1;
    camSetPyr(pyr);
    createMat3RYP(cam_info.mat,pyr);
    mouse_dy *= inverted;
}

//Edit mode looking around
static void camViewPoint(CameraInfo *ci)
{
Vec3    plr,eye,pos;
F32        dist,d, move;
int        i;
static F32 last_dist,last_ypos;
static Vec3 last_pos,ratios = { 1,1,1 }, max_snaps = { 7000,7000,7000 };
static int move_time;
F32        view_pan_x=0,view_pan_y=0,scale;
extern int mouse_dx,mouse_dy;

    if (!control_state.ignore)
    {
        if (edit_state.look)
            look(&control_state);
        scale = 0.3 * (sqrt(fabs(view_dist)) / 10);
        if (scale < 0.15)
            scale = 0.15;
        if (edit_state.mouseScale)
            scale *= edit_state.mouseScale;
        if (edit_state.zoom)
        {
            view_dist += mouse_dy * scale;
        }
        if (edit_state.pan)
        {
            view_pan_x = mouse_dx * scale;
            view_pan_y = mouse_dy * scale;
        }
    }

    playerHide(0);
    entSetAlpha( playerPtr(), 255, SET_BY_CAMERA );
    copyVec3(view_pos,ci->mat[3]);
    copyMat3(ci->mat,cam_info.cammat);
    copyVec3(ci->mat[3],pos); 
    game_state.fov = game_state.fov_1st;
    copyVec3(pos,plr);
    copyVec3(pos,eye);
    moveVinZ(eye,ci->cammat,view_dist);
    moveVinX(view_pos,ci->cammat,view_pan_x);
    moveVinY(view_pos,ci->cammat,view_pan_y);
    dist = view_dist;
    d = dist - last_dist;
    last_dist += d;
    moveVinZ(pos,ci->cammat,last_dist);
    d = (view_dist - last_dist);
    move = 4.f - (d/3);

    if (fabs(plr[0] - last_pos[0]) > max_snaps[0] || fabs(plr[0] - last_pos[0]) > max_snaps[2])
    {
        copyVec3(plr,last_pos);
        last_dist = dist;
    }
    else
    {
        for(i=0;i<3;i++)
        {
            last_pos[i] = last_pos[i] + (plr[i] - last_pos[i]) / ratios[i];
            d = plr[i] - last_pos[i];
            if ((!(last_pos[i] > 0) && !(last_pos[i] < 0)) || fabs(d) > max_snaps[i])
            {
                if (d < 0)
                    d = -max_snaps[i];
                else
                    d = max_snaps[i];
                last_pos[i] = plr[i] - d;
            }
        }
    }

    copyVec3(pos,ci->cammat[3]);
}

static void    camFirstPerson(CameraInfo *ci)
{
    F32                frame_time_30;
    static F32        roll;

    zeroVec3( control_state.cam_pyr_offset ); //Only place in camera.c control_state is set -- can we move it out?

    copyMat4(ci->mat,ci->cammat);
    createMat3RYP(ci->cammat,ci->targetPYR);
    ci->cammat[3][1] += camGetPlayerHeight();

    if(ci->lastZDist >= 0.05)
    {
        camPushPull(ci, 0, 0);
        moveVinZ(ci->cammat[3], ci->cammat, ci->lastZDist);
        camSetAlpha(ci);
    }
    else
    {
        playerHide(1);
    }

    copyVec3(ci->targetPYR,ci->pyr);
    frame_time_30 = camTimeStep();

    roll *= 1 - (0.3 * frame_time_30);

    roll = MINMAX(roll,-1,1);

    rollMat3(roll * 0.02,ci->cammat);
}

static void *sharedMemoryGetScratch(int size)
{
    static void *scratch = NULL;
    SharedMemoryHandle *shared_memory=NULL;
    SM_AcquireResult ret;
    if (!scratch)
    {
        ret = sharedMemoryAcquire(&shared_memory, "SCRATCH");
        if (ret==SMAR_DataAcquired) {
            scratch = sharedMemoryGetDataPtr(shared_memory);
        } else if (ret==SMAR_Error) {
            scratch = calloc(size,1);
        } else if (ret==SMAR_FirstCaller) {
            sharedMemorySetSize(shared_memory, size);
            scratch = sharedMemoryGetDataPtr(shared_memory);
            sharedMemoryUnlock(shared_memory);
        }
    }
    return scratch;
}

extern void camDetached(CameraInfo *ci); // camera_detached.c
void camUpdate()
{
    int        i;

    if(game_state.camera_shake) 
    {
        game_state.camera_shake -= game_state.camera_shake_falloff * camTimeStep();

        if(game_state.camera_shake < 0.01)
            game_state.camera_shake = 0;
    }
    if(game_state.camera_blur) 
    {
        game_state.camera_blur -= game_state.camera_blur_falloff * camTimeStep();

        if(game_state.camera_blur < 0.01)
            game_state.camera_blur = 0;
    }

    game_state.fov = game_state.fov_1st;
    cam_info.targetZDist = game_state.camdist;

    for(i=0;i<3;i++)
        cam_info.pyr[i] = fixAngle(cam_info.pyr[i]);

    if (cam_info.camIsLocked)
    {
        if(game_state.fov_custom)
            game_state.fov = game_state.fov_custom;
        return;
    }

    if (editMode())
    {
        camViewPoint(&cam_info);
    }
    else if ( getDebugCameraPosition(&cam_info ))
    {
        // Don't do anything.
    }
    else if (game_state.viewCutScene)
    {
        if (game_state.cutSceneLERPStart)
        {
            int total_time = game_state.cutSceneLERPEnd - game_state.cutSceneLERPStart;
            S64 t64 = timerMsecsSince2000();        //    use msecs so it doesn't have the crappy 1 sec framerate
            int t = (int)(t64 - game_state.cutSceneLERPStart*1000);
            float tf = t*0.001f/total_time;
            if (tf < 1)
            {
                F32 dof = (game_state.cutSceneDepthOfField1 * (1-tf)) + (game_state.cutSceneDepthOfField2 * tf);
                Vec3 pyr1;
                Vec3 pyr2;
                Vec3 currentPyr;

                getMat3YPR( game_state.cutSceneCameraMat1, pyr1 );
                getMat3YPR( game_state.cutSceneCameraMat2, pyr2 );
                interpPYR(tf, pyr1, pyr2, currentPyr);
                createMat3YPR(game_state.cutSceneCameraMat, currentPyr);

                //    ack.. who writes a lerp to have a be the end vector instead of the start vector =/
                lerpVec3(game_state.cutSceneCameraMat2[3], tf, game_state.cutSceneCameraMat1[3], game_state.cutSceneCameraMat[3]);

                if (dof)
                {
                    setCustomDepthOfField(DOFS_SNAP, dof, 0, dof*0.3, 1, dof*2, 1, 10000, &g_sun);
                }
            }
            else if (tf >= 1)
            {
                game_state.cutSceneLERPStart = game_state.cutSceneLERPEnd = 0;
                copyMat4(game_state.cutSceneCameraMat2, game_state.cutSceneCameraMat);
                game_state.cutSceneDepthOfField = game_state.cutSceneDepthOfField2;
                if (game_state.cutSceneDepthOfField)
                {
                    setCustomDepthOfField(DOFS_SNAP, game_state.cutSceneDepthOfField, 0, game_state.cutSceneDepthOfField*0.3, 1, game_state.cutSceneDepthOfField*2, 1, 10000, &g_sun);
                }
            }
        }
        copyMat4(game_state.cutSceneCameraMat, cam_info.cammat);
    }
    else if(control_state.detached_camera)
    {
        camDetached(&cam_info);
    }
    else if (!control_state.first && game_state.camdist)
    {
        PERFINFO_AUTO_START("camThirdPerson",1);
        camThirdPerson(&cam_info);
        PERFINFO_AUTO_STOP();
    }
    else
    {
        if(!control_state.first)
        {
            control_state.first = 1;
            sendThirdPerson();
        }
        game_state.fov = game_state.fov_1st;
        camFirstPerson(&cam_info);
    }

    if(game_state.fov_custom) //mm
    {
        game_state.fov = game_state.fov_custom;
    }
    
    if (game_state.camera_shared_master)
        memcpy(sharedMemoryGetScratch(sizeof(cam_info)), &cam_info, sizeof(cam_info));
    else if (game_state.camera_shared_slave)
        memcpy(&cam_info, sharedMemoryGetScratch(sizeof(cam_info)), sizeof(cam_info));
}


