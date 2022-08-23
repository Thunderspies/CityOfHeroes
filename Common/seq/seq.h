#ifndef _SEQ_H
#define _SEQ_H
 
#include "seq/gfxtree.h"
#ifdef CLIENT
#include <utilitieslib/components/earray.h>
#endif

#include "TriggeredMove.h"

typedef struct GroupDef GroupDef;
typedef struct DefTracker DefTracker;
typedef struct Entity Entity;
typedef struct TrickInfo TrickInfo;
typedef struct SkeletonAnimTrack SkeletonAnimTrack;
typedef struct GfxNode GfxNode;
typedef struct ModelHeader ModelHeader;

#define MAX_LODS 4            //(including highest lod, lod 0)
#define HIGHEST_LOD_TO_HAVE_SHADOW 0
#define ANIM_STR_LEN 64 
#define MAX_ANIMATIONS 4
#define MAX_FUTUREMOVETRACKERS 4
#define MAX_SEQFX 6
#define MAX_SEQTYPENAME 50

#define SEQ_LOAD_FULL 0            //load animation tracks and other tracks
#define SEQ_LOAD_HEADER_ONLY 1  //only load headers and Models with no data
#define SEQ_LOAD_FULL_SHARED 2 //only load headers and Models with no data

#define SEQ_DEFAULT_CAMERA_HEIGHT 6.0
#define SEQ_DEFAULT_SHADOW_TEXTURE "PlayerShadowCircle"

enum
{
    BLEND_NONE        = (0),
    BLEND_ROTS        = (1 << 0),
    BLEND_XLATES    = (1 << 1),
};

enum
{
    STATIC_LIGHT_NOT_DONE            = 0,
    STATIC_LIGHT_DONE                = 1,
    STATIC_LIGHT_ON_CREATE_FAILED    = 2,
};

typedef enum
{
    SEQ_NO_SHADOW            = 0,
    SEQ_SPLAT_SHADOW        = 1,
} eShadowType;

typedef enum
{
    SEQ_ENTCOLL_CAPSULE            = 0,    //default ent collisoin type. hard hit
    SEQ_ENTCOLL_REPULSION        = 1,    //push away like a magnet
    SEQ_ENTCOLL_BOUNCEUP        = 2,    //push straight up
    SEQ_ENTCOLL_BOUNCEFACING    = 3,    //push in the direction the ent is facing
    SEQ_ENTCOLL_STEADYUP        = 4,    //push smoothly up
    SEQ_ENTCOLL_STEADYFACING    = 5,    //push smoothly in the direction the ent is facing
    SEQ_ENTCOLL_LAUNCHUP        = 6,    //
    SEQ_ENTCOLL_LAUNCHFACING    = 7,    //
    SEQ_ENTCOLL_DOOR            = 8,    //do no normal entity collision, insert yourself in the world collision instead like a Door using the GEO_COLLISION object of the default graphics file
    SEQ_ENTCOLL_LIBRARYPIECE    = 9,    //as door, but use the world group instead
    SEQ_ENTCOLL_NONE            = 10,   //No collision at all
    SEQ_ENTCOLL_BONECAPSULES    = 11,    // collide with type->boneCapsules as normal capsule collision

} eCollType;

typedef enum
{
    SEQ_SELECTION_BONES            = 1,    //default
    SEQ_SELECTION_CAPSULE        = 2,
    SEQ_SELECTION_LIBRARYPIECE    = 3,
    SEQ_SELECTION_DOOR            = 4,
    SEQ_SELECTION_BONECAPSULES    = 5,    // use default if it fits on screen, otherwise locate closest surface
} eSelection;

typedef enum
{
    SEQ_PLACE_SETBACK        = 0,
    SEQ_PLACE_DEAD_ON        = 1,
} ePlacement;
//Rem character is looking down positive z, so this is really an amount to move in negative z
#define COMBATBIAS_DISTANCE 0.8

typedef enum
{
    SEQ_LOW_QUALITY_SHADOWS        = 1,
    SEQ_MEDIUM_QUALITY_SHADOWS    = 2,
    SEQ_HIGH_QUALITY_SHADOWS    = 3,
} eShadowQuality;

