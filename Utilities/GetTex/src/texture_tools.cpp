/****************************************************************************
    texture_tools.cpp
    
    Execute texture processing requests via the NVIDIA texture tools
    library.
    
    Helpful links:
        http://developer.nvidia.com/object/nv_texture_tools.html
        http://code.google.com/p/nvidia-texture-tools/wiki/ApiDocumentation

  St.Lezin, 11/2009
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <utilitieslib/stdtypes.h>
#include <windows.h>
#include <utilitieslib/utils/mathutil.h>
#include "nvtt/nvtt.h"

#ifdef __cplusplus
    extern "C" {
#endif

#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/tga.h>
#include <utilitieslib/utils/dds.h>
#include "texture_tools.h"

#define GETTEX_ENABLE_CUDA_ACCELERATION 0

using namespace nvtt;

//---------------------------------------------------------------------------
//    tClientOptions
//
//    This struct is used to collect user options in one place and then
//    translate them to NVIDIA texture tool options.
//---------------------------------------------------------------------------
struct tClientOptions {

    enum tNormalMapSample {
        kNormalMapSample_n4,
        kNormalMapSample_n3x3,
        kNormalMapSample_n5x5,
        kNormalMapSample_dudv
    };
    
    enum tHeightMapDataLoc {
        kHeightMapDataLoc_none,
        //------------------------
        kHeightMapDataLoc_alpha,
        kHeightMapDataLoc_rgb
    };

    char                    input_file_path[_MAX_PATH];
    char                    output_file_path[_MAX_PATH];

    TextureType                textureType;
    int                        width;
    int                        height;
    int                        depth;
    bool                    mipmaps;
    int                        max_mipmap_level;
    MipmapFilter            mipmap_filter;
    WrapMode                wrap_mode;
    AlphaMode                alpha_mode;
    unsigned int            tga_read_flags;    // OR'd tTgaReadOpts

    // Processing normal maps
    bool                    is_normal_map;
    bool                    is_dds_file;
    bool                    normalize_mipmaps;
    
    // Options to generate normal maps
    bool                    generate_normal_map;
    bool                    swizzle_for_dxt5nm;    // prepare the file for DXT5nm compression
    bool                    has_specular_in_alpha;
    tNormalMapSample        normal_map_sample_type;
    tHeightMapDataLoc        height_map_data_loc;
        
    RoundMode                power_of_two_round_mode;
    Format                    texture_format;

    void resetOptions( void )
    {
        input_file_path[0]            = '\0';
        output_file_path[0]            = '\0';
        textureType                 = TextureType_2D;
        width                        = 0;
        height                        = 0;
        depth                        = 1;
        mipmaps                        = true;
        max_mipmap_level            = -1;    // -1 means 'no limit'
        mipmap_filter                = MipmapFilter_Box;
        wrap_mode                    = WrapMode_Mirror;
        alpha_mode                    = AlphaMode_Transparency;
        tga_read_flags                = 0;
        is_normal_map                = false;
        is_dds_file                    = false;
        normalize_mipmaps            = false;
        generate_normal_map            = false;
        swizzle_for_dxt5nm            = false;
        has_specular_in_alpha        = false;
        normal_map_sample_type        = kNormalMapSample_n4;
        height_map_data_loc        = kHeightMapDataLoc_none;
        power_of_two_round_mode        = RoundMode_None;
        texture_format                = Format_BC1;
    }

    tClientOptions() { resetOptions(); }
};

static tClientOptions g_ClientOptions[2];
static int g_ClientOptionPasses = 0;

class tGetTextErrorHandler : public ErrorHandler
{
public:
    virtual void error(Error e);
};


static bool        handleArg( tClientOptions* pOpts, tGetTexOpt optType, const char* argData );
static bool        processClientRequest( tClientOptions* opts );
static char*    allocTgaImageData( tClientOptions* opts, int* pWidth, int* pHeight, unsigned int nTgaReadOptsFlags );
static char*    allocDdsImageData( tClientOptions* opts, int* pWidth, int* pHeight );
static void        releaseImageData( char* data );  
static void        swizzleDdsTextureForDxt5nmWithSpec( U8* data, tDDS_ImageInfo* pImageInfo, tDDS_MipMapInfo mipMapsInfo[], DWORD mipMapCount );
static void        swizzleRgbaTextureDataForDxtnmWithSpecular( U8* data, 
                        DWORD imageWidth, DWORD imageHeight, DWORD bytesPerPixel,                
                        DWORD rOfs,        // Offset of red element within the pixel
                        DWORD gOfs,     // Offset of green element within the pixel
                        DWORD bOfs,     // Offset of blue element within the pixel
                        DWORD aOfs );    // Offset of alpha element within the pixel
static void        setLastErrorString( const char* szStr=NULL );
static char*&    getLastErrorString( void );
static void        addSecondPass( void );
static void        replaceExtension( char* filePathBuffer, size_t filePathBufferSz, const char* szExt );
static bool        setupAdditionalPasses( void );
static bool        setupAdditionalPass_HeightMaps( void );

    // Call this to prepare for a new operation
void textureTools_Start( void )
{
    for ( int i=0; i < ARRAY_SIZE(g_ClientOptions); i++ )
    {
        g_ClientOptions[i].resetOptions();
    }
    setLastErrorString(NULL);
    g_ClientOptionPasses = 1;
}

static void addSecondPass( void )
{
    // We only provide for, at most, two passes in this version.
    if ( g_ClientOptionPasses == 1 )
    {
        // Create the new "pass" by cloning the initial pass's options.
        memcpy( &g_ClientOptions[g_ClientOptionPasses], 
                &g_ClientOptions[g_ClientOptionPasses-1],
                sizeof(g_ClientOptions[1]) );
        g_ClientOptionPasses = 2;
    }
}

static void replaceExtension( char* filePathBuffer, size_t filePathBufferSz, const char* szExt )
{
    char* p = strrchr( filePathBuffer, '.' );
    if ( p == NULL )
    {
        p = ( filePathBuffer + strlen(filePathBuffer) );
    }
    if (( p + strlen(szExt)) < (filePathBuffer + filePathBufferSz))
    {
        strcpy_s( p, (strlen(szExt)+1), szExt);
    }
    else
    {
        // dude, like yer buffer's too schmall...
        assert(0);
    }
}

    // Call this to configure the processor from client code
bool textureTools_AddOption( tGetTexOpt optType, const char* argData )
{
    return handleArg( &g_ClientOptions[0], optType, argData );
}

static bool handleArg( tClientOptions* pOpts, tGetTexOpt optType, const char* argData )
{
    switch ( optType )
    {
        case kGetTexOpt_SrcFile :
            if ( argData != NULL )
            {
                strcpy_s( pOpts->input_file_path, ARRAY_SIZE(pOpts->input_file_path), argData );
            }
            else
            {
                setLastErrorString( "Bad argument for the -file option." );
            }        
            break;
        case kGetTexOpt_OutputDir :
            // standlone tool mode uses this; we don't. ignore.
            break;
        case kGetTexOpt_OutputFile :
            if ( argData != NULL )
            {
                strcpy_s( pOpts->output_file_path, ARRAY_SIZE(pOpts->output_file_path), argData );
            }
            else
            {
                setLastErrorString( "Bad argument for the -outdir option." );
            }        
            break;
        case kGetTexOpt_NoMipmap :
            pOpts->mipmaps = false;
            pOpts->max_mipmap_level = 0;
            break;
        case kGetTexOpt_Cubic :
            // TODO: What does "-cubic" mean to the current NVIDIA library?
            // The motivation when the old standalone tool was executed from
            // within gettex.exe seems to have been clamping. So for the time
            // being we'll set a clamping mode.
            pOpts->wrap_mode = WrapMode_Clamp;
            break;
        case kGetTexOpt_Kaiser :
            pOpts->mipmap_filter = MipmapFilter_Kaiser;        
            break;
        case kGetTexOpt_DXT5 :
            pOpts->texture_format = Format_DXT5;        
            break;
        case kGetTexOpt_DXT5nm :
            pOpts->swizzle_for_dxt5nm = true;
            pOpts->texture_format = Format_DXT5;
            break;
        case kGetTexOpt_DXT1 :
            pOpts->texture_format = Format_DXT1;        
            break;
        case kGetTexOpt_U8888 :
            pOpts->texture_format = Format_RGBA;
            break;
        case kGetTexOpt_U888 :
            pOpts->texture_format = Format_RGB;
            break;
        case kGetTexOpt_U1555 :
            pOpts->texture_format = Format_RGBA;
            break;
        case kGetTexOpt_U4444 :
            pOpts->texture_format = Format_RGBA;
            break;
        case kGetTexOpt_U565 :
            pOpts->texture_format = Format_RGB;
            break;
        case kGetTexOpt_MakeN4 :
            pOpts->generate_normal_map = true;
            pOpts->normal_map_sample_type = tClientOptions::kNormalMapSample_n4;
            break;
        case kGetTexOpt_HeightRgb :
            pOpts->generate_normal_map = true;
            pOpts->normalize_mipmaps = true;
            pOpts->height_map_data_loc = tClientOptions::kHeightMapDataLoc_rgb;
            break;
        case kGetTexOpt_SrcIsNormalMap :
            pOpts->is_normal_map = true;
            break;
        case kGetTexOpt_NormalizeMipMap :
            pOpts->normalize_mipmaps = true;
            break;
        case kGetTexOpt_ToolPath :
            // not relevant to this module
            break;
        default :
        {
            char msg[256];
            sprintf_s( msg, ARRAY_SIZE(msg), "Unhandled option: %d.", optType );
            setLastErrorString( msg );
            break;
        }
    }

    return ( getLastErrorString() == NULL );
}

static bool setupAdditionalPasses( void )
{
    bool bOK = true;
    if (( g_ClientOptions[0].swizzle_for_dxt5nm ) &&
        ( g_ClientOptions[0].height_map_data_loc != tClientOptions::kHeightMapDataLoc_none ))
    {
        // The TGA is a height map. Make a second pass to perform the processing
        bOK = setupAdditionalPass_HeightMaps();
    }
    return bOK;
}

static bool setupAdditionalPass_HeightMaps( void )
{
    // Make a second pass by copying g_ClientOptions[0].
    addSecondPass();

    //
    //    First pass will generate a temp DDS file...
    //
    
    // Modify the outfile file of the first pass. It's now a temp file.
    replaceExtension( g_ClientOptions[0].output_file_path, ARRAY_SIZE(g_ClientOptions[0].output_file_path), ".TMP.dds" );
    
    // The temp file is now input to the second pass.
    strcpy_s( g_ClientOptions[1].input_file_path, 
                ARRAY_SIZE(g_ClientOptions[1].input_file_path), 
                g_ClientOptions[0].output_file_path );
    
    // The intermediate file is RGBA so that we can conveniently mess with it.
    g_ClientOptions[0].texture_format = Format_RGBA;
    g_ClientOptions[0].swizzle_for_dxt5nm = false;

    //
    //    Second pass reads the DDS file, swizzles the data and
    //    writes a new DDS file.
    //    
    g_ClientOptions[1].is_dds_file = true;
    g_ClientOptions[1].swizzle_for_dxt5nm = true;
    g_ClientOptions[1].height_map_data_loc = tClientOptions::kHeightMapDataLoc_none;
    g_ClientOptions[1].generate_normal_map = false;
    g_ClientOptions[1].is_normal_map = true;
    
    return true;
}

    // Perform texture processing
bool textureTools_Finish( void )
{
    bool bSuccess = true;

    bSuccess = setupAdditionalPasses();

    for ( int passI=0; passI < g_ClientOptionPasses; passI++ )
    {
        if ( ! ( bSuccess = processClientRequest( &g_ClientOptions[passI] )) )
        {
            break;
        }
    }
    return bSuccess;
}

    // Returns NULL if no error was detected; otherwise, an error string.
const char* textureTools_GetLastError( void )
{
    return getLastErrorString();
}

char* allocTgaImageData( tClientOptions* opts, int* pWidth, int* pHeight, unsigned int nTgaReadOptsFlags )
{
    char* data = NULL;
    FILE* fIN = (FILE*)fopen( opts->input_file_path, "rb" );
    if ( fIN )
    {
        data = tgaLoadEx( fIN, pWidth, pHeight, nTgaReadOptsFlags);
        if (( data != NULL ) && ( opts->swizzle_for_dxt5nm ))
        {
            swizzleRgbaTextureDataForDxtnmWithSpecular( (U8*)data, 
                *pWidth, *pHeight, 4,                
                /* R */ 2,
                /* G */ 1,
                /* B */ 0,
                /* A */ 3 );
        }
        fclose( fIN );
    }
    return data;
}

