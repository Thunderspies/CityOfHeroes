#include <stdio.h>  // fopen
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <sys/stat.h>

#include "formatter/smf_util.h"
#include <utilitieslib/utils/StringUtil.h>

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>

#include "graphics/truetype/ttFontDraw.h"
#include "UI/sprite/sprite_base.h"

#include "UI/uiFocus.h"
#include "UI/uiUtil.h"

#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/MemoryMonitor.h>

static SMFSelectionCommand smf_selectionCommand = SMFSelectionCommand_None;
static char *smf_selectionPreTag = NULL;
static char *smf_selectionPostTag = NULL;

bool smf_hasDisplayedHoverBackgroundThisFrame = false;
bool s_bAllowAnchor = true;
bool smfCursor_display = true;
float smfCursor_currentTimer = 0.0f;
float smfCursor_interval = 0.6f;

SMFBlock *smf_selectionStartPane = 0;
unsigned int smf_selectionStartDisplayIndex = 0;
int smf_selectionStartYBase = 0;

SMFBlock *smf_selectionEndPane = 0;
unsigned int smf_selectionEndDisplayIndex = 0;
int smf_selectionEndXBase = 0;
int smf_selectionEndYBase = 0;
int smf_selectionEndCharacterXOffset = 0;
int smf_selectionEndCharacterYOffset = 0;
int smf_selectionEndCharacterYHeight = 0;

int smf_selectionCurrentYBase = -1;
int smf_selectionCurrentIY = 0;
bool smf_isCurrentlyDragging = false;

SMFBlock *smf_selectionSearchPane = 0;
unsigned int smf_selectionSearchDisplayIndex = 0;
int smf_selectionSearchXBase = 0;
int smf_selectionSearchYBase = 0;
int smf_selectionSearchCharacterXOffset = 0;
int smf_selectionSearchCharacterYOffset = 0;
int smf_selectionSearchCharacterYHeight = 0;

SMFSearchCommand smfSearch_currentCommand = SMFSearchCommand_None;
SMFSearchCommand smfSearch_lastCommand = SMFSearchCommand_None;
bool smfSearch_nextTabbedBlock_justSawEndPane = false;
bool smfSearch_keepStartConstant = false;

SMFEditCommand smfEdit_currentCommand = SMFEditCommand_None;
char *smfEdit_currentCharactersToInsert = 0;

bool smf_stopSettingTheSelectionThisFrame = false;

bool smf_forceReformatAll = false;
bool smf_displayedSomethingThisFrame = false;
bool smf_gainedFocusThisFrame = false;

TupleSN s_aAlignments[] =
{
    { "none",   SMAlignment_None   },
    { "left",   SMAlignment_Left   },
    { "right",  SMAlignment_Right  },
    { "center", SMAlignment_Center },
    { "middle", SMAlignment_Center },
    { "top",    SMAlignment_Top    },
    { "bottom", SMAlignment_Bottom },
    { "both",   SMAlignment_Both   },
    { 0 },
};