typedef enum
{
    SEQ_NO_SHALLOW_SPLASH        = (1 << 0),
    SEQ_NO_DEEP_SPLASH            = (1 << 1),
    SEQ_USE_NORMAL_TARGETING    = (1 << 2),
    SEQ_USE_DYNAMIC_LIBRARY_PIECE    = (1 << 3),
    SEQ_USE_CAPSULE_FOR_RANGE_FINDING = (1 << 4),
    SEQ_PLAY_FX_IF_BONE_MISSING = (1 << 5),
} eSeqTypeFlags;

typedef enum
{
    SEQ_REJECTCONTFX_NONE        = 0,
    SEQ_REJECTCONTFX_ALL        = 1,
    SEQ_REJECTCONTFX_EXTERNAL    = 2,
} eRejectContinuousFX;

typedef enum
{
    HEALTHFX_NO_COLLISION            = (1 << 0),

} eHealthFxFlags;

enum
{
    ANIM_SHOW_USEFLAGS                = (1 << 0),
    ANIM_SHOW_MY_ANIM_USE            = (1 << 1),
    ANIM_SHOW_SELECTED_ANIM_USE        = (1 << 2),
};

////////////////////////////////////////////////////////////////////
/// Seq Move Stuff 

#define MAX_CYCLE_MOVES 5
#define MAX_NEXT_MOVES 4
#define MAX_IRQ_ARRAY_SIZE 14
#define MAX_SPARSE_BITS 12
#define MAX_BONE_BITFIELD ((BONEID_COUNT+31)/32)

typedef struct SparseBits
{
    U16 bits[MAX_SPARSE_BITS];
    U32 count;
} SparseBits;

typedef struct SeqMoveRaw
{
    //State Machine
    SparseBits    required;            //Bits required to be set to fire this move        

    U32            memberBitArray[MAX_IRQ_ARRAY_SIZE];
    U32            interruptsBitArray[MAX_IRQ_ARRAY_SIZE];

    U16            idx;                                //array position--i'm always getting it, so lets just store it for clarity
    U8            nextMoveCnt;                        
    U8            cycleMoveCnt;

    U16            nextMove[MAX_NEXT_MOVES];            //randomly pick one of these
    U16            cycleMove[MAX_CYCLE_MOVES];            //Instead of cycling, go to one of these

} SeqMoveRaw;

/////////// ABOVE RAW    ////////////////////////////////////////
/////////// BELOW PARSE  ////////////////////////////////////////

typedef struct SeqFx
{
    const char  * name; // in string cache, do not cast away const!
    int            delay;
    int            flags;
} SeqFx;

typedef struct AnimP
{
    const char    * name;
    int        firstFrame;
    int        lastFrame;
} AnimP;

typedef struct TypeGfx
{
    const char *  type;
    const AnimP ** animP;
    const SeqFx ** seqFx;
    const SkeletonAnimTrack * animTrack;    //runtime Ptr to the animation track for this animation 
    int        ragdollFrame; // which frame to start ragdoll on, defaults to negative which means no ragdoll
    F32        scale;        //scale and moverate are duplicated on move for convenience
    F32        moveRate;
    F32        pitchAngle;
    F32        pitchRate;
    F32        pitchStart;
    F32        pitchEnd;
    F32        smoothSprint; //for smoothing out scaled animations
    const char * scaleRootBone; //Bone whose position should remain static when character is scaled
    const char * filename;
} TypeGfx;

typedef struct CycleMoveP
{
    const char * name;
} CycleMoveP;

typedef struct NextMoveP
{
    const char * name;
} NextMoveP;

typedef struct ClothWindInfo ClothWindInfo;

typedef struct SeqMove
{
    const char *  name;
    F32        scale;
    F32        moveRate;
    int        interpolate;
    int        priority;
    int        flags;

    SeqMoveRaw raw;

    const TypeGfx        ** typeGfx;        

    //Below can be freed after preprocessing step. Values are rewritten to raw.
    const NextMoveP    ** nextMove;
    const CycleMoveP    ** cycleMove;
    const char ** sticksOnChildStr;    //list of bits
    const char ** setsOnChildStr;    //list of bits
    const char ** setsOnReactorStr;    //list of bits
    const char ** setsStr;            //list of bits
    const char ** requiresStr;        //list of bits

    //Below can be freed after preprocessing if I had a way to handle dynamic allocing in raw
    const char ** memberStr;            //List of groups    
    const char ** interruptsStr;        //List of strings of moves and groups

    const char * windInfoName;
    const ClothWindInfo *windInfo;

} SeqMove;

