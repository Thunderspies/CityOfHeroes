/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdio.h>  // fopen
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <sys/stat.h>

#include "formatter/smf_util.h"
#include "formatter/smf_render.h"

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
#include "entity/costume.h"
#include "bases/DetailRecipe.h"

#include "gameComm/Npc.h"
#include "graphics/truetype/ttFontDraw.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiUtil.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiEnhancement.h"
#include "UI/uiGame.h"
#include "graphics/seqgraphics.h"

#include "graphics/textureatlas.h"
#include <utilitieslib/utils/timing.h>

#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/StringUtil.h>
#include "UI/uiPictureBrowser.h"
#include "win/win_init.h"

#include "entity/entity.h"
#include "entity/entclient.h"

#define TAG_MATCHES(y) (smf_aTagDefs[pBlock->iType].id==k_##y)
#define SM_BLOAT (-1)

/**********************************************************************func*
* DrawRect
*
* Draws a normal rectangle of the specified color in the specified location.
* Used to draw the highlighted background of text (e.g. mouse selection in chat window)
*/
static void DrawRect(float iX, float iY, float iZ, float iWidth, float iHeight, int iColor)
{
    float fXScale;
    float fYScale;

    AtlasTex *c =  white_tex_atlas;

    fXScale = (float)(iWidth)/(float)c->width;
    fYScale = (float)(iHeight)/(float)c->height;

    display_sprite(c,
        iX, iY, iZ,
        fXScale, fYScale, iColor);
}

/**********************************************************************func*
 * DrawRoundedRect
 *
 * Draws a rounded rectangle of the specified color in the specified location.
 * Used to draw the highlighted background in tr's (e.g. mission accept dialog)
 */
static void DrawRoundedRect(float iX, float iY, float iZ, float iWidth, float iHeight, int iColor)
{
    float fXScale;
    float fYScale;

    AtlasTex *ul = atlasLoadTexture("UL.tga");
    AtlasTex *ll = atlasLoadTexture("LL.tga");
    AtlasTex *lr = atlasLoadTexture("LR.tga");
    AtlasTex *ur = atlasLoadTexture("UR.tga");
    AtlasTex *c =  white_tex_atlas;

    display_sprite(ul,
        iX, iY, iZ,
        1.0f, 1.0f, iColor);

    fXScale = (float)(iWidth - ul->width - ur->width)/(float)c->width;
    fYScale = (float)(ul->height)/(float)c->height;

    display_sprite(c,
        iX+ul->width, iY, iZ,
        fXScale, fYScale, iColor);
    display_sprite(ur,
        iX + iWidth - ur->width, iY, iZ,
        1.0f, 1.0f, iColor);

    fXScale = (float)(iWidth)/(float)c->width;
    fYScale = (float)(iHeight - ul->height - lr->height)/(float)c->height;
    display_sprite(c,
        iX, iY+ul->height, iZ,
        fXScale, fYScale, iColor);

    display_sprite(ll,
        iX, iY + iHeight - lr->height, iZ,
        1.0f, 1.0f, iColor);

    fXScale = (float)(iWidth - ll->width - lr->width)/(float)c->width;
    fYScale = (float)(ll->height)/(float)c->height;
    display_sprite(c,
        iX+ll->width, iY + iHeight - lr->height, iZ,
        fXScale, fYScale, iColor);
    display_sprite(lr,
        iX+iWidth - lr->width, iY + iHeight - lr->height, iZ,
        1.0f, 1.0f, iColor);
}

static void DrawGradientRect(float iX, float iY, float iZ, float iWidth, float iHeight, int iColor)
{
    AtlasTex *tex = atlasLoadTexture("CharSel_OriginBar.tga");

    if (tex && tex->width && tex->height)
        display_sprite(tex, iX, iY, iZ, iWidth / tex->width, iHeight / tex->height, iColor);
}

/**********************************************************************func*
* smf_RenderCharacterString
*
* Renders a specific string to screen, including background.
* Background area explicitly determined by xPos, yPos, width, and height.
*/
static void smf_RenderCharacterString(char *text, float xPos, float yPos, float zPos, float width, float height, float heightOffset, TTDrawContext *pttFont, float fScale, int colorTop, int colorBottom, int colorBG)
{
    int rgba[4];
    rgba[0] = rgba[1] = colorTop;
    rgba[2] = rgba[3] = colorBottom;
    
    if (colorBG & 0xff)
    {
        DrawRect(xPos, yPos, zPos - 0.001, width, height, colorBG);
    }
    if (strlen(text))
    {
        printBasic(pttFont, xPos, yPos + height - heightOffset + pttFont->renderParams.outlineWidth, zPos,
            fScale, fScale, 0, text, strlen(text), rgba);
    }
}

