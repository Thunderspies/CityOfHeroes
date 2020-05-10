// Shared macros and prototypes for cloth library

#include <utilitieslib/assert/assert.h>
#pragma warning (disable:4101)

// Creation
extern void ClothCalcConstants(Cloth *cloth, int calcTexCoords);
extern void ClothSetNumIterations(Cloth *cloth, int sublod, int niterations);

// Utility
extern Cloth *ClothCopyCloth(Cloth *cloth, Cloth *pcloth);
extern Cloth *ClothBuildCopy(Cloth *cloth, Cloth *pcloth, F32 point_h_scale);

// Collisions
extern void ClothCollideCollidable(Cloth *cloth, ClothCol *clothcol);
    
// Queries
extern S32 ClothNumRenderedParticles(ClothCommonData *commonData, int sublod);

// SubLOD
extern void ClothSetMinSubLOD(Cloth *cloth, int minsublod);
extern S32  ClothSetSubLOD(Cloth *cloth, int sublod);
extern S32  ClothGetSubLOD(Cloth *cloth);
    
// Physics
extern void ClothSetMatrix(Cloth *cloth, Mat4Ptr mat);

// Update
extern void ClothUpdateCollisions(Cloth *cloth);
extern void ClothUpdateConstraints(Cloth *cloth);
typedef enum CCTRWhat {
    CCTR_COPY_DATA        = 1<<0,
    CCTR_COPY_HOOKS        = 1<<1,

    CCTR_COPY_ALL        = CCTR_COPY_HOOKS|CCTR_COPY_DATA,
} CCTRWhat;
extern void ClothCopyToRenderData(Cloth *cloth, Vec3 *hookNormals, CCTRWhat what);

extern void ClothFastUpdateNormals(ClothRenderData *renderData, int calcFirstNormals);
extern void ClothUpdateNormals(ClothRenderData *renderData);
extern void ClothUpdateIntermediatePoints(ClothRenderData *renderData);
extern void ClothDarkenConcavePoints(ClothRenderData *renderData);

// Render
extern ClothMesh *ClothCreateMeshIndices(Cloth *cloth, int sublod);

// Errors (returns -1)
extern int ClothErrorf(char const *fmt, ...);

// ClothLengthConstraint
extern void ClothLengthConstraintInitialize(ClothLengthConstraint *constraint, Cloth *cloth, S32 p1, S32 p2, F32 frac);
extern void ClothLengthConstraintUpdate(const ClothLengthConstraint *constraint, Cloth *cloth);
extern void ClothLengthConstraintFastUpdate(const ClothLengthConstraint *constraint, Cloth *cloth);

// ClothMesh
extern void ClothMeshAllocate(ClothMesh *mesh, int npoints);
extern void ClothMeshSetPoints(ClothMesh *mesh, int npoints, Vec3 *pts, Vec3 *norms, Vec2 *tcoords, Vec3 *binorms, Vec3 *tangents);
extern void ClothMeshCreateStrips(ClothMesh *mesh, int num, int type);
extern void ClothMeshCalcMinMax(ClothMesh *mesh);
extern void ClothMeshSetColorAlpha(ClothMesh *mesh, Vec3 c, F32 a);

extern void ClothStripCreateIndices(ClothStrip *strip, int num, int type);

//ClothCol
extern void ClothColInitialize(ClothCol *clothcol);
extern void ClothColDelete(ClothCol *clothcol);


// MACROS

#define CLOTH_MALLOC(type, n) (type *)malloc((n) * sizeof(type))
#define CLOTH_CALLOC(type, n) (type *)calloc(n, sizeof(type))
#define CLOTH_REALLOC(old, type, n) (type *)realloc(old, (n) * sizeof(type))
#define CLOTH_FREE(p)       { if(p) { free(p);     (p)=0; } }

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif

#ifndef RAD
#define RAD(x) ((x)*(M_PI/180.0f))
#endif

#ifndef DEG
#define DEG(x) ((x)*(180.0f/M_PI))
#endif

#ifndef SIMPLEANGLE
#define SIMPLEANGLE(x) (((x)<=-M_PI) ? (x)+(M_PI*2.0f) : (((x)>M_PI) ? (x)-(M_PI*2.0f) : (x)))
#endif

#ifndef MIN
#define MIN(x,y) ((x)<=(y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) ((x)>=(y) ? (x) : (y))
#endif

#ifndef MINMAX
#define MINMAX(x,min,max) ((x)<(min) ? (min) : (x)>(max) ? (max) : x)
#endif
