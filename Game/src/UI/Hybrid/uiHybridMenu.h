#ifndef UIHYBRIDMENU_H
#define UIHYBRIDMENU_H

#include "utilitieslib/stdtypes.h"

typedef struct AtlasTex AtlasTex;
typedef struct SMFBlock SMFBlock;
typedef struct ShoppingCart ShoppingCart;
typedef struct CBox CBox;
typedef enum HAlign HAlign;
typedef enum VAlign VAlign;

typedef struct HybridElement
{
    int index;
    const char * text;
    const char * desc1;  //tooltip for hybrid buttons

    const char * iconName0;
    const char * iconName1;
    const char * iconName2;
    const char * texName0;
    const char * texName1; 
    const char * texName2;
    const void * pData;

    F32 percent; // what percent selected are we
    F32 timer;

    AtlasTex * icon0, *icon1, *icon2; // icons on the bar
    AtlasTex * tex0, *tex1, *tex2;  // external textures determined by bar

    int storeLocked;
    int text_flags;            // set NO_MSPRINT to avoid translation (for character names)
}HybridElement;

typedef enum HybridNameType
{
    HYBRID_NAME_CREATION = 0,
    HYBRID_NAME_REGISTER,
    HYBRID_NAME_ID,
} HybridNameType;

typedef enum HybridMenuType
{
    HYBRID_MENU_CREATION = 0,
    HYBRID_MENU_TAILOR,
    HYBRID_MENU_NO_NAVIGATION,
    
} HybridMenuType;

typedef enum HybridMenuDirection
{
    DIR_LEFT = 0,
    DIR_RIGHT = 1,
    DIR_LEFT_NO_NAV = 2,
    DIR_RIGHT_NO_NAV = 3,
    DIR_LEFT_DONT_USE_ME = 4, // LEFTs need to be even numbered
    DIR_RIGHT_LOGIN = 5,
    DIR_COUNT,
} HybridMenuDirection;

#define CLR_H_HIGHLIGHT 0xfbcf31ff
#define CLR_H_BACK        0x046ecaff
#define CLR_H_WHITE        0xffffffff
#define CLR_H_GREY        0x444444ff
#define CLR_H_DARK_BACK 0x073d6cff
#define CLR_H_BRIGHT_BACK 0x93b1cbff
#define CLR_H_GLOW        0xffffffff  //artist will control this color
#define CLR_H_TAB_BACK    0x55353dff
#define CLR_H_DESC_TEXT    0xcfcfffff
#define CLR_H_DESC_TEXT_DS 0x00000099  //DS = drop shadow

extern const int HYBRID_FRAME_COLOR[3];
extern const int HYBRID_SELECTED_COLOR[3];

void resetHybridCommon();
void resetCreationMenus();

F32 stateScaleSpeed( F32 sc, F32 speed, int up );
F32 stateScale( F32 sc, int up );

int drawNavigationSet(); // tabs across the top of the screen
void drawCheckNameBar(HybridNameType nametype);  // bar across bottom of screen for checking name
void hybridCheckNameReceiveResult(int CheckNameSucess, int locked);  //result from name check
char * getNameBarName();

int drawHybridCommon(int menuType);

int isHybridLastMenu(void);
int drawHybridNext( int dir, int locked, char * text ); // next/prev button
void goHybridNext(int dir);  //automatically go to the next menu

void hybridElementInit(HybridElement *hb);

#define HB_FADE_ENDS        (1<<0)
#define HB_ROUND_ENDS        (1<<1)
#define HB_TAIL_RIGHT        (1<<2)
#define HB_TAIL_RIGHT_ROUND (1<<3)
#define HB_TAIL_LEFT_ROUND  (1<<4)
#define HB_UNSELECTABLE        (1<<5)
#define HB_FLASH            (1<<6)
#define HB_CENTERED            (1<<7)
#define HB_GREY                (1<<8)
#define HB_NO_HOVER_SCALING    (1<<9)
#define HB_FLAT                (1<<10)
#define HB_ROUND_ENDS_THIN    (1<<11)
#define HB_SMALL_FONT        (1<<12)
#define HB_HIGHLIGHT_FONT    (1<<13)
#define HB_SCALE_DOWN_FONT    (1<<14)
#define HB_ALWAYS_FULL_ALPHA (1<<15)

void hybrid_start_menu( int menu );

int drawHybridBar( HybridElement * hb, int selected, F32 x, F32 cy, F32 z, F32 wd, F32 scale, int flags, F32 alpha, HAlign ha, VAlign va );
int drawHybridBarLarge(HybridElement * hb, F32 cx, F32 cy, F32 z, F32 ht, int selected, int flags, F32 alpha );
int drawHybridTab(HybridElement * hb, F32 x, F32 y, F32 z, F32 xsc, F32 ysc, int selected, F32 alpha );
void drawHybridTabFrame(F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 fadePct);

