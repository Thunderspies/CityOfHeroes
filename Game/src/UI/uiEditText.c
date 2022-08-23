//
//
// editText.c - a file that deals with text entry and editting
//--------------------------------------------------------------------------
#include <utilitieslib/stdtypes.h>
#include "render/sprite.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiEditText.h"
#include "UI/uiChat.h"
#include "sound/sound.h"
#include "win/input.h"
#include <utilitieslib/utils/StringUtil.h>
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "language/langClientUtil.h"
#include "graphics/ttFontUtil.h"
#include "win/win_init.h"
#include <utilitieslib/components/earray.h>
#include "graphics/ttFont.h"
#include "UI/uiBox.h"
#include "UI/uiClipper.h"
#include "cmdparse/cmdcommon.h"
#include "filter/validate_name.h"
#include <utilitieslib/language/MessageStoreUtil.h>

Clip clip;

EditTextBlock edit_block[ EDIT_TOTAL_BLOCKS ];

void initEditTextBlock( EditTextBlock *e, int single_line, char *storage, int storage_len )
{
    e->sel1 = -1;
    e->single_line  = single_line;
    e->selected     = FALSE;
    e->sel1         = -1;   //where drag started
    e->sel2         = -1;   //curr drag part.
    e->sel_start    = -1;   //lesser of the ends
    e->sel_end      = -1;   //greater of the ends.  This part is non-inclusive. ie if start = 0,
    e->len          = 0;
    e->total        = storage_len;  //MAX_CHARS_TO_ENTER;

     if( storage )
    {
        e->str = storage;
        e->cursor_idx = UTF8GetLength( e->str );
    }
    else
    {
        e->str = malloc( sizeof(char)*storage_len );
        e->str[0] = '\0';
        e->cursor_idx   = 0;
        e->cblink_timer = 0;
    }
}

static void copy_to_clip( int start, int end, char  *buf )
{
    int i;

    clip.len = end - start;

    if( clip.len > MAX_CLIP_CHARS )
        clip.len = MAX_CLIP_CHARS;

    for(i=0; i<clip.len; i++ )
        clip.buf[ i ] = buf[ i + start ];
}

//
//
static void add_key_to_buff( int idx, int *len, int max, char *buf, char add )
{
    // "i" is the real string length.
    int i = *len - 1;

    // Only insert the character if the input buffer can hold at least one more character.
    if( *len < ( max - 1 ) )
    {
        // If the input buffer is not empty, move everything behind the specified byte index
        // down one byte.
        if( *len )
            while( i >= idx )
            {
                buf[ i + 1 ] = buf[ i ];
                i--;
            }

        // Update the real string length to reflect the new character to be added.
        *len = *len + 1;
    }

    // Add in the character at the correct index now.
    buf[ idx ] = add;

    // Make sure not to over flow the buffer.
    i = *len;
    if( i >= ( max -1 ) )
        i = max - 1;

    // NULL terminate the string.
    buf[i]=0;
}

/* Function add_string_to_string()
 *  This function inserts a source string into a destination string.  When the function returns,
 *  curDstLength will be the new string length of the destination string.
 *
 *  The function will not do anything if inserting the source string will cause the destination
 *  string to exceed its maximum buffer size.
 */
static void add_string_to_string(   int     insertCursor,   // In front of which byte do I insert the src string?
                                    char    *dst,           // Into which string do I insert the src string?
                                    int     *curDstLength,  // What is the current length of the dst string, including the null terminating character?
                                    int     maxDstLength,   // What is the maximum size the dst string can be, including the null terminating character?
                                    char    *src,           // The string to insert into dst.
                                    int     srcLength       // The length of the string to insert into dst, not including the null terminating character.
                                    )
{
    // If either string do not exist, do nothing.
    if(!dst || !src)
        return;

    // If the insert cursor points past the end of the current destination string, do nothing.
    if(insertCursor >= *curDstLength)
        return;

    // If the destination will not be able to the entire source string, do nothing.
    if(*curDstLength + srcLength > maxDstLength)
        return;

    // Clear out some space to put the source string.
    memmove(&dst[insertCursor] + srcLength, &dst[insertCursor], *curDstLength - insertCursor);

    // Copy the source string into the destination string.
    memcpy(&dst[insertCursor], src, srcLength);

    *curDstLength += srcLength;
    dst[*curDstLength + 1] = '\0';
}

