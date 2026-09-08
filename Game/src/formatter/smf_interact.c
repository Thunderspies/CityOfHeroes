#include "formatter/smf_util.h"
#include "formatter/smf_parse.h" // for sm_ParseTagName
#include "formatter/smf_interact.h"
#include "formatter/smf_main.h"

#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#include "UI/sprite/sprite_base.h"
#include "UI/uiInput.h"
#include "UI/uiScrollBar.h"
#include "UI/uiWindows.h"
#include "UI/uiFocus.h"
#include "UI/uiContextMenu.h"
#include <utilitieslib/utils/Cbox.h>

#include "cmdparse/cmdgame.h"
#include "UI/uiGame.h"

#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/timing.h>

#include "sound/sound.h"

#include "win/win_init.h"

#include "UI/uiUtilGame.h" // for drawBox debugging
#include "UI/uiClipper.h" // for ClipperPop() for cprnt debugging
#include "UI/sprite/sprite_text.h" // cprnt debugging
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/utils/utils.h>

static int smf_Interact_selectionStartRawIndex;
static int smf_Interact_selectionEndRawIndex;
static int smf_Interact_currentBestSMBlockDisplayIndex;
static int smf_Interact_currentBestSMBlockXDiff;
static int smf_Interact_currentBestSMBlockYDiff;
static int smf_Interact_currentCursorToRawOffset;
static int smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord;
static int smf_Interact_incrementCursorAfterThisManyMoreInsertions = 0;
static int smf_Interact_thisManyInsertionsHavePassed = 0;

static ContextMenu *smf_Interact_contextMenu = NULL;
static ContextMenu *smf_Interact_contextColorSubMenu = NULL;
static ContextMenu *smf_Interact_contextScaleSubMenu = NULL;
static ContextMenu *smf_Interact_contextTextSubMenu = NULL;

static int smf_CM_canCut(void *data)
{
    CMVisType retval = CM_HIDE;
    SMFBlock *pSMFBlock = (SMFBlock *)data;

    if (pSMFBlock &&
        pSMFBlock->contextMenuMode & SMFContextMenuMode_Cut)
    {
        retval = CM_VISIBLE;

        if (pSMFBlock->editMode == SMFEditMode_ReadWrite &&
            smf_IsAnythingSelected() &&
            (pSMFBlock == smf_selectionStartPane || pSMFBlock == smf_selectionEndPane))
        {
            retval = CM_AVAILABLE;
        }
    }

    return retval;
}

static int smf_CM_canCopy(void *data)
{
    CMVisType retval = CM_HIDE;
    SMFBlock *pSMFBlock = (SMFBlock *)data;

    if (pSMFBlock &&
        pSMFBlock->contextMenuMode & SMFContextMenuMode_Copy)
    {
        retval = CM_VISIBLE;

        if (smf_IsAnythingSelected() &&
            (pSMFBlock == smf_selectionStartPane || pSMFBlock == smf_selectionEndPane))
        {
            retval = CM_AVAILABLE;
        }
    }

    return retval;
}

static int smf_CM_canPaste(void *data)
{
    CMVisType retval = CM_HIDE;
    SMFBlock *pSMFBlock = (SMFBlock *)data;

    if (pSMFBlock &&
        pSMFBlock->contextMenuMode & SMFContextMenuMode_Paste)
    {
        retval = CM_VISIBLE;

        if (pSMFBlock->editMode == SMFEditMode_ReadWrite)
        {
            retval = CM_AVAILABLE;
        }
    }

    return retval;
}

static int smf_CM_canBoldItalicScale(void *data)
{
    CMVisType retval = CM_HIDE;
    SMFBlock *pSMFBlock = (SMFBlock *)data;

    if (pSMFBlock &&
        (pSMFBlock->contextMenuMode & SMFContextMenuMode_Format ))
    {
        retval = CM_VISIBLE;

        if (pSMFBlock->editMode == SMFEditMode_ReadWrite &&
            smf_IsAnythingSelected() &&
            (pSMFBlock == smf_selectionStartPane || pSMFBlock == smf_selectionEndPane))
        {
            retval = CM_AVAILABLE;
        }
    }

    return retval;
}

static int smf_CM_canColor(void *data)
{
    CMVisType retval = CM_HIDE;
    SMFBlock *pSMFBlock = (SMFBlock *)data;

    if (pSMFBlock &&
        pSMFBlock->contextMenuMode & SMFContextMenuMode_Color)
    {
        retval = CM_VISIBLE;

        if (pSMFBlock->editMode == SMFEditMode_ReadWrite &&
            smf_IsAnythingSelected() &&
            (pSMFBlock == smf_selectionStartPane || pSMFBlock == smf_selectionEndPane))
        {
            retval = CM_AVAILABLE;
        }
    }

    return retval;
}

static int smf_CM_canTextSub(void *data)
{
    CMVisType retval = CM_HIDE;
    SMFBlock *pSMFBlock = (SMFBlock *)data;

    if (pSMFBlock &&
        (pSMFBlock->contextMenuMode & SMFContextMenuMode_TextSub ) )
    {
        retval = CM_VISIBLE;

        if (pSMFBlock->editMode == SMFEditMode_ReadWrite)
        {
            retval = CM_AVAILABLE;
        }
    }
    
    return retval;
}

static void smf_CM_cut(void *data)
{
    smf_CopyStringToClipboard();
    smf_SetSelectionCommand(SMFSelectionCommand_Delete);
}

static void smf_CM_copy(void *data)
{
    smf_CopyStringToClipboard();
}

static void smf_CM_paste(void *data)
{
    smfEdit_currentCommand = SMFEditCommand_PasteStringAtCursor;
    smf_SetSelectionCommand(SMFSelectionCommand_Delete);
}

static void smf_CM_bold(void *data)
{
    smf_SetSelectionTags("&lt;b&gt;", "&lt;/b&gt;");
    smf_SetSelectionCommand(SMFSelectionCommand_InsertTags);
}

static void smf_CM_italic(void *data)
{
    smf_SetSelectionTags("&lt;i&gt;", "&lt;/i&gt;");
    smf_SetSelectionCommand(SMFSelectionCommand_InsertTags);
}

static void smf_CM_scaleSmall(void *data)
{
    smf_SetSelectionTags("&lt;scale 0.8&gt;", "&lt;/scale&gt;");
    smf_SetSelectionCommand(SMFSelectionCommand_InsertTags);
}

static void smf_CM_scaleMedium(void *data)
{
    smf_SetSelectionTags("&lt;scale 1.0&gt;", "&lt;/scale&gt;");
    smf_SetSelectionCommand(SMFSelectionCommand_InsertTags);
}

static void smf_CM_scaleLarge(void *data)
{
    smf_SetSelectionTags("&lt;scale 1.2&gt;", "&lt;/scale&gt;");
    smf_SetSelectionCommand(SMFSelectionCommand_InsertTags);
}

static void smf_BracketNamedColor(char *colorStr)
{
    U32 color;
    char preTagStr[50];

    color = smf_ConvertColorNameToValue(colorStr);

    // Don't need the alpha for our usage. By shifting we make it fit in
    // the lower three bytes so it'll print correctly.
    color = (color & ~0xff) >> 8;

    sprintf(preTagStr, "&lt;color #%06x&gt;", color);

    smf_SetSelectionTags(preTagStr, "&lt;/color&gt;");
    smf_SetSelectionCommand(SMFSelectionCommand_InsertTags);
}

static smf_CM_color(void *data)
{
    char *colorStr = (char *)data;

    smf_BracketNamedColor(colorStr);
}
static smf_CM_textSub(void *data)
{
    smfEdit_currentCommand = SMFEditCommand_AddCharAtCursor;
    estrConcatCharString(&smfEdit_currentCharactersToInsert, (char *)data);
    smf_SetSelectionCommand(SMFSelectionCommand_Delete);
}
static void smf_CM_colorAdapter(void* arg0)
{
    smf_CM_color((void *)arg0);
}

static void smf_CM_textSubAdapter(void* arg0)
{
    smf_CM_textSub((void *)arg0);
}

void smf_CreateContextMenu(void)
{
    if (!smf_Interact_contextColorSubMenu)
    {
        smf_Interact_contextColorSubMenu = contextMenu_Create(NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "red", "SMFCMColorRed", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "orange", "SMFCMColorOrange", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "yellow", "SMFCMColorYellow", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "palegreen", "SMFCMColorPaleGreen", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "green", "SMFCMColorGreen", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "skyblue", "SMFCMColorSkyBlue", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "royalblue", "SMFCMColorRoyalBlue", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "indigo", "SMFCMColorIndigo", NULL);
        contextMenu_addCode(smf_Interact_contextColorSubMenu, smf_CM_canColor, 0, smf_CM_colorAdapter, "violet", "SMFCMColorViolet", NULL);
    }

    if (!smf_Interact_contextScaleSubMenu)
    {
        smf_Interact_contextScaleSubMenu = contextMenu_Create(NULL);
        contextMenu_addCode(smf_Interact_contextScaleSubMenu, smf_CM_canBoldItalicScale, 0, smf_CM_scaleSmall, 0, "SMFCMScaleSmall", NULL);
        contextMenu_addCode(smf_Interact_contextScaleSubMenu, smf_CM_canBoldItalicScale, 0, smf_CM_scaleMedium, 0, "SMFCMScaleMedium", NULL);
        contextMenu_addCode(smf_Interact_contextScaleSubMenu, smf_CM_canBoldItalicScale, 0, smf_CM_scaleLarge, 0, "SMFCMScaleLarge", NULL);
    }

    if (!smf_Interact_contextTextSubMenu)
    {
        smf_Interact_contextTextSubMenu = contextMenu_Create(NULL);
        contextMenu_addCode(smf_Interact_contextTextSubMenu, smf_CM_canTextSub, 0, smf_CM_textSubAdapter, "$name", "SMFCMTextSubName", NULL);
        contextMenu_addCode(smf_Interact_contextTextSubMenu, smf_CM_canTextSub, 0, smf_CM_textSubAdapter, "$class", "SMFCMTextSubClass", NULL);
        contextMenu_addCode(smf_Interact_contextTextSubMenu, smf_CM_canTextSub, 0, smf_CM_textSubAdapter, "$origin", "SMFCMTextSubOrigin", NULL);
        contextMenu_addCode(smf_Interact_contextTextSubMenu, smf_CM_canTextSub, 0, smf_CM_textSubAdapter, "$level", "SMFCMTextSubLevel", NULL);
        contextMenu_addCode(smf_Interact_contextTextSubMenu, smf_CM_canTextSub, 0, smf_CM_textSubAdapter, "$supergroup", "SMFCMTextSubSuperGroup", NULL);
    }

    if (!smf_Interact_contextMenu)
    {
        smf_Interact_contextMenu = contextMenu_Create(NULL);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canCut, 0, smf_CM_cut, 0, "SMFCMCut", NULL);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canCopy, 0, smf_CM_copy, 0, "SMFCMCopy", NULL);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canPaste, 0, smf_CM_paste, 0, "SMFCMPaste", NULL);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canBoldItalicScale, 0, smf_CM_bold, 0, "SMFCMBold", NULL);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canBoldItalicScale, 0, smf_CM_italic, 0, "SMFCMItalic", NULL);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canBoldItalicScale, 0, NULL, 0, "SMFCMScale", smf_Interact_contextScaleSubMenu);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canColor, 0, NULL, 0, "SMFCMColor", smf_Interact_contextColorSubMenu);
        contextMenu_addCode(smf_Interact_contextMenu, smf_CM_canTextSub, 0, NULL, 0, "SMFCMTextSub", smf_Interact_contextTextSubMenu);
    }
}

