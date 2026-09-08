#include <ctype.h>
#include <utilitieslib/assert/assert.h>

#include "graphics/ttFontUtil.h"
#include "graphics/ttFont.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/StringUtil.h>

#include "render/sprite.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiChat.h" // for GetTextStyleForType
#include "UI/uiClipper.h"
#include <utilitieslib/components/EString.h>
#include "UI/sprite/sprite_text.h" // for DetermineColor()

#include <utilitieslib/utils/timing.h>

MemoryPool FormattedLineMemoryPool;
MemoryPool FormattedTextMemoryPool;

FormattedLine* createFormattedLine(){
    if(!FormattedLineMemoryPool){
        FormattedLineMemoryPool = createMemoryPool();
        initMemoryPool(FormattedLineMemoryPool, sizeof(FormattedLine), 512);
        mpSetMode(FormattedLineMemoryPool, ZeroMemoryBit);
    }
    return mpAlloc(FormattedLineMemoryPool);
}

void destroyFormattedLine(FormattedLine* line){
    mpFree(FormattedLineMemoryPool, line);
}

FormattedText* createFormattedText(){
    // We're not expecting to have that many pieces of formatted text active at the same time.
    if(!FormattedTextMemoryPool){
        FormattedTextMemoryPool = createMemoryPool();
        initMemoryPool(FormattedTextMemoryPool, sizeof(FormattedText), 16);
        mpSetMode(FormattedTextMemoryPool, ZeroMemoryBit);
    }
    return mpAlloc(FormattedTextMemoryPool);
}

// Clears the formatted texts and "destroys" it in the sense that it is returned to the MemoryPool.
//
void psudeoDestroyFormattedText(FormattedText* text){
    // This optimization does not work because ZeroMemoryBit is enabled above
    //clearArrayEx(&text->lines, destroyFormattedLine);
    destroyArrayPartialEx(&text->lines, destroyFormattedLine);
    mpFree(FormattedTextMemoryPool, text);
}

void destroyFormattedText(FormattedText* text){
    destroyArrayPartialEx(&text->lines, destroyFormattedLine);
    mpFree(FormattedTextMemoryPool, text);
}


typedef struct{
    TTTextForEachGlyphParam forEachParam;
    Array* characterWidths;
    float lastWidthSum;
} TTCharacterWidthsParam;

int TTCharacterWidthsHandler(TTCharacterWidthsParam* param){
    float characterWidth = param->forEachParam.nextGlyphLeft - param->lastWidthSum;
    arrayPushBack(param->characterWidths, *((void**)&characterWidth));
    param->lastWidthSum = param->forEachParam.nextGlyphLeft;
    return 1;
}

    //    Process all characters...
    //        Start processing a new line.
    //            Record the first character of this line.
    //            Initialize the whitespace cursor to the first character as well.
    //            Continue the loop while the given fitWidth can accomandate more characters...
    //                If the character is a whitespace character, record it as a potential spot to do word wrapping based.
    //                Record the character as a potential spot to do word wrapping, in case adding this character will cause a
    //                 long string to overflow the fitWidth.
    //                Account for the onscreen size of this glyph.
    //
    //        When the loop stops, we have overflown fitWidth and has filled the entire line.
    //        Record how long this line should be.
    //            If the whitespace character is the same as the first character still, that means there wasn't a whitespace
    //            in the string.  Break the string at the last seen character.
    //            Otherwise, the last seen character should be at the last whitespace.
    //
    //        Remove trailing whitespaces from the line so it doesn't take up any space.

/* Function formatText()
 *    This function takes a long UTF8 string and breaks it up into an array of FormattedText
 *    structures.
 *
 */
//FormattedText* formatTextSimple(TTDrawContext* font, float xScale, float yScale, int fitWidth, char* str){
//    return formatText(font, xScale, yScale, &fitWidth, 1, str);
//}
/*
FormattedText* formatTextSimpleEx(TTDrawContext* font, float xScale, float yScale, int* fitWidths, int fitWidthCount, char* str){

}
*/

FormattedText* formatText(FormattedText* text, TTDrawContext* font, float xScale, float yScale, int fitWidth, int style, char* str, int indent){
    return formatTextEx(text, font, xScale, yScale, &fitWidth, 1, style, str, indent);
}

