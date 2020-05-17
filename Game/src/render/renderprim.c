#include "render/thread/rt_queue.h"
#include "render/renderprim.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdgame.h"
#include "win/win_init.h"
#include "render/thread/rt_state.h"
#include "graphics/gfx.h"
#include "render/thread/ogl.h"


static RdrLineBox    *queuedlines;
static DrawType        *queuedcmds;
static int queuedlines_count, queuedlines_max;
static int queuedcmds_count, queuedcmds_max;
static int g_queueit;

void queuedLinesReset(void)
{
    queuedlines_count = queuedcmds_count = 0;
}

void queuedLinesDraw(void)
{
    int i;
    for (i=0; i<queuedlines_count; i++) {
        rdrQueue(queuedcmds[i],&queuedlines[i],sizeof(queuedlines[i]));
    }
    queuedLinesReset();
}

static void queuePrim(DrawType cmd,RdrLineBox *prim)
{
    if (g_queueit)
    {
        RdrLineBox *newline;
        DrawType *newcmd;

        newline = dynArrayAdd(&queuedlines, sizeof(queuedlines[0]), &queuedlines_count, &queuedlines_max, 1);
        *newline = *prim;
        newcmd = dynArrayAdd(&queuedcmds, sizeof(queuedcmds[0]), &queuedcmds_count, &queuedcmds_max, 1);
        *newcmd = cmd;
    } else {
        rdrQueue(cmd,prim,sizeof(*prim));
    }
}

int drawEnablePrimQueue(int enable)
{
    int        last = g_queueit;

    g_queueit = enable;
    return last;
}