typedef struct TypeP
{
    const char * name;
    const char * baseSkeleton;
    const char * parentType;
} TypeP;

typedef struct GroupP
{
    const char * name;
} GroupP;

typedef struct SeqBitInfo
{
    int *movesRemainingThatRequireMe; //Moves that require me, but are not required by any lower numbered bit (no need to evaluate the move again)
    int cnt;
} SeqBitInfo;

typedef struct SeqInfo
{
    const struct SeqInfo * next; //development only for listing non binary loaded bhvrs
    const struct SeqInfo * prev;

    const char * name;

    const SeqBitInfo * bits;  //Calculated after file load in FinalProcessor

    const TypeP  ** types;    //Array of structs
    const GroupP ** groups;    //Array of groups
    const SeqMove  ** moves;    //Array of structs

    int fileAge;   //development only
    int initialized;

} SeqInfo;

typedef struct SeqInfoList {
    const SeqInfo ** seqInfos;
    cStashTable seqInfoHashes;
} SeqInfoList;

extern SERVER_SHARED_MEMORY SeqInfoList seqInfoList;

typedef enum
{
    SEQMOVE_CYCLE            = (1 << 0),
    SEQMOVE_GLOBALANIM        = (1 << 1),        //Unused?
    SEQMOVE_FINISHCYCLE        = (1 << 2),
    SEQMOVE_REQINPUTS        = (1 << 3),
    SEQMOVE_COMPLEXCYCLE    = (1 << 4),
    SEQMOVE_NOINTERP        = (1 << 5),
    SEQMOVE_HITREACT        = (1 << 6),
    SEQMOVE_NOSIZESCALE        = (1 << 7),
    SEQMOVE_MOVESCALE        = (1 << 8),
    SEQMOVE_NOTSELECTABLE    = (1 << 9),
    SEQMOVE_SMOOTHSPRINT    = (1 << 10),
    SEQMOVE_PITCHTOTARGET    = (1 << 11),
    SEQMOVE_PICKRANDOMLY    = (1 << 12),
    SEQMOVE_FULLSIZESCALE    = (1 << 13), //default is CAPPEDSIZESCALE, alternates are NOSIZESCALE and FULLSIZESCALE
    SEQMOVE_ALWAYSSIZESCALE    = (1 << 14),
    SEQMOVE_FIXARMREG        = (1 << 15),
    SEQMOVE_HIDE            = (1 << 16),
    SEQMOVE_NOCOLLISION        = (1 << 17),
    SEQMOVE_WALKTHROUGH        = (1 << 18),
    SEQMOVE_CANTMOVE        = (1 << 19),
    SEQMOVE_RUNIN            = (1 << 20),
    SEQMOVE_PREDICTABLE        = (1 << 21), //Calcualted only, cant be set in text file
    SEQMOVE_FIXHIPSREG        = (1 << 22),
    SEQMOVE_DONOTDELAY        = (1 << 23), //Calcualted only, cant be set in text file
    SEQMOVE_FIXWEAPONREG    = (1 << 24),
    SEQMOVE_NODRAW            = (1 << 25),
    SEQMOVE_COSTUMECHANGE    = (1 << 26),
    SEQMOVE_HIDEWEAPON        = (1 << 27),
    SEQMOVE_HIDESHIELD        = (1 << 28),
} eMoveFlag;

 
typedef enum
{
    ENT_VISIBLE = 0,
    ENT_OCCLUDED,
    ENT_NOT_IN_FRONT_OF_CAMERA,
    ENT_INTENTIONALLY_HIDDEN,
} eVisStatus;

enum
{
    SEQFX_CONSTANT            = (1 << 0),
};

enum
{
    SEQ_RETICLE        = 0,
    SEQ_NO_RETICLE    = 1,
};
//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

typedef struct Animation 
{    
    const SeqMove *move;            //curr move in info->moves list
    const SeqMove *move_to_send;    //last non-obvious move (obvious=every client just predicts it) 
    const SeqMove *prev_move;        //prev move in info->moves list (0 if just starting)
    const SeqMove *move_lastframe;    //to see if move has changed (helps performance)
    const TypeGfx *typeGfx;            //animation to use, stored here as expedient rather than call seqGetTypeGfx
    F32            frame;                //curr frame(time) in curr move
    F32            prev_frame;            //prev frame(time) in curr move (or prev move or 0)    
    SkeletonAnimTrack * baseAnimTrack;     //seq->info now has several types, yours is found with seq->type->seqTypeName
    const SeqMove *lastMoveBeforeTriggeredMove; //mild hack for HIT REACTs
    int            triggeredMoveStartTime;
    bool        bRagdoll;
} Animation;