/*************************************************************************************************************************
 * WidthCounter
 *    Originally part of the formatTextEx() algorithm.  This structure and associated functions are used to attempt
 *    to fit text into a given width.
 */
typedef struct{
    // The text to be fitted.
    unsigned short* text;
    unsigned int characterCount;

    // The widths of the characters in the given text.
    Array* characterWidths;

    // The width to fit the text into.
    unsigned int fitWidth;

    // Interal variables.
    unsigned short* lastWhitespace;
    unsigned int lastWhiteSpaceWidth;
    unsigned short* forcedWordWrapSpot;
    float accumulatedWidth;
    float lastAccumulatedWidth;
    unsigned short* lastLineCharacter;
    unsigned int characterCursor;
    unsigned short* initialText;
} WidthCounter;

void wcInit(WidthCounter* counter, unsigned short* text, unsigned int characterCount, unsigned int fitWidth, Array* characterWidths, unsigned int initCharacterCursor){
    memset(counter, 0, sizeof(WidthCounter));
    counter->text = counter->forcedWordWrapSpot = text;
    counter->characterCount = characterCount;
    counter->characterWidths = characterWidths;
    counter->characterCursor = initCharacterCursor;
    counter->fitWidth = fitWidth;
    counter->initialText = &text[initCharacterCursor];
    counter->lastWhitespace = counter->initialText;
}

int wcRequiresForcedWordWrap(WidthCounter* counter){
    return (counter->fitWidth <= counter->accumulatedWidth) && (counter->lastWhitespace == counter->initialText);
}

int wcTextFitsWidth(WidthCounter* counter){
    return (counter->fitWidth > counter->accumulatedWidth);
}

void wcRecordWrapLocation(WidthCounter* counter)
{
    // Store the location of the last seen whitespace as a possible spot to perform word wrapping.
    if(counter->text[counter->characterCursor] == ' ')
    {
        counter->lastWhitespace = &counter->text[counter->characterCursor];
        counter->lastWhiteSpaceWidth = counter->lastAccumulatedWidth;
    }

    // Every single character is a potential spot for forced word wrapping.
    counter->forcedWordWrapSpot = &counter->text[counter->characterCursor];
}

void wcFit(WidthCounter* counter){

    // Record the wrap spot in case we never enter the following loop for some reason.
    wcRecordWrapLocation(counter);

    // Keep adding to the accumulated width until either:
    //    1) it no longer fits within the fit width, or
    //    2) we have exhausted the entire text body.
    for(;counter->fitWidth > counter->accumulatedWidth && counter->characterCursor < (int)counter->characterCount; counter->characterCursor++){
        wcRecordWrapLocation(counter);

        // Ignore all control characters.
        if(counter->text[counter->characterCursor] == '\n')
        {
            counter->characterCursor++;
            break;
        }
        if(counter->text[counter->characterCursor] < 32)
            continue;

        // Store the last accumulated width.
        //    When the accumulated witdth finally overruns the fitWidth, the last accumulated width will have the last acc. width which fits within
        //    the fit width.
        counter->lastAccumulatedWidth = counter->accumulatedWidth;

        // Caculate the new accumulated width given the current character.
        counter->accumulatedWidth += *((float*)&counter->characterWidths->storage[counter->characterCursor]);
    }

    if(wcTextFitsWidth(counter))
        counter->lastAccumulatedWidth = counter->accumulatedWidth;
}
/*
 * WidthCounter
 *************************************************************************************************************************/
#define INDENT_SIZE  20

static int TTCharacterWidthsHandlerGlyphCallback(TTTextForEachGlyphParam* param)
{
    return TTCharacterWidthsHandler((TTCharacterWidthsParam*)param);
}

