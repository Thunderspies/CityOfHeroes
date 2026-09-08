#define RT_ALLOW_VBO // RDRFIX
// clothnode.c
// Written by Steven Bennetts (stevenjb@yahoo.com) for Cryptic Studios, May 2004

#include "graphics/clothnode.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/stdtypes.h>
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include <utilitieslib/utils/textparser.h>
#include "seq/seqload.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include "graphics/FX/fxcapes.h"
#include <utilitieslib/components/StringCache.h>
#include "graphics/FX/particle.h"
#include <utilitieslib/components/StashTable.h>

#include "Cloth/Cloth.h"
#include "Cloth/ClothCollide.h"
#include "Cloth/ClothMesh.h"
#include "Cloth/ClothPrivate.h"
#include "Cloth/Wind.h"
#include "render/tex.h"
#include "seq/tricks.h"
#include "entity/entity.h"
#include "seq/anim.h"

#include "render/thread/rt_queue.h"

#include <utilitieslib/utils/timing.h>

#if CLOTH_HACK
#pragma warning (disable:4101)

//////////////////////////////////////////////////////////////////////////////
// This code

//////////////////////////////////////////////////////////////////////////////
// CLOTH_DEBUG does not have any significant performance impact,
// mostly it just enables some run-time tuning as follows:
//
// clothDebug [bitfield] Debug flags (see CLOTH_FLAG_DEBUG_* below)
// clothWindMax         world_wind_max_mag. 1=0.1 10=1.0 (see below)
// clothRipplePeriod    cloth_wind_ripple_period, 1=10%, 10=100% (default)
// clothRippleRepeat    cloth_wind_ripple_repeat, 1=10%, 10=100% (default)
// clothRippleScale        cloth_wind_ripple_scale, 1=10%, 10=100% (default)
// clothWindScale        CLOTH_WINDSCALE, 1=10%, 10=100% (default)

#define CLOTH_DEBUG 1
// Include test data for capes and flags
#define CLOTH_TEST 1

static void initEntityCapeCol(ClothObject * clothobj, int lod, Vec3 scale, const char *seqType);

//////////////////////////////////////////////////////////////////////////////
// TUNING VARIABLES

// LOD
// if camdist < cloth_lod_mindist, use LOD 0
// if camdist > cloth_lod_maxdist, use LOD MAX
// If in between, evenly transition from LOD 1 to LOD MAX
// NOTE: These values could (and SHOULD) be tuned based on CPU performance and
//  possible average framerate.
F32 cloth_lod_mindist = 20.0f; 
F32 cloth_lod_maxdist = 150.0f;

// PHYSICS
F32 CLOTH_WINDSCALE = 64.f;     // ~twice gravity // [0,1] -> [0, CLOTH_WINDSCALE]
F32 CLOTH_GRAVITY = -32.0f;        // ft/s^2

// WIND CONSTANTS
// These should be controlled by some sort of global wind manager on the server.

// Direction of world "ambient" wind.
Vec3 world_wind_dir = { 0.0f, 0.0f, 1.0f };
// Magnitude of world wind. 1.0 is the absolute maximum.
// Average should be about 0.20
F32 world_wind_max_mag = 0.20f;

// These paramaters define a wave (approximately a sin wave) used to
// create a ripple effect along the cloth.
//   Repeat is number of repetitions across the cloth.
//     Theoretically this could be converted from a wavelength in feet,
//     however, unless capes or flags vary significantly in width,
//     that shouldn't be necessary.
//  Period is the "speed" of the wave in seconds / wave
//  Scale determines the relative significance of the ripple force.
//    This is multiplied by a tunable per-animation value.
F32 cloth_wind_ripple_repeat = 1.5f;
F32 cloth_wind_ripple_period = 0.75f;
F32 cloth_wind_ripple_scale = 1.0f;

//////////////////////////////////////////////////////////////////////////////
// "READY" state. These values are used to prevent the rendering of a
// cloth before it has had time to "settle". A few frames of processing
// seem to be all thats needed for the cape to look OK.

// Number of frames before resetting 'ready' state
#define CLOTH_MAX_NO_UPDATE_FRAMES 60
// Number of frames to update cloth before rendering
#define CLOTH_PRE_RENDER_FRAMES 3

//////////////////////////////////////////////////////////////////////////////
// Testing #defines

int USE_AVGTIME = 1; // Otherwise looks pretty bad on inconsistent or low FPS

//////////////////////////////////////////////////////////////////////////////
// Stats
int ClothNodeNum = 0;
int ClothNodeVisNum = 0;

// Local state variables
#define NUM_WINDS 5
Vec3 world_wind[NUM_WINDS] = {{0,0,0}}; // set once each frame
F32 world_wind_mag[NUM_WINDS] = {0.0f}; // lengthVec3(world_wind)
F32 world_wind_scale[NUM_WINDS] = {0.0f};
F32 world_wind_dir_scale[NUM_WINDS] = {0.0f};

//////////////////////////////////////////////////////////////////////////////
// WIND

// This is called once per frame and gets a current wind direction and
//   magnitude based on the global "prevailing" or average values.
void calculateClothWindThisFrame()
{
    int i;
#if CLOTH_DEBUG
    F32 mag = game_state.clothWindMax;
    if (mag > 0.0f)
    {
        mag = mag*.10f;
        world_wind_max_mag = MINMAX(mag, 0.1f, 1.0f);
    }
#endif
    ClothWindSetDir(world_wind_dir, world_wind_max_mag);
    for (i=0; i<NUM_WINDS; i++) {
        world_wind_mag[i] = ClothWindGetRandom(world_wind[i], &world_wind_scale[i], &world_wind_dir_scale[i]);
    }
}

// Per-animation wind values (plus gravity).
// These should be stored with the animation state data to avoid
//   the additional string lookup and to allow easy tuning.

typedef struct ClothWindInfo
{
    F32 world_factor;    // Avg wind mag ~= .25, max = 1.0
    F32 local_factor;
    F32 ripple;        // Larger value produces greater ripple effect
    F32 gravity;    // gravity scale
    F32 minWind;
    Vec3 local_dir;

    char *name;
    bool useSpecialCollision;
    bool windInMovementDirection;
} ClothWindInfo;

typedef struct ClothWindInfoHolder {
    ClothWindInfo **windInfoList;
} ClothWindInfoHolder;

ClothWindInfo *WIND_INFO_DEFAULT=NULL;
ClothWindInfo *WIND_INFO_FREEZE=NULL;
ClothWindInfo *WIND_INFO_BACKRUN=NULL;
ClothWindInfo *WIND_INFO_JUMPFALL=NULL;
ClothWindInfo *WIND_INFO_JUMPUP=NULL;
ClothWindInfo *WIND_INFO_HOP=NULL;
ClothWindInfo *WIND_INFO_FLAG=NULL;
ClothWindInfo *WIND_INFO_TAIL=NULL;

ClothWindInfoHolder g_windInfoList={NULL};
StashTable htWindInfos;

TokenizerParseInfo ParseClothWindInfo[] =
{
    { "Name",            TOK_STRING(ClothWindInfo, name,    0),    0},
    { "WorldFactor",    TOK_F32(ClothWindInfo, world_factor,    0),    0},
    { "LocalFactor",    TOK_F32(ClothWindInfo, local_factor,    0),    0},
    { "Ripple",            TOK_F32(ClothWindInfo, ripple,    0),    0},
    { "Gravity",        TOK_F32(ClothWindInfo, gravity,    0),    0},
    { "MinWind",        TOK_F32(ClothWindInfo, minWind,    0),    0},
    { "LocalDir",        TOK_VEC3(ClothWindInfo, local_dir),    0},
    { "SpecialCollision",    TOK_BOOLFLAG(ClothWindInfo, useSpecialCollision,    0),    0},
    { "WindInMovementDirection",    TOK_BOOLFLAG(ClothWindInfo, windInMovementDirection,    0),    0},
    { "End",            TOK_END,        0},
    { "", 0, 0 },
};
TokenizerParseInfo ParseClothWindInfoFile[] =
{
    { "WindInfo",        TOK_STRUCT(ClothWindInfoHolder, windInfoList, ParseClothWindInfo) },
    { "", 0, 0 },
};

static void reloadWindCallback(const char *relpath, int when) {
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    loadClothWindInfo();
    // Re-set all sequencer windInfo pointers
    seqSetWindInfoPointers();
    // Re-set all cap windInfo pointers
    fxCapeInitWindInfo(NULL, NULL);
}


bool loadClothWindInfoPreprocessor(TokenizerParseInfo pti[], void* structptr)
{
    ClothWindInfo **windInfoList = ((ClothWindInfoHolder*)structptr)->windInfoList;
    int i, size = eaSize(&windInfoList);
    for (i=0; i<size; i++)
        normalVec3(windInfoList[i]->local_dir);
    return true;
}

