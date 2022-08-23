/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
//#define DEBUG

#ifdef DEBUG
#include <stdio.h>  // printf
#endif

#include <string.h> // strlen
#include <math.h>

#include "formatter/smf_util.h"
#include "formatter/smf_format.h"

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>

#include "graphics/truetype/ttFontDraw.h"
#include "graphics/ttFontUtil.h"
#include "entity/powers.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/StringUtil.h>
#include "entity/entclient.h"

#include <utilitieslib/language/AppLocale.h>
//to get the mm texture right
#include "entity/costume.h"
#include "gameComm/Npc.h"
#include "graphics/seqgraphics.h"
#include "UI/uiPictureBrowser.h"
#include "entity/entity.h"

#ifdef DEBUG
#define DBG_PRINTF(x) printf x
#else
#define DBG_PRINTF(x)
#endif


/**********************************************************************func*
 * FontSize
 *
 */
void FontSize(TTDrawContext *pttFont, float fScale, char *pch, float *piWidth, float *piHeight)
{
    float fXScreenScale = 1.0f;
    float fYScreenScale = 1.0f;

    *piHeight = ttGetFontHeight(pttFont, fScale, fScale);
    *piWidth = pch ? ttGetStringWidthNarrow(pttFont, fScale, fScale, pch) : 0;

    getToScreenScalingFactor(&fXScreenScale, &fYScreenScale);
    *piHeight = (int) ceil(*piHeight/fYScreenScale);    //We don't draw on non-pixel boundaries
    *piWidth = (int) ceil(*piWidth/fXScreenScale);        
    if (*piHeight < 18 && (getCurrentLocale() == 3)) {
        *piHeight = 18;
    }
}

#define WIDE_TEXT_SPLIT_FACTOR    5

void SplitWideTextBlock(SMBlock *pBlock, TTDrawContext *pttFont, float fScale, int iWidth)
{
    int maxPieceWidth;
    SMBlock *pCurrBlock = pBlock;
    int parentIndex = -1;
    const char *origStr = (const char *)(pBlock->pv);
    const char *origPos = origStr;
    float charWidth = 0.0f;
    float charHeight = 0.0f;
    char charStr[7];
    char *tempStr = NULL;

    // We need at least a little space and some text to go in it. We also
    // avoid splitting really tiny text because that prevents us from
    // shattering strings in windows as they expand/contract.
    if (iWidth <= 0 || fScale <= 0.5f || !pBlock || !pBlock->pv || !pBlock->pSMFBlock)
    {
        return;
    }

    maxPieceWidth = iWidth / WIDE_TEXT_SPLIT_FACTOR;

    estrCreate(&tempStr);

    // Figure out where our blocks are to be (ie. start where the first was
    // and add them sequentially thereafter).
    parentIndex = eaFind(&(pBlock->pParent->ppBlocks), pBlock);

    // We always add a finished block to the list, so this existing block
    // has to be removed or we'll point to it twice.
    eaRemove(&(pBlock->pParent->ppBlocks), parentIndex);

    // These will be updated as we add characters back.
    pBlock->pos.iMinWidth = 0.0f;
    pBlock->pos.iMinHeight = 0.0f;

    // Work through the whole chunk of display text adding it to the block.
    // Whenever we reach a character that won't fit the current block we
    // finish off that block by setting its string and adding it to the
    // parent's list. We then create a new block to follow it and prime it
    // with the start positions and so on.
    while (*origPos)
    {
        int charLen = 0;

        // We want to measure the width of the next UTF8 character, which
        // could be from 1 to 6 bytes of data. Since it's used as a string
        // in some places it must be NULL-terminated.
        memset(charStr, 0, 7);
        charLen = UTF8GetCharacterLength(origPos);
        memcpy(charStr, origPos, charLen);
        FontSize(pttFont, fScale, charStr, &charWidth, &charHeight);

        if (charHeight > pCurrBlock->pos.iMinHeight)
        {
            pCurrBlock->pos.iMinHeight = charHeight;
        }

        // Finish up the block if the character won't fit.
        if (pCurrBlock->pos.iMinWidth > 0.0f && (pCurrBlock->pos.iMinWidth + charWidth > maxPieceWidth))
        {
            // Copy the gathered-up text for this piece and store it in
            // the block.
            pCurrBlock->pv = malloc(strlen(tempStr) + 1);
            strcpy(pCurrBlock->pv, tempStr);
            estrClear(&tempStr);

            // Put the block into the correct place in the parent's list.
            // The next block will go immediately after this one.
            eaInsert(&(pBlock->pParent->ppBlocks), pCurrBlock, parentIndex);
            parentIndex++;

            // The current block is finished and stored in the list so we
            // need to start a new one to replace it.
            pCurrBlock = sm_CreateBlock();
            memcpy(pCurrBlock, pBlock, sizeof(SMBlock));
            pCurrBlock->pv = NULL;
            pCurrBlock->ppBlocks = NULL;

            // Raw string index works off the display index, so we need
            // to update it before changing the display string index.
            pCurrBlock->rawStringStartIndex = smBlock_ConvertRelativeDisplayIndexToRelativeRawIndex(pCurrBlock, origPos - origStr);
            pCurrBlock->displayStringStartIndex += (origPos - origStr);

            pCurrBlock->pos.iMinWidth = 0.0f;
            pCurrBlock->pos.iMinHeight = 0.0f;
        }

        pCurrBlock->pos.iMinWidth += charWidth;

        estrConcatFixedWidth(&tempStr, charStr, charLen);
        origPos += charLen;
    }

    // Finish up the last block. All it needs is its text and placement in
    // the parent's list. All the rest was set when it was created.
    pCurrBlock->pv = malloc(strlen(tempStr) + 1);
    strcpy(pCurrBlock->pv, tempStr);

    eaInsert(&(pBlock->pParent->ppBlocks), pCurrBlock, parentIndex);

    estrDestroy(&tempStr);
    free((void *)origStr);
}

/**********************************************************************func*
 * CalcTextSize
 *
 */
void CalcTextSize(SMBlock *pBlock, TextAttribs *pattrs, int iWidth)
{
    TTDrawContext *pttFont;
    TTFontRenderParams rp;
    float fScale;

    // Set the font up
    smf_MakeFont(&pttFont, &rp, pattrs);

    fScale = smBlock_CalcFontScale(pBlock, pattrs);
    FontSize(pttFont, fScale,
        (char *)pBlock->pv,
        &pBlock->pos.iMinWidth, &pBlock->pos.iMinHeight);

    // The splitting works but there's a race condition with the architect
    // UI, so this is turned off for now.
#if 0
    if (pBlock->pos.iMinWidth > iWidth)
    {
        SplitWideTextBlock(pBlock, pttFont, fScale, iWidth);
    }
#endif

    // Reset the font we used.
    pttFont->renderParams = rp;
}

