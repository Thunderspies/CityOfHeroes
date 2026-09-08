#include "texWordsEdit.h"
#include "texWordsPrivate.h"
#include "render/texWords.h"
#include "render/tex.h"
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/utils/mathutil.h>
//#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "entity/entclient.h"
#include "graphics/font.h"
#include "graphics/ttFontUtil.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiKeybind.h"
#include "UI/uiConsole.h"
#include "UI/uiCursor.h"
#include "UI/uiInput.h"
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "render/renderUtil.h"
#include "win/win_init.h"
#include "graphics/gfxwindow.h"
#include <utilitieslib/utils/FolderCache.h>
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include <utilitieslib/language/MessageStore.h>
#include "graphics/truetype/ttFontDraw.h"
#include "language/langClientUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/components/StringCache.h>
#include <sys/stat.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/components/MRUList.h>

#include "edit/edit_pickcolor.h"
#include "edit/edit_select.h"
#include "edit/edit_cmd.h"
#include "render/renderprim.h"

#include "graphics/textureatlas.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entDebugPrivate.h"
#include "entity/entDebug.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/strings_opt.h>

// 0 - white
// 1 - red
// 2 - green
// 3 - pale red
// 4 - pale orange (layer types)
// 5 - blue (enums)
// 6 - orange
// 7 - yellow (layer names)
// 8 - cyan (Edit Extents)
// 9 - purple (menu color)

typedef enum ExtentsMask {
    MASK_X0 = 1<<0,
    MASK_Y0 = 1<<1,
    MASK_X1 = 1<<2,
    MASK_Y1 = 1<<3,
    MASK_MOVE = 1<<4,
} ExtentsMask;

typedef struct TexWordsEditorState {
    int stretch;
    int orig;
    BasicTexture *bind;
    TexWord *texWord;
    F32 bgcolor[4];
    Color selcolor;
    int needToRebuild;
    int reOpenMenu;
    int lbutton; // left mouse button state
    int shift; // shift key state
    struct TexWordsEditorStateExtentsEdit {
        int on;
        int edit;
        int index;
        F32 x0, y0, x1, y1;
        int mousegrabbed;
        ExtentsMask mask; // Mask of what corners are grabbed
    } extents;
    struct {
        int width, height;
        int offsx, offsy;
        F32 scalex, scaley;
    } display;
    int colorpick;

    int readonly;
    int dirty;
    char lastStatus[1024];
    U32 lastStatusFading;
    int fromEditor;
    int requestQuit;
    char tweLocale[256];
    int hideText;
} TexWordsEditorState;

TexWordsEditorState tweditor_state = {0};

enum
{
    CMDTW_QUIT= 1,
    CMDTW_MENU,
    CMDTW_CREATE,
    CMDTW_LAYERINSERT,
    CMDTW_LAYERCLONE,
    CMDTW_LAYERCOPYALL,
    CMDTW_LAYERPASTEALL,
    CMDTW_LAYERSWAP,
    CMDTW_LAYERDELETE,
    CMDTW_LAYERTYPE,
    CMDTW_LAYERNAME,
    CMDTW_LAYERHIDE,
    CMDTW_LAYERUNHIDEALL,
    CMDTW_LAYERSOLO,
    CMDTW_LAYERSTRETCH,
    CMDTW_LAYERIMAGE,
    CMDTW_LAYERTEXTCHANGE,
    CMDTW_LAYERASK,
    CMDTW_LAYERCOPYSIZE,
    CMDTW_LAYERCOPYPOS,
    CMDTW_LAYERCOPYROT,
    CMDTW_LAYEREXTENTS,
    CMDTW_LAYERCOLORMODE,
    CMDTW_LAYERCOLORCHANGE,
    CMDTW_LAYERALPHACHANGE,
    CMDTW_LAYERFONT,
    CMDTW_LAYERFONTSTYLE,
    CMDTW_LAYERSUBINSERT,
    CMDTW_LAYERSUBBLEND,
    CMDTW_LAYERSUBBLENDWEIGHT,
    CMDTW_LAYERFILTERINSERT,
    CMDTW_LAYERFILTERDELETE,
    CMDTW_LAYERFILTERTYPE,
    CMDTW_LAYERFILTERBLEND,
    CMDTW_LAYERFILTERSTYLE,
    CMDTW_LAYERFILTERCOLOR,
    CMDTW_LAYERFILTERALPHA,
    CMDTW_SIZE,
    CMDTW_FILESAVE,
    CMDTW_FILESAVEDYNAMIC,
    CMDTW_FILELOAD,
    CMDTW_FILELOADNAME,
    CMDTW_FILELOADNEXT,
    CMDTW_FILESAVELIST,
    CMDTW_FILEPRUNE,
    CMDTW_FILENEW,
    CMDTW_FILECHECKOUT,
    CMDTW_FILECHECKIN,
    CMDTW_REFRESH,
    CMDTW_LBUTTON,
    CMDTW_SHIFT,
    CMDTW_APPLY,
    CMDTW_UNDO,
    CMDTW_REDO,
    CMDTW_COLOR,
    CMDTW_COLORPICK,
};

KeyBindProfile        texwordseditor_binds_profile;
TexBind *drawMeBind=NULL;

extern void texLoadInternalBasic(BasicTexture *bind, TexLoadHow mode, TexUsage use_category, int rawData);

int texWordsEditorCmdParse(char *str, int x, int y);
static void texWordsEditorSetup(void);
static void texWordsEdit_layerExtentsUnApply(void);
static void texWordsEdit_create(char *path);
char *textureToBaseTexWord(char *texturePath);
char *baseTexWordToTexture(char *texwordPath);

static char *cursorImage=NULL;
static char *cursorImageOverlay=NULL;

static TexWord layerClipboard={0};

MRUList *mruTextures;
MRUList *mruColors;

static char tmp_str[1024], tmp_str2[1024];
static int layer_index, layer_val, layer_filter_index;
static int tmp_color[4], tmp_int, tmp_int2;

Cmd texwordseditor_cmds[] =
{
    { 0, "stretch",            0, {{CMDINT(tweditor_state.stretch)}} },
    { 0, "colorpick",        CMDTW_COLORPICK, {{CMDINT(tweditor_state.colorpick)}} },
    { 0, "orig",            CMDTW_REFRESH, {{CMDINT(tweditor_state.orig)}} },
    { 0, "quit",            CMDTW_QUIT, {{CMDINT(tmp_int)}} },
    { 0, "menu",            CMDTW_MENU, {{0}} },
    { 0, "create",            CMDTW_CREATE, {{CMDSENTENCE(tmp_str)}} },
    { 0, "layerinsert",        CMDTW_LAYERINSERT, {{CMDINT(layer_index)}} },
    { 0, "layerclone",        CMDTW_LAYERCLONE, {{CMDINT(layer_index)}} },
    { 0, "layercopyall",    CMDTW_LAYERCOPYALL, {{0}} },
    { 0, "layerpasteall",    CMDTW_LAYERPASTEALL, {{0}} },
    { 0, "layerswap",        CMDTW_LAYERSWAP, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layerdelete",        CMDTW_LAYERDELETE, {{CMDINT(layer_index)}} },
    { 0, "layertype",        CMDTW_LAYERTYPE, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layername",        CMDTW_LAYERNAME, {{CMDINT(layer_index)}} },
    { 0, "layerhide",        CMDTW_LAYERHIDE, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layerunhideall",    CMDTW_LAYERUNHIDEALL, {{0}} },
    { 0, "layersolo",        CMDTW_LAYERSOLO, {{CMDINT(layer_index)}}  },
    { 0, "layerstretch",    CMDTW_LAYERSTRETCH, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layerimage",        CMDTW_LAYERIMAGE, {{CMDINT(layer_index)}, {CMDSTR(tmp_str)}} },
    { 0, "layertextchange",    CMDTW_LAYERTEXTCHANGE, {{CMDINT(layer_index)}} },
    { 0, "layerask",        CMDTW_LAYERASK, {{CMDINT(layer_index)}, { CMDSTR(tmp_str)}} },
    { 0, "layercopysize",    CMDTW_LAYERCOPYSIZE, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layercopypos",    CMDTW_LAYERCOPYPOS, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layercopyrot",    CMDTW_LAYERCOPYROT, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layerextents",    CMDTW_LAYEREXTENTS, {{CMDINT(layer_index)}} },
    { 0, "layercolormode",    CMDTW_LAYERCOLORMODE, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layercolorchange",CMDTW_LAYERCOLORCHANGE, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layeralphachange",CMDTW_LAYERALPHACHANGE, {{CMDINT(layer_index)}, {CMDINT(layer_val)}, {CMDSTR(tmp_str)}} },
    { 0, "layerfont",        CMDTW_LAYERFONT, {{CMDINT(layer_index)}, {CMDSTR(tmp_str)}} },
    { 0, "layerfontstyle",    CMDTW_LAYERFONTSTYLE, {{CMDINT(layer_index)}, {CMDSTR(tmp_str)}, {CMDINT(layer_val)}} },
    { 0, "layersubinsert",    CMDTW_LAYERSUBINSERT, {{CMDINT(layer_index)}} },
    { 0, "layersubblend",    CMDTW_LAYERSUBBLEND, {{CMDINT(layer_index)}, {CMDINT(layer_val)}} },
    { 0, "layersubblendweight",    CMDTW_LAYERSUBBLENDWEIGHT, {{CMDINT(layer_index)}, {CMDSTR(tmp_str)}} },
    { 0, "layerfilterinsert",    CMDTW_LAYERFILTERINSERT, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}} },
    { 0, "layerfilterdelete",    CMDTW_LAYERFILTERDELETE, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}} },
    { 0, "layerfiltertype",        CMDTW_LAYERFILTERTYPE, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}, {CMDINT(layer_val)}} },
    { 0, "layerfilterblend",    CMDTW_LAYERFILTERBLEND, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}, {CMDINT(layer_val)}} },
    { 0, "layerfilterstyle",    CMDTW_LAYERFILTERSTYLE, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}, {CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}} },
    { 0, "layerfiltercolor",    CMDTW_LAYERFILTERCOLOR, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}} },
    { 0, "layerfilteralpha",    CMDTW_LAYERFILTERALPHA, {{CMDINT(layer_index)}, {CMDINT(layer_filter_index)}, {CMDSTR(tmp_str)}} },
    { 0, "size",            CMDTW_SIZE, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}} },
    { 0, "bgcolor",            0, {{PARSETYPE_FLOAT, &tweditor_state.bgcolor[0]}, { PARSETYPE_FLOAT, &tweditor_state.bgcolor[1]}, { PARSETYPE_FLOAT, &tweditor_state.bgcolor[2]}, { PARSETYPE_FLOAT, &tweditor_state.bgcolor[3]}} },
    { 0, "filesave",        CMDTW_FILESAVE, {{0}} },
    { 0, "filesavedynamic",    CMDTW_FILESAVEDYNAMIC, {{0}} },
    { 0, "fileload",        CMDTW_FILELOAD, {{0}} },
    { 0, "fileloadname",    CMDTW_FILELOADNAME, {{CMDSTR(tmp_str)}} },
    { 0, "fileloadnext",    CMDTW_FILELOADNEXT, {{CMDINT(layer_val)}, {CMDINT(tmp_int)}} },
    { 0, "filesavelist",    CMDTW_FILESAVELIST, {{0}} },
    { 0, "fileprune",        CMDTW_FILEPRUNE, {{0}} },
    { 0, "filenew",            CMDTW_FILENEW, {{0}} },
    { 0, "filecheckout",    CMDTW_FILECHECKOUT, {{CMDSTR(tmp_str)}} },
    { 0, "filecheckin",        CMDTW_FILECHECKIN, {{CMDSTR(tmp_str)}} },
    { 0, "refresh",            CMDTW_REFRESH, {{0}} },
    { 0, "lbutton",            CMDTW_LBUTTON, {{CMDINT(tweditor_state.lbutton)}} },
    { 0, "shift",            CMDTW_SHIFT, {{CMDINT(tweditor_state.shift)}} },
    { 0, "hideText",        0, {{CMDINT(tweditor_state.hideText)}} },
    { 0, "apply",            CMDTW_APPLY, {{0}} },
    { 0, "undo",            CMDTW_UNDO, {{0}} },
    { 0, "redo",            CMDTW_REDO, {{0}} },
    { 0, "color",            CMDTW_COLOR, {{CMDINT(tmp_color[0])}, {CMDINT(tmp_color[1])}, {CMDINT(tmp_color[2])}, {CMDINT(tmp_color[3])}} },

    { 0, "usegl",            0, {{CMDINT(texWords_useGl)}} },

    { 0 },
};

CmdList texwordseditor_cmdlist =
{
    {{texwordseditor_cmds },
    { 0}}
};



static int statusTimer=0;

void statusf(char* format, ...)
{
    va_list args;

    if (!statusTimer)
        statusTimer = timerAlloc();

    timerStart(statusTimer);

    va_start(args, format);
    vsprintf(tweditor_state.lastStatus, format, args);
    va_end(args);
    tweditor_state.lastStatusFading = 1;
}

F32 statusTime()
{
    return timerElapsed(statusTimer);
}

void drawStatusFading()
{
    if (tweditor_state.lastStatusFading) {
        F32 elapsed = statusTime();
        F32 max = 7;
        if (elapsed > max) {
            tweditor_state.lastStatusFading = 0;
        } else {
            U8 alpha = 255 - elapsed / max * 255;
            int srgba[] = {0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000};
            int i;
            for (i=0; i<4; i++) 
                srgba[i] |= alpha;
            printBasic( &game_14, game_state.screen_x/2, game_state.screen_y - 15, 101, 1.0, 1.0, 1, tweditor_state.lastStatus, strlen(tweditor_state.lastStatus), srgba);
        }
    }
}

void loadMessage(char *msg)
{
    if (!tweditor_state.hideText) {
        statusf("%s", msg);
        drawStatusFading();
        fontRenderGame();
        fontRenderEditor();
        windowUpdate();
        tweditor_state.lastStatusFading = 0;
    }
}



static char *getBasicTexName(const char *texname)
{
    static char buf[MAX_PATH];
    char *s;
    Strncpyt(buf, texname);
    forwardSlashes(buf);
    if (s = strrchr(buf, '/'))
        strcpy(buf, s+1);
    if (s = strrchr(buf, '#'))
        *s=0;
    if (s = strrchr(buf, '.'))
        *s=0;
    return buf;
}

