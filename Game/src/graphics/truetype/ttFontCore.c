#include "graphics/truetype/ttFontCore.h"
#include "ft2build.h"
#include <freetype/freetype.h>
#include <freetype/ftmodapi.h>

#include <utilitieslib/utils/file.h>    // for fileOpen()
#include <utilitieslib/assert/assert.h>

#include "graphics/truetype/ttAppleMapping.h"
#include "graphics/truetype/ttFontBitmap.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/memlog.h>
#include "cmdparse/cmdgame.h"

#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/timing.h>

//------------------------------------------------------------
// Internal globals
//------------------------------------------------------------
static unsigned int ttStarted;
static FT_Library  library;
static struct FT_MemoryRec_ ftMemoryFuncs;

int ttFontLoad(TTSimpleFont* font, const char* filename, const char* facename);

//------------------------------------------------------------
// Timed memory allocation functions.
//------------------------------------------------------------


static void* cryptic_ft_alloc(FT_Memory ftMemory, long size)
{
    return malloc(size);
}

static void cryptic_ft_free(FT_Memory ftMemory, void* memory)
{
    free(memory);
}

static void* cryptic_ft_realloc(FT_Memory ftMemory, long cur_size, long new_size, void* memory)
{
    return realloc(memory, new_size);
}

//------------------------------------------------------------
// Module initialization
//------------------------------------------------------------
int ttStartup(){
    int error;

    // If the module has already been started, don't do anything.
    if(ttStarted)
        return 1;
        
    ftMemoryFuncs.alloc =    cryptic_ft_alloc;
    ftMemoryFuncs.free =    cryptic_ft_free;
    ftMemoryFuncs.realloc = cryptic_ft_realloc;

    // Otherwise, initialize the freetype library.
    if(!library){
        #if 1
            // MS: Switched to this so we could track the memory allocations properly.
            //     http://freetype.sourceforge.net/freetype2/docs/design/design-4.html
            
            error = FT_New_Library(&ftMemoryFuncs, &library);
            
            if(!error)
            {
                FT_Add_Default_Modules( library );
            }
        #else
            error = FT_Init_FreeType(&library);
        #endif

        if(error){
            printf("Error starting the freetype library\n");
            return 0;
        }
    }

    ttStarted = 1;
    return 1;
}


void ttShutdown(){
    int error;

    if(!ttStarted)
        return;

    if(library){
        error = FT_Done_FreeType(library);
        library = 0;
    }
}


//-----------------------------------------------------------------------------------
// TTFont creation + destruction
//-----------------------------------------------------------------------------------
TTSimpleFont* createTTSimpleFont(){
    return calloc(1, sizeof(TTSimpleFont));
}

void destroyTTSimpleFont(TTSimpleFont* font){
    if(font){
        // todo
        if (font->bold.font)
            ;
        if (font->italics.font)
            ;
        if (font->boldItalics.font)
            ;

        FT_Done_Face(font->font);
        SAFE_FREE(font->filename);
        SAFE_FREE(font->stream);
        destroyArray(font->excludeCharacters);
        free(font);
    }
}
TTCompositeFont* createTTCompositeFont(){
    return calloc(1, sizeof(TTCompositeFont));
}

void destroyTTCompositeFont(TTCompositeFont* font){
    eaDestroy(&font->fallbackFonts);
    SAFE_FREE(font);
}


//-----------------------------------------------------------------------------------
// TTFont loading + configuration
//-----------------------------------------------------------------------------------
/* Function ttFontLoad()
 *    Creates a bitmap using the given font filename.
 */
TTSimpleFont* ttFontLoadCreate(const char* filename, const char *facename){
    TTSimpleFont* font = NULL;

    memlog_printf(0, "%u: ttFontLoadCreate %s", game_state.client_frame_timestamp, filename);

    font = createTTSimpleFont();
    if(!ttFontLoad(font, filename,facename)){
        destroyTTSimpleFont(font);
        font = NULL;
    }

    return font;
}

TTFontCacheElement **ttFontCache=NULL;
static CRITICAL_SECTION CriticalSectionFontCache;
static bool CriticalSectionFontCache_inited=false;

