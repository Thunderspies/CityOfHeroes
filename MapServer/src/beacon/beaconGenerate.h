
#ifndef BEACONGENERATE_H
#define BEACONGENERATE_H

#define BEACON_GENERATE_CHUNK_SIZE 256

#define BEACON_GEN_COLUMN_INDEX(x, z) (x + z * BEACON_GENERATE_CHUNK_SIZE)

typedef struct BeaconDiskSwapBlock                BeaconDiskSwapBlock;
typedef struct BeaconGenerateChunk                BeaconGenerateChunk;
typedef struct BeaconGenerateColumn                BeaconGenerateColumn;
typedef struct BeaconGenerateColumnArea         BeaconGenerateColumnArea;
typedef struct BeaconGenerateFlatArea            BeaconGenerateFlatArea;
typedef struct LegalConn                        LegalConn;
typedef struct BeaconGenerateCheckList            BeaconGenerateCheckList;
typedef struct BeaconGenerateEdgeNode            BeaconGenerateEdgeNode;
//typedef struct BeaconDiskDataChunk                BeaconDiskDataChunk;
typedef struct BeaconServerClientData            BeaconServerClientData;
typedef struct BeaconLegalAreaCompressed        BeaconLegalAreaCompressed;
typedef struct BeaconBeaconedArea                BeaconBeaconedArea;
typedef struct Model                            Model;

typedef struct BeaconGenerateColumnAreaSide {
    BeaconGenerateColumnArea*            area;
} BeaconGenerateColumnAreaSide;

typedef struct BeaconGenerateCheckList {
    BeaconGenerateCheckList*            next;
    U32                                    distance;
    BeaconGenerateColumnArea*            areas;
} BeaconGenerateCheckList;

typedef struct BeaconGenerateFlatArea {
    BeaconGenerateChunk*                chunk;
    BeaconGenerateFlatArea*                nextChunkArea;
    BeaconGenerateCheckList*            checkList;
    BeaconGenerateColumnArea*            nonEdgeList;
    BeaconGenerateColumnArea*            edgeList;
    LegalConn*                            inBoundConn;
    int                                    allAreasCount;
    U32                                    color;
    F32                                    y_min;
    F32                                    y_max;
    U32                                    foundBeaconPositions : 1;
} BeaconGenerateFlatArea;

typedef enum BeaconGenerateSlope {
    BGSLOPE_FLAT        = 0,
    BGSLOPE_SLIPPERY    = 1,
    BGSLOPE_STEEP        = 2,
    BGSLOPE_VERTICAL    = 3,
    BGSLOPE_COUNT,
} BeaconGenerateSlope;

typedef struct BeaconGenerateEdgeNode {
    BeaconGenerateColumnArea*            columnArea;
    BeaconGenerateEdgeNode*                nextEdgeNode;
    BeaconGenerateEdgeNode*                nextYawNode;
    F32                                    yaw;
    F32                                    distSQR;
    struct {
        BeaconGenerateEdgeNode*            next;
        BeaconGenerateEdgeNode*            prev;
    } hull;
} BeaconGenerateEdgeNode;

typedef struct BeaconGenerateBeaconingInfo {
    BeaconGenerateColumnArea*            closestBeacon;//TEMP!!!

    BeaconGenerateFlatArea*                flatArea;
    U32                                    edgeDistSQR;
    F32                                    edgeCircleRadius;
    BeaconGenerateColumnArea*            nextPlacedBeacon;
    Vec3                                beaconPos;
    F32                                    beaconDistSQR;
    F32                                    beaconDistToEdgeCircleRadiusRatio;
    struct {
        BeaconGenerateColumnArea*        next;
        BeaconGenerateColumnArea*        prev;
    } edgeCircle;
    U32                                    hasBeacon            : 1;
    U32                                    doNotCheckMyCircle    : 1;
} BeaconGenerateBeaconingInfo;

#if 0
    #define BEACONGEN_CHECK_VERTS 1
    #define BEACONGEN_STORE_AREAS 1
    #define BEACONGEN_STORE_AREA_CREATOR 1
#endif

typedef struct BeaconGenerateColumnAreaTriVerts {
    Vec3 verts[3];
    
    #if BEACONGEN_CHECK_VERTS
        GroupDef* def;
    #endif
} BeaconGenerateColumnAreaTriVerts;

