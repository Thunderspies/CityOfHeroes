// -------------------------------------------------------------------------------------------------------------------
// NOVODEX SDK C WRAPPER
// -------------------------------------------------------------------------------------------------------------------
#include "NovodeX/NwWrapper.h"
#include <utilitieslib/utils/SuperAssert.h>
#if NOVODEX

#define NX_PERF_TIMING 1

//#define PARANOID_CHECK_MESH_DATA 1

#if !defined NDEBUG
    // Use the DEBUG libraries.
#if defined _M_IX86
    #pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win32/PhysXLoaderCHECKED.lib")
    #pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win32/PhysXCookingCHECKED.lib")
#elif defined _M_X64
#pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win64/PhysXLoader64CHECKED.lib")
#pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win64/PhysXCooking64CHECKED.lib")
#else
#error Unsupported arcitecture
#endif // Architecture
#else  // NDEBUG
    // Use the RELEASE libraries.
#if defined _M_IX86
    #pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win32/PhysXCooking.lib")
    #pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win32/PhysXLoader.lib")
#elif defined _M_X64
#pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win64/PhysXLoader64.lib")
#pragma comment(lib, "../../../3rdparty/PhysX/SDKs/lib/win64/PhysXCooking64.lib")
#else
#error Unsupported arcitecture
#endif // Architecture
#endif // NDEBUG

#include "NwSharedStream.h"

// Basic SDK includes.
#include <PhysXLoader.h>
#include <NxPhysics.h>
#include <NxPhysicsSDK.h>
#include <NxMaterial.h>
#include <NxScene.h>

// Shape and actor includes.
#include <NxActor.h>
#include <NxTriangleMeshDesc.h>
#include <NxTriangleMeshShapeDesc.h>

#include <NxCooking.h>
#include "NovodeX/NwRagdoll.h"
#include "NwCoreDump.h"

#include "NxUserOutputStream.h"

//#define NX_PERF_TIMING
extern "C"{

#ifdef CLIENT
    #include "graphics/camera.h"
    #include "graphics/FX/fxgeo.h"
    #include "UI/uiConsole.h"
    #include "player/player.h"
    #include <utilitieslib/utils/osdependent.h>
    #include "graphics/FX/fxdebris.h"
#endif

#ifdef SERVER
    #include "ai/entai.h"
    #include "entity/motion.h"
#endif

    #include <utilitieslib/stdtypes.h>
#ifdef NX_PERF_TIMING
    #include <utilitieslib/utils/timing.h>
#endif
    #include <utilitieslib/utils/mathutil.h>
    #include "group/groupfileload.h"
    #include "group/groupnovodex.h"
    #include "group/group.h"
    #include <utilitieslib/components/queue.h>
    #include "entity/entity.h"
    #include <utilitieslib/utils/strings_opt.h>
    #include <utilitieslib/components/StashTable.h>
    #include "cmdparse/cmdcommon.h"
    #include <utilitieslib/utils/error.h>
    #undef fopen    // file.h does not work, so get rid of this...

    void drawLine3D(Vec3 p1, Vec3 p2, int argb);
    void drawTriangle3D(Vec3 p1, Vec3 p2, Vec3 p3, int argb);

}

#include <utilitieslib/components/earray.h>

// -------------------------------------------------------------------------------------------------------------------

void nwDeleteAllScenes( );
static NxActor* getActorFromGuid(NxActorGuid uiActorGuid);
static void nwEndAllThreads();


// Only need queues for client
#ifdef CLIENT
void processMaterialDescQueue(int iSceneNum, bool bCreateMaterial );
void processActorCreationQueue(int iSceneNum, bool bCreateActor);
void processActorDeletionQueue(int iSceneNum);
void processForceQueue(int iSceneNum);
void removeActor( NxActor* pActor, int iSceneNum );
static void  nwDebugRender(const NxDebugRenderable* pDebugData);
static void processActorFunctionOneParamQueue(bool bCallFunction);
static void nwDestroyActorFunctionQueues( );
#endif

#define NXCLAMP(var, min, max) \
    ((var) < (min)? (min) : ((var) > (max)? (max) : (var)))


#ifndef NX_PERF_TIMING
#define PERFINFO_AUTO_START_STATIC(a,b,c)
#define PERFINFO_AUTO_STOP()
#define PERFINFO_AUTO_STOP_START(a,b)
#define PERFINFO_AUTO_START(a,b)
#endif

#define NX_SHAPE_STORED 1

#define DEFAULT_MAX_DYNAMIC_ACTOR_COUNT 2000
#define DEFAULT_MAX_NEW_DYNAMIC_ACTORS 500

static int iLastSceneSimulated; // for debugging

static struct
{
    float timeStepSize;
    int   timeStepMax;
    float gravity;
    float restitution;
    float staticFriction;
    float dynamicFriction;
    bool  bInited;
    float shapeSkinWidth;

} nxSceneConstants;

typedef struct Force
{
    Vec3 vEpicenter;
    F32 fRadius;
    F32 fPower;
    void* pExlcudedEmissaryData;
    int iSceneNum;
    bool bReadyToGo;
} Force;

// -------------------------------------------------------------------------------------------------------------------
// GLOBAL STATICS
// -------------------------------------------------------------------------------------------------------------------
NxState nx_state;

// NovodeX-specific parameters.
NxPhysicsSDK*            nxSDK; 
NxUtilLib*                nxUtilLib;
NxScene*                nxScene[MAX_NX_SCENES];                // max scenes for now
int                        nxMaxScenes = MAX(MAX_NX_SCENES / 2, 1);    // currently allowed max scenes

static NxMaterial*        nxDefaultMaterial[MAX_NX_SCENES];
static int                nxThreadActive[MAX_NX_SCENES];

static int**            nxTestBoxActors;
static int*                nxTestBoxActorColors;

static Queue actorDescQueue; // queue of actordescs's to be processed and then deleted
static Queue actorDeletionQueue; // queue of actor pointers to be removed from scene and have their userdata deleted
static Queue materialDescQueue;

static U32 uiCurrentNumMaterials = 1;

static StashTable convexShapeTable; // a stash table of convex shape desc's... to cache convex shape generations.
//static StashTable meshShapeTable; // a stash table of mesh shape desc's... to cache mesh shape generations.

static StashTable actorGuidTable;
static StashTable nwEmissaryDataGuidTable;

static Force forceToApply;
// -------------------------------------------------------------------------------------------------------------------

















// -------------------------------------------------------------------------------------------------------------------
// MEMORY ALLOCATION
// -------------------------------------------------------------------------------------------------------------------
struct MyAllocator : public NxUserAllocator
{

#ifdef NX_PERF_TIMING
    #if USE_NEW_TIMING_CODE
        PerfInfoStaticData* perfInfo;
    #else
        PerformanceInfo* perfInfo;
    #endif
    const char* heapPerfInfoName;
#endif
    MyAllocator()
#ifdef NX_PERF_TIMING
        : heapPerfInfoName("NxHeap"), perfInfo(NULL)
#endif
    {
    }

    virtual void* mallocDEBUG(size_t size, const char* fileName, int line)
    {
        PERFINFO_AUTO_START_STATIC(heapPerfInfoName, &perfInfo, 1);
        void* result = ::_calloc_dbg(size, 1, 1, fileName, line);
        PERFINFO_AUTO_STOP();
        return result;
    }

#undef malloc
    virtual void* malloc(size_t size)
    {
        PERFINFO_AUTO_START_STATIC(heapPerfInfoName, &perfInfo, 1);
        void* result = ::_calloc_dbg(size, 1, 1, __FILE__, __LINE__);
        PERFINFO_AUTO_STOP();
        return result;
    }

#undef realloc
    virtual void* realloc(void* memory, size_t size)
    {
        PERFINFO_AUTO_START_STATIC(heapPerfInfoName, &perfInfo, 1);
        void* result = ::_realloc_dbg(memory, size, 1, __FILE__, __LINE__);
        PERFINFO_AUTO_STOP();
        return result;
    }

#undef free
    virtual void free(void* memory)
    {

        PERFINFO_AUTO_START_STATIC(heapPerfInfoName, &perfInfo, 1);
        ::_free_dbg(memory, 1);
        PERFINFO_AUTO_STOP();
    }
} myAllocator;

static MyAllocator* debugAllocator;

struct NwOutputStream : public NxUserOutputStream
{
public:

    /**
    Reports an error code.

    \param code Error code, see #NxErrorCode
    \param message Message to display.
    \param file File error occured in.
    \param line Line number error occured on.
    */
    virtual void reportError(NxErrorCode code, const char * message, const char *file, int line)
    {
#if 0
        if ( code < NXE_DB_INFO )
        {
            FatalErrorf("Error %d: %s... %s line %d\n", code, message, file, line);
        }
        else // non-fatal error
#endif
        {
            Errorf("Error %d: %s... %s line %d\n", code, message, file, line);
        }
    }
    /**
    Reports an assertion violation.  The user should return 

    \param message Message to display.
    \param file File error occured in.
    \param line Line number error occured on.
    */
    virtual NxAssertResponse reportAssertViolation(const char * message, const char *file, int line) 
    {
        //printf("%s... %s line %d\n", message, file, line);
        //assert(0);
        FatalErrorf("%s... %s line %d\n", message, file, line);
        return NX_AR_BREAKPOINT;
    }
    /**
    Simply prints some debug text

    \param message Message to display.
    */
    virtual void print(const char * message)
    {
        printf("%s", message);
    }
} nwOutputStream;

// -------------------------------------------------------------------------------------------------------------------



















// -------------------------------------------------------------------------------------------------------------------
// LOCAL UTILITY FUNCTIONS
// -------------------------------------------------------------------------------------------------------------------
void nwSetNxMat34FromMat4( NxMat34& nxmat, const Mat4 mat)
{
    nxmat.t = (float*)mat[3];
    nxmat.M.setColumnMajor( (float*)mat );
}


static void nwDebugDumpVerts( const char* name, const float* verts, U32 uiVertCount, U8 uiStride)
{
    U32 uiIndex;
    char cFileName[512];
    const float* pfCurrentVertPointer = verts;
    STR_COMBINE_BEGIN(cFileName);
    STR_COMBINE_CAT("C:\\ageiadumps\\");
    STR_COMBINE_CAT("dump");
    STR_COMBINE_CAT(name);
    STR_COMBINE_CAT(".verts");
    STR_COMBINE_END();

    FILE* file = fopen(cFileName, "wb");
    for (uiIndex = 0; uiIndex < uiVertCount; ++uiIndex)
    {
        fwrite(pfCurrentVertPointer, sizeof(float), 3, file );
        pfCurrentVertPointer = (float*)((char*)pfCurrentVertPointer + uiStride);
    }
    fclose(file);
}

static void nwDebugDumpTris( const char* name, const U32* puiTris, U32 uiTriCount, U8 uiStride)
{
    U32 uiIndex;
    char cFileName[512];
    const U32* pfCurrentTriPointer = puiTris;
    STR_COMBINE_BEGIN(cFileName);
    STR_COMBINE_CAT("C:\\ageiadumps\\");
    STR_COMBINE_CAT(name);
    STR_COMBINE_CAT(".tris");
    STR_COMBINE_END();
    FILE* file = fopen(cFileName, "wb");
    for (uiIndex = 0; uiIndex < uiTriCount; ++uiIndex)
    {
        fwrite(pfCurrentTriPointer, sizeof(U32), 3, file );
        pfCurrentTriPointer = (U32*)((char*)pfCurrentTriPointer + uiStride);
    }
    fclose(file);
}

static void nwDebugDumpTriMesh(const char* name, const NxTriangleMeshDesc& desc)
{
    nwDebugDumpVerts(name, (const float*)desc.points, desc.numVertices, desc.pointStrideBytes);
    nwDebugDumpTris(name, (const U32*)desc.triangles, desc.numTriangles, desc.triangleStrideBytes);
}

#ifdef SERVER
extern "C" char world_name[MAX_PATH];
static void nwPullPlugOnRagdoll(int id)
{
    int i;
    Entity* pRagdollOwner = NULL;
    char cLoggingBuffer[1024];

    if ( id == -1 )
    {
#ifdef SERVER
        nx_state.notAllowed = 1;
#else
        nx_state.allowed = 0;
#endif
        // just pretend like novodex is deinited, but don't do it since it's now tainted
        nx_state.tainted = 1;
        nx_state.initialized = 0;
        nxSDK = NULL;
        for (int i=0; i<MAX_NX_SCENES; i++)
        {
            nxScene[i] = NULL;
            nxThreadActive[i] = false;
        }
        //nwDeinitializeNovodex();
    }

    // Find ragdoll
    for(i = 0; i < entities_max; i++)
    {
        if ( i >= 1 && i < entities_max && entity_state[i] & ENTITY_IN_USE )
        {
            Entity* e = entities[i];
            if ( e->ragdoll && e->ragdoll->iSceneNum == iLastSceneSimulated )
            {
                pRagdollOwner = e;
                break;
            }
        }
    }

    if ( pRagdollOwner )
    {
        char cVelocity[128];
        char cLocation[128];
        sprintf(cLocation, "%.2f, %.2f, %.2f", ENTPOS(pRagdollOwner)[0], ENTPOS(pRagdollOwner)[1], ENTPOS(pRagdollOwner)[2]);
        sprintf(cVelocity, "%.2f, %.2f, %.2f", pRagdollOwner->motion->vel[0], pRagdollOwner->motion->vel[1], pRagdollOwner->motion->vel[0]);


        STR_COMBINE_BEGIN(cLoggingBuffer);
        STR_COMBINE_CAT("---------------------\n");
        STR_COMBINE_CAT("NovodeX threw exception, ignoring and turning physics off\n");
        STR_COMBINE_CAT("Entity name = ");
        STR_COMBINE_CAT(pRagdollOwner->name);
        STR_COMBINE_CAT("\nEntity Location = ");
        STR_COMBINE_CAT(cLocation);
        STR_COMBINE_CAT("\nEntity Velocity = ");
        STR_COMBINE_CAT(cVelocity);
        STR_COMBINE_CAT("\nMap = ");
        STR_COMBINE_CAT(world_name);
        STR_COMBINE_CAT("\n---------------------\n");
        STR_COMBINE_END();
    }
    else
    {
        strcpy(cLoggingBuffer, "NovodeX threw exception, ignoring and turning physics off\n");
    }

    Errorf(cLoggingBuffer);

}
#endif

static void nwResetClientSidePhysics()
{
    nwDeinitializeNovodex();
    nwInitializeNovodex();
}

// our "give up" exception handler
static int nwGiveUpExceptionHandler(unsigned int code, PEXCEPTION_POINTERS info)
{
    /*
    DWORD curThreadID = GetCurrentThreadId();
    int iBadNovodexThreadIndex = nwGetThreadIndexFromID(curThreadID);
    if ( iBadNovodexThreadIndex >= 0)
    nwPullPlugOnRagdoll(iBadNovodexThreadIndex);
    */
#ifdef SERVER
    nwPullPlugOnRagdoll(-1);
#else
    nx_state.needToReset = 1;
//    nwResetClientSidePhysics();
#endif

    return EXCEPTION_EXECUTE_HANDLER;//nwPullPlugOnRagdoll(iBadNovodexThreadIndex);
}

static void destroyStoredShapeDesc(void* pShapeDesc)
{
    delete (NxShapeDesc*)pShapeDesc;
}

void destroyConvexShapeCache()
{
    if ( convexShapeTable )
    {
        stashTableDestroyEx(convexShapeTable, NULL, destroyStoredShapeDesc);
        convexShapeTable = NULL;
    }
}

