#define RT_PRIVATE    // Only for jimb's debug stuff
#include "render/thread/ogl.h"
#include "render/texWords.h"
#include "texWordsPrivate.h"
#include "texWordsEdit.h"
#include <utilitieslib/utils/textparser.h>
#include "render/tex.h"
#include "render/thread/rt_tex.h"
#include <utilitieslib/utils/utils.h>
#include "assert.h"
#include <utilitieslib/components/StashTable.h>
#include "render/renderUtil.h"
#include "render/pbuffer.h"
#include "graphics/seqgraphics.h"
#include "win/win_init.h"
#include "render/thread/wcw_statemgmt.h"
#include "graphics/gfxwindow.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include <utilitieslib/language/MessageStore.h>
#include "graphics/truetype/ttFontDraw.h"
#include "graphics/truetype/ttFontManager.h"
#include "language/langClientUtil.h"
#include <utilitieslib/utils/StringUtil.h>
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/rgb_hsv.h>
#include "dxtlibwrapper.h"
#include "rotoZoom.h"
#include <utilitieslib/utils/memlog.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/language/AppLocale.h>
#include "render/thread/mipmap.h"
#include "render/renderprim.h"
#include "render/thread/rt_tex.h"
#include <utilitieslib/version/AppRegCache.h>
#include "graphics/gfxLoadScreens.h"
#include <utilitieslib/utils/cpu_count.h>

#define lsprintf if (game_state.texWordVerbose) loadstart_printf
#define leprintf if (game_state.texWordVerbose) loadend_printf

TexWordList texWords_list;
StashTable htTexWords; // Indexed by TexName, only has current locale in hashtable
static char texWordsLocale[MAX_PATH];
static char texWordsLocale2[MAX_PATH];

int texWords_useGl=false;
bool texWords_multiThread=true;
bool texWord_doNotYield=false;
U32 texWords_pixels=0; // Count of pixels rendered (more like "bytes")

static CRITICAL_SECTION criticalSectionDoingTexWordRendering; // We use statics here
static CRITICAL_SECTION criticalSectionDoingTexWordInfo;
HANDLE background_renderer_handle = NULL;
DWORD background_renderer_threadID = 0;
extern TexThreadPackage * texBindsReadyForFinalProcessing;
extern CRITICAL_SECTION CriticalSectionTexLoadQueues;
static volatile long numTexWordsInThread=0;

StaticDefineInt    TexWordParamLookup[] =
{
    DEFINE_INT
    { "TexWordParam1",    0},
    { "TexWordParam2",    1},
    { "TexWordParam3",    2},
    { "TexWordParam4",    3},
    { "TexWordParam5",    4},
    { "TexWordParam6",    5},
    { "TexWordParam7",    6},
    { "TexWordParam8",    7},
    { "TexWordParam9",    8},
    { "TexWordParam10",    9},
    DEFINE_END
};

StaticDefineInt    ParseTexWordLayerType[] =
{
    DEFINE_INT
    { "None",        TWLT_NONE},
    { "BaseImage",    TWLT_BASEIMAGE},
    { "Text",        TWLT_TEXT},
    { "Image",        TWLT_IMAGE},
    DEFINE_END
};

StaticDefineInt    ParseTexWordLayerStretch[] =
{
    DEFINE_INT
    { "None",    TWLS_NONE},
    { "Full",    TWLS_FULL},
    { "Tile",    TWLS_TILE},
    DEFINE_END
};

StaticDefineInt    ParseTexWordBlendType[] =
{
    DEFINE_INT
    { "Overlay",    TWBLEND_OVERLAY},
    { "Multiply",    TWBLEND_MULTIPLY},
    { "Add",        TWBLEND_ADD},
    { "Subtract",    TWBLEND_SUBTRACT},
    { "Replace",    TWBLEND_REPLACE},
    DEFINE_END
};

StaticDefineInt    ParseTexWordFilterType[] =
{
    DEFINE_INT
    { "None",        TWFILTER_NONE},
    { "Blur",        TWFILTER_BLUR},
    { "DropShadow",    TWFILTER_DROPSHADOW},
    { "Desaturate",    TWFILTER_DESATURATE},
    DEFINE_END
};


