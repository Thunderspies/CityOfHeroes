#include "graphics/textureatlas.h"
#include <utilitieslib/components/StashTable.h>
#include "render/thread/rt_queue.h"
#include "render/tex.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include "graphics/font.h"
#include "render/sprite.h"
#include "render/tex_gen.h"
#include "render/thread/ogl.h"
#include <utilitieslib/utils/mathutil.h>
#include "textureatlasPrivate.h"
#include "render/texWords.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/qsortG.h>
#include "win/input.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/HashFunctions.h>
#include "render/pbuffer.h"
#include <utilitieslib/components/StringCache.h>

#include <windows.h>
#include <utilitieslib/utils/timing.h>
#include "auth/authUserData.h"

#define DEBUG_SPRITE_DRAW_ORDER 0

U8* uncompressRawTexInfo(TexReadInfo *rawInfo); // Uncompresses to GL_RGBA8


static StashTable named_atlas_textures = 0;
static AtlasTex **atlas_textures_array = 0;

static StashTable texture_atlases = 0;
static TextureAtlas **texture_atlases_array = 0;

AtlasTex *white_tex_atlas = 0;

static int histogram_dirty = 1;

static int cmpPriority(const void *a, const void *b)
{
    AtlasTex *texA = *((AtlasTex **)a);
    AtlasTex *texB = *((AtlasTex **)b);

    return memcmp(&texB->data->priority, &texA->data->priority, sizeof(texA->data->priority));
}

static AtlasTex *addAtlasTex(const char *sprite_name, AtlasTex * tex, int dontUseSkin);

void reskinTextures(void)
{
    StashTableIterator iHashAdjs = {0};
    StashElement helem;
    int i;

    for( i = 0, stashGetIterator( named_atlas_textures, &iHashAdjs); stashGetNextElement(&iHashAdjs, &helem); ++i ) 
    {
        AtlasTex *tex = stashElementGetPointer( helem );

        if(tex && !tex->ignoreSkin)
            addAtlasTex( tex->name, tex, 0 );
    }
}

////////////////////////////////////////////////////////////////////
// Texture Atlasing Functions
////////////////////////////////////////////////////////////////////

static void gatherAtlasTextures(TextureAtlas *atlas, AtlasTex ***tex_list)
{
    TextureAtlasRow *row;
    U32 num_cells = atlas->cells_per_side;
    U32 x, y;

    for (y = 0; y < num_cells; y++)
    {
        row = &(atlas->rows[y]);

        for (x = 0; x < num_cells; x++)
        {
            if (row->child_textures[x])
                eaPush(tex_list, row->child_textures[x]);
        }
    }
}


static int isInAtlas(AtlasTex *tex, TextureAtlas *atlas)
{
    TextureAtlasRow *row;
    U32 num_cells = atlas->cells_per_side;
    U32 x, y;

    for (y = 0; y < num_cells; y++)
    {
        row = &(atlas->rows[y]);

        for (x = 0; x < num_cells; x++)
        {
            if (row->child_textures[x] == tex)
                return 1;
        }
    }

    return 0;
}

static int addToSpecificAtlas(TextureAtlas *atlas, AtlasTex *tex)
{
    U32 x, y;
    TextureAtlasRow *row;
    AtlasTexInternal *data = tex->data;
    int x_offset, y_offset;
    double one_over_size;

    U32 num_cells = atlas->cells_per_side;
    U32 size = atlas->key.cell_size;

    if (!atlas)
        return 0;

    // look for space
    for (y = 0; y < num_cells; y++)
    {
        row = &(atlas->rows[y]);

        if (row->num_used >= num_cells)
            continue;

        for (x = 0; x < num_cells; x++)
        {
            if (!row->child_textures[x])
                break;
        }

        row->num_used++;
        row->child_textures[x] = tex;

        // space found, add to row

        x_offset = x * size;
        y_offset = y * size;

        texGenUpdateRegion(atlas->texture, data->pixdata.image, x_offset, y_offset, data->actual_width + BORDER_2, data->actual_height + BORDER_2, data->pixdata.pixel_format);

        atlasTexClearPixData(tex);

        one_over_size = 1.0f / ((float)atlas->atlas_size);
        data->u_mult = ((float)data->actual_width) * one_over_size;
        data->v_mult = ((float)data->actual_height) * one_over_size;
        data->u_offset = ((float)x_offset + BORDER_1) * one_over_size;
        data->v_offset = ((float)y_offset + BORDER_1) * one_over_size;

        data->state = ATS_ATLASED;
        data->parent_atlas = atlas;

        histogram_dirty = 1;

        return 1;
    }

    return 0;
}