static RegReader rr = 0;
static void initRR() {
    if (!rr) {
        rr = createRegReader();
        initRegReader(rr, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\TexWords");
    }
}
static const char *regGetString(const char *key, const char *deflt) {
    static char buf[512];
    initRR();
    strcpy(buf, deflt);
    rrReadString(rr, key, buf, ARRAY_SIZE(buf));
    return buf;
}

static void regPutString(const char *key, const char *value) {
    initRR();
    rrWriteString(rr, key, value);
}


static TexWordLayer ***getLayersListForIndex(int index)
{
    if (!tweditor_state.texWord) {
        return NULL;
    } else {
        TexWordLayer ***layersList = &tweditor_state.texWord->layers;
        TexWordLayer *layer=NULL;
        while (index) {
            int relativeIndex = (index % 100) - 1;
            index = index / 100;
            layer = eaGet(layersList, relativeIndex);
            if (!layer) // Bad layer
                return NULL;
            if (index)
                layersList = &layer->sublayer;
        }
        return layersList;
    }
}

static TexWordLayer *getLayerForIndex(int index)
{
    if (!tweditor_state.texWord) {
        return NULL;
    } else {
        TexWordLayer ***layersList = &tweditor_state.texWord->layers;
        TexWordLayer *layer=NULL;
        while (index) {
            int relativeIndex = (index % 100) - 1;
            index = index / 100;
            layer = eaGet(layersList, relativeIndex);
            if (!layer) // Bad layer
                return NULL;
            layersList = &layer->sublayer;
        }
        return layer;
    }
}

#define MAX_UNDOS ARRAY_SIZE(undoHist)
static TexWord undoHist[128];
static int undoIndex=0;
static int undoMax=0;

static void texWordsEdit_undoReset(void)
{
    int i;
    for (i=0; i<undoIndex; i++) {
        ParserDestroyStruct(ParseTexWord, &undoHist[i]);
    }
    undoIndex = 0;
}

static void texWordsEdit_undoJournal(void)
{
    TexWord *texWord = tweditor_state.texWord;

    if (undoIndex && texWord) {
        int ret = ParserCompareStruct(ParseTexWord, texWord, &undoHist[undoIndex-1]);
        if (!ret)
            return; // No difference, don't log!
    }

    if (undoIndex == MAX_UNDOS) {
        ParserDestroyStruct(ParseTexWord, &undoHist[0]);
        memmove(&undoHist[0], &undoHist[1], sizeof(undoHist[0])*(MAX_UNDOS-1));
        undoIndex--;
        memset(&undoHist[MAX_UNDOS-1], 0, sizeof(TexWord));
    } else {
        // In case of overwritting some leftover entry
        ParserDestroyStruct(ParseTexWord, &undoHist[undoIndex]);
    }

    if (texWord) {
        ParserCopyStruct(ParseTexWord, texWord, sizeof(TexWord), &undoHist[undoIndex]);
    }
    undoIndex++;
    undoMax = undoIndex;
}

static void texWordsEdit_undoUndo(void)
{
    int oldMax = undoMax;
    // Save current version
    texWordsEdit_undoJournal();
    undoMax = MAX(undoMax, oldMax);
    if (undoIndex<=1) {
        Beep(880, 200);
    } else {
        // Go back to last version
        undoIndex-=2; // Subtract 2 because we saved the current version above
        if (tweditor_state.texWord) {
            ParserDestroyStruct(ParseTexWord, tweditor_state.texWord);
        }
        ParserCopyStruct(ParseTexWord, &undoHist[undoIndex], sizeof(TexWord), tweditor_state.texWord);
        texWordsEdit_layerExtentsUnApply();
    }
}

static void texWordsEdit_undoRedo(void)
{
    if (undoIndex < undoMax-1) {
        undoIndex++;
        ParserDestroyStruct(ParseTexWord, tweditor_state.texWord);
        ParserCopyStruct(ParseTexWord, &undoHist[undoIndex], sizeof(TexWord), tweditor_state.texWord);
        texWordsEdit_layerExtentsUnApply();
    } else {
        Beep(880, 200);
    }
}


void texWordsEdit_reloadCallback(void)
{
    if (tweditor_state.bind) {
        tweditor_state.texWord = tweditor_state.bind->texWord;
    }
}


static TexWordLayer *findLayerByName(TexWordLayer ***layersList, char *layerName)
{
    int i;
    for (i=0; i<eaSize(layersList); i++) {
        if (stricmp(layerName, (*layersList)[i]->layerName)==0)
            return (*layersList)[i];
    }
    return NULL;
}

static char *generateName(TexWordLayer ***layersList, char *sourceName)
{
    static char buf[256];
    // Generate new name
    if (sourceName) {
        strncpyt(buf, sourceName, ARRAY_SIZE(buf) - 8);
        if (!strstri(buf, "copy")) {
            strcat(buf, " copy");
        }
        while (findLayerByName(layersList, buf)) {
            if (strEndsWith(buf, "copy")) {
                // Already has a "copy"!
                strcat(buf, " 2");
            } else {
                incrementName(buf, ARRAY_SIZE(buf)-1);
            }
        }
    } else {
        strcpy(buf, "#1");
        while (findLayerByName(layersList, buf)) {
            incrementName(buf, ARRAY_SIZE(buf)-1);
        }
    }
    return buf;
}

void texWordsEdit_layerInsert(int rawindex)
{
    TexWordLayer *layer;
    if (!tweditor_state.texWord) return;
    texWordsEdit_undoJournal();

    layer = ParserAllocStruct(sizeof(TexWordLayer));
    memset(layer, 0, sizeof(*layer));
    // Default new layer: text
    layer->type = TWLT_TEXT;
    layer->pos[0] = layer->pos[1] = 5;
    layer->size[0] = 100;
    layer->size[1] = 40;

    if (!tweditor_state.texWord->layers) {
        eaCreate(&tweditor_state.texWord->layers);
    }
    layer->layerName = ParserAllocString(generateName(&tweditor_state.texWord->layers, NULL));
    eaInsert(&tweditor_state.texWord->layers, layer, rawindex);
}

void texWordsEdit_layerClone(int index)
{
    TexWordLayer ***layersList = getLayersListForIndex(index);
    TexWordLayer *oldLayer = getLayerForIndex(index);
    TexWordLayer *newLayer;
    int relativeIndex;
    if (!tweditor_state.texWord || !layersList || !oldLayer)
        return;
    texWordsEdit_undoJournal();

    relativeIndex = eaFind(layersList, oldLayer);
    assert(relativeIndex!=-1);

    newLayer = ParserAllocStruct(sizeof(TexWordLayer));
    ParserCopyStruct(ParseTexWordLayer, oldLayer, sizeof(TexWordLayer), newLayer);
    if (newLayer->text) {
        ParserFreeString(newLayer->text);
        newLayer->text = NULL;
    }

    if (newLayer->layerName) {
        char *temp = ParserAllocString(generateName(layersList, newLayer->layerName));
        ParserFreeString(newLayer->layerName);
        newLayer->layerName = temp;
    } else {
        newLayer->layerName = ParserAllocString(generateName(layersList, NULL));
    }

    eaInsert(layersList, newLayer, relativeIndex);
}

void texWordsEdit_layerCopyAll(void)
{
    if (!tweditor_state.texWord)
        return;

    if (layerClipboard.layers) {
        ParserDestroyStruct(ParseTexWord, &layerClipboard);
    }
    ParserCopyStruct(ParseTexWord, tweditor_state.texWord, sizeof(TexWord),&layerClipboard);
}


static void cloneLayerText(TexWordLayer ***layerList)
{
    int num = eaSize(layerList);
    int i;
    for (i=0; i<num; i++) {
        TexWordLayer *layer = eaGet(layerList, i);
        if (layer->text) {
            char text[2048]="";
            char keyString[1024];
            char comment[2048];
            unsigned char *s;
            texWordsEdit_undoJournal();

            // Look up old text
            msPrintf(texWordsMessages, SAFESTR(text), layer->text);

            if (stricmp(text, "Placeholder")==0) {
                ParserFreeString(layer->text);
                layer->text = NULL;
            } else {
                // Generate a new key!
                strcpy(keyString, "TexWord_");
                strcat(keyString, text);
                for (s=keyString; *s; ) {
                    if (!isalnum((unsigned char)*s) && *s!='_') {
                        strcpy(s, s+1);
                        s[0] = toupper(s[0]);
                    } else {
                        s++;
                    }
                }
                // Verify that this is a unique key, if not, increment
                while (msContainsKey(texWordsMessages, keyString)) {
                    incrementName(keyString, 1023);
                }

                // Save the Key string/entered string in message store
                sprintf(comment, "# %s Added for %s", timerGetTimeString(), tweditor_state.texWord->name);
                msUpdateMessage(texWordsMessages, keyString, text, comment);

                ParserFreeString(layer->text);
                layer->text = ParserAllocString(keyString);
            }
        }
        if (layer->sublayer) {
            cloneLayerText(&layer->sublayer);
        }
    }
}

void texWordsEdit_layerPasteAll(void)
{
    if (!layerClipboard.layers)
        return;
    if (!tweditor_state.texWord) {
        texWordsEdit_create(textureToBaseTexWord(game_state.texWordEdit));
    }
    texWordsEdit_undoJournal();

    if (tweditor_state.texWord) {
        ParserDestroyStruct(ParseTexWord, tweditor_state.texWord);
    }
    ParserCopyStruct(ParseTexWord, &layerClipboard, sizeof(TexWord), tweditor_state.texWord);
    cloneLayerText(&tweditor_state.texWord->layers);
    ParserFreeString(tweditor_state.texWord->name);
    tweditor_state.texWord->name = ParserAllocString(textureToBaseTexWord(game_state.texWordEdit));
}


void texWordsEdit_layerSwap(int rawindex0, int rawindex1)
{
    TexWordLayer *layer0, *layer1;
    if (!tweditor_state.texWord) return;
    // Purposefully uses 0-based root indices
    layer0 = eaGet(&tweditor_state.texWord->layers, rawindex0);
    layer1 = eaGet(&tweditor_state.texWord->layers, rawindex1);
    if (rawindex0==rawindex1) return;
    if (!layer0 || !layer1) return;
    texWordsEdit_undoJournal();

    eaSet(&tweditor_state.texWord->layers, layer1, rawindex0);
    eaSet(&tweditor_state.texWord->layers, layer0, rawindex1);
}

void texWordsEdit_layerDelete(int index)
{
    TexWordLayer ***layersList = getLayersListForIndex(index);
    TexWordLayer *oldLayer = getLayerForIndex(index);
    int relativeIndex;
    if (!tweditor_state.texWord || !layersList || !oldLayer)
        return;
    texWordsEdit_undoJournal();

    relativeIndex = eaFind(layersList, oldLayer);
    assert(relativeIndex!=-1);
    eaSet(layersList, NULL, relativeIndex);
    eaRemove(layersList, relativeIndex);
    if (eaSize(layersList)==0) {
        eaDestroy(layersList);
    }
    if (oldLayer) {
        ParserDestroyStruct(ParseTexWordLayer, oldLayer);
        ParserFreeStruct(oldLayer);
    }
}

void texWordsEdit_layerType(int index, TexWordLayerType type )
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    texWordsEdit_undoJournal();

    layer->type = type;
}
void texWordsEdit_layerName(int index)
{
    char text[2048]="";
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;

    strcpy(text, layer->layerName);
    if( winGetString("Enter a layer name", text) ) {
        texWordsEdit_undoJournal();
        ParserFreeString(layer->layerName);
        layer->layerName = ParserAllocString(text);
    }
}
void texWordsEdit_layerHide(int index, int hide, int journal)
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    if (journal)
        texWordsEdit_undoJournal();

    layer->hidden = hide;
}
void texWordsEdit_layerHideAll(int hide)
{
    int i;
    if (!tweditor_state.texWord) return;
    texWordsEdit_undoJournal();

    for (i=0; i<eaSize(&tweditor_state.texWord->layers); i++) {
        tweditor_state.texWord->layers[i]->hidden = hide;
    }
}
void texWordsEdit_layerSolo(int index)
{
    int i;
    if (!tweditor_state.texWord) return;
    texWordsEdit_undoJournal();

    for (i=0; i<eaSize(&tweditor_state.texWord->layers); i++) {
        if (i == index) {
            texWordsEdit_layerHide(i, 0, 0);
        } else {
            texWordsEdit_layerHide(i, 1, 0);
        }
    }
}

void texWordsEdit_layerStretch(int index, TexWordLayerStretch stretch )
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    texWordsEdit_undoJournal();

    layer->stretch = stretch;
}

void texWordsEdit_layerImage(int index, char *textureName )
{
    char fileName[MAX_PATH];
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    texWordsEdit_undoJournal();

    if (stricmp(textureName, "browse")==0) {
        // Choose a new .texture file
        strncpy(fileName, game_state.texWordEdit, MAX_PATH - 1);
        fileName[MAX_PATH - 1] = '\0';
        fileLocateWrite(fileName, fileName);
        backSlashes(fileName);
        if (winGetFileName("*.texture", fileName, 0)) {
            strcpy(fileName, getFileName(fileName));
        } else {
            return;
        }
    } else {
        strcpy(fileName, textureName);
    }
    if (strchr(fileName, '.')) {
        *strchr(fileName, '.') = 0;
    }

    // Check for referencing itself
    {
        char myname[MAX_PATH];
        strncpy(myname, getFileName(game_state.texWordEdit), MAX_PATH - 1);
        myname[MAX_PATH - 1] = '\0';
        if (strchr(myname, '.')) {
            *strchr(myname, '.') = 0;
        }
        if (stricmp(fileName, myname)==0)
            return;
    }

    mruAddToList(mruTextures, fileName);

    if (layer->imageName)
        ParserFreeString(layer->imageName);
    layer->imageName = ParserAllocString(fileName);
    layer->image = texLoadBasic(layer->imageName, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UI);

    // Adjust auto-sized dynamic textures
    {
        BasicTexture *base = texWordGetBaseImage(tweditor_state.texWord);
        BasicTexture *bind = tweditor_state.bind;
        if (base && bind) {
            // Use default sizes, etc (may be overridden in texWordDoComposition)
            bind->origWidth = base->realWidth;
            bind->origHeight = base->realHeight;
        }
    }
}

#define FIELD(x) (stricmp(x,field)==0)
void texWordsEdit_layerAsk(int index, char *field )
{
    char text[TEXT_DIALOG_MAX_STRLEN]="";
    char prompt[TEXT_DIALOG_MAX_STRLEN];
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;

    sprintf(prompt, "Enter a value for %s:", field);
    if (FIELD("X")) {
        sprintf(text, "%1.f", layer->pos[0]);
    } else if (FIELD("Y")) {
        sprintf(text, "%1.f", layer->pos[1]);
    } else if (FIELD("ROT")) {
        sprintf(text, "%1.f", layer->rot);
    } else if (FIELD("WIDTH")) {
        sprintf(text, "%1.f", layer->size[0]);
    } else if (FIELD("HEIGHT")) {
        sprintf(text, "%1.f", layer->size[1]);
    }
    if( winGetString(prompt, text) ) {
        int val = atoi(text);
        texWordsEdit_undoJournal();
        if (FIELD("X")) {
            layer->pos[0] = val;
        } else if (FIELD("Y")) {
            layer->pos[1] = val;
        } else if (FIELD("ROT")) {
            layer->rot = val;
        } else if (FIELD("WIDTH")) {
            layer->size[0] = val;
        } else if (FIELD("HEIGHT")) {
            layer->size[1] = val;
        }
    }
}


static void texWordsEdit_layerExtentsUnApply()
{
    TexWordLayer *layer = getLayerForIndex(tweditor_state.extents.index);
    if (!tweditor_state.extents.edit) return;
    if (!layer) {
        // This layer didn't exist
        tweditor_state.extents.edit = 0;
        tweditor_state.extents.on = 0;
        return;
    }

    tweditor_state.extents.x0 = layer->pos[0];
    tweditor_state.extents.y0 = layer->pos[1];
    tweditor_state.extents.x1 = layer->pos[0]+layer->size[0];
    tweditor_state.extents.y1 = layer->pos[1]+layer->size[1];
}

void texWordsEdit_layerExtentsApply()
{
    if (tweditor_state.extents.edit) {
        int changed=false;
        TexWordLayer *layer = getLayerForIndex(tweditor_state.extents.index);
        if (!layer) return;
        texWordsEdit_undoJournal();
        if (layer->pos[0] != tweditor_state.extents.x0) {
            changed = true;
            layer->pos[0] = tweditor_state.extents.x0;
        }
        if (layer->pos[1] != tweditor_state.extents.y0) {
            changed = true;
            layer->pos[1] = tweditor_state.extents.y0;
        }
        if (layer->size[0] != tweditor_state.extents.x1 - tweditor_state.extents.x0) {
            changed = true;
            layer->size[0] = tweditor_state.extents.x1 - tweditor_state.extents.x0;
        }
        if (layer->size[1] != tweditor_state.extents.y1 - tweditor_state.extents.y0) {
            changed = true;
            layer->size[1] = tweditor_state.extents.y1 - tweditor_state.extents.y0;
        }
        //layer->font.drawSize = layer->size[1] / 2;
        if (changed) {
            texFree(tweditor_state.bind, 0);
            tweditor_state.needToRebuild = changed;
        }
    }
}

void texWordsEdit_layerExtentsFinish()
{
    texWordsEdit_layerExtentsApply();
    tweditor_state.extents.on = 0;
    tweditor_state.extents.edit = 0;
}

void texWordsEdit_layerExtents(int index)
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;

    if (tweditor_state.extents.edit && tweditor_state.extents.index == index) {
        // Just toggle off
        texWordsEdit_layerExtentsFinish();
//        tweditor_state.reOpenMenu = 1;
        return;
    }

    // Turn off old one if any
    texWordsEdit_layerExtentsFinish();

    tweditor_state.extents.on = 1;
    tweditor_state.extents.edit = 1;
    tweditor_state.extents.index = index;
    tweditor_state.extents.mousegrabbed = 0;
    // Fix up sizes
    if (layer->size[0]==0) {
        if (layer->type == TWLT_IMAGE) {
            layer->size[0] = layer->image->width;
        } else {
            layer->size[0] = 10;
        }
    }
    if (layer->size[1]==0) {
        if (layer->type == TWLT_IMAGE) {
            layer->size[1] = layer->image->height;
        } else {
            layer->size[1] = 10;
        }
    }

    texWordsEdit_layerExtentsUnApply(); // Get starting values
}

void texWordsEdit_layerCopySize(int dstIndex, int srcIndex)
{
    TexWordLayer *srcLayer = getLayerForIndex(srcIndex);
    TexWordLayer *dstLayer = getLayerForIndex(dstIndex);
    if (!srcLayer || !dstLayer)
        return;
    texWordsEdit_undoJournal();

    dstLayer->size[0] = srcLayer->size[0];
    dstLayer->size[1] = srcLayer->size[1];
}

void texWordsEdit_layerCopyPos(int dstIndex, int srcIndex)
{
    TexWordLayer *srcLayer = getLayerForIndex(srcIndex);
    TexWordLayer *dstLayer = getLayerForIndex(dstIndex);
    if (!srcLayer || !dstLayer)
        return;
    texWordsEdit_undoJournal();

    dstLayer->pos[0] = srcLayer->pos[0];
    dstLayer->pos[1] = srcLayer->pos[1];
}

void texWordsEdit_layerCopyRot(int dstIndex, int srcIndex)
{
    TexWordLayer *srcLayer = getLayerForIndex(srcIndex);
    TexWordLayer *dstLayer = getLayerForIndex(dstIndex);
    if (!srcLayer || !dstLayer)
        return;
    texWordsEdit_undoJournal();

    dstLayer->rot = srcLayer->rot;
}

void texWordsEdit_layerTextChange(int index)
{
    char text[TEXT_DIALOG_MAX_STRLEN]="";
    char comment[2048];
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;

    if (layer->text) {
        msPrintf(texWordsMessages, SAFESTR(text), layer->text);
        //strcpy(text, layer->text);
    }
    if( winGetString("Insert Text", text) && text[0] ) {
        char keyString[1024];
        texWordsEdit_undoJournal();
        if (layer->text && layer->text[0] && stricmp(layer->text, "Placeholder")!=0) {
            // We already have a keyString, just update the translation for this string
            strcpy(keyString, layer->text);
        } else {
            unsigned char *s;
            // Generate a new key!
            strcpy(keyString, "TexWord_");
            strcat(keyString, text);
            for (s=keyString; *s; ) {
                if (!isalnum((unsigned char)*s) && *s!='_') {
                    strcpy(s, s+1);
                    s[0] = toupper(s[0]);
                } else {
                    s++;
                }
            }
            // Verify that this is a unique key, if not, increment
            while (msContainsKey(texWordsMessages, keyString)) {
                incrementName(keyString, 1023);
            }
        }
        // Save the Key string/entered string in message store
        sprintf(comment, "# %s Added for %s", timerGetTimeString(), tweditor_state.texWord->name);
        msUpdateMessage(texWordsMessages, keyString, text, comment);

        ParserFreeString(layer->text);
        layer->text = ParserAllocString(keyString);
    }
}

void texWordsEdit_layerColorMode(int index, int newmode)
{
    int i, j;
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    if (newmode!=1 && newmode!=4) return;
    texWordsEdit_undoJournal();

    while (eaiSize(&layer->rgba) != 4)
        eaiPush(&layer->rgba, 255);

    if (newmode==1) {
        for (i=0; i<4; i++) 
            layer->rgba[i] = 0;
        for (i=0; i<4; i++) {
            for (j=0; j<4; j++) 
                layer->rgba[j]+=eaiSize(&layer->rgbas[i])>j?layer->rgbas[i][j]:255;
            eaiDestroy(&layer->rgbas[i]);
        }
        for (i=0; i<4; i++) 
            layer->rgba[i] /=4;
    } else {
        for (i=0; i<4; i++) {
            if (layer->rgbas[i])
                eaiDestroy(&layer->rgbas[i]);
            for (j=0; j<4; j++) 
                eaiPush(&layer->rgbas[i], layer->rgba[j]);
        }
        eaiDestroy(&layer->rgba);
    }
}

int askColor(Color *res)
{
    char text[TEXT_DIALOG_MAX_STRLEN], *delims=",\t ";
    Color c = *res;
    sprintf(text, "%d,%d,%d,%d", c.r, c.g, c.b, c.a);
    if( winGetString("Enter RGB or RGBA values separated by spaces or commas", text) ) {
        char *r, *g, *b, *a;
        r = strtok(text, delims);
        g = strtok(NULL, delims);
        b = strtok(NULL, delims);
        a = strtok(NULL, delims);
        if (!r || !g || !b) {
            winMsgAlert("Invalid format");
            return 0;
        }
        if (!a) a="255";
        c.r=atoi(r);
        c.g=atoi(g);
        c.b=atoi(b);
        c.a=atoi(a);
        *res = c;
        sprintf(text, "%d,%d,%d,%d", c.r, c.g, c.b, c.a);
        mruAddToList(mruColors, text);
        return 1;
    }
    return 0;
}