#define HB_DESCFRAME_FULLFRAME (1<<0)
#define HB_DESCFRAME_FADEDOWN (1<<1)
#define HB_DESCFRAME_FADELEFT (1<<2)
#define HB_DESCFRAME_HOVERBACKGROUND (1<<3)

void drawHybridDesc( SMFBlock *sm, F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, const char * text, int reparse, int alpha );
void drawHybridDescFrame(F32 x, F32 y, F32 z, F32 wd, F32 ht, int flags, F32 fadePct, F32 alpha, F32 backgroundOpacity, HAlign ha, VAlign va);

// The type is just a UID to keep track of which one is being dragged
void drawHybridSliderEmpty( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, const char * text, int type);
F32 drawHybridSlider( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, F32 val, const char * text, int type, int number );
F32 drawHybridVerticalSlider( F32 cx, F32 cy, F32 z, F32 ht, F32 val);
void drawHybridTripleSlider( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, Vec3 val, const char * text, int type );

void drawGradientEdgedBox( F32 x, F32 y , F32 z, F32 wd, F32 ht, F32 depth, int color );

int drawHybridArrow( F32 cx, F32 cy, F32 z, F32 sc, int flip );
void drawHybridFade( F32 cx, F32 cy, F32 z, F32 wd, F32 ht, F32 mid, int color );

#define HB_DISABLED (1<<0)
#define HB_FLIP_H    (1<<1)
#define HB_LIGHT_UP    (1<<2)
#define HB_SHRINK_OVER    (1<<3)
#define HB_DRAW_BACKING (1<<4)
#define HB_DRAW_NORMAL_ICON (1<<5)
#define HB_DO_NOT_EDIT_ELEMENT (1<<6)  //for buttons that may repeat on a page, we don't want them reusing the same scale

void clearHybridToolTipIfUnused();
void resetHybridToolTipAwareness();
int drawHybridButtonWindow( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags, int window );
int drawHybridButton( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags );

#define HYBRID_BAR_BUTTON_DISABLED (1<<0)
#define HYBRID_BAR_BUTTON_NO_OUTLINE (1<<1)
#define HYBRID_BAR_DO_NOT_EDIT_ELEMENT (1<<2)  //for buttons that may repeat on a page, we don't want them reusing the same scale
#define HYBRID_BAR_BUTTON_ARROW_LEFT (1<<3)
#define HYBRID_BAR_BUTTON_ARROW_RIGHT (1<<4)

int drawHybridBarButton( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 wd, F32 sc, int buttonColor, int flags );

#define HB_STORE_PRODUCT (1<<0)
#define HB_STORE_CATEGORY (1<<1)

int drawHybridStoreSmallButton( char * product, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int window );
int drawHybridStoreLargeButton( char * product, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int window );
int drawHybridStoreMenuButton( const char * product, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags );
int drawHybridStoreWindowButton( const char * product, const char * storeTex, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int buttonFlags, const char * tooltip, int window );
int drawHybridStoreAddShoppingCartMenuButton( const ShoppingCart * cart, U32 first, U32 last, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags);
void drawHybridStoreLockIcon( F32 cx, F32 cy, F32 z, F32 sc, int color );
void drawHybridStoreLockIconCustomTooltip( F32 cx, F32 cy, F32 z, F32 sc, int color, const char * tooltip );
void drawHybridTextEntry( SMFBlock * pBlock, F32 cx, F32 cy, F32 z, F32 wd, F32 ht  );

extern const F32 HBPOPUPFRAME_TOP_OFFSET;
extern const F32 HBPOPUPFRAME_BOTTOM_OFFSET;
extern const F32 HBPOPUPFRAME_LEFT_OFFSET;
extern const F32 HBPOPUPFRAME_RIGHT_OFFSET;

void drawHybridPopupFrame(F32 x, F32 y, F32 z, F32 wd, F32 ht);
void drawHybridScrollBar(F32 x, F32 y, F32 z, F32 ht, F32 * yOffset, F32 maxOffset, CBox * mouseScrollArea);
void drawSectionArrow(F32 cx, F32 cy, F32 z, F32 sc, F32 alphaPct, F32 dir);

//tooltips used in hybrid menus
//(these are free'd when hybrid common is reset)
void setHybridTooltip(CBox * box, const char * text);
void setHybridTooltipWindow(CBox * box, const char * text, int window);

int getLastNavMenuType();

void hybridMenuFlash(int incomplete);
void drawLWCUI(F32 cx, F32 cy, F32 z, F32 sc);

#endif