void ttFontCacheDump(void)
{
    int num, i;
    EnterCriticalSection(&CriticalSectionFontCache);
    num = eaSize(&ttFontCache);
    for (i=0; i<num; i++) {
        printf("Font: %20s, refs: %d, loaded: %s\n", 
            ttFontCache[i]->name, ttFontCache[i]->referenceCount, ttFontCache[i]->font?"YES":"NO");
    }
    LeaveCriticalSection(&CriticalSectionFontCache);
}

void ttFontUnloadCached(const char *fontName)
{
    int num, i;
    EnterCriticalSection(&CriticalSectionFontCache);
    num = eaSize(&ttFontCache);
    for (i=0; i<num; i++) {
        if (stricmp(ttFontCache[i]->name, fontName) == 0 || game_state.quickLoadFonts) {
            assert(ttFontCache[i]->referenceCount > 0);
            ttFontCache[i]->referenceCount--;
            if (ttFontCache[i]->referenceCount==0) {
                // unload
                destroyTTSimpleFont(ttFontCache[i]->font);
                ttFontCache[i]->font = NULL;
            }
            LeaveCriticalSection(&CriticalSectionFontCache);
            return;
        }
    }
    assert(0);
    LeaveCriticalSection(&CriticalSectionFontCache);
}

TTSimpleFont* ttFontLoadCached(const char *filename, int load){
    return ttFontLoadCachedFace(filename,load,NULL);
}

TTSimpleFont* ttFontLoadCachedFace(const char *filename, int load, const char *facename){
    char path[MAX_PATH];
    TTFontCacheElement *elem;
    int num, i;

    sprintf(path, "fonts/%s", filename);
    if (!fileExists(path))
        return NULL;

    if (!CriticalSectionFontCache_inited) {
        InitializeCriticalSection(&CriticalSectionFontCache);
        CriticalSectionFontCache_inited = true;
    }
    EnterCriticalSection(&CriticalSectionFontCache);
    if (load)
        memlog_printf(0, "%u: ttFontLoadCached %s", game_state.client_frame_timestamp, filename);

    
    num = eaSize(&ttFontCache);
    for (i=0; i<num; i++) {
        if (stricmp(filename, ttFontCache[i]->name)==0 || game_state.quickLoadFonts) {
            elem = ttFontCache[i];
            if (load && !elem->font) {
                elem->font = ttFontLoadCreate(path,facename);
            }
            if (load)
                elem->referenceCount++;
            LeaveCriticalSection(&CriticalSectionFontCache);
            return elem->font;
        }
    }
    elem = calloc(sizeof(TTFontCacheElement), 1);
    elem->name = strdup(filename);
    if (load) {
        elem->referenceCount++;
        elem->font = ttFontLoadCreate(path,facename);
    }
    eaPush(&ttFontCache, elem);

    LeaveCriticalSection(&CriticalSectionFontCache);

    return elem->font;
}

static unsigned long fileWrapperIO(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count)
{
    FILE* file;

    if(0 == count)
        return 0;

    file = stream->descriptor.pointer;
    fseek( file, offset, SEEK_SET );
    return (unsigned long)fread( buffer, 1, count, file );
}

static void fileWrapperClose(FT_Stream stream)
{
    FILE* file;
    file = stream->descriptor.pointer;
    fclose(file);
}

#undef __FTERRORS_H__                                           
#define FT_ERRORDEF( e, v, s )  { e, s },                       
#define FT_ERROR_START_LIST     {                               
#define FT_ERROR_END_LIST       { 0, 0 } };                     

const struct                                                    
{                                                               
    int          err_code;
    const char*  err_msg;
} ft_errors[] = 

#include FT_ERRORS_H 

static int dontConfuseWorkspaceWhiz[] = {1,2}; // this stops the previous 2 lines from confusing WWhiz's parser