static unsigned int smf_ConvertNormalToSelectedColor(unsigned int normalColor)
{
    unsigned int red, green, blue, alpha, gray, white;
    red = normalColor / (1 << 24);
    green = normalColor / (1 << 16) % (1 << 8);
    blue = normalColor / (1 << 8) % (1 << 8);
    alpha = normalColor % (1 << 8);

    gray = (red + green + blue) / 3;
    white = 256;

    red = red * 2 / 3 + gray * 1 / 3;
    green = green * 2 / 3 + gray * 1 / 3;
    blue = blue * 2 / 3 + gray * 1 / 3;

    red = red * 4 / 5 + white * 1 / 5;
    green = green * 4 / 5 + white * 1 / 5;
    blue = blue * 4 / 5 + white * 1 / 5;

    return red * (1 << 24) + green * (1 << 16) + blue * (1 << 8) + alpha;
}

/**********************************************************************func*
* smf_RenderTextAndWhitespace
*
* Handles all rendering to the screen for a specified SMBlock.
* Assumes that the given SMBlock is either text (k_sm_text) or whitespace (k_sm_ws).
*/
static void smf_RenderTextAndWhitespace(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    SMAnchor *pAnchor = NULL;
    float fScale;
    char *displayText = 0;
    float *characterWidths;
    TTDrawContext *pttFont;
    TTFontRenderParams rp;
    int colorNormal, colorNormal2, colorNormalBG;
    int colorHover, colorHover2, colorHoverBG;
    int colorSelected, colorSelected2, colorSelectedBG;
    bool isHovered;
    int startingBytesAlloced, endingBytesAlloced;

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    characterWidths = smf_GetCharacterWidths(pBlock, pattrs);

    // Although tracked, multiply nested anchors are ignored. 
    // Always use the first anchor if available.
    if(eaiSize(&pattrs->piAnchor) > 1 && s_bAllowAnchor)
    {
        pAnchor = (SMAnchor *)pattrs->piAnchor[1];
    }

    if (TAG_MATCHES(sm_text))
    {
        displayText = (char *)pBlock->pv;
    }

    if (!displayText)
    {
        displayText = "";
    }

    smf_MakeFont(&pttFont, &rp, pattrs);

    fScale = smBlock_CalcFontScale(pBlock, pattrs);

    isHovered = (pAnchor ? pAnchor->bHover : pBlock->bHover);

    /*************************************************
    *               Color Determination              *
    *************************************************/

    if(pAnchor)
    {
        // Nested anchors ARE used, at least in mission descriptions... but they're just used as color tags.
        // Would be nice to fix the nested anchors so this hack wouldn't be needed...
        if(eaiSize(&pattrs->piAnchor)>2)
        {
            colorNormal = 0x80e080ff;
            colorNormal2 = 0x80e080ff;
            
            colorHover = 0x80e080ff;
            colorHover2 = 0x80e080ff;
        }
        else
        {
            colorNormal = pattrs->piColor[eaiSize(&pattrs->piColor) - 1];
            if (pattrs->piColor2[eaiSize(&pattrs->piColor2) - 1])
            {
                colorNormal2 = pattrs->piColor2[eaiSize(&pattrs->piColor2) - 1];
            }
            else
            {
                colorNormal2 = colorNormal;
            }

            colorHover = pattrs->piLinkHover[eaiSize(&pattrs->piLinkHover) - 1];
            colorHover2 = colorHover;
        }
        colorNormalBG = pattrs->piLinkBG[eaiSize(&pattrs->piLinkBG) - 1];
        if (smf_hasDisplayedHoverBackgroundThisFrame)
        {
            // This is a hack to make it not display the alpha'd bgcolor twice in the mission accept dialog.
            colorHoverBG = 0;
        }
        else
        {
            colorHoverBG = pattrs->piLinkHoverBG[eaiSize(&pattrs->piLinkHoverBG) - 1];
        }

        colorSelected = pattrs->piLinkSelect[eaiSize(&pattrs->piLinkSelect) - 1];
        colorSelected2 = colorSelected;
        if (!colorSelected || colorSelected == 0x000000ff)
        {
            colorSelected = (int) smf_ConvertNormalToSelectedColor((unsigned int) colorNormal);
            colorSelected2 = (int) smf_ConvertNormalToSelectedColor((unsigned int) colorNormal2);
        }
        colorSelectedBG = pattrs->piLinkSelectBG[eaiSize(&pattrs->piLinkSelectBG) - 1];
    }
    else
    {
        colorNormal = pattrs->piColor[eaiSize(&pattrs->piColor) - 1];
        if (pattrs->piColor2[eaiSize(&pattrs->piColor2) - 1])
        {
            colorNormal2 = pattrs->piColor2[eaiSize(&pattrs->piColor2) - 1];
        }
        else
        {
            colorNormal2 = colorNormal;
        }
        colorNormalBG = 0; // pattrs->piColorBG is unimplemented.

        colorHover = colorNormal; // NOTE: hover coloring is disabled for standard text
        colorHover2 = colorNormal2; // Change these two lines to enable it again
        colorHoverBG = 0; // pattrs->piColorHoverBG is unimplemented.

        colorSelected = pattrs->piColorSelect[eaiSize(&pattrs->piColorSelect) - 1];
        colorSelected2 = colorSelected;
        if (!colorSelected || colorSelected == 0x000000ff)
        {
            colorSelected = (int) smf_ConvertNormalToSelectedColor((unsigned int) colorNormal);
            colorSelected2 = (int) smf_ConvertNormalToSelectedColor((unsigned int) colorNormal2);
        }
        colorSelectedBG = pattrs->piColorSelectBG[eaiSize(&pattrs->piColorSelectBG) - 1];
    }

    /****************************************************
    * Print State-Dependent Text Blocks and Backgrounds *
    ****************************************************/

    if (pBlock->bSelected) // implies smf_IsBlockSelectable() due to functionality in smf_Select().
    {
        if (TAG_MATCHES(sm_ws))
        {
            smf_RenderCharacterString(displayText, pBlock->pos.iX + iXBase, pBlock->pos.iY + iYBase, iZBase, 
                pBlock->pos.iWidth, pBlock->pos.iHeight, pBlock->pos.iMinHeight / 10, 
                pttFont, fScale, colorSelected, colorSelected2, colorSelectedBG);
        }
        else
        {
            unsigned int displayLength = smBlock_GetDisplayLength(pBlock);
            unsigned int pBlockEarlyDisplayIndex = pBlock->displayStringStartIndex;
            unsigned int pBlockLateDisplayIndex = pBlock->displayStringStartIndex + displayLength;
            
            // The selection indexes are the spatially-ordered edges of the entire selection,
            // before those values are constrained to fit within the SMBlock.
            unsigned int selectionEarlyDisplayIndex, selectionLateDisplayIndex;
            SMFBlock *selectionEarlyPane, *selectionLatePane;

            // The final indexes are the actual values that denote the edges of the
            // rendered selection within this SMBlock.
            unsigned int finalEarlyDisplayIndex, finalLateDisplayIndex;
            
            unsigned int prePiece_ByteLength = 0;
            unsigned int selPiece_ByteLength = 0;
            unsigned int postPiece_ByteLength = 0;
            unsigned int prePiece_DisplayLength = 0;
            unsigned int selPiece_DisplayLength = 0;
            unsigned int postPiece_DisplayLength = 0;
            float prePiece_DisplayWidth = 0;
            float selPiece_DisplayWidth = 0;
            float postPiece_DisplayWidth = 0;

            unsigned int index;

            if (smf_IsSelectionStartBeforeSelectionEnd())
            {
                selectionEarlyDisplayIndex = smf_selectionStartDisplayIndex;
                selectionLateDisplayIndex = smf_selectionEndDisplayIndex;
                selectionEarlyPane = smf_selectionStartPane;
                selectionLatePane = smf_selectionEndPane;
            }
            else
            {
                selectionEarlyDisplayIndex = smf_selectionEndDisplayIndex;
                selectionLateDisplayIndex = smf_selectionStartDisplayIndex;
                selectionEarlyPane = smf_selectionEndPane;
                selectionLatePane = smf_selectionStartPane;
            }

            if (selectionEarlyPane == pBlock->pSMFBlock)
            {
                finalEarlyDisplayIndex = max(selectionEarlyDisplayIndex, pBlockEarlyDisplayIndex);
            }
            else
            {
                finalEarlyDisplayIndex = pBlockEarlyDisplayIndex;
            }
            finalEarlyDisplayIndex = min(finalEarlyDisplayIndex, pBlockLateDisplayIndex);

            if (selectionLatePane == pBlock->pSMFBlock)
            {
                finalLateDisplayIndex = min(selectionLateDisplayIndex, pBlockLateDisplayIndex);
            }
            else
            {
                finalLateDisplayIndex = pBlockLateDisplayIndex;
            }
            finalLateDisplayIndex = max(finalLateDisplayIndex, pBlockEarlyDisplayIndex);
        
            // Output pre-selected piece of the SMBlock, if it exists.
            if (pBlockEarlyDisplayIndex < finalEarlyDisplayIndex)
            {
                char *prePiece;
                unsigned int displayIndex;
                
                prePiece_DisplayLength = finalEarlyDisplayIndex - pBlockEarlyDisplayIndex;
                prePiece_DisplayWidth = 0;                
                estrCreate(&prePiece);

                for (displayIndex = 0; displayIndex < prePiece_DisplayLength; displayIndex++)
                {
                    unsigned int characterByteCount = UTF8GetCharacterLength(displayText + prePiece_ByteLength);
                    estrConcatFixedWidth(&prePiece, displayText + prePiece_ByteLength, characterByteCount);
                    prePiece_ByteLength += characterByteCount;
                    if( characterWidths && (int)displayIndex < eafSize(&characterWidths) )
                        prePiece_DisplayWidth += characterWidths[displayIndex];
                }

                smf_RenderCharacterString(prePiece, pBlock->pos.iX + iXBase, pBlock->pos.iY + iYBase, iZBase, 
                    prePiece_DisplayWidth, pBlock->pos.iHeight, pBlock->pos.iMinHeight / 10, 
                    pttFont, fScale, (isHovered ? colorHover : colorNormal), 
                    (isHovered ? colorHover2 : colorNormal2), (isHovered ? colorHoverBG : colorNormalBG));

                estrDestroy(&prePiece);
            }

            // Output selected piece of the SMBlock, if it exists.
            // This wouldn't exist if the cursor is in this SMBlock but nothing is selected.
            if (finalEarlyDisplayIndex < finalLateDisplayIndex)
            {
                char *selPiece;
                unsigned int displayIndex;
                
                selPiece_DisplayLength = finalLateDisplayIndex - finalEarlyDisplayIndex;
                selPiece_DisplayWidth = 0;                
                estrCreate(&selPiece);
                
                for (displayIndex = 0; displayIndex < selPiece_DisplayLength; displayIndex++)
                {
                    unsigned int characterByteCount = UTF8GetCharacterLength(displayText + prePiece_ByteLength + selPiece_ByteLength);
                    estrConcatFixedWidth(&selPiece, displayText + prePiece_ByteLength + selPiece_ByteLength, characterByteCount);
                    selPiece_ByteLength += characterByteCount;
                    if( characterWidths && (int)(prePiece_DisplayLength + displayIndex) < eafSize(&characterWidths)  )
                        selPiece_DisplayWidth += characterWidths[prePiece_DisplayLength + displayIndex];
                }

                // Fix for both-justified words.
                if (finalLateDisplayIndex == pBlockLateDisplayIndex)
                {
                    if (characterWidths)
                    {
                        unsigned int totalCharWidth = 0;
                        for (index = 0; index < displayLength && (int)index < eafSize(&characterWidths) ; index++)
                        {
                            totalCharWidth += characterWidths[index];
                        }
                        selPiece_DisplayWidth += (pBlock->pos.iWidth - totalCharWidth);
                    }
                    else
                    {
                        if (finalEarlyDisplayIndex == pBlockEarlyDisplayIndex)
                        {
                            selPiece_DisplayWidth = pBlock->pos.iWidth;
                        }
                    }
                }

                smf_RenderCharacterString(selPiece, pBlock->pos.iX + iXBase + prePiece_DisplayWidth, pBlock->pos.iY + iYBase, iZBase, 
                    selPiece_DisplayWidth, pBlock->pos.iHeight, pBlock->pos.iMinHeight / 10, 
                    pttFont, fScale, colorSelected, colorSelected2, colorSelectedBG);

                estrDestroy(&selPiece);
            }

            // Output post-selected piece of the SMBlock, if it exists.
            if (finalLateDisplayIndex < pBlockLateDisplayIndex)
            {
                char *postPiece;
                unsigned int displayIndex;
                
                postPiece_DisplayLength = pBlockLateDisplayIndex - finalLateDisplayIndex;
                postPiece_DisplayWidth = 0;                
                estrCreate(&postPiece);

                for (displayIndex = 0; displayIndex < postPiece_DisplayLength; displayIndex++)
                {
                    unsigned int characterByteCount = UTF8GetCharacterLength(displayText + prePiece_ByteLength + selPiece_ByteLength + postPiece_ByteLength);
                    estrConcatFixedWidth(&postPiece, displayText + prePiece_ByteLength + selPiece_ByteLength + postPiece_ByteLength, characterByteCount);
                    postPiece_ByteLength += characterByteCount;
                    if( characterWidths && (int)(prePiece_DisplayLength + selPiece_DisplayLength + displayIndex) < eafSize(&characterWidths) )
                        postPiece_DisplayWidth += characterWidths[prePiece_DisplayLength + selPiece_DisplayLength + displayIndex];
                }

                smf_RenderCharacterString(postPiece, pBlock->pos.iX + iXBase + prePiece_DisplayWidth + selPiece_DisplayWidth, pBlock->pos.iY + iYBase, iZBase, 
                    postPiece_DisplayWidth, pBlock->pos.iHeight, pBlock->pos.iMinHeight / 10, 
                    pttFont, fScale, (isHovered ? colorHover : colorNormal), 
                    (isHovered ? colorHover2 : colorNormal2), (isHovered ? colorHoverBG : colorNormalBG));    

                estrDestroy(&postPiece);
            }
        }
    }
    else
    {
        /*************************************************
        *             Completely unselected              *
        *************************************************/

        smf_RenderCharacterString(displayText, pBlock->pos.iX + iXBase, pBlock->pos.iY + iYBase, iZBase, 
            pBlock->pos.iWidth, pBlock->pos.iHeight, pBlock->pos.iMinHeight / 10, 
            pttFont, fScale, (isHovered ? colorHover : colorNormal), 
            (isHovered ? colorHover2 : colorNormal2), (isHovered ? colorHoverBG : colorNormalBG));
    }

    if (smBlock_HasFocus(pBlock) && smfCursor_display &&
        (smf_selectionEndDisplayIndex == pBlock->displayStringStartIndex || characterWidths))
    {
        /*************************************************
        *                Draw the cursor                 *
        *************************************************/

        unsigned int letterIndex = smf_selectionEndDisplayIndex - pBlock->displayStringStartIndex;
        float letterXOffset = 0;
        unsigned int index;

        if(characterWidths)
        {
            for (index = 0; index < letterIndex && (int)index < eafSize(&characterWidths); index++)
            {
                letterXOffset += characterWidths[index];
            }
        }

        DrawRect(pBlock->pos.iX + iXBase + letterXOffset, pBlock->pos.iY + iYBase, iZBase, 
                    1, pBlock->pos.iHeight, 0xffffffff);
    }

    eafDestroy(&characterWidths);

    // Reset the font we used.
    pttFont->renderParams = rp;

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }
}

