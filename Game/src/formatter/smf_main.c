#include <stdio.h>  // fopen
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <sys/stat.h>

#include "formatter/smf_main.h"
#include "formatter/smf_util.h"
#include "formatter/smf_interact.h"
#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "smf_select.h"
#include "formatter/smf_render.h"

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#include "UI/sprite/sprite_base.h" // for the scissors calls
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/network/crypt.h>

#include "cmdparse/cmdcommon.h" // for TIMESTEP

#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include "win/input.h"

#include <utilitieslib/utils/MemoryMonitor.h>

#include "sound/sound.h"

#include "UI/uiUtilGame.h" // for drawBox debugging
#include "UI/uiScrollBar.h" // gScrollBarDraggingLastFrame
#include "UI/uiClipper.h"
#include "UI/uiInput.h"

#include "win/win_init.h"
#include <utilitieslib/utils/StringUtil.h>

TextAttribs gTextAttr_Gray12 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x666666ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x666666ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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

TextAttribs gTextAttr_White9 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_9,
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

TextAttribs gTextAttr_White12 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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

TextAttribs gTextAttr_WhiteNoOutline12 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_Chat =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&chatFont,
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

TextAttribs gTextAttr_Arena =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffff00ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffff00ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_Green =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x48ff48ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x48ff48ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_LightHybridBlueHybrid12 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x046ecaff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x046ecaff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&hybridbold_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_DarkHybridBlueHybrid12 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x073d6cff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x073d6cff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&hybridbold_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_WhiteHybridBold12 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&hybridbold_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_WhiteHybridBold18 =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&hybridbold_18,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_WhiteTitle18Italics =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&title_18,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

TextAttribs gTextAttr_WhiteTitle24Italics =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&title_24,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

/**********************************************************************func*
* smf_CopyStringToClipboard
*
* This function copies whatever is in the selection string to the clipboard.
* Call this function to initiate a copy (in a bind on Control+C for example).
*/
void smf_CopyStringToClipboard(void)
{
    char *copyString;

    copyString = smf_GetSelectionString();

    if (copyString && strlen(copyString) > 0)
    {
        HGLOBAL handle;

        if (!OpenClipboard(hwnd))
        {
            printf("smf_CopyStringToClipboard() couldn't open the clipboard!  GetLastError() == %i", GetLastError());
            return;
        }

        if (!EmptyClipboard())
        {
            printf("smf_CopyStringToClipboard() couldn't empty the clipboard!  GetLastError() == %i", GetLastError());
            return;
        }
        
        handle = GlobalAlloc(GMEM_MOVEABLE, strlen(copyString) + 1);
        if (handle)
        {
            char* handleCopy;
            handleCopy = GlobalLock(handle);
            strcpy(handleCopy, copyString);
            GlobalUnlock(handle);
            SetClipboardData(CF_TEXT, handle);
        }
        
        if (!CloseClipboard())
        {
            printf("smf_CopyStringToClipboard() couldn't close the clipboard!  GetLastError() == %i", GetLastError());
            return;
        }
    }
}

