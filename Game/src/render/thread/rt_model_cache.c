/***************************************************************************
*     Copyright (c) 2005-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
*/
#define RT_PRIVATE
#include "render/thread/ogl.h"
#include "render/thread/wcw_statemgmt.h"
#include "render/thread/rt_model_cache.h"
#include <utilitieslib/components/MemoryPool.h>
#include "render/thread/rt_shadow.h"

static const char MODELCACHE_VBO[] = "OpenGL ModelCache_VBO";
static const char RGB_VBO[] = "OpenGL RGB_VBO";

void modelConvertRgbBufferDirect(ConvertRgb *c)
{
    int        id = c->handle;
    U8        *rgbs = c->rgbs;

    WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, id);

    if (rdr_caps.rgbbuffers_as_floats_hack)
    {
        int        i,j,count=0;
        int        buffer_size = c->vert_count * 4 * 3;
        F32        *frgbs;

        frgbs = _alloca(buffer_size);

        for(i=0;i<c->vert_count * 4;i+=4)
        {
            for(j=0;j<3;j++)
                frgbs[count++] = rgbs[i+j] * 1.f / 255.f;
        }
        //!!!!!!!!DO this with null
        //WCW_BufferData(GL_ARRAY_BUFFER_ARB, 0, NULL, GL_STATIC_DRAW_ARB,RGB_VBO);
        WCW_BufferData(GL_ARRAY_BUFFER_ARB, c->vert_count * 4 * 3, frgbs, GL_STATIC_DRAW_ARB,RGB_VBO);

    }
    else
        WCW_BufferData(GL_ARRAY_BUFFER_ARB, c->vert_count * 4, rgbs, GL_STATIC_DRAW_ARB,RGB_VBO);
    modelBindDefaultBuffer();
}

extern MemoryPool MP_NAME(VBO);
extern CRITICAL_SECTION vbo_crit_sect;
void modelFreeVboDirect(VBO **vbo_p)
{
    VBO    *vbo = *vbo_p;

    if (vbo->verts)
        free(vbo->verts);
    else
        WCW_DeleteBuffers(GL_ARRAY_BUFFER_ARB,1,&vbo->vertex_array_id,MODELCACHE_VBO);
    if (vbo->tris)
        free(vbo->tris);
    else
        WCW_DeleteBuffers(GL_ELEMENT_ARRAY_BUFFER_ARB,1,&vbo->element_array_id,MODELCACHE_VBO);
    if (vbo->tex_ids)
        free(vbo->tex_ids);
    if (vbo->shadow)
        shadowFreeDirect(vbo->shadow);

    EnterCriticalSection(&vbo_crit_sect);
    MP_FREE(VBO,vbo);
    LeaveCriticalSection(&vbo_crit_sect);
}

void createVboDirect(U8 *data)
{
    int        *tris;
    Vec3    *verts;
    TexID    *tex_ids;
    VBO        *vbo;

    vbo        = *(VBO**)    data;
    tex_ids    = (TexID*)    (data+4);
    tris    = (int*)    (tex_ids + vbo->tex_count);
    verts    = (Vec3*)    (tris + vbo->tri_count * 3);

    vbo->tex_ids = memcpy(malloc(vbo->tex_count * sizeof(TexID)),tex_ids,vbo->tex_count * sizeof(TexID));

    if (!vbo->no_vbo)
    {
        glGenBuffersARB(1,&vbo->element_array_id); CHECKGL;
        WCW_BindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo->element_array_id);
        if (vbo->tri_count)
            WCW_BufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo->tri_count * 3 * sizeof(GL_INT), tris, GL_STATIC_DRAW_ARB,MODELCACHE_VBO);

        glGenBuffersARB(1,&vbo->vertex_array_id); CHECKGL;
        WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, vbo->vertex_array_id);
        WCW_BufferData(GL_ARRAY_BUFFER_ARB, vbo->vert_buffer_bytes, verts, GL_STATIC_DRAW_ARB,MODELCACHE_VBO);

        WCW_BindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    }
}