int ttFontLoad(TTSimpleFont* font, const char* filename, const char* facename){
    int error;
    FT_Face    face;
    float fontLoadTime;
    int startTime;
    FT_Open_Args streamArg = {0};
    FILE* fontFile;

    CRITICAL_SECTION CriticalSectionFontLoad;
    bool CriticalSectionFontLoad_inited=false;
    if (!CriticalSectionFontLoad_inited) {
        InitializeCriticalSection(&CriticalSectionFontLoad);
        CriticalSectionFontLoad_inited = true;
    }
    EnterCriticalSection(&CriticalSectionFontLoad);

    // Make sure the module has been started.
    ttStartup();


    startTime = timerCpuTicks();

    fontFile = fileOpen(filename, "rb");
    if(!fontFile)
    {
        LeaveCriticalSection(&CriticalSectionFontLoad);
        return 0;
    }

    font->filename = strdup(filename);

    streamArg.flags = ft_open_stream;
    streamArg.stream = calloc(1, sizeof(struct FT_StreamRec_));
    streamArg.stream->descriptor.pointer = fontFile;
    streamArg.stream->read = fileWrapperIO;
    streamArg.stream->close = fileWrapperClose;
    streamArg.stream->size = fileGetSize(fontFile);

    font->stream = streamArg.stream;
    if (facename) {
        int i = 0, max = 1;
        while (i < max){
            error = FT_Open_Face(library, &streamArg, i, &face);
            if(error){
                printf("Error loading the font collection '%s'\n", filename);
                LeaveCriticalSection(&CriticalSectionFontLoad);
                return 0;
            }
            if (stricmp(face->family_name,facename) == 0)
                break; //found the correct face
            max = face->num_faces;
            i++;
        }
        if (i == max) {//didnt find face
            printf("Error loading the font '%s' from collection '%s'\n", facename, filename);
            LeaveCriticalSection(&CriticalSectionFontLoad);
            return 0;
        }
    }
    else {
        error = FT_Open_Face(library, &streamArg, 0, &face);
    }
    font->font = face;

    fontLoadTime = timerSeconds(timerCpuTicks() - startTime);
    if(error){
        printf("Error loading the font '%s'\n", filename);
        LeaveCriticalSection(&CriticalSectionFontLoad);
        return 0;
    }

    if(!face->charmap){
        // The freetype library should have tried to load a unicode character map
        // by default.
        // If a character map doesn't exist after the face is loaded, it means that
        // it was not able to find a unicode map.

        // Try to load an "Apple Roman" encoding map.
        error = FT_Select_Charmap(face, ft_encoding_apple_roman);
        if(error){
            // This point should never be reached.  It means that there isn't a unicode
            // nor an "Apple Roman" character map in the font.  
            // The library will need to learn to deal with these types of fonts we encounter them.
            assert(0);
        }
    }

#if 0
    printf("The font %s has:\n", filename);
    printf("\t%i glyphs\n", face->num_glyphs);
    printf("\tloaded in %f seconds\n", fontLoadTime);
#endif
    LeaveCriticalSection(&CriticalSectionFontLoad);
    return 1;
}

/* Function ttFontAddFallback()
*    Each font can be either a single font or a composite font.  A single font corresponds to a single font file.
*    However, given the number of characters Unicode can represent, it is unlikely that the font will contain
*    glyphs for all possible characters.  To make glyph bitmap generation easier, each font can also become a
*    composite font.
*
*    When a single font is turned into a composite font, the original (single) font becomes the primary
*    font while the additional fonts becomes the fallback fonts.  All this means is that if a glyph bitmap generation
*    request cannot be fulfilled using the primary font, the bitmap generation function will use the fallback
*    fonts in the order that they are added to attempt to generate a bitmap.
*
*    Parameters:
*        font - the primary font
*        fallbackFont - the fallback font to add to the primary font
*/
void ttFontAddFallback(TTCompositeFont* font, TTSimpleFont* fallbackFont){
    if(!fallbackFont)
        return;

    eaPush(&font->fallbackFonts, fallbackFont);
}



/* Function ttFontGetSubFont()
*    This maps the primary font and the fallback fonts into a single space, which
*    allows retrieval of all fonts in their order of importance.
*
*    Returns:
*        valid TTSimpleFont - The font specified by the index.
*        NULL - The index does not specify a valid font.
*/
TTSimpleFont* ttFontGetSubfont(TTCompositeFont* font, unsigned int index)
{
    return eaGet(&font->fallbackFonts, index);
}