char* allocDdsImageData( tClientOptions* opts, int* pWidth, int* pHeight )
{
    tDDS_ImageInfo imageInfo = { 0 };
    tDDS_MipMapInfo mipMaps[1];
    char* data = NULL;

    if ( ddsGetImageInfoFromFile( opts->input_file_path, &imageInfo, mipMaps, ARRAY_SIZE(mipMaps) ) >= 1 )
    {
        size_t imageBuffSz = ( mipMaps[0].mipMapWidth * mipMaps[0].mipMapHeight * imageInfo.bytesPerPixel );
        data = (char*)malloc( imageBuffSz );
        FILE* fIN = (FILE*)fopen( opts->input_file_path, "rb" );
        if ( fIN )
        {
            fseek( fIN, imageInfo.offsetToImage0, SEEK_SET );
            fread( data, 1, imageBuffSz, fIN );
            fclose( fIN );
            
            if ( opts->swizzle_for_dxt5nm )
            {
                swizzleDdsTextureForDxt5nmWithSpec( (U8*)data, &imageInfo, mipMaps, 1 ); // we only swizzle mipmap 0
            }
            *pWidth = mipMaps[0].mipMapWidth;
            *pHeight = mipMaps[0].mipMapHeight;
        }    
    }
    return data;
}

static void releaseImageData( char* data )
{
    free( data );
}

