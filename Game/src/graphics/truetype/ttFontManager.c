#include "graphics/truetype/ttFontManager.h"
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/components/StashTable.h>    // for hashCalc()
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/utils/utils.h>
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/HashFunctions.h>

CRITICAL_SECTION CriticalSectionFontManager;
bool CriticalSectionFontManager_inited=false;

static int ttCacheElementHash(const TTFMCacheElement* element, int hashSeed){
    return hashCalc((const char*)&element->renderParams, sizeof(TTFontRenderParams), hashSeed);
}

static int ttCacheElementCompare(const TTFMCacheElement* refKey, const TTFMCacheElement* key){
    if(memcmp(    ((unsigned char*)&refKey->renderParams), 
        ((unsigned char*)&key->renderParams), 
        sizeof(TTFontRenderParams)) == 0)
        return 0;
    else
        return 1;
}

TTFMCacheElement* createTTFMCacheElement(){
    return calloc(1, sizeof(TTFMCacheElement));
}

void destroyTTFMCacheElement(TTFMCacheElement* element){
    if (element->bitmap)
        destroyTTFontBitmap(element->bitmap);
    free(element);
}


TTFontManager* createTTFontManager(){
    TTFontManager* manager;

    if (!CriticalSectionFontManager_inited) {
        InitializeCriticalSection(&CriticalSectionFontManager);
        CriticalSectionFontManager_inited=true;
    }

    manager = calloc(1, sizeof(TTFontManager));
    manager->fonts = createArray();
    manager->glyphCache = stashTableCreate(128, StashDefault, StashKeyTypeFixedSize, sizeof(TTFontRenderParams));
    return manager;
}


static void destroyTTFMCacheElementAdapter(void* arg0)
{
    destroyTTFMCacheElement((TTFMCacheElement*)arg0);
}

void destroyTTFontManager(TTFontManager* manager){
    // Destroy all cache elements and hash table
    stashTableDestroyEx(manager->glyphCache, NULL, destroyTTFMCacheElementAdapter);
    // Destroy cache hash table.
    destroyArray(manager->fonts);
    free(manager);
}


void ttFMFreeFont(TTFontManager* manager, TTCompositeFont* font)
{
    int index = arrayFindElement(manager->fonts, font);
    StashTableIterator iter;
    StashElement elem;
    assert(index!=-1);
    arrayRemoveAndFill(manager->fonts, index);
    // Remove all elements from the hashtable that include this font


    
    stashGetIterator(manager->glyphCache, &iter);

    while (stashGetNextElement(&iter, &elem))
    {
        TTFMCacheElement* pCacheElement = stashElementGetPointer(elem);
        if (pCacheElement->font == font)
        {
            stashRemovePointer(manager->glyphCache, stashElementGetStringKey(elem), NULL);
            destroyTTFMCacheElement(pCacheElement);
        }
    }
}


/* Function ttFMAddFont()
*    This function places a font under the control of the given font manager.
*
*    Parameters:
*        manager - the font manager to add the font to.
*        font - the font to be placed under the manager's control.
*/
void ttFMAddFont(TTFontManager* manager, TTCompositeFont* font){
    // If the font is already being managed by another manager, don't do anything with it.
    if(font->manager)
        return;

    // Otherwise, place the font under the manager's control.
    arrayPushBack(manager->fonts, font);
    font->manager = manager;
}

/* Function ttFMSetMemoryLimit()
*    This function tells the manager how much memory it should use total for all fonts that it controls.
*    
*/
void ttFMSetMemoryLimit(TTFontManager* manager, unsigned int memoryLimit){

}

// Given a base font and render parameters, retrieve a cached glyph texture.
TTFMCacheElement* ttFMGetTexture(TTFontManager* manager, TTCompositeFont* font, TTFontRenderParams* renderParams){
    TTFMCacheElement key;
    TTFMCacheElement* result;
    key.font = font;
    key.renderParams = *renderParams;

    // FIXME!!
    //    Update usage info here!
    //return gHashFindValue(manager->glyphCache, &key);
    if (stashFindPointer(manager->glyphCache, &key, &result))
        return result;
    return NULL;
}

int valueCapAndBottom(int value, int cap, int bottom){
    assert(cap >= bottom);
    if(value > cap)
        value = cap;
    else if(value < bottom)
        value = bottom;

    return value;
}

static char* ttFontBuildFilenameWithSuffix(char* fontFilename, char* suffix)
{
    char* extension;
    static char newFilename[MAX_PATH];

    if (strStartsWith(fontFilename, "fonts/"))
        fontFilename+=strlen("fonts/");

    // Do we really need to do anything with the filename?
    if(!suffix || suffix[0] == '\0')
        return fontFilename;

    // Were we given a good filename?
    extension = strrchr(fontFilename, '.');
    if(!extension)
        return NULL;

    // Copy everything up to the file extension.
    strncpy(newFilename, fontFilename, extension - fontFilename);
    newFilename[extension - fontFilename] = '\0';

    // Add suffix.
    strcat(newFilename, suffix);

    // Add extension.
    strcat(newFilename, extension);
    return newFilename;
}

// Using the filename of the primary font, try to initialize a substitue with the given file suffix.
void ttFontLoadSubstitute(TTSimpleFont* primaryFont, TTFontSubstitue* substitute, char* fileSuffix)
{
    char* fontFilename;

    assert(primaryFont && substitute && fileSuffix);
    if(!primaryFont || !substitute || !fileSuffix)
        return;

    if(!primaryFont->filename)
    {
        substitute->state = TTFSS_FILE_NOT_PRESENT;
        return;
    }

    fontFilename = ttFontBuildFilenameWithSuffix(primaryFont->filename, fileSuffix);
    substitute->font = ttFontLoadCached(fontFilename, 1);
    if(substitute->font)
    {
        substitute->state = TTFSS_LOADED;    
    }
    else
    {
        substitute->state = TTFSS_FILE_NOT_PRESENT;
    }
}

