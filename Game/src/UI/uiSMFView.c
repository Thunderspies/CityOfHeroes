/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#define SMFVIEW_PRIVATE

#include <utilitieslib/assert/assert.h>

#include "gameComm/wdwbase.h"
#include "UI/sprite/sprite_base.h"

#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"

#include "formatter/smf_main.h"
#include "uiSMFView.h"

#include <utilitieslib/components/earray.h>
#include "UI/uiTabControl.h"

#define MARGIN 5

/**********************************************************************func*
 * smfview_Create
 *
 */
SMFView *smfview_Create(int wnd)
{
    SMFView *pview = (SMFView *)calloc(sizeof(SMFView), 1);

    pview->sb.wdw = wnd;
    pview->radius = R10;
    pview->block = smfBlock_Create();
    pview->hasScrollBar = true;

    return pview;
}

/**********************************************************************func*
 * smfview_CreateInPlace
 *
 */
SMFView *smfview_CreateInPlace(SMFView *pview, int wnd)
{
    memset(pview, 0, sizeof(SMFView));

    pview->sb.wdw = wnd;
    pview->radius = R10;
    pview->block = smfBlock_Create();

    return pview;
}

/**********************************************************************func*
 * smfview_Destroy
 *
 */
void smfview_Destroy(SMFView *pview)
{
    if(pview!=NULL)
    {
        smfBlock_Destroy(pview->block);
        free(pview);
    }
}

/**********************************************************************func*
 * smfview_SetWindowRadius
 *
 */
void smfview_SetWindowRadius(SMFView *pview, int radius)
{
    assert(pview!=NULL);

    pview->radius = radius;
}

/**********************************************************************func*
 * smfview_SetLocation
 *
 */
void smfview_SetLocation(SMFView *pview, int x, int y, int z)
{
    assert(pview!=NULL);

    pview->x = x;
    pview->y = y;
    pview->z = z;
}

/**********************************************************************func*
 * smfview_SetBaseLocation
 *
 */
void smfview_SetBaseLocation(SMFView *pview, int x, int y, int z, int wd, int ht)
{
    assert(pview!=NULL);

    pview->xbase = x;
    pview->ybase = y;
    pview->zbase = z;
    pview->wdbase = wd;
    pview->htbase = ht;
}

/**********************************************************************func*
 * smfview_SetSize
 *
 */
void smfview_SetSize(SMFView *pview, int wd, int ht)
{
    assert(pview!=NULL);

    // changing height doesn't change the formatting
    if(pview->wd != wd)
    {
        pview->bReformat = true;
    }

    pview->wd = wd;
    pview->ht = ht;
}

/**********************************************************************func*
 * smfview_SetAttribs
 *
 */
void smfview_SetAttribs(SMFView *pview, TextAttribs *pattrs)
{
    assert(pview!=NULL);

    if(pview->pattrs!=pattrs)
    {
        pview->bReformat = true;
    }

    pview->pattrs = pattrs;
}

/**********************************************************************func*
* smfview_SetAttribs
*
*/
void smfview_SetScale(SMFView *pview, float scale)
{
    assert(pview!=NULL);

    if(!pview->pattrs)
        return;

    if( pview->pattrs->piScale!=(int*)((int)scale) )
    {
        pview->bReformat = true;
    }

    pview->pattrs->piScale=(int*)((int)scale) ;
}

/**********************************************************************func*
 * smfview_SetText
 *
 */
void smfview_SetText(SMFView *pview, char *pch)
{
    assert(pview!=NULL);

    pview->pch = pch;
}

/**********************************************************************func*
 * smfview_Reparse
 *
 */
void smfview_Reparse(SMFView *pview)
{
    assert(pview!=NULL);

    pview->bReparse = true;
}

/**********************************************************************func*
 * smfview_Reformat
 *
 */
void smfview_Reformat(SMFView *pview)
{
    assert(pview!=NULL);

    pview->bReformat = true;
}


/**********************************************************************func*
 * smfview_GetHeight
 *
 */