typedef struct BeaconGenerateColumnArea {
    union{
        BeaconGenerateFlatArea*            flatArea;
        BeaconGenerateBeaconingInfo*    beacon;
    };        
    BeaconGenerateColumnArea*            nextColumnArea;
    BeaconGenerateColumnArea*            nextChunkLegalArea;
    BeaconGenerateColumnArea*            nextInFlatAreaSubList;
    BeaconGenerateColumnAreaSide        sides[8];
    F32                                    y_min;
    F32                                    y_max;
    S16                                    y_min_S16;
    S16                                    y_max_S16;
    U16                                    columnIndex;
    U16                                    columnAreaIndex;
    U8                                    usedSides;
    U8                                    foundSides;
    U32                                    slope                        : 2;
    U32                                    isLegal                        : 1;
    U32                                    inChunkLegalList            : 1;
    U32                                    inEdgeList                    : 1;
    U32                                    inNonEdgeList                : 1;
    U32                                    inCheckList                    : 1;
    U32                                    makeBeacon                    : 1;
    U32                                    isEdge                        : 1;
    U32                                    noGroundConnections         : 1;
    U32                                    inCompressedLegalList        : 1;
    U32                                    doNotMakeBeaconsInVolume    : 1;

    #if BEACONGEN_CHECK_VERTS
        BeaconGenerateColumnAreaTriVerts    triVerts;    // TEMP!!!!!!
    #endif
} BeaconGenerateColumnArea;

typedef struct BeaconGenerateColumn {
    BeaconGenerateChunk*                chunk;
    BeaconGenerateColumnArea*            areas;
    int                                    areaCount;
    struct {
        BeaconGenerateFlatArea*            flatArea;
        BeaconGenerateColumnArea*        curArea;
    } propagate;
    U32                                    isIndexed        : 1;

    #if BEACONGEN_CHECK_VERTS
        // TEMP!!!!!!
        struct {
            struct {
                GroupDef*    def;
                F32            y_min;
                F32            y_max;
                Vec3        verts[3];
            }* tris;
            int count;
            int maxCount;
        } tris;
    #endif
} BeaconGenerateColumn;

typedef struct BeaconGenerateLegalAreaList {
    BeaconGenerateColumnArea*            areas;
    int                                    count;
} BeaconGenerateLegalAreaList;

typedef struct BeaconGenerateChunk {
    BeaconDiskSwapBlock*                swapBlock;
    BeaconGenerateColumn                columns[SQR(BEACON_GENERATE_CHUNK_SIZE)];
    BeaconGenerateLegalAreaList            legalAreas[BGSLOPE_COUNT];
    U32                                    columnsChecked;
    BeaconGenerateFlatArea*                flatAreas;
    int                                    flatAreaCount;
} BeaconGenerateChunk;

//typedef struct BeaconDiskDataChunk {
//    BeaconDiskDataChunk*                next;
//    int                                    size;
//    LPOVERLAPPED                        overlapped;
//    char                                data[128 * 1024];
//} BeaconDiskDataChunk;

typedef struct BeaconLegalAreaCompressed {
    BeaconLegalAreaCompressed*            next;
    
    U8                                    x;
    U8                                    z;
    
    union {
        U32                                y_index;
        F32                                y_coord;
    };
    
    U32                                    isIndex             : 1;
    U32                                    checked             : 1;
    U32                                    foundInReceiveList    : 1;

    struct {
        #if BEACONGEN_STORE_AREAS
            struct {
                F32        y_min;
                F32        y_max;

                #if BEACONGEN_CHECK_VERTS
                    // TEMP!!!!!!
                    
                    Vec3    triVerts[3];
                    char*    defName;
                #endif
            }* areas;
            int maxCount;
        #endif
        
        int count;
        
        #if BEACONGEN_STORE_AREA_CREATOR
            U32 ip;
            int    cx;
            int cz;
        #endif
    } areas;

    #if BEACONGEN_CHECK_VERTS
        // TEMP!!!!!!
        
        struct {
            struct {
                char*    defName;
                F32        y_min;
                F32        y_max;
                Vec3    verts[3];
            }* tris;
            int count;
            int maxCount;
        } tris;
    #endif
} BeaconLegalAreaCompressed;