static void smf_HandleKeyboardInput()
{
    KeyInput* input;
    int key;
    bool moveSelectionStartToEnd = false;
    if (!smf_selectionEndPane || smf_gainedFocusThisFrame)
    {
        return;
    }

    input = inpGetKeyBuf();

    while (input)
    {
        key = input->key;

        // Only use the numpad for text editing/movement if numlock is off.
        // Otherwise those keys should generate numbers (or pass through to
        // keybinds if the text is not writable).
        if (!inpIsNumLockOn())
        {
            switch (key)
            {
                case INP_NUMPAD1:
                    key = INP_END;
                    break;
                case INP_NUMPAD2:
                    key = INP_DOWN;
                    break;
                case INP_NUMPAD3:
                    key = INP_PRIOR;
                    break;
                case INP_NUMPAD4:
                    key = INP_LEFT;
                    break;
                case INP_NUMPAD6:
                    key = INP_RIGHT;
                    break;
                case INP_NUMPAD7:
                    key = INP_HOME;
                    break;
                case INP_NUMPAD8:
                    key = INP_UP;
                    break;
                case INP_NUMPAD9:
                    key = INP_NEXT;
            }
        }

        if (KIT_EditKey == input->type)
        {
            smfSearch_keepStartConstant = (input->attrib & KIA_SHIFT);

            switch (key)
            {
            case INP_LEFT:
                if (input->attrib & KIA_CONTROL)
                {
                    smfSearch_currentCommand = SMFSearchCommand_PreviousBeginningOfWord;
                }
                else
                {
                    if (smf_selectionEndDisplayIndex > 0)
                    {
                        smf_selectionSearchPane = smf_selectionEndPane;
                        smf_selectionSearchDisplayIndex = smf_selectionEndDisplayIndex - 1;
                        smf_selectionSearchXBase = smf_selectionEndXBase;
                        smf_selectionSearchYBase = smf_selectionEndYBase;
                        smf_selectionSearchCharacterXOffset = -1;
                        smf_selectionSearchCharacterYOffset = -1;
                        smf_selectionSearchCharacterYHeight = -1;

                        smfSearch_currentCommand = SMFSearchCommand_Done;

                        // All movement puts the cursor back to the start of its 'on' phase.
                        smfCursor_display = true;
                        smfCursor_currentTimer = 0.0f;
                    }
                    else
                    {
                        smfSearch_currentCommand = SMFSearchCommand_EndOfLineBeforeSelectedEndLine;
                    }

                    if (!smfSearch_keepStartConstant)
                    {
                        moveSelectionStartToEnd = true;
                    }
                }
                break;
            case INP_RIGHT:
                if (input->attrib & KIA_CONTROL)
                {
                    smfSearch_currentCommand = SMFSearchCommand_NextBeginningOfWord;
                }
                else
                {
                    if (smf_selectionEndDisplayIndex < smfBlock_GetDisplayLength(smf_selectionEndPane))
                    {
                        smf_selectionSearchPane = smf_selectionEndPane;
                        smf_selectionSearchDisplayIndex = smf_selectionEndDisplayIndex + 1;
                        smf_selectionSearchXBase = smf_selectionEndXBase;
                        smf_selectionSearchYBase = smf_selectionEndYBase;
                        smf_selectionSearchCharacterXOffset = -1;
                        smf_selectionSearchCharacterYOffset = -1;
                        smf_selectionSearchCharacterYHeight = -1;

                        smfSearch_currentCommand = SMFSearchCommand_Done;

                        // All movement puts the cursor back to the start of its 'on' phase.
                        smfCursor_display = true;
                        smfCursor_currentTimer = 0.0f;
                    }
                    else
                    {
                        smfSearch_currentCommand = SMFSearchCommand_StartOfLineAfterSelectedEndLine;
                    }

                    if (!smfSearch_keepStartConstant)
                    {
                        smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
                        smf_selectionStartPane = smf_selectionEndPane;
                        smf_selectionStartYBase = smf_selectionEndYBase;
                        moveSelectionStartToEnd = true;
                    }
                }
                break;
            case INP_UP:
                smfSearch_currentCommand = SMFSearchCommand_WordAboveSelectedEnd;

                if (!smfSearch_keepStartConstant)
                {
                    moveSelectionStartToEnd = true;
                }
                break;
            case INP_DOWN:
                smfSearch_currentCommand = SMFSearchCommand_WordBelowSelectedEnd;

                if (!smfSearch_keepStartConstant)
                {
                    moveSelectionStartToEnd = true;
                }
                break;
            case INP_HOME:
                smfSearch_currentCommand = SMFSearchCommand_StartOfSelectedEndLine;

                if (!smfSearch_keepStartConstant)
                {
                    moveSelectionStartToEnd = true;
                }
                break;
            case INP_END:
                smfSearch_currentCommand = SMFSearchCommand_EndOfSelectedEndLine;

                if (!smfSearch_keepStartConstant)
                {
                    moveSelectionStartToEnd = true;
                }
                break;
            case INP_BACKSPACE:
                if (smf_selectionStartPane == smf_selectionEndPane &&                     smf_selectionStartDisplayIndex == smf_selectionEndDisplayIndex)
                {
                    // Only remove the character if there wasn't any text selected.
                    smfEdit_currentCommand = SMFEditCommand_RemoveCharBeforeCursor;
                }
                else
                {
                    smf_SetSelectionCommand(SMFSelectionCommand_Delete);
                }

                if (!smfSearch_keepStartConstant)
                {
                    moveSelectionStartToEnd = true;
                }
                break;
            case INP_DELETE:
                if (smf_selectionStartPane == smf_selectionEndPane && 
                    smf_selectionStartDisplayIndex == smf_selectionEndDisplayIndex)
                {
                    // Only remove the character if there wasn't any text selected.
                    smfEdit_currentCommand = SMFEditCommand_RemoveCharAfterCursor;
                }
                else
                {
                    smf_SetSelectionCommand(SMFSelectionCommand_Delete);
                }

                if (!smfSearch_keepStartConstant)
                {
                    moveSelectionStartToEnd = true;
                }
                break;
            case INP_TAB:
                if (smf_selectionEndPane && smf_selectionEndPane->tabNavigationSetName)
                {
                    smfSearch_keepStartConstant = false;
                    if (input->attrib & KIA_SHIFT)
                    {
                        smfSearch_currentCommand = SMFSearchCommand_PreviousTabbedBlock;
                    }
                    else
                    {
                        smfSearch_currentCommand = SMFSearchCommand_NextTabbedBlock;
                    }
                }
                break;
            case INP_RETURN:
            case INP_NUMPADENTER:
                if (smf_selectionEndPane && smf_IsBlockMultiline(smf_selectionEndPane))
                {
                    smfEdit_currentCommand = SMFEditCommand_AddLineBreakAtCursor;
                }
            xcase INP_ESCAPE:
                smfSearch_currentCommand = SMFSearchCommand_ClearSelection;
            xcase INP_A:
                smf_SelectAllText(smf_selectionEndPane);
                smfEdit_currentCommand = SMFEditCommand_Done;
            xcase INP_X:
                smf_CopyStringToClipboard();
                smf_SetSelectionCommand(SMFSelectionCommand_Delete);
            xcase INP_C:
                smf_CopyStringToClipboard();
            xcase INP_V:
                smfEdit_currentCommand = SMFEditCommand_PasteStringAtCursor;
                smf_SetSelectionCommand(SMFSelectionCommand_Delete);
            xdefault:
                // Do nothing
                break;
            }
        }
        else if (KIT_Ascii == input->type || KIT_Unicode == input->type)
        {
            unsigned char converted[7];

            if (KIT_Unicode == input->type)
            {
                unsigned short src_unicode[2];

                src_unicode[0] = input->unicodeCharacter;
                src_unicode[1] = '\0';
                WideToUTF8StrConvert(src_unicode, converted, 7);
            }
            else
            {
                converted[0] = input->asciiCharacter;
                converted[1] = '\0';
            }

            smfSearch_keepStartConstant = false;

            if (!inpLevel(INP_CONTROL))
            {
                if (smf_selectionEndPane && smf_IsBlockEditable(smf_selectionEndPane))
                {
                    // It is probably a better idea to make the return key
                    // "EditKey" in the keyboard input code. (Tab is one.)
                    // 13 = return, 27 = escape
                    if (13 != converted[0] && 27 != converted[0])
                    {
                        // Add a character - no need to delete unless something
                        // is selected
                        if (smf_IsAnythingSelected())
                        {
                            smf_SetSelectionCommand(SMFSelectionCommand_Delete);
                        }

                        smfEdit_currentCommand = SMFEditCommand_AddCharAtCursor;
                        estrConcatCharString(&smfEdit_currentCharactersToInsert, converted);
                    }
                }
            }
        }

        if (moveSelectionStartToEnd && smf_GetSelectionCommand() != SMFSelectionCommand_Delete)
        {
            smf_selectionStartDisplayIndex = smf_selectionEndDisplayIndex;
            smf_selectionStartPane = smf_selectionEndPane;
            smf_selectionStartYBase = smf_selectionEndYBase;
        }

        inpGetNextKey(&input);
    }
}