/**********************************************************************func*
 * CalcSpaceSize
 *
 */
void CalcSpaceSize(SMBlock *pBlock, TextAttribs *pattrs)
{
    TTDrawContext *pttFont;
    TTFontRenderParams rp;
    float fScale;

    // Set the font up
    smf_MakeFont(&pttFont, &rp, pattrs);

    fScale = smBlock_CalcFontScale(pBlock, pattrs);
    FontSize(pttFont, fScale," ", &pBlock->pos.iMinWidth, &pBlock->pos.iMinHeight);

    // Reset the font we used.
    pttFont->renderParams = rp;

    if (pBlock->pos.iMinWidth < 3)
    {
        pBlock->pos.iMinWidth = 3;
    }
}

/**********************************************************************func*
 * GetMinimumSize
 *
 */
static void GetMinimumSize(SMBlock *pBlock, TextAttribs *pattrs, int iWidth)
{
    int iChildMaxWidth = 0;
    int iChildMaxHeight = 0;
    int iChildSumWidth = 0;
    int iChildSumHeight = 0;

    if(pBlock->bHasBlocks)
    {
        int i;
        for(i=0; i<eaSize(&pBlock->ppBlocks); i++)
        {
            GetMinimumSize(pBlock->ppBlocks[i], pattrs, iWidth);

            if(iChildMaxWidth < pBlock->ppBlocks[i]->pos.iMinWidth)
            {
                iChildMaxWidth = pBlock->ppBlocks[i]->pos.iMinWidth;
            }
            if(iChildMaxHeight < pBlock->ppBlocks[i]->pos.iMinHeight)
            {
                iChildMaxHeight = pBlock->ppBlocks[i]->pos.iMinHeight;
            }

            iChildSumWidth += pBlock->ppBlocks[i]->pos.iMinWidth;
            iChildSumHeight += pBlock->ppBlocks[i]->pos.iMinHeight;
        }
    }

    if(smf_ApplyFormatting(pBlock, pattrs))
    {
        // Must be first
        // Nothing more to do
    }
    else if(TAG_MATCHES(sm_text))
    {
        CalcTextSize(pBlock, pattrs, iWidth);
    }
    else if(TAG_MATCHES(sm_ws))
    {
        CalcSpaceSize(pBlock, pattrs);
    }
    else if(TAG_MATCHES(sm_br))
    {
        CalcSpaceSize(pBlock, pattrs);
        pBlock->pos.iMinWidth = 0;
    }
    else if(TAG_MATCHES(sm_table))
    {
        pBlock->pos.iMinWidth = iChildMaxWidth + pBlock->pos.iBorder*2;
        pBlock->pos.iMinHeight = iChildSumHeight + pBlock->pos.iBorder*2;
    }
    else if(TAG_MATCHES(sm_tr))
    {
        pBlock->pos.iMinWidth = iChildSumWidth + pBlock->pos.iBorder*2;
        pBlock->pos.iMinHeight = iChildMaxHeight + pBlock->pos.iBorder*2;
    }
    else if(TAG_MATCHES(sm_image))
    {
        SMImage *pimg = (SMImage *)pBlock->pv;
        int iWidth = 0;
        int iHeight = 0;
        AtlasTex *ptex;

        if(strnicmp("pow:", pimg->achTex, 4)==0)
        {
            const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pimg->achTex+4);
            if(ppowBase->eType == kPowerType_Boost)
            {
                iWidth = 48;
                iHeight = 48;
            }
            else
            {
                iWidth = 32;
                iHeight = 32;
            }
        }
        else if(strnicmp("rec:", pimg->achTex, 4)==0)
        {
            iWidth = 96; 
            iHeight = 64;
        }
        else if(strnicmp("npc:", pimg->achTex, 4)==0)
        {
            char *data = pimg->achTex+strlen("npc:");
            int idx = atoi(data);
            if(idx>=0)
            {
                const cCostume *pCostume = npcDefsGetCostume(idx, 0);
                if(pCostume!=NULL && (ptex =seqGetHeadshot( pCostume, 0))!=NULL)
                {
                    iWidth = 128;
                    iHeight = 128;

                    if(ptex)
                    {
                        int widthIsSmaller = (ptex->width<ptex->height);
                        F32 scale = widthIsSmaller?((F32)iHeight)/((F32)ptex->height): ((F32)iWidth)/((F32)ptex->width);
                        iWidth = scale*ptex->width;
                        iHeight = scale*ptex->height;
                    }
                }
            }
        }
        else if(strnicmp("svr:", pimg->achTex, 4)==0)
        {
            char *data = pimg->achTex+strlen("svr:"); 
            int idx = atoi(data);
            if(idx>=0)
            {
                Entity * e = entFromId(idx);
                if( e && e->costume )
                {
                    if((ptex =seqGetHeadshot(e->costume, 0))!=NULL)
                    {
                        iWidth = 128;
                        iHeight = 128;

                        if(ptex)
                        {
                            int widthIsSmaller = (ptex->width<ptex->height);
                            F32 scale = widthIsSmaller?((F32)iHeight)/((F32)ptex->height): ((F32)iWidth)/((F32)ptex->width);
                            iWidth = scale*ptex->width;
                            iHeight = scale*ptex->height;
                        }
                    }
                }
            }
        }        
        else if(strnicmp("empty:", pimg->achTex, 6)==0) 
        { 
            char *stringPos = NULL;
            char *data;
            estrPrintCharString(&stringPos, pimg->achTex + 6);
            iWidth = iHeight = 0;
            
            data = strtok(stringPos, "_");
            if(data)
            {
                iWidth = atoi(stringPos);
                data = strtok(NULL, "_");
                if(data)
                    iHeight = atoi(data);
            }

            ptex = NULL;
            estrDestroy(&stringPos);
        }
        else if(strnicmp("picturebrowser:", pimg->achTex, strlen("picturebrowser:"))==0) 
        { 
            char *data = pimg->achTex+strlen("picturebrowser:");
            PictureBrowser *pb;
            int idx;
//            F32 scale = (pattrs->piScale)?.01*(*pattrs->piScale):1;
            F32 scale = smBlock_CalcFontScale(pBlock, pattrs);

            iWidth = iHeight = 0;

            idx = atoi(data);
            pb = picturebrowser_getById(idx);
            picturebrowser_setScale(pb, scale);
            if(pb)
            {
                iWidth = pb->wd;
                iHeight = pb->ht;
            }
                
            ptex = NULL;
        }
        else if(strnicmp("mmframe:", pimg->achTex, 8)==0)
        {
            int idx = atoi(pimg->achTex+8);
            if(idx>=0)
            {
                const cCostume *pCostume = npcDefsGetCostume(idx, 0);
                if((ptex =seqGetMMShot( pCostume, 0, PICTURETYPE_BODYSHOT))!=NULL)
                {
                    iWidth = ptex->width;
                    iHeight = ptex->height;
                }
                else
                {
                    iWidth = 256;
                    iHeight = 256;
                }
            }
        }
        else if(strnicmp("mmmap:", pimg->achTex, 4)==0)
        {
            iWidth = 256;
            iHeight = 256;
        }
        else if((ptex = atlasLoadTexture(pimg->achTex))!=NULL)
        {
            iWidth = ptex->width;
            iHeight = ptex->height;
        }
        if(pimg->iWidth <= 0)
        {
            pimg->iWidth = iWidth;
        }
        if(pimg->iHeight <= 0)
        {
            pimg->iHeight = iHeight;
        }
        if(pBlock && pBlock->pSMFBlock && pBlock->pSMFBlock->masterScale)
        {
            pimg->iHeight *= pBlock->pSMFBlock->masterScale;
            pimg->iWidth *= pBlock->pSMFBlock->masterScale;
        }
        pBlock->pos.iMinWidth = pimg->iWidth + pBlock->pos.iBorder*2;
        pBlock->pos.iMinHeight = pimg->iHeight + pBlock->pos.iBorder*2;
    }
    else if(TAG_MATCHES(sm_td))
    {
        pBlock->pos.iMinWidth = iChildMaxWidth + pBlock->pos.iBorder*2;
        pBlock->pos.iMinHeight = iChildMaxHeight + pBlock->pos.iBorder*2;
    }
    else if(TAG_MATCHES(sm_span))
    {
        pBlock->pos.iMinWidth = iChildMaxWidth + pBlock->pos.iBorder*2;
        pBlock->pos.iMinHeight = iChildMaxHeight + pBlock->pos.iBorder*2;
    }
    else if(pBlock->iType == -1) // Top level block
    {
        pBlock->pos.iMinWidth = iChildMaxWidth;
        pBlock->pos.iMinHeight = iChildMaxHeight;
    }
    else
    {
        pBlock->pos.iMinWidth = 0;
        pBlock->pos.iMinHeight = 0;
    }
}