// -------------------------------------------------------------------------------------------------------------------
NxMaterialIndex nwGetIndexFromMaterialDesc(const NxMaterialDesc& materialDesc, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );
    // Ok, we've got a material... let's check to see if it's already in our scene
    // This should be made into a faster lookup (hash table?), but for now a linear search will do
    // since the number of unique materials ought to be very low (less than 10)
    for (NxU32 uiMaterial=0; uiMaterial<pScene->getNbMaterials(); ++uiMaterial)
    {
        NxMaterial* testMaterial = pScene->getMaterialFromIndex(uiMaterial);
        if ( 
            fabsf(testMaterial->getRestitution() - materialDesc.restitution) < 0.1f
            && fabsf(testMaterial->getStaticFriction() - materialDesc.staticFriction) < 0.1f
            && fabsf(testMaterial->getDynamicFriction() - materialDesc.dynamicFriction) < 0.1f
            )
        {
            // we have a match!
            return uiMaterial;
        }
    }

    // If we got this far, the material is a new, unique material
    // and must be added to the material array
    //
    // This must be queued up.


    if ( materialDescQueue && uiCurrentNumMaterials < 255)
    {
        NxMaterialDesc* pNewMaterialDesc = new NxMaterialDesc();
        *pNewMaterialDesc = materialDesc;
        qEnqueue(materialDescQueue, pNewMaterialDesc);
        ++uiCurrentNumMaterials;
        return uiCurrentNumMaterials-1; // the index we expect
    }
    else
    {
        assert(nxDefaultMaterial[iSceneNum]);
        return nxDefaultMaterial[iSceneNum]->getMaterialIndex(); // failed to make material?
    }
}
// -------------------------------------------------------------------------------------------------------------------

static bool nwActorWantsToSleep(NxActor* pActor)
{
    F32 fSleepLinVel = pActor->getSleepLinearVelocity();
    F32 fSleepAngVel = pActor->getSleepAngularVelocity();

    return (
        (pActor->getLinearVelocity().magnitudeSquared() < fSleepLinVel*fSleepLinVel)
        && (pActor->getAngularVelocity().magnitudeSquared() < fSleepAngVel*fSleepAngVel)
        );
}

// -------------------------------------------------------------------------------------------------------------------
// This isn't the real surface area,
// it's 1/2 the surface area of the AABB
F32 nwCalcActorSurfaceArea(NxActor* pActor)
{
    NxVec3 vDiag;
    NxBounds3 bounds;
    F32 fSurface = 0;
    assert(pActor->getNbShapes() == 1);
    pActor->getShapes()[0]->getWorldBounds(bounds);
    vDiag = bounds.max - bounds.min;
    vDiag.x = fabsf(vDiag.x);
    vDiag.y = fabsf(vDiag.y);
    vDiag.z = fabsf(vDiag.z);
    return ( vDiag.x * vDiag.y + vDiag.x * vDiag.z + vDiag.y * vDiag.z );
}

// -------------------------------------------------------------------------------------------------------------------

void nwSetPhysicsQuality( ePhysicsQuality pq)
{
    nx_state.physicsQuality = pq;
#ifdef CLIENT
    fxSetMaxDebrisCount();
#endif
}

void nwSetRagdollCount( int count )
{
#ifdef SERVER
    if (count > 0 && count <= MAX_NX_SCENES)
        nxMaxScenes = count;
#endif
}

// -------------------------------------------------------------------------------------------------------------------
// USER DATA
// -------------------------------------------------------------------------------------------------------------------
/*
 *    This is where you put anything that will need to access from just an actor pointer.
    Never set the userdata pointer under actor to something that is not this class, as
    we delete this when the actor is deleted.
 */
// -------------------------------------------------------------------------------------------------------------------

#ifdef CLIENT
class NwEmissary
{
public:
    NwEmissary()
    {
        ZeroStruct(&_Data);
        _Data.pEmissary = this;
        _Data.uiCreateTick = global_state.global_frame_count;
        pJoint = NULL;
        jointDesc = NULL;
        uiActorGuidWeFilteredOut = 0;
    }

    // Mutators
    NxActor* GetActor( ) const
    {
        return (NxActor*)_Data.pActor;
    }

    //Accessors

    void SetActor( NxActor* pActor )
    {
        _Data.pActor = pActor;
        if ( pActor )
        {
            _Data.fSurfaceAreaOverMass = nwCalcActorSurfaceArea(pActor) * 0.2f / pActor->getMass();
        }
        else
            _Data.fSurfaceAreaOverMass = 0.0f;
    }

    void addAccelOnce( NxVec3 vAccel )
    {
        addVec3( _Data.vAccelToAddOnce, vAccel, _Data.vAccelToAddOnce );
        _Data.bToAddForce = true;
    }

    void addRecuringAccel( NxVec3 vAccel )
    {
        addVec3( _Data.vAccelToAdd, vAccel, _Data.vAccelToAdd );
    }

    void addAngularAccelOnce( NxVec3 vAccel )
    {
        addVec3( _Data.vAngAccelToAddOnce, vAccel, _Data.vAngAccelToAddOnce );
        _Data.bToAddForce = true;
    }

    void setActorGuid(NxActorGuid uiActorGuid)
    {
        _Data.uiActorGuid = uiActorGuid;
    }

    void setIsJointed(bool bToSet)
    {
        _Data.bCurrentlyJointed = bToSet;
    }

    void setJointCollidesWorld(bool bToSet )
    {
        _Data.bJointCollidesWorld = bToSet;
    }

    bool getJointCollidesWorld( )
    {
        return _Data.bJointCollidesWorld;
    }

    NwEmissaryDataGuid getGuid()
    {
        return _Data.uiOurGuid;
    }

    void setGuid(NwEmissaryDataGuid uiOurGuid)
    {
        _Data.uiOurGuid = uiOurGuid;
    }

    NxActorGuid getActorGuid()
    {
        return _Data.uiActorGuid;
    }

    void notifyJointBroke(F32 fBreakingForce, NxVec3 const& vJointAnchor)
    {
        copyVec3(vJointAnchor, _Data.vJointAnchor);
        _Data.bJointJustBroke = true;
        _Data.bCurrentlyJointed = false;
        pJoint = NULL;
    }

    bool wantsEndTouchNotification( )
    {
        return !!_Data.bNotifyOnEndTouch;
    }

    bool NotifyFxGeoEndTouch( NxActor* pActor )
    {
        if ( !pActor || !_Data.pActor)
            return false;

        NxActor* pOurActor = (NxActor*)_Data.pActor;

        NxVec3 vTowardUs = pOurActor->getGlobalPosition() - pActor->getGlobalPosition();
//        vTowardUs.normalize();

        NxVec3 vRelativeVelocity = pActor->getLinearVelocity() - pOurActor->getLinearVelocity();

        // Check if it's coming toward us
        if ( vRelativeVelocity.dot(vTowardUs) > 0.0f )
        {
            // Large drag factor
            pActor->setLinearVelocity(pActor->getLinearVelocity() * 0.9f);
            pActor->setAngularVelocity(pActor->getAngularVelocity() * 0.9f);
            return true;
        }

        return false;
    }

    // Novodex callbacks
    U8 NotifyFxGeoStartTouch( bool bContactExists, const NxVec3* vContactPos, const NxVec3* vContactNorm, float fContactForceTotal )
    {
        if ( _Data.fPhysicsHitForce < fContactForceTotal )
            _Data.fPhysicsHitForce = fContactForceTotal;
        if ( bContactExists )
        {
            _Data.uPhysicsHitObject = 2;
            copyVec3( *vContactPos, _Data.vPhysicsHitPos );
            copyVec3( *vContactNorm, _Data.vPhysicsHitNorm );
        }
        else
        {
            _Data.uPhysicsHitObject = 1;
        }

        return true;
    }

    U32 getAge()
    {
        return global_state.global_frame_count - _Data.uiCreateTick;
    }

    void everyFrame( )
    {
        const U32 cuiMaxFramesForFiltering = 30;
        NxActor* pActor = (NxActor*)_Data.pActor;

        if ( uiActorGuidWeFilteredOut && global_state.global_frame_count - _Data.uiCreateTick > cuiMaxFramesForFiltering )
        {
            assert(nxScene[NX_CLIENT_SCENE]);
            // clear the filtering
            NxActor* pOtherActor = getActorFromGuid(uiActorGuidWeFilteredOut);
            if ( pOtherActor )
                nxScene[NX_CLIENT_SCENE]->setActorPairFlags(*pActor, *pOtherActor, 0);
            uiActorGuidWeFilteredOut = 0;
        }

        if ( pActor->isSleeping() && !_Data.bToAddForce )
            return;

        NxVec3 vTotalAccelToAdd;
        NxVec3 vTotalAccelToAddOnce;
        NxVec3 vTotalAngAccelToAdd;
        copyVec3(_Data.vAccelToAdd, vTotalAccelToAdd );
        copyVec3(_Data.vAccelToAddOnce, vTotalAccelToAddOnce );
        if ( _Data.bToAddForce && _Data.fSurfaceAreaOverMass )
            vTotalAccelToAddOnce *= _Data.fSurfaceAreaOverMass;
        copyVec3(_Data.vAngAccelToAddOnce, vTotalAngAccelToAdd );
        if ( _Data.bToAddForce && _Data.fSurfaceAreaOverMass )
            vTotalAngAccelToAdd *= _Data.fSurfaceAreaOverMass;

        vTotalAccelToAdd += vTotalAccelToAddOnce;

        zeroVec3(_Data.vAccelToAddOnce );
        zeroVec3(_Data.vAngAccelToAddOnce );

        if ( pActor )
        {
            if ( !vTotalAccelToAdd.isZero() )
            {
                if ( !vTotalAccelToAddOnce.isZero() || !nwActorWantsToSleep(pActor) )
                {
                    pActor->addForce(vTotalAccelToAdd, NX_ACCELERATION);
                }
            }

            if ( !vTotalAngAccelToAdd.isZero() )
            {
                if ( _Data.fSurfaceAreaOverMass )
                    vTotalAngAccelToAdd *= _Data.fSurfaceAreaOverMass;
                pActor->addTorque(vTotalAngAccelToAdd, NX_ACCELERATION);
            }

            if ( _Data.bCurrentlyJointed && _Data.fJointDrag )
            {
                pActor->setLinearVelocity( pActor->getLinearVelocity() * (1.0f - _Data.fJointDrag) );
            }
            else if ( _Data.fDrag)
            {
                pActor->setLinearVelocity( pActor->getLinearVelocity() * (1.0f - _Data.fDrag) );
            }
        }


        _Data.bToAddForce = false;
    }


    NwEmissaryData* GetInternalStructAddress()
    {
        return &_Data;
    }
    NxJointDesc*    jointDesc;
    NxJoint*        pJoint;
    NxActorGuid        uiActorGuidWeFilteredOut;
private:
    NwEmissaryData _Data;
};
#endif
// -------------------------------------------------------------------------------------------------------------------

#ifdef CLIENT
static U32 filterOutCurrentAABBOverlaps(NxActor* pActor, int iSceneNum)
{
    if ( nx_state.hardware )
        return 0; // not supported in HW
    assert( pActor->getNbShapes() == 1);
    assert( nxScene[iSceneNum] );
    NxShape* pShape = pActor->getShapes()[0];
    NxBounds3 bounds;
    pShape->getWorldBounds(bounds);
    NxShape* pOverlappingShapes[1];

    U32 uiNumOverlap = nxScene[iSceneNum]->overlapAABBShapes(bounds, NX_ALL_SHAPES, 1, pOverlappingShapes, NULL, 1 << nxGroupKinematic );

    for (U32 uiOverlapIndex = 0; uiOverlapIndex < uiNumOverlap; ++uiOverlapIndex)
    {
        if ( pOverlappingShapes[uiOverlapIndex] == pShape )
            continue;
        //NxGroupType group = (NxGroupType)pOverlappingShapes[uiOverlapIndex]->getGroup();
        nxScene[iSceneNum]->setActorPairFlags(pOverlappingShapes[uiOverlapIndex]->getActor(), *pActor, NX_IGNORE_PAIR);
        if ( pActor->userData )
        {
            NwEmissary* pEmissary = (NwEmissary*)pActor->userData;
            NxActor* pKinematicActor = &pOverlappingShapes[uiOverlapIndex]->getActor();
            if ( pKinematicActor && pKinematicActor->userData )
            {
                NwEmissary* pKinematicEmissary = (NwEmissary*)pKinematicActor->userData;
                pEmissary->uiActorGuidWeFilteredOut = pKinematicEmissary->getActorGuid();
            }
        }
    }

    return uiNumOverlap;
}
#endif

static NxActorGuid addActorToGuidTable(NxActor* pActor)
{
    static NxActorGuid uiCurrentGuid = 1; // we use an incrementing guid generator
    if ( !actorGuidTable )
        actorGuidTable = stashTableCreateInt(512);

    // Generate guid
    NxActorGuid uiOurGuid = uiCurrentGuid++;
    bool bUnique = stashIntAddPointer(actorGuidTable, uiOurGuid, pActor, false);
    if (!bUnique)
    {
        Errorf("Novodex actor guid %d somehow already in use!\n", uiOurGuid);
    }
    return uiOurGuid;
}

static NxActor* getActorFromGuid(NxActorGuid uiActorGuid)
{
    NxActor* pActor;
    if (!actorGuidTable || !uiActorGuid)
        return NULL;
    if (stashIntFindPointer(actorGuidTable, uiActorGuid, (void**)&pActor))
        return pActor;
    return NULL;
}

static NxActor* removeActorFromGuidTable(NxActorGuid uiActorGuid)
{
    NxActor* pActor;
    if (!actorGuidTable || !uiActorGuid)
        return NULL;
    if (stashIntRemovePointer(actorGuidTable, uiActorGuid, (void**)&pActor))
        return pActor;
    return NULL;
}


static NwEmissaryDataGuid addNwEmissaryDataToGuidTable(NwEmissaryData* pData)
{
    static NwEmissaryDataGuid uiCurrentGuid = 1; // we use an incrementing guid generator
    if ( !nwEmissaryDataGuidTable )
        nwEmissaryDataGuidTable = stashTableCreateInt(512);

    // Generate guid
    NwEmissaryDataGuid uiOurGuid = uiCurrentGuid++;
    bool bUnique = stashIntAddPointer(nwEmissaryDataGuidTable, uiOurGuid, pData, false);
    if (!bUnique)
    {
        Errorf("Novodex emissary data guid %d somehow already in use!\n", uiOurGuid);
    }
    return uiOurGuid;
}

NwEmissaryData* nwGetNwEmissaryDataFromGuid(NwEmissaryDataGuid uiDataGuid)
{
    NwEmissaryData* pData;
    if (!nwEmissaryDataGuidTable || !uiDataGuid)
        return NULL;
    if (stashIntFindPointer(nwEmissaryDataGuidTable, uiDataGuid, (void**)&pData))
        return pData;
    return NULL;
}

static NwEmissaryData* removeNwEmissaryDataFromGuidTable(NwEmissaryDataGuid uiDataGuid)
{
    NwEmissaryData* pData;
    if (!nwEmissaryDataGuidTable || !uiDataGuid)
        return NULL;
    if (stashIntRemovePointer(nwEmissaryDataGuidTable, uiDataGuid, (void**)&pData))
        return pData;
    return NULL;
}