// Given a compsite font, return the subfont that can properly display the given unicode character.
TTSimpleFont* ttFontGetFontForCharacter(TTCompositeFont* compositeFont, unsigned short unicodeCharacter){
    unsigned int subFontIndex = 0;

    // Generate the glyph bitmap using the freetype library.
    while(1){
        unsigned short character;
        unsigned int glyphIndex;
        TTSimpleFont* subFont;
        FT_Face face;

        subFont = ttFontGetSubfont(compositeFont, subFontIndex++);
        if(!subFont)
            return 0;

        face = subFont->font;

        // If we're using a font that only has the "Apple Roman" encoding character map,
        // be sure to translate the character code.
        if(face->charmap->encoding == ft_encoding_apple_roman){
            character = unicodeToApple(unicodeCharacter);
            if(character == 0)
                continue;
        }else
            character = unicodeCharacter;

        // Block out any unwanted characters.
        // FIXME!!!
        // consider using a hashtable if the size is too large.
        if(subFont->excludeCharacters){
            int i;

            // Compare the character being processed to the excluded character list...
            for(i = 0; i < subFont->excludeCharacters->size; i++){
                // If there are any matches, this sub-font cannot be used to display this
                // glyph.  Move on the the next sub-font.
                if(unicodeCharacter == (unsigned short)subFont->excludeCharacters->storage[i])
                    break;
            }

            if(i != subFont->excludeCharacters->size)
                continue;
        }

        glyphIndex = FT_Get_Char_Index(face, character);

        // Can this font be used to render this character?
        if(glyphIndex == 0){
            // If not, continue trying other fallback fonts.
            continue;
        }

        return subFont;
    }
}

//-----------------------------------------------------------------------------------
// TTFont rendering
//-----------------------------------------------------------------------------------
/* Function ttFontRenderGlyph()
 *    This function takes a composite font and attempts to generate glyph specified by the unicode character.
 *
 *    When the function succeeds, it returns the FT_Face that succeeded in rendering the glyph.  The renderred
 *    results are contains in the returned FT_Face.
 */
FT_Face ttFontRenderGlyphEx(TTSimpleFont* simpleFont, TTFontRenderParams* params)
{
    int error;
    int glyphIndex;
    FT_Face face = NULL;

    // Do we have a freetype font structure to perform rendering with?
    assert(simpleFont->font);
    if(!simpleFont->font)
        return NULL;

    // Which freetype font are we going to use?
    if(    params->bold && params->italicize)
    {
        if (simpleFont->boldItalics.state == TTFSS_LOADED) {
            assert(simpleFont->boldItalics.font);
            face = simpleFont->boldItalics.font->font;
        }
    } 
    else if(params->bold)
    {
        if (simpleFont->bold.state == TTFSS_LOADED) {
            assert(simpleFont->bold.font);
            face = simpleFont->bold.font->font;
        }
    }
    else if(params->italicize)
    {
        if (simpleFont->italics.state == TTFSS_LOADED) {
            assert(simpleFont->italics.font);
            face = simpleFont->italics.font->font;
        }
    }

    if(NULL == face)
        face = simpleFont->font;

    // Can this font render the requested unicode character?
    glyphIndex = FT_Get_Char_Index(face, params->unicodeCharacter);
    if(0 == glyphIndex)
        return NULL;

    // Generate the glyph bitmap using the freetype library.
    error = FT_Set_Pixel_Sizes(
        face,   /* handle to face object            */
        params->defaultSize,    /* pixel_width                      */
        params->defaultSize);    /* pixel_height                     */
    if(error){
        printf("Error setting glyph size\n");
    }

    {
        FT_Matrix matrix;
        FT_Vector pen;
        float scaleFactor;
        pen.x = 0;
        pen.y = 0;


        scaleFactor = ((float)params->renderSize) / params->defaultSize * 64 * 1024;
        matrix.xx = (FT_Fixed)(scaleFactor);//(1 << log2(scaleFactor));
        matrix.xy = 0;
        matrix.yx = 0;
        matrix.yy = (FT_Fixed)(scaleFactor);//(1 << log2(scaleFactor));

        // If an italicized verison of the font cannot be loaded, try to generate
        // an italiicized version manually.
        if(params->italicize && !params->bold && simpleFont->italics.state == TTFSS_FILE_NOT_PRESENT ||
            params->italicize && params->bold && simpleFont->boldItalics.state == TTFSS_FILE_NOT_PRESENT)
        {
            int skewFactor;
            skewFactor = 0.15 * scaleFactor;
            matrix.xy = (FT_Fixed)(skewFactor);
        }

        FT_Set_Transform( face, &matrix, &pen );
    }


    {
        int flag;
        switch(simpleFont->glyphGenMode)
        {
        case TTGGM_DEFAULT:
        case TTGGM_AUTOHINT:
            flag = FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT;
            break;
        case TTGGM_INTERPRETER:
            flag = FT_LOAD_DEFAULT;
            break;
        
        case TTGGM_NOHINT:
            flag = FT_LOAD_NO_HINTING;
            break;
        default:
            assert(0 && "invalid glyph generation mode");
        }
        error = FT_Load_Glyph( 
            face,                    /* handle to face object */
            glyphIndex,                /* glyph index           */
            flag );                    
    }

    if(error){
        // The glyph was not found in this font.
        // This should never happen as the glyph index was just retrieved?
        return NULL;
    }

    if(face->glyph->format != ft_glyph_format_bitmap){
        error = FT_Render_Glyph(
            face->glyph,        /* glyph slot  */
            ft_render_mode_normal );                /* render mode */

        if(error){
            //printf("Glyph rendering error\n");
            return 0;
        }
    }

    return face;
    
}