typedef struct FutureMoveTracker
{
    int move_to_play;    //move that is being delayed
    F32 triggertime;     //how long to delay it  (thought add these to ent events?)
    F32 currtime;         //how long it's been delayed so far
    int triggerFxNetId;     //what message will trigger this move
    U8  inuse;             //bookkeeping: does this FutureMoveTracker have anything in it? 
} FutureMoveTracker;

typedef struct HealthFx
{
    char **continuingFx;
    char **oneShotFx;
    char **libraryPiece; //the geometry to be passed into 
    eHealthFxFlags flags;
    Vec2 range;
} HealthFx;

typedef struct SequencerVar
{
    char * variableName;
    char * replacementName;
} SequencerVar;

typedef struct LastBoneTransforms
{
    Vec3 last_xlates[BONEID_COUNT];
    Quat last_rotations[BONEID_COUNT];
} LastBoneTransforms;

typedef struct BoneCapsule
{
    BoneId bone;
    F32 radius;
} BoneCapsule;

typedef struct SeqType
{
    char        name[128];                //File name w/o .txt ("male")

    ///// Sequencer //////////////////////////////////////////
    char        seqname[128];            //Sequencer File from data/sequencers 
    char        seqTypeName[128];        //Type inside the Sequencer to use (default is name of the sequencer file, (prop_basic in this case) )
    char        **constantStateStr;
    U32            constantState[STATE_ARRAY_SIZE];
    SequencerVar **sequencerVars;
    F32            animScale;                // multiplier for animation speed

    /// Base graphics ///////////////////////////////////////
    char        *graphics;                //Name of default graphics to use if costume doesn't provide them (also the door collision)
    F32            loddist[MAX_LODS];        //Range at which to switch to this LOD
    F32            fade[2];                //When to start, and when to stop fading out
    HealthFx    **healthFx;
    char        **fx;                    //Name of fx to be added to this seq on creation.
    Vec3        geomscale;                //Amount to scale gfx
    Vec3        geomscalemax;

    //// Shadow /////////////////////////////////////////////
    eShadowType    shadowType;                //Splat or None(default)
    char        *shadowTexture;            //Splat: --Default "PlayerShadowCircle"
    eShadowQuality shadowQuality;        //Low, Medium, High.  Low quality shadows don't project onto high density meshes, and don't project very far.  Good for Cars and other big shadow things
    Vec3        shadowSize;                //Splat: Height, Depth, Width, Depth --default= 3.6 / depth depends on ENTTYPE /3.6.  (Number is automatically scaled up if you are)
    Vec3        shadowOffset;            //Vector to move the shadow around. Kind of a hack for artist to twiddle shadow position

    //// Selection and Reticle /////////////////////////////////////////////////
    eSelection    selection;
    F32            reticleHeightBias;        //how much to move the reticle up 
    F32            reticleWidthBias;        //how much to move the reticle out
    F32            reticleBaseBias;        //how much to move the reticle base up (or down)
    int            notselectable;

    //// Collision and Physics///////////////////////////////////////////////////
    eCollType    collisionType;
    Vec3        capsuleSize;    
    Vec3        capsuleOffset;
    BoneCapsule    **boneCapsules;
    F32            bounciness;
    int            bigMonster;                //If I am a big monster, I don't collide with non big monsters
    int            pushable;                // Boolean to enable pushing this guy.

    // General //////////////////////////////////////////////////////////////////////
    eSeqTypeFlags flags;                //NoShallowSplash: doesn't make a splash when first touching water. NoDeepSplash: Doesn't make secondary splash when deep in water
    int            lightAsDoorOutside;        //dont use player's overbright on this ent outside
    int            rejectContinuingFX;
    int            randombonescale;        //Should this character randomly pick a bonescale?
    int            fullBodyPortrait;
    int            useStaticLighting;
    ePlacement    placement;
    int            ticksToLingerAfterDeath;   //Server side variable
    int            ticksToFadeAwayAfterDeath; //Client side variable
    F32            minimumambient;            
    F32            vissphereradius;        //Radius of myself for visibility purposes.
    U8            maxAlpha;            
    F32            reverseFadeOutDist;
    F32            spawnOffsetY;        // Distance to move the entity from its spawn position in the direction of its y-vector
    int            noStrafe;            // Makes a player with this sequencer use nostrafe movement mode.
    F32            turnSpeed;            // limits turn speed, invokes TURN* animbits // GGFIXME: move this?
    F32            cameraHeight;
    int            hasRandomName; //Weird to be here, but I was assigned the bug, and this is the only place I know
    char        *randomNameFile; 
    char        *onClickFx;            //Name of fx to be played on click

    ///// Scaling Numbers //////////////////////////////////////////////////////////////////////////
    char        *bonescalefat;        //anm file name for bone scaling system
    char        *bonescaleskinny;    //anm file name for bone scaling system
    Vec3        shoulderScaleSkinny;    //How much to move shoulder positions as you scale bones
    Vec3        shoulderScaleFat;        //How much to move shoulder positions as you scale bones
    Vec3        hipScale;        //How much to move hip positions as you scale bones
    Vec3        neckScale;        //How much to move neck & head positions as you scale bones
    Vec3        legScale;        // used to shorten/lengthen legs
    F32            headScaleRange;
    F32            shoulderScaleRange;
    F32            chestScaleRange;
    F32            waistScaleRange;
    F32            hipScaleRange;
    F32            legScaleMin;        
    F32            legScaleMax;    
    F32            legScaleRatio;        // how much leg scale adjusts the height of player's hips
    Vec3        headScaleMin;
    Vec3        headScaleMax;

    StashTable    sequencerVarsStash;

    ///// Development Mode Info ///////////////////////////////////////////////////////////////////
    char        *filename;                //Full File path from /data (i.e. "ent_types/male.txt") for reloading and click to source
    int            fileAge; //Development mode
    
} SeqType;