// This function does all work for the SMF system that is required at the beginning of every frame.
void smf_HandlePerFrame()
{
    static int windowWidthLastFrame = -1;
    static int windowHeightLastFrame = -1;
    static int windowWidth;
    static int windowHeight;
    int startingBytesAlloced, endingBytesAlloced;

    // Make sure the right-click menu is ready to go if it hasn't been
    // previously initialized.
    smf_CreateContextMenu();

    if (smf_analyzeMemory >= 1)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    if (smf_selectionEndPane && smf_displayedSomethingThisFrame)
    {
        if (smf_selectionEndPane->displayedThisFrameSinceLastTimeIWasASelectionEndpoint)
        {
            smf_selectionEndPane->displayedThisFrameSinceLastTimeIWasASelectionEndpoint = false;
        }
        else
        {
            smf_ClearSelection();
        }
        smf_displayedSomethingThisFrame = false;
    }

    windowWidthLastFrame = windowWidth;
    windowHeightLastFrame = windowHeight;
    windowClientSizeThisFrame(&windowWidth, &windowHeight);

    smf_forceReformatAll = windowWidthLastFrame != -1 && (windowWidthLastFrame != windowWidth ||
                                                          windowHeightLastFrame != windowHeight);

    smfCursor_currentTimer += TIMESTEP/30;
    if (smfCursor_currentTimer >= smfCursor_interval)
    {
        smfCursor_display = !smfCursor_display;
        smfCursor_currentTimer -= smfCursor_interval;
    }

    smf_selectionCurrentYBase = -1;
    smf_hasDisplayedHoverBackgroundThisFrame = false;

    if (smfSearch_currentCommand != SMFSearchCommand_None)
    {
        if (smf_selectionSearchPane)
        {
            if (!smfBlock_HasFocus(smf_selectionSearchPane))
            {
                smf_SelectAllText(smf_selectionSearchPane);
            }
            else
            {
                smf_selectionEndPane = smf_selectionSearchPane;
                smf_selectionEndDisplayIndex = smf_selectionSearchDisplayIndex;
                smf_selectionEndXBase = smf_selectionSearchXBase;
                smf_selectionEndYBase = smf_selectionSearchYBase;
                smf_selectionEndCharacterXOffset = smf_selectionSearchCharacterXOffset;
                smf_selectionEndCharacterYOffset = smf_selectionSearchCharacterYOffset;
                smf_selectionEndCharacterYHeight = smf_selectionSearchCharacterYHeight;

                if (!smfSearch_keepStartConstant)
                {
                    smf_selectionStartPane = smf_selectionSearchPane;
                    smf_selectionStartDisplayIndex = smf_selectionSearchDisplayIndex;
                    smf_selectionStartYBase = smf_selectionSearchYBase;
                }
            }

            if (smf_selectionEndPane && smf_selectionEndPane->sound_successfulKeyboardInput)
            {
                sndPlay(smf_selectionEndPane->sound_successfulKeyboardInput, SOUND_PLAYER);
            }
        }
        else
        {
            if (smf_selectionEndPane && smf_selectionEndPane->sound_failedKeyboardInput)
            {
                sndPlay(smf_selectionEndPane->sound_failedKeyboardInput, SOUND_PLAYER);
            }
        }
    }

    smfSearch_lastCommand = smfSearch_currentCommand;
    smfSearch_currentCommand = SMFSearchCommand_None;
    smfSearch_nextTabbedBlock_justSawEndPane = false;
    smf_selectionSearchPane = 0;
    smf_selectionSearchDisplayIndex = 0;
    smf_selectionSearchXBase = 0;
    smf_selectionSearchYBase = 0;
    smf_selectionSearchCharacterXOffset = 0;
    smf_selectionSearchCharacterYOffset = 0;
    smf_selectionSearchCharacterYHeight = 0;

    smf_ClearSelectionCommand();

    if (!smfEdit_currentCharactersToInsert)
    {
        estrCreate(&smfEdit_currentCharactersToInsert);
    }

    smfEdit_currentCommand = SMFEditCommand_None;

    smf_stopSettingTheSelectionThisFrame = false;

    smf_HandleKeyboardInput();

    if (smf_isCurrentlyDragging && !mouseUp(MS_LEFT)
        && !mouseDragging() && !gScrollBarDraggingLastFrame)
    {
        smfSearch_currentCommand = SMFSearchCommand_BlockNearestCursor;
        smfSearch_keepStartConstant = true;
    }

    //clipperPush(NULL);
    //cprnt(50, 50, 1, 1, 1, smf_currentCharactersToInsert);
    //clipperPop();

    // Must be after smf_HandleKeyboardInput because that's where the copying occurs.
    smf_HandleSelectPerFrame();

    smf_gainedFocusThisFrame = false;

    if (smf_analyzeMemory >= 1)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }
}


