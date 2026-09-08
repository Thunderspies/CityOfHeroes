/****************************************************************************
    gettex.c

    Texture processing tool.
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <share.h>
#include <utilitieslib/stdtypes.h>
#ifdef _WIN32
#include <utilitieslib/utils/wininclude.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <dirent.h>
#endif
#include <utilitieslib/utils/utils.h>
#include "seq/tricks.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/tga.h>
#include "tdither.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "windows.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <conio.h>
#include <utilitieslib/utils/FolderCache.h>
#include "render/tex.h"
#include <utilitieslib/utils/fileutil.h>
#include "render/dd.h"
#include "mipmap.h"
#include "texloaddesc.h"
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/SharedMemory.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/process_util.h>
#include "askOptions.h"
#include <utilitieslib/utils/timing.h>
#include "seq/AutoLOD.h"
#include "gettex.h"
#include "texture_tools.h"

#define USE_CJPEG_EXE 0
// If GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES == 1 the code can run either
// in the legacy mode (shelling out to nvdxt.exe and dxtex.ex) or the current
// mode (calling into the NVIDIA Texture Tools Library.
// If 0, only the NVIDIA Texture Tools Library mode is supported.
#define GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES 0

// Test function
#define GETTEXT_PREPARE_COMPARISON_FILE(_dds_file_path_)

#if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES

    // Define as 1 to make the code use the old mode (shelling to the
    // standalone tools) by default. Define as 0 to make the new mode
    // the default. Either default can be overridden by the command line switch:
    //        -use_nvdxt_exe 1  -or-  -use_nvdxt_exe 0
    #define GETTEX_DEFAULT_TO_LEGACY_TEXTURE_TOOL_EXES 1

    // If GETTEX_USE_NVDXT_EXE_FOR_HEIGHTMAPS == 1 we send heightmap
    // textures to the old NVIDIA nvdxt.exe standalone tool, rather than using
    // the NVIDIA Texture Tools library. This allows old heigthmaps to be processed
    // the same as before.
    // Set to 0 to turn off the hack.
    // Can be overridden by:
    //        -nvdxt_exe_for_bumpmaps 0   -or-   -nvdxt_exe_for_bumpmaps 1
    #define GETTEX_USE_NVDXT_EXE_FOR_HEIGHTMAPS 1

    // If GETTEX_ENABLE_COMPARISON_TEST_MODE == 1 we add code which can create two versions
    // of the .DDS file: one using the old mode (nvdxt.exe or dxtex.exe) and marked with an
    // extra "LEGACY_MODE" in its name ("foo.LEGACY_MODE.dds"). It also writes a *.txt file
    // ("foo.LEGACY_MODE.dds.txt") with the command line used to generate the file.
    //
    // To use this feature, add "-ComparisonTestMode" to the command line.
    //
    #define GETTEX_ENABLE_COMPARISON_TEST_MODE 1
    #if GETTEX_ENABLE_COMPARISON_TEST_MODE
        static void debug_PrepareComparisonFile( const char* dds_file_path );
        #undef    GETTEXT_PREPARE_COMPARISON_FILE
        #define GETTEXT_PREPARE_COMPARISON_FILE(_dds_file_path_)    debug_PrepareComparisonFile(_dds_file_path_)
    #endif
    
#endif

static int                force_mode;
static int                cutout_mode;
static int                no_tex_opt_check;
static int                no_dxtex;
static int                no_check_out=0;
static int                no_check_in=1;
static int                doprune=0;
static int                monitor = 0;
static int                do_scan = 1;
static int                spew_initial_dupes = 0;
static int                override_perforce=0;
static int                scanlog = 0;
static int                scan_check_alpha = 0;
static int                noperforce = 1;
static int                num_reprocess=0;
static int                num_repack = 0;
static char                *tgaPath=NULL;
static char                nvdxt_path[MAX_PATH];
static char                dxtex_path[MAX_PATH];
static char                cjpeg_path[MAX_PATH];
static int                reprocessing_on_the_fly=0;
static FolderCache*        fctexlib = NULL;
static FolderCache**    fcbtexlibs_OtherBranches = NULL; // mEArrayHandle. For catching accidental changes to the wrong branch.
static int                scan_world=1;
static int                scan_character=1;
static int                scan_other=1;

//---------------------------------------------------------------------------
// kToolType_xxx
//
//  These are the legacy stand-alone tool exe's which can
//  be selected to perform the desired texture processing.
//  If external usage is disabled, though, execTexProcessor()
//  will use the NVIDIA Texture Tools Library (DLL) instead.
//---------------------------------------------------------------------------
typedef enum 
{
    kTool_NV_NvDxtExe,     // NVIDIA texture tool, nvdxt.exe
    kTool_DX_DxTexExe,    // DirectX texture tool, dxtexe.exe
    kTool_CJpeg,
    //---------------------
    kTool_COUNT
} tExternalToolType;

static bool                    gForceExternalToolUsage = false;    // an override for the current item
static char                    gExternalTexProcessorCmdLine[1000];
static tExternalToolType    gExternalToolType = kTool_NV_NvDxtExe;

#define GETTEX_USE_LEGACY_TEXTURE_TOOL_EXES()    (false)

#if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES

    static bool gDebug_StandaloneTextureToolMode = GETTEX_DEFAULT_TO_LEGACY_TEXTURE_TOOL_EXES;
    static bool gDebug_ComparisonTestMode = false;
    
    #if GETTEX_USE_NVDXT_EXE_FOR_HEIGHTMAPS
        // We need to set options for either mode, since we may use standalone
        // tools even when we're using the NVIDIA Toolkit for everything else.
        #define GETTEX_UPDATE_LEGACY_TOOL_EXE_OPTS()    (true)
        bool gDebug_ForceStandaloneTextureToolsForHeighmaps = true;
    #else
        #define GETTEX_UPDATE_LEGACY_TOOL_EXE_OPTS()    (gDebug_StandaloneTextureToolMode)
    #endif
    #undef  GETTEX_USE_LEGACY_TEXTURE_TOOL_EXES
    #define GETTEX_USE_LEGACY_TEXTURE_TOOL_EXES()    (gDebug_StandaloneTextureToolMode || gForceExternalToolUsage)
#endif

static FILE        *scan_log = NULL;

//-----------------------------------------------------------------------------
// Options displayed to the user in the askOptions dialog.
//
// Please do not add non-artist options (i.e. -nocheckout, etc) to this list
// If we want, we could add a -programmer arg that switches to a second, full
// set of options.
//-----------------------------------------------------------------------------
static OptionList option_list[] = {
    //-----------------------------------------------------------------------------------------------
    // name                                            value           regKey            flags
    //-----------------------------------------------------------------------------------------------
    {"Search for changes",                             &do_scan,        "Scan"},
    {"World (WORLD, MAPS, loading_screens)",           &scan_world,     "ScanWorld",     OLF_CHILD},
    {"Character (ENEMIES, NPCS, PLAYERS, MasterMind)", &scan_character, "ScanCharacter", OLF_CHILD},
    {"Misc (FX, GUI, UI, Items, etc)",                 &scan_other,     "ScanMisc",      OLF_CHILD},
    {"Monitor",                                        &monitor,        "Monitor"},
};

static StashTable texs_by_trick_file = 0;
typedef struct TexList
{
    StashTable texs_hash;
} TexList;


//void quantTex(void *data,int w,int h); //#include tdither.h instead?

typedef struct
{
    char *name;
    U8        compress;
/*    // The following need to be saved into .texture files
    int        width, height;
    int        flags;
    F32        fade[2];
    U8        alpha;*/
} TexDef;

typedef struct
{
    int        width,height;
    U8        *data;
} TgaInfo;

static TexDef    **tex_defs;


typedef struct tgaHeader{
    U8 IDLength;
    U8 CMapType;
    U8 ImgType;
    U8 CMapStartLo;
    U8 CMapStartHi;
    U8 CMapLengthLo;
    U8 CMapLengthHi;
    U8 CMapDepth;
    U8 XOffSetLo;
    U8 XOffSetHi;
    U8 YOffSetLo;
    U8 YOffSetHi;
    U8 WidthLo;
    U8 WidthHi;
    U8 HeightLo;
    U8 HeightHi;
    U8 PixelDepth;
    U8 ImageDescriptor;
} TgaHeader;

#if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES
    static struct {
        const char*    flagStr[kTool_COUNT];
        bool        bQuoteVal;    // whether to surround szOptData in double quotes (e.g., paths).
    } s_LegacyToolFlagsTable[kGetTexOpt_COUNT] = { 0 };
#endif    

static void getFileNameFromGameName(char *gameName, char *fileName);
static bool checkForDuplicateNames(const char *texturefilename, bool bPrintToConsole);
static bool checkForDuplicateNamesRemove(char *texturefilename);


static void initLegacyToolFlagsTable()
{
    #if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES
        // For a given option, each tool can have a different command line flag to represent it.
        s_LegacyToolFlagsTable[ kGetTexOpt_SrcFile        ].flagStr[kTool_NV_NvDxtExe]        = "-file";
        s_LegacyToolFlagsTable[ kGetTexOpt_SrcFile        ].flagStr[kTool_DX_DxTexExe]        = "";
        s_LegacyToolFlagsTable[ kGetTexOpt_SrcFile        ].flagStr[kTool_CJpeg]            = "";
        s_LegacyToolFlagsTable[ kGetTexOpt_SrcFile        ].bQuoteVal                        = true;

        s_LegacyToolFlagsTable[ kGetTexOpt_OutputDir      ].flagStr[kTool_NV_NvDxtExe]        = "-outdir";
        s_LegacyToolFlagsTable[ kGetTexOpt_OutputDir      ].bQuoteVal                        = true;

        s_LegacyToolFlagsTable[ kGetTexOpt_OutputFile     ].flagStr[kTool_NV_NvDxtExe]        = NULL;    // doesn't map directly to nvdtx.exe flag
        s_LegacyToolFlagsTable[ kGetTexOpt_OutputFile     ].flagStr[kTool_DX_DxTexExe]        = "";
        s_LegacyToolFlagsTable[ kGetTexOpt_OutputFile     ].flagStr[kTool_CJpeg]            = "";

        s_LegacyToolFlagsTable[ kGetTexOpt_Mipmap         ].flagStr[kTool_NV_NvDxtExe]        = "";
        s_LegacyToolFlagsTable[ kGetTexOpt_Mipmap         ].flagStr[kTool_DX_DxTexExe]        = "-m";

        s_LegacyToolFlagsTable[ kGetTexOpt_NoMipmap       ].flagStr[kTool_NV_NvDxtExe]        = "-nomipmap";
        s_LegacyToolFlagsTable[ kGetTexOpt_NoMipmap       ].flagStr[kTool_DX_DxTexExe]        = "";

        s_LegacyToolFlagsTable[ kGetTexOpt_Cubic          ].flagStr[kTool_NV_NvDxtExe]        = "-cubic";

        s_LegacyToolFlagsTable[ kGetTexOpt_Kaiser         ].flagStr[kTool_NV_NvDxtExe]        = "-kaiser";

        s_LegacyToolFlagsTable[ kGetTexOpt_DXT5           ].flagStr[kTool_NV_NvDxtExe]        = "-dxt5";
        s_LegacyToolFlagsTable[ kGetTexOpt_DXT5           ].flagStr[kTool_DX_DxTexExe]        = "-DXT5";

        s_LegacyToolFlagsTable[ kGetTexOpt_DXT5nm         ].flagStr[kTool_NV_NvDxtExe]        = "-dxt5";    // not available; we use DXT5 instead
        s_LegacyToolFlagsTable[ kGetTexOpt_DXT5nm         ].flagStr[kTool_DX_DxTexExe]        = "-DXT5";    // not available; we use DXT5 instead

        s_LegacyToolFlagsTable[ kGetTexOpt_DXT1           ].flagStr[kTool_NV_NvDxtExe]        = "-dxt1";
        s_LegacyToolFlagsTable[ kGetTexOpt_DXT1           ].flagStr[kTool_DX_DxTexExe]        = "-DXT1";

        s_LegacyToolFlagsTable[ kGetTexOpt_U8888          ].flagStr[kTool_NV_NvDxtExe]        = "-u8888";

        s_LegacyToolFlagsTable[ kGetTexOpt_U888           ].flagStr[kTool_NV_NvDxtExe]        = "-u888";

        s_LegacyToolFlagsTable[ kGetTexOpt_U1555          ].flagStr[kTool_NV_NvDxtExe]        = "-u1555";

        s_LegacyToolFlagsTable[ kGetTexOpt_U4444          ].flagStr[kTool_NV_NvDxtExe]        = "-u4444";

        s_LegacyToolFlagsTable[ kGetTexOpt_U565           ].flagStr[kTool_NV_NvDxtExe]        = "-u565";

        s_LegacyToolFlagsTable[ kGetTexOpt_MakeN4         ].flagStr[kTool_NV_NvDxtExe]        = "-n4";

        s_LegacyToolFlagsTable[ kGetTexOpt_HeightRgb      ].flagStr[kTool_NV_NvDxtExe]        = "-rgb";

        s_LegacyToolFlagsTable[ kGetTexOpt_SrcIsNormalMap ].flagStr[kTool_NV_NvDxtExe]        = NULL;    // no corresponding flag

        s_LegacyToolFlagsTable[ kGetTexOpt_NormalizeMipMap].flagStr[kTool_NV_NvDxtExe]        = NULL;    // no corresponding flag

        // Legacy options
        // -----------------------------------------------------------
        s_LegacyToolFlagsTable[ kGetTexOpt_ToolPath       ].flagStr[kTool_NV_NvDxtExe]        = "";
        s_LegacyToolFlagsTable[ kGetTexOpt_ToolPath       ].flagStr[kTool_DX_DxTexExe]        = "";
        s_LegacyToolFlagsTable[ kGetTexOpt_ToolPath       ].flagStr[kTool_CJpeg]            = "";            
    #endif
}