typedef struct SeqInst
{
    GfxNode        *gfx_root;                //The part of the gfxtree that is this Entity
    int            gfx_root_unique_id;
    U32            state[STATE_ARRAY_SIZE];            //compared vs seq->info's move requirements
#ifdef CLIENT
    U32            stance_state[STATE_ARRAY_SIZE];
#else
    U32            sticky_state[STATE_ARRAY_SIZE];        //states to be automatically renewed each frame.
#endif
    U32            state_lastframe[STATE_ARRAY_SIZE];    //to see if state has changed (helps performance)
    const SeqType*        type;                    //read in from ent_type explaining how to build this seq

    bool        forceInterrupt;        // Any move represented in the current state interrupts the current move.

    const SeqInfo * info;            //Represents sequencer (.seq) file
    Animation   animation;            //attached to gfx_node, [1] so I don't have to change it everywhere

    int            unique_id;        
    Vec3        currgeomscale;            //size
    Vec3        fxSetGeomScale;
    F32            curranimscale;            //how fast to play animation (not use now)

    int            move_predict_seed;        //seed for certain move predictions
    U8            anim_incrementor;        //info on what move choose 

    Vec3        posLastFrame;

    GroupDef    * worldGroupPtr;

    char calculatedSeqTypeName[MAX_SEQTYPENAME];

    F32            bonescale_ratio;        //-1.0 to +1.0 ( 0.0 == no bonescaleing at all
    F32            shoulderScale;            // adjusts shoulder joint position
    F32            hipScale;                // adjusts hip joint position
    F32            neckScale;                // adjusts neck & head position
    F32            legScale;                // scales legs in (relative) y direction 
    F32            armScale;                // scales arms in (relative) x direction 
    F32            headScales[3];                // overall multipliers for head/face bones

    ModelHeader* bonescale_anm_skinny;    //-1.0 bonescale_ratio == use %100 of these scale values
    ModelHeader* bonescale_anm_fat;        //+1.0 bonescale_ratio == use %100 of these scale values
    
    int            updated_appearance;
    int            handle;        //safe identifier fx system uses.  Could be expanded so more things use it.    
    int            lod_level;                    //Current LOD (which lod_level model should be in use now)
    int            loadingObjects;                //Are any of my bits of geometry still loading.

    // PER BONE INFO
    LastBoneTransforms*    pLastBoneTransforms;

    U32            uiBoneAnimationStored[MAX_BONE_BITFIELD];
    U32            uiBoneAnimationStoredLastFrame[MAX_BONE_BITFIELD];
    U32            uiBoneRotationBlendingBitField[MAX_BONE_BITFIELD];
    U32            uiBoneTranslationBlendingBitField[MAX_BONE_BITFIELD];

    F32            curr_interp_frame;                //how long we've been interpolating this move

    // For registration correction
    F32            curr_fixInterp;                    //how much registration fixing to use (allows smooth interpolation)
    bool        bFixWeaponReg;                    // register to WEAPON/HAND joint instead of the wrist


    bool        bCurrentlyInRagdollMode;
    bool        noCollWithHealthFx; //Set by the healthfx state flag HEALTHFX_NO_COLLISION 

    F32            percentHealthLastCheck;

    F32            moveRateAnimScale;

    //Client Only info
#ifdef CLIENT
    //What the GfxNode needs to know about it's parent sequencer
    SeqGfxData    seqGfxData;

    Vec3        ulFromRoot;
    Vec3        lrFromRoot;
    F32            reticleInterpFrame;
    F32            framesOccluded;
    U8            hasBeenOnCamera;
    eSelection    selectionUsedThisFrame;
    eSelection    selectionUsedLastFrame;
    eVisStatus    visStatus;
    eVisStatus    visStatusLastFrame;
    Mat4        oldSeqRoot;
    U32            lastFrameUpdate;

    Vec3         bonescalesAAA[BONEID_COUNT];   //derived from above three, passed on to the gfx node.
    const char*    newgeometry[BONEID_COUNT];    
    const char*    newgeometryfile[BONEID_COUNT];    
    int            surface;                    // what kind of terrain ent is standing on, for playsound calls
    U8            moved_instantly;


    U32 powerTargetEntId;  //AIMPITCH
    Vec3    powerTargetPos;
    Vec2    currTargetingPitchVec;

    Vec3    headTargetPoint;

    //Custom Geometry
    U8            newcolor1[BONEID_COUNT][4];    //rgba
    U8            newcolor2[BONEID_COUNT][4];
    TextureOverride newtextures[BONEID_COUNT];        //I could bind these up in init nodes...    

    char *        rgbs_array[BONEID_COUNT];  //is this hackery?
    int            static_light_done;

    int            *healthFx; //the const seq fx to be killed when you leave this health range
    int            *healthGeometryFx; //the const seq fx geometry to be killed when you leave this health range

    Vec3        headOffset; //height of the head in the base animation

    Vec3        stuckFeetPyr;
    F32            headYaw;
    F32            chestYaw;
    F32            feetAreStuck;

    eaiHandle seqfx; //the fx spawned on this guy at creation to be killed on death.
    eaiHandle seqcostumefx; //the fx spawned on this guy from costume definitions
    int            const_seqfx[MAX_SEQFX]; //the const seq fx to be killed at the end of the move
    int            volume_fx;  //fx on this guy because of the volume he is in
    int            volume_fx_type;
    int            onClickFx;  //fx on this guy because he was clicked
    int            big_splash;  //have I done a big splash that I do when I'm first submerged?
    int            worldGroupFx; //the fx created to draw the worldGeometry specified in the entType
    //For move prediction
    FutureMoveTracker futuremovetrackers[MAX_FUTUREMOVETRACKERS]; 

    //Various alpha values from which to derive actual alpha value of anim
    int            alphasvr;        //alpha set by the sever from game events
    int            alphacam;        //alpha set by camera
    int            alphadist;        //alpha set by distance
    int            alphafade;        //alpha set by fading in or out
    int            alphafx;        //alpha set by fx attached to this ent


    //Used only for doors, currently
    int            alphasort;        // child models need alpha sort

    //For SimpleShadows
    GfxNode * simpleShadow;
    int simpleShadowUniqueId;
    F32 maxDeathAlpha;

    Vec3        basegeomscale;            //original currgeomscale before custome scale

    //Silliness
    Vec3        posTrayWasLastChecked;  //position the last time I checked to see what tray I was in.  (If it hasn't changed, I'm in the same tray and don't need to do the collision check again)

    //Click to Source fx count
    int            ctsFxCount;

    //Set by the attached fx ( light pulse also set by fx, but that's in seqGfxData, problably shouldn't be )
    U8            noReticle;        //This entity should reject the reticle     
    U8            forceInheritAlpha; //This entity should force all attached effects to match its own alpha
    U8            notPerceptible; // This entity is not visible to this client due to stealth/perception

#else

    Vec3 *pbonescalesAAA;

#endif


} SeqInst;