FormattedText* formatTextEx(FormattedText* text, TTDrawContext* font, float xScale, float yScale, int* fitWidths, int fitWidthCount, int style, char* str, int indent)
{
    TTCharacterWidthsParam characterWidthsParam;
    static Array* characterWidths = NULL;
    //FormattedText* text = createFormattedText();
    int i;
    int curFitWidth;
     int fitWidthCursor = 0;
    int finalAccumulatedWidth = 0;
    TTFontRenderParams oldParams = font->renderParams;

    // Do not do anything if valid fit widths are given.
    if(fitWidthCount <= 0 || !fitWidths)
        return text;
    if(fitWidthCount == 1 && *fitWidths == 0)
        return text;
    if(!text)
        text = createFormattedText();

    if(!characterWidths)
        characterWidths = createArray();
    clearArray(characterWidths);

    // Convert the given string into UTF-16 format and store it with the formatted text.
    {
        unsigned int characterCount;
        characterCount = UTF8ToWideStrConvert(str, NULL, 0);
        text->characterCount = characterCount;
        if(characterCount + 1 > text->bufferSize){
            text->bufferSize = characterCount + 1;
            text->sharedTextBuffer = realloc(text->sharedTextBuffer, (text->bufferSize) * sizeof(unsigned short));
        }
        UTF8ToWideStrConvert(str, text->sharedTextBuffer, text->bufferSize);
    }

    GetTextStyleForType(style, &font->renderParams);

    // Calculate all character widths.
    characterWidthsParam.forEachParam.handler = TTCharacterWidthsHandlerGlyphCallback;
    characterWidthsParam.characterWidths = characterWidths;
    characterWidthsParam.lastWidthSum = 0.0;
    ttTextForEachGlyph(font, (TTTextForEachGlyphParam*)&characterWidthsParam, 0, 0, xScale, yScale, text->sharedTextBuffer, text->characterCount, true);
    if(characterWidths->size == 0)
        return text;

    //    Process all characters, make lines
    for(i = 0; i < (int)text->characterCount;)
    {
        // Start processing a new line.
        FormattedLine* line;
        float accumulatedWidth = 0;
        unsigned short* lastLineCharacter;
        WidthCounter counter;

        line = createFormattedLine();

         curFitWidth = fitWidths[fitWidthCursor];
        if( i != 0 && indent )
        {
            curFitWidth -= INDENT_SIZE*xScale;
            line->xOffset = INDENT_SIZE*xScale;
        }
        else
            line->xOffset = 0;

        applyToScreenScalingFactori(&curFitWidth, NULL);
        fitWidthCursor++;
        if(fitWidthCursor >= fitWidthCount)
            fitWidthCursor = fitWidthCount - 1;

        line->text = &text->sharedTextBuffer[i];

        // Start a new width counter to calculate how to fit the given text into the given width.
        wcInit(&counter, text->sharedTextBuffer, text->characterCount, curFitWidth, characterWidths, i);
        wcFit(&counter);

        // If a forced word wrap is required, check if it's possible to avoid this by going directly to the
        // next line.
         if((fitWidthCursor < fitWidthCount) && wcRequiresForcedWordWrap(&counter)){
            int nextWidth;
            WidthCounter nextWidthCounter;

            nextWidth = fitWidths[fitWidthCursor];
            if( indent )
                nextWidth -= INDENT_SIZE*xScale;
            applyToScreenScalingFactori(&nextWidth, NULL);
            wcInit(&nextWidthCounter, text->sharedTextBuffer, text->characterCount, nextWidth, characterWidths, i);
            wcFit(&nextWidthCounter);

            // If the text will fit into the next line, the place the text there instead.
            if(wcTextFitsWidth(&nextWidthCounter)){
                // Add an empty line to the text.
                // FIXME!!!
                //    It might be better to add a line with 0 characters.
                //    It's a tiny bit easier to deal with when reading them back.
                arrayPushBack(&text->lines, NULL);

                // Move the fit width cursor since we used up another one.
                fitWidthCursor++;
                if(fitWidthCursor >= fitWidthCount)
                    fitWidthCursor = fitWidthCount - 1;

                memcpy(&counter, &nextWidthCounter, sizeof(WidthCounter));
            }
        }

         //    Record how long this line should be.
        //        Two possible actions:
        //            force word wrap at the last seen character.
        //                If we're looking at a long string without any whitespaces that overflows the fitWidth.
        //                    In this case, the lastWhitespace pointer will have been unaltered by the for-loop.
        //                if we're looking at the last character in the string and the entire string fits.
        //                    In this case, the entire string fits.  Wrapping at the last character is the same as
        //                    ending the line at the last character.
        //            word wrap at the last seen position.
        //
        //        If the whitespace character is the same as the first character still, that means there wasn't a whitespace
        //        in the string.  Break the string at the last seen character.
        if(wcRequiresForcedWordWrap(&counter)){
            lastLineCharacter = counter.forcedWordWrapSpot - 1;
            i = lastLineCharacter - text->sharedTextBuffer;
            i++;
        }
        else if(wcTextFitsWidth(&counter))
        {
            lastLineCharacter = counter.forcedWordWrapSpot;
            i = lastLineCharacter - text->sharedTextBuffer;
            i++;
        }
        else
        {
            // Otherwise, the last seen character should be at the last whitespace.
            lastLineCharacter = counter.lastWhitespace;
            counter.lastAccumulatedWidth = counter.lastWhiteSpaceWidth;
            i = lastLineCharacter - text->sharedTextBuffer;

            if(text->sharedTextBuffer[i] == ' ')
                i++;
        }


         line->characterCount = lastLineCharacter - line->text + 1;
         line->lineWidthRemainder = curFitWidth - counter.lastAccumulatedWidth;
        line->lineWidth = counter.lastAccumulatedWidth;
        arrayPushBack(&text->lines, line);

        // Sanity check.
        //    If all of the fitwidths has been exhausted and we are unable to fit any characters
        //    into the last fitwidth, do not continue into an infinite loop, trying to fit characters
        //    into finite number of lines that can't have their widths fitted.
        if(text->lines.size >= fitWidthCount && line->characterCount == 0)
            break;
    }
    if(text->sharedTextBuffer[text->characterCount-1] == '\n'){
        FormattedLine* line;
        line = createFormattedLine();
        line->characterCount = 0;
        line->lineWidthRemainder = curFitWidth;    // programmer laziness bug.  This is supposed to be next fit width.
        line->text = L"";
        arrayPushBack(&text->lines, line);
    }
    //destroyArray(characterWidths);

    font->renderParams = oldParams;
     return text;
}
/*
Array* formatTextBlocksSimple(TTDrawContext* font, float xScale, float yScale, int fitWidth, char** strings, int stringCount){
    int stringCursor;
    Array* formattedTexts = createArray();

    // Walk through all strings that were passed in.
    for(stringCursor = 0; stringCursor < stringCount; stringCursor++){
        FormattedText* text;

        text = formatText(font, xScale, yScale, fitWidth, strings[stringCursor]);
        arrayPushBack(formattedTexts, text);
    }

    return formattedTexts;
}
*/
Array* formatTextBlocks(Array* formattedTexts, TTDrawContext* font, float xScale, float yScale, int fitWidth, int* style, char** strings, int stringCount, int indent){
    int stringCursor;
    if(!formattedTexts)
        formattedTexts = createArray();

    PERFINFO_AUTO_START("formatTextBlocks", 1);

    // Walk through all strings that were passed in.
     for(stringCursor = 0; stringCursor < stringCount; stringCursor++){
        FormattedText* text;

         if (stringCursor < formattedTexts->size) {
            text = formattedTexts->storage[stringCursor];
            if (text->xScale != xScale ||
                text->yScale != yScale ||
                text->fitWidth != fitWidth ||
                text->fontSize != font->renderParams.renderSize)
            {
                psudeoDestroyFormattedText(text);
                text = createFormattedText();
                arraySetAt(formattedTexts, stringCursor, text);
            } else {
                continue;
            }
        } else {
             text = createFormattedText();
             arrayPushBack(formattedTexts, text);
        }

        formatText(text, font, xScale, yScale, fitWidth, (style ? style[stringCursor]: 0), strings[stringCursor], indent);
        // Save cached info
        text->xScale = xScale;
        text->yScale = yScale;
        text->fitWidth = fitWidth;
        text->fontSize = font->renderParams.renderSize;
    }

    PERFINFO_AUTO_STOP();

    return formattedTexts;
}