/**********************************************************************func*
* smf_Navigate
*
* Handles default SMF anchor onClick functionality.
* Also called by SelectionCallback...
*/
bool smf_Navigate(char *pch)
{
    if(strnicmp(pch, "cmd:", 4)!=0)
    {
        return false;
    }

    pch+=4;
    cmdParse(pch);

    return true;
}

static void smf_SwitchBreakingToNonBreakingSpace(SMFBlock *pSMFBlock, unsigned int rawIndex)
{
    if (pSMFBlock->rawString[rawIndex] == ' ' && (rawIndex && pSMFBlock->rawString[rawIndex - 1] == ' ') )
    {
        estrRemove(&pSMFBlock->rawString, rawIndex - 1, 1);

        // Character names and global chat handles can't have more than one
        // space in a row so there's no need to replace the one we just
        // removed.
        if (pSMFBlock->inputMode != SMFInputMode_PlayerNames &&
            pSMFBlock->inputMode != SMFInputMode_PlayerNamesAndHandles)
        {
            estrInsert(&pSMFBlock->rawString, rawIndex - 1, "&nbsp;", 6);
        }
    }
}

bool smf_IsAnythingSelected(void)
{
    bool retval = false;

    // The start is the first character in the selection and the end is the
    // first character NOT in the selection.
    if (smf_selectionStartPane != smf_selectionEndPane ||
        smf_selectionStartDisplayIndex != smf_selectionEndDisplayIndex)
    {
        retval = true;
    }

    return retval;
}

static void smf_DetermineSelectedSection(SMBlock *pBlock)
{
    if (!pBlock || (!TAG_MATCHES(sm_text) && pBlock->iType != -1) || smf_GetSelectionCommand() == SMFSelectionCommand_None)
    {
        return;
    }

    if (TAG_MATCHES(sm_text))
    {
        int pBlockTextLength = smBlock_GetDisplayLength(pBlock);

        if (smf_Interact_selectionStartRawIndex == -1)
        {
            // Not smf_HasFocus2() because that requires it to be the end of the selection.
            int relativeIndex = smf_selectionStartDisplayIndex - pBlock->displayStringStartIndex;
            if (relativeIndex >= 0 && relativeIndex <= pBlockTextLength)
            {
                smf_Interact_selectionStartRawIndex = smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionStartDisplayIndex);
            }
        }

        if (smf_Interact_selectionEndRawIndex == -1)
        {
            int relativeIndex = smf_selectionEndDisplayIndex - pBlock->displayStringStartIndex;
            if (relativeIndex >= 0 && relativeIndex <= pBlockTextLength)
            {
                smf_Interact_selectionEndRawIndex = smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex);
            }
        }
    }

    if(pBlock->bHasBlocks)
    {
        int i;
        int iSize = eaSize(&pBlock->ppBlocks);
        for(i = 0; i < iSize; i++)
        {
            smf_DetermineSelectedSection(pBlock->ppBlocks[i]);
        }
    }
}

static void smf_removeLeadingWhiteSpace(char **rawString)
{
    // continue to remove all leading whitespace, it gets skipped by the renderer and leads to problems if we leave it laying around
    int bFoundWhiteSpace = 1;

    while ( estrLength(rawString) && bFoundWhiteSpace)
    {
        bFoundWhiteSpace = 0;
        if (strnicmp(*rawString, "&nbsp;", 6)==0 )
        {
            estrRemove(rawString, 0, 6);
            bFoundWhiteSpace = 1;
        }
        if( (*rawString)[0] == ' ' )
        {
            estrRemove(rawString, 0, 1);
            bFoundWhiteSpace = 1;
        }
    }    
}

static void smf_modifyRawStringSection(char **rawString, int prePos, int postPos)
{
    if (!rawString)
        return;

    switch (smf_GetSelectionCommand())
    {
        case SMFSelectionCommand_Delete:
            {
                estrRemove(rawString, prePos + 1, postPos - prePos);
                smf_removeLeadingWhiteSpace(rawString);
            }
            break;

        case SMFSelectionCommand_InsertTags:
        {
            char *preTag = smf_GetSelectionPreTag();
            char *postTag = smf_GetSelectionPostTag();

            if (postPos > prePos)
            {
                estrInsert(rawString, postPos + 1, postTag, strlen(postTag));
                estrInsert(rawString, prePos + 1, preTag, strlen(preTag));
            }

            break;
        }

        case SMFSelectionCommand_None:
        default:
            break;
    }
}

static void smf_UpdateSelectionInRawString(SMFBlock *pSMFBlock)
{
    int deletedEarlyIndex, deletedLateIndex, workingEarlyIndex, workingLateIndex, workingTagEnd;

    if (smf_GetSelectionCommand() == SMFSelectionCommand_None || !smf_IsBlockEditable(pSMFBlock) || !smfBlock_HasFocus(pSMFBlock))
    {
        return;
    }

    // I'm assuming that both the start and end positions are within this SMFBlock.
    // I don't think I can do this under the "process one SMFBlock at a time" algorithm
    // if I don't make this assumption.

    deletedEarlyIndex = min(smf_Interact_selectionStartRawIndex, smf_Interact_selectionEndRawIndex);
    deletedLateIndex = max(smf_Interact_selectionStartRawIndex, smf_Interact_selectionEndRawIndex);

    workingEarlyIndex = workingLateIndex = deletedLateIndex;

    while (workingLateIndex >= deletedEarlyIndex)
    {
        int tagIndex;

        while (pSMFBlock->rawString[workingEarlyIndex] != '>' && workingEarlyIndex >= deletedEarlyIndex)
        {
            workingEarlyIndex--;
        }

        smf_modifyRawStringSection(&pSMFBlock->rawString, workingEarlyIndex, workingLateIndex - 1);

        pSMFBlock->editedThisFrame = true;
        smf_SwitchBreakingToNonBreakingSpace(pSMFBlock, workingEarlyIndex + 1);

        workingTagEnd = workingLateIndex = workingEarlyIndex;

        while (pSMFBlock->rawString[workingEarlyIndex] != '<' && workingEarlyIndex >= deletedEarlyIndex)
        {
            if (pSMFBlock->rawString[workingEarlyIndex] == ' ' ||
                pSMFBlock->rawString[workingEarlyIndex] == '\r' ||
                pSMFBlock->rawString[workingEarlyIndex] == '\n' ||
                pSMFBlock->rawString[workingEarlyIndex] == '\t')
            {
                workingTagEnd = workingEarlyIndex;
            }
            workingEarlyIndex--;
        }

        tagIndex = sm_ParseTagName(&pSMFBlock->rawString[workingEarlyIndex + 1], workingTagEnd - workingEarlyIndex - 1, smf_aTagDefs);
        if (tagIndex == -1)
        {
            smf_modifyRawStringSection(&pSMFBlock->rawString, workingEarlyIndex, workingLateIndex);
            smf_SwitchBreakingToNonBreakingSpace(pSMFBlock, workingEarlyIndex);
        }
        else if (tagIndex == 41) // 41 is <br>)
        {
            smf_modifyRawStringSection(&pSMFBlock->rawString, workingEarlyIndex-1, workingLateIndex);
            smf_SwitchBreakingToNonBreakingSpace(pSMFBlock, workingEarlyIndex);
        }
        workingLateIndex = workingEarlyIndex;
    }

    smf_selectionStartDisplayIndex = min(smf_selectionStartDisplayIndex, smf_selectionEndDisplayIndex);
    smf_selectionEndDisplayIndex = smf_selectionStartDisplayIndex;
    smf_selectionEndCharacterXOffset = -1;
    smf_selectionEndCharacterYOffset = -1;
    smf_selectionEndCharacterYHeight = -1;
}

/**********************************************************************func*
* smf_DetermineIndexOfClosestCharacter
*
* Returns the index into the string contained in pBlock that is 
* the closest cursor position to the coordinates defined by xPos and yPos.
* The position of pBlock itself is defined by pBlock, iXBase, and iYBase.
* The index is in "relative display space", relative meaning the start of
* the SMBlock is set to zero.
*/
static int smf_DetermineRelativeDisplayIndexOfClosestCharacter(SMBlock* pBlock, TextAttribs *pattrs, int iXBase, int iYBase, float xPos, float yPos)
{
    TTDrawContext *pttFont;
    TTFontRenderParams rp;
    float fontScale;
    int fontHeight;
    unsigned short *lineText;
    float *characterWidths;
    int characterWidthsSize;
    float x = iXBase + pBlock->pos.iX, x1 = 0, x2 = 0;
    int clickedCharacterIndex;

    if (!TAG_MATCHES(sm_text))
    {
        return 0;
    }

    smf_MakeFont(&pttFont, &rp, pattrs);
    fontScale = smBlock_CalcFontScale(pBlock, pattrs);
    fontHeight = ttGetFontHeight(pttFont, fontScale, fontScale);
    lineText = (unsigned short *)pBlock->pv;
    characterWidths = smf_GetCharacterWidths(pBlock, pattrs);

    characterWidthsSize = eafSize(&characterWidths);
    for (clickedCharacterIndex = 0; clickedCharacterIndex < characterWidthsSize; clickedCharacterIndex++)
    {
        x += characterWidths[clickedCharacterIndex];
        if (x > xPos)
        {
            // Record the two glyph boundaries that the mouse click fell within.
            x1 = x - characterWidths[clickedCharacterIndex];
            x2 = x;
            break;
        }
    }

    // If we're closer to the end than the beginning of the character, move the cursor to after the character.
    if (xPos - x1 >= x2 - xPos && clickedCharacterIndex < characterWidthsSize)
    {
        clickedCharacterIndex++;
    }

    eafDestroy(&characterWidths);

    // Reset the font we used.
    pttFont->renderParams = rp;

    return clickedCharacterIndex;
}

