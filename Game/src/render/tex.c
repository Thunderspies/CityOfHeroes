#include <stdio.h>
#include <search.h>
#include <fcntl.h>
#include "render/thread/ogl.h"
#include <utilitieslib/utils/file.h>
#include "render/tex.h"
#include "graphics/jpeg.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiConsole.h"
#include "seq/tricks.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/memcheck.h>
#include "dd.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include "graphics/gfx.h"
#include "graphics/gfxSettings.h"
#include "graphics/gfxLoadScreens.h"
#include "render/render.h"
#include "render/bump.h"
#include "assert.h"
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/utils/timing.h>
#include "graphics/sun.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/memlog.h>
#include "render/texUnload.h"
#include "render/texWords.h"
#include "render/renderprim.h"
#include "render/thread/rt_tex.h"
#include "render/renderstats.h"
#include "seq/anim.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/StashTable.h>
#include "render/tex_gen.h"
#include <utilitieslib/utils/osdependent.h>
#include <utilitieslib/utils/osdependent.h>
#include "LWC.h"

static const char TEXTURE_MEMMONITOR_NAME[] = "OpenGL Textures";

int disable_parallel_tex_thread_loader = 0;
int delay_texture_loading = 0;  // Adds a delay to simulate slow loading
TexBind *white_tex_bind, *invisible_tex_bind, *grey_tex_bind, *black_tex_bind; 
BasicTexture *white_tex, *invisible_tex, *grey_tex, *black_tex, *dummy_bump_tex, *dummy_dynamic_cubemap_tex, *building_lights_tex;

int texLoadCalls, texLoadStdCalls;

extern CRITICAL_SECTION CriticalSectionTexLoadQueues; //for the thread linked list communication
CRITICAL_SECTION CriticalSectionQueueingLoads; // blocked as long as the thread is still queueing loads
extern CRITICAL_SECTION CriticalSectionTexLoadData; // blocked whenever texLoadData is running (to allow it to be called from both threads)
extern HANDLE background_loader_handle;
extern DWORD background_loader_threadID;
static volatile long numTexLoadsInThread=0;
TexThreadPackage * texBindsReadyForFinalProcessing;
int texMemoryUsage[2]={0};

static int texCheckForSwaps(BasicTexture *basicTexture);
static void texFindNeededBinds(void);
static void texCreateCompositeTextureFromBasic(BasicTexture *basicBind);
static void texCreateCompositeTextures();

// In thread variables:
static int queuingTexLoads=0;
static int queuingTexLoadsDefered=0;
static TexThreadPackage * queuedTexLoads = NULL;
// Out of thread: (the WinMain thread)
static int queuingTexLoadsOutOfThread=0;
static int queuingTexLoadsDeferedOutOfThread=0;


BasicTexture **g_basicTextures;
TexBind **g_compositeTextures;
static StashTable g_basicTextures_ht=0;
static StashTable g_compositeTextures_ht=0;
MP_DEFINE(BasicTexture);
MP_DEFINE(TexBind);
MP_DEFINE(ScrollsScales);

#define MAX_TEXDETAILREDUCE 4
static U32 maxTextureSize[MAX_TEXDETAILREDUCE+1] = { 0x7fffffff, 512, 256, 128, 128 };
static int reduceAllAmount[MAX_TEXDETAILREDUCE+1] = { 0, 0, 0, 1, 2 };

int texGetNumMipsToSkip(U32 tex_width, U32 tex_height, int is_for_entity)
{
    // TODO: Also limit this size by the maximum size supported by the hardware. 
    //  rdr_caps.max_texture_size is supposed to capture this, but it's calculated by the unreliable
    //  glGetIntegerv(GL_MAX_TEXTURE_SIZE... method. This should be switched to 
    //  glTexImage2D(GL_PROXY_TEXTURE_2D... / glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D...
    
    int num, mipLevel = is_for_entity?game_state.entityMipLevel:game_state.actualMipLevel;
    MIN1(mipLevel, MAX_TEXDETAILREDUCE);
    for (num = 0; (num < reduceAllAmount[mipLevel] || tex_width > maxTextureSize[mipLevel] || tex_height > maxTextureSize[mipLevel])
        && (int)tex_width > game_state.reduce_min && (int)tex_height > game_state.reduce_min; num++)
    {
        tex_width >>= 1;
        tex_height >>= 1;
    }
    return num;
}

static int countMipMapLevels(int width, int height) {
    int v = MAX(width, height);
    int r;
    for (r=0; v; (v >>= 1), r++);
    return r;
}

ScrollsScales *createScrollsScales(void)
{
    ScrollsScales *ret;
    MP_CREATE(ScrollsScales, 16);
    ret = MP_ALLOC(ScrollsScales);
    return ret;
}

void destroyScrollsScales(ScrollsScales *scrollsScales)
{
    MP_FREE(ScrollsScales, scrollsScales);
}

TexBind *createTexBind(void)
{
    TexBind *ret;
    MP_CREATE(TexBind, 2048);
    ret = MP_ALLOC(TexBind);
    return ret;
}

void destroyTexBind(TexBind *bind)
{
    if (!bind)
        return;
    if (bind->allocated_scrollsScales) {
        destroyScrollsScales(bind->scrollsScales);
    }
    MP_FREE(TexBind, bind);
}


//RDRFIX
void texDisableThreadedLoading(void) {
    rdrCheckNotThread();
    disable_parallel_tex_thread_loader++;
}

void texEnableThreadedLoading(void) {
    rdrCheckNotThread();
    disable_parallel_tex_thread_loader--;
    if (disable_parallel_tex_thread_loader<0) {
        assert(!"Bad!");
        disable_parallel_tex_thread_loader=0;
    }
}


/*called only by TexLoad Image*/ 
static int ddsLoad(FILE *fp, TexReadInfo *info, int file_bytes, int needRawData, int forEntity)
{
    DDSURFACEDESC2    ddsd;
    char            filecode[4];

    if (!file_bytes)
        return 0;

    memset(info,0,sizeof(TexReadInfo));

//  Old method (doesn't respect low mip requests)
//    if (needRawData) {
//        info->data = (U8 *)malloc(file_bytes);
//        fread(info->data, 1, file_bytes, fp);
//        info->size = file_bytes;
//        info->format = TEXFMT_RAW_DDS;
//        return 0;
//    }

    /* verify the type of file */
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        Errorf("File Format Error: attempting to load dds from .texture without DDS tag");
        return 0;
    }
   
    /* get the surface desc */
    fread(&ddsd, sizeof(ddsd), 1, fp);
    
    info->size = file_bytes - (4 + sizeof(ddsd));

    switch(ddsd.ddpfPixelFormat.dwFourCC)
    {
        case FOURCC_DXT1:
            info->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            info->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            info->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            if (ddsd.ddpfPixelFormat.dwFlags == DDS_RGBA && ddsd.ddpfPixelFormat.dwRGBBitCount == 32 && ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff000000)
                info->format = TEXFMT_ARGB_8888;
            else if (ddsd.ddpfPixelFormat.dwFlags == DDS_RGB  && ddsd.ddpfPixelFormat.dwRGBBitCount == 24)
                info->format = TEXFMT_ARGB_0888;
            else if (ddsd.ddpfPixelFormat.dwFlags == DDS_RGB  && ddsd.ddpfPixelFormat.dwRGBBitCount == 16 && ddsd.ddpfPixelFormat.dwGBitMask == 0x000007e0)
                info->format = TEXFMT_ARGB_0565;
            else if (ddsd.ddpfPixelFormat.dwFlags == DDS_RGBA && ddsd.ddpfPixelFormat.dwRGBBitCount == 16 && ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0x00008000)
                info->format = TEXFMT_ARGB_1555;
            else if (ddsd.ddpfPixelFormat.dwFlags == DDS_RGBA && ddsd.ddpfPixelFormat.dwRGBBitCount == 16 && ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0x0000f000)
                info->format = TEXFMT_ARGB_4444;
            else
            {
                Errorf("cant decode compressed texture");
                //free(info->data);
                return 0;
            }
    }
