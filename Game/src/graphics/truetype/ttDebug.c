#include "ttDebug.h"
#include "cmdparse/cmdgame.h"
#include "render/renderprim.h"
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/StringUtil.h>
#include "graphics/truetype/ttFontDraw.h"
#include "graphics/truetype/ttFontBitmap.h"
#include "graphics/truetype/ttFontCore.h"
#include "graphics/truetype/ttFontManager.h"
#include "graphics/ttFontUtil.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "render/tex.h"

//-------------------------------------------------------------------------------
// TT box drawing functions
typedef struct
{
    float x;
    float y;
} TTPointFloatXY;

typedef struct
{
    union
    {
        struct
        {
            float left;
            float top;
        };
        TTPointFloatXY upperLeft;
    };
    union
    {
        struct
        {
            float right;
            float bottom;
        };
        TTPointFloatXY lowerRight;
    };
} TTExtents;

typedef struct
{
    TTPointFloatXY origin;
    float width;
    float height;
} TTBox;

typedef struct
{
    Color topLeft;
    Color topRight;
    Color bottomLeft;
    Color bottomRight;
} TTBoxColor;

typedef struct
{
    TTBox box;
    TTBoxColor color;
} TTColoredBox;

static void ttFixRectangleBasic(float* x1, float* y1, float* x2, float* y2)
{
    assert(x1 && y1 && x2 && y2);
#define SWAP(type, val1, val2)    \
    {                            \
    type temp;                \
    temp = val1;            \
    val1 = val2;            \
    val2 = temp;            \
    }

    if(*x1 > *x2){
        SWAP(float, *x1, *x2);
    }

    if(y1 > y2){
        SWAP(float, *y1, *y2);
    }
#undef SWAP
}

// Stolen from Martin's code somewhere
static void ttDrawBoxBasic(float x1, float y1, float x2, float y2, int argb_ul, int argb_ur, int argb_ll, int argb_lr, int filled){

    assert(filled);
    ttFixRectangleBasic(&x1, &y2, &x2, &y2);
    drawFilledBox4(x1, y1, x2, y2, argb_ul, argb_ur, argb_ll, argb_lr);
}

/*
void ttDrawFilledBox(TTColoredBox* box)
{
    ttDrawBoxBasic(
        box->box.left, box->box.bottom, box->box.right, box->box.top,
        box->color.topLeft.integer, box->color.topRight.integer, box->color.bottomLeft.integer, box->color.bottomRight.integer,
        1);
}

void ttDrawBox(TTColoredBox* box)
{
    ttDrawBoxBasic(
        box->box.left, box->box.bottom, box->box.right, box->box.top,
        box->color.topLeft.integer, box->color.topRight.integer, box->color.bottomLeft.integer, box->color.bottomRight.integer,
        0);
}
*/

void ttDrawFilledBox(TTColoredBox* box, float x, float y)
{
    float originX = box->box.origin.x + x;
    float originY = box->box.origin.y + y;
    ttDrawBoxBasic(
        originX, originY, originX + box->box.width, originY + box->box.height,
        box->color.topLeft.integer, box->color.topRight.integer, box->color.bottomLeft.integer, box->color.bottomRight.integer,
        1);
}

void ttDrawBox(TTColoredBox* box, float x, float y)
{
    float originX = box->box.origin.x + x;
    float originY = box->box.origin.y + y;
    ttDrawBoxBasic(
        originX, originY, originX + box->box.width, originY + box->box.height,
        box->color.topLeft.integer, box->color.topRight.integer, box->color.bottomLeft.integer, box->color.bottomRight.integer,
        0);
}

//static void ttDrawBoxBasic(float x1, float y1, float x2, float y2, int argb_ul, int argb_ur, int argb_ll, int argb_lr)
//{
//    ttFixRectangleBasic(&x1, &y2, &x2, &y2);
//}

//-------------------------------------------------------------------------------
// TT Debug text

