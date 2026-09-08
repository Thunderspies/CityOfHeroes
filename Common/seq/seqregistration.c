#include <utilitieslib/stdtypes.h>
#include "seq/seqregistration.h"
#include <utilitieslib/utils/mathutil.h>
#include "seq/gfxtree.h"
#include "seq/seq.h"
#include "graphics/gfx.h"
#include "graphics/gfxDebug.h"
#include "graphics/font.h"
#include <float.h>
#include "gridcoll/gridcoll.h"
#include "player/player.h"
#include "entity/entity.h"

#ifdef CLIENT
#include "fxutil.h"
#include "UI/uiUtil.h" // debug only
#include "graphics/camera.h" // debug only
#include "cmdparse/cmdgame.h"
#endif

#include <utilitieslib/utils/timing.h>

#ifdef CLIENT
#    define FIX_ARM_REG game_state.fixArmReg
#else
#    define FIX_ARM_REG 1
#endif

typedef struct {float x,y,z,w;} SeqRegQuat;

static void srQuatToMat(const SeqRegQuat* q,Mat3 R)
{
    F32        tx, ty, tz, twx, twy, twz, txx, txy, txz, tyy, tyz, tzz;

    tx  = 2.f*q->x;
    ty  = 2.f*q->y;
    tz  = 2.f*q->z;
    twx = tx*q->w;
    twy = ty*q->w;
    twz = tz*q->w;
    txx = tx*q->x;
    txy = ty*q->x;
    txz = tz*q->x;
    tyy = ty*q->y;
    tyz = tz*q->y;
    tzz = tz*q->z;

    R[0][0] = 1.f-(tyy+tzz);
    R[0][1] = txy-twz;
    R[0][2] = txz+twy;
    R[1][0] = txy+twz;
    R[1][1] = 1.f-(txx+tzz);
    R[1][2] = tyz-twx;
    R[2][0] = txz-twy;
    R[2][1] = tyz+twx;
    R[2][2] = 1.f-(txx+tyy);
}


// given three sides of a triangle, compute all three angles. (Assume thetaX is angle opposite side x)
static void solveArmAngles(F32 a, F32 b, F32 c, F32 * thetaA, F32 * scale)
{
    // law of cosines
    F32 val = (SQR(a) - SQR(c) - SQR(b)) / (-2. * c * b);

    if(ABS(val) > 1.0f)
    {
        *thetaA = 0.0001;    // ZERO...
        *scale =  b / (a + c);
    }
    else
    {
        *thetaA = acosf(val);
        *scale = 1.0f;
    }
}

// this version restricts how far the arms will bend, & scales to make up the difference
#define MAX_DELTA (10 * (PI / 180.))
static F32 solveArmAngles2(F32 a, F32 b, F32 c, F32 origThetaB, F32 * thetaA, F32 * scale)
{
    // DEBUG -- trying to scale arms to prevent stretching them all the way out

    F32 sinA;
    // law of cosines
    F32 cosB = (SQR(b) - SQR(c) - SQR(a)) / (-2. * c * a);

    F32 thetaB = acosf(MINMAX(cosB, -1, 1));

#if CLIENT
     if(game_state.testInt1 && thetaB > (origThetaB + MAX_DELTA))
    {
        thetaB = origThetaB + MAX_DELTA;        
    }
     else if(game_state.testInt1 && thetaB < (origThetaB - MAX_DELTA))
    {
        thetaB = origThetaB - MAX_DELTA;
    }
    else
#endif
    {
        // use law of sines to get angle A
        sinA = sinf( thetaB ) * a / b;
        *thetaA = asinf(MINMAX(sinA,-1,1));
        *scale = 1.0f;
        return thetaB;
    }

      *scale = b / sqrtf((SQR(a) + SQR(c) - (2 * a * c * cosf(thetaB))));
     sinA = sinf( thetaB ) * (a * (*scale)) / b;
     *thetaA = asinf(MINMAX(sinA,-1,1));

    return thetaB;
}


void unscaleBone(GfxNode * node, F32 x, F32 y, F32 z)
{
    Mat4 mat;
    Mat4 final;
    Vec3 vec;

    setVec3(vec, x, y, z);

    copyMat4( unitmat, mat ); 
    scaleMat3Vec3(mat, vec);
    mulMat3(  mat, node->mat,   final);
    copyMat3(final, node->mat);
}

void scaleBone(GfxNode * node, F32 x, F32 y, F32 z)
{
    Mat4 mat;
    Mat4 final;
    Vec3 vec;

    setVec3(vec, x, y, z);

    copyMat4( unitmat, mat ); 
    scaleMat3Vec3(mat, vec);
    mulMat3(node->mat, mat, final);
    copyMat3(final, node->mat);
}


static F32 srQuatNorm(SeqRegQuat * q)
{
     F32 val = SQR(q->w) + SQR(q->x) + SQR(q->y) + SQR(q->z);
    return val;
}


static void normalizeSRQuat(SeqRegQuat * q)
{
    F32 n = sqrt(srQuatNorm(q));
    q->x /= n;
    q->y /= n;
    q->z /= n;
    q->w /= n;
}




