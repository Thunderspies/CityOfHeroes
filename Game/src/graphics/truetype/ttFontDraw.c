#include "graphics/truetype/ttFontDraw.h"
#include "graphics/truetype/ttFontCore.h"
#include "graphics/truetype/ttFontManager.h"

//#include "render/thread/wcw_statemgmt.h"    // WCW_BindTexture()
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/components/StringTable.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/HashFunctions.h>
#include "render/thread/rt_queue.h"
#include <utilitieslib/components/StashTable.h>

#include "UI/sprite/sprite_base.h"    // for applyToScreenScalingFactorf()
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_list.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiGame.h"            // for shell_menu()
#include "render/tex.h"
#include "graphics/textureatlas.h"

#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>

typedef struct Clipper2D Clipper2D;

//-------------------------------------------------------------------
// Super hacky debugging stuff
typedef struct
{
    int scale_debug;
    float scale;    // global text scaling

    unsigned int gatherForEachGlyphReport;
    StuffBuff report;
} TTDrawDebugState;
TTDrawDebugState ttDrawDebugState;


typedef struct{
    TTTextForEachGlyphParam forEachGlyphParam;
    float x;
    float y;
    float z;
    int rgba[4];
    Clipper2D *clipper;
} TTDrawTextParam;

typedef struct{
    TTTextForEachGlyphParam forEachGlyphParam;
    int rgba[4];
    RenderCallback callback;
    void *userData;
} TTDrawTextParamSoftware;


//typedef struct TTTextDimensionParam TTTextDimensionParam;
//int ttGetStringDimensionsHandler(TTTextDimensionParam* param);

void ttDrawDebugSetScale(float scale)
{
    ttDrawDebugState.scale = scale;
    ttDrawDebugState.scale_debug = scale!=0;
}

char* ttGetReport()
{
    return ttDrawDebugState.report.buff;
}

void ttReport(char* format, ...)
{
    va_list args;
    char buffer[1024];

    if(!ttDrawDebugState.gatherForEachGlyphReport)
        return;

    va_start(args, format);
    _vsnprintf(buffer, ARRAY_SIZE(buffer), format, args);
    buffer[ARRAY_SIZE(buffer)-1] = '\0';
    va_end(args);

    if(!ttDrawDebugState.report.buff)
        initStuffBuff(&ttDrawDebugState.report, 1024);
    addSingleStringToStuffBuff(&ttDrawDebugState.report, buffer);
}

//#define ttReport

void ttClearReport()
{
    //clearStuffBuff(&ttDrawDebugState.report);
}

/*****************************************************************************************
 * TTDrawContext
 */
TTDrawContext* createTTDrawContext(){
    return calloc(1, sizeof(TTDrawContext));
}

void initTTDrawContext(TTDrawContext* context, int drawSize){
    // The user requested this draw size.
    context->renderParams.renderSize = drawSize;

    context->renderParams.defaultSize = drawSize;
    //context->renderParams.defaultSize = 64;
}

//------------------------------------------------------------------------------
// Generic glyph processing
//------------------------------------------------------------------------------
#define WITHIN_TOLERANCE(val, target, tolerance) ((val < target + tolerance) && (val > target - tolerance))
static int ttNormalizeRenderScaling(TTDrawContext* context, float* xScale, float* yScale)
{
    float intendedXRenderSize;
    float intendedYRenderSize;
    int oldRenderSize = context->renderParams.renderSize;

    // Given the current rendersize and scaling factors, what is the real rendersize?
    intendedXRenderSize = (float)context->renderParams.renderSize * *xScale;
    intendedYRenderSize = (float)context->renderParams.renderSize * *yScale;


    //if(!WITHIN_TOLERANCE(*xScale, 1, 0.1) || !WITHIN_TOLERANCE(*yScale, 1, 0.1))
    {
        float* scaleLarge;    // Larger of the two scale values
        float* scaleSmall;    // Smaller of the two scale values
        float* intendedScaleLarge;
        float* intendedScaleSmall;

        // JS:    Always respect y size.
        //        Since the glyph placement code depends the glyphs dimension data to line the glyps
        //        up, scaling y will not work correctly unless the glyph placement code also takes the
        //        scaling factor into consideration.
        // JE:  Despite the existence of this comment, it appears the code was doing the exact
        //      opposite of what the comment dictated.  Changed.  Looks better.  Yatta!
        //old, broken code?    if(!context->dynamic && *xScale > *yScale){
        if(context->dynamic || *xScale > *yScale)
        {
            scaleLarge = xScale;
            intendedScaleLarge = &intendedXRenderSize;

            scaleSmall = yScale;
            intendedScaleSmall = &intendedYRenderSize;
        }
        else
        {
            scaleLarge = yScale;
            intendedScaleLarge = &intendedYRenderSize;

            scaleSmall = xScale;
            intendedScaleSmall = &intendedXRenderSize;
        }

        context->renderParams.renderSize = ceil(((float)*scaleSmall * context->renderParams.renderSize));

        if(context->dynamic)
        {
            // If the font is going to be dynamic, changing size continuously, hold the font at the current hinting size
            // so the glyphs do not bounce around.
            *scaleLarge = *intendedScaleLarge / (float)context->renderParams.renderSize;
            *scaleSmall = *intendedScaleSmall / (float)context->renderParams.renderSize;
        }
        else
        {
            // If the font is going to be static, non-moving, non-scaling, calculate proper hinting size so the
            // text looks perfect.
            context->renderParams.defaultSize = context->renderParams.renderSize;

            // Do not distort static text so that it looks crisp.
            *scaleLarge = 1.0;
            *scaleSmall = 1.0;
        }
    }

    return oldRenderSize;
}