static int ttFontFaceGetPixel(unsigned char* row, int element, FT_Bitmap* ftBitmap)
{
    switch(ftBitmap->pixel_mode)
    {
    case ft_pixel_mode_mono:
        {
            return (row[element / 8] & (0x80 >> (element % 8))) ?  255 : 0;
        }
        break;
    case ft_pixel_mode_grays:
        {
            return row[element];
        }
        break;
    default:
        assert(0 && "unsupported glyph bitmap format");
        return 0;
        break;
    }
}

/* Function ttFontGenerateBitmap()
*    This function assumes that the given font has already had ttFontRenderGlyphXXX() called on it.
*    It then takes the bitmap that was generated and converts it into a TTFontBitmap, which can
*    then be sent to openGL.
*/
TTFontBitmap* ttFontGenerateBitmap(FT_Face face)
{
    TTFontBitmap* bitmap;
    int i;
    int j;
    unsigned char* srcRowBuffer;
    unsigned short* dstRowBuffer;
    int buffer_size, width, height;

    // If there isn't any bitmap data to be converted, do nothing.
    // This might happen if the font has not had ttFontRenderGlyphXXX() called on it.
    //    if((font->font)->glyph->bitmap)
    //        return NULL;


    bitmap = createTTFontBitmap();

    // Copy some bitmap information so that we know how to position
    // both this and the next character.
    bitmap->bitmapInfo.bitmapLeft = face->glyph->bitmap_left;
    bitmap->bitmapInfo.bitmapTop = face->glyph->bitmap_top;
    bitmap->bitmapInfo.advanceX = face->glyph->advance.x >> 6;

    width = bitmap->bitmapInfo.width = face->glyph->bitmap.width;
    height = bitmap->bitmapInfo.height = face->glyph->bitmap.rows;

    // Convert this to a lum + alpha image
    buffer_size = width * height * sizeof(unsigned short);

    dstRowBuffer = bitmap->bitmap = buffer_size ? calloc(1, buffer_size) : NULL;
    srcRowBuffer = face->glyph->bitmap.buffer;

    // Copy each row in the src image.
    for(i = 0; i < height; i++)
    {
        // Copy each element in the row.
        for(j = 0; j < width; j++)
        {
            LumAlphaPixel* dst = (LumAlphaPixel*)&dstRowBuffer[j];
            
            // This will be a texture with only alpha and luminance value where the entire
            // texture is white and the alpha channel matches the glyph bitmap.
            dst->alpha = ttFontFaceGetPixel(srcRowBuffer, j, &face->glyph->bitmap);            // Alpha
            dst->luminance = 255;        // Luminance
        }
                                                                                
        srcRowBuffer += face->glyph->bitmap.pitch;
        dstRowBuffer += width;
    }

    return bitmap;
}