static void addTexToTrickHash(const char *tex_name, char *trick_file_name)
{
    TexList *tl;

    PERFINFO_AUTO_START("stashTableCreateWithStringKeys", 1);
    if (!texs_by_trick_file)
        texs_by_trick_file = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);

    PERFINFO_AUTO_STOP_START("stashFindPointer", 1);
    if (!stashFindPointer(texs_by_trick_file, trick_file_name, &tl))
    {
        PERFINFO_AUTO_STOP_START("new trick file", 1);
        tl = malloc(sizeof(TexList));
        tl->texs_hash = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
        stashAddPointer(texs_by_trick_file, trick_file_name, tl, false);
    }
    else
    {
        int i;
        PERFINFO_AUTO_STOP_START("existing trick file", 1);
        if (stashFindInt(tl->texs_hash, tex_name, &i)) {
            PERFINFO_AUTO_STOP();
            return;
        }
    }
    PERFINFO_AUTO_STOP_START("add texture", 1);
    stashAddInt(tl->texs_hash, tex_name, 1, true);
    PERFINFO_AUTO_STOP();
}

#define TGAVERIFYALPHA_TEMPBUFFERSIZE 1024
#define TGAVERIFYALPHA_ALPHACUTOFF 247
#define TGAVERIFYALPHA_ALPHAPERCENTAGE 100

// Verify that the 32-bit texture actually needs 32 bits
static U8 tgaVerifyAlpha(FILE *file, TgaHeader *tga_header, const char *fname, bool warnOnAlphaChange)
{
    static U32 temp_buffer[TGAVERIFYALPHA_TEMPBUFFERSIZE];
    int width = tga_header->WidthHi * 256 + tga_header->WidthLo;
    int height = tga_header->HeightHi * 256 + tga_header->HeightLo;
    U8 alpha = 1;

    // At this point, file is pointing to just after the 10-byte header
    assert(sizeof(TgaHeader) == 18);
    assert(tga_header->PixelDepth == 32);
    assert(width > 0 && height > 0);

#if 0
    // Skip the image ID field, if any
    if (tga_header->IDLength > 0)
    {
        fseek(file, tga_header->IDLength, SEEK_CUR);
    }
#else
    fseek(file, 18 + tga_header->IDLength, SEEK_SET);
#endif


    if (tga_header->ImgType == 1) // color-mapped image
    {
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BRIGHT);
        printf("  Warning: Unhandled image type 1 (paletted) in tgaVerifyAlpha() for file %s\n", tga_header->ImgType, fname);
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
        // Probably not needed?
        /*
        U16 CMapStart = tga_header->CMapStartHi * 256 tga_header->CMapStartLo;
        U16 CMapLength = tga_header->CMapLengthHi * 256 + tga_header->CMapLengthLo;
        */
    }
    else if (tga_header->ImgType == 2) // true-color image
    {
        int total_pixels = width * height;
        int pixels_read = 0;
        int opaque_pixel_count = 0;

        while (pixels_read < total_pixels)
        {
            int i;
            int pixels_to_read = total_pixels - pixels_read;
            if (pixels_to_read > TGAVERIFYALPHA_TEMPBUFFERSIZE)
                pixels_to_read = TGAVERIFYALPHA_TEMPBUFFERSIZE;

            if (pixels_to_read == fread(temp_buffer, 4, pixels_to_read, file))
            {

                for (i = 0; i < pixels_to_read; i++)
                {
                    U8 alpha = temp_buffer[i] >> 24;
                    if (alpha >= TGAVERIFYALPHA_ALPHACUTOFF)
                        opaque_pixel_count++;
                }
            }
            else
            {
                consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BRIGHT);
                printf("  Warning: fread failed in tgaVerifyAlpha() for file %s\n", fname);
                consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
                break;
            }

            pixels_read += pixels_to_read;
        }

        // if TGAVERIFYALPHA_ALPHAPERCENTAGE% or more of the pixels are opaque
        if (opaque_pixel_count * 100 >= total_pixels * TGAVERIFYALPHA_ALPHAPERCENTAGE)
            alpha = 0;
    }
    else if (tga_header->ImgType == 10) // run-length encoded, true-color image
    {
        int total_pixels = width * height;
        int pixels_read = 0;
        int opaque_pixel_count = 0;

        assert(TGAVERIFYALPHA_TEMPBUFFERSIZE >= 128);

        while (pixels_read < total_pixels)
        {
            static U8 RLEbyte;
            bool freadFailed = false;

            if (1 == fread(&RLEbyte, 1, 1, file))
            {
                U8 RLEcount = (RLEbyte & 0x7F) + 1;

                if (RLEbyte & 0x80) // compressed packet
                {
                    if (1 == fread(temp_buffer, 4, 1, file))
                    {
                        U8 alpha = temp_buffer[0] >> 24;
                        if (alpha >= TGAVERIFYALPHA_ALPHACUTOFF)
                            opaque_pixel_count+=RLEcount;
                    }
                    else
                        freadFailed = true;
                }
                else // uncompressed packet
                {
                    if (RLEcount == fread(temp_buffer, 4, RLEcount, file))
                    {
                        int i;
                        for (i = 0; i < RLEcount; i++)
                        {
                            U8 alpha = temp_buffer[i] >> 24;
                            if (alpha >= TGAVERIFYALPHA_ALPHACUTOFF)
                                opaque_pixel_count++;
                        }

                    }
                    else
                        freadFailed = true;
                }
                
                pixels_read += RLEcount;
            }
            else
                freadFailed = true;

            if (freadFailed)
            {
                consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BRIGHT);
                printf("  Warning: fread failed in tgaVerifyAlpha() for file %s\n", fname);
                consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
                break;
            }
        }

        // if TGAVERIFYALPHA_ALPHAPERCENTAGE% or more of the pixels are opaque
        if (opaque_pixel_count * 100 >= total_pixels * TGAVERIFYALPHA_ALPHAPERCENTAGE)
            alpha = 0;
    }
    else
    {
        // We don't handle these:
        // if (tga_header->ImgType == 3) // black-and-white image
        // if (tga_header->ImgType == 9) // run-length encoded, color-mapped image
        // 
        // if (tga_header->ImgType == 11) // run-length encoded, black-and-white image
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BRIGHT);
        printf("  Warning: Unhandled image type %d in tgaVerifyAlpha() for file %s\n", tga_header->ImgType, fname);
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
    }

    if (warnOnAlphaChange && alpha == 0)
    {
        consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
        printf("  File %s is saved as 32bpp but %d%% of the pixels were %d or greater and therefore the texture is considered opaque\n", fname, TGAVERIFYALPHA_ALPHAPERCENTAGE, TGAVERIFYALPHA_ALPHACUTOFF);
        consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
    }

    return alpha;
}

static int tgaGetInfo(void *mem, U8 *alpha,int *width,int *height, const char *filename)
{
    int            bpp;
    TgaHeader    *tga_header;

    tga_header = mem;
    bpp = (tga_header->PixelDepth + 1) >> 3;
    if (bpp == 2) {
        char error[1024];
        sprintf(error, "GetTex Error: File (%s) is a 16-bit TGA, only 24-bit (opaque) and 32-bit (w/ alpha) TGA files are supported.", filename);
        ErrorFilenamef(filename, error);
        msgAlert(compatibleGetConsoleWindow(), error);
        *alpha = 0;
        *width = 9999;
        *height = 9999;
        return 0;
    }

    // Do not check the lower 4 bits of the image descriptor field since tgaWrite() does not set them properly for the temporary file.
    if (bpp == 4)
        *alpha = 1;
    else
        *alpha = 0;

    *width = tga_header->WidthLo + tga_header->WidthHi * 256;
    *height = tga_header->HeightLo + tga_header->HeightHi * 256;
    return 1;
}

static int tgaGetInfoFromFilename(const char *fname, U8 *alpha, int *width, int *height, bool warnOnAlphaChange)
{
    FILE        *file;
    TgaHeader    header;

    file = fopen(fname,"rb");
    if (!file)
        return 0;
    if (sizeof(TgaHeader)==fread(&header,1,sizeof(TgaHeader),file)) {
        int ret = tgaGetInfo(&header,alpha,width,height, fname);

        // Verify that the 32-bit texture actually needs 32 bits
        // Don't verify for bumpmaps.  They may need all white alpha for specular.
        // Likewise exclude icons, which can be used as cursors and require 32 bit
        bool bRequire32bit = strEndsWith(fname, "_bump") || strEndsWith(fname, "_bump.tga") || strstriConst(fname, "gui\\icons\\");
        if (*alpha && !bRequire32bit)
            *alpha = tgaVerifyAlpha(file, &header, fname, warnOnAlphaChange);

        fclose(file);
        return ret;
    } else {
        fclose(file);
        *width=9999;
        *height=9999;
        return 0;
    }
}


static TexOpt *getTexOpts(const char *name, TexOptFlags *texopt_flags, U8 *alpha)
{
    TexOpt *tex_opt;

    PERFINFO_AUTO_START("trickFromTextureName", 1);
    tex_opt = trickFromTextureName(name, texopt_flags);

    if (tex_opt && texopt_flags && *texopt_flags & TEXOPT_FADE)
    {
        // Doesn't make sense to fade textures without mipmaps
        *texopt_flags &= ~TEXOPT_NOMIP;
    }

    PERFINFO_AUTO_STOP();
    return tex_opt;
}



// Fills in a tfh from a source .tga file
static void makeTexHeader(const char *filename, char *gameName, TextureFileHeader *tfh, int get_tga_info) {
    TexOpt *tex_opt;
    char fname[MAX_PATH];

    PERFINFO_AUTO_START("getTexOpts", 1);
    tex_opt = getTexOpts(gameName, &tfh->flags, NULL);
    PERFINFO_AUTO_STOP_START("fade", 1);

    if (tfh->flags & TEXOPT_FADE) {
        tfh->fade[0] = tex_opt->texopt_fade[0];
        tfh->fade[1] = tex_opt->texopt_fade[1];
    } else {
        tfh->fade[0]=tfh->fade[1]=0;
    }
    //tfh->file_size = -1; // don't care
    //tfh->header_size = -1; // don't care
    if (get_tga_info) {
        PERFINFO_AUTO_STOP_START("get_tga_info", 1);
        if (!strstriConst(filename, "texture_library")) {
            sprintf(fname, "texture_library/%s", filename);
        } else {
            strcpy(fname, filename);
        }
        tgaGetInfoFromFilename(fname, &tfh->alpha, &tfh->width, &tfh->height, false);
    }
    PERFINFO_AUTO_STOP_START("addTexToTrickHash", 1);
    tfh->pad[0]='T';
    tfh->pad[1]='X';
    tfh->pad[2]='2';

    addTexToTrickHash(filename, tex_opt?tex_opt->file_name:"");
    PERFINFO_AUTO_STOP();
}

// Should match TexOptFlags in Game\render\texEnumbs.h
static char *flag_to_name[] = {
    "FADE ",
    "TRUECOLOR ",
    "TRILINEAR ",
    "old3 ",
    "DUAL ",
    "old5 ",
    "CLAMPS ",
    "CLAMPT ",
    "old8 ",
    "MIRRORS ",
    "MIRRORT ",
    "REPLACEABLE ",
    "BUMPMAP ",
    "REPEATS ",
    "REPEATT ",
    "CUBEMAP ",
    "NOMIP ",
    "JPEG ",
    "NODITHER ",
    "NOCOLL ",
    "SURFACESLICK ",
    "SURFACEICY ",
    "SURFACEBOUNCY ",
    "BORDER ",
    "OLDTINT ",
    "DOUBLEFUSION ",
    "POINTSAMPLE ",
    "NORMALMAP ",
    "SPECINALPHA ",
    "FALLBACKFORCEOPAQUE ",
};