void ebAddCharacter(EditTextBlock* block, char character)
{
    int realInsertIndex;
    int editStringLength;

    // Given the position of the cursor in as character index count into an UTF-8 string,
    // find the equivilent byte index.
    realInsertIndex = UTF8GetCharacter(block->str, block->cursor_idx) - block->str;
    if(realInsertIndex < 0)
        return;

    editStringLength = strlen(block->str) + 1;
    add_key_to_buff(realInsertIndex, &editStringLength, block->total, block->str, character);

    sndPlay( "type", SOUND_PLAYER );

    block->len++;
    block->cursor_idx++;
}

void ebAddWideCharacter(EditTextBlock* block, unsigned short character)
{
    char* utf8String;
    int utf8StrLength;
    int realInsertIndex;
    int editStringLength;

    // Convert the character into UTF-8 format.
    utf8String = WideToUTF8CharConvert(character);
    utf8StrLength = strlen(utf8String);

    // Given the position of the cursor in as character index count into an UTF-8 string,
    // find the equivilent byte index.
     realInsertIndex = UTF8GetCharacter(block->str, block->cursor_idx) - block->str; // isn't the string UTF8? can't we just use the index given?
    // realInsertIndex = block->cursor_idx;

    // Store all bytes into the current input buffer.
    editStringLength = strlen(block->str) + 1;
    add_string_to_string( realInsertIndex, block->str, &editStringLength, block->total, utf8String, utf8StrLength );

    block->len++;
    block->cursor_idx++;
}

//
// Display current string.
//
int display_edit_string( EditTextBlock *e, int xstart, int y, int z,
                         int width, int height, float sc,
                         TTDrawContext *fnt, int type, int password )
{
    int ret, rgba[4];
    char* printedString = e->str;

    // variables for password stuff
    char* password_output = 0;

    GetTextColorForType(type, rgba);

    if ( password & 1) // fill a string with stars
                       // 1 = DISPLAY_AS_PASSWORD
    {
        int i = 0;
        int length = strlen( e->str );
        password_output = malloc( length + 1 );

        for ( i = 0; i < length; i++ )
            password_output[i] = '*';

        password_output[length] = '\0';
        printedString = password_output;
    }

    font( fnt );
    font_color( rgba[1], rgba[0]);

    e->cblink_timer += TIMESTEP;
    if( e->cblink_timer > 30 )
        e->cblink_timer -= 30;

    if ( e->len == 0 && e->selected && e->cblink_timer < 16 )
    {
        unsigned char cursorString[2] = "|";

        // Draw a "|" character a little in front of where the the next character should appear.
        prnt(xstart, y, z+2, sc, sc, cursorString);
        ret = 1;
    }
    else
    {
        ret = drawCursorText( xstart, y, z, sc, width, height, ( char ** )&printedString,
                  &type, (e->selected && e->cblink_timer < 16), e->cursor_idx );
    }

    if( password_output )
        free( password_output );

    return ret;
}

int editting_on;

