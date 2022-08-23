#include "UI/uiClipper.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include "uiListView.h"        // For UIBox stuff.  Move it out of that file.
#include "win/win_init.h"
#include "UI/sprite/sprite_base.h"
#include <utilitieslib/assert/assert.h>
#include "render/sprite.h"
#include <utilitieslib/utils/mathutil.h>
#include "UI/uiGame.h"

//-----------------------------------------------------------------------------------------------------------------
// Clipper2D
//-----------------------------------------------------------------------------------------------------------------
// Allows each piece of display code to set its own clipping rectangle without having to worry about wiping
// someone elses.
//
// Note that each clipperPush() *MUST* be followed by a corresponding clipperPop().  Otherwise, sprites and
// text will be clipped unexpectedly.
//
typedef struct Clipper2D
{
    UIBox box;            // Box where a 2D element should be clipped to.
    UIBox glBox;        // Same box in GL coordinates.
    char* filename;
    int lineno;
} Clipper2D;

static MemoryPool ClipperMemPool;
static Clipper2D** ClipperStack;

// Converts the box coordinates from top-left origin to bottom-left origin.
static void clipperConvertToGLCoord(UIBox* box)
{
    int clientWidth, clientHeight;
    float xsc, ysc, xOffset, yOffset;

    if(!box)
        return;

    uiBoxNormalize(box);
    calc_scrn_scaling_with_offset(&xsc, &ysc, &xOffset, &yOffset);
    if (!isLetterBoxMenu())
    {
        windowClientSizeThisFrame(&clientWidth, &clientHeight);
    }
    else
    {
        clientHeight = ysc * DEFAULT_SCRN_HT;
    }
    box->height *= ysc;
    box->width *= xsc;
    box->x *= xsc;
    box->x += xOffset;
    box->y *= ysc;
    box->y -= yOffset;
    box->y = clientHeight - (box->y + box->height);
}

void clipperPushRestrict(UIBox* box)
{
    Clipper2D* clipper;
    int bLeft, bTop, bRight, bBottom;
    int cLeft, cTop, cRight, cBottom;
    UIBox newClipBox = {0};

    // If no restriction box was given, remove all clipping restrictions temporarily.
    if(!box)
    {
        clipperPush(box);
        return;
    }

    clipper = clipperGetCurrent();
    if(!clipper)
    {
        clipperPush(box);
        return;
    }

    uiBoxNormalize(box);
    bLeft = box->x;
    bTop = box->y;
    bRight = bLeft + box->width;
    bBottom = bTop + box->height;

    cLeft = clipper->box.x;
    cTop = clipper->box.y;
    cRight = cLeft + clipper->box.width;
    cBottom = cTop + clipper->box.height;

    // Rectangle intersection test.
    if(bLeft > cRight || bTop > cBottom || cLeft > bRight || cTop > bBottom)
    {
        // If the two rectangles do not intersect, shrink the clipper box to nothing.
        clipperPush(&newClipBox);
        return;
    }

    // The rectangles intersect in some way
    newClipBox.x = MAX(bLeft, cLeft);
    newClipBox.y = MAX(bTop, cTop);
    newClipBox.width = MIN(bRight, cRight) - newClipBox.x;
    newClipBox.height = MIN(bBottom, cBottom) - newClipBox.y;
    clipperPush(&newClipBox);
}

void clipperPush(UIBox* box)
{    
    Clipper2D* clipper;
    int clipperIndex;

    if(!ClipperMemPool) 
    {
        ClipperMemPool = createMemoryPool();
        initMemoryPool(ClipperMemPool, sizeof(Clipper2D), 16);
    }

    clipper = mpAlloc(ClipperMemPool);
    clipperIndex = eaPush(&ClipperStack, clipper);

    if(!box) // no box, clip to fullscreen 
            // Pushing null onto stack leaves us in wierd state where we are trying to scissor (there is a stack)
            // but there is no box.
    {
        UIBox fullscreen;
        int w,h;

         windowClientSize( &w, &h );
        uiBoxDefine( &fullscreen, 0, 0, 10000, 10000);
        clipperModifyCurrent(&fullscreen);
    }
    else
        clipperModifyCurrent(box);

    assert(clipperIndex < 128);        // Is someone forgetting to pop some clippers?
}

void clipperPushCBox( CBox *box )
{
    UIBox uibox;
    uiBoxFromCBox(&uibox, box);
    clipperPush( &uibox );
}

void clipperPop()
{
    Clipper2D* clipper;
    assert(eaSize(&ClipperStack) > 0);    // Is someone popping clippers that do not belong to them?
    clipper = eaPop(&ClipperStack);

    if(clipper)
        mpFree(ClipperMemPool, clipper);
}