int formattedTextBlockLineCount(Array* formattedTextBlock){
    int lineCount = 0;
    int formattedTextCursor;

    for(formattedTextCursor = 0; formattedTextCursor < formattedTextBlock->size; formattedTextCursor++){
        FormattedText* text;
        text = formattedTextBlock->storage[formattedTextCursor];

        lineCount += text->lines.size;
    }

    return lineCount;
}

/**********************************************************************************************
 * String drawing utilities
 *
 */

void printBasicWide(TTDrawContext*    font, float x, float y, float z, float xScale, float yScale, int center, unsigned short *str, int charsToPrint, int rgba[4]){
    applyToScreenScalingFactorf(&x, &y);
    printBasicWideAbsolute(font, x, y, z, xScale, yScale, center, str, charsToPrint, rgba);
};

/* Function printBasicWiteAbsolute()
 *    Assumes that the specified x, y is in absolute cooridinates.
 *
 */
#include "win/win_init.h"
void printBasicWideAbsolute(TTDrawContext*    font, float x, float y, float z, float xScale, float yScale, int center, unsigned short *str, int charsToPrint, int rgba[4]){
    //TTBufferredText*    bufferredText;

    if(!str)
        return;

    if(center){
        int stringWidth;
        int stringHeight;

        ttGetStringDimensionsWithScaling(font, xScale, yScale, str, charsToPrint, (center & CENTER_X)?&stringWidth:NULL, (center & CENTER_Y)?&stringHeight:NULL, NULL, true);
        if(center & CENTER_X)
            x -= (float)stringWidth/2;

        if(center & CENTER_Y)
            y += (float)stringHeight/2; // - (float)stringHeight/10;
//            y += (float)stringHeight/2 - (float)stringHeight/10;
    }

    ttDrawText2DWithScalingSprite(font, x, y, z, xScale, yScale, rgba,  str,  charsToPrint);
}