TextAttribs* smf_CreateTextAttribs()
{
    TextAttribs *pattrs = calloc(1, sizeof(TextAttribs));
    eaiCreate(&pattrs->piBold);
    eaiCreate(&pattrs->piItalic);
    eaiCreate(&pattrs->piColor);
    eaiCreate(&pattrs->piColor2);
    eaiCreate(&pattrs->piColorHover);
    eaiCreate(&pattrs->piColorSelect);
    eaiCreate(&pattrs->piColorSelectBG);
    eaiCreate(&pattrs->piScale);
    eaiCreate(&pattrs->piFace);
    eaiCreate(&pattrs->piFont);
    eaiCreate(&pattrs->piAnchor);
    eaiCreate(&pattrs->piLink);
    eaiCreate(&pattrs->piLinkBG);
    eaiCreate(&pattrs->piLinkHover);
    eaiCreate(&pattrs->piLinkHoverBG);
    eaiCreate(&pattrs->piLinkSelect);
    eaiCreate(&pattrs->piLinkSelectBG);
    eaiCreate(&pattrs->piOutline);
    eaiCreate(&pattrs->piShadow);
    return pattrs;
}

TextAttribs* smf_CreateDefaultTextAttribs()
{
    TextAttribs *pattrs = calloc(1, sizeof(TextAttribs));
    return pattrs;
}

void smf_DestroyTextAttribs(TextAttribs *pattrs)
{
    if (pattrs)
    {
        eaiDestroy(&pattrs->piBold);
        eaiDestroy(&pattrs->piItalic);
        eaiDestroy(&pattrs->piColor);
        eaiDestroy(&pattrs->piColor2);
        eaiDestroy(&pattrs->piColorHover);
        eaiDestroy(&pattrs->piColorSelect);
        eaiDestroy(&pattrs->piColorSelectBG);
        eaiDestroy(&pattrs->piScale);
        eaiDestroy(&pattrs->piFace);
        eaiDestroy(&pattrs->piFont);
        eaiDestroy(&pattrs->piAnchor);
        eaiDestroy(&pattrs->piLink);
        eaiDestroy(&pattrs->piLinkBG);
        eaiDestroy(&pattrs->piLinkHover);
        eaiDestroy(&pattrs->piLinkHoverBG);
        eaiDestroy(&pattrs->piLinkSelect);
        eaiDestroy(&pattrs->piLinkSelectBG);
        eaiDestroy(&pattrs->piOutline);
        eaiDestroy(&pattrs->piShadow);
        SAFE_FREE(pattrs);
    }
}

void smf_DestroyDefaultTextAttribs(TextAttribs *pattrs)
{
    if (pattrs)
    {
        SAFE_FREE(pattrs);
    }
}

/**********************************************************************func*
* smf_InitTextAttribs
*
* This function resets pattrs to the default values specified.
* If no default values are specified, a hardcoded set of defaults is used instead.
* pattrs is the primary TextAttribs object used during execution of SMF parsing/rendering.
*/
void smf_InitTextAttribs(TextAttribs *pattrs, TextAttribs *pdefaults)
{
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
        /* piFace        */  (int *)&smfDefault,
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

    PERFINFO_AUTO_START("smf_InitTextAttribs", 1);

    if(pdefaults == NULL)
    {
        pdefaults = &s_taDefaults;
    }

    if(!pattrs->piBold)
    {
        eaiCreate(&pattrs->piBold);
        eaiCreate(&pattrs->piItalic);
        eaiCreate(&pattrs->piColor);
        eaiCreate(&pattrs->piColor2);
        eaiCreate(&pattrs->piColorHover);
        eaiCreate(&pattrs->piColorSelect);
        eaiCreate(&pattrs->piColorSelectBG);
        eaiCreate(&pattrs->piScale);
        eaiCreate(&pattrs->piFace);
        eaiCreate(&pattrs->piFont);
        eaiCreate(&pattrs->piAnchor);
        eaiCreate(&pattrs->piLink);
        eaiCreate(&pattrs->piLinkBG);
        eaiCreate(&pattrs->piLinkHover);
        eaiCreate(&pattrs->piLinkHoverBG);
        eaiCreate(&pattrs->piLinkSelect);
        eaiCreate(&pattrs->piLinkSelectBG);
        eaiCreate(&pattrs->piOutline);
        eaiCreate(&pattrs->piShadow);

    }
    else
    {
        eaiSetSize(&pattrs->piBold,            0);
        eaiSetSize(&pattrs->piItalic,            0);
        eaiSetSize(&pattrs->piColor,            0);
        eaiSetSize(&pattrs->piColor2,            0);
        eaiSetSize(&pattrs->piColorHover,        0);
        eaiSetSize(&pattrs->piColorSelect,    0);
        eaiSetSize(&pattrs->piColorSelectBG,    0);
        eaiSetSize(&pattrs->piScale,            0);
        eaiSetSize(&pattrs->piFace,            0);
        eaiSetSize(&pattrs->piFont,            0);
        eaiSetSize(&pattrs->piAnchor,            0);
        eaiSetSize(&pattrs->piLink,            0);
        eaiSetSize(&pattrs->piLinkBG,            0);
        eaiSetSize(&pattrs->piLinkHover,        0);
        eaiSetSize(&pattrs->piLinkHoverBG,    0);
        eaiSetSize(&pattrs->piLinkSelect,        0);
        eaiSetSize(&pattrs->piLinkSelectBG,    0);
        eaiSetSize(&pattrs->piOutline,        0);
        eaiSetSize(&pattrs->piShadow,            0);
    }

    // Cram the initial values in. Yes, this is evil.
    eaiPush(&pattrs->piBold,            (int)pdefaults->piBold);
    eaiPush(&pattrs->piItalic,        (int)pdefaults->piItalic);
    eaiPush(&pattrs->piColor,            (int)pdefaults->piColor);
    eaiPush(&pattrs->piColor2,        (int)pdefaults->piColor2);
    eaiPush(&pattrs->piColorHover,    (int)pdefaults->piColorHover);
    eaiPush(&pattrs->piColorSelect,    (int)pdefaults->piColorSelect);
    eaiPush(&pattrs->piColorSelectBG, (int)pdefaults->piColorSelectBG);
    eaiPush(&pattrs->piScale,            (int)pdefaults->piScale);
    eaiPush(&pattrs->piFace,            (int)pdefaults->piFace);
    eaiPush(&pattrs->piFont,            (int)pdefaults->piFont);
    eaiPush(&pattrs->piAnchor,        (int)pdefaults->piAnchor);
    eaiPush(&pattrs->piLink,            (int)pdefaults->piLink);
    eaiPush(&pattrs->piLinkBG,        (int)pdefaults->piLinkBG);
    eaiPush(&pattrs->piLinkHover,        (int)pdefaults->piLinkHover);
    eaiPush(&pattrs->piLinkHoverBG,    (int)pdefaults->piLinkHoverBG);
    eaiPush(&pattrs->piLinkSelect,    (int)pdefaults->piLinkSelect);
    eaiPush(&pattrs->piLinkSelectBG,    (int)pdefaults->piLinkSelectBG);
    eaiPush(&pattrs->piOutline,        (int)pdefaults->piOutline);
    eaiPush(&pattrs->piShadow,        (int)pdefaults->piShadow);

    s_bAllowAnchor = true;

    PERFINFO_AUTO_STOP();
}