//#define BIGGEST_TEXTURE_NOT_TO_REDUCE 16 == game_state.reduce_min
    {
        int blockSize = (info->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        int num, mipLevel = texGetNumMipsToSkip(ddsd.dwWidth, ddsd.dwHeight, forEntity);

        // Skip past mip levels we don't want to load
        for (num = 0; num < mipLevel && ddsd.dwMipMapCount > 0; num++)
        {
            int size;
            if (info->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
                    info->format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
                    info->format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
            {
                size = ((ddsd.dwWidth+3)/4)*((ddsd.dwHeight+3)/4)*blockSize;
            } else {
                size = ddsd.dwWidth * ddsd.dwHeight * (ddsd.ddpfPixelFormat.dwRGBBitCount / 8);
            }
            ddsd.dwWidth >>= 1;
            ddsd.dwHeight >>= 1;
            if (ddsd.dwHeight==0) ddsd.dwHeight=1; // Just in case of a 32x1 texture!
            if (ddsd.dwWidth==0) ddsd.dwWidth=1; // And in case of a 1x32
            info->size -= size;
            ddsd.dwMipMapCount--;
            fseek(fp, size, SEEK_CUR);
        }

        info->mip_count        = ddsd.dwMipMapCount;
        info->width            = ddsd.dwWidth;
        info->height        = ddsd.dwHeight;
        if (needRawData) 
        {
            int total_bytes = info->size + sizeof(ddsd) + 4;
            // Need the "whole" .dds file, but with the new parameters
            info->data = (U8 *)malloc(total_bytes);
            strcpy(info->data, "DDS ");
            memcpy(info->data + 4, &ddsd, sizeof(ddsd));
            fread(info->data + 4 + sizeof(ddsd), 1, info->size, fp);
            info->size = total_bytes;
            info->format = TEXFMT_RAW_DDS;
        } else {
            // Normal texture read
            info->data = (U8 *)malloc(info->size);
            fread(info->data, 1, info->size, fp);
        }
    }
    return 1;
}
//IN THREAD

static void makeTexPow2(TexReadInfo *info)
{
    int    i,w,h;
    U8    *data;

    w = 1 << log2i(info->width);
    h = 1 << log2i(info->height);
    if (w == info->width && h == info->height)
        return;
    data = calloc(w*h,3);
    for(i=0;i<info->height;i++)
    {
        memcpy(&data[i*w*3],&info->data[i*info->width*3],info->width*3);
    }
    free(info->data);
    info->data = data;
    info->width = w;
    info->height = h;
    info->size = w*h*3;
}

static void EnterQueueingLoadsCS()
{
    EnterCriticalSection(&CriticalSectionQueueingLoads);
}

static void LeaveQueueingLoadsCS()
{
    LeaveCriticalSection(&CriticalSectionQueueingLoads);
}

int  texReadFile(char * texName, char * directoryName, TexReadInfo * info)
{
    TextureFileHeader tfh;
    char filename[MAX_PATH];
    int numread;
    int filePosition;
    int fileSize;
    FILE    *tex_file;

    if(directoryName)
    {
        STR_COMBINE_SSSSS(filename, "texture_library/", directoryName, "/", texName, ".texture");
    }
    else
        strcpy(filename, texName);

    //open the file
    tex_file = fileOpen(filename, "rb");
    if (!tex_file) {
        //report the error somehow
        return 0;
    }

    // Read the data
    numread=fread(&tfh, 1, sizeof(tfh), tex_file);
    if (numread!=sizeof(tfh)) {
        Errorf("Error loading texture file '%s'!", filename);
        return 1;
    }
    numread = tfh.header_size - sizeof(tfh);


    //Get bind->file_pos and bind->file_bytes ints
    filePosition = tfh.header_size;
    fileSize = tfh.file_size;
    fseek(tex_file,filePosition,0);

    ddsLoad(tex_file,info,fileSize,0,0);

    if (!info->data) {
        printf("Warning: Error loading texture_library/%s/%s.texture\n", directoryName,texName);
    }

    fclose(tex_file);
    return 1;
}


int  texWriteFile(char * texName, TexReadInfo * info)
{
    TextureFileHeader tfh;
    DDSURFACEDESC2 surface;
    char filename[MAX_PATH];
    char * extension;
    char fileType[] = { 'D', 'D', 'S', ' '};
    FILE    *tex_file;

    strcpy(filename, texName);
    //STR_COMBINE_SSSSS(filename, "texture_library/", directoryName, "/", texName, ".texture");

    //open the file
    tex_file = fileOpen(filename, "wb");
    if (!tex_file) {
        //report the error somehow
        return 0;
    }

    extension = strchr(filename, '.');
    sprintf(extension, ".dds");
    extension[4] = 0;

    //SETUP THE .TEXTURE STRUCTURES:
    //--------------------------------------------------------
    //Contents:
    //1) TextureFileHeader (29 bytes) 
    //2) filename (variable null terminated) 
    //3) "DDS " (4 bytes) 
    //4) DDSURFACEDESC2 (?) 
    //5) DATA (file_bytes - (4 + sizeof(ddsd)))
    
    //(1)fill out the tfh
    tfh.header_size = sizeof(tfh) + strlen(filename) +1; // Number of bytes in header data of file (not sizeof(struct)) +1 because of null at end of filename
    tfh.file_size = info->size + 4 + sizeof(surface); // Number of bytes in data chunk of file (everything after the header + name + mip = .tga file or .dds file)
    tfh.width = info->width; 
    tfh.height = info->height;
    tfh.flags;    //STILL NEED
    tfh.fade[2];
    tfh.alpha = 1; // boolean flag
    tfh.pad[0] = 'T'; // =TEX for v1 .Textures, =TX2 for v2 .Textures
    tfh.pad[1] = 'X';
    tfh.pad[2] = '2';
    fwrite(&tfh, sizeof(tfh), 1, tex_file);

    //(2) filename
    //change from .texture to .dds
    fwrite(filename, 1, strlen(filename)+1, tex_file);    //+1 for null termination

    //(3) DDS
    fwrite(fileType, 1, sizeof(fileType), tex_file);

    //(4) DDSURFACESC2
    surface.dwSize = sizeof(surface);                 // size of the DDSURFACEDESC structure
    //we don't actually have access to these flags, and we don't appear to use them, so lets try not having them.
    //surface.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;                // determines what fields are valid
    surface.dwHeight = info->height;               // height of surface to be created
    surface.dwWidth = info->width;                // width of input surface
    surface.dwLinearSize = info->size;                    //Formless late-allocated optimized surface size
    surface.dwBackBufferCount = 0;      // number of back buffers requested
    surface.dwMipMapCount = 0;          // number of mip-map levels requestde
    surface.dwAlphaBitDepth = 0;        // depth of alpha buffer requested
    surface.dwReserved = 0;             // reserved
    surface.lpSurface = 0;              // pointer to the associated surface memory
    surface.ddckCKDestOverlay.dwColorSpaceHighValue = surface.ddckCKDestOverlay.dwColorSpaceLowValue = 0;      // color key for destination overlay use
    surface.dwEmptyFaceColor = 0;       // Physical color for empty cubemap faces
    surface.ddckCKDestBlt.dwColorSpaceHighValue = surface.ddckCKDestBlt.dwColorSpaceLowValue = 0;          // color key for destination blt use
    surface.ddckCKSrcOverlay.dwColorSpaceHighValue = surface.ddckCKSrcOverlay.dwColorSpaceLowValue = 0;       // color key for source overlay use
    surface.ddckCKSrcBlt.dwColorSpaceHighValue = surface.ddckCKSrcOverlay.dwColorSpaceLowValue = 0;           // color key for source blt use
    //surface.ddpfPixelFormat;        // pixel format description of the surface
    surface.ddpfPixelFormat.dwSize = sizeof(DDSURFACEDESC2);                 // size of structure
    //again, lets try without the flags.
    //surface.ddpfPixelFormat.dwFlags;                // pixel format flags
    surface.ddpfPixelFormat.dwFourCC = 0;               // (FOURCC code)
    switch(info->format)
    {
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        surface.ddpfPixelFormat.dwFourCC = FOURCC_DXT1;
        break;
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        surface.ddpfPixelFormat.dwFourCC =  FOURCC_DXT3;
        break;
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        surface.ddpfPixelFormat.dwFourCC = FOURCC_DXT5;
        break;
    case TEXFMT_ARGB_8888:
        surface.ddpfPixelFormat.dwFlags = DDS_RGBA;
        surface.ddpfPixelFormat.dwRGBBitCount = 32;
        surface.ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;
        break;
    case TEXFMT_ARGB_0888:
        surface.ddpfPixelFormat.dwFlags = DDS_RGB;
        surface.ddpfPixelFormat.dwRGBBitCount = 24;
        break;
    case TEXFMT_ARGB_0565:
        surface.ddpfPixelFormat.dwFlags = DDS_RGB;
        surface.ddpfPixelFormat.dwRGBBitCount = 16;
        surface.ddpfPixelFormat.dwGBitMask = 0x000007e0;
        break;
    case TEXFMT_ARGB_1555:
        surface.ddpfPixelFormat.dwFlags = DDS_RGBA;
        surface.ddpfPixelFormat.dwRGBBitCount = 16;
        surface.ddpfPixelFormat.dwRGBAlphaBitMask = 0x00008000;
        break;
    case TEXFMT_ARGB_4444:
        surface.ddpfPixelFormat.dwFlags = DDS_RGBA;
        surface.ddpfPixelFormat.dwRGBBitCount = 16;
        surface.ddpfPixelFormat.dwRGBAlphaBitMask = 0x0000f000;
        break;
    default:
            Errorf("cant decode compressed texture");
            //free(info->data);
            return 0;
    }
    //the above appear to be the only fields used, so lets stick to them.
    /*surface.ddpfPixelFormat.dwRGBBitCount;          // how many bits per pixel
    surface.ddpfPixelFormat.dwRBitMask;             // mask for red bit
    surface.ddpfPixelFormat.dwGBitMask;             // mask for green bits
    surface.ddpfPixelFormat.dwBBitMask;             // mask for blue bits
    surface.ddpfPixelFormat.dwRGBAlphaBitMask;      // mask for alpha channel*/
    //end of ddpfPixelFormat
    surface.ddsCaps.dwCaps = surface.dwLinearSize ;                // direct draw surface capabilities
    surface.ddsCaps.dwCaps2 = surface.ddsCaps.dwCaps3 = surface.ddsCaps.dwCaps4 = 0;
    surface.dwTextureStage = 0;         // stage in multitexture cascade
    fwrite(&surface, sizeof(surface), 1, tex_file);

    //(4) DATA
        fwrite(info->data, 1, info->size, tex_file);

    fclose(tex_file);
    return 1;
}

/* load texture data from the .texture file.  Called from either thread */
static void texLoadData(TexThreadPackage *pkg)
{
    FILE    *tex_file;
    TexReadInfo * info;
    BasicTexture * bind;
    char filename[MAX_PATH];

    //printf("loading tex: %s\n", pkg->bind->name);
    
    bind = pkg->bind;
    info = &pkg->info;

    // to simulate slow texture loading
    if (delay_texture_loading && !(bind->use_category & TEX_FOR_UI)) {
        Sleep(delay_texture_loading);
    }

    if (bind->texWordParams) {
        // No actual data to load, but we still need a package going through the queues to get
        //  processed on the other end
        return;
    }

    memlog_printf(0, "%u: texLoadData %s%s", game_state.client_frame_timestamp, pkg->needRawData?"RAW ":" ", bind->name);

    EnterCriticalSection(&CriticalSectionTexLoadData);

    STR_COMBINE_SSSSS(filename, "texture_library/", bind->dirname, "/", bind->name, ".texture");
    
    tex_file = fileOpen(filename, "rb");
    if (!tex_file) {
        printf("Error!  File 'texture_library/%s/%s.texture' not found when loading texture '%s'!\n", bind->dirname, bind->name, bind->name);
        LeaveCriticalSection(&CriticalSectionTexLoadData);
        return;
    }
    if (quickload) 
    {
        // With -quickloadtextures, we can end up having not in sync headers, this will fix it partially
        bind->file_bytes = fileSize(filename) - bind->file_pos;
    }

    fseek(tex_file,bind->file_pos,0);

    if (bind->flags & TEX_TGA) {
        Errorf("Refusing to load TGA format texture file %s/%s!", bind->dirname,bind->name);
        //tgaLoad(tex_file,info);
    } else if (bind->flags & TEX_JPEG)
    {
        char    *mem = malloc(bind->file_bytes);

        fread(mem,bind->file_bytes,1,tex_file);
        jpegLoad(mem,bind->file_bytes,info);
        free(mem);
        if (info->data)  // JS:  Make sure the texture loading succeeded before doing this
            makeTexPow2(info);
    }
    else
        ddsLoad(tex_file,info,bind->file_bytes,pkg->needRawData,bind->use_category & TEX_FOR_ENTITY);

    bind->memory_use[pkg->needRawData] = info->size;
    texMemoryUsage[pkg->needRawData] += bind->memory_use[pkg->needRawData];
    if (!pkg->needRawData) {
        // Only track GL memory, RAW memory is already tracked
        memMonitorTrackUserMemory(TEXTURE_MEMMONITOR_NAME, 1, MOT_ALLOC, bind->memory_use[pkg->needRawData]);
    }

    if (!info->data) {
        printf("Warning: Error loading texture_library/%s/%s.texture\n", bind->dirname,bind->name);
    }

    fclose(tex_file);
    
    LeaveCriticalSection(&CriticalSectionTexLoadData);

    if (pkg->needRawData && info->data) {
        assert(bind->rawReferenceCount);
        // Flag as loaded from this thread (no real reason to send back to other thread, but we do
        //  anyway, if nothing else to count number of bytes loaded)
        bind->rawInfo = calloc(sizeof(TexReadInfo), 1);
        *bind->rawInfo = *info;
        bind->load_state[pkg->needRawData] = TEX_LOADED;
    }
}

static TexOpt *trickFromTextureDirName(const char *dirname, const char *name, TexOptFlags *texopt_flags)
{
    char    buf[1000];

    STR_COMBINE_SSS(buf, dirname, "/", name);
    return trickFromTextureName(buf, texopt_flags);
}

//END IN THREAD


//OUT OF THREAD


//END OUT OF THREAD


/*Removes tex file suffix, and returns .tga or .dds flag based on that suffix.  
s = input name, res = chopped name (can be same ptr) */
static int texFixName(const char *s,char *res, int size)
{
int        type = 0;

    if (s != res)
        strncpyt(res,s,size);

    if( strlen(res) < 4 )
        return TEX_TGA; //I guess, maybe error out?

    for(;*s;s++,res++)
    {
        //*res = tolower(*s);
    }
    if (res[-4] == '.')
    {
        if (strcmp(res-3,"tga")==0) {
            // Even though we don't load .tga files anymore, this is still used (through texFind)
            type = TEX_TGA;
        } else if (strcmp(res-3,"jpg")==0)
            type = TEX_JPEG;
        else if (strcmp(res-3,"dds")==0)
            type = TEX_DDS;
        res[-4] = 0;
    }
    //fix for files that were incorrectly named .texture instead of dds.
    else if(res[-8] == '.')
    {
        if (strcmp(res-7,"texture")==0) {
            // Even though we don't load .tga files anymore, this is still used (through texFind)
            type = TEX_DDS;
        }
        res[-8] = 0;
    }
    *res = 0;
    return type;
}

int texResetTrickBasedParametersBasic(BasicTexture *bind, BasicTexture **binds, int i)
{
    TexOpt    *texopt;
    TexOptFlags texopt_flags = 0;
    TexFlags old_flags = bind->flags;
    TexOptFlags old_texopt_flags = bind->texopt_flags;

    // Clear flags that should only be set in this function
    bind->flags &= ~(TEX_REPLACEABLE);

    texopt = trickFromTextureDirName(bind->dirname,bind->name, &texopt_flags);

    if (texopt)
        bind->gloss = texopt->gloss;
    else
        bind->gloss = 1;

    bind->texopt_flags = texopt_flags;
    bind->texopt_surface = texopt?texopt->surface:0;

    if (texopt_flags & TEXOPT_REPLACEABLE)
        bind->flags |= TEX_REPLACEABLE;

    if( bind->name && strstriConst(bind->name, "cubemap") )
        return 0; // temp hack until get flags for sub faces properly

    // Check to see if any flags changed that need a reload
    #define RELOAD_TEXOPT_FLAGS (TEXOPT_CLAMPS|TEXOPT_CLAMPT|TEXOPT_REPEATS|TEXOPT_REPEATT|TEXOPT_NOMIP|TEXOPT_MIRRORS|TEXOPT_MIRRORT|TEXOPT_BORDER)
    if ((old_texopt_flags&RELOAD_TEXOPT_FLAGS) != (bind->texopt_flags&RELOAD_TEXOPT_FLAGS))
        return 1;
    return 0;

}

static void texSetBindsSubBasic(BasicTexture *bind, BasicTexture **binds, int i)
{
    texResetTrickBasedParametersBasic(bind, binds, i);

    if (bind->flags & TEX_CUBEMAPFACE || bind->texopt_flags & TEXOPT_CUBEMAP) { // only the first face is marked with this flag upon load
        bind->texopt_flags |= TEXOPT_CLAMPS|TEXOPT_CLAMPT;
        bind->target = GL_TEXTURE_CUBE_MAP_ARB;
    } else
        bind->target = GL_TEXTURE_2D;

    bind->load_state[0] = TEX_NOT_LOADED;
    bind->load_state[1] = TEX_NOT_LOADED;

    bind->hasBeenComposited = 0;
    bind->texWord = texWordFind(bind->name, 0);
}

static BasicTexture *texFindVerify(const char *name, BasicTexture *fallback, char *fieldName, char *filename, char *trickname)
{
    BasicTexture *ret;
    if (!name || stricmp(name, "SWAPPABLE")==0) {
        ret = fallback;
    } else {
        ret = texFind(name);
        if (!ret) {
            if (!quickload)
                ErrorFilenamef(filename, "Material trick %s is referencing missing %s texture %s", trickname, fieldName, name);
            ret = fallback;
        }
    }
    return ret;    
}

static BasicTexture *texFindVerifyCubemap(const char *name, BasicTexture *fallback, char *fieldName, char *filename, char *trickname)
{
    // Look for special signifier for forcing use of dynamic texture
    if (stricmp(name, "%dynamic%") == 0)
    {
        return dummy_dynamic_cubemap_tex;
    }

    return texFindVerify(name, fallback, fieldName, filename, trickname);
}

static void fixLightingScalesOnFallback(TexBind *bind)
{
    F32 ambient=0.5, diffuse=0.5;

    // Fix ambient/diffuse scales here
    switch (bind->bind_blend_mode.shader) {
        xcase BLENDMODE_BUMPMAP_MULTIPLY:
            ambient = 0.25;
            diffuse = 0.5;
        xcase BLENDMODE_MULTIPLY:
        xcase BLENDMODE_COLORBLEND_DUAL:
            ambient = 0.5;
            diffuse = 0.5;
        xcase BLENDMODE_ADDGLOW:
            ambient = 0.25;
            diffuse = 0.5;
        xcase BLENDMODE_ALPHADETAIL:
        xcase BLENDMODE_SUNFLARE:
        xcase BLENDMODE_BUMPMAP_COLORBLEND_DUAL:
            ambient = 1;
            diffuse = 1;
            break;
    }
    scaleVec3(bind->texopt->scrollsScales.ambientScaleTrick, ambient, bind->scrollsScales->ambientScale);
    scaleVec3(bind->texopt->scrollsScales.diffuseScaleTrick, diffuse, bind->scrollsScales->diffuseScale);
    
}

BlendModeType optimizeBlendMode(BlendModeShader shader, TexBind *bind)
{
    BlendModeType blend_mode = BlendMode(shader, 0);
    bool buildingable=true;
    if (shader!= BLENDMODE_MULTI)
        return blend_mode;
    if (bind->tex_layers[TEXLAYER_MASK] == white_tex || !(rdr_caps.features & GFXF_MULTITEX_DUAL)) {
        blend_mode.blend_bits = BMB_SINGLE_MATERIAL;
    }
#define CHECKSCROLLSCALE(layer) \
    if (bind->tex_layers[layer]!=white_tex && !(bind->texopt->scrollsScales.texopt_scale[layer][0]==1 && \
        bind->texopt->scrollsScales.texopt_scale[layer][1]==1 && \
        bind->texopt->scrollsScales.texopt_scroll[layer][0]==0 && \
        bind->texopt->scrollsScales.texopt_scroll[layer][1]==0)) \
            buildingable = false;
#define CHECKSCROLL(layer) \
    if (bind->tex_layers[layer]!=white_tex && !( \
        bind->texopt->scrollsScales.texopt_scroll[layer][0]==0 && \
        bind->texopt->scrollsScales.texopt_scroll[layer][1]==0)) \
            buildingable = false;
    CHECKSCROLLSCALE(TEXLAYER_BASE1);
    CHECKSCROLLSCALE(TEXLAYER_BUMPMAP1);
    CHECKSCROLLSCALE(TEXLAYER_DUALCOLOR1);
    CHECKSCROLLSCALE(TEXLAYER_MASK);
#define CHECKLAYER(layer, tex) \
    if (bind->tex_layers[layer] != tex) buildingable = false;
    CHECKLAYER(TEXLAYER_BASE2, white_tex);
    CHECKLAYER(TEXLAYER_BUMPMAP2, dummy_bump_tex);
    CHECKLAYER(TEXLAYER_DUALCOLOR2, white_tex);
    if (bind->tex_layers[TEXLAYER_MULTIPLY2]==white_tex) buildingable = false;
    if (bind->tex_layers[TEXLAYER_MASK]==white_tex) buildingable = false;
    if (buildingable)
        blend_mode.blend_bits = BMB_BUILDING;
    if (bind->texopt->flags & TEXOPT_OLDTINT)
        blend_mode.blend_bits |= BMB_OLDTINTMODE;

    if (bind->tex_layers[TEXLAYER_CUBEMAP] && (rdr_caps.allowed_features & GFXF_CUBEMAP) ) {
        onlydevassert(bind->tex_layers[TEXLAYER_CUBEMAP]->flags & TEX_CUBEMAPFACE);
        // @todo Is this necessary and relevant?
        // The BMB_CUBEMAP flag will be set on the blend mode at render time based on
        // a number of factors (e.g. main viewport, edit mode, etc.)
        blend_mode.blend_bits |= BMB_CUBEMAP;
    }

    return blend_mode;
}

static void setSwappableInfo(TexBind *bind, TexOpt *texopt)
{
    int i;
    bool supportsMultiTex = rdr_caps.features & GFXF_MULTITEX;

    memset(bind->tex_swappable, 0, sizeof(bind->tex_swappable));
    if (!texopt) {
        bind->tex_swappable[TEXLAYER_GENERIC] = 1;
    } else if (bind->bind_blend_mode.shader == BLENDMODE_MULTI) {
        // Fleshed out MultiTex
        for (i=0; i<TEXLAYER_MAX_LAYERS; i++) {
            bind->tex_swappable[i] = texopt->swappable[texLayerIndexToBlendIndex(i)];
        }
    } else if (texopt->flags & TEXOPT_TREAT_AS_MULTITEX) {
        // Auto converted, or fallback texture
        bind->tex_swappable[TEXLAYER_BASE] = texopt->swappable[BLEND_BASE1];
        switch(bind->bind_blend_mode.shader) {
        xcase BLENDMODE_MODULATE:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_MULTIPLY1];
        xcase BLENDMODE_MULTIPLY:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_MULTIPLY1];
        xcase BLENDMODE_COLORBLEND_DUAL:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_DUALCOLOR1];
        xcase BLENDMODE_ADDGLOW:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_ADDGLOW1];
        xcase BLENDMODE_ALPHADETAIL:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_MULTIPLY1];
        xcase BLENDMODE_BUMPMAP_MULTIPLY:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_MULTIPLY1];
            bind->tex_swappable[TEXLAYER_BUMPMAP1] = texopt->swappable[BLEND_BUMPMAP1];
        xcase BLENDMODE_BUMPMAP_COLORBLEND_DUAL:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_DUALCOLOR1];
            bind->tex_swappable[TEXLAYER_BUMPMAP1] = texopt->swappable[BLEND_BUMPMAP1];
        xcase BLENDMODE_WATER:
            bind->tex_swappable[TEXLAYER_GENERIC] = texopt->swappable[BLEND_MULTIPLY1];
            bind->tex_swappable[TEXLAYER_BUMPMAP1] = texopt->swappable[BLEND_BUMPMAP1];
        xdefault:
            assertmsg(0, "Not all blendmodes configured right - tell Jimb");
        }
    } else {
        // Old-style texture
        bind->tex_swappable[TEXLAYER_GENERIC] = 1;
    }
}

