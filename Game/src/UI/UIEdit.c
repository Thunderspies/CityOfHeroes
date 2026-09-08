//---------------------------------------------------------------------------------------------------------------
// UIEdit: MultiLine edit control
//---------------------------------------------------------------------------------------------------------------
#include <utilitieslib/utils/wininclude.h>
#include "UI/uiUtilMenu.h"
#include "filter/validate_name.h"
#include "UIEdit.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiFocus.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/StringUtil.h>
#include "UI/sprite/sprite_text.h"
#include "win/input.h"
#include "UI/uiUtil.h"
#include "UI/uiClipper.h"
#include "graphics/ttFont.h"
#include "graphics/ttFontUtil.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include "cmdparse/cmdcommon.h"
#include "UI/uiInput.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include "sound/sound.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiUtilGame.h"
#include "uiIME.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/error.h>

static unsigned short* newlineCharacter = L"\n";

//---------------------------------------------------------------------------------------------------------------
// ttWrapper section: used to be in ttFontUtil, but not used anywhere else
//---------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------
// UIEdit: Internal functions
//---------------------------------------------------------------------------------------------------------------
static void uiEditLineMarkDirty(UIEdit* edit, UIEditLine* line);
static int uiEditSingleLineMode(UIEdit* edit);
static void uiEditMoveCursorToClosestCharacter(UIEdit *edit, float xRel, float yRel);
static UIEditLine* uiEditLineCreate();
static void uiEditLinePrepareDestroy(UIEdit* edit, UIEditLine* line);
static void uiEditLineDestroy(UIEditLine* line);
static void uiEditPasteWideText(UIEdit* edit, unsigned short* textIn);

char *UTF8EscapedGetNextCharacter(char *pch)
{
    char *pchRet = UTF8GetNextCharacter(pch);
    if(pchRet-pch == 1)
    {
        if(*pch=='\\' && *pchRet)
        {
            pchRet++;
        }
    }

    return pchRet;
}

int WideCharToUTF8Escaped(unsigned short *str, int iCntIn, char *pchOut, int iSizeOut)
{
    char* narrowStr;
    char* escapedStr;

    int requiredLength = WideCharToMultiByte(CP_UTF8, 0, str, iCntIn, NULL, 0, NULL, NULL);

    estrCreate(&narrowStr);
    estrReserveCapacity(&narrowStr, requiredLength+1);

    WideCharToMultiByte(CP_UTF8, 0, str, iCntIn, narrowStr, requiredLength, NULL, NULL);
    narrowStr[requiredLength]='\0';
    escapedStr = (char *)escapeString(narrowStr);
    estrDestroy(&narrowStr);

    requiredLength = strlen(escapedStr);

    if(pchOut)
    {
        strncpyt(pchOut, escapedStr, iSizeOut);
    }

    return requiredLength;
}


// Returns the number of characters that can be inserted into the edit box without
// going over the set character limt.
static int uiEditGetStringInsertLimit(UIEdit* edit, unsigned short* str, int characterCount)
{
    // If there is no limit, then go ahead and say the entire string can be inserted.
    if(edit->limitType == UIECL_NONE)
        return characterCount;

    if(edit->limitType == UIECL_WIDE)
    {
        return min(edit->limitCount - edit->contentCharacterCount, characterCount);
    }

    if(edit->limitType == UIECL_UTF8)
    {
        int requiredLength;

        // Check to see if the entire string can fit.
        // This should be true most of the time.
        requiredLength = WideCharToMultiByte(CP_UTF8, 0, str, characterCount, NULL, 0, NULL, NULL);
        if(edit->limitCount - edit->contentUTF8ByteCount - requiredLength >= 0)
            return characterCount;

        // It was not possible to fit the entire string.
        // Convert the entire string to UTF8 and figure out how much will fit.
        {
            char* narrowStr = NULL;
            int allowedCharacterCount = 0;
            int allowedUTF8ByteCount = 0;
            int availableByteCount = edit->limitCount - edit->contentUTF8ByteCount;
            char* cursor;
            char* lastCursor;

            estrCreate(&narrowStr);
            estrReserveCapacity(&narrowStr, requiredLength);
            WideCharToMultiByte(CP_UTF8, 0, str, characterCount, narrowStr, requiredLength, NULL, NULL);
            lastCursor = cursor = narrowStr;

            while((cursor = UTF8GetNextCharacter(cursor)) && *cursor)
            {
                int characterLength = cursor - lastCursor;

                // Can we fit this character into the box?
                if(availableByteCount - allowedUTF8ByteCount - characterLength < 0)
                    break;

                // If so, record that one more character can be inserted.
                allowedUTF8ByteCount += characterLength;
                allowedCharacterCount++;
                lastCursor = cursor;
            }

            estrDestroy(&narrowStr);
            return allowedCharacterCount;
        }
    }

    if(edit->limitType == UIECL_UTF8_ESCAPED)
    {
        int requiredLength;

        // Check to see if the entire string can fit.
        requiredLength = WideCharToUTF8Escaped(str, characterCount, NULL, 0);

        if(edit->limitCount - edit->contentUTF8ByteCount - requiredLength >= 0)
            return characterCount;

        // It was not possible to fit the entire string.
        // Convert the entire string to UTF8 and figure out how much will fit.
        {
            char* narrowStr = NULL;
            int allowedCharacterCount = 0;
            int allowedUTF8ByteCount = 0;
            int availableByteCount = edit->limitCount - edit->contentUTF8ByteCount;
            char* cursor;
            char* lastCursor;

            estrCreate(&narrowStr);
            estrReserveCapacity(&narrowStr, requiredLength+1);

            WideCharToUTF8Escaped(str, characterCount, narrowStr, requiredLength+1);

            lastCursor = cursor = narrowStr;

            while(cursor = UTF8EscapedGetNextCharacter(cursor))
            {
                int characterLength = cursor - lastCursor;

                // Can we fit this character into the box?
                if(availableByteCount - allowedUTF8ByteCount - characterLength < 0)
                    break;

                // If so, record that one more character can be inserted.
                allowedUTF8ByteCount += characterLength;
                allowedCharacterCount++;
                lastCursor = cursor;
            }

            estrDestroy(&narrowStr);
            return allowedCharacterCount;
        }
    }
    assert(0 && "bad UIEdit content limit type");
    return 0;
}

static void uiEditUpdateContentCount(UIEdit* edit, int add, unsigned short* str, unsigned int charCount)
{
    int narrowByteCount;

    if(edit->limitType == UIECL_UTF8_ESCAPED)
    {
        narrowByteCount = WideCharToUTF8Escaped(str, charCount, NULL, 0);
    }
    else
    {
        narrowByteCount = WideCharToMultiByte(CP_UTF8, 0, str, charCount, 0, 0, NULL, NULL);
    }

    if(add)
    {
        if(edit->limitType == UIECL_UTF8 || edit->limitType == UIECL_UTF8_ESCAPED)
        {
            verify(edit->contentUTF8ByteCount + narrowByteCount <= edit->limitCount);
        }
        else if(edit->limitType == UIECL_WIDE)
        {
            verify(edit->contentCharacterCount + (int)charCount <= edit->limitCount);
        }
        edit->contentUTF8ByteCount += narrowByteCount;
        edit->contentCharacterCount += charCount;
    }
    else
    {
        edit->contentUTF8ByteCount -= narrowByteCount;
        edit->contentCharacterCount -= charCount;
    }
}

// Returns the number of characters inserted.
static int uiEditLineInsertString(UIEdit* edit, UIEditLine* line, int characterIndex, unsigned short* pstr, unsigned int charCount)
{
    int insertLimit;
    wchar_t *str = NULL;

    if(!edit || !line || !pstr || !charCount)
        return 0;

    // --------------------
    // validate the string

    if( edit->nonAsciiBlocked )
    {
        char strIn[1024*4];
        strncpy( strIn, WideToUTF8StrTempConvert( pstr, NULL ), ARRAY_SIZE( strIn ) - 1 );
        stripNonAsciiChars(strIn);
        estrWideConcatCharString( &str, UTF8ToWideStrConvertStatic( strIn ) );
    }
    else
    {
        estrWideConcatCharString( &str, pstr );
    }

    // --------------------
    // insert it

    insertLimit = uiEditGetStringInsertLimit(edit, str, charCount);
    if(insertLimit)
    {
        estrWideInsert(&line->lineText, characterIndex, str, insertLimit);
        uiEditUpdateContentCount(edit, 1, str, insertLimit);
        uiEditLineMarkDirty(edit, line);
    }

    // --------------------
    // finally

    estrWideDestroy( &str );
    return insertLimit;
}

static int strWideFindCharacter(unsigned short* str, int charCount, unsigned short character)
{
    int i;

    for(i = 0; i < charCount; i++)
    {
        if(str[i] == '\n')
        {
            return i;
        }
    }
    return -1;
}