void texWordsEdit_layerColorChange(int index, int colorindex)
{
    eaiHandle *color;
    int i;
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    if (colorindex<0 || colorindex>4) return;
    texWordsEdit_undoJournal();

    if (colorindex==0) {
        color = &layer->rgba;
    } else {
        color = &layer->rgbas[colorindex-1];
    }

    while (eaiSize(color) != 4)
        eaiPush(color, 255);
    for (i=0; i<4; i++) 
        (*color)[i] = tweditor_state.selcolor.rgba[i];
}

void texWordsEdit_layerAlphaChange(int index, int colorindex, char *val)
{
    eaiHandle *color;
    int alpha;
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    if (colorindex<0 || colorindex>4) return;
    texWordsEdit_undoJournal();

    if (colorindex==0) {
        color = &layer->rgba;
    } else {
        color = &layer->rgbas[colorindex-1];
    }

    while (eaiSize(color) != 4)
        eaiPush(color, 255);
    alpha = (*color)[4];

    if (stricmp(val, "ASK")==0) {
        char text[TEXT_DIALOG_MAX_STRLEN];
        sprintf(text, "%1.f", alpha*100/255.f);
        if( !winGetString("Enter an opacity (0 - 100)", text) )
            return;
        alpha = round(atof(text)*255/100.f);
    } else {
        alpha = atoi(val);
    }

    (*color)[3] = alpha;
}

void texWordsEdit_layerFont(int index, char *fontName)
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    texWordsEdit_undoJournal();

    if (layer->font.fontName) {
        ParserFreeString(layer->font.fontName);
    }
    layer->font.fontName = ParserAllocString(fontName);
}

void texWordsEdit_layerFontStyle(int index, char *field, int val)
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    texWordsEdit_undoJournal();

    if (FIELD("bold")) {
        layer->font.bold = val;
    } else if (FIELD("italic")) {
        layer->font.italicize = val;
    } else if (FIELD("size")) {
        layer->font.drawSize = val;
    } else if (FIELD("dropxyoffs")) {
        layer->font.dropShadowXOffset = val;
        layer->font.dropShadowYOffset = val;
    } else if (FIELD("dropxoffs")) {
        layer->font.dropShadowXOffset = val;
    } else if (FIELD("dropyoffs")) {
        layer->font.dropShadowYOffset = val;
    } else if (FIELD("outline")) {
        layer->font.outlineWidth = val;
    } else {
        Errorf("Invalid field name: %s", field);
    }
}

void texWordsEdit_layerSubInsert(int index)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayer *sublayer;
    if (!layer) return;
    if (layer->sublayer && layer->sublayer[0]) return;
    texWordsEdit_undoJournal();

    sublayer = ParserAllocStruct(sizeof(TexWordLayer));
    memset(sublayer, 0, sizeof(*sublayer));
    eaPush(&layer->sublayer, sublayer);

    layer->subBlend = TWBLEND_MULTIPLY;
    layer->subBlendWeight = 1.0;
    sublayer->imageName = ParserAllocString("white");
    sublayer->stretch = TWLS_FULL;
    sublayer->type = TWLT_IMAGE;
}


void texWordsEdit_layerSubBlendMode(int index, TexWordBlendType blendType)
{
    TexWordLayer *layer = getLayerForIndex(index);
    if (!layer) return;
    texWordsEdit_undoJournal();

    layer->subBlend = blendType;
}

void texWordsEdit_layerSubBlendWeight(int index, char *weight)
{
    TexWordLayer *layer = getLayerForIndex(index);
    F32 newWeight;
    if (!layer) return;
    texWordsEdit_undoJournal();

    if (stricmp(weight, "ask")==0) {
        char text[TEXT_DIALOG_MAX_STRLEN];
        sprintf(text, "%1.f", layer->subBlendWeight*100);
        if( !winGetString("Enter a weight (0 - 100)", text) ) 
            return;
        newWeight = atof(text)/100.f;
    } else {
        newWeight = atof(weight);
    }

    layer->subBlendWeight = newWeight;
}

void texWordsEdit_layerFilterInsert(int index, int filterIndex)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    if (!layer) return;
    texWordsEdit_undoJournal();

    filter = ParserAllocStruct(sizeof(TexWordLayerFilter));
    memset(filter, 0, sizeof(*filter));
    filter->type = TWFILTER_DROPSHADOW;
    filter->magnitude = 3;
    filter->offset[0] = filter->offset[1] = 3;
    eaiCreate(&filter->rgba);
    eaiPush(&filter->rgba, 0);
    eaiPush(&filter->rgba, 0);
    eaiPush(&filter->rgba, 0);
    eaiPush(&filter->rgba, 255);
    filter->spread = 0.5;

    if (!layer->filter) {
        eaCreate(&layer->filter);
    }
    eaInsert(&layer->filter, filter, filterIndex);
}

void texWordsEdit_layerFilterDelete(int index, int filterIndex)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    if (!layer) return;
    if (filterIndex >= eaSize(&layer->filter))
        return;
    texWordsEdit_undoJournal();

    filter = eaRemove(&layer->filter, filterIndex);
    if (filter) {
        ParserDestroyStruct(ParseTexWordLayerFilter, filter);
        ParserFreeStruct(filter);
    }

}
void texWordsEdit_layerFilterType(int index, int filterIndex, TexWordFilterType type)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    if (!layer) return;
    if (filterIndex >= eaSize(&layer->filter))
        return;
    filter = layer->filter[filterIndex];
    texWordsEdit_undoJournal();

    if (filter->type != type) {
        filter->type = type;
        if (type == TWFILTER_BLUR || type == TWFILTER_DESATURATE) {
            filter->blend = TWBLEND_REPLACE;
            filter->offset[0] = 0;
            filter->offset[1] = 0;
        } else {
            filter->blend = TWBLEND_OVERLAY;
        }
        texWordVerify(tweditor_state.texWord, true);
    }
}
void texWordsEdit_layerFilterBlend(int index, int filterIndex, TexWordBlendType blend)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    if (!layer) return;
    if (filterIndex >= eaSize(&layer->filter))
        return;
    filter = layer->filter[filterIndex];
    texWordsEdit_undoJournal();

    if (filter->blend != blend) {
        filter->blend = blend;
        texWordVerify(tweditor_state.texWord, true);
    }
}

void texWordsEdit_layerFilterStyle(int index, int filterIndex, char *field, char *val)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    F32 value;
    if (!layer) return;
    if (filterIndex >= eaSize(&layer->filter))
        return;
    filter = layer->filter[filterIndex];

    if (stricmp(val, "ask")==0) {
        char text[TEXT_DIALOG_MAX_STRLEN], prompt[TEXT_DIALOG_MAX_STRLEN];
        sprintf(prompt, "Enter a value for the filter's %s:", field);
        if (FIELD("offsetX")) {
            sprintf(text, "%d", filter->offset[0]);
        } else if (FIELD("offsetY")) {
            sprintf(text, "%d", filter->offset[1]);
        } else if (FIELD("offsetXY")) {
            sprintf(text, "%d", filter->offset[0]);
        } else if (FIELD("magnitude")) {
            sprintf(text, "%d", filter->magnitude);
        } else if (FIELD("percent")) {
            sprintf(text, "%1.f", filter->percent);
        } else if (FIELD("spread")) {
            sprintf(text, "%1.f", filter->spread);
        }
        if( !winGetString(prompt, text) )
            return;
        value = atof(text);
    } else {
        value = atof(val);
    }
    texWordsEdit_undoJournal();
    if (FIELD("offsetX")) {
        filter->offset[0] = value;
    } else if (FIELD("offsetY")) {
        filter->offset[1] = value;
    } else if (FIELD("offsetXY")) {
        filter->offset[0] = value;
        filter->offset[1] = value;
    } else if (FIELD("magnitude")) {
        filter->magnitude = value;
    } else if (FIELD("percent")) {
        filter->percent = value;
    } else if (FIELD("spread")) {
        filter->spread = value;
    }
    texWordVerify(tweditor_state.texWord, true);
}
void texWordsEdit_layerFilterColor(int index, int filterIndex)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    int i;
    if (!layer) return;
    if (filterIndex >= eaSize(&layer->filter))
        return;
    filter = layer->filter[filterIndex];
    texWordsEdit_undoJournal();

    while (eaiSize(&filter->rgba) != 4)
        eaiPush(&filter->rgba, 255);
    for (i=0; i<4; i++) 
        filter->rgba[i] = tweditor_state.selcolor.rgba[i];
}

void texWordsEdit_layerFilterAlpha(int index, int filterIndex, char *val)
{
    TexWordLayer *layer = getLayerForIndex(index);
    TexWordLayerFilter *filter;
    int alpha;
    if (!layer) return;
    if (filterIndex >= eaSize(&layer->filter))
        return;
    filter = layer->filter[filterIndex];
    texWordsEdit_undoJournal();

    while (eaiSize(&filter->rgba) != 4)
        eaiPush(&filter->rgba, 255);
    alpha = filter->rgba[4];

    if (stricmp(val, "ASK")==0) {
        char text[TEXT_DIALOG_MAX_STRLEN];
        sprintf(text, "%1.f", alpha*100/255.f);
        if( !winGetString("Enter an opacity (0 - 100)", text) )
            return;
        alpha = atof(text)*255/100.f;
    } else {
        alpha = atoi(val);
    }

    filter->rgba[3] = alpha;
}

void texWordsEdit_size(int x, int y)
{
    if (!tweditor_state.texWord)
        return;
    texWordsEdit_undoJournal();

    if (x!=-1) {
        if (x)
            x = 1 << log2i(x);
        tweditor_state.texWord->width = x;
    }
    if (y!=-1) {
        if (y)
            y = 1 << log2i(y);
        tweditor_state.texWord->height = y;
    }
}


void texWordsEdit_fileLoadName(const char *fileName)
{
    char fn_buf[MAX_PATH];
    char *s = fn_buf;
    const char *fn;
    BasicTexture *tex;
    Strncpyt(fn_buf, fileName);

    if (tweditor_state.dirty) {
        if (0==winMsgYesNo("You have unsaved changes.  Do you want to discard all changes and continue with opening a new file?")) {
            return;
        }
    }

    forwardSlashes(s);
    if (strStartsWith(s, fileDataDir())) {
        s+= strlen(fileDataDir());
    } else if (strstri(s, "/src/")) {
        s = strstri(s, "/src/") + 5;
    } else if (s[1]==':') {
        char msg[1024];
        sprintf_s(SAFESTR(msg), "You are attempting to load a file which is not in %s (perhaps GameFix while running Game or vice versa?).  Do you want to continue?", fileDataDir());
        // Loaded something *not* from within our game data dir!
        if (0==winMsgYesNo(msg)) {
            return;
        }
    }
    while (*s=='/') s++;
    fn = getFileName(s);
    tex = texFind(fn);
    if (tex) {
        estrPrintf(&game_state.texWordEdit, "%s%s%s%s", tex->dirname, "/", tex->name, ".texture");
    } else {
        estrPrintCharString(&game_state.texWordEdit, s);
    }
    if (strEndsWith(game_state.texWordEdit, ".tga")) {
        changeFileExtEString(game_state.texWordEdit, ".texture", &game_state.texWordEdit);
    }

    texWordsEditorSetup();
    unbindKeyProfile(&ent_debug_binds_profile); // Because it gets rebound

//    debug_state.closeMenu = 0;
    texWordsEdit_undoReset();
}

void texWordsEdit_fileLoad(void)
{
    char fileName[MAX_PATH];
    // Choose a new .texture file
    strncpy(fileName, game_state.texWordEdit, MAX_PATH - 1);
    fileName[MAX_PATH - 1] = '\0';
    fileLocateWrite(fileName, fileName);
    backSlashes(fileName);
    if (winGetFileName("*.texture", fileName, 0)) {
        texWordsEdit_fileLoadName(fileName);
    }
}

void texWordsEdit_fileSave()
{
    TexWord *texWord = tweditor_state.texWord;
    if (texWord) {
        char fullname[MAX_PATH];
        TexWordList dummy={0};
        int ok;
        eaPush(&dummy.texWords, texWord);
        fileLocateWrite(texWord->name, fullname);
        fileRenameToBak(fullname);
        mkdirtree(fullname);
        ok = ParserWriteTextFile(texWord->name, ParseTexWordFiles, &dummy, 0, 0);
        statusf("File saved %s", ok?"successfully":"FAILED");
        msSaveMessageStore(texWordsMessages);
    }
    tweditor_state.dirty = false;
}

static void cleanupMessageStoreOnSaveAs(TexWord *texWord)
{
    msClearExtendedDataFlags(texWordsMessages);
}


void texWordsEdit_fileSaveDynamic(void)
{
    char fileName[MAX_PATH];
    if (!tweditor_state.texWord)
        return;

    // Choose a new .texword file
    strcpy(fileName, "c:\\game\\data\\texts\\Base\\texture_library\\Dynamic\\NewDynamic.texword");
    if (winGetFileName("*.texword", fileName, 1)) {
        int i;
        TexWord *texWord;
        char textureName[1024], *s;
        texWordsEdit_undoReset();

        // Extract texture name
        strcpy(textureName, getFileName(fileName));
        s = strrchr(textureName, '.');
        if (s)
            *s=0;
        if (texWordFind(textureName, 0)) {
            Errorf("There is already a layout named \"%s\", please save under a different name.", textureName);
            return;
        }

        // Create a new TexWord first
        texWord = ParserAllocStruct(sizeof(TexWord));
        // Copy
        ParserCopyStruct(ParseTexWord, tweditor_state.texWord, sizeof(TexWord), texWord);

        // Change any BaseImage to Image
        for (i=0; i<eaSize(&texWord->layers); i++)
        {
            TexWordLayer *layer = texWord->layers[i];
            if (layer->type == TWLT_BASEIMAGE) {
                layer->type = TWLT_IMAGE;
                if (layer->imageName)
                    ParserFreeString(layer->imageName);
                layer->imageName = NULL;
                if (tweditor_state.bind)
                    layer->imageName = ParserAllocString(tweditor_state.bind->name);
            }
        }
        if (tweditor_state.bind) {
            texWord->width = tweditor_state.bind->width;
            texWord->height = tweditor_state.bind->height;
        }

        estrPrintCharString(&game_state.texWordEdit, baseTexWordToTexture(fileName));

        // Remove all modified message store strings not in this texWord
        cleanupMessageStoreOnSaveAs(texWord);

        cloneLayerText(&texWord->layers);
        ParserFreeString(texWord->name);
        texWord->name = ParserAllocString(textureToBaseTexWord(game_state.texWordEdit));

        // add to global lists
        eaPush(&texWords_list.texWords, texWord);
        stashAddPointer(htTexWords, textureName, texWord, false);

        {
            TexWordParams *params = createTexWordParams();
            params->layoutName = allocAddString(textureName);
            tweditor_state.bind = texLoadDynamic(params, TEX_LOAD_DONT_ACTUALLY_LOAD, TEX_FOR_UTIL, NULL)->tex_layers[TEXLAYER_BASE];
            tweditor_state.texWord = tweditor_state.bind->texWord;
            assert(tweditor_state.texWord == texWord);
        }

        // Save to disk
        texWordsEdit_fileSave();
    }
}

void texWordsEdit_fileNew()
{
    TexWord *texWord = tweditor_state.texWord;
    if (texWord) {
        if (winMsgYesNo("Are you sure you want to remove the layout information associated with this file (this action cannot be undone)?"))
        {
            char textureName[MAX_PATH], *s;
            char fullname[MAX_PATH];
            TexWordList dummy={0};
            texWordsEdit_undoJournal();


            eaPush(&dummy.texWords, texWord);
            fileLocateWrite(texWord->name, fullname);
            if (fileExists(fullname)) {
                fileRenameToBak(fullname);
            }

            // Remove from global lists
            eaRemove(&texWords_list.texWords, eaFind(&texWords_list.texWords, texWord));
            // Extract texture name
            strcpy(textureName, getFileName(texWord->name));
            s = strrchr(textureName, '.');
            *s=0;
            stashRemovePointer(htTexWords, textureName, NULL);

            tweditor_state.bind->texWord = texWordFind(tweditor_state.bind->name, 0);
            tweditor_state.texWord = tweditor_state.bind->texWord;
            if (tweditor_state.bind->hasBeenComposited || tweditor_state.bind->texWord) {
                // Free the old composited data
                texFree(tweditor_state.bind, 0);
            }
            tweditor_state.needToRebuild = 1;
            texWordsEdit_undoReset();
        }
    }
}

void texWordsEdit_fileCheckout(char *fileName)
{
    char *s = fileName;
    forwardSlashes(s);
    tweditor_state.readonly = false;
    texWordsEdit_undoReset();
}

void texWordsEdit_fileCheckin(char *fileName)
{
    char *s = fileName;
    forwardSlashes(s);
    tweditor_state.readonly = true;
    texWordsEdit_undoReset();
}

static void texWordsEdit_create(char *path)
{
    if (!tweditor_state.texWord) {
        char textureName[MAX_PATH], *s;
        TexWord *texWord = ParserAllocStruct(sizeof(TexWord));
        TexWordLayer *layer;
        memset(texWord, 0, sizeof(*texWord));
        texWord->name = ParserAllocString(path);

        layer = ParserAllocStruct(sizeof(TexWordLayer));
        memset(layer, 0, sizeof(*layer));
        layer->type = TWLT_TEXT;
        layer->pos[0] = layer->pos[1] = 5;
        layer->size[0] = 100;
        layer->size[1] = 40;
        layer->layerName = ParserAllocString("#2");
        eaPush(&texWord->layers, layer);

        layer = ParserAllocStruct(sizeof(TexWordLayer));
        memset(layer, 0, sizeof(*layer));
        if (tweditor_state.fromEditor) {
            layer->type = TWLT_IMAGE;
            layer->imageName = ParserAllocString("grey");
        } else {
            layer->type = TWLT_BASEIMAGE;
        }
        layer->stretch = TWLS_FULL;
        layer->layerName = ParserAllocString("#1");
        eaPush(&texWord->layers, layer);
        texWordVerify(texWord, true);

        // Add to global lists
        eaPush(&texWords_list.texWords, texWord);
        // Extract texture name
        strcpy(textureName, getFileName(texWord->name));
        s = strrchr(textureName, '.');
        if (s)
            *s=0;
        stashAddPointer(htTexWords, textureName, texWord, false);

        if (texFind(textureName)==NULL) {
            // Dynamic texture
            TexWordParams *params = createTexWordParams();

            if (tweditor_state.bind != white_tex) {
                texRemoveRefBasic(tweditor_state.bind);
            }
            params->layoutName = allocAddString(textureName);
            tweditor_state.bind = texLoadDynamic(params, TEX_LOAD_IN_BACKGROUND, TEX_FOR_UI, NULL)->tex_layers[TEXLAYER_BASE];
            tweditor_state.bind = tweditor_state.bind->actualTexture;
        } else {
            tweditor_state.bind->texWord = texWordFind(tweditor_state.bind->name, 0);
        }
        tweditor_state.texWord = tweditor_state.bind->texWord;
        if (tweditor_state.bind->hasBeenComposited || tweditor_state.bind->texWord) {
            // Free the old composited data
            texFree(tweditor_state.bind, 0);
        }
        tweditor_state.needToRebuild = 1;
        texWordsEdit_undoReset();
        texForceTexLoaderToComplete(1);
    }
}