static void texResetTrickBasedParametersComposite(TexBind *bind, bool forceFallback)
{
    int i;
    TexOpt    *texopt;
    TexOptFlags texopt_flags=0;
    char *trickFileName="NONE";
    char *trickName="NONE";
    bool supportsMultiTex = (rdr_caps.features & GFXF_MULTITEX) && !forceFallback;

    texopt = trickFromTextureDirName(bind->dirname,bind->name,&texopt_flags);

#ifdef _FULLDEBUG
    // a debug/trace filter to make it easier to track the processing of particular
    // tricks/texture binds
    // @todo why does the strstri wrapper not take a const char buffer?
    if ( *g_client_debug_state.trace_tex != 0 && strstriConst(bind->name, g_client_debug_state.trace_tex ))
    {
        printf( "trace: texResetTrickBasedParametersComposite: filter hit - break here to step\n" );
    }
#endif

    // If this is a tex_opt in our delayed load list, do nothing with it now
    if(trickIsDelayedLoadTexOpt(texopt))
        return;

    bind->texopt = texopt;
    bind->is_fallback_material = forceFallback;

    if (!forceFallback) {
        // This should never re-assign an existing scrollsScales pointer
        assert(!bind->scrollsScales || !texopt || bind->scrollsScales == &texopt->scrollsScales);
        bind->scrollsScales = texopt?&texopt->scrollsScales:NULL;
    }

    if (texopt) {
        trickFileName = texopt->file_name;
        trickName = texopt->name;

        //if (!(texopt_flags & TEXOPT_MULTITEX) || supportsMultiTex) {
        bind->bind_scale[0][0] = texopt->scrollsScales.texopt_scale[BLEND_OLD_TEX_BASE][0];
        bind->bind_scale[0][1] = texopt->scrollsScales.texopt_scale[BLEND_OLD_TEX_BASE][1];
        bind->bind_scale[1][0] = texopt->scrollsScales.texopt_scale[BLEND_OLD_TEX_GENERIC_BLEND][0];
        bind->bind_scale[1][1] = texopt->scrollsScales.texopt_scale[BLEND_OLD_TEX_GENERIC_BLEND][1];
        //}
        if (bind->scrollsScales)
        {
            copyVec3(texopt->scrollsScales.ambientScaleTrick, bind->scrollsScales->ambientScale);
            copyVec3(texopt->scrollsScales.diffuseScaleTrick, bind->scrollsScales->diffuseScale);
            if (texopt_flags & TEXOPT_OLDTINT) {
                scaleVec3(bind->scrollsScales->ambientScale, 0.5, bind->scrollsScales->ambientScale);
                scaleVec3(bind->scrollsScales->diffuseScale, 0.5, bind->scrollsScales->diffuseScale);
            }
        }
    } else {
        bind->bind_scale[0][0] = 1;
        bind->bind_scale[0][1] = 1;
        bind->bind_scale[1][0] = 1;
        bind->bind_scale[1][1] = 1;
    }

    for (i=1; i<TEXLAYER_MAX_LAYERS; i++) // Reset all but Base1 (which might come from a dynamic texture)
        bind->tex_layers[i] = NULL;

    if (texopt_flags & TEXOPT_MULTITEX || (texopt_flags & TEXOPT_TREAT_AS_MULTITEX && !supportsMultiTex && texopt->fallback.useFallback))
    {
        bind->tex_layers[TEXLAYER_BASE1] = texFindVerify(texopt->blend_names[BLEND_BASE1], white_tex, "Base1", trickFileName, trickName);
        if ((rdr_caps.features & GFXF_BUMPMAPS))
            bind->tex_layers[TEXLAYER_BUMPMAP1] = texFindVerify(texopt->blend_names[BLEND_BUMPMAP1], dummy_bump_tex, "BumpMap1", trickFileName, trickName);

        if (supportsMultiTex && (!(texopt->model_flags & OBJ_FANCYWATER) || ((texopt->model_flags & OBJ_FANCYWATER) && game_state.waterMode > WATER_OFF)))
        {
            // Multi-tex texture!
            bind->tex_layers[TEXLAYER_MULTIPLY1] = texFindVerify(texopt->blend_names[BLEND_MULTIPLY1], white_tex, "Multiply1", trickFileName, trickName);
            bind->tex_layers[TEXLAYER_DUALCOLOR1] = texFindVerify(texopt->blend_names[BLEND_DUALCOLOR1], white_tex, "DualColor1", trickFileName, trickName);
            bind->tex_layers[TEXLAYER_ADDGLOW1] = texFindVerify(texopt->blend_names[BLEND_ADDGLOW1], black_tex, "AddGlow1", trickFileName, trickName);

            bind->tex_layers[TEXLAYER_MASK] = texFindVerify(texopt->blend_names[BLEND_MASK], white_tex, "Mask", trickFileName, trickName);

            bind->tex_layers[TEXLAYER_BASE2] = texFindVerify(texopt->blend_names[BLEND_BASE2], white_tex, "Base2", trickFileName, trickName);
            bind->tex_layers[TEXLAYER_MULTIPLY2] = texFindVerify(texopt->blend_names[BLEND_MULTIPLY2], white_tex, "Multiply2", trickFileName, trickName);
            bind->tex_layers[TEXLAYER_DUALCOLOR2] = texFindVerify(texopt->blend_names[BLEND_DUALCOLOR2], white_tex, "DualColor2", trickFileName, trickName);
            bind->tex_layers[TEXLAYER_BUMPMAP2] = texFindVerify(texopt->blend_names[BLEND_BUMPMAP2], dummy_bump_tex, "BumpMap2", trickFileName, trickName);

            if( texopt->blend_names[BLEND_CUBEMAP] && (rdr_caps.allowed_features & GFXF_CUBEMAP) )
                bind->tex_layers[TEXLAYER_CUBEMAP] = texFindVerifyCubemap(texopt->blend_names[BLEND_CUBEMAP], NULL, "CubeMap", trickFileName, trickName);

            if ((texopt->model_flags & OBJ_FANCYWATER) && (game_state.waterMode > WATER_OFF)) {
                bind->bind_blend_mode = BlendMode(BLENDMODE_WATER, 0);
            } else {
                bind->bind_blend_mode = optimizeBlendMode(BLENDMODE_MULTI, bind);

                // if no multiply2 texture, make sure that multiply2 flag not set (fix for bad material data)
                if( bind->scrollsScales && bind->scrollsScales->multiply2Reflect && bind->tex_layers[TEXLAYER_MULTIPLY2] == white_tex )
                    bind->scrollsScales->multiply2Reflect = 0;
            }
        } else {
            BlendIndex scale0=BLEND_BASE1;
            BlendIndex scale1=BLEND_MULTIPLY1;
            // Doesn't support it, fall back to some simpler texture mode
            if (texopt->fallback.useFallback) {
                TexOptFallback *fallback = &texopt->fallback;
                // Explicit fallback specified
                bind->bind_scale[0][0] = fallback->scaleST[0][0];
                bind->bind_scale[0][1] = fallback->scaleST[0][1];
                bind->bind_scale[1][0] = fallback->scaleST[1][0];
                bind->bind_scale[1][1] = fallback->scaleST[1][1];
                bind->tex_layers[TEXLAYER_BASE] = texFindVerify(fallback->base_name, white_tex, "Fallback::Base", trickFileName, trickName);
                if (fallback->blend_name && stricmp(fallback->blend_name, "none")!=0) {
                    bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(fallback->blend_name, grey_tex, "Fallback::Blend", trickFileName, trickName);
                } else {
                    bind->tex_layers[TEXLAYER_GENERIC] = grey_tex;
                }
                if ((rdr_caps.features & GFXF_BUMPMAPS) && fallback->bumpmap && stricmp(fallback->bumpmap, "none")!=0)
                    bind->tex_layers[TEXLAYER_BUMPMAP1] = texFindVerify(fallback->bumpmap, dummy_bump_tex, "Fallback::BumpMap", trickFileName, trickName);
                else
                    bind->tex_layers[TEXLAYER_BUMPMAP1] = NULL;

                bind->bind_blend_mode = BlendMode(fallback->blend_mode, 0);

                // Promote blend modes that have a bumpmap
                if (bind->tex_layers[TEXLAYER_BUMPMAP1])
                {
                    if (bind->bind_blend_mode.shader == 0) {
                        bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_MULTIPLY, 0);
                    } else if (bind->bind_blend_mode.shader == BLENDMODE_COLORBLEND_DUAL) {
                        bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0);
                    }
                }
                if (bind->bind_blend_mode.shader == BLENDMODE_COLORBLEND_DUAL) {
                    // Guessing this is a world/building shader
                    bind->bind_blend_mode.blend_bits |= BMB_NV1XWORLD;
                }
                // Copy lighting scales
                if (bind->scrollsScales)
                {
                    copyVec3(texopt->fallback.ambientScaleTrick, bind->scrollsScales->ambientScale);
                    copyVec3(texopt->fallback.diffuseScaleTrick, bind->scrollsScales->diffuseScale);
                    copyVec3(texopt->fallback.ambientMin, bind->scrollsScales->ambientMin);
                }
            } else {
                // Auto-fallback mode
                if (texopt->model_flags & OBJ_FANCYWATER) {
                    // Water wants bumpmap multiply
                    bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(texopt->blend_names[BLEND_MULTIPLY1], white_tex, "Multiply1", trickFileName, trickName);
                    scale1 = BLEND_MULTIPLY1;
                    if ((rdr_caps.features & GFXF_BUMPMAPS) && texopt->blend_names[BLEND_BUMPMAP1])
                        bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_MULTIPLY, 0);
                    else
                        bind->bind_blend_mode = BlendMode(BLENDMODE_MULTIPLY, 0);
                } else if (texopt->flags & TEXOPT_OLDTINT) {
                    bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(texopt->blend_names[BLEND_ADDGLOW1], black_tex, "AddGlow1", trickFileName, trickName);
                    scale1 = BLEND_ADDGLOW1;
                    bind->bind_blend_mode = BlendMode(BLENDMODE_ADDGLOW, 0);
                } else if (texopt->blend_names[BLEND_ADDGLOW1] || texopt->blend_names[BLEND_DUALCOLOR1]) {
                    bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(texopt->blend_names[BLEND_DUALCOLOR1], white_tex, "DualColor1", trickFileName, trickName);
                    scale1 = BLEND_DUALCOLOR1;
                    if ((rdr_caps.features & GFXF_BUMPMAPS) && texopt->blend_names[BLEND_BUMPMAP1])
                        bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0);
                    else
                        bind->bind_blend_mode = BlendMode(BLENDMODE_COLORBLEND_DUAL, BMB_NV1XWORLD);
                } else if (texopt->scrollsScales.multiply1Reflect && texopt->blend_names[BLEND_MULTIPLY1]) {
                    // Reflect only looks decent on Multiply
                    bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(texopt->blend_names[BLEND_MULTIPLY1], white_tex, "Multiply1", trickFileName, trickName);
                    scale1 = BLEND_MULTIPLY1;
                    bind->bind_blend_mode = BlendMode(BLENDMODE_MULTIPLY, 0);
                } else {
                    // Needs to be at least COLORBLEND_DUAL
                    bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(texopt->blend_names[BLEND_DUALCOLOR1], white_tex, "DualColor1", trickFileName, trickName);
                    scale1 = BLEND_DUALCOLOR1;
                    if ((rdr_caps.features & GFXF_BUMPMAPS) && texopt->blend_names[BLEND_BUMPMAP1])
                        bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0);
                    else
                        bind->bind_blend_mode = BlendMode(BLENDMODE_COLORBLEND_DUAL, BMB_NV1XWORLD);
                }
                fixLightingScalesOnFallback(bind);
                // Setup scales
                bind->bind_scale[0][0] = texopt->scrollsScales.texopt_scale[scale0][0];
                bind->bind_scale[0][1] = texopt->scrollsScales.texopt_scale[scale0][1];
                bind->bind_scale[1][0] = texopt->scrollsScales.texopt_scale[scale1][0];
                bind->bind_scale[1][1] = texopt->scrollsScales.texopt_scale[scale1][1];
            }

            bind->is_fallback_material = 1;

            for (i=TEXLAYER_BUMPMAP1+1; i<TEXLAYER_MAX_LAYERS; i++) 
                bind->tex_layers[i] = NULL;
        }
    } else {
        // Simple/old-style texture type
        if (texopt && texopt->blend_names[BLEND_BASE1])
            bind->tex_layers[TEXLAYER_BASE1] = texFind(texopt->blend_names[BLEND_BASE1]);
        if (!bind->tex_layers[TEXLAYER_BASE1])// Dynamic textures set this themselves, as texFind will fail
            bind->tex_layers[TEXLAYER_BASE1] = texFind(bind->name);
        if (!bind->tex_layers[TEXLAYER_BASE1])
            bind->tex_layers[TEXLAYER_BASE1]=white_tex;

        if ((rdr_caps.features & GFXF_BUMPMAPS) && texopt && texopt->blend_names[BLEND_BUMPMAP1])
            bind->tex_layers[TEXLAYER_BUMPMAP1] = texFind(texopt->blend_names[BLEND_BUMPMAP1]);
        else
            bind->tex_layers[TEXLAYER_BUMPMAP1] = NULL;

        if (texopt && texopt->blend_names[BLEND_OLD_TEX_GENERIC_BLEND])
        {
            // SCD 01/27/2011
            // If a multi material is demoted then it might have gotten the BLEND_OLD_TEX_GENERIC_BLEND
            // set from the DualColor1 or Base1 (In the Base1 case it is just hardcoded to the 'white' texture name)
            // This code previously did not allow a 'Swappable' name for a DualColor1 mapping in this
            // case without an error message. A texFindVerify is more appropriate and follows the
            // practice used to validate other textures that have a fallback when they are specified as 'Swappable'
            bind->tex_layers[TEXLAYER_GENERIC] = texFindVerify(texopt->blend_names[BLEND_OLD_TEX_GENERIC_BLEND], grey_tex, "DualColor1", trickFileName, trickName);
        }
        else
        {
            if(!stricmp(bind->name, "invisible"))
            {
                bind->tex_layers[TEXLAYER_GENERIC] = invisible_tex;
            }
            else
            {
                bind->tex_layers[TEXLAYER_GENERIC] = grey_tex;
            }
        }
        if (texopt) {
            if (texopt->blend_mode == BLENDMODE_BUMPMAP_COLORBLEND_DUAL && !(rdr_caps.features & GFXF_BUMPMAPS)) {
                bind->bind_blend_mode = BlendMode(BLENDMODE_COLORBLEND_DUAL, 0);
            } else {
                bind->bind_blend_mode = BlendMode(texopt->blend_mode, 0);
            }
        } else
            bind->bind_blend_mode = BlendMode(0,0);
        // Promote blend modes that have a bumpmap
        if (bind->tex_layers[TEXLAYER_BUMPMAP1])
        {
            if (bind->bind_blend_mode.shader == 0) {
                bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_MULTIPLY, 0);
            } else if (bind->bind_blend_mode.shader == BLENDMODE_COLORBLEND_DUAL) {
                bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0);
            }
        }
        //originally so non ADDGLOW textures on buildings that had textures with ADDGLOW
        //would not glow, this doesn't work now, becuase it's always the same for all texbinds
        //and no reference through texlinks to the attached object is available
        //so I need to think of another way to fix this...
        if (bind->bind_blend_mode.shader == BLENDMODE_ADDGLOW) //bizzare this is texture flag, it should be preset in texture binds, not here
        {
            if(!stricmp(bind->tex_layers[TEXLAYER_GENERIC]->name, "grey"))
                bind->tex_layers[TEXLAYER_GENERIC] = texFind("black");
        }
        for (i=TEXLAYER_BUMPMAP1+1; i<TEXLAYER_MAX_LAYERS; i++) 
            bind->tex_layers[i] = NULL;

        if (bind->bind_blend_mode.shader == BLENDMODE_COLORBLEND_DUAL) // && texopt && texopt->flags & TEXOPT_TREAT_AS_MULTITEX)
        {
            // This seems to be better for GF2 users - all objects not on a GfxNode get this now
            bind->bind_blend_mode.blend_bits |= BMB_NV1XWORLD;
        }

        if (texopt && texopt->blend_names[BLEND_CUBEMAP] && (rdr_caps.allowed_features & GFXF_CUBEMAP))
            bind->tex_layers[TEXLAYER_CUBEMAP] = texFindVerifyCubemap(texopt->blend_names[BLEND_CUBEMAP], NULL, "CubeMap", trickFileName, trickName);
    }
    assert(bind->tex_layers[TEXLAYER_BASE1]);
    bind->width = bind->tex_layers[TEXLAYER_BASE1]->width;
    bind->height = bind->tex_layers[TEXLAYER_BASE1]->height;

    bind->needs_alphasort = !!texNeedsAlphaSort(bind, bind->bind_blend_mode);

    setSwappableInfo(bind, texopt);

    // Verify data
    if (0) {
        // Turn this on after they're fixed!
        // JE: Can't anymore, as they export normal maps now and process them as normal textures
        BasicTexture *bump;
        bump = bind->tex_layers[TEXLAYER_BUMPMAP1];
        if (bump && !(bump->flags & TEX_BUMPMAP)) {
            ErrorFilenamef(texopt->file_name, "Texture \"%s\" specifies \"%s\" as a bumpmap, but \"%s\" is not a bumpmap", bind->name, bump->name, bump->name);
            bind->tex_layers[TEXLAYER_BUMPMAP1] = dummy_bump_tex;
        }
        bump = bind->tex_layers[TEXLAYER_BUMPMAP2];
        if (bump && !(bump->flags & TEX_BUMPMAP)) {
            ErrorFilenamef(texopt->file_name, "Texture \"%s\" specifies \"%s\" as a bumpmap, but \"%s\" is not a bumpmap", bind->name, bump->name, bump->name);
            bind->tex_layers[TEXLAYER_BUMPMAP2] = dummy_bump_tex;
        }
    }

    assert((rdr_caps.features & GFXF_BUMPMAPS) || !blendModeHasBump(bind->bind_blend_mode));

    if(bind->tex_lod)
    {
        if(bind->scrollsScales)
        {
            // Copy original/newly reloaded data
            if(!bind->tex_lod->allocated_scrollsScales)
            {
                bind->tex_lod->scrollsScales = createScrollsScales();
                bind->tex_lod->allocated_scrollsScales = 1;
            }
            memcpy(bind->tex_lod->scrollsScales, bind->scrollsScales, sizeof(*bind->scrollsScales));
        }
        else if(bind->tex_lod->scrollsScales)
        {
            if(bind->tex_lod->allocated_scrollsScales)
            {
                destroyScrollsScales(bind->tex_lod->scrollsScales);
                bind->tex_lod->allocated_scrollsScales = 0;
            }
            bind->tex_lod->scrollsScales = NULL;
        }
        texResetTrickBasedParametersComposite(bind->tex_lod, true);
    }

}