typedef struct
{
    TTBox box;
    float advanceX;
} TTGlyphInfo;
TTGlyphInfo* ttGlyphInfoCreate()
{
    return calloc(1, sizeof(TTGlyphInfo));
}
void ttGlyphInfoDestroy(void* infoData)
{
    TTGlyphInfo* info = (TTGlyphInfo*)infoData;
    if(info)
        free(info);
}

typedef struct TTDebugText
{
    // Which font do I use to draw myself?
    TTDrawContext font;

    float xScale;    // How should the text be distorted?
    float yScale;

    // What text do I draw?
    WCHAR* text;
    int characterCount;

    // What are the boxes for each of the characters?
    TTGlyphInfo** glyphInfos;
} TTDebugText;

TTDebugText* ttDebugTextCreate(TTDrawContext* font, float xScale, float yScale, char* text)
{
    TTDebugText* obj;
    if(!font || !text)
        return NULL;

    obj = calloc(1, sizeof(TTDebugText));

    obj->font = *font;

    obj->characterCount = UTF8ToWideStrConvert(text, NULL, 0);
    obj->text = malloc(sizeof(WCHAR) * obj->characterCount);
    UTF8ToWideStrConvert(text, obj->text, obj->characterCount * sizeof(WCHAR));

    ttDebugTextAlterScale(obj, xScale, yScale);
    return obj;
}

void ttDebugTextDestroy(TTDebugText* obj)
{
    if(!obj)
        return;
    if(obj->glyphInfos)
    {
        eaDestroyEx(&obj->glyphInfos, ttGlyphInfoDestroy);
    }

    if(obj->text)
        free(obj->text);
}

typedef struct
{
    TTTextForEachGlyphParam forEachGlyphParam;
    TTDebugText* text;
}TTDebugTextParam;


static int ttDebugTextGatherGlyphInfo(TTDebugTextParam* param)
{
    const TTFMCacheElement* element = param->forEachGlyphParam.cachedTexture;
    TTTextForEachGlyphParam* glyphParam = &param->forEachGlyphParam;
    TTGlyphInfo* info = ttGlyphInfoCreate();

    info->box.origin.x = glyphParam->left;
    info->box.origin.y = glyphParam->top;
    info->box.width = glyphParam->xScale * element->bitmapInfo.width;
    info->box.height = glyphParam->yScale * element->bitmapInfo.height;
    info->advanceX = element->bitmapInfo.advanceX;
    eaPush(&param->text->glyphInfos, info);
    return 1;
}

static int ttDebugTextGatherGlyphInfoGlyphCallback(TTTextForEachGlyphParam* param)
{
    return ttDebugTextGatherGlyphInfo((TTDebugTextParam*)param);
}

void ttDebugTextAlterScale(TTDebugText* obj, float xScale, float yScale)
{
    TTDebugTextParam param;

    // Destroy any previously gathered glyph info.
    if(obj->glyphInfos)
        eaClearEx(&obj->glyphInfos, ttGlyphInfoDestroy);

    // Save the specified scale.
    obj->xScale = xScale;
    obj->yScale = yScale;

    // Gather the glyph info for the text at the specified scale.
    param.forEachGlyphParam.handler = ttDebugTextGatherGlyphInfoGlyphCallback;
    param.text = obj;
    ttTextForEachGlyph(&obj->font, (TTTextForEachGlyphParam*)&param, 0, 0, xScale, yScale, obj->text, obj->characterCount, true);
}

void ttDebugTextDraw(TTDebugText* obj, float x, float y)
{
    // Draw all the glyph boxes first.

    if(1){
        int i;
        TTColoredBox box;
        box.color.bottomLeft = CreateColorRGB(128, 0, 0);
        box.color.bottomRight = box.color.topLeft = box.color.topRight = box.color.bottomLeft;

        for(i = eaSize(&obj->glyphInfos)-1; i >= 0; i--)
        {
            box.box = obj->glyphInfos[i]->box;
            ttDrawBox(&box, x, y);
        }
    }


    // Draw the glyphs.
    {
        int rgba[4];
        rgba[0] = 0xff0000ff;
        rgba[1] = 0xff;
        rgba[2] = 0xff;
        rgba[3] = 0xff0000ff;
        ttDrawText2DWithScaling(&obj->font, x, y, -1, obj->xScale, obj->yScale, rgba, obj->text, obj->characterCount);
    }
}