/**********************************************************************func*
* smf_RenderImage
*
* Handles all rendering to the screen for a specified SMBlock.
* Assumes that the specified SMBlock is an image (k_sm_image).
*/
static void smf_RenderImage(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    SMAnchor *pAnchor = NULL;
    SMImage *pimg = (SMImage *)pBlock->pv;
    char *pchTexName = pimg->achTex;
    float fXScale;
    float fYScale;
    AtlasTex *ptex = NULL;
    int startingBytesAlloced, endingBytesAlloced;

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    // Although tracked, multiply nested anchors are ignored. Always use
    // the first anchor if available.
    if(eaiSize(&pattrs->piAnchor) > 1 && s_bAllowAnchor)
    {
        pAnchor = (SMAnchor *)pattrs->piAnchor[1];
    }

    if(pAnchor)
    {
        if(pAnchor->bHover)
        {
            if(pimg->iHighlight>0)
            {
                AtlasTex *ptexWhite = white_tex_atlas;
                int iColor = pattrs->piLinkHover[eaiSize(&pattrs->piLinkHover)-1];

                fXScale = (float)(pimg->iWidth+pimg->iHighlight*2)/(float)ptexWhite->width;
                fYScale = (float)(pimg->iHeight+pimg->iHighlight*2)/(float)ptexWhite->height;

                if(pBlock->pSMFBlock && pBlock->pSMFBlock->masterScale)
                {
                    fXScale *= pBlock->pSMFBlock->masterScale;
                    fYScale *= pBlock->pSMFBlock->masterScale;
                }

                display_sprite(ptexWhite,
                    pBlock->pos.iX + pBlock->pos.iBorder - pimg->iHighlight + iXBase,
                    pBlock->pos.iY + pBlock->pos.iBorder - pimg->iHighlight + iYBase,
                    iZBase,
                    fXScale, fYScale, iColor);
            }

            if(pimg->achTexHover[0]!='\0')
            {
                pchTexName = pimg->achTexHover;
            }
        }
    }

    if(strnicmp("pow:", pchTexName, 4)==0)
    {
        int iLevel;
        const BasePower *ppowBase = powerdict_GetBasePowerByString(&g_PowerDictionary, pchTexName+4, &iLevel);
        if(ppowBase->eType == kPowerType_Boost)
        {
            uiEnhancement *pEnh = NULL;
            fXScale = (float)pimg->iWidth/(float)48;
            fYScale = (float)pimg->iHeight/(float)48;

            if(pBlock->pSMFBlock && pBlock->pSMFBlock->masterScale)
            {
                fXScale *= pBlock->pSMFBlock->masterScale;
                fYScale *= pBlock->pSMFBlock->masterScale;
            }

            pEnh = uiEnhancementCreate(ppowBase->pchFullName, iLevel);
            uiEnhancementDraw(pEnh, pBlock->pos.iX + pBlock->pos.iBorder + pimg->iWidth/2 + iXBase,
                pBlock->pos.iY + pBlock->pos.iBorder + pimg->iHeight/2 + iYBase,
                iZBase,
                fXScale,
                fXScale,
                0, 0, true);
            uiEnhancementFree(&pEnh);
        }
        else
        {
            ptex = atlasLoadTexture(ppowBase->pchIconName);
        }
    }
    else if(strnicmp("rec:", pchTexName, 4)==0)
    {
        const DetailRecipe *pRec = detailrecipedict_RecipeFromName(pchTexName+4);

        if (pRec) 
        {
            fXScale = (float)pimg->iWidth/(float) 96;

            if(pBlock->pSMFBlock && pBlock->pSMFBlock->masterScale)
            {
                fXScale *= pBlock->pSMFBlock->masterScale;
            }

            uiRecipeDrawIcon(pRec, pRec->level,                
                pBlock->pos.iX + pBlock->pos.iBorder/5.0f + iXBase,
                pBlock->pos.iY + pBlock->pos.iBorder/5.0f + iYBase,
                iZBase, fXScale*1.31f, 0, true, CLR_WHITE);

        }

    }
    else if(strnicmp("npc:", pchTexName, 4)==0 )
    {
        int idx = atoi(pchTexName+4);
        if(idx>=0)
        {
            const cCostume *pCostume = npcDefsGetCostume(idx, 0);
            if(pCostume!=NULL)
            {
                ptex = seqGetHeadshot(pCostume, 0);
            }
        }
    }
    else if(strnicmp("svr:", pchTexName, 4)==0)
    {
        int idx = atoi(pchTexName+4);
        if(idx>=0)
        {
            Entity * e = entFromId(idx);
            if( e && e->costume )
            {
                ptex =seqGetHeadshot(e->costume, 0);
            }
        }
    }
    else if(strnicmp("costume:", pchTexName, 8)==0)
    {
        const NPCDef *npcDef = npcFindByName(pchTexName+8, NULL);
        if (npcDef)
        {
            const cCostume *pCostume = eaGetConst(&npcDef->costumes, 0);

            if(pCostume!=NULL)
            {
                ptex = seqGetHeadshot(pCostume, 0);
            }
        }
    }
    else if(strnicmp("picturebrowser:", pchTexName, strlen("picturebrowser:"))==0) 
    { 
        char *data = pimg->achTex+strlen("picturebrowser:");
        PictureBrowser *pb;
        int idx;

        idx = atoi(data);
        pb = picturebrowser_getById(idx);
        if(pb)
        {
            picturebrowser_setLoc( pb, pBlock->pos.iX + pBlock->pos.iBorder + iXBase , 
                pBlock->pos.iY + pBlock->pos.iBorder + iYBase, iZBase+1 );
            picturebrowser_display( pb );
        }

        ptex = NULL;
    }
    else if(strnicmp("empty:", pchTexName, 6)==0)
    {
        char *stringPos;
        char *data;
        estrCreate(&stringPos);
        estrConcatCharString(&stringPos, pimg->achTex + 6);

        data = strtok(stringPos, "_");
        if(data)
        {
            pimg->iWidth = atoi(data);
            data = strtok(NULL, "_");
            if(data)
                pimg->iHeight = atoi(data);
        }

        fXScale = fYScale = 1;

        ptex = NULL;
        estrDestroy(&stringPos);
    }
    else if(strnicmp("mmframe:", pchTexName, 8)==0)
    {
        int idx = atoi(pchTexName+8);
        if(idx>=0)
        {
            const cCostume* pCostume = npcDefsGetCostume(idx, 0);
            if(pCostume!=NULL)
            {
                ptex = seqGetMMShot( pCostume, 0, PICTURETYPE_BODYSHOT );
            }
            if(ptex!=NULL)
            {
                pimg->iWidth = ptex->width;
                pimg->iHeight = ptex->height;
            }
        }
    }
    else if(strnicmp("mmmap:", pchTexName, 6)==0)
    {        
        ptex = atlasLoadTexture(pchTexName + 6);
    }
    else
    {
        ptex = atlasLoadTexture(pchTexName);
    }

    if(ptex!=NULL)
    {
        int scr_scaled_disabled = is_scrn_scaling_disabled();
        int scalingXOffSet = 0;
        fXScale = (float)pimg->iWidth/(float)ptex->width;
        fYScale = (float)pimg->iHeight/(float)ptex->height;

//        if(pBlock->pSMFBlock && pBlock->pSMFBlock->masterScale)
//        {
//            fXScale *= pBlock->pSMFBlock->masterScale;
//            fYScale *= pBlock->pSMFBlock->masterScale;
//        }

        if (!scr_scaled_disabled && !isMenu(MENU_GAME))
        {
            int screenW, screenH;
            float screenScaleX, screenScaleY;
            float screenScale;
            windowClientSize(&screenW, &screenH);
            screenScaleX = (float)screenW / DEFAULT_SCRN_WD;
            screenScaleY = (float)screenH / DEFAULT_SCRN_HT;
            screenScale =MIN(screenScaleX, screenScaleY);
            fXScale *= screenScale/screenScaleX;
            fYScale *= screenScale/screenScaleY;
            scalingXOffSet = pimg->iWidth*(1.f-fXScale)/2;
        }
        display_sprite(ptex,
            pBlock->pos.iX + pBlock->pos.iBorder + iXBase + scalingXOffSet,
            pBlock->pos.iY + pBlock->pos.iBorder + iYBase,
            iZBase,
            fXScale, fYScale, pimg->iColor);
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }
}