void smf_InitDefaultTextAttribs(TextAttribs *pattrs, TextAttribs *pdefaults)
{
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
        /* piFace        */  (int *)&smfDefault,
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

    if(pdefaults == NULL)
    {
        pdefaults = &s_taDefaults;
    }

    // Cram the initial values in. Yes, this is evil.
    pattrs->piBold = pdefaults->piBold;
    pattrs->piItalic = pdefaults->piItalic;
    pattrs->piColor = pdefaults->piColor;
    pattrs->piColor2 = pdefaults->piColor2;
    pattrs->piColorHover = pdefaults->piColorHover;
    pattrs->piColorSelect = pdefaults->piColorSelect;
    pattrs->piColorSelectBG = pdefaults->piColorSelectBG;
    pattrs->piScale = pdefaults->piScale;
    pattrs->piFace = pdefaults->piFace;
    pattrs->piFont = pdefaults->piFont;
    pattrs->piAnchor = pdefaults->piAnchor;
    pattrs->piLink = pdefaults->piLink;
    pattrs->piLinkBG = pdefaults->piLinkBG;
    pattrs->piLinkHover = pdefaults->piLinkHover;
    pattrs->piLinkHoverBG = pdefaults->piLinkHoverBG;
    pattrs->piLinkSelect = pdefaults->piLinkSelect;
    pattrs->piLinkSelectBG = pdefaults->piLinkSelectBG;
    pattrs->piOutline = pdefaults->piOutline;
    pattrs->piShadow = pdefaults->piShadow;
}

void smf_NotifyOfExternalScrollDiff(int yDiff)
{
    if (smf_selectionEndPane)
    {
        smf_selectionEndYBase -= yDiff;
    }
    if (smf_selectionStartPane)
    {
        smf_selectionStartYBase -= yDiff;
    }
}

int smf_GetInternalScrollDiff(SMFBlock *pSMFBlock)
{
    return smf_IsBlockScrollable(pSMFBlock) ? 0 : pSMFBlock->internalScrollDiff;
}


void smf_SetFlags(SMFBlock *pSMFBlock, SMFEditMode editMode, SMFLineBreakMode lineBreakMode, 
                  SMFInputMode inputMode, int inputLimit, SMFScrollMode scrollMode, 
                  SMFOutputMode outputMode, SMFDisplayMode displayMode, SMFContextMenuMode contextMenuMode,
                  SMAlignment defaultHorizAlign, char *coselectionSetName, char *tabNavigationSetName,
                  int tabNavigationSetIndex)
{
    pSMFBlock->editMode = (editMode ? editMode : SMFEditMode_DefaultValue);
    pSMFBlock->lineBreakMode = (lineBreakMode ? lineBreakMode : SMFLineBreakMode_DefaultValue);
    pSMFBlock->inputMode = (inputMode ? inputMode : SMFInputMode_DefaultValue);
    pSMFBlock->inputLimit = (inputLimit > 0 ? (inputLimit < 0x7fffffff ? inputLimit : 0x7fffffff) : inputMode==SMFInputMode_NonnegativeIntegers?0x7fffffff:SMFInputLimit_DefaultValue);
    pSMFBlock->scrollMode = (scrollMode ? scrollMode : SMFScrollMode_DefaultValue);
    pSMFBlock->outputMode = (outputMode ? outputMode : SMFOutputMode_DefaultValue);
    pSMFBlock->displayMode = (displayMode ? displayMode : SMFDisplayMode_DefaultValue);
    pSMFBlock->contextMenuMode = (contextMenuMode ? contextMenuMode : SMFContextMenuMode_DefaultValue);
    pSMFBlock->defaultHorizAlign = defaultHorizAlign;

    if (pSMFBlock->coselectionSetName)
    {
        SAFE_FREE(pSMFBlock->coselectionSetName);
    }
    pSMFBlock->coselectionSetName = strdup(coselectionSetName);

    if (pSMFBlock->tabNavigationSetName)
    {
        SAFE_FREE(pSMFBlock->tabNavigationSetName);
    }
    pSMFBlock->tabNavigationSetName = strdup(tabNavigationSetName);

    pSMFBlock->tabNavigationSetIndex = tabNavigationSetIndex;
}