/* Function prntBasic
 *    This function prints the specified number of characters of the given string at the given
 *    coordinates using the given scale.
 *
 *    Note that the scale value should be specified relative to a 640x480 screen.  Screen/resolution
 *    resizes will be automatically taken into consideration.
 */

void printBasic(TTDrawContext*    font, float x, float y, float z, float xScale, float yScale, int center, const char *str, int charsToPrint, int rgba[4])
{
    unsigned short *wstr;

    if(!str)
        return;

    wstr = UTF8ToWideStrConvertStatic(str);

    printBasicWide(font, x, y, z, xScale, yScale, center, wstr, charsToPrint, rgba);
}
/*
 * String drawing utilities
 **********************************************************************************************/



/**********************************************************************************************
 * String Dimensions utilties
 */
void strDimensionsBasicWide(TTDrawContext* font, float xScale, float yScale, unsigned short* str, int charsToPrint, int* stringWidth, int* stringHeight, int* nextGlyphLeft){
    ttGetStringDimensionsWithScaling(font, xScale, yScale, str, charsToPrint, stringWidth, stringHeight, nextGlyphLeft, true);
}

void strDimensionsBasic(TTDrawContext* font, float xScale, float yScale, const char* str, int charsToPrint, int* stringWidth, int* stringHeight, int* nextGlyphLeft)
{
    unsigned short *wstr = UTF8ToWideStrConvertStatic(str);

    strDimensionsBasicWide(font, xScale, yScale, wstr, charsToPrint, stringWidth, stringHeight, nextGlyphLeft);
}

void strDimensionsScaledWide(TTDrawContext* font, float xScale, float yScale, unsigned short* str, int charsToPrint, float* stringWidth, float* stringHeight, float* nextGlyphLeft)
{
    int iWidth, iHeight, iGlyphLeft;
    if (charsToPrint==0 || !str) {
        if (stringWidth)
            *stringWidth = 0;
        if (stringHeight)
            *stringHeight = 0;
        if (nextGlyphLeft)
            *nextGlyphLeft = 0;
        return;
    }
    ttGetStringDimensionsWithScaling(font, xScale, yScale, str, charsToPrint, stringWidth?&iWidth:NULL, stringHeight?&iHeight:NULL, nextGlyphLeft?&iGlyphLeft:NULL, true);
    if(stringWidth)
    {
        *stringWidth = iWidth;
        reverseToScreenScalingFactorf(stringWidth, NULL);
    }
    if(stringHeight)
    {
        *stringHeight = iHeight;
        reverseToScreenScalingFactorf(NULL, stringHeight);
    }
    if(nextGlyphLeft)
    {
        *nextGlyphLeft = iGlyphLeft;
        reverseToScreenScalingFactorf(nextGlyphLeft, NULL);
    }
}
/*
 * String Dimensions utilties
 **********************************************************************************************/