static bool uiEditLineSetNewline(UIEdit* edit, UIEditLine* line, int hasNewLine)
{
    int addNewLine = 0;
    hasNewLine = (hasNewLine ? 1 : 0);
    if(!edit || !line)
        return false;

    // Setting newline status on the line to the same status as before?
    if(line->hasNewLine == hasNewLine)
        return true;

    if(hasNewLine)
        uiEditUpdateContentCount(edit, hasNewLine, newlineCharacter, 1);
    else
        uiEditUpdateContentCount(edit, hasNewLine, newlineCharacter, -1);

    line->hasNewLine = hasNewLine;
    return true;
}

static void uiEditLineMarkDirty(UIEdit* edit, UIEditLine* line)
{
    edit->dirty = 1;    // If some line is dirty, the entire control needs reformatting.
    line->dirty = 1;
}

static UIEditLine* uiEditAddEmptyLine(UIEdit* edit, int lineIndex)
{
    UIEditLine* line = uiEditLineCreate();
    lineIndex = MINMAX(lineIndex, 0, eaSize(&edit->lines));
    if(!edit->lines)
        eaCreate(&edit->lines);
    eaInsert(&edit->lines, line, lineIndex);
    return line;
}

static UIEditLine* uiEditGetLineOrCreate(UIEdit* edit, int absOffset)
{
    UIEditLine* line = eaGet(&edit->lines, absOffset);
    if(!line)
    {
        line = uiEditLineCreate();
        eaPush(&edit->lines, line);
    }
    return line;
}

static UIEditLine* uiEditGetCursorLine(UIEdit* edit);
static UIEditLine* uiEditGetCursorOffsetLine(UIEdit* edit, int lineOffset);
static int uiEditGetCursorLineIndex(UIEdit* edit, int offset);
static int uiEditGetCursorCharacterIndex(UIEdit* edit, int offset);

static UIEditLine* uiEditGetCursorLine(UIEdit* edit)
{
    int validatedIndex = uiEditGetCursorLineIndex(edit, 0);
    return eaGet(&edit->lines, validatedIndex);
}

static UIEditLine* uiEditGetCursorOffsetLine(UIEdit* edit, int lineOffset)
{
    int validatedIndex = uiEditGetCursorLineIndex(edit, lineOffset);
    return eaGet(&edit->lines, validatedIndex);
}

// Retrieve the index of the line that is "offset" amount from the line the cursor is on.
static int uiEditGetCursorLineIndex(UIEdit* edit, int offset)
{
    int newIndex = edit->cursor.lineIndex + offset;
    newIndex = MINMAX(newIndex, 0, eaSize(&edit->lines)-1);
    return newIndex;
}

static int uiEditCanGetCursorOffsetLine(UIEdit* edit, int offset)
{
    int index = edit->cursor.lineIndex + offset;
    if(index == MINMAX(index, 0, eaSize(&edit->lines)-1))
        return 1;
    else
        return 0;
}

// Retrieve the index of the character that is "offset" amount from the character the cursor is on.
static int uiEditGetCursorCharacterIndex(UIEdit* edit, int offset)
{
    UIEditLine* line = uiEditGetCursorLine(edit);
    int newIndex;

    assert(line);
    newIndex = edit->cursor.characterIndex + offset;
    newIndex = MINMAX(newIndex, 0, (int)estrWideLength(&line->lineText));
    return newIndex;
}

static int uiEditHasLine(UIEdit* edit, int absOffset)
{
    int newIndex;
    newIndex = absOffset;
    newIndex = MINMAX(newIndex, 0, eaSize(&edit->lines)-1);
    return newIndex == absOffset;
}

typedef enum
{
    UIECM_LEFT,
    UIECM_RIGHT,
    UIECM_UP,
    UIECM_DOWN,
    UIECM_NEXT_SPACE,
    UIECM_PREV_SPACE,
} UIEditCursorMovement;

static void uiEditFixLineCursor(UIEdit* edit)
{
    int validateLineIndex = uiEditGetCursorLineIndex(edit, 0);
    UIEditLine* line = uiEditGetCursorLine(edit);
    assert(line);

    // Is the cursor pointing at some invalid line?
    if(validateLineIndex != edit->cursor.lineIndex)
    {
        edit->cursor.lineIndex = validateLineIndex;
        edit->cursor.characterIndex = estrWideLength(&line->lineText);
        edit->cursorMoved = 1;
    }

    // Is the cursor pointing at a previous line?
    if(edit->cursor.characterIndex < 0)
    {
        // Don't do anything if it is not possible to put the cursor
        // at the prev line.
        int prevLineIndex = uiEditGetCursorLineIndex(edit, -1);
        if(edit->cursor.lineIndex == prevLineIndex)
        {
            edit->cursor.characterIndex = 0;
            return;
        }

        // Set the line cursor to the last line.
        edit->cursor.lineIndex = uiEditGetCursorLineIndex(edit, -1);

        // Fix up the character cursor so it's pointing at something valid.
        line = uiEditGetCursorLine(edit);
        edit->cursor.characterIndex += estrWideLength(&line->lineText)+1;
        edit->cursorMoved = 1;
    }

    // Is the cursor pointing at the next line?
    if(edit->cursor.characterIndex > (int)estrWideLength(&line->lineText))
    {
        int nextLineIndex = uiEditGetCursorLineIndex(edit, 1);
        if(edit->cursor.lineIndex == nextLineIndex)
        {
            edit->cursor.characterIndex = estrWideLength(&line->lineText);
            return;
        }

        // Set the line cursor to the next line.
        edit->cursor.lineIndex = nextLineIndex;

        // Fix up the character cursor so it's pointing at something valid.
        edit->cursor.characterIndex = edit->cursor.characterIndex - estrWideLength(&line->lineText) - 1;
        edit->cursorMoved = 1;
    }
}

static void uiEditFixCharacterCursor(UIEdit* edit)
{
    edit->cursor.characterIndex = uiEditGetCursorCharacterIndex(edit, 0);
}

void uiEditSetCursorPos(UIEdit* edit, int idx)
{
    edit->cursor.characterIndex = 0;
    edit->cursor.characterIndex = uiEditGetCursorCharacterIndex(edit, idx);
}

typedef enum
{
    UIECD_FORWARD = 1,
    UIECD_BACKWARD = -1,
} UIEditCursorDirection;


static int uiEditCursorSkipNonwhiteSpace(UIEdit* edit, int startIndex, UIEditCursorDirection direction)
{
    int i;
    UIEditLine* line = uiEditGetCursorLine(edit);

    for(i = MINMAX(startIndex + direction, 0, (int)estrWideLength(&line->lineText)); i >= 0 && i <= (int)estrWideLength(&line->lineText); i += direction)
    {
        if(!iswspace(line->lineText[i]))
            continue;
        else
            break;
    }

    return i;
}

static int uiEditCursorSkipWhiteSpace(UIEdit* edit, int startIndex, UIEditCursorDirection direction)
{
    int i;
    UIEditLine* line = uiEditGetCursorLine(edit);

    for(i = MINMAX(startIndex + direction, 0, (int)estrWideLength(&line->lineText)); i >= 0 && i <= (int)estrWideLength(&line->lineText); i += direction)
    {
        if(iswspace(line->lineText[i]))
            continue;
        else
            break;
    }

    return i;
}

// Return in the index of the next space, in the specified direction, starting from the cursor location.
static int uiEditCursorFindSpace(UIEdit* edit, int index, UIEditCursorDirection direction)
{
    UIEditLine* line = uiEditGetCursorLine(edit);

    if(!line->lineText)
        return 0;
    if(direction == UIECD_FORWARD)
    {
        index = uiEditCursorSkipNonwhiteSpace(edit, index, direction);
        index = uiEditCursorSkipWhiteSpace(edit, index, direction);
    }
    else
    {
        index = uiEditCursorSkipWhiteSpace(edit, index, direction);
        index = uiEditCursorSkipNonwhiteSpace(edit, index, direction);
        index += 1;
    }

    index = MINMAX(index, 0, (int)estrWideLength(&line->lineText));
    return index;
}