void smf_SetScissorsBox(SMFBlock *pSMFBlock, float xPos, float yPos, float width, float height)
{
    if (!pSMFBlock->scissorsBox)
    {
        pSMFBlock->scissorsBox = malloc(sizeof(CBox));
    }

    BuildScaledCBox(pSMFBlock->scissorsBox, xPos, yPos, width, height);
}

void smf_ClearScissorsBox(SMFBlock *pSMFBlock)
{
    if (pSMFBlock->scissorsBox)
    {
        SAFE_FREE(pSMFBlock->scissorsBox);
    }
}

void smf_SetMasterScale(SMFBlock *pSMFBlock, float scale)
{
    if (pSMFBlock)
        pSMFBlock->masterScale = scale;
}

static void encodeRawString(char **pReplaceString, const char *pRawString)
{
    unsigned int i;

    if (*pReplaceString)
    {
        estrClear(pReplaceString);
    }
    else
    {
        estrCreate(pReplaceString);
    }

    // This is a hack, could integrate better with smf_InteractWithAddedByte,
    // only if the latter wasn't so tied down to the cursor...
    for (i = 0; i < strlen(pRawString); i++)
    {
        if (pRawString[i] == '%')
        {
            estrConcatStaticCharArray(pReplaceString, "&#37;");
        }
        else if (pRawString[i] == '&')
        {
            estrConcatStaticCharArray(pReplaceString, "&amp;");
        }
        else if (pRawString[i] == '<')
        {
            estrConcatStaticCharArray(pReplaceString, "&lt;");
        }
        else if (pRawString[i] == '>')
        {
            estrConcatStaticCharArray(pReplaceString, "&gt;");
        }
        else
        {
            estrConcatFixedWidth(pReplaceString, pRawString + i, 1);
        }
    }
}

// Should this be forced to go through the interact code, inserting each character, like paste is?
void smf_SetRawText(SMFBlock *pSMFBlock, const char *pch, bool encodeCharacters)
{
    if (pch)
    {
        // Only need to force the string to empty if there's no encoding.
        // The encoding function will do that work for us because it can't
        // assume it's getting a pointer to an existing EString.
        if (!encodeCharacters)
        {
            estrPrintCharString(&pSMFBlock->rawString, pch);
        }
        else
        {
            encodeRawString(&pSMFBlock->rawString, pch);
        }
    }
    pSMFBlock->editedThisFrame = 1;
}

void smf_InsertIntoRawText(SMFBlock *pSMFBlock, char *pch, int insertPos, bool encodeCharacters)
{
    char *pInsertedText = NULL;
    char *pNewRawText = NULL;
    int existingLen = 0;
    int newLen = 0;
    int addPos = insertPos;

    if (pSMFBlock && pSMFBlock->rawString && pch && strlen(pch) > 0)
    {
        // Can't get this before we're sure the pointer is non-NULL.
        existingLen = strlen(pSMFBlock->rawString);

        // We only encode the text which is being added. Otherwise we'll
        // double-encode the rest of the text which will break it.
        if (!encodeCharacters)
        {
            estrCreate(&pInsertedText);
        }
        else
        {
            encodeRawString(&pInsertedText, pch);
        }

        // We need the real length (ie. after encoding) 
        newLen = strlen(pInsertedText);

        estrCreate(&pNewRawText);

        // We can't insert past the end of the string, so we just append.
        if (addPos > existingLen)
        {
            addPos = existingLen;
        }

        if (addPos < 0)
        {
            addPos = 0;
        }

        // The resulting string will be a beginning part, the new characters
        // and an ending part (depending on exactly where we insert the new
        // stuff). The beginning or ending can be optional but not both.
        if (addPos > 0)
        {
            estrConcatFixedWidth(&pNewRawText, pSMFBlock->rawString, addPos);
        }

        estrConcatFixedWidth(&pNewRawText, pInsertedText, newLen);

        if (addPos < existingLen)
        {
            estrConcatFixedWidth(&pNewRawText, pSMFBlock->rawString + addPos,
                existingLen - addPos);
        }

        // Replace the old text with the compound.
        estrDestroy(&pSMFBlock->rawString);
        pSMFBlock->rawString = pNewRawText;
    }
}

void smf_SetCharacterInsertSounds(SMFBlock *pSMFBlock, char *successfulSound, char *failedSound)
{
    if (!pSMFBlock)
    {
        return;
    }

    SAFE_FREE(pSMFBlock->sound_successfulKeyboardInput);
    SAFE_FREE(pSMFBlock->sound_failedKeyboardInput);

    pSMFBlock->sound_successfulKeyboardInput = strdup(successfulSound);
    pSMFBlock->sound_failedKeyboardInput = strdup(failedSound);
}