//---------------------------------------------------------------------------------------------
// TTTextWrapper
//---------------------------------------------------------------------------------------------
struct TTTextWrapper
{
    unsigned short* wideText;    // The text to be wrapped.
    int textLength;                // How much text should be processed?

    unsigned int processCursor;    // Where did we leave off in the wideText last time?

    WidthCounter wc;
    TTTextLine* line;
    TTDrawContext* font;
    Array* characterWidths;

    unsigned int ownsWideText        : 1;    // Deso the wrapper own the wide text string?
};

TTTextWrapper* ttTextWrapperCreate(void)
{
    TTTextWrapper* obj = calloc(1, sizeof(TTTextWrapper));
    obj->line = ttTextLineCreate();
    obj->characterWidths = createArray();
    return obj;
}

void ttTextWrapperDestroy(TTTextWrapper* obj)
{
    if(!obj)
        return;
    if(obj->line)
        ttTextLineDestroy(obj->line);
    if(obj->characterWidths)
        destroyArray(obj->characterWidths);
    if(obj->wideText && obj->ownsWideText)
        estrDestroy((char**)&obj->wideText);
}

TTTextLine* ttTextLineCreate(void)
{
    return calloc(1, sizeof(TTTextLine));
}

void ttTextLineDestroy(TTTextLine* obj)
{
    if(!obj)
        return;
    free(obj);
}

TTTextLine* ttTextLineCone(TTTextLine* obj)
{
    // IMPLEMENTME!!!
    return NULL;
}

void ttWrapperSetTextWide(TTTextWrapper* obj, unsigned short* text, unsigned int textLength)
{
    obj->wideText = text;
    obj->textLength = textLength;
    obj->processCursor = 0;
    clearArray(obj->characterWidths);
}

void ttWrapperSetFont(TTTextWrapper* obj, TTDrawContext* font)
{
    obj->font = font;
    obj->line->lineHeight = ttGetFontHeight(font, 1.0, 1.0);
}

