#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "group/group.h"

#define MAX_PORTALS 8

typedef enum MGRoomType {
    MG_Start,
    MG_End,
    MG_Hall,
    MG_Room,
    MG_Door,
     MG_Elevator,

    MG_NumTypes,
} MGRoomType;

typedef enum MGRoomSize {
    MG_Small,
    MG_Medium,
    MG_Large,

    MG_NumSizes,
} MGRoomSize;

typedef struct {
    char name[128];                //name of the piece as it appears in the editor
    char path[512];                //full path, including name
    int portals;                //number of portals on this piece
    MGRoomType type;            //room, hall, start, etc...
    MGRoomSize size;            //small, medium, large, etc...
    Vec2 ** convexHull;            //points of the convex hull, counterclockwise, might leave out some points
                                //so that an L-shaped room doesn't exclude significantly more space from the
                                //map than it actually takes up
    double area;                //rough approximation of the area of this piece
    Vec3 inside;                //some point inside the tray, acceptable for room markers, etc...
    int pointInside;            //false iff Vec3 inside is not valid
    int hasObjective;            //true iff this piece can have objectives
    Vec3 portal[MAX_PORTALS];    //the location of the portals on this piece
    Vec3 normal[MAX_PORTALS];    //the corresponding normal of each portal
} MapPiece;

typedef struct {
    char mapSet[128];
    MapPiece ** pieces;
} MapSetGenerator;

typedef struct MGNode {
    MapPiece * piece;
    struct MGNode * adjacent[MAX_PORTALS];    //max number of portals for a room, can be increased with no problem
    struct MGNode * doors[MAX_PORTALS];        //adjacent doorframes, doors, locked doors, etc...
    int source;                                //index in adjacent of the room that comes before this room
    Mat4 mat;                                //position of this piece in the map
    DefTracker * tracker;
    struct MGNode * startNode;                //first node created in this map
    int depth;                                //distance from the start node
    int number;                                //unique number of room on this map, increasing from start
    int hasExtents;                            //if !=0, indicates which floor this node knows the extents for
    int floor;                                //floor number, so we can place elevators correctly
    int roomsThisFloor;                        //number of rooms on this floor that are beyond this one
    Vec2 min,max;                            //the coordinates of the extents markers, if known
    int hasDoors;                            //true iff this room has doors, every other room has doors
    float area;                                //area of this node and all child nodes
    int objectiveRating;                    //used when placing objectives to rate how good a node is
    int roomRating;                            //used when placing room markers to rate how good a node is
} MGNode;

typedef struct {
    char missionSet[128];
    MapSetGenerator * msg;
    //rules
    char mapSet[128];
    char sceneFile[128];
    char villainsSceneFile[128];
    char ambientSound[128];
    float minArea;
    float maxArea;
    int branchFactor;
    int minHalls;
    int maxHalls;
    int minDepth;
    int maxDepth;
    int numRooms;
    int numObjectives;
    int minObjectiveDepth;
    int roomsPerFloor;
} MissionSetRules;

typedef struct {
    int halls;
    double maxArea;
    double minArea;
    int depth;
} CurrentMapStatus;

void initGenerators2();
int preprocessGenerators();
MGNode * generateMap(const char * mapSet,int randomseed);
void destroyMGNode(MGNode * node);
MGNode * getNthNode(MGNode * node,int n);
double getArea(GroupDef * def);

typedef struct Packet Packet;
void mapGeneratorUpkeep();
void mapGeneratorPacketHandler(Packet * pak);

//debugging
void generateTestMap();

#endif
