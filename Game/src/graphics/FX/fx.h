#ifndef _FX_H
#define _FX_H

#include <utilitieslib/stdtypes.h>
#include "fxutil.h"
#include <utilitieslib/components/MemoryPool.h>

typedef struct FxCape FxCape;
typedef struct FxGeo FxGeo;
typedef struct FxInfo FxInfo;
typedef struct FxBhvr FxBhvr;
typedef struct Mailbox Mailbox;
typedef struct FxFluid FxFluid;

#define SOFT_KILL            0
#define HARD_KILL            1
#define MAX_DUMMYEVENTS        100        //An effect can have x total events
#define MAX_DUMMYCONDITIONS 100        //Spread amoung y conditions.  
#define    MAX_DUMMYINPUTS        30        //With z inputs
#define FXGEO_IS_NOT_KEY    0
#define CATCH_COUNT            10
#define FXGEO_IS_KEY        1
#define FXGEO_NOCUSTOMSCALE 0
#define FXGEO_NOLIGHTTRACKER 0

#define FX_POWERFX    0
#define FX_WORLDFX    1
#define FX_ENTITYFX    2
#define FX_VOLUMEFX    2

#define FXDEBUG_TEMP 0

#define MAXCONDITIONS 96 //TO DO dynamically allocate, this is clumsily implemented, careful

//Places to put KEY fxgeos for the fx to use later
//Use (seq and bone OR gfxnode OR neither) + offset (offset often = unitmat) 
typedef struct FxKey
{
    GfxNode        * gfxnode;  //Attach this KEY fxgeo to this gfxnode -or else-
    SeqInst        * seq;        //Find the gfxnode of this bone on this seq and use it
    BoneId        bone;
    Mat4        offset;        //xform from parent gfxnode or world position. used right now by world fx
    U32            childBone:1;    // Whether or not this input wants the bone of a child FX hanging off the GfxNode
} FxKey;

//FxParams is passed into fxcreate. It's a form filled out by anyone wanting a new fx.
typedef struct FxParams
{
    FxKey        keys[MAX_DUMMYINPUTS];    //KEY fxgeos are created with the fx, and are the only fxgeos 
    int            keycount;                //    that can be connected to gfxnodes outside the fx
    U32            power;                //unused now
    F32            fadedist;                //for world fx only right noe, when to fade this fx out
    int            kickstart;                //for world fx only, tells system to leap into existence (only particles support this right now...)
    int            fxtype;                    //FX_POWERFX etc
    F32            duration;
    F32            radius;
    GroupDef    *externalWorldGroupPtr;
    int            net_id;                    // currently unused
    int            targetSeqHandle;        // Stored separately, since it might be needed by a child effect.

    // Override parameters for tintable FX and capes
    U8            colors[4][4];
    int            numColors; // 0-4
    const U8*    pGroupTint;
    const char*        textures[4];
    const char*        geometry;

    U32            dontRewriteKeys        :1;
    U32            isPersonal            :1;
    U32            isPlayer            :1;
    U32            isUserColored        :1;
    U32            vanishIfNoBone        :1;
} FxParams;

//Fx Object is the base controller for an fx. It holds the list of fxgeos created,  
//and the fxinfo that lays out it's timeline.
typedef struct FxObject
{
    //////////////List Management
    struct        FxObject * next;
    struct        FxObject * prev;
    int            handle;
    U32            net_id;    //Secret ID of the effect for the server to refer to it later
                        //same ID across server, so events can be lined up on server...
    int            currupdatestate; //FX_UPDATE_AND_DRAW_ME etc
    eFxVisibility visibility; //used by world fx now, 

    //////////////Components
    FxGeo        * fxgeos;                //Interesting points with geometry
    int            fxgeo_count;
    FxInfo        * fxinfo;                //fxinfo + fxparams = all you need to know
    FxParams    fxp;                    //copy of original params for future reference (mostly debug now)
    
    DefTracker  * light;                    //light info used to statically light this fx's world fx
    int            useStaticLighting;            // used for seqs with the useStaticLighting flag set
    F32            light_age;                //frame when was this light gotten (can't use it past frame it was gotten)
    SeqGfxData  * seqGfxData;            //parent seq, for dynamic lighting an fx

    GroupDef    * externalWorldGroupPtr;    //groupDef passed in from outside to make entType fx able to use them more easily 

    //////////////State 
    F32            duration;                //hard cut off either from fxinfo or from lennard's code 
    F32            age;                    //current age in frames
    F32            last_age;                //age on last frame
    F32            day_time;                //current time of day
    F32            last_day_time;            //previous time of day
    F32*        frameLastCondition;        //age last successful condition, for each condition
    F32            lastForcePower;
    int            fxobject_flags;            //set in varios spots, triggers conditions w/ events
    int            conditionsMet[MAXCONDITIONS/32];            //conditions done already, to catch do once only conditions
    U32            state[STATE_ARRAY_SIZE];//parent fx or seq can set...(+sticky state, other into a struct?)
    int            lifeStage;                //slow dying so geometry fade out can work
    U8            primeHasHit; //posible a hack, so fx knows not to keep putting it in the mailbox, and when it dies, it knows to discharge any remainders

    int            handle_of_parent_seq;    //new crazy concept, knowing about the seq you are attached to...
    int            handle_of_target_seq;    // if a target was passed in as a key, this is it's seq (for ChildFx)
    int            last_move_of_parent;
    U32            power;                    //#between 1 (lowest power), and 10 (highest power)
    F32            radius;

    Vec3        parentLinVel;
    Vec3        parentAngVel;

    //////////////Temp Debug
    int            infoage;
    int            pmax_debug;
    int            curr_detail_fx;


} FxObject;

