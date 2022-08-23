#ifndef _TEXTUREATLASPRIVATE_H
#define _TEXTUREATLASPRIVATE_H

#include <utilitieslib/stdtypes.h>
#include "render/tex.h"
#include "render/tex_gen.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/components/MemoryPool.h>
#include "graphics/truetype/ttFontManager.h"
#include <utilitieslib/components/EString.h>
#include <windows.h>

#define BORDER_1 1
#define BORDER_2 2

#define MAX_ATLAS_SIZE 128
#define MIN_ATLAS_ROWS 8
#define MAX_ATLAS_ROWS 16
#define MAX_SLOT_HEIGHT 32

// slot height    atlas size    num rows
// 4            64            16
// 8            128            16
// 16            128            8
// 32            256            8

typedef enum
{
    ATS_UNKNOWN = 0,
    ATS_HAVE_FILENAME,
    ATS_HAVE_PIXDATA,
    ATS_ATLASED,
    ATS_BTEXED,
} AtlasTexState;

typedef struct AtlasTexFontParam
{
    unsigned short renderSize;
    unsigned int italicize    : 1;
    unsigned int bold        : 1;
    unsigned int outline    : 1;
    unsigned int dropShadow    : 1;
    unsigned int softShadow : 1;
    unsigned char outlineWidth;
    unsigned char dropShadowXOffset;
    unsigned char dropShadowYOffset;
    unsigned char softShadowSpread;
    unsigned int face;
    struct TTCompositeFont* font;
} AtlasTexFontParam;

typedef struct AtlasTexInternal
{
    int priority;

    TTFMCacheElement *font_element;

    AtlasTexState state;

    float u_mult;
    float v_mult;
    float u_offset;
    float v_offset;

    U16 actual_width;
    U16 actual_height;

    U32 is_gentex : 1;

    union
    {
        // ATS_HAVE_PIXDATA
        struct
        {
            int pixel_format;
            U8 *image;
        } pixdata;
        
        // ATS_BTEXED
        BasicTexture *basic_texture;
        
        // ATS_ATLASED
        struct TextureAtlas *parent_atlas;
    };

} AtlasTexInternal;

typedef struct TextureAtlasKey
{
    AtlasTexFontParam font_param;
    int pixel_format;
    U32 cell_size;            // power of two.
    U32 atlas_index;

} TextureAtlasKey;

typedef struct TextureAtlasRow
{
    AtlasTex **child_textures;
    U32 num_used;

} TextureAtlasRow;

typedef struct TextureAtlas
{
    // Keep the key first in the struct!
    TextureAtlasKey key;

    BasicTexture *texture;
    TextureAtlasRow *rows;

    U32 cells_per_side;
    U32 atlas_size;                // = cells_per_side * cell_size.  Atlases are square.

} TextureAtlas;

////////////////////////////////////////////////////////////////////////////

MP_DEFINE(AtlasTexInternal);
MP_DEFINE(AtlasTex);
MP_DEFINE(TextureAtlas);

////////////////////////////////////////////////////////////////////////////

U8 *borderize(U8 *src, int dst_width, int dst_height, int src_width, int src_height, int bpp)
{
    int y;
    U8 *buffer;
    U8 *dst;

    int src_line = src_width * bpp;
    int dst_line = dst_width * bpp;
    int dst_line_minus = dst_line - bpp;

    assert(dst_width <= src_width && dst_height <= src_height);

    dst = buffer = (U8*)malloc((dst_width+BORDER_2) * (dst_height+BORDER_2) * bpp);

    // first line
    memcpy(dst, src, bpp);
    dst += bpp;

    memcpy(dst, src, dst_line);
    dst += dst_line;

    memcpy(dst, src + dst_line_minus, bpp);
    dst += bpp;


    for (y = 0; y < dst_height; y++)
    {
        memcpy(dst, src, bpp);
        dst += bpp;

        memcpy(dst, src, dst_line);
        dst += dst_line;

        memcpy(dst, src + dst_line_minus, bpp);
        dst += bpp;

        src += src_line;
    }

    // last line
    src -= src_line;

    memcpy(dst, src, bpp);
    dst += bpp;

    memcpy(dst, src, dst_line);
    dst += dst_line;

    memcpy(dst, src + dst_line_minus, bpp);
    dst += bpp;


    return buffer;
}

////////////////////////////////////////////////////////////////////////////

static INLINEDBG void atlasTexCopyPixData(AtlasTex *tex, U8 *bitmap, int src_width, int src_height, PixelType pixel_type)
{
    AtlasTexInternal *data = tex->data;

    assert(src_height > 0 && src_width > 0);

    if ((pixel_type & PIX_MASK) == PIX_LA)
    {
        data->pixdata.pixel_format = GL_LUMINANCE_ALPHA;
        data->pixdata.image = borderize(bitmap, data->actual_width, data->actual_height, src_width, src_height, 2);
    }
    else if ((pixel_type & PIX_MASK) == PIX_RGBA)
    {
        data->pixdata.pixel_format = GL_RGBA;
        data->pixdata.image = borderize(bitmap, data->actual_width, data->actual_height, src_width, src_height, 4);
    }
    else
        assertmsg(0, "Bad value for pixel_type!");

    data->state = ATS_HAVE_PIXDATA;
}

static INLINEDBG void atlasTexClearPixData(AtlasTex *tex)
{
    if (tex->data->pixdata.image)
        free(tex->data->pixdata.image);
    tex->data->pixdata.image = 0;
    tex->data->state = ATS_UNKNOWN;
}

extern bool g_is_doing_atlas_tex;