void texCreateLOD(TexBind *orig_bind)
{
    if (!orig_bind->tex_lod) {
        orig_bind->tex_lod = createTexBind();
        memcpy(orig_bind->tex_lod, orig_bind, sizeof(*orig_bind));
        orig_bind->tex_lod->tex_lod = NULL;
        if (orig_bind->scrollsScales) {
            orig_bind->tex_lod->scrollsScales = createScrollsScales();
            orig_bind->tex_lod->allocated_scrollsScales = 1;
            memcpy(orig_bind->tex_lod->scrollsScales, orig_bind->scrollsScales, sizeof(*orig_bind->scrollsScales));
        }
        texResetTrickBasedParametersComposite(orig_bind->tex_lod, true);
    }
}

void texFreeLOD(TexBind *orig_bid)
{
    if (orig_bid->tex_lod)
    {
        destroyTexBind(orig_bid->tex_lod);
        orig_bid->tex_lod = NULL;
    }
}

void texSetBindsSubComposite(TexBind *bind)
{
    texResetTrickBasedParametersComposite(bind, false);
}

//Set flags and such in the Tex_Binds.  Must be done after all texLoadHeaders because 
//tex_links can refer to binds in any .rom file, so all need to be loaded.
static void texSetBinds(void)
{
    int    i,count;

    //set bind flags and such (done in separate for loop so texbind names are all in place for texfind
    count = eaSize(&g_basicTextures);
    for( i = 0 ; i < count ; i++ )
    {
        texSetBindsSubBasic(g_basicTextures[i], g_basicTextures, i);
    }

    count = eaSize(&g_compositeTextures);
    for( i = 0 ; i < count ; i++ )
    {
        texSetBindsSubComposite(g_compositeTextures[i]);
    }
}

void texResetBinds(void)
{
    int    i,count;

    count = eaSize(&g_basicTextures);
    for( i = 0 ; i < count; i++ )
    {
        if (texResetTrickBasedParametersBasic(g_basicTextures[i], g_basicTextures, i)) {
            printf("reload: texture %s needs to be reloaded from disk because of trick changes\n", g_basicTextures[i]->name);
            texFree(g_basicTextures[i], 0);
        }
    }
    count = eaSize(&g_compositeTextures);
    for( i = 0 ; i < count; i++ )
    {
        texResetTrickBasedParametersComposite(g_compositeTextures[i], false);
    }
}

void texResetAnisotropic(void)
{
    int texAnisotropic = MAX(MIN(game_state.texAnisotropic, rdr_caps.maxTexAnisotropic), 1);
    int i;
    if (rdr_caps.supports_anisotropy)
    {
        rdrQueue(DRAWCMD_SETANISO, &texAnisotropic, sizeof(texAnisotropic));
        for (i=eaSize(&g_basicTextures)-1; i>=0; i--) {
            BasicTexture *bind = g_basicTextures[i];
            if (bind->id && !bind->cube_face_idx) // don't set for cubemap faces (just main cubemap texture)
                rdrQueue( (bind->texopt_flags&TEXOPT_CUBEMAP) ? DRAWCMD_RESETANISO_CUBEMAP:DRAWCMD_RESETANISO, &bind->id, sizeof(bind->id));
        }
    }
}

static char *basefolder=NULL;
static int tex_load_header_count;

// texFillInBind: called from main thread only at load time
int texFillInBind(char *filename, BasicTexture *bind) {
    TextureFileHeader tfh;
    static char *extradata=NULL; // extra space to hold name and mip header stuff
    char *name, *curData;
    char *mipdata;
    int    mipdatasize;
    char *s;
    int numread=0,logging, sizeRemaining;
    StashElement    element;
    FILE *tex_file;
    char        buf[1000];

    #define MAX_HEADER_SIZE 1024 // largest texture header we expect to run across (fyi largest so far is 570 as of 1/13/11)

    if (!extradata) {
        extradata = calloc(1, MAX_HEADER_SIZE);
    }

    bind->flags &= ~(TEX_ALPHA|TEX_BUMPMAP|TEX_TGA|TEX_DDS|TEX_JPEG); // clear flags set in this function

    logging = fileSetLogging(0);
    tex_file = fileOpen(filename, "rbR");
    fileSetLogging(logging);
    if (!tex_file) {
        Errorf("Error opening texture file '%s'!", filename);
        return 1;
    }

    tex_load_header_count++;

    // Read the data
    numread=fread(&tfh, 1, sizeof(tfh), tex_file);
    if (numread<sizeof(tfh)) {
        Errorf("Error loading texture file '%s'!", filename);
        return 1;
    }

    curData = extradata;
    assert(tfh.header_size > sizeof(tfh));
    sizeRemaining = tfh.header_size - sizeof(tfh); // header_size includes base TextureFileHeader
    if (sizeRemaining > MAX_HEADER_SIZE) {
        Errorf("Error loading texture file '%s'!  Too large of cached mipmap data, need to bump MAX_HEADER_SIZE.", filename);
        return 1;
    }

    // read remaining texture header data
    numread=fread(curData, 1, sizeRemaining, tex_file);
    if (numread < sizeRemaining) {
        Errorf("Error loading texture file '%s'!", filename);
        return 1;
    }

    name = curData; // First bytes of extradata is the name
    curData += strlen(name)+1;
    mipdata = NULL;
    mipdatasize = 0;
    if (tfh.pad[0]=='T' && tfh.pad[1]=='X' && tfh.pad[2]=='2') {
        // Texture file v2
        numread = sizeof(tfh) + strlen(name)+1;
        if (numread < tfh.header_size) {
            // There's extra data, must be mip map data to preload
            mipdata = curData; // Pointer to begining of mip header data
            mipdatasize = tfh.header_size - numread;
        }
    }

    fclose(tex_file);

    //Get bind->file_pos and bind->file_bytes ints
    bind->file_pos = tfh.header_size;
    bind->file_bytes = tfh.file_size;

    //Get bind->dir_name, and bind->name
    s = strstri(name, basefolder);
    if (!s) {
        assert(0); // Never happens, but should be fine anyway?
        s = name;
    } else {
        s = name + strlen(basefolder);
        while (*s=='/') s++;
    }
    strcpy(buf,s);
    s = strrchr(buf,'/');
    if (s)
        *s = 0;
    bind->dirname = allocAddString(buf);

    s = strrchr(name,'/');
    if (!s) {
        s = name;
        assert(0); // Never happens, but should be fine anyway?
    } else {
        s++;
        assert(*s); // if the texture name ended in a /, we've got problems
    }
    bind->flags |= texFixName(s,s,-1);
    if (!stashFindElement(g_basicTextures_ht, s, &element))
    {
        stashAddPointerAndGetElement(g_basicTextures_ht, s, bind, false, &element);
    }
#ifndef FINAL
    else
    {
        BasicTexture *old = stashElementGetPointer(element);
        if (stricmp(old->dirname, bind->dirname)!=0)
        {
            char fn1[MAX_PATH];
            char fn2[MAX_PATH];
            char la1[128];
            char la2[128];
            char *newestFile;
            U32 timestamp1, timestamp2;
            sprintf(fn1, "texture_library/%s/%s.texture", old->dirname, s);
            strcpy(la1, "UNKNOWN");
            timestamp1 = fileLastChanged(fn1);
            sprintf(fn2, "texture_library/%s/%s.texture", bind->dirname, s);
            strcpy(la2, "UNKNOWN");
            timestamp2 = fileLastChanged(fn2);
            if (timestamp1 > timestamp2) {
                newestFile = fn1;
            } else {
                newestFile = fn2;
            }
            ErrorFilenamef(newestFile, "Duplicate texture %s found in both of these paths:\n  %s (%s)\n  %s (%s)\n", s, old->dirname, la1, bind->dirname, la2);
        }
    }
#endif
    bind->name = stashElementGetStringKey(element);

    if (tfh.width>MAX_TEX_SIZE || tfh.height>MAX_TEX_SIZE)
        ErrorFilenamef(filename,"Texture Too big - Max tex size is (%dX%d) this one is (%dX%d)\n", MAX_TEX_SIZE,MAX_TEX_SIZE,tfh.width,tfh.height);
    if( (tfh.width<=0 || tfh.height<=0) || tfh.alpha<0 || tfh.alpha>1 )
        FatalErrorFilenamef(filename,"GETTEX running or Bad Texture\n");

    if(tfh.alpha && !(tfh.flags & TEXOPT_FADE))
        bind->flags |= TEX_ALPHA;
    if(tfh.flags & TEXOPT_BUMPMAP)
        bind->flags |= TEX_BUMPMAP;

    /*hack for bunch of textures with invalid flags*/ // fpe added
    if(tfh.flags & TEXOPT_CUBEMAP && strstriConst(bind->name, "cubemap"))
        bind->flags |= TEX_CUBEMAPFACE;

    assert((tfh.flags & TEXOPT_JPEG) || !(bind->flags & TEX_JPEG)); // If the texture was built with TEXOPT_JPEG, then the flags should include TEX_JPEG
    bind->height = tfh.height;
    bind->width = tfh.width;
    bind->realHeight = 1 << log2i(bind->height);
    bind->realWidth = 1 << log2i(bind->width);

    assert(bind->height <= bind->realHeight);
    assert(bind->width <= bind->realWidth);

    {
        TexOpt    *texopt;
        texopt = trickFromTextureDirName(bind->dirname,bind->name,&bind->texopt_flags);
        if (texopt)
        {
            bind->texopt_surface = texopt->surface;
        } 
        else 
            bind->texopt_surface = 0;

    }

    // Store mip header data on the bind
    if (mipdatasize) {
        bind->mipdata = malloc(mipdatasize);
        memcpy(bind->mipdata, mipdata, mipdatasize);
        bind->mipsize = mipdatasize;
    } else {
        bind->mipdata = NULL;
        bind->mipsize = 0;
    }

    bind->mipid = 0;
    return 0;
}