TokenizerParseInfo ParseDropShadow[] =
{
    {    "",                TOK_STRUCTPARAM| TOK_U8(TexWordLayerFont, dropShadowXOffset,0) },
    {    "",                TOK_STRUCTPARAM| TOK_U8(TexWordLayerFont, dropShadowYOffset,0) },
    {    "\n",            TOK_END,                                0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseTexWordLayerFont[] = {
    { "Name",                    TOK_STRING(TexWordLayerFont,fontName,0)            },
    { "Size",                    TOK_INT(TexWordLayerFont,drawSize, 16)        },
    { "Italic",                    TOK_BOOLFLAG(TexWordLayerFont,italicize,0)        },
    { "Bold",                    TOK_BOOLFLAG(TexWordLayerFont,bold,0)                },
    { "Outline",                TOK_U8(TexWordLayerFont,outlineWidth,0)        },
    { "DropShadow",                TOK_NULLSTRUCT(ParseDropShadow)    },
//    { "SoftShadow",                TOK_U8(TexWordLayerFont,softShadowSpread)    },

    { "EndFont",                TOK_END,        0                                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseTexWordLayerFilterOffset[] =
{
    {    "",                TOK_STRUCTPARAM | TOK_INT(TexWordLayerFilter, offset[0],0) },
    {    "",                TOK_STRUCTPARAM | TOK_INT(TexWordLayerFilter, offset[1],0) },
    {    "\n",            TOK_END,                                0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseTexWordLayerFilter[] = {
    { "Type",                    TOK_INT(TexWordLayerFilter,type, 0), ParseTexWordFilterType    },
    { "Magnitude",                TOK_INT(TexWordLayerFilter,magnitude, 1)    },
    { "Percent",                TOK_F32(TexWordLayerFilter,percent, 1)    },
    { "Spread",                    TOK_F32(TexWordLayerFilter,spread, 0)    },
    { "Color",                    TOK_INTARRAY(TexWordLayerFilter,rgba)            },
    { "Offset",                    TOK_NULLSTRUCT(ParseTexWordLayerFilterOffset)},

    { "Blend",                    TOK_INT(TexWordLayerFilter,blend, 0), ParseTexWordBlendType    },

    { "EndFilter",                TOK_END,        0                                            },
    { "", 0, 0 }
};



TokenizerParseInfo ParseTexWordLayer[] = {
    { "Name",                    TOK_STRING(TexWordLayer,layerName,0)    },
    { "Type",                    TOK_INT(TexWordLayer,type, 0), ParseTexWordLayerType    },
    { "Stretch",                TOK_INT(TexWordLayer,stretch, 0), ParseTexWordLayerStretch},
    { "Text",                    TOK_STRING(TexWordLayer,text,0)            },
    { "Image",                    TOK_STRING(TexWordLayer,imageName,0)    },
    { "Color",                    TOK_INTARRAY(TexWordLayer,rgba)            },
    { "Color0",                    TOK_INTARRAY(TexWordLayer,rgbas[0])        },
    { "Color1",                    TOK_INTARRAY(TexWordLayer,rgbas[1])        },
    { "Color2",                    TOK_INTARRAY(TexWordLayer,rgbas[2])        },
    { "Color3",                    TOK_INTARRAY(TexWordLayer,rgbas[3])        },
    { "Pos",                    TOK_VEC2(TexWordLayer,pos)            },
    { "Size",                    TOK_VEC2(TexWordLayer,size)            },
    { "Rot",                    TOK_F32(TexWordLayer,rot,0)            },
    { "Hidden",                    TOK_BOOLFLAG(TexWordLayer,hidden,0),        },
    { "Font",                    TOK_EMBEDDEDSTRUCT(TexWordLayer,font,ParseTexWordLayerFont)},

    { "Filter",                    TOK_STRUCT(TexWordLayer,filter, ParseTexWordLayerFilter) },

    { "SubLayer",                TOK_STRUCT(TexWordLayer,sublayer, ParseTexWordLayer) },
    { "SubBlend",                TOK_INT(TexWordLayer,subBlend, 0), ParseTexWordBlendType    },
    { "SubBlendWeight",            TOK_F32(TexWordLayer,subBlendWeight, 1), 0},

    { "EndLayer",                TOK_END,        0                                    },
    { "EndSubLayer",            TOK_END,        0                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseTexWordLayerSize[] =
{
    {    "",                TOK_STRUCTPARAM | TOK_INT(TexWord, width,0) },
    {    "",                TOK_STRUCTPARAM | TOK_INT(TexWord, height,0) },
    {    "\n",            TOK_END,                                0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseTexWord[] = {
    { "TexWord",                    TOK_IGNORE,    0}, // hack, so we can read in individual entries
    { "Name",                        TOK_CURRENTFILE(TexWord,name)                },
    { "Size",                        TOK_NULLSTRUCT(ParseTexWordLayerSize) },
    { "Layer",                        TOK_STRUCT(TexWord,layers, ParseTexWordLayer) },

    { "EndTexWord",                    TOK_END,        0                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseTexWordFiles[] = {
    { "TexWord",                    TOK_STRUCT(TexWordList,texWords, ParseTexWord) },
    { "", 0, 0 }
};

MP_DEFINE(TexWordParams);
TexWordParams *createTexWordParams(void)
{
    MP_CREATE(TexWordParams, 16);
    return MP_ALLOC(TexWordParams);
}
void destroyTexWordParams(TexWordParams *params)
{
    // this is actually a pointer to someone else's memory: SAFE_FREE(params->layoutName);
    // so are these: eaClearEx(&params->parameters, NULL);
    eaDestroy(&params->parameters);
    MP_FREE(TexWordParams, params);
}

MP_DEFINE(TexWordLoadInfo);
TexWordLoadInfo *createTexWordLoadInfo(void)
{
    TexWordLoadInfo *ret;
    EnterCriticalSection(&criticalSectionDoingTexWordInfo);
    MP_CREATE(TexWordLoadInfo, 4);
    ret = MP_ALLOC(TexWordLoadInfo);
    LeaveCriticalSection(&criticalSectionDoingTexWordInfo);
    return ret;
}
void destroyTexWordLoadInfo(TexWordLoadInfo *loadinfo)
{
    EnterCriticalSection(&criticalSectionDoingTexWordInfo);
    SAFE_FREE(loadinfo->data);
    MP_FREE(TexWordLoadInfo, loadinfo);
    LeaveCriticalSection(&criticalSectionDoingTexWordInfo);
}

int texWordGetPixelsPerSecond(void)
{
    U32        delta_ticks;
    U32        delta_pixels;
    F32        time;
    static    U32 last_ticks;
    static  U32 last_pixels=0;
    static    U32 last_pps_ticks=0;
    static  int pps=0;

    last_ticks = timerCpuTicks();

    // Calc PPS over a few seconds
    delta_ticks = last_ticks - last_pps_ticks;
    time = (F32)delta_ticks / (F32)timerCpuSpeed();
    if (time > 1.0)
    {
        delta_pixels = texWords_pixels - last_pixels;
        last_pixels = texWords_pixels;
        pps = delta_pixels / time;
        last_pps_ticks = last_ticks;
        if (pps && game_state.texWordVerbose) {
            printf("TWPPS: % 9d\n", pps);
        }
    }
    return pps / 1000 * 1000 + numTexWordsInThread;
}

int texWordGetTotalPixels(void)
{
    return texWords_pixels;
}

int texWordGetLoadsPending(void)
{
    return numTexWordsInThread;
}

#define INIT_YIELD_AMOUNT 3072
static int yield_amount=INIT_YIELD_AMOUNT;
static void texWordsPixelsRendered(int pixels, bool yield)
{
    static int yield_sum=0;
    texWords_pixels+=pixels;
    //printf("twpr: % 6d\n", pixels);
    if (yield && !texWord_doNotYield) {
        yield_sum+=pixels;
        if (yield_sum > yield_amount) {
            yield_sum = 0;
            Sleep(1);
        }
    }
    if (!yield) {
        loadUpdate("TexWords", pixels/32);
    }
}

void texWordsFlush(void)
{
    // Sets a flag (that may get cleared at some arbitrary time later) that makes the background thread
    //  take up more (up to 50%) CPU
    texWord_doNotYield=true;
}

// void texWordsSetThreadPriority(U32 priority)
// {
//     if(background_renderer_handle)
//         SetThreadPriority(background_renderer_handle, priority);
// }

/*TexWords Renderer thread: All I do is sleep, waiting to be given things by QueueUserAPC*/
static DWORD WINAPI texWordsThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
        //if (fileIsUsingDevData()) {
        //    SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_BELOW_NORMAL - 1);
        //}
        for(;;)
            SleepEx(INFINITE, TRUE);
        return 0; 
    EXCEPTION_HANDLER_END
}

void initBackgroundTexWordRenderer(void)
{
    InitializeCriticalSection(&criticalSectionDoingTexWordInfo);
    InitializeCriticalSection(&criticalSectionDoingTexWordRendering);

    if(background_renderer_handle == NULL )
    {
        DWORD dwThrdParam = 1; 

        background_renderer_handle = (HANDLE)_beginthreadex( 
            NULL,                        // no security attributes 
            0,                           // use default stack size  
            texWordsThread,                 // thread function 
            &dwThrdParam,                // argument to thread function 
            0,                           // use default creation flags 
            &background_renderer_threadID);                // returns the thread identifier 

        //_ASSERTE(heapValidateAll());
        assert(background_renderer_handle != NULL);
    }
}


static int texWordsNameCmp(const TexWord ** info1, const TexWord ** info2 )
{
    return stricmp( getFileName((char*)(*info1)->name), getFileName((char*)(*info2)->name) );
}

TexWord *texWordFind(const char *texName, int search)
{
    char baseTexName[128];
    char *s;
    char *locale=NULL;
    bool isBase=false;
    TexWord *ret;
    Strncpyt(baseTexName, texName);
    if (s = strchr(baseTexName, '#')) {
        *s=0;
        locale = s+1;
        if (stricmp(locale, "Base")==0)
            isBase = true;
    }
    stashFindPointer( htTexWords, baseTexName, &ret ); // Search for TexWord in current locale
    if (!ret && (isBase || search)) { // Search for TexWord in #Base locale if the texture was #Base
        strcat(baseTexName, "#Base");
        stashFindPointer( htTexWords, baseTexName, &ret );
    } else if (!ret && !isBase && !locale) {
        // If a #Base .tga does not exist (nor any other locale), and a #Base texword does exist, use it!
        strcat(baseTexName, "#Base");
        if (!texFind(baseTexName)) {
            stashFindPointer( htTexWords, baseTexName, &ret );
        }
    }
    if (!ret && isBase && game_state.texWordVerbose) {
        // Debug hack to fill everything that would have a texword with a dummy file
        stashFindPointer( htTexWords, "DUMMYLAYOUT", &ret );
    }
    return ret;
}

BasicTexture *texWordGetBaseImage(TexWord *texWord)
{
    int numlayers = eaSize(&texWord->layers);
    int i;
    BasicTexture *ret=NULL;
    for (i=0; i<numlayers; i++) {
        if (texWord->layers[i]->type == TWLT_BASEIMAGE)
            return texWord->layers[i]->image;
        if (texWord->layers[i]->type == TWLT_IMAGE)
            ret = texFind(texWord->layers[i]->imageName);
    }
    return ret;
}


static int layerNameCount=0, subLayerNameCount=0;
bool texWordVerifyLayer(TexWordLayer *layer, bool fix, bool subLayer)
{
    bool ret, ret2;

    if (!layer->layerName || !layer->layerName[0]) {
        if (fix) {
            char buf[256];
            if (subLayer) {
                sprintf(buf, "sub%d", ++subLayerNameCount);
            } else {
                sprintf(buf, "#%d", ++layerNameCount);
            }
            layer->layerName = ParserAllocString(buf);
        }
        ret = false;
    }

    if (layer->type == TWLT_IMAGE) {
        if (!layer->imageName) {
            if (fix)
                layer->imageName = ParserAllocString("white");
            ret = false;
        }
    } else if (layer->type == TWLT_TEXT) {
        if (!layer->text) {
            if (fix)
                layer->text = ParserAllocString("Placeholder");
            ret=false;
        }
        if (!layer->font.fontName) {
            if (fix)
                layer->font.fontName = ParserAllocString("arial.ttf");
            ret = false;
        }
        if (layer->stretch != TWLS_NONE) {
            if (fix)
                layer->stretch = TWLS_NONE;
            ret = false;
        }
    }
    if (layer->sublayer) {
        int numsublayers = eaSize(&layer->sublayer);
        if (numsublayers>1) {
            Errorf("Error: found more than 1 sublayer in a texword file");
            if (fix) {
                int i;
                for (i=1; i<numsublayers; i++) {
                    ParserDestroyStruct(ParseTexWordLayer, layer->sublayer[i]);
                    layer->sublayer[i] = NULL;
                }
                eaSetSize(&layer->sublayer, 1);
            }
            ret = false;
        }
        ret2 = texWordVerifyLayer(layer->sublayer[0], fix, true);
        ret &= ret2;
    }
    if (layer->subBlendWeight < 0 || layer->subBlendWeight > 1) {
        ret = false;
        if (fix)
            layer->subBlendWeight = MIN(MAX(layer->subBlendWeight, 0), 1);
    }

    if (layer->filter) {
        int numfilters = eaSize(&layer->filter);
        int i;
        for (i=0; i<numfilters; i++) {
            TexWordLayerFilter *filter = layer->filter[i];
            if (filter->percent > 1.0) {
                ret = false;
                if (fix)
                    filter->percent = 1.0;
            }
            if (filter->magnitude < 0) {
                ret = false;
                if (fix)
                    filter->magnitude = 0;
            }
        }
    }

    return ret;
}

bool texWordVerify(TexWord *texWord, bool fix) {
    bool ret=true, ret2;
    int numlayers;
    int i;

    if (!texWord)
        return false;

    numlayers = eaSize(&texWord->layers);
    layerNameCount=numlayers-1;
    subLayerNameCount=0;
    for (i=0; i<numlayers; i++) {
        ret2 = texWordVerifyLayer(texWord->layers[i], fix, false);
        ret &= ret2;
    }
    return ret;
}

bool texWordsPreprocessor(TokenizerParseInfo pti[], TexWordList *twl)
{
    int numTexWords=eaSize(&twl->texWords);
    int i;
    for (i=0; i<numTexWords; i++) {
        texWordVerify(twl->texWords[i], true);
    }
    return true;
}


void texWordsLoadInfo()
{
    // Free old stuff
    if (htTexWords) {
        stashTableDestroy(htTexWords);
        htTexWords = 0;
    }
    if (texWords_list.texWords) {
        ParserDestroyStruct(ParseTexWordFiles, &texWords_list);
    }

    if (isProductionMode() && game_state.texWordEdit) {
        // In texWordEdit production, tack on any extra ones sitting around on disk
        ParserLoadFiles("texts", ".texword", NULL, 0, ParseTexWordFiles, &texWords_list, NULL, NULL, texWordsPreprocessor);
    } else {
        ParserLoadFiles("texts", ".texword", "texWords.bin", 0, ParseTexWordFiles, &texWords_list, NULL, NULL, texWordsPreprocessor);
    }

    htTexWords = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);

    {
        int num_structs, i;
        int count=0, count2=0;

        //Clean up TexWord names, and sort them
        num_structs = eaSize(&texWords_list.texWords);
        qsort(texWords_list.texWords, num_structs, sizeof(void*), (int (*) (const void *, const void *)) texWordsNameCmp);

        // Add entries of the appropriate locale to the hashtable
        for (i=0; i<num_structs; i++) {
            StashElement element;
            TexWord *texWord = texWords_list.texWords[i];
            // texWord->name = "texts/Locale/path/texturename.texword"
            char locale[MAX_PATH];
            char textureName[MAX_PATH];
            char *s = texWord->name;
            if (strStartsWith(s, "texts"))
                s+=strlen("texts");
            while (*s=='/' || *s=='\\') s++;
            strcpy(locale, s);
            forwardSlashes(locale);
            s = strchr(locale, '/');
            *s=0;
            // Extract texture name
            strcpy(textureName, getFileName(texWord->name));
            s = strrchr(textureName, '.');
            *s=0;
            if (stricmp(locale, texWordsLocale)==0 || texWordsLocale2[0] && stricmp(locale, texWordsLocale2)==0) {
                // Good!
                count++;
            } else {
                // Add in under specific locale
                strcat(textureName, "#");
                strcat(textureName, locale);
            }
            stashFindElement(htTexWords, textureName, &element);
            if (element) {
                if (texWordsLocale2[0]) {
                    // Multiple locales (#UK;#English) might cause this, use locale2
                    if (stricmp(locale, texWordsLocale2)==0) {
                        // Use this one, remove the old one
                        stashRemovePointer(htTexWords, textureName, NULL);
                    }
                } else {
                    Errorf("Duplicate TexWord defined:\n%s\n%s", ((TexWord*)stashElementGetPointer(element))->name, texWord->name);
                }
            }
            stashAddPointer(htTexWords, textureName, texWord, false);
        }

        // Fix up textures on reload
        for (i=eaSize(&g_basicTextures)-1; i>=0; i--) {
            BasicTexture *bind = g_basicTextures[i];
            if (bind->texWordParams) {
                bind->texWord = texWordFind(bind->texWordParams->layoutName, 1);
            } else {
                bind->texWord = texWordFind(bind->name, 0);
            }
            if (bind->texWord)
                count2++;
            if (bind->hasBeenComposited || bind->texWord) {
                // Free the old composited data
                texFree(bind, 0);
            }
        }
        if (count2) {
            writeConsole(OUTPUT_VERBOSE, "Loaded %d texWords for current locale, %d textures assigned texWords", count, count2);
        } else {
            writeConsole(OUTPUT_VERBOSE, "Loaded %d texWords for current locale", count);
        }
    }
}

bool texWordsNeedsReload = false;
static void reloadTexWordCallback(const char *relpath, int when){
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    texWordsNeedsReload = true;
}

void texWordsCheckReload(void) {
    if (texWordsNeedsReload && isDevelopmentMode()) {
        if (numTexWordsInThread==0) {
            texWordsNeedsReload = false;
            texWordsLoad(texWordsLocale);
        } else {
            printf("Delaying reload until TexWordRenderer is finished (%d remaining)\n", numTexWordsInThread);
            texWord_doNotYield=true;
        }
    }
}

static bool g_allow_loading = false;
void texWordsAllowLoading(bool allow)
{
    g_allow_loading = allow;
}

void texWordsReloadText(char *localeName)
{
    // Reload appropriate message store
    char textResourcePath[512];
    char *textResourcePaths[] = {
        textResourcePath,    NULL,
        NULL
    };

    if (stricmp(localeName, "Base")==0) {
        localeName = "English";
    }

    sprintf(textResourcePath, "texts\\%s\\textureWords.ms", localeName); // Test locale
    LoadMessageStore(&texWordsMessages,textResourcePaths,NULL,0,NULL,NULL,NULL,MSLOAD_EXTENDED);
}

void texWordsLoad(char *localeName)
{
    static    int inited=0;
    static char override[32];
    char messageStoreName[MAX_PATH]={0};
    char searchPath[MAX_PATH];
    char *effLocaleName=localeName;

    if (!g_allow_loading) {
        strcpy(override, localeName);
        return;
    } else if (override[0]) {
        effLocaleName = localeName = override;
    }

    if (game_state.texWordEdit && stricmp(localeName, "English")==0) {
        effLocaleName = localeName = "Base";
    }

    strcpy(texWordsLocale2, "");
    // Set the search path
    if (stricmp(effLocaleName, "English")==0) {
        if (getCurrentRegion() == REGION_EU) {
            // Assume Britain
            sprintf(searchPath, "#UK;#%s", effLocaleName);
            strcpy(texWordsLocale2, "UK");
        } else {
            // US
            // #English
            sprintf(searchPath, "#%s", effLocaleName);
        }
    } else {
        // #French;#Base
        sprintf(searchPath, "#%s;#Base", effLocaleName);
    }
    texSetSearchPath(searchPath);

    strcpy(texWordsLocale, localeName);

    if (numTexWordsInThread!=0) {
        printf("Not reloading texWords immediately...\n");
        texWordsNeedsReload=true;
        override[0] = 0;
        return;
    }
    texWord_doNotYield=false;

    if (getNumRealCpus() > 1) {
        yield_amount = INIT_YIELD_AMOUNT * 10;
    } else if (getNumVirtualCpus() > 1) {
        yield_amount = INIT_YIELD_AMOUNT * 2;
    }

//    // Reload appropriate message store
//    if (stricmp(effLocaleName, "Base")==0) {
//        char textResourcePath[512];
//        if(texWordsMessages){
//            destroyMessageStore(texWordsMessages);
//            texWordsMessages = NULL;
//        }
//        texWordsMessages = createMessageStore();
//        sprintf(textResourcePath, "texts\\%s\\textureWords.ms", locGetName(1)); // Test locale
//        initMessageStore( texWordsMessages, textResourcePath, NULL, 1);
//    }

    // Load the layout information (all locales loaded, keeps hashtable to current locale)
    texWordsLoadInfo();

    // Should run this at the end (only happens while reloading)
    texCheckSwapList();

    // Only in the tweditor
    texWordsEdit_reloadCallback();

    if (!inited) {
        // Add callback for re-loading texWord files
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "texts/*.texword", reloadTexWordCallback);
        inited = 1;
    }
    override[0] = 0;
}

static VOID CALLBACK texWordLayerLoadFont( ULONG_PTR dwParam )
{
    TexWordLayerFont *font = (TexWordLayerFont *)dwParam;

    PERFINFO_AUTO_START("texWordLayerLoadFont", 1);
    ttFontLoadCached(font->fontName, 1);
    PERFINFO_AUTO_STOP();
}


static void texWordLayerLoadDeps(TexWordLayer *layer, TexLoadHow mode, TexUsage use_category, BasicTexture *texBindParent)
{
    if (layer->hidden)
        return;
    if (layer->sublayer && layer->sublayer[0])
        texWordLayerLoadDeps(layer->sublayer[0], mode, use_category, texBindParent);
    switch(layer->type) {
        case TWLT_BASEIMAGE:
            // Assume already loaded, as only the loading of this calls this function
            if (texWords_useGl) {
                layer->image = texBindParent;
            } else {
                layer->image = texLoadRawData(texBindParent->name, mode, use_category);
            }
            break;
        case TWLT_IMAGE:
            if (texWords_useGl) {
                layer->image = texLoadBasic(layer->imageName, mode, use_category);
            } else {
                layer->image = texLoadRawData(layer->imageName, mode, use_category);
            }
            break;
        case TWLT_TEXT:
            if( mode == TEX_LOAD_NOW_CALLED_FROM_LOAD_THREAD || mode == TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD)
            {
                texWordLayerLoadFont((ULONG_PTR)&layer->font);
            }
            else if( mode == TEX_LOAD_IN_BACKGROUND || mode == TEX_LOAD_IN_BACKGROUND_FROM_BACKGROUND )
            {
                extern HANDLE background_loader_handle;
                assert(background_loader_handle);
                QueueUserAPC(texWordLayerLoadFont, background_loader_handle, (ULONG_PTR)&layer->font );
            }
            else
                assert(0);
            break;
    }
}

void texWordLoadDeps(TexWord *texWord, TexLoadHow mode, TexUsage use_category, BasicTexture *texBindParent)
{
    int i, numlayers = eaSize(&texWord->layers);

    for (i=0; i<numlayers; i++) {
        texWordLayerLoadDeps(texWord->layers[i], mode, use_category, texBindParent);
    }
}

static void checkEverythingIsLoadedLayer(TexWordLayer *layer, BasicTexture *texBindParent)
{
    if (layer->hidden)
        return;
    if (layer->sublayer && layer->sublayer[0])
        checkEverythingIsLoadedLayer(layer->sublayer[0], texBindParent);
    switch(layer->type) {
        case TWLT_BASEIMAGE:
            assertmsg(texBindParent == layer->image || !isDevelopmentMode(),
                "This TexWord seems to be referring to a texture that does not exist in the English version (Only a #Base?)");
            if (texWords_useGl) {
            } else {
                assert(layer->image && layer->image->actualTexture->load_state[1] == TEX_LOADED);
                assert(layer->image->actualTexture->rawReferenceCount);
            }
            break;
        case TWLT_IMAGE:
            if (texWords_useGl) {
                assert(layer->image && layer->image->actualTexture->load_state[0] == TEX_LOADED);
            } else {
                assert(layer->image && layer->image->actualTexture->load_state[1] == TEX_LOADED);
                assert(layer->image->actualTexture->rawReferenceCount);
            }
            break;
        case TWLT_TEXT:
            assert(ttFontLoadCached(layer->font.fontName, 0));
            break;
    }
}

static void checkEverythingIsLoaded(TexWord *texWord, BasicTexture *texBindParent)
{
    // Could dynamically load stuff here

    // Sanity checks
    int i, numlayers = eaSize(&texWord->layers);

    for (i=0; i<numlayers; i++) {
        checkEverythingIsLoadedLayer(texWord->layers[i], texBindParent);
    }
}

static void unloadDataAfterCompositionLayer(TexWordLayer *layer)
{
    if (layer->hidden)
        return;
    if (layer->sublayer && layer->sublayer[0])
        unloadDataAfterCompositionLayer(layer->sublayer[0]);
    switch(layer->type) {
        case TWLT_BASEIMAGE:
            if (texWords_useGl) {
            } else {            
                texUnloadRawData(layer->image);
            }
            break;
        case TWLT_IMAGE:
            if (texWords_useGl) {
                assert(layer->image && layer->image->actualTexture->load_state[0] == TEX_LOADED);
            } else {
                texUnloadRawData(layer->image);
            }
            break;
        case TWLT_TEXT:
            ttFontUnloadCached(layer->font.fontName);
            break;
    }
}

static void unloadDataAfterComposition(TexWord *texWord)
{
    int i, numlayers = eaSize(&texWord->layers);

    for (i=0; i<numlayers; i++) {
        unloadDataAfterCompositionLayer(texWord->layers[i]);
    }
}

static Color makeColor(eaiHandle colors)
{
    Color ret;
    if (eaiSize(&colors)==3) {
        ret.integer = 0xff << 24 |
            colors[2] << 16 |
            colors[1] << 8 | 
            colors[0];
    } else if (eaiSize(&colors)==4) {
        ret.integer = colors[3] << 24 |
            colors[2] << 16 |
            colors[1] << 8 | 
            colors[0];
    } else {
        ret.integer = 0xffff00ff;
    }
    return ret;
}


static int bufferSizeX, bufferSizeY;
static void bufferSetSize(int x, int y)
{
    bufferSizeX=x;
    bufferSizeY=y;
}
typedef enum PBName {
    PB_LASTLAYER,
    PB_THISLAYER,
    PB_SUBLAYER,
    PB_TEMP,
} PBName;
static U8* pixbuffers[3] = {0};
static U8* getBuffer(PBName buf) {
    if (!pixbuffers[buf]) {
        pixbuffers[buf] = calloc(bufferSizeX*bufferSizeY*4, 1);
    }
    return pixbuffers[buf];
}
static void freeBuffers()
{
    int i;
    for (i=0; i<ARRAY_SIZE(pixbuffers); i++) {
        SAFE_FREE(pixbuffers[i]);
    }
}
static void swapBuffers(PBName a, PBName b)
{
    U8* temp=pixbuffers[a];
    pixbuffers[a] = pixbuffers[b];
    pixbuffers[b] = temp;
}


void renderTexWordLayerImageGL(TexWordLayer *layer, int x, int y, int sizeX, int sizeY, int screenSizeX, int screenSizeY)
{
    F32 s = 0, t = 0;
    BasicTexture *image = layer->image;
    F32 u = (float)image->width/image->realWidth;
    F32 v = (float)image->height/image->realHeight;
    int i;
    Color rgba[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};

    rdrCheckThread();
    if (layer->rgbas[0]) {
        for (i=0; i<4; i++) {
            rgba[i] = makeColor(layer->rgbas[i]);
        }
    } else if (layer->rgba) {
        for (i=0; i<4; i++) {
            rgba[i] = makeColor(layer->rgba);
        }
    }

    if (layer->stretch == TWLS_TILE) {
        s = -x/(F32)sizeX;
        t = -y/(F32)sizeY;
        u = (screenSizeX - x) / (F32)sizeX;
        v = (screenSizeY - y) / (F32)sizeY;

        x = y = 0;
        sizeX = screenSizeX;
        sizeY = screenSizeY;
    }

    modelDrawState(DRAWMODE_SPRITE, ONLY_IF_NOT_ALREADY);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);

    texBind(TEXLAYER_BASE, image->actualTexture->id);
    glBegin( GL_QUADS );
    WCW_ColorU32(rgba[0].integer);
    glTexCoord2f( s, t );
    glVertex2f( x, screenSizeY - y );

    WCW_ColorU32(rgba[1].integer);
    glTexCoord2f( u, t );
    glVertex2f( x+sizeX, screenSizeY - y );

    WCW_ColorU32(rgba[2].integer);
    glTexCoord2f( u, v );
    glVertex2f( x+sizeX, screenSizeY - (y+sizeY) );

    WCW_ColorU32(rgba[3].integer);
    glTexCoord2f( s, v );
    glVertex2f( x, screenSizeY - (y+sizeY) );
    glEnd(); CHECKGL;
}


U8* uncompressRawTexInfo(TexReadInfo *rawInfo) // Uncompresses to GL_RGBA8
{
    int w, h, depth, total_width, rowBytes, src_format, SpecifiedMipMaps;
    U8 *buffer, *rawBuffer;
    if (rawInfo->format == TEXFMT_RAW_DDS) {
        lsprintf("nvDXTdecompress()...");
        SpecifiedMipMaps = 1;
        // total_width is the sum total of the widths of all mip levels
        buffer = nvDXTdecompressC(&w, &h, &depth, &total_width, &rowBytes, &src_format, SpecifiedMipMaps, rawInfo->data );

        free(rawInfo->data);
        rawInfo->data = buffer;
        rawInfo->width = w;
        rawInfo->height = h;
        rawInfo->size = rowBytes * h; // ?
        if (depth == 4) {
            rawInfo->format = TEXFMT_ARGB_8888;
        } else if (depth == 3) {
            rawInfo->format = TEXFMT_ARGB_0888;
        } else {
            assert(!"bad bit depth");
        }
        leprintf("done.");
    }
    
    if (rawInfo->format == GL_RGBA8) {
        buffer = rawInfo->data;
    } else {
        int i, size;
        U8 c;
        size = rawInfo->width*rawInfo->height*4;
        // Uncompress
        buffer = rawInfo->data;
        switch (rawInfo->format) {
            case TEXFMT_ARGB_8888:
                lsprintf("TEXFMT_ARGB_8888 resample %dx%d...", rawInfo->width, rawInfo->height);
                for (i=0; i<size; i+=4) {
                    c = buffer[i];
                    buffer[i] = buffer[i+2];
                    buffer[i+2] = c;
                }
                leprintf("done.");
                break;
            case TEXFMT_ARGB_0888:
                lsprintf("TEXFMT_ARGB_0888 resample %dx%d...", rawInfo->width, rawInfo->height);
                buffer = calloc(size, 1);
                rawBuffer = rawInfo->data;
                size = w*h;
                for (i=0; i<size; i++) {
                    ((int*)buffer)[i] =
                        (rawBuffer[i*3]<<16)|
                        (rawBuffer[i*3+1]<<8)|
                        rawBuffer[i*3+2]|
                        0xff000000;
                }
                free(rawInfo->data);
                rawInfo->data = buffer;
                leprintf("done.");
                break;
            case GL_RGB8:
                lsprintf("GL_RGB8 resample %dx%d...", rawInfo->width, rawInfo->height);
                buffer = calloc(size, 1);
                rawBuffer = rawInfo->data;
                size = rawInfo->width*rawInfo->height;
                for (i=0; i<size-1; i++) {
                    ((int*)buffer)[i] = *((int*)&rawBuffer[i*3])|0xff000000;
                }
                // protect last pixel from accessing outside of data boundary
                ((int*)buffer)[i] = rawBuffer[i*3+2]<<16 | rawBuffer[i*3+1]<<8 | rawBuffer[i*3] | 0xff000000;
                free(rawInfo->data);
                rawInfo->data = buffer;
                leprintf("done.");
                break;
            default:
                assert(!"Unsupported texture format");
                break;
        }
        rawInfo->format = GL_RGBA8;
    }
    return buffer;
}


static int cleanupColors(Color rgba[4], TexWordLayer *layer)
{
    int i;
    int numColors;
    if (layer->rgbas[0]) {
        for (i=0; i<4; i++) {
            rgba[i] = makeColor(layer->rgbas[i]);
        }
        numColors = 4;
    } else if (layer->rgba) {
        for (i=0; i<4; i++) {
            rgba[i] = makeColor(layer->rgba);
        }
        numColors = 1;
    } else {
        numColors = 0;
    }
    // Simplify colors
    if (numColors==4) {
        if (rgba[0].integer==rgba[1].integer &&
            rgba[1].integer==rgba[2].integer &&
            rgba[2].integer==rgba[3].integer)
            numColors = 1;
    }
    if (numColors==1) {
        if (rgba[0].integer == 0xffffffff)
            numColors = 0;
    }
    return numColors;
}


// rotate the point x, y around ptx,pty by Rot degrees
void rotatePointAroundPoint(int *x, int *y, int ptx, int pty, F32 rot)
{
    Vec3 rotmat;
    Vec3 pos;
    Vec3 pos2;
    Mat3 mat;
    // Move upper left corner based on rotation
    rotmat[0] = rotmat[1] = 0;
    rotmat[2] = rot*PI/180;
    createMat3PYR(mat, rotmat);
    setVec3(pos, *x - ptx, *y - pty, 0);
    mulVecMat3(pos, mat, pos2);
    *x = ptx + pos2[0];
    *y = pty + pos2[1];
}

typedef struct TexWordImage {
    U8 *buffer;
    int sizeX, sizeY;
    int pitch;
} TexWordImage;

static void renderTexWordImage(TexWordImage *dstImage, TexWordImage* srcImage, int x, int y, int sizeX, int sizeY, F32 rot, Color rgba[], int numColors, int blend, bool yield)
{

    if (sizeX == srcImage->sizeX && sizeY == srcImage->sizeY && rot==0 && !blend) // TODO: support blending on the other draw functions?
    {
        int srcy;
        int w = srcImage->sizeX;
        int h = srcImage->sizeY;
        int xoffs=0, yoffs=0;
        U8 *buffer = dstImage->buffer;

        if (x + w > dstImage->sizeX) {
            w = dstImage->sizeX - x;
        }
        if (y + h > dstImage->sizeY) {
            h = dstImage->sizeY - y;
        }
        if (x<0) {
            xoffs = -x;
            w-=xoffs;
        }
        if (y<0) {
            yoffs = -y;
        }
        if (numColors == 0)
        {
            lsprintf("Blt...");
            w*=4;
            for (srcy=yoffs; srcy<h; srcy++) {
                memcpy(&buffer[((y+srcy)*bufferSizeX + x+xoffs)<<2], &srcImage->buffer[srcy*srcImage->pitch + (xoffs<<2)], w);
                texWordsPixelsRendered(w, yield);
            }
            leprintf("done.");
        } else {
            Color interp;
            Color left, right;
            F32 horizw, vertw;
            lsprintf("BltColorized...");
            interp.integer = rgba[0].integer; // for 1-color mode
            for (srcy=yoffs; srcy<h; srcy++) {
                int srcx;
                Color *dst=(Color*)&buffer[((y+srcy)*bufferSizeX + x+xoffs)<<2];
                Color *src=(Color*)&srcImage->buffer[srcy*srcImage->pitch + (xoffs<<2)];
#define UL 2
#define UR 3
#define LR 0
#define LL 1
                if (numColors!=1) {
                    // four-color version
                    // THIS IS SLOW!  Could slope-walk the color values, MMX multiply them
                    // But, this is pretty much only used on loading screens, if anywhere
                    vertw = srcy/(F32)srcImage->sizeY;
                    left.r = rgba[UL].r*vertw + rgba[LL].r*(1-vertw);
                    left.g = rgba[UL].g*vertw + rgba[LL].g*(1-vertw);
                    left.b = rgba[UL].b*vertw + rgba[LL].b*(1-vertw);
                    left.a = rgba[UL].a*vertw + rgba[LL].a*(1-vertw);
                    right.r = rgba[UR].r*vertw + rgba[LR].r*(1-vertw);
                    right.g = rgba[UR].g*vertw + rgba[LR].g*(1-vertw);
                    right.b = rgba[UR].b*vertw + rgba[LR].b*(1-vertw);
                    right.a = rgba[UR].a*vertw + rgba[LR].a*(1-vertw);
                }
                if (numColors!=1) {
                    // four-color version
                    for (srcx=0; srcx<w; srcx++, dst++, src++) {
                        horizw = srcx/(F32)w;
                        interp.r = left.r * horizw + right.r * (1-horizw);
                        interp.g = left.g * horizw + right.g * (1-horizw);
                        interp.b = left.b * horizw + right.b * (1-horizw);
                        interp.a = left.a * horizw + right.a * (1-horizw);
                        dst->r = src->r*interp.r>>8;
                        dst->g = src->g*interp.g>>8;
                        dst->b = src->b*interp.b>>8;
                        dst->a = src->a*interp.a>>8;
                    }
                } else {
                    // single-color version
                    for (srcx=0; srcx<w; srcx++, dst++, src++) {
                        dst->r = src->r*interp.r>>8;
                        dst->g = src->g*interp.g>>8;
                        dst->b = src->b*interp.b>>8;
                        dst->a = src->a*interp.a>>8;
                    }
                }
                texWordsPixelsRendered(w*4, yield);
            }
            leprintf("done.");
        }
    } else {
        CDXSurface src, dst;
        RECT r;
        int destx, desty;
        double scalex, scaley;
        lsprintf("DrawRotoZoom...");
        r.left = 0;
        r.right = srcImage->sizeX;
        r.top = 0;
        r.bottom = srcImage->sizeY;
        src.buffer = srcImage->buffer;
        src.clipRect.top = 0;
        src.clipRect.bottom = srcImage->sizeY;
        src.clipRect.left = 0;
        src.clipRect.right = srcImage->sizeX;
        src.pitch = srcImage->pitch;
        dst.buffer = dstImage->buffer;
        dst.clipRect.top = 0;
        dst.clipRect.bottom = dstImage->sizeY;
        dst.clipRect.left = 0;
        dst.clipRect.right = dstImage->sizeX;
        dst.pitch = dstImage->pitch;

        destx = x + sizeX/2;
        desty = y + sizeY/2;
        if (rot) {
            // rotate the point destx, desty around x,y by Rot degrees
            rotatePointAroundPoint(&destx, &desty, x, y, rot);
        }
        scalex = sizeX/(double)srcImage->sizeX;
        scaley = sizeY/(double)srcImage->sizeY;
        DrawBlkRotoZoom(&src, &dst, destx, desty, &r, rot*PI/180, scalex, scaley, rgba, numColors, blend);
        leprintf("done.");
        texWordsPixelsRendered(srcImage->sizeX*srcImage->sizeY*4, yield);
    }
}

void renderTexWordLayerImageSoftware(PBName target, TexWordLayer *layer, int x, int y, int sizeX, int sizeY, int screenSizeX, int screenSizeY, F32 rot, bool yield)
{
    BasicTexture *image = layer->image->actualTexture;
    U8*    buffer = getBuffer(target);
    Color rgba[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
    int numColors;
    TexWordImage src, dest;

    uncompressRawTexInfo(image->rawInfo);

    numColors = cleanupColors(rgba, layer);

    src.buffer = image->rawInfo->data;
    src.sizeX = MIN(image->width, image->rawInfo->width); // not technically right...  some textures are lower res... this will break with -reducemip
    src.sizeY = MIN(image->height, image->rawInfo->height);
    src.pitch = image->rawInfo->width*4;
    dest.buffer = buffer;
    dest.pitch = bufferSizeX*4;
    dest.sizeX = screenSizeX;
    dest.sizeY = screenSizeY;
    renderTexWordImage(&dest, &src, x, y, sizeX, sizeY, rot, rgba, numColors, 0, yield);

}

void renderTexWordLayerImage(PBName target, TexWordLayer *layer, int x, int y, int sizeX, int sizeY, int screenSizeX, int screenSizeY, F32 rot, bool yield)
{
    if (texWords_useGl) {
        renderTexWordLayerImageGL(layer, x, y, sizeX, sizeY, screenSizeX, screenSizeY);
        texWordsPixelsRendered(sizeX*sizeY*4, yield);
    } else {
        memset(getBuffer(target), 0, bufferSizeX*bufferSizeY*4);
        texWordsPixelsRendered(sizeX*sizeY, yield);
        if (layer->stretch == TWLS_TILE) {
            int x0=x, y0=y;
            int xwalk, ywalk;
            while (x0 > 0) x0 -= sizeX;
            while (y0 > 0) y0 -= sizeY;
            for (ywalk=y0; ywalk< screenSizeY; ywalk+=sizeY) {
                for (xwalk=x0; xwalk < screenSizeX; xwalk+=sizeX) {
                    int x2=xwalk, y2=ywalk;
                    rotatePointAroundPoint(&x2, &y2, x, y, rot);
                    renderTexWordLayerImageSoftware(target, layer, x2, y2, sizeX, sizeY, screenSizeX, screenSizeY, rot, yield);
                }
            }
        } else {
            renderTexWordLayerImageSoftware(target, layer, x, y, sizeX, sizeY, screenSizeX, screenSizeY, rot, yield);
        }
    }
}

static TTFontManager* texWordsFontManager=NULL;
static void registerFont(TTCompositeFont *font)
{
    if (!texWordsFontManager) {
        texWordsFontManager = createTTFontManager();
    }
    ttFMAddFont(texWordsFontManager, font);

}

static void unregisterFont(TTCompositeFont *font)
{
    ttFMFreeFont(texWordsFontManager, font);
}

void renderTexWordLayerTextGL(TexWordLayer *layer, int x, int y, int sizeX, int sizeY, int screenSizeX, int screenSizeY, unsigned short *text, TTDrawContext *font, Color rgba[])
{
    int width, height;
    F32 xscale, yscale;
    int i;
    Color rgba2[4];

    rdrCheckThread();
    texWordsPixelsRendered(sizeX*sizeY, false);

    for (i=0; i<4; i++) {
        rgba2[i] = colorFlip(rgba[(i+3)%4]);
    }

    ttGetStringDimensionsWithScaling(font, 1.0, 1.0, text, wcslen(text), &width, &height, NULL, false);

    // Fit the text into the box
    // full stretch
    xscale = sizeX / (F32)width;
    yscale = sizeY / (F32)height;

    modelDrawState(DRAWMODE_SPRITE, ONLY_IF_NOT_ALREADY);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);
    ttDrawText2DWithScaling(
        font,
        x, // x
        screenSizeY - y - sizeY, // y
        0.0, // z
        xscale, // xscale
        yscale, // yscale
        (int*)rgba2,
        text, wcslen(text));

}

typedef struct TextRenderData {
    U8 * buffer;
    int sizeX, sizeY;
    int screenSizeX;
    int screenSizeY;
    int numColors;
    int x, y;
    F32 rot;
    bool yield;
} TextRenderData;

static void textRenderCallback(void *data, float top, float left, float sizeY, float sizeX, TTFontBitmap *glyph, int irgba[4])
{
    TextRenderData *renderData = (TextRenderData*)data;
    int x = left;
    int y = top;
    Color *rgba = (Color*)irgba;
    TexWordImage src, dest;

    src.buffer = glyph->bitmapRGBA;
    src.sizeX = glyph->bitmapInfo.width;
    src.sizeY = glyph->bitmapInfo.height;
    src.pitch = glyph->bitmapInfo.width*4;
    dest.buffer = renderData->buffer;
    dest.pitch = bufferSizeX*4;
    dest.sizeX = renderData->screenSizeX;
    dest.sizeY = renderData->screenSizeY;

    if (renderData->rot) {
        // Rotate the x and y location around the text origin
        rotatePointAroundPoint(&x, &y, renderData->x, renderData->y, renderData->rot);
        // Pass rotation down to DrawRotoZoom
    }

    renderTexWordImage(&dest, &src, x, y, sizeX, sizeY, renderData->rot, rgba, renderData->numColors, 1, renderData->yield);
}

void renderTexWordLayerTextSoftware(PBName target, TexWordLayer *layer, int x, int y, int sizeX, int sizeY, int screenSizeX, int screenSizeY, unsigned short *text, TTDrawContext *font, Color rgba[], int numColors, F32 rot, bool yield)
{
    U8* buffer = getBuffer(target);
    int width, height;
    F32 xscale, yscale;
    TextRenderData renderData;

    memset(buffer, 0x00, bufferSizeX*bufferSizeY*4);
    texWordsPixelsRendered(sizeX*sizeY, yield);

    font->renderParams.renderToBuffer = 1;
    ttGetStringDimensionsWithScaling(font, 1.0, 1.0, text, wcslen(text), &width, &height, NULL, false);

    // Fit the text into the box
    // full stretch
    xscale = sizeX / (F32)width;
    yscale = sizeY / (F32)height;

    renderData.sizeX = sizeX;
    renderData.sizeY = sizeY;
    renderData.buffer = buffer;
    renderData.screenSizeY = screenSizeY;
    renderData.screenSizeX = screenSizeX;
    renderData.numColors = numColors;
    renderData.yield = yield;
    renderData.x = x;
    renderData.y = y;
    renderData.rot = rot;

    ttDrawText2DWithScalingSoftware(
        font,
        x, // x
        y + sizeY, // y
        xscale, // xscale
        yscale, // yscale
        (int*)rgba,
        text, wcslen(text),
        textRenderCallback, &renderData);
}


void renderTexWordLayerText(PBName target, TexWordLayer *layer, int x, int y, int sizeX, int sizeY, int screenSizeX, int screenSizeY, F32 scaleX, F32 scaleY, BasicTexture *texBindParent, F32 rot, bool yield)
{
    char text[2048];
    unsigned short wstr[1024];
    char *effText=NULL;
    Color rgba[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
    TTDrawContext *fontDefault = &title_18;
    TTDrawContext *font = fontDefault;
    int numColors;
    F32 effScale = (scaleX + scaleY)/2;

    numColors = cleanupColors(rgba, layer);

    // Setup font
    if (layer->font.fontName) {
        static TTDrawContext drawContext;
        TTSimpleFont *chosenFont;
        memset(&drawContext, 0, sizeof(drawContext));
        font = &drawContext;
        // TODO: asynchronously do these (mostly File I/O, crit sect the FT calls)
        initTTDrawContext(font, layer->font.drawSize?layer->font.drawSize:(sizeY*2));
        chosenFont = ttFontLoadCached(layer->font.fontName, 0);
        // End async
        if (!chosenFont) {
            if (isDevelopmentMode())
                Errorf("Invalid font specified: %s", layer->font.fontName);
            font = fontDefault; // Error, font not found!
        } else {
            TTCompositeFont *compositeFont;
            compositeFont = createTTCompositeFont();
            ttFontAddFallback(compositeFont, chosenFont);
            ttFontAddFallback(compositeFont, ttMontBold);
            ttFontAddFallback(compositeFont, ttMingLiu);
            ttFontAddFallback(compositeFont, ttGulim);

            font->font = compositeFont;
            font->renderParams.italicize = layer->font.italicize;
            font->renderParams.bold = layer->font.bold;
            font->renderParams.dropShadow = layer->font.dropShadowXOffset||layer->font.dropShadowYOffset;
            font->renderParams.dropShadowXOffset = round(layer->font.dropShadowXOffset*scaleX);
            if (layer->font.dropShadowXOffset && !font->renderParams.dropShadowXOffset)
                font->renderParams.dropShadowXOffset = 1;
            font->renderParams.dropShadowYOffset = round(layer->font.dropShadowYOffset*scaleY);
            if (layer->font.dropShadowYOffset && !font->renderParams.dropShadowYOffset)
                font->renderParams.dropShadowYOffset = 1;
            font->renderParams.outline = !!layer->font.outlineWidth;
            font->renderParams.outlineWidth = layer->font.outlineWidth*effScale;
            if (layer->font.outlineWidth && !font->renderParams.outlineWidth)
                font->renderParams.outlineWidth = 1;
            font->renderParams.softShadow = !!layer->font.softShadowSpread;
            font->renderParams.softShadowSpread = round(layer->font.softShadowSpread*effScale);
            if (layer->font.softShadowSpread && !font->renderParams.softShadowSpread)
                font->renderParams.softShadowSpread = 1;
            font->dynamic = 1;
            registerFont(font->font);
        }
    }

    if (texBindParent->texWordParams) {
        int index = StaticDefineIntGetInt(TexWordParamLookup, layer->text);
        if (index >=0 && index < eaSize(&texBindParent->texWordParams->parameters)) {
            effText = texBindParent->texWordParams->parameters[index];
        }
    } 
    if (!effText) {
        effText = layer->text;
    }
    msPrintf(texWordsMessages, SAFESTR(text), effText);

    UTF8ToWideStrConvert(text, wstr, ARRAY_SIZE(wstr)-1);

    if (texWords_useGl) {
        renderTexWordLayerTextGL(layer, x, y, sizeX, sizeY, screenSizeX, screenSizeY, wstr, font, rgba);
    } else {
        renderTexWordLayerTextSoftware(target, layer, x, y, sizeX, sizeY, screenSizeX, screenSizeY, wstr, font, rgba, numColors, rot, yield);
    }

    if (font != fontDefault) {
        unregisterFont(font->font);
        destroyTTCompositeFont(font->font);
    }
}


static void drawQuad()
{
    glBegin( GL_QUADS );
    glTexCoord2f( 0, 1 );
    glVertex2f( 0 , bufferSizeY );
    glTexCoord2f( 1, 1 );
    glVertex2f( bufferSizeX, bufferSizeY );
    glTexCoord2f( 1, 0 );
    glVertex2f( bufferSizeX, 0 );
    glTexCoord2f( 0, 0 );
    glVertex2f( 0, 0 );
    glEnd(); CHECKGL;
}

#pragma warning(push)
#pragma warning(disable:4730)
static void blendLayers(U8* dest, U8* bottom, U8* top, TexWordBlendType blend, F32 topWeight, bool yield)
{
    Color t,b,*d;
    int i, j;
    U8 topWeightByte = topWeight * 255;
    U64 topWeightMMX;
    U8 invTopWeightByte = 255 - topWeightByte;
    U64 invTopWeightMMX;
    F32    float255 = 255.0f;
    if (blend == TWBLEND_REPLACE) {
        texWordsPixelsRendered(bufferSizeX*bufferSizeY, yield);
        memcpy(dest, bottom, bufferSizeX*bufferSizeY*4);
        return;
    }

    if (topWeight!=255) {
        topWeightMMX = topWeightByte<<16|topWeightByte;
        topWeightMMX<<=32;
        topWeightMMX |= topWeightByte<<16|topWeightByte;
        topWeightMMX<<=8;
        invTopWeightMMX = invTopWeightByte<<16|invTopWeightByte;
        invTopWeightMMX<<=32;
        invTopWeightMMX |= invTopWeightByte<<16|invTopWeightByte;
        invTopWeightMMX<<=8;
    }
    for (i=0; i<bufferSizeY; i++) { // height
        unsigned int *rowdest=(unsigned int*)&dest[bufferSizeX*4*i];
        unsigned int *rowbottom=(unsigned int*)&bottom[bufferSizeX*4*i];
        unsigned int *rowtop=(unsigned int*)&top[bufferSizeX*4*i];
        for (j=0; j<bufferSizeX; j++) {
            t.integer = rowtop[j];
            b.integer = rowbottom[j];
            d = (Color*)&rowdest[j];
            if (topWeightByte==0) {
                d->integer = b.integer;
                continue;
            }
            switch (blend) {
                case TWBLEND_OVERLAY:
                    if (t.a == 0) {
                        d->integer = b.integer;
                    } else if (t.a == 255) {
                        d->integer = t.integer;
                    } else {
                        F32 topAlpha = t.a/255.0;
                        F32 bottomAlpha = b.a/255.0;
                        F32 bottomAlphaFact = (1.0 - topAlpha)*bottomAlpha;
                        F32 finalAlpha = topAlpha + bottomAlphaFact;
                        F32 finalAlphaSaturateFact = 1.0/finalAlpha;
#if 0
                        // ftol calls happen here!
                        d->r = (topAlpha * t.r + bottomAlphaFact * b.r)*finalAlphaSaturateFact;
                        d->g = (topAlpha * t.g + bottomAlphaFact * b.g)*finalAlphaSaturateFact;
                        d->b = (topAlpha * t.b + bottomAlphaFact * b.b)*finalAlphaSaturateFact;
                        d->a = finalAlpha * 255;
#else
                        // Same thing but without ftol calls
                        // Also does rounding instead of truncating
                        // Doesn't actually seem to be any faster
                        DWORD temp0;
                        DWORD temp1;
                        // Load interesting values onto the stack
                        _asm {
                            fld        dword ptr[topAlpha]
                            fld        dword ptr[bottomAlphaFact]
                            fld        dword ptr[finalAlphaSaturateFact]
                            // Now, st(0) = finalAlphaSaturateFact, st(1) = bottomAlphaFact, st(2) = topAlpha
                        }
#define OVERLAY_BLEND_ASM(top, bot, out)                \
                        temp0=top; temp1=bot;            \
                                                        \
                        _asm {fild    dword ptr[temp0]}    \
                            /* Now, st(0) = t.r, st(1) = finalAlphaSaturateFact, st(2) = bottomAlphaFact, st(3) = topAlpha    */ \
                        _asm {fmul    st, st(3)}    /* top * topAlpha    */    \
                        _asm {fild    dword ptr[temp1]}    \
                            /* Now, st(0) = b.r, st(1) = top*topAlpha, etc */    \
                        _asm {fmul    st, st(3)}    /* bot * bottomAlphaFact */        \
                        _asm {faddp    st(1), st}    /* st = st(0) + st(1), pop */    \
                        _asm {fmul    st, st(1)}    /* *= finalAlphaSaturateFact */    \
                        _asm {fistp    dword ptr[temp0]}    \
                        out = temp0;

                        OVERLAY_BLEND_ASM(t.r, b.r, d->r);
                        OVERLAY_BLEND_ASM(t.g, b.g, d->g);
                        OVERLAY_BLEND_ASM(t.b, b.b, d->b);
                        _asm { // Remove excess stuff from the stack
                            fstp    st
                            fstp    st
                            fstp    st
                        }
                        //d->a = finalAlpha * 255;
                        _asm {
                            fld        dword ptr[finalAlpha]
                            fld        dword ptr[float255]
                            fmulp    st(1), st
                            fistp    dword ptr[temp0]
                        }
                        d->a = temp0;
#endif
                    }
                xcase TWBLEND_MULTIPLY:
#if 0
                    d->r = t.r*b.r >> 8;
                    d->g = t.g*b.g >> 8;
                    d->b = t.b*b.b >> 8;
                    d->a = t.a*b.a >> 8;
#else
                    // Same thing in MMX (~12% speedup)
                    __asm {
                        pxor mm0,mm0
                        punpcklbw mm0, dword ptr[t]
                        pxor mm1,mm1
                        punpcklbw mm1, dword ptr[b]
                        pmulhuw mm0, mm1
                        psraw mm0, 8
                        packsswb mm0, mm1
                        mov  edx,dword ptr [d]
                        movd dword ptr[edx], mm0
                    }
#endif
                xcase TWBLEND_ADD:
                    if (t.a==0) {
                        d->integer = b.integer;
                    } else if (b.a == 0) {
                        d->integer = t.integer;
                    } else {
                        // TODO: make this fast?
                        F32 topAlpha = t.a/255.0;
                        F32 bottomAlpha = b.a/255.0;
                        F32 finalAlpha = MIN(1.0, topAlpha + bottomAlpha);
                        if (finalAlpha==1.0) {
                            // Just add
                            d->r = MIN(t.r*topAlpha+b.r*bottomAlpha,255);
                            d->g = MIN(t.g*topAlpha+b.g*bottomAlpha,255);
                            d->b = MIN(t.b*topAlpha+b.b*bottomAlpha,255);
                        } else {
                            F32 invFinalAlpha=1.0/finalAlpha;
                            // Add and saturate
                            d->r = MIN((t.r*topAlpha+b.r*bottomAlpha)*invFinalAlpha,255);
                            d->g = MIN((t.g*topAlpha+b.g*bottomAlpha)*invFinalAlpha,255);
                            d->b = MIN((t.b*topAlpha+b.b*bottomAlpha)*invFinalAlpha,255);
                        }
                        d->a = finalAlpha*255;
                    }
                xcase TWBLEND_SUBTRACT:
                    d->r = MAX(b.r-t.r,0);
                    d->g = MAX(b.g-t.g,0);
                    d->b = MAX(b.b-t.b,0);
                    d->a = MAX(b.a-t.a,0);
                    break;
            }
            // TODO: is the loss of precision here (1.0*1.0 = 254/255) acceptable?
            if (topWeightByte!=255) {
#if 0
                d->r = (d->r * topWeightByte + b.r * invTopWeightByte) >> 8;
                d->g = (d->g * topWeightByte + b.g * invTopWeightByte) >> 8;
                d->b = (d->b * topWeightByte + b.b * invTopWeightByte) >> 8;
                d->a = (d->a * topWeightByte + b.a * invTopWeightByte) >> 8;
#else
                // MMX version
                __asm {
                    pxor mm0,mm0
                    mov  edx,dword ptr [d]
                    punpcklbw mm0, dword ptr[edx]
                    pxor mm1,mm1
                    movq mm1, qword ptr[topWeightMMX]
                    pmulhuw mm0, mm1
                    // mm0 contains d * topWeightByte in H.O.
                    pxor mm2,mm2
                    punpcklbw mm2, dword ptr[b]
                    pxor mm1,mm1
                    movq mm1, qword ptr[invTopWeightMMX]
                    pmulhuw mm2, mm1
                    // mm2 contains b * invTopWeightByte in W
                    paddusw mm0, mm2
                    // mm0 contains sum
                    psraw mm0, 8
                    packsswb mm0, mm1
                    mov  edx,dword ptr [d]
                    movd dword ptr[edx], mm0
                    emms
                }
#endif
            }
        }
        texWordsPixelsRendered(bufferSizeX*4, yield);
    }
    // Empty Machine State (reset FPU for FP ops instead of MMX)
    __asm {
        emms
    }
}
#pragma warning(pop)

static void filterNoop(U8* dest, U8* src, TexWordLayerFilter *filter, bool yield, F32 scale)
{
    Color s;
    int i, j;
    for (i=0; i<bufferSizeY; i++) {
        unsigned int *rowdest=(unsigned int*)&dest[bufferSizeX*4*i];
        unsigned int *rowsrc=(unsigned int*)&src[bufferSizeX*4*i];
        for (j=0; j<bufferSizeX; j++) {
            s.integer = rowsrc[j];
            rowdest[j] = s.integer;
        }
        texWordsPixelsRendered(bufferSizeX*4, yield);
    }
}

static void filterKernel(U8* dest, U8* src, S32 *kernel, int kwidth, int kheight, int offsx, int offsy, bool yield)
{
    int kx, ky, i, j, kmx = kwidth/2, kmy = kheight/2, x, y, ki, ybase, xbase;
    S32 r, g, b, a, w, pixw;
    register Color s;
    Color *d;
    for (i=0; i<bufferSizeY; i++) {
        unsigned int *rowdest=(unsigned int*)&dest[bufferSizeX*i<<2];
        ybase = i - kmy - offsy;
        for (j=0; j<bufferSizeX; j++) {
            xbase = j - kmx - offsx;
            r=g=b=a=w=0;
            ki=0;
            for (ky=0; ky<kheight; ky++) {
                y = ybase + ky;
                if (y>=0 && y<bufferSizeY) {
                    unsigned int *srcwalk = (unsigned int*)&src[(bufferSizeX*y + xbase)<<2];
                    for (kx=0; kx<kwidth; kx++, srcwalk++, ki++) {
                        x = xbase + kx;
                        if (x>=0 && x<bufferSizeX) {
                            pixw=kernel[ki];
                            if (pixw) {
                                s.integer = *srcwalk;
                                w+=pixw;
                                if (s.integer) {
                                    r+=pixw*s.r;
                                    g+=pixw*s.g;
                                    b+=pixw*s.b;
                                    a+=pixw*s.a;
                                }
                            }
                        }
                    }
                } else {
                    ki+=kwidth;
                }
            }
            d = (Color*)&rowdest[j];
            if (w==0) {
                d->integer = 0;
            } else {
                d->r = r/w;
                d->g = g/w;
                d->b = b/w;
                d->a = a/w;
            }
        }
        texWordsPixelsRendered(bufferSizeX*kwidth*kheight, yield);
    }
}

static int kernel_width;
static int *makeKernel(int magnitude)
{
    static int kernel[21*21] = {0};
    int mag = MIN(10, MAX(0, magnitude));
    int i, j;
    int dx, dy;
    kernel_width = mag*2+1;
    for (i=0; i<kernel_width; i++) { // y
        dy = mag - i;
        for (j=0; j<kernel_width; j++) { // x
            dx = mag - j;
            kernel[i*kernel_width+j] = MAX(0,round(mag + 1 - sqrt(dx*dx+dy*dy)));
        }
    }
    return kernel;
}

static void convKernel(int *intKernel, OUT F32 **fkernel, OUT U8 ** bkernelNotZero)
{
    static F32 kernel[21*21]={0};
    static U8 bkernel[21*21]={0};
    int i, j;
    int w=0;
    F32 invKernelWeight;
    for (i=0; i<kernel_width; i++) {
        for (j=0; j<kernel_width; j++) {
            w+=intKernel[i*kernel_width+j];
        }
    }
    invKernelWeight = 1.0/w;
    for (i=0; i<kernel_width; i++) {
        for (j=0; j<kernel_width; j++) {
            kernel[i*kernel_width+j] = intKernel[i*kernel_width+j]*invKernelWeight;
            bkernel[i*kernel_width+j] = kernel[i*kernel_width+j]!=0;
        }
    }
    *fkernel = kernel;
    *bkernelNotZero = bkernel;
}

static void filterBlur(U8* dest, U8* src, TexWordLayerFilter *filter, bool yield, F32 scale)
{
    int *kernel = makeKernel(filter->magnitude*scale);
    filterKernel(dest, src, kernel, kernel_width, kernel_width, filter->offset[0], -filter->offset[1], yield);
}

static void filterKernelColorize(U8* dest, U8* src, S32 *kernel, int kwidth, int kheight, int offsx, int offsy, Color color, U8 spread, bool yield)
{
    int kx, ky, i, j, kmx = kwidth/2, kmy = kheight/2, x, y, ki, ybase, xbase;
    S32 r, g, b, a, w, pixw;
    Color s, *d;
    F32 alphaScale = color.a/255.0;
    U8 invspread = 255-spread;
    // Slow function, not actually used

    texWordsPixelsRendered(bufferSizeX*bufferSizeY*kwidth*kheight, yield);

    for (i=0; i<bufferSizeY; i++) { // loop over destination pixels
        unsigned int *rowdest=(unsigned int*)&dest[bufferSizeX*i<<2];
        ybase = i - kmy - offsy;
        for (j=0; j<bufferSizeX; j++) {
            xbase = j - kmx - offsx;
            r=g=b=a=w=0;
            ki=0;
            for (ky=0; ky<kheight; ky++) {
                y = ybase + ky;
                if (y>=0 && y<bufferSizeY) {
                    unsigned int *srcwalk = (unsigned int*)&src[(bufferSizeX*y + xbase)<<2];
                    for (kx=0; kx<kwidth; kx++, srcwalk++, ki++) {
                        x = xbase + kx;
                        if (x>=0 && x<bufferSizeX) {
                            pixw=kernel[ki];
                            if (pixw) {
                                s.integer = *srcwalk;
                                w+=pixw;
                                if (s.a) {
                                    a+=pixw*s.a;
                                }
                            }
                        }
                    }
                } else {
                    ki+=kwidth;
                }
            }
            d = (Color*)&rowdest[j];
            if (w==0) {
                d->integer = 0;
            } else {
                d->integer = color.integer; // Write RGB (A gets overwritten)
                if (spread) {
                    U8 effa = a/w; // 0..255
                    if (effa >= invspread) {
                        if (a) {
                            d->a = alphaScale * 255;
                        } else {
                            d->a = 0;
                        }
                    } else {
                        // Map 0..invspread to 0..255
                        d->a = a*255/(w*invspread)*alphaScale;
                    }
                } else {
                    d->a = a*alphaScale/w;
                }
            }
        }
    }
}

// Applies a spread to the alpha, unpacks a buffer full of just alpha into rgba
static void filterSpreadColorUnpack(U8* buffer, U8 spread, Color c, bool yield)
{
    Color s;
    int i, j;
    U8 invspread = 255-spread;
    F32 alphaFactor = c.a/255.f;

    s.integer = c.integer;
    for (i=bufferSizeY-1; i>=0; i--) {
        unsigned int *rowout=(unsigned int*)&buffer[bufferSizeX*i<<2];
        U8 *rowsrc=&buffer[bufferSizeX*i];
        for (j=bufferSizeX-1; j>=0; j--) {
            s.a = rowsrc[j];
            if (s.a==0) {
                //s.integer = c0.integer;
            } else if (!spread) {
                // keep the alpha, copy the color
                //s.integer = (s.a<<24)|c0.integer;
                s.a = s.a * alphaFactor;
            } else {
                // spread and alpha
                if (s.a >= invspread) {
                    // alpha of max (255)
                    s.a = c.a;
                } else {
                    // Map 0..invspread to 0..alpha (255)
                    s.a = s.a*c.a/invspread;
                }
            }
            rowout[j] = s.integer;
        }
        texWordsPixelsRendered(bufferSizeX, yield);
    }
}

// Approximates the GL version (identical?)
static void filterKernelColorizeNoSpreadNoAlpha(U8* dest, U8* src, S32 *intKernel, int kwidth, int kheight, int offsx, int offsy, Color color, bool yield)
{
    int i, j, kmx = kwidth/2, kmy = kheight/2, x, y, ki, xbase, ymax, xmax;
    F32 *kernel;
    U8 *kernelNotZero;
    Color s;
    F32 alphaScale = color.a/255.0;
    Color baseColor = color;
    int pixelsPerLineCount = bufferSizeX*kwidth*kheight;
    baseColor.a = 0;

    lsprintf("zeroing...");
    // Set base color
    ZeroMemory(dest, bufferSizeX*bufferSizeY);
    leprintf("done.");
    lsprintf("the rest...");
    convKernel(intKernel, &kernel, &kernelNotZero);

    // walk over all source, and add to dest
    for (i=0; i<bufferSizeY; i++) {
        unsigned int *srcwalk=(unsigned int*)&src[bufferSizeX*i<<2];
        ymax = MIN(i - kmy - offsy + kheight, bufferSizeY);
        for (j=0; j<bufferSizeX; j++, srcwalk++) {
            s.integer = *srcwalk;
            if (!s.a)
                continue;
            y = i - kmy - offsy;
            ki = 0;
            xbase = j - kmx + offsx;
            xmax = xbase + kwidth;
            for (; y<ymax; y++) {
                if (y<0) {
                    ki+=kwidth;
                    continue;
                }
                for (x=xbase; x<xmax; x++, ki++) {
                    if (x<0 || x>=bufferSizeX)
                        continue;
                    if (kernelNotZero[ki]) {
                        F32 kw = kernel[ki];
                        // Orig: dest[(bufferSizeX*y + x)] += s.a*kw;
                        DWORD dwadd;
                        DWORD alpha = s.a;
                        _asm {
                            // fadd = falpha*kw;
                            fild    dword ptr[alpha]
                            fmul    dword ptr[kw]
                            // add = fadd;
                            fistp    dword ptr[dwadd] // Round to nearest
                        }
                        dwadd += dest[(bufferSizeX*y + x)];
                        if (dwadd > 255) {
                            dest[(bufferSizeX*y + x)] = 255;
                        } else {
                            dest[(bufferSizeX*y + x)] = dwadd;
                        }
                    }
                }
            }
        }
        texWordsPixelsRendered(pixelsPerLineCount, yield);
    }
    leprintf("done.");
}

static void filterKernelColorizeNoSpreadNoAlphaGL(U8* dest, U8* src, S32 *intKernel, int kwidth, int kheight, int offsx, int offsy, Color color, bool yield)
{
    int i, j;
    F32 *kernel;
    U8  *kernelNotZero;
    F32 ds, dt;
    F32 s0, t0;
    int tempid;
    int kmx = kwidth/2, kmy = kheight/2;

    rdrCheckThread();
    texWordsPixelsRendered(bufferSizeX*bufferSizeY*kwidth*kheight, yield);

    modelDrawState(DRAWMODE_SPRITE, ONLY_IF_NOT_ALREADY);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);

    glGenTextures (1, &tempid); CHECKGL;
    texBind(0, tempid);

    WCW_ActiveTexture(0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECKGL;
    if (rdr_caps.supports_anisotropy) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, rdr_view_state.texAnisotropic); CHECKGL;
    }
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, bufferSizeX, bufferSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, src ); CHECKGL;

    glClearColor(0, 0, 0, 0); CHECKGL;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECKGL;
    WCW_EnableBlend();
    WCW_BlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_ALPHA_TEST); CHECKGL;

    ds = 1.0/bufferSizeX;
    dt = 1.0/bufferSizeY;
    s0 = -offsx*ds;
    t0 = -offsy*dt;
    convKernel(intKernel, &kernel, &kernelNotZero);
    // Loop through all kernel entries and blend them with addative to the frame buffer
    for (j=0; j<kheight; j++) {
        for (i=0; i<kwidth; i++) {
            if (kernelNotZero[j*kwidth+i]) {
                float s = s0 + (i-kmx)*ds, t = t0 + (j-kmy)*dt;
                float u = 1 + s, v = 1 + t;

                WCW_Color4(255,255,255, 255*kernel[j*kwidth+i]);

                glBegin( GL_QUADS );
                glTexCoord2f( s, v );
                //glVertex2f( x, screenSizeY - y );
                glVertex2f( 0 , bufferSizeY );
                glTexCoord2f( u, v );
                //glVertex2f( x+sizeX, screenSizeY - y );
                glVertex2f( bufferSizeX, bufferSizeY );
                glTexCoord2f( u, t );
                //glVertex2f( x+sizeX, screenSizeY - (y+sizeY) );
                glVertex2f( bufferSizeX, 0 );
                glTexCoord2f( s, t );
                //glVertex2f( x, screenSizeY - (y+sizeY) );
                glVertex2f( 0, 0 );
                glEnd(); CHECKGL;
            }
        }
    }
    
    glDeleteTextures(1, &tempid); CHECKGL;
    texSetWhite(TEXLAYER_BASE);
    glReadPixels(0, 0, bufferSizeX, bufferSizeY, GL_ALPHA, GL_UNSIGNED_BYTE, dest); CHECKGL;
}


static void filterKernelColorizeFast(U8* dest, U8* src, S32 *kernel, int kwidth, int kheight, int offsx, int offsy, Color color, U8 spread, bool yield)
{
//    int i, j;
    U8 alpha = color.a;
    lsprintf("filter..");
    if (texWords_useGl) {
        filterKernelColorizeNoSpreadNoAlphaGL(dest, src, kernel, kwidth, kheight, offsx, offsy, color, yield);
    } else {
        filterKernelColorizeNoSpreadNoAlpha(dest, src, kernel, kwidth, kheight, offsx, offsy, color, yield);
    }
    leprintf("      done.");
    lsprintf("spread..");
    filterSpreadColorUnpack(dest, spread, color, yield);
    leprintf("done.");
}

static void filterDropShadow(U8* dest, U8* src, TexWordLayerFilter *filter, bool yield, F32 scale)
{
    int *kernel = makeKernel(filter->magnitude*scale);
    if (0)
    {
        // This version will be perfectly accurate, but much, much slower
        filterKernelColorize(dest, src, kernel, kernel_width, kernel_width, filter->offset[0], -filter->offset[1], makeColor(filter->rgba), filter->spread*255, yield);
    }
    else 
    {
        filterKernelColorizeFast(dest, src, kernel, kernel_width, kernel_width, filter->offset[0], -filter->offset[1], makeColor(filter->rgba), filter->spread*255, yield);
    }
}

static void filterDesaturate(U8* dest, U8* src, TexWordLayerFilter *filter, bool yield, F32 scale)
{
    int i, j;
    register Color s;
    Color *d;
    Vec3 hsv, rgb;
    F32 invmag = 1 - filter->percent;
    for (i=0; i<bufferSizeY; i++) {
        unsigned int *rowdest=(unsigned int*)&dest[bufferSizeX*i<<2];
        unsigned int *rowsrc =(unsigned int*)&src[bufferSizeX*i<<2];
        for (j=0; j<bufferSizeX; j++) {
            s.integer = rowsrc[j];
            rgb[0] = s.r/255.0;
            rgb[1] = s.g/255.0;
            rgb[2] = s.b/255.0;
            rgbToHsv(rgb, hsv);
            hsv[1]*= invmag;
            hsvToRgb(hsv, rgb);
            d = (Color*)&rowdest[j];
            d->r = rgb[0]*255;;
            d->g = rgb[1]*255;;
            d->b = rgb[2]*255;;
            d->a = s.a;
        }
        texWordsPixelsRendered(bufferSizeX*4, yield);
    }
}

static void filterLayer(U8* dest, U8* src, TexWordLayerFilter *filter, bool yield, F32 scaleX, F32 scaleY)
{
    int i;
    F32 effScale = (scaleX + scaleY)/2;
    struct {
        TexWordFilterType type;
        void (*filterFunc)(U8 *dest, U8 *src, TexWordLayerFilter *filter, bool yield, F32 scale);
    } mapping[] = {
        {TWFILTER_NONE, filterNoop},
        {TWFILTER_BLUR, filterBlur},
        {TWFILTER_DROPSHADOW, filterDropShadow},
        {TWFILTER_DESATURATE, filterDesaturate},
    };

    for (i=0; i<ARRAY_SIZE(mapping); i++) {
        if (mapping[i].type == filter->type) {
            mapping[i].filterFunc(dest, src, filter, yield, effScale);
            return;
        }
    }
    if (isDevelopmentMode()) {
        Errorf("Invalid filter function!");
    }
    filterNoop(dest, src, filter, yield, effScale);
}

static int g_layernum;
void renderTexWordLayer(PBName destBuf, TexWordLayer *layer, int screenPosX, int screenPosY, int screenSizeX, int screenSizeY, F32 scaleX, F32 scaleY, BasicTexture *texBindParent, bool yield)
{
    static int debug_colors[] = {
        0xff0000ff,
        0xff007fff,
        0xff00ffff,
        0xff00ff00,
        0xffff0000,
        0xffff00ff,
    };
    int sizeX;
    int sizeY;
    int x, y, i;
    PBName renderMeInto = destBuf;
    bool renderSubLayer = layer->sublayer && layer->sublayer[0] && !layer->sublayer[0]->hidden;
    bool filter = eaSize(&layer->filter)>0;

    if (layer->hidden) {
        memset(getBuffer(destBuf), 0, 4*bufferSizeX*bufferSizeY);
        return;
    }

    if (renderSubLayer) {
        lsprintf("getBuffer...");
        getBuffer(renderMeInto);
        getBuffer(PB_SUBLAYER);
        getBuffer(PB_THISLAYER);
        leprintf("done.");

        lsprintf("Rendering sub-layer...");
        renderTexWordLayer(PB_SUBLAYER, layer->sublayer[0], screenPosX, screenPosY, screenSizeX, screenSizeY, scaleX, scaleY, texBindParent, yield);
        renderMeInto = PB_THISLAYER;
        leprintf("    done rendering sub-layer.");
    } else {
        lsprintf("getBuffer...");
        getBuffer(renderMeInto);
        leprintf("done.");
    }

    if (layer->size[0])
        sizeX = layer->size[0]*scaleX;
    else if (layer->image)
        sizeX = layer->image->width*scaleX;
    else
        sizeX = screenSizeX;
    if (layer->size[1])
        sizeY = layer->size[1]*scaleY;
    else if (layer->image)
        sizeY = layer->image->height*scaleY;
    else
        sizeY = screenSizeY;

    if (layer->pos[0])
        x = layer->pos[0]*scaleX;
    else
        x = screenPosX;
    if (layer->pos[1])
        y = layer->pos[1]*scaleY;
    else
        y = screenPosY;

    lsprintf("Drawing...");
    if (texWords_useGl) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECKGL;
    } else {
        memset(getBuffer(renderMeInto), 0, 4*bufferSizeX*bufferSizeY);
    }

    if (texWords_useGl && layer->rot) {
        Vec3 rot;
        Vec3 pos;
        Vec3 pos2;
        Mat3 mat;
        F32 x2, y2;
        glPushMatrix(); CHECKGL;

        // Move upper left corner based on rotation
        x2 = x;
        y2 = screenSizeY - y;
        rot[0] = rot[1] = 0;
        rot[2] = -layer->rot*PI/180;
        createMat3PYR(mat, rot);
        setVec3(pos, x2, y2, 0);
        mulVecMat3(pos, mat, pos2);
        glTranslatef(x2-pos2[0], y2-pos2[1], 0); CHECKGL;
        glRotatef(layer->rot, 0, 0, 1); CHECKGL;
    } else {
        // NON-GL Done in sub-functions
    }

    switch(layer->type) {
        case TWLT_BASEIMAGE:
        case TWLT_IMAGE:
            if (texWords_useGl) {
                WCW_BlendFunc(GL_ONE, GL_ZERO); // Extract the full texture values
                glDisable(GL_ALPHA_TEST); CHECKGL;
            }
            switch(layer->stretch) {
                case TWLS_FULL:
                    renderTexWordLayerImage(renderMeInto, layer, screenPosX, screenPosY, screenSizeX, screenSizeY, screenSizeX, screenSizeY, layer->rot, yield);
                    break;
                case TWLS_NONE:
                case TWLS_TILE:
                    renderTexWordLayerImage(renderMeInto, layer, x, y, sizeX, sizeY, screenSizeX, screenSizeY, layer->rot, yield);
                    break;
            }
            break;
        case TWLT_TEXT:
            if (texWords_useGl) {
                //WCW_BlendFunc(GL_SRC_ALPHA_SATURATE, GL_DST_ALPHA); // "top" to "bottom" rendered
                glEnable(GL_ALPHA_TEST); CHECKGL;

                WCW_BlendFunc(GL_ONE, GL_ONE); // Extract the full texture values
                //glDisable(GL_ALPHA_TEST);
            }
            renderTexWordLayerText(renderMeInto, layer, x, y, sizeX, sizeY, screenSizeX, screenSizeY, scaleX, scaleY, texBindParent, layer->rot, yield);
            break;
    }

    if (0) {
        modelDrawState(DRAWMODE_COLORONLY, ONLY_IF_NOT_ALREADY);
        modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);
        glLineWidth( 3.0 ); CHECKGL;
        glDisable(GL_ALPHA_TEST); CHECKGL;
        if (1) {
            WCW_ColorU32(debug_colors[g_layernum%ARRAY_SIZE(debug_colors)]);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x,screenSizeY-y);
            glVertex2f(x+sizeX,screenSizeY-(y+sizeY));
            glVertex2f(x+sizeX,screenSizeY-y);
            glVertex2f(x,screenSizeY-(y+sizeY));
            glEnd(); CHECKGL;
        }
    }

    if (texWords_useGl && layer->rot) {
        glPopMatrix(); CHECKGL;
    }

    if (0) {
        if (1) {
            WCW_ColorU32(debug_colors[g_layernum%ARRAY_SIZE(debug_colors)]);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x,screenSizeY-y);
            glVertex2f(x+sizeX,screenSizeY-(y+sizeY));
            glVertex2f(x+sizeX,screenSizeY-y);
            glVertex2f(x,screenSizeY-(y+sizeY));
            glEnd(); CHECKGL;
        }
    }

    leprintf("    done.");
    if (texWords_useGl) {
        lsprintf("glReadPixels() ");
        glReadPixels(screenPosX, screenPosY, bufferSizeX, bufferSizeY, GL_RGBA, GL_UNSIGNED_BYTE, getBuffer(renderMeInto)); CHECKGL;
        leprintf("done.");
    }

    if (renderSubLayer) {
        lsprintf("Blending with sub-layer (%s)...", StaticDefineIntRevLookup(ParseTexWordBlendType, layer->subBlend));
        blendLayers(getBuffer(destBuf), getBuffer(renderMeInto), getBuffer(PB_SUBLAYER), layer->subBlend, layer->subBlendWeight, yield);
        leprintf("done.");
    }
    // Perform after effect filtering
    for (i=0; i<eaSize(&layer->filter)>0; i++)
    {
        PBName tempbuf = (destBuf == PB_THISLAYER)?PB_SUBLAYER:PB_THISLAYER;
        lsprintf("Filtering...");
        filterLayer(getBuffer(tempbuf), getBuffer(destBuf), layer->filter[i], yield, scaleX, scaleY);
        leprintf("    done.");
        lsprintf("Blending with filter (%s)...", StaticDefineIntRevLookup(ParseTexWordBlendType, layer->filter[i]->blend));
        blendLayers(getBuffer(destBuf), getBuffer(tempbuf), getBuffer(destBuf), layer->filter[i]->blend, 1.0, yield);
        leprintf("done.");
    }
}

static void texWordDoComposition(TexWord *texWord, BasicTexture *texBindParent, bool yield)
{    // Let's do it!
    int use_pbuffer=((rdr_caps.allowed_features & GFXF_BUMPMAPS) && systemSpecs.videoCardVendorID!=VENDOR_INTEL) ?1:0; // GF2s PBuffers are *slow*, *slow*, *slow*
    int desired_aa=4;
    int required_aa=1;
    int sizeX, sizeY;
    int screenX, screenY;
    int actualSizeX, actualSizeY;
    int ULx, ULy;
    F32 scaleX=1.0, scaleY=1.0;
    U8 * pixbuf;
    extern bool g_doingHeadShot;
    extern PBuffer pbufHeadShot; // Share a pbuffer with the headShot system
    int i, j;
    int numlayers = eaSize(&texWord->layers);

    memlog_printf(NULL, "twDoComposition()        %s", texBindParent->name);

    if (game_state.texWordVerbose)
        setLoadTimingPrecistion(3);

    checkEverythingIsLoaded(texWord, texBindParent);

    if (!yield)
        // If the threaded renderer is rendering something and pausing to finish,
        // we just want it to finish ASAP so we can render something in the foreground
        texWord_doNotYield = true;
    EnterCriticalSection(&criticalSectionDoingTexWordRendering);
    if (!yield)
        texWord_doNotYield = false;
    lsprintf("Compositing texture...");

    use_pbuffer= 1; // Non-pbuffer not supported

    // 0. Misc init

    sizeX = texWord->width;
    sizeY = texWord->height;
    if (sizeX==0)
        sizeX = texBindParent->width;
    if (sizeY==0)
        sizeY = texBindParent->height;
    if (sizeX==0)
        sizeX = 128;
    if (sizeY==0)
        sizeY = 128; // Default fallback

    if (game_state.actualMipLevel && !(texBindParent->texopt_flags & TEXOPT_NOMIP)) {
        int mipLevel = texGetNumMipsToSkip(sizeX, sizeY, 0);
        for (i = 0; i < mipLevel; i++)
        {
            if (sizeX > game_state.reduce_min) {
                sizeX /= 2;
                scaleX /= 2;
            }
            if (sizeY > game_state.reduce_min) {
                sizeY /= 2;
                scaleY /= 2;
            }
        }
    }
    ULx = 0;
    ULy = 0;
    actualSizeX = 1 << log2i(sizeX);
    actualSizeY = 1 << log2i(sizeY);

    if (texWords_useGl) {
        lsprintf("Initializing...");

        // Hides various UI elements, FPS, etc
        g_doingHeadShot = true;

        // 1. Create pbuffer
        if (use_pbuffer) {
            initHeadShotPbuffer(actualSizeX, actualSizeY, desired_aa, required_aa);
            assert(pbufHeadShot.software_multisample_level<=1); // Not supported otherwise
//            scaleX = scaleY = pbufHeadShot.software_multisample_level;
//            // TODO: trickle a scale down or no software AA...
//            sizeX *= scaleX;
//            sizeY *= scaleY;

        } else {
            // This code does NOT work
            windowSize( &screenX, &screenY );
            ULx = ((F32)screenX)/2.0 - (F32)sizeX / 2.0;
            ULy = ((F32)screenY)/2.0 - (F32)sizeY / 2.0;
            if (ULx < 0) {
                // Image is larger than the visible screen
                ULx = 0;
                sizeX = screenX;
                // TODO: trickle a scale down
            }
            if (ULy < 0) {
                // Image is larger than the visible screen
                ULy = 0;
                sizeY = screenY;
                // TODO: trickle a scale down
            }

            actualSizeX = 1 << log2i(sizeX);
            actualSizeY = 1 << log2i(sizeY);
        }

        glClearColor(0,0,0,0); CHECKGL;
        rdrSetup2DRendering();
        //WCW_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        WCW_BlendFunc(GL_SRC_ALPHA_SATURATE, GL_DST_ALPHA); // "top" to "bottom" rendered
        //WCW_BlendFunc(GL_ONE, GL_ZERO); // Extract the full texture values
        leprintf("done.");
    }

    bufferSetSize(actualSizeX, actualSizeY);

    // 2. Go through layers and draw to pbuffer
    for (i=0; i<numlayers; i++) {
        g_layernum=i;
        lsprintf("Rendering layer #%d...", i);
        renderTexWordLayer(PB_THISLAYER, texWord->layers[i], ULx, ULy, sizeX, sizeY, scaleX, scaleY, texBindParent, yield);
        leprintf("  done rendering layer.");
        // Blend with last layer
        if (pixbuffers[PB_LASTLAYER]) {
            lsprintf("Blending layer #%d with previous (%s)...", i, StaticDefineIntRevLookup(ParseTexWordBlendType, TWBLEND_OVERLAY));
            blendLayers(getBuffer(PB_LASTLAYER), getBuffer(PB_THISLAYER), getBuffer(PB_LASTLAYER), TWBLEND_OVERLAY, 1.0, yield);
            leprintf("done.");
        } else {
            swapBuffers(PB_LASTLAYER, PB_THISLAYER);
        }
    }
    lsprintf("Finalizing...");
    if (texWords_useGl) {
        // Reset
        WCW_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST); CHECKGL;
        rdrFinish2DRendering();
    }
    
    //windowUpdate();

    // 3. Extract image to memory
    pixbuf = pixbuffers[PB_LASTLAYER];
    if (texWords_useGl) {
        lsprintf("Post-processing (flip)...");
        // 3b. Flip and process image
        for (i=0; i<sizeY/2; i++) { // height
            Color t1,t2;
            unsigned int *rowtop=(unsigned int*)&pixbuf[actualSizeX*4*i];
            unsigned int *rowbot=(unsigned int*)&pixbuf[actualSizeX*4*(sizeY - i - 1)];
            for (j=0; j<sizeX; j++) {
                t1.integer=rowtop[j];
                t2.integer=rowbot[j];
                rowtop[j]=t2.integer;
                rowbot[j]=t1.integer;
            }
        }
        leprintf("done.");
    }


    if (texWords_useGl) {
        if (use_pbuffer) {
            winMakeCurrent();
        }
        // Cleanup
        glClearColor( 0.0, 0.0, 0.0, 0.0 ); CHECKGL;
        gfxWindowReshape(); //Restore

        g_doingHeadShot = false;
    }

    // Package up data for main thread to process
    assert(!texBindParent->texWordLoadInfo);
    texBindParent->texWordLoadInfo = createTexWordLoadInfo();
    texBindParent->texWordLoadInfo->data = pixbuf;
    pixbuffers[PB_LASTLAYER] = NULL;
    texBindParent->texWordLoadInfo->actualSizeX = actualSizeX;
    texBindParent->texWordLoadInfo->actualSizeY = actualSizeY;
    texBindParent->texWordLoadInfo->sizeX = sizeX/scaleX;
    texBindParent->texWordLoadInfo->sizeY = sizeY/scaleY;

    freeBuffers();

    leprintf("  done.");
    leprintf("done.");
    unloadDataAfterComposition(texWord);
    LeaveCriticalSection(&criticalSectionDoingTexWordRendering);
    if (texWord_doNotYield) {
        // We are in the background thread and the foreground requested we quickly speed through things to give
        //  it control, let's sleep for a ms to yield to the foreground
        Sleep(1);
    }
}