// multiplicative inverse
static void srQuatInv(SeqRegQuat * q, SeqRegQuat * res)
{
    // FUTURE OPTIM: can eliminate division if we know quat is already normalized
       F32 n  = srQuatNorm(q);
    res->w =    (q->w / n);  // FUTURE OPTIM: multiply by 1/n
    res->x =  - (q->x / n);
    res->y =  - (q->y / n);
    res->z =  - (q->z / n);
}

static void srQuatToVec3(SeqRegQuat * q, Vec3 v)
{
    v[0] = q->x;
    v[1] = q->y;
    v[2] = q->z;
    // do I need to divide by 'w' or something? or just assume normalized?
}



void mulSRQuat(SeqRegQuat * a, SeqRegQuat * b, SeqRegQuat * res)
{

    //An optimization can also be made by rearranging to
    //    w = w1w2 - x1x2 - y1y2 - z1z2
    //    x = w1x2 + x1w2 + y1z2 - z1y2
    //    y = w1y2 + y1w2 + z1x2 - x1z2
    //    z = w1z2 + z1w2 + x1y2 - y1x2

     res->w = (a->w * b->w) - (a->x * b->x) - (a->y * b->y) - (a->z * b->z); 

    res->x = (a->w * b->x) + (a->x * b->w) + (a->y * b->z) - (a->z * b->y);

     res->y = (a->w * b->y) + (a->y * b->w) + (a->z * b->x) - (a->x * b->z); 
    
    res->z = (a->w * b->z) + (a->z * b->w) + (a->x * b->y) - (a->y * b->x); 
}


//    r = Q * V * Q'
static void srQuatRotVec3(Vec3 v, SeqRegQuat * q, Vec3 r)
{
    SeqRegQuat inv;
    SeqRegQuat qv;
    SeqRegQuat temp;
    SeqRegQuat temp2;

     srQuatInv(q, &inv);

     qv.w = 0;
    qv.x = v[0];
    qv.y = v[1];
    qv.z = v[2];

    mulSRQuat(q, &qv, &temp);
    mulSRQuat(&temp, &inv, &temp2);

     srQuatToVec3(&temp2, r);
}


// THIS CAN BE OPTIMIZED:
//If v1 and v2 are already normalised then |v1||v2|=1 so,
//
//x = (v1 x v2).x
//y = (v1 x v2).y
//z = (v1 x v2).z
//w = 1 + v1•v2
//
//If v1 and v2 are not already normalised then multiply by |v1||v2| gives:
//
//x = (v1 x v2).x
//y = (v1 x v2).y
//z = (v1 x v2).z
//w = |v1||v2| + v1•v2
static void srQuatFromAxisAngle(SeqRegQuat * q, Vec3 axis, F32 theta)
{
    F32 theta_div_2 = theta/2.0f;
    F32 sin_theta; // = sinf(theta_div_2);
    mathutil_sincosf(theta_div_2, &sin_theta, &(q->w));

    normalVec3(axis);    // FIXME: shouldn't modify passed in vector without warning :(

     q->x = axis[0] * sin_theta;
    q->y = axis[1] * sin_theta;
    q->z = axis[2] * sin_theta;
     //q->w = cosf(theta_div_2);

    normalizeSRQuat(q);
}

#ifdef CLIENT
static void drawRelLineChest(SeqInst * seq, Vec3 va, Vec3 vb, UINT color)
{
    Mat4 mat, mat2, a, b, waistMat;
    F32 width = 5;

    GfxNode * node = seqFindGfxNodeGivenBoneNum(seq, BONEID_WAIST);
    gfxTreeFindWorldSpaceMat(waistMat, node);

    copyMat4(unitmat, a);
    copyMat4(unitmat, b);

    copyVec3(va, a[3]);
    copyVec3(vb, b[3]);

    mulMat4(waistMat, a, mat);     
    mulMat4(waistMat, b, mat2);

    if(color == CLR_BLACK)
        width *= 3;

    addDebugLine(mat[3], mat2[3],
        ((color >> 24) & 0xFF), 
        ((color >> 8 ) & 0xFF),
        ((color >> 16) & 0xFF),
        ((color      ) & 0xFF),
        width);  
}

drawWorldLine(Vec3 a, Vec3 b, UINT color, float width)
{
    addDebugLine(a, b,
        ((color >> 24) & 0xFF), 
        ((color >> 16) & 0xFF),
        ((color >> 8 ) & 0xFF),
        ((color      ) & 0xFF),
        width);  

}
#endif

static F32 getVec3Angle(Vec3 a, Vec3 b)
{
    // Solve for THETA -->  X . Y = |X| |Y| cos(theta)
    F32 val = dotVec3(a, b) / ( lengthVec3(a) * lengthVec3(b));
    return acosf( MINMAX(val, -1, 1) );
}

static F32 getSRQuatRotBetweenVectors(Mat3 mat, Vec3 a, Vec3 b, SeqRegQuat * q, SeqInst * seqDbg)
{
    Vec3 axis, temp;
    F32 theta;        
    Mat4 inv;

    theta = getVec3Angle(a, b);

       crossVec3(a, b, axis);
     normalVec3(axis);    
    
    // make axis relative to given mat
    invertMat4Copy(mat, inv);
    mulVecMat3(axis, inv, temp);
    copyVec3(temp, axis);
    
     srQuatFromAxisAngle(q, axis, -theta);

     return theta;
}

