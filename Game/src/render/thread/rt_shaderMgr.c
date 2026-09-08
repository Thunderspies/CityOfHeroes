#define RT_PRIVATE
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include "render/thread/ogl.h"
#include "rt_shaderMgr.h"
#include "render/thread/rt_state.h"
#include "render/thread/rt_cgfx.h"
#include "render/renderUtil.h"
#include "render/thread/wcw_statemgmt.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/mathutil.h>
#include "render/thread/rt_effects.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/crypt.h>
#include "seq/anim.h"
#include "render/thread/rt_shadowmap.h"
#include "shlobj.h"
#include <utilitieslib/utils/osdependent.h>
#include "render/thread/rt_util.h"
#include "render/thread/rt_init.h"

#include <utilitieslib/assert/assert.h>
#include <stdio.h>
#include <io.h>
#include <sys/stat.h>

#define SHADER_SUBDIR            "shaders/"
#define SHADER_SUBDIR_CGFX        SHADER_SUBDIR "cgfx/"

// Enable and disable the use of pre-baked CG shaders stored on disk.
#define LOAD_PREBAKED_CG_SHADERS 1
#if     LOAD_PREBAKED_CG_SHADERS
    #define GET_PREBAKED_SHADER_TEXT( shaderFileFullPath, pBuildDescriptor, pCompiledPgmText )    \
            getPreBakedShaderText( shaderFileFullPath, pBuildDescriptor, pCompiledPgmText )
#else
    #define GET_PREBAKED_SHADER_TEXT( shaderFileFullPath, pBuildDescriptor, pCompiledPgmText )    false
#endif                

#define STORE_PREBAKED_CG_SHADERS 1
#if     STORE_PREBAKED_CG_SHADERS
    #define STORE_PREBAKED_SHADER( pBuildDescriptor, compiledPgmText )    \
            storePreBakedShader( pBuildDescriptor, compiledPgmText )
#else
    #define STORE_PREBAKED_SHADER( pBuildDescriptor, compiledPgmText )
#endif

// Subdirectory of C:\Documents and Settings\<your_name>\Local Settings\Application Data
// where pre-baked shader output is written.
#define LOCAL_USER_DATA_SUBDIR            "\\NCSoft\\CoX"

#define INFO_FILE_CONTENT_MD5_FIELD        "CONTENT="
#define INFO_FILE_CONTENT_MD5_FIELD_LEN    8                // strlen(INFO_FILE_CONTENT_MD5_FIELD)

typedef struct {
    int    shaderMode;                        // DrawModeType for VPs and BlendModeType for FPs
    int    chipFlags;                        // if non-0, flags that need to be set in rdr_caps.chip to use the profile
    GLuint profile;                            // profiles are tCgShaderProfile values for CG
    U32    extraCompileFlags;
    char   *filenameRoot;
    int    required_features;                // shader features
    char   *extraDefineSet;                    // space-separated list of defines for the shader compiler
} tShaderPgmBuildSpec;

typedef struct 
{
    GLuint        programHdl;                    // OpenGL shader "name"
    char**        buildHints;                    // options related to *running* the Cg compiler (e.g., the target profile)
    int            buildHintsNum;                // number of live items in the array
    int            buildHintsArrSz;            // current array size (may include unused elements)
    char**        compilerCmdLineArgs;        // command line args passed to the Cg compiler (NULL terminated)
    int            compilerCmdLineArgsNum;        // number of live items in the array
    int            compilerCmdLineArgsArrSz;    // current array size (may include unused elements)
    
    U32            srcFileMD5[4];                // an MD5 of the source contents.
    
    char        descriptorText[2048];        // plain text formatted output of buildHints and compilerCmdLineArgs
    U32            shaderKey[4];                // an MD5 of descriptorText[]
    
    // For the "binary" files (OK, not really binary, but we pretend they are) the input directory
    // and the output directory can be different. For example, on Vista/Win7 and beyond we may read the binary
    // data from an installation directory (c:\game data\...) but if we have a newer source file and recompile
    // the shader we can't write to that directory
    char        binFileNameIN[2048];        // input file holding the compiled shader built to these specifications
    char        binFileNameOUT[2048];        // output file holding the compiled shader built to these specifications
} tShaderBuildDescriptor;

/* ****
    The master table of fragment programs for materials and
    their variations.

    The table is first indexed by the material and then
    by the option bits as follows:
            [ BlendModeShader enum ] [ BlendModeBits ]
    For example:
            [ BLENDMODE_MULTI ] [ BMB_HIGH_QUALITY|BMB_SHADOWMAP ]
    The MULTI shader has the most variations as it is currently the
    most general material.

    Note that BlendModeBits are generally a combination of user
    feature settings and hw capability (e.g. BMB_SHADOWMAP) or
    particular to a material specialization (e.g.,BMB_BUILDING) or 
    promotion (e.g., BMB_HIGH_QUALITY).

    Not all combinations of blend mode bits result in a different
    material as some of the simpler legacy materials don't support
    some of the more advanced features.
*/
int g_shaderMgrFragmentProgramVariants[BLENDMODE_NUMENTRIES][BMB_VARIANT_COUNT];

int shaderMgrVertexPrograms[DRAWMODE_NUMENTRIES];
int shaderMgrVertexProgramsHQ[DRAWMODE_NUMENTRIES];
static char shaderMgrOverrideShaderDir[64] = { 0 };

typedef enum tShaderPathRootType
{
    kPathRoot_GameData,                // A relative path used with the pigg files or loose files in c:\game\data
    kPathRoot_Custom,                // A path specified by some client code (e.g., from a command line param).
    kPathRoot_GlobalAppDataPath,    // A path rooted in the Windows user data path ("c:\documents and settings\blah...").
    //------
    kShaderPathRoot_COUNT,
    kShaderPathRoot_NONE
} tShaderPathRootType;

typedef enum tShaderOptimizationLevel
{
    // -On option to Cg compiler
    kShaderOptimization_LOWEST        = 0,
    kShaderOptimization_STANDARD    = 1,
    kShaderOptimization_HIGH        = 2,
    kShaderOptimization_HIGHEST        = 3,
    //----------------------------
    kShaderOptimization_DEFAULT = kShaderOptimization_HIGHEST
} tShaderOptimizationLevel;

// Please use getShaderFileBaseDir() to access this
static char sShaderDirectory[kShaderPathRoot_COUNT][_MAX_PATH+1] = { '\0' };    

static bool sbHaveArbVertexShaderNames = false;
static bool sbHaveArbFragmentShaderNames = false;
static bool sbHaveCgVertexShaderNames = false;
static bool sbHaveCgFragmentShaderNames = false;

typedef struct tIncludeFileSearchCookie
{
    bool    bFoundStale;
    char    szSrcFileDir[_MAX_PATH];
    char    szBinFileDir[_MAX_PATH];
} tIncludeFileSearchCookie;

static tIncludeFileSearchCookie    s_IncludeFileSearchCookie = { 0 };

//
//    N.B. - variable and function naming convention:
//
//        "FS" suffix = Is, or creates, a directory or path (directory + file name) using forward slashes
//        "BS" suffix = Is, or creates, a directory or path (directory + file name) using backslashes
//    

static bool                    LoadShaderProgram(U32 compileFlags, char *filename, tShaderProgramType target, tCgShaderProfile profile, int *prog);
static bool                 loadProgram( U32 compileFlags /* combined tShaderCompileFlags */, char* filename, tShaderProgramType target, tCgShaderProfile profile, GLuint programHandle);
static bool                 loadProgramCG( U32 compileFlags /* combined tShaderCompileFlags */, char* cohRelativeSrcPath, tShaderProgramType target, tCgShaderProfile profile, GLuint programHandle);
static bool                 loadProgramARB_OBSOLETE( U32 compileFlags /* combined tShaderCompileFlags */, char* filename, tShaderProgramType target, tCgShaderProfile profile, GLuint programHandle);
static void                 loadProgramCacheReset(bool shouldDoCaching);
static void                 addDefine(char *define);
static bool                 hasDefine(char *define);
static void                 addDefineSet(char *defineSet);    // space-separated list
static void                    setShaderOptimizationLevel( tShaderBuildDescriptor* pBuildDescriptor, tShaderOptimizationLevel nLevel );
static CGprogram            prepareCgProgram( tShaderProgramType target, tCgShaderProfile profile, U32 compileFlags, 
                                const char* cohRelativeSrcFilePathFS, tShaderBuildDescriptor* pBuildDescriptor,
                                char** pCompiledPgmText /*OUT*/);
static void                 prepareARBProgram( GLenum target, U32 compileFlags, char* filename, char* path, char** pProgramText );
static const char*            getShaderFileBaseDir(tShaderPathRootType pathRootType);
static const char*            getCurrentShaderFileBaseDirARB_OBSOLETE(void);    // standard COH path or as overridden by (debug) user
static void                    fixTrailingSlash( char* buffer, size_t buffSz, char trailingSlash /* or 0 to remove */ );
static void                    prepareShaderProfileOption(    tCgShaderProfile profile, tShaderBuildDescriptor* pBuildDescriptor );
static tShaderPathRootType    makeCgShaderSrcFullPathFS( const char* cohRelativePathNameFS, char* buffer, int buffSz );
static void                    getShaderProgramFileTextCG( const char* shaderSrcFullPath, char** ppProgramText, int* pSourceTextLen /* or NULL */ );
static void                    getShaderProgramFileTextARB_OBSOLETE( const char* filename, char** ppProgramText, bool* pFromCache /* or NULL */);
static void                    initShaderBuildDescriptor( GLuint programHdl,    // OpenGL shader "name"
                                            const char* cohRelativeFilePath,
                                            tShaderProgramType target, tCgShaderProfile profile, U32 compileFlags,
                                            const char* shaderDirectory, tShaderPathRootType rootType,
                                            tShaderBuildDescriptor* pBuildDescriptor );
static void                    updateBuildDescriptorContentMd5( tShaderBuildDescriptor* pBuildDescriptor, const char* cgSourceText, int cgSourceTextLen );
static void                    releaseShaderBuildDescriptorMem( tShaderBuildDescriptor* pBuildDescriptor );
static void                    addStringToBuildHints( tShaderBuildDescriptor* pBuildDescriptor, const char* szName, const char* szValue );
static void                    addStringToCmdLineArgs( tShaderBuildDescriptor* pBuildDescriptor, const char* szStr );
static int                    createBuildDescriptorDescrText( tShaderBuildDescriptor* pBuildDescriptor );    // returns length of buffer
static void                    computeBuildKey( tShaderBuildDescriptor* pBuildDescriptor );
static void                    computeBufferMD5( const U8* buffer, const int buffLen, U32 md5Key[4] /*OUT*/ );
static bool                    getPreBakedShaderText( const char* szCgShaderSrcFullPath, tShaderBuildDescriptor* pBuildDescriptor, char** pCompiledPgmText /*OUT*/ );
static void                    storePreBakedShader( tShaderBuildDescriptor* pBuildDescriptor, const char* compiledPgmText );
static tShaderPathRootType    makePreBakedShaderFileInputPathBS( const char* cohRelativeSrcFilePathFS, U32 shaderKey[4], 
                                char* buffer, int bufferSz );
static tShaderPathRootType    makePreBakedShaderFileOutputPathBS( const char* cohRelativeSrcFilePathFS, U32 shaderKey[4], 
                                    char* buffer, int bufferSz );
static bool                    makePreBakeDirectory( const char* szDir );
static void                    getCgShaderCacheRootDirBS(char* pathBuff, size_t buffSz);
static void                    makeMd5KeyString( U32 key[4], char* strBuffer33, size_t buffSz /* at least 33 */ );
static void                    makeMd5KeyStringFromContent( const char* szContent, size_t contentLen, char* keyStrBuff33, size_t keyStrBuffSz );
static void                    makeStandardizedFilePathMd5KeyString( const char* szPathStr, char* strBuffer33, size_t buffSz /* at least 33 */ );
static void                    initIncludeFileSearchCookie( const char* szSrcFileDir, const char* szBinFileDir );
static FileScanAction        rebuildIncludeFileInfo_CB(char* dir, struct _finddata32_t* data);
static FileScanAction        checkIncludeFile_CB(char* dir, struct _finddata32_t* data);
static bool                    checkForStaleIncludeFiles( const char* szSrcFileDir, const char* szBinFileDir );
static bool                    isIncludeFileStale( const char* fileNameNoDirPath /* ex: foo.cgh */, 
                                                        const char* srcDirFullPath, const char* szBinDirFullPath );
static bool                    isBinFileStale( tShaderBuildDescriptor* pBuildDescriptor );
static const char*            findContentMd5Field( const char* szInfoFileText );
static void                    rebuildIncludeFileInfo( const char* szSrcDir, const char* szCacheDir );

static void GenPrograms(tShaderProgramType target, int *prog)
{
    ValidateShaderProgramTypeConstant(target);
    if ( rt_cgGetCgShaderMode() )
    {
        rt_cgGenerateCgShaderIds(target,1,prog);
        if ( target == kShaderPgmType_VERTEX )
        {
            sbHaveCgVertexShaderNames = true;
        }
        else
        {
            sbHaveCgFragmentShaderNames = true;
        }
    }
    else
    {
        glGenProgramsARB(1, prog); CHECKGL;
        if ( target == kShaderPgmType_VERTEX )
        {
            sbHaveArbVertexShaderNames = true;
        }
        else
        {
            sbHaveArbFragmentShaderNames = true;
        }
    }
}

void shaderMgr_SetCgShaderPath( const char* szPath )
{
    char* shaderPathBuffer = sShaderDirectory[kPathRoot_Custom];

    if (( szPath == NULL ) || ( *szPath == '\0' ))
    {
        // if no path sent, just reset.
        shaderPathBuffer[0] = '\0';
        return;
    }
    
    // If the path relative? If so, make it relative to the 
    // app's directory ("bin").
    if ( *szPath == '.' )
    {
        int i;
        int nPathsToPluck = 1;    // 1 for the module file name
        const char* szPathAdj = szPath;
        char* p;
        GetModuleFileNameA(NULL,sShaderDirectory[kPathRoot_Custom],ARRAY_SIZE(sShaderDirectory[kPathRoot_Custom]));

        // Handle the single dot prefix: "." by itself or "./blah"        
        if ( szPathAdj[1] == '\0' )
        {
            szPathAdj++;
        }
        else if (( szPathAdj[1] == '\\' ) || ( szPathAdj[1] == '/' ))
        {
            szPathAdj += 2;
        }
        
        // Handle the ".." prefix
        while (( szPathAdj[0] == '.' ) &&
               ( szPathAdj[1] == '.' ))
        {
            nPathsToPluck++;
            if ( szPathAdj[2] != '\0' )
                szPathAdj += 3;    // pass the "..\"
            else
                szPathAdj += 2;    // pass the ".."
        }
        
        p = ( sShaderDirectory[kPathRoot_Custom] + strlen(sShaderDirectory[kPathRoot_Custom]) );
        for ( i=0; i < nPathsToPluck; i++ )
        {
            // back up to the next path boundary
            while (( p != sShaderDirectory[kPathRoot_Custom] ) && ( *p != '/' ) && ( *p != '\\' ))
            {
                p--;
            }
            if ( p != sShaderDirectory[kPathRoot_Custom] ) p--;
        }
        if ( *szPathAdj != '\0' )
        {
            *(++p) = '/';
            strncpy_s( p+1, ( ARRAY_SIZE(sShaderDirectory[kPathRoot_Custom]) - strlen(sShaderDirectory[kPathRoot_Custom]) ), szPathAdj, _TRUNCATE );
        }
        else
        {
            *(++p) = '\0';
        }
        for ( p = sShaderDirectory[kPathRoot_Custom]; *p != '\0'; p++ )
        {
            if ( *p == '\\' ) *p = '/';
        }
    }
    else
    {
        strncpy_s( sShaderDirectory[kPathRoot_Custom], ARRAY_SIZE(sShaderDirectory[kPathRoot_Custom]), szPath, _TRUNCATE );
    }
    
    // Make sure this folder is monitored for changes by the FolderCache system
    fileAddGameDataDir( sShaderDirectory[kPathRoot_Custom] );

    strcat(sShaderDirectory[kPathRoot_Custom], "/"); // postpend slash now required for downstream stuff
}

const char* getCurrentShaderFileBaseDirARB_OBSOLETE(void)
{
    // standard COH path or as overridden by (debug) user
    tShaderPathRootType rootType = ( sShaderDirectory[kPathRoot_Custom][0] != '\0' ) 
        ? kPathRoot_Custom : kPathRoot_GameData;
    return getShaderFileBaseDir( rootType );
}