static void addToAtlas(AtlasTex *tex)
{
    TextureAtlasKey key;
    TextureAtlas *atlas;
    TextureAtlas* pTempAtlas;
    U32 width, height, rendersize;
    TTFontRenderParams *renderParams = tex->data->font_element ? &tex->data->font_element->renderParams : 0;

    ZeroStruct(&key);

    if (renderParams)
    {
        key.font_param.bold = renderParams->bold;
        key.font_param.dropShadow = renderParams->dropShadow;
        key.font_param.dropShadowXOffset = renderParams->dropShadowXOffset;
        key.font_param.dropShadowYOffset = renderParams->dropShadowYOffset;
        key.font_param.face = renderParams->face;
        key.font_param.font = renderParams->font;
        key.font_param.italicize = renderParams->italicize;
        key.font_param.outline = renderParams->outline;
        key.font_param.outlineWidth = renderParams->outlineWidth;
        key.font_param.renderSize = renderParams->renderSize;
        key.font_param.softShadow = renderParams->softShadow;
        key.font_param.softShadowSpread = renderParams->softShadowSpread;
    }

    key.pixel_format = tex->data->pixdata.pixel_format;

    width = 1 << log2i(tex->width + BORDER_2);
    height = 1 << log2i(tex->height + BORDER_2);
    rendersize = 1 << log2i(key.font_param.renderSize + BORDER_2);

    key.cell_size = MAX(width, height);
    if (rendersize <= MAX_SLOT_HEIGHT)
        key.cell_size = MAX(key.cell_size, rendersize);
    key.atlas_index = 0;

    // look for an atlas with available space
    while (stashFindPointer(texture_atlases, &key, &pTempAtlas))
    {
        if ( addToSpecificAtlas(pTempAtlas, tex))
            return;
        key.atlas_index++;
    }

    // no atlases found with available space, create a new one
    atlas = allocTextureAtlas(&key);
    eaPush(&texture_atlases_array, atlas);
    stashAddPointer(texture_atlases, &atlas->key, atlas, false);

    // add the texture to the atlas
    {
        int ret = addToSpecificAtlas(atlas, tex);
        assertmsg(ret, "Problem in texture atlasing!");
    }
}


////////////////////////////////////////////////////////////////////
// AtlasTex Helpers
////////////////////////////////////////////////////////////////////

static int texAtlasHash(const TextureAtlas* atlas, int hashSeed)
{
    return hashCalc((const char*)&atlas->key, sizeof(atlas->key), hashSeed);
}

static int texAtlasCmp(const TextureAtlas* atlas1, const TextureAtlas* atlas2)
{
    return memcmp((unsigned char*)&atlas1->key, (unsigned char*)&atlas2->key, sizeof(atlas1->key));
}

static INLINEDBG void atlasInit(void)
{
    if (named_atlas_textures)
        return;

    named_atlas_textures = stashTableCreateWithStringKeys(5000, StashDeepCopyKeys);
    eaCreate(&atlas_textures_array);

    texture_atlases = stashTableCreate(100, StashDeepCopyKeys, StashKeyTypeFixedSize, sizeof(TextureAtlasKey));
    eaCreate(&texture_atlases_array);
}