static void makeTexOptString(char *fname, char *buf2, size_t buf2ArraySize, TexOptFlags texopt_flags)
{
    int        len, i;

    buf2[0] = 0;

    if (texopt_flags & TEXOPT_FADE) {
        TexOpt *tex_opt = getTexOpts(fname, NULL, NULL);
        sprintf(buf2,"Fade %f %f",tex_opt->texopt_fade[0],tex_opt->texopt_fade[1]);
    }
    // include all flags
    for (i=0; i<32; i++) {
        if (texopt_flags & (1<<i)) {
            if (i<ARRAY_SIZE(flag_to_name)) {
                strcat_s(buf2, buf2ArraySize, flag_to_name[i]); // space already in flag_to_name
            }
        }
    }
    len = (int)strlen(buf2);
    if (len)
        buf2[len-1] = 0;
}


// Returns 1 if there is a flag difference between the TextureFileHeader in the tex_inf structure and
//   the one that will be generated from the existing .tga/tricks
static int texOptDifferent(const char *filename, char *gameName, FILE *log_file)
{
    char    *s,buf2[1000];
    TextureFileHeader *oldtfh=NULL;
    TexInf *oldti=NULL;
    TextureFileHeader newtfh;

    if (no_tex_opt_check)
        return 0;

    // A texture opt is different if any of the parameters in the header of the file are different

    PERFINFO_AUTO_START("name fixup", 1);
    strcpy(buf2,gameName);
    forwardSlashes(buf2);
    s = strstri(buf2,"texture_library");
    if (s)
        s += strlen("texture_library/");
    else
        s = buf2;
    PERFINFO_AUTO_STOP_START("getTexInf", 1);
    oldti = getTexInf(s);
    if (oldti)
        oldtfh = &oldti->tfh;

    if (!oldtfh) // .texture doesn't exist
    {
        PERFINFO_AUTO_STOP();
        return -1; // must be a new texture
    }
    PERFINFO_AUTO_STOP_START("makeTexHeader", 1);
    makeTexHeader(filename, gameName, &newtfh, scan_check_alpha);
    PERFINFO_AUTO_STOP();

    if ((newtfh.flags & TEXOPT_JPEG)!=(oldtfh->flags  & TEXOPT_JPEG))
    {
        if (log_file)
            fprintf(log_file, "  JPEG flag differs   %s\n", filename);
        return 1;
    }

    if ((newtfh.flags & TEXOPT_NOMIP)!=(oldtfh->flags  & TEXOPT_NOMIP))
    {
        if (log_file)
            fprintf(log_file, "  NOMIP flag differs  %s\n", filename);
        return 1;
    }

    if ((newtfh.flags & TEXOPT_TRUECOLOR)!=(oldtfh->flags  & TEXOPT_TRUECOLOR))
    {
        if (log_file)
            fprintf(log_file, "  TRUECOLOR flag diff %s\n", filename);
        return 1;
    }

    if ((newtfh.flags & TEXOPT_FADE)!=(oldtfh->flags  & TEXOPT_FADE))
    {
        if (log_file)
            fprintf(log_file, "  FADE flag differs   %s\n", filename);
        return 1;
    }

    if ((newtfh.flags & TEXOPT_BUMPMAP)!=(oldtfh->flags  & TEXOPT_BUMPMAP))
    {
        if (log_file)
            fprintf(log_file, "  BUMPMAP flag diff   %s\n", filename);
        return 1;
    }

    if (newtfh.fade[0]!=oldtfh->fade[0] ||
        newtfh.fade[1]!=oldtfh->fade[1])
    {
        return 1;
    }

    if (scan_check_alpha)
    {
        // Do this check since we added the tgaVerifyAlpha() functionality
        if (newtfh.alpha!=oldtfh->alpha)
        {
            if (log_file)
                fprintf(log_file, "  Alpha Change %d->%d   %s\n", oldtfh->alpha, newtfh.alpha, filename);
            return 1;
        }
    }

    return 0;
/*    // does it need to check these?  No- timestamp check should check this kind of change
    if (newtfh.alpha!=oldtfh->alpha)
        return 1;
    if (newtfh.width!=oldtfh->width)
        return 1;
    if (newtfh.height!=oldtfh->height)
        return 1;
    return 0;
    */
}


static void tgaQuant(const char *fname,char *outname)
{
    FILE    *file;
    TgaInfo    info;
    int        i,size, components;

    file = fopen(fname,"rb");
    if (!file)
        return;
    info.data = tgaLoad(file,&info.width,&info.height);
    fclose(file);
    if (!info.data)
        return;
    size = info.width * info.height * 4;
    components = 3;
    for(i=3;i<size;i+=4)
    {
        if (info.data[i] != 255)
        {
            components = 4;
            break;
        }
    }
    quantTex(info.data,info.width,info.height);
    tgaSave(outname,info.data,info.width,info.height,components);
}

static void cropTex(TgaInfo    *info)
{
    char    *data;
    U32        *pix;
    int        i,w,h,x,y,starty,endy,startx,endx;

    pix = (void*)info->data;
    for(y=0;y<info->height;y++)
    {
        starty = y;
        for(x=0;x<info->width;x++)
        {
            if (pix[y*info->width + x])
                goto done1;
        }
    }
    done1:

    for(y=info->height-1;y>=0;y--)
    {
        endy = y;
        for(x=0;x<info->width;x++)
        {
            if (pix[y*info->width + x])
                goto done2;
        }
    }
    done2:

    for(x=0;x<info->width;x++)
    {
        startx = x;
        for(y=0;y<info->height;y++)
        {
            if (pix[y*info->width + x])
                goto done3;
        }
    }
    done3:

    for(x=info->width-1;x>=0;x--)
    {
        endx = x;
        for(y=0;y<info->height;y++)
        {
            if (pix[y*info->width + x])
                goto done4;
        }
    }
    done4:

    w = endx-startx+1;
    h = endy-starty+1;
    data = calloc(w*h,4);
    for(i=0;i<h;i++)
    {
        memcpy(&data[i*w*4],&info->data[((i+starty)*info->width + startx)*4],w*4);
    }
    info->width = w;
    info->height = h;
    free(info->data);
    info->data = data;
}

static void tgaMakePow2(const char *fname,char *outname,int alpha)
{
    FILE    *file;
    TgaInfo    info = {0};
    int        i,j,size,components,w,h;
    U8        *data;

    file = fopen(fname,"rb");
    if (!file)
        return;
    info.data = tgaLoad(file,&info.width,&info.height);
    fclose(file);
    if (!info.data)
        return;
    size = info.width * info.height * 4;
    components = 3;

    if (alpha)
    {
        components = 4;
    }
    else
    {
        for(i=3;i<size;i+=4)
        {
            if (info.data[i] != 255)
            {
                components = 4;
                break;
            }
        }
    }

    if (0)
        cropTex(&info);

    w = 1 << log2i(info.width);
    h = 1 << log2i(info.height);
    data = calloc(w*h,4);
    // copy old non-p2 image into new bigger p2 rect. Duplicate right and bottom edge to fill out the extra space
    for(i=0;i<info.height;i++)
    {
        memcpy(&data[i*w*4],&info.data[i*info.width*4],info.width*4);
        for(j=info.width;j<w;j++)
        {
            memcpy(&data[(i*w+j)*4],&info.data[(i*info.width+info.width-1)*4],4);
        }
    }
    free(info.data);
    for(i=info.height;i<h;i++)
    {
        memcpy(&data[i*w*4],&data[(info.height-1)*w*4],w*4);
    }
    tgaSave(outname,data,w,h,components);
    free(data);
}

/*
Fill up two global arrays of TexDefs (tex name + compression type) with texnames from the given directory.  
tex_defs        all textures changed since oldest_time (the last time textures.rom was changed)
*/
static void getFileNames(char *fname)
{
    struct _finddata_t fileinfo;
    int        handle,test,len;
    char    buf[1200];

    if (strnicmp(fname + strlen(fname) - 4, ".tga", 4)==0) {
        // just one file!
        strcpy(buf, fname);
        forwardSlashes(fname);
        (*strrchr(fname, '/'))=0;
    } else {
        sprintf(buf,"%s/*",fname);
    }
    for(test = handle = _findfirst(buf,&fileinfo);test >= 0;test = _findnext(handle,&fileinfo))
    {
        if (fileinfo.name[0] == '.' || fileinfo.name[0] == '_')
            continue;
        if (strchr(fileinfo.name,' '))
            continue;
        sprintf(buf,"%s/%s",fname,fileinfo.name);
        len = (int)strlen(buf);
        if (len > 4 && stricmp(buf + len - 4,".tga")==0)
        {
            TexDef *td = calloc(sizeof(*td),1);
            td->name = strdup(buf);
            eaPush(&tex_defs, td);
        }
        if (fileinfo.attrib & _A_SUBDIR)
            getFileNames(buf);
    }
    _findclose(handle);
}


#if 0
// This is not used, because artists rely on it not working right
void fadeDDS(unsigned char *mem, int len, float fade0, float fade1) {
    DDSURFACEDESC2    *ddsd;
    int mipcount, level, i, width, height, size, offset=0;
    float    ratio = 0.2,grey = 128;
    float close_mix = 256.0*fade0, far_mix=256.0*fade1;
    float mult, add;

    ddsd = (DDSURFACEDESC2*)(mem + 4);
    mipcount = ddsd->dwMipMapCount;
    width = ddsd->dwWidth;
    height = ddsd->dwHeight;
    mem = mem + 4 + sizeof(DDSURFACEDESC2);

    mult=1.0; add=0.0;

    for (level = 0; level <= mipcount && (width || height); ++level)
    {
        if (width == 0)
            width = 1;
        if (height == 0)
            height = 1;

        size = width*height*4;

        // Now, loop through and process
        ratio = 1.f/256.f * (close_mix - (SQR(level) * (256-far_mix) / SQR(mipcount)));
        ratio = MINMAX(ratio,0,1);
        // In theory, this should be add=grey*(1-ratio) and mult=ratio, but
        // this wacky thing duplicates the bug in the initial implementation
        add = ratio * add + (grey * (1-ratio));
        mult *= ratio;
        for(i=0;i<size;i++)
        {
            if ((i&3) == 3)
                continue;
            mem[i+offset] = qtrunc(mem[i+offset] * mult + add);
        }

        offset += size;
        width  >>= 1;
        height >>= 1;
    }

}
#endif

static int countMipMapLevels(int width, int height) {
    int v = MAX(width, height);
    int r;
    for (r=0; v; (v >>= 1), r++);
    return r;
}