#define OBJECT_LOADING_COMPLETE 0 
#define OBJECT_LOADING_IN_PROGRESS 1

// when do we need to do scaling, registration, etc.
static INLINEDBG int seqScaledOnServer(const SeqInst *seq)
{
    return seq->type->collisionType == SEQ_ENTCOLL_BONECAPSULES;
}

#ifdef CLIENT
typedef struct CostumePart CostumePart;
typedef struct BodyPart BodyPart;
typedef struct Entity Entity;
void animInitAllNodes(void);
void animRemoveAnimation(Animation *animation);
void animAddAnimation(Animation * animation, ModelHeader * header);
bool seqGetScreenAreaOfBones( SeqInst * seq, Vec2 screenUL, Vec2 screenLR, Vec3 worldMin, Vec3 worldMax );
int seqGetSides( SeqInst * seq, const Mat4 entmat, Vec2 screenUL, Vec2 screenUR, Vec3 worldMin, Vec3 worldMax );
int seqGetSteadyTop( SeqInst * seq, Vec3 steady_top );
void seqSetLod(SeqInst * seq, F32 len);
void seqPlaySound(SeqInst *inst);
void seqManageSeqTriggeredFx( SeqInst * seq, int isNewMove, Entity * e );
void seqManageHitPointTriggeredFx( SeqInst * seq, F32 percentHealth, int randomNumber  );