/**********************************************************************func*
* smf_InteractWithSelection
*
* This function changes the state of the selection due to interaction with the user.
*/
static void smf_InteractWithMouse(SMBlock *pBlock, TextAttribs *pattrs, CBox *box, int iXBase, int iYBase, int iZBase)
{
    // Stop us from changing the selection as a result of clicks on a context
    // menu.
    if (contextMenu_IsActive())
    {
        return;
    }

    if (!TAG_MATCHES(sm_ws) && !TAG_MATCHES(sm_text))
    {
        return;
    }

    if (!smf_IsBlockSelectable(pBlock->pSMFBlock))
    {
        // Not sure if this is correct...
        // I'm worried that this will cause some strange interaction with the mouse...
        return;
    }

    if (mouseCollision(box) && pBlock->pSMFBlock)
    {
        // eat the SMFBlock's click and drag if we're directly over a SMBlock...
        // let the SMBlock code handle it.
        pBlock->pSMFBlock->clickedOnThisFrame = false;
        pBlock->pSMFBlock->draggedOnThisFrame = false;
    }

    if (pBlock->pSMFBlock && pBlock->pSMFBlock->draggedOnThisFrame)
    {
        bool thisBlockIsBetterThanTheCurrent = false;
        int thisSMBlockXDiff, thisSMBlockYDiff;
        
        float mouseX, mouseY;
        mouseX = gMouseInpCur.x;
        mouseY = gMouseInpCur.y;
        reverseToScreenScalingFactorf(&mouseX, &mouseY);

        thisSMBlockYDiff = max(max(0, (iYBase + pBlock->pos.iY) - mouseY), max(0, mouseY - (iYBase + pBlock->pos.iY + pBlock->pos.iHeight)));
        thisSMBlockXDiff = max(max(0, (iXBase + pBlock->pos.iX) - mouseX), max(0, mouseX - (iXBase + pBlock->pos.iX + pBlock->pos.iWidth)));

        if (thisSMBlockYDiff < smf_Interact_currentBestSMBlockYDiff || smf_Interact_currentBestSMBlockYDiff == -1)
        {
            thisBlockIsBetterThanTheCurrent = true;
        }
        else if (thisSMBlockYDiff > smf_Interact_currentBestSMBlockYDiff)
        {
            thisBlockIsBetterThanTheCurrent = false;
        }
        else
        {
            // Same Y diff, check X

            if (thisSMBlockXDiff < smf_Interact_currentBestSMBlockXDiff || smf_Interact_currentBestSMBlockXDiff == -1)
            {
                thisBlockIsBetterThanTheCurrent = true;
            }
            else
            {
                thisBlockIsBetterThanTheCurrent = false;
            }
        }

        if (thisBlockIsBetterThanTheCurrent)
        {
            //drawBox(box, iZBase, 0x6666ffff, 0);
            if (mouseX > iXBase + pBlock->pos.iX)
            {
                smf_Interact_currentBestSMBlockDisplayIndex = pBlock->displayStringStartIndex + smBlock_GetDisplayLength(pBlock);
            }
            else
            {
                smf_Interact_currentBestSMBlockDisplayIndex = pBlock->displayStringStartIndex;
            }
            smf_Interact_currentBestSMBlockXDiff = thisSMBlockXDiff;
            smf_Interact_currentBestSMBlockYDiff = thisSMBlockYDiff;
        }
    }

    if (!smf_isCurrentlyDragging)
    {
        if (isDown(MS_LEFT) && !mouseDragging() && !gScrollBarDraggingLastFrame)
        {
            if ((pBlock->pSMFBlock && pBlock->pSMFBlock->scissorsBox &&
                 !mouseCollision(pBlock->pSMFBlock->scissorsBox)) ||
                !mouseCollision(box))
            {
                if (!smf_stopSettingTheSelectionThisFrame)
                {
                    if (smf_selectionEndPane)
                    {
                        smf_ReturnFocus();
                    }

                    if (smf_printDebugLines)
                    {
                        printf("smf_InteractWithMouse(): Resetting the SMF selection to null.\n");
                    }

                    smf_selectionStartPane = 0;
                    smf_selectionStartDisplayIndex = 0;
                    smf_selectionStartYBase = 0;
                    
                    smf_selectionEndPane = 0;
                    smf_selectionEndDisplayIndex = 0;
                    smf_selectionEndXBase = 0;
                    smf_selectionEndYBase = 0;
                    smf_selectionEndCharacterXOffset = 0;
                    smf_selectionEndCharacterYOffset = 0;
                    smf_selectionEndCharacterYHeight = 0;

                     // turns off the "drag outside of SMFBlock" searching such that it doesn't copy that search over.
                    smfSearch_currentCommand = SMFSearchCommand_None;
                }
            }
            else
            {
                if (!smf_stopSettingTheSelectionThisFrame)
                {
                    float mouseX, mouseY;
                    mouseX = gMouseInpCur.x;
                    mouseY = gMouseInpCur.y;
                    reversePointToScreenScalingFactorf(&mouseX, &mouseY);

                    smf_TakeFocus(pBlock->pSMFBlock);

                    if (smf_printDebugLines)
                    {
                        printf("smf_InteractWithMouse(): Setting the SMF selection to a new starting location.\n");
                    }

                    smf_selectionStartPane = pBlock->pSMFBlock;
                    if (smf_IsBlockScrollable(pBlock->pSMFBlock))
                    {
                        smf_selectionStartDisplayIndex = pBlock->displayStringStartIndex + smf_DetermineRelativeDisplayIndexOfClosestCharacter(pBlock, pattrs, iXBase + pBlock->pSMFBlock->currentDisplayOffsetX, iYBase + pBlock->pSMFBlock->currentDisplayOffsetY, mouseX, mouseY);
                    }
                    else
                    {
                        smf_selectionStartDisplayIndex = pBlock->displayStringStartIndex + smf_DetermineRelativeDisplayIndexOfClosestCharacter(pBlock, pattrs, iXBase, iYBase, mouseX, mouseY);
                    }
                    smf_selectionStartYBase = iYBase;

                    smf_selectionEndPane = pBlock->pSMFBlock;
                    smf_selectionEndDisplayIndex = smf_selectionStartDisplayIndex;
                    smf_selectionEndXBase = iXBase;
                    smf_selectionEndYBase = iYBase;
                    smf_selectionEndCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionEndDisplayIndex - pBlock->displayStringStartIndex);
                    smf_selectionEndCharacterYOffset = pBlock->pos.iY;
                    smf_selectionEndCharacterYHeight = pBlock->pos.iHeight;

                    // turns off the "drag outside of SMFBlock" searching such that it doesn't copy that search over.
                    smfSearch_currentCommand = SMFSearchCommand_None;
                }
                smf_isCurrentlyDragging = true;
            }
        }
    }
    else
    {
        if (mouseCollision(box) && !mouseDragging() && 
            !gScrollBarDraggingLastFrame && smfBlock_HasFocus(pBlock->pSMFBlock) &&
            (!pBlock->pSMFBlock->scissorsBox || mouseCollision(pBlock->pSMFBlock->scissorsBox)))
        {
            if (!smf_stopSettingTheSelectionThisFrame)
            {
                float mouseX, mouseY;
                mouseX = gMouseInpCur.x;
                mouseY = gMouseInpCur.y;
                reversePointToScreenScalingFactorf(&mouseX, &mouseY);

                if (!smfBlock_HasFocus(pBlock->pSMFBlock))
                {
                    smf_TakeFocus(pBlock->pSMFBlock);
                }

                if (smf_printDebugLines)
                {
                    printf("smf_InteractWithMouse(): Moving the SMF selection's ending location.\n\tStarting location is %d\n", smf_selectionStartDisplayIndex);
                }

                smf_selectionEndPane = pBlock->pSMFBlock;
                if (smf_IsBlockScrollable(pBlock->pSMFBlock))
                {
                    smf_selectionEndDisplayIndex = pBlock->displayStringStartIndex + smf_DetermineRelativeDisplayIndexOfClosestCharacter(pBlock, pattrs, iXBase + pBlock->pSMFBlock->currentDisplayOffsetX, iYBase + pBlock->pSMFBlock->currentDisplayOffsetY, mouseX, mouseY);
                }
                else
                {
                    smf_selectionEndDisplayIndex = pBlock->displayStringStartIndex + smf_DetermineRelativeDisplayIndexOfClosestCharacter(pBlock, pattrs, iXBase, iYBase, mouseX, mouseY);
                }
                smf_selectionEndXBase = iXBase;
                smf_selectionEndYBase = iYBase;
                smf_selectionEndCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionEndDisplayIndex - pBlock->displayStringStartIndex);
                smf_selectionEndCharacterYOffset = pBlock->pos.iY;
                smf_selectionEndCharacterYHeight = pBlock->pos.iHeight;

                // turns off the "drag outside of SMFBlock" searching such that it doesn't copy that search over.
                smfSearch_currentCommand = SMFSearchCommand_None;
            }
        }

        if (!isDown(MS_LEFT))
        {
            if (smf_printDebugLines)
            {
                printf("smf_InteractWithMouse(): Turning off smf_isCurrentlyDragging.\n");
            }

            smf_isCurrentlyDragging = false;

            // turns off the "drag outside of SMFBlock" searching such that it doesn't copy that search over.
            smfSearch_currentCommand = SMFSearchCommand_None;
        }

        if (mouseDragging() || gScrollBarDraggingLastFrame)
        {
            smf_ClearSelection();
        }
    }
}