// -------------------------------------------------------------------------------------------------------------------
// USER CONTACT REPORT (called upon actor collisions)
// -------------------------------------------------------------------------------------------------------------------
#ifdef CLIENT
class NwUserContactNotifyFxGeo : public NxUserContactReport
{
public:
    virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
    {

        if ( events & NX_NOTIFY_ON_START_TOUCH )
        {
            if (
                ( pair.actors[0]->getNbShapes() > 0 && pair.actors[0]->getShapes()[0]->getGroup() == nxGroupKinematic ) 
                || ( pair.actors[1]->getNbShapes() > 0 && pair.actors[1]->getShapes()[0]->getGroup() == nxGroupKinematic ) 
                )
            {
                // If either is a kinematic, do nothing
                return;
            }
            if ( nx_state.hardware )
            {
                F32 fContactForceTotal = 0.02f;
                NwEmissary* pEmissaryA = (NwEmissary*)pair.actors[0]->userData;
                NwEmissary* pEmissaryB = (NwEmissary*)pair.actors[1]->userData;

                if (
                    ( pEmissaryA && pEmissaryA->getAge() < 5 )
                    || ( pEmissaryB && pEmissaryB->getAge() < 5 )
                    )
                    return;

                if ( pEmissaryA )
                    pEmissaryA->NotifyFxGeoStartTouch( false, NULL, NULL, fContactForceTotal );
                if ( pEmissaryB )
                    pEmissaryB->NotifyFxGeoStartTouch( false, NULL, NULL, fContactForceTotal );
            }
            else
            {
                NxVec3 vContactPos, vContactNorm;
                NxVec3 vBestContactPos, vBestContactNorm;
                bool bContactPointExists = false;

                float fContactForceTotal = pair.sumFrictionForce.magnitudeSquared() + pair.sumNormalForce.magnitudeSquared();

                NxContactStreamIterator streamIter(pair.stream);
                while(streamIter.goNextPair())
                {
                    //user can also call getShape() and getNumPatches() here
                    while(streamIter.goNextPatch())
                    {
                        //user can also call getPatchNormal() and getNumPoints() here
                        vContactNorm = streamIter.getPatchNormal();

                        while(streamIter.goNextPoint())
                        {
                            vContactPos = streamIter.getPoint();

                            /*
                            printVec3(vContactPos);
                            printf("\t");
                            printVec3(vContactNorm);-
                            printf("\n");
                            */
                            vBestContactNorm = vContactNorm;
                            vBestContactPos = vContactPos;
                            bContactPointExists = true;
                        }
                    }
                }

                if (fabsf(vBestContactNorm.magnitudeSquared() - 1.0f) > 0.1f)
                {
                    vBestContactNorm.zero();
                    vBestContactNorm.y = 1.0f;
                }

                // The contact normal should always point toward the dynamic object, for consitency.

                if ( pair.actors[0]->isDynamic() && !pair.actors[1]->isDynamic() )
                {
                    // actor 0 is dynamic, flip the normal if it points away
                    NxVec3 vTowardsDynamicActor = pair.actors[0]->getGlobalPosition() - vBestContactPos;
                    if ( vBestContactNorm.dot(vTowardsDynamicActor) < 0.0f  )
                        vBestContactNorm *= -1.0f;
                }
                else if ( !pair.actors[0]->isDynamic() && pair.actors[1]->isDynamic() )
                {
                    // actor 1 is dynamic, flip the normal if it points away
                    NxVec3 vTowardsDynamicActor = pair.actors[1]->getGlobalPosition() - vBestContactPos;
                    if ( vBestContactNorm.dot(vTowardsDynamicActor) < 0.0f  )
                        vBestContactNorm *= -1.0f;        
                }
                // else, they are both dynamic, in which case we don't know what to do!
                vBestContactPos += vBestContactNorm * 0.1f;
                vBestContactPos += NxVec3(qfrand() * 0.02f, qfrand() * 0.02f, qfrand() * 0.02f);

                NwEmissary* pEmissary = (NwEmissary*)pair.actors[0]->userData;
                if ( pEmissary )
                    pEmissary->NotifyFxGeoStartTouch( bContactPointExists, &vBestContactPos, &vBestContactNorm, fContactForceTotal );
                pEmissary = (NwEmissary*)pair.actors[1]->userData;
                if ( pEmissary )
                    pEmissary->NotifyFxGeoStartTouch( bContactPointExists, &vBestContactPos, &vBestContactNorm, fContactForceTotal );
            }
        }
        else if ( events & NX_NOTIFY_ON_TOUCH && !nx_state.hardware)
        {
            NwEmissary* pEmissary = (NwEmissary*)pair.actors[0]->userData;
            if ( pEmissary && pEmissary->wantsEndTouchNotification())
                pEmissary->NotifyFxGeoEndTouch( pair.actors[1] );
            pEmissary = (NwEmissary*)pair.actors[1]->userData;
            if ( pEmissary && pEmissary->wantsEndTouchNotification())
                pEmissary->NotifyFxGeoEndTouch( pair.actors[0] );
        }

    }
} NwInstancedUserContactNotifyFxGeo;


class NwJointBreakNotify : public NxUserNotify
{
    virtual bool onJointBreak(NxReal breakingForce, NxJoint & brokenJoint)
    {
        // make changes to the actor
        NxActor* pActorA;
        NxActor* pActorB;
        brokenJoint.getActors(&pActorA, &pActorB);

        if ( pActorA && pActorA->userData )
        {
            NwEmissary* pEmissary = (NwEmissary*)pActorA->userData;
            pEmissary->notifyJointBroke(breakingForce, brokenJoint.getGlobalAnchor());
        //    pActorA->getShapes()[0]->setGroup(nxGroupLargeDebris);
        }

        if ( pActorB && pActorB->userData )
        {
            NwEmissary* pEmissary = (NwEmissary*)pActorB->userData;
            pEmissary->notifyJointBroke(breakingForce, brokenJoint.getGlobalAnchor());
        //    pActorB->getShapes()[0]->setGroup(nxGroupLargeDebris);
        }

        return true;
    }
    virtual void onWake(NxActor** actors, NxU32 count)
    {
        return;
    }

    virtual void onSleep(NxActor** actors, NxU32 count)
    {
        return;
    }

} NwUserJointBreakNotify;
#endif













// -------------------------------------------------------------------------------------------------------------------
// NOVODEX INIT/SHUTDOWN FUNCTIONS (immediate)
// -------------------------------------------------------------------------------------------------------------------
void  nwSetNovodexDefaults( )
{
    // don't set nx_state.allowed here, unless you want to make sure it is always on or off

    // this is because it is now stored in the registry via the gfx_settings menu.

}

U8 nwInitializeNovodex( )
{
#ifdef CLIENT
    //    if (nx_state.initialized || !nx_state.allowed || !IsUsingWin2kOrXp())
    if ( nx_state.initialized || !nx_state.allowed || !nx_state.gameInPhysicsPossibleState || !IsUsingWin2kOrXp() )
        return false;
#else
    if ( nx_state.initialized || nx_state.notAllowed )
        return false;
#endif
    nx_state.initialized = 1;
    nx_state.initTick = global_state.global_frame_count;
    nx_state.maxDynamicActorCount = DEFAULT_MAX_DYNAMIC_ACTOR_COUNT;
    nx_state.maxNewDynamicActors = DEFAULT_MAX_NEW_DYNAMIC_ACTORS;
    nx_state.entCollision = 1;
#ifdef SERVER 
    nx_state.threaded = 1;
#else
    nx_state.threaded = 1;
#endif 

#ifdef CLIENT
    nwCreateThreadQueues();
#elif SERVER && !BEACONIZER
    nwCreateRagdollQueues();
#endif
    nwCreatePhysicsSDK();

#ifdef CLIENT 
    int iScene = nwCreateScene();
    assert( iScene == NX_CLIENT_SCENE);
#endif

    if ( createNxWorldShapes() > 0 )
    {
        initActorSporeGrid();
    }
    else
    {
        nx_state.gameInPhysicsPossibleState = 0;
        nwDeinitializeNovodex();
        return false;
    }

#ifdef CLIENT
    addInfluenceSphere(0, playerPtr());
    fxInitDebrisManager();
#endif

    return true;
}
// -------------------------------------------------------------------------------------------------------------------
U8 nwDeinitializeNovodex( )
{
    if ( !nx_state.initialized )
        return false;
    nx_state.initialized = 0;
    nx_state.deinitTick = global_state.global_frame_count;
#ifdef CLIENT
    fxDeinitDebrisManager();
#endif

    // stop threads
    nwEndAllThreads();

    removeAllInfluenceSpheres();

    deinitActorSporeGrid();

#ifdef CLIENT
    nwDeleteThreadQueues();
    nwDestroyActorFunctionQueues( );
#elif SERVER && !BEACONIZER
    nwDeleteRagdollQueues();
#endif

    destroyConvexShapeCache();

    destroyNxWorldShapes();
    nwDeleteAllScenes();
    nwDeletePhysicsSDK();
    // clear the guid tables
    if(actorGuidTable)
    {
        if ( stashGetValidElementCount(actorGuidTable) > 0 )
            Errorf("Actor guid table not empty");
        stashTableDestroy(actorGuidTable);
        actorGuidTable = NULL;
    }
    if(nwEmissaryDataGuidTable)
    {
//        if ( stashGetValidElementCount(nwEmissaryDataGuidTable) > 0 )
//            Errorf("NwEmissaryData guid table not empty");
        stashTableDestroy(nwEmissaryDataGuidTable);
        nwEmissaryDataGuidTable = NULL;
    }

    nx_state.dynamicActorCount = 0;
    nx_state.staticActorCount = 0;
    nx_state.meshShapeCount = 0;
    return true;
}
// -------------------------------------------------------------------------------------------------------------------
#ifdef CLIENT
void nwCreateThreadQueues()
{
    assert( !actorDescQueue && !actorDeletionQueue && !materialDescQueue);

    const int iMaxQueueSize = 32768;
    actorDescQueue = createQueue();
    actorDeletionQueue = createQueue();
    materialDescQueue = createQueue();

    qSetMaxSizeLimit( actorDescQueue, iMaxQueueSize );
    qSetMaxSizeLimit( actorDeletionQueue, iMaxQueueSize );
    qSetMaxSizeLimit( materialDescQueue, iMaxQueueSize );
    
    initQueue( actorDescQueue, 100 );
    initQueue( actorDeletionQueue, 100 );
    initQueue( materialDescQueue, 100 );
    uiCurrentNumMaterials = 1; // there is another default material
}
// -------------------------------------------------------------------------------------------------------------------
void nwDeleteThreadQueues()
{
    if ( !actorDescQueue )
    {
        assert( !actorDeletionQueue && !materialDescQueue);
        return;
    }

    // flush queues

    if ( nxScene[NX_CLIENT_SCENE] )
    {
        processActorCreationQueue(NX_CLIENT_SCENE, false);
        processActorDeletionQueue(NX_CLIENT_SCENE);
        processMaterialDescQueue(NX_CLIENT_SCENE, false);
    }

    assert( qGetSize(actorDescQueue) == 0 && qGetSize(actorDeletionQueue) == 0 && qGetSize(materialDescQueue) == 0);

    destroyQueue(actorDescQueue);
    destroyQueue(actorDeletionQueue);
    destroyQueue(materialDescQueue);

    actorDescQueue = NULL;
    actorDeletionQueue = NULL;
    materialDescQueue = NULL;
}
#endif

// -------------------------------------------------------------------------------------------------------------------
void nwCreatePhysicsSDK()
{
    // Create the physics SDK and set relevant parameters.

    if(nxSDK)
    {
        return;
    }


    NxPhysicsSDKDesc desc;

#ifdef SERVER
    desc.flags = NX_SDKF_NO_HARDWARE;
#else
    desc.flags &= ~NX_SDKF_NO_HARDWARE;
#endif


    debugAllocator = new MyAllocator();
    nxSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, debugAllocator, &nwOutputStream,desc);
    assert(nxSDK);

    nxUtilLib = NxGetUtilLib();

    //nxSDK->setParameter(NX_MIN_SEPARATION_FOR_PENALTY, -0.02f);
#ifdef SERVER
    nxSDK->setParameter(NX_CONTINUOUS_CD, 1);
#elif CLIENT
    nxSDK->setParameter(NX_SKIN_WIDTH,0.3f);
#endif
//    nxSDK->setGlobalExceptionHandler( nwGiveUpExceptionHandler );
    //nxSDK->setParameter(NX_BOUNCE_THRESHOLD, -20);
    nxSDK->setParameter(NX_BOUNCE_THRESHOLD, -20); // Lets try the bounce threshold




    for (int i=0; i<MAX_NX_SCENES; i++)
    {
        if ( nxScene[i] != NULL )
        {
            nxScene[i] = NULL;
        }
    }

}
// -------------------------------------------------------------------------------------------------------------------
void nwDeletePhysicsSDK()
{
    nwDeleteAllScenes();
    nxSDK->release();
    nxSDK = NULL;

    delete debugAllocator;
    debugAllocator = NULL;
}
// -------------------------------------------------------------------------------------------------------------------
U8 nwEnabled()
{
#ifdef CLIENT 
    if (nx_state.initialized && nxSDK && nxScene[NX_CLIENT_SCENE])
        return true;
#elif SERVER
    if ( nx_state.initialized && nxSDK )
        return true;
#endif
    return false;
}
// -------------------------------------------------------------------------------------------------------------------
void nwDeleteScene( int iScene )
{
    PERFINFO_AUTO_START("nwDeleteScene", 1);
    assert( nxSDK && nxScene[iScene] );
#ifdef CLIENT
    while (nxScene[iScene]->getNbActors() > 0 )
    {
        NxActor* pActor = nxScene[iScene]->getActors()[0];
        if ( pActor )
        {
            removeActor(pActor, iScene);
        }
    }
#endif
    nxSDK->releaseScene(*(nxScene[iScene]));
    nxThreadActive[iScene] = 0;
    nxScene[iScene] = NULL;
    nx_state.sceneCount--;
    PERFINFO_AUTO_STOP();
}
static void nwDeleteAllScenes( )
{
    for (int i=0; i<MAX_NX_SCENES; i++)
    {
        if ( nxScene[i] != NULL )
        {
            nwDeleteScene(i);
        }
    }

}

U8 nwHardwarePresent()
{
    return !!(nxSDK && nxSDK->getHWVersion() != NX_HW_VERSION_NONE);
}