//
//
int enter_text( EditTextBlock *e, int filter )
{
    unsigned char   *input_str = 0;
    int ret_code;
    KeyInput* input;

    ret_code = 0;
    e->len = UTF8GetLength(e->str);
     input = inpGetKeyBuf();

    while(input)
    {
        // If the input is an edit key...
        e->cblink_timer = 0;

        if( KIT_EditKey == input->type )
        {
            switch( input->key )
            {
                case INP_DELETE:
                    {
                        if( e->sel1 != -1 ) //delete the selected block.
                            UTF8RemoveChars(e->str, e->sel_start, e->sel_end + 1);
                        else if ( e->cursor_idx < e->len)
                            UTF8RemoveChars(e->str, e->cursor_idx, e->cursor_idx + 1);
                    }break;
                case INP_BACKSPACE:
                    {
                        if( e->cursor_idx )
                        {
                            e->cursor_idx--;
                            UTF8RemoveChars(e->str, e->cursor_idx, e->cursor_idx + 1);
                        }
                    }break;
                case INP_HOME:
                    {
                        e->cursor_idx = 0;
                    }break;
                case INP_END:
                    {
                        e->cursor_idx = e->len;
                    }break;
                case INP_LEFT:
                    {
                        e->cursor_idx--;
                    }break;
                case INP_RIGHT:
                    {
                        if( e->cursor_idx < UTF8GetLength( e->str ) )
                            e->cursor_idx++;
                    }break;
                case INP_RETURN:
                case INP_NUMPADENTER:
                    {
                        ret_code |= TEXT_RETURN;
                    }break;
                case INP_ESCAPE:
                    {
                        ret_code |= TEXT_ESCAPE;
                    }break;
                case INP_UP:
                    {
                        ret_code |= TEXT_UP;
                    }break;
                case INP_DOWN:
                    {
                        ret_code |= TEXT_DOWN;
                    }break;
            }
        }

        // If the input is some ascii character...
        if( KIT_Ascii == input->type )
        {
            if( inpLevel( INP_CONTROL ) )
            {
                if( input->asciiCharacter == 'c' || input->asciiCharacter == 'C' )
                {
                    if( e->sel1 != -1 )
                        copy_to_clip( e->sel_start, e->sel_end, e->str );
                }
                else if( input->asciiCharacter == 'x' || input->asciiCharacter == 'X' )
                {
                    if( e->sel1 != -1 )
                    {
                        copy_to_clip( e->sel_start, e->sel_end, e->str );

                        if ( e->sel_start < e->cursor_idx )
                            e->cursor_idx = e->sel_start;

                        UTF8RemoveChars(e->str, e->sel_start, e->sel_end + 1);

                        e->sel1 = -1;
                    }
                }
                else if( input->asciiCharacter == 'v' || input->asciiCharacter == 'V' )
                {
                    int i;

                    i = clip.len;
                    while( i ){
                        ebAddCharacter(e, clip.buf[ --i ]);
                    }
                }
            }       //End control keys.
            else
            {
                if( e->cursor_idx < e->total - 1 )  //leave room for 0 @ end.
                 {
                     // It is probably a better idea to make the tab and return key "EditKeys"
                    // in the keyboard input code.
                    // 13 = return, 27 = escape
                    if ( '\t' != input->asciiCharacter && 13 != input->asciiCharacter && 27 != input->asciiCharacter )
                    {

                        if( !filter || (filter && characterFilter( input->asciiCharacter )) )
                            ebAddCharacter(e, input->asciiCharacter);
                    }
                }
            }
        }

        // If the input is some unicode character...
        if( KIT_Unicode == input->type )
        {
            ebAddWideCharacter(e, input->unicodeCharacter);
        }

        inpGetNextKey(&input);
    }//for all keys in buffer.

    return ret_code;
}   //End of entering characters.


//---------------------------------------------------------------------------------------------------------
// Text Displaying / Formatting
//---------------------------------------------------------------------------------------------------------

void setFontColorAndStyle(int style, int styleIsColor)
{
    if(styleIsColor)
    {
        font_color(style, style);
    }
    else
    {
        int rgba[4];

        GetTextColorForType(style, rgba);
        GetTextStyleForType(style, &font_grp->renderParams);

        font_color(rgba[1], rgba[0]);
    }
}