static FileScanAction texLoadHeaderProcessor(char *dir, struct _finddata32_t *data)
{
    BasicTexture *bind;
    static char *ext = ".texture";
    static int ext_len = 8; // strlen(ext);
    char filename[MAX_PATH];
    int len;

    if (data->name[0]=='_') return FSA_NO_EXPLORE_DIRECTORY;
    if (data->attrib & _A_SUBDIR)
        return FSA_EXPLORE_DIRECTORY;
    len = strlen(data->name);
    if (len<ext_len || strnicmp(data->name + len - ext_len, ext, ext_len)!=0) // not a .texture file
        return FSA_EXPLORE_DIRECTORY;

    // special case for LWC in stages after stage 1: may have already loaded this texture for a previous stage, so ignore it here
    if(LWC_IsActive() && LWC_GetLastCompletedDataStage() > LWC_STAGE_1) {
        BasicTexture *match = 0;
        char * c;

        strcpy(filename, data->name);
        c = strrchr(filename, '.');
        if(c) *c = 0;
        if (stashFindPointer(g_basicTextures_ht, filename, &match)) {
            return FSA_EXPLORE_DIRECTORY;
        }
    }

    STR_COMBINE_SSS(filename, dir, "/", data->name);

    MP_CREATE(BasicTexture, 2048);
    bind = MP_ALLOC(BasicTexture);

    if (0==texFillInBind(filename, bind)) {
        // Check if the name is the same as the filename
        char nametest[128];
        strcpy(nametest, data->name);
        *(strrchr(nametest, '.'))=0;
        if (stricmp(nametest, bind->name)!=0 && strlen(bind->name)>0) {
            FatalErrorf("File \"%s\" is misnamed, containing texture named \"%s\"!", filename, bind->name);
        }
        eaPush(&g_basicTextures, bind);
    }

    return FSA_EXPLORE_DIRECTORY;
}

// Load texture headers from disk.  Called at startup, and also when new piggs get loaded during gameplay (LWC).
//    After loading new textures, do secondary initialization for those textures here as well (eg create composites,
//    tricks, texture swaps, etc)
void texScanForTexHeaders(bool bInitialLoad)
{
    fileScanAllDataDirs(basefolder, texLoadHeaderProcessor);

    // TBD, clean up the rest of this function to make it work better for initial load vs stages data loads
    //    Eg should have a list of basictextures that were just loaded in addition to the full gBasicTexture list, 
    //    and the functions below here operate on the newly loaded textures only.  Might work now with all textures, but
    //    wasting time redoing whats already been set up for the previously loaded textures.

    if(bInitialLoad)
        texFindNeededBinds();

    // Create default CompositeTextures for each BasicTexture
    texCreateCompositeTextures();

    // Load textures from tricks
    if(bInitialLoad)
        trickCreateTextures();

    // Swap in localized versions of the textures
    texCheckSwapList();

    texSetBinds(); //must be done right after all headers are loaded
}

int texGetNumBasicTextures(void)
{
    return eaSize(&g_basicTextures);
}

static FILE* gettex_lock_handle=NULL;
static void releaseGettexLock() {
    fclose(gettex_lock_handle);
    gettex_lock_handle = 0;
}

static void waitForGettexToFinish() {
    char *lockfilename = "c:\\gettex.lock";
    int pm=0;
    fileForceRemove(lockfilename);
    while ((gettex_lock_handle = fopen(lockfilename, "wl")) == NULL) {
        if (!pm) {
            printf("Gettex still running, waiting for it to finish...");
            pm=1;
        }
        Sleep(100);
        fileForceRemove(lockfilename);
    }
    if (pm) {
        printf(" done.\n");
    }
    if (gettex_lock_handle > 0) {
        fprintf(gettex_lock_handle, "Ouroboros.exe\r\n");
    }
}


static void reloadTextureCallback(const char *relpath, int when) {
    char texname[MAX_PATH];
    char filename[MAX_PATH];
    char *s;
    BasicTexture *bind;
    bool isCubemap = false;

    if (strstr(relpath, "/_")) return;
    if (dirExists(relpath)) return; // This should only happen if there's a folder with named something.texture!
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    strcpy(filename, relpath);
    strcpy(texname, relpath);
    s = texname;
    if (strrchr(texname, '/')) {
        s = strrchr(texname, '/') + 1;
    }
    if (strrchr(s, '.')) {
        *strrchr(s, '.')=0;
    }
    bind = texFind(s);
    if (!bind) {
        bind = MP_ALLOC(BasicTexture);

        waitForGettexToFinish();
        if (0==texFillInBind(filename, bind)) {
            // Check if the name is the same as the filename
            if (stricmp(s, bind->name)!=0) {
                printf("File \"%s\" is misnamed, containing texture named \"%s\" - not reloading!\n", relpath, bind->name);
            } else {
                texSetBindsSubBasic(bind, NULL, 0);
                texCheckForSwaps(bind);
                eaPush(&g_basicTextures, bind);
                if (!texFindComposite(s)) {
                    texCreateCompositeTextureFromBasic(bind);
                    texSetBindsSubComposite(texFindComposite(s));
                }
                // We added a new texture, reload models in case they need them!
                modelRebuildTextures();
            }
        }
        releaseGettexLock();
    } else {
        // Save this information before it gets wiped out
        isCubemap = (bind->target == GL_TEXTURE_CUBE_MAP_ARB) ||
                    (bind->target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB && bind->target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB);

        if (bind->load_state[0] != TEX_LOADING) { // If the texture is loading, don't re-load it
            if (stricmp(bind->name, "white")!=0 &&
                stricmp(bind->name, "grey")!=0 &&
                stricmp(bind->name, "invisible")!=0 &&
                stricmp(bind->name, "black")!=0)// Don't free white (or a bad texture that happens to be filled in with White's data)
            {
                texFree(bind, 0);
                texFree(bind, 1);
            } else {
                bind->id = 0;
            }
            waitForGettexToFinish();
            if (0==texFillInBind(filename, bind)) { // If this was deleted, then it became white, so we want to re-fill it with useful info
                texSetBindsSubBasic(bind, NULL, 0);
            }
            {
                // Need to update width/heigh/etc on TexBind
                TexBind *compositeBind = texFindComposite(bind->name);
                if (compositeBind) {
                    compositeBind->width = bind->width;
                    compositeBind->height = bind->height;
                }
            }
            texCheckForSwaps(bind);
            releaseGettexLock();
        }
    }

    // For cubemaps, force all faces to reload together
    if (isCubemap)
    {
        char* tempName = (char*)bind->name;
        int bindFaceNumPosition = strlen(tempName) - 1, i;
        const char old_bind_face_char = tempName[bindFaceNumPosition];
        int filenameFaceNumPosition = strlen(filename);

        while (filenameFaceNumPosition >= 0 && filename[filenameFaceNumPosition] != old_bind_face_char)
            filenameFaceNumPosition--;

        if (filenameFaceNumPosition >= 0)
        {
            // Go from 0 to 6 inclusive so we catch both cases where the faces are numbered 0-5 or numbered 1-6
            for(i=0; i<=6; i++)
            {
                BasicTexture *face_bind;
                tempName[bindFaceNumPosition] = '0' + i;
                face_bind = texFind(tempName);

                if (face_bind && face_bind->load_state[0] == TEX_LOADED)
                {
                    filename[filenameFaceNumPosition] = '0' + i;

                    // recursion!
                    reloadTextureCallback(filename, when);
                }
            }
        }
        tempName[bindFaceNumPosition] = old_bind_face_char; // restore name to incoming state
        filename[filenameFaceNumPosition] = old_bind_face_char;    // probably not necessary
    }
}

static void texCreateCompositeTextureFromBasic(BasicTexture *basicBind)
{
    StashElement    element;
    TexBind *bind;
    bind = createTexBind();
    bind->dirname = basicBind->dirname;
    bind->name = basicBind->name;
    bind->orig_bind = bind;
    // Everything else is filled in in texSetBindsSubComposite
    if (!stashFindElement(g_compositeTextures_ht, bind->name, &element)) {
        eaPush(&g_compositeTextures, bind);
        stashAddPointerAndGetElement(g_compositeTextures_ht, bind->name, bind, false, &element);
    } else {
        destroyTexBind(bind);
    }
}

static void texCreateCompositeTextures()
{
    int i;
    int count=eaSize(&g_basicTextures);

    for (i=0; i<count; i++) {
        texCreateCompositeTextureFromBasic(g_basicTextures[i]);
    }
}


/* Given a path to textures, load all .texture files in the path
     This should only be called once on init
     It loads into the global "g_basicTextures" and "g_compositeTextures"
*/
int texLoadHeaders(void)
{
    int estimatedBinds = 40000; // Estimated number of binds for Issue 21
    assert(g_basicTextures==NULL); // This should only be called once
    assert(g_basicTextures_ht==0);
    assert(g_compositeTextures==NULL); // This should only be called once
    assert(g_compositeTextures_ht==0);

    // Enable debug tracking of texture loads in different threads
    memlog_enableThreadId(NULL);

    // Allocate an array to store all of the binds
    eaCreate(&g_basicTextures);
    eaSetCapacity(&g_basicTextures, estimatedBinds);
    eaCreate(&g_compositeTextures);
    eaSetCapacity(&g_compositeTextures, estimatedBinds);

    // Read into each bind:
    tex_load_header_count = 0;
    basefolder = "texture_library";

    writeConsole(OUTPUT_DEBUG, "Loading texture headers");

    // Load basic textures from disk
    g_basicTextures_ht = stashTableCreateWithStringKeys(estimatedBinds, StashDeepCopyKeys);
    g_compositeTextures_ht = stashTableCreateWithStringKeys(estimatedBinds, StashDefault); // Shallow copy

    texScanForTexHeaders(true);

    // Add callback for re-loading textures
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "texture_library/*.texture", reloadTextureCallback);

    writeConsole(OUTPUT_INFO, "Loaded texture headers");
    
    return tex_load_header_count;
}

/* unloads all texbinds from the graphics card */
void texFreeAll()
{
    // Just want to release them from GL
    int i;
    for (i=0; i<eaSize(&g_basicTextures); i++) {
        texFree(g_basicTextures[i], 0);
        texFree(g_basicTextures[i], 1);
    }
}

void texFree( BasicTexture *bind, int freeRawData )
{
    bool freed=false;
    
    if (bind->load_state[freeRawData] == TEX_LOADED) {
        //printf("freeing tex: %s\n", bind->name);

        if (freeRawData) {
            if (bind!=white_tex && bind->rawReferenceCount==0) {
                memlog_printf(0, "%u: texFree RAW %s", game_state.client_frame_timestamp, bind->name);
                // Free raw data
                if (bind->rawInfo)
                {
                    SAFE_FREE(bind->rawInfo->data);
                    SAFE_FREE(bind->rawInfo);
                }
                freed=true;
            }
        } else {
            // Free GL data
            if (bind->id > 0) { // not 0
                memlog_printf(0, "%u: texFree %s", game_state.client_frame_timestamp, bind->name);
                memMonitorTrackUserMemory(TEXTURE_MEMMONITOR_NAME, 1, MOT_FREE, bind->memory_use[0]);
                rdrTexFree(bind->id);
                bind->hasBeenComposited = 0;
                bind->id = 0;
                if (bind->origWidth) {
                    bind->width = bind->origWidth;
                    bind->height = bind->origHeight;
                }
                freed=true;

                // Mac only until we're sure this is stable.
                if (IsUsingCider() && bind->mipid > 0)
                {
                    rdrTexFree(bind->mipid);
                    bind->mipid = 0;
                }
            }
        }
        if (freed) {
            bind->load_state[freeRawData] = TEX_NOT_LOADED;
            texMemoryUsage[freeRawData] -= bind->memory_use[freeRawData];
            bind->memory_use[freeRawData] = 0;
        }
    }
}

bool texRemoveRefBasic( BasicTexture *basicBind )
{
    if (basicBind && basicBind->texWordParams) {
        // This is in fact a dynamic texture
        if (basicBind->load_state[0] == TEX_LOADING) {
            // Wait for it to finish loading?  Kill it when it's done?
            // TODO
        } else {
            // Unloaded or fully loaded
            texFree(basicBind, 0);
            eaRemove(&g_basicTextures, eaFind(&g_basicTextures, basicBind));
            destroyTexWordParams(basicBind->texWordParams);
            MP_FREE(BasicTexture, basicBind);
            return true;
        }
    }
    return false;
}

// For dynamic textures, used to say a GroupDef is done with it
void texRemoveRef( TexBind *bind )
{
    if (texRemoveRefBasic(bind->tex_layers[TEXLAYER_BASE]))
        destroyTexBind(bind);
}


// Returns ptr to the Basic TexBind with the given name or zero. 
BasicTexture *texFind(const char *name)
{
    char search[128];
    BasicTexture *match = 0;

    assert(name!=(char*)0xfafafafa);

    if (!name)
        return NULL;

    assert(*(int*)name!=0xfafafafa);

    if (name[0]=='!')
        name++;

    texFixName(name, search, ARRAY_SIZE(search));
    if (!stashFindPointer(g_basicTextures_ht, search, &match)) {
        //printf("Warning: Missing texture: '%s'\n", name);
        return NULL;
    }
    return match;
}