int smfview_GetHeight(SMFView *pview)
{
    int iHeight;

    assert(pview!=NULL);

    iHeight = smf_ParseAndFormat(pview->block,
        pview->pch,
        0, 0, 0,
        pview->wd-2*MARGIN, pview->ht,
        false,
        pview->bReparse,
        pview->bReformat,
        pview->pattrs, 0);

    pview->bReparse = false;
    pview->bReformat = false;

    return iHeight;
}

/**********************************************************************func*
* smfview_setAlignment
*
*/
void smfview_setAlignment( SMFView *pview, int alignment )
{
    smf_SetFlags(pview->block, 0, 0, 0, 0, 0, 0, 0, 0, alignment, 0, 0, 0);
    pview->bReparse = false;
    pview->bReformat = true;
}
/**********************************************************************func*
 * smfview_Draw
 *
 */
void smfview_Draw(SMFView *pview)
{
    smfview_DrawWithCallback(pview, NULL);
}

void smfview_DrawWithCallback(SMFView *pview, int (*callback)(char *))
{
    CBox box;
    assert(pview!=NULL);

    if( pview->sb.wdw==0 || window_getMode(pview->sb.wdw) == WINDOW_DISPLAYING )
    {
        float x, y, z, wd, ht, sc;
        int color, bcolor;
        int iHeight;
        int yoffset = 0;

        if(pview->sb.wdw == 0)
        {
            x = pview->xbase;
            y = pview->ybase;
            z = pview->zbase;
            wd = pview->wdbase;
            ht = pview->htbase;
            sc = 1.0;
            color = bcolor = winDefs[1].loc.color|0xff; // same default as scrollbar
        }
        else
        {
            if(!window_getDims(pview->sb.wdw, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
                return;
        }
        if (pview->tabs)
        {
            drawTabControl(pview->tabs, x+pview->x+PIX3*5*sc, y+pview->y, z+pview->z, pview->wd-PIX3*10*sc, PIX3*7*sc, sc, color, color, TabDirection_Horizontal);
            y += PIX3*7*sc;
            yoffset += PIX3*7*sc;
        }

        set_scissor(true);
          scissor_dims(x+pview->x, y+pview->y, pview->wd, pview->ht);
        BuildCBox(&box, x+pview->x, y+pview->y, pview->wd, pview->ht);

        iHeight = smf_ParseAndDisplay(pview->block,
            pview->pch,
            x+pview->x+MARGIN, y - pview->sb.offset + pview->y, z+pview->z,
            pview->wd-2*MARGIN, pview->ht,
            pview->bReparse,
            pview->bReformat,
            pview->pattrs,
              callback,
            0, 
            true);

        pview->bReparse = false;
        pview->bReformat = false;

        set_scissor(false);

        if(pview->hasScrollBar)
        {
            if(pview->sb.wdw == 0)
            {
                doScrollBar(&pview->sb, pview->ht-2*R10, iHeight+MARGIN, x+wd, y+pview->y+R10+yoffset, z+pview->z, &box, 0);
            }
            else
            {
                doScrollBar(&pview->sb, pview->ht, iHeight+MARGIN, wd, pview->y+yoffset, z+pview->z, &box, 0 );
            }
        }
    }
}

/* End of File */

/**********************************************************************func*
* smfview_AddTabs
*
*/
void smfview_AddTabs(SMFView *pview, char** tabNames, uiTabActionFunc func)
{
    int i, n = eaSize(&tabNames);

    if (!pview->tabs)
        pview->tabs = uiTabControlCreate(TabType_Undraggable, func, 0, 0, 0, 0 );

    for (i = 0; i < n; i++)
        uiTabControlAdd(pview->tabs, tabNames[i], (uiTabData)i);
    uiTabControlSelect(pview->tabs, 0);
}

/* End of File */

bool smfview_HasFocus(SMFView *pview)
{
    return pview && smfBlock_HasFocus(pview->block);
}

void smfview_SetScrollBar(SMFView *pview, bool hasScrollBar)
{
    if (!pview)
    {
        return;
    }
    
    pview->hasScrollBar = hasScrollBar;
}

SMFBlock *smfview_GetBlock(SMFView *view)
{
    return view->block;
}
void smfview_ResetScrollBarOffset(SMFView *view)
{
    if (!view)
        return;
    view->sb.offset = 0;
}