void dumpFormattedTextArray( int xp, int yp, int zp, float xsc, float ysc, int fitHeight, float font_height, Array* textArray,
                            int *paragraph_style, int style_is_color, int center, int drawCursor, int cursorLocation, int align_bot_ht )
{
    float penX = xp, penY = yp;
    int formattedTextCursor;
    int formattedLineCursor;

    int printableLines;
    int linesToPrint;
    int displayStartLineCount;

    // How many lines can we print, total, given the fitHeight?
    printableLines = fitHeight / (font_height * ysc);

    // How many lines are we supposed to print, given the strings to be printed?
    linesToPrint = formattedTextBlockLineCount(textArray);

    // Given the number of bottom lines to skip, starting from which line should the text be displayed?
    displayStartLineCount = linesToPrint - printableLines;

    // see if we need to bottom-align the text
    if(align_bot_ht)
    {
        int visibleLines = align_bot_ht / (font_height * ysc);
        if(linesToPrint < visibleLines)
        {    
             penY += align_bot_ht - ((visibleLines+1) * font_height * ysc);
              penY += (visibleLines - linesToPrint) * (font_height * ysc);
        }
    }

    // Find out which line of which FormattedText we're supposed to start printing.
    formattedLineCursor = 0;
    formattedTextCursor = 0;
    {
        int startLineCandidate1 = 0;
        int startLineCandidate2 = 0;
        int cursorLocationCopy = cursorLocation;
        int formattedTextCursorCopy;

        if(displayStartLineCount > 0){
            int displayStartLine = displayStartLineCount;
            for(; formattedTextCursor < textArray->size; formattedTextCursor++){
                FormattedText* text;
                text = textArray->storage[formattedTextCursor];

                startLineCandidate1 += text->lines.size;
                displayStartLine -= text->lines.size;
                if(displayStartLine < 0){
                    startLineCandidate1 += displayStartLine;
                    if(startLineCandidate1 < 0)
                        startLineCandidate1 = 0;
                    break;
                }
            }
        }

        formattedTextCursorCopy = formattedTextCursor;

        for(formattedTextCursor = 0; formattedTextCursor < textArray->size; formattedTextCursor++)
        {
            FormattedText* text = textArray->storage[formattedTextCursor];
            for(; startLineCandidate2 < text->lines.size; startLineCandidate2++)
            {
                FormattedLine* line = text->lines.storage[startLineCandidate2];
                if( !line )
                    break;
                cursorLocation -= line->characterCount;
                if(cursorLocation < 0)
                {
                    cursorLocation += line->characterCount;
                    break;
                }
            }
        }

        if( startLineCandidate1 < startLineCandidate2 ) //|| !drawCursor )
        {
            formattedLineCursor = startLineCandidate1;
            cursorLocation = cursorLocationCopy;

            for(formattedTextCursor = 0; formattedTextCursor < textArray->size; formattedTextCursor++)
            {
                FormattedText* text = textArray->storage[formattedTextCursor];
                for( startLineCandidate2 = 0; startLineCandidate2 < startLineCandidate1; startLineCandidate2++)
                {
                    FormattedLine* line = text->lines.storage[startLineCandidate2];
                    cursorLocation -= line->characterCount;
                    if(cursorLocation < 0)
                    {
                        cursorLocation += line->characterCount;
                        break;
                    }
                }
            }
            formattedLineCursor = startLineCandidate1;
            formattedTextCursor = formattedTextCursorCopy;
        }
        else
        {
            formattedLineCursor = startLineCandidate2;
            formattedTextCursor = 0;
            //cursorLocation = cursorLocationCopy;
        }
    }

    {
        int firstIteration = 1;
        int linesPrinted = 0;
        int cursorX = 0;
        int cursorDrawn = FALSE;
        int rgba[4];
        int w, h;

        windowClientSizeThisFrame( &w, &h );

        // Try to print all lines of all formated text while staying within the limit of the number of lines that
        // can be printed.
        for(; formattedTextCursor < textArray->size && linesPrinted < printableLines; formattedTextCursor++)
        {
            FormattedText* text;
            text = textArray->storage[formattedTextCursor];

            if(firstIteration)
                firstIteration = 0;
            else
                formattedLineCursor = 0;

            for(; formattedLineCursor < text->lines.size && linesPrinted < printableLines; formattedLineCursor++)
            {
                FormattedLine* line = text->lines.storage[formattedLineCursor];
                TTFontRenderParams oldparams = font_grp->renderParams;

                if(!line)
                    continue;
                {
                    if(paragraph_style)
                    {
                        setFontColorAndStyle(paragraph_style[formattedTextCursor], style_is_color);
                    }
                    else
                    {
                        font_color(CLR_WHITE, CLR_WHITE);
                    }

                    cursorLocation -= line->characterCount;
                    determineTextColor(rgba);

                    if ( cursorLocation <= 0 && drawCursor && cursorDrawn == FALSE )
                    {
                        unsigned short cursorString[2] = L"|";
                        int cursorY = 0;

                        // if cursorLocation is 0, we are at the end of a line, check to see if it the last character
                        // was an enter
                        if( cursorLocation == 0 && drawCursor && line->text[line->characterCount - 1] == 10 )
                        {
                            cursorY = (F32)font_height * ysc;
                            //applyToScreenScalingFactori(&absolutePenX, &absolutePenX);
                            printBasicWide(font_grp, penX, penY + cursorY, zp+2, xsc, ysc, 0, cursorString, 1, rgba);

                        }
                        else
                        {
                            int absolutePenX = penX;
                            int absolutePenY = penY;

                            // jump to where the cursor is
                            cursorLocation += line->characterCount;

                            // Figure out the relative position of the character following the cursor. (stored as cursorX)
                            //    Note that cursorX is the absolute pixel width of the string in front of the cursor.
                            //    For whatever reason, all text drawing functions accept "unscaled" coordinates in the shell.
                            ttGetStringDimensionsWithScaling( font_grp, xsc, ysc, line->text, cursorLocation, NULL, NULL, &cursorX, true);

                            applyToScreenScalingFactori(&absolutePenX, &absolutePenY);

                            // Draw a "|" character a little in front of where the the next character should appear.
                            printBasicWideAbsolute(font_grp, absolutePenX + cursorX, absolutePenY, zp+2, xsc, ysc, 0, cursorString, 1, rgba);
                        }

                        cursorDrawn = TRUE;
                    }

                    {
                        Clipper2D* clipper = clipperGetCurrent();
                          UIBox textBox = {penX, penY-(font_height*ysc), 10000, (font_height*ysc)};
                        if(!clipper || shell_menu() || uiBoxIntersects(&textBox, clipperGetBox(clipper)))
                        {
                            printBasicWide(font_grp, penX + line->xOffset, penY, zp, xsc, ysc, center, line->text, line->characterCount, rgba);
                        }
                    }
                }

                penY += ( F32 )font_height * ysc;
                // This is almost always passing a negative value as the textLength parameter, WTH?
                ttGetStringDimensionsWithScaling( font_grp, xsc, ysc, line->text, cursorLocation + line->characterCount, NULL, NULL, &cursorX, true);
                linesPrinted++;

                font_grp->renderParams = oldparams;
            } // end for

            if ( drawCursor && !cursorDrawn ) // draw cursor even when there is no text
            {
                TTFontRenderParams oldparams = font_grp->renderParams;
                unsigned short cursorString[2] = L"|";

                if(paragraph_style)
                {
                    setFontColorAndStyle(paragraph_style[formattedTextCursor], style_is_color);
                }
                else
                {
                    font_color(CLR_WHITE, CLR_WHITE);
                }
                determineTextColor(rgba);

                //penY -= ( F32 )font_height * ysc;
                applyToScreenScalingFactorf(&penX, &penY);
                printBasicWideAbsolute(font_grp, penX + cursorX, penY, zp+2, xsc, ysc, 0, cursorString, 1, rgba);

                font_grp->renderParams = oldparams;
            }
        }
    }
}