// #if CLIENT
// static void xyprintmat4(int x, int y, char * desc, Mat4 mat)
// {
//     int i;
//     xyprintf(x,y,"%s",desc);
//     for(i=0;i<4;i++)
//     {
//            xyprintf(x,y+i+1,"%.2f\t%.2f\t%.2f", mat[i][0], mat[i][1], mat[i][2]);
//     }
// }
// 
// static void xyprintQuat(int x, int y, char * desc, SeqRegQuat * q)
// {
//     Mat4 mat;
//     Vec3 pyr;
//     srQuatToMat(q, mat);
//     getMat3PYR(mat, pyr);
//     xyprintf(x,y,"%10s %.2f\t%.2f\t%.2f\t",desc, pyr[0],pyr[1],pyr[2]);
// }
// #endif

// the big daddy function - called after animation has been updated
static void s_adjustArm(SeqInst *seq, bool bRight, GfxNode *chestNode, GfxNode *shoulderNode,
                        GfxNode *armNode, GfxNode *elbowNode, GfxNode *wristNode, GfxNode *palmNode)
{
    Mat4 transformedShoulder, transformedArm, transformedElbow, transformedWrist, transformedPalm;
    Mat4 originalShoulder, originalArm, originalElbow, originalWrist, originalPalm;
    Mat4 matArmScale, matArmUnscale;
    Vec3 shoulderElbowVec, elbowWristVec, shoulderWristVec, elbowShoulderVec;
    Vec3 newShoulderElbowVec, newElbowWristVec, newElbowRelPos;
    Vec3 axis;
    F32 theta, scale, dbgThetaB;
    SeqRegQuat q;

    if(!chestNode || !shoulderNode || !armNode || !elbowNode || !wristNode)
        return;

    if(seq->armScale)
    {
        // set up scaling / unscaling matrices
        zeroMat4(matArmScale);
        matArmScale[0][0] = matArmScale[1][1] = matArmScale[2][2] = (1 + seq->armScale);
        zeroMat4(matArmUnscale);
        matArmUnscale[0][0] = matArmUnscale[1][1] = matArmUnscale[2][2] = (1.0f / (1 + seq->armScale));
    }

    // compute where joints WILL be w/ full scaling & translation)
    mulMat4(chestNode->mat,            shoulderNode->mat,    transformedShoulder);
    mulMat4(transformedShoulder,    armNode->mat,        transformedArm);
    mulMat4(transformedArm,            elbowNode->mat,        transformedElbow);
    mulMat4(transformedElbow,        wristNode->mat,        transformedWrist);
    if(seq->bFixWeaponReg)
        mulMat4(transformedWrist,    palmNode->mat,        transformedPalm);

    if(seq->shoulderScale)
    {
        // remove scaling on col (assume shoulder translation only)
        Mat4 matTemp;
        Vec3 scaleVec;

        copyMat4(shoulderNode->mat, matTemp);

        if(seq->shoulderScale > 0)
            scaleVec3(seq->type->shoulderScaleFat, ABS(seq->shoulderScale), scaleVec);
        else
            scaleVec3(seq->type->shoulderScaleSkinny, ABS(seq->shoulderScale), scaleVec);    
        if(!bRight)
            scaleVec[0] *= -1; //x axis is symmetrical
        subVec3(matTemp[3], scaleVec, matTemp[3]);

        mulMat4(chestNode->mat, matTemp, originalShoulder);
    }
    else
        mulMat4(chestNode->mat, shoulderNode->mat, originalShoulder);

    mulMat4(originalShoulder, armNode->mat, originalArm);

    if(seq->armScale)
    {
        // remove scaling on larm
        Mat4 matTemp;
        copyMat4(elbowNode->mat,    matTemp);
        mulMat3(elbowNode->mat,        matArmUnscale,    matTemp);
        mulMat4(originalArm,        matTemp,        originalElbow);
    }
    else
        mulMat4(originalArm, elbowNode->mat, originalElbow);

    mulMat4(originalElbow, wristNode->mat, originalWrist);

    if(seq->bFixWeaponReg)
    {
        if(seq->armScale)
        {
            Mat4 matTemp;
            copyMat4(palmNode->mat, matTemp);
            mulMat3(matArmScale,    palmNode->mat,    matTemp);
            mulMat4(originalWrist,    matTemp,        originalPalm);
        }
        else
            mulMat4(originalWrist, palmNode->mat, originalPalm);
    }

#if CLIENT
    // debug -- draw line from chest to destination point
    if(game_state.fixRegDbg)
    {
        Mat4 chestMat;
        Mat4 mat2, mat3, waistMat;

        GfxNode *node = seqFindGfxNodeGivenBoneNum(seq, BONEID_WAIST);
        gfxTreeFindWorldSpaceMat(waistMat, node);
        gfxTreeFindWorldSpaceMat(chestMat, chestNode);

        // draw where arm WOULD be without scaling AND without shoulder adjustment
        mulMat4(waistMat, originalShoulder, mat2); // now we're relative to the world
        addDebugLine(chestMat[3], mat2[3], 0, 255, 0, 255, 6);

        mulMat4(waistMat, originalArm, mat3);
        addDebugLine(mat2[3], mat3[3], 0, 255, 0, 255, 6);

        mulMat4(waistMat, originalElbow, mat2);
        addDebugLine(mat2[3], mat3[3], 0, 255, 0, 255, 6);

        mulMat4(waistMat, originalWrist, mat3);
        addDebugLine(mat2[3], mat3[3], 0, 255, 0, 255, 6);

        if(seq->bFixWeaponReg)
        {
            mulMat4(waistMat, originalPalm, mat2);
            addDebugLine(mat2[3], mat3[3], 0, 255, 0, 255, 6);
        }

        // draw where arm WOULD be without scaling, but with shoulder adjustment
        mulMat4(waistMat, transformedShoulder, mat2);
        addDebugLine(chestMat[3], mat2[3], 0, 255, 255, 255, 6);

        mulMat4(waistMat, transformedArm, mat3);
        addDebugLine(mat2[3], mat3[3], 0, 255, 255, 255, 6);

        mulMat4(waistMat, transformedElbow, mat2);
        addDebugLine(mat2[3], mat3[3], 0, 255, 255, 255, 6);

        mulMat4(waistMat, transformedWrist, mat3);
        addDebugLine(mat2[3], mat3[3], 0, 255, 255, 255, 6);

        if(seq->bFixWeaponReg)
        {
            mulMat4(waistMat, transformedPalm, mat2);
            addDebugLine(mat2[3], mat3[3], 0, 255, 255, 255, 6);
        }
    }
#endif

    if(!seq->bFixWeaponReg)
    {
        // adjust for interpolation (allows easing in and out of arm fixing)
        posInterp(seq->curr_fixInterp, transformedWrist[3], originalWrist[3], originalWrist[3]); 

        // compute new angles between joints
        subVec3(transformedArm[3], transformedElbow[3], elbowShoulderVec);
        subVec3(transformedWrist[3], transformedElbow[3], elbowWristVec);

        dbgThetaB = solveArmAngles2(
                        distance3(transformedElbow[3],    transformedWrist[3]),
                        distance3(transformedArm[3],    originalWrist[3]),        // this one must use the original (unscaled/translated pos, as intended in animation)
                        distance3(transformedArm[3],    transformedElbow[3]),
                        getVec3Angle(elbowWristVec, elbowShoulderVec),
                        &theta,                                                    // angle opposite elbow-wrist line
                        &scale);
    }
    else
    {
        // adjust for interpolation (allows easing in and out of arm fixing)
        posInterp(seq->curr_fixInterp, transformedPalm[3], originalPalm[3], originalPalm[3]); 

        // compute new angles between joints
        subVec3(transformedArm[3], transformedElbow[3], elbowShoulderVec);
        subVec3(transformedPalm[3], transformedElbow[3], elbowWristVec);

        dbgThetaB = solveArmAngles2(
                        distance3(transformedElbow[3],    transformedPalm[3]),
                        distance3(transformedArm[3],    originalPalm[3]),        // this one must use the original (unscaled/translated pos, as intended in animation)
                        distance3(transformedArm[3],    transformedElbow[3]),
                        getVec3Angle(elbowWristVec, elbowShoulderVec),
                        &theta,                                                    // angle opposite elbow-wrist line
                        &scale);
    }

    // scale arms -- necessary if arms are not long enough to reach target location // this'll be off for palm registration
    if(FIX_ARM_REG && scale != 1.0)
    {
        scaleBone(armNode, scale, 1, 1);
        unscaleBone(elbowNode, (1 / scale), 1, 1);
        scaleBone(elbowNode, scale, 1, 1);
        unscaleBone(wristNode, (1 / scale), 1, 1);

        // re-compute...
        mulMat4(transformedShoulder,    armNode->mat,    transformedArm);
        mulMat4(transformedArm,            elbowNode->mat,    transformedElbow);
        mulMat4(transformedElbow,        wristNode->mat,    transformedWrist);
        if(seq->bFixWeaponReg)
            mulMat4(transformedWrist,    palmNode->mat,    transformedPalm);
    }

#if CLIENT
    if(game_state.fixRegDbg && seq == playerPtr()->seq)
    {
        xyprintf(10,(bRight?16:15), "%s Theta: %.2f  Scale: %.2f  OrigTheta %.1f  NewTheta   %.1f   Diff %.1f   MaxDelta %.1f", 
                                    (bRight?"Right":" Left"), theta, scale, 
                                    getVec3Angle(elbowWristVec, elbowShoulderVec),
                                    dbgThetaB,
                                    getVec3Angle(elbowWristVec, elbowShoulderVec) - dbgThetaB,
                                    MAX_DELTA);
    }
#endif

    if(seq->bFixWeaponReg)
        subVec3(originalPalm[3],    transformedArm[3], shoulderWristVec);
    else
        subVec3(originalWrist[3],    transformedArm[3], shoulderWristVec);
    subVec3(transformedElbow[3],    transformedArm[3], shoulderElbowVec);

    //----- ADJUST SHOULDER -----------------------------------------------------------------------------------------------------------------

    // compute axis of rotation (IMPROVEME: for now, use cross product of new shoulder-wrist and new shoulder->elbow
    crossVec3(shoulderElbowVec, shoulderWristVec, axis);
    normalVec3(axis);

//     {
//         Vec3 a;
//         addVec3(axis, transformedArm[3], a);
//         if(game_state.fixRegDbg)
//             drawRelLineChest(seq, transformedArm[3], a, CLR_DARK_RED);
//     }

    srQuatFromAxisAngle(&q, axis, -theta);
    srQuatRotVec3(shoulderWristVec, &q, newShoulderElbowVec);

    {
        F32 lenA, lenB;
        lenA = lengthVec3(newShoulderElbowVec);
        lenB = lengthVec3(shoulderElbowVec);
        scaleVec3(newShoulderElbowVec, (lenB/lenA), newShoulderElbowVec);
        addVec3(newShoulderElbowVec, transformedArm[3], newElbowRelPos);  
#if CLIENT
        if(game_state.fixRegDbg)
            drawRelLineChest(seq, transformedArm[3], newElbowRelPos, CLR_ORANGE);
#endif
    }

    theta = getSRQuatRotBetweenVectors(transformedArm, shoulderElbowVec, newShoulderElbowVec, &q, seq);
#if CLIENT
    if(game_state.fixRegDbg)
        xyprintf(10,bRight?28:27, "%s Theta: %.2f\tScale: %.2f", (bRight?"Right":" Left"), theta, scale);
#endif

    if(FIX_ARM_REG)
    {
        Mat3 matRot, matTemp;
        srQuatToMat(&q, matRot);
        mulMat3(armNode->mat, matRot, matTemp);
        copyMat3(matTemp, armNode->mat);
    }

    //----- ADJUST ELBOW -----------------------------------------------------------------------------------------------------------------

    mulMat4(transformedShoulder,    armNode->mat,        transformedArm);    
    mulMat4(transformedArm,            elbowNode->mat,        transformedElbow);
    mulMat4(transformedElbow,        wristNode->mat,        transformedWrist);
    if(seq->bFixWeaponReg)
        mulMat4(transformedWrist,    palmNode->mat,        transformedPalm);

//     drawRelLineChest(seq, transformedElbow[3], transformedWrist[3], CLR_PARAGON);

    if(seq->bFixWeaponReg)
    {
        subVec3(transformedPalm[3], transformedElbow[3], elbowWristVec);
        subVec3(originalPalm[3], transformedElbow[3], newElbowWristVec);
    }
    else
    {
        subVec3(transformedWrist[3], transformedElbow[3], elbowWristVec);
        subVec3(originalWrist[3], transformedElbow[3], newElbowWristVec);
    }

//     {
//         Vec3 a,b,c,d,cross;
//         addVec3(transformedArm[3], newShoulderElbowVec, a);
//         addVec3(newElbowWristVec, a, b);
//         if(game_state.fixRegDbg)
//             drawRelLineChest(seq, a, b, CLR_ORANGE);
// 
//         copyVec3(elbowWristVec, c);
//         copyVec3(newElbowWristVec, d);
//         normalVec3(c);    // not sure if normalizing is necessary (but can't hurt)
//         normalVec3(d);
//         crossVec3(c, d, cross);
//         normalVec3(cross);
//         addVec3(cross, a, b);
//        drawRelLineChest(seq, a, b, CLR_DARK_RED);
//     }

    if(FIX_ARM_REG)
    {
        Mat3 matRot;
        theta = getSRQuatRotBetweenVectors(transformedElbow, elbowWristVec, newElbowWristVec, &q, seq);
        srQuatToMat(&q, matRot);

        if(seq->armScale)
        {
            // remove scaling, apply rotation, reapply scaling
            Mat3 matTempA, matTempB;
            mulMat3(elbowNode->mat,    matArmUnscale,    matTempA);
            mulMat3(matTempA,        matRot,            matTempB);
            mulMat3(matTempB,        matArmScale,    elbowNode->mat);
        }
        else
        {
            Mat3 matTemp;
            mulMat3(elbowNode->mat, matRot, matTemp);
            copyMat3(matTemp, elbowNode->mat);
        }
    }


    //----- ADJUST WRIST -----------------------------------------------------------------------------------------------------------------
    // (new wrist is same orientation as original wrist)
    if(FIX_ARM_REG)
    {
        Mat3 matInv, matRot;
        mulMat4(transformedArm,        elbowNode->mat,    transformedElbow);
        mulMat4(transformedElbow,    wristNode->mat,    transformedWrist);
        if(seq->bFixWeaponReg)
            mulMat4(transformedWrist,    palmNode->mat,        transformedPalm);

        invertMat3Copy(transformedWrist, matInv);
        mulMat3(matInv, originalWrist, matRot);

        if(seq->armScale)
        {
            Mat3 matTemp;
            mulMat3(wristNode->mat, matRot, matTemp);
            mulMat3(matTemp, matArmScale, wristNode->mat);
        }
        else
        {
            Mat3 matTemp;
            mulMat3(wristNode->mat, matRot, matTemp);
            copyMat3(matTemp, wristNode->mat);
        }
    }

#if CLIENT
    if(game_state.fixRegDbg)
    {
        drawRelLineChest(seq, transformedElbow[3], transformedWrist[3], CLR_ORANGE);
        if(seq->bFixWeaponReg)
            drawRelLineChest(seq, transformedWrist[3], transformedPalm[3], CLR_ORANGE);
    }
#endif
}