//#define DEBUGTEXTDISPLAY 1
// When enabled, this will cause a message to be printed out to a log file every time a unique string gets displayed
#ifdef DEBUGTEXTDISPLAY

FILE *fTextDebug=0;
StashTable htTextDebug=0;

typedef struct TTCachedDebugString {
    unsigned short *text;
    TTDrawContext *drawContext;
    int xscale;
    int yscale;
    int size;
} TTCachedDebugString;

unsigned int tdbHash(const void* key, int hashSeed)
{
    TTCachedDebugString *ttcsd = (TTCachedDebugString*)key;
    int hashval = hash(ttcsd->text, wcslen(ttcsd->text)*2, hashSeed);
    hashval ^= (int)ttcsd->drawContext;
    hashval ^= ttcsd->xscale;
    hashval ^= ttcsd->yscale;
    hashval ^= ttcsd->size;
    return hashval;
}

int tdbCompare(const void* refKey, const void* key)
{
    TTCachedDebugString *refElem = (TTCachedDebugString*)refKey;
    TTCachedDebugString *elem = (TTCachedDebugString*)key;
    if (refElem->xscale != elem->xscale)
        return refElem->xscale - elem->xscale;
    if (refElem->size != elem->size)
        return refElem->size - elem->size;
    if (refElem->yscale != elem->yscale)
        return refElem->yscale - elem->yscale;
    if (refElem->drawContext != elem->drawContext)
        return refElem->drawContext - elem->drawContext;
    return wcscmp(refElem->text, elem->text);
}

static StringTable stDebugStrings=0;

TTCachedDebugString *createTTCachedDebugString(unsigned short *text, TTDrawContext *drawContext, float xscale, float yscale, int size)
{
    TTCachedDebugString *ret = malloc(sizeof(TTCachedDebugString));;
    if (!fTextDebug) {
        fTextDebug = fopen("C:\\textdebug.txt","w");
    }
    if (!stDebugStrings) {
        stDebugStrings = createStringTable();
        initStringTable(stDebugStrings, 8 * 1024);
        strTableSetMode(stDebugStrings, WideString);
    }
    ret->text = strTableAddString(stDebugStrings, text);
    ret->drawContext = drawContext;
    ret->xscale = *(int*)&xscale;
    ret->yscale = *(int*)&yscale;
    ret->size = size;
    return ret;
}


int printFont(TTDrawContext* context, unsigned short* buffer) {
    if (context == &tinyfont) { return swprintf(buffer,L"tinyfont");
    } else if (context == &game_9) { return swprintf(buffer,L"game_9");
    } else if (context == &game_12) { return swprintf(buffer,L"game_12"); 
    } else if (context == &game_14) { return swprintf(buffer,L"game_14");
    } else if (context == &game_18) { return swprintf(buffer,L"game_18");
    } else if (context == &gamebold_9) { return swprintf(buffer,L"gamebold_9");
    } else if (context == &gamebold_12) { return swprintf(buffer,L"gamebold_12");
    } else if (context == &gamebold_14) { return swprintf(buffer,L"gamebold_14");
    } else if (context == &gamebold_18) { return swprintf(buffer,L"gamebold_18");
    } else if (context == &title_9) { return swprintf(buffer,L"title_9");
    } else if (context == &title_12) { return swprintf(buffer,L"title_12");
    } else if (context == &title_14) { return swprintf(buffer,L"title_14");
    } else if (context == &title_18) { return swprintf(buffer,L"title_18");
    } else if (context == &chatThin) { return swprintf(buffer,L"chatThin");
    } else if (context == &chatFont) { return swprintf(buffer,L"chatFont");
    } else if (context == &chatBold) { return swprintf(buffer,L"chatBold");
    } else if (context == &smfLarge) { return swprintf(buffer,L"smfLarge");
    } else if (context == &smfDefault) { return swprintf(buffer,L"smfDefault");
    } else if (context == &smfSmall) { return swprintf(buffer,L"smfSmall");
    } else if (context == &smfVerySmall) { return swprintf(buffer,L"smfVerySmall");
    } else if (context == &smfMono) { return swprintf(buffer,L"smfMono");
    } else if (context == &smfComputer) { return swprintf(buffer,L"smfComputer");
    } else if (context == &profileFont) { return swprintf(buffer,L"profileFont");
    } else if (context == &entityNameText) { return swprintf(buffer,L"entityNameText");
    } else if (context == &referenceFont) { return swprintf(buffer,L"referenceFont");
    } else {
        return swprintf(buffer,L"%i",context);
    }

}