int texWordsEdit_leftMouse(int newstate)
{
    if (tweditor_state.extents.edit) {
        if (!newstate && tweditor_state.extents.mousegrabbed) {
            tweditor_state.extents.mousegrabbed=0;
            texWordsEdit_layerExtentsApply();
            return 1;
        }
        if (newstate) {
            // Check for grab on corner
            if (tweditor_state.extents.mask) {
                tweditor_state.extents.mousegrabbed=1;
            }
        }
    }
    return 0;
}


static void textureSpaceToScreenSpace(F32 *x, F32 *y)
{
    int screenX, screenY;
    windowClientSize(&screenX, &screenY);
    if (x)
        *x = tweditor_state.display.offsx + *x * tweditor_state.display.scalex;
    if (y)
        *y = screenY - (tweditor_state.display.offsy + *y * tweditor_state.display.scaley);
}

static void screenSpaceToTextureSpace(F32 *x, F32 *y)
{
    int screenX, screenY;
    windowClientSize(&screenX, &screenY);
    if (x)
        *x = (*x - tweditor_state.display.offsx) / tweditor_state.display.scalex;
    if (y)
        *y = ((screenY - *y) - tweditor_state.display.offsy) / tweditor_state.display.scaley;
}

static bool nearPoint(F32 x0, F32 y0, F32 x1, F32 y1) {
    return (SQR(y1-y0) + SQR(x1-x0) <= SQR(5));
}

static bool nearLine(F32 x0, F32 x1) {
    return (SQR(x1-x0) < SQR(4));
}

static bool inRange(F32 x, F32 x0, F32 x1) {
    return (x > x0 && x<x1);
}

void texWordsEdit_mouseMove(int x, int y)
{
    static int x0, y0;
    static int cursorHotX, cursorHotY;
    static F32 oldTexX, oldTexY;
    int dx, dy;
    int screenX, screenY;
    dx = x - x0;
    dy = y - y0;

    x0 = x;
    y0 = y;

    windowClientSize(&screenX, &screenY);
    y = screenY - y;

    if (tweditor_state.extents.edit && !tweditor_state.extents.mousegrabbed) {
        F32 x0 = tweditor_state.extents.x0,
            x1 = tweditor_state.extents.x1,
            y0 = tweditor_state.extents.y0,
            y1 = tweditor_state.extents.y1;
        textureSpaceToScreenSpace(&x0, &y0);
        textureSpaceToScreenSpace(&x1, &y1);

        // Detect what corner the mouse is over
        if (nearPoint(x, y, x0, y0)) {
            cursorImage = "cursor_win_diagresize_foreward.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_X0|MASK_Y0;
        } else if (nearPoint(x, y, x0, y1)) {
            cursorImage = "cursor_win_diagresize_backward.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_X0|MASK_Y1;
        } else if (nearPoint(x, y, x1, y0)) {
            cursorImage = "cursor_win_diagresize_backward.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_X1|MASK_Y0;
        } else if (nearPoint(x, y, x1, y1)) {
            cursorImage = "cursor_win_diagresize_foreward.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_X1|MASK_Y1;
        } else if (nearLine(x, x0) && inRange(y, y1, y0)) {
            cursorImage = "cursor_win_horizresize.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 15; cursorHotY = 10;
            tweditor_state.extents.mask = MASK_X0;
        } else if (nearLine(x, x1) && inRange(y, y1, y0)) {
            cursorImage = "cursor_win_horizresize.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 15; cursorHotY = 10;
            tweditor_state.extents.mask = MASK_X1;
        } else if (nearLine(y, y0) && inRange(x, x0, x1)) {
            cursorImage = "cursor_win_vertresize.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_Y0;
        } else if (nearLine(y, y1) && inRange(x, x0, x1)) {
            cursorImage = "cursor_win_vertresize.tga";
            cursorImageOverlay = NULL;
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_Y1;
        } else if (inRange(x, x0, x1) && inRange(y, y1, y0)) {
            cursorImage = "cursor_win_diagresize_backward.tga";
            cursorImageOverlay = "cursor_win_diagresize_foreward.tga";
            cursorHotX = 10; cursorHotY = 15;
            tweditor_state.extents.mask = MASK_MOVE;
            oldTexX=x;
            oldTexY=y;
            screenSpaceToTextureSpace(&oldTexX, &oldTexY);
            oldTexX = round(oldTexX);
            oldTexY = round(oldTexY);
        } else {
            cursorImage = NULL;
            cursorImageOverlay = NULL;
            tweditor_state.extents.mask = 0;
        }
    } else if (tweditor_state.extents.edit && tweditor_state.extents.mousegrabbed) {
        F32 texX=x, texY=y;
        // Leave cursor!
        // Update values
        screenSpaceToTextureSpace(&texX, &texY);
        texX = round(texX);
        texY = round(texY);
        if (tweditor_state.extents.mask & MASK_X0) {
            if (texX >= 0 && texX < tweditor_state.extents.x1) {
                tweditor_state.extents.x0 = texX;
            }
        }
        if (tweditor_state.extents.mask & MASK_X1) {
            if (texX <= tweditor_state.bind->width && texX > tweditor_state.extents.x0) {
                tweditor_state.extents.x1 = texX;
            }
        }
        if (tweditor_state.extents.mask & MASK_Y0) {
            if (texY >= 0 && texY < tweditor_state.extents.y1) {
                tweditor_state.extents.y0 = texY;
            }
        }
        if (tweditor_state.extents.mask & MASK_Y1) {
            if (texY <= tweditor_state.bind->height && texY > tweditor_state.extents.y0) {
                tweditor_state.extents.y1 = texY;
            }
        }
        if (tweditor_state.extents.mask & MASK_MOVE) {
            F32 dX = texX - oldTexX;
            F32 dY = texY - oldTexY;
            tweditor_state.extents.x0+=dX;
            tweditor_state.extents.x1+=dX;
            tweditor_state.extents.y0+=dY;
            tweditor_state.extents.y1+=dY;
            oldTexX = texX;
            oldTexY = texY;
        }

    } else {
        cursorImage = NULL;
        cursorImageOverlay = NULL;
    }

    if (cursorImage) {
        setCursor(cursorImage, cursorImageOverlay, FALSE, cursorHotX, cursorHotY);
    }

    return;
}

static void texWordsEditorResetState(void)
{
    memset(&tweditor_state, 0, sizeof(tweditor_state));

    tweditor_state.stretch = 1;
    tweditor_state.bgcolor[0]=0;
    tweditor_state.bgcolor[1]=0;
    tweditor_state.bgcolor[2]=0;
    tweditor_state.bgcolor[3]=0;
    tweditor_state.selcolor.a=255;
    drawMeBind = NULL;
}

void texWordsEditorKeybindInit(){
    static int texWordEditorInitialized = 0;

    if(!texWordEditorInitialized){
        texWordEditorInitialized = 1;

        cmdOldInit(texwordseditor_cmds);

        texWordsEditorResetState();

        bindKeyProfile(&texwordseditor_binds_profile);
        memset(&texwordseditor_binds_profile, 0, sizeof(texwordseditor_binds_profile));
        texwordseditor_binds_profile.name = "TexWordsEditor";
        texwordseditor_binds_profile.parser = texWordsEditorCmdParse;
        texwordseditor_binds_profile.trickleKeys = 1;


        bindKey("space",    "++stretch",        0);
        bindKey("c",        "++colorpick",        0);
        bindKey("j",        "layerunhideall",    0);
        bindKey("tab",        "++orig",            0);
        bindKey("esc",        "quit",                0);
        bindKey("m",        "menu",                0);
        bindKey("rbutton",    "menu",                0);
        bindKey("F5",        "refresh",            0);
        bindKey("F6",        "++usegl",            0);
        bindKey("F7",        "filesave",            0);
        bindKey("lbutton",    "+lbutton",            0);
        bindKey("lshift",    "+shift",            0);
        bindKey("enter",    "apply",            0);
        bindKey("ctrl+z",    "undo",                0);
        bindKey("F12",        "undo",                0);
        bindKey("ctrl+y",    "redo",                0);
        bindKey("shift+comma",    "fileloadnext 0 0",    0);
        bindKey("shift+period",    "fileloadnext 1 0",    0);
        bindKey("comma",    "fileloadnext 0 0",    0);
        bindKey("period",    "fileloadnext 1 0",    0);
        bindKey("1",        "layerextents 1",    0);
        bindKey("2",        "layerextents 2",    0);
        bindKey("3",        "layerextents 3",    0);
        bindKey("4",        "layerextents 4",    0);
        bindKey("5",        "layerextents 5",    0);
        bindKey("6",        "layerextents 6",    0);
        bindKey("7",        "layerextents 7",    0);
        bindKey("8",        "layerextents 8",    0);
        bindKey("9",        "layerextents 9",    0);

        unbindKeyProfile(&texwordseditor_binds_profile);
    }
}

char *textureToBaseTexWord(char *texturePath)
{
    // texture_library/path/file#Base.texture or
    // c:\game\data\texture_library/path/file#Base.texture
    // to
    // texts/Base/texture_library/path/file.texword
    static char buf[1024];
    char *path, *ext;
    forwardSlashes(texturePath);
    if (strStartsWith(texturePath, fileDataDir())) {
        texturePath += strlen(fileDataDir());
    }
    while (*texturePath=='/') ++texturePath;

    if (strStartsWith(texturePath, "texture_library")) {
        path = strchr(texturePath, '/');
        path++;
    } else {
        path = texturePath;
    }
    sprintf(buf, "texts/Base/texture_library/%s", path);
    ext = strrchr(buf, '.');
    if (strrchr(buf, '#'))
        ext = strrchr(buf, '#');
    if (ext) {
        strcpy(ext, ".texword");
    } else {
        strcat(buf, ".texword");
    }
    return buf;
}

char *baseTexWordToTexture(char *texwordPath)
{
    // texts/Base/texture_library/path/file.texword
    // or c:/game/data/texts/Base/texture_library/path/file.texword
    // to
    // texture_library/path/file.texture // This file might not exist
    static char buf[1024];
    char *path, *ext;
    forwardSlashes(texwordPath);
    if (strStartsWith(texwordPath, fileDataDir())) {
        texwordPath += strlen(fileDataDir());
    }
    while (*texwordPath=='/') ++texwordPath;

    if (strStartsWith(texwordPath, "texts/")) {
        path = strchr(texwordPath, '/');
        path++;
    } else {
        path = texwordPath;
    }
    if (strStartsWith(path, "Base/")) {
        path = strchr(path, '/');
        path++;
    } else {
        path = path;
    }
    if (strStartsWith(path, "texture_library/")) {
        path = strchr(path, '/');
        path++;
    } else {
        path = path;
    }
    sprintf(buf, "texture_library/%s", path);
    ext = strrchr(buf, '.');
    if (ext) {
        strcpy(ext, ".texture");
    } else {
        strcat(buf, ".texture");
    }
    return buf;
}

static int weights0to100[] = {0, 10, 25, 50, 75, 90, 100};
static int weightsInts[] = {1, 2, 3, 4, 5, 7, 10, 15};
char *makeColorStringFromEArray(eaiHandle color) {
    static char colorstring[64];
    if (color) {
        sprintf(colorstring, "^#(%d,%d,%d,%d)%d,%d,%d,%d", color[0], color[1], color[2],
            eaiSize(&color)==4?color[3]:255,
            color[0], color[1], color[2],
            eaiSize(&color)==4?color[3]:255);
    } else {
        sprintf(colorstring, "255,255,255,255");
    }
    return colorstring;
}
char *makeColorStringFromColor(Color color) {
    static char colorstring[64];
    sprintf(colorstring, "^#(%d,%d,%d,%d)%d,%d,%d,%d", color.r, color.g, color.b, color.a,
        color.r, color.g, color.b, color.a);
    return colorstring;
}

static void addColorChoice(DebugMenuItem *root, eaiHandle currentColor, char *prefix, char *name, char *setCommand, char *alphaCommand)
{
    char buf[1024];
    char buf2[1024];
    DebugMenuItem *sub, *alphamenu;
    int i;
    F32 alpha;
    if (eaiSize(&currentColor)==4) {
        alpha = currentColor[3]/255.f;
    } else {
        alpha = 1.0;
    }

    sprintf(buf, "COLOR%s$$%s: %s", prefix, name, makeColorStringFromEArray(currentColor));
    sub = addDebugMenuItem(root, buf, "Change the color", 0);

    sprintf(buf, "Set to current color: %s", makeColorStringFromColor(tweditor_state.selcolor));
    addDebugMenuItem(sub, buf, setCommand, 0);

    sprintf(buf, "ALPHA%s$$Set Opacity (%d%%)", prefix, round(alpha*100));
    alphamenu = addDebugMenuItem(sub, buf, "Set Alpha/Opacity", 0);

    for (i=1; i<ARRAY_SIZE(weights0to100); i++) {
        sprintf(buf, "%d", round(weights0to100[i]*255/100.f));
        sprintf(buf2, alphaCommand, buf);

        sprintf(buf, "%d%%", weights0to100[i]);
        if (round(alpha*100) == weights0to100[i]) {
            strcat(buf, " ^2[Selected]");
        }
        addDebugMenuItem(alphamenu, buf, buf2, 0);
    }
    sprintf(buf2, alphaCommand, "ASK");
    addDebugMenuItem(alphamenu, "ASK", buf2, 0);
}