//-------------------------------------------------------------------------------
typedef struct
{
    int enabled;
} TTDebugState;

TTDebugState ttDebugState;

void ttDebugEnable(int enable)
{
    if(enable == ttDebugState.enabled)
        return;

    if(enable)
    {
        // Turn off game 2d interface.
        game_state.disableGameGUI = 1;
    }
    else
    {
        // Turn on game 2d interface.
        game_state.disableGameGUI = 0;
    }

    ttDebugState.enabled = enable;
}

int ttDebugEnabled()
{
    return ttDebugState.enabled;
}

void ttDrawString(float x, float y, char* format, ...)
{
#define bufferSize 1024
    char buffer[bufferSize];
    va_list args;

    va_start(args, format);
    _vsnprintf(buffer, bufferSize, format, args);
    buffer[bufferSize-1] = '\0';
    va_end(args);

    {
        WCHAR wbuffer[bufferSize];
        int rgba[4];
        int characterCount;
        extern TTDrawContext referenceFont;

        rgba[0] = 0xff;
        rgba[1] = 0xff;
        rgba[2] = 0xff;
        rgba[3] = 0xff;

        characterCount = UTF8ToWideStrConvert(buffer, wbuffer, bufferSize * sizeof(WCHAR));
        ttDrawText2DWithScaling(&referenceFont, x, y, -1, 1.0, 1.0, rgba, wbuffer, characterCount);
    }
#undef bufferSize
}

extern void ttDrawDebugSetScale(float scale);
extern char* ttGetReport();

void ttDebug()
{
    static TTDebugText* text1;
    static TTDebugText* text2;
    float globalScale = 1.0;

    // Clear the screen
    //    Draw a big polygon over the entire screen.
    ttDrawBoxBasic(0, 0, game_state.screen_x, game_state.screen_y, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 1);

    if(!text1)
    {
        extern TTDrawContext chatFont;
        text1 = ttDebugTextCreate(&chatFont, 1.0, 1.0, "Na");
        ttDebugTextAlterScale(text1, 10.0, 10.0);
        text1->font.renderParams.bold = 0;
    }

    if(!text2)
    {
        extern TTDrawContext chatFont;
        text2 = ttDebugTextCreate(&chatFont, 1.0, 1.0, "v");
        ttDebugTextAlterScale(text2, 10.0, 10.0);
        text2->font.renderParams.bold = 1;
    }

    if(inpLevel(INP_RBUTTON))
    {
        float scale;
        int mouseY;
        inpMousePos(NULL, &mouseY);
        scale = (float)(game_state.screen_y - mouseY) / game_state.screen_y * 10.0;
        ttDrawString(0, 0, "Scale: %.5f", scale);
        ttDrawString(0, 15, "Effective size: %.4f", text1->font.renderParams.renderSize * scale);
        ttDrawDebugSetScale(globalScale);
        ttDebugTextAlterScale(text1, scale, scale);
        ttDebugTextAlterScale(text2, scale, scale);
        ttDrawDebugSetScale(1.0);
    }

    {
        float x = 100, y = 100;
        int wd;

        ttDrawDebugSetScale(globalScale);
        strDimensionsBasicWide(&text1->font, text1->xScale, text1->yScale, text1->text, wcslen(text1->text), &wd, NULL, NULL);
        ttDebugTextDraw(text1, 100, 100);
        ttDebugTextDraw(text2, 100+wd, 100);
        ttDrawDebugSetScale(1.0);
    }

    {
        char* report;
        report = ttGetReport();
        if(report)
            ttDrawString(0, 90, report);
    }
}