void checkTextDebug(TTDrawContext* context, float xScale, float yScale,int size, unsigned short* text)
{
    static TTCachedDebugString dummyTtcsd;
    TTCachedDebugString *ttcsd=&dummyTtcsd;
    if (!htTextDebug) {
        htTextDebug = stashTableCreate(1000, StashDefault, StashKeyTypeFixedSize, sizeof(TTCachedDebugString));
    }

    // Dummy element for lookup
    ttcsd->text = text;
    ttcsd->drawContext = context;
    ttcsd->xscale = *(int*)&xScale;
    ttcsd->yscale = *(int*)&yScale;
    ttcsd->size = size;
    // Get cached value
    if (!stashFindPointer(htTextDebug, ttcsd, NULL))
        unsigned short buffer[128];
        // Failed lookup, create one!
        ttcsd = createTTCachedDebugString(text, context, xScale, yScale,size);
        stashAddPointer(htTextDebug, ttcsd, ttcsd, false);
        fprintf(fTextDebug,"%S\t %f\t %f\t %i\t ",text,xScale,yScale,size);
        printFont(context,buffer);
        fprintf(fTextDebug,"%S\b",buffer);
        fflush(fTextDebug);
    }
}

#endif


void ttTextForEachGlyph(TTDrawContext* context, TTTextForEachGlyphParam* param, float x, float y, float xScale, float yScale, unsigned short* text, unsigned int textLength, int allowScaling){
    int i;
    TTFMCacheElement* element;

    //Kerning Testing: int glyphIndex;
    //Kerning Testing: int useKerning;
    //Kerning Testing: FT_Face face;
    //Kerning Testing: int previous;
    int oldRenderSize = context->renderParams.renderSize;
    int oldDefaultSize = context->renderParams.defaultSize;
    float xStringScale, yStringScale;
    float xScaleIn = xScale;
    float stringOffset=0; //JE: I'm initializing this to 0 because it's throwing a RTC exception in Full Debug... not sure what it should be

    if(!context->font || !context->font->manager)
        return;
    if(xScale < 0)
        xScale = 0;
    if(yScale < 0)
        yScale = 0;

    if (xScale == 0 || yScale == 0)
        return;

    ttClearReport();
    ttReport("Drawing string: \"%S\" size: %i scale: %.2f, %.2f\n", text, oldRenderSize, xScale, yScale);
#ifdef DEBUGTEXTDISPLAY
    if (x != 0.0f && y != 0.0f) {
        checkTextDebug(context,xScale,yScale,context->renderParams.renderSize,text);
        textLength = printFont(context,text);
    }
        
#endif


    // FIXME!!!
    //    The following two function calls really don't belong in this function.
    //    They are in here so properly apply these scaling factors to all text processing functions.

    if (allowScaling) {
        // Before processing any of the glyphs, alter the scaling factor if we are in the shell.
        applyToScreenScalingFactorf(&xScale, &yScale);
    }

    // Make sure we are requesting a sane render size, given the scaling factors we are requesting.
    // For example, it doesn't make sense to have a render size 5 and then apply a 10x scaling factor
    // to scale it to the correct size on a 1600x1200 display.  This would result in ugly and/or unreadable
    // text.  Instead, use a render size of 50 and a scaling factor of 1.
    ttNormalizeRenderScaling(context, &xScale, &yScale);
    if(context->renderParams.renderSize != oldRenderSize)
    {
        ttReport("Normalized to size %i font at scale %.2f, %.2f\n", context->renderParams.renderSize, xScale, yScale);
    }



    xStringScale = xScale;
    yStringScale = yScale;

    //Kerning Testing: previous = 0;

    // For each character in the string...
    for(i = 0; i < (int)textLength; i++){
        xScale = xStringScale;
        yScale = yStringScale;

        context->renderParams.unicodeCharacter = text[i];        // In case the character needs to be renderred to texture, it will need to know which glyph to render.
        if('\0' == text[i])
            break;
        param->unicodeCharacter = text[i];                        // ForEach handler can tell which character it is looking at.
        //printf("Processing %C\n", text[i]);

        // Do we have one of these letters cached somewhere?
        PERFINFO_AUTO_START("ttFMGetCachedTexture", 1);
            element = ttFMGetCachedTexture(context->font, &context->renderParams);
        PERFINFO_AUTO_STOP();

        if(ttDrawDebugState.scale_debug)
        {
            xScale *= ttDrawDebugState.scale;
            yScale *= ttDrawDebugState.scale;
        }

        // We have the texture now.
        if(element){
//Kerning Testing: FT_GlyphSlot slot;
//Kerning Testing: TTSimpleFont* subFont;

//Kerning Testing: 
//            // Try to find a subfont that has knowledge of the current character.
//            //PERFINFO_AUTO_START("ttFontGetFontForCharacter", 1);
//                subFont = ttFontGetFontForCharacter(context->font, text[i]);
//            //PERFINFO_AUTO_STOP();
//
//            // If such a character cannot be found, fail silently.
//            // FIXME!!!
//            //    Failing silently is not a good idea!.
//            if(!subFont)
//                continue;
//
//            face = subFont->font;
//            glyphIndex = FT_Get_Char_Index(face, text[i]);
//            useKerning = FT_HAS_KERNING(face);
//
//            slot = face->glyph;
//
//            //Kerning Testing: 
//            // retrieve kerning distance and adjust the position of this letter as appropriate.
//            //if(useKerning && previous && glyphIndex){
//            //    FT_Vector  delta;
//
//            //    FT_Get_Kerning( face, previous, glyphIndex,
//            //          ft_kerning_unscaled, &delta );  // was using "ft_kerning_default"
//            //
//
//            //    x += (delta.x >> 6) * xScale;
//            ////    printf("\tKerning: %i\n", delta.x >> 6);
//            //}
//
//            //Kerning Testing: previous = glyphIndex;

            {
#if 0
                float penX;
                float penY;

                float top;
                float left;
                float right;
                float bottom;

                penX = x;
                penX += element->bitmapInfo.bitmapLeft * xScale;
                penY = y + element->bitmapInfo.bitmapTop;

                top = penY;
                bottom = penY - (element->bitmapInfo.realSize * element->drawinfo.bottom);// + 3;
                top = (top - bottom) * yScale + bottom;

                left = penX;
                right = penX + (element->bitmapInfo.realSize * element->drawinfo.right);
                right = (right - left) * xScale + left;

                param->cachedTexture = element;
                param->top = top;
                param->left = left;
                param->right = right;
                param->bottom = bottom;
#endif

                param->cachedTexture = element;
                param->top = y - element->bitmapInfo.bitmapTop * yScale - 1;
                param->left = x + element->bitmapInfo.bitmapLeft * xScale;
                param->xScale = xScale;
                param->yScale = yScale;

                if(context->dynamic)
                {
                    TTFMCacheElement* appliedGlyph;
                    int newRenderSize = context->renderParams.renderSize;
                    float advanceScale;
                    context->renderParams.renderSize = oldRenderSize;
                    appliedGlyph = ttFMGetCachedTexture(context->font, &context->renderParams);
                    context->renderParams.renderSize = newRenderSize;
                    assert(appliedGlyph);

                    if (ttDrawDebugState.scale_debug && ttDrawDebugState.scale) {
                        advanceScale = ttDrawDebugState.scale;
                    } else {
                        advanceScale = xScaleIn;
                    }
                    if (allowScaling) {
                        applyToScreenScalingFactorf(&advanceScale, NULL);
                    }
                    x += appliedGlyph->bitmapInfo.advanceX * advanceScale;
                }
                else
                       x += element->bitmapInfo.advanceX * xScale;// / (ttDrawDebugState.scale ? ttDrawDebugState.scale : 1.0);
                param->nextGlyphLeft = ceil(x);
                   //    printf("Processing glyph: \"%C\"  advance: %i -- %.3f  \n", param->unicodeCharacter, element->bitmapInfo.advanceX, x);

                if(!param->handler(param))
                    break;
//                if (param->handler == (GlyphHandler)ttGetStringDimensionsHandler) {
//                    printf("  bi: ax:%d t:%d l:%d w:%d h:%d rs:%d\n", element->bitmapInfo.advanceX, element->bitmapInfo.bitmapTop, element->bitmapInfo.bitmapLeft, element->bitmapInfo.width, element->bitmapInfo.height, element->bitmapInfo.realSize);
//                    printf("  elem: t:%g l:%g b:%g r:%g xo:%g yo:%g xe:%g ye:%g\n", element->top, element->left, element->bottom, element->right, element->xOrigin, element->yOrigin, element->xEnd, element->yEnd);
//                }
            }
        }
    }
    context->renderParams.renderSize = oldRenderSize;
    context->renderParams.defaultSize = oldDefaultSize;
}