static TupleSN s_aColors[] =
{
    { "AliceBlue",            0xf0f8ffff },
    { "AntiqueWhite",         0xfaebd7ff },
    { "Aqua",                 0x00ffffff },
    { "Aquamarine",           0x7fffd4ff },
    { "Azure",                0xf0ffffff },
    { "Beige",                0xf5f5dcff },
    { "Bisque",               0xffe4c4ff },
    { "Black",                0x000000ff },
    { "BlanchedAlmond",       0xffebcdff },
    { "Blue",                 0x0000ffff },
    { "BlueViolet",           0x8a2be2ff },
    { "Brown",                0xa52a2aff },
    { "BurlyWood",            0xdeb887ff },
    { "CadetBlue",            0x5f9ea0ff },
    { "Chartreuse",           0x7fff00ff },
    { "Chocolate",            0xd2691eff },
    { "Coral",                0xff7f50ff },
    { "CornflowerBlue",       0x6495edff },
    { "Cornsilk",             0xfff8dcff },
    { "Crimson",              0xdc143cff },
    { "Cyan",                 0x00ffffff },
    { "DarkBlue",             0x00008bff },
    { "DarkCyan",             0x008b8bff },
    { "DarkGoldenRod",        0xb8860bff },
    { "DarkGray",             0xa9a9a9ff },
    { "DarkGreen",            0x006400ff },
    { "DarkKhaki",            0xbdb76bff },
    { "DarkMagenta",          0x8b008bff },
    { "DarkOliveGreen",       0x556b2fff },
    { "Darkorange",           0xff8c00ff },
    { "DarkOrchid",           0x9932ccff },
    { "DarkRed",              0x8b0000ff },
    { "DarkSalmon",           0xe9967aff },
    { "DarkSeaGreen",         0x8fbc8fff },
    { "DarkSlateBlue",        0x483d8bff },
    { "DarkSlateGray",        0x2f4f4fff },
    { "DarkTurquoise",        0x00ced1ff },
    { "DarkViolet",           0x9400d3ff },
    { "DeepPink",             0xff1493ff },
    { "DeepSkyBlue",          0x00bfffff },
    { "DimGray",              0x696969ff },
    { "DodgerBlue",           0x1e90ffff },
    { "Feldspar",             0xd19275ff },
    { "FireBrick",            0xb22222ff },
    { "FloralWhite",          0xfffaf0ff },
    { "ForestGreen",          0x228b22ff },
    { "Fuchsia",              0xff00ffff },
    { "Gainsboro",            0xdcdcdcff },
    { "GhostWhite",           0xf8f8ffff },
    { "Gold",                 0xffd700ff },
    { "GoldenRod",            0xdaa520ff },
    { "Gray",                 0x808080ff },
    { "Green",                0x008000ff },
    { "GreenYellow",          0xadff2fff },
    { "HoneyDew",             0xf0fff0ff },
    { "HotPink",              0xff69b4ff },
    { "IndianRed",            0xcd5c5cff },
    { "Indigo",               0x4b0082ff },
    { "Ivory",                0xfffff0ff },
    { "Khaki",                0xf0e68cff },
    { "Lavender",             0xe6e6faff },
    { "LavenderBlush",        0xfff0f5ff },
    { "LawnGreen",            0x7cfc00ff },
    { "LemonChiffon",         0xfffacdff },
    { "LightBlue",            0xadd8e6ff },
    { "LightCoral",           0xf08080ff },
    { "LightCyan",            0xe0ffffff },
    { "LightGoldenRodYellow", 0xfafad2ff },
    { "LightGrey",            0xd3d3d3ff },
    { "LightGreen",           0x90ee90ff },
    { "LightPink",            0xffb6c1ff },
    { "LightSalmon",          0xffa07aff },
    { "LightSeaGreen",        0x20b2aaff },
    { "LightSkyBlue",         0x87cefaff },
    { "LightSlateBlue",       0x8470ffff },
    { "LightSlateGray",       0x778899ff },
    { "LightSteelBlue",       0xb0c4deff },
    { "LightYellow",          0xffffe0ff },
    { "Lime",                 0x00ff00ff },
    { "LimeGreen",            0x32cd32ff },
    { "Linen",                0xfaf0e6ff },
    { "Magenta",              0xff00ffff },
    { "Maroon",               0x800000ff },
    { "MediumAquaMarine",     0x66cdaaff },
    { "MediumBlue",           0x0000cdff },
    { "MediumOrchid",         0xba55d3ff },
    { "MediumPurple",         0x9370d8ff },
    { "MediumSeaGreen",       0x3cb371ff },
    { "MediumSlateBlue",      0x7b68eeff },
    { "MediumSpringGreen",    0x00fa9aff },
    { "MediumTurquoise",      0x48d1ccff },
    { "MediumVioletRed",      0xc71585ff },
    { "MidnightBlue",         0x191970ff },
    { "MintCream",            0xf5fffaff },
    { "MistyRose",            0xffe4e1ff },
    { "Moccasin",             0xffe4b5ff },
    { "NavajoWhite",          0xffdeadff },
    { "Navy",                 0x000080ff },
    { "OldLace",              0xfdf5e6ff },
    { "Olive",                0x808000ff },
    { "OliveDrab",            0x6b8e23ff },
    { "Orange",               0xffa500ff },
    { "OrangeRed",            0xff4500ff },
    { "Orchid",               0xda70d6ff },
    { "PaleGoldenRod",        0xeee8aaff },
    { "PaleGreen",            0x98fb98ff },
    { "PaleTurquoise",        0xafeeeeff },
    { "PaleVioletRed",        0xd87093ff },
    { "PapayaWhip",           0xffefd5ff },
    { "Paragon",              0x00deffff },
    { "PeachPuff",            0xffdab9ff },
    { "Peru",                 0xcd853fff },
    { "Pink",                 0xffc0cbff },
    { "Plum",                 0xdda0ddff },
    { "PowderBlue",           0xb0e0e6ff },
    { "Purple",               0x800080ff },
    { "Red",                  0xff0000ff },
    { "RosyBrown",            0xbc8f8fff },
    { "RoyalBlue",            0x4169e1ff },
    { "SaddleBrown",          0x8b4513ff },
    { "Salmon",               0xfa8072ff },
    { "SandyBrown",           0xf4a460ff },
    { "SeaGreen",             0x2e8b57ff },
    { "SeaShell",             0xfff5eeff },
    { "Sienna",               0xa0522dff },
    { "Silver",               0xc0c0c0ff },
    { "SkyBlue",              0x87ceebff },
    { "SlateBlue",            0x6a5acdff },
    { "SlateGray",            0x708090ff },
    { "Snow",                 0xfffafaff },
    { "SpringGreen",          0x00ff7fff },
    { "SteelBlue",            0x4682b4ff },
    { "Tan",                  0xd2b48cff },
    { "Teal",                 0x008080ff },
    { "Thistle",              0xd8bfd8ff },
    { "Tomato",               0xff6347ff },
    { "Turquoise",            0x40e0d0ff },
    { "Violet",               0xee82eeff },
    { "Villagon",             0xfca91aff },
    { "VioletRed",            0xd02090ff },
    { "Wheat",                0xf5deb3ff },
    { "White",                0xffffffff },
    { "WhiteSmoke",           0xf5f5f5ff },
    { "Yellow",               0xffff00ff },
    { "YellowGreen",          0x9acd32ff },
    { "InfoBlue",              0xaadeffff },
    { "ArchitectTitle",          CLR_MM_TITLE_OPEN },
    { "Architect",              CLR_MM_TEXT },
    { "ArchitectDark",          CLR_MM_TEXT_DARK },
    { "ArchitectError",          CLR_MM_TEXT_ERROR },
    { "ArchitectErrorLit",      CLR_MM_BUTTON_ERROR_TEXT_LIT },
    { 0 },
};

/***************************************************************************/
/* SMF Memory Analysis Flags                                               */
/***************************************************************************/

int smf_analyzeMemory = 0; // set to 1 to turn on SMF's surface-level memory analysis, 2 for deeper
bool smf_analyzeMemoryIsLeaking = false; // set a data breakpoint on this to check for true
int smf_drawDebugBoxes = 0; // set to 1 to draw boxes for the SMFBlock areas, 2 to draw scissors boxes for the SMFBlock areas.
int smf_printDebugLines = 0; // set to 1 to get some printf's that I have strewn throughout the SMF code.