/**********************************************************************func*
 * GetRequestedWidth
 *
 */
static void GetRequestedWidth(SMBlock *pBlock, int iWidth)
{
    int iChildMaxWidth = 0;
    int iChildSumWidth = 0;
    int i;

    // Most blocks can't request a width, they demand a certain minimum
    // width instead. This function deals with mainly with tables and their
    // size requests.

    // When this function is done, pos.iWidth will be set to the width
    // request as a percentage of the whole width the element is allowed
    // to take. (Non-table elements will be 0, since they have no request.)

    if(pBlock->bHasBlocks)
    {
        for(i=eaSize(&pBlock->ppBlocks)-1; i>=0; i--)
        {
            GetRequestedWidth(pBlock->ppBlocks[i], iWidth);

            if(iChildMaxWidth < pBlock->ppBlocks[i]->pos.iWidth)
            {
                iChildMaxWidth = pBlock->ppBlocks[i]->pos.iWidth;
            }

            iChildSumWidth += pBlock->ppBlocks[i]->pos.iWidth;
        }
    }

    if(TAG_MATCHES(sm_table))
    {
        int aiWidths[SMF_MAX_COLS];
        int iSize;

        // OK, by the time we got here, we've dealt with all of our children.

        memset(aiWidths, -1, sizeof(aiWidths));

        // Find the requested width for each column in the table.
        iSize = eaSize(&pBlock->ppBlocks);
        for(i=0; i<iSize; i++)
        {
            if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_tr)
            {
                int j;
                int iCnt=0;
                int iSize2 = eaSize(&pBlock->ppBlocks[i]->ppBlocks);

                for(j=0; j<iSize2; j++)
                {
                    SMBlock *pTd = pBlock->ppBlocks[i]->ppBlocks[j];

                    if(smf_aTagDefs[pTd->iType].id == k_sm_td && iCnt<SMF_MAX_COLS)
                    {
                        if(aiWidths[iCnt]<pTd->pos.iWidth)
                        {
                            aiWidths[iCnt] = pTd->pos.iWidth;
                        }
                        iCnt++;
                    }
                }
            }
        }

        iChildSumWidth = 0;
        for(i=0; aiWidths[i]>=0 && i<SMF_MAX_COLS; i++)
        {
            iChildSumWidth += aiWidths[i];
        }

        // OK, now propagate the widths back down the tree
        for(i=0; i<iSize; i++)
        {
            if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_tr)
            {
                int j;
                int iCnt=0;
                int iSize2 = eaSize(&pBlock->ppBlocks[i]->ppBlocks);

                pBlock->ppBlocks[i]->pos.iWidth = iChildSumWidth;

                for(j=0; j<iSize2 && iCnt<SMF_MAX_COLS; j++)
                {
                    if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_td)
                    {
                        pBlock->ppBlocks[i]->ppBlocks[j]->pos.iWidth = aiWidths[iCnt];
                    }
                    iCnt++;
                }
            }
        }

        pBlock->pos.iWidth = iChildSumWidth;
    }
    else if(TAG_MATCHES(sm_tr))
    {
        pBlock->pos.iWidth = iChildSumWidth;
    }
    else if(TAG_MATCHES(sm_td))
    {
        pBlock->pos.iWidth = (((SMColumn *)pBlock->pv)->iWidthRequested);
    }
    else if(pBlock->iType == -1) // top level block
    {
        pBlock->pos.iWidth = iChildSumWidth;
    }
    else
    {
        // Non-table blocks have no width at this point.
        pBlock->pos.iWidth  = 0;
        pBlock->pos.iHeight = 0;
    }
}

/**********************************************************************func*
 * CalcWidths
 *
 */