static int uiEditMoveCursor(UIEdit* edit, UIEditCursorMovement movement)
{
    UIEditCursor oldCursor = edit->cursor;
    switch(movement)
    {
    case UIECM_LEFT:
        // Incremement the cursor.
        edit->cursor.characterIndex--;

        // Fix up the cursor because it might be pointing to an invalid location now.
        uiEditFixLineCursor(edit);

        break;

    case UIECM_RIGHT:
        edit->cursor.characterIndex++;
        uiEditFixLineCursor(edit);
        break;
    case UIECM_UP:
        // If we're already at the first line, move the cursor to the beginning of the line.
        if(!edit->cursor.lineIndex)
            edit->cursor.characterIndex = 0;
        else
        {
            int xloc, yloc;

            edit->cursor.lineIndex--;

            yloc = edit->cursor.lineIndex*edit->fontHeight - edit->vertScroll;
            xloc = edit->cursor.lastXPos;
            uiEditMoveCursorToClosestCharacter(edit, xloc, yloc);

            uiEditFixCharacterCursor(edit);
            uiEditFixLineCursor(edit);
        }
        break;
    case UIECM_DOWN:
        if(edit->cursor.lineIndex == eaSize(&edit->lines) - 1)
        {
            UIEditLine* line = uiEditGetCursorLine(edit);
            edit->cursor.characterIndex = estrWideLength(&line->lineText);
        }
        else
        {
            int xloc, yloc;

            edit->cursor.lineIndex++;

            yloc = edit->cursor.lineIndex*edit->fontHeight - edit->vertScroll;
            xloc = edit->cursor.lastXPos;
            uiEditMoveCursorToClosestCharacter(edit, xloc, yloc);

            uiEditFixCharacterCursor(edit);
            uiEditFixLineCursor(edit);
        }
        break;
    case UIECM_NEXT_SPACE:
        {
            int nextSpaceIndex = uiEditCursorFindSpace(edit, edit->cursor.characterIndex, 1);
            if(nextSpaceIndex == edit->cursor.characterIndex)
            {
                uiEditMoveCursor(edit, UIECM_RIGHT);
            }
            else
            {
                edit->cursor.characterIndex = nextSpaceIndex;
            }
        }
        break;
    case UIECM_PREV_SPACE:
        {
            int prevSpaceIndex = uiEditCursorFindSpace(edit, edit->cursor.characterIndex, -1);
            if(prevSpaceIndex == edit->cursor.characterIndex)
            {
                uiEditMoveCursor(edit, UIECM_LEFT);
            }
            else
            {
                edit->cursor.characterIndex = prevSpaceIndex;
            }

        }
        break;
    }

    // Report if the cursor was moved.
    if(memcmp(&oldCursor, &edit->cursor, sizeof(UIEditCursor)) == 0)
        return 0;

    edit->cursorMoved = 1;
    return 1;
}

void uiEditShowCursor(UIEdit* edit)
{
    edit->drawCursor = 1;
    edit->cursorTimer = edit->cursorInterval;
}

static void uiEditReformat(UIEdit* edit)
{
    int i;
    int alteredSection = 0;

    ttWrapperSetFont(edit->wrapper, edit->font);

    // Examine each of the existing lines and reformat all the dirty lines.
    for(i = 0; i < eaSize(&edit->lines); i++)
    {
        UIEditLine* curLine = edit->lines[i];

        // If a line is dirty, all lines starting from it must be reformatted.
        if(curLine->dirty)
            alteredSection = 1;
        curLine->dirty = 0;

        // Special case to avoid a bunch of line flow-backs.
        // If a line has become completely emtpy and does not have a newline character on it, it can be safely deleted
        // from the lines buffer.
        if(estrWideLength(&curLine->lineText) == 0 && !curLine->hasNewLine && i != eaSize(&edit->lines)-1)
        {
            eaRemove(&edit->lines, i);
            uiEditLinePrepareDestroy(edit, curLine);
            uiEditLineDestroy(curLine);
            i--;
            if(i < (int)edit->cursor.lineIndex)
                edit->cursor.lineIndex--;
            continue;
        }

        if(alteredSection || edit->boundsChanged)
        {
            TTTextLine* wrappedLine;
            UIEditLine* nextLine;
            unsigned int newCharacterCount;
            ttWrapperSetTextWide(edit->wrapper, curLine->lineText, estrWideLength(&curLine->lineText));
             wrappedLine = ttGetLine(edit->wrapper, max(edit->boundsText.width/*edit->textScale*/, edit->minWidth), 0, edit->textScale);

            // The only time this happens is when the line is too small to fit even a single character.
            // If the line already exists and now cannot be wrapped at all, this means someone has shrunk the
            // edit control width way too small.  Doing nothing is the right behavior.
            if(!wrappedLine)
            {
                curLine->dirty = 0;    // Remember to mark the line as clean so we don't format this line again.
                continue;
            }

            newCharacterCount = wrappedLine->characterCount;
            assert(newCharacterCount <= estrWideLength(&curLine->lineText));    // Only supports inserts right now.

            // Line overflow?
            if(newCharacterCount < estrWideLength(&curLine->lineText))
            {
                bool hasNewLine = curLine->hasNewLine;
                // Grab the next line...
                if (hasNewLine) nextLine = uiEditAddEmptyLine(edit, i+1);
                else nextLine = uiEditGetLineOrCreate(edit, i+1);
                assert(nextLine);

                // and place the extra text there.
                estrWideInsert(&nextLine->lineText, 0, &curLine->lineText[newCharacterCount], estrWideLength(&curLine->lineText) - newCharacterCount);


                // Get rid of the exta text from the current line.
                estrWideSetLength(&curLine->lineText, newCharacterCount);

                uiEditLineSetNewline(edit, curLine, 0);
                uiEditLineSetNewline(edit, nextLine, hasNewLine);

                if((signed)edit->cursor.lineIndex>i && hasNewLine)
                {
                    edit->cursor.lineIndex++;
                }
                else if((signed)edit->cursor.lineIndex==i && edit->cursor.characterIndex>(signed)newCharacterCount)
                {
                    edit->cursor.characterIndex++;
                }
            }
            else
            {
                // Line underflow?
                if(!curLine->hasNewLine && uiEditHasLine(edit, i+1))
                {
                    int nextLineIndex = i + 1;
                    int rwidth = wrappedLine->remainderWidth - 15;
                    if (rwidth < 0) rwidth = 0;
                    nextLine = edit->lines[nextLineIndex];

                    // Otherwise, try to fit text from the next line into the left over space.
                    ttWrapperSetTextWide(edit->wrapper, nextLine->lineText, estrWideLength(&nextLine->lineText));
                    wrappedLine = ttGetLine(edit->wrapper, rwidth ,edit->boundsText.width, edit->textScale);
                    if(wrappedLine)
                    {
                        // Move the text to be flown back from the beginning of the next line to the current line.
                        estrWideInsert(&curLine->lineText, estrWideLength(&curLine->lineText), nextLine->lineText, wrappedLine->characterCount);

                        // Remove the same characters from the beginning of the next line.
                        estrWideRemove(&nextLine->lineText, 0, wrappedLine->characterCount);

                        // Flowing back something that's under the cursor?
                        if(nextLineIndex == edit->cursor.lineIndex && edit->cursor.characterIndex <= wrappedLine->characterCount)
                        {
                            edit->cursor.characterIndex -= max(wrappedLine->characterCount+1, 1);
                            uiEditFixLineCursor(edit);
                        }
                    }
                    // Has the contents of the last line been flowed back?
                    // If so, delete the line.
                    if(estrWideLength(&nextLine->lineText) == 0)
                    {
                        int nextLineHasNewLine = nextLine->hasNewLine;
                        // FIXEAR: Is this safe considering the for loop index?
                        eaRemove(&edit->lines, nextLineIndex);
                        uiEditLinePrepareDestroy(edit, nextLine);
                        uiEditLineDestroy(nextLine);
                        uiEditLineSetNewline(edit, curLine, nextLineHasNewLine);
                    }
                }
            }

            // If a line has a newline character in it, the next line does not need to be reformatted.
            if(curLine->hasNewLine)
                alteredSection = 0;
        }
        uiEditFixLineCursor(edit);
    }
    uiEditFixLineCursor(edit);
    edit->boundsChanged = 0;
}


/* Function uiEditInsertString()
*    Internal function. Contains no error checking.
*    Deletes the character following the cursor.
*/
static void uiEditRemove(UIEdit* edit)
{
    UIEditLine* curLine = uiEditGetCursorLine(edit);
    UIEditLine* prevLine = uiEditGetCursorOffsetLine(edit, -1);
    unsigned short character = (curLine&&curLine->lineText ? curLine->lineText[edit->cursor.characterIndex] : 0);
    if(character)
    {
        estrWideRemove(&curLine->lineText, edit->cursor.characterIndex, 1);
    }
    edit->cursor.characterIndex = uiEditGetCursorCharacterIndex(edit, 0);
    uiEditLineMarkDirty(edit, curLine);
    uiEditLineMarkDirty(edit, prevLine);        // Mark the prev line as dirty so that any flow back can happen.

    uiEditUpdateContentCount(edit, 0, &character, 1);
}

/* Function uiEditInsertString()
 *    Internal function. Contains no error checking.
 *    Inserts the specified string at the cursor location.
 */
static void uiEditInsertString(UIEdit* edit, unsigned short* str, unsigned int charCount)
{
    UIEditLine* curLine;

    if(verify(edit && str && charCount))
    {
        curLine = uiEditGetCursorLine(edit);
        uiEditLineInsertString(edit, curLine, edit->cursor.characterIndex, str, charCount);
        edit->cursor.characterIndex += charCount;
        edit->cursorMoved = 1;
    }
}



static void uiEditInsertUnicodeCharacter(UIEdit* edit, unsigned short character)
{
    unsigned short str[2] = {character, 0};

    if(uiEditGetStringInsertLimit(edit, str, 1)
       && (!edit->nonAsciiBlocked || !wcharFilter(character)))
    {
        uiEditInsertString(edit, str, 1);
        // -AB: this shouldn't be here :2005 Jun 10 03:51 PM
        //edit->cursorMoved = 1;

        if(edit->keyClick)
            sndPlay(edit->keyClick, SOUND_PLAYER);
    }
    else
    {
        if(edit->fullClick)
            sndPlay(edit->fullClick, SOUND_PLAYER);
    }
}