const char* getShaderFileBaseDir(tShaderPathRootType pathRootType)
{
    static bool bInitialized = false;
    
    if ( ! bInitialized )
    {
        //
        //    kPathRoot_GameData
        //
        //    Using "" as the base path works with loose files as well as piggs.
        //    The file.c overrides will search data directories specified in gamedatadirs.txt
        //    to build an absolute path, so we don't need to use fileDataDir() to construct
        //    a full path ourselves.
        //
        sShaderDirectory[kPathRoot_GameData][0] = '\0';

        //
        //    kPathRoot_GameData
        //
        //     sShaderDirectory[kPathRoot_GameData] is not auto built. You have to call 
        //    shaderMgr_SetCgShaderPath() to set it.
        //

        //
        //    kPathRoot_GlobalAppDataPath
        //
        //    Construct a path which works with Windows7 and beyond -- i.e., something in
        //    the user's directory (c:\documents and settings\<USER_NAME>\blah...).
        //
        {
            char* buffer = sShaderDirectory[kPathRoot_GlobalAppDataPath];
            size_t buffSz = ARRAY_SIZE(sShaderDirectory[kPathRoot_GlobalAppDataPath]);
        
            HRESULT hRes = SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, buffer);
            if (SUCCEEDED(hRes))
            {
                strcat_s(buffer, buffSz, LOCAL_USER_DATA_SUBDIR );
                SHCreateDirectoryExA(NULL, buffer, NULL);
                forwardSlashes(buffer);
                fixTrailingSlash( buffer, buffSz, '/' ); 
            }
            else
            {
                buffer[0] = '\0';
            }
        }
    
        bInitialized = true;
    }

    return sShaderDirectory[pathRootType];
}

static void fixTrailingSlash( char* buffer, size_t buffSz, char trailingSlash /* or 0 to remove */ )
{
    char* p;
    size_t stringLen = strlen(buffer);
    
    if ( stringLen == 0 )
    {
        //
        //    Special case. If the string is empty, don't add a slash.
        //
        return;
    }
    else
    {
        p = ( buffer + stringLen - 1 );
    }
    if (( *p == '\\' ) || ( *p == '/' ))
    {
        *p = trailingSlash;        // may change slash type or zero it out
    }
    else
    {
        if (( trailingSlash != '\0' ) && ( stringLen < ( buffSz - 2 ) ))
        {
            // want a trailing slash and we have room for it
            buffer[stringLen++] = trailingSlash;
            buffer[stringLen]   = '\0';
        }
    }
}

void shaderMgr_SetOverrideShaderDir( void )
{
    const char* overrideDir = "";
    if ( game_state.use_prev_shaders ) {
        overrideDir = "prev/";
    } else if ( game_state.normalmap_test_mode > 0 ) {
        overrideDir = "normal_map_test/";
    }
    strcpy_s( shaderMgrOverrideShaderDir, ARRAY_SIZE(shaderMgrOverrideShaderDir), overrideDir );
}

static bool LoadShaderProgram(U32 compileFlags, char *filename, tShaderProgramType target, tCgShaderProfile profile, int *prog)
{
    bool ret=true;
    ValidateShaderProgramTypeConstant( target );
    
    PERFINFO_AUTO_START("LoadShaderProgram", 1);
    PERFINFO_AUTO_START("Regular", 1);
    
    ret = loadProgram(compileFlags, filename, target, profile, *prog);

    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();

    return ret;
}

static bool LoadFragmentPrograms(U32 compileFlags, BlendModeShader which, int* prog)
{
    char fullPath[MAX_PATH] = "\0";
    char* formatShaderPath;
    bool bDebugShader = false;
    tShaderPgmBuildSpec* shaderSpecTbl;
    tShaderPgmBuildSpec* shaderSpec = NULL;
    int i;

#if RT_SUPPORT_ARB_SHADER_PATH
    if (!rt_cgGetCgShaderMode())
    {
        // ARB PATH - DEPRECATED
        static tShaderPgmBuildSpec sFragmentProgramTbl_ARB[] = {
            { BLENDMODE_MODULATE,                0,    GL_FRAGMENT_PROGRAM_ARB, 0, "modulate",              0, NULL },
            { BLENDMODE_MULTIPLY,                0,    GL_FRAGMENT_PROGRAM_ARB, 0, "multiplyReg",           0, NULL },
            { BLENDMODE_COLORBLEND_DUAL,         0,    GL_FRAGMENT_PROGRAM_ARB, 0, "colorBlendDual",        0, NULL },
            { BLENDMODE_ADDGLOW,                 0,    GL_FRAGMENT_PROGRAM_ARB, 0, "addGlow",               0, NULL },
            { BLENDMODE_ALPHADETAIL,             0,    GL_FRAGMENT_PROGRAM_ARB, 0, "alphaDetail",           0, NULL },
            { BLENDMODE_BUMPMAP_MULTIPLY,        0,    GL_FRAGMENT_PROGRAM_ARB, 0, "bumpmapMultiply",       0, NULL },
            { BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0,    GL_FRAGMENT_PROGRAM_ARB, 0, "bumpmapColorblendDual", 0, NULL },
            { BLENDMODE_WATER,                   0,    GL_FRAGMENT_PROGRAM_ARB, 0, "water",                 0, NULL },
            { BLENDMODE_MULTI,                   0,    GL_FRAGMENT_PROGRAM_ARB, 0, "multi9",                0, NULL },
            { BLENDMODE_SUNFLARE,                0,    GL_FRAGMENT_PROGRAM_ARB, 0, "sunflare",              0, NULL },
            // -- END MARKER --
            { -1 }
        };
        shaderSpecTbl = sFragmentProgramTbl_ARB;
        compileFlags |= kCompileFlag_ARB;
        //
        //    %s params: 
        //        1) custom shader sub-dir (always "" for ARB)
        //        2) shader file name root
        //
        formatShaderPath = SHADER_SUBDIR "arb/%s%s.fp";
    }
    else
#endif
    {
        static tShaderPgmBuildSpec sFragmentProgramTbl[] = {
            { BLENDMODE_MODULATE,                0,    0, 0, "modulate",              0, NULL },
            { BLENDMODE_MULTIPLY,                0,    0, 0, "multiplyReg",           0, NULL },
            { BLENDMODE_COLORBLEND_DUAL,         0,    0, 0, "colorBlendDual",        0, NULL },
            { BLENDMODE_ADDGLOW,                 0,    0, 0, "addGlow",               0, NULL },
            { BLENDMODE_ALPHADETAIL,             0,    0, 0, "alphaDetail",           0, NULL },
            { BLENDMODE_BUMPMAP_MULTIPLY,        0,    0, 0, "bumpmapMultiply",       0, NULL },
            { BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0,    0, 0, "bumpmapColorblendDual", 0, NULL },
            //       { BLENDMODE_WATER,                   NV3X, CG_PROFILE_FP40,   0, "water", 0, NULL },
            { BLENDMODE_WATER,                   0,    0, 0, "water",                 0, NULL },
            { BLENDMODE_MULTI,                   0,    0, 0, "multi9",                0, NULL },
            { BLENDMODE_SUNFLARE,                0,    0, 0, "sunflare",              0, NULL },
            // -- END MARKER --
            { -1 }
        };
        shaderSpecTbl = sFragmentProgramTbl;
        compileFlags |= kCompileFlag_Cg;
        //
        //    %s params: 
        //        1) custom shader sub-dir (or "")
        //        2) shader file name root
        //
        formatShaderPath = SHADER_SUBDIR "cgfx/%s%sfp.cg";

        //if we are using Cg programs and we have a shadertest case set to one 
        //  that utilizes the debug fragment shader, swap out the filename for the
        //  regular shaders
        switch(game_state.shaderTest)
        {
        case 5:     //"TEXCOORDS0",
        case 6:     //"TEXCOORDS1",
        case 7:     //"WHICH_SHADER",
        case 8:     //"SHOWLIGHTING",
        case 10:    //"TEXCOORD_TEST"
            bDebugShader = true;
            break;
        }

        if(game_state.shaderTest == 7)
        {
            const Vec3 vColor[BLENDMODE_NUMENTRIES] = {
                { 0.00f, 0.00f, 0.00f },
                { 0.50f, 0.00f, 0.00f },
                { 0.00f, 0.50f, 0.00f },
                { 0.50f, 0.50f, 0.00f },
                { 0.00f, 0.00f, 0.50f },

                { 0.50f, 0.00f, 0.50f },
                { 0.00f, 0.50f, 0.50f },
                { 1.00f, 0.50f, 0.00f },
                { 0.25f, 0.00f, 0.50f },
                { 0.93f, 0.86f, 0.50f },
            };
            char stringWhichColor[128];

            //special case for WHICH_SHADER, set #define for color

            snprintf(stringWhichColor, 128, "WHICH_SHADER_R=%4.2f", vColor[which][0]);
            addDefine(stringWhichColor);
            snprintf(stringWhichColor, 128, "WHICH_SHADER_G=%4.2f", vColor[which][1]);
            addDefine(stringWhichColor);
            snprintf(stringWhichColor, 128, "WHICH_SHADER_B=%4.2f", vColor[which][2]);
            addDefine(stringWhichColor);
        }
    }    // end Cg specific path
    
    // common
    for ( i=0; shaderSpecTbl[i].shaderMode != -1; i++ )
    {
        if (( shaderSpecTbl[i].shaderMode == which ) &&
            (( shaderSpecTbl[i].chipFlags & rdr_caps.chip ) == shaderSpecTbl[i].chipFlags ))
        {
            shaderSpec = &shaderSpecTbl[i];
            break;
        }
    }
    assert( shaderSpec != NULL );
    
    if ( shaderSpec->extraDefineSet )
        addDefineSet( shaderSpec->extraDefineSet );
        
    #ifndef FINAL
        //
        // Support loading shaders from custom subdirectories.
        //
        fullPath[0] = '\0';
        if ( shaderMgrOverrideShaderDir[0] != '\0' )
        {
            snprintf(fullPath, MAX_PATH, formatShaderPath,shaderMgrOverrideShaderDir,shaderSpec->filenameRoot);
            if ( ! fileExists(fullPath) )
            {
                 fullPath[0] = '\0';
            }
        }
        if ( fullPath[0] == '\0' )
    #endif
        {
            snprintf(fullPath, MAX_PATH, formatShaderPath,"",shaderSpec->filenameRoot);
        }
    return LoadShaderProgram(
                compileFlags, 
                fullPath, kShaderPgmType_FRAGMENT,
                shaderSpec->profile,
                prog);
}

void shaderMgr_ReleaseShaderIds(void)
{
    int i;
    
    if (( sbHaveCgVertexShaderNames ) || ( sbHaveCgFragmentShaderNames ))
    {
        rt_cgResetCgShaderIds();
        sbHaveCgVertexShaderNames = false;
        sbHaveCgFragmentShaderNames = false;
    }
    
    if ( sbHaveArbFragmentShaderNames )
    {
        int j;

        // delete generic material shader variations
        for (i = 0; i < BLENDMODE_NUMENTRIES; ++i)
        {
            for (j=0; j < BMB_VARIANT_COUNT; ++j)
            {
                if (g_shaderMgrFragmentProgramVariants[i][j])
                {
                    glDeleteProgramsARB(1, &g_shaderMgrFragmentProgramVariants[i][j]); CHECKGL;
                }
            }
        }

        // delete special effects shaders
        for (i = 0; getSpecialShaderName(i,NULL,0,NULL,NULL); i++){
            glDeleteProgramsARB(1,&shaderEffectsPrograms[i]); CHECKGL;
        }

        sbHaveArbFragmentShaderNames = false;
    }
    
    if ( sbHaveArbVertexShaderNames )
    {
        for (i = 0; i < ARRAY_SIZE(shaderMgrVertexPrograms); i++) {
            glDeleteProgramsARB(1,&shaderMgrVertexPrograms[i]); CHECKGL;
        }

        for (i = 0; i < ARRAY_SIZE(shaderMgrVertexProgramsHQ); i++){
            glDeleteProgramsARB(1,&shaderMgrVertexProgramsHQ[i]); CHECKGL;
        }
            
        sbHaveArbVertexShaderNames = false;
    }
    WCW_FetchGLState();
    WCW_ResetState();
}

// this function will add preprocess compilation defines
// for the enabled blend mode bit features
static void addDefinesForVariant( int shader, int bmb )
{
    static struct {
        int flag;
        char *define;
    } multiFlags[] = {
        {BMB_HIGH_QUALITY,      "BIT_HIGH_QUALITY"},
        {BMB_SHADOWMAP,            "BIT_SHADOWMAP"},
        {BMB_CUBEMAP,           "BIT_CUBEMAP"},
        {BMB_PLANAR_REFLECTION, "BIT_PLANAR_REFLECTION"},
        {BMB_SINGLE_MATERIAL,   "BIT_SINGLE_MATERIAL"},
        {BMB_BUILDING,          "BIT_BUILDING"},
#ifndef FINAL
        {BMB_DEBUG,                "BIT_DEBUG"},
#endif
    };
    int j;

    // walk through the blend mode bits for variation and
    // add defines for those that are set
    for (j=0; j<ARRAY_SIZE(multiFlags); j++)
    {
        if (bmb & multiFlags[j].flag) {
            addDefine(multiFlags[j].define);
        }
    }

    // If we are compiling a shadow mapping variant then we need to set the quality
    // @todo there is a better way to do this/home for this I imagine
    if (bmb&BMB_SHADOWMAP)
    {
        switch (game_state.shadowShaderSelection)
        {
        case SHADOWSHADER_NONE:
            // @todo - I think this will only currently happen if shadows are disabled but we try
            // to compile a shadowmap variant.
            // We should remove this as a choice now that we have BMB_SHADOWMAP to avoid problems
            // In the interim we don't do anything at the moment which means shadow shaders
            // that have this will get the default (fast) until a new setting is enabled.
            break;
        
        case SHADOWSHADER_FAST:
            break;
        case SHADOWSHADER_MEDIUMQ:
            addDefine( "SHADOWMAP_RUNTIME_PCF_2x2" );
            break;
        case SHADOWSHADER_HIGHQ:
            addDefine( "SHADOWMAP_RUNTIME_PCF_4x4" );
            break;
        case SHADOWSHADER_DEBUG:
            addDefine("SHADOWMAP_RUNTIME_DEBUG");
            break;
        default :
            assert(false && "unknown shadow shader");
            break;
        }
    }

    if (rdr_caps.limited_fragment_instructions && shader == BLENDMODE_MULTI)
        addDefine("LIMIT_FRAGMENT_INSTRUCTIONS");
}

// test the blend mode bits for the given shader blend mode
// and return true if the variant is supported
static bool isVariantSupported( int shader, int bmb )
{
#ifndef FINAL
    // We only load/compile debug variants if we are currently
    // have shader debugging enabled. This allows us to avoid
    // having everyone pay the penalty to initialize the debug
    // shaders on every launch and just initialize them when someone
    // uses them for the first time.
    if (bmb&BMB_DEBUG)
    {
        // if we have loaded them already then full shader rebuilds
        // also need to rebuild the debug variants
        if (!g_mt_shader_debug_state.flag_loaded && !g_mt_shader_debug_state.flag_loading)
            return false;
    }
#endif

    if (shader == BLENDMODE_MODULATE && !rt_cgGetCgShaderMode())
        return false;

    // MULTI has the most variations and the most can go wrong
    // with support for it
    if (shader == BLENDMODE_MULTI)
    {
        // if GFXF_MULTITEX is not available
        // or has been disabled by implementation limitation then we don't
        // bother trying any of the MULTI variants
        if (!(rdr_caps.allowed_features&GFXF_MULTITEX))
            return false;
        
        // the 'dual' multi variants may not be supported even if 'single material'
        // manages to squeak by
        if ( (bmb&BMB_SINGLE_MATERIAL)==0 && !(rdr_caps.allowed_features&GFXF_MULTITEX_DUAL))
            return false;

        if (bmb&BMB_HIGH_QUALITY && !(rdr_caps.allowed_features&GFXF_MULTITEX_HQBUMP))
            return false;
    }
    else
    {
        // certain blend mode bits are MULTI specific so we don't need
        // to compile them otherwise.

        // these don't ever apply to non MULTI variants
        if (bmb&(BMB_SINGLE_MATERIAL|BMB_BUILDING))
            return false;
    }

    if (shader == BLENDMODE_WATER)
    {
        if (!(rdr_caps.allowed_features&GFXF_WATER))
            return false;
        if (bmb&(BMB_HIGH_QUALITY|BMB_CUBEMAP))
            return false;
    }

    if (bmb&BMB_HIGH_QUALITY)
    {
        if (!(rdr_caps.allowed_features&GFXF_HQBUMP))
            return false;
    }

    if (bmb&BMB_SHADOWMAP && !(rdr_caps.allowed_features&GFXF_SHADOWMAP) )
        return false;

    if (bmb&(BMB_CUBEMAP|BMB_PLANAR_REFLECTION) && !(rdr_caps.allowed_features&GFXF_CUBEMAP) )
        return false;

    return true;
}

