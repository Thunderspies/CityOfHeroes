#include "formatter/smf_util.h"
#include "smf_select.h"

#include <utilitieslib/components/Earray.h>
#include <utilitieslib/components/estring.h>

#include "UI/uiInput.h"
#include <utilitieslib/utils/CBox.h>

#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/SuperAssert.h>

static char *smf_selectionString = 0;

// This holds what text was selected last frame. We know this is the complete
// selection text so it can be used for cut'n'paste and so on.
static char *smf_lastSelectionString = 0;

void smf_HandleSelectPerFrame(void)
{
    if (!smf_selectionString)
    {
        estrCreate(&smf_selectionString);
    }

    if (!smf_lastSelectionString)
    {
        estrCreate(&smf_lastSelectionString);
    }

    // The selection becomes "yesterday's news" so we can compile a new one
    // for this frame but anyone who needs a full selection can still get it.
    estrPrintEString(&smf_lastSelectionString, smf_selectionString);
    estrClear(&smf_selectionString);
}

char *smf_GetSelectionString(void)
{
    char *retval = smf_selectionString;

    // If we're called outside of the normal sequence of operations (eg. from
    // a context menu callback) the selection string might not have been
    // assembled yet, or might have been wiped prior to the new frame's work.
    // Thus we need the latched copy.
    if (!retval || strlen(retval) == 0)
    {
        retval = smf_lastSelectionString;
    }

    return retval;
}

// Cannot be in smf_HandlePerFrame because it requires the SMBlock itself, which isn't known at frame begin.
void smf_ResetSelectedBlocks(SMBlock *pBlock)
{
    SMAnchor *pAnchor;

    pBlock->bHover = false;
    pBlock->bSelected = false;
    if (pBlock->iType == kFormatTags_Anchor)
    {
        pAnchor = (SMAnchor *)pBlock->pv;
        pAnchor->bHover = false;
        pAnchor->bSelected = false;
    }

    if (pBlock->bHasBlocks)
    {
        int i;
        int iSize = eaSize(&pBlock->ppBlocks);
        for(i=0; i<iSize; i++)
        {
            smf_ResetSelectedBlocks(pBlock->ppBlocks[i]);
        }
    }
}

void smf_RebuildPartialSelectionFromSMFBlock(SMFBlock *pSMFBlock, int x, int y, int z)
{
    if (!smf_IsBlockSelectable(pSMFBlock))
    {
        return;
    }

    if (pSMFBlock == smf_selectionStartPane)
    {
        if (smf_selectionStartYBase == -1)
        {
            smf_selectionStartYBase = y;
        }
    }

    if (pSMFBlock == smf_selectionEndPane)
    {
        if (smf_selectionEndXBase == -1)
        {
            smf_selectionEndXBase = x;
        }

        if (smf_selectionEndYBase == -1)
        {
            smf_selectionEndYBase = y;
        }
    }

    if (pSMFBlock == smf_selectionSearchPane)
    {
        if (smf_selectionSearchXBase == -1)
        {
            smf_selectionSearchXBase = x;
        }

        if (smf_selectionSearchYBase == -1)
        {
            smf_selectionSearchYBase = y;
        }
    }
}

static void smf_RebuildPartialSelectionFromSMBlock(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    if (smBlock_HasFocus(pBlock)) // implies smf_IsBlockSelected().
    {
        if (smf_selectionEndCharacterXOffset == -1)
        {
            smf_selectionEndCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionEndDisplayIndex - pBlock->displayStringStartIndex);
        }

        if (smf_selectionEndCharacterYOffset == -1)
        {
            smf_selectionEndCharacterYOffset = pBlock->pos.iY;
        }

        if (smf_selectionEndCharacterYHeight == -1)
        {
            smf_selectionEndCharacterYHeight = pBlock->pos.iHeight;
        }
    }

    if (smf_IsSearchSMBlock(pBlock)) // does not imply smf_IsBlockSelected().
    {
        if (smf_selectionSearchCharacterXOffset == -1)
        {
            smf_selectionSearchCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionSearchDisplayIndex - pBlock->displayStringStartIndex);
        }

        if (smf_selectionSearchCharacterYOffset == -1)
        {
            smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        }

        if (smf_selectionSearchCharacterYHeight == -1)
        {
            smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;
        }
    }
}

