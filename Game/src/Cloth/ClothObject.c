#include "Cloth/Cloth.h"
#include "Cloth/ClothPrivate.h"
#include "Cloth/ClothMesh.h"

//============================================================================
// ClothLOD and ClothObject Organization
//
// Cloth is divided into multiple "detail" levels using a two-tiered system:
// I.  LODs each contains a Cloth structure with a set number of particles
// II. SubLODs determine which mesh to use to render the particles
//     and how much processing to perform.
//
// A ClothObject is a grouping of one or more ClothLODs.
// Each ClothLOD contains a Cloth structure and the ClothMeshes used to render it
//
// Each ClothLOD has a MinSubLOD and a MaxSubLOD associated with it.
//   NOTE: MinSubLOD is stored in the Cloth structure because it is required
//   by the Cloth code to determine the maximum number of Particles to create.
//   MaxSubLOD is only used by the ClothLOD code and is stored there.
//
// ClothObjectCreateLODs() will take take a ClothObject with a single LOD
//   and create additional LODs based on the first LOD.
//
// ClothObjectDetailNum() will calculate how many "detail" levels exist for
//   a ClothObject by summing the number of SubLODs for each ClothLOD.
// ClothObjectSetDetail() will set the active "detail" level by selecting
//   a ClothLOD and setting its SubLOD.
//
// The idea is to enable the game to set a single "detail" level based
//   on the distance from the camera and possibly other factors.
//
// For example, a typical 9x9 Cloth with 3 LODs might have the following
// "detail" levels:
//
// "Detail"    LOD        SubLOD    Mesh
//         0     0         0        17 x 17 (9+8 x 9+8) with 33 points along each edge
//        1     0         1        17 x 17 (9+8 x 9+8)
//        2     1         1         9 x 9  (5+4 x 5+4)
//        3     1         2         5 x 5
//        4     1         3         5 x 5
//        5     2         2         3 x 3
//        6     2         3         3 x 3
//
// Note: LOD 0 is created with MaxSubLOD = 1 since faster/better results
//  are obtained by switching to a lower LOD before removing intermediate
//  points entirely. Likewise LOD 1 has MinSubLOD = 2 and LOD 2 has
//  MinSubLOD = 3 since more detail at those LODs would not be noticed.

//============================================================================
// CLOTH LOD
//============================================================================

static ClothLOD *ClothLODCreate(int minsublod, int maxsublod)
{
    int j;
    ClothLOD *lod = CLOTH_MALLOC(ClothLOD, 1);
    lod->mCloth = ClothCreateEmpty(minsublod);
    for (j=0; j<CLOTH_SUB_LOD_NUM; j++)
        lod->Meshes[j] = 0;
    if (maxsublod < 0)
        maxsublod = CLOTH_SUB_LOD_NUM-1;
    lod->MaxSubLOD = MINMAX(maxsublod, minsublod, CLOTH_SUB_LOD_NUM-1);
    return lod;
}

static void ClothLODDelete(ClothLOD *lod)
{
    int j;
    for (j=0; j<CLOTH_SUB_LOD_NUM; j++)
    {
        if (lod->Meshes[j])
            ClothMeshDelete(lod->Meshes[j]);
    }
    if (lod->mCloth)
        ClothDelete(lod->mCloth);
    CLOTH_FREE(lod);
}

// Create the ClothMesh data for each SubLOD
static int ClothLODCreateMeshes(ClothLOD *lod)
{
    int i,res=0;
    int min = lod->mCloth->MinSubLOD;
    int max = lod->MaxSubLOD;
    // Special Case: SubLOD 3 uses the same mesh as SubLOD 2
    if (max == 3 && min != 3)
        max = 2;
    for(i=min; i<=max; i++)
    {
        lod->Meshes[i] = ClothCreateMeshIndices(lod->mCloth, i);
    }
    return res;
}

ClothMesh *ClothLODGetMesh(ClothLOD *lod, int sublod)
{
    int min = lod->mCloth->MinSubLOD;
    int max = lod->MaxSubLOD;
    // Special Case: SubLOD 3 uses the same mesh as SubLOD 2
    if (max == 3 && min != 3)
        max = 2;
    sublod = MINMAX(sublod, min, max);
    return lod->Meshes[sublod];
};


//============================================================================
// CLOTH OBJECT
//============================================================================

ClothObject *ClothObjectCreate()
{
    int i;
    ClothObject *obj = CLOTH_CALLOC(ClothObject, 1);
    obj->NumLODs = 0;
    obj->LODs = 0;
    obj->CurLOD = 0;
    obj->CurSubLOD = 0;
    obj->GameData = 0;
    return obj;
}