static bool CalcWidths(SMBlock *pBlock, int iWidth)
{
    bool bRet = true;
    int iChildMaxWidth = 0;
    int iChildSumWidth = 0;
    int k;

    // Previous to this function, you should call GetRequestedWidth.

    // This function reconciles width requests and minimum required size
    // for all the elements. Therefore, for most elements, the width
    // calculated is the same as the minimum width. Tables, though, have
    // to go through a bunch of rigamarole.

    // The width requests from tables don't guarantee that the enclosed
    // element will fit. If the request is too small, it is made large
    // enough to fit. Also, respecting a width request for one column might
    // make it impossible to fit another column. This (and other cases)
    // are handled as best as possible.

    // It is possible for CalcWidths to be unable to reconcile the contraints
    // given. If this occurs, it returns false.

    iWidth -= pBlock->pos.iBorder*2;

    if(pBlock->bHasBlocks)
    {
        int iNumBlocks = eaSize(&pBlock->ppBlocks);
        for(k=0; k<iNumBlocks; k++)
        {
            int i;

            // Previously we've went bottom up to find the minimum and
            // requested sizes. Now we'll go top-down and finalize them.

            if(smf_aTagDefs[pBlock->ppBlocks[k]->iType].id == k_sm_table)
            {
                SMBlock *pTable = pBlock->ppBlocks[k];
                float afPortions[SMF_MAX_COLS];
                int iCnt;
                int iCntMin;
                int aiMinWidths[SMF_MAX_COLS];
                int aiWidths[SMF_MAX_COLS];
                int iSum;
                int iDisperse;
                int iNeeded;
                int iRowBorder = 0;
                int iSize;
                int tempWidth = iWidth;

                DBG_PRINTF(("Fit the table in %d\n", iWidth));

                memset(aiMinWidths, -1, sizeof(aiMinWidths));
                memset(aiWidths, -1, sizeof(aiWidths));

                // TODO: If it seems like we just did all of this, we did. The info
                // just wasn't stored anywhere.
                iSize = eaSize(&pTable->ppBlocks);
                for(i=0; i<iSize; i++)
                {
                    if(smf_aTagDefs[pTable->ppBlocks[i]->iType].id == k_sm_tr)
                    {
                        int j;
                        int iSize2 = eaSize(&pTable->ppBlocks[i]->ppBlocks);
                        if(pTable->ppBlocks[i]->pos.iBorder>iRowBorder)
                        {
                            iRowBorder = pTable->ppBlocks[i]->pos.iBorder;
                        }

                        for(iCnt=0, j=0; j<iSize2; j++)
                        {
                            SMBlock *pTd = pTable->ppBlocks[i]->ppBlocks[j];

                            if(smf_aTagDefs[pTd->iType].id == k_sm_td && iCnt<SMF_MAX_COLS)
                            {
                                if(aiWidths[iCnt]<pTd->pos.iWidth)
                                {
                                    aiWidths[iCnt] = pTd->pos.iWidth;
                                }

                                if(aiMinWidths[iCnt]<pTd->pos.iMinWidth)
                                {
                                    aiMinWidths[iCnt] = pTd->pos.iMinWidth+1;
                                }

                                iCnt++;
                            }
                        }
                    }
                }
                // TODO: End of repetition

                // Shrink the available width by the biggest row border;
                tempWidth -= iRowBorder*2;

                // Get the requested percentages.
                for(iCnt=0, i=0; aiWidths[i]>=0 && i<SMF_MAX_COLS; i++)
                {
                    afPortions[i] = (float)aiWidths[i]/100.0f;
                    iCnt++;
                    DBG_PRINTF(("portion[%d] = %f\n", i, afPortions[i]));
                }

                // If a column didn't request a size, then it will have a
                // portion == zero at this point.

                DBG_PRINTF(("%d columns\n", iCnt));

                DBG_PRINTF(("first pass\n"));

                // Do a first pass at sizes.
                // This will give sizes to columns which have a requested
                // width ("stretchy columns").  Ones without a request
                // ("fixed columns") will be fixed up later.
                for(iSum=0, i=0; i<iCnt; i++)
                {
                    if(afPortions[i]>0)
                    {
                        aiWidths[i] = ceil(afPortions[i]*tempWidth);
                        DBG_PRINTF(("size[%d] = %d\n", i, aiWidths[i]));

                        // If the requested size is too small, calc an alternative
                        // size equal to the min. If this one fails further down,
                        // it's OK.
                        if(aiWidths[i] < aiMinWidths[i])
                        {
                            afPortions[i] = ((float)aiMinWidths[i]+0.5)/(float)tempWidth;
                            aiWidths[i] = ceil(afPortions[i]*tempWidth);
                            DBG_PRINTF(("was too small updated size[%d] = %d\n", i, aiWidths[i]));
                        }

                        iSum += aiWidths[i];
                    }
                }

                // Now check for minimums.
                // If a column had no requested size or if the requested size
                // was too small, we handle it here by allocating the
                // minimum width needed.

                // The total amount needed for all the fixed columns
                iNeeded = 0;

                // Count of fixed columns.
                iCntMin = 0;

                for(i=0; i<iCnt; i++)
                {
                    if(aiWidths[i]<aiMinWidths[i])
                    {
                        iNeeded += aiMinWidths[i];
                        aiWidths[i] = aiMinWidths[i];
                        DBG_PRINTF(("col[%d] needs %d.\n", i, aiWidths[i]));

                        // If the request was too small to support, ignore
                        // the request. Make the stretchy column into a
                        // fixed column.
                        afPortions[i] = 0;
                        iCntMin++;
                    }
                }
                DBG_PRINTF(("Need %d (there is %d left)\n", iNeeded, tempWidth-iSum));

                DBG_PRINTF(("second pass\n"));

                // Do a second pass at sizes.
                // This time, the columns with a requested size are only
                // allocated from what's left over from the minimum needs
                // of the fixed columns.

                // This is the amount each stretchy column needs to give back
                // to the fixed ones so they can fit.
                DBG_PRINTF(("%d - %d - %d = %d/%d to give back\n", tempWidth, iSum, iNeeded, tempWidth-iSum-iNeeded, iCnt-iCntMin));
                if(iCnt!=iCntMin)
                {
                    // The magic -1 on the next line is used to make sure we
                    // actually get enough-- fractional sizes aren't allowed.
                    iDisperse = (tempWidth-iSum-iNeeded)/(iCnt-iCntMin)-1;
                    if(iDisperse>0)
                    {
                        // If there is extra space, then we don't need to give
                        // any up.
                        iDisperse=0;
                    }
                }
                else
                {
                    iDisperse=0;
                }

                // We need to do this even if we aren't taking stuff back
                // since some of the previous stretchy columns might now
                // be fixed columns. We need to update iSum.
                for(iSum=0, i=0; i<iCnt; i++)
                {
                    if(afPortions[i]!=0)
                    {
                        aiWidths[i] += iDisperse;
                        iSum += aiWidths[i];
                        DBG_PRINTF(("size[%d] = %d\n", i, aiWidths[i]));
                    }
                }

                DBG_PRINTF(("Totalling %d\n", iSum));

                // The fixed columns get whatever is left over split
                // between them equally.
                DBG_PRINTF(("%d - %d - %d = %d/%d to give out\n", tempWidth, iSum, iNeeded, tempWidth-iSum-iNeeded, iCntMin));
                if(iCntMin>0)
                {
                    iDisperse = (tempWidth-iSum-iNeeded)/iCntMin;
                }
                else
                {
                    iDisperse = 0;
                }

                // Again, always do this to update iSum.
                for(iSum=0, i=0; i<iCnt; i++)
                {
                    if(afPortions[i]==0)
                    {
                        aiWidths[i] += iDisperse;
                        DBG_PRINTF(("size[%d] = %d\n", i, aiWidths[i]));
                    }

                    iSum += aiWidths[i];
                }

                // Spread any excess (due to fractions) over the columns
                for(i=0; i<iCnt && iSum<tempWidth; i++)
                {
                    aiWidths[i]++;
                    iSum++;
                    DBG_PRINTF(("fixup size[%d] = %d\n", i, aiWidths[i]));
                }

                pTable->pos.iWidth = iSum+iRowBorder*2;

                // Ta daaa! All the columns now have a reasonable width.

                // Of course, if the incoming data was bad (like they asked
                // for > 100% of the space to be split up, or there wasn't
                // enough space to fit all the columns) then the overall
                // width may be larger then the original.
                //
                // Because we control the input (and this isn't for an
                // error-tolerant web-browser), this code doesn't try to
                // handle this gracefully. It does flag the problem, though.

                if(iSum>tempWidth)
                {
                    DBG_PRINTF(("**** Data doesn't fit into given width!\n"));
                    bRet = false;
                }
                for(i=0; i<iCnt; i++)
                {
                    if(aiWidths[i]<aiMinWidths[i])
                    {
                        DBG_PRINTF(("**** Column %d doesn't have enough space!\n", i));
                        bRet = false;
                    }
                }

                // OK, now propagate the widths back down the tree.
                // Have them determine their widths as well (if they need to).
                for(i=0; i<iSize; i++)
                {
                    if(smf_aTagDefs[pTable->ppBlocks[i]->iType].id == k_sm_tr)
                    {
                        int j;
                        int iCnt=0;
                        int iSize2 = eaSize(&pTable->ppBlocks[i]->ppBlocks);

                        pTable->ppBlocks[i]->pos.iWidth = tempWidth+iRowBorder*2;

                        for(j=0; j<iSize2; j++)
                        {
                            if(smf_aTagDefs[pTable->ppBlocks[i]->ppBlocks[j]->iType].id == k_sm_td)
                            {
                                pTable->ppBlocks[i]->ppBlocks[j]->pos.iWidth = (int)aiWidths[iCnt];
                                bRet = CalcWidths(pTable->ppBlocks[i]->ppBlocks[j], (int)aiWidths[iCnt]) && bRet;
                                iCnt++;
                            }
                            else
                            {
                                // Well-formed sml shouldn't get here.
                                pTable->ppBlocks[i]->ppBlocks[j]->pos.iWidth = 0;
                                pTable->ppBlocks[i]->ppBlocks[j]->pos.iMinWidth = 0;
                                pTable->ppBlocks[i]->ppBlocks[j]->pos.iHeight = 0;
                                pTable->ppBlocks[i]->ppBlocks[j]->pos.iMinHeight = 0;
                                DBG_PRINTF(("** Extra out-of-band stuff in <tr> definition.\n"));
                            }
                        }
                    }
                    else
                    {
                        // Well-formed sml shouldn't get here.
                        pTable->ppBlocks[i]->pos.iWidth = 0;
                        pTable->ppBlocks[i]->pos.iMinWidth = 0;
                        pTable->ppBlocks[i]->pos.iHeight = 0;
                        pTable->ppBlocks[i]->pos.iMinHeight = 0;
                        DBG_PRINTF(("** Extra out-of-band stuff in <table> definition.\n"));
                    }
                }
            }
            else
            {
                if(pBlock->ppBlocks[k]->bHasBlocks)
                {
                    pBlock->ppBlocks[k]->pos.iWidth = iWidth;
                    bRet = CalcWidths(pBlock->ppBlocks[k], iWidth) && bRet;
                }
                else
                {
                    pBlock->ppBlocks[k]->pos.iWidth = pBlock->ppBlocks[k]->pos.iMinWidth;
                }
            }
        }
    }

    return bRet;
}