/**********************************************************************func*
* smf_ParseAndFormat
*
*/
int smf_ParseAndFormat(SMFBlock *pSMFBlock, char *pch,
                       int x, int y, int z, int w, int h,
                       bool isAllowedToInteract, bool bReparse, bool bReformat, 
                       TextAttribs *ptaDefaults, int (*callback)(char *))
{
    static TextAttribs *attrs = 0;
    U32 crc = 0;
    int startingBytesAlloced, endingBytesAlloced;

    if (!pSMFBlock)
    {
        return -1;
    }

    PERFINFO_AUTO_START("smf_ParseAndFormat", 1);

    if (smf_analyzeMemory >= 1)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    if (h <= 0 && pSMFBlock->pBlock)
    {
        h = pSMFBlock->pBlock->pos.iHeight;
    }

    if (h <= 0)
    {
        h = 99999; // hack
    }

    if (!attrs)
    {
        attrs = smf_CreateTextAttribs();
    }

    // pointer comparison on purpose.
    // If you're passing in a string, you expect it to force a reparse etc., even if it's the same string.
    if (pSMFBlock->rawString != pch) 
    {
        smf_SetRawText(pSMFBlock, pch, false);
    }

    if(w!=pSMFBlock->iLastWidth || h!=pSMFBlock->iLastHeight)
    {
        bReformat = true;
    }

    if (ptaDefaults)
    {
        if ((int)ptaDefaults->piScale != pSMFBlock->lastFontScale)
        {
            bReformat = true;
        }
        pSMFBlock->lastFontScale = (int)ptaDefaults->piScale;
    }

    if(pSMFBlock->pBlock==NULL)
    {
        bReparse=true;
    }

    if(pch) // if NULL don't bother checking
    {
        PERFINFO_AUTO_START("checkCRC", 1);
        cryptAdler32Init();
        crc = cryptAdler32(pch, strlen(pch));

        if (crc != pSMFBlock->ulCrc)
        {
            bReparse = true;
        }

        if ((pch[0]=='\0') && (!pSMFBlock->pBlock))
        {
            // Adding in a bit of a hack to ensure that there is at least one sm_text block in every parsed tree structure.
            // I need this to exist for my selection/cursor code.
            bReparse = true;
        }
        PERFINFO_AUTO_STOP();
    }

    // Do NOT reference pch below here!  Use pSMFBlock->rawString instead!
    // The raw data in the SMF block can change, but pch won't update...

    if (isAllowedToInteract && !pSMFBlock->interactedThisFrame)
    {
        if (smfEdit_currentCommand != SMFEditCommand_None)
        {
            bReparse = true;
            bReformat = true;
        }

         smf_InitTextAttribs(attrs, ptaDefaults);
         smf_Interact(pSMFBlock, attrs, x, y, z, w, h, callback);
        pSMFBlock->interactedThisFrame = true;
    }

    if (smf_forceReformatAll)
    {
        bReparse = true;
    }

    if(bReparse && !pSMFBlock->dont_reparse_ever_again)
    {
        pSMFBlock->ulCrc = crc;

        if(pSMFBlock->pBlock!=NULL)
        {
            sm_DestroyBlock(pSMFBlock->pBlock);
        }

        pSMFBlock->pBlock = smf_CreateAndParse(pSMFBlock->rawString, pSMFBlock);
        bReformat=true;
    }

    if(bReformat)
    {
        pSMFBlock->iLastYBase = y;
        pSMFBlock->iLastWidth = w;
        pSMFBlock->iLastHeight = h;

        smf_InitTextAttribs(attrs, ptaDefaults);

        if(!smf_Format(pSMFBlock->pBlock, attrs, w, h, pSMFBlock->lineBreakMode))
        {
            //printf("*** Unable to fit!\n");
        }
    }

    if (smf_analyzeMemory >= 1)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    PERFINFO_AUTO_STOP();

    return(pSMFBlock->pBlock->pos.iHeight);
}

/**********************************************************************func*
* smf_ParseAndDisplay
*
*/
int smf_ParseAndDisplay(SMFBlock *pSMFBlock, const char *pch,
                        int x, int y, int z, int w, int h,
                        bool bReparse, bool bReformat, TextAttribs *ptaDefaults, 
                        int (*callback)(char *), char *coselectionSetName, bool selectable)
{
    static SMFBlock s_Block = { 0 };
    SMFEditMode editMode = (selectable ? SMFEditMode_ReadOnly : SMFEditMode_Unselectable);
    int returnMe;
    int startingBytesAlloced, endingBytesAlloced;

    if (smf_analyzeMemory >= 1)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    if(pSMFBlock==NULL)
    {
        pSMFBlock = &s_Block;
    }

    if (pch && (!pSMFBlock->rawString || stricmp(pSMFBlock->rawString, pch)))
    {
        smf_SetRawText(pSMFBlock, pch, false);
    }

    smf_SetFlags(pSMFBlock, editMode, SMFLineBreakMode_MultiLineBreakOnWhitespace, 
        SMFInputMode_AnyTextNoTagsOrCodes, -1, SMFScrollMode_ExternalOnly, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
        SMAlignment_Left, coselectionSetName, 0, 0);

    returnMe = smf_Display(pSMFBlock, x, y, z, w, h, bReparse, bReformat, ptaDefaults, callback);

    if (smf_analyzeMemory >= 1)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    return returnMe;
}

int smf_Display(SMFBlock *pSMFBlock, int x, int y, int z, int w, int h, 
                bool bReparse, bool bReformat, TextAttribs *ptaDefaults, 
                int (*callback)(char *))
{
    return smf_DisplayEx(pSMFBlock, x, y, z, w, h, true, bReparse, bReformat, ptaDefaults, callback );
}