static void addLayerToMenu(DebugMenuItem *root, TexWordLayer *layer, TexWordLayer *parentlayer, int parentIndex, int relativeIndex, int depth, TexWordLayer **allLayers, char *parentLayerString) {
    DebugMenuItem *layeritem;
    DebugMenuItem *sub;
    char buf[1024];
    char buf2[1024];
    char buf3[1024];
    char buf4[1024];
    int i;
    int numLayers = eaSize(&allLayers);
    char indexString[32];
    char indexStringPrint[32];
    int index;

    // Calculate internal index number
    index = parentIndex + ((relativeIndex+1) * pow(100,depth));

    if (parentLayerString) {
        sprintf(indexStringPrint, "%s.%s", parentLayerString, layer->layerName);
    } else {
        sprintf(indexStringPrint, "%s", layer->layerName);
    }
    sprintf(indexString, "%d", index);

    if (parentLayerString) {
        sprintf(buf, "Sub-Layer ^7%s", indexStringPrint);
        layeritem = addDebugMenuItem(root, buf, "A sub layer is blended with the layer\nit's attached to", 0);

        // Blend mode
        sprintf(buf, "BLEND%s$$Blend Mode: ^5%s", indexString, StaticDefineIntRevLookup(ParseTexWordBlendType, parentlayer->subBlend));
        sub = addDebugMenuItem(layeritem, buf, "Change the way the sub layer is blended\nwith it's parent layer", 0);
        
        sprintf(buf, "layersubblend %d %d", parentIndex, TWBLEND_MULTIPLY);
        addDebugMenuItem(sub, "Change to ^4Multiply", buf, 0);
        sprintf(buf, "layersubblend %d %d", parentIndex, TWBLEND_OVERLAY);
        addDebugMenuItem(sub, "Change to ^4Overlay", buf, 0);
        sprintf(buf, "layersubblend %d %d", parentIndex, TWBLEND_ADD);
        addDebugMenuItem(sub, "Change to ^4Add", buf, 0);
        sprintf(buf, "layersubblend %d %d", parentIndex, TWBLEND_SUBTRACT);
        addDebugMenuItem(sub, "Change to ^4Subtract", buf, 0);

        sprintf(buf, "BLENDWEIGHT%s$$Blend Weight: ^5%1.f%%", indexString, parentlayer->subBlendWeight*100);
        sub = addDebugMenuItem(layeritem, buf, "Change the weight of the blending between\nlayers", 0);
        {
            for (i=1; i<ARRAY_SIZE(weights0to100); i++) {
                sprintf(buf, "%d%%", weights0to100[i]);
                if (parentlayer->subBlendWeight == weights0to100[i]/100.0f) {
                    strcat(buf, " ^2[Selected]");
                }
                sprintf(buf2, "layersubblendweight %d %.2f", parentIndex, weights0to100[i]/100.f);
                addDebugMenuItem(sub, buf, buf2, 0);
            }
            sprintf(buf2, "layersubblendweight %d ASK", parentIndex);
            addDebugMenuItem(sub, "ASK", buf2, 0);
        }
    } else {
        sprintf(buf, "Layer ^7%s ^5(%s)", indexStringPrint, StaticDefineIntRevLookup(ParseTexWordLayerType, layer->type));
        layeritem = addDebugMenuItem(root, buf, NULL, 0);
    }

    // Name
    sprintf(buf2, "layername %s", indexString);
    sub = addDebugMenuItem(layeritem, "Change Name", buf2, 0);

    // Type Select
    {
        sprintf(buf, "TYPE%s$$Type ^4%s", indexString, StaticDefineIntRevLookup(ParseTexWordLayerType, layer->type));
        sub = addDebugMenuItem(layeritem, buf, "Change the type of layer this is", 0);

        sprintf(buf, "layertype %s %d", indexString, TWLT_BASEIMAGE);
        addDebugMenuItem(sub, "Change to ^4BaseImage", buf, 0);
        sprintf(buf, "layertype %s %d", indexString, TWLT_TEXT);
        addDebugMenuItem(sub, "Change to ^4Text", buf, 0);
        sprintf(buf, "layertype %s %d", indexString, TWLT_IMAGE);
        addDebugMenuItem(sub, "Change to ^4Image", buf, 0);
    }

    // Hidden
    if (layer->hidden) {
        sprintf(buf, "HIDE%s$$Hidden: ^1ON", indexString);
        sub = addDebugMenuItem(layeritem, buf, "This layer is hidden and will not be rendered", 0);
        {
            sprintf(buf, "layerhide %s 0", indexString);
            addDebugMenuItem(sub, "^2Unhide layer", buf, 0);

            if (!parentLayerString) {
                sprintf(buf, "layersolo %s", indexString);
                addDebugMenuItem(sub, "Hide all other layers", buf, 0);
            }
        }
    } else {
        sprintf(buf, "HIDE%s$$Hidden: ^5OFF", indexString);
        sub = addDebugMenuItem(layeritem, buf, "This layer is not hidden", 0);
        {
            sprintf(buf, "layerhide %s 1", indexString);
            addDebugMenuItem(sub, "^1Hide layer", buf, 0);

            if (!parentLayerString) {
                sprintf(buf, "layersolo %s", indexString);
                addDebugMenuItem(sub, "Hide all other layers", buf, 0);
            }
        }
    }

    if (layer->stretch != TWLS_FULL || layer->type == TWLT_TEXT) {
        if (tweditor_state.extents.edit && tweditor_state.extents.index == index) {
            sprintf(buf, "layerextents %s", indexString);
            addDebugMenuItem(layeritem, "^8Edit Extents ^2[ON]", buf, 0);
        } else {
            sprintf(buf, "layerextents %s", indexString);
            addDebugMenuItem(layeritem, "^8Edit Extents", buf, 0);
        }
    }

    if (layer->type == TWLT_IMAGE)
    {
        DebugMenuItem *sub2;
        // Image selection
        sprintf(buf, "TEXTURE%s$$Image ^2%s", indexString, layer->imageName);
        sub = addDebugMenuItem(layeritem, buf, NULL, 0);

        sprintf(buf, "layerimage %s browse", indexString);
        addDebugMenuItem(sub, "Browse for image...", buf, 0);
        sprintf(buf, "layerimage %s white", indexString);
        addDebugMenuItem(sub, "White", buf, 0);

        mruUpdate(mruTextures);
        if (mruTextures->count) {
            sub2 = addDebugMenuItem(sub, "Most Recently Used", "List of most recently used textures", 0);
            for (i=mruTextures->count-1; i>=0; i--) {
                sprintf(buf, "layerimage %s %s", indexString, mruTextures->values[i]);
                addDebugMenuItem(sub2, mruTextures->values[i], buf, 0);
            }
        }
    }

    if (layer->type == TWLT_BASEIMAGE || layer->type == TWLT_IMAGE) // Or for everything?
    { // Stretch
        sprintf(buf, "STRETCH%s$$Stretch ^5%s", indexString, StaticDefineIntRevLookup(ParseTexWordLayerStretch, layer->stretch));
        sub = addDebugMenuItem(layeritem, buf, NULL, 0);

        sprintf(buf, "layerstretch %s %d", indexString, TWLS_NONE);
        addDebugMenuItem(sub, "Change to ^4None", buf, 0);

        sprintf(buf, "layerstretch %s %d", indexString, TWLS_TILE);
        addDebugMenuItem(sub, "Change to ^4Tile", buf, 0);

        sprintf(buf, "layerstretch %s %d", indexString, TWLS_FULL);
        addDebugMenuItem(sub, "Change to ^4Full", buf, 0);
    }

    if (layer->type == TWLT_TEXT)
    { // Text
        char text[2048], *s;
        wchar_t wbuffer[2048]={0};
        int i;
        int len;
        len = sprintf(text, "Text: ^2");
        s = text + len;
        msPrintf(texWordsMessages, s, ARRAY_SIZE(text) - len, layer->text);
    //    UTF8ToWideStrConvert(s, wbuffer, ARRAY_SIZE(wbuffer));
    //    wcstombs(s, wbuffer, ARRAY_SIZE(text)-(s-text));

        sprintf(buf, "layertextchange %s", indexString);
        addDebugMenuItem(layeritem, text, buf, 0);

        // Font
        sprintf(buf, "FONT%s$$Font: ^5%s", indexString, layer->font.fontName?layer->font.fontName:"arial.ttf");
        sub = addDebugMenuItem(layeritem, buf, "", 0);
        for (i=0; i<eaSize(&ttFontCache); i++) {
            sprintf(buf, "layerfont %s \"%s\"", indexString, ttFontCache[i]->name);
            addDebugMenuItem(sub, ttFontCache[i]->name, buf, 0);
        }
        // Font style
        sub = addDebugMenuItem(layeritem, "Font Style", "", 0);
        {
            TexWordLayerFont *font = &layer->font;
            DebugMenuItem *sub2, *sub3;

            if (font->bold) {
                sprintf(buf2, "layerfontstyle %s bold 0", indexString);
                addDebugMenuItem(sub, "Bold ^2[ON]", buf2, 0);
            } else {
                sprintf(buf2, "layerfontstyle %s bold 1", indexString);
                addDebugMenuItem(sub, "Bold ^1[OFF]", buf2, 0);
            }

            if (font->italicize) {
                sprintf(buf2, "layerfontstyle %s italic 0", indexString);
                addDebugMenuItem(sub, "Italic ^2[ON]", buf2, 0);
            } else {
                sprintf(buf2, "layerfontstyle %s italic 1", indexString);
                addDebugMenuItem(sub, "Italic ^1[OFF]", buf2, 0);
            }

            if (!font->dropShadowXOffset && !font->dropShadowYOffset) {
                sprintf(buf, "DROPSHADOW%s$$Drop Shadow: ^5NONE", indexString);
            } else if (font->dropShadowXOffset == font->dropShadowYOffset) {
                sprintf(buf, "DROPSHADOW%s$$Drop Shadow: ^5%d", indexString, font->dropShadowXOffset);
            } else {
                sprintf(buf, "DROPSHADOW%s$$Drop Shadow: ^5%d,%d", indexString, font->dropShadowXOffset, font->dropShadowYOffset);
            }
            sub2 = addDebugMenuItem(sub, buf, " ", 0);
            sub3 = addDebugMenuItem(sub2, "X/Y Offset", "Change both the X and Y offset\n of the dropshadow at once", 0);
            for (i=0; i<5; i++) {
                if (i==0) {
                    sprintf(buf, "No dropshadow");
                } else {
                    sprintf(buf, "%d", i);
                }
                if (font->dropShadowXOffset == font->dropShadowYOffset && font->dropShadowXOffset == i) {
                    strcat(buf, " ^2[Selected]");
                }
                sprintf(buf2, "layerfontstyle %s dropxyoffs %d", indexString, i);
                addDebugMenuItem(sub3, buf, buf2, 0);
            }
            sub3 = addDebugMenuItem(sub2, "X Offset", "Change the X offset of the\n dropshadow", 0);
            for (i=0; i<5; i++) {
                sprintf(buf, "%d", i);
                if (font->dropShadowXOffset == i) {
                    strcat(buf, " ^2[Selected]");
                }
                sprintf(buf2, "layerfontstyle %s dropxoffs %d", indexString, i);
                addDebugMenuItem(sub3, buf, buf2, 0);
            }
            sub3 = addDebugMenuItem(sub2, "Y Offset", "Change the Y offset of the\n dropshadow", 0);
            for (i=0; i<5; i++) {
                sprintf(buf, "%d", i);
                if (font->dropShadowYOffset == i) {
                    strcat(buf, " ^2[Selected]");
                }
                sprintf(buf2, "layerfontstyle %s dropyoffs %d", indexString, i);
                addDebugMenuItem(sub3, buf, buf2, 0);
            }


            if (font->outlineWidth) {
                sprintf(buf, "OUTLINE%s$$Outline: ^5%dpx", indexString, font->outlineWidth);
            } else {
                sprintf(buf, "OUTLINE%s$$Outline: ^5None", indexString);
            }
            sub2 = addDebugMenuItem(sub, buf, "Change the outline settings", 0);
            for (i=0; i<=10; i++) {
                if (i) {
                    sprintf(buf, "%d", i);
                } else {
                    sprintf(buf, "None");
                }
                if (i==font->outlineWidth) {
                    strcat(buf, " ^2[Selected]");
                }
                sprintf(buf2, "layerfontstyle %s outline %d", indexString, i);
                addDebugMenuItem(sub2, buf, buf2, 0);
            }

            if (font->drawSize) {
                sprintf(buf, "FONTSIZE%s$$Font Size: ^5%d", indexString, font->drawSize);
            } else {
                sprintf(buf, "FONTSIZE%s$$Font Size: ^5Auto", indexString);
            }
            sub2 = addDebugMenuItem(sub, buf, "Font size affects the size the font is drawn at\nbefore stretching to fit the designated\nextents.  Setting this value as close to the\nactual size will produce the best results.", 0);
            {
                int sizes[] = {8,12,16,32,64,128};
                sprintf(buf, "Auto");
                if (font->drawSize == 0) {
                    strcat(buf, " ^2[Selected]");
                }
                sprintf(buf2, "layerfontstyle %s size %d", indexString, 0);
                addDebugMenuItem(sub2, buf, buf2, 0);
                for (i=0; i<ARRAY_SIZE(sizes); i++) {
                    sprintf(buf, "%d", sizes[i]);
                    if (font->drawSize == sizes[i]) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfontstyle %s size %d", indexString, sizes[i]);
                    addDebugMenuItem(sub2, buf, buf2, 0);
                }
            }

        }
    }

    // Color
    if (layer->rgbas[0]) {
        // 4-color mode
        sprintf(buf, "COLOR%s$$Color ^5(4-Color mode)", indexString);
        sub = addDebugMenuItem(layeritem, buf, "", 0);
        sprintf(buf, "layercolormode %s 1", indexString);
        addDebugMenuItem(sub, "Switch to ^5Single-color^0 mode", buf, 0);
        for (i=0; i<4; i++) {
            sprintf(buf, "Color%s",
                i==0?"UL":
                i==1?"UR":
                i==2?"LR":
                i==3?"LL":0);
            sprintf(buf2, "layercolorchange %s %d", indexString, i+1);
            sprintf(buf3, "%s-%d", indexString, i);
            sprintf(buf4, "layeralphachange %s %d %%s", indexString, i+1);
            addColorChoice(sub, layer->rgbas[i], buf3, buf, buf2, buf4);
        }
    } else {
        // Single color mode
        sprintf(buf, "COLOR%s$$Color ^5(Single-color mode)", indexString);
        sub = addDebugMenuItem(layeritem, buf, "", 0);
        sprintf(buf, "layercolormode %s 4", indexString);
        addDebugMenuItem(sub, "Switch to ^54-Color^0 mode", buf, 0);

        sprintf(buf, "layercolorchange %s 0", indexString);
        sprintf(buf2, "layeralphachange %s 0 %%s", indexString);
        addColorChoice(sub, layer->rgba, indexString, "Color", buf, buf2);
    }

    if (layer->stretch != TWLS_FULL || layer->type == TWLT_TEXT) {
        // Size : WxH
        sprintf(buf, "SZE%s$$Size ^5%1.f^9x^5%1.f", indexString, layer->size[0], layer->size[1]);
        sub = addDebugMenuItem(layeritem, buf, "Change size", 0);
        //   Type in values
        sprintf(buf, "Width: %1.f", layer->size[0]);
        sprintf(buf2, "layerask %s WIDTH", indexString);
        addDebugMenuItem(sub, buf, buf2, 0);
        sprintf(buf, "Height: %1.f", layer->size[1]);
        sprintf(buf2, "layerask %s HEIGHT", indexString);
        addDebugMenuItem(sub, buf, buf2, 0);
        //   Grab from Layer 1,2,3..
        {
            int i;
            DebugMenuItem *copyMenu = addDebugMenuItem(sub, "Copy from layer", "", 0);
            for (i=0; i<eaSize(&allLayers); i++) if ((i+1)!=index) {
                sprintf(buf, "Layer ^7%s^0 (%1.fx%1.f)", allLayers[i]->layerName, allLayers[i]->size[0], allLayers[i]->size[1]);
                sprintf(buf2, "layercopysize %s %d", indexString, i+1);
                addDebugMenuItem(copyMenu, buf, buf2, 0);
            }
        }

        // Pos : X,Y
        sprintf(buf, "POS%s$$Pos ^5%1.f^9x^5%1.f", indexString, layer->pos[0], layer->pos[1]);
        sub = addDebugMenuItem(layeritem, buf, "Change position", 0);
        //   Type in values
        sprintf(buf, "X: %1.f", layer->pos[0]);
        sprintf(buf2, "layerask %s X", indexString);
        addDebugMenuItem(sub, buf, buf2, 0);
        sprintf(buf, "Y: %1.f", layer->pos[1]);
        sprintf(buf2, "layerask %s Y", indexString);
        addDebugMenuItem(sub, buf, buf2, 0);
        //   Grab from Layer 1,2,3..
        {
            int i;
            DebugMenuItem *copyMenu = addDebugMenuItem(sub, "Copy from layer", "", 0);
            for (i=0; i<eaSize(&allLayers); i++) if ((i+1)!=index) {
                sprintf(buf, "Layer ^7%s^0 (%1.f,%1.f)", allLayers[i]->layerName, allLayers[i]->pos[0], allLayers[i]->pos[1]);
                sprintf(buf2, "layercopypos %s %d", indexString, i+1);
                addDebugMenuItem(copyMenu, buf, buf2, 0);
            }
        }

        // Rotation : R
        sprintf(buf, "ROT%s$$Rotation ^5%1.f^9 degrees", indexString, layer->rot);
        sub = addDebugMenuItem(layeritem, buf, "Change rotation", 0);
        //   Type in values
        sprintf(buf, "Rotation: %1.f degrees", layer->rot);
        sprintf(buf2, "layerask %s ROT", indexString);
        addDebugMenuItem(sub, buf, buf2, 0);
        //   Grab from Layer 1,2,3..
        {
            int i;
            DebugMenuItem *copyMenu = addDebugMenuItem(sub, "Copy from layer", "", 0);
            for (i=0; i<eaSize(&allLayers); i++) if ((i+1)!=index) {
                sprintf(buf, "Layer ^7%s^0 (%1.f)", allLayers[i]->layerName, allLayers[i]->rot);
                sprintf(buf2, "layercopyrot %s %d", indexString, i+1);
                addDebugMenuItem(copyMenu, buf, buf2, 0);
            }
        }
    }

    // Filters
    {
        DebugMenuItem *allFiltersMenu;
        int filterIndex;
        sprintf(buf, "FILTERS%s$$Filters (%d)", indexString, eaSize(&layer->filter));
        allFiltersMenu = addDebugMenuItem(layeritem, buf, "", 0);

        for (filterIndex=0; filterIndex<eaSize(&layer->filter); filterIndex++) 
        {
            TexWordLayerFilter *filter = layer->filter[filterIndex];
            DebugMenuItem *filterMenu, *sub2, *sub3;
            // Has a filter
            if (filter->type == TWFILTER_DESATURATE) {
                sprintf(buf, "FILTER%s-%d$$Filter: ^5%s ^6%1.f^5%%", indexString, filterIndex, StaticDefineIntRevLookup(ParseTexWordFilterType, filter->type), filter->percent*100);
            } else {
                sprintf(buf, "FILTER%s-%d$$Filter: ^5%s ^6%d^5px", indexString, filterIndex, StaticDefineIntRevLookup(ParseTexWordFilterType, filter->type), filter->magnitude);
            }
            filterMenu = addDebugMenuItem(allFiltersMenu, buf, "This layer has a filter", 0);

            // Filter|Type menu
            { // Type Select
                sprintf(buf, "FILTERTYPE%s-%d$$Type ^4%s", indexString, filterIndex, StaticDefineIntRevLookup(ParseTexWordFilterType, filter->type));
                sub = addDebugMenuItem(filterMenu, buf, "Change the type of filtering", 0);

                sprintf(buf, "layerfiltertype %s %d %d", indexString, filterIndex, TWFILTER_BLUR);
                addDebugMenuItem(sub, "Change to ^4Blur", buf, 0);
                sprintf(buf, "layerfiltertype %s %d %d", indexString, filterIndex, TWFILTER_DROPSHADOW);
                addDebugMenuItem(sub, "Change to ^4DropShadow", buf, 0);
                sprintf(buf, "layerfiltertype %s %d %d", indexString, filterIndex, TWFILTER_DESATURATE);
                addDebugMenuItem(sub, "Change to ^4Desaturate", buf, 0);
            }

            // Filter|Magnitude
            if (filter->type == TWFILTER_BLUR || filter->type == TWFILTER_DROPSHADOW)
            {
                sprintf(buf, "MAGNITUDE%s-%d$$Magnitude: ^5%dpx", indexString, filterIndex, filter->magnitude);
                sub = addDebugMenuItem(filterMenu, buf, "Change the magnitude of the effect", 0);
                for (i=0; i<ARRAY_SIZE(weightsInts); i++) {
                    sprintf(buf, "%d", weightsInts[i]);
                    if (filter->magnitude == weightsInts[i]) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfilterstyle %s %d magnitude %d", indexString, filterIndex, weightsInts[i]);
                    addDebugMenuItem(sub, buf, buf2, 0);
                }
                sprintf(buf2, "layerfilterstyle %s %d magnitude ASK", indexString, filterIndex);
                addDebugMenuItem(sub, "ASK", buf2, 0);
            }

            // Filter|Percent
            if (filter->type == TWFILTER_DESATURATE) {
                sprintf(buf, "PERCENTAGE%s-%d$$Percentage: ^5%1.f%%", indexString, filterIndex, filter->percent*100);
                sub = addDebugMenuItem(filterMenu, buf, "Change the magnitude of the effect", 0);
                for (i=0; i<ARRAY_SIZE(weights0to100); i++) {
                    sprintf(buf, "%d%%", weights0to100[i]);
                    if (filter->magnitude == weights0to100[i]/100.f) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfilterstyle %s %d percent %.2f", indexString, filterIndex, weights0to100[i]/100.f);
                    addDebugMenuItem(sub, buf, buf2, 0);
                }
                sprintf(buf2, "layerfilterstyle %s %d percent ASK", indexString, filterIndex);
                addDebugMenuItem(sub, "ASK", buf2, 0);
            }

            // Filter|Spread
            if (filter->type == TWFILTER_DROPSHADOW) {
                sprintf(buf, "SPREAD%s-%d$$Spread: ^5%1.f%%", indexString, filterIndex, filter->spread*100.f);
                sub = addDebugMenuItem(filterMenu, buf, "Change the spread", 0);
                for (i=0; i<ARRAY_SIZE(weights0to100); i++) {
                    sprintf(buf, "%d%%", weights0to100[i]);
                    if (filter->spread == weights0to100[i]/100.0f) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfilterstyle %s %d spread %.2f", indexString, filterIndex, weights0to100[i]/100.f);
                    addDebugMenuItem(sub, buf, buf2, 0);
                }
                sprintf(buf2, "layerfilterstyle %s %d spread ASK", indexString, filterIndex);
                addDebugMenuItem(sub, "ASK", buf2, 0);
            }

            // Filter|Offset
            if (filter->type == TWFILTER_BLUR || filter->type == TWFILTER_DROPSHADOW) {
                sprintf(buf, "FILTEROFFSET%s-%d$$Offset: ^5%d,%d", indexString, filterIndex, filter->offset[0], filter->offset[1]);
                sub2 = addDebugMenuItem(filterMenu, buf, "Change the offset of the filter", 0);
                sub3 = addDebugMenuItem(sub2, "X/Y Offset", "Change both the X and Y offset\n of the filter at once", 0);
                for (i=-10; i<=10; i++) {
                    sprintf(buf, "%d", i);
                    if (filter->offset[0] == filter->offset[1] && filter->offset[1] == i) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfilterstyle %s %d offsetXY %d", indexString, filterIndex, i);
                    addDebugMenuItem(sub3, buf, buf2, 0);
                }
                sprintf(buf2, "layerfilterstyle %s %d offsetXY ASK", indexString, filterIndex);
                addDebugMenuItem(sub3, "ASK", buf2, 0);

                sub3 = addDebugMenuItem(sub2, "X Offset", "Change the X offset of the\n dropshadow", 0);
                for (i=-10; i<=10; i++) {
                    sprintf(buf, "%d", i);
                    if (filter->offset[0] == i) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfilterstyle %s %d offsetX %d", indexString, filterIndex, i);
                    addDebugMenuItem(sub3, buf, buf2, 0);
                }
                sprintf(buf2, "layerfilterstyle %s %d offsetX ASK", indexString, filterIndex);
                addDebugMenuItem(sub3, "ASK", buf2, 0);

                sub3 = addDebugMenuItem(sub2, "Y Offset", "Change the Y offset of the\n dropshadow", 0);
                for (i=-10; i<=10; i++) {
                    sprintf(buf, "%d", i);
                    if (filter->offset[1] == i) {
                        strcat(buf, " ^2[Selected]");
                    }
                    sprintf(buf2, "layerfilterstyle %s %d offsetY %d", indexString, filterIndex, i);
                    addDebugMenuItem(sub3, buf, buf2, 0);
                }
                sprintf(buf2, "layerfilterstyle %s %d offsetY ASK", indexString, filterIndex);
                addDebugMenuItem(sub3, "ASK", buf2, 0);
            }

            // Filter|Color
            if (filter->type == TWFILTER_DROPSHADOW) {
                sprintf(buf, "FILTERCOLOR%s-%d", indexString, filterIndex);
                sprintf(buf2, "layerfiltercolor %s %d", indexString, filterIndex);
                sprintf(buf3, "layerfilteralpha %s %d %%s", indexString, filterIndex);
                addColorChoice(filterMenu, filter->rgba, buf, "Color", buf2, buf3);
            }

            // Filter|BlendMode
            // Blend mode
            sprintf(buf, "BLEND%s-%d$$Blend Mode: ^5%s", indexString, filterIndex, StaticDefineIntRevLookup(ParseTexWordBlendType, filter->blend));
            sub = addDebugMenuItem(filterMenu, buf, "Change the way the filter is blended\nwith the unfiltered layer", 0);

            sprintf(buf, "layerfilterblend %s %d %d", indexString, filterIndex, TWBLEND_MULTIPLY);
            addDebugMenuItem(sub, "Change to ^4Multiply", buf, 0);
            sprintf(buf, "layerfilterblend %s %d %d", indexString, filterIndex, TWBLEND_OVERLAY);
            addDebugMenuItem(sub, "Change to ^4Overlay", buf, 0);
            sprintf(buf, "layerfilterblend %s %d %d", indexString, filterIndex, TWBLEND_ADD);
            addDebugMenuItem(sub, "Change to ^4Add", buf, 0);
            sprintf(buf, "layerfilterblend %s %d %d", indexString, filterIndex, TWBLEND_SUBTRACT);
            addDebugMenuItem(sub, "Change to ^4Subtract", buf, 0);
            sprintf(buf, "layerfilterblend %s %d %d", indexString, filterIndex, TWBLEND_REPLACE);
            addDebugMenuItem(sub, "Change to ^4Replace", buf, 0);

            sprintf(buf2, "layerfilterdelete %s %d", indexString, filterIndex);
            addDebugMenuItem(filterMenu, "^1Remove Filter", buf2, 0);
        }
        // add filters
        {
            DebugMenuItem *insertFilter = addDebugMenuItem(allFiltersMenu, "Insert New Filter", " ", 0);
            sprintf(buf2, "layerfilterinsert %s 0", indexString);
            addDebugMenuItem(insertFilter, "On top", buf2, 0);
            for (i=0; i<eaSize(&layer->filter); i++) {
                if (i == eaSize(&layer->filter)-1) {
                    sprintf(buf, "On bottom");
                } else {
                    sprintf(buf, "Under filter %d", i+1);
                }
                sprintf(buf2, "layerfilterinsert %s %d", indexString, i+1);
                addDebugMenuItem(insertFilter, buf, buf2, 0);
            }
        }
    }

    // Delete
    if (parentLayerString) {
        sprintf(buf, "layerdelete %s", indexString);
        sprintf(buf2, "^1Delete Sub-Layer ^7%s", indexStringPrint);
        addDebugMenuItem(layeritem, buf2, buf, 0);
    } else {
        sprintf(buf, "layerclone %s", indexString);
        addDebugMenuItem(layeritem, "Duplicate Layer", buf, 0);
        sprintf(buf, "layerdelete %s", indexString);
        addDebugMenuItem(layeritem, "^1Delete Layer", buf, 0);
    }

    // Move
    if (relativeIndex != 0 && !parentLayerString) {
        sprintf(buf, "layerswap %d %d", relativeIndex, relativeIndex-1);
        addDebugMenuItem(layeritem, "Move up", buf, 0);
    }
    if (relativeIndex != numLayers-1 && !parentLayerString) {
        sprintf(buf, "layerswap %d %d", relativeIndex, relativeIndex+1);
        addDebugMenuItem(layeritem, "Move down", buf, 0);
    }

    // Sub-layers
    if (layer->sublayer && layer->sublayer[0]) {
        // Already have one
        addLayerToMenu(layeritem, layer->sublayer[0], layer, index, 0, depth+1, allLayers, indexStringPrint);
    } else {
        sprintf(buf, "layersubinsert %s", indexString);
        addDebugMenuItem(layeritem, "Insert New Sub-Layer", buf, 0);
    }

}



