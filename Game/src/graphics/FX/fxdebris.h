#ifndef _FXDEBRIS_H
#define _FXDEBRIS_H

#include <utilitieslib/components/Queue.h>
#include <utilitieslib/components/MemoryPool.h>
#include "NovodeX/NwWrapper.h"

#if NOVODEX
typedef struct NwEmissaryData NwEmissaryData;
typedef struct GfxNode GfxNode;
typedef struct FxGeo FxGeo;
typedef struct GridIdxList GridIdxList;
typedef struct Grid Grid;
//typedef struct FxBhvr FxBhvr;

typedef struct FxDebris 
{
    GfxNode* pGfxNode;
    int iGfxNodeId;
    NwEmissaryDataGuid nxEmissary;
    Vec3 vScale;
    int iLastPos[3];
    GridIdxList* pGridIdxList;
    int iFxID;
    //const FxBhvr* bhvr;
} FxDebris;

typedef struct FxFadeOutDebris
{
    struct FxFadeOutDebris* pNext;
    GfxNode* pGfxNode;
    int iGfxNodeId;
} FxFadeOutDebris;

typedef struct FxDebrisManager
{
    Queue        debrisQueue;
    U32            uiMaxDebris;
    MemoryPool    mpDebris;
    MemoryPool  mpFadeOutDebris;
    Grid*        gridDebris;
    FxFadeOutDebris* pFadeOutList;
    U32            uiDebrisCount;
} FxDebrisManager;

void fxInitDebrisManager();
void fxDeinitDebrisManager();
bool fxAddDebris(FxGeo* pGeo, Vec3 vScale, int iFxID);
void fxUpdateDebrisManager();
void fxSetMaxDebrisCount();
void fxApplyForce(Vec3 vEpicenter, F32 fRadius, F32 fPower, F32 fPowerJitter, F32 fCentripetal, eNxForceType eForce, Mat3Ptr pmXform );
U32  fxGetDebrisObjectCount();
U32  fxGetDebrisMaxObjectCount();
#endif

#endif