//
//
void drawTextStd( int x, int y, int z, float scale, int height, float line_ht, Array* text, int*color, int align_bot_ht )
{
    dumpFormattedTextArray(x, y, z, scale, scale, height, line_ht, text, color, false, 0, 0, 0, align_bot_ht);
}

//
//
int drawCursorText( int xp, int yp, int zp, float sc, int width, int height,
                   char **text, int *line_style, int drawCursor, int cursorLocation )
{
    int retVal;
    static Array* formattedTextArray;
    if(!formattedTextArray)
        formattedTextArray = createArray();

    formatTextBlocks(formattedTextArray, font_grp, sc, sc, width, NULL, text, 1, 0);
    retVal = formattedTextBlockLineCount(formattedTextArray);

    dumpFormattedTextArray(xp, yp, zp, sc, sc, height, FONT_HEIGHT, formattedTextArray, line_style, false, 0, drawCursor, cursorLocation, 0);
    clearArrayEx(formattedTextArray, psudeoDestroyFormattedText);

    return retVal;
}

// go through all the effort needed to wrap text (LAZY HACK FUNCTION - avoid using)
//
void wrappedText( int x, int y, int z, int wd, char* txt, int type )
{
    static Array* text = NULL;
    char ** helptext;
    int *color, w, h;
    float sc = 1.f;
    float ratio = 1.f;
    unsigned int uiNeededSize;

    static char *s_pchBuff = NULL;
    static unsigned int s_uiSize = 0;

    if( !txt )
        return;

    if( shell_menu() ) // voodoo hackery
    {
        windowClientSizeThisFrame( &w, &h );
        ratio = (float)(w*.75)/(float)h; 
    }

    uiNeededSize = msPrintf( menuMessages, NULL, 0, txt ) + 1;
    if(uiNeededSize>s_uiSize)
    {
        s_uiSize = max(uiNeededSize*2, 512);
        s_pchBuff = realloc(s_pchBuff, s_uiSize);
    }
    msPrintf( menuMessages, s_pchBuff, s_uiSize, txt );

    eaCreate( &helptext );
    eaiCreate( &color );

    eaPush( &helptext, s_pchBuff );
    eaiPush( &color, type );

    if(!text)
        text = createArray();
    clearArrayEx(text, psudeoDestroyFormattedText);

    if(type!=INFO_PROFILE_TEXT)
        font(&game_12);
    formatTextBlocks( text, font_grp, 1.0, 1.0, wd/sc, color, helptext, 1, 0);

   drawTextStd( x, y, z, 1.f, 1000, MIN(15,15*ratio), text, color, 0 );

    eaDestroy( &helptext );
    eaiDestroy( &color );
}

int editTextOnLostFocus(EditTextBlock* block, void* requester)
{
    block->selected = 0;
    return 1;
}