int nwCreateScene()
{
    int iNewScene = 0;

    if ( !nxSceneConstants.bInited )
    {
        nxSceneConstants.timeStepSize = 1.0f / 60.0f;
        nxSceneConstants.timeStepMax = 10;
        nxSceneConstants.gravity = -80.0f;
        nxSceneConstants.restitution = 0.5f;
#ifdef SERVER
        nxSceneConstants.staticFriction = 1.0f;
        nxSceneConstants.dynamicFriction = 0.75f;
        nxSceneConstants.shapeSkinWidth = 0.10f;
#else
        nxSceneConstants.staticFriction = 0.75f;
        nxSceneConstants.dynamicFriction = 0.5f;
        nxSceneConstants.shapeSkinWidth = 0.05f;
#endif        
        nxSceneConstants.bInited = true;
    }
    PERFINFO_AUTO_START("nwCreateScene", 1);

    for (iNewScene=0; iNewScene < nxMaxScenes; ++iNewScene)
    {
        if (!nxScene[iNewScene])
            break;
    }
    
    if ( iNewScene == nxMaxScenes )
    {
        PERFINFO_AUTO_STOP();
        return -1; // error code, we failed to create a new scene
    }

    assert( nxScene[iNewScene] == NULL );

    // Create a new scene.

    NxSceneDesc sceneDesc;
    sceneDesc.gravity = NxVec3(0, nxSceneConstants.gravity, 0);

    // On the client, try a HW scene first, then sw
#ifdef CLIENT
    nxScene[iNewScene] = NULL;

    if (!nx_state.softwareOnly && (nxSDK->getHWVersion() != NX_HW_VERSION_NONE))
    {
        sceneDesc.simType = NX_SIMULATION_HW;
        nxScene[iNewScene] = nxSDK->createScene(sceneDesc);
    }

    if (!nxScene[iNewScene]) // no hardware found or allowed
    {
        // try a software scene
        sceneDesc.simType = NX_SIMULATION_SW;
        nxScene[iNewScene] = nxSDK->createScene(sceneDesc);
        nx_state.hardware = 0;
        if (!nxScene[iNewScene])
        {
            PERFINFO_AUTO_STOP();
            return -1;
        }
    }
    else
    {
        nx_state.hardware = 1; // found HW
    }

#else // server is SW only

    sceneDesc.simType = NX_SIMULATION_SW;
    sceneDesc.flags = 0;
    nxScene[iNewScene] = nxSDK->createScene(sceneDesc);
    if (!nxScene[iNewScene])
    {
        PERFINFO_AUTO_STOP();
        return -1;
    }

#endif

    // Now substepping is supported by the hardware, apparently, so use it either way
    nxScene[iNewScene]->setTiming(nxSceneConstants.timeStepSize, nxSceneConstants.timeStepMax, NX_TIMESTEP_FIXED);
#ifdef CLIENT

    // don't let jointed actors collide with static geometry
    
    nxScene[iNewScene]->setGroupCollisionFlag(nxGroupStatic, nxGroupJointed, true); // MDC Test
    nxScene[iNewScene]->setGroupCollisionFlag(nxGroupDebris, nxGroupDebris, true); // Have debris collide with other debris
    
    nxScene[iNewScene]->setUserContactReport(&NwInstancedUserContactNotifyFxGeo );
    nxScene[iNewScene]->setUserNotify(&NwUserJointBreakNotify);

    nxScene[iNewScene]->setActorGroupPairFlags(nxGroupDebris, nxGroupLargeDebris, NX_NOTIFY_ON_START_TOUCH ); // Re-enabled large object collision
    nxScene[iNewScene]->setActorGroupPairFlags(nxGroupDebris, nxGroupStatic, NX_NOTIFY_ON_START_TOUCH );
    nxScene[iNewScene]->setActorGroupPairFlags(nxGroupLargeDebris, nxGroupStatic, NX_NOTIFY_ON_START_TOUCH );

    nxScene[iNewScene]->setActorGroupPairFlags(nxGroupKinematic, nxGroupDebris, NX_NOTIFY_ON_TOUCH );
    nxScene[iNewScene]->setActorGroupPairFlags(nxGroupKinematic, nxGroupLargeDebris, NX_NOTIFY_ON_TOUCH );
#endif

    // Create a default material.
    nxDefaultMaterial[iNewScene] = nxScene[iNewScene]->getMaterialFromIndex(0);
    nxDefaultMaterial[iNewScene]->setRestitution(nxSceneConstants.restitution);
    nxDefaultMaterial[iNewScene]->setStaticFriction(nxSceneConstants.staticFriction);
    nxDefaultMaterial[iNewScene]->setDynamicFriction(nxSceneConstants.dynamicFriction);

    nx_state.sceneCount++;
    nxThreadActive[iNewScene] = 0;
    PERFINFO_AUTO_STOP();
    return iNewScene;
}
// -------------------------------------------------------------------------------------------------------------------
void nwCoreDump()
{
#ifdef CLIENT
    NxCoreDump(nxSDK, "nxcore.dmp", false, true);
#endif
}
// -------------------------------------------------------------------------------------------------------------------
















// -------------------------------------------------------------------------------------------------------------------
// NOVODEX SCENE STEPPING
// -------------------------------------------------------------------------------------------------------------------
static void nwSimulate(double stepsize, int iSceneNum)
{

    if(!nxThreadActive[iSceneNum])
    {
        NxScene* pScene = nxScene[iSceneNum];
        assert( pScene );
        iLastSceneSimulated = iSceneNum;
        pScene->simulate(stepsize); // let novodex handle dividing up the time
        nxThreadActive[iSceneNum] = 1;
    }
}
// -------------------------------------------------------------------------------------------------------------------
static void nwFetchResults(int iSceneNum)
{
    if ( !nwEnabled() )
        return;
    if(nxThreadActive[iSceneNum])
    {
        NxScene* pScene = nxScene[iSceneNum];
        assert( pScene );
        __try {
        while (!pScene->fetchResults(NX_RIGID_BODY_FINISHED, true));
        } __except(nwGiveUpExceptionHandler(GetExceptionCode(), GetExceptionInformation()))
        {
        }
        nxThreadActive[iSceneNum] = 0;
    }

#ifdef CLIENT 
    if ( nx_state.needToReset )
    {
        Errorf("Resetting client side physics due to exception\n");
        nwResetClientSidePhysics();
        nx_state.needToReset = 0;
    }
#endif
}
// -------------------------------------------------------------------------------------------------------------------
#ifdef CLIENT
static void processActorCreationQueue(int iSceneNum, bool bCreateActor)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );

    // Create queued actors!
    NxActorDesc* pNewActorDesc = (NxActorDesc*)qDequeue(actorDescQueue);
    while (pNewActorDesc)
    {
        if ( pNewActorDesc->userData ) // if there is no user data, don't add the actor
        {
            if ( bCreateActor )
            {
                // Before we create the actor, make sure any material indicies are ok
                if (pNewActorDesc->shapes[0] && pNewActorDesc->shapes[0]->materialIndex >= pScene->getNbMaterials() )
                {
                    Errorf("Had to correct faulty material index %u, when material count is %u\n", pNewActorDesc->shapes[0]->materialIndex, pScene->getNbMaterials());
                    pNewActorDesc->shapes[0]->materialIndex = 0; // default material
                }
                NxActor* pNewActor = pScene->createActor( *pNewActorDesc );
                if ( pNewActor )
                {
                    NwEmissary* pEmissary = (NwEmissary*) pNewActor->userData;
                    pEmissary->SetActor(pNewActor);
                    if ( pEmissary->jointDesc )
                    {
                        pEmissary->jointDesc->actor[0] = pNewActor;
                        pEmissary->pJoint = nxScene[iSceneNum]->createJoint(*pEmissary->jointDesc);
                        if ( pEmissary->pJoint )
                            pEmissary->setIsJointed(true);
                        //pEmissary->pJoint->setBreakable(1000.0f, 1000.0f);
                        delete pEmissary->jointDesc;
                        pEmissary->jointDesc = NULL;

                        // jointed actors are in the jointed group
                        if ( !pEmissary->getJointCollidesWorld() )
                            pNewActor->getShapes()[0]->setGroup(nxGroupJointed);
                    }

                    // Add to guid table
                    NxActorGuid uiNewGuid = addActorToGuidTable(pNewActor);
                    pEmissary->setActorGuid(uiNewGuid);

                    // Check to see if there is a kinematic where we created this actor.
                    // if so, filter out the collisions, at least temporarily
                    filterOutCurrentAABBOverlaps(pNewActor, iSceneNum);
                }
                else
                {
                    Errorf("Novodex failed to create actor.");
                }
            }
        }

        delete pNewActorDesc->body;
        while ( pNewActorDesc->shapes.size() > 0 )
        {
            if ( (U32)pNewActorDesc->shapes.back()->userData != NX_SHAPE_STORED )
                delete pNewActorDesc->shapes.back();
            pNewActorDesc->shapes.popBack();
        }
        delete pNewActorDesc;

        pNewActorDesc = (NxActorDesc*)qDequeue(actorDescQueue);
    }
}


// -------------------------------------------------------------------------------------------------------------------
static void processMaterialDescQueue(int iSceneNum, bool bCreateMaterial)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );

    // Create queued actors!
    NxMaterialDesc* pNewMaterialDesc = (NxMaterialDesc*)qDequeue(materialDescQueue);
    while (pNewMaterialDesc)
    {
        if ( bCreateMaterial )
        {
            NxMaterial* pNewMaterial = pScene->createMaterial( *pNewMaterialDesc );
            if ( !pNewMaterial )
            {
                Errorf("Novodex failed to create material.");
                --uiCurrentNumMaterials;
            }
        }

        delete pNewMaterialDesc;

        pNewMaterialDesc = (NxMaterialDesc*)qDequeue(materialDescQueue);
    }

    if (pScene->getNbMaterials() != uiCurrentNumMaterials)
    {
        Errorf("Novodex has an incorrect number of materials: %u, expected %u\n", pScene->getNbMaterials(), uiCurrentNumMaterials);
    }
}

// -------------------------------------------------------------------------------------------------------------------
static void removeActor( NxActor* pActor, int iSceneNum )
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );
#ifdef CLIENT
    NwEmissary* pEmissary = (NwEmissary*) pActor->userData;
    if ( pEmissary )
    {
        if ( pEmissary->getActorGuid() )
        {
            removeActorFromGuidTable(pEmissary->getActorGuid());
        }

        if ( pEmissary->pJoint )
        {
            nxScene[iSceneNum]->releaseJoint(*pEmissary->pJoint);
            pEmissary->pJoint = NULL;
        }

        if (pEmissary->GetInternalStructAddress() != removeNwEmissaryDataFromGuidTable(pEmissary->getGuid()))
        {
            Errorf("Failed to properly remove nw emissary data with guid %d\n", pEmissary->getGuid());
        }

        delete pEmissary;
    }
    else if ( pActor->isDynamic() )
    {
        Errorf("Dyanmic actors should always have an NwEmissary!\n");
    }
    if ( pActor->isDynamic())
        nx_state.dynamicActorCount--;
    else
        nx_state.staticActorCount--;
    //delete pActor->userData;
    pActor->userData = NULL;

    if ( nx_state.hardware )
    {
        NxShape* const pShape = pActor->getShapes()[0];
        if ( pShape )
        {
            NxTriangleMeshShape* pTriShape = pShape->isTriangleMesh();
            if ( pTriShape )
            {
                U32 uiPCount = pTriShape->getTriangleMesh().getPageCount();
                while ( uiPCount )
                {
                    if ( pTriShape->isPageInstanceMapped(--uiPCount) )
                        pTriShape->unmapPageInstance(uiPCount);
                }

            }
        }
    }
#endif
    pScene->releaseActor(*pActor);
}
// -------------------------------------------------------------------------------------------------------------------
static void processActorDeletionQueue(int iSceneNum)
{
    NxActor* pActor = (NxActor*)qDequeue(actorDeletionQueue );

    while ( pActor )
    {
        removeActor( pActor, iSceneNum );
        pActor = (NxActor*)qDequeue(actorDeletionQueue );
    }
}
static void processEveryActorCallback(int iSceneNum )
{
    int iTotalActors = nxScene[iSceneNum]->getNbActors();

    for (int i = 0; i<iTotalActors; ++i)
    {
        NxActor* pActor = nxScene[iSceneNum]->getActors()[i];
        if ( pActor && pActor->userData )
        {
            NwEmissary* pUserdata = (NwEmissary*)pActor->userData;
            if ( pUserdata )
            {
                if ( !pUserdata->GetActor() )
                {
                    pUserdata->SetActor(pActor);
                }

                // Callback!
                pUserdata->everyFrame();
            }
        }
    }

}
#endif
// -------------------------------------------------------------------------------------------------------------------
#ifdef SERVER
float fTimeSinceLastUpdate = 0.0f;
#endif


#define FOR_EVERY_VALID_SCENE     for (iSceneNum=0; iSceneNum<MAX_NX_SCENES; iSceneNum++) if ( nxScene[iSceneNum] != NULL )


void nwStepScenes(double stepsize)
{
    int iSceneNum;
    if(nx_state.threaded)
    {

        PERFINFO_AUTO_START("NovodeX-Threaded", 1);
            PERFINFO_AUTO_START("fetch", 1);
                FOR_EVERY_VALID_SCENE
                {
                    nwFetchResults(iSceneNum);
#ifdef CLIENT
                    if ( nx_state.debug && nwEnabled())
                    {
                        const NxDebugRenderable* pDebugRenderable = nxScene[iSceneNum]->getDebugRenderable();
                        nwDebugRender(pDebugRenderable);
                    }
#endif
                }

                // nwfetch results can kill novodex
                if (!nwEnabled())
                {
                    PERFINFO_AUTO_STOP();
                    PERFINFO_AUTO_STOP();
                    return;
                }

#ifdef SERVER
#if !BEACONIZER
            PERFINFO_AUTO_STOP_START("processRagdollDeletionQueues", 1);
            processRagdollDeletionQueues();
            PERFINFO_AUTO_STOP_START("processRagdollQueue", 1);
            nwProcessRagdollQueue();
#endif
#endif
            PERFINFO_AUTO_STOP_START("updateNxActors", 1);
                FOR_EVERY_VALID_SCENE updateNxActors(iSceneNum);
#ifdef CLIENT
            PERFINFO_AUTO_STOP_START("processActorQueues", 1);
            FOR_EVERY_VALID_SCENE {
                processActorFunctionOneParamQueue(true);
                processMaterialDescQueue(iSceneNum, true);
                processActorCreationQueue(iSceneNum, true);
                processActorDeletionQueue(iSceneNum);
                //processForceQueue(iSceneNum);
                processEveryActorCallback(iSceneNum);
            }
#endif
            PERFINFO_AUTO_STOP_START("simulate", 1);
            FOR_EVERY_VALID_SCENE nwSimulate(stepsize, iSceneNum);
            PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
    }
    else
    {
        PERFINFO_AUTO_START("NovodeX-Unthreaded", 1);

#ifdef SERVER
#if !BEACONIZER
        PERFINFO_AUTO_STOP_START("processRagdollDeletionQueues", 1);
        processRagdollDeletionQueues();
        PERFINFO_AUTO_STOP_START("processRagdollQueue", 1);
        nwProcessRagdollQueue();
#endif
#endif

        FOR_EVERY_VALID_SCENE
        {

            PERFINFO_AUTO_START("updateNxActors", 1);
                updateNxActors(iSceneNum);
#ifdef CLIENT
            PERFINFO_AUTO_STOP_START("processActorQueues", 1);
                processMaterialDescQueue(iSceneNum, true);
                processActorCreationQueue(iSceneNum, true);
                processActorDeletionQueue(iSceneNum);
                //processForceQueue(iSceneNum);
                processEveryActorCallback(iSceneNum);
#endif
            PERFINFO_AUTO_STOP_START("simulate", 1);
                nwSimulate(stepsize, iSceneNum);
            PERFINFO_AUTO_STOP_START("fetch", 1);
                nwFetchResults(iSceneNum);

            PERFINFO_AUTO_STOP();
        }
        PERFINFO_AUTO_STOP();
    }
}

// -------------------------------------------------------------------------------------------------------------------
void nwEndThread(int iSceneNum) // warning, this could cause confusion, only to block until thread is done
{
    nwFetchResults(iSceneNum);

    assert( !nwEnabled() || !nxThreadActive[iSceneNum]);
}
// -------------------------------------------------------------------------------------------------------------------
static void nwEndAllThreads()
{
    int iSceneNum;
    FOR_EVERY_VALID_SCENE
    {
        nwEndThread(iSceneNum);
    }
}
// -------------------------------------------------------------------------------------------------------------------














// -------------------------------------------------------------------------------------------------------------------
// NOVODEX GLOBAL READ (immediate)
// -------------------------------------------------------------------------------------------------------------------
float nwGetDefaultGravity()
{
    assert(nxScene[NX_CLIENT_SCENE]);
    NxVec3 vGrav;
    nxScene[NX_CLIENT_SCENE]->getGravity(vGrav);
    return vGrav.y;
}
// -------------------------------------------------------------------------------------------------------------------












