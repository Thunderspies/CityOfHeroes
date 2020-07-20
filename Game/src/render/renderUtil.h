#ifndef _RENDERUTIL_H
#define _RENDERUTIL_H

#include <utilitieslib/stdtypes.h>
#include "render/thread/rt_queue.h"
#include "render/thread/rt_init.h"

#define SPEC_BUF_SIZE 4096

typedef struct SystemSpecs
{
    DWORDLONG        maxPhysicalMemory;
    DWORDLONG        availablePhysicalMemory; //At time specs are fetched
    char    videoCardName[256];
    int        videoCardVendorID;
    int        videoCardDeviceID;
    F32        CPUSpeed;
    int        numVirtualCPUs;
    int        numRealCPUs;
    char    cpuIdentifier[256];
    U32        cpuInfo[4];
    time_t    videoDriverDate;

    // OS version:
    int        lowVersion, highVersion, build;
    int        servicePackMajor, servicePackMinor;
    char    bandwidth[256]; // from updater

    U32        videoMemory;
    int        numMonitors;

    char    lastPiggCorruption[64];

    int        isFilledIn;
} SystemSpecs;

extern SystemSpecs    systemSpecs;

typedef struct RdrLightmapState RdrLightmapState;

#if (0 || defined(_FULLDEBUG)) && !defined(TEST_CLIENT)
    #define DOING_CHECKGL
#endif

#ifdef DOING_CHECKGL
    #define CHECKGL do { rdrCheckThread(); checkgl(__FILE__,__LINE__); } while(0)
    #define INEXACT_CHECKGL do { rdrCheckThread(); checkgl("somewhere before " __FILE__,__LINE__); } while(0)
    #define CHECKNOTGLTHREAD do { rdrCheckNotThread(); } while(0)
#else
    #define CHECKGL do {} while(0)
    #define INEXACT_CHECKGL do {} while(0)
    #define CHECKNOTGLTHREAD do {} while(0)
#endif

// List of video card vendor IDs and device IDs available here (might not be definitive):
// http://www.pcidatabase.com/vendors.php?sort=name
#define VENDOR_AMD    0x1002
#define VENDOR_NV     0x10DE
#define VENDOR_INTEL  0x8086
#define VENDOR_S3G     0x5333
#define VENDOR_VIA     0x1106
#define VENDOR_SIS     0x1039

enum {
    SHADERTEST_SPEC_ONLY = 1,
    SHADERTEST_DIFFUSE_ONLY,
    SHADERTEST_NORMALS_ONLY,
    SHADERTEST_TANGENTS_ONLY,
    SHADERTEST_BASEMAT_ONLY,
    SHADERTEST_BLENDMAT_ONLY,
};

void rdrExamineDriversEarly(oivd);
void rdrInit(void);
void getGlState();
void checkgl(char *fname,int line);
void rdrFixMat(Mat4 mat);
void rdrSetupPerspectiveProjection(F32 fovy, F32 aspect, F32 znear, F32 zfar, bool bFlipX, bool bFlipY);
void rdrSetupOrthographicProjection(int width, int height, int ortho_zoom);
void rdrSetupFrustum(F32 l, F32 r, F32 b, F32 t, F32 znear, F32 zfar);
void rdrSetupOrtho(F32 l, F32 r, F32 b, F32 t, F32 znear, F32 zfar);
void rdrSendProjectionMatrix(void);
void rdrPerspective(F32 fovy, F32 aspect, F32 znear, F32 zfar, bool bFlipX, bool bFlipY);
void rdrViewport(int x, int y, int width,int height, bool bNeedScissoring);
void rdrPixBufDumpBmp(const char *filename);
void rdrGetPixelColor(int x, int y, U8 * pixbuf);
void rdrGetPixelDepth(F32 x, F32 y, F32 * depthbuf);
void rdrInitDepthComplexity();
void rdrDoDepthComplexity(F32 fps);
int renderNvparse(char *str);
U8 *rdrPixBufGet(int *width,int *height);
void rdrPrintScreenSystemSpecs( void ); //TO DO move elsewhere
void rdrGetSystemSpecs( SystemSpecs * systemSpecs );
void rdrGetSystemSpecString( SystemSpecs * systemSpecs, char * buf ); // buf size should be at least SPEC_BUF_SIZE
char *rdrGetSystemSpecCSVString( void );
char *rdrChipToString(int chipbit);
char *rdrFeatureToString(int featurebit);
void rdrGetFrameBuffer(F32 x, F32 y, int width, int height, GetFrameBufType type, void *buf);
void rdrPrintSystemSpecs( SystemSpecs * systemSpecs );
char *rdrFeaturesString(bool onlyUsed);
void rdrSetChipOptions(void);
void rdrInitTopOfFrame(void);
void rdrInitTopOfView(const Mat4 viewmat, const Mat4 inv_viewmat);
void rdrInitLightmapRenderState(RdrLightmapState *state);
void reloadShaderCallback(const char *relpath, int when);
void setCgShaderPathCallback( const char* shaderDirectory );
void setCgShaderMode( unsigned int shaderMode );
void setNormalMapTestMode( const char* testMode );
void setUseDXT5nmNormals( unsigned int useDXT5nmNormals );
int getShaderDebugSpecialDefines(const char** defineArray /*OUT*/, int defineArraySz /*IN*/);
int rdrToggleFeatures(int gfx_features_enable, int gfx_features_disable, bool onlyDynamic);
void rdrSetNumberOfTexUnits( int num_units );        // use this to tell the engine how many texture units are available
void rdrClearOldDriverCheck(void);

#endif
