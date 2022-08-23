#ifndef SMF_UTIL_H__
#define SMF_UTIL_H__

#include <utilitieslib/stdtypes.h> // for bool
#include "graphics/truetype/ttFontDraw.h"
#include <utilitieslib/utils/Cbox.h>

/***************************************************************************/
/* SMF Memory Analysis Flags                                               */
/***************************************************************************/

extern int smf_analyzeMemory; // set to 1 to turn on SMF's surface-level memory analysis, 2 for deeper
extern bool smf_analyzeMemoryIsLeaking; // set a data breakpoint on this to check for true
extern int smf_drawDebugBoxes; // set to 1 to draw interact boxes for the SMFBlock areas, 2 to draw scissors boxes for the SMFBlock areas.
extern int smf_printDebugLines; // set to 1 to get some printf's that I have strewn throughout the SMF code.

#ifndef TAG_MATCHES
#define TAG_MATCHES(y) (smf_aTagDefs[pBlock->iType].id==k_##y)
#endif

#define SM_MAX_PARAMS 10

typedef struct TupleSN
{
    char *pchName;
    int iVal;
} TupleSN;

typedef struct TupleSS
{
    // Data type which maps a string to another string.
    char *pchName;
    char *pchVal;
} TupleSS;

typedef enum SMAlignment
{
    SMAlignment_None,
    SMAlignment_Left,
    SMAlignment_Right,
    SMAlignment_Center,
    SMAlignment_Top,
    SMAlignment_Bottom,
    SMAlignment_Both,
} SMAlignment;

typedef struct SMPosition
{
    float iX;
    float iY;
    float iWidth;
    float iHeight;

    SMAlignment alignHoriz;
    SMAlignment alignVert;

    float iMinWidth;
    float iMinHeight;

    int iBorder;

} SMPosition;

typedef struct SMBlock SMBlock; // forward decl since it's self-referential

/***************************************************************************/
/***************************************************************************/

typedef enum SMFLineBreakMode
{
    SMFLineBreakMode_None = 0,
    SMFLineBreakMode_SingleLine,
    SMFLineBreakMode_MultiLineBreakOnWhitespace,
    SMFLineBreakMode_MultiLineBreakOnWhitespaceWithInverseTabbing,
} SMFLineBreakMode;

extern const SMFLineBreakMode SMFLineBreakMode_DefaultValue;

typedef enum SMFInputMode
{
    SMFInputMode_None = 0,
    SMFInputMode_AnyTextNoTagsOrCodes,
    SMFInputMode_NonnegativeIntegers,
    SMFInputMode_Alphanumeric,
    SMFInputMode_PlayerNames,
    SMFInputMode_PlayerNamesAndHandles,
} SMFInputMode;

extern const SMFInputMode SMFInputMode_DefaultValue;
extern const int SMFInputLimit_DefaultValue;

typedef enum SMFEditMode
{
    SMFEditMode_None = 0,
    SMFEditMode_Unselectable,
    SMFEditMode_ReadOnly,
    SMFEditMode_ReadWrite
} SMFEditMode;

extern const SMFEditMode SMFEditMode_DefaultValue;

typedef enum SMFScrollMode
{
    SMFScrollMode_None = 0,
    SMFScrollMode_ExternalOnly,
    SMFScrollMode_InternalScrolling
} SMFScrollMode;

extern const SMFScrollMode SMFScrollMode_DefaultValue;

typedef enum SMFOutputMode
{
    SMFOutputMode_None = 0,
    SMFOutputMode_StripAllTagsAndCodes,
    SMFOutputMode_StripAllTags,
    SMFOutputMode_StripFlaggedTagsAndCodes,
    SMFOutputMode_StripFlaggedTags,
    SMFOutputMode_StripNoTags,
} SMFOutputMode;

extern const SMFOutputMode SMFOutputMode_DefaultValue;

typedef enum SMFDisplayMode
{
    SMFDisplayMode_None = 0,
    SMFDisplayMode_AllCharacters,
    SMFDisplayMode_AsterisksOnly,
    SMFDisplayMode_NumbersWithCommas,
} SMFDisplayMode;

extern const SMFDisplayMode SMFDisplayMode_DefaultValue;

