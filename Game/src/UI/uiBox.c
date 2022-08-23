#include "UI/uiBox.h"
#include <utilitieslib/utils/mathutil.h>
#include "assert.h"
#include "gameComm/wdwbase.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include <stdlib.h>
#include <utilitieslib/components/MemoryPool.h>
#include "UI/sprite/sprite_base.h"

MP_DEFINE(UIBox);
UIBox* uiBoxCreate(void)
{
    MP_CREATE(UIBox, 128);
    return MP_ALLOC(UIBox);
}

void uiBoxDestroy(UIBox* box)
{
    MP_FREE(UIBox, box);
}

void uiBoxNormalize(UIBox* box)
{
    if(!box)
        return;
    if(box->height < 0)
    {
        box->origin.y -= box->height;
        box->height = -box->height;
    }
    if(box->width < 0)
    {
        box->origin.x -= box->width;
        box->width = -box->width;
    }
}

void uiBoxToCBox(UIBox* uiBox, CBox* cBox)
{
    if(!uiBox || !cBox)
        return;
    cBox->upperLeft.x = uiBox->origin.x;
    cBox->upperLeft.y = uiBox->origin.y;
    cBox->lowerRight.x = uiBox->origin.x + uiBox->width;
    cBox->lowerRight.y = uiBox->origin.y + uiBox->height;
}

void uiBoxFromCBox(UIBox* uiBox, CBox* cBox)
{
    if(!uiBox || !cBox)
        return;
    uiBox->origin.x = cBox->upperLeft.x;
    uiBox->origin.y = cBox->upperLeft.y;
    uiBox->width = cBox->lowerRight.x - uiBox->origin.x;
    uiBox->height = cBox->lowerRight.y - uiBox->origin.y;
}

int uiBoxIntersects(UIBox* box1, UIBox* box2)
{
    if(!box1 || !box2)
        return 0;

    uiBoxNormalize(box1);
    uiBoxNormalize(box2);

    if(    box2->x > box1->x + box1->width || box2->y > box1->y + box1->height ||
        box1->x > box2->x + box2->width || box1->y > box2->y + box2->height)
        return 0;

    return 1;
}

int uiBoxEncloses(UIBox* outterBox, UIBox* innerBox)
{
    if(!outterBox || !innerBox)
        return 0;

    if(    outterBox->x > innerBox->x || outterBox->y > outterBox->y ||
        outterBox->x + outterBox->width < innerBox->x + innerBox->width ||
        outterBox->y + outterBox->height < innerBox->y + innerBox->height)
        return 0;

    return 1;
}

void uiBoxAlter(UIBox* box, UIBoxAlterType type, UIBoxAlterDirection direction, unsigned int magnitudeIn)
{
    CBox extents;
    int magnitude = magnitudeIn;
    if(!box)
        return;
    uiBoxNormalize(box);
    uiBoxToCBox(box, &extents);

    if(direction & UIBAD_LEFT)
    {
        extents.left -= magnitude * type;
        if(extents.left > extents.right)
            extents.left = extents.right;
    }
    if(direction & UIBAD_TOP)
    {
        extents.top -= magnitude * type;
        if(extents.top > extents.bottom)
            extents.top = extents.bottom;
    }
    if(direction & UIBAD_RIGHT)
    {
        extents.right += magnitude * type;
        if(extents.right < extents.left)
            extents.right = extents.left;
    }
    if(direction & UIBAD_BOTTOM)
    {
        extents.bottom += magnitude * type;
        if(extents.bottom < extents.top)
            extents.bottom = extents.top;
    }

    uiBoxFromCBox(box, &extents);
}

int uiMouseClickHit(UIBox* box, int button)
{
    CBox cbox = {0};
    if(!box)
        return 0;
    uiBoxNormalize(box);
    uiBoxToCBox(box, &cbox);
    return mouseClickHit(&cbox, button);
}

int uiMouseUpHit(UIBox* box, int button)
{
    CBox cbox = {0};
    if(!box)
        return 0;
    uiBoxNormalize(box);
    uiBoxToCBox(box, &cbox);
    return mouseUpHit(&cbox, button);
}

int uiMouseCollision(UIBox* box)
{
    CBox cbox = {0};
    if(!box)
        return 0;
    uiBoxNormalize(box);
    uiBoxToCBox(box, &cbox);
    return mouseCollision(&cbox);
}

void uiDrawBox(UIBox* uiBox, int z, int borderColor, int interiorColor)
{
    CBox cBox;
    if(!uiBox)
        return;
    uiBoxToCBox(uiBox, &cBox);
    drawBox(&cBox, z, borderColor, interiorColor);
}

int uiBoxDrawStdButton(UIBox buttonBox, float z, int color, char *txt, float txtScale, int flags)
{
     return drawStdButton(buttonBox.x + buttonBox.width/2, buttonBox.y + buttonBox.height/2, z, buttonBox.width, buttonBox.height, color, txt, txtScale, flags);
}

void uiBoxDefine(UIBox* box, float x, float y, float width, float height)
{
    if(!box)
        return;
    box->x = x;
    box->y = y;
    box->width = width;
    box->height = height;
}

void uiBoxDefineScaled(UIBox * uibox, F32 xp, F32 yp, F32 wd, F32 ht)
{
    ScreenScaleOption option = getScreenScaleOption();
    switch (option)
    {
    case SSO_SCALE_NONE:
    case SSO_SCALE_TEXTURE:
        setScreenScaleOption(SSO_SCALE_ALL);
        reverseToScreenScalingFactorf(&xp, &yp);
        reverseToScreenScalingFactorf(&wd, &ht);
        setScreenScaleOption(option);
        break;
    case SSO_SCALE_ALL:
    case SSO_SCALE_POSITION:
    default:
        break;
    }

    uiBoxDefine(uibox, xp, yp, wd, ht);
}


//------------------------------------------------------------
//  Helper for inbounding a child in a parent.
// this works clockwise from the right side so that
// the x and y will always be within the parent bounds
// even if height or width is greater.
//----------------------------------------------------------
void uiBoxMakeInbounds(UIBox const *parent, UIBox *child)
{
    if( verify( parent && child ))
    {
        child->x += MIN((parent->x + parent->width) - (child->x + child->width), 0);
        child->y += MIN((parent->y + parent->height) - (child->y + child->height), 0);
        child->x = MAX(child->x, parent->x);
        child->y = MAX(child->y, parent->y);
    }
}