static void uiEditInsertAsciiCharacter(UIEdit* edit, char character)
{
    if( ( !edit->respectWhiteSpace &&
          ( character == 127 /*delete*/ ||
            character == '\t' )
        ) ||
        character == 13 /*carraige return*/ ||
        character == 27 /*escape*/ ||
        character == '`' )
        return;
    uiEditInsertUnicodeCharacter(edit, character);
}

// returns the index of the line that was broken.
static int uiEditBreakLineAtCursor(UIEdit* edit)
{
    UIEditLine* newLine = uiEditLineCreate();
    UIEditLine* curLine = uiEditGetCursorLine(edit);
    int characterToMove = estrWideLength(&curLine->lineText) - edit->cursor.characterIndex;

    if( characterToMove < 0 )
        characterToMove = 0;

    // Insert the new line right after the line the cursor is on.
    eaInsert(&edit->lines, newLine, edit->cursor.lineIndex + 1);
    //uiEditLineInsertString( edit, newLine, 0, &curLine->lineText[edit->cursor.characterIndex], characterToMove);
    estrWideInsert(&newLine->lineText, 0, &curLine->lineText[edit->cursor.characterIndex], characterToMove);
    estrWideRemove(&curLine->lineText, edit->cursor.characterIndex, characterToMove);

    edit->cursor.characterIndex = estrWideLength(&curLine->lineText);

    edit->cursor.characterIndex = 0;
    edit->cursor.lineIndex += 1;
    uiEditLineMarkDirty(edit, newLine);

    return edit->cursor.lineIndex - 1;
}

// Move the vert scroll to where the cursor is.
static void uiEditUpdateScrollLocation(UIEdit* edit)
{
    int fontHeight = edit->fontHeight;
    unsigned int beginLineIndex;
    unsigned int endLineIndex;

    // Which lines are in the viewing area right now?
    beginLineIndex = edit->vertScroll/fontHeight;
    endLineIndex = (edit->vertScroll + edit->boundsText.height)/fontHeight;

    if(!uiEditSingleLineMode(edit))
    {
        // Is the cursor above or below the viewing area?
        if(edit->cursor.lineIndex > beginLineIndex && edit->cursor.lineIndex < endLineIndex)
            return;
    }

    // If so, move the viewing area so that the cursor falls within it.
    if(edit->cursor.lineIndex <= beginLineIndex)
    {
        edit->vertScroll = edit->cursor.lineIndex * fontHeight;
    }
    else
    {
        edit->vertScroll = (edit->cursor.lineIndex+1) * fontHeight - edit->boundsText.height;
    }
}

//---------------------------------------------------------------------------------------------------------------
// UIEdit: Public interface
//---------------------------------------------------------------------------------------------------------------
static UIEditLine* uiEditLineCreate()
{
    return calloc(1, sizeof(UIEditLine));
}

static void uiEditLineDestroy(UIEditLine* line)
{
    if(!line)
        return;
    if(line->lineText)
        estrWideDestroy(&line->lineText);
    free(line);
}

static void uiEditLinePrepareDestroy(UIEdit* edit, UIEditLine* line)
{
    uiEditLineSetNewline(edit, line, 0);
}

MP_DEFINE(UIEdit);
static UIEdit* uiedit_Create( TTTextWrapper *wrapper, F32 cursorInterval, F32 textScale )
{
    UIEdit *res = NULL;

    // create the pool,
    MP_CREATE(UIEdit, 5);
    res = MP_ALLOC( UIEdit );
    if( verify( res ))
    {
        res->wrapper = wrapper;
        res->cursorInterval = cursorInterval;
        res->textScale = textScale;
    }

    // --------------------
    // finally, return

    return res;
}

static void uiEditLineDestroy(UIEditLine* line);
static void uiEditLineDestroyCallback(void* arg0)
{
    uiEditLineDestroy((UIEditLine*)arg0);
}

static void uiedit_Destroy( UIEdit *edit )
{
    if( edit )
    {
        if(edit->lines)
        {
            eaDestroyEx(&edit->lines, uiEditLineDestroyCallback);
        }
        if(edit->wrapper)
        {
            ttTextWrapperDestroy(edit->wrapper);
        }

        MP_FREE(UIEdit, edit);
    }
}



UIEdit* uiEditCreate()
{
//     UIEdit* edit = calloc(1, sizeof(UIEdit));
//     edit->wrapper = ttTextWrapperCreate();
//     edit->cursorInterval = 0.55;
//     edit->textScale = 1.0;
    UIEdit *edit = uiedit_Create( ttTextWrapperCreate(), .55f, 1.f );
    if( verify( edit  ))
    {
        uiEditAddEmptyLine(edit, 0);
        uiEditSetClicks(edit, "type2", "mouseover");
    }
    return edit;
}

void uiEditDestroy(UIEdit* edit)
{
    uiedit_Destroy(edit);
}

static void uiEditLineDestroyAdapter(void* arg0)
{
    uiEditLineDestroy((UIEditLine*)arg0);
}

void uiEditClear(UIEdit* edit)
{
    if(!edit)
        return;
    eaClearEx(&edit->lines, uiEditLineDestroyAdapter);
    uiEditAddEmptyLine(edit, 0);
    edit->cursor.lineIndex = 0;
    edit->cursor.characterIndex = 0;
    edit->contentCharacterCount = 0;
    edit->contentUTF8ByteCount = 0;
}

static void s_UpdateBoundsLayout(UIEdit *edit)
{
    if( verify( edit ))
    {
        float oldWidth;
        struct BoundsLayout *layout = &edit->boundsLayout;

        oldWidth = edit->boundsText.width;

        edit->boundsText = edit->boundsPage;
        edit->boundsText.x += layout->leftIndent;
        edit->boundsText.width -= layout->leftIndent;

        // reformat if text width has changed
        if(oldWidth != edit->boundsText.width)
        {
            edit->dirty = 1;
            edit->boundsChanged = 1;
        }
    }
}

void uiEditSetBounds(UIEdit* edit, UIBox bounds)
{
    if( verify( edit ))
    {
        // --------------------
        // set the page bounds

        edit->boundsPage = bounds;

        // --------------------
        // update the layout

        s_UpdateBoundsLayout(edit);
    }
}

void uiEditSetLeftIndent(UIEdit *edit, F32 dxLeft)
{
    if( verify( edit ))
    {
        edit->boundsLayout.leftIndent = dxLeft;
        s_UpdateBoundsLayout(edit);
    }
}


void uiEditSetFontScale(UIEdit* edit, float scale)
{
    float oldScale;
    if(!edit)
        return;

    oldScale = edit->textScale;
    edit->textScale = scale;

    if(oldScale != edit->textScale)
    {
        float sc = edit->textScale;
         edit->dirty = 1;
        edit->boundsChanged = 1;
        reverseToScreenScalingFactorf( NULL, &sc );
        edit->fontHeight = ttGetFontHeight(edit->font,sc, sc);
    }
}

void uiEditSetMinWidth(UIEdit* edit, float minWidth)
{
    if(!edit)
        return;
    if(minWidth != edit->minWidth)
    {
        edit->minWidth = minWidth;
        edit->dirty = 1;
        edit->boundsChanged = 1;
    }
}

int uiEditCanReceiveInput(UIEdit* edit)
{
    if(!edit || !edit->canEdit || !uiInFocus(edit))
        return 0;
    else
        return 1;
}

static int uiEditSingleLineMode(UIEdit* edit)
{
    int fitLines = edit->boundsText.height / edit->fontHeight;
    if(fitLines == 1)
        return 1;
    else
        return 0;
}