static AtlasTex *getAtlasTex(const char *sprite_name)
{
    StashElement elem;
    atlasInit();

    stashFindElement(named_atlas_textures, sprite_name, &elem);

    if (elem)
        return (AtlasTex *)stashElementGetPointer(elem);

    return 0;
}

bool g_is_doing_atlas_tex=false;
bool isDoingAtlasTex(void)
{
    return g_is_doing_atlas_tex;
}


static AtlasTex *addAtlasTex(const char *sprite_name, AtlasTex * tex, int dontUseSkin)
{

    BasicTexture *tex_header=0;
    const char * name_ptr = NULL;
    int skin = 0;
    char villain_sprite_name[1000] = {0};

    if( game_state.skin != UISKIN_HEROES)
    {
        if( game_state.skin == UISKIN_PRAETORIANS )
        {
            if (!dontUseSkin)
            {
                Strncpyt( villain_sprite_name, "p_" );
            }
            skin = 2;
        }
        else if( game_state.skin == UISKIN_VILLAINS )
        {
            if (!dontUseSkin)
            {
                Strncpyt( villain_sprite_name, "v_" );
            }
            skin = 1;
        }

        Strncatt( villain_sprite_name, sprite_name );
        tex_header = texFind(villain_sprite_name);
        name_ptr = villain_sprite_name;
    }

    if( !tex_header )
    {
        tex_header = texFind(sprite_name);
        name_ptr = sprite_name;
    }

    if (!tex_header)
        return white_tex_atlas;

    if( tex && tex->skin == skin ) // texture unchanged, no need to reload
        return tex;

    if( !tex )
    {
        tex = allocAtlasTex();

        atlasInit();

        stashAddPointer(named_atlas_textures, dontUseSkin ? name_ptr : sprite_name, tex, false);
        eaPush(&atlas_textures_array, tex);
        tex->name = (char *) allocAddString(sprite_name);
    }

    tex->width = tex->data->actual_width = tex_header->actualTexture->width;
    tex->height = tex->data->actual_height = tex_header->actualTexture->height;

    tex->data->state = ATS_HAVE_FILENAME;
    tex->skin = skin;
    tex->ignoreSkin = dontUseSkin;

    // don't atlas it if the texture is too big or if it is a texWord
    if (tex_header->texWord || (tex->height + BORDER_2) > MAX_SLOT_HEIGHT || (tex->width + BORDER_2) > MAX_SLOT_HEIGHT || game_state.dont_atlas)
        atlasTexToBasicTexture(tex, name_ptr);

    histogram_dirty = 1;

    return tex;
}

static AtlasTex *genAtlasTex(U8 *bitmap, U32 width, U32 height, PixelType pixel_type, char *name, TTFMCacheElement *font_element)
{
    AtlasTex *tex = allocAtlasTex();

    atlasInit();

    eaPush(&atlas_textures_array, tex);

    if (name && name[0])
        tex->name = (char *) allocAddString(name);
    else
        tex->name = (char *) allocAddString("AUTOGEN_ATLASTEX");

    tex->width = tex->data->actual_width = width;
    tex->height = tex->data->actual_height = height;

    tex->data->is_gentex = 1;

    tex->data->font_element = font_element;

    // don't atlas it if the texture is too big
    if ((tex->height + BORDER_2) > MAX_SLOT_HEIGHT || (tex->width + BORDER_2) > MAX_SLOT_HEIGHT || game_state.dont_atlas || (pixel_type & PIX_DONTATLAS))
        atlasTexToBasicTextureFromPixData(tex, bitmap, width, height, pixel_type);
    else
        atlasTexCopyPixData(tex, bitmap, width, height, pixel_type);

    histogram_dirty = 1;

    return tex;
}

static AtlasTex *getOrAddAtlasTex(const char *sprite_name, int dontUseSkin)
{
    AtlasTex *tex = getAtlasTex(sprite_name);
    if (tex)
        return tex;

    return addAtlasTex(sprite_name, NULL, dontUseSkin);
}