static void texWordSendToGL(TexWord *texWord, BasicTexture *texBindParent)
{
    RdrTexParams    rtex = {0};

    memlog_printf(NULL, "twSendToGL()            %s", texBindParent->name);
    lsprintf("Sending new texture to GL...");
    assert(texBindParent->texWordLoadInfo);

    // 4. Free base texture
    texFree(texBindParent, 0);

    rtex.id = texBindParent->id = rdrAllocTexHandle();
    rtex.clamp_s = rtex.clamp_t = 1;
    // TODO: we might need to fix up the width/height values here, but do we want to just trick the game into
    //  thinking this is the old textures resolution instead?
    if (game_state.texWordVerbose && game_state.texWordEdit)
        rtex.point_sample = 1;    // for debugging
    else if (!(texBindParent->texopt_flags & TEXOPT_NOMIP))
        rtex.mipmap = 1;

    rtex.src_format    = GL_RGBA;
    rtex.dst_format = texGetFormat(texBindParent);
    rtex.width        = texBindParent->texWordLoadInfo->actualSizeX;
    rtex.height        = texBindParent->texWordLoadInfo->actualSizeY;
    // Fix resolution of parent TexBind
    {
        TexBind *texBindComposite = texFindComposite(texBindParent->name);
        if (texBindComposite) {
            texBindComposite->width = texBindParent->texWordLoadInfo->sizeX;
            texBindComposite->height = texBindParent->texWordLoadInfo->sizeY;
        }
    }

    // These aren't really used anymore?  Just the change on the TexBind above.
    // Save old resolution (restored in texFree)
    texBindParent->origWidth = texBindParent->width;
    texBindParent->origHeight = texBindParent->height;
    // Put in new resolution
    texBindParent->width = texBindParent->texWordLoadInfo->sizeX;
    texBindParent->height = texBindParent->texWordLoadInfo->sizeY;

    texBindParent->realWidth = texBindParent->texWordLoadInfo->actualSizeX;
    texBindParent->realHeight = texBindParent->texWordLoadInfo->actualSizeY;

    rdrTexCopy(&rtex,texBindParent->texWordLoadInfo->data,rtex.width * rtex.height * 4);
    // Free buffer
    destroyTexWordLoadInfo(texBindParent->texWordLoadInfo);
    texBindParent->texWordLoadInfo = NULL;

    leprintf("done.");
}