TTTextLine* ttGetLine(TTTextWrapper* obj, unsigned int curLineWidth, unsigned int nextLineWidth, float sc)
{
    int characterCount;
    if(!obj || (int)obj->processCursor >= obj->textLength)
        return NULL;

    characterCount = obj->textLength;
    if(characterCount == 0)
    {
        TTTextLine* line = obj->line;
        line->wideText = &obj->wideText[obj->processCursor];
        line->characterCount = 0;
        line->fitWidth = curLineWidth;
        line->font = obj->font;
        line->lineWidth = 0;
        line->remainderWidth = curLineWidth;
        return line;
    }

    // Calculate all character widths if it hasn't been done yet.
    if(obj->characterWidths->size == 0)
    {
        TTCharacterWidthsParam characterWidthsParam;
        characterWidthsParam.forEachParam.handler = TTCharacterWidthsHandlerGlyphCallback;
        characterWidthsParam.characterWidths = obj->characterWidths;
        characterWidthsParam.lastWidthSum = 0.0;
        ttTextForEachGlyph(obj->font, (TTTextForEachGlyphParam*)&characterWidthsParam, 0, 0, sc, sc, obj->wideText, characterCount, true);
    }
    
    {
        // Start processing a new line.
        TTTextLine* line = obj->line;
        float accumulatedWidth = 0;
        unsigned short* lastLineCharacter;
        WidthCounter counter;
        unsigned int curFitWidth;

        curFitWidth = curLineWidth;
        applyToScreenScalingFactori(&curFitWidth, NULL);

        line->wideText = &obj->wideText[obj->processCursor];

        // Sanity check.  Can any characters be fitted at all?
        if(!obj->characterWidths->storage || (*(float*)&obj->characterWidths->storage[obj->processCursor] > curFitWidth))
            return NULL;
        

        // Start a new width counter to calculate how to fit the given text into the given width.
        wcInit(&counter, obj->wideText, characterCount, curFitWidth, obj->characterWidths, obj->processCursor);
        wcFit(&counter);

        // If a forced word wrap is required, check if it's possible to avoid this by going directly to the
        // next line.
        if(nextLineWidth && wcRequiresForcedWordWrap(&counter)){
            int nextWidth;
            WidthCounter nextWidthCounter;

            nextWidth = nextLineWidth;
            applyToScreenScalingFactori(&nextWidth, NULL);
            wcInit(&nextWidthCounter, obj->wideText, characterCount, nextWidth, obj->characterWidths, obj->processCursor);
            wcFit(&nextWidthCounter);

            // If the text will fit into the next line, the place the text there instead.
            if(wcTextFitsWidth(&nextWidthCounter)){
                line->characterCount = 0;
                line->fitWidth = curFitWidth;
                line->font = obj->font;
                line->lineWidth = 0;
                line->remainderWidth = curFitWidth;
                return line;
            }
        }

        //    Record how long this line should be.
        //        Two possible actions:
        //            force word wrap at the last seen character.
        //                If we're looking at a long string without any whitespaces that overflows the fitWidth.
        //                    In this case, the lastWhitespace pointer will have been unaltered by the for-loop.
        //                if we're looking at the last character in the string and the entire string fits.
        //                    In this case, the entire string fits.  Wrapping at the last character is the same as
        //                    ending the line at the last character.
        //            word wrap at the last seen position.
        //
        //        If the whitespace character is the same as the first character still, that means there wasn't a whitespace
        //        in the string.  Break the string at the last seen character.
        if(wcRequiresForcedWordWrap(&counter)){
            lastLineCharacter = counter.forcedWordWrapSpot - 1;
            obj->processCursor = lastLineCharacter - obj->wideText;
            obj->processCursor++;
        }
        else if(wcTextFitsWidth(&counter))
        {
            lastLineCharacter = counter.forcedWordWrapSpot;
            obj->processCursor = lastLineCharacter - obj->wideText;
            obj->processCursor++;
        }
        else
        {
            // Otherwise, the last seen character should be at the last whitespace.
            lastLineCharacter = counter.lastWhitespace;
            counter.lastAccumulatedWidth = counter.lastWhiteSpaceWidth;
            obj->processCursor = lastLineCharacter - obj->wideText;

            while(obj->wideText[obj->processCursor] == ' ')
                obj->processCursor++;
        }

        line->characterCount = lastLineCharacter - line->wideText + 1;
        line->fitWidth = curFitWidth;
        line->font = obj->font;
        line->lineWidth = counter.lastAccumulatedWidth;
        line->remainderWidth = curFitWidth - counter.lastAccumulatedWidth;
    }

    if( obj->line->characterCount )
        return obj->line;
    else
        return 0;
}

//-------------------------------------------------------------------------------------
// Non-insane text wrapping code
//-------------------------------------------------------------------------------------
extern TTDrawContext* font_grp;
FormattedText* stFormatText(const char* text, int width, float scale)
{
    static FormattedText* formattedText;
    if(formattedText)
    {
        psudeoDestroyFormattedText(formattedText);
        formattedText = NULL;
    }
    formattedText = formatTextEx(formattedText, font_grp, scale, scale, &width, 1, 0, (char*)text, 0);
    return formattedText;
}

void stDrawFormattedText(UIBox bounds, float z, FormattedText* text, float scale)
{
    int i;
    int fontHeight = ttGetFontHeight(font_grp, scale, scale);
    int rgba[4];

     determineTextColor(rgba);
    //clipperPushRestrict(&bounds);
    for(i = 0; i < text->lines.size; i++)
    {
        FormattedLine* line = text->lines.storage[i];
        if(line)
        {
            int i;
            for(i = line->characterCount-1; i > 0; i--)
            {
                if(line->text[i] != ' ')
                    break;
            }
             printBasicWide(font_grp, bounds.x + bounds.width/2, bounds.y+fontHeight, z, scale, scale, CENTER_X, line->text, i+1, rgba);
        }
        bounds.y += fontHeight;
    }
    //clipperPop();
}

unsigned int stGetTextWidth(FormattedText* text)
{
    int i;
    unsigned int maxWidth = 0;
    for(i = 0; i < text->lines.size; i++)
    {
        FormattedLine* line = text->lines.storage[i];
        maxWidth = max(maxWidth, line->lineWidth);
    }
    return maxWidth;
}

unsigned int stGetTextHeight(FormattedText* text, float scale)
{
    unsigned int fontHeight = ttGetFontHeight(font_grp, scale, scale);
    return fontHeight * text->lines.size;
}