void ClothObjectDelete(ClothObject *obj)
{
    int i;
    for (i=0; i<obj->NumLODs; i++)
        ClothLODDelete(obj->LODs[i]);
    CLOTH_FREE(obj->LODs);
    CLOTH_FREE(obj);    
}

//////////////////////////////////////////////////////////////////////////////

int ClothObjectAddLOD(ClothObject *obj, int minsublod, int maxsublod)
{
    obj->NumLODs++;
    obj->LODs = CLOTH_REALLOC(obj->LODs, ClothLOD *, obj->NumLODs);
    obj->LODs[obj->NumLODs-1] = ClothLODCreate(minsublod, maxsublod);
    return obj->NumLODs-1;
}

// Given LOD 0 as input, and maxnum = 2, the following LOD/SubLOD's will be generated:
// LOD  Dimentions  MinSubLOD MaxSubLod
//  0    9x9         0         2
//  1    5x5         1         3
//  2    3x3         2         3
// (SubLODs: 0=edge curves, 1=midpoints, 2=no interp pts, 3=fast math)
// NOTE: point_y_scale scales X values based on Height (which could be Y or Z).
// The scaling will be cumulative (i.e. relative to the previous LOD),
// So if maxnum = 2, and point_y_scale = .8, the resulting X values will be
// scaled by .8 and .6 with respect to LOD 0.
int ClothObjectCreateLODs(ClothObject *obj, int maxnum, F32 point_y_scale, F32 stiffness)
{
    int j,res;
    int lodnum = 0;
    Cloth *topcloth = obj->LODs[0]->mCloth;
    int minsublod = topcloth->MinSubLOD;

    if (!topcloth)
        return 0;
    while (maxnum > 0)
    {
        ClothLOD *newlod;
        Cloth *newcloth;
        if (minsublod < 2)
            minsublod++;
        newcloth = ClothCreateEmpty(minsublod);
        res = ClothBuildLOD(newcloth, topcloth, 1+lodnum, point_y_scale, stiffness);
        if (res < 0)
            break;
        newlod = CLOTH_MALLOC(ClothLOD, 1);
        newlod->mCloth = newcloth;
        for (j=0; j<CLOTH_SUB_LOD_NUM; j++)
            newlod->Meshes[j] = 0;
        newlod->MaxSubLOD = CLOTH_SUB_LOD_NUM-1;
        obj->NumLODs++;
        obj->LODs = CLOTH_REALLOC(obj->LODs, ClothLOD *, obj->NumLODs);
        obj->LODs[obj->NumLODs-1] = newlod;        
        maxnum--;
        lodnum++;
    }
    return lodnum;
}

Cloth *ClothObjGetLODCloth(ClothObject *obj, int lod)
{
    if (lod == -1)
        lod = obj->CurLOD;
    return obj->LODs[lod]->mCloth;
}

ClothMesh *ClothObjGetLODMesh(ClothObject *obj, int lod, int sublod)
{
    if (lod == -1)
        lod = obj->CurLOD;
    if (sublod == -1)
        sublod = obj->CurSubLOD;
    return ClothLODGetMesh(obj->LODs[lod], sublod);
}

// Set the actual LOD, calling ClothCopyCloth if necessary
// And setting the LOD Cloth's SubLOD to obj->CurSubLOD
int ClothObjectSetLOD(ClothObject *obj, int lod)
{
    int oldlod = obj->CurLOD;
    obj->CurLOD = MINMAX(lod, 0, obj->NumLODs-1);
    if (obj->CurLOD != oldlod)
    {
        Cloth *oldcloth = obj->LODs[oldlod]->mCloth;
        ClothCopyCloth(obj->LODs[obj->CurLOD]->mCloth,oldcloth);
        ClothObjectSetSubLOD(obj, obj->CurSubLOD);
    }
    return obj->CurLOD;
}

// Set obj->CurSubLOD and set the current LOD Cloth's SubLOD (fast)
int ClothObjectSetSubLOD(ClothObject *obj, int sub)
{
    obj->CurSubLOD = ClothSetSubLOD(obj->LODs[obj->CurLOD]->mCloth,sub);
    return obj->CurSubLOD;
}

// Get the number of "detail" levels for the ClothObject LOD's.
// This is the sum of the number of SubLODs for each LOD Cloth.
int ClothObjectDetailNum(ClothObject *obj)
{
    int i;
    int num = 0;
    for (i=0; i<obj->NumLODs; i++)
    {
        num += obj->LODs[i]->MaxSubLOD - obj->LODs[i]->mCloth->MinSubLOD + 1;
    }
    return num;
}