//------------------------------------------------------------------------------
// Basic text drawing
//------------------------------------------------------------------------------

/*************************************************************************************************
* Function ttDrawText2DWithScaling() and related stuff
*    This function will attempt to draw a string using openGL immediately.  ttTextForEachGlyph()
*    does the bulk of the work, calculating where each glyph ought to be.  ttDrawText2DWithScalingHandler()
*    is then called by ttTextForEachGlyph() for each glyph that should be drawn in the string, which
*    simply relays the information to openGL for displaying.
*
*/
int ttDrawText2DWithScalingHandlerSprite(TTDrawTextParam* param)
{
#if 0
    RdrGlyph                *rg = createRdrGlyph();
    TTTextForEachGlyphParam    *glyphParam = &param->forEachGlyphParam;


    rg->element    = &param->forEachGlyphParam.cachedTexture->drawinfo;
    rg->top        = glyphParam->top;
    rg->left    = glyphParam->left;
    rg->bottom    = glyphParam->bottom;
    rg->right    = glyphParam->right;
    rg->z        = param->z;
    memcpy(rg->rgba,param->rgba,sizeof(rg->rgba));
#endif
    TTTextForEachGlyphParam    *glyphParam = &param->forEachGlyphParam;
    AtlasTex *tex = param->forEachGlyphParam.cachedTexture->tex;

    if (tex)
    {
        display_sprite_ex(    tex, 0,
                            glyphParam->left, glyphParam->top, param->z,
                            glyphParam->xScale, glyphParam->yScale,
                            param->rgba[0], param->rgba[1], param->rgba[2], param->rgba[3],
                            0,0,1,1,
                            0,0,
                            param->clipper,
                            0, H_ALIGN_LEFT, V_ALIGN_TOP);
    }

    return 1;
}