static void increaseBorderBuffer(int **ppiLeft, int **ppiRight, int leftv, int rightv)
{
    int oldsize = eaiSize(ppiLeft);
    int i;
    eaiSetSize(ppiLeft,oldsize * 2);
    eaiSetSize(ppiRight,oldsize * 2);
    for (i = eaiSize(ppiLeft) - 1; i >= oldsize; i--)
    {
        (*ppiLeft)[i] = leftv;
        (*ppiRight)[i] = rightv;
    }
}

/**********************************************************************func*
 * FloatImage
 *
 */
static void FloatImage(SMBlock *pBlock, SMAlignment eAlign, int iX, int iY, int **ppiLeft, int **ppiRight)
{
    int k;
    int iMax;
    int iMin;
    int bbSize = eaiSize(ppiLeft);

    // Find the max left value over the vertical span. This is
    // where the element will begin.
    //
    // (Now that I think about it, there isn't any way that any line after
    // this one can be any thinner since we do layout top to bottom and we
    // don't support anything which could do something out of order. I guess
    // this doesn't cause any harm, though.)
    iMax=0;
    iMin=(*ppiRight)[iY];
    for(k=0; k<pBlock->pos.iHeight; k++)
    {
        while (k+iY >= bbSize)
        {
            // Always use the parent's data, this will set the same values
            // as if the calling CalcHeights did the doubling.
            increaseBorderBuffer(ppiLeft, ppiRight, pBlock->pParent->pos.iBorder, pBlock->pParent->pos.iWidth - pBlock->pParent->pos.iBorder);
            bbSize = eaiSize(ppiLeft);
        }
        if(iMax<(*ppiLeft)[iY+k])
        {
            iMax = (*ppiLeft)[iY+k];
        }
        if((*ppiRight)[iY+k]<iMin)
        {
            iMin = (*ppiRight)[iY+k];
        }
    }

    DBG_PRINTF(("  max margin from %d to %d = %d\n", iY, iY+k-1, iMax));
    DBG_PRINTF(("  min margin from %d to %d = %d\n", iY, iY+k-1, iMin));

    pBlock->pos.iX = iMax;
    pBlock->pos.iY = iY;

    if(iMin<iMax+pBlock->pos.iWidth)
    {
        // Uck. It doesn't fit here.
        // A real formatter would deal with this in a graceful fashion.
        // This code isn't going to.
        //
        // Instead, it's not going to update the left margin at all.
        DBG_PRINTF(("  Doesn't fit!!\n"));
    }
    else
    {
        int *piSide;
        int iMargin;

        // OK, push the margin out from the max we found by the
        // width of the element.
        if(eAlign==SMAlignment_Right)
        {
            piSide = *ppiRight;
            iMargin = iMin-pBlock->pos.iWidth;
            pBlock->pos.iX = iMargin;
            DBG_PRINTF(("  right align\n"));
        }
        else
        {
            piSide = *ppiLeft;
            iMargin = iMax+pBlock->pos.iWidth;
        }
        DBG_PRINTF(("  %d %d\n", eAlign, SMAlignment_Right));

        for(k=0; k<pBlock->pos.iHeight; k++)
        {
            while (k+iY >= bbSize)
            {
                increaseBorderBuffer(ppiLeft, ppiRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
                bbSize = eaiSize(ppiLeft);
            }
            piSide[iY+k] = iMargin;
        }
        DBG_PRINTF(("  set new margin to %d\n", iMargin));
    }

}

/**********************************************************************func*
 * CalcHeights
 *
 */

static void CalcHeights(SMBlock *pBlock, SMFLineBreakMode lineBreakMode)
{
    static SMBlock **ppFloatsLeft = NULL;
    static SMBlock **ppFloatsRight = NULL;
    static int *piLeft = 0;
    static int *piRight = 0;
    int iChildSumWidth = 0;
    int iChildMaxHeight = 0;
    int iX;
    int iY;
    int iLastLeft;
    int iLastRight;
    int i;
    int bbSize;

    // Previous to this function, you should call GetRequestedWidth and
    // CalcWidths.

    // This function takes the widths calculated and does layout (word
    // wrap, mainly) of all the blocks. When this function is complete,
    // each block will have it's final width and height as well as an
    // X and Y location with respect to its enclosing block's origin.
    // You can call CalcLocations after this function to make the X and Y
    // values absolute instead.

    // Handing this function a block which failed CalcWidths will give
    // undefined results.

    if(pBlock->bHasBlocks)
    {
        int iNumBlocks = eaSize(&pBlock->ppBlocks);

        for(i=0; i<iNumBlocks; i++)
            CalcHeights(pBlock->ppBlocks[i], lineBreakMode);
    }

    if (!piLeft)
    {
        eaiCreateWithCapacity(&piLeft, 1000);
        eaiCreateWithCapacity(&piRight, 1000);
        eaCreateWithCapacity(&ppFloatsLeft, 1000);
        eaCreateWithCapacity(&ppFloatsRight, 1000);
    }

    // eaClear touches too much memory
    eaiSetSize(&piLeft, 0);
    eaiSetSize(&piLeft, 1000);
    eaiSetSize(&piRight, 0);
    eaiSetSize(&piRight, 1000);

    eaSetSize(&ppFloatsLeft, 0);
    eaSetSize(&ppFloatsRight, 0);
    
    bbSize = eaiSize(&piLeft);

    for(i=bbSize - 1; i>= 0; i--)
    {
        piLeft[i] = pBlock->pos.iBorder;
        piRight[i] = pBlock->pos.iWidth-pBlock->pos.iBorder;
    }

    iX = iLastLeft = pBlock->pos.iBorder;
    iY = iLastRight = pBlock->pos.iBorder;

    while (iY >= bbSize)
    {
        increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
        bbSize = eaiSize(&piLeft);
    }

    if(pBlock->bHasBlocks)
    {
        int j;
        int iFirstBlockOnLine = 0;
        int iNumBlocks = eaSize(&pBlock->ppBlocks);

        DBG_PRINTF(("Container: %s (%s): loc(%d, %d) size(%dx%d) border(%d)\n",
            smf_aTagDefs[pBlock->iType].pchName,
            pBlock->bFreeOnDestroy&&pBlock->pv ? pBlock->pv : "",
            pBlock->pos.iX,
            pBlock->pos.iY,
            pBlock->pos.iWidth,
            pBlock->pos.iHeight,
            pBlock->pos.iBorder));

        for(i=0; i<iNumBlocks; i++)
        {
            bool bFloating = false;

            // handle floating images
            if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_image)
            {
                if(pBlock->ppBlocks[i]->pos.alignHoriz==SMAlignment_Left
                    || pBlock->ppBlocks[i]->pos.alignHoriz==SMAlignment_Right)
                {
                    bFloating = true;
                }
            }

            if ((iChildSumWidth+pBlock->ppBlocks[i]->pos.iWidth > piRight[iY]-piLeft[iY]
                 || bFloating
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_table
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_tr
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_p
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_p_end
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_span
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_span_end
                 || smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_br)
                && lineBreakMode != SMFLineBreakMode_SingleLine)
            {
                int iCntVisibleBlocks = 0;
                int iCurCnt = 0;

                DBG_PRINTF(("  Wrapping: %d > %d-%d\n", iChildSumWidth+pBlock->ppBlocks[i]->pos.iWidth, piRight[iY], piLeft[iY]));

                // Figure out how many items are on the line so we can justify
                // the text properly.
                if(pBlock->pos.alignHoriz == SMAlignment_Both)
                {
                    if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_br)
                    {
                        piRight[iY] = iX;
                    }
                    else
                    {
                        for(j=iFirstBlockOnLine; j<i; j++)
                        {
                            if(pBlock->ppBlocks[j]->pos.iWidth>0)
                            {
                                iCntVisibleBlocks++;
                            }
                        }
                    }
                }

                // Promote everything on this line so that they all have the
                // same baseline.
                for(j=iFirstBlockOnLine; j<i; j++)
                {
                    DBG_PRINTF(("    Height fixup: %d (%s = %s)\n", iChildMaxHeight, smf_aTagDefs[pBlock->ppBlocks[j]->iType].pchName, pBlock->ppBlocks[j]->bFreeOnDestroy&&pBlock->ppBlocks[j]->pv ? pBlock->ppBlocks[j]->pv : ""));
                    pBlock->ppBlocks[j]->pos.iHeight = iChildMaxHeight;

                    if(pBlock->pos.alignHoriz == SMAlignment_Right)
                    {
                        pBlock->ppBlocks[j]->pos.iX += piRight[iY]-iX;
                    }
                    else if(pBlock->pos.alignHoriz == SMAlignment_Center)
                    {
                        pBlock->ppBlocks[j]->pos.iX += (piRight[iY]-iX)/2;
                    }
                    else if(pBlock->pos.alignHoriz == SMAlignment_Both && piRight[iY] != iX)
                    {
                        if(pBlock->ppBlocks[j]->pos.iWidth>0)
                        {
                            pBlock->ppBlocks[j]->pos.iX += ceil(iCurCnt*(piRight[iY]-iX)/(float)iCntVisibleBlocks);
                            pBlock->ppBlocks[j]->pos.iWidth += (ceil((iCurCnt+1)*(piRight[iY]-iX)/(float)iCntVisibleBlocks) -
                                                                ceil(iCurCnt*(piRight[iY]-iX)/(float)iCntVisibleBlocks));
                            iCurCnt++;
                        }
                    }
                }

                // Move the cursor down.
                iY += iChildMaxHeight;

                while (iY >= bbSize)
                {
                    increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
                    bbSize = eaiSize(&piLeft);
                }

                // Do the inverse tab.
                if (lineBreakMode == SMFLineBreakMode_MultiLineBreakOnWhitespaceWithInverseTabbing)
                {
                    piLeft[iY] += 20;
                }

                ///////
                //
                // START of special cases for particular tags.
                //
                // These really should be done more parametrically, but I don't
                // think I'm at the limit for that yet.
                //

                if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_table)
                {
                    // tables are always the full width, so they need to "clear left"
                    while(piLeft[iY]!=pBlock->pos.iBorder)
                    {                        
                        iY++;
                        while (iY >= bbSize)
                        {
                            increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
                            bbSize = eaiSize(&piLeft);
                        }
                    }
                }
                else if(iFirstBlockOnLine<=(i-1)
                    && smf_aTagDefs[pBlock->ppBlocks[i-1]->iType].id == k_sm_ws)
                {
                    // Make the size of spaces at the end of the line zero
                    pBlock->ppBlocks[i-1]->pos.iWidth = 0;
                    pBlock->ppBlocks[i-1]->pos.iHeight = 0;
                }
                else if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_p_end)
                {
                    // Paragraphs have a bit of space after them
                    iY += 10;
                }

                //
                // END of special cases for tags
                //
                ///////

                if(bFloating)
                {
                    FloatImage(pBlock->ppBlocks[i], pBlock->ppBlocks[i]->pos.alignHoriz, iX, iY, &piLeft, &piRight);
                    if(pBlock->ppBlocks[i]->pos.alignHoriz==SMAlignment_Right)
                    {
                        eaPush(&ppFloatsRight, pBlock->ppBlocks[i]);
                    }
                    else
                    {
                        eaPush(&ppFloatsLeft, pBlock->ppBlocks[i]);
                    }

                    // Reset word wrap for the new line
                    iFirstBlockOnLine = i+1;
                }
                else
                {
                    // Reset word wrap for the new line
                    iFirstBlockOnLine = i;
                }
                iChildSumWidth = 0;
                iChildMaxHeight = 0;

                while (iY >= bbSize)
                {
                    increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
                    bbSize = eaiSize(&piLeft);
                }

                // Move the cursor to the left-most point available on this
                // line.            
                {
                    int j;
                    int iYShift = iY;

                    iX = piLeft[iY];

                    // OK, now we need to do some floating fixup

                    if(piRight[iY]>iLastRight)
                    {
                        while(iYShift>=0 && piRight[iYShift]>iLastRight)
                        {
                            iYShift--;
                        }
                        // We need to move the floater on this line down to
                        // fill the space.
                        j=eaSize(&ppFloatsRight);
                        while(j>0)
                        {
                            j--;
                            if( 6*ppFloatsRight[j]->pos.iHeight < (iY-iYShift+1) && ppFloatsRight[j]->pos.iHeight+ppFloatsRight[j]->pos.iY-1 == iYShift)
                            {
                                ppFloatsRight[j]->pos.iY += (iY-iYShift+1)/2;
                            }
                        }
                    }

                    if(piLeft[iY]<iLastLeft)
                    {
                        while(iYShift>=0 && piLeft[iYShift]<iLastLeft)
                        {
                            iYShift--;
                        }
                        // We need to move the floater on this line down to
                        // fill the space.
                        j=eaSize(&ppFloatsLeft);
                        while(j>0)
                        {
                            j--;
                            if( 6*ppFloatsLeft[j]->pos.iHeight < (iY-iYShift+1) && ppFloatsLeft[j]->pos.iHeight+ppFloatsLeft[j]->pos.iY-1 == iYShift)
                            {
                                ppFloatsLeft[j]->pos.iY += (iY-iYShift+1)/2;
                            }
                        }
                    }

                    iLastRight = piRight[iY];
                    iLastLeft = piLeft[iY];
                }
                DBG_PRINTF(("  start line at %d\n", iX));
            }

            if(!bFloating)
            {
                // This keeps whitespace from indenting at the beginning
                // of the line.
                if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id == k_sm_ws
                    && iX==piLeft[iY] &&
                    lineBreakMode != SMFLineBreakMode_SingleLine)
                {
                    pBlock->ppBlocks[i]->pos.iWidth = 0;
                    pBlock->ppBlocks[i]->pos.iHeight = 0;
                }

                pBlock->ppBlocks[i]->pos.iX = iX;
                pBlock->ppBlocks[i]->pos.iY = iY;

                iX += pBlock->ppBlocks[i]->pos.iWidth;
                iChildSumWidth += pBlock->ppBlocks[i]->pos.iWidth;

                if(iChildMaxHeight < pBlock->ppBlocks[i]->pos.iHeight)
                {
                    iChildMaxHeight = pBlock->ppBlocks[i]->pos.iHeight;
                }
            }

            if(smf_aTagDefs[pBlock->ppBlocks[i]->iType].id != k_sm_ws)
            {
                DBG_PRINTF(("Final: %s (%s): loc(%d, %d) size(%dx%d)\n",
                    smf_aTagDefs[pBlock->ppBlocks[i]->iType].pchName,
                    pBlock->bFreeOnDestroy&&pBlock->pv ? pBlock->pv : "",
                    pBlock->ppBlocks[i]->pos.iX,
                    pBlock->ppBlocks[i]->pos.iY,
                    pBlock->ppBlocks[i]->pos.iWidth,
                    pBlock->ppBlocks[i]->pos.iHeight));
            }
        }

        // Clean up block on last line
        for(j=iFirstBlockOnLine; j<i; j++)
        {
            DBG_PRINTF(("    Height fixup: %d (%s = %s)\n", iChildMaxHeight, smf_aTagDefs[pBlock->ppBlocks[j]->iType].pchName, pBlock->ppBlocks[j]->bFreeOnDestroy&&pBlock->ppBlocks[j]->pv ? pBlock->ppBlocks[j]->pv : ""));
            pBlock->ppBlocks[j]->pos.iHeight = iChildMaxHeight;

            if(pBlock->pos.alignHoriz == SMAlignment_Right)
            {
                pBlock->ppBlocks[j]->pos.iX += piRight[iY]-iX;
            }
            else if(pBlock->pos.alignHoriz == SMAlignment_Center)
            {
                pBlock->ppBlocks[j]->pos.iX += (piRight[iY]-iX)/2;
            }
        }
        // And move the height to include the last line.
        iY += iChildMaxHeight;

        while (iY >= bbSize)
        {
            increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
            bbSize = eaiSize(&piLeft);
        }

        // Make sure the height can fit anything floating as well.
        for(i=bbSize-1; i>=0; i--)
        {
            if(piLeft[i] != pBlock->pos.iBorder || piRight[i] != pBlock->pos.iWidth-pBlock->pos.iBorder)
            {
                break;
            }
        }
        if(i>iY)
        {
            iY = i + 1;
        }
    }

    iY += pBlock->pos.iBorder;

    while (iY >= bbSize)
    {
        increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
        bbSize = eaiSize(&piLeft);
    }

    if(pBlock->iType == -1)
    {
        if(piLeft[i] != pBlock->pos.iBorder || piRight[i] != pBlock->pos.iWidth-pBlock->pos.iBorder)
        {
            iY++;
            while (iY >= bbSize)
            {
                increaseBorderBuffer(&piLeft, &piRight, pBlock->pos.iBorder, pBlock->pos.iWidth-pBlock->pos.iBorder);
                bbSize = eaiSize(&piLeft);
            }
        }
        pBlock->pos.iHeight = iY;
    }
    else if(TAG_MATCHES(sm_table))
    {
        pBlock->pos.iHeight = iY;
    }
    else if(TAG_MATCHES(sm_tr))
    {
        pBlock->pos.iHeight = iY;
    }
    else if(TAG_MATCHES(sm_td))
    {
        pBlock->pos.iHeight = iY;
    }
    else if(TAG_MATCHES(sm_span))
    {
        pBlock->pos.iHeight = iY;
    }
    else if(TAG_MATCHES(sm_text))
    {
        pBlock->pos.iHeight = pBlock->pos.iMinHeight;
    }
    else if(TAG_MATCHES(sm_br))
    {
        pBlock->pos.iHeight = pBlock->pos.iMinHeight;
    }
    else if(TAG_MATCHES(sm_ws))
    {
        pBlock->pos.iHeight = pBlock->pos.iMinHeight;
    }
    else if(TAG_MATCHES(sm_image))
    {
        pBlock->pos.iHeight = pBlock->pos.iMinHeight;
    }
    else
    {
        pBlock->pos.iHeight = 0;
    }
}