typedef struct BeaconDiskSwapBlockGeoRef {
    GroupDef*                            groupDef;
    Model*                                model;
    Mat4                                mat;
    int                                    used;
} BeaconDiskSwapBlockGeoRef;

typedef struct BeaconBeaconedArea {
    BeaconBeaconedArea*                    next;
    BeaconGenerateColumnArea*            columnArea;
} BeaconBeaconedArea;

typedef struct BeaconGeneratedBeacon {
    Vec3    pos;
    int        noGroundConnections;
} BeaconGeneratedBeacon;

typedef struct BeaconDiskSwapBlock {
    BeaconDiskSwapBlock*                nextSwapBlock;

    //struct {
    //    BeaconDiskSwapBlock*            next;
    //    //BeaconDiskDataChunk*            chunks;
    //    int                                totalSize;
    //    HANDLE                            hFile;
    //} diskData;

    struct {
        BeaconDiskSwapBlock*            next;
        BeaconDiskSwapBlock*            prev;
    } inMemory;
    
    struct {
        BeaconDiskSwapBlock*            next;
        BeaconDiskSwapBlock*            prev;
    } legalList;
    
    struct {
        BeaconLegalAreaCompressed*        areasHead;
        int                                totalCount;
        int                                uncheckedCount;
    } legalCompressed;
    
    struct {
        int*                            refs;
        int                                count;
        int                                maxCount;
    } geoRefs;
    
    struct {
        BeaconGeneratedBeacon*            beacons;
        int                                count;
        int                                maxCount;
    } generatedBeacons;
    
    struct {
        BeaconServerClientData**        clients;
        int                                count;
        int                                maxCount;
        U32                                assignedTime;
    } clients;

    U32                                    createdIndex;
    int                                    x;
    int                                    z;
    //char*                                fileName;
    //U32                                    fileUID;
    Array                                beacons;
    U32                                    instanceID;
    BeaconGenerateChunk*                chunk;

    F32                                    y_min;
    F32                                    y_max;
    int                                    visitCount;
    
    U32                                    surfaceCRC;

    U32                                    gotMinMax    : 1;
    U32                                    isLegal        : 1;
    U32                                    sent        : 1;
    U32                                    inverted    : 1;
    U32                                    isInMemory    : 1;
    U32                                    addedLegal    : 1;
    U32                                    foundCRC    : 1;
    U32                                    verifiedCRC    : 1;
} BeaconDiskSwapBlock;

// Beacon processing stuff.

typedef struct BeaconProcessRaisedConnection {
    float minHeight;
    float maxHeight;
} BeaconProcessRaisedConnection;

typedef struct BeaconProcessConnection {
    U8                                    distanceXZ;
    S8                                    yaw90;
    U16                                    raisedCount;
    BeaconProcessRaisedConnection*        raisedConns;
    F32                                    minHeight;
    F32                                    maxHeight;
    S32                                    targetIndex                : 24;
    U32                                    hasBeenChecked            : 1;
    U32                                    makeGroundConnection    : 1;
    U32                                    reachedByGround            : 1;
    U32                                    reachedByRaised            : 1;
    U32                                    reachedBySomething        : 1;
    U32                                    failedWalkCheck            : 1;
} BeaconProcessConnection;

#define ANGLE_INCREMENT 2
#define ANGLE_INCREMENT_COUNT (360 / ANGLE_INCREMENT)

typedef struct BeaconProcessAngleInfo {
    F32                                    ignoreMin;
    F32                                    ignoreMax;
    F32                                    openMin;
    F32                                    openMax;
    Vec3                                posReached;
    F32                                    posReachedDistXZ;
    U32                                    handledForGround    : 1;
    U32                                    handledForRaised    : 1;
    U32                                    done                : 1;
} BeaconProcessAngleInfo;

typedef struct BeaconProcessAnglesInfo {
    BeaconProcessAngleInfo                angle[ANGLE_INCREMENT_COUNT];
    int                                    curGroup;
    U8                                    completedCount;
} BeaconProcessAnglesInfo;