int ttDrawText2DWithScalingHandler(TTDrawTextParam* param)
{
    TTTextForEachGlyphParam    *glyphParam = &param->forEachGlyphParam;
    AtlasTex *tex = param->forEachGlyphParam.cachedTexture->tex;

    if (tex)
    {
        display_sprite_immediate_ex(tex, 0,
                                    glyphParam->left, glyphParam->top, param->z,
                                    glyphParam->xScale, glyphParam->yScale,
                                    param->rgba[0], param->rgba[1], param->rgba[2], param->rgba[3],
                                    0,0,1,1,
                                    0,0,
                                    param->clipper,
                                    0);
    }

    return 1;
}

int ttDrawText2DWithScalingSoftwareHandler(TTDrawTextParamSoftware* param)
{
#if 0
    const TTFMCacheElement* element = param->forEachGlyphParam.cachedTexture;
    TTTextForEachGlyphParam* glyphParam = &param->forEachGlyphParam;
    float top, left, bottom, right;

    top = glyphParam->top;
    left = glyphParam->left;
    bottom = glyphParam->bottom;
    right = glyphParam->right;

    param->callback(param->userData, top, left, bottom, right, element->bitmap, param->rgba);
#endif

    const TTFMCacheElement* element = param->forEachGlyphParam.cachedTexture;
    TTTextForEachGlyphParam* glyphParam = &param->forEachGlyphParam;
    float height = glyphParam->yScale * element->bitmapInfo.height;
    float width = glyphParam->xScale * element->bitmapInfo.width;

    param->callback(param->userData, glyphParam->top, glyphParam->left, height, width, element->bitmap, param->rgba);

    return 1;
}



/* Function ttDrawText2DWithScaling()
*    Parameters:
*        context -
*        x -
*        y -
*        z -
*        xScale -
*        yScale -
*        rgba - {left-bottom, left-top, right-top, right-bottom}
*
*/
#include "UI/uiClipper.h"
#include "win/win_init.h"

static int ttDrawText2DWithScalingHandlerSpriteGlyphCallback(TTTextForEachGlyphParam* param)
{
    return ttDrawText2DWithScalingHandlerSprite((TTDrawTextParam*)param);
}

void ttDrawText2DWithScalingSprite(TTDrawContext* context, float x, float orig_y, float z, float xScale, float yScale, int rgba[4], unsigned short* text, int textLength){
    TTDrawTextParam drawParam;
    float            y = orig_y;

    if (xScale == 0 || yScale == 0)
        return;

    PERFINFO_AUTO_START("ttDrawText2DWithScalingSprite", 1);

        memset(&drawParam, 0, sizeof(drawParam));
        drawParam.forEachGlyphParam.handler = ttDrawText2DWithScalingHandlerSpriteGlyphCallback;
        if(context->dynamic)
        {
            // Assume that the dynamic text is going to moving and scaling frequently.
            // Allow it to draw anywhere so its movement looks smooth.
        }
        else
        {
            x = ceil(x);    // Do not attempt to start drawing a glyph in between pixels so the texture is not
            y = ceil(y);    // resampled when drawn.
        }
        drawParam.z = z;
        memcpy(drawParam.rgba, rgba, sizeof(int) * 4);

        //printf("--------------- Drawing string ---------------\n");

        // Some people pass in too large of a length
        textLength = MIN(textLength, (int)wcslen(text));

        drawParam.clipper = clipperGetCurrent();
        ttTextForEachGlyph(context, (TTTextForEachGlyphParam*)&drawParam, x, y, xScale, yScale, text, textLength, true);
        
    PERFINFO_AUTO_STOP();
}

static int ttDrawText2DWithScalingHandlerGlyphCallback(TTTextForEachGlyphParam* param)
{
    return ttDrawText2DWithScalingHandler((TTDrawTextParam*)param);
}

void ttDrawText2DWithScaling(TTDrawContext* context, float x, float y, float z, float xScale, float yScale, int rgba[4], unsigned short* text, int textLength){
    TTDrawTextParam drawParam;
    int w, h;

    if (xScale == 0 || yScale == 0)
        return;

    PERFINFO_AUTO_START("ttDrawText2DWithScaling", 1);

        windowClientSize(&w, &h);
        y = h - y;

        memset(&drawParam, 0, sizeof(drawParam));
        drawParam.forEachGlyphParam.handler = ttDrawText2DWithScalingHandlerGlyphCallback;
        if(context->dynamic)
        {
            // Assume that the dynamic text is going to moving and scaling frequently.
            // Allow it to draw anywhere so its movement looks smooth.
        }
        else
        {
            x = ceil(x);    // Do not attempt to start drawing a glyph in between pixels so the texture is not
            y = ceil(y);    // resampled when drawn.
        }
        drawParam.z = z;
        memcpy(drawParam.rgba, rgba, sizeof(int) * 4);

        //printf("--------------- Drawing string ---------------\n");

        ttTextForEachGlyph(context, (TTTextForEachGlyphParam*)&drawParam, x, y, xScale, yScale, text, textLength, true);
        
    PERFINFO_AUTO_STOP();
}