/**********************************************************************func*
* smf_RenderTableRow
*
* Handles all rendering to the screen for a specified SMBlock.
* Assumes that the specified SMBlock is a table row (k_sm_tr).
*/
static void smf_RenderTableRow(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    SMAnchor *pAnchor = NULL;
    SMRow *prow = (SMRow *)pBlock->pv;
    int startingBytesAlloced, endingBytesAlloced;

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    // Although tracked, multiply nested anchors are ignored. Always use
    // the first anchor if available.
    if(eaiSize(&pattrs->piAnchor) > 1 && s_bAllowAnchor)
    {
        pAnchor = (SMAnchor *)pattrs->piAnchor[1];
    }

    if(prow->iHighlight)
    {
        int iColor = 0;

        if(pAnchor && pAnchor->bHover)
        {
            iColor = pattrs->piLinkHoverBG[eaiSize(&pattrs->piLinkHoverBG)-1];
        }
        else if(pAnchor && prow->iSelected)
        {
            int iAlpha;

            iColor = pattrs->piLinkHoverBG[eaiSize(&pattrs->piLinkHoverBG)-1];
            iAlpha = (iColor&0xff)/2;
            //iColor = iColor&~0xff | iAlpha;
        }
        else
        {
            iColor = pattrs->piLinkBG[eaiSize(&pattrs->piLinkBG)-1];
        }

        if(iColor&0xff)
        {
            smf_hasDisplayedHoverBackgroundThisFrame = true;

            if (prow->iStyle == 1)
            {
                DrawGradientRect(pBlock->pos.iX + iXBase,
                    pBlock->pos.iY + iYBase - SM_BLOAT, iZBase,
                    pBlock->pos.iWidth, pBlock->pos.iHeight+SM_BLOAT*2,
                    iColor);
            }
            else
            {
                DrawRoundedRect(pBlock->pos.iX + iXBase,
                    pBlock->pos.iY + iYBase - SM_BLOAT, iZBase,
                    pBlock->pos.iWidth, pBlock->pos.iHeight+SM_BLOAT*2,
                    iColor);
            }
        }
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }
}