static void smBlock_InteractWithSearch(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase)
{
    unsigned int pBlockDisplayLength;

    if (!TAG_MATCHES(sm_text) || !smfBlock_HasFocus(pBlock->pSMFBlock))
    {
        return;
    }

    pBlockDisplayLength = smBlock_GetDisplayLength(pBlock);

    if (smfSearch_currentCommand == SMFSearchCommand_None)
    {
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_WordAboveSelectedEnd)
    {
        if (iYBase + pBlock->pos.iY >= smf_selectionEndYBase + smf_selectionEndCharacterYOffset)
        {
            // Current block is not above selection's end block.
            return;
        }

        if (smf_selectionSearchPane &&
            iYBase + pBlock->pos.iY < smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset)
        {
            // Current block is above the current search solution block.
            return;
        }

        if (pBlock->pos.iX > smf_selectionEndCharacterXOffset &&
            (smf_selectionSearchPane && iYBase + pBlock->pos.iY == smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset))
        {
            // Current block is after the selection's end block, but not below the current search solution block.
            return;
        }

        if (smf_selectionSearchPane &&
            iYBase + pBlock->pos.iY == smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset &&
            pBlock->pos.iX <= smf_selectionSearchCharacterXOffset)
        {
            // Current block is before the current search solution block.
            return;
        }

        // Current block is a better solution.
        smf_selectionSearchPane = pBlock->pSMFBlock;
        smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex + smf_DetermineRelativeDisplayIndexOfClosestCharacter(pBlock, pattrs, iXBase, iYBase, iXBase + smf_selectionEndCharacterXOffset, iYBase);
        smf_selectionSearchXBase = iXBase;
        smf_selectionSearchYBase = iYBase;
        smf_selectionSearchCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionSearchDisplayIndex - pBlock->displayStringStartIndex);
        smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

        // All movement puts the cursor back to the start of its 'on' phase.
        smfCursor_display = true;
        smfCursor_currentTimer = 0.0f;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_WordBelowSelectedEnd)
    {
        if (iYBase + pBlock->pos.iY <= smf_selectionEndYBase + smf_selectionEndCharacterYOffset)
        {
            // Current block is not below selection's end block.
            return;
        }

        if (smf_selectionSearchPane &&
            iYBase + pBlock->pos.iY > smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset)
        {
            // Current block is below the current search solution block.
            return;
        }

        if (pBlock->pos.iX > smf_selectionEndCharacterXOffset &&
            (smf_selectionSearchPane && iYBase + pBlock->pos.iY == smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset))
        {
            // Current block is after the selection's end block, but not above the current search solution block.
            return;
        }

        if (smf_selectionSearchPane &&
            iYBase + pBlock->pos.iY == smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset &&
            pBlock->pos.iX <= smf_selectionSearchCharacterXOffset)
        {
            // Current block is before the current search solution block.
            return;
        }

        // Current block is a better solution.
        smf_selectionSearchPane = pBlock->pSMFBlock;
        smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex + smf_DetermineRelativeDisplayIndexOfClosestCharacter(pBlock, pattrs, iXBase, iYBase, iXBase + smf_selectionEndCharacterXOffset, iYBase);
        smf_selectionSearchXBase = iXBase;
        smf_selectionSearchYBase = iYBase;
        smf_selectionSearchCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionSearchDisplayIndex - pBlock->displayStringStartIndex);
        smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

        // All movement puts the cursor back to the start of its 'on' phase.
        smfCursor_display = true;
        smfCursor_currentTimer = 0.0f;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_EndOfLineBeforeSelectedEndLine)
    {
        if (iYBase + pBlock->pos.iY >= smf_selectionEndYBase + smf_selectionEndCharacterYOffset)
        {
            // Current block is equal to or after the selection's ending line.
            return;
        }

        if (smf_selectionSearchPane &&
            (((iYBase + pBlock->pos.iY == smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset) &&
              (pBlock->pos.iX <= smf_selectionSearchCharacterXOffset)) ||
             (iYBase + pBlock->pos.iY < smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset)))
        {
            // Current block is equal to or before the current search solution block.
            return;
        }

        // Current block is a better solution.
        smf_selectionSearchPane = pBlock->pSMFBlock;
        smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex + pBlockDisplayLength;
        smf_selectionSearchXBase = iXBase;
        smf_selectionSearchYBase = iYBase;
        smf_selectionSearchCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, pBlockDisplayLength);
        smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

        // All movement puts the cursor back to the start of its 'on' phase.
        smfCursor_display = true;
        smfCursor_currentTimer = 0.0f;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_StartOfSelectedEndLine)
    {
        if (iYBase + pBlock->pos.iY != smf_selectionEndYBase + smf_selectionEndCharacterYOffset)
        {
            // Current block is not on the selection's ending line.
            return;
        }

        if (smf_selectionSearchPane &&
            pBlock->pos.iX >= smf_selectionSearchCharacterXOffset)
        {
            // Current block is equal to or after the current search solution block.
            return;
        }

        // Current block is a better solution.
        smf_selectionSearchPane = pBlock->pSMFBlock;
        smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
        smf_selectionSearchXBase = iXBase;
        smf_selectionSearchYBase = iYBase;
        smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
        smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

        // All movement puts the cursor back to the start of its 'on' phase.
        smfCursor_display = true;
        smfCursor_currentTimer = 0.0f;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_EndOfSelectedEndLine)
    {
        if (iYBase + pBlock->pos.iY != smf_selectionEndYBase + smf_selectionEndCharacterYOffset)
        {
            // Current block is not on the selection's ending line.
            return;
        }

        if (smf_selectionSearchPane &&
            pBlock->pos.iX <= smf_selectionSearchCharacterXOffset)
        {
            // Current block is equal to or before the current search solution block.
            return;
        }

        // Current block is a better solution.
        smf_selectionSearchPane = pBlock->pSMFBlock;
        smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex + pBlockDisplayLength;
        smf_selectionSearchXBase = iXBase;
        smf_selectionSearchYBase = iYBase;
        smf_selectionSearchCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, pBlockDisplayLength);
        smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

        // All movement puts the cursor back to the start of its 'on' phase.
        smfCursor_display = true;
        smfCursor_currentTimer = 0.0f;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_StartOfLineAfterSelectedEndLine)
    {
        if (iYBase + pBlock->pos.iY <= smf_selectionEndYBase + smf_selectionEndCharacterYOffset)
        {
            // Current block is equal to or before the selection's ending line.
            return;
        }

        if (smf_selectionSearchPane &&
            (((iYBase + pBlock->pos.iY == smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset) &&
            (pBlock->pos.iX >= smf_selectionSearchCharacterXOffset)) ||
            (iYBase + pBlock->pos.iY > smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset)))
        {
            // Current block is equal to or after the current search solution block.
            return;
        }

        // Current block is a better solution.
        smf_selectionSearchPane = pBlock->pSMFBlock;
        smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
        smf_selectionSearchXBase = iXBase;
        smf_selectionSearchYBase = iYBase;
        smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
        smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
        smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

        // All movement puts the cursor back to the start of its 'on' phase.
        smfCursor_display = true;
        smfCursor_currentTimer = 0.0f;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_NextTabbedBlock)
    {
        // Do nothing at the SMBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_PreviousTabbedBlock)
    {
        // Do nothing at the SMBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_PreviousBeginningOfWord)
    {
        if (pBlock->pSMFBlock == smf_selectionEndPane)
        {
            if (pBlock->pSMFBlock != smf_selectionSearchPane)
            {
                if (iYBase > smf_selectionSearchYBase && 
                    pBlock->displayStringStartIndex < smf_selectionEndDisplayIndex)
                {
                    smf_selectionSearchPane = pBlock->pSMFBlock;
                    smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
                    smf_selectionSearchXBase = iXBase;
                    smf_selectionSearchYBase = iYBase;
                    smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
                    smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
                    smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

                    // All movement puts the cursor back to the start of its 'on' phase.
                    smfCursor_display = true;
                    smfCursor_currentTimer = 0.0f;
                }
            }
            else
            {
                if (pBlock->displayStringStartIndex < smf_selectionEndDisplayIndex &&
                    (!smf_selectionSearchPane || 
                    pBlock->displayStringStartIndex > smf_selectionSearchDisplayIndex))
                {
                    smf_selectionSearchPane = pBlock->pSMFBlock;
                    smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
                    smf_selectionSearchXBase = iXBase;
                    smf_selectionSearchYBase = iYBase;
                    smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
                    smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
                    smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

                    // All movement puts the cursor back to the start of its 'on' phase.
                    smfCursor_display = true;
                    smfCursor_currentTimer = 0.0f;
                }
                else if (pBlock->displayStringStartIndex == smf_selectionEndDisplayIndex &&
                    !smf_selectionSearchPane)
                {
                    smf_selectionSearchPane = pBlock->pSMFBlock;
                    smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
                    smf_selectionSearchXBase = iXBase;
                    smf_selectionSearchYBase = iYBase;
                    smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
                    smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
                    smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;

                    // All movement puts the cursor back to the start of its 'on' phase.
                    smfCursor_display = true;
                    smfCursor_currentTimer = 0.0f;
                }
            }
        }
        else if (iYBase < smf_selectionEndYBase && 
            (!smf_selectionSearchPane || iYBase > smf_selectionSearchYBase ||
             (iYBase == smf_selectionSearchYBase && pBlock->displayStringStartIndex > smf_selectionSearchDisplayIndex)))
        {
            smf_selectionSearchPane = pBlock->pSMFBlock;
            smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex + smBlock_GetDisplayLength(pBlock);
            smf_selectionSearchXBase = iXBase;
            smf_selectionSearchYBase = iYBase;
            smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
            smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
            smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;    
            smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord = true;

            // All movement puts the cursor back to the start of its 'on' phase.
            smfCursor_display = true;
            smfCursor_currentTimer = 0.0f;
        }
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_NextBeginningOfWord)
    {
        if (pBlock->pSMFBlock == smf_selectionEndPane)
        {
            if (pBlock->displayStringStartIndex > smf_selectionEndDisplayIndex &&
                (!smf_selectionSearchPane || smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord ||
                 pBlock->displayStringStartIndex < smf_selectionSearchDisplayIndex))
            {
                smf_selectionSearchPane = pBlock->pSMFBlock;
                smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
                smf_selectionSearchXBase = iXBase;
                smf_selectionSearchYBase = iYBase;
                smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
                smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
                smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;
                smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord = false;

                // All movement puts the cursor back to the start of its 'on' phase.
                smfCursor_display = true;
                smfCursor_currentTimer = 0.0f;
            }
            else if (pBlock->displayStringStartIndex == smf_selectionEndDisplayIndex &&
                !smf_selectionSearchPane)
            {
                smf_selectionSearchPane = pBlock->pSMFBlock;
                smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex + smBlock_GetDisplayLength(pBlock);
                smf_selectionSearchXBase = iXBase;
                smf_selectionSearchYBase = iYBase;
                smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
                smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
                smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;    
                smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord = true;

                // All movement puts the cursor back to the start of its 'on' phase.
                smfCursor_display = true;
                smfCursor_currentTimer = 0.0f;
            }
        }
        else if (iYBase > smf_selectionEndYBase && 
            (!smf_selectionSearchPane || smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord || iYBase < smf_selectionSearchYBase ||
             (iYBase == smf_selectionSearchYBase && pBlock->displayStringStartIndex < smf_selectionSearchDisplayIndex)))
        {
            smf_selectionSearchPane = pBlock->pSMFBlock;
            smf_selectionSearchDisplayIndex = pBlock->displayStringStartIndex;
            smf_selectionSearchXBase = iXBase;
            smf_selectionSearchYBase = iYBase;
            smf_selectionSearchCharacterXOffset = pBlock->pos.iX;
            smf_selectionSearchCharacterYOffset = pBlock->pos.iY;
            smf_selectionSearchCharacterYHeight = pBlock->pos.iHeight;    
            smf_Interact_nextBeginningOfWord_selectionIsAtCurrentWord = false;

            // All movement puts the cursor back to the start of its 'on' phase.
            smfCursor_display = true;
            smfCursor_currentTimer = 0.0f;
        }
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_BlockNearestCursor)
    {
        // Do nothing at the SMBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_ClearSelection)
    {
        // Do nothing at the SMBlock level.
        return;
    }
}

static void smfBlock_InteractWithSearch(SMFBlock *pSMFBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase, int width, int height)
{
    // Don't return if !smf_HasFocus()... tabbing is across focus.

    if (smfSearch_currentCommand == SMFSearchCommand_None)
    {
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_WordAboveSelectedEnd)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_WordBelowSelectedEnd)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_EndOfLineBeforeSelectedEndLine)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_StartOfSelectedEndLine)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_EndOfSelectedEndLine)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_StartOfLineAfterSelectedEndLine)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_NextTabbedBlock)
    {
        float fScale = ((float)pattrs->piScale[eaiSize(&pattrs->piScale)-1]/SMF_FONT_SCALE);
        fScale *= ((pSMFBlock && pSMFBlock->masterScale) ? pSMFBlock->masterScale : 1.0f);

        if (smf_selectionEndPane->tabNavigationSetIndex == -1)
        {
            // Non-indexed tab navigation.
            if (pSMFBlock->tabNavigationSetName && smf_selectionEndPane->tabNavigationSetName &&
                !stricmp(pSMFBlock->tabNavigationSetName, smf_selectionEndPane->tabNavigationSetName) &&
                fScale > 0)
            {
                if (smfSearch_nextTabbedBlock_justSawEndPane)
                {
                    // The last SMFBlock that was interacted with was the selected block.
                    // That means this SMFBlock is the correct search block.
                    smf_selectionSearchPane = pSMFBlock;
                    smf_selectionSearchDisplayIndex = 0;
                    smf_selectionSearchXBase = iXBase;
                    smf_selectionSearchYBase = iYBase;
                    smf_selectionSearchCharacterXOffset = -1;
                    smf_selectionSearchCharacterYOffset = -1;
                    smf_selectionSearchCharacterYHeight = -1;

                    smfSearch_currentCommand = SMFSearchCommand_Done;
                    smfSearch_nextTabbedBlock_justSawEndPane = false;
                }
                if (!smf_selectionSearchPane)
                {
                    // Default to the first SMFBlock you find (tab from the last to the first).
                    smf_selectionSearchPane = pSMFBlock;
                    smf_selectionSearchDisplayIndex = 0;
                    smf_selectionSearchXBase = iXBase;
                    smf_selectionSearchYBase = iYBase;
                    smf_selectionSearchCharacterXOffset = -1;
                    smf_selectionSearchCharacterYOffset = -1;
                    smf_selectionSearchCharacterYHeight = -1;
                }
            }
            if (pSMFBlock == smf_selectionEndPane)
            {
                // Flag this SMFBlock so the next one will be marked as the selected block.
                smfSearch_nextTabbedBlock_justSawEndPane = true;
                // Don't bother with all the selectionSearch variables.
            }
        }
        else
        {
            // Indexed tab navigation.
            // NYI
        }
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_PreviousTabbedBlock)
    {
        float fScale = ((float)pattrs->piScale[eaiSize(&pattrs->piScale)-1]/SMF_FONT_SCALE);
        fScale *= ((pSMFBlock && pSMFBlock->masterScale) ? pSMFBlock->masterScale : 1.0f);

        if (smf_selectionEndPane->tabNavigationSetIndex == -1)
        {
            // Non-indexed tab navigation.
            if (pSMFBlock->tabNavigationSetName && smf_selectionEndPane->tabNavigationSetName &&
                !stricmp(pSMFBlock->tabNavigationSetName, smf_selectionEndPane->tabNavigationSetName) &&
                fScale > 0)
            {
                if (pSMFBlock == smf_selectionEndPane && smf_selectionSearchPane)
                {
                    // If smf_selectionSearchPane is null, we have the first legal item selected,
                    // so find the last legal item.
                    smfSearch_currentCommand = SMFSearchCommand_Done;
                }
                else
                {
                    smf_selectionSearchPane = pSMFBlock;
                    smf_selectionSearchDisplayIndex = 0;
                    smf_selectionSearchXBase = iXBase;
                    smf_selectionSearchYBase = iYBase;
                    smf_selectionSearchCharacterXOffset = -1;
                    smf_selectionSearchCharacterYOffset = -1;
                    smf_selectionSearchCharacterYHeight = -1;
                }
            }
        }
        else
        {
            // Indexed tab navigation.
            // NYI
        }
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_PreviousBeginningOfWord)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_NextBeginningOfWord)
    {
        // Do nothing at the SMFBlock level.
        return;
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_BlockNearestCursor)
    {
        unsigned int thisTopYDiff, thisBotYDiff, thisYDiff;
        unsigned int otherTopYDiff, otherBotYDiff, otherYDiff;
        float mouseX, mouseY;
        mouseX = gMouseInpCur.x;
        mouseY = gMouseInpCur.y;
        reversePointToScreenScalingFactorf(&mouseX, &mouseY);

        if (!smfBlock_HasFocus(pSMFBlock))
        {
            // Not allowed to select this block.
            return;
        }

        thisTopYDiff = max(0, iYBase - mouseY);
        thisBotYDiff = max(0, mouseY - (iYBase + height));
        thisYDiff = max(thisTopYDiff, thisBotYDiff);
        otherTopYDiff = max(0, smf_selectionSearchYBase - mouseY);
        otherBotYDiff = max(0, mouseY - (smf_selectionSearchYBase + smf_selectionSearchCharacterYOffset + smf_selectionSearchCharacterYHeight));
        otherYDiff = max(otherTopYDiff, otherBotYDiff);

        // I'm explicitly not worrying about x position in this code.
        // If we have multiple SMFBlocks that are coselectable and overlap in y position,
        // then this implementation doesn't function.

        if (!smf_selectionSearchPane || thisYDiff < otherYDiff)
        {
            if (thisTopYDiff > 0 || (thisBotYDiff == 0 && iXBase > mouseX))
            {
                // Set search to beginning of this SMFBlock.
                smf_selectionSearchPane = pSMFBlock;
                smf_selectionSearchDisplayIndex = 0;
                smf_selectionSearchXBase = iXBase;
                smf_selectionSearchYBase = iYBase;
                smf_selectionSearchCharacterXOffset = -1;
                smf_selectionSearchCharacterYOffset = -1;
                smf_selectionSearchCharacterYHeight = -1;
            }
            else
            {
                // Set search to end of this SMFBlock.
                smf_selectionSearchPane = pSMFBlock;
                smf_selectionSearchDisplayIndex = smfBlock_GetDisplayLength(pSMFBlock);
                smf_selectionSearchXBase = iXBase;
                smf_selectionSearchYBase = iYBase;
                smf_selectionSearchCharacterXOffset = -1;
                smf_selectionSearchCharacterYOffset = -1;
                smf_selectionSearchCharacterYHeight = -1;
            }
        }
    }
    else if (smfSearch_currentCommand == SMFSearchCommand_ClearSelection)
    {
        smf_ClearSelection();
        smfSearch_currentCommand = SMFSearchCommand_Done;
    }
}