const SMFLineBreakMode SMFLineBreakMode_DefaultValue = SMFLineBreakMode_MultiLineBreakOnWhitespace;
const SMFInputMode SMFInputMode_DefaultValue = SMFInputMode_AnyTextNoTagsOrCodes;
const int SMFInputLimit_DefaultValue = 1023;
const SMFEditMode SMFEditMode_DefaultValue = SMFEditMode_ReadOnly;
const SMFScrollMode SMFScrollMode_DefaultValue = SMFScrollMode_ExternalOnly;
const SMFOutputMode SMFOutputMode_DefaultValue = SMFOutputMode_StripAllTags;
const SMFDisplayMode SMFDisplayMode_DefaultValue = SMFDisplayMode_AllCharacters;
const SMFContextMenuMode SMFContextMenuMode_DefaultValue = SMFContextMenuMode_None;

MP_DEFINE(SMBlock);
/**********************************************************************func*
* sm_CreateBlock
*
*/
SMBlock *sm_CreateBlock(void)
{
    SMBlock *pnew;

    // Initialize the memory pool (only does anything the first time it's called).
    MP_CREATE(SMBlock, 64);

    pnew = MP_ALLOC(SMBlock);

    return pnew;
}

/**********************************************************************func*
* sm_AppendBlock
*
*/
SMBlock *sm_AppendBlock(SMBlock *pBlockParent, SMBlock *pBlock)
{
    assert(pBlockParent->bHasBlocks);

    pBlock->pParent = pBlockParent;
    pBlock->pSMFBlock = pBlockParent->pSMFBlock;
    eaPush(&pBlockParent->ppBlocks, pBlock);

    return pBlock;
}

/**********************************************************************func*
* sm_CreateContainer
*
*/
SMBlock *sm_CreateContainer(void)
{
    SMBlock *pnew = sm_CreateBlock();

    pnew->bHasBlocks = true;
    eaCreate(&pnew->ppBlocks);

    return pnew;
}

/**********************************************************************func*
* sm_AppendNewBlock
*
*/
SMBlock *sm_AppendNewBlock(SMBlock *pBlock)
{
    return sm_AppendBlock(pBlock, sm_CreateBlock());
}


/**********************************************************************func*
* sm_AppendNewBlock
*
*/
SMBlock *sm_AppendNewBlockAndData(SMBlock *pBlock, int iSize)
{
    SMBlock *pnew = sm_AppendBlock(pBlock, sm_CreateBlock());
    if(pnew!=NULL && iSize>0)
    {
        pnew->pv = calloc(iSize, 1);
        pnew->bFreeOnDestroy = true;
    }
    return pnew;
}

/**********************************************************************func*
* sm_AppendNewContainer
*
*/
SMBlock *sm_AppendNewContainer(SMBlock *pBlock)
{
    return sm_AppendBlock(pBlock, sm_CreateContainer());
}

/**********************************************************************func*
* sm_AppendNewContainer
*
*/
SMBlock *sm_AppendNewContainerAndData(SMBlock *pBlock, int iSize)
{
    SMBlock *pnew = sm_AppendBlock(pBlock, sm_CreateContainer());
    if(pnew!=NULL && iSize>0)
    {
        pnew->pv = calloc(iSize, 1);
        pnew->bFreeOnDestroy = true;
    }
    return pnew;
}

/**********************************************************************func*
* indent
*
*/
static void indent(int i)
{
    while(i>0)
    {
        printf("  ");
        i--;
    }
}

/**********************************************************************func*
* sm_BlockDump
*
*/
void sm_BlockDump(SMBlock *pBlock, int iLevel, SMTagDef aTagDefs[])
{
    indent(iLevel);

    if(pBlock->iType>=0)
    {
        printf("%-6.6s: ",aTagDefs[pBlock->iType].pchName);
    }
    else if(pBlock->iType==-1)
    {
        printf("block: ");
    }
    else
    {
        printf("%-3d: ", pBlock->iType);
    }

    if(pBlock->pv && pBlock->bFreeOnDestroy)
    {
        char ach[50];
        sprintf_s(SAFESTR(ach), "\"%.30s\"", (char *)pBlock->pv);
        printf("%-30.30s (0x%p)", ach, (char *)pBlock->pv);
    }
    else
    {
        //        printf("(null)");
    }

    printf("  (%fx%f)", pBlock->pos.iMinWidth, pBlock->pos.iMinHeight);
    printf("  (%fx%f)", pBlock->pos.iWidth, pBlock->pos.iHeight);
    printf("  (%f, %f)", pBlock->pos.iX, pBlock->pos.iY);

    printf("\n");

    if(pBlock->bHasBlocks)
    {
        int i;
        int iNumBlocks = eaSize(&pBlock->ppBlocks);
        indent(iLevel);
        printf("{\n");
        for(i=0; i<iNumBlocks; i++)
        {
            sm_BlockDump(pBlock->ppBlocks[i], iLevel+1, aTagDefs);
        }
        indent(iLevel);
        printf("}\n");
    }
}


/**********************************************************************func*
* sm_DestroyBlock
*
*/
void sm_DestroyBlock(SMBlock *pBlock)
{
    PERFINFO_AUTO_START("sm_DestroyBlock", 1);
    if(pBlock->bHasBlocks)
    {
        int i;
        for(i=eaSize(&pBlock->ppBlocks)-1; i>=0; i--)
        {
            sm_DestroyBlock(pBlock->ppBlocks[i]);
            pBlock->ppBlocks[i] = S32_TO_PTR(0xd00dd00d);
        }

        eaDestroy(&pBlock->ppBlocks);
    }

    if(pBlock->pv && pBlock->bFreeOnDestroy)
    {
        free(pBlock->pv);
        pBlock->pv = S32_TO_PTR(0xd00dd00d);
    }

    MP_FREE(SMBlock, pBlock);
    PERFINFO_AUTO_STOP();
}