// If compilation fails then disable problem feature variations
// this will disable features for whatever bits are set, though
// generally you would want to do it one bit at time as you
// add features of increasing complexity which cause compilation
// to fail.
// @todo the way this was done before and as currently modified
// are likely not really sophisticated enough to handle variation
// failure as needed or desired.
static void disableVariantFeature( int shader, int bmb )
{
    // MULTI has the most variations and the most can go wrong
    // with support for it
    if (shader == BLENDMODE_MULTI)
    {
        // the 'single material' simplification of MULTI
        // can't even make it on this implementation then
        // just disable the entire feature
        if (bmb&BMB_SINGLE_MATERIAL)
        {
            rdr_caps.allowed_features &= ~GFXF_MULTITEX;
            rdr_caps.allowed_features &= ~GFXF_MULTITEX_DUAL;
            rdr_caps.allowed_features &= ~GFXF_MULTITEX_HQBUMP;
        }
        else if (bmb&BMB_HIGH_QUALITY)
        {
            rdr_caps.allowed_features &= ~GFXF_MULTITEX_HQBUMP;
        }
        else
        {
            rdr_caps.allowed_features &= ~GFXF_MULTITEX_DUAL;
            rdr_caps.allowed_features &= ~GFXF_MULTITEX_HQBUMP;
        }

        return;
    }

    if (!bmb)
    {
        // if this is the base material, i.e., no feature bits set
        // and it fails to compile then we need to disable that
        // shader altogether
        if (shader == BLENDMODE_WATER)
            rdr_caps.allowed_features &= ~GFXF_WATER;

        return;
    }

    // additional common feature bits
    if (bmb&BMB_HIGH_QUALITY)
        rdr_caps.allowed_features &= ~GFXF_HQBUMP;

    if (bmb&BMB_SHADOWMAP)
        rdr_caps.allowed_features &= ~GFXF_SHADOWMAP;

    if (bmb&(BMB_CUBEMAP|BMB_PLANAR_REFLECTION))
        rdr_caps.allowed_features &= ~GFXF_CUBEMAP;

}

// return the shader program id for this material blend mode and variation
int    shaderMgr_lookup_fp( BlendModeType blend_type )
{
    int bmb = blend_type.blend_bits & BMB_VARIANT_MASK;    // be sure to mask off any special purpose flags
    int pgmID;

    onlydevassert(blend_type.shader >= 0);
    onlydevassert(blend_type.shader < BLENDMODE_NUMENTRIES);
    onlydevassert(bmb >= 0);
    onlydevassert(bmb < BMB_VARIANT_COUNT);

#ifndef FINAL
    // if shader debug hud is active modify blend mode accordingly to
    // select appropriate debug shader if necessary
    // the HUD also has a press and hold toggle to 'undo' the debug shaders
    // so you can quickly compare debug/non debug without dismissing the hud
    if (g_mt_shader_debug_state.flag_loaded && g_mt_shader_debug_state.flag_enable && g_mt_shader_debug_state.flag_use_debug_shaders )
    {
        bmb |= BMB_DEBUG;
    }
#endif

    pgmID = g_shaderMgrFragmentProgramVariants[blend_type.shader][bmb];

    onlydevassert(pgmID!= 0 && "unsupported shader variant requested");

    return pgmID;
}