void uiEditDisplay(UIEdit* edit)
{
    static char *s_str = NULL;
    static unsigned int s_iSize = 0;
    int i;
     int fontHeight = edit->fontHeight;
    int iSize;
    float dxPosCursor;
    float fXOffset = 0;
    PointFloatXYZ pen = {edit->boundsText.x, edit->boundsText.y, edit->z};

    // Update the edit cursor timer.
    if(edit->inEditMode)
    {
        edit->cursorTimer -= TIMESTEP/30;
        if(edit->cursorTimer <= 0.f)
        {
            edit->drawCursor = !edit->drawCursor;
            edit->cursorTimer += edit->cursorInterval;
        }
    }
    else
    {
        edit->drawCursor = 0;
    }

    // Display all lines.
    font(edit->font);
    font_color(edit->textColor, edit->textColor);

    if(uiEditSingleLineMode(edit))
    {
        int linesToSkip = ceil(edit->vertScroll / fontHeight);
        pen.y -= linesToSkip * fontHeight;
    }
    else
    {
        pen.y -= edit->vertScroll;
    }

    // get the dxPosCursor, and in-bounds the pen's x coord
    {
        float fStringWidth;

        if(edit->password)
        {
            int iWidth;
            if(estrWideLength(&edit->lines[edit->cursor.lineIndex]->lineText)+1 > s_iSize)
            {
                s_iSize = estrWideLength(&edit->lines[edit->cursor.lineIndex]->lineText)+1;
                s_str = realloc(s_str, s_iSize);
            }
            memset(s_str, '*', s_iSize-1);
            s_str[s_iSize-1]='\0';

            strDimensionsBasic(edit->font, edit->textScale, edit->textScale,
                s_str, edit->cursor.characterIndex, &iWidth, NULL, NULL);
            dxPosCursor = iWidth;
            reverseToScreenScalingFactorf(&dxPosCursor, NULL);

            strDimensionsBasic(edit->font, edit->textScale, edit->textScale,
                s_str, edit->contentCharacterCount, &iWidth, NULL, NULL);
            fStringWidth = iWidth;
            reverseToScreenScalingFactorf(&fStringWidth, NULL);
        }
        else
        {
            strDimensionsScaledWide(edit->font, edit->textScale, edit->textScale,
                edit->lines[edit->cursor.lineIndex]->lineText,
                edit->cursor.characterIndex, &dxPosCursor, NULL, NULL);

            strDimensionsScaledWide(edit->font, edit->textScale, edit->textScale,
                edit->lines[edit->cursor.lineIndex]->lineText,
                estrWideLength(&edit->lines[edit->cursor.lineIndex]->lineText),
                &fStringWidth, NULL, NULL);

        }

        if(dxPosCursor>edit->boundsText.width)
        {
            pen.x = pen.x+edit->boundsText.width-dxPosCursor;
        }
        else if(edit->rightAlign)
        {
            fXOffset = edit->boundsText.width-fStringWidth;
        }
    }

    iSize = eaSize(&edit->lines);
    for(i = 0; i < iSize; i++)
    {
        UIEditLine* line = edit->lines[i];
        int rgba[4];
        UIBox lineBox;

        uiBoxDefine(&lineBox, pen.x, pen.y, edit->minWidth, fontHeight);
        pen.y += fontHeight;

        // Only draw lines that will be visible.
        //if(!clipperIntersects(&lineBox))
        //    continue;

        edit->cursor.lastXPos = (int)(dxPosCursor+fXOffset);

        if(edit->drawCursor && edit->cursor.lineIndex == i)
        {
            float scx=1.0f , scy=1.0f;
            AtlasTex *ptex = atlasLoadTexture("map_zoom_uparrow.tga");
            CBox bx = {0};

            reverseToScreenScalingFactorf(&scx, &scy);

            clipperPush(0);

            // store the drawn position. NOTE: don't include the 1/2 texture
            // width offset because this position represents the cursor location
            // not the texture location.
            edit->cursor.xDrawn = pen.x + dxPosCursor + fXOffset;
            display_sprite(ptex,
                           edit->cursor.xDrawn + scx * (-ptex->width/2),
                           (pen.y-fontHeight*.25),
                           pen.z,
                           scx*edit->textScale, scy*edit->textScale, CLR_WHITE);

            // TODO: I couldn't get the top cursor to work well. It's fine
            // until you have an entry box which has variant text sizes, like
            // the chat box.
            // display_rotated_sprite(ptex, (pen.x + width + scx * (-ptex->width/2)), (pen.y-cursorHeight*1.25), pen.z, scx, scy, CLR_WHITE, PI, 0);

            clipperPop();
        }

        determineTextColor(rgba);

         if(edit->password)
        {
            printBasic(edit->font, pen.x+fXOffset, pen.y, pen.z, edit->textScale, edit->textScale, 0, s_str, estrWideLength(&line->lineText), rgba);
        }
        else
        {
            printBasicWide(edit->font, pen.x+fXOffset, pen.y, pen.z,
                edit->textScale, edit->textScale,
                0, line->lineText, estrWideLength(&line->lineText), rgba);
        }

        // --------------------
        // underline

        if( line->underline.active )
        {
            UIUnderline *underline = &line->underline;
            int len = estrWideLength(&line->lineText);
            S32 diUnderline = -1;

            // get the number of characters to underline
            if( INRANGE0( underline->iStart, len ) )
            {
                // inbounds the underline
                if( underline->diLen >= 0 && underline->iStart + underline->diLen > len )
                {
                    underline->diLen = len;
                }

                // set the local
                diUnderline = underline->diLen < 0 ? len - underline->iStart : underline->diLen;
            }


            if( diUnderline < 0 )
            {
                uiunderline_Clear(underline);
            }
            else
            {
                F32 dx_ul = 0.f;
                F32 dx_ulStart = 0.f;
                wchar_t *str_ul = line->lineText + underline->iStart;
                F32 x_ul;
                F32 y_ul = pen.y;

                // get the width to the start of the underline
                strDimensionsScaledWide(edit->font, edit->textScale, edit->textScale,
                                        line->lineText,
                                        underline->iStart,
                                        &dx_ulStart,
                                        NULL, NULL);

                // get the width of the underline string
                strDimensionsScaledWide(edit->font, edit->textScale, edit->textScale,
                                        str_ul,
                                        diUnderline,
                                        &dx_ul,
                                        NULL, NULL);
                x_ul = pen.x + dx_ulStart;

                // draw the line
                drawHorizontalLine( x_ul+fXOffset, y_ul, dx_ul, pen.z, edit->textScale, CLR_WHITE );
            }
        }
    }

    // Display selection tint on selected text.
    // IMPLEMENTME!!!
}

//------------------------------------------------------------
//  act as if the backspace key had been pressed, moves the cursor
// backward and delete the character there, join lines if necessary
//----------------------------------------------------------
void uiedit_Backspace(UIEdit *edit, U32 amount)
{
    if( verify( edit ))
    {
        U32 i;
        UIEditLine* prevLine = uiEditGetCursorOffsetLine(edit, -1);

        for( i = 0; i < amount; ++i )
        {
            // If a newline is there in the previous line, remove that first.
            if(edit->cursor.characterIndex == 0 && uiEditCanGetCursorOffsetLine(edit, -1) && prevLine->hasNewLine)
            {
                uiEditLineSetNewline(edit, prevLine, 0);
                uiEditLineMarkDirty(edit, prevLine);
            }

            // otherwise, try to remove the character to the left of the cursor
            else if(uiEditMoveCursor(edit, UIECM_LEFT))
            {
                uiEditRemove(edit);
            }
        }
    }
}



#include "win/win_init.h"
void uiEditDefaultKeyboardHandler(UIEdit* edit, KeyInput* input)
{
    if(!edit || !input)
        return;

    // Handle character inserts.
    if(KIT_Ascii == input->type )
    {
        uiEditInsertAsciiCharacter(edit, input->asciiCharacter);
    }
    else if(KIT_Unicode == input->type)
    {
        uiEditInsertUnicodeCharacter(edit, input->unicodeCharacter);
    }
    else if(KIT_EditKey == input->type)
    {
        bool bDidSomething = true;

        // Handle text delete.
        if(INP_DELETE == input->key || INP_DECIMAL == input->key)
        {
            UIEditLine* line = uiEditGetCursorLine(edit);
            int removeCharacter = 1;
            int moveCursorBack = 0;

            // If the cursor is sitting at the end of a line, see if we can delete characters
            // from the next line.  If not, the cursor is sitting at the very end of entire
            // body of text.  Do nothing.
            if(edit->cursor.characterIndex == estrWideLength(&line->lineText))
            {
                if(line->hasNewLine)
                {
                    uiEditLineSetNewline(edit, line, 0);
                    uiEditLineMarkDirty(edit, line);
                    uiEditMoveCursor(edit, UIECM_RIGHT);
                    return;
                }
                if(!uiEditMoveCursor(edit, UIECM_RIGHT))
                {
                    removeCharacter = 0;
                }
                else
                {
                    moveCursorBack = 1;
                }
            }

            if(removeCharacter)
            {
                uiEditRemove(edit);
                if(moveCursorBack)
                    uiEditMoveCursor(edit, UIECM_LEFT);
            }

        }
        else if(INP_BACK == input->key)
        {
            uiedit_Backspace(edit, 1);
        }
        // Handle paste.
        else if(INP_V == input->key && input->attrib & KIA_CONTROL)
        {
            if(OpenClipboard(hwnd)){
                HANDLE handle = GetClipboardData(CF_UNICODETEXT);

                if(handle){
                    unsigned short* data = GlobalLock(handle);
                    uiEditPasteWideText(edit, data);
                }

                GlobalUnlock(handle);
                CloseClipboard();
            }
        }
        // Handle copy.
        //
        // If text has been altered, mark the line where the text was altered as dirty.
        //
        // Handle cursor key left/right movements.
        else if(INP_LEFT == input->key || INP_NUMPAD4 == input->key)
        {
            if(input->attrib & KIA_CONTROL)
                uiEditMoveCursor(edit, UIECM_PREV_SPACE);
            else
                uiEditMoveCursor(edit, UIECM_LEFT);
        }
        else if(INP_RIGHT == input->key || INP_NUMPAD6 == input->key)
        {
            if(input->attrib & KIA_CONTROL)
                uiEditMoveCursor(edit, UIECM_NEXT_SPACE);
            else
                uiEditMoveCursor(edit, UIECM_RIGHT);
        }
        // Handle cursor key up/down movements.
        else if(INP_UP == input->key || INP_NUMPAD8 == input->key)
        {
            uiEditMoveCursor(edit, UIECM_UP);
        }
        else if(INP_DOWN == input->key || INP_NUMPAD2 == input->key)
        {
            uiEditMoveCursor(edit, UIECM_DOWN);
        }
        else if(INP_RETURN == input->key || INP_NUMPADENTER == input->key)
        {
            int oldCurLineIndex;
            UIEditLine* newLine;
            UIEditLine* oldCurLine;

            if(!edit->disallowReturn
                && uiEditGetStringInsertLimit(edit, newlineCharacter, 1))
            {
                oldCurLineIndex = uiEditBreakLineAtCursor(edit);

                oldCurLine = edit->lines[oldCurLineIndex];
                newLine = edit->lines[oldCurLineIndex+1];

                // Add a newline marker to the line.
                 uiEditLineSetNewline(edit, newLine, oldCurLine->hasNewLine);
                 uiEditLineSetNewline(edit, oldCurLine, 1);
                edit->cursorMoved = true;
            }
        }
        else if(INP_HOME == input->key || INP_NUMPAD7 == input->key)
        {
            // The first "home" keypress brings the cursor to the beginning of the line.
            if(edit->cursor.characterIndex)
                edit->cursor.characterIndex = 0;
            // The second "home" keypress brings the cursor to the beginning of the text.
            else
                edit->cursor.lineIndex = 0;
        }
        else if(INP_END == input->key || INP_NUMPAD1 == input->key)
        {
            UIEditLine* curLine = uiEditGetCursorLine(edit);

            // The first "home" keypress brings the cursor to the end of the line.
            if(edit->cursor.characterIndex != estrWideLength(&curLine->lineText))
                edit->cursor.characterIndex = estrWideLength(&curLine->lineText);
            // The second "home" keypress brings the cursor to the beginning of the text.
            else
            {
                int lastLineIndex = eaSize(&edit->lines) - 1;
                UIEditLine* lastLine = edit->lines[lastLineIndex];
                edit->cursor.lineIndex = lastLineIndex;
                edit->cursor.characterIndex = estrWideLength(&lastLine->lineText);
            }
        }
        else if(INP_ESCAPE == input->key)
        {
            uiEditReturnFocus(edit);
        }
        else
        {
            bDidSomething = false;
        }

        if(bDidSomething && edit->keyClick)
            sndPlay(edit->keyClick, SOUND_PLAYER);

        //
        // Handle text selection.
        //        Did text selection begin?
        //        If so, and if the new selection can't be merge with the old selection...
        //            Mark the begin and end byte of the new selection.
        //
        //        If not, then we're continuing to alter our selection.
        //        Mark the new cursor location as the new end byte for the selection.
        //
    }
    uiEditShowCursor(edit);
}