/**********************************************************************func*
* FindTupleSN
*
*/
TupleSN *FindTupleSN(TupleSN *pTuples, char *pch)
{
    while(pTuples->pchName!=NULL)
    {
        if(stricmp(pTuples->pchName, pch)==0)
        {
            return pTuples;
        }
        pTuples++;
    }

    return NULL;
}

/**********************************************************************func*
* FindTupleSS
*
*/
TupleSS *FindTupleSS(TupleSS *pTuples, char *pch)
{
    while(pTuples->pchName!=NULL)
    {
        if(stricmp(pTuples->pchName, pch)==0)
        {
            return pTuples;
        }
        pTuples++;
    }

    return NULL;
}

/**********************************************************************func*
* sm_GetVal
*
*/
char *sm_GetVal(char *pchAttrib, TupleSS *pTuples)
{
    TupleSS *tup = FindTupleSS(pTuples, pchAttrib);
    if(tup)
    {
        return tup->pchVal;
    }

    return NULL;
}

/**********************************************************************func*
* sm_GetAlignment
*
*/
int sm_GetAlignment(char *pchAttrib, TupleSS *pTuples)
{
    TupleSS *tup = FindTupleSS(pTuples, pchAttrib);
    if(tup)
    {
        TupleSN *tup2 = FindTupleSN(s_aAlignments, tup->pchVal);
        if(tup2!=NULL)
        {
            return tup2->iVal;
        }
    }

    return SMAlignment_None;
}

/**********************************************************************func*
* sm_GetColor
*
*/
U32 sm_GetColor(char *pchAttrib, TupleSS *pTuples)
{
    U32 uiRet = 0x000000ff;

    TupleSS *tup = FindTupleSS(pTuples, pchAttrib);
    if(tup)
    {
        if(tup->pchVal[0]=='#')
        {
            sscanf(tup->pchVal+1, "%x", &uiRet);

            if(strlen(&tup->pchVal[1])<=6)
            {
                uiRet <<= 8;
                uiRet |= 0xff;
            }
        }
        else
        {
            TupleSN *tup2 = FindTupleSN(s_aColors, tup->pchVal);
            if(tup2!=NULL)
            {
                uiRet = tup2->iVal;
            }
        }
    }

    return uiRet;
}

/**********************************************************************func*
* smf_ConvertColorNameToValue
*
*/
U32 smf_ConvertColorNameToValue(char *pchColorName)
{
    U32 uiRet = 0x000000ff;

    if (pchColorName)
    {
        TupleSN *tup = FindTupleSN(s_aColors, pchColorName);

        if (tup)
        {
            uiRet = tup->iVal;
        }
    }

    return uiRet;
}

void smf_ClearSelection(void)
{
    if (smf_printDebugLines)
    {
        printf("smf_ClearSelection() called.\n");
    }

    smf_selectionStartPane = 0;
    smf_selectionStartDisplayIndex = 0;
    smf_selectionStartYBase = 0;

    smf_selectionEndPane = 0;
    smf_selectionEndDisplayIndex = 0;
    smf_selectionEndXBase = 0;
    smf_selectionEndYBase = 0;
    smf_selectionEndCharacterXOffset = 0;
    smf_selectionEndCharacterYOffset = 0;
    smf_selectionEndCharacterYHeight = 0;

    smf_selectionCurrentYBase = -1;
    smf_selectionCurrentIY = 0;
    smf_isCurrentlyDragging = false;    

    smf_selectionSearchPane = 0;
    smf_selectionSearchDisplayIndex = 0;
    smf_selectionSearchXBase = 0;
    smf_selectionSearchYBase = 0;
    smf_selectionSearchCharacterXOffset = 0;
    smf_selectionSearchCharacterYOffset = 0;
    smf_selectionSearchCharacterYHeight = 0;

    smf_selectionCommand = SMFSelectionCommand_None;

    smf_ReturnFocus();
}

void smf_ClearSelectionCommand(void)
{
    smf_selectionCommand = SMFSelectionCommand_None;
}

SMFSelectionCommand smf_GetSelectionCommand(void)
{
    return smf_selectionCommand;
}

bool smf_SetSelectionCommand(SMFSelectionCommand newCmd)
{
    bool retval = true;

    if (smf_selectionCommand != SMFSelectionCommand_None)
    {
        retval = false;
    }

    smf_selectionCommand = newCmd;

    return retval;
}

bool smf_SetSelectionTags(char *preTag, char *postTag)
{
    bool retval = true;

    if (smf_selectionCommand != SMFSelectionCommand_None)
    {
        retval = false;
    }

    if (!smf_selectionPreTag)
    {
        estrCreate(&smf_selectionPreTag);
    }
    else
    {
        estrClear(&smf_selectionPreTag);
    }

    if (!smf_selectionPostTag)
    {
        estrCreate(&smf_selectionPostTag);
    }
    else
    {
        estrClear(&smf_selectionPostTag);
    }

    if (preTag)
    {
        estrConcatCharString(&smf_selectionPreTag, preTag);
    }

    if (postTag)
    {
        estrConcatCharString(&smf_selectionPostTag, postTag);
    }

    return retval;
}

char *smf_GetSelectionPreTag(void)
{
    return smf_selectionPreTag;
}

char *smf_GetSelectionPostTag(void)
{
    return smf_selectionPostTag;
}

/**********************************************************************func*
* smf_PushAttrib
*
*/
void smf_PushAttrib(SMBlock *pBlock, TextAttribs *pattr)
{
    int **ppi = (int **)pattr;

    if(ppi!=NULL
        && pBlock->iType>=0
        && pBlock->iType<kFormatTags_Count)
    {
        eaiPush(&ppi[pBlock->iType], *(int *)(&pBlock->pv));
    }
}