typedef enum SMFContextMenuMode
{
    SMFContextMenuMode_None = 0,
    SMFContextMenuMode_Cut = 1,
    SMFContextMenuMode_Copy = 1 << 1,
    SMFContextMenuMode_Paste = 1 << 2,
    SMFContextMenuMode_Format = 1 << 3,
    SMFContextMenuMode_Color = 1 << 4,
    SMFContextMenuMode_TextSub = 1 << 5,
    SMFContextMenuMode_CutCopyPaste = SMFContextMenuMode_Cut | SMFContextMenuMode_Copy | SMFContextMenuMode_Paste,
    SMFContextMenuMode_CutCopyPasteFormat = SMFContextMenuMode_CutCopyPaste | SMFContextMenuMode_Format,
    SMFContextMenuMode_CutCopyPasteFormatColor = SMFContextMenuMode_CutCopyPaste | SMFContextMenuMode_Format | SMFContextMenuMode_Color,
    SMFContextMenuMode_CutCopyPasteFormatColorText = SMFContextMenuMode_CutCopyPasteFormatColor | SMFContextMenuMode_TextSub,
} SMFContextMenuMode;

extern const SMFContextMenuMode SMFContextMenuMode_DefaultValue;

typedef struct SMFBlock
{
    int iLastYBase;
    int iLastWidth;
    int iLastHeight;
    SMAlignment defaultHorizAlign;
    U32 ulCrc;
    SMBlock *pBlock;
    char *rawString;
    char *outputString;
    unsigned int displayStringLength;
    int dont_reparse_ever_again;
    int lastFontScale;
    float masterScale;
    SMFEditMode editMode;
    SMFLineBreakMode lineBreakMode;
    SMFInputMode inputMode;
    SMFScrollMode scrollMode;
    SMFOutputMode outputMode;
    SMFDisplayMode displayMode;
    SMFContextMenuMode contextMenuMode;
    int inputLimit;
    char *coselectionSetName;
    char *tabNavigationSetName;
    int tabNavigationSetIndex;
    int currentDisplayOffsetX;
    int currentDisplayOffsetY;
    bool interactedThisFrame;
    bool clickedOnThisFrame;
    bool rightClickedOnThisFrame;
    bool draggedOnThisFrame;
    CBox *scissorsBox;
    int internalScrollDiff;
    bool editedThisFrame;
    char *sound_successfulKeyboardInput;
    char *sound_failedKeyboardInput;
    bool displayedThisFrameSinceLastTimeIWasASelectionEndpoint;
} SMFBlock;

typedef struct SMBlock
{
    // Definition of an atomic item in the text stream. Can also contain
    // blocks hierarchically.

    SMPosition pos;
    // Location and dimension info for the block, inherited by children.

    int iType;
    // An arbitrary type given by the user of the smparser so they can
    // tell what the following void * is.

    bool bFreeOnDestroy;
    // true if pv should be freed on destroy.

    void *pv;
    // holds whatever the caller gives us

    bool bHasBlocks;
    // If this block contains others, this is true

    SMBlock **ppBlocks;
    // Managed array holding the blocks this block contains.

    SMBlock *pParent;
    // Points back to the owning block. Can be used for tree traversal.

    SMFBlock *pSMFBlock;

    int bgcolor;

    bool bHover;

    bool bSelected;

    unsigned int rawStringStartIndex;

    unsigned int displayStringStartIndex;

} SMBlock;

/***************************************************************************/
/***************************************************************************/

typedef SMBlock *(*SMTagProc)(SMBlock *pBlock, int iTag, TupleSS[SM_MAX_PARAMS]);
// The callback function for tags

typedef SMBlock *(*SMTextProc)(SMBlock *pBlock, const unsigned char *pchText, int iLen, int preparseStartIndex, int *postparseStartIndex, bool createIfEmpty);
// The callback function for plain text. The given string is NOT
// terminated by \0. Make sure you use iLen to determine where the string ends.
// It's unsigned char because this is a UTF-8 byte string.

typedef SMBlock *(*SMInitProc)(SMBlock *pBlock);
// The callback function for creating the base container block.

typedef struct SMTagDef
{
    char *pchName;
    // tag name

    int id;
    // The identity (aka tag-type) of this def. Just make it the same
    //   as the pfn function name as a string with the SMF_TAG macro

    SMTagProc  pfn;
    // function to execute for this tag

    TupleSS aParams[SM_MAX_PARAMS];
    // Parameter name and default defs. The first one will be filled in
    // if no attrib name is given.

    int displayLength;

    bool outputToString;

} SMTagDef;

extern SMTagDef smf_aTagDefs[];