// Given detail, choose the correct LOD Cloth and
//   set its SubLOD to the appropriate lecel
int ClothObjectSetDetail(ClothObject *obj, int detail)
{
    int i,j;
    int level = 0;
    int lod=-1,sublod;
    for (i=0; i<obj->NumLODs; i++)
    {
        int numsub = obj->LODs[i]->MaxSubLOD - obj->LODs[i]->mCloth->MinSubLOD + 1;
        int sub = detail - level;
        if (sub < numsub)
        {
            lod = i;
            sublod = obj->LODs[i]->mCloth->MinSubLOD + sub;
            break;
        }
        level += numsub;
    }
    if (lod < 0) // Use Max LOD
    {
        lod = obj->NumLODs-1;
        sublod = obj->LODs[lod]->MaxSubLOD;
    }
    ClothObjectSetLOD(obj, lod);
    ClothObjectSetSubLOD(obj, sublod);
    return lod;
}

//////////////////////////////////////////////////////////////////////////////
// Create Meshes for each LOD Cloth.

int ClothObjectCreateMeshes(ClothObject *obj)
{
    int i,res=0;
    for (i=0; i<obj->NumLODs; i++)
    {
        ClothLOD *lod = obj->LODs[i];
        res = ClothLODCreateMeshes(lod);
        if (res < 0)
            return res;
    }
    return res;
}

//////////////////////////////////////////////////////////////////////////////
// Set values for all LOD Cloths

void ClothObjectSetColRad(ClothObject *obj, F32 colrad)
{
    int i;
    for (i=0; i<obj->NumLODs; i++)
        ClothSetColRad(obj->LODs[i]->mCloth, colrad);
}
void ClothObjectSetGravity(ClothObject *obj, F32 g)
{
    int i;
    for (i=0; i<obj->NumLODs; i++)
        ClothSetGravity(obj->LODs[i]->mCloth, g);
}

void ClothObjectSetDrag(ClothObject *obj, F32 drag)
{
    int i;
    for (i=0; i<obj->NumLODs; i++)
        ClothSetDrag(obj->LODs[i]->mCloth, drag);
}

void ClothObjectSetWind(ClothObject *obj, Vec3 dir, F32 speed, F32 maxspeed)
{
    int i;
    for (i=0; i<obj->NumLODs; i++)
        ClothSetWind(obj->LODs[i]->mCloth, dir, speed, maxspeed);
}

void ClothObjectSetWindRipple(ClothObject *obj, F32 amt, F32 repeat, F32 period)
{
    int i;
    for (i=0; i<obj->NumLODs; i++)
        ClothSetWindRipple(obj->LODs[i]->mCloth, amt, repeat, period);
}

//////////////////////////////////////////////////////////////////////////////
// Call Update functions on the current LOD Cloth

void ClothObjectUpdatePosition(ClothObject *clothobj, F32 dt, Mat4 newmat, Vec3 *hooks, int freeze)
{
    ClothUpdatePosition(clothobj->LODs[clothobj->CurLOD]->mCloth, dt, newmat, hooks, freeze);
}

void ClothObjectUpdatePhysics(ClothObject *clothobj, F32 dt)
{
    ClothUpdatePhysics(clothobj->LODs[clothobj->CurLOD]->mCloth, dt);
}

// Update the Mesh pointers for all meshes in the active LOD.
void ClothObjectUpdateDraw(ClothObject *clothobj)
{
    ClothLOD *lod = clothobj->LODs[clothobj->CurLOD];
    Cloth *cloth = lod->mCloth;
    ClothRenderData *renderData = &cloth->renderData;
    renderData->currentMesh = ClothObjGetLODMesh(clothobj, -1, -1);
    renderData->CurSubLOD = clothobj->CurSubLOD;

    ClothUpdateDraw(renderData);

    if (renderData->currentMesh) {
        int nump = ClothNumRenderedParticles(&renderData->commonData, renderData->CurSubLOD);
        ClothMeshSetPoints(renderData->currentMesh, nump, renderData->RenderPos, renderData->Normals, renderData->TexCoords, renderData->BiNormals, renderData->Tangents);
    }
}

void ClothObjectUpdate(ClothObject *clothobj, F32 dt)
{
    ClothObjectUpdatePhysics(clothobj, dt);
    ClothCopyToRenderData(clothobj->LODs[clothobj->CurLOD]->mCloth, NULL, CCTR_COPY_ALL);
    ClothObjectUpdateDraw(clothobj);
}

//////////////////////////////////////////////////////////////////////////////