/**********************************************************************func*
* smf_PopAttrib
*
*/
int smf_PopAttrib(SMBlock *pBlock, TextAttribs *pattr)
{
    int idx = pBlock->iType-kFormatTags_Count;

    return smf_PopAttribInt(idx, pattr);
}

/**********************************************************************func*
* smf_PopAttribInt
*
*/
int smf_PopAttribInt(int idx, TextAttribs *pattr)
{
    int **ppi = (int **)pattr;

    if(ppi!=NULL
        && idx>=0
        && idx<kFormatTags_Count
        && eaiSize(&ppi[idx])>1)
    {
        return eaiPop(&ppi[idx]);
    }

    return 0;
}

/**********************************************************************func*
* smf_ApplyFormatting
*
* This function applies the formatting changes from an SMBlock object to the TextAttribs object.
* Returns true if the SMBlock object is a formatting object, false if it is not a formatting object.
*
*/
bool smf_ApplyFormatting(SMBlock *pBlock, TextAttribs *pattrs)
{
    bool returnMe = true;
    int startingBytesAlloced, endingBytesAlloced;

    if (smf_analyzeMemory >= 2)
    {
        startingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = false;
    }

    if(pBlock->iType == kFormatTags_Font+kFormatTags_Count)
    {
        int iFont = smf_PopAttrib(pBlock, pattrs);
        SMFont *pFont = (SMFont *)&iFont;

        if(pFont->bColor)
            smf_PopAttribInt(kFormatTags_Color, pattrs);
        if(pFont->bColor2)
            smf_PopAttribInt(kFormatTags_Color2, pattrs);
        if(pFont->bFace)
            smf_PopAttribInt(kFormatTags_Face, pattrs);
        if(pFont->bScale)
            smf_PopAttribInt(kFormatTags_Scale, pattrs);

    }
    // Need these color<->link switches so color tags work inside of anchors
    else if(pBlock->iType == kFormatTags_Anchor) { //push a fake color tag
        int temp = pattrs->piColor[eaiSize(&pattrs->piColor)-1];
        pattrs->piColor[eaiSize(&pattrs->piColor)-1] = pattrs->piLink[eaiSize(&pattrs->piLink)-1];
        pattrs->piLink[eaiSize(&pattrs->piLink)-1] = temp;
        smf_PushAttrib(pBlock, pattrs);
    }
    else if(pBlock->iType == kFormatTags_Anchor+kFormatTags_Count) { //push a fake color tag
        int temp = pattrs->piColor[eaiSize(&pattrs->piColor)-1];
        pattrs->piColor[eaiSize(&pattrs->piColor)-1] = pattrs->piLink[eaiSize(&pattrs->piLink)-1];
        pattrs->piLink[eaiSize(&pattrs->piLink)-1] = temp;
        smf_PopAttrib(pBlock, pattrs);
    }
    else if(pBlock->iType<kFormatTags_Count)
    {
        smf_PushAttrib(pBlock, pattrs);
    }
    else if(pBlock->iType>=kFormatTags_Count && pBlock->iType<kFormatTags_Count*2)
    {
        smf_PopAttrib(pBlock, pattrs);
    }
    else
    {
        returnMe = false;
    }

    if (smf_analyzeMemory >= 2)
    {
        endingBytesAlloced = memMonitorBytesAlloced();
        smf_analyzeMemoryIsLeaking = (endingBytesAlloced != startingBytesAlloced);
    }

    return returnMe;
}


/**********************************************************************func*
* MakeFont
*
*/
void smf_MakeFont(TTDrawContext **ppttFont, TTFontRenderParams *prp, TextAttribs *pattrs)
{
    // Set the font
    *ppttFont = (TTDrawContext *)pattrs->piFace[eaiSize(&pattrs->piFace)-1];

    // Remember the original renderparams
    *prp = (*ppttFont)->renderParams;

    // Now set each renderparam
    (*ppttFont)->renderParams.bold      = pattrs->piBold[eaiSize(&pattrs->piBold)-1]   ? 1 : 0;
    (*ppttFont)->renderParams.italicize = pattrs->piItalic[eaiSize(&pattrs->piItalic)-1] ? 1 : 0;

    (*ppttFont)->renderParams.outlineWidth = pattrs->piOutline[eaiSize(&pattrs->piOutline)-1];
    (*ppttFont)->renderParams.outline = (*ppttFont)->renderParams.outlineWidth ? 1 : 0;

    (*ppttFont)->renderParams.dropShadowXOffset =
        (*ppttFont)->renderParams.dropShadowYOffset =
        pattrs->piShadow[eaiSize(&pattrs->piShadow)-1];
    (*ppttFont)->renderParams.dropShadow = (*ppttFont)->renderParams.dropShadowXOffset ? 1 : 0;

    // We could add these later
    // pttFont->softShadow
    // pttFont->softShadowSpread
}

typedef struct {
    TTTextForEachGlyphParam forEachParam;
    float* characterWidths;
    float lastWidthSum;
} smf_CharacterWidthsParam;

static int smf_CharacterWidthsHandler(smf_CharacterWidthsParam* param){
    float characterWidth = param->forEachParam.nextGlyphLeft - param->lastWidthSum;
    eafPush(&param->characterWidths, characterWidth);
    param->lastWidthSum = param->forEachParam.nextGlyphLeft;
    return 1;
}