typedef struct FxEngine
{
    int            fx_on;                        //whether to show FX
    
    int            fx_auto_timers;                // Whether to use individual fx auto timers.    

    /////////FxObjects
    FxObject    * fx;
    int            fxcount;

    MemoryPool  fxgeo_mp;
    MemoryPool  fx_mp;
        
    /////////FxInfos and FxBhvrs////////////////////////////
    FxInfo        * fxinfos;        //Only fxinfos loaded outside the binary.
    int            fxinfocount;    //count of fxinfos loaded outside the binary (not very important)

    FxBhvr        * fxbhvrs;        //Only fxbhvrs loaded outside the binary
    int            fxbhvrcount;    //count of fxbhvrs loaded outside the binary (not very important)

//#if NOVODEX
    FxFluid        * fxfluids;        //Only FxFluidDescs loaded outside the binary
    int            fxfluidcount;    //count of FxFluidDescs loaded outside the binary (not very important)
//#endif

    FxCape        * fxCapes;        //Only fxCapes loaded outside the binary
    int            fxCapecount;    //count of fxCapes loaded outside the binary (not very important)

    int            no_expire_worldfx;

} FxEngine;

enum
{
    FX_FULLY_ALIVE                        = 0,
    FX_GENTLY_KILLED_PRE_LAST_UPDATE    = 1,
    FX_GENTLY_KILLED_POST_LAST_UPDATE    = 2,
    FX_FADINGOUT                        = 3,
};

//Debug Stuff
typedef enum FxDebugType
{
    FXTYPE_TRAVELING_PROJECTILE            = (1 << 0),
    FXTYPE_NEVER_DIES                    = (1 << 1),
    FXTYPE_PECULIAR_DEATH_CONDITIONS    = (1 << 2),
    FXTYPE_LIFESPAN_IN_CONDITIONS        = (1 << 3),
} FxDebugType;

typedef struct FxDebugAttributes
{
    int lifeSpan;
    int type;

} FxDebugAttributes;
//End Debug Stuff

///////////////////////////////////////////////////
//Tools for sending in Fx /////////////////////////
//Little trick for the World Fx stuff to easily store it's fxobjects

// Generated by mkproto
void fxInitFxParams( FxParams * fxparams );
int fxCreate( const char * fxname, FxParams * fxp );
void fxChangeParams( int fxid, FxParams * fxp );
int fxManageWorldFx( int fxid, char * fxname, Mat4 mat, F32 fadestart, F32 fadedist, DefTracker * light, U8* pTintRGB);
void fxSetUseStaticLighting( int fxid, int useStaticLighting );
void fxResetStaticLighting( int fxid ); //clears the lighting on all fxgeos so that it will be recalculated next frame
int  fxDelete( int fxid, int killtype );
void fxEngineInitialize(void);
void fxRunEngine(void);
void fxDoForEachFx(SeqInst *seq, void(*applyFunction)(FxObject*, void*), void *data);
void fxRunParticleSystems(FxObject* fx, void* notUsed);
void fxAdvanceEffect(FxObject* fx, void* notUsed);
void fxTestFxInTfxTxt(void);
#ifdef NOVODEX_FLUIDS
void fxDrawFluids(void);
#endif
void fxReInit(void);
void fxNotifyMailboxOnEvent(int fxid, Mailbox * mailbox, int flag); //TO DO savage hack
int fxIsFxAlive(int fxid);
void fxExtendDuration(int fxid, F32 added_duration);
FxGeo * getOldestFxGeo( FxObject * fx );
void fxUpdateFxStateFromParentSeqs();
void fxDestroyForReal( int fxid, int part_kill_type );
void fxChangeWorldPosition( int fx_id, Mat4 mat);
void fxClearPartHigh(void);
void fxSelectNextFxForDetailedInfo();
void fxFetchSides( int fxid, Vec3 ul, Vec3 lr );
void fxCleanUp(void);
FxDebugAttributes * FxDebugGetAttributes( int fxid, const char * name, FxDebugAttributes * attribs );
char * fxPrintFxDebug();
void fxSetIndividualAutoTimersOn(int on);
void fxChangeSeqHandles(int oldSeqHandle, int newSeqHandle);
void fxReloadSequencers(void);
void fxActivateForceFxNodes(Vec3 vEpicenter, F32 fRadius, F32 fPower);
int fxIsCameraSpace(const char *fxname);
// End mkproto

#endif