// -------------------------------------------------------------------------------------------------------------------
// MESH COOKING (delayed)
// -------------------------------------------------------------------------------------------------------------------
U8 nwCookMeshShape(const char* name, const void* vertArray, int vertCount, int vertStride, const void* triArray, int triCount, int triStride, char** streamOutput, int* streamSize)
{
    NwSharedStream stream(name, true);

    // Create descriptor for concave mesh
    NxTriangleMeshDesc meshDesc;
    meshDesc.numVertices         = vertCount;
    meshDesc.pointStrideBytes    = vertStride;
    meshDesc.points              = vertArray;
    meshDesc.numTriangles        = triCount;
    meshDesc.triangles           = triArray;
    meshDesc.triangleStrideBytes = triStride;
    meshDesc.flags               = 0;

    NxInitCooking();



#ifdef PARANOID_CHECK_MESH_DATA
    for(int i = 0; i < (int)meshDesc.numTriangles * 3; i++)
    {
        assert(((int*)meshDesc.triangles)[i] >= 0 && ((int*)meshDesc.triangles)[i] < (int)meshDesc.numVertices);
    }

    for(int i = 0; i < (int)meshDesc.numVertices; i++)
    {
        int iFloatStartByte = i * meshDesc.pointStrideBytes;
        F32* pFirstFloat = (F32*)(((char*)meshDesc.points) + iFloatStartByte);
        for (int j=0; j<3; j++)
        {
            assert(fabsf(pFirstFloat[j]) < 1e6);
        }
    }

#endif

    PERFINFO_AUTO_START("novodexCooking", 1);
    bool status = NxCookTriangleMesh(meshDesc, stream);
    PERFINFO_AUTO_START("copying stream", 1);
    if ( !status )
        return false;
    assert( streamOutput );
    *streamSize = stream.getBufferSize();
    *streamOutput = (char*)malloc(*streamSize);
    memcpy(*streamOutput, stream.getBuffer(), *streamSize);
    PERFINFO_AUTO_STOP();

    return true;
}

void* nwLoadMeshShape(const char* pcStream, int iStreamSize)
{
    PERFINFO_AUTO_START("copyStream", 1);
    NwSharedStream stream((const NxU8*)pcStream, iStreamSize, true);
    NxTriangleMesh* mesh;

    PERFINFO_AUTO_STOP_START("createTriangleMesh", 1);
    mesh = nxSDK->createTriangleMesh(stream);
    PERFINFO_AUTO_STOP();

    NxTriangleMeshShapeDesc* shapeDesc = new NxTriangleMeshShapeDesc();
    shapeDesc->meshData = mesh;

    nx_state.meshShapeCount++;

    return reinterpret_cast< void* >(shapeDesc);
}

void* nwCookAndLoadMeshShape(const char* name, const void* vertArray, int vertCount, int vertStride, const void* triArray, int triCount, int triStride )
{

    NxTriangleMeshShapeDesc* shapeDesc;
    /*
    if (meshShapeTable && stashFindPointer(meshShapeTable, name, (void**)(&shapeDesc) ) )
        return shapeDesc;
        */

    NwSharedStream stream(name, true);
    NxTriangleMesh* mesh;

    // Create descriptor for concave mesh
    NxTriangleMeshDesc meshDesc;
    meshDesc.numVertices         = vertCount;
    meshDesc.pointStrideBytes    = vertStride;
    meshDesc.points              = vertArray;
    meshDesc.numTriangles        = triCount;
    meshDesc.triangles           = triArray;
    meshDesc.triangleStrideBytes = triStride;
#ifdef CLIENT
    if (nx_state.hardware)
        meshDesc.flags               = NX_MF_HARDWARE_MESH;
    else
#endif
        meshDesc.flags               = 0;

    NxInitCooking();



#ifdef PARANOID_CHECK_MESH_DATA
    for(int i = 0; i < (int)meshDesc.numTriangles * 3; i++)
    {
        assert(((int*)meshDesc.triangles)[i] >= 0 && ((int*)meshDesc.triangles)[i] < (int)meshDesc.numVertices);
    }

    for(int i = 0; i < (int)meshDesc.numVertices; i++)
    {
        int iFloatStartByte = i * meshDesc.pointStrideBytes;
        F32* pFirstFloat = (F32*)(((char*)meshDesc.points) + iFloatStartByte);
        for (int j=0; j<3; j++)
        {
            assert(fabsf(pFirstFloat[j]) < 1e6);
        }
    }

#endif

    /*
    if (stricmp(name, "_arenaint_platform_01") == 0)
    {
        // dump it
        nwDebugDumpTriMesh(name, meshDesc);
    }
    */

    PERFINFO_AUTO_START("novodexCooking", 1);
    bool status = NxCookTriangleMesh(meshDesc, stream);
    assert(status);
    stream.resetByteCounter();
    PERFINFO_AUTO_STOP_START("createTriangleMesh", 1);
    mesh = nxSDK->createTriangleMesh(stream);
    PERFINFO_AUTO_STOP();

    shapeDesc = new NxTriangleMeshShapeDesc();
    shapeDesc->meshData = mesh;
    // debug shapeDesc->userData = strdup(name);

    nx_state.meshShapeCount++;

    /*
    if (!meshShapeTable)
        meshShapeTable = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);
    stashAddPointer(meshShapeTable, name, shapeDesc, false);
    */


    return reinterpret_cast< void* >(shapeDesc);
}
// -------------------------------------------------------------------------------------------------------------------
void nwFreeMeshShape(void* shapeParam)
{

    if(!shapeParam || !nxSDK)
    {
        return;
    }
    
    assert(nx_state.meshShapeCount > 0);
    
    nx_state.meshShapeCount--;

    NxTriangleMeshShapeDesc* shapeDesc = (NxTriangleMeshShapeDesc*)shapeParam;


    if ( shapeDesc->meshData )
        nxSDK->releaseTriangleMesh(*shapeDesc->meshData);
    
    delete shapeDesc;
}
// -------------------------------------------------------------------------------------------------------------------
void nwFreeStreamBuffer()
{
    NwSharedStream::freeTempBuffer();
}
// -------------------------------------------------------------------------------------------------------------------














// -------------------------------------------------------------------------------------------------------------------
// ACTOR CREATION/DELETION (delayed)
// -------------------------------------------------------------------------------------------------------------------
U8 nwCanAddDynamicActor()
{
    if (
        ( !nx_state.maxDynamicActorCount || nx_state.dynamicActorCount < nx_state.maxDynamicActorCount )
        && ( qGetSize(actorDescQueue) < nx_state.maxNewDynamicActors )
        )
        return true;
    return false;
}
void* nwCreateBoxActor(Mat4 mWorldSpaceMat, Vec3 vSize, NxGroupType group, float density, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );
    NxBoxShapeDesc boxDesc;
    boxDesc.dimensions = vSize;
    boxDesc.materialIndex = nxDefaultMaterial[iSceneNum]->getMaterialIndex();
    boxDesc.group = group;

    NxBodyDesc bodyDesc;

    NxActorDesc actorDesc;
    actorDesc.body    = density ? &bodyDesc   : NULL;
    actorDesc.density = density;
    nwSetNxMat34FromMat4( actorDesc.globalPose, mWorldSpaceMat);
    actorDesc.shapes.pushBack(&boxDesc);

    PERFINFO_AUTO_START("createActor", 1);
    
    NxActor* actor = pScene->createActor(actorDesc);    

    PERFINFO_AUTO_STOP();
    if ( density )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;
    
    return actor;
}
// -------------------------------------------------------------------------------------------------------------------
void* nwCreateSphereActor(Mat4 mWorldSpaceMat, float fRadius, NxGroupType group, float fDensity, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );
    NxSphereShapeDesc sphereDesc;
    sphereDesc.radius = fRadius;
    sphereDesc.materialIndex = nxDefaultMaterial[iSceneNum]->getMaterialIndex();
    sphereDesc.group = group;

    NxBodyDesc bodyDesc;

    NxActorDesc actorDesc;
    actorDesc.body    = fDensity ? &bodyDesc   : NULL;
    actorDesc.density = fDensity;
    nwSetNxMat34FromMat4( actorDesc.globalPose, mWorldSpaceMat);
    actorDesc.shapes.pushBack(&sphereDesc);

    PERFINFO_AUTO_START("createActor", 1);

    NxActor* actor = pScene->createActor(actorDesc);    

    PERFINFO_AUTO_STOP();

    if ( fDensity )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;



    return actor;
}
// -------------------------------------------------------------------------------------------------------------------
#ifdef CLIENT
NxMaterialIndex  nwGetMaterialIndexFromGeoOwner( const FxGeo* geoOwner, int iSceneNum )
{
    if ( geoOwner )
    {
        NxMaterialDesc materialDesc;
        materialDesc.restitution = geoOwner->bhvr->physRestitution;
        materialDesc.staticFriction = geoOwner->bhvr->physStaticFriction;
        materialDesc.dynamicFriction = geoOwner->bhvr->physDynamicFriction;
        // do bounds checking
        materialDesc.restitution = NXCLAMP(materialDesc.restitution, 0.0f, 1.0f);
        materialDesc.dynamicFriction = NXCLAMP(materialDesc.dynamicFriction, 0.0f, 1.0f);
        materialDesc.staticFriction = NXCLAMP(materialDesc.staticFriction, 0.0f, 5.0f);

        if ( materialDesc.restitution < nxSceneConstants.restitution )
            materialDesc.restitutionCombineMode = NX_CM_MIN;
        else
            materialDesc.restitutionCombineMode = NX_CM_MAX;

#ifdef CLIENT
        if ( materialDesc.dynamicFriction < nxSceneConstants.dynamicFriction )
            materialDesc.frictionCombineMode = NX_CM_MIN;
        else
            materialDesc.frictionCombineMode = NX_CM_MAX;
#endif

        return nwGetIndexFromMaterialDesc(materialDesc, iSceneNum);
    }
    else
        return nxDefaultMaterial[iSceneNum]->getMaterialIndex();

}
// -------------------------------------------------------------------------------------------------------------------
NwEmissaryDataGuid  nwPushSphereActor(const float* position, const float* vel, const float* angVel, float density, float radius, NxGroupType group, const FxGeo* geoOwner, bool bDisabledCollision, int iSceneNum)
{
    /*
    return reinterpret_cast< void* >(novodex->createSphereActor(NxVec3(position[0], position[1], position[2]), 
        density, radius));
    */
    
    NxSphereShapeDesc* sphereDesc = new NxSphereShapeDesc;
    sphereDesc->radius = radius;
    sphereDesc->group = group;

    sphereDesc->materialIndex = nwGetMaterialIndexFromGeoOwner( geoOwner, iSceneNum );


    NxBodyDesc* bodyDesc = new NxBodyDesc;
    
    if(density < 0)
    {
        bodyDesc->flags |= NX_BF_KINEMATIC;
        density *= -1;
    }


    NxActorDesc* actorDesc = new NxActorDesc;
    actorDesc->body    = density ? bodyDesc   : NULL;
    bodyDesc->flags |= NX_BF_FROZEN_ROT;
    scaleVec3(vel, 30.0f, bodyDesc->linearVelocity);
    scaleVec3(angVel, 30.0f, bodyDesc->angularVelocity);
    actorDesc->density = density;
    actorDesc->globalPose.t = position;
    actorDesc->group = group;

    if ( bDisabledCollision )
        actorDesc->flags |= NX_AF_DISABLE_RESPONSE;


    actorDesc->shapes.pushBack(sphereDesc);
    NwEmissary* pEmissary = new NwEmissary;
    pEmissary->setGuid(addNwEmissaryDataToGuidTable(pEmissary->GetInternalStructAddress()));

    actorDesc->userData = pEmissary;
    
    if ( density )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;



    int iSuccess = qEnqueue( actorDescQueue, (void*) actorDesc );
    assert( iSuccess );

    return pEmissary->getGuid();
    //return nxScene->createActor(actorDesc);    
}
// -------------------------------------------------------------------------------------------------------------------
void nwCookAndStoreConvexShape( const char* name, const float* verts, int vertcount, NxGroupType group, const FxGeo* geoOwner, int iSceneNum )
{
    NwSharedStream stream(name, true);
    NxConvexMesh* mesh = NULL;

    {
        const bool bInflate = false;
        NxInitCooking();

        NxConvexMeshDesc convexDesc;
        convexDesc.numVertices = vertcount;
        convexDesc.pointStrideBytes = sizeof(float)*3;
        convexDesc.numTriangles = 0;
        convexDesc.points = verts;
        convexDesc.flags |= NX_CF_COMPUTE_CONVEX;

        if ( bInflate )
        {
            NxCookingParams params;
            params.skinWidth = 0.05f;
            NxSetCookingParams(params);

            convexDesc.flags |= NX_CF_INFLATE_CONVEX;
        }

        bool status = NxCookConvexMesh(convexDesc, stream);
        if ( !status )
        {
            //nwDebugDumpVerts(name, (const float*)convexDesc.points, convexDesc.numVertices, convexDesc.pointStrideBytes);
            Errorf("Novodex sdk failed to cook mesh %s", name);
            return;
        }
        stream.resetByteCounter();
        mesh = nxSDK->createConvexMesh(stream);
        if ( !mesh )
        {
            Errorf("Novodex sdk failed to create cooked mesh %s", name);
            return;
        }
    }


    NxConvexShapeDesc* convexShapeDesc = new NxConvexShapeDesc;
    convexShapeDesc->group = group;
    convexShapeDesc->meshData = mesh;

    convexShapeDesc->skinWidth = nxSceneConstants.shapeSkinWidth;

    convexShapeDesc->materialIndex = nwGetMaterialIndexFromGeoOwner( geoOwner, iSceneNum );

    convexShapeDesc->userData = (void*)NX_SHAPE_STORED;


    if (!convexShapeTable)
        convexShapeTable = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);
    stashAddPointer(convexShapeTable, name, convexShapeDesc, false);
}

U8 nwFindConvexShapeDesc( const char* name, void** convexShapeDesc )
{
    if (convexShapeTable && stashFindPointer(convexShapeTable, name, convexShapeDesc) )
        return true;
    return false;
}

#ifdef CLIENT
NwEmissaryDataGuid  nwPushConvexActor(const char* name, Mat4 convexTransform, const Vec3 velocity, const Vec3 angVelocity, float fDensity, const FxGeo* geoOwner, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );

    NxBodyDesc* bodyDesc = new NxBodyDesc;
    NxActorDesc* actorDesc = new NxActorDesc;

    NxConvexShapeDesc* convexShapeDesc;

    if ( !nwFindConvexShapeDesc(name, (void**)&convexShapeDesc) )
    {
        Errorf("Couldn't find convex shape %s", name);
        return NULL;
    }

    convexShapeDesc->materialIndex = nwGetMaterialIndexFromGeoOwner( geoOwner, iSceneNum );

    actorDesc->shapes.pushBack(convexShapeDesc);
    actorDesc->density = fDensity;
    actorDesc->group = convexShapeDesc->group;

    if (fDensity > 0.0f )
        actorDesc->body = bodyDesc;

    scaleVec3(velocity, 30.0f, bodyDesc->linearVelocity);
    scaleVec3(angVelocity, 30.0f, bodyDesc->angularVelocity);

    nwSetNxMat34FromMat4( actorDesc->globalPose, convexTransform);

    NwEmissary* pEmissary = new NwEmissary;
    pEmissary->setGuid(addNwEmissaryDataToGuidTable(pEmissary->GetInternalStructAddress()));
    actorDesc->userData = pEmissary;

    if ( fDensity )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;

    int iSuccess = qEnqueue( actorDescQueue, (void*) actorDesc );
    assert( iSuccess );

    return pEmissary->getGuid();
}
#endif