////////////////////////////////////////////////////////////////////
// Stats
////////////////////////////////////////////////////////////////////
#define HIST_SIZE 128
#define INC_AMOUNT 25

static void showHistogram(void)
{
    int oktorotate = 0;
    int i, x, y, idx;
    int totalSize = HIST_SIZE * HIST_SIZE * 4;
    U8 *buffer;
    static BasicTexture *histogram = 0;
    int num = eaSize(&atlas_textures_array);

    if (!histogram)
        histogram = texGenNew(HIST_SIZE, HIST_SIZE, "TextureAtlas Histogram");

    if (histogram_dirty)
    {
        buffer = _alloca(totalSize * sizeof(U8));
        ZeroStructs(buffer, totalSize);

        for (i = 0; i < num; i++)
        {
            x = atlas_textures_array[i]->width;
            y = atlas_textures_array[i]->height;

            if (oktorotate && x < y)
            {
                int t = y;
                y = x;
                x = t;
            }

            if (x >= HIST_SIZE)
                x = HIST_SIZE - 1;
            if (y >= HIST_SIZE)
                y = HIST_SIZE - 1;

            idx = (x + (y * HIST_SIZE)) * 4;
            if (buffer[idx] < 255 - INC_AMOUNT)
                buffer[idx] += INC_AMOUNT;
        }

        for (i = 0; i < totalSize; i += 4)
        {
            buffer[i + 1] = buffer[i + 2] = buffer[i];
            buffer[i + 3] = 255;
        }

        texGenUpdate(histogram, buffer);

        histogram_dirty = 0;
    }

    display_sprite_tex(histogram, 20, 140, -2, 2, 2, 0xffffffff);

}

static void calcStats(int *atlas_tex_count, int *btex_count, int *not_loaded, int *atlas_total, int *atlas_used, int *atlas_unused, int *btex_total, int *btex_used, int *btex_unused)
{
    int i;
    int num = eaSize(&atlas_textures_array);

    int atex_size = 0;
    int atex_used_size = 0;
    int btex_size = 0;
    int btex_used_size = 0;
    int num_atlased = 0;
    int num_btexed = 0;
    int not_loaded_count = 0;

    AtlasTex *tex;
    TextureAtlas *atlas;

    for (i = 0; i < num; i++)
    {
        tex = atlas_textures_array[i];

        switch (tex->data->state)
        {
        case ATS_ATLASED:
            atex_used_size += tex->width * tex->height;
            num_atlased++;
            break;

        case ATS_BTEXED:
            btex_used_size += tex->width * tex->height;
            btex_size += tex->data->basic_texture->realWidth * tex->data->basic_texture->realHeight;
            num_btexed++;
            break;

        default:
            not_loaded_count++;
            break;
        }
    }

    num = eaSize(&texture_atlases_array);
    for (i = 0; i < num; i++)
    {
        atlas = texture_atlases_array[i];
        atex_size += atlas->atlas_size * atlas->atlas_size;
    }

    if (atlas_total)
        *atlas_total = atex_size;
    if (atlas_used)
        *atlas_used = atex_used_size;
    if (atlas_unused)
        *atlas_unused = atex_size - atex_used_size;

    if (btex_total)
        *btex_total = btex_size;
    if (btex_used)
        *btex_used = btex_used_size;
    if (btex_unused)
        *btex_unused = btex_size - btex_used_size;

    if (atlas_tex_count)
        *atlas_tex_count = num_atlased;
    if (btex_count)
        *btex_count = num_btexed;
    if (not_loaded)
        *not_loaded = not_loaded_count;
}