typedef enum SMFTags
{
    k_sm_none = 0,
    k_sm_ws,
    k_sm_bsp,
    k_sm_br,
    k_sm_image,
    k_sm_table,
    k_sm_table_end,
    k_sm_tr,
    k_sm_tr_end,
    k_sm_td,
    k_sm_td_end,
    k_sm_span,
    k_sm_span_end,
    k_sm_p,
    k_sm_p_end,
    k_sm_nolink,
    k_sm_nolink_end,
    k_sm_text,
    k_sm_anchor,

    k_sm_b,
    k_sm_i,
    k_sm_color,
    k_sm_color2,
    k_sm_colorhover,
    k_sm_colorselect,
    k_sm_colorselectbg,
    k_sm_scale,
    k_sm_face,
    k_sm_font,
    k_sm_outline,
    k_sm_shadow,
    k_sm_toggle_end,
    k_sm_toggle_link_end,
} SMFTags;

#define SMF_TAG(x) k_##x, x
#define SMF_TAG_NONE(x) 0, x


// SMTagDef aTagDefs[] =
// {
//    { "tag",     SMF_TAG(DoTag),   {{ "param1", "default" }, { "param2", "moo" }, { "param3", "foo" }, { "param4", "bar" }}  },
//    { "tag2",    SMF_TAG(DoTag2),  {{ "param1", "default" }, { "param2", "moo" }, { "param3", "foo" }, { "param4", "bar" }} },
//    { 0 }, // required sentinel for end of list
// }

/***************************************************************************/
/***************************************************************************/

SMBlock *sm_CreateBlock(void);
// Creates a new block.

void sm_DestroyBlock(SMBlock *pBlock);
// Rescursively destroys the container block. After this function, all
// storage will be released, including storage within other blocks.
// Assumes that all void points which are not null are shallow and can
// be released with free().

SMBlock *sm_AppendBlock(SMBlock *pBlockParent, SMBlock *pBlock);
// Appends the given block to the parent's list of blocks.
// Returns the new block.

SMBlock *sm_AppendNewBlock(SMBlock *pBlock);
// Creates a new block and adds it to the given parent block. This
// block cannot contain child blocks. Returns the new block.

SMBlock *sm_AppendNewBlockAndData(SMBlock *pBlock, int iSize);
// Creates a new block and adds it to the given parent block. This
// block cannot contain child blocks. Returns the new block. A buffer
// of the given size is allocated from the heap, cleared, and put in
// the block's data pointer.

SMBlock *sm_CreateContainer(void);
// Creates a new block which holds other blocks.

SMBlock *sm_AppendNewContainer(SMBlock *pBlockParent);
// Creates a new block which holds other blocks, adds it to the given
// parent block. Returns the new block.

SMBlock *sm_AppendNewContainerAndData(SMBlock *pBlockParent, int iSize);
// Creates a new block which holds other blocks, adds it to the given
// parent block. Returns the new block. A buffer of the given size is
// allocated from the heap, cleared, and put in the block's data pointer.

void sm_BlockDump(SMBlock *pBlock, int iLevel, SMTagDef aTagDefs[]);
// A debugging function used to dump the given block at the given
// indenting level. It assumes that the type of the block is equal
// to the index in aTagDef.

TupleSN *FindTupleSN(TupleSN *pTuples, char *pch);
TupleSS *FindTupleSS(TupleSS *pTuples, char *pch);

char *sm_GetVal(char *pchAttrib, TupleSS *pTuples);
// Returns the value for a particular attrib in a parameter list.

int sm_GetAlignment(char *pchAttrib, TupleSS *pTuples);
// Returns the alignment value for a particular attrib in a param list.

U32 sm_GetColor(char *pchAttrib, TupleSS *pTuples);
// Returns a 32 bit hex color for a particular attrib. Supports a bunch
// of named colors as well as HTML-ish #xxxxxx values.

U32 smf_ConvertColorNameToValue(char *pchColorName);
// Directly looks up a given color name in the list, returning the RGB value.

typedef enum SMFSearchCommand
{
    SMFSearchCommand_None,
    SMFSearchCommand_Done, // Set smf_selectionSearch to this to signal that you've determined the correct search value.
    SMFSearchCommand_WordAboveSelectedEnd,
    SMFSearchCommand_WordBelowSelectedEnd,
    SMFSearchCommand_EndOfLineBeforeSelectedEndLine,
    SMFSearchCommand_StartOfSelectedEndLine,
    SMFSearchCommand_EndOfSelectedEndLine,
    SMFSearchCommand_StartOfLineAfterSelectedEndLine,
    SMFSearchCommand_NextTabbedBlock,
    SMFSearchCommand_PreviousTabbedBlock,
    SMFSearchCommand_PreviousBeginningOfWord,
    SMFSearchCommand_NextBeginningOfWord,
    SMFSearchCommand_BlockNearestCursor,
    SMFSearchCommand_ClearSelection,
} SMFSearchCommand;