NwEmissaryDataGuid nwPushCapsuleActor(const Mat4 mCapsuleTransform, const float fRadius, const float fHeight, NxGroupType group, float fDensity, int iSceneNum)
{
    NxCapsuleShapeDesc* capsuleDesc = new NxCapsuleShapeDesc;
    capsuleDesc->radius = fRadius;
    capsuleDesc->height = fHeight;
    capsuleDesc->group = group;

    capsuleDesc->materialIndex = nxDefaultMaterial[iSceneNum]->getMaterialIndex();


    NxBodyDesc* bodyDesc = new NxBodyDesc;
    
    if(fDensity < 0)
    {
        bodyDesc->flags |= NX_BF_KINEMATIC;
        fDensity *= -1;
    }


    NxActorDesc* actorDesc = new NxActorDesc;
    actorDesc->body    = fDensity ? bodyDesc   : NULL;
    //bodyDesc->flags |= NX_BF_FROZEN_ROT;
    //scaleVec3(vel, 30.0f, bodyDesc->linearVelocity);
    //scaleVec3(angVel, 30.0f, bodyDesc->angularVelocity);
    actorDesc->density = fDensity;
    nwSetNxMat34FromMat4( actorDesc->globalPose, mCapsuleTransform);


    actorDesc->shapes.pushBack(capsuleDesc);
    NwEmissary* pEmissary = new NwEmissary;
    pEmissary->setGuid(addNwEmissaryDataToGuidTable(pEmissary->GetInternalStructAddress()));

    actorDesc->userData = pEmissary;
    
    if ( fDensity )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;



    int iSuccess = qEnqueue( actorDescQueue, (void*) actorDesc );
    assert( iSuccess );

    return pEmissary->getGuid();
}
#endif
// -------------------------------------------------------------------------------------------------------------------
void* nwCreateConvexActor(const char* name, Mat4 convexTransform, const float* verts, int vertcount, NxGroupType group, float fDensity, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );

    
    NwSharedStream stream(name, true);
    NxConvexMesh* mesh = NULL;

    const bool bLoadConvexShapeOffDisk = true;

    /*
    if (bLoadConvexShapeOffDisk && stream.isValid())
    {
        mesh = nxSDK->createConvexMesh(stream);
    }
    else
    */
    {
        NxInitCooking();

        NxConvexMeshDesc convexDesc;
        convexDesc.numVertices = vertcount;
        convexDesc.pointStrideBytes = sizeof(float)*3;
        convexDesc.numTriangles = 0;
        convexDesc.points = verts;
        convexDesc.flags |= NX_CF_COMPUTE_CONVEX;

        bool status = NxCookConvexMesh(convexDesc, stream);
        assert( status );
        if ( !status )
            return NULL;
        stream.resetByteCounter();
        mesh = nxSDK->createConvexMesh(stream);
    }

    NxConvexShapeDesc convexShapeDesc;
    convexShapeDesc.group = group;
    convexShapeDesc.meshData = mesh;
    convexShapeDesc.materialIndex = nxDefaultMaterial[iSceneNum]->getMaterialIndex();
    


    NxBodyDesc bodyDesc;
    NxActorDesc actorDesc;

    actorDesc.shapes.pushBack(&convexShapeDesc);
    actorDesc.density = fDensity;
    if (fDensity > 0.0f )
        actorDesc.body = &bodyDesc;
    
    nwSetNxMat34FromMat4( actorDesc.globalPose, convexTransform);
    
#ifdef CLIENT
    NwEmissary* pEmissary = new NwEmissary;
    pEmissary->setGuid(addNwEmissaryDataToGuidTable(pEmissary->GetInternalStructAddress()));
    actorDesc.userData = pEmissary;
#endif


    if ( fDensity )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;

    return pScene->createActor(actorDesc);


}

void* nwCreateCapsuleActor(Mat4 mCapsuleTransform, const float fRadius, const float fHeight, NxGroupType group, float fDensity, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );
    
    NxCapsuleShapeDesc locCapsuleDesc;
    NxBodyDesc locBodyDesc;
    NxActorDesc locActorDesc;

    NxCapsuleShapeDesc* capsuleDesc = &locCapsuleDesc;//new NxCapsuleShapeDesc;
    capsuleDesc->radius = fRadius;
    capsuleDesc->height = fHeight;
    capsuleDesc->group = group;

    capsuleDesc->materialIndex = nxDefaultMaterial[iSceneNum]->getMaterialIndex();



    NxBodyDesc* bodyDesc = &locBodyDesc;//new NxBodyDesc;

    if(fDensity < 0)
    {
        bodyDesc->flags |= NX_BF_KINEMATIC;
        fDensity *= -1;
    }

    NxActorDesc* actorDesc = &locActorDesc;//new NxActorDesc;
    actorDesc->body    = fDensity ? bodyDesc   : NULL;
    actorDesc->density = fDensity;

    nwSetNxMat34FromMat4( actorDesc->globalPose, mCapsuleTransform);

    //actorDesc->globalPose.t -= actorDesc->globalPose.M.getColumn(1) * (fHeight*0.5f);

    actorDesc->shapes.pushBack(capsuleDesc);

#ifdef CLIENT
    NwEmissary* pEmissary = new NwEmissary;
    assert(0); // not supported yet

    actorDesc->userData = pEmissary;
#endif

    if ( fDensity )
        nx_state.dynamicActorCount++;
    else
        nx_state.staticActorCount++;

    /*
    int iSuccess = qEnqueue( actorDescQueue, (void*) actorDesc );
    assert( iSuccess );
    */

    void* pActor = pScene->createActor(*actorDesc);    


    return pActor;
}


// -------------------------------------------------------------------------------------------------------------------
void* nwCreateMeshActor(Mat4 meshTransform, GroupDef *def, void* shape, NxGroupType group, int iSceneNum)
{
    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );

    assert( meshTransform && shape );

    NxActorDesc actorDesc;
    NxTriangleMeshShapeDesc* shapeDesc = reinterpret_cast<NxTriangleMeshShapeDesc*>(shape);
    assert(shapeDesc);
    shapeDesc->materialIndex = nxDefaultMaterial[iSceneNum]->getMaterialIndex();
    shapeDesc->group = group;
    shapeDesc->skinWidth = nxSceneConstants.shapeSkinWidth;
    actorDesc.shapes.pushBack(shapeDesc);
    
    nwSetNxMat34FromMat4( actorDesc.globalPose, meshTransform );
#ifdef CLIENT
    //NwEmissary* pEmissary = new NwEmissary;
    //actorDesc.userData = pEmissary;
#endif
    
    nx_state.staticActorCount++;

   
    actorDesc.group = group;
    NxActor* pActor = pScene->createActor(actorDesc);

#ifdef CLIENT
    if ( nx_state.hardware && pActor )
    {
        bool bUploaded = false;
        U32 uiPCount = shapeDesc->meshData->getPageCount();
        while ( uiPCount )
        {
            NxShape* const pShape = pActor->getShapes()[0];
            if ( pShape )
            {
                NxTriangleMeshShape* pTriShape = pShape->isTriangleMesh();
                if ( pTriShape )
                    bUploaded = pTriShape->mapPageInstance(--uiPCount);
            }
//             assert (bUploaded);
            if(!bUploaded)
                Errorf("couldn't upload shape %s\n", def ? def->name : "<no def>");
        }
    }
#endif
    return pActor;
}
// -------------------------------------------------------------------------------------------------------------------
void  nwDeleteActor(void* actorParam, int iSceneNum)
{
    if(!actorParam)
    {
        return;
    }

    NxScene* pScene = nxScene[iSceneNum];
    assert( pScene );


    NxActor* actor = (NxActor*)actorParam;


#ifdef CLIENT
    int iSuccess = qEnqueue( actorDeletionQueue, (void*) actor );
    assert( iSuccess );
#elif SERVER
    if ( actor->isDynamic())
        nx_state.dynamicActorCount--;
    else
        nx_state.staticActorCount--;
    pScene->releaseActor(*actor);
#endif
}
// -------------------------------------------------------------------------------------------------------------------
void* nwGetActorFromEmissaryGuid(U32 uiDataGuid)
{
    NwEmissaryData* pData = nwGetNwEmissaryDataFromGuid(uiDataGuid);
    if ( pData )
    {
        return pData->pActor;
    }
    return NULL;
}
// -------------------------------------------------------------------------------------------------------------------














// -------------------------------------------------------------------------------------------------------------------
// ACTOR READ FUNCTIONS (immediate)
// -------------------------------------------------------------------------------------------------------------------
void nwGetActorMat4( void* actor, float* mat4 )
{
    NxActor* pActor = (NxActor*)actor;

    NxMat34 nxMat = pActor->getGlobalPose();

    copyMat4((Vec3*)&nxMat, (Vec3*)mat4);
    transposeMat3( (Vec3*)mat4 );
    //assert(validateMat4( ((Vec3*)mat4) ) );
}
// -------------------------------------------------------------------------------------------------------------------
void  nwGetActorLinearVelocity(void* actor, float* velocity)
{
    /*
    NxActor* pActor = (NxActor*)actor;

    NxVec3 vVel = pActor->getLinearVelocity() * (1.0f / 30.0f);
    copyVec3( (float*)&vVel, velocity );
    */
    scaleVec3(((NxActor*)actor)->getLinearVelocity(), 0.0333333f, velocity ); 
}
// -------------------------------------------------------------------------------------------------------------------
void  nwGetActorAngularVelocity(void* actor, float* velocity)
{
    /*
    NxActor* pActor = (NxActor*)actor;

    NxVec3 vVel = pActor->getAngularVelocity() * (1.0f / 30.0f );
    copyVec3( (float*)&vVel, velocity );
    */
    scaleVec3(((NxActor*)actor)->getAngularVelocity(), 0.0333333f, velocity ); 
}
// -------------------------------------------------------------------------------------------------------------------
void  nwSetActorDamping( void* actor, F32 fAngular, F32 fLinear )
{
    NxActor* pActor = (NxActor*)actor;

    pActor->setAngularDamping(fAngular);
    pActor->setLinearDamping(fLinear);

}
// -------------------------------------------------------------------------------------------------------------------
void  nwSetActorSleepVelocities(void* actor, F32 fAngular, F32 fLinear)
{
    NxActor* pActor = (NxActor*)actor;

    pActor->setSleepAngularVelocity(fAngular);
    pActor->setSleepLinearVelocity(fLinear);

}
// -------------------------------------------------------------------------------------------------------------------
void  nwSetActorMass(void* actor, F32 fMass )
{
    NxActor* pActor = (NxActor*)actor;

    pActor->setMass(fMass);
}
// -------------------------------------------------------------------------------------------------------------------
static void nwFreeSimpleTriMesh(NxSimpleTriangleMesh& triMesh)
{
    if(triMesh.points!=NULL)
        delete[] triMesh.points;

    if(triMesh.triangles!=NULL)
        delete[] triMesh.triangles;
}
// -------------------------------------------------------------------------------------------------------------------
static void nwCreateCCDMeshFromShape( NxSimpleTriangleMesh& triMesh, NxShape* pShape )
{
    NxBoxShape* pBoxShape = pShape->isBox();
    NxCapsuleShape* pCapsuleShape = pShape->isCapsule();
    NxF32 fSkin;

    int indexSize=sizeof(NxU32);
    NxVec3 vCCDBoxDimensions;

    if ( pBoxShape )
    {
        vCCDBoxDimensions = pBoxShape->getDimensions();
    }
    else if ( pCapsuleShape )
    {
        vCCDBoxDimensions.y = pCapsuleShape->getHeight() * 0.5f;
        vCCDBoxDimensions.x = vCCDBoxDimensions.z = pCapsuleShape->getRadius() * 0.7071f; // (root 2) / 2

    }
    else
    {
        assert(0); // Shape not CCD supported yet.
    }

    fSkin = MIN(vCCDBoxDimensions.z,  MIN(vCCDBoxDimensions.x, vCCDBoxDimensions.y) ) * 0.5f;


    NxBox obb=NxBox(NxVec3(0.0f,0.0f,0.0f),vCCDBoxDimensions,NxMat33(NX_IDENTITY_MATRIX));

    triMesh.points=new NxVec3[8];
    triMesh.numVertices=8;
    triMesh.pointStrideBytes=sizeof(NxVec3);

    triMesh.numTriangles=2*6;
    triMesh.triangles=new NxU8[indexSize*2*6*3];
    triMesh.triangleStrideBytes=indexSize*3;

    triMesh.flags=0;

    NxComputeBoxPoints(obb,(NxVec3 *)triMesh.points);
    memcpy((NxU32 *)triMesh.triangles,NxGetBoxTriangles(),sizeof(NxU32)*2*6*3);


    assert(triMesh.isValid());

    /*
    Now move the points inwards by ccdSkin
    */

    NxVec3 *normals=new NxVec3[triMesh.numVertices];

    if(indexSize==sizeof(NxU32))
        NxBuildSmoothNormals(triMesh.numVertices,triMesh.numVertices,(NxVec3 *)triMesh.points,(NxU32 *)triMesh.triangles,NULL,normals,false);
    else if(indexSize==sizeof(NxU16))
        NxBuildSmoothNormals(triMesh.numVertices,triMesh.numVertices,(NxVec3 *)triMesh.points,NULL,(NxU16 *)triMesh.triangles,normals,false);
    else
        NX_ASSERT(!"Invalid index size");

    NxVec3 *pointPtr=(NxVec3 *)triMesh.points;
    for(NxU32 i=0;i<triMesh.numVertices;i++)
    {
        NxVec3 point=pointPtr[i];

        point+=normals[i]*fSkin;

        pointPtr[i]=point;
    }

    delete[] normals;

}
// -------------------------------------------------------------------------------------------------------------------
F32 nwCalcCCDMotionThreshold( const NxShape* pShape )
{
    const NxBoxShape* pBoxShape = pShape->isBox();
    const NxCapsuleShape* pCapsuleShape = pShape->isCapsule();

    F32 fMinDepth = -1.0f;

    if ( pBoxShape )
    {
        NxVec3 vBoxDimensions = pBoxShape->getDimensions();
        fMinDepth = MIN( vBoxDimensions.x, MIN(vBoxDimensions.y, vBoxDimensions.z));
    }
    else if ( pCapsuleShape )
    {
        fMinDepth = pCapsuleShape->getRadius();
    }
    else
    {
        assert(0); // shape not supported
    }

    assert( fMinDepth > 0.0f );

    // Now, calculate the min speed at which penetration as deep as the min half dimension
    // is possible
    // Speed = distance / time, the distance is the depth, the time is the timestep size
    // FIXME: do we care about the number of sub-iterations? probably should stay on the safe
    // side

    // Multiply by 1/2 to make it only a quarter penetration before ccd kicks in
    F32 fThreshold = fMinDepth * 0.9f;

    return fThreshold;
}
// -------------------------------------------------------------------------------------------------------------------
void  nwAddCCDSkeletonForActor(void* actor)
{
    NxF32 fCCDMotionThreshold = FLT_MAX;
    NxActor* pActor = (NxActor*)actor;

    int nbShapes = pActor->getNbShapes();
    NxShape* const* shapeArray = pActor->getShapes();

    for (int i=0; i< nbShapes; i++)
    {
        NxShape* pShape = shapeArray[i];

        NxSimpleTriangleMesh CCDMesh;
        nwCreateCCDMeshFromShape( CCDMesh, pShape );

        NxCCDSkeleton* pCCDSkel = nxSDK->createCCDSkeleton(CCDMesh);

        nwFreeSimpleTriMesh(CCDMesh);

        pShape->setCCDSkeleton(pCCDSkel);

        NxF32 fShapeCCDMotionThreshold = nwCalcCCDMotionThreshold(pShape);
        if ( fShapeCCDMotionThreshold < fCCDMotionThreshold )
            fCCDMotionThreshold = fShapeCCDMotionThreshold;
    }

    if ( nbShapes > 0 )
        pActor->setCCDMotionThreshold( fCCDMotionThreshold );
}
// -------------------------------------------------------------------------------------------------------------------
void  nwRemoveCCDSkeletonForActor(void* actor)
{
    NxActor* pActor = (NxActor*)actor;

    int nbShapes = pActor->getNbShapes();
    NxShape* const* shapeArray = pActor->getShapes();

    for (int i=0; i< nbShapes; i++)
    {
        NxShape* pShape = shapeArray[i];
        NxCCDSkeleton* pCCDSkel = pShape->getCCDSkeleton();
        pShape->setCCDSkeleton(0);
        if (pCCDSkel)
        {
            nxSDK->releaseCCDSkeleton(*pCCDSkel);
        }
    }
}
// -------------------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------------------
// ACTOR FUNCTION QUEUE
// -------------------------------------------------------------------------------------------------------------------
Queue actorFunctionQueueOneParam;
typedef void (*ActorFunctionOneParam)(NxActorGuid guidActor, U32 paramOne);