int clipperGetCount()
{
    return eaSize(&ClipperStack);
}

int clipperIntersects(UIBox* box)
{
    Clipper2D* clipper = clipperGetCurrent();
    if(!clipper)
        return 1;
    if(!box)
        return 0;

    return uiBoxIntersects(&clipper->box, box);
}

UIBox* clipperGetBox(Clipper2D* clipper)
{
    if(!clipper)
        return NULL;
    else
        return &clipper->box;
}

UIBox* clipperGetGLBox(Clipper2D* clipper)
{
    if(!clipper)
        return NULL;
    else
        return &clipper->glBox;
}

void clipperSetCurrentDebugInfo(char* filename, int lineno)
{
    Clipper2D* clipper = clipperGetCurrent();
    if(!clipper)
        return;
    clipper->filename = filename;
    clipper->lineno = lineno;
}

void clipperModifyCurrent(UIBox* box)
{
    Clipper2D* clipper = clipperGetCurrent();
    if(!box || !clipper)
        return;

    uiBoxNormalize(box);
    clipper->box = *box;
    clipper->glBox = *box;
    clipperConvertToGLCoord(&clipper->glBox);
}

Clipper2D* clipperGetCurrent()
{
    return eaGet(&ClipperStack, eaSize(&ClipperStack)-1);
}

// return 0 if completely clipped away
int clipperClipValuesGLSpace(Clipper2D* clipper, Vec2 ul, Vec2 lr, float *u1, float *v1, float *u2, float *v2)
{
    Vec2 clipper_ul, clipper_lr;
    Vec2 x_offsets, y_offsets;
    float clipper_width, clipper_height, box_width, box_height, u_span, v_span;

    if (!clipper)
        return 1;

    clipper_width = clipper->glBox.width;
    clipper_height = clipper->glBox.height;
    clipper_ul[0] = clipper->glBox.x;
    clipper_lr[0] = clipper->glBox.x + clipper_width;
    clipper_ul[1] = clipper->glBox.y + clipper_height;
    clipper_lr[1] = clipper->glBox.y;

    // trivial rejections
    if (lr[0] <= clipper_ul[0])
        return 0;
    if (ul[0] >= clipper_lr[0])
        return 0;
    if (lr[1] >= clipper_ul[1])
        return 0;
    if (ul[1] <= clipper_lr[1])
        return 0;

    x_offsets[0] = clipper_ul[0] - ul[0];
    x_offsets[1] = lr[0] - clipper_lr[0];
    y_offsets[0] = clipper_lr[1] - lr[1];
    y_offsets[1] = ul[1] - clipper_ul[1];

    if (x_offsets[0] <= 0 &&
        x_offsets[1] <= 0 &&
        y_offsets[0] <= 0 &&
        y_offsets[1] <= 0)
        return 1;

    // now we have work to do
    box_width = lr[0] - ul[0];
    box_height = ul[1] - lr[1];
    u_span = *u2 - *u1;
    v_span = *v2 - *v1;

    if (x_offsets[0] > 0)
    {
        float t = u_span * x_offsets[0] / box_width;
        *u1 += t;
        ul[0] = clipper_ul[0];
    }

    if (x_offsets[1] > 0)
    {
        float t = u_span * x_offsets[1] / box_width;
        *u2 -= t;
        lr[0] = clipper_lr[0];
    }

    if (y_offsets[0] > 0)
    {
        float t = v_span * y_offsets[0] / box_height;
        *v2 -= t;
        lr[1] = clipper_lr[1];
    }

    if (y_offsets[1] > 0)
    {
        float t = v_span * y_offsets[1] / box_height;
        *v1 += t;
        ul[1] = clipper_ul[1];
    }

    return 1;
}

int clipperTestValuesGLSpace(Clipper2D* clipper, Vec2 ul, Vec2 lr)
{
    F32 xp, yp;
    F32 xScale, yScale, xOffset, yOffset;
    calc_scrn_scaling_with_offset(&xScale, &yScale, &xOffset, &yOffset);

    if (!clipper)
        return 1;

    xp = lr[0]+xOffset;
    yp = lr[1]+yOffset;

    if (xp <= clipper->glBox.x)
        return 0;
    if (yp >= clipper->glBox.y + clipper->glBox.height)
        return 0;

    xp = ul[0]+xOffset;
    yp = ul[1]+yOffset;

    if (xp >= clipper->glBox.x + clipper->glBox.width)
        return 0;
    if (yp <= clipper->glBox.y)
        return 0;

    return 1;
}