typedef struct BeaconProcessInfo {
    BeaconProcessConnection*            beacons;
    BeaconDiskSwapBlock*                diskSwapBlock;
    S32                                    beaconCount;

    struct {
        S32                                physicsSteps;
        S32                                walkedConnections;
        F32                                totalDistance;
    } stats;

    BeaconProcessAnglesInfo*            angleInfo;
    
    U32                                    isLegal : 1;
} BeaconProcessInfo;

typedef struct BeaconServerClientData    BeaconServerClientData;

typedef struct BeaconDiskSwapBlockGrid    BeaconDiskSwapBlockGrid;

typedef struct BeaconProcessBlocks {
    StashTable                            table;
    BeaconDiskSwapBlockGrid*            grid;
    BeaconDiskSwapBlock*                list;
    int                                    count;
    
    int                                    grid_min_xyz[3];
    int                                    grid_max_xyz[3];

    struct {
        int                                count;
        BeaconDiskSwapBlock*            head;
        BeaconDiskSwapBlock*            tail;
    } inMemory;

    struct {
        BeaconDiskSwapBlock*            head;
        int                                count;
    } legal;
    
    struct {
        BeaconDiskSwapBlockGeoRef*        refs;
        int                                count;
        int                                maxCount;
    } geoRefs;
    
    struct {
        BeaconGeneratedBeacon*            beacons;
        int                                count;
        int                                maxCount;
    } generatedBeacons;
    
    struct {
        struct {
            Vec3                        pos;
            int                            actorID;
        }* pos;            
        int                                count;
        int                                maxCount;
    } encounterPositions;
} BeaconProcessBlocks;

extern BeaconProcessBlocks bp_blocks;

// beaconConnection.c -----------------------------------------------------------------------

void* beaconAllocateMemory(U32 size);

// beaconGenerate.c -------------------------------------------------------------------------

BeaconGenerateChunk* createBeaconGenerateChunk(void);
void destroyBeaconGenerateChunk(BeaconGenerateChunk* chunk);

BeaconGenerateColumnArea* createBeaconGenerateColumnArea(void);
void destroyBeaconGenerateColumnArea(BeaconGenerateColumnArea* area);

BeaconGenerateFlatArea* createBeaconGenerateFlatArea(void);
void destroyBeaconGenerateFlatArea(BeaconGenerateFlatArea* area);

BeaconGenerateCheckList* createBeaconGenerateCheckList(void);
void destroyBeaconGenerateCheckList(BeaconGenerateCheckList* list);

void beaconClearSwapBlockChunk(BeaconDiskSwapBlock* block);

int beaconMakeLegalColumnArea(BeaconGenerateColumn* column, BeaconGenerateColumnArea* area, int doNotMakeBeaconsInVolume);

BeaconGenerateColumnArea* beaconGetColumnAreaFromYPos(BeaconGenerateColumn* column, F32 y);

int beaconGetValidStartingPointLevel(GroupDef* def);
void beaconResetGenerator(void);
void beaconGenerateMeasureWorld(S32 quiet);
void beaconCreateSurfaceSets(void);
void beaconExtractSurfaces(int grid_x, int grid_z, int dx, int dz, int justEdges);

void beaconGenerateCombatBeacons(void);

void beaconAddToCheckList(BeaconGenerateFlatArea* flatArea, BeaconGenerateColumnArea* columnArea);

void beaconMakeBeaconsInChunk(BeaconGenerateChunk* chunk, int quiet);

void beaconPropagateLegalColumnAreas(BeaconDiskSwapBlock* swapBlock);

S32 beaconGenerateUseNewPositions(S32 set);

// beaconGenerateSwap.c ---------------------------------------------------------------------

BeaconDiskSwapBlock* beaconGetDiskSwapBlockByGrid(int gridx, int gridz);
BeaconDiskSwapBlock* beaconGetDiskSwapBlock(int x, int z, int create);
void beaconAddToInMemoryList(BeaconDiskSwapBlock* block);
void beaconMakeDiskSwapBlocks(void);
void beaconClearNonAdjacentSwapBlocks(BeaconDiskSwapBlock* centerBlock);
void beaconDestroyDiskSwapInfo(int quiet);

// beaconClientserver.c ---------------------------------------------------------------------

void destroyBeaconLegalAreaCompressed(BeaconLegalAreaCompressed* area);
BeaconLegalAreaCompressed* beaconAddLegalAreaCompressed(BeaconDiskSwapBlock* block);


#endif