static int smf_DetermineCursorOffsetFromAddedCharacter(SMBlock *pBlock, int defaultReturnValue)
{
    int finalReturnValue = defaultReturnValue;
    int rawStringLength = estrLength(&pBlock->pSMFBlock->rawString);
    int smf_selectionEndRawIndex = smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex) + smf_Interact_currentCursorToRawOffset;
//    int smf_selectionEndRawIndex = smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex);
    int rawIndexOfClosingCharacter, rawIndexOfOpeningCharacter, rawIndexOfEndOfTagName; // needs to be int to stop it from underflowing past 0...
    int tagIndex;

    // This check shouldn't be necessary, since this function is only called when you know
    // that the end of the selection is in pBlock, but it's here just to be safe...
    if (!smBlock_HasFocus(pBlock))
    {
        return defaultReturnValue;
    }

    /**********************
     * Check for SMF tags *
     **********************/

    rawIndexOfClosingCharacter = smf_selectionEndRawIndex;

    while (rawIndexOfClosingCharacter < rawStringLength && pBlock->pSMFBlock->rawString[rawIndexOfClosingCharacter] != '>')
    {
        if (pBlock->pSMFBlock->rawString[rawIndexOfClosingCharacter] == '<' &&
            rawIndexOfClosingCharacter != smf_selectionEndRawIndex)
        {
            // This cannot be a valid SMF tag, since we're opening a tag to the right of the inserted character.
            rawIndexOfClosingCharacter = rawStringLength;
        }
        rawIndexOfClosingCharacter++;
    }

    if (rawIndexOfClosingCharacter < rawStringLength)
    {
        // There was a closing angle bracket.
        rawIndexOfOpeningCharacter = rawIndexOfClosingCharacter - 1;
        rawIndexOfEndOfTagName = rawIndexOfClosingCharacter;

        while (rawIndexOfOpeningCharacter > -1 && pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] != '<')
        {
            if (pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] == '>')
            {
                // This cannot be a valid SMF tag, since there was a close before an open.
                rawIndexOfOpeningCharacter = -1;
            }
            if (pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] == ' ' ||
                pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] == '\r' ||
                pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] == '\n' ||
                pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] == '\t')
            {
                rawIndexOfEndOfTagName = rawIndexOfOpeningCharacter;
            }
            rawIndexOfOpeningCharacter--;
        }

        if (rawIndexOfOpeningCharacter != -1)
        {
            // There was an opening angle bracket.
            tagIndex = sm_ParseTagName(&pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter + 1], rawIndexOfEndOfTagName - rawIndexOfOpeningCharacter - 1, smf_aTagDefs);

            if (tagIndex != -1)
            {
                finalReturnValue = rawIndexOfOpeningCharacter - smf_selectionEndRawIndex;
            }
        }
    }

    /*****************************
     * Check for character codes *
     *****************************/

    rawIndexOfOpeningCharacter = smf_selectionEndRawIndex;

    while (rawIndexOfOpeningCharacter >= 0 && 
            pBlock->pSMFBlock->rawString[rawIndexOfOpeningCharacter] != '&')
    {
        rawIndexOfOpeningCharacter--;
    }

    if (rawIndexOfOpeningCharacter + 6 > smf_selectionEndRawIndex &&
        !strnicmp(pBlock->pSMFBlock->rawString + rawIndexOfOpeningCharacter, "&nbsp;", 6))
    {
        finalReturnValue = rawIndexOfOpeningCharacter - smf_selectionEndRawIndex + 1;
    }

    if (rawIndexOfOpeningCharacter + 4 > smf_selectionEndRawIndex &&
        !strnicmp(pBlock->pSMFBlock->rawString + rawIndexOfOpeningCharacter, "&lt;", 4))
    {
        finalReturnValue = rawIndexOfOpeningCharacter - smf_selectionEndRawIndex + 1;
    }

    if (rawIndexOfOpeningCharacter + 4 > smf_selectionEndRawIndex &&
        !strnicmp(pBlock->pSMFBlock->rawString + rawIndexOfOpeningCharacter, "&gt;", 4))
    {
        finalReturnValue = rawIndexOfOpeningCharacter - smf_selectionEndRawIndex + 1;
    }

    if (rawIndexOfOpeningCharacter + 5 > smf_selectionEndRawIndex &&
        !strnicmp(pBlock->pSMFBlock->rawString + rawIndexOfOpeningCharacter, "&amp;", 5))
    {
        finalReturnValue = rawIndexOfOpeningCharacter - smf_selectionEndRawIndex + 1;
    }

    if (rawIndexOfOpeningCharacter + 5 > smf_selectionEndRawIndex &&
        !strnicmp(pBlock->pSMFBlock->rawString + rawIndexOfOpeningCharacter, "&#37;", 5))
    {
        finalReturnValue = rawIndexOfOpeningCharacter - smf_selectionEndRawIndex + 1;
    }

    return finalReturnValue;
}

