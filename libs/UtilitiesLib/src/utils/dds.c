//
//    dds.c
//
//    Reads DDS file data
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <share.h>

#ifdef _WIN32
    #include "utilitieslib/utils/wininclude.h"
    #include <io.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <time.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #include <dirent.h>
#endif

#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/endian.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/dds.h"

#define MATCH_FOURCC(_dword_,_fourcc_code_)                ( endianSwapU32(_dword_) == _fourcc_code_)
#define MATCH_FOURCC_PTR(_dword_ptr_,_fourcc_code_)        MATCH_FOURCC(*((DWORD*)(_dword_ptr_)),_fourcc_code_)

#if 0

    #include "../../../../3rdparty/DirectX/Include/ddraw.h"
    #include "../../../../3rdparty/DirectX/Include/d3d10shader.h"

#else

    // http://msdn.microsoft.com/en-us/library/ee417788%28VS.85%29.aspx
    typedef struct  {
        UINT dwSize;
        UINT dwFlags;
        UINT dwFourCC;
        UINT dwRGBBitCount;
        UINT dwRBitMask;
        UINT dwGBitMask;
        UINT dwBBitMask;
        UINT dwABitMask;
    } DDS_PIXELFORMAT;

    //    http://msdn.microsoft.com/en-us/library/ee417785%28VS.85%29.aspx
    typedef struct  {
        DWORD dwSize;
        DWORD dwFlags;
        DWORD dwHeight;
        DWORD dwWidth;
        DWORD dwLinearSize;
        DWORD dwDepth;
        DWORD dwMipMapCount;
        DWORD dwReserved1[11];
        DDS_PIXELFORMAT ddpf;
        DWORD dwCaps;
        DWORD dwCaps2;
        DWORD dwCaps3;
        DWORD dwCaps4;
        DWORD dwReserved2;
    } DDS_HEADER;

    // http://msdn.microsoft.com/en-us/library/ee418116%28VS.85%29.aspx
    typedef enum DXGI_FORMAT
    {
        DXGI_FORMAT_UNKNOWN = 0,
        DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
        DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
        DXGI_FORMAT_R32G32B32A32_UINT = 3,
        DXGI_FORMAT_R32G32B32A32_SINT = 4,
        DXGI_FORMAT_R32G32B32_TYPELESS = 5,
        DXGI_FORMAT_R32G32B32_FLOAT = 6,
        DXGI_FORMAT_R32G32B32_UINT = 7,
        DXGI_FORMAT_R32G32B32_SINT = 8,
        DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
        DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
        DXGI_FORMAT_R16G16B16A16_UNORM = 11,
        DXGI_FORMAT_R16G16B16A16_UINT = 12,
        DXGI_FORMAT_R16G16B16A16_SNORM = 13,
        DXGI_FORMAT_R16G16B16A16_SINT = 14,
        DXGI_FORMAT_R32G32_TYPELESS = 15,
        DXGI_FORMAT_R32G32_FLOAT = 16,
        DXGI_FORMAT_R32G32_UINT = 17,
        DXGI_FORMAT_R32G32_SINT = 18,
        DXGI_FORMAT_R32G8X24_TYPELESS = 19,
        DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
        DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
        DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
        DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
        DXGI_FORMAT_R10G10B10A2_UNORM = 24,
        DXGI_FORMAT_R10G10B10A2_UINT = 25,
        DXGI_FORMAT_R11G11B10_FLOAT = 26,
        DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
        DXGI_FORMAT_R8G8B8A8_UNORM = 28,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
        DXGI_FORMAT_R8G8B8A8_UINT = 30,
        DXGI_FORMAT_R8G8B8A8_SNORM = 31,
        DXGI_FORMAT_R8G8B8A8_SINT = 32,
        DXGI_FORMAT_R16G16_TYPELESS = 33,
        DXGI_FORMAT_R16G16_FLOAT = 34,
        DXGI_FORMAT_R16G16_UNORM = 35,
        DXGI_FORMAT_R16G16_UINT = 36,
        DXGI_FORMAT_R16G16_SNORM = 37,
        DXGI_FORMAT_R16G16_SINT = 38,
        DXGI_FORMAT_R32_TYPELESS = 39,
        DXGI_FORMAT_D32_FLOAT = 40,
        DXGI_FORMAT_R32_FLOAT = 41,
        DXGI_FORMAT_R32_UINT = 42,
        DXGI_FORMAT_R32_SINT = 43,
        DXGI_FORMAT_R24G8_TYPELESS = 44,
        DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
        DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
        DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
        DXGI_FORMAT_R8G8_TYPELESS = 48,
        DXGI_FORMAT_R8G8_UNORM = 49,
        DXGI_FORMAT_R8G8_UINT = 50,
        DXGI_FORMAT_R8G8_SNORM = 51,
        DXGI_FORMAT_R8G8_SINT = 52,
        DXGI_FORMAT_R16_TYPELESS = 53,
        DXGI_FORMAT_R16_FLOAT = 54,
        DXGI_FORMAT_D16_UNORM = 55,
        DXGI_FORMAT_R16_UNORM = 56,
        DXGI_FORMAT_R16_UINT = 57,
        DXGI_FORMAT_R16_SNORM = 58,
        DXGI_FORMAT_R16_SINT = 59,
        DXGI_FORMAT_R8_TYPELESS = 60,
        DXGI_FORMAT_R8_UNORM = 61,
        DXGI_FORMAT_R8_UINT = 62,
        DXGI_FORMAT_R8_SNORM = 63,
        DXGI_FORMAT_R8_SINT = 64,
        DXGI_FORMAT_A8_UNORM = 65,
        DXGI_FORMAT_R1_UNORM = 66,
        DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
        DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
        DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
        DXGI_FORMAT_BC1_TYPELESS = 70,
        DXGI_FORMAT_BC1_UNORM = 71,
        DXGI_FORMAT_BC1_UNORM_SRGB = 72,
        DXGI_FORMAT_BC2_TYPELESS = 73,
        DXGI_FORMAT_BC2_UNORM = 74,
        DXGI_FORMAT_BC2_UNORM_SRGB = 75,
        DXGI_FORMAT_BC3_TYPELESS = 76,
        DXGI_FORMAT_BC3_UNORM = 77,
        DXGI_FORMAT_BC3_UNORM_SRGB = 78,
        DXGI_FORMAT_BC4_TYPELESS = 79,
        DXGI_FORMAT_BC4_UNORM = 80,
        DXGI_FORMAT_BC4_SNORM = 81,
        DXGI_FORMAT_BC5_TYPELESS = 82,
        DXGI_FORMAT_BC5_UNORM = 83,
        DXGI_FORMAT_BC5_SNORM = 84,
        DXGI_FORMAT_B5G6R5_UNORM = 85,
        DXGI_FORMAT_B5G5R5A1_UNORM = 86,
        DXGI_FORMAT_B8G8R8A8_UNORM = 87,
        DXGI_FORMAT_B8G8R8X8_UNORM = 88,
        DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
        DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
        DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
        DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
        DXGI_FORMAT_BC6H_TYPELESS = 94,
        DXGI_FORMAT_BC6H_UF16 = 95,
        DXGI_FORMAT_BC6H_SF16 = 96,
        DXGI_FORMAT_BC7_TYPELESS = 97,
        DXGI_FORMAT_BC7_UNORM = 98,
        DXGI_FORMAT_BC7_UNORM_SRGB = 99,
        DXGI_FORMAT_FORCE_UINT = 0xffffffffUL,
    } DXGI_FORMAT, *LPDXGI_FORMAT;

    // http://msdn.microsoft.com/en-us/library/ee415880%28VS.85%29.aspx
    typedef enum D3D10_RESOURCE_DIMENSION
    {
        D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
        D3D10_RESOURCE_DIMENSION_BUFFER = 1,
        D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
        D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
        D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4,
    } D3D10_RESOURCE_DIMENSION;

    // http://msdn.microsoft.com/en-us/library/ee417787%28VS.85%29.aspx
    typedef struct  {
        DXGI_FORMAT dxgiFormat;
        D3D10_RESOURCE_DIMENSION resourceDimension;
        UINT miscFlag;
        UINT arraySize;
        UINT reserved;
    } DDS_HEADER_DXT10;

    #define DDPF_FOURCC    0x04
    #define DDPF_RGB    0x40