static void displayAtlasInfo(TextureAtlas *atlas)
{
    int x = 5, y = 50;
    int mouseX, mouseY;

    display_sprite_tex(atlas->texture, 300, 140, -2, 2, 2, 0xffffffff);

    xyprintf(x, y++, "Atlas cell size:          %d", atlas->key.cell_size);

    if (atlas->key.pixel_format == GL_RGBA)
        xyprintf(x, y++, "Atlas format:             RGBA");
    else if (atlas->key.pixel_format == GL_LUMINANCE_ALPHA)
        xyprintf(x, y++, "Atlas format:             LA");

    xyprintf(x, y++, "Atlas index:              %d", atlas->key.atlas_index);

    if (atlas->key.font_param.font)
        xyprintf(x, y++, "Font render size:         %d", atlas->key.font_param.renderSize);
    else
        y++;

    y++;

    xyprintf(x, y++, "Atlas size:               %d", atlas->atlas_size);
    xyprintf(x, y++, "Atlas rows:               %d", atlas->cells_per_side);

    if (!inpMousePos(&mouseX, &mouseY))
    {
        mouseX -= 300;
        mouseY -= 140;

        if (mouseX >= 0 && mouseY >= 0)
        {
            mouseX /= atlas->key.cell_size * 2;
            mouseY /= atlas->key.cell_size * 2;

            if (mouseX < (int)atlas->cells_per_side && mouseY < (int)atlas->cells_per_side)
            {
                AtlasTex *tex = atlas->rows[mouseY].child_textures[mouseX];

                if (tex)
                {
                    y++;

                    xyprintf(x, y++, "AtlasTex name:            %s", tex->name);
                    xyprintf(x, y++, "AtlasTex size:            %d x %d", tex->width, tex->height);
                    xyprintf(x, y++, "AtlasTex priority:        %d", tex->data->priority);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////

void atlasDisplayStats(void)
{
    int x = 5, y = 10;
    int num_atlases, i, j;
    int atex_count, atex_used, atex_unused, atex_total, btex_count, notloaded_count;
    TextureAtlas *atlas;
    TextureAtlasKey key;

    calcStats(&atex_count, &btex_count, &notloaded_count, &atex_total, &atex_used, &atex_unused, 0, 0, 0);

    xyprintf(x, y++, "Number of atlased textures:   %d / %d", atex_count, eaSize(&atlas_textures_array));
    xyprintf(x, y++, "Number of btexed textures:    %d / %d", btex_count, eaSize(&atlas_textures_array));
    xyprintf(x, y++, "Number of notloaded textures: %d / %d", notloaded_count, eaSize(&atlas_textures_array));
    xyprintf(x, y++, "Number of texture atlases:    %d", eaSize(&texture_atlases_array));
    xyprintf(x, y++, "Total atlas area:             %d", atex_total);
    xyprintf(x, y++, "Used atlas area:              %d (%d%%)", atex_used, atex_total ? (100 * atex_used / atex_total) : 0);
    xyprintf(x, y++, "Unused atlas area:            %d (%d%%)", atex_unused, atex_total ? (100 * atex_unused / atex_total) : 0);

    showHistogram();

    if (game_state.atlas_display < 0)
        game_state.atlas_display = 0;
    if (game_state.atlas_display >= eaSize(&texture_atlases_array))
        game_state.atlas_display = eaSize(&texture_atlases_array) - 1;


    num_atlases = eaSize(&texture_atlases_array);
    j = 0;
    for (i = 0; i < num_atlases; i++)
    {
        atlas = texture_atlases_array[i];
        if (atlas->key.atlas_index == 0)
        {
            memcpy(&key, &atlas->key, sizeof(key));

            while (atlas)
            {
                if (j == game_state.atlas_display)
                    displayAtlasInfo(atlas);
                
                j++;
                key.atlas_index++;
                if (!stashFindPointer(texture_atlases, &key, &atlas))
                    atlas = NULL;
            }
        }
    }
}

void atlasDoFrame(void)
{
    int i, num_textures;
#if 0
    int j, k, num_atlases, tex_per_atlas;
    TextureAtlas *atlas, *texatlas;
    TextureAtlasKey key;
    AtlasTex **tex_list = 0;

    eaCreate(&tex_list);

    // rearrange atlases
    num_atlases = eaSize(&texture_atlases_array);
    for (i = 0; i < num_atlases; i++)
    {
        texatlas = texture_atlases_array[i];
        if (texatlas->key.atlas_index == 0)
        {
            tex_per_atlas = atlas->cells_per_side * atlas->cells_per_side;

            memcpy(&key, &texatlas->key, sizeof(key));

            eaSetSize(&tex_list, 0);
            
            atlas = texatlas;
            while (atlas)
            {
                gatherAtlasTextures(atlas, &tex_list);

                key.atlas_index++;
                stashFindPointer(texture_atlases, &key, &atlas);
            }

            num_textures = eaSize(&tex_list);
            qsortG(tex_list, num_textures, sizeof(*tex_list), cmpPriority);

            atlas = texatlas;
            j = 0;
            while (atlas && j < num_textures)
            {
                for (k = j; k < j + tex_per_atlas && k < num_textures; k++)
                {
                    if (!isInAtlas(tex_list[k], atlas))
                    {
                        moveToAtlas(tex_list[k], atlas);
                    }
                }

                j += tex_per_atlas;

                key.atlas_index++;
                stashFindPointer(texture_atlases, &key, &atlas);
            }
        }
    }

    eaDestroy(&tex_list);
#endif
    // reset priority
    num_textures = eaSize(&atlas_textures_array);
    for (i = 0; i < num_textures; i++)
        atlas_textures_array[i]->data->priority = 0;
}

AtlasTex *atlasLoadTextureEx(const char *sprite_name, int dontUseSkin)
{
    AtlasTex *tex;
    char spr_name[1000];

    PERFINFO_AUTO_START("atlasLoadTexture", 1);

    if (strEndsWith(sprite_name, ".tga")) 
    {
        strcpy(spr_name, sprite_name);
        spr_name[strlen(spr_name)-4] = 0;
        tex = getOrAddAtlasTex(spr_name, dontUseSkin);
    } 
    else 
        tex = getOrAddAtlasTex(sprite_name, dontUseSkin);


    PERFINFO_AUTO_STOP();

    return tex;
}

AtlasTex *atlasLoadTexture(const char *sprite_name)
{
    return atlasLoadTextureEx(sprite_name, 0);
}

AtlasTex *atlasGenTextureEx(U8 *src_bitmap, U32 width, U32 height, PixelType pixel_type, char *name, TTFMCacheElement *font_element)
{
    AtlasTex *tex;

    PERFINFO_AUTO_START("atlasGenTexture", 1);

    tex = genAtlasTex(src_bitmap, width, height, pixel_type, name, font_element);

    PERFINFO_AUTO_STOP();

    return tex;
}

AtlasTex *atlasGenTextureFromBasic(BasicTexture *basic_texture, bool flipX, bool flipY)
{
    AtlasTex *tex = allocAtlasTex();
    AtlasTexInternal *data = tex->data;

    tex->width = basic_texture->actualTexture->realWidth;
    tex->height = basic_texture->actualTexture->realHeight;
    tex->name = (char *) allocAddString("white");
    data->state = ATS_BTEXED;

    data->basic_texture = basic_texture;

    data->actual_width = basic_texture->actualTexture->realWidth;
    data->actual_height = basic_texture->actualTexture->realHeight;
    data->u_mult = flipX ? -1.0f : 1.0f;
    data->v_mult = flipY ? -1.0f : 1.0f;
    data->u_offset = flipX ? 1.0f : 0.0f;
    data->v_offset = flipY ? 1.0f : 0.0f;

    histogram_dirty = 1;

    return tex;
}

int atlasUpdateTexturePbuffer(Vec2 ul, Vec2 lr, AtlasTex *tex, PBuffer *pbuffer)
{
    if(!tex || !pbuffer || pbuffer->width == 0 || pbuffer->height == 0 || !tex->data || !tex->data->basic_texture)
    {
        return 0;
    }

    {
    int width = lr[0]-ul[0];
    int height = lr[1]-ul[1];
    F32 widthPercent = width/pbuffer->width;
    F32 heightPercent = height/pbuffer->height;
    F32 startX = ul[0]/pbuffer->width;
    F32 startY = (lr[1])/pbuffer->height;

    tex->width = width;
    tex->height = height;
    tex->data->u_mult = widthPercent;
    tex->data->v_mult = -heightPercent;
    tex->data->u_offset = startX;
    tex->data->v_offset = startY;
    
    tex->data->u_mult = tex->width/(F32)pbuffer->width;
    tex->data->v_mult = -tex->height/(F32)pbuffer->height;

    if (rdr_frame_state.numFBOs > 1)
    {
        printf("ERROR in atlasUpdateTexturePbuffer() - this probably doesn't work with SLI");
        assert(0); // this probably doesn't work with SLI
    }

    if(tex->data->basic_texture->id != pbuffer->color_handle[pbuffer->curFBO])
    {
        rdrTexFree(tex->data->basic_texture->id);
    }

    
    tex->data->basic_texture->id = pbuffer->color_handle[pbuffer->curFBO];
    }
    return 1;
}

void atlasUpdateTexture(AtlasTex *tex, U8 *src_bitmap, int srcWidth, int srcHeight, PixelType pixel_type)
{

    PERFINFO_AUTO_START("atlasUpdateTexture", 1);
    if(tex->data->state == ATS_HAVE_PIXDATA)
    {
        //todo: update this kind of texture as well.
        atlasTexClearPixData(tex);
        assertmsg(0, "atlasUpdateTexture not currently set up for non BTEXTED atlas textures.");
    }
    else if(tex->data->state == ATS_BTEXED)
    {
        BasicTexture *old = tex->data->basic_texture;
        texFree(tex->data->basic_texture, 1);
        texFree(tex->data->basic_texture, 0);
        tex->width = tex->data->actual_width = srcWidth;
        tex->height = tex->data->actual_height = srcHeight;
        atlasTexToBasicTextureFromPixData(tex, src_bitmap, srcWidth, srcHeight,pixel_type);
        texGenFree(old);    //this assumes that you're using a basic texture coming from something like atlasTexToBasicTextureFromPixData
        //this function won't work if called on something like the white texture.
    }
    else //if(tex->data->state == ATS_HAVE_PIXDATA)
    {
        //todo: update this kind of texture as well.
        //atlasTexClearPixData(tex);
        assertmsg(0, "atlasUpdateTexture not currently set up for non BTEXTED atlas textures.");
    }
        

    //assertmsg(0, "atlasUpdateTexture not implemented yet!  Bug Conor if you need it.");

    PERFINFO_AUTO_STOP();
    return;
}

void atlasUpdateTextureFromScreen(AtlasTex *tex, U32 x, U32 y, U32 width, U32 height)
{
    assert(tex->data->state == ATS_BTEXED);
    tex->width = tex->data->actual_width = width;
    tex->height = tex->data->actual_height = height;

    texGenUpdateFromScreen( tex->data->basic_texture, x, y, width, height, false);
}

void atlasGetModifiedUVs(AtlasTex *tex, float u, float v, float *uprime, float *vprime)
{
    AtlasTexInternal *data;

    if (!tex)
        return;

    data = tex->data;

    if (uprime)
        *uprime = u * data->u_mult + data->u_offset;
    if (vprime)
        *vprime = v * data->v_mult + data->v_offset;
}

int atlasDemandLoadTexture(AtlasTex *tex)
{
    static AtlasTex* texDebug;
    AtlasTexInternal *data = tex->data;
    static int prev_texid = 0;
    int texid = 0;

    texDebug = tex;
    PERFINFO_AUTO_START("atlasDemandLoadTexture", 1);
    data->priority++;

    // demand load the unatlased texture
    if (tex->data->state == ATS_BTEXED)
    {
        texid = texDemandLoad(tex->data->basic_texture);
        goto finish_demand_load;
    }

    // atlas textures if needed
    if (data->state == ATS_HAVE_FILENAME)
    {
        TexReadInfo *rawInfo = 0;
        BasicTexture *tex_header = 0;
        int use_cov = false;
        char villain_sprite_name[256] = {0};

        if( tex->skin == 2 )
        {
            Strncpyt( villain_sprite_name, "p_" );
            Strncatt( villain_sprite_name, tex->name );
            tex_header = texFind(villain_sprite_name);
        }
        else if( tex->skin == 1 )
        {
            Strncpyt( villain_sprite_name, "v_" );
            Strncatt( villain_sprite_name, tex->name );
            tex_header = texFind(villain_sprite_name);
        }

        if(!tex_header)
            tex_header = texFind(tex->name);
        else
            use_cov = true;

        if (!tex_header)
        {
            texid = texDemandLoad(white_tex);
            goto finish_demand_load;
        }

        tex_header = texLoadRawData(use_cov?villain_sprite_name:tex->name, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UI);
        if (!tex_header)
        {
            texid = texDemandLoad(white_tex);
            goto finish_demand_load;
        }

        rawInfo = tex_header->actualTexture->rawInfo;
        assert(rawInfo);

        uncompressRawTexInfo(tex_header->actualTexture->rawInfo);

        while (rawInfo->width < tex->data->actual_width || rawInfo->height < tex->data->actual_height)
        {
            tex->data->actual_width /= 2;
            tex->data->actual_height /= 2;
        }

        atlasTexCopyPixData(tex, rawInfo->data, rawInfo->width, rawInfo->height, PIX_RGBA);
        texUnloadRawData(tex_header);
    }

    if (data->state == ATS_HAVE_PIXDATA)
        addToAtlas(tex);

    assert(tex->data->state == ATS_ATLASED);

    // return the texture atlas handle
    texid = tex->data->parent_atlas->texture->id;



finish_demand_load:
#if DEBUG_SPRITE_DRAW_ORDER
    if (game_state.test1)
    {
        TextureAtlas *atlas = tex->data->state == ATS_ATLASED ? tex->data->parent_atlas : 0;
        if (atlas)
        {
            if (atlas->key.font_param.font)
                log_printf("texture_atlas", "%sTexId: 0x%04d   Size: %2d x %2d   Texture: %35s   CellSize: %2d   Font: 0x%08x   RenderSize: %2d", prev_texid != texid ? "  **" : "    ", texid, tex->width, tex->height, tex->name, atlas->key.cell_size, atlas->key.font_param.font, atlas->key.font_param.renderSize);
            else
                log_printf("texture_atlas", "%sTexId: 0x%04d   Size: %2d x %2d   Texture: %35s   CellSize: %2d", prev_texid != texid ? "  **" : "    ", texid, tex->width, tex->height, tex->name, atlas->key.cell_size);
        }
        else
            log_printf("texture_atlas", "%sTexId: 0x%04d   Size: %2d x %2d   Texture: %35s", prev_texid != texid ? "  **" : "    ", texid, tex->width, tex->height, tex->name);
        prev_texid = texid;
    }
#endif

    PERFINFO_AUTO_STOP();
    return texid;
}

void atlasMakeWhite(void)
{
    white_tex_atlas = allocAtlasTex();

    atlasInit();

    stashAddPointer(named_atlas_textures, "white", white_tex_atlas, false);
    eaPush(&atlas_textures_array, white_tex_atlas);

    white_tex_atlas->name = (char *) allocAddString("white");
    white_tex_atlas->data->state = ATS_HAVE_FILENAME;

    atlasTexToBasicTexture(white_tex_atlas, "white");

    histogram_dirty = 1;
}

void atlasFreeAll(void)
{
    int i,num;
    AtlasTex *tex;

    num = eaSize(&atlas_textures_array);
    for (i = 0; i < num; i++)
    {
        tex = atlas_textures_array[i];
        if (tex->data->is_gentex)
            continue;

        // TODO remove from atlas
        if (0 && tex->data->state == ATS_ATLASED)
        {
            tex->data->state = ATS_HAVE_FILENAME;
        }
    }
}