void adjustArms(SeqInst *seq)
{
    // do we even need to do correction?
#if CLIENT
    if(!game_state.fixRegDbg)
#endif
        if((ABS(seq->shoulderScale) <= 0.01 && !seq->armScale) || seq->curr_fixInterp <= 0.001)
            return;

    PERFINFO_AUTO_START("adjustArms",1);

#if CLIENT
    if(game_state.fixRegDbg && seq == playerPtr()->seq)
    {
        xyprintf(10, 5, "curr_fixInterp: %.2f", seq->curr_fixInterp);
        xyprintf(10, 6, "Timestep:       %.3f", TIMESTEP);
        xyprintf(10, 7, "Max Theta:      %s", (game_state.testInt1 ? "On" : "OFF"));
    }
#endif

    // left arm
    s_adjustArm(seq, false,
                seqFindGfxNodeGivenBoneNum(seq, BONEID_CHEST),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_COL_L),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_UARML),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_LARML),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_HANDL),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_WEPL));

    // right arm
    s_adjustArm(seq, true,
                seqFindGfxNodeGivenBoneNum(seq, BONEID_CHEST),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_COL_R),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_UARMR),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_LARMR),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_HANDR),
                seqFindGfxNodeGivenBoneNum(seq, BONEID_WEPR));

    PERFINFO_AUTO_STOP();
}