void shaderMgr_InitFPs(void)
{
    int i, j, i_shader;
    static bool executedOnce = false;
    bool bUseCG = rt_cgGetCgShaderMode();

    //consoleSetColor(COLOR_GREEN|COLOR_BLUE, 0);
    writeConsole(OUTPUT_INFO, "Compiling fragment shaders");
    //consoleSetDefaultColor();

    PERFINFO_AUTO_START("shaderMgr_InitFPs", 1);
    loadProgramCacheReset(false); // LSTL - disabling the ARB program cache
    
    WCW_BindFragmentProgram(0);
    WCW_DisableFragmentProgram();
    WCW_PrepareToDraw();
    rt_cgResetComboShaderHandles();

    // generate the name IDs for our shader programs
    PERFINFO_AUTO_STOP_START("GenPrograms", 1);
    // Don't execute if we don't need to
    if ((( !bUseCG ) && ( ! sbHaveArbFragmentShaderNames )) ||
        ((  bUseCG ) && ( ! sbHaveCgFragmentShaderNames )))
    {
        // Generate a fragment program ID/name for each blendmode and variation
        for (i = 0; i < BLENDMODE_NUMENTRIES; ++i)
        {
            for (j=0; j < BMB_VARIANT_COUNT; ++j)
            {
                // do not allocate an ID for variants which are unsupported
                if (isVariantSupported(i, j))
                    GenPrograms(kShaderPgmType_FRAGMENT, &g_shaderMgrFragmentProgramVariants[i][j]);
            }
        }

        // Generate a fragment program ID/name for Special effects shaders
        {
            for (i = 0; getSpecialShaderName(i,NULL,0,NULL,NULL); i++)
            {
                GenPrograms(kShaderPgmType_FRAGMENT, &shaderEffectsPrograms[i]);
            }
        }
    }

    //****
    // Compile the shaders for each base material and its variations
    PERFINFO_AUTO_STOP_START("CompileShaderVariations", 1);
    for ( i_shader = 0; i_shader < BLENDMODE_NUMENTRIES; ++i_shader )
    {
        int bmb, bmb_prev = 0;
        for (bmb=0; bmb < BMB_VARIANT_COUNT; ++bmb)
        {
            // do not compile variants which are unsupported
            if (!isVariantSupported(i_shader, bmb))
                continue;

            // add defines to tailor compilation for variation
            addDefinesForVariant( i_shader, bmb );

            // now that all the variant definitions have been made for this permutation,
            // compile the shader
            if ( !LoadFragmentPrograms(0, i_shader, &g_shaderMgrFragmentProgramVariants[i_shader][bmb]) )
            {
                // If compilation fails then disable problem feature variations
                if (!executedOnce)    // do not disable features on shader reload
                {
                    // determine which bit just got set and disable that
                    int bmb_new = (bmb^bmb_prev)&bmb;
                    disableVariantFeature( i_shader, bmb_new );
                }
            }
            bmb_prev = bmb;
        }
    }

    //****
    // Compile special effects shaders used by rt_effects
    PERFINFO_AUTO_STOP_START("loadProgram:specialEffects", 1);
    {
        char shaderRelativePath[128];
        U32 compileFlags = 0;
        char* shaderExtraDefineSet = NULL;
        for (i=0; getSpecialShaderName(i,shaderRelativePath,ARRAY_SIZE(shaderRelativePath),&compileFlags,&shaderExtraDefineSet); i++) {
            if(shaderExtraDefineSet)
                addDefineSet(shaderExtraDefineSet);
            loadProgram(
                    compileFlags, 
                    shaderRelativePath, 
                    kShaderPgmType_FRAGMENT, 
                    ( bUseCG ) ? 0 : GL_FRAGMENT_PROGRAM_ARB,
                    shaderEffectsPrograms[i]
                );
        }
    }

    // Disable features that failed to compile and load
    if (!executedOnce) // do not disable features on shader reload
        rdr_caps.features &= rdr_caps.allowed_features;

    executedOnce = 1;
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

void shaderMgr_InitVPs(void)
{
    // ARB PATH - DEPRECATED
    static tShaderPgmBuildSpec sVertexProgramTbl_ARB[] = {
       { DRAWMODE_SPRITE,                        0,    GL_VERTEX_PROGRAM_ARB, 0,                      "fixed_function",  0,             "NOLIGHT" },
       { DRAWMODE_DUALTEX,                       0,    GL_VERTEX_PROGRAM_ARB, 0,                      "fixed_function",  0,             "NOLIGHT" },
       { DRAWMODE_DUALTEX_NORMALS,               0,    GL_VERTEX_PROGRAM_ARB, 0,                      "fixed_function",  0,             NULL },
       { DRAWMODE_COLORONLY,                     0,    GL_VERTEX_PROGRAM_ARB, 0,                      "fixed_function",  0,             "NOLIGHT" },
       { DRAWMODE_FILL,                          0,    GL_VERTEX_PROGRAM_ARB, 0,                      "fixed_function",  0,             "NOLIGHT" },
       { DRAWMODE_HW_SKINNED,                    0,    GL_VERTEX_PROGRAM_ARB, kCompileFlag_NotPosInv, "skin",            0,             NULL },
       { DRAWMODE_BUMPMAP_NORMALS,               0,    GL_VERTEX_PROGRAM_ARB, 0,                      "bump",            0,             NULL },
       { DRAWMODE_BUMPMAP_SKINNED,               0,    GL_VERTEX_PROGRAM_ARB, kCompileFlag_NotPosInv, "skin_bump",       0,             NULL},
       { DRAWMODE_BUMPMAP_RGBS,                  0,    GL_VERTEX_PROGRAM_ARB, 0,                      "bump_rgb",        0,             NULL},
       { DRAWMODE_BUMPMAP_DUALTEX,               0,    GL_VERTEX_PROGRAM_ARB, 0,                      "bump_dual",       0,             NULL},
       { DRAWMODE_BUMPMAP_MULTITEX,              0,    GL_VERTEX_PROGRAM_ARB, 0,                      "bump_dual_multi", GFXF_MULTITEX|GFXF_WATER,NULL},
       { DRAWMODE_BUMPMAP_MULTITEX_RGBS,         0,    GL_VERTEX_PROGRAM_ARB, 0,                      "bump_dual_multi", GFXF_MULTITEX, "RGBS"},
       { DRAWMODE_BUMPMAP_SKINNED_MULTITEX,      0,    GL_VERTEX_PROGRAM_ARB, kCompileFlag_NotPosInv, "skin_bump",       GFXF_MULTITEX, "MULTI"},
       // -- END MARKER --
       { -1 }
    };

    // Vertex Shaders for legacy hardware which are compatible with the register combiner programs (ARBFP is not available)
    static tShaderPgmBuildSpec sVertexProgramTbl_RegisterCombiner[] = {
        { DRAWMODE_SPRITE,                        0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_DUALTEX,                       0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_DUALTEX_NORMALS,               0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_LIT_GL   TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_COLORONLY,                     0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_FILL,                          0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_HW_SKINNED,                    0,    0,     kCompileFlag_NotPosInv, "vp_master_legacy_",  0, "SKIN=1 LIGHT_SPACE=VIEW   VERTEX_LIT=DIFFUSE     TC_XFORM=NONE       PIXEL_LIT=NONE      REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_NORMALS,               0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=MODEL  VERTEX_LIT=DIFFUSE     TC_XFORM=TC_OFFSET  PIXEL_LIT=BUMP_SPEC REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_SKINNED,               0,    0,     kCompileFlag_NotPosInv, "vp_master_legacy_",  0, "SKIN=1 LIGHT_SPACE=VIEW   VERTEX_LIT=NONE        TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_RGBS,                  0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=MODEL  VERTEX_LIT=PRELIT      TC_XFORM=TC_OFFSET  PIXEL_LIT=BUMP_SPEC REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_DUALTEX,               0,    0,     0,                      "vp_master_legacy_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=NONE        TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=NONE" },
        // -- END MARKER --
        { -1 }
    };

    // vertex shaders for ARBFP capable system
    static tShaderPgmBuildSpec sVertexProgramTbl[] = {
        { DRAWMODE_SPRITE,                        0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_DUALTEX,                       0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_DUALTEX_NORMALS,               0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_LIT_GL   TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_DUALTEX_LIT_PP,                  0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=VERT_COLOR  TC_XFORM=TC_MATRIX  PIXEL_LIT=LIT       REFLECT=FAUX_0_1"},
        { DRAWMODE_COLORONLY,                     0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_FILL,                          0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=FF_UNLIT_GL TC_XFORM=TC_MATRIX  PIXEL_LIT=NONE      REFLECT=FAUX_0_1"},
        { DRAWMODE_HW_SKINNED,                    0,    0,     kCompileFlag_NotPosInv, "vp_master_",  0, "SKIN=1 LIGHT_SPACE=VIEW   VERTEX_LIT=DIFFUSE     TC_XFORM=NONE       PIXEL_LIT=NONE      REFLECT=NONE     USE_NV_CLIP_PLANES" },
        { DRAWMODE_BUMPMAP_NORMALS,               0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=MODEL  VERTEX_LIT=DIFFUSE     TC_XFORM=TC_OFFSET  PIXEL_LIT=BUMP_SPEC REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_NORMALS_PP,            0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=VERT_COLOR  TC_XFORM=TC_OFFSET  PIXEL_LIT=BUMP_ALL  REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_SKINNED,               0,    0,     kCompileFlag_NotPosInv, "vp_master_",  0, "SKIN=1 LIGHT_SPACE=VIEW   VERTEX_LIT=NONE        TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=NONE     USE_NV_CLIP_PLANES" },
        { DRAWMODE_BUMPMAP_RGBS,                  0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=MODEL  VERTEX_LIT=PRELIT      TC_XFORM=TC_OFFSET  PIXEL_LIT=BUMP_SPEC REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_DUALTEX,               0,    0,     0,                      "vp_master_",  0, "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=NONE        TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=NONE" },
        { DRAWMODE_BUMPMAP_MULTITEX,              0,    0,     0,                      "vp_master_", GFXF_MULTITEX|GFXF_WATER,    "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=PRELIT_WHITE TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=FAUX_MULTI" },
        { DRAWMODE_BUMPMAP_MULTITEX_RGBS,         0,    0,     0,                      "vp_master_", GFXF_MULTITEX,                "SKIN=0 LIGHT_SPACE=VIEW   VERTEX_LIT=PRELIT       TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=FAUX_MULTI"},
        { DRAWMODE_BUMPMAP_SKINNED_MULTITEX,      0,    0,     kCompileFlag_NotPosInv, "vp_master_", GFXF_MULTITEX,                "SKIN=1 LIGHT_SPACE=VIEW   VERTEX_LIT=PRELIT_WHITE TC_XFORM=NONE       PIXEL_LIT=BUMP_ALL  REFLECT=FAUX_MULTI     USE_NV_CLIP_PLANES"},
        { DRAWMODE_DEPTH_SKINNED,                 0,    0,     kCompileFlag_NotPosInv, "shadowmap_skin_", 0,              NULL},
        // -- END MARKER --
        { -1 }
    };

    U32 baseCompileFlags = kCompileFlag_Cg;
#ifndef FINAL
    const char* fileNamePathFmt = game_state.use_prev_shaders ? SHADER_SUBDIR "cgfx/prev/%svp.cg": SHADER_SUBDIR "cgfx/%svp.cg";
#else
    const char* fileNamePathFmt    = SHADER_SUBDIR "cgfx/%svp.cg";
#endif
    tShaderPgmBuildSpec* pVertexSpecTbl = sVertexProgramTbl;
    static int executedOnce = 0;
    bool multiOkay=true, multiHqOkay=true, hqOkay=true, waterOkay=true;
    bool bUseCG = rt_cgGetCgShaderMode();
    bool bShaderMatch = false;
    int i;

    if ( ! bUseCG )
    {
        // ARB PATH
        baseCompileFlags    = kCompileFlag_ARB;
        fileNamePathFmt        = SHADER_SUBDIR "arb/%s.vp";
        pVertexSpecTbl        = sVertexProgramTbl_ARB;
    }
    
    //consoleSetColor(COLOR_GREEN, 0);
    writeConsole(OUTPUT_INFO, "Compiling vertex shaders");
    //consoleSetDefaultColor();

    PERFINFO_AUTO_START("shaderMgr_InitVPs", 1);

    loadProgramCacheReset(false);
    
    WCW_BindVertexProgram(0);
    WCW_DisableVertexProgram();
    WCW_PrepareToDraw();
    rt_cgResetComboShaderHandles();

    // If we're not running the ARB assembly path, do not initialize it.
    assert(rdr_caps.chip & ARBVP);

    PERFINFO_AUTO_STOP_START("GenPrograms", 1);
    if ((( ! bUseCG ) && (! sbHaveArbVertexShaderNames )) || 
        ((   bUseCG ) && ( ! sbHaveCgVertexShaderNames )))
    {
        // Generate the program objects
        for (i = 0; i < ARRAY_SIZE(shaderMgrVertexPrograms); i++)
            GenPrograms(kShaderPgmType_VERTEX, &shaderMgrVertexPrograms[i]);

        for (i = 0; i < ARRAY_SIZE(shaderMgrVertexProgramsHQ); i++)
            GenPrograms(kShaderPgmType_VERTEX, &shaderMgrVertexProgramsHQ[i]);
    }

    if (rdr_caps.use_vertshaders) {
        if ((rdr_caps.chip & ARBFP) || !bUseCG )
        {
            // when ARBFP is present we attempt to load all material variants and HQ versions
            int nLastLoadedDrawModeType = -1;
            PERFINFO_AUTO_STOP_START("LoadShaderProgram:regular", 1);

            // Load the vertex programs
            bShaderMatch = false;
            for (i=0; pVertexSpecTbl[i].shaderMode != -1; i++) {
                bool ret;
                tShaderPgmBuildSpec* vertexPgmSpec = &pVertexSpecTbl[i];
                if (( bShaderMatch && ( nLastLoadedDrawModeType == vertexPgmSpec->shaderMode )) ||
                    (( vertexPgmSpec->chipFlags != 0 )  &&
                     (( rdr_caps.chip & vertexPgmSpec->chipFlags ) != vertexPgmSpec->chipFlags )))
                {
                    // Either we've already handled this draw_mode, or we don't support this profile. 
                    // Go to the next one.
                    bShaderMatch = false;
                    continue;
                }
                bShaderMatch = true;
                nLastLoadedDrawModeType = vertexPgmSpec->shaderMode;
                if (vertexPgmSpec->extraDefineSet)
                    addDefineSet(vertexPgmSpec->extraDefineSet);
                if (vertexPgmSpec->required_features & GFXF_MULTITEX)
                    addDefine("MULTI");
                // If there is hardware profile support for user clip planes when not using position invariance
                // (e.g. for skinned objects) go ahead and use it if so instructed.
                if ( rdr_caps.supports_vp_clip_planes_nv && rt_cgGetCgShaderMode() && hasDefine("USE_NV_CLIP_PLANES"))
                    vertexPgmSpec->profile = CG_PROFILE_VP40;
                if ((vertexPgmSpec->required_features & rdr_caps.allowed_features) || !vertexPgmSpec->required_features) {
                    char filePath[2048];
                    _snprintf( filePath, ARRAY_SIZE(filePath), fileNamePathFmt, vertexPgmSpec->filenameRoot );
                    ret = LoadShaderProgram(
                                baseCompileFlags | vertexPgmSpec->extraCompileFlags, 
                                filePath, 
                                kShaderPgmType_VERTEX,
                                vertexPgmSpec->profile,
                                &shaderMgrVertexPrograms[vertexPgmSpec->shaderMode]
                            );
                    if ((vertexPgmSpec->required_features & GFXF_MULTITEX) && !ret)
                        multiOkay = false;
                    if ((vertexPgmSpec->required_features & GFXF_WATER) && !ret)
                        waterOkay = false;
                }
            }
            PERFINFO_AUTO_STOP_START("LoadShaderProgram:HQBump", 1);
            nLastLoadedDrawModeType = -1;
            if (rdr_caps.allowed_features & GFXF_HQBUMP) {
                bShaderMatch = false;
                for (i=0; pVertexSpecTbl[i].shaderMode != -1; i++) {
                    bool ret;
                    char filePath[2048];
                    tShaderPgmBuildSpec* vertexPgmSpec = &pVertexSpecTbl[i];
                    if (( bShaderMatch && ( nLastLoadedDrawModeType == vertexPgmSpec->shaderMode )) ||
                        (( vertexPgmSpec->chipFlags != 0 )  &&
                         (( rdr_caps.chip & vertexPgmSpec->chipFlags ) != vertexPgmSpec->chipFlags )))
                    {
                        // Either we've already handled this draw_mode, or we
                        // support this profile. Go to the next one.
                        bShaderMatch = false;
                        continue;
                    }
                    bShaderMatch = true;
                    nLastLoadedDrawModeType = vertexPgmSpec->shaderMode;        
                    _snprintf( filePath, ARRAY_SIZE(filePath), fileNamePathFmt, vertexPgmSpec->filenameRoot );
                    if (vertexPgmSpec->extraDefineSet)
                        addDefineSet(vertexPgmSpec->extraDefineSet);
                    if (vertexPgmSpec->required_features & GFXF_MULTITEX)
                        addDefine("MULTI");
                    if (!IsUsingCider() && !game_state.safemode && rdr_caps.chip & NV4X && rt_cgGetCgShaderMode() && hasDefine("USE_NV_CLIP_PLANES"))
                        vertexPgmSpec->profile = CG_PROFILE_VP40;
                    addDefine("BIT_HIGH_QUALITY");
                    if ((vertexPgmSpec->required_features & rdr_caps.allowed_features) || !vertexPgmSpec->required_features) {
                        ret = LoadShaderProgram(
                                    baseCompileFlags | vertexPgmSpec->extraCompileFlags, 
                                    filePath, 
                                    kShaderPgmType_VERTEX,
                                    vertexPgmSpec->profile,
                                    &shaderMgrVertexProgramsHQ[vertexPgmSpec->shaderMode]);
                        if ((vertexPgmSpec->required_features & GFXF_MULTITEX) && !ret)
                            multiHqOkay = false;
                        else if (!ret)
                            hqOkay = false;
                    }
                }
            }
        }
        else
        {
            int nLastLoadedDrawModeType = -1;
            // when ARBFP is NOT available so create legacy vertex programs to feed register combiners
            pVertexSpecTbl = sVertexProgramTbl_RegisterCombiner;
            PERFINFO_AUTO_STOP_START("LoadShaderProgram:vp_legacy-register_combiners", 1);

            // Load the vertex programs
            bShaderMatch = false;
            for (i=0; pVertexSpecTbl[i].shaderMode != -1; i++) {
                bool ret;
                tShaderPgmBuildSpec* vertexPgmSpec = &pVertexSpecTbl[i];
                if (( bShaderMatch && ( nLastLoadedDrawModeType == vertexPgmSpec->shaderMode )) ||
                    (( vertexPgmSpec->chipFlags != 0 )  &&
                    (( rdr_caps.chip & vertexPgmSpec->chipFlags ) != vertexPgmSpec->chipFlags )))
                {
                    // Either we've already handled this draw_mode, or we don't support this profile. 
                    // Go to the next one.
                    bShaderMatch = false;
                    continue;
                }
                bShaderMatch = true;
                nLastLoadedDrawModeType = vertexPgmSpec->shaderMode;
                if (vertexPgmSpec->extraDefineSet)
                    addDefineSet(vertexPgmSpec->extraDefineSet);
                if ((vertexPgmSpec->required_features & rdr_caps.allowed_features) || !vertexPgmSpec->required_features) {
                    char filePath[2048];
                    _snprintf( filePath, ARRAY_SIZE(filePath), fileNamePathFmt, vertexPgmSpec->filenameRoot );
                    ret = LoadShaderProgram(
                        baseCompileFlags | vertexPgmSpec->extraCompileFlags, 
                        filePath, 
                        kShaderPgmType_VERTEX,
                        vertexPgmSpec->profile,
                        &shaderMgrVertexPrograms[vertexPgmSpec->shaderMode]
                    );
                }
            }
        }
    }

    if (!executedOnce) // do not disable features on shader reload
    {
        if (!multiOkay)
            rdr_caps.allowed_features &= ~(GFXF_MULTITEX|GFXF_MULTITEX_DUAL|GFXF_MULTITEX_HQBUMP);
        if (!multiHqOkay)
            rdr_caps.allowed_features &= ~(GFXF_MULTITEX_HQBUMP);
        if (!hqOkay)
            rdr_caps.allowed_features &= ~(GFXF_HQBUMP);
        if (!waterOkay)
            rdr_caps.allowed_features &= ~(GFXF_WATER);

        // Disable features that failed to load
        rdr_caps.features &= rdr_caps.allowed_features;
    }

    executedOnce = 1;
    PERFINFO_AUTO_STOP();
    loadProgramCacheReset(false); // Destroy temporary caches
    PERFINFO_AUTO_STOP();
}



static int shader_numMacros=0;
typedef struct MacroParams {
    char *paramNames[8];
    int num_params;
} MacroParams;

static struct {
    char *name; // pointer to other data
    MacroParams params;
    char *body; // estring, destroy me!
} shader_macros[64] = {0};


static char *macroParamDelims = ", \t()";

// Takes "(a,b)", destroys it, returns character 1 after the ")"
static char *fillParams(char *line, MacroParams *params)
{
    char *last=NULL;
    char *s;
    char *tok;
    params->num_params=0;
    if (!line || !*line)
        return line;
    s = strchr(line, ')');
    if (!s)
        return line;
    *s = '\0';
    tok = strtok_r(line, macroParamDelims, &last);
    while (tok) {
        params->paramNames[params->num_params++] = tok;
        tok = strtok_r(NULL, macroParamDelims, &last);
    }
    return last+1;
}

static void addMacro(char *macroName, char *macroBody)
{
    char *last=NULL;
    char *tok;
    assert(shader_numMacros < ARRAY_SIZE(shader_macros));
    tok = strtok_r(macroName, macroParamDelims, &last);
    shader_macros[shader_numMacros].name = tok;
    fillParams(last, &shader_macros[shader_numMacros].params);
    shader_macros[shader_numMacros++].body = macroBody;
}

static bool isvalidchar(unsigned char c)
{
    return isalnum(c) || c=='_';
}

// Compares two strings, stopp at the first non-alphanumeric character
static int strcmpalnum(const unsigned char *a, const unsigned char *b)
{
    for (; *a==*b && isvalidchar(*a) && isvalidchar(*b); a++, b++);
    if (!isvalidchar(*a) && !isvalidchar(*b))
        return 0; // Off end of word on both
    return *a-*b;
}

static void macroConcat(char **dst, int index, MacroParams *replaceParams)
{
    char *walk;
    bool wordStart=true;
    int i;
    MacroParams *sourceParams = &shader_macros[index].params;
    int numParams = MIN(replaceParams->num_params, sourceParams->num_params);
    for (walk=shader_macros[index].body; *walk;)
    {
        char *next = walk+1;
        bool replaced=false;
        if (wordStart)
        {
            for (i=0; i<numParams && !replaced; i++) {
                if (strcmpalnum(walk, sourceParams->paramNames[i])==0)
                {
                    // Copy the replaced word, advance past end of source word
                    estrConcatCharString(dst, replaceParams->paramNames[i]);
                    next = walk + strlen(sourceParams->paramNames[i]);
                    replaced = true;
                }
            }
        }
        if (!replaced) {
            // Copy 1 character
            estrConcatChar(dst, *walk);
        }
        // Determine whether the next character could be the beginning of a symbol
        if (isvalidchar((unsigned char)*walk))
            wordStart = false;
        else
            wordStart = true;
        walk = next;
    }
}

static char *expandMacros(char *line)
{
    static char *buffer=NULL;
    char **ret=NULL;
    char *walk;
    bool wordStart=true;
    int i;
    if (!buffer)
        estrCreate(&buffer);
    estrClear(&buffer);
    for (walk=line; *walk && !ret; walk++)
    {
        if (wordStart) {
            for (i=0; i<shader_numMacros && !ret; i++)
            {
                if (strcmpalnum(walk, shader_macros[i].name)==0)
                {
                    char *s;
                    MacroParams params;
                    // Found this macro!
                    // Concat the part before the macro
                    ret = &buffer;
                    estrConcatFixedWidth(ret, line, walk - line);
                    // Concat the macro
                    // Parse the variable replacements
                    s = fillParams(walk + strlen(shader_macros[i].name), &params);
                    macroConcat(ret, i, &params);
                    // Concat the part after the macro
                    estrConcatCharString(ret, s);
                }
            }
        }
        // Determine whether the next character could be the beginning of a symbol
        if (isvalidchar((unsigned char)*walk))
            wordStart = false;
        else
            wordStart = true;
    }
    return ret?*ret:NULL;
}

static int shader_numDefines=0;
static char *shader_defines[64] = {0};
static void addDefine(char *define)
{
    int i;
    assert(shader_numDefines < ARRAY_SIZE(shader_defines));
    for ( i=0; i < shader_numDefines; i++ )
    {
        if ( strcmp( shader_defines[i], define ) == 0 )
        {
            // yeah, yeah, yeah. we know.
            return;
        }
    }
    shader_defines[shader_numDefines++] = strdup(define);
}

static bool hasDefine(char *define)
{
    int i;
    assert(shader_numDefines < ARRAY_SIZE(shader_defines));
    for ( i=0; i < shader_numDefines; i++ )
    {
        if ( strcmp( shader_defines[i], define ) == 0 )
        {
            return true;
        }
    }
    return false;
}

void removeDefine( char *define )
{
    int i;
    for ( i=0; i < shader_numDefines; i++ )
    {
        if ( strcmp( shader_defines[i], define ) == 0 )
        {
            free( shader_defines[i] );
            while ( i < (shader_numDefines-1) )
            {
                shader_defines[i] = shader_defines[i+1];
            }
            shader_numDefines--;    
            return;
        }
    }
}

static void addDefineSet(char *defineSet)
{
    // Handles a space-separated list
    char define[128];
    char* p = defineSet;    
    while ( true )
    {
        int nIndex = 0;
        while (*p == ' ' ) p++;
        if ( *p == '\0' )
        {
            break;
        }
        while (( nIndex < ARRAY_SIZE(define)) &&
                ( p[nIndex] != ' ' ) && ( p[nIndex] != '\0' ))
        {
            define[nIndex] = p[nIndex];
            nIndex++;
        }
        define[nIndex] = '\0';
        addDefine( define );

        p += nIndex;
    }
}

static void setShaderOptimizationLevel( tShaderBuildDescriptor* pBuildDescriptor, tShaderOptimizationLevel nLevel )
{
#if RT_SUPPORT_ARB_SHADER_PATH
    if (rt_cgGetCgShaderMode())
#endif
    {
        char defineStr[16];    
        sprintf_s( defineStr, ARRAY_SIZE(defineStr), "-O%d", nLevel );        
        addStringToCmdLineArgs( pBuildDescriptor, defineStr );
    }
}

static int meetsSingleCondition(char *condition)
{
    static int numchips=0;
    static char *chips[32] = {0};    // make sure large enough to accommodate all flags
    int i=0;

    if (numchips==0) { // init
        // walk over the rdr_caps.chips flags and add any
        // flag names to a string array to test against condition
        for (i=0;i<32; i++) {
            if ((1<<i) & rdr_caps.chip) {
                char *chipname = rdrChipToString(1 << i);
                chips[numchips] = chipname;
                numchips++;
            }
        }
    }
    onlydevassert(numchips);

    // Constants
    if (stricmp(condition, "true")==0 ||
        stricmp(condition, "1")==0)
    {
        return 1;
    }

    // Chip types
    for (i=0; i<numchips; i++) {
        if (stricmp(condition, chips[i])==0)
            return 1;
    }

    // Defines
    for (i=0; i<shader_numDefines; i++) {
        if (stricmp(condition, shader_defines[i])==0)
            return 1;
    }

    // /shadertest variables
    {
        const char* defines[32];
        int nCount = getShaderDebugSpecialDefines( defines, ARRAY_SIZE(defines) );
        for (i=0; i < nCount; i++) {
            if (stricmp(condition, defines[i])==0)
                return 1;
        }
    }

    // Special defines
    if (!(rdr_caps.features & GFXF_FPRENDER)) {
        if (stricmp(condition, "NO_FPRENDER")==0)
            return 1;
    }

    return 0;
}

int meetsCondition(char *condition)
{
    char *last=NULL;
    char *tok;
#define CONDITIONDELIMS "\t |"
    tok = strtok_r(condition, CONDITIONDELIMS, &last);
    while (tok) {
        if (meetsSingleCondition(tok))
            return 1;
        tok = strtok_r(NULL, CONDITIONDELIMS, &last);
    }
    return 0;
}

static char *workingBase=NULL;
static char *origBase;
static char *defines_string = 0;

static char *initPreProcess(char *data)
{
    if (data) {
        workingBase=strdup(data);
        origBase = data;
        return workingBase;
    }
    return NULL;
}

static void resetPreprocess(void)
{
    int i;
    if (defines_string)
        estrDestroy(&defines_string);
    estrCreate(&defines_string);
    for (i=0; i<shader_numDefines; i++) {
        estrConcatCharString(&defines_string, shader_defines[i]);
        estrConcatChar(&defines_string, ' ');
        SAFE_FREE(shader_defines[i]);
    }
    for (i=0; i<shader_numMacros; i++) {
        estrDestroy(&shader_macros[i].body);
    }
    shader_numDefines = 0;
    shader_numMacros = 0;
}

static void finishPreProcess(char *data)
{
    free(data);
    workingBase=NULL;
    origBase=NULL;
}

void commentLine(char *linestart)
{
    char *origLineStart = origBase + (linestart - workingBase);
    if (*origLineStart!='\r' && *origLineStart!='\n')
        *origLineStart = '#';
}

void shaderPreProcessIfDefs(char *data, int ignore, char *filename)
{
    char *working=initPreProcess(data);
#define DELIMS "\r\n"
#define TOK(x) (stricmp(tok, x)==0)
    char *line;

    line = strtok(working, DELIMS);
    while (line) {
        char *s;
        s = line;
        while (*s==' ' || *s=='\t') s++;
        if (*s=='#') {
            char *last=NULL;
            char *tok;
            //printf("%s\n", line);
            s++;
            // Comment or preprocessor directive
            tok = strtok_r(s, " \t", &last);
            if (!tok) {
                // nothing
            } else if (TOK("ifdef")) {
                if (ignore) {
                    // Nested if
                    shaderPreProcessIfDefs(NULL, 2, filename);
                } else {
                    if (meetsCondition(last)) {
                        // doit
                        shaderPreProcessIfDefs(NULL, 0, filename);
                    } else {
                        // don't do it!
                        shaderPreProcessIfDefs(NULL, 1, filename);
                    }
                }
            } else if (TOK("ifndef")) {
                if (ignore) {
                    // Nested if
                    shaderPreProcessIfDefs(NULL, 2, filename);
                } else {
                    if (meetsCondition(last)) {
                        // don't do it!
                        shaderPreProcessIfDefs(NULL, 1, filename);
                    } else {
                        // doit
                        shaderPreProcessIfDefs(NULL, 0, filename);
                    }
                }
            } else if (TOK("else")) {
                if (ignore==0) {
                    // All after this will not do anything
                    ignore = 1;
                } else if (ignore==1) {
                    // We failed the first clause of the if, do this one!
                    ignore = 0;
                }
            } else if (TOK("elseifdef") || TOK("elifdef") || TOK("elif") || TOK("elseif")) {
                if (ignore==0) {
                    // All elseifdefs after this will not do anything
                    ignore = 2;
                } else if (ignore==1) {
                    // We failed the first clause of the if, do this one if applicable
                    if (meetsCondition(last)) {
                        // doit
                        ignore = 0;
                    } else {
                        // don't do it!
                        ignore = 1;
                    }
                } else if (ignore==2) {
                    // continue ignoring
                }
            } else if (TOK("endif")) {
                if (working)
                    ErrorFilenamef(filename, "#endif without matching #ifdef");
                return;
            } else if (TOK("define")) {
                if (!ignore)
                    addDefine(last);
            }
        } else {
            if (ignore) {
                // ignore it
                commentLine(line);
                //printf("-%s\n", line);
            } else {
                // Just a line, leave it be
                //printf("+%s\n", line);
            }
        }

        line = strtok(NULL, DELIMS);
    }

    if (working) {
        finishPreProcess(working);
    }
#undef DELIMS
#undef TOK
}

void shaderPreProcessMacros(char **data)
{
    char *working = initPreProcess(*data);
    static char *ret=NULL;
    char *line;
    bool didmacro=false;
    bool inmacro=false;
    char *macroName=NULL;
    char *macroBody=NULL;
#define DELIMS "\r\n"
#define TOK(x) (tok && stricmp(tok, x)==0)
#define CRLF "\r\n"

    if (!ret)
        estrCreate(&ret);
    estrClear(&ret);

    line = strtok(working, DELIMS);
    while (line) {
        char *s;
        s = line;
        while (*s==' ' || *s=='\t') s++;
        if (*s=='#') {
            char *last=NULL;
            char *tok;
            //printf("%s\n", line);
            s++;
            // Comment or preprocessor directive
            tok = strtok_r(s, " \t", &last);
            if (TOK("macro")) {
                didmacro = true;
                assert(!inmacro);
                if (!inmacro) {
                    macroName = last;
                    inmacro = true;
                    estrCreate(&macroBody);
                }
            } else if (TOK("endmacro")) {
                assert(inmacro);
                if (inmacro) {
                    inmacro = false;
                    // finish up the macro
                    if (strEndsWith(macroBody, CRLF)) {
                        estrSetLength(&macroBody, strlen(macroBody)-strlen(CRLF));
                    }
                    addMacro(macroName, macroBody);
                    macroBody = NULL;
                }
            } else {
                estrConcatCharString(&ret, line);
                if (last) {
                    estrConcatChar(&ret, ' ');
                    estrConcatCharString(&ret, last);
                }
                estrConcatStaticCharArray(&ret, CRLF);
                if (inmacro) {
                    estrConcatCharString(&macroBody, line);
                    if (last) {
                        estrConcatChar(&macroBody, ' ');
                        estrConcatCharString(&macroBody, last);
                    }
                    estrConcatStaticCharArray(&macroBody, CRLF);
                }
            }
        } else {
            char *expanded=NULL;
            if (inmacro) {
                estrConcatCharString(&macroBody, line);
                estrConcatStaticCharArray(&macroBody, CRLF);
            } else {
                // non-comment, search for macros
                expanded = expandMacros(line);
                if (expanded) {
                    didmacro = true;
                    estrConcatCharString(&ret, expanded);
                    //estrDestroy(&expanded);
                    estrConcatStaticCharArray(&ret, CRLF);
                } else {
                    estrConcatCharString(&ret, line);
                    estrConcatStaticCharArray(&ret, CRLF);
                }
            }
        }

        line = strtok(NULL, DELIMS);
    }

    finishPreProcess(working);
    if (didmacro) {
        // Replace the string!
        SAFE_FREE(*data);
        *data = strdup(ret);
    }
//    estrDestroy(&ret);
//    printf("New:\n%s\n", *data);
}

void shaderPreProcessIncludes(char **data, char *path, char *sourcefilename)
{
    char *working = initPreProcess(*data);
    char *ret=NULL;
    char *line;
    bool foundinclude = false;
#define DELIMS "\r\n"
#define TOK(x) (tok && stricmp(tok, x)==0)
#define CRLF "\r\n"

    estrCreate(&ret);

    line = strtok(working, DELIMS);
    while (line) {
        char *s;
        s = line;
        while (*s==' ' || *s=='\t') s++;
        if (*s=='#')
        {
            char *last=NULL;
            char *tok;
            //printf("%s\n", line);
            s++;
            // Comment or preprocessor directive
            tok = strtok_r(s, " \t", &last);
            if (TOK("include"))
            {
                if (last)
                {
                    char filename[MAX_PATH] = {0};
                    char *includeText;
                    foundinclude = true;
                    if (last[0] == '\"')
                    {
                        char *stemp;
                        last++;
                        stemp = strrchr(last, '\"');
                        if (stemp && stemp >= last)
                            *stemp = 0;
                    }

                    if (!strchr(last, '/') && !strchr(last, '\\'))
                        sprintf(filename, "%s/%s", path, last);
                    else
                        sprintf(filename, "%s", last);
                    includeText = fileAlloc(filename, 0);
                    if (includeText)
                    {
                        estrConcatCharString(&ret, includeText);
                        estrConcatStaticCharArray(&ret, CRLF);
                        fileFree(includeText);
                    }
                    else
                    {
                        ErrorFilenamef(sourcefilename, "Include file not found: \"%s\"", filename);
                    }
                }
            }
            else
            {
                estrConcatCharString(&ret, line);
                if (last)
                {
                    estrConcatChar(&ret, ' ');
                    estrConcatCharString(&ret, last);
                }
                estrConcatStaticCharArray(&ret, CRLF);
            }
        }
        else
        {
            // non-include
            estrConcatCharString(&ret, line);
            estrConcatStaticCharArray(&ret, CRLF);
        }

        line = strtok(NULL, DELIMS);
    }

    finishPreProcess(working);
    if (foundinclude) {
        // Replace the string!
        SAFE_FREE(*data);
        *data = strdup(ret);
    }
    estrDestroy(&ret);
    //    printf("New:\n%s\n", *data);
}

int shader_perf_test_hack=0;

char *loadDummyProgram(int numInstructions)
{
    static char *lines[] = {
        "LRP a.x, a.y, a, 1;",
        "LRP a.y, a.z, a, 1;",
        "LRP a.z, a.w, a, 1;",
        "LRP a.w, a.x, a, 1;",
    };
    static char *prog=NULL;
    estrClear(&prog);
    estrConcatf(&prog, "%s\n", "!!ARBfp1.0");
    estrConcatf(&prog, "%s\n", "PARAM const0 = program.env[0];");
    estrConcatf(&prog, "%s\n", "TEMP a;");
    estrConcatf(&prog, "%s\n", "MOV a, const0;");
    numInstructions -= 3;
    while (numInstructions > 0) {
        estrConcatf(&prog, "%s\n", lines[numInstructions%ARRAY_SIZE(lines)]);
        numInstructions--;
    }
    estrConcatf(&prog, "%s\n", "MOV result.color.xyz, a;");
    estrConcatf(&prog, "%s\n", "MOV result.color.w, 1;");
    estrConcatf(&prog, "%s\n", "END");
    return strdup(prog);
}

static bool lpc_enabled=false;
static StashTable lpc_table;
static void freeFunc(char *str)
{
    free(str);
}
static void freeFuncAdapter(void* arg0)
{
    freeFunc((char *)arg0);
}

static void loadProgramCacheReset(bool shouldDoCaching)
{
    lpc_enabled = shouldDoCaching;
    if (lpc_enabled) {
        if (lpc_table) {
            stashTableClearEx(lpc_table, NULL, freeFuncAdapter);
        } else {
            lpc_table = stashTableCreateWithStringKeys(16, StashDefault|StashDeepCopyKeys);
        }
    } else {
        if (lpc_table) {
            stashTableDestroyEx(lpc_table, NULL, freeFuncAdapter);
            lpc_table = NULL;
        }
    }
}

static char *loadProgramCacheFind(const char *filename) // Returns duplicate copy
{
    const char *orig_string;
    if (!lpc_enabled)
        return NULL;
    if (stashFindPointerConst(lpc_table, filename, &orig_string)) {
        return strdup(orig_string);
    }
    return NULL;
}
static void loadProgramCacheStore(const char *filename, const char *programText)
{
    if (!lpc_enabled)
        return;
    assert(!loadProgramCacheFind(filename));
    stashAddPointer(lpc_table, filename, strdup(programText), true);
}

static StashTable lpc_crctable;
static bool loadProgramCacheChanged(tShaderProgramType target, GLuint programHandle, const char *programText)
{
    int key=0;
    U32 crc=0, cached_crc=0;
    int i;
    if (!lpc_crctable) {
        lpc_crctable = stashTableCreateInt(64);
    }
    switch(target) {
        xcase kShaderPgmType_FRAGMENT:
            key = 0x10000000;
        xcase kShaderPgmType_VERTEX:
            key = 0x20000000;
        xdefault:
            assert(0);
    }

    key |= programHandle;
    cryptAdler32Init();
    cryptAdler32Update(programText, strlen(programText));
    // Hash defines
    for (i=0; i<shader_numDefines; i++) {
        cryptAdler32Update(shader_defines[i], strlen(shader_defines[i]));
    }
    // Hash special defines
    {
        const char* specialDefines[32];
        int numDefines = getShaderDebugSpecialDefines(specialDefines,ARRAY_SIZE(specialDefines));
        for (i=0; i < numDefines; i++) {
            cryptAdler32Update(specialDefines[i],strlen(specialDefines[i]));
        }
    }
    if (!(rdr_caps.features & GFXF_FPRENDER)) {
        cryptAdler32Update("NO_FPRENDER", strlen("NO_FPRENDER"));
    }
    crc = cryptAdler32Final();
    if (stashIntFindInt(lpc_crctable, key, &cached_crc)) {
        if (crc == cached_crc)
            return false;
    }
    stashIntAddInt(lpc_crctable, key, crc, true);
    return true;
}

void shaderMgr_GetShaderSrcFileTextCG(const char* cohRelativePathNameFS, char** ppProgramText )
{
    char fullPath[2048];
    makeCgShaderSrcFullPathFS( cohRelativePathNameFS, fullPath, ARRAY_SIZE(fullPath) );
    getShaderProgramFileTextCG( fullPath, ppProgramText, NULL );
}

void shaderMgr_MakeFullShaderSrcPath( const char* cohRelativePathNameFS, char* buffer, int buffSz )
{
    makeCgShaderSrcFullPathFS( cohRelativePathNameFS, buffer, buffSz );
}

static tShaderPathRootType makeCgShaderSrcFullPathFS( const char* cohRelativePathNameFS, char* buffer, int buffSz )
{
    tShaderPathRootType foundType = kShaderPathRoot_NONE;
    tShaderPathRootType pathTypeList[4];
    bool bCheckFileExistence = true;
    int typeI=0;
    
    // NOTE that the last item in the array (before 'NONE') serves as the default.
    if ( sShaderDirectory[kPathRoot_Custom][0] != '\0' )
    {
        pathTypeList[0] = kPathRoot_Custom;
        pathTypeList[1] = kPathRoot_GameData;
        pathTypeList[2] = kShaderPathRoot_NONE;    // list terminator
    }
    else
    {
        pathTypeList[0] = kPathRoot_GameData;
        pathTypeList[1] = kShaderPathRoot_NONE;    // list terminator
    }
    assert(( cohRelativePathNameFS != NULL ) && ( *cohRelativePathNameFS != '\0' ));
    
    // Check for the file before determining the path?
    if (( strEndsWith( cohRelativePathNameFS, "/" )) || ( strEndsWith( cohRelativePathNameFS, "\\" ) ))
    {
        bCheckFileExistence = false;
    }

    while ( pathTypeList[typeI] != kShaderPathRoot_NONE )
    {
        foundType = pathTypeList[typeI];
        sprintf_s( buffer, buffSz, "%s%s", getShaderFileBaseDir(pathTypeList[typeI]), cohRelativePathNameFS );
        if (( ! bCheckFileExistence ) || ( fileExists(buffer) ))
        {
            foundType = pathTypeList[typeI];
            break;
        }
        typeI++;
    }
    
    forwardSlashes(buffer);
    
    // We may exit the loop without having found a file. If so, the last item in the
    // list determines the place where the file should have been.
    return foundType;
}

static void getShaderProgramFileTextCG( const char* shaderSrcFullPath, char** ppProgramText, int* pSourceTextLen )
{
    // Load the program into memory
    PERFINFO_AUTO_START("fileAlloc", 1);
    if ( pSourceTextLen )
    {
        *pSourceTextLen = fileSize( shaderSrcFullPath );
    }
    *ppProgramText = fileAlloc( shaderSrcFullPath, 0 );
    PERFINFO_AUTO_STOP();
}

#if RT_SUPPORT_ARB_SHADER_PATH
static void getShaderProgramFileTextARB_OBSOLETE( const char* filename, char** ppProgramText, bool* pFromCache /* or NULL */)
{
    bool bFromCache = false;
    // Look for already pre-processed version in cache
    PERFINFO_AUTO_START("loadProgramCacheFind",1);
    *ppProgramText = loadProgramCacheFind(filename); // Returned duplicate copy
    PERFINFO_AUTO_STOP();
    if (*ppProgramText) {
        bFromCache = true;
    } else {
        char fullPath[_MAX_PATH+1];
        snprintf( fullPath, ARRAY_SIZE(fullPath), "%s%s", getCurrentShaderFileBaseDirARB_OBSOLETE(), filename );
        bFromCache = false;
        // Load the program into memory
        PERFINFO_AUTO_START("fileAlloc", 1);
        *ppProgramText = fileAlloc(fullPath, 0);
        PERFINFO_AUTO_STOP();
    }

    if ( pFromCache )
    {
        *pFromCache = bFromCache;
    }
}
#endif

void buildShaderDebugFileName( char* filename, char* debug_fn, size_t debug_fn_sz )
{
    int i;
    char ext[16]="";
    strcpy(ext, strrchr(filename, '.'));
    strcpy(debug_fn, "./shaders_processed");
    strcat(debug_fn, strrchr(filename, '/'));
    *strrchr(debug_fn, '.')=0;
    for (i=0; i<shader_numDefines; i++)
        strcatf(debug_fn, "_%s", shader_defines[i]);
    strcat(debug_fn, ext);
}

void writeProgramTextToDebugFile(char* filename, char* debug_fn, char* programText )
{
    FILE *f;
    mkdirtree(debug_fn);
    _chmod(debug_fn, _S_IREAD | _S_IWRITE);
    f = fileOpen(debug_fn, "w");
    fwrite(programText, 1, strlen(programText), f);
    fclose(f);
}

void runNvidiaPerfText(char* debug_fn)
{
    char cmdline[MAX_PATH];
    char *data, *s;
    static char *output_file = "c:\\temp\\nvshaderperf.txt";
    sprintf(cmdline, "\"C:\\Program Files\\NVIDIA Corporation\\NVIDIA NVShaderPerf\\nvshaderperf.exe\" %s > %s", debug_fn, output_file);
    system(cmdline);
    data = fileAlloc(output_file, NULL);
    s = strstri(data, "Cycles:");
    if (s) {
        float cycles;
        sscanf(s, "Cycles: %f", &cycles);
        OutputDebugStringf("%1.1f cycles: %s\r\n", cycles, debug_fn);
    }
    fileFree(data);
}

void outputPerfInfo(tShaderProgramType target, char* debug_fn)
{
    int i, iv;
    struct {
        int glid;
        char *name;
    } fields[] = {
        {GL_PROGRAM_INSTRUCTIONS_ARB, "Ins"},
        {GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB, "NatIns"},
        {GL_PROGRAM_TEMPORARIES_ARB, "ProgTemp"},
        {GL_PROGRAM_NATIVE_TEMPORARIES_ARB, "NatTemp"},
        {GL_PROGRAM_ALU_INSTRUCTIONS_ARB, "ALU"},
        {GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, "NatALU"},
        {GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, "NatTEX"},
    };
    GLenum progCode = ( target == kShaderPgmType_FRAGMENT ) ? GL_FRAGMENT_PROGRAM_ARB : GL_VERTEX_PROGRAM_ARB;
    if (!(progCode == GL_FRAGMENT_PROGRAM_ARB)) {
        OutputDebugStringf("%s\r\n", debug_fn);
    }
    OutputDebugStringf("     ");
    for (i=0; i<ARRAY_SIZE(fields); i++) {
        glGetProgramivARB(progCode, fields[i].glid, &iv); CHECKGL;
        OutputDebugStringf("%s:%d ", fields[i].name, iv);
    }
    OutputDebugStringf("\r\n");
}

GLenum handleShaderCompileErrors( tShaderProgramType target, GLuint programHandle, GLenum result, char* filename, char* programText )
{
    const char * errstring;
    extern void status_printf(char const *fmt, ...);
    PERFINFO_AUTO_START("Error", 1);
    if (result == GL_INVALID_ENUM && target == kShaderPgmType_FRAGMENT) {
        // GL_FRAGMENT_PROGRAM_ARB is not supported
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
        return GL_NO_ERROR;
    }

    if (strStartsWith(filename, "shaders/arb/error.") || strStartsWith(filename, RT_CGFX_SHADER_PATH_BASE "/error")) {
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
        return GL_NO_ERROR;
    }
    //FatalErrorf("Compilation of \"%s\" failed\n", filename);        
    consoleSetColor(COLOR_RED|COLOR_BRIGHT, 0);
    printf("Shader Compilation failure in %s\r\n", filename);
    if (defines_string)
        printf("Defines: '%s'\n", defines_string);
    //OutputDebugString(glGetString(GL_PROGRAM_FORMAT_ASCII_ARB));
    errstring = glGetString(GL_PROGRAM_ERROR_STRING_ARB); CHECKGL;
    if (errstring) {
        int line=0, column=0;
        printf("%s", errstring);
        if (rdr_caps.chip & R200)
            printf("\r\n");
        sscanf(errstring, "line %d, column %d", &line, &column);
        if (line) {
            char *ptr=programText;
            line--;
            while (line && ptr) {
                line--;
                ptr = strchr(ptr, '\n');
                if (ptr)
                    ptr++;
            }
            if (ptr) {
                char buf[1024];
                char *eol = strchr(ptr, '\n');
                if (!eol)
                    eol = ptr + strlen(ptr);
                if (eol==ptr) {
                    buf[0]=0;
                } else {
                    strncpyt(buf, ptr, MIN(eol - ptr, ARRAY_SIZE(buf)));
                }
                printf("%s\r\n", buf);
                for (column; column>=0; column--)
                    printf(" ");
                printf(" ^");
            }
        }
    }
    printf("\r\n");
    consoleSetDefaultColor();
    if (isDevelopmentMode())
        printf("Shader compilation failed\n");

    if(target == kShaderPgmType_FRAGMENT)
    {
        loadProgram(( rt_cgGetCgShaderMode() ? kCompileFlag_Cg : kCompileFlag_ARB ),
            (( rt_cgGetCgShaderMode() ) ? RT_CGFX_SHADER_PATH_BASE "/errorfp.cg" : "shaders/arb/error.fp" ),
            kShaderPgmType_FRAGMENT,
            (( rt_cgGetCgShaderMode() ) ? 0 : GL_FRAGMENT_PROGRAM_ARB ), 
            programHandle);
    }
    else if(target == kShaderPgmType_VERTEX)
    {
        loadProgram((rt_cgGetCgShaderMode() ? kCompileFlag_Cg : kCompileFlag_ARB ), 
            (( rt_cgGetCgShaderMode() ) ? RT_CGFX_SHADER_PATH_BASE "/errorvp.cg" : "shaders/arb/error.vp"),
            kShaderPgmType_VERTEX, 
            (( rt_cgGetCgShaderMode() ) ? 0 : GL_VERTEX_PROGRAM_ARB ), 
            programHandle);
    }
    PERFINFO_AUTO_STOP();
    return result;
}

static CGprogram loadCgErrorShader( tShaderProgramType target, U32 compileFlags, char** pProgramText  )
{
    CGprogram pgmHandle = NULL;
    char* szErrorShader;
    tCgShaderProfile profile;
    tShaderBuildDescriptor buildDescriptor = { 0 };
    if ( target == kShaderPgmType_FRAGMENT )
    {
        szErrorShader = RT_CGFX_SHADER_PATH_BASE "/errorfp.cg";
        profile = rt_cgGetCgShaderProfileForMode(rt_cgGetCgShaderMode(), kShaderPgmType_FRAGMENT);
    }
    else
    {
        szErrorShader = RT_CGFX_SHADER_PATH_BASE "/errorvp.cg";
        profile = rt_cgGetCgShaderProfileForMode(rt_cgGetCgShaderMode(), kShaderPgmType_VERTEX);
    }
    initShaderBuildDescriptor( 0, szErrorShader, target, profile, compileFlags, RT_CGFX_SHADER_PATH_BASE, kPathRoot_GameData, &buildDescriptor );
    pgmHandle = prepareCgProgram( target, profile, compileFlags, szErrorShader, &buildDescriptor, pProgramText );
    releaseShaderBuildDescriptorMem( &buildDescriptor );
    
    return pgmHandle;
}

// This loads a program and compiles it, replaces ATI/NV/FP specific lines where necessary
// returns true on success
static bool loadProgram(U32 compileFlags, char* filename, tShaderProgramType target,
                    tCgShaderProfile profile, 
                    GLuint programHandle)
{
    #if RT_SUPPORT_ARB_SHADER_PATH
        bool bIsCG = (( compileFlags & kCompileFlag_Cg ) == kCompileFlag_Cg );
        if ( bIsCG )
        {
    #endif
            return loadProgramCG( compileFlags, filename, target, profile, programHandle );
    #if RT_SUPPORT_ARB_SHADER_PATH
        }
        else
        {
            return loadProgramARB_OBSOLETE( compileFlags, filename, target, profile, programHandle );
        }
    #endif
}

// returns true on success
static bool loadProgramCG(U32 compileFlags, char* cohRelativeSrcPath, tShaderProgramType target,
                        tCgShaderProfile profile, GLuint programHandle)
{
    extern char reloading_shader[MAX_PATH];
    char* compilegPgmText    = NULL;
    GLenum result            = GL_NO_ERROR;
    bool perfThisShader        = game_state.nvPerfShaders && (!reloading_shader[0] || 0==stricmp(reloading_shader+1, cohRelativeSrcPath));
    CGprogram hCgPgm        = NULL;
    tShaderBuildDescriptor shaderBuildDescriptor = { 0 };
    char shaderDir[2048];
    char shaderFullPath[2048];
    tShaderPathRootType rootType;

    PERFINFO_AUTO_START("loadProgramCG", 1);
    assert(cohRelativeSrcPath != NULL);
    ValidateShaderProgramTypeConstant( target );
    assert(programHandle > 0);

    // Figure out the real profile
    if(!profile)
    {
        profile = rt_cgGetCgShaderProfileForMode(rt_cgGetCgShaderMode(), target);
    }

    {
        // Get the shader's directory without the file name
        char *pName;
        strncpy_s(shaderDir, ARRAY_SIZE(shaderDir),cohRelativeSrcPath,_TRUNCATE);
        forwardSlashes(shaderDir);
        pName = strrchr(shaderDir, '/');
        if (pName) *pName = '\0';
    }
    
    // Determine the specifics needed to compile this shader
    rootType = makeCgShaderSrcFullPathFS( cohRelativeSrcPath, shaderFullPath, ARRAY_SIZE(shaderFullPath) );
    initShaderBuildDescriptor( programHandle, cohRelativeSrcPath, target, profile, compileFlags, shaderDir, rootType, &shaderBuildDescriptor );
    hCgPgm = prepareCgProgram( target, profile, compileFlags, cohRelativeSrcPath, &shaderBuildDescriptor, &compilegPgmText );
    if ( hCgPgm == NULL )
    {
        GLenum err = cgGetError();    // suck up the error.
        hCgPgm = loadCgErrorShader( target, compileFlags, &compilegPgmText );
        result = ~GL_NO_ERROR;
    }
    
    if(result == GL_NO_ERROR)
    {
        if (game_state.writeProcessedShaders || perfThisShader) 
        {
            char debug_fn[MAX_PATH];
            PERFINFO_AUTO_START("writeProcessedShaders", 1);
            buildShaderDebugFileName( cohRelativeSrcPath, debug_fn, ARRAY_SIZE(debug_fn));
            writeProgramTextToDebugFile( cohRelativeSrcPath, debug_fn, compilegPgmText );
            if (perfThisShader && target == kShaderPgmType_FRAGMENT) {
                runNvidiaPerfText(debug_fn);
            }
            PERFINFO_AUTO_STOP();
        }
        resetPreprocess();
        PERFINFO_AUTO_START("cgPrepareAndCacheProgramHdl", 1);
        {
            const char* szPgmName = strrchr( cohRelativeSrcPath, '/' );    // get the file name without any paths
            szPgmName = ( szPgmName == NULL ) ? cohRelativeSrcPath : ( szPgmName + 1 );
            ValidateShaderProgramTypeConstant( target );
            rt_cgPrepareAndCacheProgramHdl( target, profile, programHandle, hCgPgm, szPgmName );
        }
        PERFINFO_AUTO_STOP();
    }

    if(result != GL_NO_ERROR)
    {
        char* sourceProgramText = NULL;
        getShaderProgramFileTextCG( shaderFullPath, &sourceProgramText, NULL );
        result = handleShaderCompileErrors(target,programHandle,result,cohRelativeSrcPath,sourceProgramText);
        fileFree(sourceProgramText);
    }

    // Clean up
    if ( compilegPgmText )
    {
        free(compilegPgmText); 
    }
    releaseShaderBuildDescriptorMem( &shaderBuildDescriptor );
    
    PERFINFO_AUTO_STOP();
    return result == GL_NO_ERROR;
}

// This loads a program and compiles it, replaces ATI/NV/FP specific lines where necessary
// returns true on success
// The ARB shader path is DEPRECATED
#if RT_SUPPORT_ARB_SHADER_PATH
static bool loadProgramARB_OBSOLETE(U32 compileFlags, char* filename, tShaderProgramType target, tCgShaderProfile profile, GLuint programHandle)
{
    extern char reloading_shader[MAX_PATH];
    char*  programText;
    GLenum result = GL_NO_ERROR;
    bool perfThisShader = game_state.nvPerfShaders && (!reloading_shader[0] || 0==stricmp(reloading_shader+1, filename));
    bool from_cache = false;
    GLenum nArbShaderProfile = profile; //    GL_FRAGMENT_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB
        
    PERFINFO_AUTO_START("loadProgram", 1);

    assert(filename != NULL);
    ValidateShaderProgramTypeConstant( target );
    assert(programHandle > 0);

    if (shader_perf_test_hack && (target == kShaderPgmType_FRAGMENT)) {
        programText = loadDummyProgram(shader_perf_test_hack);
    } else {
        getShaderProgramFileTextARB_OBSOLETE( filename, &programText, &from_cache );
    }
    
    if(programText == NULL)
    {
        Errorf("Error loading %s", filename);
        result = !GL_NO_ERROR;
    }
    
    if ( result == GL_NO_ERROR )
    {
        bool skip_rest = false;
        char debug_fn[MAX_PATH];
        if (!from_cache)
        {
            char *stemp;
            char path[MAX_PATH];
            PERFINFO_AUTO_START("prepareARBProgram", 1);
            strcpy(path, filename);
            forwardSlashes(path);
            stemp = strrchr(path, '/');
            if (stemp)
                *stemp = 0;

            prepareARBProgram( target, compileFlags, filename, path, &programText );
            PERFINFO_AUTO_STOP();
        }
        // Do this after caching
        PERFINFO_AUTO_START("loadProgramCacheChanged", 1);
        // LSTL - disabling this because it causes the shaders to fail when
        // we switch from Cg to ARB mode and back.
        #if 0
        if ( !loadProgramCacheChanged(target, programHandle, programText)) {
            // The source+defines have not changed, so the result will
            // be the same, skip the rest of this and the sending to GL!
            skip_rest = true;
            result = GL_NO_ERROR;
        }
        #endif
        PERFINFO_AUTO_STOP();
        if ( !skip_rest ) {
            PERFINFO_AUTO_START("shaderPreProcessIfDefs", 1);
            shaderPreProcessIfDefs(programText, 0, filename);
            PERFINFO_AUTO_STOP();
        }
        PERFINFO_AUTO_START("resetPreprocess", 1);
        resetPreprocess();
        if (game_state.writeProcessedShaders || perfThisShader) {
            buildShaderDebugFileName( filename, debug_fn, ARRAY_SIZE(debug_fn));
            writeProgramTextToDebugFile( filename, debug_fn, programText );
            if (perfThisShader && target == kShaderPgmType_FRAGMENT) {
                runNvidiaPerfText(debug_fn);
            }
        }
        PERFINFO_AUTO_STOP();

        {
            PERFINFO_AUTO_START("gl", 1);
            {
                // Bind the program object
                glBindProgramARB(nArbShaderProfile, programHandle); CHECKGL;

                // Compile the program
                glProgramStringARB(nArbShaderProfile, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)strlen(programText), programText);
                // Check to see if the program compiled
                result = gldGetError();
                if(result != GL_NO_ERROR) {
                    // Sometimes it fails the first time on ATI?
                    // Compile the program
                    glProgramStringARB(nArbShaderProfile, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)strlen(programText), programText);
                    // Check to see if the program compiled
                    result = gldGetError();
                }
                if (perfThisShader) {
                    outputPerfInfo(target, debug_fn);
                }
            }
            PERFINFO_AUTO_STOP();
        }
    }

    if(result != GL_NO_ERROR)
    {
        result = handleShaderCompileErrors(target,programHandle,result,filename,programText);
    }

    // Clean up
    fileFree(programText);
    PERFINFO_AUTO_STOP();
    return result == GL_NO_ERROR;
}
#endif

static void prepareARBProgram( tShaderProgramType target, U32 compileFlags, char* filename, char* path, char** pProgramText )
{
    shaderPreProcessIncludes(pProgramText, path, filename);
    PERFINFO_AUTO_STOP_START("shaderPreProcessMacros", 1);
    shaderPreProcessMacros(pProgramText);
    PERFINFO_AUTO_STOP_START("loadProgramCacheStore", 1);
    loadProgramCacheStore(filename, *pProgramText);
    PERFINFO_AUTO_STOP();
}

static void prepareShaderProfileOption(tCgShaderProfile profile,
                                tShaderBuildDescriptor* pBuildDescriptor )
{
    const char* szProfileDescr = "";
    char extraOptsForCustomProfile[256];
    
    if ( rt_cgfxGetProfileInfo( 
                profile,
                &szProfileDescr,
                extraOptsForCustomProfile, ARRAY_SIZE(extraOptsForCustomProfile) ) )
    {
        char handleStr[16];
        sprintf_s( handleStr, ARRAY_SIZE(handleStr), "%u", pBuildDescriptor->programHdl );
    
        addStringToBuildHints( pBuildDescriptor, "HANDLE", handleStr );
        addStringToBuildHints( pBuildDescriptor, "PROFILE", szProfileDescr );
        if ( *extraOptsForCustomProfile != '\0' )
        {
            bool bMore = true;
            char* pArg = extraOptsForCustomProfile;
            char* p;
            // Process the tab-separated list
            while ( bMore )
            {
                while ( *pArg == ' ' ) pArg++;
                p = pArg;
                while (( *p != '\0' ) && ( *p != '\t' ))
                {
                    p++;
                }
                if ( *p == '\0' ) bMore = false;
                *p = '\0';
                addStringToCmdLineArgs( pBuildDescriptor, pArg );
                pArg = (p + 1);
            }
        }
    }
}

static CGprogram prepareCgProgram(
                tShaderProgramType target,
                tCgShaderProfile profile,
                U32 compileFlags,
                const char* cohRelativeSrcFilePathFS,
                tShaderBuildDescriptor* pBuildDescriptor,    // see initShaderBuildDescriptor()
                char** pCompiledPgmText        // OUT (compiled pgm)
            )
{
    const char* szPgmName = NULL;
    CGprogram program;
    char* cgSourceText = NULL;
    char* altSourceText = NULL;
    char shaderFileFullPath[2048];
    
    makeCgShaderSrcFullPathFS( cohRelativeSrcFilePathFS, shaderFileFullPath, ARRAY_SIZE(shaderFileFullPath) );
    
    szPgmName = strrchr( cohRelativeSrcFilePathFS, '/' );    // get the file name without any paths
    szPgmName = ( szPgmName == NULL ) ? cohRelativeSrcFilePathFS : ( szPgmName + 1 );

    if (shader_perf_test_hack && (target == kShaderPgmType_FRAGMENT)) 
    {
        altSourceText = loadDummyProgram(shader_perf_test_hack);
        cgSourceText = altSourceText;
    }
    else
    {
        int cgSourceTextSz;
        getShaderProgramFileTextCG( shaderFileFullPath, &cgSourceText, &cgSourceTextSz );
        if ( cgSourceText == NULL)
        {
            Errorf("Error loading %s", cohRelativeSrcFilePathFS);
            return NULL;
        }
        else
        {
            updateBuildDescriptorContentMd5( pBuildDescriptor, cgSourceText, cgSourceTextSz );
        }
    }
    if ( *pCompiledPgmText )
    {
        free(*pCompiledPgmText);
        *pCompiledPgmText = NULL;
    }
    
    //
    //    Do we have a pre-baked shader?
    //
    if (( altSourceText == NULL ) &&    // not using a dummy program
        ( game_state.shaderCache ) &&
        ( GET_PREBAKED_SHADER_TEXT( shaderFileFullPath, pBuildDescriptor, pCompiledPgmText ) ) )
    {
        program = rt_cgCreateProgramFromBuffer(
                        szPgmName, 
                        CG_OBJECT, 
                        *pCompiledPgmText,
                        target,
                        profile,
                        "main", 
                        pBuildDescriptor->compilerCmdLineArgs
                    );
    }
    else
    {    
        program = rt_cgCreateProgramFromBuffer(
                        szPgmName, 
                        CG_SOURCE, 
                        cgSourceText,
                        target,
                        profile,
                        "main", 
                        pBuildDescriptor->compilerCmdLineArgs
                    );

        *pCompiledPgmText = strdup(cgGetProgramString(program, CG_COMPILED_PROGRAM));
        STORE_PREBAKED_SHADER( pBuildDescriptor, *pCompiledPgmText );
    }

    if ( cgSourceText )
    {    
        fileFree( cgSourceText );
    }
    
    return program;
}

// Allocates memory for the string
static void addStringToCmdLineArgs( tShaderBuildDescriptor* pBuildDescriptor, const char* szStr )
{
    dynArrayFit( 
        (void**)&pBuildDescriptor->compilerCmdLineArgs, 
        sizeof(const char*), 
        &pBuildDescriptor->compilerCmdLineArgsArrSz, 
        pBuildDescriptor->compilerCmdLineArgsNum );
    pBuildDescriptor->compilerCmdLineArgs[pBuildDescriptor->compilerCmdLineArgsNum++] = 
        ( szStr == NULL ) ? NULL : strdup(szStr);
}

// Allocates memory for the string
static void addStringToBuildHints( tShaderBuildDescriptor* pBuildDescriptor, const char* szName, const char* szValue )
{
    char* nameValueStr = NULL;
    if (( szName != NULL ) && ( szValue != NULL ))
    {
        size_t nameValueStrSz = ( strlen(szName) + 1 + strlen(szValue) + 1 );
        nameValueStr = (char*)malloc( nameValueStrSz );
        sprintf_s( nameValueStr, nameValueStrSz, "%s=%s", szName, szValue );
    }

    dynArrayFit(
        (void**)&pBuildDescriptor->buildHints, 
        sizeof(const char*), 
        &pBuildDescriptor->buildHintsArrSz, 
        pBuildDescriptor->buildHintsNum );
    pBuildDescriptor->buildHints[pBuildDescriptor->buildHintsNum++] = nameValueStr;
}

static void initShaderBuildDescriptor(
                GLuint programHdl,    // OpenGL shader "name"
                const char* cohRelativeFilePath,
                tShaderProgramType target,
                tCgShaderProfile profile,
                U32 compileFlags,
                const char* shaderDirectory, 
                tShaderPathRootType rootType,
                tShaderBuildDescriptor* pBuildDescriptor
            )
{
    char* arg;
    int i;
    const char* kPrintfFmt = "-I%s%s";
    size_t buffNeeded;
    
    assert(( pBuildDescriptor->buildHintsNum == 0 ) &&
            ( pBuildDescriptor->compilerCmdLineArgsNum == 0 ));
            
    pBuildDescriptor->programHdl = programHdl;
    
    // See updateBuildDescriptorContentMd5()
    memset( pBuildDescriptor->srcFileMD5, 0, sizeof( pBuildDescriptor->srcFileMD5 ) );
    
    // Add the shader's directory to the include path
    buffNeeded = ( strlen(kPrintfFmt) + strlen(getShaderFileBaseDir(rootType)) + strlen(shaderDirectory) + 3);
    arg = _alloca( buffNeeded );
    sprintf_s(arg, buffNeeded, kPrintfFmt, getShaderFileBaseDir(rootType), shaderDirectory);
    addStringToCmdLineArgs( pBuildDescriptor, arg );
    
    // Compiler optimization hint
    // For debugging, an override is provided.
    {
        tShaderOptimizationLevel optLevel = kShaderOptimization_DEFAULT;
        #ifndef FINAL
        if ( game_state.shaderOptimizationOverride >= kShaderOptimization_LOWEST )
        {
            if ( game_state.shaderOptimizationOverride <= kShaderOptimization_HIGHEST )
            {
                optLevel = game_state.shaderOptimizationOverride; 
            }
            else
            {
                consoleSetColor(COLOR_RED|COLOR_BRIGHT, 0);
                printf( "Shader optimization override out of range. Ignoring...\n" );
                consoleSetDefaultColor();
                game_state.shaderOptimizationOverride = -1;
            }
        }
        #endif
        setShaderOptimizationLevel( pBuildDescriptor, optLevel );
    }
                
    // Standard optimization
    addStringToCmdLineArgs( pBuildDescriptor, "-fastprecision" );

    if(isDevelopmentOrQAMode())
    {
        addStringToCmdLineArgs( pBuildDescriptor, "-strict" );
    }
    
    if ( game_state.use_dxt5nm_normal_maps )
    {
        addDefine( "DXT5NM_NORMAL_MAPS" );
    }
    
    if (!(rdr_caps.features & GFXF_FPRENDER))
    {
        addDefine( "NO_FPRENDER" );
    }

    for (i = 0; i < shader_numDefines; ++i)
    {
        arg = _alloca(strlen(shader_defines[i]) + 3);
        sprintf(arg, "-D%s", shader_defines[i]);
        addStringToCmdLineArgs( pBuildDescriptor, arg );
    }

    // walk over the rdr_caps.chips flags and add any
    // flags as defines when compiling shaders
    // @todo note that when any changes are made to the
    // chips flags that is generally going to invalidate
    // the shader cache and force all the shaders to recompile
    // on the next launch
    for (i=0; i<32; i++) {
        if ((1<<i) & rdr_caps.chip) {
            char *chipname = rdrChipToString(1 << i);
            arg = _alloca(strlen(chipname) + 3);
            sprintf(arg, "-D%s", chipname);
            addStringToCmdLineArgs( pBuildDescriptor, arg );
        }
    }

    {
        const char* specialDefines[32];
        int numDefines = getShaderDebugSpecialDefines(specialDefines,ARRAY_SIZE(specialDefines));
        for (i=0; i < numDefines; i++)
        {
            arg = _alloca(strlen(specialDefines[i]) + 3);
            sprintf(arg, "-D%s", specialDefines[i]);
            addStringToCmdLineArgs( pBuildDescriptor, arg );
        }
    }

    // Position Invariance
    if (( target == kShaderPgmType_VERTEX ) && (profile == CG_PROFILE_ARBVP1) && (( compileFlags & kCompileFlag_NotPosInv ) == 0 ))
    {
        const char* kTag = "-posinv";
        arg = _alloca(strlen(kTag)+1);
        strcpy( arg, kTag );
        addStringToCmdLineArgs( pBuildDescriptor, arg );
    }

    prepareShaderProfileOption( profile, pBuildDescriptor );

    //@todo SCD, hack to be eliminated or at least add a test for validity
    // For the moment increase the number of uniform constants allowed in fragment programs
    // over the default 32 set for arbfp1. Supported by most hardware.
    if (target == kShaderPgmType_FRAGMENT && profile == CG_PROFILE_ARBFP1 && rdr_caps.max_program_local_parameters)
    {
        {
            const char* kTag = "-profileopts";
            arg = _alloca(strlen(kTag)+1);
            strcpy( arg, kTag );
            addStringToCmdLineArgs( pBuildDescriptor, arg );
        }
        {
            char kTag[256]; //"MaxLocalParams=128";
            sprintf(kTag, "MaxLocalParams=%d", rdr_caps.max_program_local_parameters);
            arg = _alloca(strlen(kTag)+1);
            strcpy( arg, kTag );
            addStringToCmdLineArgs( pBuildDescriptor, arg );
        }
    }
    
    addStringToCmdLineArgs( pBuildDescriptor, NULL );    // terminator, needed when we pass the array to Cg

    //
    // Prebake file name
    //
    computeBuildKey( pBuildDescriptor );
    makePreBakedShaderFileInputPathBS( cohRelativeFilePath, pBuildDescriptor->shaderKey,
        pBuildDescriptor->binFileNameIN, ARRAY_SIZE(pBuildDescriptor->binFileNameIN) );
    makePreBakedShaderFileOutputPathBS( cohRelativeFilePath, pBuildDescriptor->shaderKey,
        pBuildDescriptor->binFileNameOUT, ARRAY_SIZE(pBuildDescriptor->binFileNameOUT) );

}

static void updateBuildDescriptorContentMd5( tShaderBuildDescriptor* pBuildDescriptor,
                                        const char* cgSourceText, int cgSourceTextLen )
{
    computeBufferMD5( cgSourceText, cgSourceTextLen, pBuildDescriptor->srcFileMD5 );
}

static void releaseShaderBuildDescriptorMem( tShaderBuildDescriptor* pBuildDescriptor )
{
    int i;
    if ( pBuildDescriptor->buildHintsNum != 0 )
    {
        for ( i=0; i < pBuildDescriptor->buildHintsNum; i++ )
        {
            if ( pBuildDescriptor->buildHints[i] )
                free( pBuildDescriptor->buildHints[i] );
        }
        free( pBuildDescriptor->buildHints );
        pBuildDescriptor->buildHints = NULL;
    }
    if ( pBuildDescriptor->compilerCmdLineArgsNum != 0 )
    {
        for ( i=0; i < pBuildDescriptor->compilerCmdLineArgsNum; i++ )
        {
            if ( pBuildDescriptor->compilerCmdLineArgs[i] )
                free( pBuildDescriptor->compilerCmdLineArgs[i] );
        }
        free( pBuildDescriptor->compilerCmdLineArgs );
        pBuildDescriptor->compilerCmdLineArgs = NULL;
    }
}

static int createBuildDescriptorDescrText( tShaderBuildDescriptor* pBuildDescriptor )
{
    //
    //    The "descriptor text" is a concatenation of the compiler build hints
    //    (i.e., options for running the compiler) and the compiler command line
    //    args. Its primary purpose is to fully describe how a shader variant was
    //    produced. 
    //
    //    *** The MD5 of this text serves as a key to locating this variant. ***
    //
    //    Other than being MD5'd, this text is never parsed or used as input to
    //    anything. It is a descriptor only.
    //
    // For debugging convenience we currently write a copy of this text to the
    // file system (as a *.txt file). The markers dividing the "build hints" and
    // CGC compile flags are just a convenience for people looking at the text.
    //
    char* pBuff = pBuildDescriptor->descriptorText;
    size_t buffLeft = ARRAY_SIZE(pBuildDescriptor->descriptorText);
    size_t concatLen;
    bool bOverflow = false;    
    int listI;

    struct {
        const char* szListLabel;    // for the benefit of human readers only...
        const char** pList;
        int nListSz;
    } buildProps[] = {
        { "======= BUILD HINTS =======\n",
            pBuildDescriptor->buildHints, 
            pBuildDescriptor->buildHintsNum },
        { "==== CGC COMPILE FLAGS ====\n",
            pBuildDescriptor->compilerCmdLineArgs, 
            pBuildDescriptor->compilerCmdLineArgsNum }
    };

    for ( listI=0; listI < ARRAY_SIZE(buildProps); listI++ )
    {
        if ( buildProps[listI].nListSz > 0 )
        {
            int i;
            concatLen = strlen(buildProps[listI].szListLabel);
            if ( concatLen >= buffLeft ) { bOverflow = true; break; }
            strcpy( pBuff, buildProps[listI].szListLabel );
            pBuff += concatLen;
            buffLeft -= concatLen;

            for ( i=0; i < buildProps[listI].nListSz; i++ )
            {
                if ( buildProps[listI].pList[i] != NULL )    // not the NULL list termiantor
                {
                    concatLen = strlen(buildProps[listI].pList[i]) + 1;
                    if ( concatLen >= buffLeft ) { bOverflow = true; break; }
                    strcpy( pBuff, buildProps[listI].pList[i] );
                    pBuff += ( concatLen - 1 );
                    *(pBuff++) = '\n';
                    buffLeft -= concatLen;
                }
            }
            if ( bOverflow ) break;
        }
    }
    
    return ( bOverflow ) ? 0 : (int)( ARRAY_SIZE(pBuildDescriptor->descriptorText) - buffLeft );
}

static void computeBuildKey( tShaderBuildDescriptor* pBuildDescriptor )
{
    int buffUsed = createBuildDescriptorDescrText( pBuildDescriptor );
    computeBufferMD5( pBuildDescriptor->descriptorText, buffUsed, pBuildDescriptor->shaderKey );
}

static void computeBufferMD5( const U8* buffer, const int buffLen, U32 md5Key[4] /*OUT*/ )
{
    cryptMD5Init();
    cryptMD5Update( (U8*)buffer, buffLen );
    cryptMD5Final(md5Key);
}

static bool getPreBakedShaderText( const char* szCgShaderSrcFullPath, tShaderBuildDescriptor* pBuildDescriptor, char** pCompiledPgmText /*OUT*/ )
{
    if (( pBuildDescriptor->binFileNameIN[0] == '\0' ) ||
        ( isBinFileStale( pBuildDescriptor )))
    {
        *pCompiledPgmText = NULL;
    }
    else
    {
        *pCompiledPgmText = fileAlloc(pBuildDescriptor->binFileNameIN,0);    
    }
    return ( *pCompiledPgmText != NULL );
}

static void makeMd5KeyString( U32 key[4], char* strBuffer33, size_t buffSz /* at least 33 */ )
{
    assert( buffSz >= ( 8 * 4 + 1 ) );
    sprintf_s( strBuffer33, buffSz, "%08x%08x%08x%08x", 
                key[0], key[1], key[2], key[3] );    
}

static void makeMd5KeyStringFromContent( const char* szContent, size_t contentLen, char* keyStrBuff33, size_t keyStrBuffSz )
{
    U32 key[4];
    computeBufferMD5( (U8*)szContent, (int)contentLen, key );
    makeMd5KeyString( key, keyStrBuff33, keyStrBuffSz );
}

static tShaderPathRootType makePreBakedShaderFileInputPathBS( const char* cohRelativeSrcFilePathFS, U32 shaderKey[4],
                                    char* buffer, int bufferSz )
{
    // In theory, we could get the bin file from an override directory and write
    // it to a base directory, but this is unlikely, given the current conventions.
    // So the input file path and the output file path are the same.
    return makePreBakedShaderFileOutputPathBS( cohRelativeSrcFilePathFS, shaderKey, buffer, bufferSz );
}

static tShaderPathRootType makePreBakedShaderFileOutputPathBS( const char* cohRelativeSrcFilePathFS, U32 shaderKey[4],
                                    char* buffer, int bufferSz )
{
    tShaderPathRootType rootType = kPathRoot_GlobalAppDataPath;
    char md5KeyStr[33];
    size_t len;
    const char* szAdjustedPath = cohRelativeSrcFilePathFS;
    
    if ( strStartsWith( szAdjustedPath, SHADER_SUBDIR_CGFX ) )
    {
        szAdjustedPath += strlen(SHADER_SUBDIR_CGFX);
    }
    
    getCgShaderCacheRootDirBS( buffer, bufferSz );
    len = strlen(buffer);
    buffer += len;
    bufferSz -= len;
    
    makeMd5KeyString( shaderKey, md5KeyStr, ARRAY_SIZE(md5KeyStr) );
    
    sprintf_s( buffer, bufferSz, "\\%s.%s", szAdjustedPath, md5KeyStr );
    backSlashes( buffer );

    return rootType;
}

static bool makePreBakeDirectory( const char* szDir )
{
    char pathBuffer[2048];
    char* p;
    char oldP;
    
    // TBD: Seems like we would already have a function which makes
    // a new directory, including any missing elements... ???
    if ( _access_s( szDir, 0 ) == 0 )
    {
        return true;
    }
    strncpy_s( pathBuffer, ARRAY_SIZE(pathBuffer), szDir, _TRUNCATE );
    backSlashes( pathBuffer );
    
    p = pathBuffer;
    while ( true )
    {
        if (( *p == '\\' ) || ( *p == '\0' ))
        {
            oldP = *p;
            *p = '\0';
            if ( _access_s( pathBuffer, 0 ) != 0 )
            {
                if ( _mkdir( pathBuffer ) != 0 )
                {
                    return false;
                }
            }
            *p = oldP;
        }
        if ( *p == '\0' )
        {
            break;
        }
        p++;
    }
    return true;
}

static void storePreBakedShader( tShaderBuildDescriptor* pBuildDescriptor, const char* compiledPgmText )
{
    FILE* fWorkFile = NULL;
    char* pLastSlash = strrchr( pBuildDescriptor->binFileNameOUT, '\\' );
    *pLastSlash = '\0';
    if ( ! makePreBakeDirectory( pBuildDescriptor->binFileNameOUT ) )
    {
        return;
    }
    *pLastSlash = '\\';
    fWorkFile = fopen( pBuildDescriptor->binFileNameOUT, "w" );
    assert(fWorkFile);
    if(fWorkFile)
    {
        fwrite( compiledPgmText, 1, strlen(compiledPgmText), fWorkFile );
        fclose(fWorkFile);
    }

    {
        // write the informational file (optional)    
        char infoFileName[2048];
        sprintf_s( infoFileName, ARRAY_SIZE(infoFileName), "%s.txt", pBuildDescriptor->binFileNameOUT );
        fWorkFile = fopen( infoFileName, "w" );
        if(fWorkFile)
        {
            char contentMd5Str[33];
            makeMd5KeyString( pBuildDescriptor->srcFileMD5, contentMd5Str, ARRAY_SIZE(contentMd5Str) );
            fprintf( fWorkFile, "%s%s\n", INFO_FILE_CONTENT_MD5_FIELD, contentMd5Str );
            fwrite( pBuildDescriptor->descriptorText, 1, strlen(pBuildDescriptor->descriptorText), fWorkFile );
            fclose( fWorkFile );
        }
    }
}

static void clearPreBakedShadersRecursive( const char* startDir )
{
    struct _finddata_t fileInfo = { 0 };
    char searchPath[_MAX_PATH];
    intptr_t searchHandle = -1;
    
    sprintf_s( searchPath, ARRAY_SIZE(searchPath), "%s\\*.*", startDir );
    if (( searchHandle = _findfirst( searchPath, &fileInfo ) ) != -1 )
    {
        do
        {
            if ( fileInfo.name[0] != '.' )
            {
                if ( fileInfo.attrib & _A_SUBDIR )
                {
                    char nextDir[_MAX_PATH];
                    sprintf_s( nextDir, ARRAY_SIZE(nextDir), "%s\\%s", startDir, fileInfo.name );
                    clearPreBakedShadersRecursive( nextDir );
                }
                else if ( strstr( fileInfo.name, ".cg" ) != NULL )
                {
                    char nameToNuke[_MAX_PATH];
                    sprintf_s( nameToNuke, ARRAY_SIZE(nameToNuke), "%s\\%s", startDir, fileInfo.name );
                    remove( nameToNuke );
                }
            }
        } while ( _findnext( searchHandle, &fileInfo ) == 0 );
        _findclose( searchHandle );
    }
}

static void makeStandardizedFilePathMd5KeyString( const char* szPathStr, char* strBuffer33, size_t buffSz /* at least 33 */ )
{
    //
    // Rather than depend on the caller to send a standard format, make a copy
    // of the path and force it to lower-case backslash format. So callers who
    // send "r:/foo/bar" and callers who send "R:\Foo\Bar" will get the same result.
    //
    #define kEmptyString "_EMPTY_STRING_"
    char backSlashFmtPath[_MAX_PATH];
    U32 key[4];
    
    if ( szPathStr[0] != '\0' )
    {
        strcpy_s( backSlashFmtPath, ARRAY_SIZE(backSlashFmtPath), szPathStr );
        backSlashes( backSlashFmtPath );
        strlwr( backSlashFmtPath );
        // Standardize on no trailing slash...
        fixTrailingSlash( backSlashFmtPath, ARRAY_SIZE(backSlashFmtPath), 0 );
    }
    else
    {
        // String is empty so use a dummy string to generate the MD5.
        strcpy_s( backSlashFmtPath, ARRAY_SIZE(backSlashFmtPath), kEmptyString );
    }
    computeBufferMD5( backSlashFmtPath, (int)strlen(backSlashFmtPath) , key );
    makeMd5KeyString( key, strBuffer33, buffSz );
}

static void getCgShaderCacheRootDirBS(char* pathBuff, size_t buffSz)
{
    char srcDirMd5KeyStr[33];
    const char* shaderBaseDir = getShaderFileBaseDir( kPathRoot_GlobalAppDataPath );
    const char* szShaderSrcDirFullPath;
    assert( *shaderBaseDir != '\0' );

    //
    //    Subdirectory name is a string-ized MD5 of the source directory's full path.
    //
    //    This is so pre-built shader files from different source directories
    //    won't get intermixed.
    //
    szShaderSrcDirFullPath = getShaderFileBaseDir(kPathRoot_Custom);
    if ( *szShaderSrcDirFullPath == '\0' )
    {
        szShaderSrcDirFullPath = getShaderFileBaseDir( kPathRoot_GameData );
    }
    makeStandardizedFilePathMd5KeyString( szShaderSrcDirFullPath, srcDirMd5KeyStr, ARRAY_SIZE(srcDirMd5KeyStr) );
    
    sprintf_s( pathBuff, buffSz, "%s%s%s",
                shaderBaseDir,
                SHADER_SUBDIR,
                srcDirMd5KeyStr );
    backSlashes( pathBuff );
}


void shaderMgr_ClearShaderCache(void)
{
    char shaderDir[_MAX_PATH];
    char srcPath[_MAX_PATH];
    getCgShaderCacheRootDirBS( shaderDir, ARRAY_SIZE(shaderDir) );
    
    makeCgShaderSrcFullPathFS(SHADER_SUBDIR_CGFX,srcPath,ARRAY_SIZE(srcPath) );
    backSlashes(srcPath);
    fixTrailingSlash(srcPath, ARRAY_SIZE(srcPath), 0);
    
    clearPreBakedShadersRecursive( shaderDir );
    rebuildIncludeFileInfo( srcPath, shaderDir );
}

const char* findContentMd5Field( const char* szInfoFileText )
{
    const char* szMd5 = "";
    const char* lineStart = szInfoFileText;
    
    while ( lineStart != '\0' )
    {
        if ( strncmp( lineStart, INFO_FILE_CONTENT_MD5_FIELD, INFO_FILE_CONTENT_MD5_FIELD_LEN ) == 0 )
        {
            szMd5 = ( lineStart + INFO_FILE_CONTENT_MD5_FIELD_LEN );
            while ( *szMd5 == ' ' ) szMd5++;
            break;    
        }
        while (( *lineStart != '\n' ) && ( *lineStart != '\0' ))
        {
            lineStart++;
        }
        if ( *lineStart == '\n' ) lineStart++;
    }
    return szMd5;
}

static bool isBinFileStale( tShaderBuildDescriptor* pBuildDescriptor )
{
    bool bIsStale = true;
    char infoFilePath[_MAX_PATH];
    char* infoFileContent = NULL;
    sprintf_s( infoFilePath, ARRAY_SIZE(infoFilePath), "%s.txt", pBuildDescriptor->binFileNameIN );

    if (( fileExists(pBuildDescriptor->binFileNameIN) ) &&
        ( fileExists(infoFilePath) ) &&
        (( infoFileContent = fileAlloc(infoFilePath,0) ) != NULL ))
    {
        char buildDescrMd5Str[33];
        const char* szContentMd5 = findContentMd5Field( infoFileContent );
        makeMd5KeyString( pBuildDescriptor->srcFileMD5, buildDescrMd5Str, ARRAY_SIZE(buildDescrMd5Str) );
        if ( strncmp( szContentMd5, buildDescrMd5Str, strlen(buildDescrMd5Str) ) == 0 )
        {
            bIsStale = false;
        }
        fileFree( infoFileContent );
    }
    return bIsStale;
}

static bool isIncludeFileStale( const char* fileNameNoDirPath /* ex: foo.cg */,
                    const char* srcDirFullPath, const char* szBinDirFullPath )
{
    // In the case of an include file, there is no binary file in the bin
    // directory, just an informational file holding the MD5 of the header's
    // source data when we last compiled shaders.
    bool bIsStale = true;
    char srcFileFullPath[_MAX_PATH];
    char infoFileFullPath[_MAX_PATH];
    char* includeFileContent = NULL;
    size_t inclFileSz = 0;
    
    sprintf_s( srcFileFullPath, ARRAY_SIZE(srcFileFullPath), "%s\\%s", srcDirFullPath, fileNameNoDirPath );
    sprintf_s( infoFileFullPath, ARRAY_SIZE(infoFileFullPath), "%s\\%s.txt", szBinDirFullPath, fileNameNoDirPath );

    inclFileSz = fileSize( srcFileFullPath );
    if (( inclFileSz > 0 ) && 
        (( includeFileContent = fileAlloc( srcFileFullPath, 0 )) != NULL ))
    {
        char* infoFileContent = NULL;
        char includeFileContentMd5Str[33];
        makeMd5KeyStringFromContent( includeFileContent, inclFileSz, 
                                    includeFileContentMd5Str, ARRAY_SIZE(includeFileContentMd5Str) );
        if ((( infoFileContent = fileAlloc( infoFileFullPath, 0 )) != NULL ) &&
            ( strncmp( 
                includeFileContentMd5Str, 
                findContentMd5Field( infoFileContent ), 
                strlen(includeFileContentMd5Str) ) == 0 ))
        {
            bIsStale = false;
        }
        if ( infoFileContent )
        {
            fileFree( infoFileContent );        
        }
    }

    if ( includeFileContent )
    {
        fileFree( includeFileContent );
    }
    
    return bIsStale;
}

static FileScanAction checkIncludeFile_CB(char* dir, struct _finddata32_t* data)
{
    if ((!( data->attrib & _A_SUBDIR )) && ( strEndsWith( data->name, ".cgh" )))
    {
        char extendedBinPath[_MAX_PATH];
        const char* szSubdir = (dir + strlen(s_IncludeFileSearchCookie.szSrcFileDir));
        sprintf_s( extendedBinPath,
                    ARRAY_SIZE(extendedBinPath),
                    "%s%s%s",
                    s_IncludeFileSearchCookie.szBinFileDir,
                    (( *szSubdir == '\0' ) ? "" : "\\" ),
                    szSubdir 
                );
        backSlashes( extendedBinPath );
        if ( isIncludeFileStale( data->name, dir, extendedBinPath ) )
        {
            s_IncludeFileSearchCookie.bFoundStale = true;
            return FSA_STOP;
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

static FileScanAction rebuildIncludeFileInfo_CB(char* dir, struct _finddata32_t* data)
{
    if ((!( data->attrib & _A_SUBDIR )) && ( strEndsWith( data->name, ".cgh" )))
    {
        char cacheDir[_MAX_PATH];
        char includeFileName[_MAX_PATH];
        char infoFileName[_MAX_PATH];
        const char* szSubdir = (dir + strlen(s_IncludeFileSearchCookie.szSrcFileDir));
        int inclFileSz;
        char* includeFileContent = NULL;

        sprintf_s( cacheDir,ARRAY_SIZE(cacheDir),
                    "%s%s", s_IncludeFileSearchCookie.szBinFileDir, szSubdir );
        backSlashes( cacheDir );
        sprintf_s( infoFileName, ARRAY_SIZE(infoFileName),
                    "%s\\%s.txt", cacheDir,    data->name     );
        sprintf_s( includeFileName, ARRAY_SIZE(includeFileName), 
                    "%s\\%s", dir, data->name );
        forwardSlashes( includeFileName );
        inclFileSz = fileSize( includeFileName );
        if (( inclFileSz > 0 ) && 
            (( includeFileContent = fileAlloc( includeFileName, 0 )) != NULL ))
        {
            FILE* infoFile = NULL;
            char includeFileContentMd5Str[33];
            makeMd5KeyStringFromContent( includeFileContent, inclFileSz, 
                                        includeFileContentMd5Str, ARRAY_SIZE(includeFileContentMd5Str) );
            //
            //    Replace the info file for future reference.
            //
            makePreBakeDirectory( cacheDir );
            if (( infoFile = fopen( infoFileName, "w" )) != NULL )
            {
                fprintf( infoFile, "%s%s\n", INFO_FILE_CONTENT_MD5_FIELD, includeFileContentMd5Str );
                fclose( infoFile );
            }
        }
        if ( includeFileContent )
        {
            fileFree( includeFileContent );
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

static void initIncludeFileSearchCookie( const char* szSrcFileDir, const char* szBinFileDir )
{
    strcpy_s( s_IncludeFileSearchCookie.szSrcFileDir, 
                ARRAY_SIZE(s_IncludeFileSearchCookie.szSrcFileDir),
                 szSrcFileDir );
    strcpy_s( s_IncludeFileSearchCookie.szBinFileDir, 
                ARRAY_SIZE(s_IncludeFileSearchCookie.szBinFileDir),
                 szBinFileDir );
    s_IncludeFileSearchCookie.bFoundStale = false;
}

static bool checkForStaleIncludeFiles( const char* szSrcFileDir, const char* szBinFileDir )
{
    initIncludeFileSearchCookie( szSrcFileDir, szBinFileDir );
    fileScanAllDataDirs(szSrcFileDir, checkIncludeFile_CB);
    return s_IncludeFileSearchCookie.bFoundStale;
}

static void rebuildIncludeFileInfo( const char* szSrcDir, const char* szCacheDir )
{
    initIncludeFileSearchCookie( szSrcDir, szCacheDir );
    fileScanAllDataDirs(szSrcDir, rebuildIncludeFileInfo_CB);
}

void shaderMgr_PrepareShaderCache( void )
{
    if ( game_state.shaderCache )
    {
        time_t newestHeader = { 0 };
        time_t newestShader    = { 0 };
        char shaderCache[_MAX_PATH];
        char shaderSrcDir[_MAX_PATH];

        makeCgShaderSrcFullPathFS(SHADER_SUBDIR_CGFX,shaderSrcDir,ARRAY_SIZE(shaderSrcDir) );
        backSlashes(shaderSrcDir);
        fixTrailingSlash(shaderSrcDir,ARRAY_SIZE(shaderSrcDir),0);
        
        getCgShaderCacheRootDirBS(shaderCache,ARRAY_SIZE(shaderCache) );
        
        if ( checkForStaleIncludeFiles( shaderSrcDir, shaderCache ) )
        {
            // Some of the include files changed.
            shaderMgr_ClearShaderCache();
        }
    }
}

// handle commands from the main thread to control the shader debug facility
void shaderMgr_ShaderDebugControl( void* data )
{
#ifndef FINAL
    // lazy initialize shader debug facility and compile/load shaders
    // so we don't have everyone paying the penalty for this mode unless it
    // is invoked
    if (!g_mt_shader_debug_state.flag_loaded)
    {
        printf("Loading debug shaders...\n");
        // @todo this is a hack so that we can minimize the amount
        // of loading we do both at startup and when debug shaders are requested
        g_mt_shader_debug_state.flag_loading = true;
        reloadShaderCallbackDirect( NULL );
        g_mt_shader_debug_state.flag_loading = false;
        
        g_mt_shader_debug_state.flag_loaded = true;
        g_mt_shader_debug_state.flag_enable = true;
        printf("done\n");
    }
    else
    {
        g_mt_shader_debug_state.flag_enable = !g_mt_shader_debug_state.flag_enable;
    }
#endif
}