BasicTexture *texFindRandom(void)
{
    BasicTexture *match = 0;
    StashTableIterator iter;
    StashElement elem;
    int i = 0, idx = getCappedRandom(stashGetValidElementCount(g_basicTextures_ht));
    stashGetIterator(g_basicTextures_ht, &iter);
    do
    {
        stashGetNextElement(&iter, &elem);
        match = stashElementGetPointer(elem);
    } while (++i < idx);
    return match;
}

char *texCheckForSceneMaterialSwap(char *name)
{
    int i;
    int count;
    TexSwap **ts = sceneGetMaterialSwaps(&count);
    if (!name)
        return NULL;
    for (i=0; i<count; i++) {
        if (stricmp(name, ts[i]->src)==0) {
            return ts[i]->dst;
        }
    }
    return name;
}

// Returns ptr to the Composite TexBind with the given name or zero. 
TexBind *texFindComposite(const char *name)
{
    char search[128];
    TexBind *match = 0;

    PERFINFO_AUTO_START("texFindComposite", 1);
    if (name[0]=='!')
        name++;
    texFixName(name, search, ARRAY_SIZE(search));

    // Check for scene material swaps
    name = texCheckForSceneMaterialSwap(search);

    if (!stashFindPointer(g_compositeTextures_ht, name, &match)) {
        //printf("Warning: Missing texture: '%s'\n", name);
        PERFINFO_AUTO_STOP();
        return NULL;
    }
    PERFINFO_AUTO_STOP();
    return match;
}


/*Callback that the background loader is given to do
    this is called from any thread
    if we are queueing, it adds the package to the list
    if we are loading on demand (or loading the queued files), we load immediately
*/
static VOID CALLBACK texDoThreadedTextureLoading( ULONG_PTR dwParam)
{
    TexThreadPackage * pkg;

    PERFINFO_AUTO_START("texDoThreadedTextureLoading", 1);
    
    pkg = (TexThreadPackage*)dwParam;

    //debug printf("texDoThreadedTextureLoading(%s)\n", pkg->bind.dirname);

    if (pkg->should_queue==1 || (pkg->should_queue==-1 && queuingTexLoads)) {
        EnterCriticalSection(&CriticalSectionTexLoadQueues); 
        //printf("queuing tex: %s\n", pkg->bind->name);
        listAddForeignMember(&queuedTexLoads, pkg);
        LeaveCriticalSection(&CriticalSectionTexLoadQueues);
    } else {
        bool addToFinalList=true;
        if (!pkg->needRawData && pkg->bind->texWord) {
            // All of this texture's depencies are loaded, let the texWords system know
            //  so it can do the work
            if (texWordDoThreadedWork(pkg, background_loader_threadID==GetCurrentThreadId())) {
                // We're done, don't load anything else for this texture
                addToFinalList = false;
            }
        }
        if (addToFinalList) {
            // Either a plain texture or a texWord that needs it's GL data loaded
            PERFINFO_AUTO_START("texLoadData", 1);
                texLoadData(pkg);
            PERFINFO_AUTO_STOP();

            EnterCriticalSection(&CriticalSectionTexLoadQueues);
            listAddForeignMember(&texBindsReadyForFinalProcessing, pkg);
            LeaveCriticalSection(&CriticalSectionTexLoadQueues);
        }
    }
    InterlockedDecrement(&numTexLoadsInThread);

    PERFINFO_AUTO_STOP();
}

//static int pkgCompareFileLoc(TexThreadPackage * * elem1, TexThreadPackage * * elem2) {
//
//    if (!(*elem1)->bind.texWord == !(*elem2)->bind.texWord) {
//        // Sort by filename (assume piggs are alphabetical and faster to load that way?)
//        return stricmp((*elem1)->bind.dirname, (*elem2)->bind.dirname);
//    } else {
//        // One of these has a TexWord, it needs to be loaded last because of 
//        //  dependencies
//        return !(*elem2)->bind.texWord - !(*elem1)->bind.texWord;
//    }
//}

/*
    this is now only called from main thread
    sorts the texture packages, and then passes them to texLoadTexLinks
*/
static void texDoThreadedQueuedTextureLoading( void )
{
    TexThreadPackage * pkg;
    TexThreadPackage * prevPkg;
    int totaldata=0;
    int pixelsStart=texWordGetTotalPixels();
    int count = 0;

    // JE: Cannot sort trivially, as the TexWords system may have queued up dependencies to be
    //   loaded before doing compositing (could make a seperate queue for compositing after all
    //   textures are loaded)
//    // Sort list before loading!
//    listQsort(&queuedTexLoads,
//        (int (__cdecl *)(const void **, const void **))pkgCompareFileLoc);

    // We need to start on the earliest added texture (the one at the end of the list),
    //  because TexWords may have queued up some dependencies to be loaded first

    // find end
    for (pkg = queuedTexLoads; pkg && ++count && pkg->next; pkg = pkg->next);

    writeConsole(OUTPUT_DEBUG, "Loading %i textures", count);
    
    int i = count;
    for( ; pkg ; pkg = prevPkg, i-- )
    {
        bool addToFinalList=true;
        prevPkg = pkg->prev;
        listRemoveMember(pkg, &queuedTexLoads);

        if (!pkg->needRawData && pkg->bind->texWord) {
            // All of this texture's depencies are loaded, let the texWords system know
            //  so it can do the work
            if (texWordDoThreadedWork(pkg, background_loader_threadID==GetCurrentThreadId())) {
                // We're done, don't load anything else for this texture
                addToFinalList = false;
            }
        }
        if (addToFinalList) {
            // Either a plain texture or a texWord that needs it's GL data loaded
            PERFINFO_AUTO_START("texLoadData", 1);
                texLoadData(pkg);
            PERFINFO_AUTO_STOP();

            totaldata+=pkg->info.size;

            EnterCriticalSection(&CriticalSectionTexLoadQueues); 
            listAddForeignMember(&texBindsReadyForFinalProcessing, pkg);
            LeaveCriticalSection(&CriticalSectionTexLoadQueues);
        }
        texCheckThreadLoader();
    }
    
    assert(!queuedTexLoads && !i);

    writeConsole(OUTPUT_INFO, "Loaded %i textures (%.3f Mbytes %.3f TexWord MPixels)", count, totaldata / 1000000.0f, (texWordGetTotalPixels() - pixelsStart) / 1000000.0f);
};

long texLoadsPending(int include_misc)
{
    return numTexLoadsInThread + (include_misc ? texWordGetLoadsPending() : 0);
}