void loadClothWindInfo()
{
    static bool inited=false;
    int i;
    int iSize;
    if (g_windInfoList.windInfoList) {
        ParserDestroyStruct(ParseClothWindInfoFile, &g_windInfoList);
    }
    // Load list of wind descriptions
    ParserLoadFiles(NULL, "cloth/WindInfo.txt", "clothWindInfo.bin", 0, ParseClothWindInfoFile, &g_windInfoList, NULL, NULL, loadClothWindInfoPreprocessor);
    if (htWindInfos) {
        stashTableDestroy(htWindInfos);
        htWindInfos = NULL;
    }

    // make hashtable
    htWindInfos = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
    iSize = eaSize(&g_windInfoList.windInfoList);
    for (i=0; i<iSize; i++) {
        stashAddPointer(htWindInfos, g_windInfoList.windInfoList[i]->name, g_windInfoList.windInfoList[i], false);
    }

    // Find some hardcoded defaults
    WIND_INFO_DEFAULT=getWindInfo("DEFAULT");
    WIND_INFO_FREEZE=getWindInfo("FREEZE");
    WIND_INFO_BACKRUN=getWindInfo("BACKRUN");
    WIND_INFO_JUMPFALL=getWindInfo("JUMPFALL");
    WIND_INFO_JUMPUP=getWindInfo("JUMPUP");
    WIND_INFO_HOP=getWindInfo("HOP");
    WIND_INFO_FLAG=getWindInfo("FLAG");
    WIND_INFO_TAIL=getWindInfo("TAIL");

    if (!inited) {
        // Add callback for re-loading
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "cloth/WindInfo.txt", reloadWindCallback);
        inited = true;
    }
}

ClothWindInfo *getWindInfo(const char *name)
{
    if ( htWindInfos && name )
        return stashFindPointerReturnPointer(htWindInfos, name);
    return NULL;
}

const ClothWindInfo *getWindInfoConst(const char *name)
{
    return cpp_const_cast(const ClothWindInfo*)( getWindInfo(name) );
}
// This sets the cloths Wind, WindRipple, and Gravity values each frame.

// NOTE: Currently there is no function to retrieve wind "effects" such
//   as a car passing by or a blast attack. The support code is there, it just
//   needs to be enabled by defining GetWindEffect() which should set a
//   normalized vector in world space and return the magnitude of the vector [0,1].
#define WIND_EFFECT_FUNC 0
extern F32 GetWindEffect(const Vec3 worldpos, Vec3 windeffectdir);

// 'chestmat' is used to transform the local wind into node space
// CLOTHFIX: 'animname' is used to look up the wind values
// 'movedir' is used for 'negative' local wind which flows in the direction
//   the node is moving
// return 1 if we need to 'freeze' the cloth
// return 2 if we need to use the low collision LOD (simple planes)
int update_wind(Cloth *cloth, ClothWindInfo *windInfo, Mat4 chestmat, Vec3 movedir, F32 moveamt,
                                            Mat4 entmat, Vec3 entmovedir, F32 entmoveamt,
                                            Vec3 dPYR)
{
    ClothWindInfo *effwindinfo=NULL;
    int res = 0;
    int i;

    if (windInfo) {
        // We were given a specific set of wind parameters to use, use them!
        effwindinfo = windInfo;
    } else 
    // Look for special situations rather than animation names
    if (movedir && entmovedir && dPYR) {
        // Calc movement direction in character space
        Vec3 movedir2;
        Vec3 entmovedir2;
        // Need to calculate this so it doesn't look at pitch?
        // Or just orientation of character, not skinned chest?  But then spins will mess it up...
        mulVecMat3Transpose(movedir, chestmat, movedir2);
        if (game_state.clothDebug && game_state.testDisplayValues)
            copyVec3(movedir2, game_state.testVec1);
        mulVecMat3Transpose(entmovedir, entmat, entmovedir2);
        if (game_state.clothDebug && game_state.testDisplayValues)
            copyVec3(entmovedir2, game_state.testVec2);

        // First look at character movement cases
        if (entmoveamt > 6) {
            if (entmovedir2[2]<-0.60) {
                // Character is moving backwards
                effwindinfo = WIND_INFO_BACKRUN;
            } else if (entmovedir2[1] > 0.5 && entmovedir2[2] > 0.65) {
                effwindinfo = WIND_INFO_HOP;
            } else if (entmovedir2[1] > 0.65) {
                effwindinfo = WIND_INFO_JUMPUP;
            } else if (entmovedir2[1] < -0.35) {
                effwindinfo = WIND_INFO_JUMPFALL;
            }
            if (entmovedir2[2]>0.60) {
                // Character moving forwards
                res = 3;
            }
        }
        // Now look at cape root movement cases where the character movement isn't enough
        if (effwindinfo==NULL) {
            if (moveamt > 6) {
                if (movedir2[2]<-0.65) {
                    effwindinfo = WIND_INFO_BACKRUN;
                } else if (movedir2[2]>0.65) {
                    // Character moving forwards
                    res = 3;
                }
            }
        }

        // Now, look at the speed of change in rotation
        if (ABS(dPYR[0]) > 25 || ABS(dPYR[1]) > 25 || ABS(dPYR[2]) > 25) {
            effwindinfo = WIND_INFO_FREEZE;
        }
    }

    if (effwindinfo==NULL) {
        // None determined by anything else, use default
        effwindinfo=WIND_INFO_DEFAULT;
    }

    if (!effwindinfo || effwindinfo->world_factor == 0.0f && effwindinfo->local_factor == 0.0f)
    {
        Vec3 winddir;
        zeroVec3(winddir);
        ClothSetWind(cloth, winddir, 0.0f, 0.0f);
        return 1;
    }
    else
    {
        Vec3 localwind, totalwind, winddir;
        F32 windmag, windmax, windripple;
        F32 lfact = effwindinfo->local_factor;
        F32 effectmag = 0.0f;
        int iWindNum;
        iWindNum = rand()*NUM_WINDS/(RAND_MAX+1);
        mulVecMat3(effwindinfo->local_dir, chestmat, localwind);
        
        if (effwindinfo->useSpecialCollision)
        {
            res = 2; // Use simple collisions while moving backwards
        }
        if (effwindinfo->windInMovementDirection)
        {
#if 0
            // Scale based on movement amount
            lfact *= moveamt / 21.0f; // 21 ft/sec = average running speed
#endif
            // Use movement direction for local direction
            copyVec3(movedir, localwind);
        }

        // Scale World wind efects by effwindinfo->world_factor
        scaleVec3( world_wind[iWindNum], effwindinfo->world_factor, winddir);
#if WIND_EFFECT_FUNC
        {
            Vec3 windeffect;
            effectmag = GetWindEffect(chestmat[3], windeffect); // Retrieve wind a position
            scaleAddVec3(windeffect, effwindinfo->world_factor, winddir, winddir);
        }
#endif
        // Scale Local wind effects by the transformed effwindinfo->local_dir
        scaleAddVec3(localwind, lfact, winddir, winddir);
        // Normalize and clamp
        windmag = normalVec3(winddir);
        if (windmag > 2.0f)
            windmag = 2.0f;

        // Determine the ripple scale, based on effwindinfo, cloth_wind_ripple_scale,
        //   and the max current wind magnitude
        windmax = ((MAX(world_wind_mag[iWindNum],windInfo?windInfo->minWind:0) + effectmag) * effwindinfo->world_factor + lfact);
        windripple = effwindinfo->ripple * windmax * cloth_wind_ripple_scale;

#if CLOTH_DEBUG
        // Tuning
        {
            F32 wind   = game_state.clothWindScale * .10f;
            F32 scale  = game_state.clothRippleScale * .10f;
            F32 repeat = game_state.clothRippleRepeat * .10f;
            F32 period = game_state.clothRipplePeriod * .10f;
            if (wind > 0.f)
                CLOTH_WINDSCALE  = wind * 64.0f;
            if (scale > 0.f)
                cloth_wind_ripple_scale  = scale * 1.0f;
            if (repeat > 0.f)
                cloth_wind_ripple_repeat = repeat * 1.5f;
            if (period > 0.f)
                cloth_wind_ripple_period = period * 1.0f;
        }
#endif

        // Set the appropriate Cloth values
        ClothSetWind(cloth, winddir, windmag, CLOTH_WINDSCALE);
        ClothSetWindRipple(cloth, windripple, cloth_wind_ripple_repeat, cloth_wind_ripple_period);
        ClothSetGravity(cloth, CLOTH_GRAVITY*effwindinfo->gravity);
        
        return res;
    }
}

