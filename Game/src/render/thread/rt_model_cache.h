#ifndef _RT_MODEL_CACHE_H
#define _RT_MODEL_CACHE_H

#define VBOS_USE     1    //initialize this object as using VBOs
#define VBOS_DONTUSE 2    //This model is being used for something besides straight drawing, don't set VBOs ( stencil shodow objects in particular )

#include <utilitieslib/stdtypes.h>

typedef struct Model Model;

typedef F32 Weights[2]; 
typedef S16 Matidxs[2]; 

typedef struct
{
    U16        id;        //~name (idx of this texture name in texture name list)
    U16        count;  //number of tris in model->sts that use this texture 
} TexID;

typedef struct
{
    int handle;
    int    vert_count;
    U8 rgbs[1];
} ConvertRgb;

#if defined(RT_PRIVATE) || defined(RT_ALLOW_VBO)

typedef struct ShadowInfo ShadowInfo;

typedef struct VBO
{
    // per triangle data
    int            element_array_id;    
    int            *tris;            

    // per vertex data
    int            vertex_array_id;
    Vec3        *verts;            //
    Vec3        *norms;            //
    Vec2        *sts;            //
    Vec2        *sts2;            //
    Vec2        *sts3;            // lightmap coords
    Vec4        *tangents;        // bump mapping info (w component is binormal sign, binormal calculated on the fly)
    Weights        *weights;        // skinning bone weights
    Matidxs        *matidxs;        // skinning matrix palette indices

    // stencil shadows 
    ShadowInfo    *shadow;

    U32            bump_init : 1;
    U32            in_use : 1;
    U32            no_vbo : 1;
    U32            sts2_init : 1;

    // all copied from model
    int            tri_count,vert_count;
    int            vert_buffer_bytes;
    int            tex_count;
    TexID        *tex_ids;
    U32            flags;

    int            frame_id;        // used for some debugging stuff

} VertexBufferObject, VBO;
#endif

#ifdef RT_PRIVATE
void modelConvertRgbBufferDirect(ConvertRgb *c);
void createVboDirect(U8 *data);
static INLINEDBG int modelBindBuffer(VBO *vbo);
static INLINEDBG void modelBindDefaultBuffer(void);
void modelFreeVboDirect(VBO **vbo_p);

#include "render/thread/rt_init.h"
#include "render/thread/wcw_statemgmt.h"

static INLINEDBG int modelBindBuffer(VBO *vbo)
{
    if (!rdr_caps.use_vbos)
        return 1;
    WCW_BindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo->element_array_id);
    WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, vbo->vertex_array_id);
    return 1;
}

static INLINEDBG void modelBindDefaultBuffer()
{
    WCW_BindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}
#endif


#endif
