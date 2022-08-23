/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/utils.h>

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiTree.h"
#include "UI/uiInput.h"

#include <utilitieslib/components/MemoryPool.h>

#include "formatter/smf_main.h"

#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/Cbox.h>

#include <utilitieslib/language/MessageStoreUtil.h>

static TextAttribs s_taDefaults =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// T R E E   U I   C O N T R O L 
////////////////////////////////////////////////////////////////////////////////////////////

#define SPACE_BETWEEN_NODES        10.0f
#define INDENT_SPACE            10.0f


static void uiTreeMeasureNode(uiTreeNode *pNode, float width, float scale)
{ 
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    float indentSpace = INDENT_SPACE;
    int textHeight = 0.0f;
    
    if (pNode->fpDraw)
        textHeight = pNode->fpDraw(pNode, 0.0f, 0.0f, 0.1f, width, scale, false);

    pNode->heightThisNode = pNode->height = textHeight;
    if (pNode->state&UITREENODE_EXPANDED)
    {
        int i, count = eaSize(&pNode->children);

        for (i = 0; i < count; i++)
        {
            uiTreeMeasureNode(pNode->children[i], width-(indentSpace*scale), scale);
            if (pNode->children[i]->height > 0.0f)
                pNode->height += pNode->children[i]->height;
        }
    }
}

void uiTreeRecalculateNodeSize(uiTreeNode *pNode, float width, float scale)
{

    // remove old size from parent
    uiTreeNode *pParent = pNode->pParent;
    while (pParent)
    {
        if (pNode->height > 0.0f)
            pParent->height -= pNode->height;
        pParent = pParent->pParent;
    }

    // calculate new size
    uiTreeMeasureNode(pNode, width, scale);

    // add new size back to parent
    pParent = pNode->pParent;
    while (pParent)
    {
        if (pNode->height > 0.0f)
            pParent->height += pNode->height;
        pParent = pParent->pParent;
    }
}

float uiTreeDisplay(uiTreeNode *pNode, int depth, float xorig, float *yorig, float ytop, float zorig, float width, 
                    float *height, float scale, int bDisplay)
{
    int textHeight;
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int bRecalcSize = false;
    uiTreeNode *pParent = NULL;

    if (!pNode) 
        return 0.0f;

    if (pNode->height < 0.0f)
        uiTreeMeasureNode(pNode, width, scale);

    // draw and measure text
    if (bDisplay && *yorig + pNode->height >= ytop) 
    {
        if (*yorig + pNode->heightThisNode >= ytop && *height + pNode->heightThisNode >= 0.0f)
        {
            if (pNode->fpDraw)
                textHeight = pNode->fpDraw(pNode, xorig, *yorig, zorig, width, scale, !(pNode->state&UITREENODE_HIDDEN));
            else
                textHeight = 0.0f;

            if (ytop > *yorig)
                *height -= (*yorig + pNode->heightThisNode - ytop);
            else
                *height -= pNode->heightThisNode;
        }

        *yorig += pNode->heightThisNode;

        if (pNode->state&UITREENODE_EXPANDED) 
        {
            // draw any children 
            int i, count = eaSize(&pNode->children);

            for (i = 0; i < count; i++)
            {
                float ht = uiTreeDisplay(pNode->children[i], depth+1, xorig+(10*scale), yorig, ytop, zorig, 
                                width-(10*scale), height, scale, !(pNode->children[i]->state&UITREENODE_HIDDEN));
            }
        }
    } else     {
        *yorig += pNode->height;
    }

    return pNode->height;
}

float uiTreeGenericTextDrawCallback(uiTreeNode *pNode, float x, float y, float z, float width, 
                                    float scale, int display)
{
    static SMFBlock *block = 0;
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int textHeight;
    CBox box;
    int box_color = CLR_NORMAL_BACKGROUND;
    int forground_color = CLR_NORMAL_FOREGROUND;

    if (!block)
    {
        block = smfBlock_Create();
    }

    if (display)
    {
        s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*(scale*1.0f)));
        textHeight = smf_ParseAndDisplay(block, textStd((char *)pNode->pData), x+19*scale, 
                                            y+2*scale, z, width-19*scale, 0,    false, false, 
                                            &s_taDefaults, NULL, 0, false);

        // check for mouse over
        BuildCBox( &box, x, y, width, textHeight+5*scale);
        if (mouseCollision(&box))
        {
            box_color = CLR_MOUSEOVER_BACKGROUND;
            forground_color = CLR_MOUSEOVER_FOREGROUND;
        }

        // draw frame
        drawFlatFrame(PIX2, R10, x, y, z-0.5, width, textHeight+5*scale, scale, forground_color, 
            box_color);

        // check for click
        if( mouseClickHit( &box, MS_LEFT ) )
        {
            pNode->state ^= UITREENODE_EXPANDED;

            ////////////////////////////////////////////
            // if this node has changed state recalculate size
            uiTreeRecalculateNodeSize(pNode, width, scale);
        }

        // expanded arrow
        if (pNode->state & UITREENODE_EXPANDED)
        {
            AtlasTex * leftArrow  = atlasLoadTexture( "Jelly_arrow_down.tga");    

            display_sprite( leftArrow, x+6.0*scale, y+8.0*scale, z, 0.5*scale, scale, CLR_WHITE);
        } else {
            AtlasTex * leftArrow  = atlasLoadTexture( "teamup_arrow_expand.tga");    // uses same arrows as uiTray.c

            display_sprite( leftArrow, x+10.0*scale, y+5.0*scale, z, scale, 0.4*scale, CLR_WHITE);
        }

    } else {
        s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*(scale*1.0f)));
        textHeight = smf_ParseAndFormat(block, textStd((char *)pNode->pData), 0.0f, 0.0f,
                                        0.1f, width, 0, false, false, false, &s_taDefaults, 0);
    }

    pNode->heightThisNode = textHeight+spaceBetweenNodes*scale;

    return pNode->heightThisNode;
}

MP_DEFINE(uiTreeNode);

uiTreeNode* uiTreeNewNode() 
{
    uiTreeNode *pNode;

    MP_CREATE(uiTreeNode, 128); // currently, this is 1k at a time

    pNode = MP_ALLOC(uiTreeNode);
    memset(pNode, 0, sizeof(uiTreeNode));

    //    pNode->displayString = NULL;
//     pNode->pParent = NULL;
//     pNode->children = NULL;
//     pNode->pData = NULL;
//     pNode->fpDraw = NULL;
    pNode->height = -1.0f;
    pNode->heightThisNode = -1.0f;
//    pNode->state = UITREENODE_NONE;
    pNode->fpDraw = uiTreeGenericTextDrawCallback;
//    pNode->fpFree = NULL;

    return pNode;
}

void uiTreeFree(uiTreeNode *pRootNode)
{
    static bool recursion;

    if(!pRootNode)
        return;

    // free children
    recursion = true;
    eaDestroyEx(&pRootNode->children, uiTreeFree);
    recursion = false;

    // check to see if we need to clean up this node's data
    if(pRootNode->fpFree)
        pRootNode->fpFree(pRootNode);

    if(!recursion && pRootNode->pParent)
        eaFindAndRemove(&pRootNode->pParent->children, pRootNode);

    MP_FREE(uiTreeNode,pRootNode);
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