static int ttDrawText2DWithScalingSoftwareHandlerGlyphCallback(TTTextForEachGlyphParam* param)
{
    return ttDrawText2DWithScalingSoftwareHandler((TTDrawTextParamSoftware*)param);
}

void ttDrawText2DWithScalingSoftware(TTDrawContext* context, float x, float y, float xScale, float yScale, int rgba[4], unsigned short* text, int textLength, RenderCallback callback, void *userData)
{
    TTDrawTextParamSoftware drawParam;

    if (xScale == 0 || yScale == 0)
        return;

    PERFINFO_AUTO_START("ttDrawText2DWithScalingSoftware", 1);

        memset(&drawParam, 0, sizeof(drawParam));
        drawParam.forEachGlyphParam.handler = ttDrawText2DWithScalingSoftwareHandlerGlyphCallback;
        if(context->dynamic)
        {
            // Assume that the dynamic text is going to moving and scaling frequently.
            // Allow it to draw anywhere so its movement looks smooth.
        }
        else
        {
            x = ceil(x);    // Do not attempt to start drawing a glyph in between pixels so the texture is not
            y = ceil(y);    // resampled when drawn.
        }
        memcpy(drawParam.rgba, rgba, sizeof(int) * 4);
        drawParam.callback = callback;
        drawParam.userData = userData;

        //printf("--------------- Drawing string ---------------\n");

        ttTextForEachGlyph(context, (TTTextForEachGlyphParam*)&drawParam, x, y, xScale, yScale, text, textLength, false);

    PERFINFO_AUTO_STOP();
}



/*************************************************************************************************
* Function ttGetStringDimensionsWithScaling() and related stuff
*
*/
typedef struct TTTextDimensionParam {
    TTTextForEachGlyphParam forEachGlyphParam;
    float stringHeight;        // Same as the tallest glyph in the string.
} TTTextDimensionParam;

int ttGetStringDimensionsHandler(TTTextDimensionParam* param){

    float height = param->forEachGlyphParam.yScale * param->forEachGlyphParam.cachedTexture->bitmapInfo.height;
//    printf("%c: t:%g l:%g b:%g r:%g ngl:%g\n", param->forEachGlyphParam.unicodeCharacter, param->forEachGlyphParam.top, param->forEachGlyphParam.left, param->forEachGlyphParam.bottom, param->forEachGlyphParam.right, param->forEachGlyphParam.nextGlyphLeft);
    if (height > param->stringHeight)
        param->stringHeight = height;

    // Always continue processing the string so we can tell how wide the entire string is.
    return 1;
}

#define ENABLE_RESULT_CACHING 1
StashTable htStrDimsCache=0;
float cachedXScale=1, cachedYScale=1;

typedef struct TTCachedStringDimensions {
    int width;
    int height;
    int nextGlyphLeft;
    struct  {
        const unsigned short *text;
        TTDrawContext *drawContext;
        int shell_mode;
        int scale; // actually a float, but quicker to deal with as an int
        int bold; //manually check for boldness, to avoid caching mistakes
    } hashKeySubSet;
} TTCachedStringDimensions;


static StashTable stCachedStrings=0;

bool cacheTTStringOnly(const unsigned short *text, const unsigned short** output)
{
    bool bAlreadyCached = false;
    bool bSuccess;
    StashElement elem;
    if (!stCachedStrings)
    {
        stCachedStrings = stashTableCreateWithStringKeys(8 * 1024, StashDeepCopyKeys | StashWideString );
    }
    bSuccess = stashAddIntAndGetElement(stCachedStrings, text, 1, false, &elem);
    if (!bSuccess) // must already be there
    {
        bAlreadyCached = true; 
        bSuccess = stashFindElement(stCachedStrings, text, &elem);
        assert(bSuccess); // we should only get here if it failed to add it in the first place, which should only happen if it was already there
    }
    *output = stashElementGetWideStringKey(elem);

    return bAlreadyCached;
}

MP_DEFINE(TTCachedStringDimensions);
TTCachedStringDimensions *createTTCachedStringDimensions(unsigned short *text, TTDrawContext *drawContext, float fscale, int shell_mode)
{
    TTCachedStringDimensions*ret;
    MP_CREATE(TTCachedStringDimensions, 128);
    ret = MP_ALLOC(TTCachedStringDimensions);

    cacheTTStringOnly(text, &ret->hashKeySubSet.text);
    assert(ret->hashKeySubSet.text != NULL); // better have some text
    ret->hashKeySubSet.drawContext = drawContext;
    ret->hashKeySubSet.scale = *(int*)&fscale;
    ret->hashKeySubSet.shell_mode = shell_mode;
    ret->hashKeySubSet.bold = drawContext->renderParams.bold;
    return ret;
}

void destroyTTCachedStringDimensions(TTCachedStringDimensions *ttcsd)
{
    MP_FREE(TTCachedStringDimensions, ttcsd);
}