void uiEditCommaNumericUpdate(UIEdit *edit)
{
    char *estr;
    if(!edit)
        return;
    
    estr=uiEditGetUTF8Text(edit);
    if(estr)
    {
        char ach[256];
        char *pch=estr;
        int iLen;
        int iDest = 0;
        int i=0;
        
        UIEditLine* line = uiEditGetCursorLine(edit);
        int idx = edit->cursor.characterIndex;
        
        // Get all the digits from the string, get rid of
        //   everything else.
        // Try to keep the cursor stable.
        while(*pch && i<255)
        {
            if(*pch>='0' && *pch<='9')
            {
                ach[i++] = *pch;
            }
            else if(i<idx)
            {
                idx--;
            }
            
            pch++;
        }
        ach[i] = '\0';
        
        // Normalize the value
        i = atoi(ach);
        
        // This will leave zeroes when they are editing.
        if(i!=0)
        {
            itoa(i, ach, 10);
        }
        
        // Now reprint it with commas
        iLen = strlen(ach);
        estrReserveCapacity(&estr, iLen+1);
        *estr = '\0';
        
        estr[iDest++] = ach[0];
        for(i=iLen-1; i>0; i--)
        {
            if(i%3==0)
            {
                if(iDest<idx)
                {
                    idx++;
                }
                estr[iDest++] = ',';
            }
            estr[iDest++] = ach[iLen-i];
        }
        estr[iDest++] = '\0';

        uiEditSetUTF8Text(edit, estr);
        uiEditSetCursorPos(edit, idx);

        estrDestroy(&estr);
    }
}

void uiEditCommaNumericEditKeyboardHandler(UIEdit* edit, KeyInput *input)
{
    if(!edit || !input)
        return;

    if(input->type == KIT_Ascii)
    {
        if((input->key >= '0' && input->key <= '9'))
        {
            uiEditDefaultKeyboardHandler(edit, input);
        }
    }
    else if(KIT_EditKey == input->type)
    {
        if(INP_DELETE == input->key || INP_DECIMAL == input->key)
        {
            UIEditLine* line = uiEditGetCursorLine(edit);
            // Fix for COH-8436
            // If the user hasn't yet clicked on an edit field, line->lineText is NULL
            // These tests are included to detect that condition, and prohibit dereferencing
            // it if NULL
            if (line && line->lineText)
            {
                if(edit->cursor.characterIndex != estrWideLength(&line->lineText))
                {
                    if(line->lineText[edit->cursor.characterIndex] == L',')
                    {
                        uiEditMoveCursor(edit, UIECM_RIGHT);
                        uiEditRemove(edit);
                        uiEditMoveCursor(edit, UIECM_LEFT);
                    }
                    else
                    {
                        uiEditRemove(edit);
                    }
                }
            }

        }
        else if(INP_BACK == input->key)
        {
            UIEditLine* line = uiEditGetCursorLine(edit);

            if(edit->cursor.characterIndex>0)
            {
                if(line && line->lineText && line->lineText[edit->cursor.characterIndex-1] == L',')
                {
                    uiEditMoveCursor(edit, UIECM_LEFT);
                }
            }
            uiEditMoveCursor(edit, UIECM_LEFT);
            uiEditRemove(edit);
        }
        else if(INP_LEFT == input->key || INP_NUMPAD4 == input->key)
        {
            UIEditLine* line = uiEditGetCursorLine(edit);

            uiEditMoveCursor(edit, UIECM_LEFT);
            if(edit->cursor.characterIndex > 0)
            {
                if(line && line->lineText && line->lineText[edit->cursor.characterIndex-1] == L',')
                {
                    uiEditMoveCursor(edit, UIECM_LEFT);
                }
            }
        }
        else if(INP_RIGHT == input->key || INP_NUMPAD6 == input->key)
        {
            UIEditLine* line = uiEditGetCursorLine(edit);

            if(line && line->lineText && line->lineText[edit->cursor.characterIndex] == L',')
            {
                uiEditMoveCursor(edit, UIECM_RIGHT);
            }
            uiEditMoveCursor(edit, UIECM_RIGHT);
        }
        else
        {
            uiEditDefaultKeyboardHandler(edit, input);
        }
    }
    else
    {
        uiEditDefaultKeyboardHandler(edit, input);
    }

    uiEditCommaNumericUpdate(edit);
}

typedef struct{
    TTTextForEachGlyphParam forEachParam;
    float* characterWidths;
    float lastWidthSum;
} UIEditCharacterWidthsParam;

static int uiEditCharacterWidthsHandler(UIEditCharacterWidthsParam* param){
    float characterWidth = param->forEachParam.nextGlyphLeft - param->lastWidthSum;
    eafPush(&param->characterWidths, characterWidth);
    param->lastWidthSum = param->forEachParam.nextGlyphLeft;
    return 1;
}

float* uiEditLineGetCharacterWidths(UIEdit* edit, UIEditLine* line)
{
    int i;
    UIEditCharacterWidthsParam characterWidthsParam;
    characterWidthsParam.forEachParam.handler = (GlyphHandler)uiEditCharacterWidthsHandler;
    characterWidthsParam.characterWidths = NULL;
    characterWidthsParam.lastWidthSum = 0.0;
    ttTextForEachGlyph(edit->font, (TTTextForEachGlyphParam*)&characterWidthsParam, 0, 0, edit->textScale, edit->textScale, line->lineText, estrWideLength(&line->lineText), true);
    for(i = 0; i < eafSize(&characterWidthsParam.characterWidths); i++)
    {
        reverseToScreenScalingFactorf(&characterWidthsParam.characterWidths[i], NULL);
    }
    return characterWidthsParam.characterWidths;
}