int smf_DisplayEx(SMFBlock *pSMFBlock, int x, int y, int z, int w, int h, 
                bool bCanInteract, bool bReparse, bool bReformat, TextAttribs *ptaDefaults, 
                int (*callback)(char *))
{
    static TextAttribs attrs;
    int startingBytesAlloced, endingBytesAlloced;

    if (w < 0 || !pSMFBlock)
    {
        return 0;
    }

    PERFINFO_AUTO_START("smf_Display", 1);

    if (smf_analyzeMemory >= 1)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    pSMFBlock->displayedThisFrameSinceLastTimeIWasASelectionEndpoint = true;
    smf_displayedSomethingThisFrame = true;

    if (h <= 0 && pSMFBlock->pBlock)
    {
        h = pSMFBlock->pBlock->pos.iHeight;
    }

    if (h <= 0)
    {
        h = 99999; // hack
    }

    smf_ParseAndFormat(pSMFBlock, pSMFBlock->rawString, x, y, z, w, h, bCanInteract, bReparse, bReformat, ptaDefaults, callback);
    
    smf_InitTextAttribs(&attrs, ptaDefaults); // Reuses defaults
    smf_ResetSelectedBlocks(pSMFBlock->pBlock);
    smf_RebuildPartialSelectionFromSMFBlock(pSMFBlock, x, y, z);
    smf_Select(pSMFBlock->pBlock, &attrs, x, y, z);

    smf_DetermineCurrentOffsets(pSMFBlock, x, y, z, w, h);

    if (pSMFBlock->scissorsBox)
    {
        set_scissor(TRUE);
        scissor_dims(pSMFBlock->scissorsBox->upperLeft.x, pSMFBlock->scissorsBox->upperLeft.y, 
                     CBoxWidth(pSMFBlock->scissorsBox), CBoxHeight(pSMFBlock->scissorsBox));

        if (smf_drawDebugBoxes >= 2)
        {
            clipperPush(NULL);
            drawBox(pSMFBlock->scissorsBox, z, 0xff0000ff, 0);
            clipperPop();
        }
    }
    smf_InitTextAttribs(&attrs, ptaDefaults); // Reuses defaults
    if (smf_IsBlockScrollable(pSMFBlock))
    {
        smf_Render(pSMFBlock->pBlock, &attrs, x + pSMFBlock->currentDisplayOffsetX, y + pSMFBlock->currentDisplayOffsetY, z);
    }
    else
    {
        smf_Render(pSMFBlock->pBlock, &attrs, x, y, z);
    }
    if (pSMFBlock->scissorsBox)
    {
        set_scissor(FALSE);
    }

    pSMFBlock->interactedThisFrame = false;

    if (smf_analyzeMemory >= 1)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    PERFINFO_AUTO_STOP();

    return pSMFBlock->pBlock->pos.iHeight;
}

void smf_EncodeAllCharactersInPlace(char **eString)
{
    unsigned int index;

    for (index = 0; index < estrLength(eString); index++)
    {
        if ((*eString)[index] == ' ')
        {
            if (index > 0 && (*eString)[index - 1] == ' ')
            {
                estrRemove(eString, index, 1);
                estrInsert(eString, index, "&nbsp;", 6);
                index += 5;
            }
        }
        else if ((*eString)[index] == '<')
        {
            estrRemove(eString, index, 1);
            estrInsert(eString, index, "&lt;", 4);
            index += 3;
        }
        else if ((*eString)[index] == '>')
        {
            estrRemove(eString, index, 1);
            estrInsert(eString, index, "&gt;", 4);
            index += 3;
        }
        else if ((*eString)[index] == '&')
        {
            if (strnicmp((*eString) + index, "&nbsp;", 6) &&
                strnicmp((*eString) + index, "&lt;", 4) &&
                strnicmp((*eString) + index, "&gt;", 4) &&
                strnicmp((*eString) + index, "&amp;", 5) &&
                strnicmp((*eString) + index, "&#37;", 5))
            {
                estrRemove(eString, index, 1);
                estrInsert(eString, index, "&amp;", 5);
                index += 4;
            }
        }
        else if ((*eString)[index] == '%')
        {
            estrRemove(eString, index, 1);
            estrInsert(eString, index, "&#37;", 5);
            index += 4;
        }
    }
}


void smf_DecodeAllCharactersInPlace(char **eString)
{
    unsigned int index;

    for (index = 0; index < estrLength(eString); index++)
    {
        if (!strnicmp((*eString) + index, "&nbsp;", 6))
        {
            estrRemove(eString, index, 6);
            estrInsert(eString, index, " ", 1);
            // don't change index, since we're only inserting one character.
        }
        else if (!strnicmp((*eString) + index, "&lt;", 4))
        {
            estrRemove(eString, index, 4);
            estrInsert(eString, index, "<", 1);
            // don't change index, since we're only inserting one character.
        }
        else if (!strnicmp((*eString) + index, "&gt;", 4))
        {
            estrRemove(eString, index, 4);
            estrInsert(eString, index, ">", 1);
            // don't change index, since we're only inserting one character.
        }
        else if (!strnicmp((*eString) + index, "&amp;", 5))
        {
            estrRemove(eString, index, 5);
            estrInsert(eString, index, "&", 1);
            // don't change index, since we're only inserting one character.
        }
        else if (!strnicmp((*eString) + index, "&#37;", 5))
        {
            estrRemove(eString, index, 5);
            estrInsert(eString, index, "%", 1);
            // don't change index, since we're only inserting one character.
        }
    }
}