float* smf_GetCharacterWidths(SMBlock *pBlock, TextAttribs *pattrs)
{
    unsigned int i;
    TTDrawContext *pttFont;
    TTFontRenderParams rp;
    smf_CharacterWidthsParam characterWidthsParam;
    unsigned int characterWidthsSize;
    float fScale;
    unsigned char *shortText;
    unsigned int shortTextSize;
    unsigned short *lineText;
    unsigned int lineTextSize;

    if (!TAG_MATCHES(sm_text))
    {
        return NULL;
    }

    smf_MakeFont(&pttFont, &rp, pattrs);

    characterWidthsParam.forEachParam.handler = (GlyphHandler)smf_CharacterWidthsHandler;
    characterWidthsParam.characterWidths = NULL;
    characterWidthsParam.lastWidthSum = 0.0;

    fScale = smBlock_CalcFontScale(pBlock, pattrs);

    shortText = (char *)pBlock->pv;
    shortTextSize = shortText ? strlen(shortText) : 0;
    lineTextSize = (shortTextSize + 1) * sizeof(unsigned short);
    lineText = (unsigned short *) malloc(lineTextSize);

    if(!shortText || shortText[0] == '\0')
    {
        lineText[0] = '\0';
    }
    else
    {
        UTF8ToWideStrConvert(shortText, lineText, lineTextSize);
    }
    ttTextForEachGlyph(pttFont, (TTTextForEachGlyphParam*)&characterWidthsParam, 0, 0, fScale, fScale, lineText, estrWideLength(&lineText), true);
    characterWidthsSize = eafSize(&characterWidthsParam.characterWidths);
    for(i = 0; i < characterWidthsSize; i++)
    {
        reverseToScreenScalingFactorf(&characterWidthsParam.characterWidths[i], NULL);
    }

    free(lineText);

    // Reset the font we used.
    pttFont->renderParams = rp;

    return characterWidthsParam.characterWidths;
}

bool smf_IsSelectionStartBeforeSelectionEnd()
{
    if (!smf_selectionStartPane || !smf_selectionEndPane)
    {
        return false; // dummy value
    }

    if (smf_selectionStartPane == smf_selectionEndPane)
    {
        return smf_selectionStartDisplayIndex < smf_selectionEndDisplayIndex;
    }
    else
    {
        return smf_selectionStartYBase < smf_selectionEndYBase;
    }
}

bool smf_IsBlockSelectable(SMFBlock *pSMFBlock)
{
    // The default state is selectable.
    return !pSMFBlock || pSMFBlock->editMode != SMFEditMode_Unselectable;
}

bool smf_IsBlockEditable(SMFBlock *pSMFBlock)
{
    // The default state is not editable.
    return pSMFBlock && pSMFBlock->editMode == SMFEditMode_ReadWrite;
}

bool smf_IsBlockMultiline(SMFBlock *pSMFBlock)
{
    // The default state is multi-line.
    return !pSMFBlock || pSMFBlock->lineBreakMode != SMFLineBreakMode_SingleLine;
}

bool smf_IsBlockScrollable(SMFBlock *pSMFBlock)
{
    // The default state is not scrollable.
    return pSMFBlock && pSMFBlock->scrollMode == SMFScrollMode_InternalScrolling;
}

unsigned int smBlock_ConvertRelativeDisplayIndexToRelativeRawIndex(SMBlock *pBlock, unsigned int relativeDisplayIndex)
{
    char *relativeRawString = pBlock->pSMFBlock->rawString + pBlock->rawStringStartIndex;
    unsigned int relativeRawIndex = 0;

    unsigned int relativeDisplayIterator;
    for (relativeDisplayIterator = 0; relativeDisplayIterator < relativeDisplayIndex; relativeDisplayIterator++)
    {
        if (!strnicmp(relativeRawString + relativeRawIndex, "&nbsp;", 6))
        {
            relativeRawIndex += 6;
        }
        else if (!strnicmp(relativeRawString + relativeRawIndex, "&lt;", 4))
        {
            relativeRawIndex += 4;
        }
        else if (!strnicmp(relativeRawString + relativeRawIndex, "&gt;", 4))
        {
            relativeRawIndex += 4;
        }
        else if (!strnicmp(relativeRawString + relativeRawIndex, "&amp;", 5))
        {
            relativeRawIndex += 5;
        }
        else if (!strnicmp(relativeRawString + relativeRawIndex, "&#37;", 5))
        {
            relativeRawIndex += 5;
        }
        else
        {
            relativeRawIndex += UTF8GetCharacterLength(relativeRawString + relativeRawIndex);            
        }
    }

    return relativeRawIndex;
}
unsigned int smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(SMBlock *pBlock, unsigned int absoluteDisplayIndex)
{
    unsigned int relativeDisplayIndex = absoluteDisplayIndex - pBlock->displayStringStartIndex;
    return pBlock->rawStringStartIndex + smBlock_ConvertRelativeDisplayIndexToRelativeRawIndex(pBlock, relativeDisplayIndex);
}

unsigned int smBlock_GetDisplayLength(SMBlock *pBlock)
{
    return (TAG_MATCHES(sm_ws) ? 1 : pBlock->pv ? UTF8GetLength((unsigned char *) pBlock->pv) : 0);
}

unsigned int smBlock_GetRawLength(SMBlock *pBlock)
{
    return smBlock_ConvertRelativeDisplayIndexToRelativeRawIndex(pBlock, smBlock_GetDisplayLength(pBlock));
}

unsigned int smfBlock_GetDisplayLength(SMFBlock *pSMFBlock)
{
    if (!pSMFBlock)
    {
        return 0;
    }

    return pSMFBlock->displayStringLength;
}