static void swizzleDdsTextureForDxt5nmWithSpec( U8* data, tDDS_ImageInfo* pImageInfo, tDDS_MipMapInfo mipMapsInfo[], DWORD mipMapCount )
{
    if ( pImageInfo->compressionTypeFOURCC == 'RGBA' )
    {
        //
        // We know how to swizzle uncompressed data
        //
        
        //
        // First, find out how the color values are stored (RGBA, ARGB, ... ), 
        // rather than assuming.
        //
        // From MSDN:
        //    "For instance, given the A8R8G8B8 format, the red mask would be 0x00ff0000."
        //
        DWORD rOfs, gOfs, bOfs, aOfs;
        {
            DWORD nMask = 0x000000FF;
            DWORD byteOfs;
            for ( byteOfs=0; byteOfs < 4; byteOfs++ )
            {
                if (( pImageInfo->redBitsMask   & nMask ) != 0 ) rOfs = byteOfs;
                if (( pImageInfo->greenBitsMask & nMask ) != 0 ) gOfs = byteOfs;
                if (( pImageInfo->blueBitsMask  & nMask ) != 0 ) bOfs = byteOfs;
                if (( pImageInfo->alphaBitsMask & nMask ) != 0 ) aOfs = byteOfs;
                nMask <<= 8;
            }
        }
    
        //
        //    Do the work
        //
        for ( DWORD i=0; i < mipMapCount; i++ )
        {
            U8* pPixel = ( data + mipMapsInfo[i].imageDataOfs );
            swizzleRgbaTextureDataForDxtnmWithSpecular( data, 
                mipMapsInfo[i].mipMapWidth, mipMapsInfo[i].mipMapHeight, pImageInfo->bytesPerPixel,                
                rOfs, gOfs, bOfs, aOfs );
        }        
    }
}

