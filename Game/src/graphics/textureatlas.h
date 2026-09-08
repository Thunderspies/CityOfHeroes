#ifndef _TEXTUREATLAS_H
#define _TEXTUREATLAS_H

#include <utilitieslib/stdtypes.h>

typedef struct PBuffer PBuffer;
typedef struct BasicTexture BasicTexture;
typedef struct TTFMCacheElement TTFMCacheElement;


// a texture that is part of an atlas
typedef struct AtlasTex
{
    int skin;
    int width, height;
    char *name;
    int ignoreSkin;

    struct AtlasTexInternal *data;
} AtlasTex;


typedef enum
{
    // pixel type
    PIX_LA = 1,
    PIX_RGBA = 2,
    PIX_MASK = 3,
    // bit flags
    PIX_DONTATLAS = 1<<3,
    PIX_NO_VFLIP = 1<<4,
} PixelType;


// this needs to be called at the beginning of each frame
void atlasDoFrame(void);

void atlasFreeAll(void);

void atlasDisplayStats(void);

AtlasTex *atlasLoadTextureEx(const char *sprite_name, int dontUseSkin);
AtlasTex *atlasLoadTexture(const char *sprite_name);

AtlasTex *atlasGenTextureEx(U8 *src_bitmap, U32 width, U32 height, PixelType pixel_type, char *name, TTFMCacheElement *font_element);
static INLINEDBG AtlasTex *atlasGenTexture(U8 *src_bitmap, U32 width, U32 height, PixelType pixel_type, char *name) { return atlasGenTextureEx(src_bitmap, width, height, pixel_type, name, 0); }
AtlasTex *atlasGenTextureFromBasic(BasicTexture *basic_texture, bool flipX, bool flipY);

void atlasUpdateTexture(AtlasTex *tex, U8 *src_bitmap, int srcWidth, int srcHeight, PixelType pixel_type);
int atlasUpdateTexturePbuffer(Vec2 ul, Vec2 lr, AtlasTex *tex, PBuffer *pbuffer);    //returns positive on actual update, 0 on no change, negative on failure
void atlasUpdateTextureFromScreen(AtlasTex *tex, U32 x, U32 y, U32 width, U32 height);


void atlasGetModifiedUVs(AtlasTex *tex, float u, float v, float *uprime, float *vprime);
int atlasDemandLoadTexture(AtlasTex *tex);


extern AtlasTex *white_tex_atlas;
void atlasMakeWhite(void);
void reskinTextures(void);

bool isDoingAtlasTex(void);

#endif //_TEXTUREATLAS_H