typedef enum SMFEditCommand
{
    SMFEditCommand_None,
    SMFEditCommand_Done,
    SMFEditCommand_AddCharAtCursor,
    SMFEditCommand_AddLineBreakAtCursor,
    SMFEditCommand_RemoveCharBeforeCursor,
    SMFEditCommand_RemoveCharAfterCursor,
    SMFEditCommand_PasteStringAtCursor,
} SMFEditCommand;

typedef enum SMFSelectionCommand
{
    SMFSelectionCommand_None,
    SMFSelectionCommand_Delete,
    SMFSelectionCommand_InsertTags,
} SMFSelectionCommand;

//********************************************************************
// PLEASE DON'T DIRECTLY USE THESE VARIABLES OUTSIDE OF SMF FUNCTIONS!
extern bool smfCursor_display;
extern float smfCursor_currentTimer;
extern float smfCursor_interval;

extern SMFBlock *smf_selectionStartPane;
extern unsigned int smf_selectionStartDisplayIndex;
extern int smf_selectionStartYBase;

extern SMFBlock *smf_selectionEndPane;
extern unsigned int smf_selectionEndDisplayIndex;
extern int smf_selectionEndXBase;
extern int smf_selectionEndYBase;
extern int smf_selectionEndCharacterXOffset;
extern int smf_selectionEndCharacterYOffset;
extern int smf_selectionEndCharacterYHeight;

extern int smf_selectionCurrentYBase;
extern int smf_selectionCurrentIY;
extern bool smf_isCurrentlyDragging;

extern SMFBlock *smf_selectionSearchPane;
extern unsigned int smf_selectionSearchDisplayIndex;
extern int smf_selectionSearchXBase;
extern int smf_selectionSearchYBase;
extern int smf_selectionSearchCharacterXOffset;
extern int smf_selectionSearchCharacterYOffset;
extern int smf_selectionSearchCharacterYHeight;

extern SMFSearchCommand smfSearch_currentCommand;
extern SMFSearchCommand smfSearch_lastCommand;
extern bool smfSearch_nextTabbedBlock_justSawEndPane;
extern bool smfSearch_keepStartConstant;

extern SMFEditCommand smfEdit_currentCommand;
extern char *smfEdit_currentCharactersToInsert;

extern bool smf_stopSettingTheSelectionThisFrame;

extern bool smf_forceReformatAll;
extern bool smf_displayedSomethingThisFrame;
extern bool smf_gainedFocusThisFrame;

// PLEASE DON'T DIRECTLY USE THESE VARIABLES OUTSIDE OF SMF FUNCTIONS!
//********************************************************************

void smf_ClearSelection(void);
void smf_ClearSelectionCommand(void);
SMFSelectionCommand smf_GetSelectionCommand(void);
bool smf_SetSelectionCommand(SMFSelectionCommand newCmd);
bool smf_SetSelectionTags(char *preTag, char *postTag);
char *smf_GetSelectionPreTag(void);
char *smf_GetSelectionPostTag(void);

// Maximum number of columns in a table.
#define SMF_MAX_COLS 10

#define SMF_MAX_IMG_NAME 256
#define SMF_FONT_SCALE (100.0f)

// This variable is a bit of a hack to not display hover backgrounds on text/ws blocks
// when a tr has displayed its hover background.
extern bool smf_hasDisplayedHoverBackgroundThisFrame;

extern bool s_bAllowAnchor;

typedef struct SMAnchor
{
    bool bHover;
    bool bSelected;
#pragma warning(push, 1) // For C++ compilation
    char ach[];
#pragma warning(pop)
} SMAnchor;

typedef struct TextAttribs
{
    int *piBold;
    int *piItalic;
    int *piColor;
    int *piColor2;
    int *piColorHover; //disabled.. fix two lines in smf_Render to re-enable.
    int *piColorSelect;
    int *piColorSelectBG;
    int *piScale;
    int *piFace;
    int *piFont; // I think this value isn't actually used for anything... apparently it's supposed to be a TTDrawContext cast to an int?
    int *piAnchor;
    int *piLink;
    int *piLinkBG;
    int *piLinkHover;
    int *piLinkHoverBG;
    int *piLinkSelect;
    int *piLinkSelectBG;
    int *piOutline;
    int *piShadow;
} TextAttribs;