/**********************************************************************func*
 * CalcAlignment
 *
 */
static void CalcAlignment(SMBlock *pBlock)
{
    int i;

    if(pBlock->bHasBlocks)
    {
        int iMaxHeight = 0;

        if(pBlock->pos.alignVert!=SMAlignment_None)
        {
            for(i=eaSize(&pBlock->ppBlocks)-1; i>=0; i--)
            {
                if(pBlock->ppBlocks[i]->pos.iHeight+pBlock->ppBlocks[i]->pos.iY > iMaxHeight)
                {
                    iMaxHeight = pBlock->ppBlocks[i]->pos.iHeight+pBlock->ppBlocks[i]->pos.iY;
                }
            }

            // Do vertical alignment.
            // It's already top aligned by default.
            if(iMaxHeight < pBlock->pos.iHeight)
            {
                int iShift = 0;
                if(pBlock->pos.alignVert == SMAlignment_Center)
                {
                    iShift = (pBlock->pos.iHeight-iMaxHeight)/2;
                }
                else if(pBlock->pos.alignVert == SMAlignment_Bottom)
                {
                    iShift = pBlock->pos.iHeight-iMaxHeight;
                }

                for(i=eaSize(&pBlock->ppBlocks)-1; i>=0; i--)
                {
                    pBlock->ppBlocks[i]->pos.iY += iShift;
                }
            }

        }

        for(i=eaSize(&pBlock->ppBlocks)-1; i>=0; i--)
        {
            CalcAlignment(pBlock->ppBlocks[i]);
        }
    }
}