static void smf_DealWithNumberCommas(SMBlock *pBlock)
{
    if( pBlock->pSMFBlock->displayMode == SMFDisplayMode_NumbersWithCommas)
    { 
        int len = strlen(pBlock->pSMFBlock->rawString), index;
        char * start = pBlock->pSMFBlock->rawString;
        int cursorMoveDistance = 0;

        // first remove any existing commas
        while(start && *start)
        {
            if( *start == ',' ) 
            {
                estrRemove(&pBlock->pSMFBlock->rawString, start-pBlock->pSMFBlock->rawString, 1);
                cursorMoveDistance--;
            }

            start++;

        }

        // now from the rear, add commas every 3 chars (assuming everything is int here)
        len = strlen(pBlock->pSMFBlock->rawString);
        index = len-3;
        while( index > 0 )
        {
            estrInsert(&pBlock->pSMFBlock->rawString,index, ",", 1);
            index -= 3;
            cursorMoveDistance++;
        }
        if (smf_selectionStartPane && smf_selectionEndPane)
        {
            if( (int)smf_selectionEndDisplayIndex + cursorMoveDistance >= 0 )
                smf_selectionEndDisplayIndex += cursorMoveDistance;
            else
                smf_selectionEndDisplayIndex = 0;
 
            smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
        }
    }
}

// This isn't necessarily a "display space" character anymore...
// This char could be part of a UTF8 non-ASCII character.
static void smf_InteractWithAddedByte(SMBlock *pBlock, unsigned char inputByte)
{
    bool removePreviousCharacter = false;
    bool shouldAddCharacter = true;
    int cursorMoveDistance = 1;
    static char *addMe = NULL;

    // Shortcuts to read-only values that makes the code easier to read
    SMFInputMode inputMode = pBlock->pSMFBlock->inputMode;
    int inputLimit = pBlock->pSMFBlock->inputLimit;
    int smf_selectionEndRawIndex = smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex) + smf_Interact_currentCursorToRawOffset;
    unsigned int displayStringLength = smfBlock_GetDisplayLength(pBlock->pSMFBlock);

    if (!addMe)
        estrCreateEx(&addMe, 64);
    estrClear(&addMe);

    // Don't check for focus here, since HasFocus2 is dependent on the length of pBlock->pv,
    // which doesn't get updated during the interact loop.
    //if (!smf_HasFocus2(pBlock))

    // Cull characters depending on if the input mode allows it or not.
    if (inputMode == SMFInputMode_AnyTextNoTagsOrCodes)
    {
        if (inputByte == '\t' || inputByte == '\n' || inputByte == '\r')
        {
            // Don't allow these characters, SMF doesn't handle them well.
            shouldAddCharacter = false;
        }

        if (inputLimit > 0 && displayStringLength >= (unsigned int) inputLimit)
        {
            // Display character limit
            shouldAddCharacter = false;
        }

        if (inputLimit > 0 && pBlock->pSMFBlock->rawString && strlen(pBlock->pSMFBlock->rawString) >= (unsigned int) inputLimit * 4)
        {
            // Hacky raw character limit
            shouldAddCharacter = false;
        }

        if (inputByte == ' ' && 
            (pBlock->pSMFBlock->rawString[smf_selectionEndRawIndex] == ' ' ||
             pBlock->pSMFBlock->rawString[smf_selectionEndRawIndex - 1] == ' '))
        {
            estrInsert(&addMe, 0, "&nbsp;", 6);
        }
        else if (inputByte == '<')
        {
            estrInsert(&addMe, 0, "&lt;", 4);
        }
        else if (inputByte == '>')
        {
            estrInsert(&addMe, 0, "&gt;", 4);
        }
        else if (inputByte == '&')
        {
            estrInsert(&addMe, 0, "&amp;", 5);
        }
        else if (inputByte == '%')
        {
            estrInsert(&addMe, 0, "&#37;", 5);
        }
        else
        {
            estrInsert(&addMe, 0, &inputByte, 1);
        }

        if (inputByte >= 0x80 && inputByte <= 0xBF)
        {
            // This byte is a non-starting byte of a multi-byte UTF-8 sequence.
            cursorMoveDistance = 0;
        }
        else if (inputByte >= 0xC2 && inputByte <= 0xDF)
        {
            // This byte is the start of a 2-byte UTF-8 sequence.
            cursorMoveDistance = 0;
            smf_Interact_incrementCursorAfterThisManyMoreInsertions = 2;
            smf_Interact_thisManyInsertionsHavePassed = 0;
        }
        else if (inputByte >= 0xE0 && inputByte <= 0xEF)
        {
            // This byte is the start of a 3-byte UTF-8 sequence.
            cursorMoveDistance = 0;
            smf_Interact_incrementCursorAfterThisManyMoreInsertions = 3;
            smf_Interact_thisManyInsertionsHavePassed = 0;
        }
        else if (inputByte >= 0xF0 && inputByte <= 0xF4)
        {
            // This byte is the start of a valid 4-byte UTF-8 sequence.
            cursorMoveDistance = 0;
            smf_Interact_incrementCursorAfterThisManyMoreInsertions = 4;
            smf_Interact_thisManyInsertionsHavePassed = 0;
        }
    }
    else if (inputMode == SMFInputMode_NonnegativeIntegers)
    {
        int blockAsInt, tailInt;
        int index = smf_GetCursorIndex(); 
        char * str = pBlock->pSMFBlock->rawString;
        char * endStr = 0;
        char * tailStr = 0;
        char input_length[32];
        int i, number_count=0;

        INT64 newNumber, inputMult = 1; 

        if (inputByte < '0' || inputByte > '9')
        {
            // Illegal character for SMFInputMode_NonnegativeIntegers
            shouldAddCharacter = false;
        }

        // count number of digits, never any reason to exceed that
        itoa( inputLimit, input_length, 10 );
        for( i = strlen(str)-1; i >=0; i-- ) // get count without commas
        {
            if( str[i] >= '0' && str[i] <= '9' )
                number_count++;
        }

        if (inputLimit > 0 && number_count >= (int)strlen(input_length) )
        {
            // Display character limit
            shouldAddCharacter = false;
        }

        if( str )
            endStr = str + strlen(str)-1;

        // depending where the cursor is, it will make a difference on how much is added to number
        while( endStr && endStr - str >= index )
        {
            if( *endStr != ',' ) // if something besides commas got in here we have bigger problems 
            {
                if( inputMult == 1000000000)
                {
                    shouldAddCharacter = false;
                    break;
                }
                inputMult *= 10;
            }
            tailStr = endStr;
            endStr--;
        }
        
        blockAsInt = str ? atoi_ignore_nonnumeric(str) : 0;
        tailInt = tailStr?  atoi_ignore_nonnumeric(tailStr) : 0;

        blockAsInt -= tailInt;

        if( index ) // if the cursor is 0, then we are adding number to front
        {
            if( blockAsInt <= 214748364 ) // make sure multipliing this doesn't wrap
                blockAsInt *= 10;
            else
                shouldAddCharacter = false;
        }

        newNumber = blockAsInt;
        newNumber += ((int) inputByte - (int) '0')*inputMult;
        newNumber += tailInt;
        if( newNumber > 0x7fffffff )
            shouldAddCharacter = false;
        
        if (inputLimit > 0 && shouldAddCharacter && newNumber > inputLimit )
        {
            // Adding this character would make the value too high
            shouldAddCharacter = false;
        }

        if (newNumber == 0 && displayStringLength > 0)
        {
            if (inputByte == '0')
            {
                // Only allow one zero on a line
                shouldAddCharacter = false;
            }
            // Get rid of the leading zero
            removePreviousCharacter = true;
        }

        estrInsert(&addMe, 0, &inputByte, 1);
    }
    else if (inputMode == SMFInputMode_Alphanumeric
                || inputMode == SMFInputMode_PlayerNames
                || inputMode == SMFInputMode_PlayerNamesAndHandles)
    {
        shouldAddCharacter = false;

        // We allow digits, letters, dashes, apostrophes, periods and question
        // marks. We also allow spaces but only if there are non-spaces on
        // either side of them (so they can be at the end of the line at the
        // input stage). Handles can also contain an @ symbol but only at
        // the beginning of the line and if there are none in the existing
        // text so far.
        if ((inputByte >= '0' && inputByte <= '9') ||
            (inputByte >= 'A' && inputByte <= 'Z') ||
            (inputByte >= 'a' && inputByte <= 'z'))
        {
            shouldAddCharacter = true;
        }
        else if (inputMode != SMFInputMode_Alphanumeric
            && (inputByte == '\'' || inputByte == '-' || inputByte == '.' ||
                inputByte == '?'))
        {
            shouldAddCharacter = true;
        }
        else if (inputByte == ' ' && inputMode != SMFInputMode_Alphanumeric
                    && pBlock->pSMFBlock->rawString[smf_selectionEndRawIndex] != ' '
                    && (smf_selectionEndRawIndex == 0 || pBlock->pSMFBlock->rawString[smf_selectionEndRawIndex - 1] != ' '))
        {
            shouldAddCharacter = true;
        }
        else if (inputMode == SMFInputMode_PlayerNamesAndHandles &&
            inputByte == '@' && smf_selectionEndRawIndex == 0 &&
            strchr(pBlock->pSMFBlock->rawString, '@') == NULL)
        {
            shouldAddCharacter = true;
        }

        if (inputLimit > 0 && displayStringLength >= (unsigned int) inputLimit)
        {
            // Display character limit
            shouldAddCharacter = false;
        }

        if (inputLimit > 0 && pBlock->pSMFBlock->rawString && strlen(pBlock->pSMFBlock->rawString) >= (unsigned int) inputLimit * 4)
        {
            // Hacky raw character limit
            shouldAddCharacter = false;
        }

        if (shouldAddCharacter)
        {
            estrInsert(&addMe, 0, &inputByte, 1);
        }
    }

    if (shouldAddCharacter)
    {
        // Add (and remove if necessary) the characters.
        if (removePreviousCharacter && smf_selectionEndRawIndex > 0 )
        {
            estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 1, 1);
            estrInsert(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 1, addMe, estrLength(&addMe));
        }
        else
        {
            estrInsert(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, addMe, estrLength(&addMe));
        }
        pBlock->pSMFBlock->editedThisFrame = true;

        cursorMoveDistance = smf_DetermineCursorOffsetFromAddedCharacter(pBlock, cursorMoveDistance);

        if (smf_Interact_incrementCursorAfterThisManyMoreInsertions)
        {
            smf_Interact_thisManyInsertionsHavePassed++;
            if (smf_Interact_thisManyInsertionsHavePassed == smf_Interact_incrementCursorAfterThisManyMoreInsertions)
            {
                cursorMoveDistance++;
                smf_Interact_currentCursorToRawOffset -= smf_Interact_incrementCursorAfterThisManyMoreInsertions;
                smf_Interact_incrementCursorAfterThisManyMoreInsertions = 0;
            }
        }

        // This updates the value for the next smf_InteractWithAddedCharacter().
        pBlock->pSMFBlock->displayStringLength++;
        smf_Interact_currentCursorToRawOffset += (estrLength(&addMe) - 1);
        if (inputByte >= 0x80)
        {
            smf_Interact_currentCursorToRawOffset++;
        }

        // Handle moving the cursor.
        if (removePreviousCharacter)
        {
            cursorMoveDistance--;
        }
        if (smf_selectionStartPane && smf_selectionEndPane)
        {
            smf_selectionEndDisplayIndex += cursorMoveDistance;
            smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
        }

        if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
        {
            sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
        }

        smf_DealWithNumberCommas(pBlock);
    }
    else
    {
        if (pBlock->pSMFBlock->sound_failedKeyboardInput)
        {
            sndPlay(pBlock->pSMFBlock->sound_failedKeyboardInput, SOUND_PLAYER);
        }
    }
}