//////////////////////////////////////////////////////////////////////////////
// COLLISION DATA
// See ClothCollide.c for descriptions of collision primitives.
// This code section should be replaced by informmation stored with the skeleton

#define SUPPORT_BOX_COL (1 && CLOTH_SUPPORT_BOX_COL)

typedef struct ClothBoneInfo
{
    char *nodename;
    char *nextname;
    CLOTH_COLTYPE type;
    F32 rad;
    F32 exten1;
    F32 exten2;
    Vec3 offset;
    Vec3 dir;
#if SUPPORT_BOX_COL
    Vec3 dirx;
    Vec3 diry;
#endif
    // Load time values
    char *sameAs;
    bool specialBackwardsFlag;
    bool specialForwardsFlag;
} ClothBoneInfo;

// Kind of a hack to enable a back plane only while running backwards:
#define CLOTH_COL_SPECIAL_BACKWARDS_FLAG 0x1000
#define CLOTH_COL_SPECIAL_FORWARDS_FLAG 0x2000
#define CLOTH_COL_CYLINDER_SPECIAL (CLOTH_COL_CYLINDER | CLOTH_COL_SPECIAL_BACKWARDS_FLAG)
/*
static ClothBoneInfo bone_info[MAX_NUM_LODS][MAX_CLOTH_COL] =
{
    // LOD 0, Use reasonably accurate collisions
    {
        { "HEAD", "",        CLOTH_COL_CYLINDER,    2.0f, 0.0f, 3.0f, { 0.0f, -0.25f, 0.5}, {0, 2.f, 0.0f} }, // HEAD
        { "WAIST", "",      CLOTH_COL_CYLINDER, 1.0f, 2.f, 0.5f, { 0.3f, 0.f, 0.45}, {0, 2.f, -0.2f} },    // BACK L
        { "WAIST", "",      CLOTH_COL_CYLINDER, 1.0f, 2.f, 0.5f, {-0.3f, 0.f, 0.45}, {0, 2.f, -0.2f} },    // BACK R
        { "UARMR", "LARMR", CLOTH_COL_BALLOON,   0.5f, 0, 0, {0.f, 0.f, 0.1f} },        // RIGHT ARM
        { "UARML", "LARML", CLOTH_COL_BALLOON,   0.5f, 0, 0, {0.f, 0.f, 0.1f} },
        { "LARMR", "HANDR", CLOTH_COL_CYLINDER,  0.3f, 0, 0.4, {0.f, 0.f, 0.0f} },        // LEFT ARM
        { "LARML", "HANDL", CLOTH_COL_CYLINDER,  0.3f, 0, 0.4, {0.f, 0.f, 0.0f} },
        { "ULEGR", "FOOTR", CLOTH_COL_CYLINDER,  0.5f, -.5, 1.0f, {0.f, 0.f, 0.0f} },    // RIGHT LEG
        { "ULEGL", "FOOTL", CLOTH_COL_CYLINDER,  0.5f, -.5, 1.0f, {0.f, 0.f, 0.0f} },    // LEFT LEG
        { "", "", CLOTH_COL_PLANE,  0.0f, 0, 0, {0.f, 0.1f, 0.f}, {0, 1.f, 0} },         // GROUND PLANE

        // Special case: BACK PLANE enabled only while moving backwards
        { "HIPS", "",      CLOTH_COL_CYLINDER_SPECIAL, 3.5f, 4.f, 4.0f, { 0.0f, 0.f, 2.0}, {0, 0.99f, -0.1f} },
        { 0 },
    },
    // LOD 1, Sacrafice some subtle effects for speed
    //        The plane along the back won't allow the cape to get in front of the character at all.
    {
        { "HIPS", "", CLOTH_COL_PLANE,  0.0f, 0, 0, {0.f, 0.f, -0.5f}, {0, 0, -1.f} },    // BACK PLANE
        { "UARMR", "LARMR", CLOTH_COL_BALLOON,  0.3f, 0, .5f, {0.f, 0.f, 0.0f} },        // RIGHT ARM
        { "UARML", "LARML", CLOTH_COL_BALLOON,  0.3f, 0, .5f, {0.f, 0.f, 0.0f} },        // LEFT ARM
        //{ "ULEGR", "FOOTR", CLOTH_COL_CYLINDER,  0.5f, 0, 1.0, {0.f, 0.f, 0.0f} },
        //{ "ULEGL", "FOOTL", CLOTH_COL_CYLINDER,  0.5f, 0, 1.0, {0.f, 0.f, 0.0f} },
        { "", "", CLOTH_COL_PLANE,  0.0f, 0, 0, {0.f, 0.1f, 0.f}, {0, 1.f, 0} },         // GROUND PLANE
        { 0 },
    },
    // LOD 2, Just use two planes for collisions (super fast)
    {
        { "HIPS", "", CLOTH_COL_PLANE,  0.0f, 0, 0, {0.f, 0.f, -0.5f}, {0, 0, -1.f} },    // BACK PLANE
        { "", "", CLOTH_COL_PLANE,  0.0f, 0, 0, {0.f, 0.1f, 0.f}, {0, 1.f, 0} },        // GROUND PLANE
        { 0 },
    }
};
*/

typedef struct ClothColInfoLOD {
    ClothBoneInfo **boneInfo;
} ClothColInfoLOD;

typedef struct ClothColInfo {
    char *type; // Male, Huge, Fem, etc
    ClothColInfoLOD **lod;
} ClothColInfo;

typedef struct ClothColInfoHolder {
    ClothColInfo **colInfoList;
} ClothColInfoHolder;

ClothColInfoHolder g_colInfoList={NULL};
StashTable htColInfos;
static int clothColReload=0;

StaticDefineInt ShapeTypeEnum[] =
{
    DEFINE_INT
    {"None",        CLOTH_COL_NONE},
    {"Sphere",        CLOTH_COL_SPHERE},
    {"Plane",        CLOTH_COL_PLANE},
    {"Cylinder",    CLOTH_COL_CYLINDER},
    {"Balloon",        CLOTH_COL_BALLOON},
#if CLOTH_SUPPORT_BOX_COL
    {"Box",            CLOTH_COL_BOX},
#endif
    DEFINE_END
};

TokenizerParseInfo ParseClothBoneInfo[] =
{
    { "BoneName",        TOK_STRUCTPARAM|TOK_STRING(ClothBoneInfo, nodename,    0),    0},
    { "NextBone",        TOK_STRING(ClothBoneInfo, nextname,    0),    0},
    { "Type",            TOK_INT(ClothBoneInfo, type,    0),    ShapeTypeEnum},
    { "Radius",            TOK_F32(ClothBoneInfo, rad,    0),    0},
    { "Exten1",            TOK_F32(ClothBoneInfo, exten1,    0),    0},
    { "Exten2",            TOK_F32(ClothBoneInfo, exten2,    0),    0},
    { "Offset",            TOK_VEC3(ClothBoneInfo, offset)},
    { "Direction",        TOK_VEC3(ClothBoneInfo, dir)},
    { "Dir",            TOK_REDUNDANTNAME|TOK_VEC3(ClothBoneInfo, dir)},
#if SUPPORT_BOX_COL
    { "DirX",            TOK_VEC3(ClothBoneInfo, dirx),    0},
    { "DirY",            TOK_VEC3(ClothBoneInfo, diry),    0},
#endif
    { "SameAs",            TOK_STRING(ClothBoneInfo, sameAs,    0),    0},
    { "SpecialFlag",    TOK_REDUNDANTNAME | TOK_BOOLFLAG(ClothBoneInfo, specialBackwardsFlag,    0),    0},
    { "SpecialBackwardsFlag",    TOK_BOOLFLAG(ClothBoneInfo, specialBackwardsFlag,    0),    0},
    { "SpecialForwardsFlag",    TOK_BOOLFLAG(ClothBoneInfo, specialForwardsFlag,    0),    0},
    { "End",            TOK_END,        0},
    { "", 0, 0 },
};

TokenizerParseInfo ParseClothColInfoLOD[] =
{
    { "Bone",            TOK_STRUCT(ClothColInfoLOD, boneInfo, ParseClothBoneInfo) },
    { "End",            TOK_END,        0},
    { "", 0, 0 },
};

TokenizerParseInfo ParseClothColInfo[] =
{
    { "Type",            TOK_STRING(ClothColInfo, type,    0),    0},
    { "LOD",            TOK_STRUCT(ClothColInfo, lod, ParseClothColInfoLOD) },
    { "End",            TOK_END,        0},
    { "", 0, 0 },
};

TokenizerParseInfo ParseClothColInfoFile[] =
{
    { "ColInfo",        TOK_STRUCT(ClothColInfoHolder, colInfoList, ParseClothColInfo) },
    { "End",            TOK_END,        0},
    { "", 0, 0 },
};