// File menu node stuff

typedef enum TWEError
{
    TWEERROR_MISSING=1<<0,
    TWEERROR_SIZEMISMATCH=1<<1,
    TWEERROR_ALPHAMISMATCH=1<<2,
} TWEError;

typedef struct TextureTreeNode {
    struct TextureTreeNode *child;
    struct TextureTreeNode *next;
    const char *name;
    BasicTexture *bind;
    int processed;
    TWEError error;
} TextureTreeNode;
MP_DEFINE(TextureTreeNode);

static TextureTreeNode *treeroot=NULL;

static void addNode(TextureTreeNode **node, char *name, BasicTexture *bind)
{
    char *s;
    TextureTreeNode *nadd;
    s = strchr(name, '/');
    if (!s) {
        // This is a terminal node
        if (*node==NULL) {
            // NULL list
            nadd = *node = MP_ALLOC(TextureTreeNode);
        } else {
            // Add to list, alphabetically
            TextureTreeNode *n=*node;
            TextureTreeNode *prev=NULL;
            nadd = MP_ALLOC(TextureTreeNode);
            while (n) {
                int stricmpres = stricmp(name, n->name);
                if (stricmpres==0) {
                    assert(0);
                } else if (stricmpres<0) {
                    // Insert before n
                    if (prev) {
                        prev->next = nadd;
                    } else {
                        // Insert at head of list
                        *node = nadd;
                    }
                    nadd->next = n;
                    break;
                } else {
                    // continue on!
                    prev = n;
                    n = n->next;
                }
            }
            if (!n) {
                // Insert at end of list
                prev->next = nadd;
            }
        }
        nadd->name = allocAddString(name);
        nadd->bind = bind;
        nadd->processed = !bind || !!bind->texWord;            

        // check for errors
        if (bind) {
            char origname[256];
            strcpy(origname, nadd->bind->name);
            if (strchr(origname, '#')) {
                BasicTexture *origbind;
                *strchr(origname, '#')=0;
                if (!(origbind = texFind(origname))) {
                    nadd->error |= TWEERROR_MISSING;
                } else {
                    if (origbind->width != bind->width ||
                        origbind->height != bind->height)
                    {
                        nadd->error |= TWEERROR_SIZEMISMATCH;
                    }
                    if ((origbind->flags & TEX_ALPHA) != (bind->flags & TEX_ALPHA))
                    {
                        nadd->error |= TWEERROR_ALPHAMISMATCH;
                    }
                }
            }
        }
    } else {
        // Just a node in the tree
        char first[MAX_PATH];
        char rest[MAX_PATH];
        strncpyt(first, name, s-name+1);
        strcpy(rest, s+1);
        if (*node == NULL) {
            nadd = *node = MP_ALLOC(TextureTreeNode);
        } else {
            // Add to list, alphabetically
            TextureTreeNode *n=*node;
            TextureTreeNode *prev=NULL;
            nadd = NULL;
            while (n) {
                int stricmpres = stricmp(first, n->name);
                if (stricmpres==0) {
                    nadd = n;
                    break;
                } else if (stricmpres<0) {
                    // Insert before n
                    if (prev) {
                        prev->next = nadd = MP_ALLOC(TextureTreeNode);
                    } else {
                        // Insert at head of list
                        *node = nadd = MP_ALLOC(TextureTreeNode);
                    }
                    nadd->next = n;
                    break;
                } else {
                    // continue on!
                    prev = n;
                    n = n->next;
                }
            }
            if (!nadd) {
                // Insert at end of list
                prev->next = nadd = MP_ALLOC(TextureTreeNode);
            }
        }
        if (!nadd->name)
            nadd->name = allocAddString(first);
        addNode(&nadd->child, rest, bind);
    } 
}

static void freeNode(TextureTreeNode **node)
{
    TextureTreeNode *walk=*node;
    TextureTreeNode *next;
    if (!*node)
        return;
    next = walk;
    while (next) {
        walk = next;
        next = walk->next;
        freeNode(&walk->child);
        MP_FREE(TextureTreeNode, walk);
    }
    *node = NULL;
}

typedef struct TexCounts {
    int numTex;
    int numProcessed;
    int numErrors;
} TexCounts;

static TexCounts getCounts(TextureTreeNode *node)
{
    TexCounts ret={0};
    while (node) {
        if (node->child) {
            // Folder
            TexCounts sub = getCounts(node->child);
            ret.numTex += sub.numTex;
            ret.numProcessed += sub.numProcessed;
            ret.numErrors += sub.numErrors;
        } else {
            // Texture
            ret.numProcessed += node->processed;
            ret.numErrors += !!node->error;
            ret.numTex++;
        }
        node = node->next;
    }
    return ret;
}

static void addNodeToMenu(DebugMenuItem *parent, TextureTreeNode *treeroot, char *dir)
{
    DebugMenuItem *sub;
    TextureTreeNode *node=treeroot;
    char buf[1024], buf2[1024];
    while (node) {
        sprintf(buf, "%s", node->name);
        if (node->child) {
            TexCounts counts = getCounts(node->child);
            strcatf(buf, " %s(%d/%d)", counts.numProcessed==counts.numTex?"^2":"^1", counts.numProcessed, counts.numTex);
            if (counts.numErrors) {
                strcatf(buf, " ^3%d ERRORS", counts.numErrors);
            }
            sub = addDebugMenuItem(parent, buf, "", 0);
            sprintf(buf2, "%s/%s", dir, node->name);
            addNodeToMenu(sub, node->child, buf2);
        } else {
            if (!node->bind) {
                strcat(buf, " ^5(dynamic)");
            } else if (node->processed) {
                strcat(buf, " ^5(has layout)");
            }
            if (node->error & TWEERROR_MISSING)
            {
                strcat(buf, " ^1NAME MIS-MATCH");
            }
            if (node->error & TWEERROR_SIZEMISMATCH)
            {
                strcat(buf, " ^1SIZE MIS-MATCH");
            }
            if (node->error & TWEERROR_ALPHAMISMATCH)
            {
                strcat(buf, " ^1ALPHA MIS-MATCH");
            }
            if (strStartsWith(dir, "texture_library/"))
                dir += strlen("texture_library/");
            sprintf(buf2, "fileloadname \"%s/%s\"", dir, node->name);
            addDebugMenuItem(parent, buf, buf2, 0);
        }
        node = node->next;
    }
}

static void addAllNodes(void)
{
    int i;
    char buf[1024];

    MP_CREATE(TextureTreeNode, 64);
    treeroot = NULL;
    for (i=0; i<eaSize(&g_basicTextures); i++) {
        BasicTexture *bind = g_basicTextures[i];
        if (stricmp(bind->dirname, "dynamicTexture")==0)
            continue;
        if (strstriConst(bind->name, "#Base") || // Base texture in need of layout
            (bind->texWord && bind->actualTexture == bind)) // Texture without a #Base but already has a layout
        {
            sprintf(buf, "%s/%s", bind->dirname, bind->name);
            forwardSlashes(buf);
            addNode(&treeroot, buf, bind);
        }
    }
    // Add dynamicTexWords
    for (i=0; i<eaSize(&texWords_list.texWords); i++) {
        TexWord *texWord = texWords_list.texWords[i];
        strcpy(buf, getFileName(texWord->name));
        if (strchr(buf, '.'))
            *strchr(buf, '.')=0;
        if (!texFind(buf)) {
            // Dynamic!
            strcpy(buf, texWord->name);
            forwardSlashes(buf);
            if (strStartsWith(buf, "TEXTS/BASE/TEXTURE_LIBRARY/"))
                strcpy(buf, buf + strlen("TEXTS/BASE/TEXTURE_LIBRARY/"));
            addNode(&treeroot, buf, NULL);
        }
    }
}

static const char *getNextFileNameRecurse(TextureTreeNode *node, int next, TextureTreeNode **prev, bool *returnFirst)
{
    const char *ret;
    char buf[1024];
    while (node) {
        if (node->child) {
            ret = getNextFileNameRecurse(node->child, next, prev, returnFirst);
            if (ret)
                return ret;
        } else {
            if (*returnFirst)
                return node->name;
            strcpy(buf, node->name);
            if (strchr(buf, '#'))
                *strchr(buf, '#')=0;
            if (strchr(buf, '.'))
                *strchr(buf, '.')=0;
            if (node->bind == tweditor_state.bind ||
                tweditor_state.bind->texWordParams && stricmp(tweditor_state.bind->texWordParams->layoutName, buf)==0)
            {
                // Found our node
                if (next==0) {
                    if (*prev)
                        return (*prev)->name;
                    else
                        return node->name;
                } else
                    *returnFirst = true;
            }
            *prev = node;
        }
        node = node->next;
    }
    return NULL;
}

static const char *getNextFileName(int next) {
    const char *ret;
    bool returnFirst=false;
    TextureTreeNode *prev=NULL;
    addAllNodes();

    ret = getNextFileNameRecurse(treeroot, next, &prev, &returnFirst);
    if (!ret) {
        ret = prev->name;
    }

    freeNode(&treeroot);
    return ret;
}

void texWordsEdit_fileLoadNext(int next) {
    const char *filename = getNextFileName(next);
    if (filename)
        texWordsEdit_fileLoadName(filename);
}

static void writeNode(FILE *fout, TextureTreeNode *treeroot, char *dir)
{
    TextureTreeNode *node=treeroot;
    char buf[1024];
    while (node) {
        sprintf(buf, "%s/%s", dir, node->name);
        while (buf[0]=='/') {
            strcpy(buf, buf+1);
        }
        if (node->child) {
            writeNode(fout, node->child, buf);
        } else {
            const char *lastAuthor = "";
            if (node->processed && node->bind && node->bind->texWord) {
                lastAuthor = "Unknown";
                if (strstriConst(lastAuthor, "file checked out")) {
                    lastAuthor = "CheckedOut";
                } else if (strstriConst(lastAuthor, "Not in database")) {
                    lastAuthor = "NewFile";
                }
            }

            fprintf(fout, "%c %-11s %s\n", node->processed?'X':' ', lastAuthor, buf);
        }
        node = node->next;
    }
}

void texWordsEdit_fileSaveList(void) {
    FILE *fout = fopen("C:\\texture_localization.txt", "w");
    if (!fout)
        return;
    addAllNodes();
    fprintf(fout, "P %-11s Filename\n", "Last Author");
    // Add tree to list
    writeNode(fout, treeroot, "");

    freeNode(&treeroot);
    fclose(fout);
}

typedef struct TextEntry {
    char *key;
    char *text;
    char *filename;
} TextEntry;

int textEntryCompare(const void *a, const void *b)
{
    TextEntry *tea = *(TextEntry**)a;
    TextEntry *teb = *(TextEntry**)b;
    return stricmp(tea->key, teb->key);
}

void texWordsEdit_filePrune(void)
{
    int i, j;
    StashTable htKeys = stashTableCreateWithStringKeys(1024, StashDeepCopyKeys);
    TextEntry **eaText=NULL;
    // Assume all texWords are loaded at this point
    // Make list of all text, generating new keys as we go along, saving each texWord as we go
    for (i=0; i<eaSize(&texWords_list.texWords); i++) {
        TexWord *texWord = texWords_list.texWords[i];
        bool texWordChanged=false;
        for (j=0; j<eaSize(&texWord->layers); j++) {
            TexWordLayer *layer = texWord->layers[j];
            if (layer->type == TWLT_TEXT) {
                // generate new key
                char keyString[2048];
                char text[2048];
                unsigned char *s;
                // Generate a new key!
                strcpy(keyString, "TexWord_");
                msPrintf(texWordsMessages, SAFESTR(text), layer->text);
                if (strStartsWith(text, "TexWord_")) {
                    printf("Untranslated string: %s\n", text);
                    //assert(0);
                    strcpy(keyString, text);
                } else {
                    strcat(keyString, text);
                }
                for (s=keyString+strlen("TexWord_"); *s; ) {
                    if (!isalnum((unsigned char)*s) && *s!='_') {
                        strcpy(s, s+1);
                        s[0] = toupper(s[0]);
                    } else {
                        s++;
                    }
                }
                // Verify that this is a unique key, if not, increment
                while (stashFindPointerReturnPointer(htKeys, keyString)) {
                    incrementName(keyString, 1023);
                }
                if (stricmp(keyString, layer->text)!=0) {
                    // change texword, flag
                    texWordChanged = true;
                    ParserFreeString(layer->text);
                    layer->text = ParserAllocString(keyString);                    
                }
                // add key and string to hashtable and list
                stashAddInt(htKeys, keyString, 1, false);
                {
                    TextEntry *te = calloc(sizeof(TextEntry), 1);
                    te->key = strdup(keyString);
                    te->filename = texWord->name;
                    te->text = strdup(text);
                    eaPush(&eaText, te);
                }
            }
        }
        // if flagged, save texword
        if (texWordChanged) {
            char fullname[MAX_PATH];
            TexWordList dummy={0};
            int ok;
            eaPush(&dummy.texWords, texWord);
            fileLocateWrite(texWord->name, fullname);
            fileRenameToBak(fullname);
            mkdirtree(fullname);
            ok = ParserWriteTextFile(texWord->name, ParseTexWordFiles, &dummy, 0, 0);
            printf("File %s saved %s\n", fullname, ok?"successfully":"FAILED");
        }
    }
    // Sort list by key
    eaQSort(eaText, textEntryCompare);
    // Write new list
    {
        FILE *fout = fileOpen("C:\\game\\data\\texts\\English\\textureWords2.ms", "w");
        for (i=0; i<eaSize(&eaText); i++) {
            TextEntry *te = eaText[i];
            if (strStartsWith(te->filename, "texts/base/texture_library/")) {
                fprintf(fout, "# Used in %s\n", te->filename + strlen("texts/base/texture_library/"));
            } else {
                fprintf(fout, "# Used in %s\n", te->filename);
            }
            if (strchr(te->text, '\"')) {
                fprintf(fout, "\"%s\", <<%s>>\r\n", te->key, te->text);
            } else {
                fprintf(fout, "\"%s\", \"%s\"\r\n", te->key, te->text);
            }
        }
        fclose(fout);
    }
}