// Sets default cacheable size, also does some maintenence, so should be called every frame or few
static void destroyTTCachedStringDimensionsAdapter(void* arg0)
{
    destroyTTCachedStringDimensions((TTCachedStringDimensions *)arg0);
}

void ttSetCacheableFontScaling(float xScale, float yScale)
{
    if (xScale != cachedXScale || yScale != cachedYScale) {
        cachedXScale = xScale;
        cachedYScale = yScale;
        if (htStrDimsCache)
            stashTableDestroyEx(htStrDimsCache, NULL, destroyTTCachedStringDimensionsAdapter);
        htStrDimsCache = 0;
        if (stCachedStrings) {
            stashTableClear(stCachedStrings);
        }
    }
    // Check to see if hashtable has grown too large and needs to be cleaned!
    if (htStrDimsCache && stashGetOccupiedSlots(htStrDimsCache) > 1024) {
        stashTableClearEx(htStrDimsCache, NULL, destroyTTCachedStringDimensionsAdapter);
        //htStrDimsCache = 0;
        if (stCachedStrings) {
            stashTableClear(stCachedStrings);
        }
    }
}

// Thread-safe if one of the threads always says allowCache=false
static int ttGetStringDimensionsHandlerGlyphCallback(TTTextForEachGlyphParam* param)
{
    return ttGetStringDimensionsHandler((TTTextDimensionParam*)param);
}

void ttGetStringDimensionsWithScaling(TTDrawContext* context, float xScale, float yScale, unsigned short* text, unsigned int textLength, int* widthOut, int* heightOut, int* nextGlyphLeft, int allowCacheAndScaling){
    int cacheable=0;
    TTTextDimensionParam param = {0};
    int oldRenderSize = context->renderParams.renderSize;
    static TTCachedStringDimensions dummyTtcsd;
     TTCachedStringDimensions *ttcsd=&dummyTtcsd;

     if(!widthOut && !heightOut && !nextGlyphLeft)
        return;

     if (((int)textLength) <= 0) {
        if (widthOut)
            *widthOut =    0;
        if(heightOut)
            *heightOut = 0;
        if(nextGlyphLeft)
            *nextGlyphLeft = 0;
        return;
    }

#    if ENABLE_RESULT_CACHING
        if (!htStrDimsCache) {
            htStrDimsCache = stashTableCreate(64, StashDefault, StashKeyTypeFixedSize, sizeof(ttcsd->hashKeySubSet));
        }
        //cacheable = text && (xScale == cachedXScale) && (yScale == cachedYScale);
          cacheable = text && (xScale == yScale) && allowCacheAndScaling && !shellMode();
        //cacheable = cacheable && (context == cachedDrawContext);
#    endif
    if (cacheable)
    {
        unsigned int width = wcslen(text);
        if (width > textLength) {
            // Not measuring the whole string
            cacheable = false;
        }
    }
    if (cacheable) {
        // Dummy element for lookup
        // First, try to find the text in the text cache
        bool bAlreadyCached = cacheTTStringOnly(text, &ttcsd->hashKeySubSet.text);
        ttcsd->hashKeySubSet.drawContext = context;
        ttcsd->hashKeySubSet.scale = *(int*)&xScale;
        ttcsd->hashKeySubSet.shell_mode = shellMode();
        ttcsd->hashKeySubSet.bold = context->renderParams.bold;
        // Get cached value, only if the string was cached
        if ( bAlreadyCached && stashFindPointer(htStrDimsCache, &ttcsd->hashKeySubSet, &ttcsd) )
        {
            // Was in the cache!
            if (widthOut)
                *widthOut = ttcsd->width;
            if (heightOut)
                *heightOut = ttcsd->height;
            if(nextGlyphLeft)
                *nextGlyphLeft = ttcsd->nextGlyphLeft;
            return;
        }
        // Failed lookup, create one!
        ttcsd = createTTCachedStringDimensions(text, context, xScale, shellMode());
    }

    param.forEachGlyphParam.handler = ttGetStringDimensionsHandlerGlyphCallback;
    param.stringHeight = 0.0;
 
    ttTextForEachGlyph(context, (TTTextForEachGlyphParam*)&param, 0, 0, xScale, yScale, text, textLength, allowCacheAndScaling);

    if(widthOut || cacheable)
    {
        int width;
        if(param.forEachGlyphParam.cachedTexture
            && param.forEachGlyphParam.cachedTexture->renderParams.outline)
        {
            width = param.forEachGlyphParam.nextGlyphLeft + param.forEachGlyphParam.cachedTexture->renderParams.outlineWidth;
          }
        else
        {
            width = param.forEachGlyphParam.nextGlyphLeft;
        }
        if (cacheable)
            ttcsd->width = width;
        if (widthOut)
            *widthOut =    width;
    }
    if(heightOut)
        *heightOut = param.stringHeight;
    if(nextGlyphLeft)
        *nextGlyphLeft = param.forEachGlyphParam.nextGlyphLeft;

    context->renderParams.renderSize = oldRenderSize;

    if (cacheable) {
        bool bUnique;
        ttcsd->height = param.stringHeight;
        ttcsd->nextGlyphLeft = param.forEachGlyphParam.nextGlyphLeft;
        bUnique = stashAddPointer(htStrDimsCache, &ttcsd->hashKeySubSet, ttcsd, false);
        assert(bUnique);
    }
}