typedef struct AFQOneParam
{
    ActorFunctionOneParam    pFunc;
    NxActorGuid                guidActor;
    U32                        paramOne;
} AFQOneParam;


static void nwEnqueueActorFunctionOneParam( AFQOneParam* pAFQObject )
{
    if ( !actorFunctionQueueOneParam )
    {
        actorFunctionQueueOneParam = createQueue();
        qSetMaxSizeLimit(actorFunctionQueueOneParam, 8192);
        initQueue(actorFunctionQueueOneParam, 32);
    }
    qEnqueue(actorFunctionQueueOneParam, pAFQObject);

}

static void processActorFunctionOneParamQueue(bool bCallFunction)
{
    if ( !actorFunctionQueueOneParam )
        return;

    AFQOneParam* pAFQObject = (AFQOneParam*)qDequeue(actorFunctionQueueOneParam);
    while (pAFQObject)
    {
        if ( bCallFunction )
            pAFQObject->pFunc(pAFQObject->guidActor, pAFQObject->paramOne);
        free(pAFQObject);
        pAFQObject = (AFQOneParam*)qDequeue(actorFunctionQueueOneParam);
    }

}


static void nwDestroyActorFunctionQueues( )
{
    if ( actorFunctionQueueOneParam )
    {
        processActorFunctionOneParamQueue(false);
        destroyQueue(actorFunctionQueueOneParam);
    }
    actorFunctionQueueOneParam = NULL;
}








// -------------------------------------------------------------------------------------------------------------------
// ACTOR WRITE FUNCTIONS (delayed)
// -------------------------------------------------------------------------------------------------------------------
static void nwInternalSetActorMat4(NxActorGuid guidActor, U32 pMatPtrInU32Form )
{
    NxMat34* pMat = (NxMat34*)(uintptr_t)pMatPtrInU32Form;
    NxActor* pActor = getActorFromGuid(guidActor);
    if (pActor)
    {
        if(pActor->readBodyFlag(NX_BF_KINEMATIC))
            pActor->moveGlobalPose(*pMat);
        else
            pActor->setGlobalPose(*pMat);
    }
    // no longer allocated separately -- delete pMat;
    // delete pMat (this is done in other Cryptic games; do NOT do this - MDC, 2019)
}

void  nwSetActorMat4(NxActorGuid guidActor , float* mat4)
{
    if ( nx_state.threaded )
    {
        AFQOneParam* pAFQObject = (AFQOneParam*)malloc(sizeof(AFQOneParam) + sizeof(NxMat34));
        NxMat34* pNxMat = (NxMat34*)(pAFQObject + 1); // single allocation
        nwSetNxMat34FromMat4(*pNxMat, (Vec3*)mat4);
        pAFQObject->guidActor = guidActor;
        pAFQObject->pFunc = nwInternalSetActorMat4;

        pAFQObject->paramOne = (U32)(uintptr_t)pNxMat;

        nwEnqueueActorFunctionOneParam(pAFQObject);
    }
    else
    {
        NxMat34 tempMat;
        nwSetNxMat34FromMat4(tempMat, (Vec3*)mat4);
        nwInternalSetActorMat4(guidActor, (U32)(uintptr_t)(&tempMat));
    }

}

// -------------------------------------------------------------------------------------------------------------------
void  nwSetActorLinearVelocity(void* actor, const float* velocity)
{
    NxActor* pActor = (NxActor*)actor;

    NxVec3 vVelocity;
    copyVec3( velocity, vVelocity );
    vVelocity *= 30.0f;

    pActor->setLinearVelocity( vVelocity );
}
// -------------------------------------------------------------------------------------------------------------------
void  nwSetActorAngularVelocity(void* actor, const float* velocity)
{
    NxActor* pActor = (NxActor*)actor;

    NxVec3 vVelocity;
    copyVec3( velocity, vVelocity );
    vVelocity *= 30.0f;


    pActor->setAngularVelocity( vVelocity );
}
// -------------------------------------------------------------------------------------------------------------------
F32   nwGetActorMass(void* actor)
{
    NxActor* pActor = (NxActor*)actor;

    return pActor->getMass();

}
// -------------------------------------------------------------------------------------------------------------------
U8 nwIsSleeping(void* actor)
{
    NxActor* pActor = (NxActor*)actor;
    if (pActor->isSleeping() )
        return true;
    return false;
}
// ------------------------------------------------------------------------------------------------------------------
void  nwActorApplyAcceleration(void* actor, const float* acceleration, bool bPermanent)
{
    NxActor* pActor = (NxActor*)actor;

    NxVec3 vAccel;
    copyVec3( acceleration, vAccel);


#ifdef CLIENT 
    NwEmissary* pEmissary = (NwEmissary*)pActor->userData;
    if ( pEmissary )
    {
        if ( bPermanent )
            pEmissary->addRecuringAccel( vAccel );
        else
            pEmissary->addAccelOnce( vAccel );
    }
#else
    assert( !bPermanent );
    pActor->addForce(vAccel, NX_ACCELERATION);
#endif
}
// -------------------------------------------------------------------------------------------------------------------
#ifdef CLIENT
void  nwAddJoint(NwEmissaryData* nwEmissaryData, eNxJointDOF jointDOF, const Vec3 vLocalAnchor, const Mat4 mLocalMat, F32 fLinLimit, F32 fAngLimit, F32 fMaxForce, F32 fMaxTorque, F32 fLinSpring, F32 fLinSpringDamp, F32 fAngSpring, F32 fAngSpringDamp)
{
    NxD6JointDesc* pNewJointDesc = new NxD6JointDesc;
    NwEmissary* pEmissary = (NwEmissary*)nwEmissaryData->pEmissary;

    if (!( jointDOF & eNxJointDOF_RotateX ))
        pNewJointDesc->twistMotion = NX_D6JOINT_MOTION_LOCKED;
    else
    {
        if ( fAngLimit )
        {
            pNewJointDesc->twistMotion = NX_D6JOINT_MOTION_LIMITED;
            pNewJointDesc->twistLimit.high.value = RAD(fAngLimit);
            pNewJointDesc->twistLimit.low.value = -RAD(fAngLimit);
        }
        else 
            pNewJointDesc->twistMotion = NX_D6JOINT_MOTION_FREE;

        if ( fAngSpring )
        {
            pNewJointDesc->twistDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
            pNewJointDesc->twistDrive.spring = fAngSpring;
            pNewJointDesc->twistDrive.damping = fAngSpringDamp;
        }

    }

    if (!( jointDOF & eNxJointDOF_RotateY ))
        pNewJointDesc->swing1Motion = NX_D6JOINT_MOTION_LOCKED;
    else
    {
        if ( fAngLimit )
        {
            pNewJointDesc->swing1Motion = NX_D6JOINT_MOTION_LIMITED;
            pNewJointDesc->swing1Limit.value = RAD(fAngLimit);
        }
        else 
            pNewJointDesc->swing1Motion = NX_D6JOINT_MOTION_FREE;

        if ( fAngSpring )
        {
            pNewJointDesc->swingDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
            pNewJointDesc->swingDrive.spring = fAngSpring;
            pNewJointDesc->swingDrive.damping = fAngSpringDamp;
        }

    }


    if (!( jointDOF & eNxJointDOF_RotateZ ))
        pNewJointDesc->swing2Motion = NX_D6JOINT_MOTION_LOCKED;
    else
    {
        if ( fAngLimit )
        {
            pNewJointDesc->swing2Motion = NX_D6JOINT_MOTION_LIMITED;
            pNewJointDesc->swing2Limit.value = RAD(fAngLimit);
        }
        else 
            pNewJointDesc->swing2Motion = NX_D6JOINT_MOTION_FREE;

        if ( fAngSpring )
        {
            pNewJointDesc->swingDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
            pNewJointDesc->swingDrive.spring = fAngSpring;
            pNewJointDesc->swingDrive.damping = fAngSpringDamp;
        }

    }





    if (!( jointDOF & eNxJointDOF_TranslateX ))
        pNewJointDesc->xMotion = NX_D6JOINT_MOTION_LOCKED;
    else
    {
        if ( fLinLimit )
            pNewJointDesc->xMotion = NX_D6JOINT_MOTION_LIMITED;
        else
            pNewJointDesc->xMotion = NX_D6JOINT_MOTION_FREE;

        if ( fLinSpring )
        {
            pNewJointDesc->xDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
            pNewJointDesc->xDrive.spring = fLinSpring;
            pNewJointDesc->xDrive.damping = fLinSpringDamp;
        }
    }

    if (!( jointDOF & eNxJointDOF_TranslateY ))
        pNewJointDesc->yMotion = NX_D6JOINT_MOTION_LOCKED;
    else
    {
        if ( fLinLimit )
            pNewJointDesc->yMotion = NX_D6JOINT_MOTION_LIMITED;
        else
            pNewJointDesc->yMotion = NX_D6JOINT_MOTION_FREE;

        if ( fLinSpring )
        {
            pNewJointDesc->yDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
            pNewJointDesc->yDrive.spring = fLinSpring;
            pNewJointDesc->yDrive.damping = fLinSpringDamp;
        }
    }

    if (!( jointDOF & eNxJointDOF_TranslateZ ))
        pNewJointDesc->zMotion = NX_D6JOINT_MOTION_LOCKED;
    else
    {
        if ( fLinLimit )
            pNewJointDesc->zMotion = NX_D6JOINT_MOTION_LIMITED;
        else
            pNewJointDesc->zMotion = NX_D6JOINT_MOTION_FREE;

        if ( fLinSpring )
        {
            pNewJointDesc->zDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
            pNewJointDesc->zDrive.spring = fLinSpring;
            pNewJointDesc->zDrive.damping = fLinSpringDamp;
        }
    }


    if ( fLinLimit != 0.0f )
        pNewJointDesc->linearLimit.value = fLinLimit;

    Vec3 vGlobalAnchorTemp;
    mulVecMat4(vLocalAnchor, mLocalMat, vGlobalAnchorTemp);
    copyVec3(vGlobalAnchorTemp, pNewJointDesc->localAnchor[1]);
    copyVec3(vLocalAnchor, pNewJointDesc->localAnchor[0]);

    zeroVec3(pNewJointDesc->localAxis[0]);
    pNewJointDesc->localAxis[0][0] = 1.0f;
    copyVec3(mLocalMat[0], pNewJointDesc->localAxis[1]);

    zeroVec3(pNewJointDesc->localNormal[0]);
    pNewJointDesc->localNormal[0][1] = 1.0f;
    copyVec3(mLocalMat[1], pNewJointDesc->localNormal[1]);


    // handle breakable joints
    if ( fMaxForce )
        pNewJointDesc->maxForce = fMaxForce;
    if ( fMaxTorque )
        pNewJointDesc->maxTorque = fMaxTorque;


    if ( !nx_state.hardware )
        pNewJointDesc->projectionMode = NX_JPM_POINT_MINDIST;

    /* Trying this out */
    pNewJointDesc->swingDrive.driveType = NX_D6JOINT_DRIVE_POSITION;
    pNewJointDesc->swingDrive.spring = 250.0f;
    pNewJointDesc->swingDrive.damping = 100.0f;
    NxQuat q;
    q.id();
    pNewJointDesc->driveOrientation = q;
    /* Seems to work well; puts some limits on joint accel */



    pEmissary->jointDesc = pNewJointDesc;
}
#endif
// -------------------------------------------------------------------------------------------------------------------









// -------------------------------------------------------------------------------------------------------------------
// PLAYER INTERACTIO
// -------------------------------------------------------------------------------------------------------------------
/*
void  nwUpdatePlayerPosition(Entity* pPlayerEnt)
{

}
*/

#ifdef CLIENT

// Assumes that vEpicenter is outside of bounds
static NxF32 checkBounds1D( const NxF32& fToCheck, const NxF32& fMax, const NxF32& fMin)
{
    if ( fToCheck > fMax )
        return fMax;
    if ( fToCheck < fMin )
        return fMin;
    return fToCheck;
}
static NxVec3 getClosestPointOnBounds(const NxVec3 vEpicenter, const NxBounds3& bounds)
{
    NxVec3 vResult;
    vResult.x = checkBounds1D(vEpicenter.x, bounds.min.x, bounds.max.x);
    vResult.y = checkBounds1D(vEpicenter.y, bounds.min.y, bounds.max.y);
    vResult.z = checkBounds1D(vEpicenter.z, bounds.min.z, bounds.max.z);
    return vResult;
}


/*
void  nwApplySphericalExplosion(Vec3 vEpicenter, F32 fRadius, F32 fPower, void* pNxEmissaryData, int iSceneNum)
{
    copyVec3( vEpicenter, forceToApply.vEpicenter );
    forceToApply.fRadius = fRadius;
    forceToApply.fPower = fPower;
    forceToApply.pExlcudedEmissaryData = pNxEmissaryData;
    forceToApply.iSceneNum = iSceneNum;

    forceToApply.bReadyToGo = true;
}
*/