static void reloadColInfoCallback(const char *relpath, int when) {
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    loadClothColInfo();
    clothColReload = 2;
}


bool loadClothColInfoPreprocessor(TokenizerParseInfo pti[], void* structptr)
{
    ClothColInfo **colInfoList = ((ClothColInfoHolder*)structptr)->colInfoList;
    int i, j, k, numtypes = eaSize(&colInfoList);
    bool ret = true;
    for (i=0; i<numtypes; i++) {
        ClothColInfo *colInfo = colInfoList[i];
        int numlods = eaSize(&colInfo->lod);
        for (j=0; j<numlods; j++) {
            ClothColInfoLOD *lod = colInfo->lod[j];
            int numbones = eaSize(&lod->boneInfo);
            for (k=0; k<numbones; k++) {
                ClothBoneInfo * boneInfo = lod->boneInfo[k];
                if (boneInfo->sameAs && boneInfo->sameAs[0]) {
                    // Find one on same level and copy values
                    ClothBoneInfo * boneInfoSource = NULL;
                    int ii;
                    for (ii=0; ii<numbones; ii++) {
                        if (stricmp(lod->boneInfo[ii]->nodename, boneInfo->sameAs)==0) {
                            boneInfoSource = lod->boneInfo[ii];
                            break;
                        }
                    }
                    if (!boneInfoSource) {
                        Errorf("Could not find SameAs Bone '%s'", boneInfo->sameAs);
                        ret=0;
                    } else {
                        // Copy values
                        copyVec3(boneInfoSource->dir, boneInfo->dir);
                        copyVec3(boneInfoSource->dirx, boneInfo->dirx);
                        copyVec3(boneInfoSource->diry, boneInfo->diry);
                        copyVec3(boneInfoSource->offset, boneInfo->offset);
                        boneInfo->exten1 = boneInfoSource->exten1;
                        boneInfo->exten2 = boneInfoSource->exten2;
                        boneInfo->rad = boneInfoSource->rad;
                        boneInfo->specialBackwardsFlag = boneInfoSource->specialBackwardsFlag;
                        boneInfo->specialForwardsFlag = boneInfoSource->specialForwardsFlag;
                        boneInfo->type = boneInfoSource->type;
                    }
                }
                normalVec3(boneInfo->dir);
#if SUPPORT_BOX_COL
                normalVec3(boneInfo->dirx);
                normalVec3(boneInfo->diry);
#endif
                if (boneInfo->specialBackwardsFlag) {
                    boneInfo->type |= CLOTH_COL_SPECIAL_BACKWARDS_FLAG;
                }
                if (boneInfo->specialForwardsFlag) {
                    boneInfo->type |= CLOTH_COL_SPECIAL_FORWARDS_FLAG;
                }
            }
        }
    }
    return ret;
}

void loadClothColInfo()
{
    static bool inited=false;
    int i, iSize;
    if (g_colInfoList.colInfoList) {
        ParserDestroyStruct(ParseClothColInfoFile, &g_colInfoList);
    }
    // Load list of collision descriptions
    ParserLoadFiles(NULL, "cloth/ColInfo.txt", "clothColInfo.bin", 0, ParseClothColInfoFile, &g_colInfoList, NULL, NULL, loadClothColInfoPreprocessor);

    if (htColInfos) {
        stashTableDestroy(htColInfos);
        htColInfos = NULL;
    }

    // make hashtable
    htColInfos = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
    iSize = eaSize(&g_colInfoList.colInfoList);
    for (i=0; i<iSize; i++) {
        stashAddPointer(htColInfos, g_colInfoList.colInfoList[i]->type, g_colInfoList.colInfoList[i], false);
    }

    if (!inited) {
        // Add callback for re-loading
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "cloth/*.txt", reloadColInfoCallback);
        inited = true;
    }
}

ClothColInfo *getColInfo(const char *seqType)
{
    return stashFindPointerReturnPointer(htColInfos, seqType);
}

ClothColInfoLOD *getColInfoLOD(const char *seqType, int lod)
{
    ClothColInfo *colInfo = stashFindPointerReturnPointer(htColInfos, seqType);
    if (colInfo) {
        lod = MIN(lod, eaSize(&colInfo->lod)-1);
        return colInfo->lod[lod];
    }
    return NULL;
}



// Calculated each frame and stored because the collisions might be
//   partially updated more than once in a frame
// Stored in a local array, LocalColData[][]
typedef struct ClothBodyColData
{
    int skip;
    Vec3 colpos1, colpos2;
    F32  rad, exten1, exten2;
#if SUPPORT_BOX_COL
    Vec3 xvec, yvec, zvec;
#endif
} ClothBodyColData;

static ClothBodyColData LocalColData[MAX_NUM_LODS][MAX_CLOTH_COL];


//////////////////////////////////////////////////////////////////////////////
// COLLISIONS

// Process all of the collision data for LOD 'lod' and calculate
//   the collision primitive positions.
// 'entmat' is the matrix for the entity, used for collisions not associated
//   with a bone.
// 'colBackwardsFlag' is used to flag backwards motion which uses special collision
// Does not update the Cloth collisions (see clothNodeUpdateClothCollisions())
static void clothNodeUpdateCollisionData(Cloth *cloth, int lod, Mat4 entmat, SeqInst * seq, GfxNode *clothnode, int colBackwardsFlag, int colForwardsFlag)
{
    int i;
    int tlod = MIN(lod, MAX_NUM_LODS-1);
    ClothObject * clothobj = clothnode->clothobj;
    ClothNodeData * nodedata = clothobj->GameData;
    //char *seqType = nodedata->seqType; // seq->type.name;

#if CLOTH_DEBUG
    {
        static int no_collisions = 0;
        if (no_collisions)
            return;
    }
#endif
    // Collisions
    switch(nodedata->type)
    {
      case CLOTH_TYPE_FLAG:
        nodedata->seqType = "flag";
      case CLOTH_TYPE_TAIL:
      case CLOTH_TYPE_CAPE:
      {
        ClothCapeData * capedata = (ClothCapeData *)nodedata;
        ClothColInfoLOD *colInfoLOD = getColInfoLOD(nodedata->seqType, tlod);
        int numbones;
        if (!colInfoLOD) {
            // fall back to male
            colInfoLOD = getColInfoLOD("male", tlod);
        }
        numbones = eaSize(&colInfoLOD->boneInfo);
        for (i=0; i<numbones; i++)
        {
            GfxNode *bonenode;
            Mat4 bonemat1,bonemat2;
            Vec3 offset;
            int coltype;
            ClothBoneInfo *boneinfo = colInfoLOD->boneInfo[i];
            ClothBodyColInfo *colinfo = &capedata->colinfo[tlod][i];
            ClothBodyColData *coldata = &LocalColData[tlod][i];
            
            if (colinfo->bonenum1 >= 0)
            {
                bonenode = gfxTreeFindBoneInAnimation( colinfo->bonenum1, seq->gfx_root->child, seq->handle, 1 );
                gfxTreeFindWorldSpaceMat(bonemat1, bonenode);
                subVec3(bonemat1[3], cloth->PositionOffset, bonemat1[3]);
            }
            else
            {
                copyMat4(entmat, bonemat1);
            }
            //mulVecVec3(boneinfo->offset, capedata->scale, offset);
            copyVec3(boneinfo->offset, offset); // JE: For some reason *not* scaling this offset seems to get the results I'd expect...
            mulVecMat4(offset, bonemat1, coldata->colpos1);

            if (boneinfo->type & CLOTH_COL_SPECIAL_BACKWARDS_FLAG)
                coldata->skip = colBackwardsFlag ? 0 : 1;
            else if (boneinfo->type & CLOTH_COL_SPECIAL_FORWARDS_FLAG)
                coldata->skip = colForwardsFlag ? 0 : 1;
            else
                coldata->skip = 0;
            if (coldata->skip)
                continue;
            
            coltype = boneinfo->type & ~(CLOTH_COL_SPECIAL_FORWARDS_FLAG|CLOTH_COL_SPECIAL_BACKWARDS_FLAG);

            switch (coltype)
            {
              default:
              case CLOTH_COL_BALLOON:
              case CLOTH_COL_CYLINDER:
              {
                  if (colinfo->bonenum2 >= 0)
                  {
                      bonenode = gfxTreeFindBoneInAnimation( colinfo->bonenum2, seq->gfx_root->child, seq->handle, 1 );
                      gfxTreeFindWorldSpaceMat(bonemat2, bonenode);
                      subVec3(bonemat2[3], cloth->PositionOffset, bonemat2[3]);
                      mulVecMat4(offset, bonemat2, coldata->colpos2);
                  }
                  else
                  {
                      Vec3 tdir1,tdir2;
                      mulVecVec3(boneinfo->dir, capedata->scale, tdir1);
                      mulVecMat3(tdir1, bonemat1, tdir2);
                      addVec3(coldata->colpos1, tdir2, coldata->colpos2);
                  }
                  coldata->rad = boneinfo->rad * capedata->avgscale;
                  coldata->exten1 = boneinfo->exten1 * capedata->avgscale;
                  coldata->exten2 = boneinfo->exten2 * capedata->avgscale;
                  break;
              }
              case CLOTH_COL_PLANE:
              {
                  Vec3 tdir1;
                  mulVecVec3(boneinfo->dir, capedata->scale, tdir1);
                  mulVecMat3(tdir1, bonemat1, coldata->colpos2);
                  break;
              }
              case CLOTH_COL_SPHERE:
              {
                  coldata->rad = boneinfo->rad * capedata->avgscale;
                  break;
              }
#if SUPPORT_BOX_COL
              case CLOTH_COL_BOX:
              {
                  mulVecMat3(boneinfo->dirx, bonemat1, coldata->xvec);
                  mulVecMat3(boneinfo->diry, bonemat1, coldata->yvec);
                  mulVecMat3(boneinfo->dir,  bonemat1, coldata->zvec);
                  coldata->exten1 = boneinfo->exten1 * capedata->avgscale;
                  coldata->exten2 = boneinfo->exten2 * capedata->avgscale;
                  coldata->rad    = boneinfo->rad * 2.0f * capedata->avgscale;
                  break;
              }
#endif
            }
        }
        break;
      }
      default:
        break;
    }
}

