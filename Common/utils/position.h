/* File position.h
 *    This module contains some generic operations that can be done using some positional
 *    information.  It is expected that the pointers passed to the functions are actually
 *    pointers to structures that match the Position structure's format.  The position 
 *    structure isn't meant to be used by itself.
 *
 *    The condition functions required by some of the functions gives the user control 
 *    as to which Positions are acceptable as the result of the function.  These condition
 *    functions are almost always passed the current Position and the reference Position
 *    under consideration.  The pointers passed back to the condition functions are simply
 *    pointers passed into the position functions.  Therefore, they can be "safely" casted
 *    back to pointers of whatever structure they are.
 */


#ifndef POSITION_H
#define POSITION_H

typedef struct Position Position;
typedef struct Entity Entity;
typedef struct Beacon Beacon;

#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/ArrayOld.h>

typedef struct Position{
    Mat4 mat;
} Position;


typedef enum{
    OFFSET_CARTESIAN,
    OFFSET_PARAMETRIC,
} OffsetType;

/* Structure RelativePosition
 *    A relative position can be stated in two formats, cartesian or parametric.
 *    A cartesian relative position contains the x, y, and z offset in mat[3].  A
 *    parametric relative position contains yaw and distance offset in mat[3].
 *    mat[0] ~ mat[2] is used for storing orientation information as usual.
 *
 */
//typedef struct{
//    union{
//        Mat4 mat;
//
//        struct{
//            Mat4 mat;
//        } cartesian;
//
//        struct{
//            Mat3 mat3;
//            float yaw;
//            float distance;
//        } parametric;
//    };
//    
//    OffsetType type;
//} RelativePosition;

/* Condition functions
 *    These functions tell if a certain condition has been met given some Position(s).
 *    See each function that requires a condition function for details on what
 *    is expected of the condition functions.
 *
 *    Returns:
 *        0 - condition not met
 *        1 - condition met
 */
typedef int (*Condition)(Beacon*, Entity*);


Beacon* posFindNearest(Array* searchlist, const Mat4 ref);
Beacon* posCondFindNearest(Array* searchlist, Entity* e, Condition accept);
//Position* posCondFindBest(Array* searchlist, Condition compare);
//Array* posFindNearby(Array* searchlist, Position* ref, Condition nearby);
//void posFindNearbyEx(Array* searchlist, Position* ref, Condition nearby, Array* result);
int posWithinDistance(const Mat4 source, const Mat4 target, float distance);
//int posWithinDistance2D(Position* source, Position* target, float distance);
float posGetDistSquared(const Mat4 source, const Mat4 target);
//float posGetDistSquared2D(Position* source, Position* target);
//float posGetDist(Position* source, Position* target);
//float posGetDist2D(Position* source, Position* target);
//void posGetUnitVector(Position* source, Position* target, Vec3 result);
//void posGetVector(Position* source, Position* target, Vec3 result);
//float posGetHeading(Position* source);
//float posGetHeading2(Position* source, Position* target);
//float posGetPitch(Position* source);
//
//void posApplyRandomOffset(Position* source, Position* destination, int maxXOffset, int maxYOffset, int maxZOffset);
//
//void posApplyOffset(Position* source, Position* destination, RelativePosition* offset);
//void posApplyCOffset(Position* source, Position* destination, float xOffset, float yOffset, float zOffset);
//void posApplyPOffset(Position* source, Position* destination, float yaw, float distance);
//
//void posConvertRelative(RelativePosition* pos, OffsetType newType);
//
//
//RelativePosition* createRelativePosition();
//void destroyRelativePosition(RelativePosition* pos);

#define posPoint(pos) ((pos)->mat[3])
#define posX(pos) ((pos)->mat[3][0])
#define posY(pos) ((pos)->mat[3][1])
#define posZ(pos) ((pos)->mat[3][2])
#define posParamsXYZ(pos) posX(pos), posY(pos), posZ(pos)


#endif