typedef struct TexWordThreadPackage {
    TexWord *texWord;
    BasicTexture *texBindParent;
    bool yield;
    TexThreadPackage *texPkg;
} TexWordThreadPackage;

static VOID CALLBACK texWordDoThreadedWorkSub( TexWordThreadPackage *twPkg)
{
    PERFINFO_AUTO_START("texWordDoThreadedWorkSub", 1);
        PERFINFO_AUTO_START("texWordDoComposition", 1);
            texWordDoComposition(twPkg->texWord, twPkg->texBindParent, twPkg->yield);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("other", 1);
            EnterCriticalSection(&CriticalSectionTexLoadQueues); 
            listAddForeignMember(&texBindsReadyForFinalProcessing, twPkg->texPkg);
            LeaveCriticalSection(&CriticalSectionTexLoadQueues);
            InterlockedDecrement(&numTexWordsInThread);
            free(twPkg);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

int texWordDoThreadedWork(TexThreadPackage *pkg, bool yield)
{
    TexWord *texWord=pkg->bind->texWord;
    BasicTexture *texBindParent=pkg->bind;
    TexWordThreadPackage *twPkg;

    if (texWords_useGl || !texWords_multiThread) {
        return 0;
    }

    twPkg = calloc(sizeof(TexWordThreadPackage),1);
    twPkg->texWord = texWord;
    twPkg->texBindParent = texBindParent;
    twPkg->yield = yield;
    twPkg->texPkg = pkg;
    InterlockedIncrement(&numTexWordsInThread);

    // Do software composition
    // queue this in another thread (or do it immediately in the case of a foreground load)
    if (yield) {
        QueueUserAPC((PAPCFUNC)texWordDoThreadedWorkSub, background_renderer_handle, (ULONG_PTR)twPkg);
    } else {
        // Do it now!
        texWordDoThreadedWorkSub(twPkg);
    }
    return 1;
}

void texWordDoFinalComposition(TexWord *texWord, BasicTexture *texBindParent)
{
    memlog_printf(NULL, "twDoFinalComposition()    %s", texBindParent->name);
    if (texWords_useGl || !texWords_multiThread) {
        PERFINFO_AUTO_START("texWordDoComposition", 1);
            texWordDoComposition(texWord, texBindParent, false);
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_START("texWordSendToGL", 1);
        texWordSendToGL(texWord, texBindParent);
    PERFINFO_AUTO_STOP();
}