static void smf_PasteStringFromClipboard(SMBlock *pBlock)
{
    // This check shouldn't be necessary, since this function is only called when you know
    // that the end of the selection is in pBlock, but it's here just to be safe...
    if (!smBlock_HasFocus(pBlock))
    {
        return;
    }

    if (OpenClipboard(hwnd))
    {
        HANDLE handle = GetClipboardData(CF_UNICODETEXT);

        if (handle)
        {
            unsigned short *data = GlobalLock(handle);
            char *utf8_data;
            unsigned int index;
            index = WideToUTF8StrConvert(data, NULL, 0);
            utf8_data = malloc(index + 1);
            WideToUTF8StrConvert(data, utf8_data, index);
            for (index = 0; index < strlen(utf8_data); index++)
            {
                smf_InteractWithAddedByte(pBlock, utf8_data[index]);
            }
            free(utf8_data);
            GlobalUnlock(handle);

            pBlock->pSMFBlock->editedThisFrame = true;
            smfEdit_currentCommand = SMFEditCommand_Done;
        }

        CloseClipboard();
    }
}

static void smf_InteractWithEdit(SMBlock *pBlock, TextAttribs *pattrs)
{
    int pBlockDisplayLength;
    int smf_selectionEndRawIndex;

    if ((!TAG_MATCHES(sm_text) && !TAG_MATCHES(sm_ws)) || !smfBlock_HasFocus(pBlock->pSMFBlock) || !smf_IsBlockEditable(pBlock->pSMFBlock))
    {
        return;
    } 

    pBlockDisplayLength = smBlock_GetDisplayLength(pBlock);
    if (smBlock_HasFocus(pBlock))
    {
        smf_selectionEndRawIndex = smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex);
    }

    switch (smfEdit_currentCommand)
    {
    case SMFEditCommand_None:
        // do nothing;
    xcase SMFEditCommand_Done:
        // also do nothing;
    xcase SMFEditCommand_AddCharAtCursor:
        if (estrLength(&smfEdit_currentCharactersToInsert) && smBlock_HasFocus(pBlock))
        {
            unsigned int i;

            for (i = 0; i < estrLength(&smfEdit_currentCharactersToInsert); i++)
            {
                smf_InteractWithAddedByte(pBlock, smfEdit_currentCharactersToInsert[i]);
            }

            // We've used everything, so emptying the string lets us tell
            // whether anything was added between now and the next per-frame
            // pass.
            estrClear(&smfEdit_currentCharactersToInsert);

            smfEdit_currentCommand = SMFEditCommand_Done;
        }
    xcase SMFEditCommand_AddLineBreakAtCursor:
    {
        // Shortcuts to read-only values that makes the code easier to read
        SMFInputMode inputMode = pBlock->pSMFBlock->inputMode;
        int inputLimit = pBlock->pSMFBlock->inputLimit;
        unsigned int displayStringLength = smfBlock_GetDisplayLength(pBlock->pSMFBlock);

        if (inputMode != SMFInputMode_NonnegativeIntegers && 
            (inputLimit <= 0 || (displayStringLength < (unsigned int) inputLimit && 
                                    pBlock->pSMFBlock->rawString && 
                                    strlen(pBlock->pSMFBlock->rawString) < (unsigned int) inputLimit * 4)) &&
            smBlock_HasFocus(pBlock))
        {
            if (smf_IsBlockMultiline(pBlock->pSMFBlock))
            {
                estrInsert(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, "<br>", 4);
                smf_selectionEndDisplayIndex += 1;
                smf_selectionEndCharacterXOffset = -1;
                smf_selectionEndCharacterYOffset = -1;
                smf_selectionEndCharacterYHeight = -1;
                smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                pBlock->pSMFBlock->editedThisFrame = true;
            }

            if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
            {
                sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
            }

            smfEdit_currentCommand = SMFEditCommand_Done;
        }
    }
    xcase SMFEditCommand_RemoveCharBeforeCursor:
        if (smBlock_HasFocus(pBlock))
        {
            // if we are the beginning of our current block, we have to jump back one block
            if( smf_selectionEndDisplayIndex == pBlock->displayStringStartIndex && smf_selectionEndDisplayIndex > 0 && pBlock->pParent )
            {
                int i;

                 for( i = 1; i < eaSize(&pBlock->pParent->ppBlocks); i++ )
                {
                    if( pBlock == pBlock->pParent->ppBlocks[i] )
                    {
                        pBlock = pBlock->pParent->ppBlocks[i-1];
                        pBlockDisplayLength = smBlock_GetDisplayLength(pBlock);
                    }
                }
            }

            if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex - 4, "<br>", 4))
            {
                // Remove the line break.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 4, 4);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - 4);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex -= smf_aTagDefs[41].displayLength; // 41 is <br>
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex - 6, "&nbsp;", 6))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 6, 6);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - 6);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex -= 1;
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex - 4, "&lt;", 4))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 4, 4);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - 4);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex -= 1;
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex - 4, "&gt;", 4))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 4, 4);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - 4);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex -= 1;
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex - 5, "&amp;", 5))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 5, 5);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - 5);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex -= 1;
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex - 5, "&#37;", 5))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - 5, 5);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - 5);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex -= 1;
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (smf_selectionEndDisplayIndex > pBlock->displayStringStartIndex &&
                smf_selectionEndDisplayIndex <= pBlock->displayStringStartIndex + pBlockDisplayLength)
            {
                int rawCharacterLength = UTF8GetCharacterLength(pBlock->pSMFBlock->rawString + smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex - 1));

                if( pBlock->pSMFBlock->displayMode == SMFDisplayMode_NumbersWithCommas && pBlock->pSMFBlock->rawString[smf_selectionEndRawIndex - 1] == ',' )
                    smf_selectionEndRawIndex--;

                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex - rawCharacterLength, rawCharacterLength);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex - rawCharacterLength);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    smf_selectionEndDisplayIndex--;
                    smf_selectionEndCharacterXOffset = -1;
                    smf_selectionEndCharacterYOffset = -1;
                    smf_selectionEndCharacterYHeight = -1;
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                smf_DealWithNumberCommas(pBlock);

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else
            {
                if (pBlock->pSMFBlock->sound_failedKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_failedKeyboardInput, SOUND_PLAYER);
                }
            }
        }
        else if (smf_selectionEndDisplayIndex == 0)
        {
            if (pBlock->pSMFBlock->sound_failedKeyboardInput)
            {
                sndPlay(pBlock->pSMFBlock->sound_failedKeyboardInput, SOUND_PLAYER);
            }
        }
    xcase SMFEditCommand_RemoveCharAfterCursor:
        // First two check to ensure the cursor is within the bounds of the post-parse string,
        // Second two check to ensure the cursor can affect the text within this SMBlock.
        if (smBlock_HasFocus(pBlock))
        {
            if (!strncmp(smf_selectionEndPane->rawString + smf_selectionEndRawIndex, "<br>", 4))
            {
                // Remove the line break.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, 4);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the text from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex, "&nbsp;", 6))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, 6);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the text from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex, "&lt;", 4))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, 4);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the text from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex, "&gt;", 4))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, 4);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the text from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex, "&amp;", 5))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, 5);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the text from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (!strnicmp(pBlock->pSMFBlock->rawString + smf_selectionEndRawIndex, "&#37;", 5))
            {
                // Remove the character code.
                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, 5);
                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the text from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else if (smf_selectionEndDisplayIndex >= pBlock->displayStringStartIndex &&
                smf_selectionEndDisplayIndex < pBlock->displayStringStartIndex + pBlockDisplayLength)
            {
                 int rawCharacterLength = UTF8GetCharacterLength(pBlock->pSMFBlock->rawString + smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(pBlock, smf_selectionEndDisplayIndex));

                 if( pBlock->pSMFBlock->displayMode == SMFDisplayMode_NumbersWithCommas && pBlock->pSMFBlock->rawString[smf_selectionEndRawIndex] == ',' )
                    smf_selectionEndRawIndex++;

                estrRemove(&pBlock->pSMFBlock->rawString, smf_selectionEndRawIndex, rawCharacterLength);

                smf_SwitchBreakingToNonBreakingSpace(pBlock->pSMFBlock, smf_selectionEndRawIndex);
                if (smf_selectionStartPane && smf_selectionEndPane)
                {
                    // smf_selectionEndCharacterIndex doesn't change since we're removing the character from after the cursor.
                    smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                }

                smf_DealWithNumberCommas(pBlock);

                if (pBlock->pSMFBlock->sound_successfulKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_successfulKeyboardInput, SOUND_PLAYER);
                }

                pBlock->pSMFBlock->editedThisFrame = true;
                smfEdit_currentCommand = SMFEditCommand_Done;
            }
            else
            {
                if (pBlock->pSMFBlock->sound_failedKeyboardInput)
                {
                    sndPlay(pBlock->pSMFBlock->sound_failedKeyboardInput, SOUND_PLAYER);
                }
            }
        }
        else if (smf_selectionEndDisplayIndex == smfBlock_GetDisplayLength(pBlock->pSMFBlock))
        {
            if (pBlock->pSMFBlock->sound_failedKeyboardInput)
            {
                sndPlay(pBlock->pSMFBlock->sound_failedKeyboardInput, SOUND_PLAYER);
            }
        }
    xcase SMFEditCommand_PasteStringAtCursor:
        smf_PasteStringFromClipboard(pBlock);
        // don't reset smfEdit_currentCommand here, because it isn't guaranteed that
        // the first time you hit this clause will be the time that the string is pasted.
    xdefault:
        // do nothing
        break;
    }

    smf_removeLeadingWhiteSpace(&pBlock->pSMFBlock->rawString);

    if (smBlock_HasFocus(pBlock))
    {
        smf_selectionEndCharacterXOffset = pBlock->pos.iX + smf_DetermineLetterXOffset(pBlock, pattrs, smf_selectionEndDisplayIndex - pBlock->displayStringStartIndex);
    }
}