// Update the Cloth collision primitives.

// 'frac' is used to enable multiple partial collision updates during a frame. 
//   It is the weight between the previous collision position and the calculated
//   collision position.
//   NOTE: Because the update overwrites the previous collision positions, frac
//   is the weight between the previous update and the calculated posiotn.
//   Thus if 3 updates occur during a frame, instead of using .33, .67, and 1.0,
//     the values .33, .5 (halway between .33 and 1.0), and 1.0
//  NOTE! 'frac' is not currently used; testing using intermediate values
//   produced noisy results, I am not certain why. -SJB.

static void clothNodeUpdateClothCollisions(Cloth *cloth, int lod, GfxNode *clothnode, F32 frac)
{
    int i;
    int tlod = MIN(lod, MAX_NUM_LODS-1);
    ClothObject * clothobj = clothnode->clothobj;
    ClothNodeData * nodedata = clothobj->GameData;
    ClothCol *clothcol;

    // Collisions
    switch(nodedata->type)
    {
      case CLOTH_TYPE_TAIL:
      case CLOTH_TYPE_CAPE:
      case CLOTH_TYPE_FLAG:
      {
        ClothCapeData * capedata = (ClothCapeData *)nodedata;
        for (i=0; i<MAX_CLOTH_COL; i++)
        {
            ClothBodyColData *coldata = &LocalColData[tlod][i];

            clothcol = ClothGetCollidable(cloth, i);
            if (!clothcol)
                break;
            if (coldata->skip)
                clothcol->Type |= CLOTH_COL_SKIP;
            else
                clothcol->Type &= ~CLOTH_COL_SKIP;

            switch (clothcol->Type)
            {
              case CLOTH_COL_BALLOON:
                ClothColSetBalloon( clothcol, coldata->colpos1, coldata->colpos2,
                                    coldata->rad, coldata->exten1, coldata->exten2, frac);
                break;
              case CLOTH_COL_CYLINDER:
                ClothColSetCylinder( clothcol, coldata->colpos1, coldata->colpos2,
                                    coldata->rad, coldata->exten1, coldata->exten2, frac);
                break;
              case CLOTH_COL_PLANE:
                ClothColSetPlane(clothcol, coldata->colpos1, coldata->colpos2, frac);
                break;
              case CLOTH_COL_SPHERE:
                ClothColSetSphere(clothcol, coldata->colpos1, coldata->rad, frac);
                break;
#if SUPPORT_BOX_COL
              case CLOTH_COL_BOX:
                ClothColSetBox(clothcol, coldata->colpos1, coldata->zvec, coldata->xvec, coldata->yvec,
                               coldata->rad, coldata->exten1, coldata->exten2, frac);
                break;
#endif
              default:
                break;
            }

#if CLOTH_DEBUG
            // For debugging ease, collision meshes are deleted when hidden and
            //  created when shown so that run-time modifications can be seen.
            if ((cloth->Flags & CLOTH_FLAG_DEBUG_COLLISION) && !(clothcol->Type & CLOTH_COL_SKIP))
            {
                ClothMesh *mesh;
                if (!clothcol->Mesh)
                {        
                    ClothColCreateMesh(clothcol, 0);
                    mesh = clothcol->Mesh;
                    mesh->TextureData[0].base = mesh->TextureData[1].base = white_tex_bind;
                    mesh->TextureData[0].generic = mesh->TextureData[1].generic = white_tex;
                }
            }
            else if (clothcol->Mesh)
            {
                ClothMeshDelete(clothcol->Mesh);
                clothcol->Mesh = 0;
            }
#endif
        }
        break;
      }
      default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////
// LOD

// See ClothObject.c for information on cloth LODs
static int set_detail(ClothObject *obj, F32 camdist, int isplayer)
{
    int detail;
    camdist -= cloth_lod_mindist;
    if (camdist <= 0) {
        detail = 0;
    } else {
        int numlod = ClothObjectDetailNum(obj);
        detail = 1 + (int)((camdist / cloth_lod_maxdist) * (numlod-1));
    }
    if (!isplayer)
        detail++;
    if (game_state.clothLOD)
        detail = game_state.clothLOD;
    return ClothObjectSetDetail(obj, detail);
}

//////////////////////////////////////////////////////////////////////////////
// UPDATE
void updateCloth(SeqInst * seq, GfxNode *clothnode, const Mat4Ptr worldmat, F32 camdist, int numhooks, Vec3 *hooks, Vec3 *hookNormals, const Vec3 positionOffset, ClothWindInfo *windInfoOverride)
{
    int i,lod,colBackwardsFlag,colForwardsFlag,freeze;
    int isplayer;
    ClothObject * clothobj = clothnode->clothobj;
    ClothNodeData * nodedata = clothobj->GameData;
    ClothCapeData * capedata = (ClothCapeData *)nodedata;
    Cloth *cloth;
    F32 dt;
    Mat4 chestmat;
    Mat4 entmat;
    int init=0;
    GfxNode *chestnode;

    clothobj->debugHookPos = hooks;
    clothobj->debugHookNormal = hookNormals;

    isplayer = (playerPtr() && (seq == playerPtr()->seq)) || (playerPtrForShell(0) && (seq == playerPtrForShell(0)->seq));

    if (seq || (nodedata->type == CLOTH_TYPE_FLAG && playerPtr())) {
        SeqInst *effSeq = seq?seq:playerPtr()->seq;
        if (seq)
        {
            clothnode->alpha = seq->seqGfxData.alpha; // Copy translucency from entity
            clothnode->seqGfxData = &effSeq->seqGfxData;
        }
        if (clothColReload) {
            clothColReload = 1;
            if (nodedata->type == CLOTH_TYPE_CAPE || nodedata->type == CLOTH_TYPE_FLAG) {
                Vec3 capescale;
                copyVec3(effSeq->currgeomscale, capescale);
#if 1
                capescale[1] *= .75f; // CLOTHFIX: Temp Hack
#endif
                for (i=0; i<clothobj->NumLODs; i++)
                {
                    initEntityCapeCol(clothobj, i, capescale, nodedata->seqType);
                }
            }
        }
    }

    if (seq) {
        copyMat4(seq->gfx_root->mat, entmat);
    } else {
        copyMat4(worldmat, entmat);
    }
    subVec3(entmat[3], positionOffset, entmat[3]);

    // Precalc some values (only affects the first time the cape is ran and is being spun up
    switch(nodedata->type)
    {
        case CLOTH_TYPE_TAIL:
        case CLOTH_TYPE_CAPE:
        
            // This is slow, if we can fix gfxNodes so they don't get recreated in animSetHeaders, this could be cached
            chestnode = gfxTreeFindBoneInAnimation(BONEID_CHEST, seq->gfx_root->child, seq->handle, 1);
            gfxTreeFindWorldSpaceMat(chestmat, chestnode);
            subVec3(chestmat[3], positionOffset, chestmat[3]);
            break;
        case CLOTH_TYPE_FLAG:
            if (seq) {
                gfxTreeFindWorldSpaceMat(chestmat, seq->gfx_root->child);
            } else {
                copyMat4(worldmat, chestmat);
            }
            subVec3(chestmat[3], positionOffset, chestmat[3]);
            break;
    }

    copyVec3(positionOffset, clothnode->mat[3]);
    //copyMat4(chestmat, clothnode->mat);

    // Time Step
    if (global_state.frame_time_30_real < 11.0f*global_state.frame_time_30)
        dt = global_state.frame_time_30 * (1.0f/30.0f);
    else
        dt = 0.0f;

    //printf("dt orig: %f ", dt);
    if (USE_AVGTIME)
    {
        if (global_state.frame_time_30_real == global_state.frame_time_30)
        {
            // smooth out frame time
            F32 avgdt;
            //printf("dtfact: %d ", nodedata->avgdtfactor);
            if (nodedata->avgdtfactor < 30) {
                nodedata->avgdtfactor++;
                avgdt = nodedata->avgdt * ((nodedata->avgdtfactor-1)/(float)nodedata->avgdtfactor) + dt * (1.f/nodedata->avgdtfactor);
            } else {
                avgdt = nodedata->avgdt * (29.f/30.f) + dt * (1.f/30.f);
            }
            nodedata->avgdt = avgdt;
            dt = avgdt;
        }
    }
    //printf("dt result: %f \n", dt);

    do { // Do multiple times to initialize the cloth node
        init = 0;
        // Time
        // Check for skipped frames
        if (global_state.global_frame_count && (!nodedata->frame || global_state.global_frame_count - nodedata->frame > CLOTH_MAX_NO_UPDATE_FRAMES))
        {
            nodedata->readycount = 0;
            init=1;
            //dt = 1.0f/30.0f; // Hack to make first frames consistent?
        }
        else if (nodedata->readycount != 0xffffffff)
        {
            nodedata->readycount++;
            if (nodedata->readycount >= CLOTH_PRE_RENDER_FRAMES)
                nodedata->readycount = 0xffffffff; // Node is ready to render
        }
        nodedata->frame = global_state.global_frame_count;

        //nodedata->readycount = 0xffffffff; // debug hack

        // LOD
        {
            lod = set_detail(clothobj, camdist, isplayer);
            cloth = ClothObjGetLODCloth(clothobj, lod);
            if (init) {
                // Initialize cloth
                copyVec3(positionOffset, cloth->PositionOffset);
                dt = 0; // to cause a freeze
                freeze = 1;
            }
        }

        ClothNodeVisNum++;

        // Flags
    #if CLOTH_DEBUG
        cloth->Flags &= ~CLOTH_FLAG_DEBUG;
        if ((game_state.clothDebug&CLOTH_FLAG_DEBUG)) cloth->Flags |= (game_state.clothDebug&CLOTH_FLAG_DEBUG);
    #endif

        if (dt == 0.0f)
            freeze = 1;
        else
            freeze = 0;

        // Update cloth internal PositionOffset to match with that of the inputs
        ClothOffsetInternalPosition(cloth, positionOffset);
        
        // Get Root Position
        switch(nodedata->type)
        {
        case CLOTH_TYPE_TAIL:
        case CLOTH_TYPE_CAPE:
        {
            if (!freeze)
            {
                F32 moveamt;
                Vec3 movedir;
                F32 entmoveamt;
                Vec3 entmovedir;
                Vec3 pyr0, pyr1, dpyr;
                F32 scale = 1/dt;
                ClothWindInfo *windInfo= (nodedata->type == CLOTH_TYPE_TAIL) ? WIND_INFO_TAIL : seq->animation.move->windInfo;
                if (windInfoOverride) windInfo = windInfoOverride;

                subVec3(chestmat[3], cloth->Matrix[3], movedir); // movement since last frame
                moveamt = normalVec3(movedir); // movement direction
                moveamt = (dt == 0.0f) ? 0.0f : moveamt * scale;

                subVec3(entmat[3], cloth->EntMatrix[3], entmovedir); // entity movement since last frame
                entmoveamt = normalVec3(entmovedir); // entity movement direction
                entmoveamt = (dt == 0.0f) ? 0.0f : entmoveamt * scale;

                getMat3YPR(cloth->Matrix, pyr0); // Slow, store this each frame?
                getMat3YPR(chestmat, pyr1);
                subVec3(pyr1, pyr0, dpyr);
                while (dpyr[0]>PI) dpyr[0]-=PI*2;
                while (dpyr[0]<-PI) dpyr[0]+=PI*2;
                while (dpyr[1]>PI) dpyr[1]-=PI*2;
                while (dpyr[1]<-PI) dpyr[1]+=PI*2;
                while (dpyr[2]>PI) dpyr[2]-=PI*2;
                while (dpyr[2]<-PI) dpyr[2]+=PI*2;
                scaleVec3(dpyr, scale, dpyr);

                if (game_state.clothDebug && game_state.testDisplayValues) {
                    copyVec3(pyr1, game_state.testVec3);
                    copyVec3(dpyr, game_state.testVec4);
                }

                freeze = update_wind(cloth, windInfo, chestmat, movedir, moveamt, entmat, entmovedir, entmoveamt, dpyr);
            }
            break;
        }
        case CLOTH_TYPE_FLAG:
        {
            if (!freeze)
            {
                ClothWindInfo *windInfo = (nodedata->type == CLOTH_TYPE_FLAG) ? WIND_INFO_FLAG : WIND_INFO_TAIL;
                if (windInfoOverride) windInfo = windInfoOverride;
                freeze = update_wind(cloth, windInfo, chestmat, 0, 0, 0, 0, 0, 0);
            }
            break;
        }
        default:
            break;
        }

        // Check for special case: freeze = 2 -> use special collisions
        if (freeze == 3) {
            colBackwardsFlag = 0;
            colForwardsFlag = 1;
            freeze = 0;
        } else if (freeze == 2) {
            colBackwardsFlag = 1;
            colForwardsFlag = 0;
            freeze = 0;
        } else {
            colBackwardsFlag = 0;
            colForwardsFlag = 0;
        }
        
        // Update Cloth
        {
            static int min_nit = 1;
            static F32 min_player_fps = 60.f;
            static F32 min_fps = 30.f;
            int nit;
            F32 frac,dfrac;
            F32 tdt;

            // Check for large cloth motions. See ClothCheckMaxMotion() for details.
            freeze = ClothCheckMaxMotion(cloth, dt, chestmat, freeze);

            // Update collisions
            if (seq || (playerPtr() && playerPtr()->seq))
                clothNodeUpdateCollisionData(cloth, lod, entmat, seq?seq:playerPtr()->seq, clothnode, colBackwardsFlag, colForwardsFlag);

            // Calc nit = Number of Iterations
            if (freeze)
                nit = 1;
            else if (isplayer)
                nit = (int)(dt * min_player_fps);
            else
                nit = (int)(dt * min_fps);

            if (nit < min_nit)
                nit = min_nit;
            
            dfrac = 1.0f / (F32)nit;
            tdt = dt * dfrac;            // partial time step

            // For each iteration...
            for (i=0; i<nit; i++)
            {
                //F32 frac1 = (F32)(i+1) * dfrac;
                F32 frac2 = 1.0f / (nit - i);

                // (Partially) Update Hooks
                PERFINFO_AUTO_START("ClothUpdateHooks",1);
                ClothUpdateAttachments(cloth, hooks, frac2, freeze);
                PERFINFO_AUTO_STOP();

                // (Partially) Update Collisions
                PERFINFO_AUTO_START("ClothUpdateBody",1);
    #if 0
                // SJB: This produces lots of noise if nit > 0, I'm not sure why.
                clothNodeUpdateClothCollisions(cloth, lod, clothnode, frac2);
    #else
                if (i==0)
                    clothNodeUpdateClothCollisions(cloth, lod, clothnode, 1.0f);
    #endif
                PERFINFO_AUTO_STOP();

                // (Partially) Update Physics
                // Moves Particles, Checks Collisions, and iterates Constraints
                PERFINFO_AUTO_START("ClothObjectUpdatePhysics",1);
                ClothObjectUpdatePhysics(clothobj, tdt);
                PERFINFO_AUTO_STOP();
            }

        }
    } while (nodedata->readycount != 0xffffffff);


    copyMat4(entmat, cloth->EntMatrix);

    ClothCopyToRenderData(cloth, hookNormals, CCTR_COPY_HOOKS);

    cloth->SkipUpdatePhysics = 0; // reset flag after 'drawing' cloth

}

void hideCloth(GfxNode * clothnode)
{
    if (clothnode)
    {
        ClothObject * clothobj = clothnode->clothobj;
        ClothNodeData * nodedata = clothobj->GameData;
        nodedata->readycount = 0;
    }
}

//////////////////////////////////////////////////////////////////////////////
// CREATE

// Init collision Info for an entity, using the data in ClothBoneInfo[][]
static void initEntityCapeCol(ClothObject * clothobj, int lod, Vec3 scale, const char *seqType)
{
    Cloth *cloth;
    ClothCol *clothcol;
    int i,j,idx;
    Vec3 upvec;
    int tlod = MIN(lod, MAX_NUM_LODS-1);
    ClothColInfo *colInfo = getColInfo(seqType);
    ClothColInfoLOD *colInfoLOD = getColInfoLOD(seqType, tlod);
    int numbones;
    int numlods;


    if (!colInfoLOD || !colInfo) {
        if (isDevelopmentMode()) {
            static const char *last=NULL;
            if (seqType!=last && (!last || strcmp(last, seqType)!=0)) {
                Errorf("Missing cape collision info for sequencer type '%s'", seqType);
                last = seqType;
            }
        }
        // fall back to male?
        colInfo = getColInfo("male");
        colInfoLOD = getColInfoLOD("male", tlod);
    }
    assert(colInfo && colInfoLOD);
    numbones = eaSize(&colInfoLOD->boneInfo);
    numlods = eaSize(&colInfo->lod);

    setVec3(upvec, 0.0f, 1.0f, 0.0f);

    cloth = ClothObjGetLODCloth(clothobj, lod);

    // Create Collidables
    cloth->NumCollidables = 0;
    for (i=0; i<numbones; i++)
    {        
        ClothBoneInfo *boneinfo = colInfoLOD->boneInfo[i];
        
        idx = ClothAddCollidable(cloth); // idx == i
        clothcol = ClothGetCollidable(cloth, idx);
        clothcol->Type = boneinfo->type & ~(CLOTH_COL_SPECIAL_BACKWARDS_FLAG|CLOTH_COL_SPECIAL_FORWARDS_FLAG);
    }

    // Setup bone data
    {
        ClothCapeData * capedata = (ClothCapeData *)clothobj->GameData;
        copyVec3(scale, capedata->scale);
        capedata->avgscale = (scale[0] + scale[1] + scale[2])*.33333333f;
        
        for (j=0; j<numlods; j++)
        {
            colInfoLOD = colInfo->lod[j];
            numbones = eaSize(&colInfoLOD->boneInfo);
            for (i=0; i<numbones; i++)
            {
                ClothBoneInfo *boneinfo = colInfoLOD->boneInfo[i];
                capedata->colinfo[j][i].bonenum1 = bone_IdFromText(boneinfo->nodename);
                capedata->colinfo[j][i].bonenum2 = bone_IdFromText(boneinfo->nextname);
            }
        }
    }
}

// This is something of a hack.
//
// The top level LOD is input as 'cape'.
// 2 additional LODs are auto-generated (3 total).
//
// There are a bunch of tunable paramaters that really need to be put
//  into a text file or some such (see CLOTHFIX comment below).

// NOTE: There are a number of assumptions about the cape and harness data
//  (see ClothBuild.c).

GfxNode * initClothCapeNode(int * nodeId, const char* textures[4], U8 colors[4][4], Model *cape, Model *harness, Vec3 scale, F32 stiffness, F32 drag, F32 point_y_scale, F32 colrad, ClothType clothType, const char *seqType, const char *trickName)
{
    Vec3 *verts;
    Vec2 *weights;
    Vec2 *texcoords;
    int *tris;
    int nverts;
    int i,j,res,ntris;
    GfxNode * node;
    ClothObject * clothobj;
    ClothCapeData * capedata;
    
    nverts = cape->vert_count;
    verts = cape->vbo->verts;
    texcoords = cape->vbo->sts;
    weights = cape->vbo->weights;
    tris = cape->vbo->tris;
    ntris = cape->tri_count;
    
    ClothNodeNum++;
    
    // Create cloth
    {
        Cloth *cloth;
        F32 *masses1;
        // CLOTHFIX: Tunable cape paramaters
        //  'mass_y_scale' scales the masses in the first (and thus all) LODs.
        //  'point_y_scale' scales the X values of the points in subsequent LODs.
        //     This is a hack to deal with the fact that capes with fewer particles
        //     will be effectively stiffer and so will otherwise appear wider.
        //  'stiffness' determines the amount of 'fold' allowed.
        //     1.0 would attempt to keep the cloth completely flat
        //     0.7 would allow the cloth to fold to angles of about 90 degrees.
        //  'colrad' is the collision radius of each particle (I wouldn't mess with it...)
        //  'drag' is the wind resistance, and should remain in the range [0.10, 0.30]
        //  See ClothObject.c for how 'lod0_[min/max]sublod' is used.
        F32 mass_y_scale; // 1.0 if masses are provided, 0.4 otherwise
        int lod0_minsublod = 0;
        int lod0_maxsublod = 1;

        if (stiffness==0.0)
            stiffness = 0.90;
        if (drag==0.0)
            drag = 0.10f;
        if (point_y_scale==0.0)
            point_y_scale = .80f;
        if (colrad==0.0)
            colrad = 0.20f;

        colrad *= scale[1];

        clothobj = ClothObjectCreate();
        ClothObjectAddLOD(clothobj, lod0_minsublod, lod0_maxsublod);
        cloth = clothobj->LODs[0]->mCloth;

        masses1 = (F32*)malloc(nverts*sizeof(F32));
        for (i=0; i<nverts; i++)
            masses1[i] = weights[i][1]; // -should- be [i][0]...
        mass_y_scale = 1.0f;
        
        // Build LOD 0
        ClothBuildGridFromTriList(cloth, nverts, verts, texcoords, masses1, mass_y_scale, stiffness, ntris, tris, scale);
        // Attach the harness
        ClothBuildAttachHarness(cloth, harness->vert_count, harness->vbo->verts);

        // Create LOD 1,2
        ClothObjectCreateLODs(clothobj, 2, point_y_scale, stiffness);

        // Set some values
        ClothObjectSetColRad(clothobj, colrad);
        ClothObjectSetGravity(clothobj, CLOTH_GRAVITY);
        ClothObjectSetDrag(clothobj, drag);
        
        if (masses1)
            free(masses1);
    }

    node = gfxTreeInsert( 0 );
    *nodeId    = node->unique_id;
    node->clothobj = (void*)clothobj;

    if( cape && cape->trick )
    {
        gfxTreeInitGfxNodeWithObjectsTricks(node, cape->trick);
        node->trick_from_where = TRICKFROM_MODEL;
    } else if (trickName && trickName[0]) {
        TrickInfo    *trickinfo;
        trickinfo = trickFromName(trickName,"cape");
        if (trickinfo)
        {
            *(gfxTreeAssignTrick( node )) = trickinfo->tnode;
            node->tricks->info = trickinfo;
            node->trick_from_where = TRICKFROM_UNKNOWN;
        }
    }

    // Game Data
    capedata = (ClothCapeData *)calloc(sizeof(ClothCapeData),1);
    clothobj->GameData = (void *)capedata;
    capedata->type = clothType;
    capedata->seqType = allocAddString((char*)seqType);
    
    // Create meshes
    ClothObjectCreateMeshes(clothobj);

    // Setup mesh textures and colors and blend mode
    updateClothNodeParams(node, textures, colors);

    // Setup collisions
    if (clothType == CLOTH_TYPE_CAPE || (clothType == CLOTH_TYPE_FLAG && playerPtr())) {
        Vec3 effscale;
        copyVec3(scale, effscale);
        if (clothType == CLOTH_TYPE_FLAG && playerPtr()) {
            capedata->seqType = "flag";
            copyVec3(playerPtr()->seq->currgeomscale, effscale);
#if 1
            effscale[1] *= .75f; // CLOTHFIX: Temp Hack
#endif
        }
        for (i=0; i<clothobj->NumLODs; i++)
        {
            initEntityCapeCol(clothobj, i, effscale, capedata->seqType);
        }
    }

    // Set default LOD
    ClothObjectSetSubLOD(clothobj, 0);

    // Hack so we get through the model draw code (copied from splat shadow code)
    node->model = cape;
    if (!node->model) {
        node->model = modelFind("GEO_HIPS","player_library/G_Object.geo",LOAD_BACKGROUND, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
        //node->model->flags |= OBJ_ALPHASORT;
    }

    return node;
}

void updateClothNodeParamsInternal(GfxNode * node, TextureOverride TextureData[2], U8 colors[4][4])
{
    ClothObject * clothobj;
    int i, j;

    clothobj = (ClothObject *)node->clothobj;

    // Setup mesh textures
    for (i=0; i<clothobj->NumLODs; i++)
    {
        ClothLOD *clothlod = clothobj->LODs[i];
        Cloth *cloth = clothlod->mCloth;
        for (j=0; j<CLOTH_SUB_LOD_NUM; j++)
        {
            if (clothlod->Meshes[j]) {
                memcpy(clothlod->Meshes[j]->TextureData,  TextureData, sizeof(clothlod->Meshes[j]->TextureData));
                memcpy(clothlod->Meshes[j]->colors, colors, sizeof(clothlod->Meshes[j]->colors));
            }
        }
    }
    // Find the blend mode, but we only know how to draw colorblend_dual or better
    node->node_blend_mode = promoteBlendMode(BlendMode(BLENDMODE_COLORBLEND_DUAL,0), promoteBlendMode(TextureData[0].base->bind_blend_mode, TextureData[1].base->bind_blend_mode));

    if (!(node->tricks && (node->tricks->flags1&TRICK_ALPHACUTOUT)))
    {
        if (node->node_blend_mode.shader == BLENDMODE_MULTI) {
            if (texNeedsAlphaSort(TextureData[0].base, node->node_blend_mode) ||
                texNeedsAlphaSort(TextureData[1].base, node->node_blend_mode))
            {
                node->flags |= GFXNODE_ALPHASORT;
            }
        } else {
            // Color blend dual
            if (((TextureData[0].generic->flags & TEX_ALPHA) ||
                (TextureData[1].generic->flags & TEX_ALPHA)))
            {
                //node->model->flags |= OBJ_ALPHASORT;
                node->flags |= GFXNODE_ALPHASORT;
            }
        }
    }
}

void updateClothNodeParams(GfxNode * node, const char * textures[4], U8 colors[4][4])
{
    ClothObject * clothobj;
    int i, j;
    TextureOverride TextureData[2];

    clothobj = (ClothObject *)node->clothobj;

    TextureData[0].base = texLoad( textures[0], TEX_LOAD_IN_BACKGROUND, TEX_FOR_ENTITY);
    TextureData[0].generic = texLoadBasic( textures[1], TEX_LOAD_IN_BACKGROUND, TEX_FOR_ENTITY);
    TextureData[1].base = texLoad( textures[2], TEX_LOAD_IN_BACKGROUND, TEX_FOR_ENTITY);
    TextureData[1].generic = texLoadBasic( textures[3], TEX_LOAD_IN_BACKGROUND, TEX_FOR_ENTITY);

    updateClothNodeParamsInternal(node, TextureData, colors);
}


void resetClothNodeParams(GfxNode *node)
{
    // For trick reloading
    ClothObject * clothobj;
    int i, j;

    clothobj = (ClothObject *)node->clothobj;
    for (i=0; i<clothobj->NumLODs; i++)
    {
        ClothLOD *clothlod = clothobj->LODs[i];
        Cloth *cloth = clothlod->mCloth;
        for (j=0; j<CLOTH_SUB_LOD_NUM; j++)
        {
            if (clothlod->Meshes[j]) {
                updateClothNodeParamsInternal(node, clothlod->Meshes[j]->TextureData, clothlod->Meshes[j]->colors);
                return;
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////////
// DELETE
// Note: Kind of a hack to free clothobj->GameData

void freeClothNode( ClothObject *clothobj )
{
    ClothNodeData * nodedata = clothobj->GameData;
    if (nodedata)
    {
        if (nodedata->system) {
            partKillSystem(nodedata->system, nodedata->system->unique_id);
            nodedata->system = NULL;
        }
    }
    rdrQueue(DRAWCMD_CLOTHFREE, &clothobj, sizeof(clothobj));
}

//////////////////////////////////////////////////////////////////////////////
// Claled once per frame. Resets counter and updates World wind.

void updateClothFrame()
{
    ClothNodeVisNum = 0;
    calculateClothWindThisFrame();
    if (clothColReload==1) {
        clothColReload=0;
    }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// For testing purposes only

#include "test_cape.h"
#include "test_flag.h"
#include "test_tail.h"

GfxNode * initTestClothNode( int * nodeId, char * texture, int type )
{
    int i,j,res;
    GfxNode * node;
    ClothObject * clothobj;
    Vec3 scale;
    
    ClothNodeNum++;
    
    node = gfxTreeInsert( 0 );
    *nodeId    = node->unique_id;
    
    clothobj = ClothObjectCreate();
    setVec3(scale, 1.f, 1.f, 1.f);
    
    switch(type)
    {
      case CLOTH_TYPE_CAPE:
        res = ClothObjectParseText(clothobj, cape_text);
        break;
      case CLOTH_TYPE_FLAG:
        res = ClothObjectParseText(clothobj, flag_text);
        break;
      case CLOTH_TYPE_TAIL:
        res = ClothObjectParseText(clothobj, tail_text);
        break;
    }
    assert(res==0);
    
    ClothObjectCreateMeshes(clothobj);
    
    // Setup mesh textures and physics
    for (i=0; i<clothobj->NumLODs; i++)
    {
        ClothLOD *clothlod = clothobj->LODs[i];
        Cloth *cloth = clothlod->mCloth;
        for (j=0; j<CLOTH_SUB_LOD_NUM; j++)
        {
            ClothMesh *mesh = clothlod->Meshes[j];
            if (mesh)
            {
                mesh->TextureData[0].base = mesh->TextureData[1].base = white_tex_bind;
                mesh->TextureData[0].generic = mesh->TextureData[1].generic = white_tex;
                memset(mesh->colors, 0xff, sizeof(mesh->colors));
            }
        }
        switch(type)
        {
          case CLOTH_TYPE_TAIL:
          case CLOTH_TYPE_CAPE:
          {
              ClothCapeData *capedata = (ClothCapeData *)calloc(sizeof(ClothCapeData),1);
              clothobj->GameData = (void *)capedata;
              capedata->type = type;
              capedata->seqType = "male";
              initEntityCapeCol(clothobj, i, scale, capedata->seqType);
              break;
          }
          default:
          {
              ClothNodeData *nodedata = (ClothNodeData *)calloc(sizeof(ClothNodeData),1);
              clothobj->GameData = (void *)nodedata;
              nodedata->type = type;
              break;
          }
        }
        if (type == CLOTH_TYPE_FLAG)
        {
            cloth->Flags |= CLOTH_FLAG_RIPPLE_VERTICAL;
        }
    }

    ClothObjectSetSubLOD(clothobj, 0);
    
    node->clothobj = (void*)clothobj;
    
    //Hack so we get through the model draw code;
    node->model = modelFind("GEO_HIPS","player_library/G_Object.geo",LOAD_BACKGROUND, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
    node->model->flags |= OBJ_ALPHASORT;

    return node;
}

void updateClothHack(SeqInst * seq, GfxNode *clothnode, Mat4Ptr worldmat, F32 camdist)
{
    int i;
    Mat4 chestmat;
    ClothObject * clothobj = clothnode->clothobj;
    ClothNodeData * nodedata = clothobj->GameData;
    
    switch(nodedata->type)
    {
    case CLOTH_TYPE_CAPE:
        {
            ClothCapeData * capedata = (ClothCapeData *)nodedata;
            GfxNode *chestnode;
            chestnode = gfxTreeFindBoneInAnimation(BONEID_CHEST, seq->gfx_root->child, seq->handle, 1);
            gfxTreeFindWorldSpaceMat(chestmat, chestnode);

            for (i=0; i<CAPE_NUMHOOKS; i++)
                mulVecMat4(cape_hook_offsets[i], chestmat, cape_hook_curpos[i]);
            updateCloth(seq, clothnode, worldmat, camdist, CAPE_NUMHOOKS, cape_hook_curpos, NULL, zerovec3, NULL);
            break;
        }
    case CLOTH_TYPE_FLAG:
        {
            if (seq) {
                gfxTreeFindWorldSpaceMat(chestmat, seq->gfx_root->child);
            } else {
                copyMat4(worldmat, chestmat);
            }

            for (i=0; i<FLAG_NUMHOOKS; i++)
                mulVecMat4(flag_hook_offsets[i], chestmat, flag_hook_curpos[i]);

            updateCloth(seq, clothnode, worldmat, camdist, FLAG_NUMHOOKS, flag_hook_curpos, NULL, zerovec3, NULL);
            break;
        }
    case CLOTH_TYPE_TAIL:
        {
            //gfxTreeFindWorldSpaceMat(chestmat, seq->gfx_root->child);
            ClothCapeData * capedata = (ClothCapeData *)nodedata;
            GfxNode *chestnode;
            chestnode = gfxTreeFindBoneInAnimation(BONEID_CHEST, seq->gfx_root->child, seq->handle, 1);
            gfxTreeFindWorldSpaceMat(chestmat, chestnode);

            for (i=0; i<TAIL_NUMHOOKS; i++)
                mulVecMat4(tail_hook_offsets[i], chestmat, tail_hook_curpos[i]);

            updateCloth(seq, clothnode, worldmat, camdist, TAIL_NUMHOOKS, tail_hook_curpos, NULL, zerovec3, NULL);
            break;
        }
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////


#endif