#endif

DWORD ddsGetImageInfo( 
            /*IN*/ char* fileData, 
            /*OUT*/ tDDS_ImageInfo* pImageInfo, 
            /*OUT*/ tDDS_MipMapInfo mipMaps[], 
            DWORD mipMapsArrSz )
{
    DWORD nDataOfs = 0;
    DDS_HEADER* pDDSHeader;
    DWORD numMipMapsFound = 0;
    
    memset( pImageInfo, 0, sizeof(tDDS_ImageInfo) );
    
    if ( ! MATCH_FOURCC_PTR((fileData+nDataOfs),'DDS ') ) // 'DDS ' -> 0x20534444
    {
        return 0;
    }
    nDataOfs += sizeof(DWORD);
    
    pDDSHeader = (DDS_HEADER*)(fileData+nDataOfs);
    if (( pDDSHeader->dwSize != sizeof(DDS_HEADER) ) ||
        ( pDDSHeader->ddpf.dwSize != sizeof(DDS_PIXELFORMAT) ))
    {
        // Bad data.
        return 0;
    }
    
    if ( pDDSHeader->ddpf.dwFlags & DDPF_RGB )
    {
        if ( pDDSHeader->ddpf.dwRGBBitCount == 32 )
        {
            pImageInfo->compressionTypeFOURCC = 'RGBA';
        }
        else
        {
            pImageInfo->compressionTypeFOURCC = 'RGB ';
        }

        pImageInfo->redBitsMask        = pDDSHeader->ddpf.dwRBitMask;
        pImageInfo->greenBitsMask    = pDDSHeader->ddpf.dwGBitMask;
        pImageInfo->blueBitsMask    = pDDSHeader->ddpf.dwBBitMask;
        pImageInfo->alphaBitsMask    = pDDSHeader->ddpf.dwABitMask;
    }
    else
    {
        pImageInfo->compressionTypeFOURCC = endianSwapU32( pDDSHeader->ddpf.dwFourCC );
    }
    pImageInfo->bytesPerPixel = (( pDDSHeader->ddpf.dwRGBBitCount + 7 ) / 8 );
    
    nDataOfs += sizeof(DDS_HEADER);
    
    if (( pDDSHeader->ddpf.dwFlags & DDPF_FOURCC ) &&
        ( MATCH_FOURCC(pDDSHeader->ddpf.dwFourCC, 'DX10' )))
    {
        // Skip the additional DX10 header.
        nDataOfs += sizeof(DDS_HEADER_DXT10);
    }
    
    pImageInfo->offsetToImage0 = nDataOfs;
    
    {
        DWORD height = pDDSHeader->dwHeight;
        DWORD width = pDDSHeader->dwWidth;
        DWORD mipMapOffset = 0;
        DWORD size;
        DWORD i;
        
        for (i = 0; 
                (( i < pDDSHeader->dwMipMapCount ) &&
                 ( i < mipMapsArrSz ) &&
                 ( width || height)); ++i)
        {
            if (width == 0)
            {
                width = 1;
            }
            if (height == 0)
            {
                height = 1;
            }
            mipMaps[i].imageDataOfs = mipMapOffset;
            mipMaps[i].mipMapWidth    = width;
            mipMaps[i].mipMapHeight = height;
            numMipMapsFound++;

            size = ( mipMaps[i].mipMapWidth * mipMaps[i].mipMapHeight * pImageInfo->bytesPerPixel );

            mipMapOffset += size;
            width >>= 1;
            height >>= 1;
        }
    }
    
    return numMipMapsFound;
}

DWORD ddsGetImageInfoFromFile( 
            const char* szDdsFilePath, 
            /*OUT*/ tDDS_ImageInfo* pImageInfo, 
            /*OUT*/ tDDS_MipMapInfo mipMaps[], 
            DWORD mipMapsArrSz )
{
    DWORD mipMapsFound = 0;
    size_t buffSz = ( sizeof(DWORD) + sizeof(DDS_HEADER) + 32 );    // plus a few bytes, just in case...
    char* buffer = (char*)malloc( buffSz );
    FILE* fDDS = fopen( szDdsFilePath, "rb" );
    if ( fDDS != NULL )
    {
        fread( buffer, 1, buffSz, fDDS );
        mipMapsFound= ddsGetImageInfo( buffer, pImageInfo, mipMaps, mipMapsArrSz );
        fclose( fDDS );
    }
    free( buffer );
    return mipMapsFound;
}