static void smBlock_InteractWithScroll(SMBlock *pBlock, int iXBase, int iYBase, int iZBase)
{
    if (smfEdit_currentCommand != SMFEditCommand_Done &&
        smfSearch_lastCommand == SMFSearchCommand_None &&
        smf_GetSelectionCommand() == SMFSelectionCommand_None)
    {
        // Don't force a scroll if the cursor hasn't (potentially) moved.
        return;
    }

    if (!smBlock_HasFocus(pBlock))
    {
        // Don't calculate if this isn't the block where the cursor is.
        return;
    }

    if (!pBlock->pSMFBlock->scissorsBox)
    {
        // Can't scroll if we don't have a box to scroll against.
        return;
    }

    if (pBlock->pSMFBlock->internalScrollDiff)
    {
        // Don't overwrite a real value.
        return;
    }

    pBlock->pSMFBlock->internalScrollDiff = MINMAX(0, 
            iYBase + pBlock->pos.iY + pBlock->pos.iHeight - pBlock->pSMFBlock->scissorsBox->lowerRight.y,
            iYBase + pBlock->pos.iY - pBlock->pSMFBlock->scissorsBox->upperLeft.y);

    // Don't change the internal scroll (for now)
}

/**********************************************************************func*
* smf_InteractWithSMBlock
*
* Handles all interaction between the user and the SMF system.
*/
static void smf_InteractWithSMBlock(SMBlock *pBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase, int (*callback)(char *pch))
{
    int startingBytesAlloced, endingBytesAlloced;

    if(!pBlock)
    {
        return;
    }
    
    if ((pBlock->pos.iWidth==0 || pBlock->pos.iX<0 || pBlock->pos.iY<0)    && pBlock->iType>kFormatTags_Count*2)
     {
        return;
    }

    PERFINFO_AUTO_START("smf_InteractWithSMBlock", 1);

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
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
        SMAnchor *pAnchor;

        BuildCBox(&box, pBlock->pos.iX + iXBase, pBlock->pos.iY + iYBase,
                        pBlock->pos.iWidth - 1, pBlock->pos.iHeight - 1);

        smf_InteractWithMouse(pBlock, pattrs, &box, iXBase, iYBase, iZBase);

        smBlock_InteractWithSearch(pBlock, pattrs, iXBase, iYBase, iZBase);

        smf_InteractWithEdit(pBlock, pattrs);

        smBlock_InteractWithScroll(pBlock, iXBase, iYBase, iZBase);

        if (!pBlock->pSMFBlock->scissorsBox || mouseCollision(pBlock->pSMFBlock->scissorsBox))
        {
            if (eaiSize(&pattrs->piAnchor) > 1 && s_bAllowAnchor)
            {
                // Although tracked, multiply nested anchors are ignored. Always use
                // the first anchor if available.
                pAnchor = (SMAnchor *)pattrs->piAnchor[1];

                if (mouseClickHit(&box, MS_LEFT))
                {
                    collisions_off_for_rest_of_frame = 1;
                    if(callback)
                    {
                        callback(pAnchor->ach);
                    }
                    else
                    {
                        smf_Navigate(pAnchor->ach);
                    }
                }
            }
        }
    }

    if(pBlock->bHasBlocks)
    {
        int i;
        int iSize = eaSize(&pBlock->ppBlocks);
        for(i=0; i<iSize; i++)
        {
            smf_InteractWithSMBlock(pBlock->ppBlocks[i], pattrs, iXBase, iYBase, iZBase, callback);
        }
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    PERFINFO_AUTO_STOP();
}

void smf_Interact(SMFBlock *pSMFBlock, TextAttribs *pattrs, int iXBase, int iYBase, int iZBase, int width, int height, int (*callback)(char *pch))
{
    CBox smfBox;

    pSMFBlock->internalScrollDiff = 0;
    pSMFBlock->editedThisFrame = false;

    smf_Interact_selectionStartRawIndex = -1;
    smf_Interact_selectionEndRawIndex = -1;
    smf_Interact_currentBestSMBlockDisplayIndex = -1;
    smf_Interact_currentBestSMBlockXDiff = -1;
    smf_Interact_currentBestSMBlockYDiff = -1;
    smf_Interact_currentCursorToRawOffset = 0;

    // Block delete/tag/etc needs to happen before any of the other interactions.
    if (smf_GetSelectionCommand() != SMFSelectionCommand_None && pSMFBlock == smf_selectionEndPane)
    {
        smf_DetermineSelectedSection(pSMFBlock->pBlock);
        smf_UpdateSelectionInRawString(pSMFBlock);
    }

    smfBlock_InteractWithSearch(pSMFBlock, pattrs, iXBase, iYBase, iZBase, width, height);

    BuildCBox(&smfBox, iXBase, iYBase, width, height);

    pSMFBlock->clickedOnThisFrame = mouseClickHit(&smfBox, MS_LEFT);
    pSMFBlock->rightClickedOnThisFrame = mouseClickHit(&smfBox, MS_RIGHT);
    pSMFBlock->draggedOnThisFrame = mouseCollision(&smfBox) && isDown(MS_LEFT) && 
                                    !mouseDragging() && !gScrollBarDraggingLastFrame &&
                                    (smfBlock_HasFocus(pSMFBlock) || !smf_selectionEndPane) &&
                                    smf_IsBlockSelectable(pSMFBlock);

    if (pSMFBlock->scissorsBox && !mouseCollision(pSMFBlock->scissorsBox))
    {
        // Don't interact with the mouse if it's outside of the scissors box.
        pSMFBlock->clickedOnThisFrame = false;
        pSMFBlock->rightClickedOnThisFrame = false;
        pSMFBlock->draggedOnThisFrame = false;
    }

    if (smf_drawDebugBoxes >= 1)
    {
        clipperPush(NULL);
        drawBox(&smfBox, iZBase, pSMFBlock->draggedOnThisFrame?0x33ff33ff:0x006600ff, 0);
        clipperPop();
    }
    
    smf_InteractWithSMBlock(pSMFBlock->pBlock, pattrs, iXBase, iYBase, iZBase, callback);

    if (pSMFBlock->clickedOnThisFrame)
    {
        // Didn't click on any of the SMBlocks.
        if (smf_IsBlockEditable(pSMFBlock)) // implies smf_IsBlockSelectable()
        {
            smf_SetCursor(pSMFBlock, smfBlock_GetDisplayLength(pSMFBlock));
            smf_stopSettingTheSelectionThisFrame = true;
        }
    }
    else if (pSMFBlock->draggedOnThisFrame)
    {
        if (smf_drawDebugBoxes >= 1)
        {
            clipperPush(NULL);
            drawBox(&smfBox, iZBase, 0x000000ff, 0); // This should never be the final displayed box.
            clipperPop();
        }

        if (smf_Interact_currentBestSMBlockDisplayIndex != -1)
        {
            if (uiNoOneHasFocus())
            {
                smf_TakeFocus(pSMFBlock);
            }

            if (smf_printDebugLines)
            {
                printf("smf_Interact(): Moving the SMF selection's ending location.\n");
            }

            smf_selectionEndPane = pSMFBlock;
            smf_selectionEndDisplayIndex = smf_Interact_currentBestSMBlockDisplayIndex;
            smf_selectionEndXBase = iXBase;
            smf_selectionEndYBase = iYBase;
            smf_selectionEndCharacterXOffset = -1;
            smf_selectionEndCharacterYOffset = -1;
            smf_selectionEndCharacterYHeight = -1;

            if (smf_drawDebugBoxes >= 1)
            {
                clipperPush(NULL);
                drawBox(&smfBox, iZBase, 0x33ff33ff, 0);
                clipperPop();
            }

            if (!smf_selectionStartPane)
            {
                if (smf_printDebugLines)
                {
                    printf("smf_Interact(): Setting the SMF selection's starting location.\n");
                }

                smf_selectionStartPane = smf_selectionEndPane;
                smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                smf_selectionStartYBase = smf_selectionEndYBase;
            }

            smf_stopSettingTheSelectionThisFrame = true;
            smf_gainedFocusThisFrame = true;
            smf_isCurrentlyDragging = true;

            // turns off the "drag outside of SMFBlock" searching such that it doesn't copy that search over.
            smfSearch_currentCommand = SMFSearchCommand_None;
        }
    }
    else if (pSMFBlock->rightClickedOnThisFrame)
    {
        int x;
        int y;

        if (rightClickCoords(&x, &y))
        {
            if (pSMFBlock->contextMenuMode != SMFContextMenuMode_None)
            {
                contextMenu_set(smf_Interact_contextMenu, x, y, pSMFBlock);
            }
        }
    }

    if (pSMFBlock == smf_selectionEndPane)
    {
        smf_selectionEndYBase = iYBase;
    }

    if (pSMFBlock == smf_selectionStartPane)
    {
        smf_selectionStartYBase = iYBase;
    }
}
void smf_setDebugDrawMode(int mode)
{
    smf_drawDebugBoxes = mode;
}