void atlasTexToBasicTexture(AtlasTex *tex, const char * name)
{
    AtlasTexInternal *data = tex->data;

    g_is_doing_atlas_tex = true;

    assert(data->state == ATS_HAVE_FILENAME);

    data->basic_texture = texLoadBasic(name, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UI);

    if (!data->basic_texture)
        data->basic_texture = white_tex;

    tex->width = data->actual_width = data->basic_texture->actualTexture->width;
    tex->height = data->actual_height = data->basic_texture->actualTexture->height;

    data->state = ATS_BTEXED;

    data->u_mult = ((float)data->actual_width) / ((float)data->basic_texture->actualTexture->realWidth);
    data->v_mult = ((float)data->actual_height) / ((float)data->basic_texture->actualTexture->realHeight);
    data->u_offset = 0;
    data->v_offset = 0;

    g_is_doing_atlas_tex = false;
}

static INLINEDBG void atlasTexToBasicTextureFromPixData(AtlasTex *tex, U8 *bitmap, int src_width, int src_height, PixelType pixel_type)
{
    AtlasTexInternal *data = tex->data;
    int pixel_format, bpp;
    int new_width = 1 << log2i(src_width);
    int new_height = 1 << log2i(src_height);
    BasicTexture *btex = texGenNew(new_width, new_height, tex->name);
    U8* buffer = NULL;

    if ((pixel_type & PIX_MASK) == PIX_LA)
    {
        bpp = 2;
        pixel_format = GL_LUMINANCE_ALPHA;
    }
    else if ((pixel_type & PIX_MASK) == PIX_RGBA)
    {
        bpp = 4;
        pixel_format = GL_RGBA;
    }
    else
        assertmsg(0, "Bad value for pixel_type!");

    // resize the image
    if (bitmap && (new_width != src_width || new_height != src_height))
    {
        int total_size = new_width * new_height * bpp;
        int src_line = src_width * bpp;
        int dst_line = new_width * bpp;
        int y;
        U8 *src, *dst;

        estrStackCreate((char**)(&buffer), total_size);
        estrSetLengthNoMemset((char**)(&buffer), total_size);
        dst = buffer;
        memset(buffer, 0, total_size);

        src = bitmap;

        // line by line copy
        for (y = 0; y < src_height; y++)
        {
            memcpy(dst, src, src_line);
            dst += dst_line;
            src += src_line;
        }
    }

    if (buffer)
        texGenUpdateEx(btex, buffer, pixel_format, 0);

    btex->width = src_width;
    btex->height = src_height;

    data->basic_texture = btex;

    data->state = ATS_BTEXED;

    data->u_mult = ((float)src_width) / ((float)btex->realWidth);
    data->v_mult = ((float)src_height) / ((float)btex->realHeight);
    data->u_offset = 0;
    data->v_offset = 0;
    if (pixel_type & PIX_NO_VFLIP)
    {
        data->v_offset = data->v_mult;
        data->v_mult *= -1;
    }
    estrDestroy((char**)(&buffer));
}

static INLINEDBG AtlasTex *allocAtlasTex(void)
{
    AtlasTex *tex;

    MP_CREATE(AtlasTex, 500);
    MP_CREATE(AtlasTexInternal, 500);

    tex = MP_ALLOC(AtlasTex);
    tex->data = MP_ALLOC(AtlasTexInternal);

    return tex;
}

static INLINEDBG void freeAtlasTex(AtlasTex *tex)
{
    if (tex->data->state == ATS_HAVE_PIXDATA)
        atlasTexClearPixData(tex);

    if (tex->name)
        free(tex->name);

    MP_FREE(AtlasTexInternal, tex->data);
    MP_FREE(AtlasTex, tex);
}

////////////////////////////////////////////////////////////////////////////

static INLINEDBG TextureAtlas *allocTextureAtlas(TextureAtlasKey *key)
{
    TextureAtlas *atlas;
    U32 y;
    U8 *bitmap;
    int bitmap_size;

    MP_CREATE(TextureAtlas, 200);

    atlas = MP_ALLOC(TextureAtlas);

    atlas->cells_per_side = MAX_ATLAS_SIZE / key->cell_size;
    if (atlas->cells_per_side > MAX_ATLAS_ROWS)
        atlas->cells_per_side = MAX_ATLAS_ROWS;
    else if (atlas->cells_per_side < MIN_ATLAS_ROWS)
        atlas->cells_per_side = MIN_ATLAS_ROWS;

    atlas->atlas_size = atlas->cells_per_side * key->cell_size;

    atlas->rows = (TextureAtlasRow*)calloc(atlas->cells_per_side, sizeof(*atlas->rows));

    for (y = 0; y < atlas->cells_per_side; y++)
    {
        atlas->rows[y].child_textures = (AtlasTex**)calloc(atlas->cells_per_side, sizeof(*atlas->rows[y].child_textures));
        atlas->rows[y].num_used = 0;
    }

    atlas->key = *key;

    atlas->texture = texGenNew(atlas->atlas_size, atlas->atlas_size, "TextureAtlas");

//    assert(atlas->atlas_size <= 256);
    bitmap_size = getImageByteCount(key->pixel_format, atlas->atlas_size, atlas->atlas_size);

//    assert(bitmap_size <= 256*256*4);
    bitmap = (U8*)malloc(bitmap_size);
    memset(bitmap, 0, bitmap_size);
    texGenUpdateEx(atlas->texture, bitmap, key->pixel_format, 0);
    free(bitmap);

    return atlas;
}

static INLINEDBG void freeTextureAtlas(TextureAtlas *atlas)
{
    U32 y;

    // TODO ensure rows are clear

    for (y = 0; y < atlas->cells_per_side; y++)
        free(atlas->rows[y].child_textures);

    texGenFree(atlas->texture);
    free(atlas->rows);
    MP_FREE(TextureAtlas, atlas);
}

#endif