/**********************************************************************func*
* smf_RenderTableDataAndSpan
*
* Handles all rendering to the screen for a specified SMBlock.
* Assumes that the specified SMBlock is a table data (k_sm_td) or span (k_sm_span).
*/
static void smf_RenderTableDataAndSpan(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    SMAnchor *pAnchor = NULL;
    SMColumn *pcol = (SMColumn *)pBlock->pv;
    int startingBytesAlloced, endingBytesAlloced;

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    // Although tracked, multiply nested anchors are ignored. Always use
    // the first anchor if available.
    if(eaiSize(&pattrs->piAnchor) > 1 && s_bAllowAnchor)
    {
        pAnchor = (SMAnchor *)pattrs->piAnchor[1];
    }

    if(pBlock->bgcolor && pBlock->bgcolor != 0x000000ff )
    {
        AtlasTex *ptexWhite = white_tex_atlas;
        F32 fXScale = (float)(pBlock->pos.iWidth-pBlock->pos.iBorder*2)/(float)ptexWhite->width;
        F32 fYScale = (float)(pBlock->pos.iHeight-pBlock->pos.iBorder*2)/(float)ptexWhite->height;

        display_sprite(ptexWhite,
            pBlock->pos.iX + pBlock->pos.iBorder + iXBase,
            pBlock->pos.iY + pBlock->pos.iBorder + iYBase,
            iZBase,
            fXScale, fYScale, pBlock->bgcolor&0xffffff88);
    }
    if(pcol->iHighlight)
    {
        int iColor = 0;

        if(pAnchor && pAnchor->bHover)
        {
            iColor = pattrs->piLinkHoverBG[eaiSize(&pattrs->piLinkHoverBG)-1];
        }
        else if(pAnchor && pcol->iSelected)
        {
            int iAlpha;

            iColor = pattrs->piLinkHoverBG[eaiSize(&pattrs->piLinkHoverBG)-1];
            iAlpha = (iColor&0xff)/2;
            iColor = iColor&~0xff | iAlpha;
        }
        else
        {
            iColor = pattrs->piLinkBG[eaiSize(&pattrs->piLinkBG)-1];
        }

        if(iColor&0xff && TAG_MATCHES(sm_td))
        {
            smf_hasDisplayedHoverBackgroundThisFrame = true;
            DrawRoundedRect(pBlock->pos.iX + iXBase,
                pBlock->pos.iY + iYBase - SM_BLOAT,
                iZBase,
                pBlock->pos.iWidth, pBlock->pos.iHeight+SM_BLOAT*2,
                iColor);
        }
        else if(iColor&0xff && TAG_MATCHES(sm_span))
        {
            smf_hasDisplayedHoverBackgroundThisFrame = true;
            DrawRoundedRect(pBlock->pos.iX + iXBase-2,
                pBlock->pos.iY + iYBase+3,
                iZBase,
                pBlock->pos.iWidth, pBlock->pos.iHeight,
                iColor);
        }
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }
}