typedef enum FormatTags
{
    kFormatTags_Bold,
    kFormatTags_Italic,
    kFormatTags_Color,
    kFormatTags_Color2,
    kFormatTags_ColorHover,
    kFormatTags_ColorSelect,
    kFormatTags_ColorSelectBG,
    kFormatTags_Scale,
    kFormatTags_Face,
    kFormatTags_Font,
    kFormatTags_Anchor,
    kFormatTags_Link,
    kFormatTags_LinkBG,
    kFormatTags_LinkHover,
    kFormatTags_LinkHoverBG,
    kFormatTags_LinkSelect,
    kFormatTags_LinkSelectBG,
    kFormatTags_Outline,
    kFormatTags_Shadow,
    kFormatTags_Count,
} FormatTags;

typedef struct SMImage
{
    char achTex[SMF_MAX_IMG_NAME];
    char achTexHover[SMF_MAX_IMG_NAME];
    int iWidth;
    int iHeight;
    int iHighlight;
    int iColor;
} SMImage;

typedef struct SMColumn
{
    int iWidthRequested;
    int iHighlight;
    int iSelected;
} SMColumn;

typedef struct SMRow
{
    int iHighlight;
    int iSelected;
    int iStyle;
} SMRow;

typedef struct SMFont
{
    int bColor   : 1;
    int bColor2     : 1;
    int bFace    : 1;
    int bScale   : 1;
    int bOutline : 1;
    int bShadow  : 1;
    int bItalic  : 1;
} SMFont;


void smf_PushAttrib(SMBlock *pBlock, TextAttribs *pattrs);
int smf_PopAttrib(SMBlock *pBlock, TextAttribs *pattr);
int smf_PopAttribInt(int idx, TextAttribs *pattr);
bool smf_ApplyFormatting(SMBlock *pBlock, TextAttribs *pattrs);
void smf_MakeFont(TTDrawContext **ppttFont, TTFontRenderParams *prp, TextAttribs *pattrs);
float* smf_GetCharacterWidths(SMBlock *pBlock, TextAttribs *pattrs);
bool smf_IsSelectionStartBeforeSelectionEnd();

bool smf_IsBlockSelectable(SMFBlock *pSMFBlock);
bool smf_IsBlockEditable(SMFBlock *pSMFBlock);
bool smf_IsBlockMultiline(SMFBlock *pSMFBlock);
bool smf_IsBlockScrollable(SMFBlock *pSMFBlock);

unsigned int smBlock_ConvertRelativeDisplayIndexToRelativeRawIndex(SMBlock *pBlock, unsigned int relativeDisplayIndex);
unsigned int smBlock_ConvertAbsoluteDisplayIndexToAbsoluteRawIndex(SMBlock *pBlock, unsigned int absoluteDisplayIndex);
unsigned int smBlock_GetDisplayLength(SMBlock *pBlock);
unsigned int smBlock_GetRawLength(SMBlock *pBlock);
unsigned int smfBlock_GetDisplayLength(SMFBlock *pSMFBlock);

float smBlock_CalcFontScale(SMBlock *pBlock, TextAttribs *pattrs);

bool smfBlock_HasFocus(SMFBlock *pSMFBlock);
bool smBlock_HasFocus(SMBlock *pBlock);
bool smf_IsSearchSMBlock(SMBlock *pBlock); // like HasFocus2 but for search not end

int smf_OnLostFocus(SMFBlock *pSMFBlock, void *requester);
void smf_ReturnFocus(); // Don't call this outside of SMF!  Bad things happen!
bool smf_TakeFocus(SMFBlock *pSMFBlock); // Don't call this outside of SMF!  Bad things happen!

void smf_SetCursor(SMFBlock *pSMFBlock, int index);
int smf_GetCursorLineInternalYPosition();
SMFBlock *smf_GetCursorLine();
int smf_GetCursorIndex();
int smf_GetCursorXPosition();

void smf_SelectAllText(SMFBlock *pSMFBlock);

void smf_DetermineCurrentOffsets(SMFBlock *pSMFBlock, int x, int y, int z, int width, int height);
int smf_DetermineLetterXOffset(SMBlock* pBlock, TextAttribs *pattrs, unsigned int letterIndex);

SMFBlock* smfBlock_Create( void );
SMFBlock* smfBlock_CreateCopy( SMFBlock* hSrcItem );
void smfBlock_Destroy( SMFBlock *hItem );

#endif