TTFMCacheElement* ttFMGetCachedTexture(TTCompositeFont* compositeFont, TTFontRenderParams* renderParams){
    TTFontManager* manager;
    TTFMCacheElement* element = NULL;

    if (!compositeFont)
        return 0;
    manager = compositeFont->manager;
    EnterCriticalSection(&CriticalSectionFontManager);

    renderParams->font = compositeFont;
    element = ttFMGetTexture(manager, compositeFont, renderParams);

    // If such a glyph texture does not already exist, generate one now.
    if(!element){
        TTFontBitmap* bitmap;
        FT_Face face;
        TTSimpleFont* simpleFont;
    
        simpleFont = ttFontGetFontForCharacter(compositeFont, renderParams->unicodeCharacter);
        if(!simpleFont) {
            LeaveCriticalSection(&CriticalSectionFontManager);
            return NULL;
        }
        if(renderParams->bold && renderParams->italicize)
        {
            if (simpleFont->boldItalics.state == TTFSS_NONE)
                ttFontLoadSubstitute(simpleFont, &simpleFont->boldItalics, "bi");
        }
        else if(renderParams->bold)
        {
            if (simpleFont->bold.state == TTFSS_NONE)
                // Try to load a bold version of the font
                ttFontLoadSubstitute(simpleFont, &simpleFont->bold, "bd");
        }
        else if(renderParams->italicize)
        {
            if (simpleFont->italics.state == TTFSS_NONE)
                // Try to load an italics version of the font
                ttFontLoadSubstitute(simpleFont, &simpleFont->italics, "i");
        }
        

        face = ttFontRenderGlyphEx(simpleFont, renderParams);                // Ask the freetype library to render a glyph
        if(!face) {
            LeaveCriticalSection(&CriticalSectionFontManager);
            return NULL;
        }
        bitmap = ttFontGenerateBitmap(face);        // Generate a bitmap openGL can understand.

        // If a bold version of the specified font cannot be found, then try to generate
        // a bold version manually.
        if(renderParams->bold && !renderParams->italicize && simpleFont->bold.state == TTFSS_FILE_NOT_PRESENT ||
            renderParams->bold && renderParams->italicize && simpleFont->boldItalics.state == TTFSS_FILE_NOT_PRESENT)
        {
            ttFontBitmapMakeBold(&bitmap, renderParams);
        }

        if(renderParams->outline){
            ttFontBitmapAddOutline(&bitmap, renderParams);
        }

        if(renderParams->dropShadow){
            renderParams->dropShadowXOffset = valueCapAndBottom(renderParams->dropShadowXOffset, 4, 0);
            renderParams->dropShadowYOffset = valueCapAndBottom(renderParams->dropShadowYOffset, 4, 0);
            ttFontBitmapAddDropShadow(&bitmap, renderParams);
        }

        // If the font is not capable of generating the given glyph, ignore the character.
        if(!bitmap) {
            LeaveCriticalSection(&CriticalSectionFontManager);
            return NULL;
        }

        // Send openGL the texture and add it to a hash table for lookup later.
        element = ttFMAddTexture(manager, compositeFont, renderParams, bitmap);

        renderParams->font = NULL;
        if (renderParams->renderToBuffer) {
            // No freeing!
        } else {
            destroyTTFontBitmap(bitmap);
        }
    }

    LeaveCriticalSection(&CriticalSectionFontManager);
    return element;
}

void ttFMClearCache(TTFontManager* manager){
    stashTableDestroyEx(manager->glyphCache, NULL, destroyTTFMCacheElementAdapter);
    manager->glyphCache = stashTableCreate(128, StashDefault, StashKeyTypeFixedSize, sizeof(TTFontRenderParams));
}

TTFMCacheElement* ttFMAddTexture(TTFontManager* manager, TTCompositeFont* font, TTFontRenderParams* renderParams, TTFontBitmap* bitmap){
    TTFMCacheElement* element;

    // Create the cache element.
    // Fill in the proper fields so that the element can be its own key.
    element = createTTFMCacheElement();
    element->font = font;
    element->renderParams = *renderParams;
    element->bitmapInfo = bitmap->bitmapInfo;

    if (element->renderParams.renderToBuffer)
    {
        int i, j;
        // Convert to RGBA instead of LumAlpha
        assert(!bitmap->bitmapRGBA);
        bitmap->bitmapRGBA = calloc(bitmap->bitmapInfo.width*bitmap->bitmapInfo.height*4, 1);
        for (j=0; j<bitmap->bitmapInfo.height; j++) {
            Color *dst = (Color*)&bitmap->bitmapRGBA[j*bitmap->bitmapInfo.width*4];
            LumAlphaPixel *src = (LumAlphaPixel*)&bitmap->bitmap[j*bitmap->bitmapInfo.width];
            for (i=0; i<bitmap->bitmapInfo.width; i++, dst++, src++) {
                dst->a = src->alpha;
                dst->r = dst->g = dst->b = src->luminance;
            }
        }

        SAFE_FREE(bitmap->bitmap);
        element->bitmap = bitmap;
    }
    else
    {
        if (bitmap->bitmapInfo.width == 0 || bitmap->bitmapInfo.height == 0)
            element->tex = 0;
        else
            element->tex = atlasGenTextureEx((U8 *)bitmap->bitmap, bitmap->bitmapInfo.width, bitmap->bitmapInfo.height, PIX_LA, "ttChar", element);
        element->bitmap = NULL;
    }
    stashAddPointer(manager->glyphCache, element, element, false);

    return element;
}