static void texWordsEditorBuildFileMenu(DebugMenuItem *root)
{
    DebugMenuItem *file = addDebugMenuItem(root, "File", "File", 0);
    DebugMenuItem *sub;
    char buf[1024], buf2[1024];

    addDebugMenuItem(file, "Load new Texture/.TexWord", "fileload", 0);
    if (tweditor_state.texWord && !tweditor_state.readonly) {
        addDebugMenuItem(file, "Save .TexWord ^3[F7]", "filesave", 0);
        addDebugMenuItem(file, "Save As New Dynamic .TexWord", "filesavedynamic", 0);
        addDebugMenuItem(file, "Remove .TexWord from this texture", "filenew", 0);
        sprintf(buf, "Checkin %s", textureToBaseTexWord(game_state.texWordEdit));
        sprintf(buf2, "filecheckin %s", textureToBaseTexWord(game_state.texWordEdit));
        addDebugMenuItem(file, buf, buf2, 0);
    } else if (tweditor_state.readonly) {
        sprintf(buf, "Checkout %s", textureToBaseTexWord(game_state.texWordEdit));
        sprintf(buf2, "filecheckout %s", textureToBaseTexWord(game_state.texWordEdit));
        addDebugMenuItem(file, buf, buf2, 0);
    }
    addDebugMenuItem(file, "Undo ^3[Ctrl-Z]^0 or ^3[F12]", "undo", 0);
    addDebugMenuItem(file, "Redo ^3[Ctrl-Y]", "redo", 0);

    sub = addDebugMenuItem(file, "All #Base textures", "A list of all textures with #Base in the name", 0);

    // Add tree to list
    addAllNodes();
    addNodeToMenu(sub, treeroot, "texture_library");
    freeNode(&treeroot);

    addDebugMenuItem(sub, "Save list to c:\\texture_localization.txt", "filesavelist", 0);

    // Jump to Next/Prev
    {
        const char *filename;
        filename = getNextFileName(0);
        if (filename) {
            sprintf(buf, "Open previous file (%s) ^3[<]", filename);
            sprintf(buf2, "fileloadname \"texture_library/%s\"", filename);
            addDebugMenuItem(file, buf, buf2, 0);
        }
        filename = getNextFileName(1);
        if (filename) {
            sprintf(buf, "Open next file (%s) ^3[>]", filename);
            sprintf(buf2, "fileloadname \"texture_library/%s\"", filename);
            addDebugMenuItem(file, buf, buf2, 0);
        }
    }
}

void texWordsEditorBuildMenu(void)
{
    DebugMenuItem *root;
    char buf[1024];
    char buf2[1024];
    int i;
    TexWord *texWord = tweditor_state.texWord;
    initEntDebug();

    if (debug_state.menuItem) {
        freeDebugMenuItem(debug_state.menuItem);
        debug_state.menuItem = NULL;
        unbindKeyProfile(&ent_debug_binds_profile);
    }

    debug_state.menuItem = root = addDebugMenuItem(NULL, "TexWords Editor Menu", NULL, 0);
    texWordsEditorBuildFileMenu(root);

    {
        DebugMenuItem *sub = addDebugMenuItem(root, "Colors", NULL, 0);
        addDebugMenuItem(sub, "To sample, hold ^3[SHIFT]", " ", 0);
        addDebugMenuItem(sub, "ASK", "color -1 -1 -1 -1 ", 0);
        addDebugMenuItem(sub, "ColorPicker ^3[C]", "colorpick 1", 0);
        addDebugMenuItem(sub, "White", "color 255 255 255 255", 0);
        addDebugMenuItem(sub, "Black", "color 0 0 0 255", 0);
        addDebugMenuItem(sub, "Invisible", "color 255 255 255 0", 0);
        mruUpdate(mruColors);
        if (mruColors->count) {
            DebugMenuItem *sub2 = addDebugMenuItem(sub, "Most Recently Used", "List of most recently used colors", 0);
            for (i=mruColors->count-1; i>=0; i--) {
                char *alpha;
                // display
                sprintf(buf, "^#(%s", mruColors->values[i]);
                if (alpha = strrchr(buf, ',')) {
                    strcpy(buf2, alpha+1);
                    sprintf(alpha, ")%s", mruColors->values[i]);
                    alpha = strrchr(buf, ',');
                    alpha[1]=0;
                    strcatf(buf, "^#(%s,%s,%s)%s", buf2, buf2, buf2, buf2);
                } else {
                    strcat(buf, ")");
                    strcat(buf, mruColors->values[i]);
                }
                // command
                sprintf(buf2, "color %s", mruColors->values[i]);
                while (strchr(buf2, ','))
                    *strchr(buf2, ',')=' ';
                addDebugMenuItem(sub2, buf, buf2, 0);
            }
        }
    }

    if (tweditor_state.readonly) {
        DebugMenuItem *sub;
        sprintf(buf, "Editing texture layout for: %s", game_state.texWordEdit);
        addDebugMenuItem(root, buf, " ", 0);
        sub = addDebugMenuItem(root, "TexWord file is read-only", NULL, 1);
            sprintf(buf, "Checkout %s", textureToBaseTexWord(game_state.texWordEdit));
            sprintf(buf2, "filecheckout %s", textureToBaseTexWord(game_state.texWordEdit));
            addDebugMenuItem(sub, buf, buf2, 0);
        addDebugMenuItem(root, "Copy all Layers to clipboard", "layercopyall", 0);
    } else if (texWord) {
        DebugMenuItem *sub;
        sprintf(buf, "Editing texture layout for: ^1%s", tweditor_state.bind->name);
        addDebugMenuItem(root, buf, " ", 0);
        sub = addDebugMenuItem(root, "Layers", NULL, 1);
        { // Layers menu
            for (i=0; i<eaSize(&texWord->layers); i++) {
                addLayerToMenu(sub, texWord->layers[i], NULL, 0, i, 0, texWord->layers, NULL);
            }
        }
        {
            DebugMenuItem *insertLayer = addDebugMenuItem(sub, "Insert New Layer", "", 0);
            addDebugMenuItem(insertLayer, "On top", "layerinsert 0", 0);
            for (i=0; i<eaSize(&texWord->layers); i++) {
                if (i == eaSize(&texWord->layers)-1) {
                    sprintf(buf, "On bottom");
                } else {
                    sprintf(buf, "Under Layer ^7%s", texWord->layers[i]->layerName);
                }
                sprintf(buf2, "layerinsert %d", i+1);
                addDebugMenuItem(insertLayer, buf, buf2, 0);
            }
        }
        addDebugMenuItem(sub, "Unhide all layers ^3[J]", "layerunhideall", 0);

        addDebugMenuItem(sub, "Copy all Layers to clipboard", "layercopyall", 0);
        if (layerClipboard.layers) {
            addDebugMenuItem(sub, "Paste all Layers from clipboard", "layerpasteall", 0);
        }

        // Image size
        {
            DebugMenuItem *sizeMenu, *sizeSubMenu;
            if (texWord->width==0 && texWord->height==0) {
                sizeMenu = addDebugMenuItem(sub, "DTS$$Dynamic Texture Size: ^5Auto", "", 0);
            } else {
                sprintf(buf, "DTS$$Dynamic Texture Size: ^5%dx%d", texWord->width, texWord->height);
                sizeMenu = addDebugMenuItem(sub, buf, "", 0);
            }

            sprintf(buf, "%d", texWord->width);
            sprintf(buf2, "IMAGEWIDTH$$Width: %s", texWord->width?buf:"Auto");
            sizeSubMenu = addDebugMenuItem(sizeMenu, buf2, "", 0);
            if (texWord->width == 0)
                addDebugMenuItem(sizeSubMenu, "Auto ^2[Selected]", "size 0 -1", 0);
            else
                addDebugMenuItem(sizeSubMenu, "Auto", "size 0 -1", 0);
            for (i=4; i<10; i++) {
                sprintf(buf, "%d", 1<<i);
                if (texWord->width == 1<<i)
                    strcat(buf, " ^2[Selected]");
                sprintf(buf2, "size %d -1", 1<<i);
                addDebugMenuItem(sizeSubMenu, buf, buf2, 0);
            }

            sprintf(buf, "%d", texWord->height);
            sprintf(buf2, "IMAGEHEIGHT$$Height: %s", texWord->height?buf:"Auto");
            sizeSubMenu = addDebugMenuItem(sizeMenu, buf2, "", 0);
            if (texWord->height == 0)
                addDebugMenuItem(sizeSubMenu, "Auto ^2[Selected]", "size -1 0", 0);
            else
                addDebugMenuItem(sizeSubMenu, "Auto", "size -1 0", 0);
            for (i=4; i<10; i++) {
                sprintf(buf, "%d", 1<<i);
                if (texWord->height == 1<<i)
                    strcat(buf, " ^2[Selected]");
                sprintf(buf2, "size -1 %d", 1<<i);
                addDebugMenuItem(sizeSubMenu, buf, buf2, 0);
            }

        }

    } else {
        DebugMenuItem *sub;
        sprintf(buf, "Editing texture layout for: %s", game_state.texWordEdit);
        addDebugMenuItem(root, buf, " ", 0);
        sub = addDebugMenuItem(root, "No TexWord defined for this texture", NULL, 1);
        sprintf(buf, "Create %s", textureToBaseTexWord(game_state.texWordEdit));
        addDebugMenuItem(sub, buf, buf, 0);
        if (layerClipboard.layers) {
            addDebugMenuItem(sub, "Paste all Layers from clipboard", "layerpasteall", 0);
        }
    }

    // General options
    {
        DebugMenuItem *options;
        options = addDebugMenuItem(root, "General Options", "", 0);
        {
            DebugMenuItem *colorChoice = addDebugMenuItem(options, "Background", "", 0);
            addDebugMenuItem(colorChoice, "^1Red", "bgcolor 1.0 0.2 0.2 1.0", 0);
            addDebugMenuItem(colorChoice, "^2Green", "bgcolor 0.2 1.0 0.2 1.0", 0);
            addDebugMenuItem(colorChoice, "^5Blue", "bgcolor 0.2 0.2 1.0 1.0", 0);
            addDebugMenuItem(colorChoice, "^0White", "bgcolor 1.0 1.0 1.0 1.0", 0);
            //addDebugMenuItem(colorChoice, "^0Invisible", "bgcolor 1.0 1.0 1.0 0.0", 0);
        }
        {
            DebugMenuItem *stretch = addDebugMenuItem(options, "Preview Stretched ^3[SPACE]", "", 0);
            addDebugMenuItem(stretch, "Stretched", "stretch 1", 0);
            addDebugMenuItem(stretch, "NOT Stretched", "stretch 0", 0);
            addDebugMenuItem(stretch, "Toggle ^3[SPACE]", "++stretch", 0);
        }
        {
            DebugMenuItem *stretch = addDebugMenuItem(options, "Preview Original ^3[TAB]", "", 0);
            addDebugMenuItem(stretch, "Show Original", "orig 1", 0);
            addDebugMenuItem(stretch, "Show Composited/Working Image", "orig 0", 0);
            addDebugMenuItem(stretch, "Toggle ^3[TAB]", "++orig", 0);
        }
    }
    addDebugMenuItem(root, "Quit", "quit 1", 0);

    entDebugInitMenu();
}

static void fixUpTexWordName()
{
    if (tweditor_state.fromEditor) {
        char name[MAX_PATH];
        TexWord *tw;
        strcpy(name, getFileName(game_state.texWordEdit));
        if (strchr(name, '.'))
            *strrchr(name, '.')=0;
        tw = texWordFind(game_state.texWordEdit, 1);
        if (tw) {
            estrPrintCharString(&game_state.texWordEdit, tw->name);
        }
    }
    if (stricmp(game_state.texWordEdit, "LAST")==0) {
        estrPrintCharString(&game_state.texWordEdit, regGetString("LastFile", "texture_library/GUI/CREATION/Login/coh.texture"));
    }
    forwardSlashesEString(&game_state.texWordEdit);
    if (strStartsWith(game_state.texWordEdit, fileDataDir())) {
        estrRemove(&game_state.texWordEdit, 0, strlen(fileDataDir()));
    }
    if (strStartsWith(game_state.texWordEdit, "c:/game/src/")) {
        estrRemove(&game_state.texWordEdit, 0, strlen("c:/game/src/"));
    }
    while (game_state.texWordEdit[0]=='/')
        estrRemove(&game_state.texWordEdit, 0, 1);
    if (strEndsWith(game_state.texWordEdit, ".tga")) {
        changeFileExtEString(game_state.texWordEdit, ".texture", &game_state.texWordEdit);
    }
    if (strEndsWith(game_state.texWordEdit, ".texword.texword"))
        estrRemove(&game_state.texWordEdit, estrLength(&game_state.texWordEdit) - strlen(".texword"), strlen(".texword"));
    if (strEndsWith(game_state.texWordEdit, ".texword")) {
        estrPrintCharString(&game_state.texWordEdit, baseTexWordToTexture(game_state.texWordEdit));
    } else {
        // .texture file, all good!
    }
    regPutString("LastFile", game_state.texWordEdit);
}

static void texWordsEditorSetup(void)
{
    char buf[1024];
    char *s;

    fixUpTexWordName();

    strcpy(buf, getFileName(game_state.texWordEdit));
    s = strrchr(buf, '.');
    if (s)
        *s=0;

    statusf("Loaded %s", game_state.texWordEdit);

    texWordsEditorKeybindInit();

    loadMessage("Generating image...");
    if (tweditor_state.bind)
        texRemoveRefBasic(tweditor_state.bind);
    if (texFind(buf)==NULL) {
        if (texWordFind(buf, 1)) {
            // Dynamic
            TexWordParams *params = createTexWordParams();
            params->layoutName = allocAddString(buf);
            tweditor_state.bind = texLoadDynamic(params, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UI, NULL)->tex_layers[TEXLAYER_BASE];
        } else {
            // New Dynamic?
            estrPrintf(&game_state.texWordEdit, "texture_library/Dynamic/%s.texture", buf);
            tweditor_state.bind = texLoadBasic(buf, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UI);
        }
    } else {
        tweditor_state.bind = texLoadBasic(buf, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UI);
    }
    tweditor_state.bind = tweditor_state.bind->actualTexture;
    tweditor_state.texWord = tweditor_state.bind->texWord;
    tweditor_state.dirty = false;
    // Check checkout status
    {
        struct stat statbuf;
        int sret;
        fileLocateWrite(textureToBaseTexWord(game_state.texWordEdit), buf);
        backSlashes(buf);
        sret = stat(buf, &statbuf);
        if (statbuf.st_mode & _S_IWRITE || sret==-1) {
            tweditor_state.readonly=0;
        } else {
            tweditor_state.readonly=1;
        }
    }

    texWordsEditorBuildMenu();
}