float ttGetStringWidthNarrow(TTDrawContext* context, float xScale, float yScale, char* text){
    TTTextDimensionParam param = {0};
#define bufferSize 10 * 1024
    unsigned short wBuffer[bufferSize];
    int textLength;
    extern TTDrawContext* font_grp;

    textLength = UTF8ToWideStrConvert(text, wBuffer, bufferSize);

    param.forEachGlyphParam.handler = ttGetStringDimensionsHandlerGlyphCallback;
    param.stringHeight = 0.0;

    ttTextForEachGlyph(context, (TTTextForEachGlyphParam*)&param, 0, 0, xScale, yScale, wBuffer, textLength, true);

    return param.forEachGlyphParam.nextGlyphLeft;
}


typedef struct TTCachedFontHeight {
    TTCompositeFont *font;
    int renderSize;
    int fontHeight;
} TTCachedFontHeight;

unsigned int hash8byte(const void* key, int hashSeed) {
    return hashCalc((void*)key, 8, hashSeed);
}

int compare8byte(const void* refKey, const void* key) {
    return memcmp(refKey, key, 8);
}

MP_DEFINE(TTCachedFontHeight);
TTCachedFontHeight *createTTCachedFontHeight(TTCompositeFont *font, int renderSize)
{
    TTCachedFontHeight *ret;
    MP_CREATE(TTCachedFontHeight, 64);
    ret = MP_ALLOC(TTCachedFontHeight);
    ret->font = font;
    ret->renderSize = renderSize;
    return ret;
}

void destroyTTCachedFontHeight(TTCachedFontHeight *ttcfh)
{
    MP_FREE(TTCachedFontHeight, ttcfh);
}

StashTable htFontHeightCache;
static TTCachedFontHeight ttcfh_dummy;

float ttGetFontHeight(TTDrawContext* context, float xScale, float yScale)
{
    TTSimpleFont* font;
    FT_Face face;
    int error;
    int i;
    int oldRenderSize = context->renderParams.renderSize;
    int oldDefaultSize = context->renderParams.defaultSize;
    float maxHeight = 0;
    TTCachedFontHeight *ttcfh = &ttcfh_dummy;

    //reverseToScreenScalingFactorf(&xScale, &yScale);
    ttNormalizeRenderScaling(context, &xScale, &yScale);

    // Check cache
    if (!htFontHeightCache) {
        // The 8 byte size is the size of the first two elements of TTCachedFontHeight
        // We don't want to hash the last element, that's what we're looking up...
        htFontHeightCache = stashTableCreate(64, StashDefault, StashKeyTypeFixedSize, 8);
    }
    // Dummy element for lookup
    ttcfh->font = context->font;
    ttcfh->renderSize = context->renderParams.renderSize;
    // Get cached value
    if (stashFindPointer(htFontHeightCache, ttcfh, &ttcfh)) {
        // Was in the cache!
        maxHeight = ttcfh->fontHeight;

        context->renderParams.renderSize = oldRenderSize;
        context->renderParams.defaultSize = oldDefaultSize;
        applyToScreenScalingFactorf(NULL, &maxHeight);
        return maxHeight;
    }
    // Failed lookup, create one!
    ttcfh = createTTCachedFontHeight(context->font, context->renderParams.renderSize);

    for(i = 0,  font = ttFontGetSubfont(context->font, i++); font; font = ttFontGetSubfont(context->font, i++))
    {
        face = font->font;
        error = FT_Set_Pixel_Sizes(
            face,                                /* handle to face object            */
            context->renderParams.renderSize,    /* pixel_width                      */
            context->renderParams.renderSize);    /* pixel_height                     */
        if(error)
        {
            printf("Error setting glyph size\n");
        }

        if(face->size->metrics.height >> 6 > maxHeight)
            maxHeight = face->size->metrics.height >> 6;

        // Leaving the default size set to something random.
        // It will be changed back to somethin appropriate when generating a glyph the next time.
    }

    // Save in cache
    ttcfh->fontHeight = maxHeight;
    stashAddPointer(htFontHeightCache, ttcfh, ttcfh, false);

    context->renderParams.renderSize = oldRenderSize;
    context->renderParams.defaultSize = oldDefaultSize;
    applyToScreenScalingFactorf(NULL, &maxHeight);
    return maxHeight;
}

// Returns whether the specified context is able to draw the specified character.
int ttFontValidateCharacter(TTDrawContext* context, unsigned short character)
{
    TTFMCacheElement* element;
    unsigned short oldCharacter = context->renderParams.unicodeCharacter;
    context->renderParams.unicodeCharacter = character;
    element = ttFMGetCachedTexture(context->font, &context->renderParams);
    context->renderParams.unicodeCharacter = oldCharacter;
    return (element ? 1 : 0);
}
/*
* TTDrawContext
*****************************************************************************************/