void texSetupParamsFromBind(BasicTexture *tex_bind, TexReadInfo *info,RdrTexParams *rtex)
{
    TexOptFlags texopt_flags;

    memset(rtex,0,sizeof(*rtex));
    texopt_flags = tex_bind->texopt_flags;

#if 0 // JE: These aren't ever used!
    if (tex_bind->flags & (TEX_TGA|TEX_JPEG))
    {
        tex_bind->flags |= TEX_RGB8;
    }
    else
    {
        if (info->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
            tex_bind->flags |= TEX_COMP4;
        else if (info->format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || info->format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
            tex_bind->flags |= TEX_COMP8;
        else if (info->format == TEXFMT_ARGB_8888 || info->format == TEXFMT_ARGB_0888)
            tex_bind->flags |= TEX_RGB8;
    }
#endif

    if (texopt_flags & TEXOPT_JPEG)
        rtex->src_format = GL_RGB;
    else
    {
        rtex->dxtc = 1;
        rtex->src_format = info->format;
    }
    rtex->width            = info->width;
    rtex->height        = info->height;
    rtex->id            = tex_bind->id;
    rtex->target        = tex_bind->target;
    rtex->dst_format    = GL_RGBA8;

    if (texopt_flags & TEXOPT_CLAMPS)
        rtex->clamp_s = 1;
    else if (texopt_flags & TEXOPT_MIRRORS)
        rtex->mirror_s = 1;
    if (texopt_flags & TEXOPT_CLAMPT)
        rtex->clamp_t = 1;
    else if (texopt_flags & TEXOPT_MIRRORT)
        rtex->mirror_t = 1;
    if (texopt_flags & TEXOPT_BORDER)
        rtex->tex_border = 1;
    if (texopt_flags & TEXOPT_POINTSAMPLE)
        rtex->point_sample = 1;
    rtex->mip_count = info->mip_count;
    if ((!(texopt_flags & TEXOPT_NOMIP)) && info->mip_count)
        rtex->mipmap = 1;
}

static int texConvertToGameFormat(BasicTexture *tex_bind, TexReadInfo * info)
{
    RdrTexParams    rtex;
    static DWORD threadid = 0;

    if (!threadid)
        threadid = GetCurrentThreadId();
    if (threadid != GetCurrentThreadId()) {
        assert(!"texLoad thinks it's in the wrong thread!  Tell Jimb!");
        return -1;
    }
    texSetupParamsFromBind(tex_bind,info,&rtex);
    if( tex_bind->target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB && tex_bind->target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB )
    {
        // id should have already been assigned by composite texture that owns the cubemap faces
        assert(tex_bind->id);
        rtex.id = tex_bind->id;
    }
    else
        rtex.id = tex_bind->id = rdrAllocTexHandle();
    rdrTexCopy(&rtex,info->data,info->size);

    return 1;
}

/*Called every frame to check to see if the background loader has finished it's work (TO DO the linked list 
won't work in multi thread with out some work)*/

void texCheckThreadLoader()
{
    static BasicTexture **eaTexBindsThisTick=NULL;

    TexThreadPackage *tail;
    TexThreadPackage *orgList;
    TexThreadPackage *pkg;//, *prevPkg;
    int        num_bytes_loaded = 0;
    int        did_something = 0;

    PERFINFO_AUTO_START("texCheckThreadLoader", 1);

    if (!texBindsReadyForFinalProcessing)
    {
        PERFINFO_AUTO_STOP();
        return;
    }
        
    EnterCriticalSection(&CriticalSectionTexLoadQueues); 
    orgList = texBindsReadyForFinalProcessing;
    texBindsReadyForFinalProcessing = NULL;
    LeaveCriticalSection(&CriticalSectionTexLoadQueues);

    // We need to start on the earliest added texture (the one at the end of the list),
    //  because TexWords may have queued up some dependencies to be loaded first

    // find the last element
    assert(orgList->prev == NULL);    // sanity check
    tail = orgList;
    while (tail->next)
    {
        assert(tail->next->prev == tail);    // sanity check
        tail = tail->next;
    }

    for(pkg = tail; pkg ; pkg = tail)
    {
        tail = pkg->prev;
        
        if (pkg->needRawData && pkg->info.data) {
            if (!pkg->bind->rawInfo) {
                // Something horrible has gone wrong?  Texture freed while in the queue to get loaded?
            } else {
                num_bytes_loaded += pkg->bind->rawInfo->size;
                did_something = 1;
            }
        }
        else if (pkg->info.data)
        {
            eaPush(&eaTexBindsThisTick, pkg->bind);
        
            PERFINFO_AUTO_START("texConvertToGameFormat", 1);
                texConvertToGameFormat(pkg->bind, &(pkg->info));
            PERFINFO_AUTO_STOP();
            
            num_bytes_loaded += pkg->bind->file_bytes;
            did_something = 1;
        }
        else if (pkg->bind->texWordParams) 
        {
            // TexWord package, no actual data
            eaPush(&eaTexBindsThisTick, pkg->bind);
            did_something = 1;
        }
        else if (pkg->bind->texWord && !pkg->needRawData)
        {
            // TexWord package, data stored elswhere
            eaPush(&eaTexBindsThisTick, pkg->bind);
            did_something = 1;
        }
        else
        {
            memlog_printf(0, "%u: BAD TEXTURE %s", game_state.client_frame_timestamp, pkg->bind->name);
            printf("Missing/bad texture %s \n  (were files deleted while you were running?)\n", pkg->bind->name);
            *pkg->bind = *white_tex;
        }

        pkg->bind->last_used_time_stamp[pkg->needRawData] = game_state.client_frame_timestamp;
        pkg->bind->load_state[pkg->needRawData] = TEX_LOADED;

        if (!pkg->needRawData && pkg->info.data)
        {
            SAFE_FREE(pkg->info.data);
        }

        free(pkg);
    }

    PERFINFO_AUTO_START("bottom stuff", 1);
        //Removing this from the critical section and list management simplifies things, I think.
        {
            int num = eaSize(&eaTexBindsThisTick);
            int i;
            for (i=0; i<num; i++) 
            {
                BasicTexture *bind = eaTexBindsThisTick[i];
                // texCheckForSwaps( bind ); Don't need this anymore?  We only load actualTextures now?

                if (bind->texWord) {
                    assert(!bind->hasBeenComposited);
                    // This is where we composite our textures
                    PERFINFO_AUTO_START("texWordDoFinalComposition", 1);
                        texWordDoFinalComposition(bind->texWord, bind);
                    PERFINFO_AUTO_STOP();
                    
                    bind->hasBeenComposited = 1;
                    bind->load_state[0] = TEX_LOADED;
                    bind->memory_use[0] = bind->width * bind->height * 4; // TODO: fix this up to be accurate
                    memMonitorTrackUserMemory(TEXTURE_MEMMONITOR_NAME, 1, MOT_ALLOC, bind->memory_use[0]);
                    texMemoryUsage[0] += bind->memory_use[0];
                }
            }
            eaSetSize(&eaTexBindsThisTick, 0);
        }
    PERFINFO_AUTO_STOP();
    
    PERFINFO_AUTO_START("loadUpdate", 1);
        loadUpdate("bg_textures",num_bytes_loaded);
    PERFINFO_AUTO_STOP();

    // We have just loaded a texture, let's check to see if any old ones can be removed
    if (did_something) {
        PERFINFO_AUTO_START("texUnloadTexturesToFitMemory", 1);
            texUnloadTexturesToFitMemory();
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_STOP();
}

static int hasStartedQueuing=0;

static VOID CALLBACK texLoadQueueStartDummy( ULONG_PTR useless) {
    PERFINFO_AUTO_START("texLoadQueueStartDummy", 1);
    queuingTexLoads++;
    if (queuingTexLoads==1) {
        // printf("Queueing of graphics load calls starting.\n");
        EnterQueueingLoadsCS();
        assert(!hasStartedQueuing);
        hasStartedQueuing=1;
    }
    PERFINFO_AUTO_STOP();
}
void texLoadQueueStart(void)  // Directs the loader to queue successive load calls
{
    static bool inited=false;
    if (!inited) {
        inited = true;
        InitializeCriticalSection(&CriticalSectionQueueingLoads);
    }

    hasStartedQueuing = 0;
    QueueUserAPC(texLoadQueueStartDummy, background_loader_handle, (ULONG_PTR)NULL );
    queuingTexLoadsOutOfThread++;
}

static VOID CALLBACK texLoadQueueFinishDummy( ULONG_PTR useless ) // Directs the loader to stop queueing load calls and begin loading all queued graphics
{
    PERFINFO_AUTO_START("texLoadQueueFinishDummy", 1);
    assert(queuingTexLoads>0);
    queuingTexLoads--;
    if (queuingTexLoads>0) {
        PERFINFO_AUTO_STOP();
        return;
    }
    // printf("Queueing of graphics load calls done, actually loading graphics...\n");

    // now done in main thread: texDoThreadedQueuedTextureLoading(); // Load in FG change
    LeaveQueueingLoadsCS();
    PERFINFO_AUTO_STOP();
}

static int texCheckForSceneSwaps(BasicTexture *texbind) {
    TexSwap **swaplist;
    int count, j;
    swaplist = sceneGetTexSwaps(&count);

    // NOTE: Any changes to the texture swapping logic should also be made in checkForTextureSwaps() in Game\entity\entityclient.c
    for (j=count-1; j>=0; j--) // Top down to allow overriding
        if (stricmp(texbind->name, swaplist[j]->src)==0) {
            if (texbind->load_state[0]==TEX_NOT_LOADED) {
                // Never loaded originally, just set up the pointer
                texbind->actualTexture = texFind(swaplist[j]->dst);
                if (!texbind->actualTexture)
                {
                    if (!quickload)
                        Errorf("*** TexSwap Warning: error finding texture '%s' to swap with '%s'\n", swaplist[j]->dst, swaplist[j]->src);
                    texbind->actualTexture = white_tex;
                }
            } else {
                texbind->actualTexture = texLoadBasic(swaplist[j]->dst, TEX_LOAD_IN_BACKGROUND, texbind->use_category);
                if (!texbind->actualTexture)
                {
                    if (!quickload)
                        Errorf("*** TexSwap Warning: error finding texture '%s' to swap with '%s'\n", swaplist[j]->dst, swaplist[j]->src);
                    texbind->actualTexture = white_tex;
                }
            }
            if (texbind->actualTexture == white_tex) {
                if (!quickload)
                    Errorf("*** TexSwap Warning: error finding texture '%s' to swap with '%s'\n", swaplist[j]->dst, swaplist[j]->src);
            } else {
                verbose_printf("Swapping texture '%s' for '%s'\n", swaplist[j]->src, swaplist[j]->dst);
            }
            if (texbind->load_state[0]==TEX_NOT_LOADED) {
                // This is no longer true, since textures are loaded more dynamically
                //printf("*** TexSwap Warning: texture '%s' was never originally referenced in scene.\n", swaplist[j]->src);
            }
            return j;
        }
    return -1;
}

// Order of applying texture permutations:
//   Start with base texture name
//   Apply scene file swaps
//   Take result from last step and apply language/search path swaps
//   Take base name from last step and look for associated Text overlays

char **textureSearchPath=NULL;
int numTextureSearchPaths=0;
void texSetSearchPath(char *searchPath)
{
    // Expects something like "#English", "#French;#Base" or just ""
    char *walk;
    while (walk = eaPop(&textureSearchPath)) 
        free(walk);
    walk = strtok(searchPath, ";, \t");
    while (walk) {
        while (*walk=='#') walk++;
        eaPush(&textureSearchPath, strdup(walk));
        walk = strtok(NULL, ";, \t");
    }
    numTextureSearchPaths = eaSize(&textureSearchPath);
}

static int texCheckForSwaps(BasicTexture *texbind) {
    char buf[MAX_PATH];
    int ret;
    int i;
    assert(texbind != (void*)0xfafafafa);
    assert(texbind->name != (void*)0xfafafafa);
    assert(*(int*)texbind->name != 0xfafafafa);
    texbind->actualTexture = texbind;
    ret = texCheckForSceneSwaps(texbind);
    // Check for localized swaps
    for (i=0; i<numTextureSearchPaths; i++) {
        BasicTexture *localized;
        sprintf(buf, "%s#%s", texbind->actualTexture->name, textureSearchPath[i]);
        localized = texFind(buf);
        if (localized && localized != white_tex) {
            //verbose_printf("Using localized texture %s/%s instead of %s\n", localized->dirname, localized->name, texbind->name);
            texbind->actualTexture = localized;
            break;
        }
    }

    return ret;
}

void texCheckSwapList(void) {
    int count, i, j;
    int *didSwap=NULL;
    TexSwap **swaplist;

    swaplist = sceneGetTexSwaps(&count);
    // Check for any swapped textures, and load them as well
    if (count) {
        char srcname[256];
        char dstname[256];
        didSwap = calloc(count, sizeof(int));
        // Call texFixName to remove .tga, etc from texture names in the swap list
        for (i=0; i < count; i++) {
            texFixName(swaplist[i]->src, srcname, sizeof(srcname));
            texFixName(swaplist[i]->dst, dstname, sizeof(dstname));
            swaplist[i]->src = (char*)allocAddString(srcname);
            swaplist[i]->dst = (char*)allocAddString(dstname);
        }
    }

    // assign new textures based on SwapList, and reset any old, non-swapped textures
    for( i = eaSize(&g_basicTextures)-1 ; i >= 0; i-- ) {
        if (-1!=(j = texCheckForSwaps(g_basicTextures[i]))) {
            didSwap[j] = true;
        }
    }
    // check which ones weren't swapped
    if (!quickload) {
        for (j=0; j<count; j++) {
            if (!didSwap[j]) {
                bool dupSwap=false;
                for (i=0; i<count; i++)
                    if (i!=j)
                        if (stricmp(swaplist[i]->src, swaplist[j]->src)==0)
                            dupSwap = true;
                if (!dupSwap) // Duplicate swaps are OK since they might include a master file with swaps, and then swap things out after that
                    Errorf("*** TexSwap Warning:  was unable to find source texture '%s' to swap (does not exist in texture_library/).\n", swaplist[j]->src);
            }
        }
    }
    if (didSwap)
        free(didSwap);
    modelRebuildTextures();
}

void texLoadQueueFinish(void) // Directs the loader to stop queueing load calls and begin loading all queued graphics
{

    QueueUserAPC(texLoadQueueFinishDummy, background_loader_handle, (ULONG_PTR)NULL );
    texCheckThreadLoader();
    queuingTexLoadsOutOfThread--;
    // Wait until all are loaded
    if (queuingTexLoadsOutOfThread==0) {
        // We want to wait until all the textures are loaded
        
        while (!hasStartedQueuing) Sleep(1); // Wait until we've *started* queueing the textures

        EnterQueueingLoadsCS(); // Wait for loads to finish
        LeaveQueueingLoadsCS();
        texDoThreadedQueuedTextureLoading();  // Load in FG change

        texCheckThreadLoader();
    }
    texCheckSwapList(); // This resets all textures to their original, and then swaps based on the swaplist
}

void texForceTexLoaderToComplete(int force_texword_flush)
{
    if (force_texword_flush) {
        texWordsFlush();
    }
    while( texLoadsPending(force_texword_flush) )
    {
        Sleep(1);
        texCheckThreadLoader();
        loadUpdate("Loading textures", 10);
    }
    texCheckThreadLoader();
}

static VOID CALLBACK texLoadQueueDeferDummy( ULONG_PTR useless ) // Directs the loader to stop queueing load calls and begin loading all queued graphics
{
    PERFINFO_AUTO_START("texLoadQueueDeferDummy", 1);
    queuingTexLoadsDefered = queuingTexLoads;
    if (queuingTexLoadsDefered) {
        //printf("Deferring queuing of graphics load calls.\n");
    }
    queuingTexLoads = 0;
    PERFINFO_AUTO_STOP();
}
void texLoadQueueDefer(void) // Directs the loader to stop queueing load calls, but not begin loading them
{
    //return;
    QueueUserAPC(texLoadQueueDeferDummy, background_loader_handle, (ULONG_PTR)NULL );
    queuingTexLoadsDeferedOutOfThread = queuingTexLoadsOutOfThread;
    queuingTexLoadsOutOfThread = 0;
}

static VOID CALLBACK texLoadQueueResumeDummy( ULONG_PTR useless ) // Directs the loader to stop queueing load calls and begin loading all queued graphics
{
    PERFINFO_AUTO_START("texLoadQueueResumeDummy", 1);
    queuingTexLoads=queuingTexLoadsDefered;
    if (queuingTexLoads>0) {
        //printf("Resuming queuing of graphics load calls.\n");
    }
    queuingTexLoadsDefered=0;
    PERFINFO_AUTO_STOP();
}
void texLoadQueueResume(void) // Stops deferring, and begins queuing again
{
    //return;
    QueueUserAPC(texLoadQueueResumeDummy, background_loader_handle, (ULONG_PTR)NULL );
    queuingTexLoadsOutOfThread=queuingTexLoadsDeferedOutOfThread;
    queuingTexLoadsDeferedOutOfThread=0;
}


static void waitLoaded(BasicTexture *match,int rawData)
{
    CHECKNOTGLTHREAD;
    // Wait for the background thread to finish loading.
    while(match->load_state[rawData] != TEX_LOADED && !disable_parallel_tex_thread_loader)
    {
        Sleep(1);
        texCheckThreadLoader(); //only called from main thread
    }
}


// Loads an actual bind (no dereferencing to actualTexture, assume that was already done)
// Checks to see if this texture has any dependencies for the texWords system
void texLoadInternalBasic(BasicTexture *bind, TexLoadHow mode, TexUsage use_category, int rawData)
{
    int    need_to_load=0;
    BasicTexture *match=bind;
    TexThreadPackage * pkg;
    bool bIsCubemapFace0 = ((match->flags & TEX_CUBEMAPFACE) != 0 || (match->texopt_flags & TEXOPT_CUBEMAP) != 0) && strEndsWith(match->name, "0");
    TexLoadHow orgCubemapMode = mode;

    PERFINFO_AUTO_START("texLoadInternalBasic", 1);

    texLoadCalls++;

    CHECKNOTGLTHREAD;

    // special case for cubemaps, need to load synchronously so subsequent faces 
    //    can know ID assigned to first face
    if ( (disable_parallel_tex_thread_loader || bIsCubemapFace0) &&
        mode == TEX_LOAD_IN_BACKGROUND)
    {
        mode = TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD;
    }

    match->use_category |= use_category;
    if (!match->use_category) {
        match->use_category = TEX_FOR_NOTSURE;
    }

    if (mode == TEX_LOAD_DONT_ACTUALLY_LOAD) {
        PERFINFO_AUTO_STOP();
        return;
    }

    if ( match->load_state[rawData] == TEX_NOT_LOADED ) {
        EnterCriticalSection(&CriticalSectionTexLoadQueues); 
        if ( match->load_state[rawData] == TEX_NOT_LOADED ) {
            need_to_load = 1;
            match->load_state[rawData] = TEX_LOADING;
        }
        LeaveCriticalSection(&CriticalSectionTexLoadQueues); 
    }

    if ( need_to_load )
    {
        // Check to see if we have any dependencies to load, load them first, in the same manner as how
        //   we're loading this texture (i.e. a LOAD_NOW will have all dependencies loaded first, a 
        //   LOAD_IN_BACKGROUND will have all dependencies loaded asynchronously, but *before* the actual
        //   texture, it is assumed that once the actual texture gets loaded all dependencies are loaded
        if (!rawData && match->texWord) {
            // Look for dependencies
            texWordLoadDeps(match->texWord, mode, use_category, match);
        }

        match->use_category |= use_category;

        pkg = calloc(sizeof(TexThreadPackage), 1);
        pkg->bind = match;//the bind to copy the texbind to once the loading is done
        pkg->needRawData = rawData;

        if( mode == TEX_LOAD_NOW_CALLED_FROM_LOAD_THREAD)
        {  //If currently in the load thread
            memlog_printf(0, "%u: texLoad %sTEX_LOAD_NOW_CALLED_FROM_LOAD_THREAD %s", game_state.client_frame_timestamp, rawData?"RAW ":" ", match->name);
            pkg->should_queue=-1; // we are in the thread, so base this on the thread's flag
            InterlockedIncrement(&numTexLoadsInThread);
            texDoThreadedTextureLoading( (ULONG_PTR)pkg ); //loads up this texture right now or queues it based on should_queue and queuingTexLoads
        }
        else if( mode == TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD)
        {
            // If currently in the main thread, but want it now!
            memlog_printf(0, "%u: texLoad %sTEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD %s", game_state.client_frame_timestamp, rawData?"RAW ":" ", match->name);
            pkg->should_queue = 0; //queuingTexLoadsOutOfThread;
            InterlockedIncrement(&numTexLoadsInThread);
            texDoThreadedTextureLoading( (ULONG_PTR)pkg ); //loads up this texture right now or queues it based on should_queue and queuingTexLoads
            texCheckThreadLoader(); //only called from main thread
            waitLoaded(match,rawData);
            assert( queuingTexLoadsOutOfThread || match->load_state[rawData] == TEX_LOADED );
        }
        else if( mode == TEX_LOAD_IN_BACKGROUND || mode == TEX_LOAD_IN_BACKGROUND_FROM_BACKGROUND )
        {
            memlog_printf(0, "%u: texLoad %sTEX_LOAD_IN_BACKGROUND %s", game_state.client_frame_timestamp, rawData?"RAW ":" ", match->name);
            assert(background_loader_handle);
            pkg->should_queue=-1; // We must use the value in the thread because that value might change by the time the thread gets this package
            InterlockedIncrement(&numTexLoadsInThread);
            //printf("queuing APC load: %s\n", pkg->bind->name);
            QueueUserAPC(texDoThreadedTextureLoading, background_loader_handle, (ULONG_PTR)pkg );
        }
        else
            assert(0);
    }

    // If the caller requested for to wait while the texture is loading and the texture is already being loaded in the background...
    if (!queuingTexLoadsOutOfThread && match->load_state[rawData] == TEX_LOADING && mode == TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD )
    {
        waitLoaded(match,rawData);
    }

    // flag the texture as having just been used.  This should effectively flag
    //        all textures when a new map loads
    match->last_used_time_stamp[rawData] = game_state.client_frame_timestamp;

    // special case for cubemaps, when load the first face, load the other 5 faces with the same texID.
    //    The loader will take care of correctly loading into the proper cubemap face slot.
    if(bIsCubemapFace0)
    {
        // find the other cubemap faces based on naming convention.  We assume that cubemap face 
        //    names end in '0', '1', '2', etc.  We will muck with the last character here to get
        //    the names of the other faces, but restore it below.
        char* tempName = (char*)match->name;
        int faceNumPosition = strlen(tempName) - 1, i;
        const char zeroeth_face_char = tempName[faceNumPosition];
        assert( zeroeth_face_char == '0' || zeroeth_face_char == '1'); // allow both 0 and 1 based naming convention
        match->cube_face_idx = 0;
        assert(match->id); // should have been set above, need it so we can propagate same id to each face so that loader correctly assigns faces
        for(i=1; i<6; i++)
        {
            BasicTexture* pFaceTexture;
            const char faceNumChar = zeroeth_face_char + i;
            tempName[faceNumPosition] = faceNumChar;
            pFaceTexture = texFind(tempName);
            //assert(pFaceTexture);
            if(pFaceTexture) {
                // no, only master face (0) has this flag -- pFaceTexture->flags |= TEX_CUBEMAPFACE;
                pFaceTexture->cube_face_idx = i;
                pFaceTexture->texopt_flags |= TEXOPT_CLAMPS|TEXOPT_CLAMPT;
                pFaceTexture->target = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i;

                // now load the face
                pFaceTexture->id = match->id;
                texLoadInternalBasic(pFaceTexture, orgCubemapMode, use_category, rawData);
            }
        }
        tempName[faceNumPosition] = zeroeth_face_char; // restore name to incoming state
    }

    PERFINFO_AUTO_STOP();
}

BasicTexture *texLoadBasic(const char *name, TexLoadHow mode, TexUsage use_category)
{
    BasicTexture *match;

    //find TexBind from the name
    match = texFind(name);
    if(!match) 
        return white_tex; //handle bad texture name

    texLoadInternalBasic(match->actualTexture, mode, use_category, 0);
    return match;
}


void texLoadInternalComposite(TexBind *bind, TexLoadHow mode, TexUsage use_category)
{
    int i;
    TexBind *match=bind;

    CHECKNOTGLTHREAD;

    if (disable_parallel_tex_thread_loader &&
        mode == TEX_LOAD_IN_BACKGROUND)
    {
        mode = TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD;
    }

    match->use_category |= use_category;
    if (!match->use_category)
        match->use_category = TEX_FOR_NOTSURE;

    for (i=ARRAY_SIZE(match->tex_layers)-1; i>=0; i--)
        if (match->tex_layers[i])
            match->tex_layers[i]->actualTexture->use_category |= use_category;

    if (mode == TEX_LOAD_DONT_ACTUALLY_LOAD) {
        return;
    }

    for (i=ARRAY_SIZE(match->tex_layers)-1; i>=0; i--)
        if (match->tex_layers[i])
            texLoadInternalBasic(match->tex_layers[i]->actualTexture, mode, use_category, 0);

}


/*
// Does actualTexture indirection, expects to be passed a texture name prior to swaps and translation

1. Return ptr to the TexBind with the given name
2. Load the texture data and if needed (if !bind->loaded)
Can be called from either thread.
*/
TexBind *texLoad(const char *name, TexLoadHow mode, TexUsage use_category)
{
    TexBind    *match;

    //find TexBind from the name
    match = texFindComposite(name);
    if(!match) 
        return white_tex_bind; //handle bad texture name

    texLoadInternalComposite(match, mode, use_category);
    return match;
}

BasicTexture *texLoadRawData(const char *name, TexLoadHow mode, TexUsage use_category)
{
    BasicTexture *match;

    //find TexBind from the name
    match = texFind(name);
    if(!match)
        match = white_tex; //handle bad texture name

    if (mode == TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD)
    {
        texFree(match, 1);
    }

    match->actualTexture->rawReferenceCount++;
    texLoadInternalBasic(match->actualTexture, mode, use_category, 1);
    return match;
}

void texUnloadRawData(BasicTexture *texBind)
{
    assert(texBind && texBind->actualTexture->rawReferenceCount && texBind->actualTexture->load_state[1]==TEX_LOADED);
    texBind->actualTexture->rawReferenceCount--;
    // Either unload it now, or it will be dynamically unloaded later
}

// "Loads" a new Trick-created texture
TexBind *texCreateTrickTexture(const char *dirname, const char *name, const char *basetexture)
{
    BasicTexture *base;
    TexBind *bind=NULL;
    // Allocate a new TexBind
    bind = createTexBind();

    // Fill it in
    bind->dirname = dirname;
    bind->name = name;
    bind->orig_bind = bind;

    // Try to get some data from bottom layer
    base = texFind(basetexture);
    if (base) {
        // Use default sizes, etc
        bind->width = base->width;
        bind->height = base->height;
    }

    // Pool strings
    if (isDevelopmentMode()) {
        // Only in Dev mode, as these pointers can only go bad if Tricks are reloaded, which is not allowed in production
        bind->name = allocAddString(name);
        bind->dirname = allocAddString(dirname);
    }
    // Store in global lists
    eaPush(&g_compositeTextures, bind);
    // Add to hashtable
    stashAddPointer(g_compositeTextures_ht, bind->name, bind, false);

    return bind;
}

// "Loads" a new, dynamic TexBind based on a TexWord layout file and various parameters
//  takes ownership of the params pointer and all sub-data
TexBind *texLoadDynamic(TexWordParams *params, TexLoadHow mode, TexUsage use_category, const char *blameFileName)
{
    TexWord *texWord;
    BasicTexture *bind=NULL;
    BasicTexture *base;

    texWord = texWordFind(params->layoutName, 1);
    if (!texWord) {
        if (isDevelopmentMode())
            ErrorFilenamef(blameFileName, "Reference to unknown TexWordLayout: %s", params->layoutName);
        destroyTexWordParams(params);
        return white_tex_bind;
    }
    // Allocate a new TexBind
    bind = MP_ALLOC(BasicTexture);

    // Fill it in
    bind->actualTexture = bind;
    bind->dirname = "dynamicTexture";
    bind->name = allocAddString(params->layoutName);
    // Assign tricks, etc,  Note that it appears these aren't used, as the rendering code doesn't trace down
    //  into ->actualTexture, and will just use the tricks on the base texture
    texSetBindsSubBasic(bind, NULL, 0);
    bind->texWord = texWord; // Must be *after* setBindsSub!
    bind->texWordParams = params;
    bind->target = GL_TEXTURE_2D;
    // Try to get some data from bottom layer
    base = texWordGetBaseImage(texWord);
    if (base) {
        // Use bottom layer's low mips
        bind->mipdata = base->mipdata;
        bind->mipsize = base->mipsize;
        // Use default sizes, etc (may be overridden in texWordDoComposition)
        bind->width = base->width;
        bind->height = base->height;
        bind->realWidth = base->realWidth;
        bind->realHeight = base->realHeight;
    }

    // Load if required
    texLoadInternalBasic(bind, mode, use_category, 0);

    // Store in global lists
    eaPush(&g_basicTextures, bind);

    {
        TexBind *ret = createTexBind();
        ret->name = bind->name;
        ret->dirname = bind->dirname;
        ret->orig_bind = ret;
        ret->tex_layers[TEXLAYER_BASE] = bind;
        texSetBindsSubComposite(ret);
        return ret;
    }
}

static void texFindNeededBinds()
{
    // Find basic textures
    white_tex            = texFind("white");
    grey_tex            = texFind("grey");
    invisible_tex        = texFind("invisible");
    if(invisible_tex)
        invisible_tex->flags &= ~TEX_ALPHA;
    black_tex            = texFind("black");
    dummy_bump_tex        = texFind("dummy_bump");
    dummy_dynamic_cubemap_tex = texFind("dummy_dynamic_cubemap_face0");
    building_lights_tex = texFind("buildingLightPattern");

    // Game crashes if these basic textures are missing (there may be more)
    devassert(white_tex && grey_tex && dummy_dynamic_cubemap_tex);
}

/* Inits global TexBinds */
void texMakeWhite()
{
    white_tex_bind        = texLoad("white.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL | TEX_FOR_UI);
    grey_tex_bind        = texLoad("grey.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);
    invisible_tex_bind    = texLoad("invisible.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);
    black_tex_bind        = texLoad("black.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);

    texLoadRawData("white.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);

    atlasMakeWhite();
}


// Attempts to determine the format the texture will use/is using in memory
//  based on cached mipmap data
int texGetFormat(BasicTexture *bind)
{
    TextureFileMipHeader *mh = (TextureFileMipHeader*)bind->mipdata;
    if (!mh) {
        // No mipmap header, assume based on other rules
        if (bind->texopt_flags & TEXOPT_JPEG) {
            return GL_RGB8; // Loading screens, etc
        } else {
            return GL_RGBA8;
        }
    } else {
        if (mh->format < 10) {
            // Our custom formats?
            return GL_RGBA8;
        } else {
            return mh->format;
        }
    }
}

///########### TEX SHOW USAGE #############################

/* Some kind of performance/debug thing only called from the console */
void texShowUsage(char *match_string)
{
    int            i,used=0;
    BasicTexture*bind;
    FILE        *file;
    char        buf[1000];

    file = fopen("c:/texusage.txt","wt");
    for(i=0;i<eaSize(&g_basicTextures);i++)
    {
        bind = g_basicTextures[i];
        //if (bind->load_state[0] != TEX_LOADED || !strstri(bind->dirname,match_string))
        //    continue;
        used += bind->memory_use[0];
        sprintf(buf,"%6d [%3d x %3d] %s/%s",bind->file_bytes,bind->width,bind->height,bind->dirname,bind->name);
        conPrintf("%s",buf);
        if (file)
            fprintf(file,"%s\n",buf);
    }
    conPrintf("Total: %d",used);
    if (file)
        fclose(file);
}

void texShowUsageRes(int xres, int yres)
{
    int            i,used=0;
    BasicTexture*bind;
    FILE        *file;
    char        buf[1000];
    U32 byteCount = getImageByteCountWithMips(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, xres, yres, countMipMapLevels(xres,yres));

    file = fopen("c:/texusage.txt","wt");
    for(i=0;i<eaSize(&g_basicTextures);i++)
    {
        bind = g_basicTextures[i];

        //if ( bind->width >= xres && bind->height >= yres )
        if ( byteCount <= bind->file_bytes )
        {
            used += bind->memory_use[0];
            sprintf(buf,"%6d [%3d x %3d] %s/%s",bind->file_bytes,bind->width,bind->height,bind->dirname,bind->name);
            conPrintf("%s",buf);
            if (file)
                fprintf(file,"%s\n",buf);
        }
    }
    conPrintf("Total: %d",used);
    if (file)
        fclose(file);
}

#define RECENT_TIME    timerCpuSpeed()*2 // how many ticks to look at in order to be considered "recent" for profiling
void texGatherStatistics()
{
#if RDRSTATS_ON
    int        i,usedm=0, usedt=0, count;
    static int countdown=0;
    static int last_recent, last_size, last_map, last_total;
    BasicTexture *bind;
    U32 recent_time;;

    if (countdown > 0) {
        // Only do this once and a while
        countdown--;
        RDRSTAT_SET(texture_usage_recent_num, last_recent);
        RDRSTAT_SET(texture_usage_recent_size, last_size);
        RDRSTAT_SET(texture_usage_map, last_map);
        RDRSTAT_SET(texture_usage_total, last_total);
        return;
    }
    countdown = 10;

    recent_time = RECENT_TIME;
    count = eaSize(&g_basicTextures);

    // Find all tetxures that are loaded and relate to the current map (world/)
    for(i=0;i<count;i++)
    {
        bind = g_basicTextures[i];
        if (bind->load_state[0] != TEX_LOADED)
            continue;

        if (bind->last_used_time_stamp[0] > game_state.client_frame_timestamp - recent_time) {
            RDRSTAT_ADD(texture_usage_recent_num, 1);
            RDRSTAT_ADD(texture_usage_recent_size, bind->memory_use[0]);
        }

        usedt += bind->memory_use[0];
        if (strnicmp(bind->dirname, "WORLD", 5)!=0)
            continue;
        usedm += bind->memory_use[0];
    }
    RDRSTAT_ADD(texture_usage_map, usedm);
    RDRSTAT_ADD(texture_usage_total, usedt);

    last_recent = RDRSTAT_GET(texture_usage_recent_num);
    last_size = RDRSTAT_GET(texture_usage_recent_size);
    last_map = RDRSTAT_GET(texture_usage_map);
    last_total = RDRSTAT_GET(texture_usage_total);
#endif
}

int texFindByFullPathName(char *search,const char **names,int max)
{
    int        i,count=0;
    TexBind    *bind;
    char    fname[1000];

    for(i=0;i<eaSize(&g_compositeTextures);i++)
    {
        bind = g_compositeTextures[i];
        STR_COMBINE_BEGIN(fname);
        STR_COMBINE_CAT(bind->dirname);
        STR_COMBINE_CAT("/");
        STR_COMBINE_CAT(bind->name);
        STR_COMBINE_END();
        //sprintf(fname,"%s/%s",bind->dirname,bind->name);
        if (strstri(fname,search))
        {
            names[count++] = bind->name;
            if (count >= max)
                break;
        }
    }
    return count;
}


static void texBindLowMips(BasicTexture *texbind)
{
    RdrTexParams            rtex;
    TexReadInfo                info;
    TextureFileMipHeader    *mh = (TextureFileMipHeader*)texbind->mipdata;

    texbind->mipid = rdrAllocTexHandle();
    info.data = texbind->mipdata + mh->structsize;
    info.size = texbind->mipsize - mh->structsize;
    info.width = mh->width;
    info.height = mh->height;
    info.mip_count = countMipMapLevels(mh->width, mh->height);
    info.format = mh->format;
    texSetupParamsFromBind(texbind, &info, &rtex);
    rtex.id = texbind->mipid;
    rdrTexCopy(&rtex,info.data,info.size);
}

int texDemandLoadActual(BasicTexture *texbind)
{
    BasicTexture *actualTexture;

    if (!texbind)
        return 0;
        
    PERFINFO_AUTO_START("texDemandLoadActual", 1);

    actualTexture = texbind->actualTexture;

    actualTexture->last_used_time_stamp[0] = game_state.client_frame_timestamp;
    if (actualTexture->load_state[0] == TEX_LOADED)
        return actualTexture->id;
    if (actualTexture->load_state[0] == TEX_NOT_LOADED)
    {
        actualTexture->use_category |= texbind->use_category;
        if (actualTexture->use_category & (TEX_FOR_UI | TEX_FOR_UTIL)) // UI and util textures *must* be loaded before drawing anything
        {
            texLoadInternalBasic(actualTexture, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, actualTexture->use_category, 0);
        } else {
            texLoadInternalBasic(actualTexture, TEX_LOAD_IN_BACKGROUND, actualTexture->use_category, 0);
        }
    }
    if (actualTexture->mipdata && !actualTexture->mipid && actualTexture->load_state[0]==TEX_LOADING) {
        // Bind the low mip levels now!
        texBindLowMips(actualTexture);
    }
    
    PERFINFO_AUTO_STOP();
    
    return actualTexture->mipid ? actualTexture->mipid : (actualTexture->id?actualTexture->id:white_tex->id);
}

void rdrTexFree(int id)
{
    rdrQueue(DRAWCMD_TEXFREE,&id,sizeof(id));
}

void rdrTexCopy(RdrTexParams *rtex,void *data,int num_bytes)
{
    RdrTexParams    *mem = rdrQueueAlloc(DRAWCMD_TEXCOPY,sizeof(*rtex) + num_bytes);

    *mem = *rtex;
    memcpy(mem+1,data,num_bytes);
    rdrQueueSend();
}

void rdrTexSubCopy(RdrSubTexParams *rtex,void *data,int num_bytes)
{
    RdrSubTexParams    *mem = rdrQueueAlloc(DRAWCMD_TEXSUBCOPY,sizeof(*rtex) + num_bytes);

    *mem = *rtex;
    memcpy(mem+1,data,num_bytes);
    rdrQueueSend();
}