int texWordsEditorCmdParse(char *str, int x, int y)
{
    Cmd            *cmd;
    CmdContext    output = {0};
    bool        didEditCommand=false;
    bool        needRefresh=false;

    output.found_cmd = false;
    output.access_level = cmdAccessLevel();
    cmd = cmdOldRead(&texwordseditor_cmdlist,str,&output);

    if (output.msg[0])
    {
        if (strncmp(output.msg,"Unknown",7)==0)
            return 0;

        conPrintf("%s",output.msg);
        return 1;
    }

    if (!cmd)
        return 0;

#define EDIT_CMD(cmdnum, func)                \
    xcase cmdnum:                            \
        texWordsEdit_layerExtentsFinish();    \
        func;                                \
        didEditCommand = true;                \
        needRefresh = true;                    \
        tweditor_state.reOpenMenu = 1;

#define NONEDIT_CMD(cmdnum, func)                \
    xcase cmdnum:                            \
    texWordsEdit_layerExtentsFinish();    \
    func;                                \
    needRefresh = true;                    \
    tweditor_state.reOpenMenu = 1;


    switch(cmd->num)
    {
    case CMDTW_QUIT:
        if (tweditor_state.extents.edit) {
            texWordsEdit_layerExtentsUnApply();
            texWordsEdit_layerExtentsFinish();
        } else {
            if (winMsgYesNo(tweditor_state.dirty?"You have unsaved changse, are you sure you want to exit?":"Are you sure you want to exit?")) {
                if (tweditor_state.fromEditor) {
                    tweditor_state.requestQuit = 1;
                } else {
                    windowExit(0);
                }
            }
        }
    xcase CMDTW_MENU:
        texWordsEdit_layerExtentsApply();
        texWordsEditorBuildMenu();
    EDIT_CMD(CMDTW_LAYERINSERT, texWordsEdit_layerInsert(layer_index));
    EDIT_CMD(CMDTW_LAYERCLONE, texWordsEdit_layerClone(layer_index));
    xcase CMDTW_LAYERCOPYALL:
        texWordsEdit_layerCopyAll();
        tweditor_state.reOpenMenu = 1;
    EDIT_CMD(CMDTW_LAYERPASTEALL, texWordsEdit_layerPasteAll());
    EDIT_CMD(CMDTW_LAYERSWAP, texWordsEdit_layerSwap(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERDELETE, texWordsEdit_layerDelete(layer_index));
    EDIT_CMD(CMDTW_LAYERTYPE, texWordsEdit_layerType(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERNAME, texWordsEdit_layerName(layer_index));
    EDIT_CMD(CMDTW_LAYERHIDE, texWordsEdit_layerHide(layer_index, layer_val, 1));
    EDIT_CMD(CMDTW_LAYERUNHIDEALL, texWordsEdit_layerHideAll(0));
    EDIT_CMD(CMDTW_LAYERSOLO, texWordsEdit_layerSolo(layer_index));
    EDIT_CMD(CMDTW_LAYERSTRETCH, texWordsEdit_layerStretch(layer_index, layer_val));
    xcase CMDTW_LAYERIMAGE:
        texWordsEdit_layerImage(layer_index, tmp_str);
        didEditCommand = true;
        needRefresh = true;
        tweditor_state.reOpenMenu = 1;
    xcase CMDTW_LAYERTEXTCHANGE:
        texWordsEdit_layerExtentsFinish();
        texWordsEdit_layerTextChange(layer_index);
        needRefresh = true;
        didEditCommand = true;
        tweditor_state.reOpenMenu = 1;
    EDIT_CMD(CMDTW_LAYERASK, texWordsEdit_layerAsk(layer_index, tmp_str));
    xcase CMDTW_LAYEREXTENTS:
        tweditor_state.reOpenMenu = 0;
        texWordsEdit_layerExtents(layer_index);
        needRefresh = true;
    EDIT_CMD(CMDTW_LAYERCOPYSIZE, texWordsEdit_layerCopySize(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERCOPYPOS, texWordsEdit_layerCopyPos(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERCOPYROT, texWordsEdit_layerCopyRot(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERCOLORMODE, texWordsEdit_layerColorMode(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERCOLORCHANGE, texWordsEdit_layerColorChange(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERALPHACHANGE, texWordsEdit_layerAlphaChange(layer_index, layer_val, tmp_str));
    xcase CMDTW_LAYERFONT:
        texWordsEdit_layerFont(layer_index, tmp_str);
        didEditCommand = true;
        needRefresh = true;
        tweditor_state.reOpenMenu = 1;
    xcase CMDTW_LAYERFONTSTYLE:
        texWordsEdit_layerFontStyle(layer_index, tmp_str, layer_val);
        didEditCommand = true;
        needRefresh = true;
        tweditor_state.reOpenMenu = 1;
    EDIT_CMD(CMDTW_LAYERSUBINSERT, texWordsEdit_layerSubInsert(layer_index));
    EDIT_CMD(CMDTW_LAYERSUBBLEND, texWordsEdit_layerSubBlendMode(layer_index, layer_val));
    EDIT_CMD(CMDTW_LAYERSUBBLENDWEIGHT, texWordsEdit_layerSubBlendWeight(layer_index, tmp_str));
    EDIT_CMD(CMDTW_LAYERFILTERINSERT, texWordsEdit_layerFilterInsert(layer_index, layer_filter_index));
    EDIT_CMD(CMDTW_LAYERFILTERDELETE, texWordsEdit_layerFilterDelete(layer_index, layer_filter_index));
    EDIT_CMD(CMDTW_LAYERFILTERTYPE, texWordsEdit_layerFilterType(layer_index, layer_filter_index, layer_val));
    EDIT_CMD(CMDTW_LAYERFILTERBLEND, texWordsEdit_layerFilterBlend(layer_index, layer_filter_index, layer_val));
    EDIT_CMD(CMDTW_LAYERFILTERSTYLE, texWordsEdit_layerFilterStyle(layer_index, layer_filter_index, tmp_str, tmp_str2));
    EDIT_CMD(CMDTW_LAYERFILTERCOLOR, texWordsEdit_layerFilterColor(layer_index, layer_filter_index));
    EDIT_CMD(CMDTW_LAYERFILTERALPHA, texWordsEdit_layerFilterAlpha(layer_index, layer_filter_index, tmp_str));
    EDIT_CMD(CMDTW_SIZE, texWordsEdit_size(tmp_int, tmp_int2));
    NONEDIT_CMD(CMDTW_FILELOAD, texWordsEdit_fileLoad());
    NONEDIT_CMD(CMDTW_FILELOADNAME, texWordsEdit_fileLoadName(tmp_str));
    xcase CMDTW_FILELOADNEXT:
        texWordsEdit_layerExtentsFinish();
        texWordsEdit_fileLoadNext(layer_val);
        tweditor_state.reOpenMenu = tmp_int;
        if (!tweditor_state.reOpenMenu) {
            if (debug_state.menuItem) {
                freeDebugMenuItem(debug_state.menuItem);
                debug_state.menuItem = NULL;
                unbindKeyProfile(&ent_debug_binds_profile);
            }
        }
        needRefresh = true;
    xcase CMDTW_FILESAVELIST:
        texWordsEdit_fileSaveList();
    xcase CMDTW_FILEPRUNE:
        texWordsEdit_filePrune();
    xcase CMDTW_FILESAVE:
        texWordsEdit_layerExtentsFinish();
        texWordsEdit_fileSave();
        tweditor_state.reOpenMenu = 1;
    NONEDIT_CMD(CMDTW_FILESAVEDYNAMIC, texWordsEdit_fileSaveDynamic());
    xcase CMDTW_FILENEW:
        texWordsEdit_layerExtentsFinish();
        texWordsEdit_fileNew();
        tweditor_state.reOpenMenu = 1;
    NONEDIT_CMD(CMDTW_FILECHECKOUT, texWordsEdit_fileCheckout(tmp_str));
    NONEDIT_CMD(CMDTW_FILECHECKIN, texWordsEdit_fileCheckin(tmp_str));
    xcase CMDTW_CREATE:
        texWordsEdit_create(tmp_str);
        tweditor_state.reOpenMenu = 1;
        needRefresh = true;
        didEditCommand = true;
    xcase CMDTW_REFRESH:
        needRefresh = true;
    case CMDTW_APPLY: // Deliberate trickle-through
        if (tweditor_state.extents.edit) {
            texWordsEdit_layerExtentsApply();
            needRefresh = true;
            didEditCommand = true;
        }
    xcase CMDTW_LBUTTON:
        if (texWordsEdit_leftMouse(tweditor_state.lbutton)) {
            didEditCommand = true;
            needRefresh = true;
        }
    xcase CMDTW_SHIFT:
        // Nothing
    xcase CMDTW_UNDO:
        texWordsEdit_undoUndo();
        didEditCommand = true;
        needRefresh = true;
        //tweditor_state.reOpenMenu = 1;
    xcase CMDTW_REDO:
        texWordsEdit_undoRedo();
        didEditCommand = true;
        needRefresh = true;
        //tweditor_state.reOpenMenu = 1;
    xcase CMDTW_COLOR:
        if (tmp_color[0]==-1) {
            askColor(&tweditor_state.selcolor);
        } else {
            tweditor_state.selcolor.r = tmp_color[0];
            tweditor_state.selcolor.g = tmp_color[1];
            tweditor_state.selcolor.b = tmp_color[2];
            tweditor_state.selcolor.a = tmp_color[3];
        }
        tweditor_state.reOpenMenu = 1;
    xcase CMDTW_COLORPICK:
        if (entDebugMenuVisible()) {
            didEditCommand = true;
            needRefresh = true;
            tweditor_state.reOpenMenu = 1;
        }
    xdefault:
        break;
    }
    if (needRefresh) {
        texFree(tweditor_state.bind, 0);
        tweditor_state.needToRebuild = 1;
        texWordVerify(tweditor_state.texWord, true);
    }
    if (didEditCommand) {
        tweditor_state.dirty = true;
    }

    return 1;
}

static void rebuildTextures()
{
    static TexBind dummyCompositeBind;
    BasicTexture *bind;
    bind = tweditor_state.bind;
    if (tweditor_state.orig) {
        char origName[256];
        static BasicTexture dummyBind;
        dummyBind.name = "dummy";

        strcpy(origName, getFileNameConst(bind->name));
        if (strchr(origName, '#')) {
            *strchr(origName, '#')=0;
        }
        dummyBind.actualTexture = texFind(origName);
        if (!dummyBind.actualTexture && tweditor_state.texWord)
            dummyBind.actualTexture = texWordGetBaseImage(tweditor_state.texWord);
        if (dummyBind.actualTexture) {
            TexWord *old = dummyBind.actualTexture->texWord;
            texFree(dummyBind.actualTexture, 0); // Free old one

            dummyBind.actualTexture->texWord = NULL;
            texLoadInternalBasic(dummyBind.actualTexture, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL, 0);
            dummyBind.actualTexture->texWord = old;

            dummyBind.width = dummyBind.actualTexture->width;
            dummyBind.height = dummyBind.actualTexture->height;
            dummyBind.realWidth = dummyBind.actualTexture->realWidth;
            dummyBind.realHeight = dummyBind.actualTexture->realHeight;
            bind = &dummyBind;
        }
    } else {
        int i;
        texForceTexLoaderToComplete(1);
        texFree(bind, 0); // Free old one
        // Free raw data
        for (i=0; i<eaSize(&g_basicTextures); i++) {
            texFree(g_basicTextures[i], 1);
        }
    }

    // Force loading of file in case it needs to get regenerated
    texLoadInternalBasic(bind, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL, 0);
    dummyCompositeBind.name = "dummy2";
    dummyCompositeBind.width = bind->width;
    dummyCompositeBind.height = bind->height;
    dummyCompositeBind.tex_layers[TEXLAYER_BASE] = bind;
    dummyCompositeBind.tex_layers[TEXLAYER_GENERIC] = white_tex;
    drawMeBind = &dummyCompositeBind;
}

void texWordsEdit_drawExtents(void)
{
    U32    color = 0xffff7fff;
    F32 x0 = tweditor_state.extents.x0,
        x1 = tweditor_state.extents.x1,
        y0 = tweditor_state.extents.y0,
        y1 = tweditor_state.extents.y1;
    textureSpaceToScreenSpace(&x0, &y0);
    textureSpaceToScreenSpace(&x1, &y1);

#if 1
    drawLine(x0, y0, x1, y0, color);
    drawLine(x1, y0, x1, y1, color);
    drawLine(x1, y1, x0, y1, color);
    drawLine(x0, y1, x0, y0, color);
#else
    glLineWidth( 1.0 ); CHECKGL;
    glBegin( GL_LINE_LOOP );
    WCW_Color4(255, 127, 255, 255);
    glVertex2f( x0, y0 );

    glVertex2f( x1, y0);

    glVertex2f( x1, y1 );

    glVertex2f( x0, y1 );
    glEnd(); CHECKGL; 
#endif

}

void doColorPicker()
{
    static int auto_close_check=0;
    static int rgb=0xffffffff;
    bool saveColor=false;
    if (tweditor_state.colorpick) {
        int            done;

        auto_close_check = 1;
        sel_count = 1;
        updatePropertiesMenu = true;
        edit_state.sel = isDown(MS_LEFT);
        done = pickColor(&rgb,1);
        if (done)
        {
            if (done!=-2) {
                saveColor = true;
            }
            tweditor_state.colorpick = 0;
            auto_close_check = 0;
        }
    } else if (auto_close_check) {
        // toggled C to close it
        auto_close_check = 0;
        saveColor = true;
    }

    if (saveColor) {
        char text[64];
        U8 temp;
        tweditor_state.selcolor.integer = rgb;
        temp = tweditor_state.selcolor.r;
        tweditor_state.selcolor.r = tweditor_state.selcolor.b;
        tweditor_state.selcolor.b = temp;
        sprintf(text, "%d,%d,%d,%d", tweditor_state.selcolor.r, tweditor_state.selcolor.g, tweditor_state.selcolor.b, tweditor_state.selcolor.a);
        mruAddToList(mruColors, text);
        texWordsEditorBuildMenu();
    }
}


FileScanAction fontProcessor(char* dir, struct _finddata32_t* data)
{
    if (!(data->attrib & _A_SUBDIR)) {
        ttFontLoadCached(data->name, 0);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

void texWordsEditorAddFonts()
{
    fileScanAllDataDirs("fonts", fontProcessor);
}

void texWordsEditor(int fromEditor)
{
    extern void engine_update();
    char status[1024];
    int rgba[] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
    tweditor_state.fromEditor = fromEditor;
    if (fromEditor) {
        strcpy(tweditor_state.tweLocale, locGetName(getCurrentLocale()));
    } else {
        strcpy(tweditor_state.tweLocale, "Base");
    }
    uigameDemoForceMenu();
    game_state.game_mode = SHOW_GAME;
    FolderCacheEnableCallbacks(1);
    showBgReset();
    cmdAccessOverride(10);

    mruTextures = createMRUList("TexWords", "TextureMRU", 16, 256);
    mruColors = createMRUList("TexWords", "ColorMRU", 16, 256);

    bindKeyProfile(&texwordseditor_binds_profile);
    texwordseditor_binds_profile.trickleKeys = 1;

    texWordsEditorAddFonts();

    texWordsEditorResetState();
    tweditor_state.fromEditor = fromEditor;
    texWordsEditorSetup();
    tweditor_state.fromEditor = fromEditor;
    // These get reset in texWordsEditorResetState
    if (fromEditor) {
        strcpy(tweditor_state.tweLocale, locGetName(getCurrentLocale()));
    } else {
        strcpy(tweditor_state.tweLocale, "Base");
    }

    ent_debug_binds_profile.trickleKeys = 1;
    keybind_setkey( &ent_debug_binds_profile, "space", "++stretch", 0, 0 );

    while (!tweditor_state.requestQuit)
    {
        autoTimerTickBegin();
        FolderCacheDoCallbacks(); // Check for directory changes
        texCheckThreadLoader();
        inpUpdate();                // Mouse and keyboard input meant as commands, mostly
        windowProcessMessages();    // Keyboard input for chat and other windows messages, mostly.
        bindCheckKeysReleased();    // mw moved this out of display().  Hopefully nothing breaks
        conProcess(); //Handle the in-game ~ console
        handleCursor();

        gfxWindowReshape();
        rdrInitTopOfFrame();
        rdrInitTopOfView(unitmat,unitmat);

        {
            int mouse_x, mouse_y;
            inpMousePos(&mouse_x, &mouse_y);
            texWordsEdit_mouseMove(mouse_x, mouse_y);
        }

        if (tweditor_state.needToRebuild || !drawMeBind) {
            loadMessage("Generating image...");
            rebuildTextures();
            tweditor_state.needToRebuild=0;
        }

        if (tweditor_state.reOpenMenu) {
            texWordsEditorBuildMenu();
            tweditor_state.reOpenMenu = 0;
            debug_state.openingTime = 0;
            debug_state.mouseMustMove = 0;
        }

        if (tweditor_state.stretch) {
            windowClientSize(&tweditor_state.display.width, &tweditor_state.display.height);
            tweditor_state.display.scalex = tweditor_state.display.width / (F32)drawMeBind->width;
            tweditor_state.display.scaley = tweditor_state.display.height / (F32)drawMeBind->height;
            tweditor_state.display.offsx = 0;
            tweditor_state.display.offsy = 0;
        } else {
            tweditor_state.display.width = drawMeBind->width;
            tweditor_state.display.height = drawMeBind->height;
            tweditor_state.display.scalex = 1.0;
            tweditor_state.display.scaley = 1.0;
            tweditor_state.display.offsx = 64;
            tweditor_state.display.offsy = 64;
        }
        if (debug_state.menuItem) {
            windowClientSize(&tweditor_state.display.width, &tweditor_state.display.height);
            tweditor_state.display.offsx = debug_state.size.greenx;
            tweditor_state.display.offsy = tweditor_state.display.height - debug_state.size.greeny;
            if (tweditor_state.stretch) {
                tweditor_state.display.width = debug_state.size.greenw;
                tweditor_state.display.height = debug_state.size.greenh;
                tweditor_state.display.scalex = tweditor_state.display.width / (F32)drawMeBind->width;
                tweditor_state.display.scaley = tweditor_state.display.height / (F32)drawMeBind->height;
            } else {
                tweditor_state.display.width = drawMeBind->width;
                tweditor_state.display.height = drawMeBind->height;
                tweditor_state.display.scalex = 1.0;
                tweditor_state.display.scaley = 1.0;
            }
        }
        display_sprite_tex( drawMeBind->tex_layers[TEXLAYER_BASE],tweditor_state.display.offsx,tweditor_state.display.offsy, 1,tweditor_state.display.scalex,tweditor_state.display.scaley,0xffffffff);

        doColorPicker();

        // Status text
        {
            strcpy(status, "");
            if (tweditor_state.dirty)
                strcat(status, "*");
            strcat(status, game_state.texWordEdit);
            if (tweditor_state.stretch)
                strcat(status, " (STRETCHED)");
            if (tweditor_state.orig)
                strcat(status, " (ORIGINAL TEXTURE)");
            strcatf(status, " Color: %d %d %d %d", tweditor_state.selcolor.r, tweditor_state.selcolor.g, tweditor_state.selcolor.b, tweditor_state.selcolor.a);

            if (!tweditor_state.hideText)
                printBasic( &game_14, 10, game_state.screen_y - 1, 100, 1.0, 1.0, 0, status, strlen(status), rgba);
        }
        if (tweditor_state.extents.edit) {
            sprintf(status, "Extents: (%1.f, %1.f)-(%1.f, %1.f) (%1.fx%1.f)", tweditor_state.extents.x0, tweditor_state.extents.y0, tweditor_state.extents.x1, tweditor_state.extents.y1, tweditor_state.extents.x1 - tweditor_state.extents.x0, tweditor_state.extents.y1 - tweditor_state.extents.y0);
            if (!tweditor_state.hideText)
                printBasic( &game_14, 10, game_state.screen_y - 15, 100, 1.0, 1.0, 0, status, strlen(status), rgba);
        } else {
            int xp, yp;
            inpMousePos(&xp, &yp);
            sprintf(status, "Image resolution: %dx%d  MousePos: %d,%d", drawMeBind->width, drawMeBind->height, xp, yp);
            if (!tweditor_state.hideText)
                printBasic( &game_14, 10, game_state.screen_y - 15, 100, 1.0, 1.0, 0, status, strlen(status), rgba);
        }
        drawStatusFading();

//        printBasic( &game_18, 100, 350, 102, 10.0, 10.0, 0, "Testinging...", strlen("Testinging..."), rgba);

#if 1
        rdrSetBgColor(tweditor_state.bgcolor);
#else
        glClearColor(tweditor_state.bgcolor[0], tweditor_state.bgcolor[1], tweditor_state.bgcolor[2], tweditor_state.bgcolor[3]); CHECKGL;
#endif
        rdrClearScreen();
        fontRenderGame();
        fontRenderEditor();


        rdrSetup2DRendering();
        // Extents
        if (tweditor_state.extents.on) {
            texWordsEdit_drawExtents();
        }

        // Menu
        if (!tweditor_state.colorpick)
            displayDebugMenu();
        rdrFinish2DRendering();

        windowUpdate();

        if (tweditor_state.shift && !entDebugMenuVisible() && !tweditor_state.colorpick) {
            HDC hdc;
            hdc = GetDC(NULL);
            if (hdc) {
                COLORREF cr;
                POINT pCursor;
                U8 alpha = tweditor_state.selcolor.a;
                GetCursorPos(&pCursor);
                cr = GetPixel(hdc, pCursor.x, pCursor.y);
                ReleaseDC(NULL, hdc);
                tweditor_state.selcolor.integer = cr;
                tweditor_state.selcolor.a = 255; //alpha;
            }
        }

        autoTimerTickEnd();
    }
    if (debug_state.menuItem) {
        freeDebugMenuItem(debug_state.menuItem);
        debug_state.menuItem = NULL;
    }
    unbindKeyProfile(&ent_debug_binds_profile); // Because it gets rebound
    unbindKeyProfile(&texwordseditor_binds_profile);
    cmdAccessOverride(0);
}