static void uiEditMoveCursorToClosestCharacter(UIEdit *edit, float xRel, float yRel)
{
    int clickedLineIndex;
    int clickedCharacterIndex;
    int fontHeight = edit->fontHeight;
    float* characterWidths;
    UIEditLine* line;
    float width;
    float offset;
    float x = 0.0f;

    if(uiEditSingleLineMode(edit))
        clickedLineIndex = 0;
    else
        clickedLineIndex = (yRel + edit->vertScroll)/fontHeight;

    line = eaGet(&edit->lines, clickedLineIndex);
    if(!line || !line->lineText )
        return;

    strDimensionsScaledWide(edit->font, edit->textScale, edit->textScale,
        line->lineText, estrWideLength(&line->lineText), &offset, NULL, NULL);

    strDimensionsScaledWide(edit->font, edit->textScale, edit->textScale,
        line->lineText, edit->cursor.characterIndex, &width, NULL, NULL);

    if(width>edit->boundsText.width)
    {
        x = edit->boundsText.width-width;
    }
    else if(edit->rightAlign)
    {
        x = edit->boundsText.width-offset;
    }

    // Which character did the user click on?
    //    Calculate all character widths.
    characterWidths = uiEditLineGetCharacterWidths(edit, line);
    if(!characterWidths)
    {
        edit->cursor.lineIndex = clickedLineIndex;
        edit->cursor.characterIndex = 0;
        return;
    }

    //    Find the character the user clicked on.
    {
        float x1=0, x2=0;

        for(clickedCharacterIndex = 0; clickedCharacterIndex < eafSize(&characterWidths); clickedCharacterIndex++)
        {
            x += characterWidths[clickedCharacterIndex];
            if(x > xRel)
            {
                // Record the two glyph boundries that the mouse click fell within.
                x1 = x - characterWidths[clickedCharacterIndex];
                x2 = x;
                break;
            }
        }

        // Which boundry are we closer to?
        if(xRel - x1 < x2 - xRel)
        {
            clickedCharacterIndex--;
        }
    }

    edit->cursor.lineIndex = clickedLineIndex;
    edit->cursor.characterIndex = MIN(clickedCharacterIndex+1, (int)estrWideLength(&line->lineText));

    eafDestroy(&characterWidths);
}

void uiEditDefaultMouseHandler(UIEdit* edit)
{
    // Did the user click somewhere in the edit box?
    if(uiMouseUpHit(&edit->boundsText, MS_LEFT))
    {
        float mouseX, mouseY;
        // Which line did the user click on?
        mouseX = gMouseInpCur.x;
        mouseY = gMouseInpCur.y;
        reversePointToScreenScalingFactorf(&mouseX, &mouseY);
        mouseX -= edit->boundsText.x;
        mouseY -= edit->boundsText.y;

        uiEditMoveCursorToClosestCharacter(edit, mouseX, mouseY);
        uiEditShowCursor(edit);
    }
}

void uiEditDefaultInputHandler(UIEdit* edit)
{
    KeyInput* input;

    input = inpGetKeyBuf();
    while(input)
    {
        uiEditDefaultKeyboardHandler(edit, input);
        inpGetNextKey(&input);
    }

    uiEditDefaultMouseHandler(edit);
}

void uiEditCommaNumericInputHandler(UIEdit* edit)
{
    KeyInput* input;

    input = inpGetKeyBuf();
    while(input)
    {
        uiEditCommaNumericEditKeyboardHandler(edit, input);
        inpGetNextKey(&input);
    }

    uiEditDefaultMouseHandler(edit);
    
    // If the cursor is to the right of a comma, move it to the left
    if(edit->cursor.characterIndex > 0)
    {
        UIEditLine* line = uiEditGetCursorLine(edit);
        if(line && line->lineText[edit->cursor.characterIndex-1] == L',')
        {
            uiEditMoveCursor(edit, UIECM_LEFT);
        }
    }
    
}

void uiEditHandleInput(UIEdit* edit)
{
    // Check if the edit control has focus.
    if(uiEditCanReceiveInput(edit))
    {
        // If so, proceed to handle inputs.
        //        If the edit control has a input handler callback, call it.
        if(edit->inputHandler)
            edit->inputHandler(edit);
        //        Otherwise, use the default input handler.
        else
            uiEditDefaultInputHandler(edit);
    }

}

void uiEditProcess(UIEdit* edit)
{
    uiEditHandleInput(edit);

    if(edit->dirty)
    {
        if(!(edit->noClip && uiEditSingleLineMode(edit)))
        {
            uiEditReformat(edit);
        }
        edit->dirty = 0;
    }
    if(edit->cursorMoved)
    {
        uiEditUpdateScrollLocation(edit);
        edit->cursorMoved = 0;
    }

    // --------------------
    // rendering

    // handles IME for the active UIEdit
    uiIME_RenderOnUIEdit( edit );

    // edit
    if(!edit->noClip)
        clipperPushRestrict(&edit->boundsText);

    uiEditDisplay(edit);

    if(!edit->noClip)
        clipperPop();
}


// returns estring
/*ok*/ unsigned short* uiEditGetWideText(UIEdit* edit)
{
    unsigned short* str = NULL;
    int i;
    if(!edit)
        return NULL;
    for(i = 0; i < eaSize(&edit->lines); i++)
    {
        UIEditLine* line = edit->lines[i];
        estrWideConcatEString(&str, line->lineText);
        //if( i != eaSize(&edit->lines)-1)
        if (line->hasNewLine)
            estrWideConcatCharString(&str, newlineCharacter);
    }
    return str;
}

// returns estring
char* uiEditGetUTF8TextBuf(char **res, UIEdit* edit)
{
    unsigned short* wideStr = NULL;
    char* str = NULL;
    int bufferLength;

    if(!edit || !res)
        return NULL;
    wideStr = uiEditGetWideText(edit);
    if(!wideStr)
        return NULL;

    bufferLength = WideToUTF8StrConvert(wideStr, NULL, 0);
    if(!bufferLength) {
        estrWideDestroy(&wideStr);
        return NULL;
    }

    estrReserveCapacity(res, bufferLength);
    estrSetLength(res, bufferLength);
    WideToUTF8StrConvert(wideStr, *res, bufferLength);
    estrWideDestroy(&wideStr);
    return *res;
}

/*ok*/ char* uiEditGetUTF8Text(UIEdit* edit)
{
    char *res = NULL;
    return uiEditGetUTF8TextBuf(&res,edit);
}

char* uiEditGetUTF8TextStatic(UIEdit* edit)
{
    static char *res = NULL;
    return uiEditGetUTF8TextBuf(&res,edit);
}


static void uiEditPasteWideText(UIEdit* edit, unsigned short* textIn)
{
    TTTextLine* wrappedLine;
    unsigned short* text = NULL;

    if(!edit || !textIn)
        return;

    // Remove all invalid characters.
    {
        unsigned int i;
        estrWideConcatCharString(&text, textIn);
        for(i = 0; i < estrWideLength(&text); i++)
        {
            // If the character seems invalid, remove it.

            // ab: gratuitious hack for player name in korean. don't
            // remove characters if it is the profile font.
            if(!ttFontValidateCharacter(edit->font, text[i]) && edit->font != &profileNameFont )
            {
                estrWideRemove(&text, i, 1);
                i--;
            }
            else if(edit->disallowReturn && text[i] == '\n')
            {
                text[i] = ' ';
            }
        }
    }

    ttWrapperSetTextWide(edit->wrapper, text, estrWideLength(&text));
    while(wrappedLine = ttGetLine(edit->wrapper, max(1000, edit->boundsText.width), 0, edit->textScale))
    {
        //UIEditLine* line;
        int intendedInsertCount;
        int actualInsertCount;
        int newlineCharIndex;

        if(0 == wrappedLine->characterCount)
            break;

        // If there is a newline character, the text wrapper should have stopped fitting text at that point.
        // The newline character would be at the end of the string if there is one.
        // Find out if we are inserting a newline character.
        newlineCharIndex = strWideFindCharacter(wrappedLine->wideText, wrappedLine->characterCount, '\n');
        if(newlineCharIndex != -1)
        {
            intendedInsertCount = newlineCharIndex;
        }
        else
            intendedInsertCount = wrappedLine->characterCount;

        actualInsertCount = uiEditLineInsertString(edit, edit->lines[edit->cursor.lineIndex], edit->cursor.characterIndex, wrappedLine->wideText, intendedInsertCount);


        if(!uiEditLineSetNewline(edit, edit->lines[edit->cursor.lineIndex], (newlineCharIndex != -1)))
            break;

        // Update the edit cursor.

        if (newlineCharIndex == -1)
        {
            edit->cursor.characterIndex += actualInsertCount;
        }
        else
        {
            // This is where we add the empty line if we're pasting something with newlines
            uiEditAddEmptyLine(edit, edit->cursor.lineIndex + 1);
            edit->cursor.characterIndex = 0;
            edit->cursor.lineIndex++;
        }

        // Stop pasting into the edit box if we've reached the content limit.
        if(actualInsertCount != intendedInsertCount)
            break;
    }

    // Remove all empty lines.
    {
        int i;
        for(i = eaSize(&edit->lines)-1; i >= 0; i--)
        {
            UIEditLine* line = edit->lines[i];
            assert(line);
            if(!line->lineText && !line->hasNewLine)
            {
                uiEditLineDestroy(line);
                // FIXEAR: Is this safe considering the loop index?
                eaRemove(&edit->lines, i);
            }
        }
    }

    if(!eaSize(&edit->lines))
        uiEditAddEmptyLine(edit, 0);

    uiEditFixLineCursor(edit);

    estrWideDestroy(&text);


    uiEditUpdateScrollLocation(edit);
}

// text = estring
void uiEditSetWideText(UIEdit* edit, unsigned short* text)
{
    if(!edit || !text)
        return;

    uiEditClear(edit);
    uiEditPasteWideText(edit, text);
}