float smBlock_CalcFontScale(SMBlock *pBlock, TextAttribs *pattrs)
{
    float fScale = 1.0f;

    if (pattrs && pattrs->piScale)
    {
        fScale = ((float)pattrs->piScale[eaiSize(&pattrs->piScale)-1]/SMF_FONT_SCALE);

        if (pBlock && pBlock->pSMFBlock && pBlock->pSMFBlock->masterScale)
            fScale *= pBlock->pSMFBlock->masterScale;
    }

    return fScale;
}

bool smfBlock_HasFocus(SMFBlock *pSMFBlock)
{
    if (!pSMFBlock || !smf_selectionEndPane)
    {
        return false;
    }

    if (!smf_IsBlockSelectable(pSMFBlock))
    {
        return false;
    }
            
    if (pSMFBlock == smf_selectionEndPane)
    {
        return true;
    }

    if (!pSMFBlock->coselectionSetName || !strcmp(pSMFBlock->coselectionSetName, ""))
    {
        return false;
    }

    if (!smf_selectionEndPane->coselectionSetName)
    {
        return false;
    }

    return !strcmp(pSMFBlock->coselectionSetName, smf_selectionEndPane->coselectionSetName);
}

bool smBlock_HasFocus(SMBlock *pBlock)
{
    int pBlockTextLength;

    if (!pBlock || !smf_selectionEndPane || !pBlock->pSMFBlock)
    {
        return false;
    }

    if (!smf_IsBlockSelectable(pBlock->pSMFBlock))
    {
        return false;
    }

    if (pBlock->pSMFBlock != smf_selectionEndPane)
    {
        return false;
    }

    pBlockTextLength = smBlock_GetDisplayLength(pBlock);

    return (pBlock->displayStringStartIndex <= smf_selectionEndDisplayIndex &&
            pBlock->displayStringStartIndex + pBlockTextLength >= smf_selectionEndDisplayIndex);
}

bool smf_IsSearchSMBlock(SMBlock *pBlock)
{
    int pBlockTextLength;

    if (!pBlock || !smf_selectionSearchPane || !pBlock->pSMFBlock)
    {
        return false;
    }

    if (pBlock->pSMFBlock != smf_selectionSearchPane)
    {
        return false;
    }

    pBlockTextLength = smBlock_GetDisplayLength(pBlock);

    return (pBlock->displayStringStartIndex <= smf_selectionSearchDisplayIndex &&
            pBlock->displayStringStartIndex + pBlockTextLength >= smf_selectionSearchDisplayIndex);
}

int smf_OnLostFocus(SMFBlock *pSMFBlock, void *requester)
{
    if (pSMFBlock && smfBlock_HasFocus(pSMFBlock))
    {
        smf_ClearSelection();
    }
    return 1;
}

// Don't call this outside of SMF!  Bad things happen!
void smf_ReturnFocus()
{
    uiReturnAllFocus();
}

// Don't call this outside of SMF!  Bad things happen!
bool smf_TakeFocus(SMFBlock *pSMFBlock)
{
    bool editable = FALSE;

    if (!pSMFBlock)
    {
        Errorf("Uninitialized SMFBlock.");
        return false;
    }

    if (pSMFBlock->editMode == SMFEditMode_ReadWrite)
    {
        editable = TRUE;
    }

    return uiGetFocusEx(pSMFBlock, editable, smf_OnLostFocus);
}

void smf_SetCursor(SMFBlock *pSMFBlock, int index)
{
    if (pSMFBlock)
    {
        smf_TakeFocus(pSMFBlock);
    }
    else
    {
        smf_ReturnFocus();
    }

    smf_selectionStartPane = pSMFBlock;
    smf_selectionStartDisplayIndex = index;
    smf_selectionStartYBase = -1;

    smf_selectionEndPane = pSMFBlock;
    smf_selectionEndDisplayIndex = index;
    smf_selectionEndXBase = -1;
    smf_selectionEndYBase = -1;
    smf_selectionEndCharacterXOffset = -1;
    smf_selectionEndCharacterYOffset = -1;
    smf_selectionEndCharacterYHeight = -1;

    smf_gainedFocusThisFrame = true;
}

SMFBlock *smf_GetCursorLine()
{
    return smf_selectionEndPane;
}

// Explicitly hacked together to get keyboard move -> scroll change working in uiChat.
// Not sure if this really has a meaning within SMF.
int smf_GetCursorLineInternalYPosition()
{
    return smf_selectionEndCharacterYOffset + smf_selectionEndCharacterYHeight;
}

int smf_GetCursorIndex()
{
    if (smf_selectionEndPane)
    {
        return smf_selectionEndDisplayIndex;
    }
    else
    {
        return -1;
    }
}

int smf_GetCursorXPosition()
{
    if (smf_selectionEndPane)
    {
        return smf_selectionEndXBase + smf_selectionEndCharacterXOffset;
    }
    else
    {
        return -1;
    }
}

void smf_SelectAllText(SMFBlock *pSMFBlock)
{
    smf_SetCursor(pSMFBlock, 0);
    smf_selectionEndDisplayIndex = smfBlock_GetDisplayLength(pSMFBlock);
}

void smBlock_DetermineCurrentOffsets(SMBlock *pBlock, int x, int y, int z, int width, int height, int cursorX, int cursorY)
{
    if (!pBlock)
    {
        return;
    }

    if (TAG_MATCHES(sm_text) && smBlock_HasFocus(pBlock))
    {
        if (cursorY > y + height - pBlock->pos.iHeight - pBlock->pSMFBlock->currentDisplayOffsetY)
        {
            pBlock->pSMFBlock->currentDisplayOffsetY = (y + height - pBlock->pos.iHeight - cursorY);
        }
    }

    if(pBlock->bHasBlocks)
    {
        int i;
        int iSize = eaSize(&pBlock->ppBlocks);
        for(i=0; i<iSize; i++)
        {
            smBlock_DetermineCurrentOffsets(pBlock->ppBlocks[i], x, y, z, width, height, cursorX, cursorY);
        }
    }
}