/**********************************************************************func*
* smf_Interact
*
* Returns true only if pBlock is part of the current selection.
* Only anchors, text, and whitespace blocks can be selected part of the current selection.
*/
static bool smf_IsBlockSelected(SMBlock *pBlock, int iXBase, int iYBase, int iZBase)
{
    SMFBlock *thisRoot;
    int thisYBase;

    // Start and end are the endpoints of the selection in chronological order.
    SMFBlock *startRoot, *endRoot;
    int startYBase, endYBase;
    unsigned int startIndex, endIndex;

    // Early and late are the endpoints of the selection in read order (top-to-bottom, left-to-right).
    SMFBlock *earlyRoot, *lateRoot;
    int earlyYBase, lateYBase;
    unsigned int earlyIndex, lateIndex;

    unsigned int pBlockTextLength;

    if (!smf_selectionStartPane || !smfBlock_HasFocus(pBlock->pSMFBlock))
    {
        return false;
    }

    if (TAG_MATCHES(sm_ws) || TAG_MATCHES(sm_text) || TAG_MATCHES(sm_anchor))
    {
        pBlockTextLength = smBlock_GetDisplayLength(pBlock);
    }
    else
    {
        return false;
    }

    /*************************************************
    *                Data Preparation                *
    *************************************************/

    thisRoot = pBlock->pSMFBlock;
    thisYBase = iYBase;

    startRoot = smf_selectionStartPane;
    startYBase = smf_selectionStartYBase;
    startIndex = smf_selectionStartDisplayIndex;

    endRoot = smf_selectionEndPane;
    endYBase = smf_selectionEndYBase;
    endIndex = smf_selectionEndDisplayIndex;

    if (smf_IsSelectionStartBeforeSelectionEnd())
    {
        earlyRoot = startRoot;
        earlyYBase = startYBase;
        earlyIndex = startIndex;

        lateRoot = endRoot;
        lateYBase = endYBase;
        lateIndex = endIndex;
    }
    else
    {
        earlyRoot = endRoot;
        earlyYBase = endYBase;
        earlyIndex = endIndex;

        lateRoot = startRoot;
        lateYBase = startYBase;
        lateIndex = startIndex;
    }

    /*************************************************
    *                  Final Check                   *
    *************************************************/

    if (earlyRoot == lateRoot)
    {
        if (earlyRoot != thisRoot)
        {
            return false;
        }
        else
        {
            if (TAG_MATCHES(sm_ws))
            {
                return (earlyIndex <= pBlock->displayStringStartIndex &&
                    lateIndex >= pBlock->displayStringStartIndex + pBlockTextLength);
            }
            else
            {
                return (earlyIndex <= pBlock->displayStringStartIndex + pBlockTextLength &&
                    lateIndex >= pBlock->displayStringStartIndex);
            }
        }
    }
    else
    {
        if (earlyRoot == thisRoot)
        {
            if (TAG_MATCHES(sm_ws))
            {
                return earlyIndex <= pBlock->displayStringStartIndex;
            }
            else
            {
                return earlyIndex <= pBlock->displayStringStartIndex + pBlockTextLength;
            }
        }
        else if (lateRoot == thisRoot)
        {
            if (TAG_MATCHES(sm_ws))
            {
                return lateIndex >= pBlock->displayStringStartIndex + pBlockTextLength;
            }
            else
            {
                return lateIndex >= pBlock->displayStringStartIndex;
            }
        }
        else
        {
            return earlyYBase <= thisYBase && thisYBase <= lateYBase;
        }
    }
}