// text = any string
void uiEditSetUTF8Text(UIEdit* edit, char* text)
{
    if(text)
    {
        // Convert the string from utf8 to utf16
        unsigned short* wideStr = NULL;
        int bufferLength;

        bufferLength = UTF8ToWideStrConvert(text, NULL, 0);
        estrWideReserveCapacity(&wideStr, bufferLength);
        estrWideSetLength(&wideStr, bufferLength);

        UTF8ToWideStrConvert(text, wideStr, bufferLength);

        // Set the text
        uiEditSetWideText(edit, wideStr);

        // Destroy the temporary utf16 buffer.
        estrWideDestroy(&wideStr);
    }
    else
    {
        uiEditClear(edit);
    }
}

void uiEditSetFont(UIEdit* edit, TTDrawContext* font)
{
    float sc;

    if(!edit || !font)
        return;

    sc = edit->textScale;

    edit->font = font;
    reverseToScreenScalingFactorf(NULL, &sc);
    edit->fontHeight = ttGetFontHeight(font, sc, sc);
    ttWrapperSetFont(edit->wrapper, font);
}

unsigned int uiEditGetFullDrawHeight(UIEdit* edit)
{
    if(!edit)
        return 0;
    assert(edit->font);

    return eaSize(&edit->lines) * edit->fontHeight;
}

int uiEditOnLostFocus(UIEdit* edit, void* requester)
{
    if(edit)
    {
        bool bEditMode = edit->inEditMode;

        edit->inEditMode = 0;

        // close out the IME too
        if( bEditMode )
        {
            uiIME_OnLoseFocus(edit);
        }
    }
    return 1;
}

void uiEditSetZ(UIEdit *edit, float z)
{
    edit->z = z;
}

/**********************************************************************func*
 * NullInputHandler
 *
 */
static void NullInputHandler(UIEdit* edit)
{
    /* Does nothing */
}

/**********************************************************************func*
 * uiEditCreateSimple
 *
 */
UIEdit * uiEditCreateSimple(char *pch, int size, TTDrawContext *font, int color, UIEditInputHandler handler)
{
    UIEdit *edit = uiEditCreate();

    if(font)
        uiEditSetFont(edit, font);
    else
        uiEditSetFont(edit, &game_12);

    if(color)
        edit->textColor = color;
    else
        edit->textColor = CLR_WHITE;

    if(handler)
        edit->inputHandler = handler;
    else
        edit->inputHandler = NullInputHandler;

    edit->limitType = UIECL_UTF8;
    edit->limitCount = size;
    edit->canEdit = 1;

    if(pch)
        uiEditSetUTF8Text(edit, pch);

    return edit;
}

/**********************************************************************func*
 * uiEditTakeFocus
 *
 */
bool uiEditTakeFocus(UIEdit *edit)
{
    if ( !edit )
    {
        Errorf("Uninitialized uiEdit box");
        return false;
    }

    if(uiGetFocusEx(edit, TRUE, uiEditOnLostFocus))
    {
        edit->canEdit = 1;
        edit->inEditMode = 1;
        uiIME_OnFocus(edit);
        return true;
    }
    return false;
}

/**********************************************************************func*
 * uiEditReturnFocus
 *
 */
void uiEditReturnFocus(UIEdit *edit)
{
    uiReturnFocus(edit);
    uiEditOnLostFocus(edit, NULL);
}

bool uiEditHasFocus( UIEdit *edit )
{
    return edit->inEditMode;
}

/**********************************************************************func*
 * uiEditEditSimple
 *
 */
void uiEditEditSimple(UIEdit *edit, float x, float y, float z, float w, float h, TTDrawContext *font, int type, int password)
{
    extern void GetTextColorForType(int type, int rgba[4]);
    UIBox uibox;
    int rgba[4];

    uiBoxDefine(&uibox, x, y, w, h);
    uiEditSetBounds(edit, uibox);
    uiEditSetZ(edit, z);

    if(font)
        uiEditSetFont(edit, font);

    if(edit->dobox)
    {
        CBox box;
        int color = CLR_BLACK;
        BuildCBox(&box,edit->boundsText.x,edit->boundsText.y,edit->boundsText.width,edit->boundsText.height);
        if( mouseCollision( &box ))
        {
            color = 0x333333ff;
            if( mouseClickHit( &box, MS_LEFT) )
                uiEditTakeFocus(edit);
        }
        drawMenuFrame( R12,edit->boundsText.x-10,edit->boundsText.y-6,z,edit->boundsText.width,edit->boundsText.height, CLR_WHITE, color, 0 );
    }   

    GetTextColorForType(type, rgba);
    edit->textColor = rgba[0];

    edit->password = password ? 1 : 0;

    uiEditProcess(edit);
}

/**********************************************************************func*
 * uiEditSetClicks
 *
 */
void uiEditSetClicks(UIEdit *edit, char *keyClick, char *fullClick)
{
    edit->keyClick = keyClick;
    edit->fullClick = fullClick;
}

/**********************************************************************func*
 * uiEditDisallowReturns
 *
 */
void uiEditDisallowReturns(UIEdit *edit, int disallow)
{
    edit->disallowReturn = disallow ? 1 : 0;
}

/**********************************************************************func*
* editInputHandler
*
*/
void editInputHandler(UIEdit* edit)
{
    KeyInput* input;

    // Handle keyboard input.
    input = inpGetKeyBuf();
    while(input)
    {
        if(input->key!=INP_RETURN && input->key!=INP_NUMPADENTER)
        {
            uiEditDefaultKeyboardHandler(edit, input);
        }
        inpGetNextKey(&input);
    }

    uiEditDefaultMouseHandler(edit);
}


//------------------------------------------------------------
//  just wrapping up a static function.
// ... should use uiEditInsertString instead?
//----------------------------------------------------------
void uiEditInsertUnicodeStr(UIEdit* edit, unsigned short* str, unsigned int charCount)
{
    if( verify( edit && str ))
    {
        U32 i;
        for( i = 0; i < charCount; ++i )
        {
            uiEditInsertUnicodeCharacter(edit, str[i]);
        }
    }
}

//------------------------------------------------------------
//  remove characters
// param iCharacter - the index to remove at
// param dCharCount - how many to remove. if kuiEditAction_ToCursor forward to cursor
//                        kuiEditAction_ToEOL remove all from iCharacter to end
//----------------------------------------------------------
void uiEditRemoveCharacters(UIEdit* edit, int iCharacter, int dCharCount )
{
    if( verify( edit ))
    {
        UIEditLine *curLine = uiEditGetCursorLine(edit);

        if( verify( curLine ))
        {
            int len = estrWideLength( &curLine->lineText );

            // in-bounds the char count
            switch ( dCharCount )
            {
            case kuiEditAction_ToCursor:
                if( verify( edit->cursor.characterIndex >= iCharacter  ))
                {
                    dCharCount = edit->cursor.characterIndex - iCharacter;
                }
                else
                {
                    dCharCount = 0;
                }
                break;
            case kuiEditAction_ToEOL:
                dCharCount = MAX(len - iCharacter,0);
                break;
            default:
                verify(dCharCount >= 0);
                break;
            };

            if( !(iCharacter == len) // special case: caret and end overlap when at end
                && verify( INRANGE0(iCharacter, len) && INRANGE0(iCharacter + dCharCount, len + 1)))
            {
                // update content count before removing
                uiEditUpdateContentCount(edit, false, curLine->lineText + iCharacter, dCharCount );

                // remove the string
                estrWideRemove(&curLine->lineText, iCharacter, dCharCount );

                // fixup the cursor
                edit->cursor.characterIndex -= dCharCount;
                uiEditFixLineCursor(edit);

                // mark dirty
                uiEditLineMarkDirty(edit, curLine);

            }

        }
    }
}

//------------------------------------------------------------
//  turns on underlining for the current line at the passed index
// param iStart : the index to start at, uses cursor if < 0, accepts uiEditAction enums
// param diLen : the length of the underline. accepts uiEditAction enums
//----------------------------------------------------------
void uiEditSetUnderlineStateCurLine(UIEdit *edit, bool active, int iStart, int diLen)
{
    if( verify( edit ))
    {
        UIEditLine *curLine = uiEditGetCursorLine(edit);
        if( verify( curLine ))
        {
            int len = estrWideLength( &curLine->lineText );

            // clear the struct
            uiunderline_Clear(&curLine->underline);

            // fixup the index
            switch ( iStart )
            {
            case kuiEditAction_ToCursor:
            {
                iStart = max(0, edit->cursor.characterIndex - diLen );
            }
            break;
            default:
                if( !verify( INRANGE0( iStart, len + 1 )))
                {
                    diLen = 0;
                }
            };

            if( iStart < 0 )
            {
                iStart = edit->cursor.characterIndex;
            }

            // start and length need to be valid
            if( active
                && verify( INRANGE0( iStart, len + 1 ) // the underline can start with new text, i.e. format all remaining text as underlined
                && (diLen < 0 || INRANGE0( iStart + diLen, len + 1 ))))
            {
                curLine->underline.active = active;
                curLine->underline.iStart = iStart;
                curLine->underline.diLen = diLen;
            }
        }
    }
}

void uiunderline_Clear(UIUnderline *underline)
{
    if( verify( underline ))
    {
        ZeroStruct(underline);
    }
}