/**********************************************************************func*
 * smf_Render
 *
 * Handles all rendering to the screen for a specified SMBlock and its children.
 */
void smf_Render(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    int startingBytesAlloced, endingBytesAlloced;

    if ((pBlock->pos.iWidth == 0 || pBlock->pos.iX < 0 || pBlock->pos.iY < 0)
            && pBlock->iType > kFormatTags_Count * 2)
    {
        return;
    }

    PERFINFO_AUTO_START("smf_Render", 1);

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    if(pBlock->iType == -1) // Top level block
    {
        // Render nothing
    }
    else if(smf_ApplyFormatting(pBlock, pattrs)) // Should be before the rest.
    {        
        // Render nothing
    }
    else if(TAG_MATCHES(sm_nolink))
    {
        s_bAllowAnchor = false;
    }
    else if(TAG_MATCHES(sm_nolink_end))
    {
        s_bAllowAnchor = true;
    }
    else if(TAG_MATCHES(sm_text) || TAG_MATCHES(sm_ws))
    {
        smf_RenderTextAndWhitespace(pBlock, pattrs, iXBase, iYBase, iZBase);
    }
    else if(TAG_MATCHES(sm_table))
    {
        // Render nothing
    }
    else if(TAG_MATCHES(sm_br))
    {
        // Render nothing
    }
    else if(TAG_MATCHES(sm_image))
    {
        smf_RenderImage(pBlock, pattrs, iXBase, iYBase, iZBase);
    }
    else if(TAG_MATCHES(sm_tr))
    {
        smf_RenderTableRow(pBlock, pattrs, iXBase, iYBase, iZBase);
    }
    else if(TAG_MATCHES(sm_td) || TAG_MATCHES(sm_span))
    {
        smf_RenderTableDataAndSpan(pBlock, pattrs, iXBase, iYBase, iZBase);
    }
    else
    {
        // Render nothing
    }

    if(pBlock->bHasBlocks)
    {
        int i;
        int iSize = eaSize(&pBlock->ppBlocks);

        for(i = 0; i < iSize; i++)
        {
            smf_Render(pBlock->ppBlocks[i], pattrs, iXBase, iYBase, iZBase);
        }
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    PERFINFO_AUTO_STOP();
}