void drawFilledBox4(int x1, int y1, int x2, int y2, int argb_ul, int argb_ur, int argb_ll, int argb_lr){
    float z = -1;
    int temp;
    RdrLineBox box;

    if(x1 > x2){
        temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if(y1 > y2){
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    box.points[0][0] = x1;
    box.points[0][1] = y1;
    box.points[0][2] = z;
    box.points[1][0] = x2;
    box.points[1][1] = y2;
    box.points[1][2] = z;
    box.colors[0] = argb_ul;
    box.colors[1] = argb_ur;
    box.colors[2] = argb_ll;
    box.colors[3] = argb_lr;
    queuePrim(DRAWCMD_FILLEDBOX,&box);
}


void drawFilledBox(int x1, int y1, int x2, int y2, int argb){
    drawFilledBox4(x1, y1, x2, y2, argb, argb, argb, argb);
}

void drawLine2(int x1, int y1, int x2, int y2, int argb1, int argb2){
    RdrLineBox    line;
    float z = -1;

    line.points[0][0] = x1;
    line.points[0][1] = y1;
    line.points[0][2] = z;
    line.points[1][0] = x2;
    line.points[1][1] = y2;
    line.points[1][2] = z;
    line.colors[0] = argb1;
    line.colors[1] = argb2;
    line.line_width = 1;
    queuePrim(DRAWCMD_LINE2D,&line);
}

void drawLine(int x1, int y1, int x2, int y2, int argb){
    drawLine2(x1, y1, x2, y2, argb, argb);
}

void drawLine3dColorWidth(const Vec3 p1, int argb1, const Vec3 p2, int argb2, F32 width, bool queueit)
{
    RdrLineBox    line;
    int            queue_state;

    copyVec3(p1,line.points[0]);
    copyVec3(p2,line.points[1]);
    line.colors[0] = argb1;
    line.colors[1] = argb2;
    line.line_width = width;
    queue_state = drawEnablePrimQueue(1);
    queuePrim(DRAWCMD_LINE3D,&line);
    drawEnablePrimQueue(queue_state);
}

void drawLine3D_2(const Vec3 p1, int argb1, const Vec3 p2, int argb2)
{
    drawLine3dColorWidth(p1,argb1,p2,argb2,1,false);
}

void drawLine3D(const Vec3 p1, const Vec3 p2, int argb)
{
    drawLine3dColorWidth(p1,argb,p2,argb,1,false);
}

void drawLine3DWidth(const Vec3 p1, const Vec3 p2, int argb, F32 width)
{
    drawLine3dColorWidth(p1,argb,p2,argb,width,false);
}

void drawLine3D_2Queued(const Vec3 p1, int argb1, const Vec3 p2, int argb2)
{
    drawLine3dColorWidth(p1,argb1,p2,argb2,1,true);
}

void drawLine3DQueued(const Vec3 p1, const Vec3 p2, int argb)
{
    drawLine3dColorWidth(p1,argb,p2,argb,1,true);
}

void drawLine3DWidthQueued(const Vec3 p1, const Vec3 p2, int argb, F32 width)
{
    drawLine3dColorWidth(p1,argb,p2,argb,width,true);
}

void drawTriangle3D_3(const Vec3 p1, int argb1, const Vec3 p2, int argb2, const Vec3 p3, int argb3){
    RdrLineBox    tri;
    int            queue_state;

    copyVec3(p1,tri.points[0]);
    copyVec3(p2,tri.points[1]);
    copyVec3(p3,tri.points[2]);
    tri.colors[0] = argb1;
    tri.colors[1] = argb2;
    tri.colors[2] = argb3;
    queue_state = drawEnablePrimQueue(1);
    queuePrim(DRAWCMD_FILLEDTRI,&tri);
    drawEnablePrimQueue(queue_state);
}

void drawTriangle3D(const Vec3 p1, const Vec3 p2, const Vec3 p3, int argb){
    drawTriangle3D_3(p1, argb, p2, argb, p3, argb);
}

void drawQuad3D(const Vec3 a,const Vec3 b,const Vec3 c,const Vec3 d,U32 color,F32 line_width)
{
    if (!line_width)
    {
        drawTriangle3D(a,b,c,color);
        drawTriangle3D(c,d,a,color);
    }
    else
    {
        drawLine3DWidth(a,b,color,line_width);
        drawLine3DWidth(b,c,color,line_width);
        drawLine3DWidth(c,d,color,line_width);
        drawLine3DWidth(d,a,color,line_width);
    }
}

void drawBox3D(const Vec3 min,const Vec3 max,const Mat4 mat,U32 color,F32 line_width)
{
    Vec3    minmax[2], pos;
    Vec3    corners[2][4];
    int        i,y;

    copyVec3(min, minmax[0]);
    copyVec3(max, minmax[1]);

     for(y=0;y<2;y++)
    {
        for(i=0;i<4;i++)
        {
            pos[0] = minmax[i==1 || i==2][0];
            pos[1] = minmax[y][1];
            pos[2] = minmax[i/2][2];
            mulVecMat4(pos, mat, corners[y][i]);
        }
    }
    drawQuad3D(corners[0][0],corners[0][1],corners[0][2],corners[0][3],color,line_width);
    drawQuad3D(corners[1][0],corners[1][1],corners[1][2],corners[1][3],color,line_width);
    for(i=0;i<4;i++)
        drawQuad3D(corners[1][i],corners[1][(i+1)&3],corners[0][(i+1)&3],corners[0][i],color,line_width);
}

void rdrSetClipPlanes(int numPlanes, float planes[6][4])
{
    RdrClipPlane rcp;
    int i;

    // Only 6 supported since OpenGL only guarantees 6 user clip planes
    assert(numPlanes < 6);

    rcp.numplanes = numPlanes;
    for (i = 0; i < numPlanes; i++)
    {
        assert(planes[i]);
        rcp.planes[i][0] = planes[i][0];
        rcp.planes[i][1] = planes[i][1];
        rcp.planes[i][2] = planes[i][2];
        rcp.planes[i][3] = planes[i][3];
    }

    rdrQueue(DRAWCMD_CLIPPLANE,&rcp,sizeof(rcp));
}

void rdrSetStencil(int enable,RdrStencilOp op,U32 func_ref)
{
    RdrStencil    rs;

    rs.enable    = enable;
    rs.func_ref    = func_ref;
    rs.op        = op;
    rdrQueue(DRAWCMD_STENCIL,&rs,sizeof(rs));
}

void rdrSetFog(F32 near_dist, F32 far_dist, const Vec3 color)
{
    RdrFog    vals;

    vals.fog_dist[0] = near_dist;
    vals.fog_dist[1] = far_dist;
    if (color)
    {
        vals.has_color = 1;
        copyVec3(color,vals.fog_color);
    }
    else
        vals.has_color = 0;
    rdrQueue(DRAWCMD_FOG,&vals,sizeof(vals));
}

void rdrGetFrameBufferAsync(F32 x, F32 y, int width, int height, GetFrameBufType type, void *buf)
{
    RdrGetFrameBuffer    get;

    get.data    = buf;
    get.x        = x;
    get.y        = y;
    get.width    = width;
    get.height    = height;
    get.type    = type;
    rdrQueue(DRAWCMD_GETFRAMEBUFFER,&get,sizeof(get));
}

void rdrGetFrameBuffer(F32 x, F32 y, int width, int height, GetFrameBufType type, void *buf)
{
    rdrGetFrameBufferAsync(x,y,width,height,type,buf);
    rdrQueueFlush();
}

void rdrGetTexSize(int texid, int *width, int *height)
{
    RdrGetTexInfo get = {0};

    get.texid = texid;
    get.widthout = width;
    get.heightout = height;

    rdrQueue(DRAWCMD_GETTEXINFO, &get, sizeof(get));
    rdrQueueFlush();
}

void *rdrGetTex(int texid, int *width, int *height, int get_color, int get_alpha, int floating_point)
{
    int bpp, w, h;
    RdrGetTexInfo get = {0};

    rdrGetTexSize(texid, &w, &h);

    if (get_alpha)
    {
        if (get_color)
        {
            get.pixelformat = GL_RGBA;
            bpp = 4;
        }
        else
        {
            get.pixelformat = GL_ALPHA;
            bpp = 1;
        }
    }
    else
    {
        get.pixelformat = GL_RGB;
        bpp = 3;
    }

    get.texid = texid;
    if (floating_point)
    {
        get.data = calloc(w * h * bpp, sizeof(float));
        get.pixeltype = GL_FLOAT;
    }
    else
    {
        get.data = calloc(w * h * bpp, sizeof(U8));
        get.pixeltype = GL_UNSIGNED_BYTE;
    }

    rdrQueue(DRAWCMD_GETTEXINFO, &get, sizeof(get));
    rdrQueueFlush();

    if (width)
        *width = w;
    if (height)
        *height = h;

    return get.data;
}

void rdrSetBgColor(const Vec3 color)
{
    rdrQueue(DRAWCMD_BGCOLOR,color,sizeof(Vec3));
}

void rdrFreeBuffer(int id,char *mem_type)
{
    int        size = sizeof(id) + strlen(mem_type)+1;
    int        *mem = malloc(size);

    mem[0] = id;
    strcpy((char*)(mem+1),mem_type);
    rdrQueue(DRAWCMD_FREEBUFFER,mem,size);
    free(mem);
}

void rdrFreeMem(void *data)
{
    rdrQueue(DRAWCMD_FREEMEM,&data,sizeof(data));
}

void rdrClearScreenEx(ClearScreenFlags clear_flags)
{
    rdrQueue(DRAWCMD_CLEAR, &clear_flags, sizeof(clear_flags));
}

static bool stipple_inited=false;
static bool stipple_needed=true;

void rdrStippleStencilNeedsRebuild(void)
{
    stipple_inited = false;
}

void rdrClearScreen(void)
{
    ClearScreenFlags clear_flags = CLEAR_DEFAULT;
    if (stipple_needed && scene_info.stippleFadeMode) {
        if (stipple_inited /*&& !game_state.test2*/)
            clear_flags &=~CLEAR_STENCIL; // Don't clear the stencil
    } else {
        stipple_inited = false;
    }
    rdrQueue(DRAWCMD_CLEAR, &clear_flags, sizeof(clear_flags));
}

// Must be called immediately after a rdrClearScreen
void rdrSetupStippleStencil(void)
{
    if (stipple_needed && scene_info.stippleFadeMode && (!stipple_inited /*|| game_state.test2*/)) {
        stipple_inited = true;
        rdrQueueCmd(DRAWCMD_SETUP_STIPPLE);
    }
}

void rdrSetAdditiveAlphaWriteMask(int on)
{
    rdrQueue(DRAWCMD_ADDITIVE_ALPHA_WRITE_MASK,&on,sizeof(on));
}