void  nwApplyForceToEmissary(NwEmissaryData* pEmissaryData, Vec3 vEpicenter, F32 fRadius, F32 fPower, F32 fPowerJitter, F32 fCentripetal, eNxForceType eForce, Mat3Ptr pmXform, int iSceneNum)
{
    NxActor* pActor = (NxActor*)pEmissaryData->pActor;
    if ( !pActor )
        return;
    NwEmissary* pEmissary = (NwEmissary*)pActor->userData;
    if ( !pEmissary )
        return;

    NxVec3 vNormDir;
    F32 fForce;
    F32 fActualPower = fPower + fPowerJitter * qfrand();

    NxBounds3 shapeBounds;
    NxShape* pShape = pActor->getShapes()[0];

    pShape->getWorldBounds(shapeBounds);

    NxVec3 vCenter(vEpicenter[0], vEpicenter[1], vEpicenter[2]);

    // If we're inside, force is 1.0, and direction is to the center
    switch (eForce)
    {
    case eNxForceType_Out:
    case eNxForceType_In:
        {
            if ( shapeBounds.contain(vCenter) )
            {
                fForce = 1.0f;
                if ( vCenter == pShape->getGlobalPosition() )
                {
                    vNormDir = NxVec3(0.0f, 1.0f, 0.0f);
                }
                else
                {
                    vNormDir = pShape->getGlobalPosition() - vCenter;
                    vNormDir.normalize();
                    vNormDir.y += 1.0f;
                }
            }
            else
            {
                // Calculate closest point to edge, use that for distance and force multiplier
                NxVec3 vTargetPoint = getClosestPointOnBounds(vCenter, shapeBounds);
                vNormDir = vTargetPoint - vCenter;
                if ( eForce == eNxForceType_In )
                {
                    fForce = -vNormDir.normalize() / fRadius;
                    if ( fForce < -1.01f )
                        fForce = 0.0f; // oops, we're outside the bounds
                }
                else // out
                {
                    fForce = 1.0f - vNormDir.normalize() / fRadius;
                    if ( fForce < 0.0f )
                        fForce = 0.0f; // oops, we're outside the bounds
                }
            }

            // Now we have our direction and force multiplier: apply the force
            if ( fForce )
            {
                NxVec3 vAngularAxis(qfrand(), qfrand(), qfrand());
                pEmissary->addAccelOnce(vNormDir * ( fForce * fActualPower ));
                pEmissary->addAngularAccelOnce(vAngularAxis * fForce * fActualPower);
            }
        }
        break;
    case eNxForceType_Up:
    case eNxForceType_Forward:
    case eNxForceType_Side:
        {
            if ( !pmXform )
            {
                Errorf("Failed to pass a mat3 xform pointer!\n");
                return;
            }
            NxVec3 vUsedAxis;
            switch (eForce)
            {
            default:
            case eNxForceType_Up:
                copyVec3(pmXform[1], vUsedAxis);
                break;
            case eNxForceType_Forward:
                copyVec3(pmXform[2], vUsedAxis);
                break;
            case eNxForceType_Side:
                copyVec3(pmXform[0], vUsedAxis);
                break;
            };

            if ( shapeBounds.contain(vCenter) )
            {
                fForce = 1.0f;
            }
            else
            {
                // Calculate closest point to edge, use that for distance and force multiplier
                NxVec3 vTargetPoint = getClosestPointOnBounds(vCenter, shapeBounds);
                vNormDir = vTargetPoint - vCenter;
                fForce = 1.0f - vNormDir.normalize() / fRadius;
                if ( fForce <= 0.0f )
                    fForce = 0.0f; // oops, we're outside the bounds
            }

            // Now we have our direction and force multiplier: apply the force
            if ( fForce )
            {
                pEmissary->addAccelOnce(vUsedAxis * ( fForce * fActualPower ));
                NxVec3 vAngularAxis(qfrand(), qfrand(), qfrand());
                pEmissary->addAngularAccelOnce(vAngularAxis * fForce * fActualPower);
            }
        }
        break;
    case eNxForceType_CWSwirl:
    case eNxForceType_CCWSwirl:
        {
            if ( !pmXform )
            {
                Errorf("Failed to pass a mat3 xform pointer!\n");
                return;
            }

            NxVec3 vAngularAxis;

            if ( eForce == eNxForceType_CWSwirl )
                scaleVec3(pmXform[1], -1.0f, vAngularAxis);
            else
                copyVec3(pmXform[1], vAngularAxis);

            if ( shapeBounds.contain(vCenter) )
            {
                fForce = 1.0f;
                if ( vCenter == pShape->getGlobalPosition() )
                {
                    vNormDir = NxVec3(0.0f, 1.0f, 0.0f);
                }
                else
                {
                    vNormDir = pShape->getGlobalPosition() - vCenter;
                    vNormDir.normalize();
                }
                vAngularAxis *= -1.0f;
            }
            else
            {
                // Calculate closest point to edge, use that for distance and force multiplier
                NxVec3 vTargetPoint = getClosestPointOnBounds(vCenter, shapeBounds);
                vNormDir = vTargetPoint - vCenter;
                fForce = 1.0f - vNormDir.normalize() / fRadius;
                if ( fForce < 0.0f )
                    fForce = 0.0f; // oops, we're outside the bounds
            }

            // Now we have our direction and force multiplier: apply the force
            if ( fForce )
            {
                // Make sure we can calc cross product
                if ( fabsf(vAngularAxis.dot(vNormDir)) < 0.99f )
                {
                    // Calculate cross product direction
                    NxVec3 vForceDir = vAngularAxis.cross(vNormDir);
                    pEmissary->addAccelOnce(vForceDir * ( fForce * fActualPower ));
                    if ( fCentripetal )
                    {
                        vNormDir -= vAngularAxis * vAngularAxis.dot(vNormDir);
                        pEmissary->addAccelOnce(vNormDir * -fCentripetal * fActualPower );
                    }
                }
                pEmissary->addAngularAccelOnce(vAngularAxis * fForce * fActualPower);
            }
        }
        break;
    default:
        break;
    };
}

U32  nwGatherEmissariesInSphere(Vec3 vEpicenter, F32 fRadius, NwEmissaryData** pEmissaryResults, U32 uiMaxResults, int iSceneNum)
{
    NxVec3 vCenter(vEpicenter[0], vEpicenter[1], vEpicenter[2]);
    NxSphere sphere(vCenter, fRadius);

    NxShape** pShapes = (NxShape**)_alloca(sizeof(NxShape*) * uiMaxResults);

    U32 uiTotalShapesFound = nxScene[iSceneNum]->overlapSphereShapes(sphere, NX_ALL_SHAPES, uiMaxResults, pShapes, NULL );
    U32 uiResults = 0;

    for (U32 uiShapeIndex = 0; uiShapeIndex < uiTotalShapesFound; uiShapeIndex++)
    {
        NxActor* pActor = &pShapes[uiShapeIndex]->getActor();
        if ( !pActor )
            continue;
        NwEmissary* pEmissary = (NwEmissary*)pActor->userData;
        if ( !pEmissary )
            continue;
        pEmissaryResults[uiResults++] = pEmissary->GetInternalStructAddress();
    }
    return uiResults;
}

/*
void processForceQueue(int iSceneNum)
{
    if ( iSceneNum == forceToApply.iSceneNum && forceToApply.bReadyToGo )
    {
        nwActuallyApplySphericalExplosion(forceToApply.vEpicenter, forceToApply.fRadius, forceToApply.fPower, forceToApply.pExlcudedEmissaryData, iSceneNum);
        forceToApply.bReadyToGo = false;
    }
}
*/


#endif







// -------------------------------------------------------------------------------------------------------------------
// NOVODEX RAYCAST
// -------------------------------------------------------------------------------------------------------------------
class MyRayReporter : public NxUserRaycastReport
{
public:

    Vec3 outPos;
    int didHit;

    MyRayReporter() :
    didHit(0)
    {
    }

    /**
    This method is called for each shape hit by the raycast. If onHit returns true, it may be
    called again with the next shape that was stabbed. If it returns false, no further shapes
    are returned, and the raycast is concluded.
    */
    bool onHit(const NxRaycastHit& hit)
    {
        copyVec3(hit.worldImpact, outPos);
        didHit = 1;
        return true;
    }
};

// -------------------------------------------------------------------------------------------------------------------
/*
int nwRaycast(Vec3 p0, Vec3 p1, Vec3 out)
{
    NxRay ray;

    copyVec3(p0, ray.orig);
    subVec3(p1, p0, ray.dir);
    NxReal length = ray.dir.normalize();
#if 0
    MyRayReporter rayReporter;
    PERFINFO_AUTO_START("raycastAllShapes", 1);
    nxScene->raycastAllShapes(ray, rayReporter, NX_STATIC_SHAPES);
    if(rayReporter.didHit){
        copyVec3(rayReporter.outPos, out);
    }
    PERFINFO_AUTO_STOP();
    return rayReporter.didHit;
#else
    PERFINFO_AUTO_START("raycastAllShapes", 1);
    NxRaycastHit hit;
    NxShape* shape = nxScene->raycastClosestShape(ray, NX_STATIC_SHAPES, hit, ~0, length);
    if(shape)
    {
        copyVec3(hit.worldImpact, out);
        PERFINFO_AUTO_STOP();    
        return 1;
    }
    PERFINFO_AUTO_STOP();    
    return 0;
#endif
}
*/
// -------------------------------------------------------------------------------------------------------------------









// -------------------------------------------------------------------------------------------------------------------
// NOVODEX DEBUGGING
// -------------------------------------------------------------------------------------------------------------------
void nwConnectToPhysXDebug()
{
    nxSDK->getFoundationSDK().getRemoteDebugger()->connect("localhost");
}

#ifdef CLIENT
#include "render/renderprim.h"
static void  nwDebugRender(const NxDebugRenderable* pDebugData)
{
    {
        nxSDK->setParameter(NX_VISUALIZATION_SCALE, 1);

        nxSDK->setParameter(NX_VISUALIZE_COLLISION_AABBS, 0);
        nxSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
        nxSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 0);

        nxSDK->setParameter(NX_VISUALIZE_JOINT_WORLD_AXES, 0);
        nxSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 0);
    }
    // Render points
    {
        U32 uiNumPoints = pDebugData->getNbPoints();
        const NxDebugPoint* pPoint = pDebugData->getPoints();
        while (uiNumPoints--)
        {
            pPoint++;
        }
    }
    // Render lines
    {
        U32 uiNumLines = pDebugData->getNbLines();
        const NxDebugLine* pLine = pDebugData->getLines();
        while ( uiNumLines-- )
        {
            drawLine3D((float*)&pLine->p0, (float*)&pLine->p1, pLine->color | 0xFF000000);
            pLine++;
        }
    }
    // Render triangles
    {
        U32 uiNumTris = pDebugData->getNbTriangles();
        const NxDebugTriangle* pTri = pDebugData->getTriangles();
        while ( uiNumTris-- )
        {
            drawTriangle3D((float*)&pTri->p0, (float*)&pTri->p1, (float*)&pTri->p2, pTri->color | 0xFF000000);
            pTri++;
        }
    }
}
#endif
// -------------------------------------------------------------------------------------------------------------------
#ifdef CLIENT
void nwPrintInfoToConsole(int toConsole)
{
    char buffer[1000];
    char tempbuf[1000];

    if (!nxScene[NX_CLIENT_SCENE])
        return;
    sprintf(buffer,
            "\n\n"
            "+- NovodeX Info -----------------------------------------------------\n"
            "|\n"
            );
    if ( nx_state.hardware )
    {
        sprintf(tempbuf,
            "|  HARDWARE MODE\n"
            );
        strcat(buffer,tempbuf);
    }
    else
    {
        sprintf(tempbuf,
            "|  SOFTWARE MODE\n"
            );
        strcat(buffer,tempbuf);
    }
    sprintf(tempbuf,
            "|  Mesh Shapes: %d\n"
            "|  Static Actors:      %d\n"
            "|  Actor Spores:      %d\n"
            "|  Dynamic Actors:      %d\n"
            "|  Materials:   %d\n"
            "|\n"
            "+--------------------------------------------------------------------\n\n",
            nx_state.meshShapeCount,
            nx_state.staticActorCount,
            nx_state.actorSporeCount,
            nx_state.dynamicActorCount,
            nxScene[NX_CLIENT_SCENE]->getNbMaterials()
            );
    strcat(buffer,tempbuf);

    if(toConsole){
        conPrintf("%s", buffer);
    }else{
        printf("%s", buffer);
    }
}
#endif
// -------------------------------------------------------------------------------------------------------------------











// -------------------------------------------------------------------------------------------------------------------
// NOVODEX FLUIDS
// -------------------------------------------------------------------------------------------------------------------
#ifdef NOVODEX_FLUIDS
void* nwCreateFluidEmitter( FxFluidEmitter* fxEmitter, int iSceneNum )
{
    NxFluidDesc fluidDesc;

    fluidDesc.setToDefault();

    fluidDesc.simulationMethod = NX_F_SPH;
    fluidDesc.staticCollisionRestitution = fxEmitter->fluid->restitution;
    fluidDesc.staticCollisionAdhesion = fxEmitter->fluid->adhesion;
    fluidDesc.maxParticles = fxEmitter->fluid->maxParticles;
    fluidDesc.restParticlesPerMeter = fxEmitter->fluid->restParticlesPerMeter;
    fluidDesc.restDensity = fxEmitter->fluid->restDensity;
    fluidDesc.stiffness = fxEmitter->fluid->stiffness;
    fluidDesc.viscosity = fxEmitter->fluid->viscosity;
    fluidDesc.damping = fxEmitter->fluid->damping;

    fluidDesc.particlesWriteData.bufferPos = (NxF32*)fxEmitter->particlePositions;
    fluidDesc.particlesWriteData.bufferPosByteStride = sizeof(fxEmitter->particlePositions[0]);
    fluidDesc.particlesWriteData.bufferVel = (NxF32*)fxEmitter->particleVelocities;
    fluidDesc.particlesWriteData.bufferVelByteStride = sizeof(fxEmitter->particleVelocities[0]);
    fluidDesc.particlesWriteData.bufferLife = fxEmitter->particleAges;
    fluidDesc.particlesWriteData.bufferLifeByteStride = sizeof(fxEmitter->particleAges[0]);
    fluidDesc.particlesWriteData.bufferDensity = fxEmitter->particleDensities;
    fluidDesc.particlesWriteData.bufferDensityByteStride = sizeof(fxEmitter->particleDensities[0]);

    fluidDesc.particlesWriteData.maxParticles = fxEmitter->fluid->maxParticles;
    fluidDesc.particlesWriteData.numParticlesPtr = &fxEmitter->currentNumParticles;
    //nxScene->getGravity(fluidDesc.externalForce);
    //fluidDesc.externalForce *= -1.0f;

    assert( fluidDesc.isValid() );


    NxFluid* newFluid = nxScene[iSceneNum]->createFluid(fluidDesc);
    assert(newFluid);

    NxFluidEmitterDesc emitterDesc;
    emitterDesc.setToDefault();
    emitterDesc.dimensionX = fxEmitter->fluid->dimensionX;
    emitterDesc.dimensionY = fxEmitter->fluid->dimensionY;

    nwSetNxMat34FromMat4( emitterDesc.relPose, fxEmitter->geoOwner->gfx_node->mat);

    emitterDesc.rate = fxEmitter->fluid->rate;
    emitterDesc.randomAngle = fxEmitter->fluid->randomAngle;
    emitterDesc.fluidVelocityMagnitude = fxEmitter->fluid->fluidVelocityMagnitude;
    emitterDesc.maxParticles = fxEmitter->fluid->maxParticles;
    emitterDesc.particleLifetime = fxEmitter->fluid->particleLifetime;
    emitterDesc.type = NX_FE_CONSTANT_FLOW_RATE;
    emitterDesc.shape = NX_FE_ELLIPSE;
    NxFluidEmitter* newFluidEmitter = newFluid->createEmitter(emitterDesc);
    assert(newFluidEmitter);

    return newFluid;
}
// -------------------------------------------------------------------------------------------------------------------
void  nwUpdateFluidEmitterTransform( void* fluid, Mat4 fluidTransform )
{
    NxFluid* nFluid = (NxFluid*)fluid;

    NxMat34 nTransform;
    nwSetNxMat34FromMat4( nTransform, fluidTransform );
    for ( NxU32 uiFluid=0; uiFluid < nFluid->getNbEmitters(); ++uiFluid )
    {
        nFluid->getEmitters()[uiFluid]->setGlobalPose( nTransform);
    }
}
// -------------------------------------------------------------------------------------------------------------------
void nwDeleteFluid( void* fluid, int iSceneNum )
{
    NxFluid* nFluid = (NxFluid*)fluid;
    if ( nFluid && nxScene[iSceneNum] )
    {
        /*
        NxU32 uiEmitter;
        for (uiEmitter=0; uiEmitter<nFluid->getNbEmitters(); ++uiEmitter)
        {
        nFluid->releaseEmitter(*(nFluid->getEmitters()[uiEmitter]));
        }
        */
        nxScene[iSceneNum]->releaseFluid(*nFluid);
    }
}
// -------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------
















// -------------------------------------------------------------------------------------------------------------------
#endif