static inline void colorToVec3( U8 r, U8 g, U8 b, Vec3 outVector )
{
    // Range expand
    outVector[0] = (( ( (float)r / 255.0f ) * 2.0f ) - 1.0f );
    outVector[1] = (( ( (float)g / 255.0f ) * 2.0f ) - 1.0f );
    outVector[2] = (( ( (float)b / 255.0f ) * 2.0f ) - 1.0f );    
}

static inline void vec3ToColor( const Vec3 vecVal, U8& r, U8& g, U8& b )
{
    // Range compress
    r = (char)( 255.0f * (( vecVal[0] + 1.0f ) / 2.0f ));
    g = (char)( 255.0f * (( vecVal[1] + 1.0f ) / 2.0f ));
    b = (char)( 255.0f * (( vecVal[2] + 1.0f ) / 2.0f ));
}

static void swizzleRgbaTextureDataForDxtnmWithSpecular( U8* data,
                DWORD imageWidth, DWORD imageHeight, DWORD bytesPerPixel /* must be 4! */,                
                DWORD rOfs,        // Offset of red element within the pixel
                DWORD gOfs,     // Offset of green element within the pixel
                DWORD bOfs,     // Offset of blue element within the pixel
                DWORD aOfs )    // Offset of alpha element within the pixel
{
    U8* pPixel = data;
    DWORD row;
    for ( row=0; row < imageHeight; row++ )
    {
        DWORD col;
        for ( col = 0; col < imageWidth; col++ )
        {
            U8 rgba[4];
            //
            //    BEFORE                     AFTER
            //    -------------------------------------------
            //    R = normal.x -------+      R = constant 0
            //    G = normal.y        |      G = normal.y
            //    B = normal.z    +---|----> B = specular
            //    A = specular ---+   +----> A = normal.x
            //

            // Normalize the vector obtained from the source normal map.
            // It should already be normalized, but let's do it again, just in case,
            // since the shader relies on x and y having come from a normalized vector
            // when it re-computes the z which we're about to throw away.
            {
                Vec3 normal_v;
                colorToVec3( pPixel[rOfs], pPixel[gOfs], pPixel[bOfs], normal_v );
                normalVec3(normal_v);
                vec3ToColor( normal_v, pPixel[rOfs], pPixel[gOfs], pPixel[bOfs] );
            }
            
            rgba[rOfs]    = 0;
            rgba[gOfs]    = pPixel[gOfs];
            rgba[bOfs]    = pPixel[aOfs];
            rgba[aOfs]    = pPixel[rOfs];
            
            pPixel[0]    = rgba[0];
            pPixel[1]    = rgba[1];
            pPixel[2]    = rgba[2];
            pPixel[3]    = rgba[3];
                                                                    
            pPixel += bytesPerPixel;
        }
    }
}