int seqAddCostumeFx(Entity *e, const CostumePart *costumeFx, const BodyPart *bp, int index, int numColors, int incompatibleFxFlags);
int seqDelAllCostumeFx(SeqInst *seq, int firstIndex);

void seqTriggerOnClickFx( SeqInst * seq, Entity *entParent );
int GetHeadScreenPos( SeqInst * seq, Vec3 HeadScreenPos );

void seqResetWorldFxStaticLighting(SeqInst *seq);
#endif

void seqClearState(U32 state[]);
void seqSetState(U32 *state,int on,int bitnum);
void seqOrState(U32 *state,int on,int bitnum);
bool testSparseBit(const SparseBits* bits, U32 bit);

#ifndef CLIENT
void seqClearToStickyState(SeqInst* seq);
#endif
bool seqGetState(U32 *state, int bitnum);
//int seqGetSeqStateBitNum(const char* statename);
void seqSetStateByName(U32* state, int on, const char* statename);
char* seqGetStateString(U32* state);
void seqUpdateServerRandomNumber(void);
void seqUpdateClientRandomNumber(int new_number);
void seqClearRandomNumberUpdateFlag(void);
void seqSynchCycles(SeqInst * seq, int special_id);
void stateParse(SeqInst * seq, U32 * state);

const SeqMove * seqProcessInst( SeqInst *seq, F32 timestep );
const SeqMove * seqProcessClientInst( SeqInst * seq, F32 timestep, int move_idx, U8 move_updated );
int seqFindAMove(SeqInst * seq, U32 state[], U32 requiresThisBit);
SeqInst * seqLoadInst( const char * type_name, GfxNode * parent, int load_type, int randSeed, Entity *entParent );
SeqInst * seqFreeInst(SeqInst * seq);
void seqClearExtraGraphics( SeqInst * seq, int for_reinit, int hard_kill );
void seqSetMove( SeqInst * seq, const SeqMove *newmove, int is_obvious );

void printSeq(SeqInst * seq);

void seqSynchCycles(SeqInst * seq, int special_id);
int getMoveFlags(SeqInst * seq, eMoveFlag flag);
void seqGetCapsuleSize(SeqInst * seq, Vec3 size, Vec3 offset);
int seqSetStaticLight(SeqInst * seq, DefTracker * lighttracker);
int seqGetCountOfObjsStillLoading(SeqInst * seq);

//different ways to set the state, TO DO standardize names
void play_bits_anim( U32 *state, int     *list );
void play_bits_anim1( U32 *state, int first, ... );
void turn_off_anim(    U32 * state, int first, ... );
void seqSetOutputs(U32 *state, const U32 *sets); 
void seqSetSparseOutputs(U32 *state, const SparseBits *sets); 
int seqSetStateFromString( U32 * state, char * newstate_string );