// unused leg registration
#ifdef CLIENT
static int findCollisionPt(Mat4 low, Mat4 hitMatOut)
{
    Vec3 start;
    Vec3 end;
    CollInfo coll  = {0};
    int hit;

    copyVec3( low[3], start );
    copyVec3( low[3], end );
    start[1] += 2.0; 
      end[1] -= 2.0;

     hit = collGrid(0, start, end, &coll, 0.15, COLL_CYLINDER | COLL_IGNOREINVISIBLE | COLL_DISTFROMSTART | COLL_BOTHSIDES);

    if(hit)
        copyMat4( coll.mat, hitMatOut );

    return hit;
}

// Shoot rays down to determine where feet should be. Then use simple inverse kinematics to determine
// how legs should be bent.
//(Only if you are on the ground)
void adjustLegs(SeqInst *seq)
{
//    GfxNode * waistNode        = seqFindGfxNodeGivenBoneNum(seq, BONEID_WAIST);

    GfxNode * hipNode        = seqFindGfxNodeGivenBoneNum(seq, BONEID_HIPS);

    GfxNode * upperNodeL    = seqFindGfxNodeGivenBoneNum(seq, BONEID_ULEGL);
    GfxNode * lowerNodeL    = seqFindGfxNodeGivenBoneNum(seq, BONEID_LLEGL);
    GfxNode * footNodeL        = seqFindGfxNodeGivenBoneNum(seq, BONEID_FOOTL);
    GfxNode * toeNodeL        = seqFindGfxNodeGivenBoneNum(seq, BONEID_TOEL);

    GfxNode * upperNodeR    = seqFindGfxNodeGivenBoneNum(seq, BONEID_ULEGR);
    GfxNode * lowerNodeR    = seqFindGfxNodeGivenBoneNum(seq, BONEID_LLEGR);
    GfxNode * footNodeR        = seqFindGfxNodeGivenBoneNum(seq, BONEID_FOOTR);
    GfxNode * toeNodeR        = seqFindGfxNodeGivenBoneNum(seq, BONEID_TOER);

    GfxNode * upperNode;
    GfxNode * lowerNode;
    GfxNode * footNode;
    GfxNode * toeNode;

    Mat4 hip;
    Mat4 upperL, lowerL, footL, toeL;
    Mat4 upperR, lowerR, footR, toeR;


    Mat4Ptr low, lowL, lowR;

    Mat4Ptr upper, lower, foot, toe;

    int hitL=0, hitR=0;
    F32 hitLDist, hitRDist;
    Mat4 hitPosL, hitPosR;
    Mat4Ptr hitPos;

    Vec3 vec, a;

    if(! (upperNodeL && upperNodeR && toeNodeR && toeNodeL))
        return;

    // everything is relative to the WORLD (due to need for collision check)
    mulMat4( seq->gfx_root->mat, hipNode->mat, hip);

    mulMat4(hip,    upperNodeL->mat,    upperL);
    mulMat4(upperL, lowerNodeL->mat,    lowerL);
    mulMat4(lowerL, footNodeR->mat,        footL);
    mulMat4(footL,    toeNodeR->mat,        toeL);

    mulMat4(hip,    upperNodeR->mat,    upperR);
    mulMat4(upperR, lowerNodeR->mat,    lowerR);
     mulMat4(lowerR, footNodeR->mat,        footR);
    mulMat4(footR,    toeNodeR->mat,        toeR);

    lowL =    (footL[3][1]) < (toeL[3][1]) ? footL : toeL;  
    lowR =    (footR[3][1]) < (toeR[3][1]) ? footR : toeR; 

    hitL = findCollisionPt(lowL, hitPosL);        
    if(hitL)
    {
        hitLDist = distance3(lowL[3], hitPosL[3]);
        if(lowL[3][1] < hitPosL[3][1])
            hitLDist = -hitLDist; // collision is above

        xyprintf(80, 20, "Left Dist:\t%.2f", hitLDist);
    }
    
    hitR = findCollisionPt(lowR, hitPosR);        
    if(hitR)
    {
        hitRDist = distance3(lowR[3], hitPosR[3]);
         if(lowR[3][1] < hitPosR[3][1])
            hitRDist = -hitRDist; // collision is above
        xyprintf(80, 21, "Right Dist:\t%.2f", hitRDist);
    }
    
    xyprintf(80,19,"Translation: %s", game_state.testInt1 ? "OFF" : "On");

    if(hitR && hitL)
         drawWorldLine(hitPosR[3], hitPosL[3],    CLR_BLACK,    5);

      if(    (hitL && hitLDist > hitRDist)
        || (game_state.testInt1 && ABS(hitLDist) < 10))
    {
         drawWorldLine(lowL[3], hitPosL[3], CLR_DARK_RED, 5);
    }
    if(    (hitR && hitRDist > hitLDist)
        || (game_state.testInt1 && ABS(hitRDist) < 10))
    {
        drawWorldLine(lowR[3], hitPosR[3], CLR_DARK_RED, 5);
    }

    if(hitR && hitL)
    {
        if(hitRDist > hitLDist)
             setVec3(vec, 0, hitRDist, 0);
        else
               setVec3(vec, 0, hitLDist, 0);
    }
    else
        return; // no IK necessary

    if(game_state.testInt1)
        return;



    vec[1] -= .1; // height of shoe
    subVec3(seq->gfx_root->mat[3], vec, seq->gfx_root->mat[3]);
 
      if( 0.02 > ABS(hitRDist - hitLDist))
        return; // close enough to not need correction

    // debug lines where legs WOULD be w/o adjustment
     {
         mulMat4( seq->gfx_root->mat, hipNode->mat, hip);

        mulMat4(hip,    upperNodeL->mat,    upperL);
        mulMat4(upperL, lowerNodeL->mat,    lowerL);
        mulMat4(lowerL, footNodeR->mat,        footL);
        mulMat4(footL,    toeNodeR->mat,        toeL);

        mulMat4(hip,    upperNodeR->mat,    upperR);
        mulMat4(upperR, lowerNodeR->mat,    lowerR);
        mulMat4(lowerR, footNodeR->mat,        footR);
        mulMat4(footR,    toeNodeR->mat,        toeR);

        drawWorldLine(upperL[3],    lowerL[3],    CLR_BLUE,    5);
        drawWorldLine(lowerL[3],    footL[3],    CLR_BLUE,    5);
        drawWorldLine(footL[3],        toeL[3],    CLR_BLUE,    5);
        drawWorldLine(upperR[3],    lowerR[3],    CLR_BLUE,    5);
        drawWorldLine(lowerR[3],    footR[3],    CLR_BLUE,    5);
        drawWorldLine(footR[3],        toeR[3],    CLR_BLUE,    5);
    }

     if(hitRDist > hitLDist)
    {
        upper = upperL;
        lower = lowerL;
        foot = footL;
        toe = toeL;
        low = lowL;
        hitPos = hitPosL;
        upperNode = upperNodeL;
        lowerNode = lowerNodeL;
        footNode = footNodeL;
        toeNode = toeNodeL;
    }
    else
    {
        upper = upperR;
        lower = lowerR;
        foot = footR;
        toe = toeR;
        low = lowR;
        hitPos = hitPosR;
        upperNode = upperNodeR;
        lowerNode = lowerNodeR;
        footNode = footNodeR;
        toeNode = toeNodeR;
    }


    // bend leg (only one)
    {
        Vec3 vec, axis, hipKneeVec, kneeFootVec, hipCollisionVec, newHipKneeVec, newKneeFootVec;
        F32 theta;
        F32 scale;
        SeqRegQuat q;
        Mat4 mat;
        Mat4 temp;
        int hit;

        // compute new geometry intersection point
        hit = findCollisionPt(lower, hitPos);

        subVec3(lower[3], upper[3], hipKneeVec);
         subVec3(toe[3], lower[3], kneeFootVec);
        subVec3(low[3], upper[3], hipCollisionVec);

        // compute axis of rotation (same as axis of rotation for bending at knee)
         setVec3(vec, 1,0,0);
        mulVecMat3(vec, upper, axis);
        normalVec3(axis);

        {
            addVec3(lower[3], axis, a);
             drawWorldLine(lower[3], a, CLR_PARAGON, 5);
            drawWorldLine(upper[3], hitPos[3], CLR_PARAGON, 5);
        }

        // compute new angles between joints
        solveArmAngles( distance3(lower[3],        toe[3]    ),
                         distance3(upper[3],        hitPos[3]    ),    // this one uses the desired foot position, where the collision occured
                        distance3(upper[3],        lower[3]    ),
                        &theta,        // angle opposite knee-foot line
                         &scale);

        xyprintf(80, 22, "Theta:\t%.2f", theta);
        xyprintf(80, 23, "Scale:\t%.2f", scale);




        //----- ADJUST UPPER LEG -----------------------------------------------------------------------------------------------------------------


         srQuatFromAxisAngle(&q, axis, -theta);
        srQuatRotVec3(hipCollisionVec, &q, newHipKneeVec);

        {
            F32 lenA, lenB;
            lenA = lengthVec3(newHipKneeVec);
            lenB = lengthVec3(hipKneeVec);
            scaleVec3(newHipKneeVec, (lenB/lenA), newHipKneeVec);
            addVec3(newHipKneeVec, upper[3], a);  
            if(game_state.fixRegDbg)
                drawWorldLine(upper[3], a, CLR_WHITE, 10);
        }

         theta = getSRQuatRotBetweenVectors(upperL, hipKneeVec, newHipKneeVec, &q, seq);
        copyMat4(unitmat, mat);            // FUTURE OPTIM: reduce # of writes
        srQuatToMat(&q, mat);
        mulMat3(upperNode->mat, mat, temp);
        if(game_state.fixLegReg)
            copyMat3(temp, upperNode->mat);

        //----- ADJUST LOWER LEG -----------------------------------------------------------------------------------------------------------------

        mulMat4(hip,    upperNode->mat,    upper);
        mulMat4(upper, lowerNode->mat,        lower);
        mulMat4(lower, footNode->mat,        foot);
        mulMat4(foot,    toeNode->mat,        toe);

         subVec3(toe[3], lower[3], kneeFootVec);
         subVec3(hitPos[3], lower[3], newKneeFootVec);

    
         if(game_state.fixRegDbg)
             drawWorldLine(lower[3], hitPos[3], CLR_WHITE, 10);
    
        getSRQuatRotBetweenVectors( lower, kneeFootVec, newKneeFootVec, &q, seq);
         copyMat4(unitmat, mat);            // FUTURE OPTIM: reduce # of writes
        srQuatToMat(&q, mat);
        mulMat3( lowerNode->mat, mat,temp);

        if(game_state.fixLegReg)
        {
            // fix interp here... 
            copyMat3(temp, lowerNode->mat);
        }


        //----- ADJUST FOOT -----------------------------------------------------------------------------------------------------------------

        //----- ADJUST WAIST TILT -----------------------------------------------------------------------------------------------------------------
    
    
        //----- INTERPOLATE -----------------------------------------------------------------------------------------------------------------
    
        //{
        //    int change = 0;

        //    change |= CHANGE_SINGLE; //PTOR need to figure out new way to know whether the anim changed materailly from last frame

        //    fixInterp(seq, upperNode->anim_id, &newrot, newpos, percent_new, &change);

        //    //If you are interpolating, then go all the way
        //    if( interpolate )
        //    {
        //        *change |= CHANGE_SINGLE; //PTOR need to figure out new way to know whether the anim changed materailly from last frame

        //        if( (seq->curr_interp_state[anim_id] & ( BLEND_ROTS | BLEND_XLATES )) && percent_new < 1.0 )
        //            animInterpNodeWithLastPosition( seq, anim_id, &newrot, newpos, percent_new, change );

        //        //Assign the newpos and newrot
        //        if( *change != NO_CHANGE) 
        //        {
        //            copyVec3( newpos, seq->last_xlates[anim_id] );
        //            copyVec3( newpos, gfxnode->mat[3] );

        //            srQuatToMat( &newrot, gfxnode->mat );   
        //            memcpy(&(seq->last_rotations[anim_id]), &newrot, sizeof(SeqRegQuat));

        //            *change = (*change) & ~CHANGE_SINGLE;
        //        }

        //        //Mark this node as recently updated
        //        seq->last_frog_id[anim_id] = seq->frog_id; 
        //    }
        //    else 
        //    {
        //        srQuatToMat( &newrot, gfxnode->mat );
        //        copyVec3( newpos, gfxnode->mat[3] );
        //    }
        //}
    }
}
#endif

void adjustLimbs(SeqInst *seq)
{
    if(    seq->gfx_root->child && seq->currgeomscale[1] != 1.0 &&
        getMoveFlags(seq, SEQMOVE_FIXHIPSREG) )
    {
        // Keep hips in the same place as unscaled playback, for swimming and such
        GfxNode *hipsNode = gfxTreeFindBoneInAnimation(BONEID_HIPS, seq->gfx_root->child, seq->handle, 1);
        if(hipsNode)
            scaleVec3(hipsNode->mat[3], 1.0/seq->currgeomscale[1], hipsNode->mat[3]);
    }

    ////// Head turning and foot registration to come back to someday
//    if(seq == playerPtr()->seq) // for now, only do player
//        adjustLegs(e);  //Someday do foot registration with the ground
//    doHeadTurn(seq); //Nothing

    // Keep wrists or weps (depending on move) in the same place as unscaled playback
    adjustArms(seq);
}