static bool processClientRequest( tClientOptions* opts )
{
    static const unsigned int kStandardTgaFlags = ( kTgaReadOpt_DontSwapToRGBA | tTgaReadOpt_AlphaToZeroOnEmptyImage );

    InputOptions            inputOpts;
    OutputOptions            outputOpts;
    CompressionOptions        compressorOpts;
    Compressor                compressor;
    tGetTextErrorHandler    errorHandler;
    bool                    rslt;
    char*                    fileData = NULL;
    unsigned int            tgaReadOpts = ( opts->tga_read_flags | kStandardTgaFlags );
    
    inputOpts.reset();
    outputOpts.reset();
    compressorOpts.reset();
    setLastErrorString(NULL);
        
    if ( opts->is_dds_file )
    {
        fileData = allocDdsImageData( opts, &opts->width, &opts->height );
    }
    else
    {
        fileData = allocTgaImageData( opts, &opts->width, &opts->height, tgaReadOpts );
    }
    if ( fileData == NULL )
    {
        return false;
    }
    inputOpts.setFormat(InputFormat_BGRA_8UB);
    inputOpts.setTextureLayout(opts->textureType, opts->width, opts->height, opts->depth);
    inputOpts.setMipmapData(fileData, opts->width, opts->height);
    //inputOpts.setAlphaMode( opts->alpha_mode );
    
    //
    //    NOTE: The API allows more control over mipmap generation if needed.
    //    See InputOptions::setMipmapData().
    //        http://code.google.com/p/nvidia-texture-tools/wiki/ApiDocumentation
    inputOpts.setMipmapGeneration(opts->mipmaps, opts->max_mipmap_level);
    if ( opts->mipmaps )
    {
        inputOpts.setMipmapFilter(opts->mipmap_filter);
    }
    inputOpts.setWrapMode(opts->wrap_mode);
    inputOpts.setNormalMap(opts->is_normal_map);
    inputOpts.setNormalizeMipmaps( opts->normalize_mipmaps );
    inputOpts.setRoundMode(opts->power_of_two_round_mode);
    
    inputOpts.setGamma(1.0f, 1.0f);
    
    if ( opts->generate_normal_map )
    {
        inputOpts.setConvertToNormalMap(true);
        switch ( opts->normal_map_sample_type )
        {
            // TODO: How do we specify these legacy preferences?
            case tClientOptions::kNormalMapSample_n4 :
            case tClientOptions::kNormalMapSample_n3x3 :
            case tClientOptions::kNormalMapSample_n5x5 :
            case tClientOptions::kNormalMapSample_dudv :
                // ???
                break;
            default :
                break;
        }
        switch ( opts->height_map_data_loc )
        {
            case tClientOptions::kHeightMapDataLoc_alpha :
                inputOpts.setHeightEvaluation(0, 0, 0, 1);
                break;
            case tClientOptions::kHeightMapDataLoc_rgb :
                //inputOpts.setHeightEvaluation(1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f, 0.0f);
                //    TODO: This seems to, at least partially, fix the large
                //    discrepency between the way the normal maps look with
                // the old tools and the way they look with the new tool.
                //inputOpts.setHeightEvaluation(-0.02, -0.02, -0.02, 0.0f);
                inputOpts.setHeightEvaluation(-0.2f, 0.0f, 0.0f, 0.0f);
                break;
            default :
                break;
        }
    }
    else if ( opts->is_normal_map )
    {
        inputOpts.setConvertToNormalMap(false);
        inputOpts.setNormalizeMipmaps(true);
    }
    else
    {
        inputOpts.setConvertToNormalMap(false);
        inputOpts.setNormalizeMipmaps(false);
    }
    
    compressorOpts.setFormat(opts->texture_format);
//    compressorOpts.setQuality(Quality_Highest);        // doesn't seem to do anything

    outputOpts.setFileName(opts->output_file_path);
    outputOpts.setErrorHandler( &errorHandler );

    #if ! GETTEX_ENABLE_CUDA_ACCELERATION
        compressor.enableCudaAcceleration(false);
    #endif
    rslt = compressor.process( inputOpts, compressorOpts, outputOpts );
    
    releaseImageData(fileData);
        
    return (( rslt ) && ( getLastErrorString() == NULL ));
}

char*& getLastErrorString( void )
{
    static char* g_last_error = NULL;
    return g_last_error;
}

static void setLastErrorString( const char* szStr )
{
    delete [] getLastErrorString();
    getLastErrorString() = NULL;
    if ( szStr != NULL )
    {
        getLastErrorString() = new char[strlen(szStr)+1];
        strcpy( getLastErrorString(), szStr );
    }
}

void tGetTextErrorHandler::error(Error e)
{
    setLastErrorString(errorString(e));
    printf( "\n\nERROR processing texture: \"%s\"\n", getLastErrorString() );
}

#ifdef __cplusplus
    }
#endif