void seqPreLoadPlayerCharacterAnimations(int load_type);
//void seqPreLoadAllAnimations(int load_type);

void changeBoneScale( SeqInst * seq, F32 newbonescale );
#ifdef CLIENT
typedef struct Appearance Appearance;
void changeBoneScaleEx( SeqInst * seq, const Appearance * appearance );
void seqManageVolumeFx(SeqInst * seq, Mat4 mat, Mat4 prevMat, Vec3 velocity );
#endif

//Debug info
void printScreenSeq(SeqInst * seq);
void printLod(SeqInst * seq);
int printGeometry( GfxNode * node, int seqHandle, int * y );
void printSeqType( SeqInst * seq);
void showAnimationDebugInfo(void);

//Optimizer
void gfxNodeSetFxUseFlag( GfxNode * node );
void gfxNodeClearFxUseFlag( GfxNode * node );

//From seqload, I should move to seqload.h someday
bool seqGetMoveIdxFromName( const char * moveName, const SeqInfo * seqInfo, U16* iResult );
const TypeGfx * seqGetTypeGfx( const SeqInfo * info, const SeqMove * move, const char * typeName );
const TypeP * seqGetTypeP( const SeqInfo * info, const char * typeName );
void seqPreloadSeqInfos(void);
const SeqInfo * seqGetSequencer( const char seqInfoName[], int loadType, int reloadForDev );
int seqAttachAnAnim( TypeGfx * typeGfx, int animLoadType );

void calcSeqTypeName( SeqInst * seq, int alignment, int praetorian, int isPlayer );

#define SEQ_RELOAD_FOR_DEV    1
int seqReinitSeq( SeqInst * seq, int loadType, int randSeed, Entity *entParent );
void seqPrepForReinit(void);
int seqIsMoveSelectable( SeqInst * seq );
int seqChangeDynamicWorldGroup(const char *pchWorldGroup, Entity *entParent);
void seqUpdateWorldGroupPtr(SeqInst * seq, Entity *entParent);
void seqUpdateGroupTrackers(SeqInst *seq, Entity *entParent);

const float* seqBoneScale(const SeqInst* seq, BoneId bone);
void seqClearBoneScales(SeqInst* seq);
void seqSetBoneScale(SeqInst* seq, BoneId bone, Vec3 vToSet);

void allocateLastBoneTransforms(SeqInst* seq);
void freeLastBoneTransforms(SeqInst* seq);

void seqGetLastRotations(SeqInst* seq, BoneId bone, Quat q);
void seqSetLastRotations(SeqInst* seq, BoneId bone, const Quat q);
float* seqLastTranslations(SeqInst* seq, BoneId bone);

void seqSetBoneAnimationStored(SeqInst* seq, BoneId bone);
bool seqIsBoneAnimationStored(SeqInst* seq, BoneId bone);
void seqClearAllBoneAnimationStoredBits(SeqInst* seq);

void seqSetBoneNeedsRotationBlending(SeqInst* seq, BoneId bone, U8 uiSet);
void seqSetBoneNeedsTranslationBlending(SeqInst* seq, BoneId bone, U8 uiSet);
bool seqBoneNeedsRotationBlending(SeqInst* seq, BoneId bone);
bool seqBoneNeedsTranslationBlending(SeqInst* seq, BoneId bone);

void seqClearAllBlendingBits(SeqInst* seq);
void seqAssignBonescaleModels( SeqInst *seq, int *randSeed );

char *  getRootHealthFxLibraryPiece( SeqInst * seq, int randomNumber );
void manageEntityHitPointTriggeredfx( Entity * e );

const SeqMove *getFirstSeqMoveByName(const SeqInst* seq, char * name);

GfxNode* seqFindGfxNodeGivenBoneNum(const SeqInst *seq, BoneId bonenum);
GfxNode* seqFindGfxNodeGivenBoneNumOfChild(const SeqInst *seq, BoneId bonenum, bool requireChild);
void seqGetBoneLine(Vec3 pos, Vec3 dir, F32 *len, const SeqInst *seq, BoneId bone);

// returns surface-to-surface distance
// returns <0 for no range found (e.g. no bone capsules)
// center, surface, and radius are output for the range found
F32 seqCapToBoneCap(Vec3 src_pos, Vec3 src_dir, F32 src_len, F32 src_rad, SeqInst *target_seq,
                    Vec3 center, Vec3 surface, F32 *radius);

#endif