void smf_DetermineCurrentOffsets(SMFBlock *pSMFBlock, int x, int y, int z, int width, int height)
{
    int cursorX, cursorY;

    if (!smfBlock_HasFocus(pSMFBlock))
    {
        return;
    }

    if (smf_selectionEndYBase != y)
    {
        return;
    }

    cursorX = smf_selectionEndXBase + smf_selectionEndCharacterXOffset;
    cursorY = smf_selectionEndYBase + smf_selectionEndCharacterYOffset;

    if (cursorX < x - pSMFBlock->currentDisplayOffsetX)
    {
        pSMFBlock->currentDisplayOffsetX = (x - cursorX + (width / 4));
    }

    if (cursorX >= x + width - pSMFBlock->currentDisplayOffsetX)
    {
        pSMFBlock->currentDisplayOffsetX = (x + width - cursorX - (width / 4));
    }

    if (pSMFBlock->currentDisplayOffsetX > 0)
    {
        pSMFBlock->currentDisplayOffsetX = 0;
    }

    if (cursorY < y - pSMFBlock->currentDisplayOffsetY)
    {
        pSMFBlock->currentDisplayOffsetY = (y - cursorY);
    }

    smBlock_DetermineCurrentOffsets(pSMFBlock->pBlock, x, y, z, width, height, cursorX, cursorY);
}

int smf_DetermineLetterXOffset(SMBlock* pBlock, TextAttribs *pattrs, unsigned int relativeCharacterIndex)
{
    float *characterWidths;
    unsigned int characterWidthsSize;
    int returnMe;
    unsigned int characterIndex;

    characterWidths = smf_GetCharacterWidths(pBlock, pattrs);
    characterWidthsSize = eafSize(&characterWidths);
    returnMe = 0;
    for (characterIndex = 0; characterIndex < relativeCharacterIndex && characterIndex < characterWidthsSize; characterIndex++)
    {
        returnMe += characterWidths[characterIndex];
    }

    eafDestroy(&characterWidths);

    return returnMe;
}

static void smfBlock_FreeData(SMFBlock *pSMFBlock)
{
    if (pSMFBlock->sound_successfulKeyboardInput)
    {
        SAFE_FREE(pSMFBlock->sound_successfulKeyboardInput);
    }
    if (pSMFBlock->sound_failedKeyboardInput)
    {
        SAFE_FREE(pSMFBlock->sound_failedKeyboardInput);
    }
    if (pSMFBlock->scissorsBox)
    {
        SAFE_FREE(pSMFBlock->scissorsBox);
    }
    if (pSMFBlock->coselectionSetName)
    {
        SAFE_FREE(pSMFBlock->coselectionSetName);
    }
    if (pSMFBlock->tabNavigationSetName)
    {
        SAFE_FREE(pSMFBlock->tabNavigationSetName);
    }
    if (pSMFBlock->rawString)
    {
        estrDestroy(&pSMFBlock->rawString);
    }
    if (pSMFBlock->outputString)
    {
        estrDestroy(&pSMFBlock->outputString);
    }
    if (pSMFBlock->pBlock)
    {
        sm_DestroyBlock(pSMFBlock->pBlock);
    }
}

MP_DEFINE(SMFBlock);
SMFBlock* smfBlock_Create()
{
    SMFBlock *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(SMFBlock, 64);
    res = MP_ALLOC( SMFBlock );
    if( verify( res ))
    {
        res->pBlock = sm_CreateBlock();
        estrCreate(&res->rawString);
        estrCreate(&res->outputString);
        res->lineBreakMode = SMFLineBreakMode_None;
        res->inputMode = SMFInputMode_None;
        res->inputLimit = 0;
        res->coselectionSetName = strdup("");
        res->tabNavigationSetName = strdup("");
        res->scissorsBox = 0;
    }
    return res;
}

SMFBlock* smfBlock_CreateCopy( SMFBlock* hSrcItem )
{
    SMFBlock* retval = NULL;

    if (verify(hSrcItem))
    {
        retval = smfBlock_Create();

        if (verify(retval))
        {
            smfBlock_FreeData(retval);

            // This is a catch-all for the simple structure members (eg.
            // last width, input limit).
            memcpy(retval, hSrcItem, sizeof(SMFBlock));

            // Must manually clear these otherwise the EString code will
            // modify the source version when it tries to copy.
            retval->rawString = NULL;
            retval->outputString = NULL;

            // Make duplicates of everything which allocates memory.
            retval->pBlock = sm_CreateBlock();

            estrPrintEString(&retval->rawString, hSrcItem->rawString);
            estrCreate(&retval->outputString);

            retval->coselectionSetName = strdup(hSrcItem->coselectionSetName);
            retval->tabNavigationSetName = strdup(hSrcItem->tabNavigationSetName);
            retval->sound_successfulKeyboardInput = strdup(hSrcItem->sound_successfulKeyboardInput);
            retval->sound_failedKeyboardInput = strdup(hSrcItem->sound_failedKeyboardInput);

            if (retval->scissorsBox)
            {
                retval->scissorsBox = malloc(sizeof(CBox));
                memcpy(retval->scissorsBox, hSrcItem->scissorsBox, sizeof(CBox));
            }
        }
    }

    return retval;
}

void smfBlock_Destroy(SMFBlock *pSMFBlock)
{
    if(pSMFBlock)
    {
        if (smf_selectionEndPane == pSMFBlock || smf_selectionStartPane == pSMFBlock)
        {
            smf_ClearSelection();
        }

        smfBlock_FreeData(pSMFBlock);

        MP_FREE(SMFBlock, pSMFBlock);
    }
}