/**********************************************************************func*
* smf_CompileSelectionString
*
* Compiles the complete string of the text that is selected.
* Assumes that it receives blocks in the proper order for the text in the string.
* Assumes that smf_selectionString is cleared out at the beginning of every frame.
*/
static void smf_CompileSelectionString(SMBlock *pBlock, int iXBase, int iYBase, int iZBase)
{
    if (!pBlock->bSelected) // implies smf_IsBlockSelectable() through the functionality in smf_Select().
    {
        return;
    }

    if (smf_selectionCurrentYBase != -1 && smf_selectionCurrentYBase != iYBase)
    {
        estrConcatMinimumWidth(&smf_selectionString, "\r\n", 2);
        smf_selectionCurrentIY = 0;
    }
    if (smf_selectionCurrentIY != pBlock->pos.iY)
    {
        estrConcatMinimumWidth(&smf_selectionString, " ", 1);
        smf_selectionCurrentIY = pBlock->pos.iY;
    }
    if (TAG_MATCHES(sm_text))
    {
        char *rawText = (char *) pBlock->pv;
        if (rawText)
        {
            unsigned int pBlockEarlyCharacterIndex = pBlock->displayStringStartIndex;
            unsigned int pBlockLateCharacterIndex = pBlock->displayStringStartIndex + (rawText ? strlen(rawText) : 0);
            unsigned int rawLength = strlen(rawText);
            char *finalText = (char *) calloc((rawLength + 1), sizeof(char));
            unsigned int selectionEarlyCharacterIndex, selectionLateCharacterIndex;
            SMFBlock *selectionEarlyPane, *selectionLatePane;
            unsigned int finalEarlyCharacterIndex, finalLateCharacterIndex;
            unsigned int selectionLength;

            if (smf_IsSelectionStartBeforeSelectionEnd())
            {
                selectionEarlyCharacterIndex = smf_selectionStartDisplayIndex;
                selectionLateCharacterIndex = smf_selectionEndDisplayIndex;
                selectionEarlyPane = smf_selectionStartPane;
                selectionLatePane = smf_selectionEndPane;
            }
            else
            {
                selectionEarlyCharacterIndex = smf_selectionEndDisplayIndex;
                selectionLateCharacterIndex = smf_selectionStartDisplayIndex;
                selectionEarlyPane = smf_selectionEndPane;
                selectionLatePane = smf_selectionStartPane;
            }

            if (selectionEarlyPane == pBlock->pSMFBlock)
            {
                finalEarlyCharacterIndex = max(selectionEarlyCharacterIndex, pBlockEarlyCharacterIndex);
                if (pBlockLateCharacterIndex < finalEarlyCharacterIndex)
                {
                    assert(0);    //    how is this the case?
                                //     if this is repro-able beyond a strange frame timing bug try to track it down. if not, remove the asserts =/
                }
                finalEarlyCharacterIndex = min(finalEarlyCharacterIndex, pBlockLateCharacterIndex);
            }
            else
            {
                finalEarlyCharacterIndex = pBlockEarlyCharacterIndex;
            }

            if (selectionLatePane == pBlock->pSMFBlock)
            {
                finalLateCharacterIndex = min(selectionLateCharacterIndex, pBlockLateCharacterIndex);
                if (pBlockEarlyCharacterIndex >  finalLateCharacterIndex)
                {
                    assert(0);    //    how is this the case?
                                //     if this is repro-able beyond a strange frame timing bug try to track it down. if not, remove the asserts =/
                }
                finalLateCharacterIndex = max(finalLateCharacterIndex, pBlockEarlyCharacterIndex);
            }
            else
            {
                finalLateCharacterIndex = pBlockLateCharacterIndex;
            }

            if (finalEarlyCharacterIndex < finalLateCharacterIndex)
            {
                selectionLength = finalLateCharacterIndex - finalEarlyCharacterIndex;
            }
            else
            {
                selectionLength = 0;
                if (finalEarlyCharacterIndex > finalLateCharacterIndex)
                {
                    //    error.    Trying to catch when this happens
                    assert(0);
                }
            }
            strncpy(finalText, rawText + (finalEarlyCharacterIndex - pBlockEarlyCharacterIndex), selectionLength);
            finalText[selectionLength] = '\0';

            estrConcatMinimumWidth(&smf_selectionString, finalText, strlen(finalText));
            SAFE_FREE(finalText);
        }
    }
    if (TAG_MATCHES(sm_ws))
    {
        estrConcatMinimumWidth(&smf_selectionString, " ", 1);
    }

    smf_selectionCurrentYBase = iYBase;
}

void smf_Select(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    SMAnchor *pAnchor;
    int startingBytesAlloced, endingBytesAlloced;

    if((pBlock->pos.iWidth==0 || pBlock->pos.iX<0 || pBlock->pos.iY<0)
        && pBlock->iType>kFormatTags_Count*2)
    {
        return;
    }

    PERFINFO_AUTO_START("smf_Select", 1);

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    if(pBlock->iType == kFormatTags_Anchor)
    {
        pAnchor = (SMAnchor *)pBlock->pv;
    }

    if(pBlock->iType == -1)
    {
        // Top level block
    }
    else if(smf_ApplyFormatting(pBlock, pattrs))
    {
        // Should be before the rest.
        // Nothing more to do for these
    }
    else if(TAG_MATCHES(sm_nolink))
    {
        s_bAllowAnchor = false;
    }
    else if(TAG_MATCHES(sm_nolink_end))
    {
        s_bAllowAnchor = true;
    }
    else
    {
        CBox box;

        smf_RebuildPartialSelectionFromSMBlock(pBlock, pattrs, iXBase, iYBase, iZBase);

        BuildCBox(&box, pBlock->pos.iX + iXBase, pBlock->pos.iY + iYBase,
            pBlock->pos.iWidth - 1, pBlock->pos.iHeight - 1);

        if (mouseCollision(&box))
        {
            pBlock->bHover = true;
        }

        if (smf_IsBlockSelected(pBlock, iXBase, iYBase, iZBase))
        {
            pBlock->bSelected = true;

            smf_CompileSelectionString(pBlock, iXBase, iYBase, iZBase);
        }

        if (eaiSize(&pattrs->piAnchor) > 1 && s_bAllowAnchor)
        {
            // Although tracked, multiply nested anchors are ignored. Always use
            // the first anchor if available.
            pAnchor = (SMAnchor *)pattrs->piAnchor[1];
            if (pBlock->bHover)
            {
                pAnchor->bHover = true;
            }
            if (pBlock->bSelected)
            {
                pAnchor->bSelected = true;
            }
        }
    }

    if(pBlock->bHasBlocks)
    {
        int i;
        int iSize = eaSize(&pBlock->ppBlocks);
        for(i=0; i<iSize; i++)
        {
            smf_Select(pBlock->ppBlocks[i], pattrs, iXBase, iYBase, iZBase);
        }
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    PERFINFO_AUTO_STOP();
}