/**********************************************************************func*
 * CalcLocations
 *
 */
static void CalcLocations(SMBlock *pBlock)
{
    int i;

    // This function makes all of the X/Y locations absolute. (Or at least
    // absolute with respect to the X/Y value specified in the top level
    // block.

    if(pBlock->bHasBlocks)
    {
        for(i=eaSize(&pBlock->ppBlocks)-1; i>=0; i--)
        {
            pBlock->ppBlocks[i]->pos.iX += pBlock->pos.iX;
            pBlock->ppBlocks[i]->pos.iY += pBlock->pos.iY;

            CalcLocations(pBlock->ppBlocks[i]);
        }
    }
}

/**********************************************************************func*
 * Format
 *
 */
bool smf_Format(SMBlock *pBlock, TextAttribs *pattrs, int iWidth, int iHeight, SMFLineBreakMode lineBreakMode)
{
    bool bRet;

    // This formatter is constricted mainly by width. Width is considered
    // the limiter since there is no horizontal scrolling. Height can be
    // varied, and (frankly) isn't even paid attention to.

    // The order of the functions (and assignment) below are important.
    // Don't re-order them. In fact, you should probably never call these
    // function directly. The Format() function should be called instead.
    // Heck, I'm even going to make them static so you can't call them.

    // If there were problems getting the block to fit in the given
    // region, then false is returned.

    PERFINFO_AUTO_START("smf_Format", 1);
        PERFINFO_AUTO_START("GetMinimumSize", 1);

            GetMinimumSize(pBlock, pattrs, iWidth);

        PERFINFO_AUTO_STOP_START("GetRequestedWidth", 1);

            GetRequestedWidth(pBlock, iWidth);

        PERFINFO_AUTO_STOP_START("CalcWidths", 1);

            pBlock->pos.iWidth = iWidth;
            bRet = CalcWidths(pBlock, iWidth);

        PERFINFO_AUTO_STOP_START("CalcHeights", 1);
    
            CalcHeights(pBlock, lineBreakMode);
        //sm_BlockDump(pBlock, 0, smf_aTagDefs);

        PERFINFO_AUTO_STOP_START("CalcAlignment", 1);

            CalcAlignment(pBlock);

        PERFINFO_AUTO_STOP_START("CalcLocations", 1);

            CalcLocations(pBlock);

        PERFINFO_AUTO_STOP();
        
        bRet = bRet && (pBlock->pos.iHeight<=iHeight);
    PERFINFO_AUTO_STOP();
    
    return bRet;
}

/* End of File */