static void writeMipMapHeader(FILE *outfile, char *ddsfile, int ddsfile_len, TextureFileHeader *tfh)
{
    DDSURFACEDESC2 *ddsd;
    TextureFileMipHeader mh;
    int mip_count;

    assert(strncmp(ddsfile, "DDS", 3)==0);

    ddsd = (DDSURFACEDESC2*)(ddsfile + 4);

    mh.structsize    = sizeof(mh);
    mh.width        = ddsd->dwWidth;
    mh.height        = ddsd->dwHeight;
    mip_count        = ddsd->dwMipMapCount;
    
    if ( mip_count == 0 )
    {
        return;
    }
    
    switch(ddsd->ddpfPixelFormat.dwFourCC)
    {
    case FOURCC_DXT1:
        mh.format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        mh.format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        mh.format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        if (ddsd->ddpfPixelFormat.dwFlags == DDS_RGBA && ddsd->ddpfPixelFormat.dwRGBBitCount == 32 && ddsd->ddpfPixelFormat.dwRGBAlphaBitMask == 0xff000000)
            mh.format = TEXFMT_ARGB_8888;
        else if (ddsd->ddpfPixelFormat.dwFlags == DDS_RGB  && ddsd->ddpfPixelFormat.dwRGBBitCount == 24)
            mh.format = TEXFMT_ARGB_0888;
        else if (ddsd->ddpfPixelFormat.dwFlags == DDS_RGB  && ddsd->ddpfPixelFormat.dwRGBBitCount == 16 && ddsd->ddpfPixelFormat.dwGBitMask == 0x000007e0)
            mh.format = TEXFMT_ARGB_0565;
        else if (ddsd->ddpfPixelFormat.dwFlags == DDS_RGBA && ddsd->ddpfPixelFormat.dwRGBBitCount == 16 && ddsd->ddpfPixelFormat.dwRGBAlphaBitMask == 0x00008000)
            mh.format = TEXFMT_ARGB_1555;
        else if (ddsd->ddpfPixelFormat.dwFlags == DDS_RGBA && ddsd->ddpfPixelFormat.dwRGBBitCount == 16 && ddsd->ddpfPixelFormat.dwRGBAlphaBitMask == 0x0000f000)
            mh.format = TEXFMT_ARGB_4444;
        else
        {
            Errorf("Error reading compressed .dds file");
            msgAlert(compatibleGetConsoleWindow(), "Error reading compressed .dds file");
            return;
        }
    }
    {
        int default_mip_levels=4;
        int blockSize = (mh.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        int num = MIN(default_mip_levels, countMipMapLevels(mh.width, mh.height));
        int num_to_skip = countMipMapLevels(mh.width, mh.height) - num;
        int bytes_to_skip=0;
        assert(countMipMapLevels(mh.width, mh.height)==ddsd->dwMipMapCount);
        
        // Skip past mip levels we don't want to load
        for (; num_to_skip; num_to_skip--) {
            int size;
            if (mh.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
                mh.format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
                mh.format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
            {
                size = ((mh.width+3)/4)*((mh.height+3)/4)*blockSize;
            } else if (mh.format == TEXFMT_ARGB_8888) {
                size = mh.width*mh.height*4;
            } else if (mh.format == TEXFMT_ARGB_0888) {
                size = mh.width*mh.height*3;
            } else /* assume 16-bit */ {
                assert((ddsd->ddpfPixelFormat.dwFlags == DDS_RGB || ddsd->ddpfPixelFormat.dwFlags == DDS_RGBA) && ddsd->ddpfPixelFormat.dwRGBBitCount == 16);
                size = mh.width*mh.height*2;
            }

            mh.width  >>= 1;
            mh.height >>= 1;
            if (mh.height==0) mh.height=1;
            if (mh.width==0) mh.width=1;
            bytes_to_skip += size;
        }
        // Write MipHeader to file, followed by the last  total_data_bytes - bytes_to_skip
        {
            int bytes_to_write = ddsfile_len - (4 + sizeof(DDSURFACEDESC2) + bytes_to_skip);
            if (bytes_to_write > 1024) {
                Errorf("Something went wrong!  The cached mipmap header is way too big!\n");
            } else {
                fwrite(&mh, 1, sizeof(mh), outfile);
                tfh->header_size += sizeof(mh);
                fwrite(ddsfile + 4 + sizeof(DDSURFACEDESC2) + bytes_to_skip, 1, bytes_to_write, outfile);
                tfh->header_size += bytes_to_write;
            }
        }
    }
}

static char big_string[10000000],*big_string_ptr = big_string;

static void addString(char **s_ptr,char *s)
{
int        len;

    len = (int)strlen(s);
    strcpy(*s_ptr,s);
    (*s_ptr) += len;
}

static int isPow2(int x)
{
int        t;

    t =  !(x & (x-1));
    return t;
}


static void getTexClearOpts( void )
{
    gForceExternalToolUsage = false;
    gExternalTexProcessorCmdLine[0] = '\0';
    gExternalToolType = kTool_NV_NvDxtExe;
    textureTools_Start();
}

static void setOptValue( tGetTexOpt optType, const char* szOptData )
{
    #if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES
        if ( GETTEX_UPDATE_LEGACY_TOOL_EXE_OPTS() )
        {
            //
            // Translate our flags to command line options for the tools
            //
            if ( s_LegacyToolFlagsTable[optType].flagStr[gExternalToolType] != NULL )
            {
                // Add a command line flag. adding quotation marks and whatnot.
                sprintf_s( gExternalTexProcessorCmdLine, ARRAY_SIZE(gExternalTexProcessorCmdLine), "%s%s%s%s%s%s%s",
                        gExternalTexProcessorCmdLine,
                        (( *gExternalTexProcessorCmdLine == '\0' ) ? "" : " "),
                        s_LegacyToolFlagsTable[optType].flagStr[gExternalToolType],
                        ((( *s_LegacyToolFlagsTable[optType].flagStr[gExternalToolType] == '\0' ) || ( szOptData == NULL )) ? "" : " "),
                        (( s_LegacyToolFlagsTable[optType].bQuoteVal ) ? "\"" : "" ),
                        (( szOptData == NULL ) ? "" : szOptData ),
                        (( s_LegacyToolFlagsTable[optType].bQuoteVal ) ? "\"" : "" )
                    );
            }        
            #if GETTEX_USE_NVDXT_EXE_FOR_HEIGHTMAPS
                if (( gDebug_ForceStandaloneTextureToolsForHeighmaps ) && ( optType == kGetTexOpt_HeightRgb ))
                {
                    gForceExternalToolUsage = true;
                }
            #endif
        }
    #endif
        {
            //
            //  Pass this on to the library in preparation
            //  for executing the texture routine.
            //
            textureTools_AddOption( optType, szOptData );
        }
}

static void setOptFlag( tGetTexOpt optType )
{
    setOptValue( optType, NULL );
}

static bool execTexProcessor( void )
{
    if ( GETTEX_USE_LEGACY_TEXTURE_TOOL_EXES() )
    {
        printf( "\n%s\n", gExternalTexProcessorCmdLine );
        return ( system(gExternalTexProcessorCmdLine) == 0 );
    }
    else
    {
        return textureTools_Finish();
    }
}

static void debug_DumpToFile(const char *filename, void *data, U32 size)
{
    FILE *outf = fopen(filename, "wb");

    if (outf != NULL)
    {
        fwrite(data, 1, size, outf);
        fclose(outf);
    }
}

static void DumpTexOptFlags( TexOptFlags texopt_flags )
{
    if ( texopt_flags != 0 )
    {
        printf( "\nFlags set:\n" );
    }
    
    #define dumpFlag( _aFlag_ ) if ( texopt_flags & _aFlag_ ) { printf( "  %s\n", #_aFlag_ ); }

    dumpFlag( TEXOPT_FADE );
    dumpFlag( TEXOPT_TRUECOLOR );
    dumpFlag( TEXOPT_TREAT_AS_MULTITEX );
    dumpFlag( TEXOPT_MULTITEX );
    dumpFlag( TEXOPT_NORANDOMADDGLOW );
    dumpFlag( TEXOPT_FULLBRIGHT );
    dumpFlag( TEXOPT_CLAMPS );
    dumpFlag( TEXOPT_CLAMPT );
    dumpFlag( TEXOPT_ALWAYSADDGLOW );
    dumpFlag( TEXOPT_MIRRORS );
    dumpFlag( TEXOPT_MIRRORT );
    dumpFlag( TEXOPT_REPLACEABLE );
    dumpFlag( TEXOPT_BUMPMAP );
    dumpFlag( TEXOPT_REPEATS );
    dumpFlag( TEXOPT_REPEATT );
    dumpFlag( TEXOPT_CUBEMAP );
    dumpFlag( TEXOPT_NOMIP );
    dumpFlag( TEXOPT_JPEG );
    dumpFlag( TEXOPT_NODITHER );
    dumpFlag( TEXOPT_NOCOLL );
    dumpFlag( TEXOPT_SURFACESLICK );
    dumpFlag( TEXOPT_SURFACEICY );
    dumpFlag( TEXOPT_SURFACEBOUNCY );
    dumpFlag( TEXOPT_BORDER );
    dumpFlag( TEXOPT_OLDTINT );
    dumpFlag( TEXOPT_DOUBLEFUSION );
    dumpFlag( TEXOPT_POINTSAMPLE );
    dumpFlag( TEXOPT_NORMALMAP );
    dumpFlag( TEXOPT_SPECINALPHA );
    dumpFlag( TEXOPT_FALLBACKFORCEOPAQUE );
    
    #undef dumpFlag
}

static int texAppend(const char *fname_const,char *gameName,FILE *outf,int compress, TextureFileHeader *tfh)
{
    U32 size = 0;
    U8* mem = NULL;
    U8 alpha;
    char    buf[1000],tmpname[1000],buf2[1000],tmpdir[1000],tmp_tga_name[1000] = "", fname[1000];
    const char *s;
    char *s2;
    int        width,height,reprocess,nomip=0,make_jpeg=0;
    TexOpt    *tex_opt;
    TexOptFlags texopt_flags=0;
    static FILE    *errfile;
    char    *outputfile = NULL;
    bool    errorFlag = false;

    PERFINFO_AUTO_START("texApped", 1);

    strcpy(fname, fname_const);

    makefullpath(fname,buf2);
    s = strstri(buf2,"texture_library/");
    if (!s)
        s = strstri(buf2,"movies/");
    sprintf(tmpname,"%s/%s",fileTempDir(),s);
    //setgamedir(buf,"tmp",tmpname);
    s2 = strrchr(tmpname,'.');
    strcpy(s2,".dds");
    mkdirtree(tmpname);
    backSlashes(tmpname);
    backSlashes(fname);

    strcpy(tmpdir,tmpname);
    s2 = strrchr(tmpdir,'\\');
    if (s2)
        *s2 = 0;
    if (!tgaGetInfoFromFilename(fname, &alpha, &width, &height, true))
    {
        Errorf("Not processing due to invalid TGA file\n");
        PERFINFO_AUTO_STOP();
        return 0;
    }

    s = strstriConst(fname,"texture_library");
    if (s)
        s += strlen("texture_library/");
    else
        s = fname;
    tex_opt = getTexOpts(gameName, &texopt_flags, &alpha);
    if (tex_opt && tex_opt->flags & TEXOPT_JPEG)
    {
        make_jpeg = 1;
        strcpy(tmpname + strlen(tmpname)-4,".jpg");
    }

    if (strEndsWith(fname, "_bump") || strEndsWith(fname, "_bump.tga"))
        assert(texopt_flags & TEXOPT_BUMPMAP);

    makeTexOptString(gameName,buf2,ARRAY_SIZE(buf2),texopt_flags);
    nomip = texopt_flags & TEXOPT_NOMIP;
    if ((!isPow2(width) || !isPow2(height)) && !nomip)
    {
        if (!errfile)
            errfile = fopen("c:/texerrs.txt","wt");
        if (errfile)
            fprintf(errfile,"texture %s is %d x %d not a power of 2\n",s,width,height);
    }
    sprintf(buf,"%4d x %4d %s %s",width,height,alpha ? "ALPHA" : "solid",s);
    tfh->alpha = alpha; //TO DO, this ain't right sometimes!
    tfh->height = height;
    tfh->width = width;

    if (buf2[0])
    {
        strcat(buf," # ");
        strcat(buf,buf2);
    }
    sprintf(buf2,"%s\n",buf);
    addString(&big_string_ptr,buf2);
    reprocess = (force_mode==1 || fileNewer(tmpname,fname) || texOptDifferent(fname, gameName, NULL) > 0);
#undef fflush
    printf("%78c\r", ' ');
    printf("%c%s",reprocess ? '*' : ' ',buf);
        fflush(stdout);

    DumpTexOptFlags( texopt_flags );
    if (reprocess && compress)
    {
        getTexClearOpts();
        num_reprocess++;
        if ( texopt_flags & TEXOPT_NORMALMAP ) {
            setOptFlag( kGetTexOpt_SrcIsNormalMap );
        }
        if (make_jpeg)
        {
            gForceExternalToolUsage = true;
            gExternalToolType = kTool_CJpeg;
            setOptValue(kGetTexOpt_ToolPath, cjpeg_path );
            setOptValue(kGetTexOpt_SrcFile, fname );
            setOptValue(kGetTexOpt_OutputFile, tmpname );
        }
        else if (!( (texopt_flags & (TEXOPT_BUMPMAP | TEXOPT_TRUECOLOR | TEXOPT_FADE | TEXOPT_NODITHER)) || (texopt_flags & (TEXOPT_CLAMPS|TEXOPT_CLAMPT) && alpha) ))
        {
            // Neither bumpmaped or Truecolor or Fade

            gExternalToolType = kTool_DX_DxTexExe;

            strcpy(tmp_tga_name,"C:\\game\\temp.tga");
            moveFromCRoot(tmp_tga_name); // put in approved temp directory under c:\game\scratch, will create dir if doesnt exist yet
            if (!isPow2(width) || !isPow2(height))
            {
                tgaMakePow2(fname,tmp_tga_name,alpha);
                tgaQuant(tmp_tga_name,tmp_tga_name);
            }
            else
                tgaQuant(fname,tmp_tga_name);

            //Requery to find out whether this texture actually has alpha.
            //Bruce says there is a bug in NVidias compressor that means we have to use DX5 compression even when we don't 
            //have an alpha channel, and something something, I got lost.
            {
                U8 actualAlpha; 
                int junk1, junk2;
                tgaGetInfoFromFilename(tmp_tga_name, &actualAlpha, &junk1, &junk2, false);
                tfh->alpha = actualAlpha;
            }
            setOptValue( kGetTexOpt_ToolPath, dxtex_path );
            setOptFlag( nomip ? kGetTexOpt_NoMipmap : kGetTexOpt_Mipmap );
            setOptValue( kGetTexOpt_SrcFile, tmp_tga_name );
            setOptValue( kGetTexOpt_OutputFile, tmpname );
        }
        else
        {
            gExternalToolType = kTool_NV_NvDxtExe;
            
            setOptValue( kGetTexOpt_ToolPath, nvdxt_path );
            s = (const char*)getFileName((char*)fname);
            if (!isPow2(width) || !isPow2(height))
            {
                sprintf(tmp_tga_name,"c:\\%s",s);
                moveFromCRoot(tmp_tga_name);
                tgaMakePow2(fname,tmp_tga_name,alpha);
            }
            setOptValue( kGetTexOpt_SrcFile,  tmp_tga_name[0] ? tmp_tga_name : fname );
            setOptValue( kGetTexOpt_OutputDir, tmpdir );            
            outputfile = tmpname;
            setOptValue( kGetTexOpt_OutputFile, outputfile );    
            if (nomip)
                setOptFlag( kGetTexOpt_NoMipmap );
            else if (texopt_flags & (TEXOPT_CLAMPS|TEXOPT_CLAMPT))
            {
                //JE: This mip filtering mode seems to handle clamped alpha edges much better than kaiser which assumes tiling
                setOptFlag( kGetTexOpt_Cubic );
            } else {
                if (!(texopt_flags & TEXOPT_NODITHER))
                    setOptFlag( kGetTexOpt_Kaiser );
//                else
//                    printf("nodither on %s\n",fname);
            }
            //strcat(buf," -dither");
            if (texopt_flags & TEXOPT_TRUECOLOR)
            {
                if (alpha)
                {
                    setOptFlag( kGetTexOpt_U8888 );
                }
                else
                {
                    setOptFlag( kGetTexOpt_U888 );
                }
            }
            /*
            else if (force16bits)
            {
                if (alpha)
                {
                    setOptFlag( kGetTexOpt_U4444 );
                }
                else
                {
                    setOptFlag( kGetTexOpt_U565 );
                }
            }
            */
            if (texopt_flags & TEXOPT_BUMPMAP) {
                setOptFlag( kGetTexOpt_MakeN4 );
                setOptFlag( kGetTexOpt_HeightRgb );
            }
        }
        
        if (! (texopt_flags & TEXOPT_TRUECOLOR))
        {
            if (( texopt_flags & TEXOPT_BUMPMAP ) ||
                ( texopt_flags & TEXOPT_NORMALMAP )) {
                setOptFlag( kGetTexOpt_DXT5nm );
            } else {
                if (alpha && !cutout_mode) {
                    setOptFlag( kGetTexOpt_DXT5 );
                } else {
                    setOptFlag( kGetTexOpt_DXT1 );
                }
            }
        }
            
        if (!no_dxtex) {
            if (outputfile && fileExists(outputfile) ) {
                if (remove(outputfile) != 0 ) { // Remove output file, since nvdxt just overwrites (doesn't truncate)
                    Errorf("\nERROR: Failed to delete previous intermediate file:\n\"%s\"\n\nTexture processing aborted.\n", outputfile);
                    errorFlag = true;
                }
            }
            if ( ! errorFlag ) {
                bool ret;
                //
                // Do the work....
                //
                PERFINFO_AUTO_START("dxtex", 1);
                ret = execTexProcessor();
                PERFINFO_AUTO_STOP();

                GETTEXT_PREPARE_COMPARISON_FILE(( outputfile ) ? outputfile : tmpname );
                if (! ret) {
                    Errorf("\nDXTex returned an error precessing this texture!  Probably a bad .TGA file.\n\nTexture processing aborted.\n");
                    errorFlag = true;
                }
            }            
        }
        if (tmp_tga_name[0])
            remove(tmp_tga_name);

    }

    if ( ! errorFlag ) {
        if (compress)
            mem = fileAlloc(tmpname,&size);
        else
            mem = fileAlloc(fname,&size);
    }
    
    if ( size==0 ) {
        return 0;
    } else {
        assert(mem);
        if (texopt_flags & TEXOPT_FADE && !make_jpeg && !(texopt_flags & TEXOPT_NOMIP))
        {
            GETTEX_applyFade(mem, tex_opt->texopt_fade[0], tex_opt->texopt_fade[1]);

            strcpy(buf, tmpname);
            s2 = strrchr(buf,'.');
            strcpy(buf + strlen(buf)-4,"-faded.dds");

            debug_DumpToFile(buf, mem, size);
        }

        // Write pre-cached mip-map levels
        if (compress && !make_jpeg && !nomip) {
            writeMipMapHeader(outf, mem, size, tfh);
        }

        fwrite(mem,1,size,outf);
        free(mem);
    }

    PERFINFO_AUTO_STOP();

    return 1;
}

#if GETTEX_ENABLE_COMPARISON_TEST_MODE
    void debug_PrepareComparisonFile( const char* dds_file_path )
    {
        if (( gDebug_ComparisonTestMode ) && 
            ( gDebug_StandaloneTextureToolMode ) &&
            ( !gForceExternalToolUsage ))
        {
            //
            // Special debug mode ("-ComparisonTestMode" command line opt)
            //
            // We make two copies of the texture:
            //  1. Using the NVIDIA Texture Tools library.
            //    2. Using the old nvdxt.exe or dxtex.exe tools.
            // We also write out a *.txt file with the nvdxt.exe or dxtex.exe
            // command line.
            //
            FILE* fLOG = NULL;
            char newName[_MAX_PATH];
            char logName[_MAX_PATH];
            strcpy_s( newName, ARRAY_SIZE(newName), dds_file_path );
            strcpy( strchr( newName, '.' ), ".LEGACY_MODE.dds" );
            remove( newName );
            rename( dds_file_path, newName );
            sprintf_s( logName, ARRAY_SIZE(logName), "%s.txt", newName );
            fLOG = fopen( logName, "w" );
            if ( fLOG )
            {
                fprintf( fLOG, "%s\n", gExternalTexProcessorCmdLine );
                fclose( fLOG );
            }
        }
    }                    
#endif

FileScanAction deleteIfMissingTga(char *dir, struct _finddata32_t *data) {
    char fullpath[MAX_PATH];
    char buf[MAX_PATH];
    char *s;

    sprintf(fullpath, "%s/%s", dir, data->name);
    forwardSlashes(fullpath);
    if (data->name[0]=='_')
        return FSA_NO_EXPLORE_DIRECTORY;
    if (!strrchr(fullpath, '.') || stricmp(strrchr(fullpath, '.'), ".texture")!=0) {
        // not a .texture file
        return FSA_EXPLORE_DIRECTORY;
    }
    assert(!(data->attrib & _S_IFDIR));

    strcpy(buf, tgaPath);
    s = strstri(buf, "texture_library");
    if (s) {
        *s = 0;
        strcat(buf, "texture_library");
    }

    s = strstri(fullpath,"texture_library");
    if (s)
        s += strlen("texture_library/");
    else
        assert(false);
    // Make .tga

    strcat(buf, "/");
    strcat(buf, s);
    if (strrchr(buf, '.')) {
        strcpy(strrchr(buf, '.'), ".tga");
    } else {
        assert(false);
    }

    if (!fileExists(buf) && !strstri(buf, "mmMaps")) {
        bool deleted=false;
        do { // Just using Continues as a Goto
            if (strchr(buf, '#')) {
                char fileName[MAX_PATH];
                getFileNameFromGameName(buf, fileName);
                if (fileExists(fileName))
                    continue;
            }

            // Check for orphaned files we don't want to delete
            // This can happen all the time now, since people may not check their .tgas in right away
            //printf("Not deleting %s's orphaned file : %s\n", fullpath);

            printf("Would delete : %s\n", fileLocateWrite_s(fullpath, NULL, 0));
            deleted = true;
            continue;
        } while (false);
        if (!deleted) {
            // Add this name to the duplicate textures checker
            checkForDuplicateNames(buf, true);
        } else {
            // Remove from duplicate name checker
            checkForDuplicateNamesRemove(buf);
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

static void getFileNameFromGameName(char *gameName, char *fileName)
{
    // convert texture_library/path/filename#locale.tga
    //      to texture_library/#locale/path/filename.tga
    static char buf2[MAX_PATH];
    char *base, *path, *locale, *ext;
    strcpy(buf2, gameName);
    base = buf2;
    path = strstri(base, "texture_library");
    if (path) {
        path += strlen("texture_library");
    } else {
        path = strchr(base, '/');
    }
    *path=0;
    path++;
    locale = strchr(path, '#');
    *locale = 0;
    locale++;

    ext = strchr(locale, '.');
    *ext=0;
    ext++;
    sprintf(fileName, "%s/#%s/%s.%s", base, locale, path, ext);
}

static void getGameNameFromFileName(char *fileName, char *gameName)
{
    // convert texture_library/#locale/path/filename.tga
    //      to texture_library/path/filename#locale.tga
    static char buf2[MAX_PATH];
    char *base, *path, *locale, *ext;
    strcpy(buf2, fileName);
    base = buf2;
    locale = strchr(base, '#');
    if (locale[-1]=='/')
        locale[-1]=0;
    *locale=0;
    locale++;
    path = strchr(locale, '/');
    if (!path) {
        // already in appropriate format
        strcpy(gameName, fileName);
    } else {
        *path=0;
        path++;
        ext = strchr(path, '.');
        *ext=0;
        ext++;
        sprintf(gameName, "%s/%s#%s.%s", base, path, locale, ext);
    }
}

StashTable htDupCheck=0;
StashTable htHasDups=0;
bool got_dups=false;

static bool checkForDuplicateNames(const char *texturefilename, bool bPrintToConsole)
{
    char filename[MAX_PATH];
    char pathname[MAX_PATH];
    char *s;
    StashElement element;

    if (!htDupCheck) {
        htDupCheck = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);
        htHasDups = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
    }
    strcpy(pathname, texturefilename);
    forwardSlashes(pathname);
    s = (char*)getFileName(pathname);
    strcpy(filename, s);
    s[-1]=0;
    s = strrchr(filename, '.');
    if (s)
        *s=0;
    if (!stashFindElement(htDupCheck, filename, &element)) {
        stashAddPointer(htDupCheck, filename, strdup(pathname), false);
    } else {
        if (stashFindPointer(htHasDups, filename, NULL)) {
            if(bPrintToConsole)
            {
                // Already noted as a duplicate!
                printf("DupTex %s\n",texturefilename);
            }
            return true;
        }
        if (stricmp(pathname, stashElementGetPointer(element))==0)
        {
            // OK
        } else {
            if(bPrintToConsole)
            {
                printf("DupTex %s\n",texturefilename);
                sprintf(pathname, "%s/%s.tga", stashElementGetPointer(element), stashElementGetStringKey(element));

                printf("DupTex %s\n",pathname);
            }
            stashAddInt(htHasDups, filename, 1, false);
            got_dups = true;
            return true;
        }
    }
    return false;
}

static bool checkForDuplicateNamesRemove(char *texturefilename)
{
    char filename[MAX_PATH];
    char pathname[MAX_PATH];
    char *s;
    char* pcResult;
    if (!htDupCheck) {
        htDupCheck = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);
        htHasDups = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
    }
    strcpy(pathname, texturefilename);
    forwardSlashes(pathname);
    s = (char*)getFileName(pathname);
    strcpy(filename, s);
    s[-1]=0;
    s = strrchr(filename, '.');
    if (s)
        *s=0;
    if (!stashFindPointer(htDupCheck, filename, &pcResult)) {
        // fine
    } else {
        stashRemoveInt(htHasDups, filename, NULL);
        if (stricmp(pathname,pcResult)==0)
        {
            stashRemovePointer(htDupCheck, filename, NULL);
        } else {
            // fine
        }
    }
    return true;
}


void initDuplicateCheck(void)
{
    int i;
    for (i=0; i<eaSize(&tex_defs); i++) {
        TexDef *td = tex_defs[i];
        checkForDuplicateNames(td->name, spew_initial_dupes==1);
    }
}

static int reason=0;

//Get texture name w/o game/src and with slashes all forward
static void getRelativeTextureName(char *name, char *outbuf)
{
    const char *s;
    forwardSlashes(name);
    s = strstriConst(name,"/src/");
    if (!s)
        s = name;
    else
        s += strlen("/src/");
    strcpy(outbuf,s);
}

int num_newer=0;
int num_not_changed=0;
int num_changed=0;
static int texWriteInnerLoop(TexDef *tex, bool just_init) {
    int j;    
    char    srcName[1000], gameName[1000];
    int        save_a_slot = 0xBADF00D;
    int        reprocess;
    char output_name[MAX_PATH];
    FILE *outfile = NULL;
    TextureFileHeader tfh;
    const char *lastauthor=NULL;
    int ret;
    const char * addFiles[2] = {NULL, NULL};

    PERFINFO_AUTO_START("checks", 1);
    PERFINFO_AUTO_START("getRelativeTextureName", 1);
    //Get texture name w/o game/src and with slashes all forward
    getRelativeTextureName(tex->name, srcName);

    // Fix case to lowercase
    if( !strnicmp( srcName, "texture_library", strlen( "texture_library" ) ) )
    {
        strncpy( srcName, "texture_library", strlen( "texture_library" ) );
    }

    strcpy(gameName,srcName);

    // Get output filename (.texture)
    if (strchr(srcName, '#')) {
        // Localized name
        getGameNameFromFileName(srcName, gameName);
    }
    sprintf(output_name, "%s/%s", fileDataDir(), gameName);
    strcpy(strrchr(output_name,'.'), ".texture");

    PERFINFO_AUTO_STOP_START("checkForDuplicateNames", 1);
    if (checkForDuplicateNames(tex->name, !just_init)) {
        reason = 16;
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
        return 0;
    }

    PERFINFO_AUTO_STOP_START("x_", 1);
    if (strStartsWith(getFileName(tex->name), "x_")) {
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
        return 0;
    }

    /*{
        TgaInfo    info;
        int        i,size, components;

        file = fopen(fname,"rb");
        if (!file)
            return;
        info.data = tgaLoad(file,&info.width,&info.height);
    }*/

    //Set whether to compress as a dds or leave a tga
    PERFINFO_AUTO_STOP_START("makeTexHeader", 1);
    makeTexHeader(tex->name, gameName, &tfh, 0); // get this texture's texopt (built on trickload)
    if (just_init) {
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
        return 0;
    }
    PERFINFO_AUTO_STOP_START("name fixup", 1);
// We don't get width and height here!
//    if (tfh.width > MAX_TEX_SIZE || tfh.height > MAX_TEX_SIZE) {
//        reason = 8;
//        PERFINFO_AUTO_STOP();
//        return 0;
//    }

    tex->compress = 1;
    // Change internal name to match file type
    {
        char *s;
        s = strrchr(gameName,'.');
        if (tfh.flags & TEXOPT_JPEG)
            strcpy(s,".jpg");
        else
            strcpy(s,".dds");

        //fill in spaces in texture names
        for(s=gameName;*s;s++) if (*s == ' ') *s = '_';
        for(s=output_name;*s;s++) if (*s == ' ') *s = '_';
    }

    printf("");

    // Check to see if this needs to be reprocessed
    {
        bool bFileNewer;
        bool bTexOptDifferent;
        PERFINFO_AUTO_STOP_START("fileNewer", 1);
        bFileNewer = fileNewer(output_name,tex->name);
        PERFINFO_AUTO_STOP_START("texOptDifferent", 1);
        bTexOptDifferent = texOptDifferent(tex->name, gameName,scan_log) > 0;
        reprocess = (force_mode || bFileNewer || bTexOptDifferent);
        if (bFileNewer)
        {
            num_newer++;
            if (scan_log)
                fprintf(scan_log, "  newer               %s\n", tex->name);
        }
        if (bTexOptDifferent)
            num_changed++;
        reason = 0;
        if (!reprocess) {
            num_not_changed++;
            if (!bFileNewer) {
                reason |= 1;
            }
            if (!bTexOptDifferent) {
                reason |= 2;
            }
        }
    }

    /*        // extra check to get the latest version if we're going to be processing
    if (no_check_out==0 && reprocess && !force_mode && fileNewer(output_name,tex->name)) {
    // Get the latest version of the .texture file just in case
        perforceSyncForce(output_name, PERFORCE_PATH_FOLDER);
    }

    // re-evaluate, in case the new .texture file is, well, newer
    reprocess = (force_mode || fileNewer(output_name,tex->name) || texOptDifferent(tex->name) > 0);
    */
    PERFINFO_AUTO_STOP();
    if (!reprocess) {
        PERFINFO_AUTO_STOP();
        return 0;
    }

    /* rules for processing:
    When GetTex sees a .TGA file that is newer than its corresponding .texture file, it sees that this
    file needs to be processed.  It will NOT process the file if the .TGA is checked out by someone else.
    If no one has the file checked out, then it will only process it if you were the last person to check
    it in.
    */

    PERFINFO_AUTO_STOP_START("mkdirtree", 1);

    mkdirtree(output_name);

    PERFINFO_AUTO_STOP_START("perforceChecks", 1);

    PERFINFO_AUTO_STOP_START("checkOut", 1);
    _chmod(output_name, _S_IWRITE | _S_IREAD);
    ret = NO_ERROR;

    PERFINFO_AUTO_STOP_START("write", 1);
    num_repack++;

    // Do a warning for any texture with Portal in its name
    {
        char *name = strrchr(output_name, '/');
        if (!name) name = output_name;
        if (strstri(name, "PORTAL")) {
            printf("\nWARNING: Texture name (%s) contains the phrase \"PORTAL\", it will not be visible\n", name);
            printf("   in-game, please rename the texture if you wish it to appear on an object\n\n");
        }
    }

    outfile = fileOpen(output_name, "wb");
    if (!outfile) {
        FatalErrorf("Error opening %s for writing!", output_name);
        PERFINFO_AUTO_STOP();
        return 0;
    }

    //Write texture header to the .texture file
    tfh.header_size = sizeof(tfh) +
        (int)(strlen(gameName) + 1); // filename

    fwrite(&tfh.header_size, 1, 4, outfile);
    assert(sizeof(tfh) % 4 ==0);
    for (j=0; j<sizeof(tfh)/4 - 1; j++) {
        fwrite(&save_a_slot, 1, 4, outfile);
    }

    fwrite(gameName, 1, strlen(gameName)+1, outfile); // Filename
    assert(ftell(outfile)==tfh.header_size);

    if (!texAppend(tex->name,gameName,outfile,tex->compress,&tfh)) //write the data to .texture
    {
        // Failed
        fclose(outfile);
        fileForceRemove(output_name);
        printf("\n");
        PERFINFO_AUTO_STOP();
        return 0;
    }

    tfh.file_size = ftell(outfile) - tfh.header_size;    //go back and write length of the data to the header
    // also write the modified length of header_size if texAppend wrote anything else
    fseek(outfile, 0, SEEK_SET);    //go to header
    fwrite(&tfh,1,sizeof(tfh),outfile);
    //No longer true (because of mip caching): assert(ftell(outfile)==tfh.header_size - strlen(gameName) - 1);

    fclose(outfile);
    // Update timestamp of the .texture file with that of the .tga
    // This is bad because if someone changes a Trick file, the .texture file will still have the
    //  exact same timestamp, and will probably be loaded from a pig instead!
    //if (!force_mode && !override_perforce)
    //    fileTouch(output_name, tex->name);

    PERFINFO_AUTO_STOP();
    return 1;
}

//tex_defs        a list textures in the given directory to be processed
static void texWrite()
{
    int        i;
    int numdefs;
    int num_skipped=0;

    /*for each TexDef in tex_defs write this implicit structure to .texture files:
        int        header_length    // number of bytes in the header that always need to get loaded,
                                // consequently also the offset of the data
        int        file_bytes        // data length
        int        width            // width of the texture
        int        height            // height of the texture
        int        flags            // tex_opt flags
        F32        fade[2]            // values for the Fade setting
        U8        alpha            // boolean
        U8        padding[3]
        char    name[variable length,null terminated] 
    */

    PERFINFO_AUTO_START("pruneDeleted", 1);
    if (doprune && (strEndsWith(tgaPath, "texture_library") || strEndsWith(tgaPath, "texture_library/") || strEndsWith(tgaPath, "texture_library\\"))) {
        // Delete .texture files for which there are no corresponding .tga files
        fileScanAllDataDirs("texture_library", deleteIfMissingTga);
    }
    PERFINFO_AUTO_STOP_START("loop over texDefs", 1);

    //(set whether this TexDef is compressed(.dds) or not (.tga) in the texdef)
    numdefs = eaSize(&tex_defs);
    for(i=0; i<numdefs; i++)
    {
        bool skip_it=false;
        // Check for exclusion from set
        static struct {
            int *scan_var;
            char*skip_me[10];
        } filters[] = {
            {&scan_world, {"loading_screens", "MAPS", "V_MAPS", "WORLD"}},
            {&scan_character, {"ENEMIES", "NPCS", "PLAYERS", "V_ENEMIES", "V_MasterMind_Pets", "V_NPCS", "V_PLAYERS",}},
        };

        if ((i+1)%100==0 || (i+1) == numdefs)
            printf("%d / %d (%2d%%) \r", i+1, numdefs, (i+1)*100 / numdefs);

        if (!do_scan) {
            skip_it = true;
        } else if ((!scan_world || !scan_character || !scan_other))
        {
            char path[MAX_PATH];
            static char last_path[MAX_PATH];
            static bool last_scanit=true;
            char *s;
            getRelativeTextureName(tex_defs[i]->name, path);
            if (strStartsWith(path, "texture_library/"))
                strcpy(path, path + strlen("texture_library/"));
            s = strchr(path, '/');
            if (s)
                *s=0;
            num_skipped++;
            if (stricmp(path, last_path)==0) {
                if (!last_scanit)
                    skip_it = true;
            } else {
                int i2, j;
                bool match=false;
                bool scanit=true;
                strcpy(last_path, path);
                last_scanit = false;
                for (i2=0; i2<ARRAY_SIZE(filters); i2++) {
                    for (j=0; j<ARRAY_SIZE(filters[i2].skip_me) && filters[i2].skip_me[j]; j++) {
                        if (stricmp(path, filters[i2].skip_me[j])==0) {
                            match = true;
                            break;
                        }
                    }
                    if (match) {
                        if (!*filters[i2].scan_var)
                            scanit = false;
                        else
                            scanit = true;
                        break;
                    }
                }
                if (match && !scanit)
                    continue; // Skip it
                if (!match) {
                    // Other
                    if (!scan_other)
                        skip_it = true;
                    else
                        last_scanit = true;
                } else {
                    last_scanit = true;
                }
            }
            if (!skip_it)
                num_skipped--;
        }

        #if GETTEX_ENABLE_COMPARISON_TEST_MODE
            if ( gDebug_ComparisonTestMode )
            {
                // Do an extra run, forcing standalone texture mode, for comparison.
                gDebug_StandaloneTextureToolMode = true;
                texWriteInnerLoop(tex_defs[i], skip_it);
                gDebug_StandaloneTextureToolMode = false;
            }
        #endif
        PERFINFO_AUTO_START("texWriteInnerLoop", 1);
        texWriteInnerLoop(tex_defs[i], skip_it);
        PERFINFO_AUTO_STOP();
    }
    PERFINFO_AUTO_STOP();

    //printf("%d newer, %d texOpt changed, %d unchanged, %d skipped\n", num_newer, num_changed, num_not_changed, num_skipped);
    ADD_MISC_COUNT(num_newer, "Count:New");
    ADD_MISC_COUNT(num_changed, "Count:TexOptChange");
    ADD_MISC_COUNT(num_not_changed, "Count:NotChanged");
    ADD_MISC_COUNT(num_skipped, "Count:Skipped");

    PERFINFO_AUTO_START("write textures.txt", 1);
    {
        //write textures.txt with big_string
        FILE * header_file = 0;
        char header_name[1000];
        sprintf(header_name, "%s/texture_library/textures.txt", fileDataDir());
        _chmod(header_name, _S_IWRITE | _S_IREAD);
        header_file = fileOpen(header_name,"wt");
        //printf("TXT file: %s\n", header_name);
        fwrite(big_string,big_string_ptr - big_string,1,header_file);
        fclose(header_file);
        //perforceAdd(header_name, PERFORCE_PATH_FILE);
        //perforceSubmit(header_name, PERFORCE_PATH_FILE, "AUTO: texWrite");
    }
    PERFINFO_AUTO_STOP();
}

/*for quicksort to alphabetize tex_defs */ 
static int cmpTexDef(const void* vaData, const void* vbData)
{
    const TexDef ** va = (const TexDef **)vaData;
    const TexDef ** vb = (const TexDef **)vbData;
const char    *sa=0,*sb=0;
int ret;

    sa = strrchr((*va)->name,'/');
    sb = strrchr((*vb)->name,'/');
    if (!sa)
        sa = (*va)->name;
    if (!sb)
        sb = (*vb)->name;
#if 0
    if (stricmp(sa,sb) == 0)
        printf("dup tex: %s %s\n",va->name,vb->name);
#endif
    ret = stricmp(sa,sb);
    if (ret==0) {
        ret = stricmp((*va)->name, (*vb)->name);
    }
    return ret;
}

/*Error check to make sure there aren't two textures with the same name*/
static int checkDups()
{
    return got_dups;
}

#if 0
char* getExecutableName()
{  
     int result;  
     static char moduleFilename[MAX_PATH];  
     result = GetModuleFileName(NULL, moduleFilename, MAX_PATH);  
     assert(result);          // Getting the executable filename should not fail.  
  
     return moduleFilename;  
}
#endif


int printTexInfo(const char *filename) {
    FILE *f = fopen(filename, "rb");
    TextureFileHeader tfh;
    char name[MAX_PATH];
    TextureFileMipHeader mh;
    bool hasmh=false;
    int i;

    if (!f) {
        printf("Error opening .texture file : %s\nTry running gettex again after closing the game\n", filename);
        return 1;
    }
    fread(&tfh, sizeof(tfh), 1, f);
    i=0;
    do {
        name[i++] = fgetc(f);
    } while (name[i-1]);
    if (tfh.pad[2] == '2' && ftell(f) < tfh.header_size) {
        fread(&mh, sizeof(mh), 1, f);
        hasmh=true;
    }
    fclose(f);
    printf("Texture file : %s\n", filename);
    printf(" internal name : %s\n", name);
    printf(" %d x %d (%s)\n", tfh.width, tfh.height, tfh.alpha ? "ALPHA" : "solid");
    printf(" flags: %d\n", tfh.flags);
    for (i=0; i<32; i++) {
        if (tfh.flags & (1<<i)) {
            if (i<ARRAY_SIZE(flag_to_name))
                printf("\t%s\n", flag_to_name[i]);
            else
                printf("\tUNKNOWN FLAG : %d\n", 1<<i);
        }
    }

    printf("\n");
    printf("Debug info:\n");
    printf("header_size: %d\nfile_size: %d\nmip overhead: %d\n", tfh.header_size, tfh.file_size, tfh.header_size - sizeof(tfh) - strlen(name)-1);
    printf("Texture file version: %c%c%c\n", tfh.pad[0], tfh.pad[1], tfh.pad[2]);
    if (hasmh) {
        printf("Cached mip levels:\n");
        printf("  %d x %d\n", mh.width, mh.height);

        printf("  format: %d", mh.format);

        if (mh.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
            printf(" (DXT1)");
        else if  (mh.format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
            printf(" (DXT3)");
        else if  (mh.format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
            printf(" (DXT5)");
        else if  (mh.format == TEXFMT_ARGB_8888)
            printf(" (ARGB_8888)");
        else if  (mh.format == TEXFMT_ARGB_0888)
            printf(" (RGB_888)");
        else if  (mh.format == TEXFMT_ARGB_0565)
            printf(" (RGB_565)");
        else if  (mh.format == TEXFMT_ARGB_1555)
            printf(" (ARGB_1555)");
        else if  (mh.format == TEXFMT_ARGB_4444)
            printf(" (ARGB_4444)");

        printf("\n");
    }

    if (!hasmh && !(tfh.flags & TEXOPT_NOMIP)) {
        printf("This file could use reprocessing to acquire cached mip data.\n");
        return 1;
    }
    return 0;
}


static FILE* gettex_lock_handle=NULL;
static char *lockfilename = "c:\\gettex.lock";
static void releaseGettexLock() {
    fclose(gettex_lock_handle);
    fileForceRemove(lockfilename);
    gettex_lock_handle = 0;
}

static void waitForGettexLock() {
    fileForceRemove(lockfilename);
    while ((gettex_lock_handle = fopen(lockfilename,"wl")) == NULL) {
        Sleep(500);
        fileForceRemove(lockfilename);
    }
    if (gettex_lock_handle != NULL) {
        fprintf(gettex_lock_handle, "gettex.exe\r\n");
    }
}


static void reprocessTex(const char *fullpath, int print_errors)
{
    TexDef dummy;
    char buf[MAX_PATH];
    dummy.name = buf;

    waitForGettexLock();
    fileWaitForExclusiveAccess(fullpath);

    if (!fileExists(fullpath))
        return;
    strcpy(dummy.name, fullpath);
    dummy.compress=1; // does this care?
    if (texWriteInnerLoop(&dummy, false))
    {
        //printf("Caught and reprocessed '%s'\n", relpath);
    }
    else if (print_errors)
    {
        printf("Didn't reprocess '%s' because either it isn't newer, it's not yours, or it is bad.\n", fullpath);
        if (reason & 1) {
            printf("  Reason:  File is not newer\n");
        }
        if (reason & 2) {
            printf("  Reason:  TexOpt hasn't changed\n");
        }
        if (reason & 4) {
            printf("  Reason:  Perforce thinks it's not yours\n");
        }
        if (reason & 8) {
            printf("  Reason:  Bad texture (larger than %dx%d or bad file)\n", MAX_TEX_SIZE, MAX_TEX_SIZE);
        }
        if (reason & 16) {
            printf("  Reason:  Duplicate texture name (if textures have been deleted, you need to restart GetTex to prune them)\n");
        }
    }

    releaseGettexLock();
}

static void reprocessOnTheFly(FolderCache* fc, FolderNode* node, const char *relpath, int when) {
    char fullpath[MAX_PATH];

    if (strstr(relpath, "/_") || relpath[0]=='_') {
        printf("Texture '%s' begins with an underscore, not processing.\n", relpath);
        return;
    }

    if (fc == fctexlib) {
        sprintf_s(fullpath, _countof(fullpath), "%s/%s", tgaPath, relpath);
        reprocessTex(fullpath, 1);
    } else {
        int i;
        bool knownBranch = false;
        for ( i=0; i < eaSize( &fcbtexlibs_OtherBranches ); i++ ) {
            if ( fc == fcbtexlibs_OtherBranches[i] ) {
                // If a second copy of GetTex is running we can't be sure
                // about what's going on. Maybe they are working in two (or more)
                // branches simultaneously...? So don't complain if there are
                // multiple copies of GetTex.exe currently active.
                knownBranch = true;
                if ( ProcessCount("gettex.exe") == 1 ) {
                    Errorf("ERROR: TGA file \"%s\"\n  saved into %s, but we are monitoring %s, NOT processing.\n", relpath, fc->gamedatadirs[0], fctexlib->gamedatadirs[0]);
                }
                break;
            }
        }
        if ( ! knownBranch ) {
            Errorf("ERROR: TGA file \"%s\"\n  saved into unknown location: \"%s\", NOT processing.\n", relpath, fc->gamedatadirs[0]);
        }
    }
}


static int stashReprocessTex(StashElement element)
{
    reprocessTex(stashElementGetStringKey(element), 0);
    return 1;
}

static void trickReloaded(const char *relpath)
{
    TexList *tl;

    if (relpath[0] == '/')
        relpath++;

    if (!stashFindPointer(texs_by_trick_file, relpath, &tl))
        return;

    stashForEachElement(tl->texs_hash, stashReprocessTex);
}

static bool getBranchRoot( const char* aPath, char* branchRoot, size_t branchRootBuffSz )
{
    //
    //    aPath can have c:\\ or c:/ at the front, or not.
    //
    //    Copies the c:\game or c:\gamefix[n] portion of aPath
    //    to branchRoot -- lower case and with forward slashes.
    //
    //    Returns true if is in the standard naming convention.
    //
    char* lastSlash;
    static const char* szGameRoot    = "c:/game";
    static const char* szGameFixRoot = "c:/gamefix";
    bool followsNamingConvention = false;
    
    // Skip c:\\ in aPath
    if (( tolower( aPath[0] ) == 'c' ) &&
        ( aPath[1] == ':' ) &&
        (( aPath[2] == '\\' ) || ( aPath[2] == '/' ))) {
        aPath += 3;
    }
    
    strncpy_s( branchRoot,     branchRootBuffSz,     "c:/", _TRUNCATE );
    strncpy_s( (branchRoot+3), (branchRootBuffSz-3), aPath, _TRUNCATE );
    _strlwr_s( branchRoot, branchRootBuffSz );
    forwardSlashes( branchRoot );
    
    // Chop branchRoot off at the end of the root-level directory
    // E.g., "c:/game/foo/bar/whatever" -> "c:/game"
    lastSlash = strchr( branchRoot+3, '/' );    // next slash after c:/...
    if ( lastSlash != NULL ) {
        *lastSlash = '\0';
    }
    if ( strcmp( szGameRoot, branchRoot ) == 0 ) {
        // Simple case of the standard branch root, c:/game.
        followsNamingConvention = true;        
    } else if ( strStartsWith( branchRoot, szGameFixRoot ) ) {
        // Could be c:/gamefix or it could be c:/gamefix[n], where n is any sequence 
        // of digits (c:/gamefix2, c:/gamefix753, etc.).
        const char* p = ( branchRoot + strlen(szGameFixRoot) );
        followsNamingConvention = true;
        while (( *p != '\0' ) && ( *p != '/' )) {
            if ( ! isdigit( *p ) ) {
                // Has something other than a digit after szGameFixRoot,
                // so doesn't follow the naming format.
                followsNamingConvention = false;
                break;
            }
            p++;
        }
    }
    
    return followsNamingConvention;
}

static bool checkOtherBranches( const char* watchPath )
{
    //
    //    Watch other branches found in the local directories so
    //    we can warn the user if he accidentally saves a modified
    //    .tga file in the wrong branch's local folders.
    //
    //    MAINTENANCE NOTE:
    //        Assumes c:\game, c:\gamefix, c:\gamfix2, etc. naming 
    //        convention.
    //
    //    Returns true if we set up some other locations to watch.
    //
    struct _finddata_t    fileInfo = { 0 };
    intptr_t            dirHandle;
    bool                moreDirs = false;
    char                watchDir[32];
    char                otherTgaPath[MAX_PATH]; // A path TGAs shouldn't be saved to
    bool                addedCaches = false;

    getBranchRoot( watchPath, watchDir, _countof(watchDir) );
    
    dirHandle = _findfirst( "c:\\game*", &fileInfo );
    moreDirs = ( dirHandle != -1 );
    while ( moreDirs ) {
        if (( fileInfo.attrib & _A_SUBDIR ) == _A_SUBDIR ) {
            char testDir[32];
            if (( getBranchRoot( fileInfo.name, testDir, _countof(testDir) ) ) &&
                ( strcmp( testDir, watchDir ) != 0 ))  {
                // We found a branch folder other than the one we're actively watching.
                FolderCache* newCache = FolderCacheCreate();
                if ( fcbtexlibs_OtherBranches == NULL ) {
                    eaCreate( &fcbtexlibs_OtherBranches );
                }
                eaPush( &fcbtexlibs_OtherBranches, newCache );
                sprintf_s( otherTgaPath, _countof(otherTgaPath), "%s%s",
                    testDir, ( watchPath + strlen(watchDir) ) );
                forwardSlashes( otherTgaPath );
                FolderCacheAddFolder(newCache, otherTgaPath, 0);
                FolderCacheQuery(newCache, NULL);
                addedCaches = true;
            }
        }
        moreDirs = ( _findnext( dirHandle, &fileInfo ) == 0 );
    }
    _findclose(dirHandle);
    return addedCaches;
}

//extern int folder_cache_debug;
void getTexMonitor(char *tgaPath) {
    char path[MAX_PATH];
    bool bDoBadWatch=false;
    //folder_cache_debug = 2;

    fctexlib = FolderCacheCreate();
    //FolderCacheSetMode(FOLDER_CACHE_MODE_I_LIKE_PIGS);
    strcpy(path, tgaPath);
    forwardSlashes(path);
    if (!strEndsWith(path, "/"))
        strcat(path, "/");
    loadstart_printf("Caching %s... ", path);
    FolderCacheAddFolder(fctexlib, path, 0);
    FolderCacheQuery(fctexlib, NULL);
    loadend_printf("done.");

    // Check to see if they have other branches, and try to ensure
    // they don't accidentally update the wrong files.
    checkOtherBranches( tgaPath );

    consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
    printf("Now monitoring for changes in .TGA files.\n");
    consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);

    reprocessing_on_the_fly = 1;

    trickSetReloadCallback(trickReloaded);
    FolderCacheSetCallbackEx(FOLDER_CACHE_CALLBACK_UPDATE, "*.tga", reprocessOnTheFly);
    while (true) {
        FolderCacheDoCallbacks();
        checkTrickReload();
        checkLODInfoReload();
        Sleep(200);
        if (_kbhit() && _getch()=='t') {
            trickReload();
        }
    }
}

void checkDataDirOutputDirConsistency(void)
{
    char *s;
    char data_dir[MAX_PATH];
    char src_dir[MAX_PATH];
    strcpy(data_dir, fileDataDir());
    strcpy(src_dir, tgaPath);
    forwardSlashes(data_dir);
    forwardSlashes(src_dir);
    s = strchr(data_dir, '/'); // c:
    if (s && strchr(s+1, '/'))
        s = strchr(s+1, '/'); // c:/game
    if (s)
        *s = 0;
    s = strchr(src_dir, '/'); // c:
    if (s && strchr(s+1, '/'))
        s = strchr(s+1, '/'); // c:/game
    if (s)
        *s = 0;
    if (stricmp(data_dir, src_dir)!=0) {
        FatalErrorf("Trying to process from %s into %s, this is not allowed!", src_dir, data_dir);
    }
}

static void gettexErrorCallback( const char* errorMsg )
{
    consoleSetColor(COLOR_RED|COLOR_BRIGHT, 0);
    printf("\n%s\n",errorMsg);
    printf("Press a key to continue..");
    if ( _getche() == 0 ) _getche();
    consoleSetDefaultColor();
    printf("\n" );
    filelog_printf(0, "Error: \"%s\"", errorMsg);
}

static void CreateToolUtilPath(char* dest, int bufSize, const char* toolName)
{
    char buf[64];
    strcpy_s(dest, bufSize, fileDataDir());
    strstriReplace_s(dest, bufSize, "/data", "/tools");
    sprintf(buf, "/util/%s", toolName);
    strcat_s(dest, bufSize, buf);
}

int main(int argc,char **argv)
{
    int        i,arg_offset = 0;
    U32        tex_mtime = 0;
    int        no_pig = 0;
    int        no_pause = 0;
    int        no_prompt = 0;

    //Get arguments
    if (argc < 2)
    {
        printf("Usage: gettex [-stayresident] [-nopig] [-override] [-force] [-forcerepack] [-cutout] [-NoTexOptCheck] [-nodxtex] <dir to scan>\n");
        printf("   Ex: gettex c:/game/src/texture_library\n");
        printf("   Or: gettex -texinfo file.texture\n");
        printf("\
    -nopig        Does not load any data from pig files\n\
    -override    Does not care who has the .tga checked out, tries to process it anyway\n\
    -force        Forces all files to be reprocessed\n\
    -forcerepack    Forces all files to be repacked (will not re-create .dds if it already exists\n\
    -cutout        ??\n\
    -NoTexOptCheck    Doesn't check for changes in trick files\n\
    -nodxtex    Doesn't actually call dxtex (will use old .dds files if they're there?)\n\
    <dir to scan>    The folder to process, can also be a list of files to process\n\
    -texinfo    Displays the information contained in the header of a .texture file\n\
    -stayresident    Leaves GetTex running, monitoring the c:\\game\\src\\texture_library for new .tga files\n\
    -doprune    Prune/delete orphaned .texture files\n\
    -noprompt    Does not ask for what options to use\n\
    -nopause    Does not pause after displaying texture info with -texinfo\n\
    -scancheckalpha    Check for 32bpp to 24bpp conversion on file scan\n\
    -scanlog    Log the differences to C:\\GetTex_scanlog.txt\n\
    -spewdupes    Spew list of all duplicate textures at startup\n\
    -cjpeg <path>    Custom cjpeg.exe file path\n"
#if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES
"    -nvdxt <path>        Custom nvdxt.exe file path\n"
"    -use_nvdxt_exe 1/0    Use NVIDIA/DX standalone texture tools (legacy mode support) "
    #if GETTEX_DEFAULT_TO_LEGACY_TEXTURE_TOOL_EXES
        " Default=1.\n"
    #else
        " Default=0.\n"
    #endif
#endif
#if GETTEX_ENABLE_COMPARISON_TEST_MODE
"    -ComparisonTestMode    Writes comparison DDS files using old and new gettex modes\n"
#endif
#if GETTEX_USE_NVDXT_EXE_FOR_HEIGHTMAPS
"    -nvdxt_exe_for_bumpmaps 1/0 If 1, uses nvdxt.exe to process heightmaps.\n"
#endif
    );
        exit(0);
    }

    EXCEPTION_HANDLER_BEGIN

    consoleInit(120, 500, 0);
    ErrorfSetCallback( gettexErrorCallback );
    initLegacyToolFlagsTable();

    printf("%s\n", getExecutableName());
    printf("Command line: ");
    for(i=1; i<argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    sharedMemorySetMode(SMM_DISABLED);
    setAssertMode((!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0) |
        ASSERTMODE_DEBUGBUTTONS);
    FolderCacheSetManualCallbackMode(1);

    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-force")==0) {
            arg_offset++;
            force_mode = 1;
            // force running dxtex on the file again and rebuilding a .texture file
        }
        else if (stricmp(argv[i],"-forcerepack")==0) {
            arg_offset++;
            force_mode = 2;
            // force at least rebuilding a .texture file (using saved .dds files)
        }
        else if (stricmp(argv[i],"-cutout")==0) {
            arg_offset++;
            cutout_mode = 1;
        }
        else if (stricmp(argv[i],"-newer")==0) {
            FatalErrorf("-newer is no longer valid!  Use GetTex.bat or GetTexAndMonitor.bat!");
            exit(0);
        }
        else if (stricmp(argv[i],"-notexoptcheck")==0) {
            arg_offset++;
            no_tex_opt_check = 1;
        }
                // avoid calling dxtex if you're trying to run from a remote login
        else if (stricmp(argv[i],"-nodxtex")==0) {
            arg_offset++;
            no_dxtex = 1;
        }
        else if (stricmp(argv[i],"-nopig")==0) {
            arg_offset++;
            no_pig = 1;
        }
        else if (stricmp(argv[i], "-override")==0) {
            arg_offset++;
            override_perforce = 1;
        }
        else if (stricmp(argv[i], "-stayresident")==0) {
            arg_offset++;
            monitor = 1;
        }
        else if (stricmp(argv[i], "-doprune")==0) {
            arg_offset++;
            doprune=1;
        }
        else if (stricmp(argv[i], "-noprompt")==0) {
            arg_offset++;
            no_prompt=1;
        }
        else if (stricmp(argv[i], "-nopause")==0) {
            arg_offset++;
            no_pause = 1;
        }
        else if (stricmp(argv[i], "-nvdxt")==0) {
            arg_offset++;
            i++;
            arg_offset++;
            Strcpy(nvdxt_path, argv[i]);
        }
        else if (stricmp(argv[i], "-cjpeg")==0) {
            arg_offset++;
            i++;
            arg_offset++;
            Strcpy(cjpeg_path, argv[i]);
        }
        else if (stricmp(argv[i], "-scanlog")==0) {
            arg_offset++;
            scanlog = 1;
        }
        else if (stricmp(argv[i], "-scancheckalpha")==0) {
            arg_offset++;
            scan_check_alpha = 1;
        }
        else if (stricmp(argv[i], "-spewdupes")==0) {
            arg_offset++;
            spew_initial_dupes = 1;
        }
        else if (stricmp(argv[i], "-texinfo")==0) {
            // Assume the next argument ends in .texture, and the next case will catch it
        }

        #if GETTEX_SUPPORT_LEGACY_TEXTURE_TOOL_EXES
            else if (stricmp(argv[i], "-use_nvdxt_exe")==0) {
                // has an option value after the switch: 1 or 0.
                if (( (i+1) < argc ) &&
                    (( *argv[i+1] == '0' ) || ( *argv[i+1] == '1' ))) {
                    gDebug_StandaloneTextureToolMode = ( *argv[i+1] == '1' ) ? true : false;
                    arg_offset += 2;
                    i++;
                } else {
                    printf( "Command line syntax error. Should be \"-use_nvdxt_exe 0\" or \"-use_nvdxt_exe 1\".\n" );
                    exit(1);
                }
            }
        #endif
        #if GETTEX_USE_NVDXT_EXE_FOR_HEIGHTMAPS
            else if (stricmp(argv[i], "-nvdxt_exe_for_bumpmaps")==0) {
                // has an option value after the switch: 1 or 0.
                if (( (i+1) < argc ) &&
                    (( *argv[i+1] == '0' ) || ( *argv[i+1] == '1' ))) {
                    gDebug_ForceStandaloneTextureToolsForHeighmaps = ( *argv[i+1] == '1' ) ? true : false;
                    arg_offset += 2;
                    i++;
                } else {
                    printf( "Command line syntax error. Should be \"-nvdxt_exe_for_bumpmaps 0\" or \"-nvdxt_exe_for_bumpmaps 1\".\n" );
                    exit(1);
                }
            }
        #endif
        #if GETTEX_ENABLE_COMPARISON_TEST_MODE
            else if (stricmp(argv[i], "-ComparisonTestMode")==0) {
                arg_offset++;
                gDebug_ComparisonTestMode = true;
            }
        #endif
        else if (stricmp(argv[i] + strlen(argv[i]) - strlen(".texture"), ".texture")==0 ||
            stricmp(argv[i] + strlen(argv[i]) - strlen(".tex"), ".tex")==0)    {
            // Hack for allowing double clicking on .texture files
            int ret = printTexInfo(argv[i]);
            if (!no_pause)
                _getch();
            exit(ret);
        }
    }

    // delay setting title until after quicklaunch texinfo would have exited
    setConsoleTitle("GetTex");

    #if GETTEX_ENABLE_COMPARISON_TEST_MODE
        if ( gDebug_ComparisonTestMode ) {
            // test mode manages this flag instead
            gDebug_StandaloneTextureToolMode = false;
        }
    #endif

    if (no_pig) {
        FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);
    } else {
        FolderCacheSetMode(FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC);
    }

    if (!no_prompt)
    {
        const char *helptext = "GetTex can now skip the step of processing existing files, and simply monitor for changes.  If you have made changes to TGA files while GetTex is running, make sure \"Search for Changes\" is checked in the next screen.  You will not see this notice again.";
        if (0==doAskOptions("GetTex", option_list, ARRAY_SIZE(option_list), 5.f, 0, helptext))
            return 0;
    }

    if (scanlog)
    {
        scan_log = fopen("C:\\GetTex_scanlog.txt", "at");
        if (scan_log)
        {
            fprintf(scan_log, "//==============================================================================\n");
            fprintf(scan_log, "%s", timerGetTimeString());
            for (i = 0; i < argc; i++)
            {
                fprintf(scan_log, " %s", argv[i]);
            }
            fprintf(scan_log, "\n");
        }
    }

    printf("Starting up... ");
    loadstart_printf("");
    fileAutoDataDir(false);
    loadend_printf("done.");

    // Detect appropriate NVDXT.EXE
    if (!nvdxt_path[0]) {
        CreateToolUtilPath(nvdxt_path, ARRAY_SIZE(nvdxt_path), "nvdxt.exe");
    }
    if (!fileExists(nvdxt_path)) {
        printf("Warning: unable to find appropriate NVDXT.EXE\n");
        Strcpy(nvdxt_path, "nvdxt.exe"); // Hope it's in the path
    }
    backSlashes(nvdxt_path);
    
    #if USE_CJPEG_EXE
    // Detect appropriate CJPEG.EXE
    if (!cjpeg_path[0]) {
        CreateToolUtilPath(cjpeg_path, ARRAY_SIZE(cjpeg_path), "cjpeg.exe");
    }
    if (!fileExists(cjpeg_path)) {
        printf("ERROR: unable to find appropriate CJPEG.EXE\n");
        exit(1);
    }
    backSlashes(cjpeg_path);
    #endif

    // dxtex path not currently configurable
    CreateToolUtilPath(dxtex_path, ARRAY_SIZE(dxtex_path), "dxtex.exe");

    // Must be after fileAutoDataDir()
    timerRecordStart("c:/game/profiler/GetTex.cohprofiler");
    PERFINFO_AUTO_START("GetTex", 1);

    PERFINFO_AUTO_START("checks", 1);

    if (( arg_offset + 1 ) >= argc ) {
        printf( "ERROR! Missing tga path!\n" );
        exit(1);
    }
    tgaPath = argv[arg_offset + 1];

    checkDataDirOutputDirConsistency();

    //loadstart_printf("Caching directory tree... ");
    //FolderCacheExclude(FOLDER_CACHE_ONLY_FOLDER, "texture_library"); // doesn't work in MODE_DEVELOPMENT, need tricks too!
    //fileDataDir();
    //loadend_printf("done.");

    PERFINFO_AUTO_STOP_START("waitForGettexLock", 1);

    // Lock .texture files (in case the game is auto-reloading)
    waitForGettexLock();

    PERFINFO_AUTO_STOP_START("trickLoad", 1);

    //Get all the trick files from data/tricks
    trickLoad();

    while (false) {
        printf("");
    }

    PERFINFO_AUTO_STOP_START("lodinfoLoad", 1);

    lodinfoLoad();

    PERFINFO_AUTO_STOP_START("texLoadAllInfo", 1);

    //Get the names and properties of the textures in c:\game\data\texture_library
    //Load them into tex_infs
    texLoadAllInfo(0);

    PERFINFO_AUTO_STOP_START("getFileNames", 1);

    loadstart_printf("Scanning for .tga files... ");

    for(i=arg_offset + 1;i<argc;i++)
        getFileNames(argv[i]);
    loadend_printf("done.");
    
    PERFINFO_AUTO_STOP_START("initDuplicateCheck", 1);
    //if (do_scan)
        loadstart_printf("Processing files that have changed...\n");

    // Sort alphabetically
    eaQSort(tex_defs, cmpTexDef);

    initDuplicateCheck();

    /*
    So now you have 
    tex_infs        a sorted list of all the textures already in c:\game\data\texture_library and their properties
    tex_defs        a list textures in the given directory
    */
    //if (do_scan)
    {
        PERFINFO_AUTO_STOP_START("texWrite", 1);
        texWrite();
        printf("%d textures reprocessed", num_reprocess);
        if (num_repack!=num_reprocess) {
            printf(", %d repacked", num_repack);
        }
        loadend_printf("");
    }

    PERFINFO_AUTO_STOP_START("checkDups", 1);

    /*Final error check*/
    //qsort(tex_defs,all_tex_count,sizeof(TexDef),(int (*) (const void *, const void *)) cmpTexDef);
#if 0 // fpe disabled, this is useless when we always have hundreds of dupes!
    if (checkDups())
    {
        printf("There were duplicate source textures. Hit return to continue.\n");
        if (!no_pause) {
            char buf[1000];
            gets(buf);
        }
    }
#endif

    PERFINFO_AUTO_STOP_START("_flushall", 1);

    _flushall();

    if (scan_log)
    {
        fprintf(scan_log, "%d textures reprocessed", num_reprocess);
        if (num_repack!=num_reprocess) {
            fprintf(scan_log, ", %d repacked", num_repack);
        }
        fprintf(scan_log, " (out of %d total textures)", num_not_changed + num_reprocess);
        fprintf(scan_log, "\n%s Done\n\n", timerGetTimeString());
        fclose(scan_log);
        scan_log = NULL;
    }

    Sleep(250); // Sleep to annoy artists, err, I mean, to wait for the files to be written in case
    // the game wants to read then right away, this seems to help a few odd cases, and a quarter of a second
    // never hurt anyone

    releaseGettexLock();

    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
    timerRecordEnd();

    if (monitor) {
        getTexMonitor(argv[arg_offset + 1]);
    }

    EXCEPTION_HANDLER_END
}
