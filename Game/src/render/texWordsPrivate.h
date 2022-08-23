#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include "render/texEnums.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/StashTable.h>

typedef enum TexWordLayerType {
    TWLT_NONE,
    TWLT_BASEIMAGE,
    TWLT_TEXT,
    TWLT_IMAGE,
} TexWordLayerType;

typedef enum TexWordLayerStretch {
    TWLS_NONE,
    TWLS_FULL,
    TWLS_TILE,
} TexWordLayerStretch;

typedef enum TexWordBlendType {
    TWBLEND_OVERLAY,
    TWBLEND_MULTIPLY,
    TWBLEND_ADD,
    TWBLEND_SUBTRACT,
    TWBLEND_REPLACE,
} TexWordBlendType;

typedef enum TexWordFilterType {
    TWFILTER_NONE,
    TWFILTER_BLUR,
    TWFILTER_DROPSHADOW,
    TWFILTER_DESATURATE,
} TexWordFilterType;


typedef struct TexWordLayerFont {
    char *fontName;

    int drawSize;
    bool italicize;
    bool bold;
    U8 outlineWidth;
    U8 dropShadowXOffset;
    U8 dropShadowYOffset;
    U8 softShadowSpread; // Probably don't use this...

} TexWordLayerFont;

typedef struct TexWordLayerFilter {
    TexWordFilterType type;
    int magnitude; // In px for blur, dropshadow, etc
    F32 percent;   // Magnitude in the 0..1 range
    eaiHandle rgba;
    int offset[2];
    F32 spread;
    TexWordBlendType blend;
} TexWordLayerFilter;

typedef struct TexWordLayer {
    char *layerName;
    // Data defining the layer
    TexWordLayerType type;
    TexWordLayerStretch stretch;
    char *text;
    F32 pos[2];
    F32 size[2];
    F32 rot;
    eaiHandle rgba;
    eaiHandle rgbas[4];
    char *imageName;
    bool hidden;
    TexWordLayerFont font;
    // Filter (one only?)
    TexWordLayerFilter **filter;

    // Sub-layer (should only have one!)
    struct TexWordLayer **sublayer;
    TexWordBlendType subBlend;
    F32 subBlendWeight;

    // Working data
    BasicTexture *image;
} TexWordLayer;

typedef struct TexWord {
    char *name;
    int width;
    int height;
    TexWordLayer **layers;
} TexWord;

typedef struct TexWordList {
    TexWord **texWords;
} TexWordList;

typedef struct TexWordLoadInfo {
    U8 *data;
    int actualSizeX;
    int actualSizeY;
    int sizeX;
    int sizeY;
} TexWordLoadInfo;



extern TokenizerParseInfo ParseTexWord[];
extern TokenizerParseInfo ParseTexWordFiles[];
extern TokenizerParseInfo ParseTexWordLayer[];
extern TokenizerParseInfo ParseTexWordLayerFilter[];
extern StaticDefineInt    ParseTexWordLayerType[];
extern StaticDefineInt    ParseTexWordLayerStretch[];
extern StaticDefineInt    ParseTexWordBlendType[];
extern StaticDefineInt    ParseTexWordFilterType[];


extern TexWordList texWords_list;
extern StashTable htTexWords; // Indexed by TexName, only has current locale in hashtable
extern int texWords_useGl;

bool texWordVerify(TexWord *texWord, bool fix);


